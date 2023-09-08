# Microsoft Developer Studio Project File - Name="RWSAssetList" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 60000
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=RWSAssetList - Win32 Release Unicode
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "RWSAssetList.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RWSAssetList.mak" CFG="RWSAssetList - Win32 Release Unicode"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RWSAssetList - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "RWSAssetList - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "RWSAssetList - Win32 Debug Unicode" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "RWSAssetList - Win32 Release Unicode" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RWSAssetList - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /O2 /I "$(RENDERWARESTUDIO)/API/inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_ATL_STATIC_REGISTRY" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib RWS.lib  /nologo /subsystem:windows /dll /machine:I386 /libpath:"$(RENDERWARESTUDIO)\API\lib"
# Begin Custom Build - Performing registration
OutDir=.\Release
TargetPath=.\Release\RWSAssetList.dll
InputPath=.\Release\RWSAssetList.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if "%OS%"=="" goto NOTNT, if not "%OS%"=="Windows_NT" goto NOTNT, regsvr32 /s /c "$(TargetPath)", echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg",goto end, :NOTNT ,echo Warning : Cannot register Unicode DLL on Windows 95,:end

# End Custom Build

!ELSEIF  "$(CFG)" == "RWSAssetList - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ    /c
# ADD CPP /nologo /MTd /W3 /Gm /ZI /Od /I "$(RENDERWARESTUDIO)/API/inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ    /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib RWS.lib  /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept /libpath:"$(RENDERWARESTUDIO)\API\lib"
# Begin Custom Build - Performing registration
OutDir=.\Debug
TargetPath=.\Debug\RWSAssetList.dll
InputPath=.\Debug\RWSAssetList.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if "%OS%"=="" goto NOTNT, if not "%OS%"=="Windows_NT" goto NOTNT, regsvr32 /s /c "$(TargetPath)", echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg",goto end, :NOTNT ,echo Warning : Cannot register Unicode DLL on Windows 95,:end

# End Custom Build

!ELSEIF  "$(CFG)" == "RWSAssetList - Win32 Debug Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "RWSAssetList___Win32_Debug_Unicode"
# PROP BASE Intermediate_Dir "RWSAssetList___Win32_Debug_Unicode"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "RWSAssetList___Win32_Debug_Unicode"
# PROP Intermediate_Dir "RWSAssetList___Win32_Debug_Unicode"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /ZI /Od /I "$(RENDERWARESTUDIO)/API/inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ  /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib RWSud.lib  /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept /libpath:"$(RENDERWARESTUDIO)\API\lib"
# Begin Custom Build - Performing registration
OutDir=.\RWSAssetList___Win32_Debug_Unicode
TargetPath=.\RWSAssetList___Win32_Debug_Unicode\RWSAssetList.dll
InputPath=.\RWSAssetList___Win32_Debug_Unicode\RWSAssetList.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if "%OS%"=="" goto NOTNT, if not "%OS%"=="Windows_NT" goto NOTNT, regsvr32 /s /c "$(TargetPath)", echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg",goto end, :NOTNT ,echo Warning : Cannot register Unicode DLL on Windows 95,:end

# End Custom Build

!ELSEIF  "$(CFG)" == "RWSAssetList - Win32 Release Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "RWSAssetList___Win32_Release_Unicode"
# PROP BASE Intermediate_Dir "RWSAssetList___Win32_Release_Unicode"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "RWSAssetList___Win32_Release_Unicode"
# PROP Intermediate_Dir "RWSAssetList___Win32_Release_Unicode"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /GZ /c
# ADD CPP /nologo /MT /W3 /O2 /I "$(RENDERWARESTUDIO)/API/inc" /D "WIN32" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /D "_USRDLL" /D "NDEBUG" /D "_ATL_STATIC_REGISTRY" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /D "_DEBUG"
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib RWSu.lib  /nologo /subsystem:windows /dll /incremental:no /debug /machine:I386 /libpath:"$(RENDERWARESTUDIO)\API\lib"
# Begin Custom Build - Performing registration
OutDir=.\RWSAssetList___Win32_Release_Unicode
TargetPath=.\RWSAssetList___Win32_Release_Unicode\RWSAssetList.dll
InputPath=.\RWSAssetList___Win32_Release_Unicode\RWSAssetList.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if "%OS%"=="" goto NOTNT, if not "%OS%"=="Windows_NT" goto NOTNT, regsvr32 /s /c "$(TargetPath)", echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg",goto end, :NOTNT ,echo Warning : Cannot register Unicode DLL on Windows 95,:end

# End Custom Build

!ENDIF 

# Begin Target

# Name "RWSAssetList - Win32 Release"
# Name "RWSAssetList - Win32 Debug"
# Name "RWSAssetList - Win32 Debug Unicode"
# Name "RWSAssetList - Win32 Release Unicode"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\RWSAssetList.cpp
# End Source File
# Begin Source File

SOURCE=.\RWSAssetList.def
# End Source File
# Begin Source File

SOURCE=.\RWSAssetList.rc
# End Source File
# Begin Source File

SOURCE=.\RWSAssetListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Utility.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AtlCtrls.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\RWSAssetListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\RWSAttribEdit.h
# End Source File
# Begin Source File

SOURCE=.\RWSAttribEditIF.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Utility.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\RWSAssetListCtrl.bmp
# End Source File
# Begin Source File

SOURCE=.\RWSAssetListCtrl.rgs
# End Source File
# End Group
# End Target
# End Project
