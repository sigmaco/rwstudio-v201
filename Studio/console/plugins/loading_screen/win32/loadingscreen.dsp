# Microsoft Developer Studio Project File - Name="Loading_Screen" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 60000
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Loading_Screen - Win32 OGL Design Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "LoadingScreen.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "LoadingScreen.mak" CFG="Loading_Screen - Win32 OGL Design Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Loading_Screen - Win32 OGL Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Loading_Screen - Win32 D3D Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Loading_Screen - Win32 OGL Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Loading_Screen - Win32 D3D Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Loading_Screen - Win32 D3D Design Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Loading_Screen - Win32 D3D Design Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Loading_Screen - Win32 OGL Design Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Loading_Screen - Win32 OGL Design Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Loading_Screen - Win32 D3D Design Metrics" (based on "Win32 (x86) Static Library")
!MESSAGE "Loading_Screen - Win32 OGL Design Metrics" (based on "Win32 (x86) Static Library")
!MESSAGE "Loading_Screen - Win32 D3D Metrics" (based on "Win32 (x86) Static Library")
!MESSAGE "Loading_Screen - Win32 OGL Metrics" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "Loading_Screen"
# PROP Scc_LocalPath "."
CPP=snCl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Loading_Screen - Win32 OGL Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Loading_Screen___Win32_OGL_Debug"
# PROP BASE Intermediate_Dir "Loading_Screen___Win32_OGL_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "bin\Win32 OGL Debug\obj"
# PROP Intermediate_Dir "bin\Win32 OGL Debug\obj"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /WX /Gm /GX /ZI /Od /I "$(RWGSDK)\include\opengl" /I "..\..\..\game_framework\source\framework\core\memoryhandler" /I "..\..\source\\" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "WIN32" /D "RW_USE_SPF" /D "RWDEBUG" /Yu"precomp.h" /FD /GZ /Zm300 /c
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=snLib.exe
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"bin\Win32 OGL Debug\LoadingScreen.lib"

!ELSEIF  "$(CFG)" == "Loading_Screen - Win32 D3D Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Loading_Screen___Win32_D3D_Debug"
# PROP BASE Intermediate_Dir "Loading_Screen___Win32_D3D_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "bin\Win32 D3D Debug\obj"
# PROP Intermediate_Dir "bin\Win32 D3D Debug\obj"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /WX /Gm /GX /ZI /Od /I "$(RWGSDK)\include\d3d8" /I "..\..\..\game_framework\source\framework\core\memoryhandler" /I "..\..\source\\" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "WIN32" /D "RW_USE_SPF" /Yu"precomp.h" /FD /GZ /Zm300 /c
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=snLib.exe
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"bin\Win32 D3D Debug\LoadingScreen.lib"

!ELSEIF  "$(CFG)" == "Loading_Screen - Win32 OGL Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Loading_Screen___Win32_OGL_Release"
# PROP BASE Intermediate_Dir "Loading_Screen___Win32_OGL_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "bin\Win32 OGL Release\obj"
# PROP Intermediate_Dir "bin\Win32 OGL Release\obj"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /WX /GX /O2 /I "$(RWGSDK)\include\opengl" /I "..\..\..\game_framework\source\framework\core\memoryhandler" /I "..\..\source\\" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "WIN32" /D "RW_USE_SPF" /Yu"precomp.h" /FD /Zm300 /c
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=snLib.exe
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"bin\Win32 OGL Release\LoadingScreen.lib"

!ELSEIF  "$(CFG)" == "Loading_Screen - Win32 D3D Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Loading_Screen___Win32_D3D_Release"
# PROP BASE Intermediate_Dir "Loading_Screen___Win32_D3D_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "bin\Win32 D3D Release\obj"
# PROP Intermediate_Dir "bin\Win32 D3D Release\obj"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /WX /GX /O2 /I "$(RWGSDK)\include\d3d8" /I "..\..\..\game_framework\source\framework\core\memoryhandler" /I "..\..\source\\" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "WIN32" /D "RW_USE_SPF" /Yu"precomp.h" /FD /Zm300 /c
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=snLib.exe
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"bin\Win32 D3D Release\LoadingScreen.lib"

!ELSEIF  "$(CFG)" == "Loading_Screen - Win32 D3D Design Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Loading_Screen___Win32_D3D_Design_Debug"
# PROP BASE Intermediate_Dir "Loading_Screen___Win32_D3D_Design_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "bin\Win32_D3D_Design_Debug\obj"
# PROP Intermediate_Dir "bin\Win32_D3D_Design_Debug\obj"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "$(RWGSDK)\include\d3d8" /I "..\..\..\game_framework\source\framework\core\memoryhandler" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Yu"precomp.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /WX /Gm /GX /ZI /Od /I "$(RWGSDK)\include\d3d8" /I "..\..\..\game_framework\source\framework\core\memoryhandler" /I "..\..\source\\" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "WIN32" /D "RW_USE_SPF" /D "RWDEBUG" /FR /Yu"precomp.h" /FD /GZ /Zm300 /c
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=snLib.exe
# ADD BASE LIB32 /nologo /out:"bin\Win32 D3D Debug\LoadingScreen.lib"
# ADD LIB32 /nologo /out:"bin\Win32 D3D Debug\LoadingScreen.lib"

!ELSEIF  "$(CFG)" == "Loading_Screen - Win32 D3D Design Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Loading_Screen___Win32_D3D_Design_Release"
# PROP BASE Intermediate_Dir "Loading_Screen___Win32_D3D_Design_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "bin\Win32_D3D_Design_Release\obj"
# PROP Intermediate_Dir "bin\Win32_D3D_Design_Release\obj"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /I "$(RWGSDK)\include\d3d8" /I "..\..\..\game_framework\source\framework\core\memoryhandler" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"precomp.h" /FD /c
# ADD CPP /nologo /MT /W3 /WX /GX /O2 /I "$(RWGSDK)\include\d3d8" /I "..\..\..\game_framework\source\framework\core\memoryhandler" /I "..\..\source\\" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "WIN32" /D "RW_USE_SPF" /Yu"precomp.h" /FD /Zm300 /c
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=snLib.exe
# ADD BASE LIB32 /nologo /out:"bin\Win32 D3D Release\LoadingScreen.lib"
# ADD LIB32 /nologo /out:"bin\Win32 D3D Release\LoadingScreen.lib"

!ELSEIF  "$(CFG)" == "Loading_Screen - Win32 OGL Design Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Loading_Screen___Win32_OGL_Design_Debug"
# PROP BASE Intermediate_Dir "Loading_Screen___Win32_OGL_Design_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "bin\Win32_OGL_Design_Debug\obj"
# PROP Intermediate_Dir "bin\Win32_OGL_Design_Debug\obj"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "$(RWGSDK)\include\opengl" /I "..\..\..\game_framework\source\framework\core\memoryhandler" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Yu"precomp.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /WX /Gm /GX /ZI /Od /I "$(RWGSDK)\include\opengl" /I "..\..\..\game_framework\source\framework\core\memoryhandler" /I "..\..\source\\" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "WIN32" /D "RW_USE_SPF" /D "RWDEBUG" /Yu"precomp.h" /FD /GZ /Zm300 /c
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=snLib.exe
# ADD BASE LIB32 /nologo /out:"bin\Win32 OGL Debug\LoadingScreen.lib"
# ADD LIB32 /nologo /out:"bin\Win32 OGL Debug\LoadingScreen.lib"

!ELSEIF  "$(CFG)" == "Loading_Screen - Win32 OGL Design Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Loading_Screen___Win32_OGL_Design_Release"
# PROP BASE Intermediate_Dir "Loading_Screen___Win32_OGL_Design_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "bin\Win32_OGL_Design_Release\obj"
# PROP Intermediate_Dir "bin\Win32_OGL_Design_Release\obj"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /I "$(RWGSDK)\include\opengl" /I "..\..\..\game_framework\source\framework\core\memoryhandler" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"precomp.h" /FD /c
# ADD CPP /nologo /MT /W3 /WX /GX /O2 /I "$(RWGSDK)\include\opengl" /I "..\..\..\game_framework\source\framework\core\memoryhandler" /I "..\..\source\\" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "WIN32" /D "RW_USE_SPF" /Yu"precomp.h" /FD /Zm300 /c
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=snLib.exe
# ADD BASE LIB32 /nologo /out:"bin\Win32 OGL Release\LoadingScreen.lib"
# ADD LIB32 /nologo /out:"bin\Win32 OGL Release\LoadingScreen.lib"

!ELSEIF  "$(CFG)" == "Loading_Screen - Win32 D3D Design Metrics"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Loading_Screen___Win32_D3D_Design_Metrics"
# PROP BASE Intermediate_Dir "Loading_Screen___Win32_D3D_Design_Metrics"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "bin\Win32_D3D_Design_Metrics\obj"
# PROP Intermediate_Dir "bin\Win32_D3D_Design_Metrics\obj"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /I "$(RWGSDK)\include\d3d8" /I "..\..\..\game_framework\source\framework\core\memoryhandler" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "WIN32" /D "RW_USE_SPF" /Yu"precomp.h" /FD /c
# ADD CPP /nologo /MT /W3 /WX /GX /O2 /I "$(RWGSDK)\include\d3d8" /I "..\..\..\game_framework\source\framework\core\memoryhandler" /I "..\..\source\\" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "WIN32" /D "RW_USE_SPF" /Yu"precomp.h" /FD /Zm300 /c
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=snLib.exe
# ADD BASE LIB32 /nologo /out:"bin\Win32 D3D Release\LoadingScreen.lib"
# ADD LIB32 /nologo /out:"bin\Win32 D3D Release\LoadingScreen.lib"

!ELSEIF  "$(CFG)" == "Loading_Screen - Win32 OGL Design Metrics"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Loading_Screen___Win32_OGL_Design_Metrics"
# PROP BASE Intermediate_Dir "Loading_Screen___Win32_OGL_Design_Metrics"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "bin\Win32_OGL_Design_Metrics\obj"
# PROP Intermediate_Dir "bin\Win32_OGL_Design_Metrics\obj"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /I "$(RWGSDK)\include\opengl" /I "..\..\..\game_framework\source\framework\core\memoryhandler" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "WIN32" /D "RW_USE_SPF" /Yu"precomp.h" /FD /c
# ADD CPP /nologo /MT /W3 /WX /GX /O2 /I "$(RWGSDK)\include\opengl" /I "..\..\..\game_framework\source\framework\core\memoryhandler" /I "..\..\source\\" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "WIN32" /D "RW_USE_SPF" /Yu"precomp.h" /FD /Zm300 /c
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=snLib.exe
# ADD BASE LIB32 /nologo /out:"bin\Win32 OGL Release\LoadingScreen.lib"
# ADD LIB32 /nologo /out:"bin\Win32 OGL Release\LoadingScreen.lib"

!ELSEIF  "$(CFG)" == "Loading_Screen - Win32 D3D Metrics"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Loading_Screen___Win32_D3D_Metrics"
# PROP BASE Intermediate_Dir "Loading_Screen___Win32_D3D_Metrics"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "bin\Win32_D3D_Metrics\obj"
# PROP Intermediate_Dir "bin\Win32_D3D_Metrics\obj"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /I "$(RWGSDK)\include\d3d8" /I "..\..\..\game_framework\source\framework\core\memoryhandler" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "WIN32" /D "RW_USE_SPF" /Yu"precomp.h" /FD /c
# ADD CPP /nologo /MT /W3 /WX /GX /O2 /I "$(RWGSDK)\include\d3d8" /I "..\..\..\game_framework\source\framework\core\memoryhandler" /I "..\..\source\\" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "WIN32" /D "RW_USE_SPF" /Yu"precomp.h" /FD /Zm300 /c
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=snLib.exe
# ADD BASE LIB32 /nologo /out:"bin\Win32 D3D Release\LoadingScreen.lib"
# ADD LIB32 /nologo /out:"bin\Win32 D3D Release\LoadingScreen.lib"

!ELSEIF  "$(CFG)" == "Loading_Screen - Win32 OGL Metrics"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Loading_Screen___Win32_OGL_Metrics"
# PROP BASE Intermediate_Dir "Loading_Screen___Win32_OGL_Metrics"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "bin\Win32_OGL_Metrics\obj"
# PROP Intermediate_Dir "bin\Win32_OGL_Metrics\obj"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /I "$(RWGSDK)\include\opengl" /I "..\..\..\game_framework\source\framework\core\memoryhandler" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "WIN32" /D "RW_USE_SPF" /Yu"precomp.h" /FD /c
# ADD CPP /nologo /MT /W3 /WX /GX /O2 /I "$(RWGSDK)\include\opengl" /I "..\..\..\game_framework\source\framework\core\memoryhandler" /I "..\..\source\\" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "WIN32" /D "RW_USE_SPF" /Yu"precomp.h" /FD /Zm300 /c
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=snLib.exe
# ADD BASE LIB32 /nologo /out:"bin\Win32 OGL Release\LoadingScreen.lib"
# ADD LIB32 /nologo /out:"bin\Win32 OGL Release\LoadingScreen.lib"

!ENDIF 

# Begin Target

# Name "Loading_Screen - Win32 OGL Debug"
# Name "Loading_Screen - Win32 D3D Debug"
# Name "Loading_Screen - Win32 OGL Release"
# Name "Loading_Screen - Win32 D3D Release"
# Name "Loading_Screen - Win32 D3D Design Debug"
# Name "Loading_Screen - Win32 D3D Design Release"
# Name "Loading_Screen - Win32 OGL Design Debug"
# Name "Loading_Screen - Win32 OGL Design Release"
# Name "Loading_Screen - Win32 D3D Design Metrics"
# Name "Loading_Screen - Win32 OGL Design Metrics"
# Name "Loading_Screen - Win32 D3D Metrics"
# Name "Loading_Screen - Win32 OGL Metrics"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\CLoadingScreen.cpp

!IF  "$(CFG)" == "Loading_Screen - Win32 OGL Debug"

!ELSEIF  "$(CFG)" == "Loading_Screen - Win32 D3D Debug"

# ADD CPP /Yu"precomp.h"

!ELSEIF  "$(CFG)" == "Loading_Screen - Win32 OGL Release"

!ELSEIF  "$(CFG)" == "Loading_Screen - Win32 D3D Release"

!ELSEIF  "$(CFG)" == "Loading_Screen - Win32 D3D Design Debug"

# ADD BASE CPP /Yu"precomp.h"
# ADD CPP /Yu"precomp.h"

!ELSEIF  "$(CFG)" == "Loading_Screen - Win32 D3D Design Release"

!ELSEIF  "$(CFG)" == "Loading_Screen - Win32 OGL Design Debug"

!ELSEIF  "$(CFG)" == "Loading_Screen - Win32 OGL Design Release"

!ELSEIF  "$(CFG)" == "Loading_Screen - Win32 D3D Design Metrics"

!ELSEIF  "$(CFG)" == "Loading_Screen - Win32 OGL Design Metrics"

!ELSEIF  "$(CFG)" == "Loading_Screen - Win32 D3D Metrics"

!ELSEIF  "$(CFG)" == "Loading_Screen - Win32 OGL Metrics"

!ENDIF 

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

SOURCE=..\splashtxd.h
# End Source File
# End Group
# End Target
# End Project
