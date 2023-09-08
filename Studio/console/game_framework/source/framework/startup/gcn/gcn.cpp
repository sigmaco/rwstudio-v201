/**********************************************************************
 *
 * File :     GCN.cpp
 *
 * Abstract : RWS application entry point
 *
 **********************************************************************
 *
 * This file is a product of Criterion Software Ltd.
 *
 * This file is provided as is with no warranties of any kind and is
 * provided without any obligation on Criterion Software Ltd. or
 * Canon Inc. to assist in its use or modification.
 *
 * Criterion Software Ltd. will not, under any
 * circumstances, be liable for any lost revenue or other damages
 * arising from the use of this file.
 *
 * Copyright (c) 2002 Criterion Software Ltd.
 * All Rights Reserved.
 *
 * RenderWare is a trademark of Canon Inc.
 *
 **********************************************************************/
#include <dolphin.h>
#include <dolphin/os.h>
#include <dolphin/gx.h>

#include "rwcore.h"

#include "rtfsyst.h"/* File system */

#include "../macros/debugmacros.h"
#include "framework/startup/startup.h"
#include "framework/mainloop/mainloop.h"
#include "framework/startup/loadgamedatabase.h"

#include "dlfs.h"
#include "dlmem.h"

using namespace RWS;

// Data/constants
const RwUInt32 kScreenWidth   = 640;
const RwUInt32 kScreenHeight  = 448;
const RwUInt32 kBitDepth      = 32;
const RwUInt32 kZBitDepth     = 16;
const RwUInt32 kFlags = static_cast<RwUInt32>(rwVIDEOMODEEXCLUSIVE);

#ifdef BUG_INDX_ALIGN
#include "dolphin/gx/gxmanage.h"
#endif /* BUG_INDX_ALIGN */

static RwTextureCallBackRead    DefaultTextureRead;

static void DebugMessageHandler(RwDebugType type __RWUNUSED__, const RwChar *str)
{
   OSReport((char *)str);
}

static bool InstallFileSystem()
{
   RWS_FUNCTION("InstallFileSystem");
 
   bool result = false;

   if (DolphinInstallFileSystem())
   {
      RWS_TRACE("Sky file system installed");
      result = true;
   }
 
   RWS_RETURN(result);
}
 
int main()
{

#if defined(SN_TARGET_NGC)
   _register_malloc = RwsDolphinHeapAlloc;
   _register_free = RwsDolphinHeapFree;
#endif

    /* Initialize the OS */
    OSInit();
    
#ifdef BUG_XF_STALL
   GXSetMisc(GX_MT_XF_FLUSH, GX_XF_FLUSH_SAFE);
#endif /* BUG_XF_STALL */

   /*
    * Initializes the DVD device driver
    */
   DVDInit();

   // NOTE: Memory system is now handled via the 'RWSMemory' code. This includes init, etc.
    
   VIInit();
    
   PADInit();

   /* fix display list write gather pipe bug */
#ifdef BUG_INDX_ALIGN
   GXSetResetWritePipe(GX_TRUE);
#endif /* BUG_INDX_ALIGN */

   RwVideoMode videoModeInfo;

   videoModeInfo.width = kScreenWidth;
   videoModeInfo.height = kScreenHeight;
   videoModeInfo.depth = kBitDepth;
   videoModeInfo.flags = static_cast<RwVideoModeFlag>(kFlags);

   if (RWS::StartUp::Open(
      InstallFileSystem,
      0, // Use Defaults
      (4 << 20), // Arena Size 4 Meg
      videoModeInfo,
      kZBitDepth,
      0))
   {

      // set the gx warning level to none
      GXSetVerifyLevel(GX_WARN_NONE);
      
#ifndef RWS_DESIGN
#define RWS_BOOTUP_FILE "bootup.dff"

#pragma message ("RWS_DESIGN not defined booting from file, RWS_BOOTUP_FILE")

      StartUp::LoadGameDatabaseFile(RWS_BOOTUP_FILE);

#endif
       
      while (true) RWS::MainLoop::Poll();

      StartUp::Close();
   }
    
   /* Halt the OS */
   OSHalt("The End.");

   return(0);
}

namespace RWS
{
   const RwInt32 MAX_NB_FILES_PER_FS = 4;

   const RwInt32 READ_BUFFER_SIZE = GCN_DVD_SECTORSIZE;
   /*
   *  
   *  \ingroup GCN
   *
   *  platform specific method to install the file system
   *
   */
   RwBool psInstallFileSystem(void)
   {
      RwChar *dDeviceName = "dvd:"; 
      RtFileSystem *dfs;
      RwChar *fsName = "dvd";
      static RwChar hBuffer[MAX_NB_FILES_PER_FS * READ_BUFFER_SIZE] __attribute__ ((aligned(32)));

      /* Init and register the host file system */
      dfs = RtGcnDVDFSystemInit(MAX_NB_FILES_PER_FS, hBuffer, READ_BUFFER_SIZE,
                                dDeviceName, fsName);
      if (dfs != 0)
      {
         /* Now actually register the file system */
         if (RtFSManagerRegister(dfs) == FALSE)
         {
            return (FALSE);
         }
         else
         {
            RtFSManagerSetDefaultFileSystem(dfs);
         }
      }
      else
      {
          return (FALSE);
      }
      return (TRUE);    
   }
}