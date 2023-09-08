/*****************************************************************************
 *
 * File :     Sky.cpp
 *
 * Abstract : PS2 StartUp, game loop and shutdown
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
#if (defined(__MWERKS__))
#include <mwutils.h>
#endif

#if defined(SN_TARGET_PS2)
#include <libsn.h>
#endif

#include "sifdev.h"
#include "sifrpc.h"

#include "rtfsyst.h" // New 3.6 File System

#include "framework/core/macros/debugmacros.h"
#include "framework/mainloop/mainloop.h"
#include "../StartUp.h"
#include "../loadgamedatabase.h"
#include "skyiop.h"

#if defined (CDROM) || defined (DVDROM)
/* 
*  If this file cannot be found you need to add a #include path
*  to the project settings for the common\include directory under
*  where your Sony toolchain is installed.
*
*  For example, if the toolchain is in C:\usr\local\sce you need 
*  to add C:\usr\local\sce\common\include to the #include paths.
*/
#include "libcdvd.h"
#endif

using namespace RWS;

namespace RWS
{
   // Select Video Mode, Width
   //
   const RwInt32 kScreenWidth = 640;

   // Select Video Mode, Height
   //
#if defined (VIDEO_MODE_PAL)
#pragma message ("VIDEO_MODE_PAL Defined")
   const RwInt32 kScreenHeight = 512;
#elif defined (VIDEO_MODE_NTSC)
#pragma message ("VIDEO_MODE_NTSC Defined");
   const RwInt32 kScreenHeight = 448;
#else
#pragma message ("VIDEO_MODE_NTSC Not Defined");
   const RwInt32 kScreenHeight = 448;
#endif   

   #if defined (CDROM) || defined (DVDROM)
   #define READ_BUFFER_SIZE SKY_IOP_SECTORSIZE
   #define MAX_NB_IOP_FILES   (5)
   #else
   #define READ_BUFFER_SIZE (64 * 1024)
   #endif

   #if !defined (CDROM) || !defined (DVDROM)
      const RwChar * FS_MODULE_PATH = "host0:../../rwsdk/tool/fsyst/sky2/iop/rtfssiop.irx";
   #else
      const RwChar * FS_MODULE_PATH = "cdrom0:\\RTFSSIOP.IRX;1";
   #endif
   
   const RwInt32 MAX_NB_FILES_PER_FS = 5;

   // Select Video Mode, color depth
   //
   const RwInt32 kBitDepth = 32;

   // Select Video Mode, z-buffer depth
   //
   const RwInt32 kZBitDepth = 16;

   // Select Video Mode, mode.
   //
   const RwInt32 kFlags = static_cast<RwUInt32>(rwVIDEOMODEEXCLUSIVE | rwVIDEOMODEFSAA1);

   // Stores the root file server path, e.g. host0: for network connections, cdrom0: for CD/DVD
   //
   char g_szRootFileServerPath[256] = "";

   // Buffer used by the filesystem
   // 
   RwChar hBuffer[MAX_NB_FILES_PER_FS * READ_BUFFER_SIZE] __attribute__((aligned(64)));


   /*
   *  
   *  \ingroup Sky
   *
   *  PS2 only: Loads a single IRX
   *
   */
   void LoadModule(const char* pModuleName)
   {
      RWS_FUNCTION("LoadModule");
      RWS_PRE(pModuleName);

      char szModule[251];
      sprintf( szModule, "%s%s", RWS::GetRootFileServerPath(), pModuleName );
      
      if (sceSifLoadModule(szModule, 0, "") < 0)
      {
         RWS_ASSERTFAIL("Cannot load module " << szModule <<
            "\nPlease check the definitions of IOP_MODULEPATH, IOP_RWSCOMMSPATH,"
            <<"RWS_AUDIO_MODULEPATH in core\\StartUp\\skyiop.h");
      }
      
      RWS_RETURNVOID();
   }

#if defined (CDROM) || defined (DVDROM)
   /*
   *  \ingroup Sky
   *
   *  PS2 only: Reboots IOP replacing default modules
   *
   */
   void RebootIOP(const char * pFileName)
   {
      RWS_FUNCTION("RebootIOP");
      RWS_PRE(pFileName);

      char szIOPFile[70];
      sprintf( szIOPFile, "%s%s", RWS::GetRootFileServerPath(), pFileName );
      
      RWS_TRACE ("Attempting to reboot IOP with file " << szIOPFile);
      while (!sceSifRebootIop (szIOPFile))
      {
         continue;
      }
      
      RWS_TRACE ("Syncing IOP");
      while (!sceSifSyncIop ())
      {
         continue;
      }
      
      
      RWS_RETURNVOID();
   }
#else
#define RebootIOP(pFileName)
#endif

   /*
   *  
   *  \ingroup Sky
   *
   *  PS2 only: Loads IRXs for pad input
   *
   */
   void LoadPadIRXs()
   {
      RWS_FUNCTION("LoadPadIRXs");

#if defined (CDROM) || defined (DVDROM)

      LoadModule(IOP_MODULEPATH_CDROM "SIO2MAN.IRX;1");
      LoadModule(IOP_MODULEPATH_CDROM "PADMAN.IRX;1");


#else // #if defined (CDROM) || defined (DVDROM)

      LoadModule(IOP_MODULEPATH "sio2man.irx");
      LoadModule(IOP_MODULEPATH "padman.irx");
      
#endif // #if defined (CDROM) || defined (DVDROM)

      RWS_RETURNVOID();
   }
 
   /*
   *  
   *  \ingroup Sky
   *
   *  PS2 only: Loads IRXs for audio
   *
   */
#ifdef WITH_AUDIO
   void LoadAudioIRXs()
   {
     RWS_FUNCTION("LoadAudioIRXs");

#if defined (CDROM) || defined (DVDROM)
     LoadModule(IOP_MODULEPATH_CDROM "LIBSD.IRX;1");
     LoadModule(IOP_MODULEPATH_CDROM "SDRDRV.IRX;1");
     LoadModule(RWS_MODULEPATH_CDROM "RWA.IRX;1");      

#else // #if defined (CDROM) || defined (DVDROM)
     LoadModule(IOP_MODULEPATH "libsd.irx");
     LoadModule(IOP_MODULEPATH "sdrdrv.irx");
     LoadModule(RWS_AUDIO_MODULEPATH  "rwa.irx");      
     
#endif // #if defined (CDROM) || defined (DVDROM) 

     RWS_RETURNVOID();
   }
#else
#define LoadAudioIRXs()
#endif

}


namespace RWS 
{
   /*
   *
   *  \ingroup Sky
   *
   *  PS2 only: Retrieves the root path for file serving. For example, host0: for networked files, cdrom0: for files
   *              on the local CD/DVD.
   *
   */
   const char * GetRootFileServerPath()
   {
      return &g_szRootFileServerPath[0];
   }


   /*
   *
   *  \ingroup Sky
   *
   *  PS2 only: Set the root path for fileserving on the PS2. Usually host0: for devkit builds,
   *  cdrom0: for CD/DVD builds.
   *
   */
   void SetRootFileServerPath( const char * szRootFileServerPath )
   {
      strncpy(g_szRootFileServerPath, szRootFileServerPath, sizeof(g_szRootFileServerPath) );
   }
   
   /*
   *  
   *  \ingroup Sky
   *
   *  Installs the platform specific file system.
   *
   */
   RwBool
   psInstallFileSystem(void)
   {
      RtFileSystem *fs;
      RwChar deviceName[5];
      RwChar fsName[RTFS_MAX_NAME_LENGTH];
      
#if defined(CDROM) || defined(DVDROM)
      int ret;
      rwstrcpy(deviceName, "dvd:");
      rwstrcpy(fsName, "dvd");
      
      if ((ret = sceSifLoadModule(FS_MODULE_PATH, 0, "")) < 0) 
      {
         RwDebugSendMessage(rwDEBUGMESSAGE, __FUNCTION__,
            "Module rtfsiop cannot be loaded");
      }
      
      // Init and register the DVD file system
      if ((fs = RtSkyIOPFSystemInit(MAX_NB_FILES_PER_FS, MAX_NB_IOP_FILES, hBuffer, READ_BUFFER_SIZE, 
                                    deviceName, fsName, TRUE, RTFSDEVICE_DVD)) != NULL)
#elif defined (DEBUGSTATION)
      rwstrcpy(fsName, "atm");
      if ((fs = RtSkyATMonFSystemInit(MAX_NB_FILES_PER_FS, hBuffer, READ_BUFFER_SIZE, 
                                      RWS::GetRootFileServerPath(), fsName)) != NULL)      
#elif defined (HDD)
      rwstrcpy(deviceName, "hdd0:");
      rwstrcpy(fsName, "hdd");
      
      // Init the Hard Disk File System
      if ((fs = RtSkyHDDFSystemInit(MAX_NB_FILES_PER_FS, hBuffer, READ_BUFFER_SIZE, 
                                    deviceName, fsName, "pfs0:", "test", "fpasswd")) != NULL)
#else
      rwstrcpy(deviceName, "hfs:");
      rwstrcpy(fsName, "hst");
      
      // Init and register the host file system
      if ((fs = RtSkyHSTFSystemInit(MAX_NB_FILES_PER_FS, hBuffer, READ_BUFFER_SIZE, deviceName, fsName)) != NULL)
#endif
      {    
         // Register the file system
         if (RtFSManagerRegister(fs) == FALSE)
         {
            return FALSE;
         }
         else
         {
            RtFSManagerSetDefaultFileSystem(fs);
#ifdef CDROM_HDD
            RtSkyIOPFSystemInitializeHDD("pfs0:", "test", "fpasswd");
#endif
         }
      }
      else
      {
         return FALSE;
      }
      return TRUE;
   }
}


/*
*  
*  \ingroup Sky
*
*  PS2 only: Main entry point
*
*/
int main(int argc, char *argv[])
{
   RWS_FUNCTION("main");

#if (defined(__MWERKS__))
   mwInit();
#endif
    
   // if the framework was run via atwinmon, the required fileserver root will be passed in argv[0]
   //    so parse it here and set the global fileserver root appropriately.
   if (argc == 0)
   {
#if (defined (CDROM) || defined (DVDROM))
      RWS::SetRootFileServerPath("cdrom0:");
#else // #if defined (CDROM) || defined (DVDROM)
      RWS::SetRootFileServerPath("host0:");
#endif   // #if defined (CDROM) || defined (DVDROM)
   }
   else
   {
      if (strnicmp(argv[0], "atfile", 6) == 0)
      {
         char * strptr = argv[0];

         while (*strptr != ',')
         {
            strptr++;
         }

         strptr++;
         *strptr = 0;
         RWS::SetRootFileServerPath(argv[0]);
      }
      else
      {
#if (defined (CDROM) || defined (DVDROM))
         RWS::SetRootFileServerPath("cdrom0:");
#else // #if defined (CDROM) || defined (DVDROM)
         RWS::SetRootFileServerPath("host0:");
#endif // #if defined (CDROM) || defined (DVDROM)
      }
   }
   
   sceSifInitRpc(0);
   sceSifInitIopHeap();
   
   // ----------------------------------------------------------------------------------------------
   // Initialise CDROM 
   // ----------------------------------------------------------------------------------------------
#if defined (CDROM)
#pragma message ("CDROM Defined")
   sceCdInit(SCECdINIT);
   sceCdMmode(SCECdCD);
#endif // #ifdef CDROM

   // ----------------------------------------------------------------------------------------------
   // Initialise DVDROM
   // ----------------------------------------------------------------------------------------------
#if defined (DVDROM)
#pragma message ("CDROM Defined")
   sceCdInit(SCECdINIT);
   sceCdMmode(SCECdDVD);
#endif

   RebootIOP (IOP_MODULEPATH_CDROM IOP_IMAGE_FILE ";1");

   // ----------------------------------------------------------------------------------------------

   // IOP has been rebooted need to reset
   //
   sceSifInitRpc(0);
   sceSifInitIopHeap();

   // ----------------------------------------------------------------------------------------------
   // Re-Initialise CDROM 
   // ----------------------------------------------------------------------------------------------
#if (defined(CDROM))
   sceCdInit(SCECdINIT);
   sceCdMmode(SCECdCD);
#endif
   // ----------------------------------------------------------------------------------------------
   // Re-Initialise DVDROM 
   // ----------------------------------------------------------------------------------------------
#if (defined(DVDROM))
   sceCdInit(SCECdINIT);
   sceCdMmode(SCECdDVD);
#endif

   sceFsReset();

   LoadPadIRXs();
   LoadAudioIRXs();

   // ----------------------------------------------------------------------------------------------
   
#if defined(SN_TARGET_PS2) & defined(RWS_DESIGN)
   // Initialise the SN Systems profile module. 
   //   Note: Only a rough per-function profile, used via the profile window in the debugger.
   //          Need to build with debug symbols so debugger can get at the function names.
   static unsigned long profdata[2048] __attribute__((aligned(16))); // quad word aligned, // 4K to 64K bytes 

#if defined (CDROM) || defined (DVDROM)

   LoadModule(IOP_MODULEPATH_CDROM "SNPROFIL.IRX;1");

#else

   LoadModule( IOP_MODULEPATH "SNProfil.irx" );

#endif

   int nRetVal = snProfInit( _4KHZ, profdata, sizeof(profdata) );

   if ( nRetVal != 0 )
   {
      printf("Profiler init failed : code %d\n", nRetVal ); // see SN_PRF in LIBSN.H 
   }
#endif

   // ----------------------------------------------------------------------------------------------

   RwVideoMode videoModeInfo;

   videoModeInfo.width = kScreenWidth;
   videoModeInfo.height = kScreenHeight;
   videoModeInfo.depth = kBitDepth;
   videoModeInfo.flags = static_cast<RwVideoModeFlag>(kFlags);

   if (RWS::StartUp::Open(
      0,
      0, // Use Defaults
      (8 << 20), // Arena Size 8 Meg
      videoModeInfo,
      kZBitDepth,
      0))
   {
#ifndef RWS_DESIGN

#if defined (CDROM) || defined (DVDROM)
    #define RWS_BOOTUP_FILE "\\BOOTUP.DFF;1"
#else
    #define RWS_BOOTUP_FILE "\\BOOTUP.DFF"
#endif
#pragma message ("RWS_DESIGN not defined booting from file, RWS_BOOTUP_FILE")
      char szBootupFile[256];
      sprintf( szBootupFile, "%s%s", RWS::GetRootFileServerPath(), RWS_BOOTUP_FILE );
      StartUp::LoadGameDatabaseFile(szBootupFile);
#endif

      // Effectively this is the main loop...
      //
      while (true) RWS::MainLoop::Poll();

      StartUp::Close();
   }
   else
   {
      RWS_ASSERTFAIL("StartUp::Open Failed unable to continue.");
   }

   // ----------------------------------------------------------------------------------------------

#if (defined(__MWERKS__))
   mwExit();
#endif

   RWS_RETURN(0);
}
 
