# Microsoft Developer Studio Project File - Name="MaxSimple" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=MAXSIMPLE - WIN32 DEBUG MAX5 WITH DEBUG OPENEXPORT
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MaxSimple.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MaxSimple.mak" CFG="MAXSIMPLE - WIN32 DEBUG MAX5 WITH DEBUG OPENEXPORT"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MaxSimple - Win32 Release Max5" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "MaxSimple - Win32 Debug Max5" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "MaxSimple - Win32 Debug Max5 with Debug OpenExport" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "MaxSimple"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MaxSimple - Win32 Release Max5"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release_Max5"
# PROP BASE Intermediate_Dir "Release_Max5"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_Max5"
# PROP Intermediate_Dir "Release_Max5"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MAXSIMPLE_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "..\..\include\RwComm" /I "..\..\include\common" /I "..\..\include\RwExpMgr" /I "..\..\include\RwExp" /I "..\..\include\RwUtil" /I "..\..\include\RwRf3Core" /I "..\..\include\RwXML" /I "c:\3dsmax5\maxsdk\include" /I "..\..\STLport-4.6\stlport" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MAXSIMPLE_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 RwExpMgr.lib menus.lib atl.lib bmm.lib RwUtil.lib RwComm.lib core.lib geom.lib mesh.lib maxutil.lib paramblk2.lib particle.lib maxscrpt.lib comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"Release_Max5/MaxSimple.dle" /libpath:"c:\3dsmax5\maxsdk\lib" /libpath:"..\..\lib\\"

!ELSEIF  "$(CFG)" == "MaxSimple - Win32 Debug Max5"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug_Max5"
# PROP BASE Intermediate_Dir "Debug_Max5"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_Max5"
# PROP Intermediate_Dir "Debug_Max5"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MAXSIMPLE_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\..\include\RwComm" /I "..\..\include\common" /I "..\..\include\RwExpMgr" /I "..\..\include\RwExp" /I "..\..\include\RwUtil" /I "..\..\include\RwRf3Core" /I "..\..\include\RwXML" /I "c:\3dsmax5\maxsdk\include" /I "..\..\STLport-4.6\stlport" /D "MAXSIMPLE_EXPORTS" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 RwUtil.lib RwExpMgr.lib menus.lib atl.lib bmm.lib version.lib RwComm.lib core.lib geom.lib mesh.lib maxutil.lib paramblk2.lib particle.lib maxscrpt.lib comctl32.lib user32.lib comdlg32.lib advapi32.lib shell32.lib kernel32.lib gdi32.lib winspool.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"Debug_Max5/MaxSimple.dle" /pdbtype:sept /libpath:"c:\3dsmax5\maxsdk\lib" /libpath:"..\..\lib\\"

!ELSEIF  "$(CFG)" == "MaxSimple - Win32 Debug Max5 with Debug OpenExport"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "MaxSimple___Win32_Debug_Max5_with_Debug_OpenExport"
# PROP BASE Intermediate_Dir "MaxSimple___Win32_Debug_Max5_with_Debug_OpenExport"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_Max5_with_Debug_OpenExport"
# PROP Intermediate_Dir "Debug_Max5_with_Debug_OpenExport"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "c:\3dsmax5\maxsdk\include" /I "..\..\include\RwComm" /I "..\..\include\common" /I "..\..\include\RwExpMgr" /I "..\..\include\RwExp" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MAXSIMPLE_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\..\include\RwComm" /I "..\..\include\common" /I "..\..\include\RwExpMgr" /I "..\..\include\RwExp" /I "..\..\include\RwUtil" /I "..\..\include\RwRf3Core" /I "..\..\include\RwXML" /I "c:\3dsmax5\maxsdk\include" /I "..\..\STLport-4.6\stlport" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MAXSIMPLE_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 RwExpMgrd.lib RwCommd.lib menus.lib atl.lib core.lib geom.lib mesh.lib maxutil.lib paramblk2.lib particle.lib maxscrpt.lib bmm.lib comctl32.lib user32.lib comdlg32.lib version.lib advapi32.lib shell32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"c:/3dsmax5/plugins/rwsimple.dle" /pdbtype:sept /libpath:"c:\3dsmax5\maxsdk\lib" /libpath:"..\..\lib\\"
# ADD LINK32 RwExpMgrd.lib menus.lib atl.lib bmm.lib version.lib RwUtild.lib RwCommd.lib core.lib geom.lib mesh.lib maxutil.lib paramblk2.lib particle.lib maxscrpt.lib comctl32.lib user32.lib comdlg32.lib advapi32.lib shell32.lib kernel32.lib gdi32.lib winspool.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"Debug_Max5_with_Debug_OpenExport/MaxSimple.dle" /pdbtype:sept /libpath:"c:\3dsmax5\maxsdk\lib" /libpath:"..\..\lib\\"

!ENDIF 

# Begin Target

# Name "MaxSimple - Win32 Release Max5"
# Name "MaxSimple - Win32 Debug Max5"
# Name "MaxSimple - Win32 Debug Max5 with Debug OpenExport"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\MaxSimple.cpp
# End Source File
# Begin Source File

SOURCE=.\MaxSimple.def
# End Source File
# Begin Source File

SOURCE=.\MaxSimpleAsset.cpp
# End Source File
# Begin Source File

SOURCE=.\MaxSimpleBuilder.cpp
# End Source File
# Begin Source File

SOURCE=.\MaxSimpleExp.cpp
# End Source File
# Begin Source File

SOURCE=.\MaxSimpleMesh.cpp
# End Source File
# Begin Source File

SOURCE=.\MaxSimpleScene.cpp
# End Source File
# Begin Source File

SOURCE=.\MaxSimpleShader.cpp
# End Source File
# Begin Source File

SOURCE=.\MaxSimpleTransform.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\MaxSimpleAsset.h
# End Source File
# Begin Source File

SOURCE=.\MaxSimpleBuilder.h
# End Source File
# Begin Source File

SOURCE=.\MaxSimpleExp.h
# End Source File
# Begin Source File

SOURCE=.\MaxSimpleMesh.h
# End Source File
# Begin Source File

SOURCE=.\MaxSimpleScene.h
# End Source File
# Begin Source File

SOURCE=.\MaxSimpleShader.h
# End Source File
# Begin Source File

SOURCE=.\MaxSimpleTransform.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
