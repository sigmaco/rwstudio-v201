# Microsoft Developer Studio Project File - Name="LoadingScreen" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 60000
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=LoadingScreen - Win32 NGC DDH DesignRelease Broadband
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "LoadingScreen.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "LoadingScreen.mak" CFG="LoadingScreen - Win32 NGC DDH DesignRelease Broadband"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "LoadingScreen - Win32 NGC DDH DesignDebug" (based on "Win32 (x86) Static Library")
!MESSAGE "LoadingScreen - Win32 NGC DDH DesignRelease" (based on "Win32 (x86) Static Library")
!MESSAGE "LoadingScreen - Win32 NGC DDH Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "LoadingScreen - Win32 NGC DDH Release" (based on "Win32 (x86) Static Library")
!MESSAGE "LoadingScreen - Win32 NGC GDEV DesignDebug" (based on "Win32 (x86) Static Library")
!MESSAGE "LoadingScreen - Win32 NGC GDEV DesignRelease" (based on "Win32 (x86) Static Library")
!MESSAGE "LoadingScreen - Win32 NGC GDEV Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "LoadingScreen - Win32 NGC GDEV Release" (based on "Win32 (x86) Static Library")
!MESSAGE "LoadingScreen - Win32 NGC GDEV DesignDebug Broadband" (based on "Win32 (x86) Static Library")
!MESSAGE "LoadingScreen - Win32 NGC GDEV DesignRelease Broadband" (based on "Win32 (x86) Static Library")
!MESSAGE "LoadingScreen - Win32 NGC DDH DesignDebug Broadband" (based on "Win32 (x86) Static Library")
!MESSAGE "LoadingScreen - Win32 NGC DDH DesignRelease Broadband" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "LoadingScreen"
# PROP Scc_LocalPath "."
CPP=snCl.exe
RSC=rc.exe

!IF  "$(CFG)" == "LoadingScreen - Win32 NGC DDH DesignDebug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoadingScreen___Win32_NGC_DDH_DesignDebug"
# PROP BASE Intermediate_Dir "LoadingScreen___Win32_NGC_DDH_DesignDebug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DDH_Design_Debug"
# PROP Intermediate_Dir "DDH_Design_Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Od /I "..\..\..\..\game_framework\source\framework\core\MemoryHandler" /I "C:\DolphinSDK1.0\include" /I "$(RWGSDK)\include\gcn" /I "../../source/" /D "_DEBUG" /D "RWDEBUG" /D "DEBUG" /D "RWS_TRACE_METRICS" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /FD -g /c
# ADD CPP /nologo /W3 /Od /I "$(DOLPHIN_ROOT)\include" /I "..\..\..\..\game_framework\source\framework\core\MemoryHandler" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "_DEBUG" /D "RWDEBUG" /D "DEBUG" /D "RWS_TRACE_METRICS" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /FD -g /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=snLib.exe
# ADD BASE LIB32 /nologo /D:SN_TARGET_NGC
# ADD LIB32 /nologo /D:SN_TARGET_NGC

!ELSEIF  "$(CFG)" == "LoadingScreen - Win32 NGC DDH DesignRelease"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoadingScreen___Win32_NGC_DDH_DesignRelease"
# PROP BASE Intermediate_Dir "LoadingScreen___Win32_NGC_DDH_DesignRelease"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DDH_Design_Release"
# PROP Intermediate_Dir "DDH_Design_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Od /I "..\..\..\..\game_framework\source\framework\core\MemoryHandler" /I "C:\DolphinSDK1.0\include" /I "$(RWGSDK)\include\gcn" /I "../../source/" /D "NDEBUG" /D "NO_RWS_MEM" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "RWS_TRACE_METRICS" /FD -g /c
# ADD CPP /nologo /W3 /Od /I "C:\DolphinSDK1.0\include" /I "..\..\..\..\game_framework\source\framework\core\MemoryHandler" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "NDEBUG" /D "NO_RWS_MEM" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "RWS_TRACE_METRICS" /FD -g /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=snLib.exe
# ADD BASE LIB32 /nologo /D:SN_TARGET_NGC
# ADD LIB32 /nologo /D:SN_TARGET_NGC

!ELSEIF  "$(CFG)" == "LoadingScreen - Win32 NGC DDH Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoadingScreen___Win32_NGC_DDH_Debug"
# PROP BASE Intermediate_Dir "LoadingScreen___Win32_NGC_DDH_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DDH_Debug"
# PROP Intermediate_Dir "DDH_Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Od /I "..\..\..\..\game_framework\source\framework\core\MemoryHandler" /I "C:\DolphinSDK1.0\include" /I "$(RWGSDK)\include\gcn" /I "../../source/" /D "HW2" /D "_DEBUG" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWDEBUG" /D "RWTERMINAL" /D "DEBUG" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /FD -g /c
# ADD CPP /nologo /W3 /Od /I "C:\DolphinSDK1.0\include" /I "..\..\..\..\game_framework\source\framework\core\MemoryHandler" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "HW2" /D "_DEBUG" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWDEBUG" /D "RWTERMINAL" /D "DEBUG" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /FD -g /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=snLib.exe
# ADD BASE LIB32 /nologo /D:SN_TARGET_NGC
# ADD LIB32 /nologo /D:SN_TARGET_NGC

!ELSEIF  "$(CFG)" == "LoadingScreen - Win32 NGC DDH Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoadingScreen___Win32_NGC_DDH_Release"
# PROP BASE Intermediate_Dir "LoadingScreen___Win32_NGC_DDH_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DDH_Release"
# PROP Intermediate_Dir "DDH_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Od /I "..\..\..\..\game_framework\source\framework\core\MemoryHandler" /I "C:\DolphinSDK1.0\include" /I "$(RWGSDK)\include\gcn" /I "../../source/" /D "NO_RWS_MEM" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "NDEBUG" /FD -g /c
# ADD CPP /nologo /W3 /Od /I "C:\DolphinSDK1.0\include" /I "..\..\..\..\game_framework\source\framework\core\MemoryHandler" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "NO_RWS_MEM" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "NDEBUG" /FD -g /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=snLib.exe
# ADD BASE LIB32 /nologo /D:SN_TARGET_NGC
# ADD LIB32 /nologo /D:SN_TARGET_NGC

!ELSEIF  "$(CFG)" == "LoadingScreen - Win32 NGC GDEV DesignDebug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoadingScreen___Win32_NGC_GDEV_DesignDebug"
# PROP BASE Intermediate_Dir "LoadingScreen___Win32_NGC_GDEV_DesignDebug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "GDEV_Design_Debug"
# PROP Intermediate_Dir "GDEV_Design_Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Od /I "..\..\..\..\game_framework\source\framework\core\MemoryHandler" /I "C:\DolphinSDK1.0\include" /I "$(RWGSDK)\include\gcn" /I "../../source/" /D "_DEBUG" /D "RWDEBUG" /D "DEBUG" /D "RWS_TRACE_METRICS" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /FD -g /c
# ADD CPP /nologo /W3 /Od /I "C:\DolphinSDK1.0\include" /I "..\..\..\..\game_framework\source\framework\core\MemoryHandler" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "_DEBUG" /D "RWDEBUG" /D "DEBUG" /D "RWS_TRACE_METRICS" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /FD -g /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=snLib.exe
# ADD BASE LIB32 /nologo /D:SN_TARGET_NGC
# ADD LIB32 /nologo /D:SN_TARGET_NGC

!ELSEIF  "$(CFG)" == "LoadingScreen - Win32 NGC GDEV DesignRelease"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoadingScreen___Win32_NGC_GDEV_DesignRelease"
# PROP BASE Intermediate_Dir "LoadingScreen___Win32_NGC_GDEV_DesignRelease"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "GDEV_Design_Release"
# PROP Intermediate_Dir "GDEV_Design_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Od /I "..\..\..\..\game_framework\source\framework\core\MemoryHandler" /I "C:\DolphinSDK1.0\include" /I "$(RWGSDK)\include\gcn" /I "../../source/" /D "NDEBUG" /D "NO_RWS_MEM" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "RWS_TRACE_METRICS" /FD -g /c
# ADD CPP /nologo /W3 /Od /I "C:\DolphinSDK1.0\include" /I "..\..\..\..\game_framework\source\framework\core\MemoryHandler" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "NDEBUG" /D "NO_RWS_MEM" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "RWS_TRACE_METRICS" /FD -g /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=snLib.exe
# ADD BASE LIB32 /nologo /D:SN_TARGET_NGC
# ADD LIB32 /nologo /D:SN_TARGET_NGC

!ELSEIF  "$(CFG)" == "LoadingScreen - Win32 NGC GDEV Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoadingScreen___Win32_NGC_GDEV_Debug"
# PROP BASE Intermediate_Dir "LoadingScreen___Win32_NGC_GDEV_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "GDEV_Debug"
# PROP Intermediate_Dir "GDEV_Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Od /I "..\..\..\..\game_framework\source\framework\core\MemoryHandler" /I "C:\DolphinSDK1.0\include" /I "$(RWGSDK)\include\gcn" /I "../../source/" /D "HW2" /D "_DEBUG" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWDEBUG" /D "RWTERMINAL" /D "DEBUG" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /FD -g /c
# ADD CPP /nologo /W3 /Od /I "C:\DolphinSDK1.0\include" /I "..\..\..\..\game_framework\source\framework\core\MemoryHandler" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "HW2" /D "_DEBUG" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWDEBUG" /D "RWTERMINAL" /D "DEBUG" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /FD -g /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=snLib.exe
# ADD BASE LIB32 /nologo /D:SN_TARGET_NGC
# ADD LIB32 /nologo /D:SN_TARGET_NGC

!ELSEIF  "$(CFG)" == "LoadingScreen - Win32 NGC GDEV Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoadingScreen___Win32_NGC_GDEV_Release"
# PROP BASE Intermediate_Dir "LoadingScreen___Win32_NGC_GDEV_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "GDEV_Release"
# PROP Intermediate_Dir "GDEV_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Od /I "..\..\..\..\game_framework\source\framework\core\MemoryHandler" /I "C:\DolphinSDK1.0\include" /I "$(RWGSDK)\include\gcn" /I "../../source/" /D "NO_RWS_MEM" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "NDEBUG" /FD -g /c
# ADD CPP /nologo /W3 /Od /I "C:\DolphinSDK1.0\include" /I "..\..\..\..\game_framework\source\framework\core\MemoryHandler" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "NO_RWS_MEM" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "NDEBUG" /FD -g /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=snLib.exe
# ADD BASE LIB32 /nologo /D:SN_TARGET_NGC
# ADD LIB32 /nologo /D:SN_TARGET_NGC

!ELSEIF  "$(CFG)" == "LoadingScreen - Win32 NGC GDEV DesignDebug Broadband"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoadingScreen___Win32_NGC_GDEV_DesignDebug_Broadband"
# PROP BASE Intermediate_Dir "LoadingScreen___Win32_NGC_GDEV_DesignDebug_Broadband"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "GDEV_Design_Debug_Braodband"
# PROP Intermediate_Dir "GDEV_Design_Debug_Braodband"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Od /I "C:\DolphinSDK1.0\include" /I "..\..\..\..\game_framework\source\framework\core\MemoryHandler" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "_DEBUG" /D "RWDEBUG" /D "DEBUG" /D "RWS_TRACE_METRICS" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /FD -g /c
# ADD CPP /nologo /W3 /Od /I "C:\DolphinSDK1.0\include" /I "..\..\..\..\game_framework\source\framework\core\MemoryHandler" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "_DEBUG" /D "RWDEBUG" /D "DEBUG" /D "RWS_TRACE_METRICS" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /FD -g /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=snLib.exe
# ADD BASE LIB32 /nologo /D:SN_TARGET_NGC
# ADD LIB32 /nologo /D:SN_TARGET_NGC

!ELSEIF  "$(CFG)" == "LoadingScreen - Win32 NGC GDEV DesignRelease Broadband"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoadingScreen___Win32_NGC_GDEV_DesignRelease_Broadband"
# PROP BASE Intermediate_Dir "LoadingScreen___Win32_NGC_GDEV_DesignRelease_Broadband"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "GDEV_Design_Release_Braodband"
# PROP Intermediate_Dir "GDEV_Design_Release_Braodband"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Od /I "C:\DolphinSDK1.0\include" /I "..\..\..\..\game_framework\source\framework\core\MemoryHandler" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "NDEBUG" /D "NO_RWS_MEM" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "RWS_TRACE_METRICS" /FD -g /c
# ADD CPP /nologo /W3 /Od /I "C:\DolphinSDK1.0\include" /I "..\..\..\..\game_framework\source\framework\core\MemoryHandler" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "NDEBUG" /D "NO_RWS_MEM" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "RWS_TRACE_METRICS" /FD -g /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=snLib.exe
# ADD BASE LIB32 /nologo /D:SN_TARGET_NGC
# ADD LIB32 /nologo /D:SN_TARGET_NGC

!ELSEIF  "$(CFG)" == "LoadingScreen - Win32 NGC DDH DesignDebug Broadband"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoadingScreen___Win32_NGC_DDH_DesignDebug_Broadband"
# PROP BASE Intermediate_Dir "LoadingScreen___Win32_NGC_DDH_DesignDebug_Broadband"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DDH_Design_Debug_Broadband"
# PROP Intermediate_Dir "DDH_Design_Debug_Broadband"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Od /I "$(DOLPHIN_ROOT)\include" /I "..\..\..\..\game_framework\source\framework\core\MemoryHandler" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "_DEBUG" /D "RWDEBUG" /D "DEBUG" /D "RWS_TRACE_METRICS" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /FD -g /c
# ADD CPP /nologo /W3 /Od /I "$(DOLPHIN_ROOT)\include" /I "..\..\..\..\game_framework\source\framework\core\MemoryHandler" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "_DEBUG" /D "RWDEBUG" /D "DEBUG" /D "RWS_TRACE_METRICS" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /FD -g /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=snLib.exe
# ADD BASE LIB32 /nologo /D:SN_TARGET_NGC
# ADD LIB32 /nologo /D:SN_TARGET_NGC

!ELSEIF  "$(CFG)" == "LoadingScreen - Win32 NGC DDH DesignRelease Broadband"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoadingScreen___Win32_NGC_DDH_DesignRelease_Broadband"
# PROP BASE Intermediate_Dir "LoadingScreen___Win32_NGC_DDH_DesignRelease_Broadband"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DDH_Design_Release_Broadband"
# PROP Intermediate_Dir "DDH_Design_Release_Broadband"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Od /I "C:\DolphinSDK1.0\include" /I "..\..\..\..\game_framework\source\framework\core\MemoryHandler" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "NDEBUG" /D "NO_RWS_MEM" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "RWS_TRACE_METRICS" /FD -g /c
# ADD CPP /nologo /W3 /Od /I "C:\DolphinSDK1.0\include" /I "..\..\..\..\game_framework\source\framework\core\MemoryHandler" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "NDEBUG" /D "NO_RWS_MEM" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "RWS_TRACE_METRICS" /FD -g /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=snLib.exe
# ADD BASE LIB32 /nologo /D:SN_TARGET_NGC
# ADD LIB32 /nologo /D:SN_TARGET_NGC

!ENDIF 

# Begin Target

# Name "LoadingScreen - Win32 NGC DDH DesignDebug"
# Name "LoadingScreen - Win32 NGC DDH DesignRelease"
# Name "LoadingScreen - Win32 NGC DDH Debug"
# Name "LoadingScreen - Win32 NGC DDH Release"
# Name "LoadingScreen - Win32 NGC GDEV DesignDebug"
# Name "LoadingScreen - Win32 NGC GDEV DesignRelease"
# Name "LoadingScreen - Win32 NGC GDEV Debug"
# Name "LoadingScreen - Win32 NGC GDEV Release"
# Name "LoadingScreen - Win32 NGC GDEV DesignDebug Broadband"
# Name "LoadingScreen - Win32 NGC GDEV DesignRelease Broadband"
# Name "LoadingScreen - Win32 NGC DDH DesignDebug Broadband"
# Name "LoadingScreen - Win32 NGC DDH DesignRelease Broadband"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\CLoadingScreen.cpp
# End Source File
# Begin Source File

SOURCE=..\..\CLoadingScreen.h
# End Source File
# Begin Source File

SOURCE=..\..\splashtxd.h
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\NGC_in_VC.h
# End Source File
# Begin Source File

SOURCE=.\precomp.h
# End Source File
# End Group
# End Target
# End Project
