# Microsoft Developer Studio Project File - Name="MayaFurShader" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=MayaFurShader - Win32 Debug Maya5 with Debug OpenExport
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MayaFurShader.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MayaFurShader.mak" CFG="MayaFurShader - Win32 Debug Maya5 with Debug OpenExport"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MayaFurShader - Win32 Debug Maya4_5" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "MayaFurShader - Win32 Debug Maya5" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "MayaFurShader - Win32 Release Maya4_5" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "MayaFurShader - Win32 Release Maya5" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "MayaFurShader - Win32 Debug Maya4_5 with Debug OpenExport" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "MayaFurShader - Win32 Debug Maya5 with Debug OpenExport" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "MayaFurShader"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MayaFurShader - Win32 Debug Maya4_5"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "MayaFurShader___Win32_Debug_Maya4_5"
# PROP BASE Intermediate_Dir "MayaFurShader___Win32_Debug_Maya4_5"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_Maya4_5"
# PROP Intermediate_Dir "Debug_Maya4_5"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MAYAFURSHADER_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "C:\Program Files\AliasWavefront\Maya4.5\include" /I "..\..\..\include\RwComm" /I "..\..\..\include\common" /I "..\..\..\include\RwMaya" /I "..\common" /I "..\..\..\include\RwUtil" /I "..\..\..\STLport-4.6\stlport" /I "$(RWGSDK)\include\null" /D "MAYA_45" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MAYAFURSHADER_EXPORTS" /D "NT_PLUGIN" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 RwUtil.lib RwComm.lib RwMaya45.lib OpenMayaAnim.lib Foundation.lib OpenMaya.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept /libpath:"C:\Program Files\AliasWavefront\Maya4.5\lib" /libpath:"..\..\..\lib\\"

!ELSEIF  "$(CFG)" == "MayaFurShader - Win32 Debug Maya5"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "MayaFurShader___Win32_Debug_Maya5"
# PROP BASE Intermediate_Dir "MayaFurShader___Win32_Debug_Maya5"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_Maya5"
# PROP Intermediate_Dir "Debug_Maya5"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "C:\Program Files\AliasWavefront\Maya4.5\include" /I "..\..\..\include\RwComm" /I "..\..\..\include\common" /I "..\..\..\include\RwMaya" /I "..\..\include\RwRf3" /I "..\..\include\RwXML" /I "..\..\include\RwMI" /I "..\..\..\rwsdk\include\null" /I "..\common" /D "MAYA_45" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MAYAFURSHADER_EXPORTS" /D "NT_PLUGIN" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "C:\Program Files\AliasWavefront\Maya5.0\include" /I "..\..\..\include\RwComm" /I "..\..\..\include\common" /I "..\..\..\include\RwMaya" /I "..\common" /I "..\..\..\include\RwUtil" /I "..\..\..\STLport-4.6\stlport" /I "$(RWGSDK)\include\null" /D "MAYA_5" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MAYAFURSHADER_EXPORTS" /D "NT_PLUGIN" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 RwCommd.lib RwExpd.lib RwMaya45d.lib OpenMayaAnim.lib Foundation.lib OpenMaya.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"C:\Program Files\AliasWavefront\Maya4.5\bin\plug-ins\OpenExportPlugins\MayaShader45.dll" /pdbtype:sept /libpath:"C:\Program Files\AliasWavefront\Maya4.5\lib" /libpath:"..\..\..\lib\\"
# ADD LINK32 RwUtil.lib RwComm.lib RwMaya5.lib OpenMayaAnim.lib Foundation.lib OpenMaya.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept /libpath:"C:\Program Files\AliasWavefront\Maya5.0\lib" /libpath:"..\..\..\lib\\"

!ELSEIF  "$(CFG)" == "MayaFurShader - Win32 Release Maya4_5"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "MayaFurShader___Win32_Release_Maya4_5"
# PROP BASE Intermediate_Dir "MayaFurShader___Win32_Release_Maya4_5"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_Maya4_5"
# PROP Intermediate_Dir "Release_Maya4_5"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "C:\Program Files\AliasWavefront\Maya4.5\include" /I "..\..\include\RwComm" /I "..\..\include\common" /I "..\..\include\RwExpMgr" /I "..\..\include\RwExp" /I "..\..\include\RwMaya" /I "..\..\include\RwRf3" /I "..\..\include\RwXML" /I "..\..\include\RwMI" /I "..\..\..\rwsdk\include\null" /D "MAYA_45" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MAYAFURSHADER_EXPORTS" /D "NT_PLUGIN" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "C:\Program Files\AliasWavefront\Maya4.5\include" /I "..\..\..\include\RwComm" /I "..\..\..\include\common" /I "..\..\..\include\RwMaya" /I "..\common" /I "..\..\..\include\RwUtil" /I "..\..\..\STLport-4.6\stlport" /I "$(RWGSDK)\include\null" /D "MAYA_45" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MAYAFURSHADER_EXPORTS" /D "NT_PLUGIN" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 RwUtil.lib RwComm.lib RwMaya45.lib OpenMayaAnim.lib Foundation.lib OpenMaya.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /libpath:"C:\Program Files\AliasWavefront\Maya4.5\lib" /libpath:"..\..\..\lib\\"

!ELSEIF  "$(CFG)" == "MayaFurShader - Win32 Release Maya5"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "MayaFurShader___Win32_Release_Maya5"
# PROP BASE Intermediate_Dir "MayaFurShader___Win32_Release_Maya5"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_Maya5"
# PROP Intermediate_Dir "Release_Maya5"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GR /GX /O2 /I "C:\Program Files\AliasWavefront\Maya4.5\include" /I "..\..\..\include\RwComm" /I "..\..\..\include\common" /I "..\..\..\include\RwMaya" /I "..\common" /D "MAYA_45" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MAYAFURSHADER_EXPORTS" /D "NT_PLUGIN" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "C:\Program Files\AliasWavefront\Maya5.0\include" /I "..\..\..\include\RwComm" /I "..\..\..\include\common" /I "..\..\..\include\RwMaya" /I "..\common" /I "..\..\..\include\RwUtil" /I "..\..\..\STLport-4.6\stlport" /I "$(RWGSDK)\include\null" /D "MAYA_5" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MAYAFURSHADER_EXPORTS" /D "NT_PLUGIN" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 RwComm.lib RwMaya45.lib OpenMayaAnim.lib Foundation.lib OpenMaya.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"C:\Program Files\AliasWavefront\Maya4.5\bin\plug-ins\OpenExportPlugins\MayaShader45.dll" /libpath:"C:\Program Files\AliasWavefront\Maya4.5\lib" /libpath:"..\..\..\lib\\"
# ADD LINK32 RwUtil.lib RwComm.lib RwMaya5.lib OpenMayaAnim.lib Foundation.lib OpenMaya.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /libpath:"C:\Program Files\AliasWavefront\Maya5.0\lib" /libpath:"..\..\..\lib\\"

!ELSEIF  "$(CFG)" == "MayaFurShader - Win32 Debug Maya4_5 with Debug OpenExport"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "MayaFurShader___Win32_Debug_Maya4_5_with_Debug_OpenExport"
# PROP BASE Intermediate_Dir "MayaFurShader___Win32_Debug_Maya4_5_with_Debug_OpenExport"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_Maya4_5_with_Debug_OpenExport"
# PROP Intermediate_Dir "Debug_Maya4_5_with_Debug_OpenExport"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "C:\Program Files\AliasWavefront\Maya4.5\include" /I "..\..\..\include\RwComm" /I "..\..\..\include\common" /I "..\..\..\include\RwMaya" /I "..\common" /D "MAYA_45" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MAYAFURSHADER_EXPORTS" /D "NT_PLUGIN" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "C:\Program Files\AliasWavefront\Maya4.5\include" /I "..\..\..\include\RwComm" /I "..\..\..\include\common" /I "..\..\..\include\RwMaya" /I "..\common" /I "..\..\..\include\RwUtil" /I "..\..\..\STLport-4.6\stlport" /I "$(RWGSDK)\include\null" /D "MAYA_45" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MAYAFURSHADER_EXPORTS" /D "NT_PLUGIN" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 RwCommd.lib RwMaya45d.lib OpenMayaAnim.lib Foundation.lib OpenMaya.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"C:\Program Files\AliasWavefront\Maya4.5\bin\plug-ins\OpenExportPlugins\MayaShader.dll" /pdbtype:sept /libpath:"C:\Program Files\AliasWavefront\Maya4.5\lib" /libpath:"..\..\..\lib\\"
# ADD LINK32 RwUtild.lib RwCommd.lib RwMaya45d.lib OpenMayaAnim.lib Foundation.lib OpenMaya.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept /libpath:"C:\Program Files\AliasWavefront\Maya4.5\lib" /libpath:"..\..\..\lib\\"

!ELSEIF  "$(CFG)" == "MayaFurShader - Win32 Debug Maya5 with Debug OpenExport"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "MayaFurShader___Win32_Debug_Maya5_with_Debug_OpenExport"
# PROP BASE Intermediate_Dir "MayaFurShader___Win32_Debug_Maya5_with_Debug_OpenExport"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_Maya5_with_Debug_OpenExport"
# PROP Intermediate_Dir "Debug_Maya5_with_Debug_OpenExport"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "C:\Program Files\AliasWavefront\Maya5.0\include" /I "..\..\..\include\RwComm" /I "..\..\..\include\common" /I "..\..\..\include\RwMaya" /I "..\common" /D "MAYA_5" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MAYAFURSHADER_EXPORTS" /D "NT_PLUGIN" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "C:\Program Files\AliasWavefront\Maya5.0\include" /I "..\..\..\include\RwComm" /I "..\..\..\include\common" /I "..\..\..\include\RwMaya" /I "..\common" /I "..\..\..\include\RwUtil" /I "..\..\..\STLport-4.6\stlport" /I "$(RWGSDK)\include\null" /D "MAYA_5" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MAYAFURSHADER_EXPORTS" /D "NT_PLUGIN" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 RwCommd.lib RwMaya5d.lib OpenMayaAnim.lib Foundation.lib OpenMaya.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"C:\Program Files\AliasWavefront\Maya5.0\bin\plug-ins\OpenExportPlugins\MayaShader.dll" /pdbtype:sept /libpath:"C:\Program Files\AliasWavefront\Maya5.0\lib" /libpath:"..\..\..\lib\\"
# ADD LINK32 RwUtild.lib RwCommd.lib RwMaya5d.lib OpenMayaAnim.lib Foundation.lib OpenMaya.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept /libpath:"C:\Program Files\AliasWavefront\Maya5.0\lib" /libpath:"..\..\..\lib\\"

!ENDIF 

# Begin Target

# Name "MayaFurShader - Win32 Debug Maya4_5"
# Name "MayaFurShader - Win32 Debug Maya5"
# Name "MayaFurShader - Win32 Release Maya4_5"
# Name "MayaFurShader - Win32 Release Maya5"
# Name "MayaFurShader - Win32 Debug Maya4_5 with Debug OpenExport"
# Name "MayaFurShader - Win32 Debug Maya5 with Debug OpenExport"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\MayaFurShader.cpp
# End Source File
# Begin Source File

SOURCE=.\MayaFurShader.def
# End Source File
# Begin Source File

SOURCE=.\MayaFurShaderPlugin.cpp
# End Source File
# Begin Source File

SOURCE=.\MayaShaderNode.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\MayaFurShader.h
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
