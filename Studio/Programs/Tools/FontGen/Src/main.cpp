/*****************************************************************************
*
* File :     main.cpp
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
* Copyright © 1993 - 2003 Criterion Software Ltd. All rights reserved.
* All Rights Reserved.
*
* RenderWare is a trademark of Canon Inc.
*
*****************************************************************************/

#include <iostream>
#include <string>
#include <cassert>
#include <windows.h>
#include <atlbase.h>
#include <fstream>

////////////////////////////////////////////////////////////////////////////////
// Renderware includes
#include "rwcore.h"
#include "rpworld.h"
#include "rtpng.h"
#include "rtbmp.h"
#include "rt2d.h"
#include "rtfsyst.h"

//////////////////////////////////////////////////////////////////////////////
// Renderware libs
#pragma comment(lib, "rwcore.lib")
#pragma comment(lib, "rtfsyst.lib")
#pragma comment(lib, "rpworld.lib")
#pragma comment(lib, "rt2d.lib")
#pragma comment(lib, "rtfsyst.lib")

using namespace std;

#ifndef _DEBUG
#define ASSERT(a) if (!a) {cout << "Fatal Error " << #a << endl;}
#define FATALERROR(a) cout << "Fatal Error " << a << endl
#else
#define ASSERT(a) assert(a)
#define FATALERROR(a) assert(!a)
#endif

#include "font.h"

namespace
{
   // Globals
   RwCamera *g_pCamera = 0;

   /////////////////////////////////////////////////////////////////////////////
   //
   RwBool AttachPlugins ()
   {
      struct Plugin
      {
         RwBool (__cdecl *m_pFunc)();
      };

      Plugin Plugins[] = 
      {
         RpWorldPluginAttach
      };

      // Attach the plugins
      RwBool bRetVal = TRUE;
      for (RwUInt32 i = 0; i < sizeof (Plugins) / sizeof (Plugin); i++)
         if (!Plugins[i].m_pFunc ())
            bRetVal = FALSE;
      
      return bRetVal;
   }

   /////////////////////////////////////////////////////////////////////////////
   // Simple debug message handler for RenderWare debug messages
#ifdef _DEBUG
   void DebugMessageHandler(RwDebugType type, const RwChar *pMessage)
   {
      OutputDebugString (pMessage);
      OutputDebugString ("\n");
   }
#endif // _DEBUG

   const RwUInt32 UNC_PATH_SIZE = 256;   /* should be sufficient */
   const RwUInt32 MAX_NB_FILES_PER_FS = 20;

   /*
   *  
   *  platform specific method to install the file system
   *
   */
   RwBool psInstallFileSystem(void)
   {
       RwChar      curDirBuffer[_MAX_PATH];
       RwUInt32    retValue;
       RtFileSystem *wfs;

       // Get the current directory.
       retValue = GetCurrentDirectory(_MAX_PATH, curDirBuffer);
       if (0 == retValue || retValue > _MAX_PATH)
       {
           return FALSE;
       }

       // This is a unc path.
       if (curDirBuffer[1] != ':')
       {
           // tag a \ to the end of the current directory 
           // only fails if the buffer size is exhausted 
           rwstrcat( curDirBuffer, "\\" );

           // Setup the file system manager
           if ((wfs = RtWinFSystemInit(MAX_NB_FILES_PER_FS, curDirBuffer, "unc")) != 0)
           {
               // Now actually register the file system
               if (RtFSManagerRegister(wfs) == FALSE)
               {
                   return (FALSE);
               }
               else
               {
                   RtFSManagerSetDefaultFileSystem(wfs);
               }
           }
           else
           {
               return (FALSE);
           }
       }
       else
       {
           RwUInt32 drivesMask;
           RwInt32 drive;
           RwChar  fsName[2];

           CharUpper(curDirBuffer);

            
           // loop through all logical drives and register a file system for each
           // valid one.
           // Start at 2: this removes a:                      
           drivesMask = GetLogicalDrives();

           for( drive = 2, drivesMask >>= 1; drivesMask != 0; drive++, drivesMask >>= 1)
           {
               if (drivesMask & 0x01)
               {
                   RwInt32 ret;
                   RwChar  deviceName[4];

                   deviceName[0] = drive + 'A' - 1;
                   deviceName[1] = ':';
                   deviceName[2] = '\\';   // Needed for Win98
                   deviceName[3] = '\0';

                   ret = GetDriveType(deviceName);

                   if ( ret != DRIVE_UNKNOWN && ret != DRIVE_NO_ROOT_DIR )
                   {
                       // Fix device name
                       deviceName[2] = '\0';

                       fsName[0] = deviceName[0];
                       fsName[1] = '\0';

                       // Setup the file system manager
                       if ((wfs = RtWinFSystemInit(MAX_NB_FILES_PER_FS, deviceName, fsName)) != 0)
                       {
                           // Now actually register the file system
                           if (RtFSManagerRegister(wfs) == FALSE)
                           {
                               return (FALSE);
                           }
                           else
                           {
                               if (deviceName[0] == curDirBuffer[0])
                               {
                                   RtFSManagerSetDefaultFileSystem(wfs);
                               }
                           }
                       }
                       else
                       {
                           return (FALSE);
                       }
                   }
               }
           }
       }

       return (TRUE);
   }   

   /////////////////////////////////////////////////////////////////////////////
   //
   RwBool Initialise ()
   {
      // Initialise RenderWare. The exceptions are used to protect against
      // any access violations inside RenderWare
      try
      {
         if (!RwEngineInit (0, 0, 0))
         {
            FATALERROR ("RwEngineInit failed");
            goto cleanup_init;
         }
      }
      catch (...)
      {
         FATALERROR ("An exception occurred initialising RenderWare");
         goto cleanup_init;
      }

#ifdef _DEBUG
      RwDebugSetHandler (DebugMessageHandler);
#endif // _DEBUG

      try
      {
            if (!RtFSManagerOpen(RTFSMAN_UNLIMITED_NUM_FS))
            {
               FATALERROR ("RtFSManagerOpen failed");
               goto cleanup_init;
            }
      }
      catch (...)
      {
         FATALERROR ("An exception occurred initialising RenderWare file system.\n");
         goto cleanup_init;
      }

      try
      {
            if (!psInstallFileSystem())
            {
               FATALERROR ("RWS::psInstallFileSystem() failed");
               goto cleanup_init;
            }
      }
      catch (...)
      {
         FATALERROR ("An exception occurred registering RenderWare file system.\n");
         goto cleanup_init;
      }

      // displayID should be 0 for all null libraries
      RwEngineOpenParams openParams;

      openParams.displayID = 0;

      // Attach plugins to RenderWare
      if (!AttachPlugins ())
         FATALERROR ("AttachPlugins failed\n");

      // Open RenderWare
      try
      {
         if (!RwEngineOpen (&openParams))
         {
            FATALERROR ("RwEngineOpen failed\n");
            goto cleanup_open;
         }
      }      
      catch (...)
      {
         FATALERROR ("An exception occurred opening RenderWare\n");
         goto cleanup_open;
      }      

      if (!RwEngineSetSubSystem(0))
         FATALERROR ("RwEngineSetSubSystem failed\n");

      // Start RenderWare
      try
      {
         if(!RwEngineStart ())
         {
            FATALERROR ("RwEngineStart failed");
            goto cleanup_start;
         }
      }
      catch (...)
      {
         FATALERROR ("An exception occurred starting RenderWare");
         goto cleanup_start;
      }      

      // Open 2d Toolkit
      try
      {
         RwImageRegisterImageFormat(RWSTRING("png"), RtPNGImageRead, 0);
         RwImageRegisterImageFormat(RWSTRING("bmp"), RtBMPImageRead, 0);

         g_pCamera = RwCameraCreate();
         Rt2dOpen(g_pCamera);
      }
      catch (...) {}

      CoInitialize (0);
      return TRUE;

      // An error occurred. Try our best to clean up RenderWare.
cleanup_start:
      try {RwEngineClose ();}
      catch (...) {}
cleanup_open:
      try {RwEngineTerm ();}
      catch (...) {}
cleanup_init:
      return FALSE;
   }

   /////////////////////////////////////////////////////////////////////////////
   //
   void Shutdown (void)
   {
      Rt2dClose();

      RwCameraDestroy(g_pCamera);

      // Shutdown RenderWare
      try
      {
         if (!RwEngineStop ()) FATALERROR ("RwEngineStop failed\n");
      }
      catch (...)
      {
         FATALERROR ("An exception occurred stopping RenderWare\n");
      }

      try
      {
         if (!RwEngineClose ()) FATALERROR ("RwEngineClose failed\n");
      }
      catch (...)
      {
         FATALERROR ("An exception occurred closing RenderWare\n");
      }

      try
      {
         if (!RwEngineTerm ()) FATALERROR ("RwEngineTerm failed\n");
      }
      catch (...)
      {
         FATALERROR ("An exception occurred terminating RenderWare\n");
      }

      CoUninitialize ();
   }

   /////////////////////////////////////////////////////////////////////////////
   //
   const char * GetName ()
   {
#if defined (RWSPLATFORM_OPENGL)
      return ("OpenGL");
#elif defined (RWSPLATFORM_DIRECTX)
      return ("DirectX");
#elif defined (RWSPLATFORM_XBOX)
      return ("Xbox");
#elif defined (RWSPLATFORM_PS2)
      return ("PS2");
#elif defined (RWSPLATFORM_GAMECUBE)
      return ("GameCube");
#else
#error   A platform must be specified.
#endif
   }

   /////////////////////////////////////////////////////////////////////////////
   //
   Rt2dFont *GetFontData(RwChar *pFontName)
   {
      Rt2dFont *pFont = Rt2dFontRead(pFontName);

      if (!pFont)
      {
         FATALERROR ("Could not extract the font data.");
      }

      return pFont;
   }

   /////////////////////////////////////////////////////////////////////////////
   //
   RwBool SaveFontData (RwChar *pInFileName, RwChar *pOutFileName, RwBool bFont)
   {
      RwBool ret = FALSE;

      PathRemoveExtension(pInFileName);

      RwChar *pFileNamePtr = PathFindFileName(pInFileName);

      RwChar *pFileName = new RwChar [rwstrlen(pFileNamePtr) + 1];
      rwstrcpy(pFileName, pFileNamePtr);

      *pFileNamePtr = '\0';

      RwChar *pPath = new RwChar [rwstrlen(pInFileName) + 1];
      rwstrcpy(pPath, pInFileName);
      Rt2dFontSetPath(pPath);

      Rt2dFont *pFont = GetFontData(pFileName);

      if (bFont)
      {
         if (WriteOutFontFile(pOutFileName, pFont))
         {
            ret = TRUE;
         }
      }
      else
      {
         if (WriteOutFontTexDictFile(pOutFileName))
         {
            ret = TRUE;
         }
      }

      delete [] pPath;

      delete [] pFileName;

      return ret;
   }

} // Anon. namespace

////////////////////////////////////////////////////////////////////////////////
int main (int argc, char *argv[])
{
   if (!Initialise () != 0)
   {
      // Error occurred
      return -1;
   }

   int nRetVal = 0;

   // Process the command line args
   if (argc == 4)
   {
      if (strcmp(argv[1], "-txd") == 0)
      {
         if (!SaveFontData (argv[2], argv[3], FALSE))
         {
            nRetVal = -2;
         }
      }
      else if (strcmp(argv[1], "-fnt") == 0)
      {
         if (!SaveFontData (argv[2], argv[3], TRUE))
         {
            nRetVal = -2;
         }
      }
   }
   else
   {
      cerr << argv[0] << ": " << GetName() << " font building tool" << endl;
      cerr << "Options:" << endl;
      cerr << "-txd: " << "Generate a texture dictionary for the font." << endl;
      cerr << "  Usage: " << argv[0] << " -txd FontMetricsFile OutputFile" << endl;
      cerr << "-fnt: " << "Generate a font file." << endl;
      cerr << "  Usage: " << argv[0] << " -fnt FontMetricsFile OutputFile" << endl;
   }

   Shutdown ();

   return nRetVal;
}

////////////////////////////////////////////////////////////////////////////////
