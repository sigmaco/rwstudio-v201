# Microsoft Developer Studio Project File - Name="gfCore" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 60000
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=gfCore - Win32 NGC DDH DesignRelease Broadband
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "gfCore.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "gfCore.mak" CFG="gfCore - Win32 NGC DDH DesignRelease Broadband"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "gfCore - Win32 NGC DDH DesignDebug" (based on "Win32 (x86) Static Library")
!MESSAGE "gfCore - Win32 NGC DDH DesignRelease" (based on "Win32 (x86) Static Library")
!MESSAGE "gfCore - Win32 NGC DDH Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "gfCore - Win32 NGC DDH Release" (based on "Win32 (x86) Static Library")
!MESSAGE "gfCore - Win32 NGC GDEV Release" (based on "Win32 (x86) Static Library")
!MESSAGE "gfCore - Win32 NGC GDEV Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "gfCore - Win32 NGC GDEV DesignDebug" (based on "Win32 (x86) Static Library")
!MESSAGE "gfCore - Win32 NGC GDEV DesignRelease" (based on "Win32 (x86) Static Library")
!MESSAGE "gfCore - Win32 NGC GDEV DesignDebug Broadband" (based on "Win32 (x86) Static Library")
!MESSAGE "gfCore - Win32 NGC GDEV DesignRelease Broadband" (based on "Win32 (x86) Static Library")
!MESSAGE "gfCore - Win32 NGC DDH DesignDebug Broadband" (based on "Win32 (x86) Static Library")
!MESSAGE "gfCore - Win32 NGC DDH DesignRelease Broadband" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "gfCore"
# PROP Scc_LocalPath "."
CPP=snCl.exe
RSC=rc.exe

!IF  "$(CFG)" == "gfCore - Win32 NGC DDH DesignDebug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "gfCore___Win32_NGC_DDH_DesignDebug"
# PROP BASE Intermediate_Dir "gfCore___Win32_NGC_DDH_DesignDebug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DDH_Design_Debug"
# PROP Intermediate_Dir "DDH_Design_Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "../../source/" /D "_DEBUG" /D "RWDEBUG" /D "DEBUG" /D "RWS_TRACE_METRICS" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /FD -g /c
# ADD CPP /nologo /W3 /GX /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "_DEBUG" /D "RWDEBUG" /D "DEBUG" /D "RWS_TRACE_METRICS" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "RWS_MEMORY_PROFILING" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "RWS_FUNCTIONPROFILE" /FD -g /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=snLib.exe
# ADD BASE LIB32 /nologo /D:SN_TARGET_NGC
# ADD LIB32 /nologo /D:SN_TARGET_NGC

!ELSEIF  "$(CFG)" == "gfCore - Win32 NGC DDH DesignRelease"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "gfCore___Win32_NGC_DDH_DesignRelease"
# PROP BASE Intermediate_Dir "gfCore___Win32_NGC_DDH_DesignRelease"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DDH_Design_Release"
# PROP Intermediate_Dir "DDH_Design_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "../../source/" /D "NDEBUG" /D "RWS_DISABLE_MEMORY_CHECKING" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "RWS_TRACE_METRICS" /FD -g /c
# ADD CPP /nologo /W3 /GX /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "NDEBUG" /D "RWS_DISABLE_MEMORY_CHECKING" /D "RWS_TRACE_METRICS" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "RWS_MEMORY_PROFILING" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "RWS_FUNCTIONPROFILE" /FD -g /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=snLib.exe
# ADD BASE LIB32 /nologo /D:SN_TARGET_NGC
# ADD LIB32 /nologo /D:SN_TARGET_NGC

!ELSEIF  "$(CFG)" == "gfCore - Win32 NGC DDH Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "gfCore___Win32_NGC_DDH_Debug"
# PROP BASE Intermediate_Dir "gfCore___Win32_NGC_DDH_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DDH_Debug"
# PROP Intermediate_Dir "DDH_Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "../../source/" /D "HW2" /D "_DEBUG" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWDEBUG" /D "RWTERMINAL" /D "DEBUG" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /FD -g /c
# ADD CPP /nologo /W3 /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "_DEBUG" /D "RWDEBUG" /D "DEBUG" /D "RWS_DISABLE_MEMORY_CHECKING" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /FD -g /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=snLib.exe
# ADD BASE LIB32 /nologo /D:SN_TARGET_NGC
# ADD LIB32 /nologo /D:SN_TARGET_NGC

!ELSEIF  "$(CFG)" == "gfCore - Win32 NGC DDH Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "gfCore___Win32_NGC_DDH_Release"
# PROP BASE Intermediate_Dir "gfCore___Win32_NGC_DDH_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DDH_Release"
# PROP Intermediate_Dir "DDH_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "../../source/" /D "RWS_DISABLE_MEMORY_CHECKING" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "NDEBUG" /FD -g /c
# ADD CPP /nologo /W3 /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "NDEBUG" /D "RWS_DISABLE_MEMORY_CHECKING" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /FD -g /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=snLib.exe
# ADD BASE LIB32 /nologo /D:SN_TARGET_NGC
# ADD LIB32 /nologo /D:SN_TARGET_NGC

!ELSEIF  "$(CFG)" == "gfCore - Win32 NGC GDEV Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "gfCore___Win32_NGC_GDEV_Release"
# PROP BASE Intermediate_Dir "gfCore___Win32_NGC_GDEV_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "GDEV_Release"
# PROP Intermediate_Dir "GDEV_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "../../source/" /D "RWS_DISABLE_MEMORY_CHECKING" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "NDEBUG" /FD -g /c
# ADD CPP /nologo /W3 /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "NDEBUG" /D "RWS_DISABLE_MEMORY_CHECKING" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /FD -g /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=snLib.exe
# ADD BASE LIB32 /nologo /D:SN_TARGET_NGC
# ADD LIB32 /nologo /D:SN_TARGET_NGC

!ELSEIF  "$(CFG)" == "gfCore - Win32 NGC GDEV Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "gfCore___Win32_NGC_GDEV_Debug"
# PROP BASE Intermediate_Dir "gfCore___Win32_NGC_GDEV_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "GDEV_Debug"
# PROP Intermediate_Dir "GDEV_Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "../../source/" /D "HW2" /D "_DEBUG" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWDEBUG" /D "RWTERMINAL" /D "DEBUG" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /FD -g /c
# ADD CPP /nologo /W3 /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "_DEBUG" /D "RWDEBUG" /D "DEBUG" /D "RWS_DISABLE_MEMORY_CHECKING" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /FD -g /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=snLib.exe
# ADD BASE LIB32 /nologo /D:SN_TARGET_NGC
# ADD LIB32 /nologo /D:SN_TARGET_NGC

!ELSEIF  "$(CFG)" == "gfCore - Win32 NGC GDEV DesignDebug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "gfCore___Win32_NGC_GDEV_DesignDebug"
# PROP BASE Intermediate_Dir "gfCore___Win32_NGC_GDEV_DesignDebug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "GDEV_Design_Debug"
# PROP Intermediate_Dir "GDEV_Design_Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "../../source/" /D "_DEBUG" /D "RWDEBUG" /D "DEBUG" /D "RWS_TRACE_METRICS" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /FD -g /c
# ADD CPP /nologo /W3 /GX /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "_DEBUG" /D "RWDEBUG" /D "DEBUG" /D "RWS_TRACE_METRICS" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "RWS_MEMORY_PROFILING" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "RWS_FUNCTIONPROFILE" /FD -g /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=snLib.exe
# ADD BASE LIB32 /nologo /D:SN_TARGET_NGC
# ADD LIB32 /nologo /D:SN_TARGET_NGC

!ELSEIF  "$(CFG)" == "gfCore - Win32 NGC GDEV DesignRelease"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "gfCore___Win32_NGC_GDEV_DesignRelease"
# PROP BASE Intermediate_Dir "gfCore___Win32_NGC_GDEV_DesignRelease"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "GDEV_Design_Release"
# PROP Intermediate_Dir "GDEV_Design_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "../../source/" /D "NDEBUG" /D "RWS_DISABLE_MEMORY_CHECKING" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "RWS_TRACE_METRICS" /FD -g /c
# ADD CPP /nologo /W3 /GX /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "NDEBUG" /D "RWS_DISABLE_MEMORY_CHECKING" /D "RWS_TRACE_METRICS" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "RWS_MEMORY_PROFILING" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "RWS_FUNCTIONPROFILE" /FD -g /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=snLib.exe
# ADD BASE LIB32 /nologo /D:SN_TARGET_NGC
# ADD LIB32 /nologo /D:SN_TARGET_NGC

!ELSEIF  "$(CFG)" == "gfCore - Win32 NGC GDEV DesignDebug Broadband"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "gfCore___Win32_NGC_GDEV_DesignDebug_Broadband"
# PROP BASE Intermediate_Dir "gfCore___Win32_NGC_GDEV_DesignDebug_Broadband"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "GDEV_Design_Debug_Broadband"
# PROP Intermediate_Dir "GDEV_Design_Debug_Broadband"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "_DEBUG" /D "RWDEBUG" /D "DEBUG" /D "RWS_TRACE_METRICS" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /FD -g /c
# ADD CPP /nologo /W3 /GX /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "_DEBUG" /D "RWDEBUG" /D "DEBUG" /D "RWS_TRACE_METRICS" /D "RWS_BROADBAND" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "RWS_MEMORY_PROFILING" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "RWS_FUNCTIONPROFILE" /FD -g /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=snLib.exe
# ADD BASE LIB32 /nologo /D:SN_TARGET_NGC
# ADD LIB32 /nologo /D:SN_TARGET_NGC

!ELSEIF  "$(CFG)" == "gfCore - Win32 NGC GDEV DesignRelease Broadband"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "gfCore___Win32_NGC_GDEV_DesignRelease_Broadband"
# PROP BASE Intermediate_Dir "gfCore___Win32_NGC_GDEV_DesignRelease_Broadband"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "GDEV_Design_Release_Broadband"
# PROP Intermediate_Dir "GDEV_Design_Release_Broadband"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "NDEBUG" /D "RWS_DISABLE_MEMORY_CHECKING" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "RWS_TRACE_METRICS" /FD -g /c
# ADD CPP /nologo /W3 /GX /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "NDEBUG" /D "RWS_DISABLE_MEMORY_CHECKING" /D "RWS_TRACE_METRICS" /D "RWS_BROADBAND" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "RWS_MEMORY_PROFILING" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "RWS_FUNCTIONPROFILE" /FD -g /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=snLib.exe
# ADD BASE LIB32 /nologo /D:SN_TARGET_NGC
# ADD LIB32 /nologo /D:SN_TARGET_NGC

!ELSEIF  "$(CFG)" == "gfCore - Win32 NGC DDH DesignDebug Broadband"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "gfCore___Win32_NGC_DDH_DesignDebug_Broadband"
# PROP BASE Intermediate_Dir "gfCore___Win32_NGC_DDH_DesignDebug_Broadband"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DDH_Design_Debug_Broadband"
# PROP Intermediate_Dir "DDH_Design_Debug_Broadband"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "_DEBUG" /D "RWDEBUG" /D "DEBUG" /D "RWS_TRACE_METRICS" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /FD -g /c
# ADD CPP /nologo /W3 /GX /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "_DEBUG" /D "RWDEBUG" /D "DEBUG" /D "RWS_TRACE_METRICS" /D "RWS_BROADBAND" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "RWS_MEMORY_PROFILING" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "RWS_FUNCTIONPROFILE" /FD -g /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=snLib.exe
# ADD BASE LIB32 /nologo /D:SN_TARGET_NGC
# ADD LIB32 /nologo /D:SN_TARGET_NGC

!ELSEIF  "$(CFG)" == "gfCore - Win32 NGC DDH DesignRelease Broadband"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "gfCore___Win32_NGC_DDH_DesignRelease_Broadband"
# PROP BASE Intermediate_Dir "gfCore___Win32_NGC_DDH_DesignRelease_Broadband"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DDH_Design_Release_Broadband"
# PROP Intermediate_Dir "DDH_Design_Release_Broadband"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "NDEBUG" /D "RWS_DISABLE_MEMORY_CHECKING" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "RWS_TRACE_METRICS" /FD -g /c
# ADD CPP /nologo /W3 /GX /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "NDEBUG" /D "RWS_DISABLE_MEMORY_CHECKING" /D "RWS_TRACE_METRICS" /D "RWS_BROADBAND" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "RWS_MEMORY_PROFILING" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "RWS_FUNCTIONPROFILE" /FD -g /c
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

# Name "gfCore - Win32 NGC DDH DesignDebug"
# Name "gfCore - Win32 NGC DDH DesignRelease"
# Name "gfCore - Win32 NGC DDH Debug"
# Name "gfCore - Win32 NGC DDH Release"
# Name "gfCore - Win32 NGC GDEV Release"
# Name "gfCore - Win32 NGC GDEV Debug"
# Name "gfCore - Win32 NGC GDEV DesignDebug"
# Name "gfCore - Win32 NGC GDEV DesignRelease"
# Name "gfCore - Win32 NGC GDEV DesignDebug Broadband"
# Name "gfCore - Win32 NGC GDEV DesignRelease Broadband"
# Name "gfCore - Win32 NGC DDH DesignDebug Broadband"
# Name "gfCore - Win32 NGC DDH DesignRelease Broadband"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "MemoryHandler"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\framework\core\memoryhandler\memoryhandler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\core\memoryhandler\memoryhandler.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\core\memoryhandler\memoryprofile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\core\memoryhandler\memoryprofile.h
# End Source File
# End Group
# Begin Group "NetStream"

# PROP Default_Filter ""
# Begin Group "GCN"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\framework\core\NetStream\GCN\hiodata.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\core\NetStream\GCN\netstrm.cpp
# End Source File
# End Group
# Begin Group "GCN BB"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\framework\core\netstream\gcn\netstrmbb.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\source\framework\core\NetStream\netstrm.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\core\NetStream\nettools.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\core\NetStream\nettools.h
# End Source File
# End Group
# Begin Group "AttributeHandler"

# PROP Default_Filter ""
# Begin Group "Allocation Policies"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\source\framework\core\attributehandler\Allocation Policies\dynamicallocationpolicy.h"
# End Source File
# Begin Source File

SOURCE="..\..\source\framework\core\attributehandler\Allocation Policies\linearallocationpolicy.h"
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\source\framework\core\attributehandler\attributehandlerstrfuncs.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\core\attributehandler\attributemacros.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\core\attributehandler\cattributehandler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\core\attributehandler\cattributehandler.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\core\attributehandler\cclassfactory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\core\attributehandler\cclassfactory.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\core\attributehandler\clinearmemorymanager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\core\attributehandler\clinearmemorymanager.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\core\attributehandler\csystemcommands.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\core\attributehandler\csystemcommands.h
# End Source File
# End Group
# Begin Group "EventHandler"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\framework\core\eventhandler\ceventhandler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\core\eventhandler\ceventhandler.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\core\eventhandler\ceventvisualisation.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\core\eventhandler\ceventvisualisation.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\core\eventhandler\eventhandlerstrfuncs.cpp
# End Source File
# End Group
# Begin Group "Macros"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\framework\core\macros\DebugMacros.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\core\macros\DebugMacros.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\core\macros\DebugOstream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\core\macros\DebugOstream.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\core\macros\IOCoercion.h
# End Source File
# End Group
# Begin Group "ResourceManager"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\framework\core\ResourceManager\CResource.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\core\resourcemanager\cresourcehandler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\core\ResourceManager\CResourceHandler.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\core\resourcemanager\cresourcemanager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\core\resourcemanager\cresourcemanager.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\core\resourcemanager\resourcemanagerstrfunc.cpp
# End Source File
# End Group
# Begin Group "StreamHandler"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\framework\core\streamhandler\streamhandler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\core\streamhandler\streamhandler.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\core\streamhandler\strfunc.h
# End Source File
# End Group
# Begin Group "Misc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\framework\core\misc\idfromstring.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\core\misc\idfromstring.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\core\misc\rwsguid.h
# End Source File
# End Group
# Begin Group "FunctionProfiler"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\framework\core\functionprofiler\profile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\core\functionprofiler\profile.h
# End Source File
# End Group
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
