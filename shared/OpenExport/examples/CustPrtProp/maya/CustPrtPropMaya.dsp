# Microsoft Developer Studio Project File - Name="CustPrtPropMaya" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=CustPrtPropMaya - Win32 Debug Maya 4_5 with Debug OpenExport
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CustPrtPropMaya.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CustPrtPropMaya.mak" CFG="CustPrtPropMaya - Win32 Debug Maya 4_5 with Debug OpenExport"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CustPrtPropMaya - Win32 Debug Maya 5" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CustPrtPropMaya - Win32 Release Maya 5" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CustPrtPropMaya - Win32 Debug Maya 4_5" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CustPrtPropMaya - Win32 Release Maya 4_5" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CustPrtPropMaya - Win32 Debug Maya 5 with Debug OpenExport" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CustPrtPropMaya - Win32 Debug Maya 4_5 with Debug OpenExport" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "Perforce Project"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CustPrtPropMaya - Win32 Debug Maya 5"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "CustPrtPropMaya___Win32_Debug_Maya_4"
# PROP BASE Intermediate_Dir "CustPrtPropMaya___Win32_Debug_Maya_4"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_Maya5"
# PROP Intermediate_Dir "Debug_Maya5"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\common" /I "C:\AW\Maya4.0\include" /I "..\..\..\include\common" /I "..\..\..\include\RwMaya" /I "..\..\..\include\RwXML" /I "..\..\..\include\RwRf3" /I "..\..\..\include\RwMI" /I "..\..\..\include\RwComm" /I "..\..\..\include\RwExp" /I "$(RWGSDK)\include\null" /D "MAYA_40" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CUSTPRTPROP_MAYA_EXPORTS" /D "NT_PLUGIN" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\..\..\STLport-4.6\stlport" /I "..\common" /I "C:\Program Files\AliasWavefront\Maya5.0\include" /I "..\..\..\include\common" /I "..\..\..\include\RwMaya" /I "..\..\..\include\RwXML" /I "..\..\..\include\RwRf3" /I "..\..\..\include\RwMI" /I "..\..\..\include\RwComm" /I "..\..\..\include\RwExp" /I "$(RWGSDK)\include\null" /I "..\..\..\include\RwUtil" /D "MAYA_50" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CUSTPRTPROP_MAYA_EXPORTS" /D "NT_PLUGIN" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 opengl32.lib Foundation.lib OpenMayaFX.lib OpenMaya.lib OpenMayaUI.lib glu32.lib OpenMayaAnim.lib CustPrtPropComm.lib RwMaya4d.lib RwMaya4Prtd.lib RwCommd.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"$(RWGDIR)/export/bin/OpenExportPlugins/CustPrtPropMaya.dll" /pdbtype:sept /libpath:"$(RWGDIR)/export/lib" /libpath:"../lib" /libpath:"c:\AW\Maya4.0\lib"
# ADD LINK32 RwUtil.lib opengl32.lib Foundation.lib OpenMayaFX.lib OpenMaya.lib OpenMayaUI.lib glu32.lib OpenMayaAnim.lib CustPrtPropComm.lib RwMaya5.lib RwMaya5Prt.lib RwComm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept /libpath:"../../../lib" /libpath:"C:\Program Files\AliasWavefront\Maya5.0\lib" /libpath:"../common/Debug"

!ELSEIF  "$(CFG)" == "CustPrtPropMaya - Win32 Release Maya 5"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "CustPrtPropMaya___Win32_Release_Maya_4"
# PROP BASE Intermediate_Dir "CustPrtPropMaya___Win32_Release_Maya_4"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_Maya5"
# PROP Intermediate_Dir "Release_Maya5"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "..\common" /I "C:\AW\Maya4.0\include" /I "..\..\..\include\common" /I "..\..\..\include\RwMaya" /I "..\..\..\include\RwXML" /I "..\..\..\include\RwRf3" /I "..\..\..\include\RwMI" /I "..\..\..\include\RwComm" /I "..\..\..\include\RwExp" /I "$(RWGSDK)\include\null" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CUSTPRTPROP_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "..\..\..\STLport-4.6\stlport" /I "..\common" /I "C:\Program Files\AliasWavefront\Maya5.0\include" /I "..\..\..\include\common" /I "..\..\..\include\RwMaya" /I "..\..\..\include\RwXML" /I "..\..\..\include\RwRf3" /I "..\..\..\include\RwMI" /I "..\..\..\include\RwComm" /I "..\..\..\include\RwExp" /I "$(RWGSDK)\include\null" /I "..\..\..\include\RwUtil" /D "MAYA_50" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CUSTPRTPROP_MAYA_EXPORTS" /D "NT_PLUGIN" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 opengl32.lib Foundation.lib OpenMayaFX.lib OpenMaya.lib OpenMayaUI.lib glu32.lib OpenMayaAnim.lib CustPrtPropComm.lib RwMaya4.lib RwMaya4Prt.lib RwComm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /libpath:"$(RWGDIR)/export/lib" /libpath:"../lib" /libpath:"c:\AW\Maya4.0\lib"
# ADD LINK32 RwUtil.lib opengl32.lib Foundation.lib OpenMayaFX.lib OpenMaya.lib OpenMayaUI.lib glu32.lib OpenMayaAnim.lib CustPrtPropComm.lib RwMaya5.lib RwMaya5Prt.lib RwComm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /libpath:"../../../lib" /libpath:"C:\Program Files\AliasWavefront\Maya5.0\lib" /libpath:"../common/Release"

!ELSEIF  "$(CFG)" == "CustPrtPropMaya - Win32 Debug Maya 4_5"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "CustPrtPropMaya___Win32_Debug_Maya_4_5"
# PROP BASE Intermediate_Dir "CustPrtPropMaya___Win32_Debug_Maya_4_5"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_Maya45"
# PROP Intermediate_Dir "Debug_Maya45"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\common" /I "C:\AW\Maya4.0\include" /I "..\..\..\include\common" /I "..\..\..\include\RwMaya" /I "..\..\..\include\RwXML" /I "..\..\..\include\RwRf3" /I "..\..\..\include\RwMI" /I "..\..\..\include\RwComm" /I "..\..\..\include\RwExp" /I "$(RWGSDK)\include\null" /D "MAYA_40" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CUSTPRTPROP_MAYA_EXPORTS" /D "NT_PLUGIN" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\..\..\STLport-4.6\stlport" /I "C:\Program Files\AliasWavefront\Maya4.5\include" /I "..\common" /I "..\..\..\include\common" /I "..\..\..\include\RwMaya" /I "..\..\..\include\RwXML" /I "..\..\..\include\RwRf3" /I "..\..\..\include\RwMI" /I "..\..\..\include\RwComm" /I "..\..\..\include\RwExp" /I "$(RWGSDK)\include\null" /I "..\..\..\include\RwUtil" /D "MAYA_45" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CUSTPRTPROP_MAYA_EXPORTS" /D "NT_PLUGIN" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 opengl32.lib Foundation.lib OpenMayaFX.lib OpenMaya.lib OpenMayaUI.lib glu32.lib OpenMayaAnim.lib CustPrtPropComm.lib RwMaya4d.lib RwMaya4Prtd.lib RwCommd.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"$(RWGDIR)/export/bin/OpenExportPlugins/CustPrtPropMaya.dll" /pdbtype:sept /libpath:"$(RWGDIR)/export/lib" /libpath:"../lib" /libpath:"c:\AW\Maya4.0\lib"
# ADD LINK32 RwUtil.lib opengl32.lib Foundation.lib OpenMayaFX.lib OpenMaya.lib OpenMayaUI.lib glu32.lib OpenMayaAnim.lib CustPrtPropComm.lib RwMaya45.lib RwMaya45Prt.lib RwComm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept /libpath:"../../../lib" /libpath:"C:\Program Files\AliasWavefront\Maya4.5\lib" /libpath:"../common/Debug"

!ELSEIF  "$(CFG)" == "CustPrtPropMaya - Win32 Release Maya 4_5"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "CustPrtPropMaya___Win32_Release_Maya_4_5"
# PROP BASE Intermediate_Dir "CustPrtPropMaya___Win32_Release_Maya_4_5"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_Maya45"
# PROP Intermediate_Dir "Release_Maya45"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "..\common" /I "C:\AW\Maya4.0\include" /I "..\..\..\include\common" /I "..\..\..\include\RwMaya" /I "..\..\..\include\RwXML" /I "..\..\..\include\RwRf3" /I "..\..\..\include\RwMI" /I "..\..\..\include\RwComm" /I "..\..\..\include\RwExp" /I "$(RWGSDK)\include\null" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CUSTPRTPROP_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "..\..\..\STLport-4.6\stlport" /I "C:\Program Files\AliasWavefront\Maya4.5\include" /I "..\common" /I "..\..\..\include\common" /I "..\..\..\include\RwMaya" /I "..\..\..\include\RwXML" /I "..\..\..\include\RwRf3" /I "..\..\..\include\RwMI" /I "..\..\..\include\RwComm" /I "..\..\..\include\RwExp" /I "$(RWGSDK)\include\null" /I "..\..\..\include\RwUtil" /D "MAYA_45" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CUSTPRTPROP_MAYA_EXPORTS" /D "NT_PLUGIN" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 opengl32.lib Foundation.lib OpenMayaFX.lib OpenMaya.lib OpenMayaUI.lib glu32.lib OpenMayaAnim.lib CustPrtPropComm.lib RwMaya4.lib RwMaya4Prt.lib RwComm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /libpath:"$(RWGDIR)/export/lib" /libpath:"../lib" /libpath:"c:\AW\Maya4.0\lib"
# ADD LINK32 RwUtil.lib opengl32.lib Foundation.lib OpenMayaFX.lib OpenMaya.lib OpenMayaUI.lib glu32.lib OpenMayaAnim.lib CustPrtPropComm.lib RwMaya45.lib RwMaya45Prt.lib RwComm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /libpath:"../../../lib" /libpath:"C:\Program Files\AliasWavefront\Maya4.5\lib" /libpath:"../common/Release"

!ELSEIF  "$(CFG)" == "CustPrtPropMaya - Win32 Debug Maya 5 with Debug OpenExport"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "CustPrtPropMaya___Win32_Debug_Maya_5_with_Debug_OpenExport"
# PROP BASE Intermediate_Dir "CustPrtPropMaya___Win32_Debug_Maya_5_with_Debug_OpenExport"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_Maya_5_with_Debug_OpenExport"
# PROP Intermediate_Dir "Debug_Maya_5_with_Debug_OpenExport"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "$(STLPORTDIR)" /I "..\common" /I "C:\Program Files\AliasWavefront\Maya5.0\include" /I "..\..\..\include\common" /I "..\..\..\include\RwMaya" /I "..\..\..\include\RwXML" /I "..\..\..\include\RwRf3" /I "..\..\..\include\RwMI" /I "..\..\..\include\RwComm" /I "..\..\..\include\RwExp" /I "..\..\..\..\rwsdk\include\null" /I "..\..\..\include\RwUtil" /D "MAYA_50" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CUSTPRTPROP_MAYA_EXPORTS" /D "NT_PLUGIN" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\..\..\STLport-4.6\stlport" /I "..\common" /I "C:\Program Files\AliasWavefront\Maya5.0\include" /I "..\..\..\include\common" /I "..\..\..\include\RwMaya" /I "..\..\..\include\RwXML" /I "..\..\..\include\RwRf3" /I "..\..\..\include\RwMI" /I "..\..\..\include\RwComm" /I "..\..\..\include\RwExp" /I "$(RWGSDK)\include\null" /I "..\..\..\include\RwUtil" /D "MAYA_50" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CUSTPRTPROP_MAYA_EXPORTS" /D "NT_PLUGIN" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 RwUtil.lib opengl32.lib Foundation.lib OpenMayaFX.lib OpenMaya.lib OpenMayaUI.lib glu32.lib OpenMayaAnim.lib CustPrtPropComm.lib RwMaya5.lib RwMaya5Prt.lib RwComm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"C:\Program Files\AliasWavefront\Maya5.0\bin\plug-ins\OpenExportPlugins\CustPrtPropMaya.dll" /pdbtype:sept /libpath:"../../../lib" /libpath:"../lib" /libpath:"C:\Program Files\AliasWavefront\Maya5.0\lib"
# ADD LINK32 RwUtild.lib opengl32.lib Foundation.lib OpenMayaFX.lib OpenMaya.lib OpenMayaUI.lib glu32.lib OpenMayaAnim.lib CustPrtPropComm.lib RwMaya5d.lib RwMaya5Prtd.lib RwCommd.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept /libpath:"../../../lib" /libpath:"C:\Program Files\AliasWavefront\Maya5.0\lib" /libpath:"../common/Debug_with_Debug_OpenExport"

!ELSEIF  "$(CFG)" == "CustPrtPropMaya - Win32 Debug Maya 4_5 with Debug OpenExport"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "CustPrtPropMaya___Win32_Debug_Maya_4_5_with_Debug_OpenExport"
# PROP BASE Intermediate_Dir "CustPrtPropMaya___Win32_Debug_Maya_4_5_with_Debug_OpenExport"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_Maya_4_5_with_Debug_OpenExport"
# PROP Intermediate_Dir "Debug_Maya_4_5_with_Debug_OpenExport"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "$(STLPORTDIR)" /I "C:\Program Files\AliasWavefront\Maya4.5\include" /I "..\common" /I "..\..\..\include\common" /I "..\..\..\include\RwMaya" /I "..\..\..\include\RwXML" /I "..\..\..\include\RwRf3" /I "..\..\..\include\RwMI" /I "..\..\..\include\RwComm" /I "..\..\..\include\RwExp" /I "..\..\..\..\rwsdk\include\null" /I "..\..\..\include\RwUtil" /D "MAYA_45" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CUSTPRTPROP_MAYA_EXPORTS" /D "NT_PLUGIN" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\..\..\STLport-4.6\stlport" /I "C:\Program Files\AliasWavefront\Maya4.5\include" /I "..\common" /I "..\..\..\include\common" /I "..\..\..\include\RwMaya" /I "..\..\..\include\RwXML" /I "..\..\..\include\RwRf3" /I "..\..\..\include\RwMI" /I "..\..\..\include\RwComm" /I "..\..\..\include\RwExp" /I "$(RWGSDK)\include\null" /I "..\..\..\include\RwUtil" /D "MAYA_45" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CUSTPRTPROP_MAYA_EXPORTS" /D "NT_PLUGIN" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 RwUtil.lib opengl32.lib Foundation.lib OpenMayaFX.lib OpenMaya.lib OpenMayaUI.lib glu32.lib OpenMayaAnim.lib CustPrtPropComm.lib RwMaya45.lib RwMaya45Prt.lib RwComm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"C:\Program Files\AliasWavefront\Maya4.5\bin\plug-ins\OpenExportPlugins\CustPrtPropMaya.dll" /pdbtype:sept /libpath:"../../../lib" /libpath:"../lib" /libpath:"C:\Program Files\AliasWavefront\Maya4.5\lib"
# ADD LINK32 RwUtild.lib opengl32.lib Foundation.lib OpenMayaFX.lib OpenMaya.lib OpenMayaUI.lib glu32.lib OpenMayaAnim.lib CustPrtPropComm.lib RwMaya45d.lib RwMaya45Prtd.lib RwCommd.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept /libpath:"../../../lib" /libpath:"C:\Program Files\AliasWavefront\Maya4.5\lib" /libpath:"../common/Debug_with_Debug_OpenExport"

!ENDIF 

# Begin Target

# Name "CustPrtPropMaya - Win32 Debug Maya 5"
# Name "CustPrtPropMaya - Win32 Release Maya 5"
# Name "CustPrtPropMaya - Win32 Debug Maya 4_5"
# Name "CustPrtPropMaya - Win32 Release Maya 4_5"
# Name "CustPrtPropMaya - Win32 Debug Maya 5 with Debug OpenExport"
# Name "CustPrtPropMaya - Win32 Debug Maya 4_5 with Debug OpenExport"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CustPrtPropMaya.cpp
# End Source File
# Begin Source File

SOURCE=.\CustPrtPropMaya.def
# End Source File
# Begin Source File

SOURCE=.\MyPrtMayaPrtSys.cpp
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

SOURCE=.\CustPrtPropMaya.h
# End Source File
# Begin Source File

SOURCE=.\MyPrtMayaPrtSys.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
