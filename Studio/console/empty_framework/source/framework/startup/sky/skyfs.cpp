/*****************************************************************************
 *
 * File :     SkyFS.cpp
 *
 * Abstract : PS2 custom file system
 *
 *****************************************************************************
 *
 * This file is a product of Criterion Software Ltd.
 *
 * This file is provided as is with no warranties of any kind and is
 * provided without any obligation on Criterion Software Ltd. or
 * Canon Inc. to assist in its use or modification.
 *
 * Criterion Software Ltd. will not, under any
 * circumstances, be liable for any lost revenue or other damages arising
 * from the use of this file.
 *
 * Copyright (c) 2000 Criterion Software Ltd.
 * All Rights Reserved.
 *
 * RenderWare is a trademark of Canon Inc.
 *
 *****************************************************************************/
#include "precomp.h"
#include "skyfs.h"
#include "skyiop.h"

#include "stdio.h"
#include "string.h"
#include "assert.h"

#include "sifdev.h"

#define READBUFFERSIZE 10240

#define RWGDFSGLOBAL(var)                               \
    (RWPLUGINOFFSET(gdfsGlobals,                        \
                    RwEngineInstance,                   \
                    SkyFSState.gdfsModuleInfo.globalsOffset)->var)

#define rwID_SKYDEVICEMODULE (MAKECHUNKID(rwVENDORID_CRITERIONINT, 0x30))

namespace
{
   typedef union RwPtrVoid RwPtrVoid;
   union RwPtrVoid
   {
       void       *ptrVoid;
       const void *constptrVoid;
   };


   typedef struct skyFile skyFile;
   struct skyFile
   {
       int                 gdfs;
       RwInt32             POS;
       RwInt32             SOF;
       RwUInt8             readBuffer[READBUFFERSIZE];
       RwUInt32            bufferPos;
       RwBool              bufferValid;
   };

   typedef struct gdfsGlobals gdfsGlobals;
   struct gdfsGlobals
   {
       RwFileFunctions     oldFS;  /* Old Filing system vectors */
       RwFreeList         *fpFreeList; /* File pointer free list */
   };

   /* These are all global to all RenderWare instances */

   typedef struct RwSkyFSState RwSkyFSState;
   struct RwSkyFSState
   {
       RwInt32             gdfsOpenFiles;
       RwChar              SubDir[256]; /* a directory name */
       RwModuleInfo        gdfsModuleInfo;
   };

   static RwSkyFSState SkyFSState;

   
   /****************************************************************************
    skyTransMode

    Attempt to convert a mode string to an open mode

    On entry  : access mode
    On exit   : integer mode
    */
   static int
   skyTransMode(const RwChar * access)
   {
       int                 mode;
       char               *r;
       char               *w;
       char               *a;
       char               *plus;
       char               *n;
       char               *d;

       /* I add a couple of new characters for now:
        * n non-blocking mode
        * d no write back d cache */

       r = strrchr(access, (int) 'r');
       w = strrchr(access, (int) 'w');
       a = strrchr(access, (int) 'a');
       plus = strrchr(access, (int) '+');
       n = strrchr(access, (int) 'n');
       d = strrchr(access, (int) 'd');

       if (plus)
           mode = SCE_RDWR;
       else if (r)
           mode = SCE_RDONLY;
       else if (w)
           mode = SCE_WRONLY;
       else if (a)
           mode = SCE_WRONLY;
       else
           return (0);

       /* later we will test for SCE_CREAT & !SCE_TRUNC as a seek to end of file */
       if (w)
           mode |= SCE_CREAT | SCE_TRUNC;

       if (a)
           mode |= SCE_CREAT;

       if (n)
           mode |= SCE_NOWAIT;

       if (d)
           mode |= SCE_NOWBDC;

       return (mode);
   }

   /****************************************************************************
    trySkyFopen

    One attempt at opening a file

    On entry   : Filename, access mode
    On exit    :
    */

   static void        *
   trySkyFopen(const RwChar * fname, const RwChar * access)
   {
       skyFile            *fp;
       int                 mode;
       RwChar              name[256];
       RwChar             *nameptr;

       /* First manipulate the filename into a Sky friendly name */
       if (strchr(fname, ':'))
       {
           strncpy(name, fname, 255);
       }
       else
       {
#ifdef MENUING
          sprintf(name, "%s%s\\%s", RWS::GetRootFileServerPath(), SkyFSState.SubDir, fname);
#else
          sprintf(name, "%s\\%s", RWS::GetRootFileServerPath(), fname);
#endif /* MENUING */

#if (defined(CDROM) || defined(DVDROM))

          /* append a ;1 to the end if required */
          name[253] = 0;
          /* we assume that any ';' indicates a version appended */
          if (!strrchr(name, ';'))
          {
             strcat(name, ";1");
          }
          strupr(&name[7]);

#endif #if (defined(CDROM) || defined(DVDROM))

       }
       /* force null termination */
       name[255] = 0;

       nameptr = name;
       while (*nameptr)
       {
#if (defined(CDROM) || defined(DVDROM))
           if (*nameptr == '/')
               *nameptr = '\\';
#else /* #if (defined(CDROM) || defined(DVDROM)) */
           if (*nameptr == '\\')
               *nameptr = '/';
#endif /* #if (defined(CDROM) || defined(DVDROM)) */
           nameptr++;
       }

       /* Allocate structure for holding info */
       fp = (skyFile *) RwFreeListAlloc(RWGDFSGLOBAL(fpFreeList), rwMEMHINTDUR_GLOBAL);
       if (!fp)
       {
           return (NULL);
       }

       mode = skyTransMode(access);
       if (!mode)
       {
           return (NULL);
       }

       fp->gdfs = sceOpen(name, mode);

       if (fp->gdfs < 0)
       {
           RwFreeListFree(RWGDFSGLOBAL(fpFreeList), fp);
           return (NULL);
       }

       /* We seek to the end of the file to get size */
       fp->SOF = fp->POS = sceLseek(fp->gdfs, 0, SCE_SEEK_END);
       if (fp->SOF < 0)
       {
           sceClose(fp->gdfs);
           RwFreeListFree(RWGDFSGLOBAL(fpFreeList), fp);
           return (NULL);
       }
       /* SCE_CREAT & !SCE_TRUNC mean seek to end of file */
       if (!((mode & SCE_CREAT) && !(mode & SCE_TRUNC)))
       {
           fp->POS = sceLseek(fp->gdfs, 0, SCE_SEEK_SET);

           if (fp->POS < 0)
           {
               sceClose(fp->gdfs);
               RwFreeListFree(RWGDFSGLOBAL(fpFreeList), fp);
               return (NULL);
           }
       }

       /* Initialise the buffer to show nothing buffered */
       fp->bufferPos = READBUFFERSIZE;

       SkyFSState.gdfsOpenFiles++;

       return ((void *) fp);
   }

   /****************************************************************************
    skyFopen

    On entry   : Filename, access mode
    On exit    :
    */

   static void        *
   skyFopen(const RwChar * name, const RwChar * access)
   {
       void               *res;

       res = trySkyFopen(name, access);

       if (res)
       {
           return (res);
       }

       return (NULL);
   }

   /****************************************************************************
    skyFclose

    On entry   : FILE * (data block specific to this implementation)
    On exit    : 0 on success
    */

   static int
   skyFclose(void *fptr)
   {
       skyFile            *fp = (skyFile *) fptr;

       assert(fptr);

       if (fp && SkyFSState.gdfsOpenFiles)
       {
           sceClose(fp->gdfs);

           RwFreeListFree(RWGDFSGLOBAL(fpFreeList), fp);

           --SkyFSState.gdfsOpenFiles;

           return (0);
       }

       return (-1);
   }

   /****************************************************************************
    skyFexist

    On entry   : Filename
    On exit    :
    */

   static              RwBool
   skyFexist(const RwChar * name)
   {
       void               *res;

       res = RwOsGetFileInterface()->rwfopen(name, "r");
       if (res)
       {
           RwOsGetFileInterface()->rwfclose(res);
           return (TRUE);
       }

       return (FALSE);
   }

   /****************************************************************************
    skyFread

    On entry   : Address to read to, block size, block count, file to read from
    On exit    : Number of bytes read
    */

   static              size_t
   skyFread(void *addr, size_t size, size_t count, void *fptr)
   {
       skyFile            *fp = (skyFile *) fptr;
       size_t              numBytesToRead = size * count;
       int                 bytesRead, bytesRead2;

       bytesRead = 0;

       /* Trim number of bytes for the size of the file */
       if ((fp->POS + (RwInt32) numBytesToRead) > fp->SOF)
       {
           numBytesToRead = fp->SOF - fp->POS;
       }

       /* First try and use the buffer */
       if ((fp->bufferPos < READBUFFERSIZE) &&
           (bytesRead < (RwInt32) numBytesToRead))
       {
           /* Pull from the buffer */
           if (numBytesToRead < (READBUFFERSIZE - fp->bufferPos))
           {
               /* Can satisfy entirely from buffer */
               bytesRead = numBytesToRead;
           }
           else
           {
               /* Pull as much as possible from the buffer */
               bytesRead = READBUFFERSIZE - fp->bufferPos;
           }

           /* Copy it */
           memcpy(addr, &fp->readBuffer[fp->bufferPos], bytesRead);

           /* Update target address and source address */
           addr = (void *) ((RwUInt8 *) addr + bytesRead);
           fp->bufferPos += bytesRead;
           fp->POS += bytesRead;
       }

       /* If next bit is bigger than a buffer, read it directly and ignore the
        * buffer.
        */
       if ((numBytesToRead - bytesRead) > 0)
       {
           if ((numBytesToRead - bytesRead) >= READBUFFERSIZE)
           {
               bytesRead2 = (numBytesToRead - bytesRead);
               bytesRead2 = sceRead(fp->gdfs, addr, bytesRead2);
               if (bytesRead2 < 0)
               {
                   bytesRead2 = 0;
               }
           }
           else
           {
               /* Go via the buffer */
               sceRead(fp->gdfs, fp->readBuffer, READBUFFERSIZE);
               bytesRead2 = (numBytesToRead - bytesRead);
               memcpy(addr, fp->readBuffer, bytesRead2);
               fp->bufferPos = bytesRead2;
           }
           fp->POS += bytesRead2;
           bytesRead += bytesRead2;
       }

       return (bytesRead / size);
   }

   /****************************************************************************
    skyFwrite

    On entry   : Address to write from, block size, block count, file to write to
    On exit    : Number of bytes written
    */

   static              size_t
   skyFwrite(const void *addr, size_t size, size_t count, void *fptr)
   {
       int                 bytesWritten;
       skyFile            *fp = (skyFile *) fptr;
       RwInt32             numBytesToWrite = size * count;
       RwPtrVoid           AddrUnion;

       assert(addr);
       assert(fptr);

       AddrUnion.constptrVoid = addr;

       bytesWritten = sceWrite(fp->gdfs, AddrUnion.ptrVoid, numBytesToWrite);

       if (-1 != bytesWritten)
       {
           fp->POS += bytesWritten;
           if (fp->POS > fp->SOF)
               fp->SOF = fp->POS;
           return (size > 0 ? bytesWritten / size : 0);
       }
       return (0);
   }

   /****************************************************************************
    skyFseek

    On entry   : file to seek in, offset, how to seek
    On exit    : 0 on success
    */

   static int
   skyFseek(void *fptr, long offset, int origin)
   {
       skyFile            *fp = (skyFile *) fptr;
       RwInt32             oldFPos, bufStart;
       RwBool              noBuffer = FALSE;

       assert(fptr);

       oldFPos = fp->POS;
       bufStart = oldFPos - fp->bufferPos;
       if (fp->bufferPos == READBUFFERSIZE)
           noBuffer = TRUE;
       fp->bufferPos = READBUFFERSIZE;

       switch (origin)
       {
           case SEEK_CUR:
               {
                   /* Does the seek stay in the buffer */
                   if (((oldFPos + offset >= bufStart) &&
                        (oldFPos + offset < bufStart + READBUFFERSIZE)) &&
                       (!noBuffer))
                   {
                       fp->bufferPos = (oldFPos + offset) - bufStart;
                       fp->POS += offset;
                   }
                   else
                   {
                       fp->POS =
                           sceLseek(fp->gdfs, oldFPos + offset,
                                    SCE_SEEK_SET);
                   }
                   break;
               }
           case SEEK_END:
               {
                   fp->POS = sceLseek(fp->gdfs, offset, SCE_SEEK_END);
                   break;
               }
           case SEEK_SET:
               {
                   fp->POS = sceLseek(fp->gdfs, offset, SCE_SEEK_SET);
                   break;
               }
           default:
               {
                   return (-1);
               }
       }

       if (noBuffer)
           fp->bufferPos = READBUFFERSIZE;

       if (fp->POS == -1)
       {
           /* This may not be valid */
           fp->POS = oldFPos;
           fp->bufferPos = READBUFFERSIZE;
           return (-1);
       }

       return (0);
   }

   /****************************************************************************
    skyFtell

    On entry   : file to tell offset in
    On exit    : current offset in file
    */

   static int
   skyFtell(void *fptr)
   {
       skyFile            *fp = (skyFile *) fptr;

       assert(fptr);

       return sceLseek(fp->gdfs, 0, SCE_SEEK_CUR);
   }

   /****************************************************************************
    skyFgets

    On entry   : Buffer to read into, max chars to read, file to read from
    On exit    : Returns pointer to string on success
    */

   static RwChar      *
   skyFgets(RwChar * buffer, int maxLen, void *fptr)
   {
       skyFile            *fp = (skyFile *) fptr;
       RwInt32             i;
       RwInt32             numBytesRead;

       assert(buffer);
       assert(fptr);

       i = 0;

       numBytesRead = skyFread(buffer, 1, maxLen - 1, fp);

       if (numBytesRead == 0)
       {
           return (0);
       }

       while (i < numBytesRead)
       {
           if (buffer[i] == '\n')
           {
               i++;

               buffer[i] = '\0';

               /* the file pointer needs */
               /* to be reset as skyFread */
               /* will have overshot the */
               /* first new line         */

               i -= numBytesRead;
               skyFseek(fp, i, SEEK_CUR);

               return (buffer);
           }
           else if (buffer[i] == 0x0D)
           {
               if ((i < (numBytesRead - 1)) && (buffer[i + 1] == '\n'))
               {
                   memcpy(&buffer[i], &buffer[i + 1],
                          (numBytesRead - i - 1));
                   numBytesRead--;
               }
               else
                   i++;
           }
           else
               i++;
       }

       /*
        * Don't return NULL because we could have read maxLen bytes
        * without finding a \n
        */
       return (buffer);
   }

   /****************************************************************************
    skyFputs

    On entry   : Buffer to write from, file to write to
    On exit    : Non negative value on success
    */

   static int
   skyFputs(const RwChar * buffer, void *fptr)
   {
       skyFile            *fp = (skyFile *) fptr;
       int                 i, j;
       RwPtrVoid           BufferUnion;
       assert(buffer);
       assert(fptr);

       BufferUnion.constptrVoid = buffer;

       i = strlen(buffer);
       j = sceWrite(fp->gdfs,  BufferUnion.ptrVoid, i);

       if (j != -1)
       {
           fp->POS += j;
           if (fp->POS > fp->SOF)
               fp->SOF = fp->POS;
       }
       if ((j == -1) || (i != j))
       {
           return (EOF);
       }
       return (j);
   }

   /****************************************************************************
    skyFeof

    On entry   : File to test for end of
    On exit    : Non zero if end of file reached
    */

   static int
   skyFeof(void *fptr)
   {
       skyFile            *fp = (skyFile *) fptr;

       assert(fptr);

       return (fp->POS >= fp->SOF);
   }

   /****************************************************************************
    skyFflush

    On entry   :
    On exit    :
    */

   static int
   skyFflush(void *fptr __RWUNUSED__)
   {
       return (0);
   }

   /****************************************************************************
    _rwSkyFSOpen

    On entry   : Instance, offset, size
    On exit    : instance pointer on success
    */

   static void        *
   _rwSkyFSOpen(void *instance, RwInt32 offset, RwInt32 size __RWUNUSED__)
   {
       RwFileFunctions    *FS;

       /* Cache the globals offset */
       SkyFSState.gdfsModuleInfo.globalsOffset = offset;

       /* Create a free list for file handle structures */
       RWGDFSGLOBAL(fpFreeList) = RwFreeListCreate(sizeof(skyFile), 5, 0, rwMEMHINTDUR_GLOBAL);
       if (!RWGDFSGLOBAL(fpFreeList))
       {
           return (NULL);
       }

       /* This is per instance of RenderWare */
       FS = RwOsGetFileInterface();

       /* save away the old filing system */
       RWGDFSGLOBAL(oldFS) = *FS;

       /* attach the new filing system */
       FS->rwfexist = skyFexist;  /* FS->rwfexist;  */
       FS->rwfopen = skyFopen;
       FS->rwfclose = skyFclose;
       FS->rwfread = skyFread;
       FS->rwfwrite = skyFwrite;
       FS->rwfgets = skyFgets;
       FS->rwfputs = skyFputs;
       FS->rwfeof = skyFeof;
       FS->rwfseek = skyFseek;
       FS->rwfflush = skyFflush;
       FS->rwftell = skyFtell;

       SkyFSState.gdfsModuleInfo.numInstances++;

       return (instance);
   }

   /****************************************************************************
    _rwSkyFSClose

    On entry   : instance, offset, size
    On exit    : instance pointer on success
    */

   static void        *
   _rwSkyFSClose(void *instance, RwInt32 offset __RWUNUSED__,
                 RwInt32 size __RWUNUSED__)
   {
       RwFileFunctions    *FS;

       FS = RwOsGetFileInterface();

       /* re-attach the old filing system - not strictly necessary,
        * but we are feeling kind today! */
       *FS = RWGDFSGLOBAL(oldFS);

       /* Blow away our free list */
       RwFreeListDestroy(RWGDFSGLOBAL(fpFreeList));

       SkyFSState.gdfsModuleInfo.numInstances--;

       return (instance);
   }
}

namespace RWS
{
   namespace Sky
   {
      /****************************************************************************
       InstallFileSystem

       On entry   :
       On exit    : TRUE on success
       */

      bool InstallFileSystem(RwChar * subdir)
      {
         bool result = false;

         SkyFSState.gdfsOpenFiles = 0;

         if (RwEngineRegisterPlugin
           (sizeof(gdfsGlobals), rwID_SKYDEVICEMODULE, _rwSkyFSOpen,
            _rwSkyFSClose) >= 0)
         {
            result = true;
            
            // If we've got a subdir then store it so that we can prepend it onto filenames
            if(subdir)
            {
               strcpy(SkyFSState.SubDir, subdir);
            }
            else
            {
               SkyFSState.SubDir[0] = '\0';
            }
         }

         return result;
      }
   }
}
