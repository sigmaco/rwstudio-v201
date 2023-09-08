/*****************************************************************************
 *
 * File :     videomode.cpp
 *
 * Abstract : Utility function used to select a best fit/suitable video mode.
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

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/core/macros/debugmacros.h"

namespace RWS
{
   namespace
   {
      const char txt_blank[] = "";
      const char txt_rwVIDEOMODEEXCLUSIVE[] = "EXCLUSIVE ";
      const char txt_rwVIDEOMODEINTERLACE[] = "INTERLACE ";
      const char txt_rwVIDEOMODEFFINTERLACE[] = "FFINTERLACE ";
      const char txt_rwVIDEOMODEFSAA0[] = "FSAA0 ";
      const char txt_rwVIDEOMODEFSAA1[] = "FSAA1 ";

      const char *isrwVIDEOMODEEXCLUSIVE (RwUInt32 flag)
      {

         if (flag & rwVIDEOMODEEXCLUSIVE) return txt_rwVIDEOMODEEXCLUSIVE;
         else return txt_blank;
      }

      const char *isrwVIDEOMODEINTERLACE (RwUInt32 flag)
      {
         if (flag & rwVIDEOMODEINTERLACE) return txt_rwVIDEOMODEINTERLACE;
         else return txt_blank;
      }

      const char *isrwVIDEOMODEFFINTERLACE (RwUInt32 flag)
      {
         if (flag & rwVIDEOMODEFFINTERLACE) return txt_rwVIDEOMODEFFINTERLACE;
         else return txt_blank;
      }

      const char *isrwVIDEOMODEFSAA0 (RwUInt32 flag)
      {
         if (flag & rwVIDEOMODEFSAA0) return txt_rwVIDEOMODEFSAA0;
         else return txt_blank;
      }

      const char *isrwVIDEOMODEFSAA1 (RwUInt32 flag)
      {
         if (flag & rwVIDEOMODEFSAA1) return txt_rwVIDEOMODEFSAA1;
         else return txt_blank;
      }
   }
   
   namespace StartUp
   {
      /**
      *
      *  \ingroup StartUp
      *
      *  Searches RenderWare graphics for the best video mode and makes it current. Returns
      *  true on success.
      *
      *  Flags can be specified to control the video mode. These are as follows...
      *
      *  rwVIDEOMODEEXCLUSIVE   = full screen, see RenderWare documentation.
      *  rwVIDEOMODEINTERLACE   = interlaced, see RenderWare documentation.
      *  rwVIDEOMODEFFINTERLACE = flicker free non-interlaced, see RenderWare documentation.
      *  rwVIDEOMODEFSAA0       = Anti-aliasing, , see RenderWare documentation.
      *  rwVIDEOMODEFSAA1       = Anti-aliasing, , see RenderWare documentation.
      *
      *  NOTE: some flags are platform specific, see RenderWare documentation.
      *
      *  \param screenWidth On entry, the desired screen width. On exit, the actual screen
      *  width set.
      *  \param screenHeight On entry, the desired screen height. On exit, the actual screen
      *  height set.
      *  \param bitDepth The required bit depth of the screen.
      *  \param flags Flags specifying additional desired properties of the screen.
      *  \param zDepth The bit depth of the Z buffer..
      *
      *  \returns Returns true on success.
      */
      RwBool SelectVideoMode( RwInt32& screenWidth, RwInt32& screenHeight, RwInt32 bitDepth, 
                            RwUInt32 flags, RwUInt32 zDepth)
      {
         RWS_FUNCTION("SelectVideoMode");

         RWS_TRACE("Request "<<screenWidth<<" "<<screenHeight<<" "<<bitDepth<<" "
                  <<isrwVIDEOMODEEXCLUSIVE (flags)
                  <<isrwVIDEOMODEINTERLACE (flags)
                  <<isrwVIDEOMODEFFINTERLACE (flags)
                  <<isrwVIDEOMODEFSAA0 (flags)
                  <<isrwVIDEOMODEFSAA1 (flags)
               );
      
         RwBool result = FALSE;
      
         RwInt32 bestSubSystem = -1;
         RwInt32 bestVideoMode = -1;
         RwVideoMode bestVideoModeInfo;

#ifdef SKY
         if (zDepth == 32)
         {
            RpSkySelectDeepZBuffer(TRUE);
         }
#endif
         
         // Check each video mode in each subsystem
         const RwInt32 nSubSystems = RwEngineGetNumSubSystems();
         for (RwInt32 subSystem = 0; subSystem < nSubSystems; ++subSystem )
         {
            RwEngineSetSubSystem(subSystem);
            const RwInt32 nVidModes = RwEngineGetNumVideoModes();
            for (RwInt32 videoMode = 0; videoMode < nVidModes; ++videoMode )
            {
               RwVideoMode videoModeInfo;
               RwEngineGetVideoModeInfo(&videoModeInfo, videoMode);
            
               // First suitable video mode, make a note
               if (bestVideoMode < 0)
               {
                  bestSubSystem = subSystem;
                  bestVideoMode = videoMode;
                  bestVideoModeInfo = videoModeInfo;
               }
               else
               {
                  // Score the video modes, 
                  // Priority flags (16pts) 
                  // Equal size (8pts)
                  // Sufficient size (4pts)
                  // Bitdepth (1pt)
                  //
                  RwUInt32 score_videoModeInfo = 0;
                  RwUInt32 score_bestVideoModeInfo = 0;

                  // Score this video mode
                  //
                  if (videoModeInfo.depth == static_cast<RwInt32>(bitDepth) ) score_videoModeInfo += 1;
                  if (videoModeInfo.width > static_cast<RwInt32>(screenWidth) ) score_videoModeInfo += 2;
                  if (videoModeInfo.height > static_cast<RwInt32>(screenHeight) ) score_videoModeInfo += 2;
                  if (videoModeInfo.width == static_cast<RwInt32>(screenWidth) ) score_videoModeInfo += 4;
                  if (videoModeInfo.height == static_cast<RwInt32>(screenHeight) ) score_videoModeInfo += 4;
                  if (videoModeInfo.flags == static_cast<RwVideoModeFlag>(flags) ) score_videoModeInfo += 16;

                  // Score current best video mode
                  //
                  if (bestVideoModeInfo.depth == static_cast<RwInt32>(bitDepth) ) score_bestVideoModeInfo += 1;
                  if (bestVideoModeInfo.width > static_cast<RwInt32>(screenWidth) ) score_bestVideoModeInfo += 2;
                  if (bestVideoModeInfo.height > static_cast<RwInt32>(screenHeight) ) score_bestVideoModeInfo += 2;
                  if (bestVideoModeInfo.width == static_cast<RwInt32>(screenWidth) ) score_bestVideoModeInfo += 4;
                  if (bestVideoModeInfo.height == static_cast<RwInt32>(screenHeight) ) score_bestVideoModeInfo += 4;
                  if (bestVideoModeInfo.flags == static_cast<RwVideoModeFlag>(flags) ) score_bestVideoModeInfo += 16;

                  RWS_TRACE("Available "<<videoModeInfo.width
                       <<" "<<videoModeInfo.height
                       <<" "<<videoModeInfo.depth<<" "
                       <<isrwVIDEOMODEEXCLUSIVE (videoModeInfo.flags)
                       <<isrwVIDEOMODEINTERLACE (videoModeInfo.flags)
                       <<isrwVIDEOMODEFFINTERLACE (videoModeInfo.flags)
                       <<isrwVIDEOMODEFSAA0 (videoModeInfo.flags)
                       <<isrwVIDEOMODEFSAA1 (videoModeInfo.flags));

                  if (score_videoModeInfo > score_bestVideoModeInfo)
                  {
                     bestSubSystem = subSystem;
                     bestVideoMode = videoMode;
                     bestVideoModeInfo = videoModeInfo;
                  }
                  else if (score_videoModeInfo == score_bestVideoModeInfo)
                  {
                     if ((videoModeInfo.width * videoModeInfo.height)
                        < (bestVideoModeInfo.width * bestVideoModeInfo.height) )
                     {
                        bestSubSystem = subSystem;
                        bestVideoMode = videoMode;
                        bestVideoModeInfo = videoModeInfo;
                     }
                  }
               }
            }
         }

         RWS_ASSERT( bestSubSystem >= 0, "No best sub-system" );
      
         // If an acceptable video mode was found, use it
         if (bestVideoMode >= 0 )
         {
            RwEngineSetSubSystem(bestSubSystem);
            RwEngineSetVideoMode(bestVideoMode);
         
            RWS_TRACE("Obtained "<<bestVideoModeInfo.width
                  <<" "<<bestVideoModeInfo.height<<" "<<bestVideoModeInfo.depth<<" "
                  <<isrwVIDEOMODEEXCLUSIVE (bestVideoModeInfo.flags)
                  <<isrwVIDEOMODEINTERLACE (bestVideoModeInfo.flags)
                  <<isrwVIDEOMODEFFINTERLACE (bestVideoModeInfo.flags)
                  <<isrwVIDEOMODEFSAA0 (bestVideoModeInfo.flags)
                  <<isrwVIDEOMODEFSAA1 (bestVideoModeInfo.flags)
               );

            // If windowing is obtained, RenderWare will be blitting to the window from a
            // sub-set of the main video raster. So from a client's point of view, the
            // dimensions of the main video raster are irrelevant.
            if ((bestVideoModeInfo.flags & rwVIDEOMODEEXCLUSIVE))
            {
               screenWidth = bestVideoModeInfo.width;
               screenHeight = bestVideoModeInfo.height;
            }
         
            RwSubSystemInfo subSystemInfo;
            RwEngineGetSubSystemInfo(&subSystemInfo, bestSubSystem);

            result = TRUE;
         }
      
         RWS_RETURN(result);
      }
   }
}
