/***************************************************************************
 *                                                                         *
 * Module  : dlfs.c                                                        *
 *                                                                         *
 * Purpose : Dolphin file system                                           *
 *                                                                         *
 **************************************************************************/

/****************************************************************************
 Includes
 */

#include <assert.h>
#include <string.h>

#include "dolphin.h"
#include "dolphin/os.h"
#include "dolphin/dvd.h"

#include "rwcore.h"
#include "dlfs.h"

/****************************************************************************
 Global prototypes
 */

/****************************************************************************
 Local Types
 */

/****************************************************************************
 Local (Static) Prototypes
 */

/****************************************************************************
 Local Defines
 */

#if !defined(EMU)
/* Use this to get FIO over EXI filesystem support */
/* The use of FIO is a bit noddy and does far too many seeks */
#define FIOFILESYSTEM
#endif /* !defined(EMU) */

#ifdef FIOFILESYSTEM
#include <dolphin/fio.h>

/* These are defined to match the initial set in viewer_server */
#define DL_FIO_EXICHAN (MCC_EXI_0)
#define DL_FIO_MCCCHAN (MCC_CHANNEL_1)
#define DL_FIO_BLKSIZE ((u8)10)
#endif /* FIOFILESYSTEM */

/* Flags for fake ANSI file API */
#ifndef SEEK_SET
#define SEEK_SET        0               /* Seek absolute */
#define SEEK_CUR        1               /* Seek relative current */
#define SEEK_END        2               /* Seek relative end */
#endif  /* SEEK_SET */

/****************************************************************************
 Globals (across program)
 */

/****************************************************************************
 Local (static) Globals
 */

#ifdef FIOFILESYSTEM
/* We do lazy init of the FIO system so that a program that doesn't refer
   to any FIO based files won't lock trying to start possibly nonexistant
   hardware */
static RwBool FIOStarted = FALSE;
#endif /* FIOFILESYSTEM */

/****************************************************************************
 Functions
 */

#define READBUFFERSIZE 10240

#define RWGDFSGLOBAL(var) (RWPLUGINOFFSET(FSGlobals, RwEngineInstance, FSModuleInfo.globalsOffset)->var)

typedef enum FTypeTag
{
    ftNAFTYPE = 0,
    ftDVD,
#ifdef FIOFILESYSTEM
    ftFIO,
#endif /* FIOFILESYSTEM */
    ftFORCEINT = RWFORCEENUMSIZEINT
} FType;

typedef struct
{
    RwUInt8        readBuffer[READBUFFERSIZE]; /* Make sure this is 32Byte aligned and a multiple of 32 */
    union {
        DVDFileInfo    fileInfo;
#ifdef FIOFILESYSTEM
        FIOHandle      fioHandle;
#endif /* FIOFILESYSTEM */
    } fileHandle;
    RwUInt32       POS;
    RwUInt32       SOF;
    
    RwUInt32       bufferPos;

    FType          fType;
}
DLFileHandle;

typedef struct
{
    RwFileFunctions     oldFileFuncs;       /* Old Filing system vectors */
    RwFreeList          *freeList;          /* File pointer free list */
}
FSGlobals;

/* These are all global to all RenderWare instances */

static RwInt32          FSOpenFiles = 0;

static RwModuleInfo     FSModuleInfo;

static RwInt32 rwID_DOLPHINDEVICEMODULE = MAKECHUNKID(rwVENDORID_CRITERIONINT, 0x30);

#ifdef FIOFILESYSTEM
/* If defined, use the FIO open modes */
#define DL_OPEN_RDONLY (FIO_OPEN_RDONLY)
#define DL_OPEN_WRONLY (FIO_OPEN_WRONLY)
#define DL_OPEN_RDWR   (FIO_OPEN_RDWR)
#define DL_OPEN_CREAT  (FIO_OPEN_CREAT)
#define DL_OPEN_TRUNC  (FIO_OPEN_TRUNC)
#else /* FIOFILESYSTEM */
/* Just use a suitable set of flag values */
#define DL_OPEN_RDONLY (1)
#define DL_OPEN_WRONLY (2)
#define DL_OPEN_RDWR   (3)
#define DL_OPEN_CREAT  (4)
#define DL_OPEN_TRUNC  (8)
#endif /* FIOFILESYSTEM */


#ifdef FIOFILESYSTEM
static RwBool
dlStartFIO(void)
{
    while (!FIOInit(DL_FIO_EXICHAN, DL_FIO_MCCCHAN, DL_FIO_BLKSIZE))
    {
        ;
    }

    while (!FIOQuery())
    {
        ;
    }
    FIOStarted = TRUE;
    return(TRUE);
}
#endif /* FIOFILESYSTEM */

/****************************************************************************
 dlAccessToMode

 On entry   : access mode
 On exit    : open mode flags

 0 is a fail.
 */

static RwUInt32
dlAccessToMode(const RwChar *access)
{
    RwUInt32 mode = 0;

    /* This is a bit crude, but it isn't mainline code */
    if (!access)
    {
        return(mode);
    }

    /* This code relies on evaluation order to prevent it running off the
       end of the string */
    /* I assume that there no difference between 'b' and non'b' files */
    if ((access[0] == 'r')
        && ((access[1] == '\0')
            || ((access[1] == 'b') && (access[2] == '\0'))))
    {
        mode = DL_OPEN_RDONLY;
    }
    else
    if ((access[0] == 'w')
        && ((access[1] == '\0')
            || ((access[1] == 'b') && (access[2] == '\0'))))
    {
        mode = DL_OPEN_WRONLY | DL_OPEN_CREAT | DL_OPEN_TRUNC;
    }
    else
    if ((access[0] == 'a')
        && ((access[1] == '\0')
            || ((access[1] == 'b') && (access[2] == '\0'))))
    {
        mode = DL_OPEN_WRONLY | DL_OPEN_CREAT;
    }
    else
    if ((access[0] == 'r')
        && (access[1] == '+')
        && ((access[2] == '\0')
            || ((access[2] == 'b') && (access[3] == '\0'))))
    {
        mode = DL_OPEN_RDWR;
    }
    else
    if ((access[0] == 'w')
        && (access[1] == '+')
        && ((access[2] == '\0')
            || ((access[2] == 'b') && (access[3] == '\0'))))
    {
        mode = DL_OPEN_RDWR | DL_OPEN_CREAT | DL_OPEN_TRUNC;
    }
    else
    if ((access[0] == 'a')
        && (access[1] == '+')
        && ((access[2] == '\0')
            || ((access[2] == 'b') && (access[3] == '\0'))))
    {
        mode = DL_OPEN_RDWR | DL_OPEN_CREAT;
    }

    return(mode);
}

/****************************************************************************
 dlFopen

 On entry   : Filename, access mode
 On exit    :
 */

static void *
dlFopen(const RwChar *name, const RwChar *access)
{
    DLFileHandle    *fp;
    RwUInt32        mode;

    mode = dlAccessToMode(access);

    if (!mode)
    {
        return (NULL);
    }
    /* Allocate structure for holding info */
    fp = (DLFileHandle *)RwFreeListAlloc(RWGDFSGLOBAL(freeList), rwMEMHINTDUR_GLOBAL);
    if (!fp)
    {
        return (NULL);
    }

    fp->fType = ftNAFTYPE;

#ifdef FIOFILESYSTEM
    /* For now, we select DVD filesystem based on request for RDONLY */
    if (mode != DL_OPEN_RDONLY)
    {
        fp->fType = ftFIO;
    }
    else
#else /* FIOFILESYSTEM */
    if (mode == DL_OPEN_RDONLY)
#endif /* FIOFILESYSTEM */
    {
        fp->fType = ftDVD;
    }

    if (fp->fType == ftNAFTYPE)
    {
        /* Failed to choose a suitable filesystem */
        RwFreeListFree(RWGDFSGLOBAL(freeList), fp);
        return (NULL);
    }

    /* Move to the specified directory */
    {
    }

#ifdef FIOFILESYSTEM
    if (fp->fType == ftFIO)
    {
        FIOStat stat;

        if (!FIOStarted)
        {
            if (!dlStartFIO())
            {
                RwFreeListFree(RWGDFSGLOBAL(freeList), fp);
                return (NULL);
            }
        }

        /* It looks as though we can't OR in FIO_OPEN_CREAT in the way we
           would on most platforms, so we do the following rather odd thing */
        if ((mode & DL_OPEN_CREAT)
            && (FIO_INVALID_HANDLE
                == (fp->fileHandle.fioHandle = FIOFopen(name, mode))))
        {
            fp->fileHandle.fioHandle = FIOFopen(name, FIO_OPEN_CREAT);
            if (FIO_INVALID_HANDLE == fp->fileHandle.fioHandle)
            {
                RwFreeListFree(RWGDFSGLOBAL(freeList), fp);
                return (NULL);
            }
        }
        FIOFclose(fp->fileHandle.fioHandle);

        /* We have choosen to map our modes to the FIO ones. Handy */
        if (FIO_INVALID_HANDLE
            == (fp->fileHandle.fioHandle = FIOFopen(name, mode)))
        {
            RwFreeListFree(RWGDFSGLOBAL(freeList), fp);
            return (NULL);
        }

        /* For some reason, we are only supporting 32bit files sizes */
        if (!(FIOFstat(fp->fileHandle.fioHandle, &stat))
            || (stat.fileSizeHigh != 0))
        {
            FIOFclose(fp->fileHandle.fioHandle);
            RwFreeListFree(RWGDFSGLOBAL(freeList), fp);
            return (NULL);
        }

        fp->SOF = stat.fileSizeLow;

        /* We may have to seek to the end of the file */
        if ((mode & DL_OPEN_CREAT) && !(mode & DL_OPEN_TRUNC))
        {
            long res;

            res = FIOFseek(fp->fileHandle.fioHandle, 0l, FIO_SEEK_LAST);

            if (-1 == res)
            {
                FIOFclose(fp->fileHandle.fioHandle);
                RwFreeListFree(RWGDFSGLOBAL(freeList), fp);
                return (NULL);
            }
            fp->POS = fp->SOF;
        }
        else
        {
            fp->POS = 0;
        }
    }
    else
#endif /* FIOFILESYSTEM */
    {
        /* Open the file for reading */
        if (!DVDOpen((RwChar *)name, &fp->fileHandle.fileInfo))
        {
            RwFreeListFree(RWGDFSGLOBAL(freeList), fp);
            return (NULL);
        }

        /* Get the file size */
        fp->SOF = DVDGetLength(&fp->fileHandle.fileInfo);
        fp->POS = 0;
    }


    /* Initialise the buffer to show nothing buffered */
    fp->bufferPos = READBUFFERSIZE;

    FSOpenFiles++;

    return ((void *)fp);
}

/****************************************************************************
 dlFclose

 On entry   : FILE * (data block specific to this implementation)
 On exit    : 0 on success
 */

static int 
dlFclose(void *fptr)
{
    DLFileHandle *fp = (DLFileHandle *)fptr;

    if (fp && FSOpenFiles)
    {
#ifdef FIOFILESYSTEM
        if (fp->fType == ftFIO)
        {
            if (FIOFclose(fp->fileHandle.fioHandle))
            {
                RwFreeListFree(RWGDFSGLOBAL(freeList), fp);
    
                FSOpenFiles--;

                return (0);
            }
        }
        else
#endif /* FIOFILESYSTEM */
        {
            if (DVDClose(&fp->fileHandle.fileInfo))
            {
                RwFreeListFree(RWGDFSGLOBAL(freeList), fp);
    
                FSOpenFiles--;

                return (0);
            }
        }
    }

    return (-1);
}

/****************************************************************************
 dlFexist

 On entry   : Filename
 On exit    :
 */

static RwBool
dlFexist(const RwChar *name)
{
    void *res;

#ifdef FIOFILESYSTEM
    /* Not obvious what to do here. For now we only check the DVD filesystem */
#endif /* FIOFILESYSTEM */

    res = RwOsGetFileInterface()->rwfopen(name, "r");
    if (res)
    {
        RwOsGetFileInterface()->rwfclose(res);
        return (TRUE);
    }

    return (FALSE);
}

/****************************************************************************
 dlFread

 On entry   : Address to read to, block size, block count, file to read from
 On exit    : Number of bytes read
 */

static size_t 
dlFread(void *addr, size_t size, size_t count, void *fptr)
{
    DLFileHandle    *fp = (DLFileHandle *)fptr;
    size_t          numBytesToRead = size * count;
    int             bytesRead, bytesRead2;

    bytesRead = 0;

    /* Trim number of bytes for the size of the file */
    if ((fp->POS + (RwInt32)numBytesToRead) > fp->SOF)
    {
        numBytesToRead = fp->SOF - fp->POS;
    }

    /* First try and use the buffer */
    if ((fp->bufferPos < READBUFFERSIZE) &&
        (bytesRead < (RwInt32)numBytesToRead))
    {
        /* Pull from the buffer */
        if (numBytesToRead < (READBUFFERSIZE-fp->bufferPos))
        {
            /* Can satisfy entirely from buffer */
            bytesRead = numBytesToRead;
        }
        else
        {
            /* Pull as much as possible from the buffer */
            bytesRead = READBUFFERSIZE-fp->bufferPos;
        }

        /* Copy it */
        memcpy(addr, &fp->readBuffer[fp->bufferPos], bytesRead);

        /* Update target address and source address */
        addr = (void *)((RwUInt8 *)addr + bytesRead);
        fp->bufferPos += bytesRead;
        fp->POS += bytesRead;
        assert(fp->POS <= fp->SOF);
    }

    /* If next bit is bigger than a buffer, read it directly and ignore the
     * buffer.
     */
    if ((numBytesToRead-bytesRead) > 0)
    {
        if ((numBytesToRead-bytesRead) >= READBUFFERSIZE)
        {
            /* If we're 32Byte aligned and a multiple of 32 do a direct read */
            if (((((RwUInt32)addr) & (32-1)) == 0) && !((numBytesToRead-bytesRead) & 0x1f))
            {
                bytesRead2 = (numBytesToRead-bytesRead);
#ifdef FIOFILESYSTEM
                if (fp->fType == ftFIO)
                {
                    if (FIOFseek(fp->fileHandle.fioHandle, fp->POS, 
                                 FIO_SEEK_TOP) != -1)
                    {
                        bytesRead2 = (int)FIOFread(fp->fileHandle.fioHandle,
                                                   addr, bytesRead2);
                    }
                    else
                    {
                        bytesRead2 = -1;
                    }
                }
                else
#endif /* FIOFILESYSTEM */
                {
                    bytesRead2 = DVDRead(&fp->fileHandle.fileInfo, addr, bytesRead2, fp->POS);
                }
                if (bytesRead2 < 0)
                {
                    bytesRead2 = 0;
                }
            }
            else
            {
                RwInt32 n;
                RwInt32 byteLeftToRead;
                RwInt32 offset;

                bytesRead2 = 0;
                byteLeftToRead = numBytesToRead-bytesRead;
                offset = fp->POS;
                n = (byteLeftToRead / READBUFFERSIZE) + 1;

                /* Go via the buffer */
                while (n--)
                {
                    RwInt32 readSize;

                    readSize = fp->SOF - (fp->POS + bytesRead2);
                    readSize = (readSize > READBUFFERSIZE) ? READBUFFERSIZE : readSize;

                    /* Read a buffers worth of data*/
#ifdef FIOFILESYSTEM
                    if (fp->fType == ftFIO)
                    {
                        if (FIOFseek(fp->fileHandle.fioHandle, offset, 
                                     FIO_SEEK_TOP) != -1)
                        {
                            u32 res;

                            res = FIOFread(fp->fileHandle.fioHandle,
                                           fp->readBuffer,
                                           ((readSize + 31) & ~0x1F));
                            if (res == 0xffffffff)
                            {
                                return (bytesRead + bytesRead2);
                            }
                        }
                        else
                        {
                            return (bytesRead + bytesRead2);
                        }
                    }
                    else
#endif /* FIOFILESYSTEM */
                    {
                        if (DVDRead(&fp->fileHandle.fileInfo, fp->readBuffer,
                            ((readSize + 31) & ~0x1F), offset) == -1)
                        {
                            return (bytesRead + bytesRead2);
                        }
                    }

                    /* Copy what we need */
                    if (byteLeftToRead >= readSize)
                    {
                        memcpy(addr, fp->readBuffer, readSize);
                        byteLeftToRead -= readSize;
                        bytesRead2 += readSize;
                        offset += readSize;

                        /* Update target address and source address */
                        addr = (void *)((RwUInt8 *)addr + readSize);
                    }
                    else
                    {
                        memcpy(addr, fp->readBuffer, byteLeftToRead);
                        bytesRead2 += byteLeftToRead;
                        offset += byteLeftToRead;

                        fp->bufferPos = byteLeftToRead;
                    }
                }
            }
        }
        else
        {
            RwInt32 bytes;
            RwInt32 read;

            bytes = OSRoundUp32B((fp->SOF - fp->POS));
            if (bytes > READBUFFERSIZE)
            {
                bytes = READBUFFERSIZE;
            }

            /* Go via the buffer */
#ifdef FIOFILESYSTEM
            if (fp->fType == ftFIO)
            {
                if (FIOFseek(fp->fileHandle.fioHandle, fp->POS, FIO_SEEK_TOP)
                    != -1)
                {
                    read = (int)FIOFread(fp->fileHandle.fioHandle,
                                            fp->readBuffer, bytes);
                }
                else
                {
                    read = -1;
                }
            }
            else
#endif /* FIOFILESYSTEM */
            {
                read = DVDRead(&fp->fileHandle.fileInfo, fp->readBuffer, bytes,
                               fp->POS);
            }
            if (read == -1)
            {
                return (bytesRead);
            }

            bytesRead2 = (numBytesToRead-bytesRead);
            memcpy(addr, fp->readBuffer, bytesRead2);
            fp->bufferPos = bytesRead2;
        }

        fp->POS += bytesRead2;
        assert(fp->POS <= fp->SOF);
        bytesRead += bytesRead2;
    }

    return (bytesRead/size);
}

/****************************************************************************
 dlFwrite

 On entry   : Address to write from, block size, block count, file to write to
 On exit    : Number of bytes written
 */

static size_t 
dlFwrite(const void *addr, size_t size, size_t count, void *fptr)
{
#ifdef FIOFILESYSTEM
    DLFileHandle    *fp = (DLFileHandle *)fptr;

    if (fp->fType == ftFIO)
    {
        RwUInt32 len;
        RwUInt32 len2;

        len = size * count;
        len2 = 0;
        if ((len != size*count)
            || (len == 0xffffffff))
        {
            /* We have to fail the size as it is the error condition */
            return(0);
        }

        /* We now flush any buffered read data, as this is invalid */
        fp->bufferPos = READBUFFERSIZE;

        /* We can't write if the buffer is not aligned on 32b, so */
        if ((RwUInt32)addr & 0x1f)
        {
            RwUInt8 buf[64];
            RwUInt8* bufStart;
            RwUInt32 i;

            bufStart = (RwUInt8*)(((RwUInt32)buf + 31) & ~0x1f);
            len2 = 32 - ((RwUInt32)addr & 0x1f);
            if (len2 > len)
            {
                len2 = len;
            }
            len -= len2;
            for (i=0; i<len2; i++)
            {
                bufStart[i] = *(((RwUInt8*)addr)++);
            }
            len2 = FIOFwrite(fp->fileHandle.fioHandle, (void *)bufStart, len2);
            if (len2 != 0xffffffff)
            {
                /* Overflow danger here */
                fp->POS += len2;
            }
            else
            {
                /* Should really seek/stat the file here, etc */
                return(len2);
            }
        }

        if (len)
        {
            len = FIOFwrite(fp->fileHandle.fioHandle, (void *)addr, len);
            if (len != 0xffffffff)
            {
                /* Overflow danger here */
                fp->POS += len;
            }
            else
            {
                /* Should really seek/stat the file here, etc */
            }
        }

        if (fp->SOF < fp->POS)
        {
            fp->SOF = fp->POS;
        }
        return(len + len2);
    }
    else
#endif /* FIOFILESYSTEM */
    {
        return (0);
    }
}

/****************************************************************************
 dlFseek

 On entry   : file to seek in, offset, how to seek
 On exit    : 0 on success
 */

static int 
dlFseek(void *fptr, long offset, int origin)
{
    DLFileHandle    *fp = (DLFileHandle *)fptr;
    RwInt32         sector;
    RwInt32         bytes;
    RwInt32         oldFPos;
    RwInt32         tempOffset;
    RwInt32      sectorPos;
    RwInt32         bufferStart;

    oldFPos = fp->POS;

    switch (origin)
    {
        case SEEK_CUR:
        {
            tempOffset = offset;
            fp->POS += offset;
            assert(fp->POS <= fp->SOF);
            /* See if we can do this common one quickly first (if we are still in the cached sectors)... */
            /* Note: two tests in one by using unsigned test:
             *       (newIndex >= 0) && (newIndex < (SECTORS_BUFFERED * SECTOR_SIZE))
             */
            sectorPos = (RwUInt32)((RwInt32)(fp->bufferPos) + offset);
            if (sectorPos >= 0)
            {
                if (sectorPos <= (RwUInt32)(READBUFFERSIZE))
                {
                    if ((fp->POS >= 0) && (fp->POS <= fp->SOF))
                    {
                        /* We're still in the buffer, cool */
                        fp->bufferPos += offset;

                        return (0);
                    }
                }
            }
            break;
        }
        case SEEK_END:
        {
            tempOffset = (fp->SOF + offset) - fp->POS;
            fp->POS     = (fp->SOF + offset);
            assert(fp->POS <= fp->SOF);

            sectorPos = ((RwInt32) fp->bufferPos) + tempOffset;

            if (sectorPos >= 0)
            {
                if (sectorPos <= (RwUInt32)(READBUFFERSIZE))
                {
                    if ((fp->POS >= 0) && (fp->POS <= fp->SOF))
                    {
                        /* We're still in the buffer, cool */
                        fp->bufferPos += tempOffset;

                        return (0);
                    }
                }
            }
            break;
        }
        case SEEK_SET:
        {
            tempOffset = offset - fp->POS;
            fp->POS     = offset;
            assert(fp->POS <= fp->SOF);

            sectorPos = ((RwInt32) fp->bufferPos) + tempOffset;

            if (sectorPos >= 0)
            {
                if (sectorPos <= (RwUInt32)(READBUFFERSIZE))
                {
                    if ((fp->POS >= 0) && (fp->POS <= fp->SOF))
                    {
                        /* We're still in the buffer, cool */
                        fp->bufferPos += tempOffset;

                        return (0);
                    }
                }
            }
            break;
        }
        default:
        {
            return (-1);
        }
    }

    if (fp->POS < 0)
    {
        /* Can't seek off the front of the file */
        fp->POS = oldFPos;
        assert(fp->POS <= fp->SOF);
        return (-1);
    }

    if (fp->POS > fp->SOF)
    {
        /* Can't seek off the end of the file */
        fp->POS = oldFPos;
        assert(fp->POS <= fp->SOF);
        return (-1);
    }

    /* Find which sector we need to be in */
    sector = fp->POS / READBUFFERSIZE;
    bufferStart = sector * READBUFFERSIZE;

    if ((fp->SOF - bufferStart) > READBUFFERSIZE)
    {
        bytes = READBUFFERSIZE; 
    }
    else
    {
        bytes = OSRoundUp32B((fp->SOF - bufferStart));
    }

    /* Load in the sector */
#ifdef FIOFILESYSTEM
    if (fp->fType == ftFIO)
    {
        if (FIOFseek(fp->fileHandle.fioHandle,
                     sector * READBUFFERSIZE, FIO_SEEK_TOP) == -1)
        {
            /* may not be quite right */
            fp->POS = oldFPos;
            assert(fp->POS <= fp->SOF);
            return (-1);
        }
        if (FIOFread(fp->fileHandle.fioHandle, fp->readBuffer, bytes)
            == 0xffffffff)
        {
            fp->POS = oldFPos;
            assert(fp->POS <= fp->SOF);
            return (-1);
        }
    }
    else
#endif /* FIOFILESYSTEM */
    {
        if (DVDRead(&fp->fileHandle.fileInfo, fp->readBuffer, bytes,
                    sector * READBUFFERSIZE) == -1)
        {
            fp->POS = oldFPos;
            assert(fp->POS <= fp->SOF);
            return (-1);
        }
    }

    /* Set the position in the buffered sector */
    fp->bufferPos = fp->POS - bufferStart;

    return (0);
}

/****************************************************************************
 dlFgets

 On entry   : Buffer to read into, max chars to read, file to read from
 On exit    : Returns pointer to string on success
 */

static RwChar *
dlFgets(RwChar *buffer, int maxLen, void *fptr)
{
    DLFileHandle    *fp = (DLFileHandle *) fptr;
    RwInt32         i;
    RwInt32         numBytesRead;

    i = 0;

    numBytesRead = dlFread(buffer, 1, maxLen - 1, fp);

    if (numBytesRead == 0)
    {
        return (NULL);
    }

    while (i < numBytesRead)
    {
        if (buffer[i] == '\n')
        {
            i++;

            buffer[i] = '\0';

            /* the file pointer needs */
            /* to be reset as dlFread */
            /* will have overshot the */
            /* first new line         */

            i -= numBytesRead;
            dlFseek(fp, i, SEEK_CUR);

            return (buffer);
        }
        else if ( buffer[i] == 0x0D )
        {
            if ((i < (numBytesRead - 1)) && (buffer[i + 1] == '\n'))
            {
                memcpy(&buffer[i], &buffer[i + 1], (numBytesRead - i - 1));
                numBytesRead--;
            }
            else
            {
                i++;
            }
        }
        else
        {
            i++;
        }
    }

    if (numBytesRead < maxLen)
    {
        DLFileHandle    *fp = (DLFileHandle *)fptr;

        /* Trim number of bytes for the size of the file */
        if (fp->POS == fp->SOF)
        {
            buffer[numBytesRead] = '\0';
        }
    }

    /*
     * Don't return NULL because we could have read maxLen bytes
     * without finding a \n
     */
    return (buffer);
}

/****************************************************************************
 dlFputs

 On entry   : Buffer to write from, file to write to
 On exit    : Non negative value on success

 Note that the trailing \0 is not written.
 */

static int 
dlFputs(const RwChar *buffer, void *fptr)
{
#ifdef FIOFILESYSTEM
    DLFileHandle    *fp = (DLFileHandle *)fptr;
    
    if (fp->fType == ftFIO)
    {
        u32 len = 0;
        const RwChar *ptr = buffer;

        while (*ptr != '\0')
        {
            len++;
        }
        
        if (!len)
        {
            return(0);
        }

        return(dlFwrite(buffer, 1, len, fptr));
    }
#endif /* FIOFILESYSTEM */
    return (-1);
}

/****************************************************************************
 dlFeof

 On entry   : File to test for end of
 On exit    : Non zero if end of file reached
 */

static int 
dlFeof(void *fptr)
{
    DLFileHandle    *fp = (DLFileHandle *)fptr;

    return (fp->POS >= fp->SOF);
}

/****************************************************************************
 dlFflush

 On entry   :
 On exit    :
 */

static int 
dlFflush(void *fptr __RWUNUSED__)
{
#ifdef FIOFILESYSTEM
    DLFileHandle    *fp = (DLFileHandle *)fptr;
    
    if (fp->fType == ftFIO)
    {
        return(!FIOFflush(fp->fileHandle.fioHandle));
    }
#endif /* FIOFILESYSTEM */
    return (0);
}

/****************************************************************************
 dlFtell

 On entry   :
 On exit    :
 */

static int 
dlFtell(void *fptr)
{
    DLFileHandle    *fp = (DLFileHandle *)fptr;

    return (fp->POS);
}

/****************************************************************************
 _rwDolphinFSOpen

 On entry   : Instance, offset, size
 On exit    : instance pointer on success
 */

static void * 
_rwDolphinFSOpen(void *instance, RwInt32 offset, RwInt32 size __RWUNUSED__)
{
    RwFileFunctions  *fileFuncs;

    /* Cache the globals offset */
    FSModuleInfo.globalsOffset = offset;

    /* Create a free list for file handle structures. Making sure they are
     * 32Byte aligned as the buffer is the first entry and this must be
     * aligned to a 32Byte boundary.
     */
    RWGDFSGLOBAL(freeList) = RwFreeListCreate(sizeof(DLFileHandle), 5, 32, rwMEMHINTDUR_GLOBAL);
    if (!RWGDFSGLOBAL(freeList))
    {
        return (NULL);
    }

    /* This is per instance of RenderWare */
    fileFuncs = RwOsGetFileInterface();

    /* save away the old filing system */
    RWGDFSGLOBAL(oldFileFuncs) = *fileFuncs;

    /* attach the new filing system */
    fileFuncs->rwfexist   = dlFexist; /* fileFuncs->rwfexist;  */
    fileFuncs->rwfopen    = dlFopen;
    fileFuncs->rwfclose   = dlFclose;
    fileFuncs->rwfread    = dlFread;
    fileFuncs->rwfwrite   = dlFwrite;
    fileFuncs->rwfgets    = dlFgets;
    fileFuncs->rwfputs    = dlFputs;
    fileFuncs->rwfeof     = dlFeof;
    fileFuncs->rwfseek    = dlFseek;
    fileFuncs->rwfflush   = dlFflush;
    fileFuncs->rwftell    = dlFtell;

#ifdef FIOFILESYSTEM
    FIOStarted = FALSE;
#endif /* FIOFILESYSTEM */

    FSModuleInfo.numInstances++;

    return (instance);
}

/****************************************************************************
 _rwDolphinFSClose

 On entry   : instance, offset, size
 On exit    : instance pointer on success
 */

static void * 
_rwDolphinFSClose(void *instance,
                  RwInt32 offset __RWUNUSED__,
                  RwInt32 size __RWUNUSED__)
{
    RwFileFunctions  *fileFuncs;

    fileFuncs = RwOsGetFileInterface();

    /*
     * re-attach the old filing system - not strictly necessary,
     * but we are feeling kind today!
     */
    *fileFuncs = RWGDFSGLOBAL(oldFileFuncs);

    /*
     * Blow away our free list
     */
    RwFreeListDestroy(RWGDFSGLOBAL(freeList));

    FSModuleInfo.numInstances--;

#ifdef FIOFILESYSTEM
    if (FIOStarted)
    {
        FIOExit();
        FIOStarted = FALSE;
    }
#endif /* FIOFILESYSTEM */

    return (instance);
}

/****************************************************************************
 DolphinInstallFileSystem

 On entry   :
 On exit    : TRUE on success
 */

RwBool
DolphinInstallFileSystem(void)
{
    /*
     * Initializes the DVD device driver
     */
    DVDInit();

    /* Set the root directory */
    /*DVDSetRoot("DOLPHIN/build/demos/dvddemo/diskroot");*/

    if (RwEngineRegisterPlugin(sizeof(FSGlobals),
                               rwID_DOLPHINDEVICEMODULE, 
                               _rwDolphinFSOpen,
                               _rwDolphinFSClose) < 0)
    {
        /* If it is negative, we've failed */
        return (FALSE);
    }

    /* Hurrah */
    return (TRUE);
}
