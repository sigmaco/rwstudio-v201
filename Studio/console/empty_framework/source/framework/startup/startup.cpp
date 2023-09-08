/*****************************************************************************
*
* File :     StartUp.cpp
*
* Abstract : 
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
* Copyright (c) 2003 Criterion Software Ltd.
* All Rights Reserved.
*
* RenderWare is a trademark of Canon Inc.
*
*****************************************************************************/
#include "precomp.h"
//#define RWS_TRACEENABLE
//#define RWS_CALLSTACKENABLE

//////////////////////////////////////////////////////////////////
// 
// RenderWare Studio Includes
//
#include "rtfsyst.h" // New 3.6 File System

#include "startup.h"
#include "videomode.h"
#include "registerimageloaders.h"
#include "attachplugins.h"
#include "debugmsghandler.h"

#if (defined (DOLPHIN))
#include "gcn/gcn.h"
#endif

#if (defined (SKY))
#include "sky/sky.h"
#include "sky/skyregisterpipes.h"
#endif

#if (defined (_XBOX))

#include "xbox/xbox.h"

#else
   #if (defined (WIN32)) 
   #include "win32/win.h"
   #endif
#endif


#include "framework/core/macros/debugmacros.h"
#include "framework/mainloop/mainloop.h"
#include "framework/toolkits/time/gftime.h"
#include "framework/core/functionprofiler/profile.h"

namespace RWS
{
   namespace StartUp
   {
      namespace
      {
#ifndef NDEBUG
         RwBool gIsInited = FALSE;
#endif
      }

      /**
      *
      *  \ingroup StartUp
      *
      *  Open the game framework and all sub-systems.
      *
      *  This includes all of RenderWare graphics (opening and starting the 
      *  engine, setting a video mode, attaching plugins and registering image
      *  loaders) plus a number of other non-RenderWare
      *  initializations.
      *
      *  \param pFSInstaller Pointer to file system install function.
      *
      *  \param screenWidth Required width of display area.
      *
      *  \param screenHeight Required height of display area.
      *
      *  \param bitDepth Required bit depth of display area.
      *
      *  \param flags Platform dependent display flags, for example rwVIDEOMODEEXCLUSIVE.
      *
      *  \param ZBufferDepth Require z buffer depth of display area.
      *
      *  \param pDisplayID This type is used to specify device dependent parameters 
      *  for use by the RenderWare Graphics API function RwEngineOpen.
      *
      *  \param memfuncs Pointer to a RwMemoryFunctions used to specify the memory 
      *  functions used by RenderWare, see RenderWare Graphics API function RwEngineInit.
      *
      *  \return Returns true on success, otherwise false.
      */
      RwBool Open(
         const FileSystemInstaller pFSInstaller,
         const RwMemoryFunctions *memfuncs,
         RwUInt32 ArenaSize,
         RwVideoMode &rVideoMode,
         const RwUInt32 ZBufferDepth,
         void* pDisplayID
         )
      {
         RWS_FUNCTION("Startup::Open");
         RWS_PRE(!gIsInited);

         RwBool result = FALSE;

         // If the memory function pointer is ZERO, then use the standard ones.

         if (!memfuncs)
         {
            static RwMemoryFunctions pDefMemFuncs =
            {
               RWSMalloc,
               RWSFree,
               RWSReAlloc,
               RWSCalloc
            };

            memfuncs = &pDefMemFuncs;
         }

         // Initialize Time toolkit
         //
         Time::Open();
         
         RWS::FunctionProfile::CProfileManager::OpenProfiler(10000, RWS::Time::GetTime);

         if (RwEngineInit(memfuncs, 0, ArenaSize))
         {
            
            if (RtFSManagerOpen(RTFSMAN_UNLIMITED_NUM_FS) != FALSE)
            {
               if (RWS::psInstallFileSystem())
               {

                  RwDebugSetHandler(DebugMessageHandler);

                  if (AttachPlugins())
                  {
                     RwEngineOpenParams openParams = { pDisplayID };

                     if (RwEngineOpen(&openParams))
                     {
                        if (SelectVideoMode(rVideoMode.width, 
                                            rVideoMode.height, 
                                            rVideoMode.depth, 
                                            rVideoMode.flags, 
                                            ZBufferDepth))
                        {
                           if (RwEngineStart())
                           {
                              if (RegisterImageLoaders())
                              {
                                 // Select suitable tolerances
                                 //
                                 RwMatrixTolerance tolerance;

                                 tolerance.Normal = 0.00001f;
                                 tolerance.Orthogonal = 0.00001f;
                                 tolerance.Identity = 0.01f;

                                 RwEngineSetMatrixTolerances (&tolerance);

                                 // Game framework startup
                                 //
                                 MainLoop::Open(rVideoMode.width, rVideoMode.height, ZBufferDepth);

                                 result = TRUE;
                              }
                              else
                              {
                                 RwEngineStop();
                              }
                           }
                        }
                        if (!result) RwEngineClose();
                     }
                  }
               }
            }
            if (!result) RwEngineTerm();
         }
#ifndef NDEBUG
         gIsInited = result;
#endif
         RWS_RETURN(result);
      }
      
      /**
      *
      *  \ingroup StartUp
      *
      *  Shutdown game framework and all sub-systems.
      *
      */
      void Close()
      {
         RWS_FUNCTION("Startup::Close");
         RWS_PRE(gIsInited);

         // Game framework shutdown
         //
         MainLoop::Close();

         // Shutdown RenderWare Graphics
         //
         RwEngineStop();
         RwEngineClose();
         RwEngineTerm();

         // Close the file system manager down         
         RtFSManagerClose();

#ifndef NDEBUG
         gIsInited = false;
#endif

         RWS::FunctionProfile::CProfileManager::CloseProfiler();

         // Shutdown Time Toolkit
         //
         Time::Close();

         // Finally, call the memory handler and ask it to dump out usages, etc...
         // MUST BE last as needs everything to free before it can check for leaks, etc...

         MemHandlerCheckAndReport();

         RWS_RETURNVOID();
      }
   }
}
