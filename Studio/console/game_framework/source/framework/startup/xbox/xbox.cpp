/*****************************************************************************
 *
 * File :     Xbox.cpp
 *
 * Abstract : Xbox startup, game loop and shutdown
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
#include <xtl.h>
#include <direct.h>

#include "rwcore.h"

#include "rtfsyst.h" // New 3.6 File System

#include "../startup.h"
#include "framework/mainloop/mainloop.h"
#include "framework/core/macros/debugmacros.h"
#include "../loadgamedatabase.h"

using namespace RWS;

namespace
{
   // Select Video Mode, Width
   //
   const RwUInt32 kScreenWidth = 640;
   
   // Select Video Mode, Height
   //
#if defined (VIDEO_MODE_PAL)
   const RwUInt32 kScreenHeight = 576;
#elif defined (VIDEO_MODE_NTSC)
   const RwUInt32 kScreenHeight = 480;
#else
   const RwUInt32 kScreenHeight = 480;
#endif

   // Used for new RW 3.6 FileSystem
   const RwUInt32 UNC_PATH_SIZE = 256;   /* should be sufficient */
   const RwUInt32 MAX_NB_FILES_PER_FS = 20;

   // Select Video Mode, color depth
   //
   const RwUInt32 kBitDepth = 32;
   
   // Select Video Mode, z-buffer depth
   //
   const RwUInt32 kZBitDepth = 32;

   // Select Video Mode, mode.
   //
   const RwVideoModeFlag kFlags = static_cast<RwVideoModeFlag>(rwVIDEOMODEEXCLUSIVE); 

   // Globals
   RwTextureCallBackRead pOriginalTextureReadCallBack = 0;

   /*
   *  
   *  \ingroup Xbox
   *
   *  Xbox only: Texture read callback
   *
   */
   RwTexture* XboxTextureReadCallBack(const RwChar* name, const RwChar* maskName)
   {
       RwTexture* pTexture = RwXboxDDSTextureRead(name, maskName);
       return pTexture ? pTexture : pOriginalTextureReadCallBack(name, maskName);
   }
}

namespace RWS
{
   /*
   *  
   *  \ingroup Xbox
   *
   *  Platform specific method to install the file system.
   *
   */
   RwBool
   psInstallFileSystem(void)
   {
      RtFileSystem *xfs;
      RwChar *deviceName = "d:";

      if ((xfs = RtXboxFSystemInit(MAX_NB_FILES_PER_FS, deviceName, "xbox")) != 0)
      {
         /* Now register the file system */
         if (RtFSManagerRegister(xfs) == FALSE)
         {
            return (FALSE);
         }
         else
         {
            RtFSManagerSetDefaultFileSystem(xfs);
         }
      }
      else
      {
         return (FALSE);
      }
      return (TRUE);
   }
}

/*
*  
*  \ingroup Xbox
*
*  Xbox only: Main entry point
*
*/
int main()
{
   RWS_FUNCTION("main");

   int result = 0;

   RwVideoMode videoModeInfo;

   videoModeInfo.width = kScreenWidth;
   videoModeInfo.height = kScreenHeight;
   videoModeInfo.depth = kBitDepth;
   videoModeInfo.flags = kFlags;

   if (StartUp::Open(0,
      0,
      (8 << 20),
      videoModeInfo,
      kZBitDepth, 0))
   {
      // Hook in native texture support
      pOriginalTextureReadCallBack = RwTextureGetReadCallBack();
      RwTextureSetReadCallBack(XboxTextureReadCallBack);

#ifndef RWS_DESIGN

#define RWS_BOOTUP_FILE "d:\\bootup.dff"

#pragma message ("RWS_DESIGN not defined booting from file, RWS_BOOTUP_FILE")

      StartUp::LoadGameDatabaseFile(RWS_BOOTUP_FILE);
#endif

      while (true) RWS::MainLoop::Poll();

      StartUp::Close();
   }

   RWS_RETURN(result);
}
