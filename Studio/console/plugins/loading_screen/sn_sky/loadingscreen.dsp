# Microsoft Developer Studio Project File - Name="LoadingScreen" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 60000
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=LoadingScreen - Win32 PS2 Release CDROM
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "LoadingScreen.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "LoadingScreen.mak" CFG="LoadingScreen - Win32 PS2 Release CDROM"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "LoadingScreen - Win32 PS2 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "LoadingScreen - Win32 PS2 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "LoadingScreen - Win32 PS2 Design Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "LoadingScreen - Win32 PS2 Design Release" (based on "Win32 (x86) Static Library")
!MESSAGE "LoadingScreen - Win32 PS2 Design Metrics" (based on "Win32 (x86) Static Library")
!MESSAGE "LoadingScreen - Win32 PS2 Metrics" (based on "Win32 (x86) Static Library")
!MESSAGE "LoadingScreen - Win32 PS2 Design Release CDROM" (based on "Win32 (x86) Static Library")
!MESSAGE "LoadingScreen - Win32 PS2 Release CDROM" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "LoadingScreen"
# PROP Scc_LocalPath "."
CPP=snCl.exe
RSC=rc.exe

!IF  "$(CFG)" == "LoadingScreen - Win32 PS2 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoadingScreen___Win32_PS2_Debug"
# PROP BASE Intermediate_Dir "LoadingScreen___Win32_PS2_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "bin\Win32_PS2_Debug\obj"
# PROP Intermediate_Dir "bin\Win32_PS2_Debug\obj"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /w /W0 /Od /I "$(RWGSDK)\include\sky2" /I "..\..\..\game_framework\source\framework\core\MemoryHandler" /D "SN_TARGET_PS2" /Fo"PS2_EE_Debug/" /FD /debug /Zm200 /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /W4 /Od /I "$(RWGSDK)\include\sky2" /I "..\..\..\game_framework\source\framework\core\MemoryHandler" /D "SN_TARGET_PS2" /Fo"PS2_EE_Debug/" /FD /debug /Zm200 /c
# SUBTRACT CPP /WX /YX /Yc /Yu
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=snLib.exe
# ADD BASE LIB32 /nologo /out:"bin\Win32_PS2_EE_Debug\LoadingScreen.lib" /D:SN_TARGET_PS2
# ADD LIB32 /nologo /out:"bin\Win32_PS2_EE_Debug\LoadingScreen.lib" /D:SN_TARGET_PS2

!ELSEIF  "$(CFG)" == "LoadingScreen - Win32 PS2 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoadingScreen___Win32_PS2_Release"
# PROP BASE Intermediate_Dir "LoadingScreen___Win32_PS2_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "bin\Win32_PS2_Release\obj"
# PROP Intermediate_Dir "bin\Win32_PS2_Release\obj"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /w /W0 /O2 /I "$(RWGSDK)\include\sky2" /I "..\..\..\game_framework\source\framework\core\MemoryHandler" /D "SN_TARGET_PS2" /D "NDEBUG" /Fo"PS2_EE_Release/" /FD /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /W4 /O2 /I "$(RWGSDK)\include\sky2" /I "..\..\..\game_framework\source\framework\core\MemoryHandler" /D "NDEBUG" /D "SN_TARGET_PS2" /Fo"PS2_EE_Release/" /FD /c
# SUBTRACT CPP /WX /YX /Yc /Yu
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=snLib.exe
# ADD BASE LIB32 /nologo /out:"bin\Win32_PS2_EE_Release\LoadingScreen.lib" /D:SN_TARGET_PS2
# ADD LIB32 /nologo /out:"bin\Win32_PS2_EE_Release\LoadingScreen.lib" /D:SN_TARGET_PS2

!ELSEIF  "$(CFG)" == "LoadingScreen - Win32 PS2 Design Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoadingScreen___Win32_PS2_Design_Debug"
# PROP BASE Intermediate_Dir "LoadingScreen___Win32_PS2_Design_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "bin\Win32_PS2_Design_Debug\obj"
# PROP Intermediate_Dir "bin\Win32_PS2_Design_Debug\obj"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /w /W0 /Od /I "$(RWGSDK)\include\sky2" /I "..\..\..\game_framework\source\framework\core\MemoryHandler" /D "SN_TARGET_PS2" /Fo"PS2_EE_Debug/" /FD /debug /Zm200 /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /W4 /Od /I "$(RWGSDK)\include\sky2" /I "..\..\..\game_framework\source\framework\core\MemoryHandler" /D "SN_TARGET_PS2" /D "RWDEBUG" /Fo"PS2_EE_Debug/" /FD /debug /Zm200 /c
# SUBTRACT CPP /WX /YX /Yc /Yu
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=snLib.exe
# ADD BASE LIB32 /nologo /out:"bin\Win32_PS2_EE_Debug\LoadingScreen.lib" /D:SN_TARGET_PS2
# ADD LIB32 /nologo /out:"bin\Win32_PS2_EE_Debug\LoadingScreen.lib" /D:SN_TARGET_PS2

!ELSEIF  "$(CFG)" == "LoadingScreen - Win32 PS2 Design Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoadingScreen___Win32_PS2_Design_Release"
# PROP BASE Intermediate_Dir "LoadingScreen___Win32_PS2_Design_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "bin\Win32_PS2_Design_Release\obj"
# PROP Intermediate_Dir "bin\Win32_PS2_Design_Release\obj"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /w /W0 /O2 /I "$(RWGSDK)\include\sky2" /I "..\..\..\game_framework\source\framework\core\MemoryHandler" /D "SN_TARGET_PS2" /D "NDEBUG" /Fo"PS2_EE_Release/" /FD /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /W4 /O2 /I "$(RWGSDK)\include\sky2" /I "..\..\..\game_framework\source\framework\core\MemoryHandler" /D "SN_TARGET_PS2" /D "NDEBUG" /Fo"PS2_EE_Release/" /FD /c
# SUBTRACT CPP /WX /YX /Yc /Yu
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=snLib.exe
# ADD BASE LIB32 /nologo /out:"bin\Win32_PS2_EE_Release\LoadingScreen.lib" /D:SN_TARGET_PS2
# ADD LIB32 /nologo /out:"bin\Win32_PS2_EE_Release\LoadingScreen.lib" /D:SN_TARGET_PS2

!ELSEIF  "$(CFG)" == "LoadingScreen - Win32 PS2 Design Metrics"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoadingScreen___Win32_PS2_Design_Metrics"
# PROP BASE Intermediate_Dir "LoadingScreen___Win32_PS2_Design_Metrics"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "bin\Win32_PS2_Design_Metrics\obj"
# PROP Intermediate_Dir "bin\Win32_PS2_Design_Metrics\obj"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /w /W0 /O2 /I "$(RWGSDK)\include\sky2" /I "..\..\..\game_framework\source\framework\core\MemoryHandler" /D "SN_TARGET_PS2" /D "NDEBUG" /Fo"PS2_EE_Release/" /FD /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /W4 /O2 /I "$(RWGSDK)\include\sky2" /I "..\..\..\game_framework\source\framework\core\MemoryHandler" /D "SN_TARGET_PS2" /D "NDEBUG" /Fo"PS2_EE_Release/" /FD /c
# SUBTRACT CPP /WX /YX /Yc /Yu
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=snLib.exe
# ADD BASE LIB32 /nologo /out:"bin\Win32_PS2_EE_Release\LoadingScreen.lib" /D:SN_TARGET_PS2
# ADD LIB32 /nologo /out:"bin\Win32_PS2_EE_Release\LoadingScreen.lib" /D:SN_TARGET_PS2

!ELSEIF  "$(CFG)" == "LoadingScreen - Win32 PS2 Metrics"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoadingScreen___Win32_PS2_Metrics"
# PROP BASE Intermediate_Dir "LoadingScreen___Win32_PS2_Metrics"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "bin\Win32_PS2_Metrics\obj"
# PROP Intermediate_Dir "bin\Win32_PS2_Metrics\obj"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /w /W0 /O2 /I "$(RWGSDK)\include\sky2" /I "..\..\..\game_framework\source\framework\core\MemoryHandler" /D "NDEBUG" /D "SN_TARGET_PS2" /Fo"PS2_EE_Release/" /FD /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /W4 /O2 /I "$(RWGSDK)\include\sky2" /I "..\..\..\game_framework\source\framework\core\MemoryHandler" /D "NDEBUG" /D "SN_TARGET_PS2" /Fo"PS2_EE_Release/" /FD /c
# SUBTRACT CPP /WX /YX /Yc /Yu
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=snLib.exe
# ADD BASE LIB32 /nologo /out:"bin\Win32_PS2_EE_Release\LoadingScreen.lib" /D:SN_TARGET_PS2
# ADD LIB32 /nologo /out:"bin\Win32_PS2_EE_Release\LoadingScreen.lib" /D:SN_TARGET_PS2

!ELSEIF  "$(CFG)" == "LoadingScreen - Win32 PS2 Design Release CDROM"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoadingScreen___Win32_PS2_Design_Release_CDROM"
# PROP BASE Intermediate_Dir "LoadingScreen___Win32_PS2_Design_Release_CDROM"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "bin\Win32_PS2_Design_Release_CDROM\obj"
# PROP Intermediate_Dir "bin\Win32_PS2_Design_Release_CDROM\obj"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /w /W0 /O2 /I "$(RWGSDK)\include\sky2" /I "..\..\..\game_framework\source\framework\core\MemoryHandler" /D "SN_TARGET_PS2" /D "NDEBUG" /Fo"PS2_EE_Release/" /FD /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /W4 /O2 /I "$(RWGSDK)\include\sky2" /I "..\..\..\game_framework\source\framework\core\MemoryHandler" /D "SN_TARGET_PS2" /D "NDEBUG" /D "CDROM" /Fo"PS2_EE_Release/" /FD /c
# SUBTRACT CPP /WX /YX /Yc /Yu
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=snLib.exe
# ADD BASE LIB32 /nologo /out:"bin\Win32_PS2_EE_Release\LoadingScreen.lib" /D:SN_TARGET_PS2
# ADD LIB32 /nologo /out:"bin\Win32_PS2_EE_Release\LoadingScreen.lib" /D:SN_TARGET_PS2

!ELSEIF  "$(CFG)" == "LoadingScreen - Win32 PS2 Release CDROM"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoadingScreen___Win32_PS2_Release_CDROM"
# PROP BASE Intermediate_Dir "LoadingScreen___Win32_PS2_Release_CDROM"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "bin\Win32_PS2_Release_CDROM\obj"
# PROP Intermediate_Dir "bin\Win32_PS2_Release_CDROM\obj"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /w /W0 /O2 /I "$(RWGSDK)\include\sky2" /I "..\..\..\game_framework\source\framework\core\MemoryHandler" /D "NDEBUG" /D "SN_TARGET_PS2" /Fo"PS2_EE_Release/" /FD /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /W4 /O2 /I "$(RWGSDK)\include\sky2" /I "..\..\..\game_framework\source\framework\core\MemoryHandler" /D "NDEBUG" /D "SN_TARGET_PS2" /D "CDROM" /Fo"PS2_EE_Release/" /FD /c
# SUBTRACT CPP /WX /YX /Yc /Yu
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=snLib.exe
# ADD BASE LIB32 /nologo /out:"bin\Win32_PS2_EE_Release\LoadingScreen.lib" /D:SN_TARGET_PS2
# ADD LIB32 /nologo /out:"bin\Win32_PS2_EE_Release\LoadingScreen.lib" /D:SN_TARGET_PS2

!ENDIF 

# Begin Target

# Name "LoadingScreen - Win32 PS2 Debug"
# Name "LoadingScreen - Win32 PS2 Release"
# Name "LoadingScreen - Win32 PS2 Design Debug"
# Name "LoadingScreen - Win32 PS2 Design Release"
# Name "LoadingScreen - Win32 PS2 Design Metrics"
# Name "LoadingScreen - Win32 PS2 Metrics"
# Name "LoadingScreen - Win32 PS2 Design Release CDROM"
# Name "LoadingScreen - Win32 PS2 Release CDROM"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\CLoadingScreen.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\CLoadingScreen.h
# End Source File
# Begin Source File

SOURCE=.\precomp.cpp
# ADD CPP /Yc"precomp.h"
# End Source File
# Begin Source File

SOURCE=.\precomp.h
# End Source File
# Begin Source File

SOURCE=.\PS2_in_VC.h
# End Source File
# Begin Source File

SOURCE=..\splashtxd.h
# End Source File
# End Group
# End Target
# End Project
