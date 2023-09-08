# Microsoft Developer Studio Project File - Name="CustPrtPropMax" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=CustPrtPropMax - Win32 Debug Max 6 with Debug OpenExport
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CustPrtPropMax.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CustPrtPropMax.mak" CFG="CustPrtPropMax - Win32 Debug Max 6 with Debug OpenExport"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CustPrtPropMax - Win32 Debug Max 5" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CustPrtPropMax - Win32 Release Max 5" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CustPrtPropMax - Win32 Debug Max 5 with Debug OpenExport" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CustPrtPropMax - Win32 Debug Max 6" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CustPrtPropMax - Win32 Release Max 6" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CustPrtPropMax - Win32 Debug Max 6 with Debug OpenExport" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "Perforce Project"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CustPrtPropMax - Win32 Debug Max 5"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "CustPrtPropMax___Win32_Debug_Max_5"
# PROP BASE Intermediate_Dir "CustPrtPropMax___Win32_Debug_Max_5"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_Max5"
# PROP Intermediate_Dir "Debug_Max5"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\common" /I "$(MAX4_INSTALL_DIR)\maxsdk\include" /I "..\..\..\include\common" /I "..\..\..\include\RwMax" /I "..\..\..\include\RwXML" /I "..\..\..\include\RwRf3" /I "..\..\..\include\RwMI" /I "..\..\..\include\RwComm" /I "..\..\..\include\RwExp" /I "$(RWGSDK)\include\null" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CUSTPRTPROP_MAX_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\..\..\STLport-4.6\stlport" /I "C:\3dsmax5\maxsdk\include" /I "..\..\..\include" /I "..\common" /I "..\..\..\include\common" /I "..\..\..\include\RwMax" /I "..\..\..\include\RwXML" /I "..\..\..\include\RwRf3" /I "..\..\..\include\RwMI" /I "..\..\..\include\RwComm" /I "..\..\..\include\RwExp" /I "$(RWGSDK)\include\null" /I "..\..\..\include\RwUtil" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CUSTPRTPROP_MAX_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 CustPrtPropComm.lib RwMax4d.lib RwMax4Prt.lib RwCommd.lib bmm.lib core.lib geom.lib gfx.lib mesh.lib maxutil.lib maxscrpt.lib manipsys.lib paramblk2.lib Particle.lib helpsys.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"$(RWGDIR)/export/bin/OpenExportPlugins/CustPrtPropMax.dll" /pdbtype:sept /libpath:"$(RWGDIR)/export/lib" /libpath:"$(MAX4_INSTALL_DIR)\maxsdk\lib" /libpath:"../lib"
# ADD LINK32 RwUtil.lib RwMax5Prt.lib RwMax5.lib RwComm.lib CustPrtPropComm.lib bmm.lib core.lib geom.lib gfx.lib mesh.lib maxutil.lib maxscrpt.lib manipsys.lib paramblk2.lib Particle.lib helpsys.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept /libpath:"C:\3dsmax5\maxsdk\lib" /libpath:"../../../lib" /libpath:"../common/Debug"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "CustPrtPropMax - Win32 Release Max 5"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "CustPrtPropMax___Win32_Release_Max_5"
# PROP BASE Intermediate_Dir "CustPrtPropMax___Win32_Release_Max_5"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_Max5"
# PROP Intermediate_Dir "Release_Max5"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "$(MAX4_INSTALL_DIR)\maxsdk\include" /I "..\common" /I "..\..\..\include\common" /I "..\..\..\include\RwMax" /I "..\..\..\include\RwXML" /I "..\..\..\include\RwRf3" /I "..\..\..\include\RwMI" /I "..\..\..\include\RwComm" /I "..\..\..\include\RwExp" /I "$(RWGSDK)\include\null" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CUSTPRTPROP_MAX_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "..\..\..\STLport-4.6\stlport" /I "c:\3dsmax5\maxsdk\include" /I "..\..\..\include" /I "..\common" /I "..\..\..\include\common" /I "..\..\..\include\RwMax" /I "..\..\..\include\RwXML" /I "..\..\..\include\RwRf3" /I "..\..\..\include\RwMI" /I "..\..\..\include\RwComm" /I "..\..\..\include\RwExp" /I "$(RWGSDK)\include\null" /I "..\..\..\include\RwUtil" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CUSTPRTPROP_MAX_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 RwMax4Prt.lib RwMax4.lib RwComm.lib CustPrtPropComm.lib bmm.lib core.lib geom.lib gfx.lib mesh.lib maxutil.lib maxscrpt.lib manipsys.lib paramblk2.lib Particle.lib helpsys.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /libpath:"$(RWGDIR)/export/lib" /libpath:"../lib" /libpath:"$(MAX4_INSTALL_DIR)\maxsdk\lib"
# ADD LINK32 RwUtil.lib RwMax5Prt.lib RwMax5.lib RwComm.lib CustPrtPropComm.lib bmm.lib core.lib geom.lib gfx.lib mesh.lib maxutil.lib maxscrpt.lib manipsys.lib paramblk2.lib Particle.lib helpsys.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /libpath:"c:\3dsmax5\maxsdk\lib" /libpath:"../../../lib" /libpath:"../common/Release"

!ELSEIF  "$(CFG)" == "CustPrtPropMax - Win32 Debug Max 5 with Debug OpenExport"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "CustPrtPropMax___Win32_Debug_Max_5_with_Debug_OpenExport"
# PROP BASE Intermediate_Dir "CustPrtPropMax___Win32_Debug_Max_5_with_Debug_OpenExport"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_Max_5_with_Debug_OpenExport"
# PROP Intermediate_Dir "Debug_Max_5_with_Debug_OpenExport"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "C:\3dsmax5\maxsdk\include" /I "..\..\..\include" /I "..\common" /I "..\..\..\include\common" /I "..\..\..\include\RwMax" /I "..\..\..\include\RwXML" /I "..\..\..\include\RwRf3" /I "..\..\..\include\RwMI" /I "..\..\..\include\RwComm" /I "..\..\..\include\RwExp" /I "..\..\..\..\rwsdk\include\null" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CUSTPRTPROP_MAX_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\..\..\STLport-4.6\stlport" /I "C:\3dsmax5\maxsdk\include" /I "..\..\..\include" /I "..\common" /I "..\..\..\include\common" /I "..\..\..\include\RwMax" /I "..\..\..\include\RwXML" /I "..\..\..\include\RwRf3" /I "..\..\..\include\RwMI" /I "..\..\..\include\RwComm" /I "..\..\..\include\RwExp" /I "$(RWGSDK)\include\null" /I "..\..\..\include\RwUtil" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CUSTPRTPROP_MAX_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 RwMax5Prtd.lib RwMax5d.lib RwCommd.lib CustPrtPropComm.lib bmm.lib core.lib geom.lib gfx.lib mesh.lib maxutil.lib maxscrpt.lib manipsys.lib paramblk2.lib Particle.lib helpsys.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"C:\3dsmax5\plugins\OpenExportPlugins\CustPrtPropMax.dll" /pdbtype:sept /libpath:"../../../lib" /libpath:"../lib" /libpath:"C:\3dsmax5\maxsdk\lib"
# ADD LINK32 RwUtild.lib RwMax5Prtd.lib RwMax5d.lib RwCommd.lib CustPrtPropComm.lib bmm.lib core.lib geom.lib gfx.lib mesh.lib maxutil.lib maxscrpt.lib manipsys.lib paramblk2.lib Particle.lib helpsys.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept /libpath:"C:\3dsmax5\maxsdk\lib" /libpath:"../../../lib" /libpath:"../common/Debug_with_Debug_OpenExport"

!ELSEIF  "$(CFG)" == "CustPrtPropMax - Win32 Debug Max 6"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "CustPrtPropMax___Win32_Debug_Max_6"
# PROP BASE Intermediate_Dir "CustPrtPropMax___Win32_Debug_Max_6"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_Max_6"
# PROP Intermediate_Dir "Debug_Max_6"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "$(STLPORTDIR)" /I "C:\3dsmax5\maxsdk\include" /I "..\..\..\include" /I "..\common" /I "..\..\..\include\common" /I "..\..\..\include\RwMax" /I "..\..\..\include\RwXML" /I "..\..\..\include\RwRf3" /I "..\..\..\include\RwMI" /I "..\..\..\include\RwComm" /I "..\..\..\include\RwExp" /I "..\..\..\..\rwsdk\include\null" /I "..\..\..\include\RwUtil" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CUSTPRTPROP_MAX_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\..\..\STLport-4.6\stlport" /I "C:\3dsmax6\maxsdk\include" /I "..\..\..\include" /I "..\common" /I "..\..\..\include\common" /I "..\..\..\include\RwMax" /I "..\..\..\include\RwXML" /I "..\..\..\include\RwRf3" /I "..\..\..\include\RwMI" /I "..\..\..\include\RwComm" /I "..\..\..\include\RwExp" /I "$(RWGSDK)\include\null" /I "..\..\..\include\RwUtil" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CUSTPRTPROP_MAX_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 RwMax5Prt.lib RwMax5.lib RwComm.lib CustPrtPropComm.lib bmm.lib core.lib geom.lib gfx.lib mesh.lib maxutil.lib maxscrpt.lib manipsys.lib paramblk2.lib Particle.lib helpsys.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"C:\3dsmax5\plugins\OpenExportPlugins\CustPrtPropMax.dll" /pdbtype:sept /libpath:"../../../lib" /libpath:"../lib" /libpath:"C:\3dsmax5\maxsdk\lib"
# ADD LINK32 RwUtil.lib RwMax6Prt.lib RwMax6.lib RwComm.lib CustPrtPropComm.lib bmm.lib core.lib geom.lib gfx.lib mesh.lib maxutil.lib maxscrpt.lib manipsys.lib paramblk2.lib Particle.lib helpsys.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept /libpath:"C:\3dsmax6\maxsdk\lib" /libpath:"../../../lib" /libpath:"../common/Debug"

!ELSEIF  "$(CFG)" == "CustPrtPropMax - Win32 Release Max 6"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "CustPrtPropMax___Win32_Release_Max_6"
# PROP BASE Intermediate_Dir "CustPrtPropMax___Win32_Release_Max_6"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_Max_6"
# PROP Intermediate_Dir "Release_Max_6"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GR /GX /O2 /I "$(STLPORTDIR)" /I "c:\3dsmax5\maxsdk\include" /I "..\..\..\include" /I "..\common" /I "..\..\..\include\common" /I "..\..\..\include\RwMax" /I "..\..\..\include\RwXML" /I "..\..\..\include\RwRf3" /I "..\..\..\include\RwMI" /I "..\..\..\include\RwComm" /I "..\..\..\include\RwExp" /I "..\..\..\..\rwsdk\include\null" /I "..\..\..\include\RwUtil" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CUSTPRTPROP_MAX_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "..\..\..\STLport-4.6\stlport" /I "c:\3dsmax6\maxsdk\include" /I "..\..\..\include" /I "..\common" /I "..\..\..\include\common" /I "..\..\..\include\RwMax" /I "..\..\..\include\RwXML" /I "..\..\..\include\RwRf3" /I "..\..\..\include\RwMI" /I "..\..\..\include\RwComm" /I "..\..\..\include\RwExp" /I "$(RWGSDK)\include\null" /I "..\..\..\include\RwUtil" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CUSTPRTPROP_MAX_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 RwUtil.lib RwMax5Prt.lib RwMax5.lib RwComm.lib CustPrtPropComm.lib bmm.lib core.lib geom.lib gfx.lib mesh.lib maxutil.lib maxscrpt.lib manipsys.lib paramblk2.lib Particle.lib helpsys.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"C:\3dsmax5\plugins\OpenExportPlugins\CustPrtPropMax.dll" /libpath:"../../../lib" /libpath:"../lib" /libpath:"c:\3dsmax5\maxsdk\lib"
# ADD LINK32 RwUtil.lib RwMax6Prt.lib RwMax6.lib RwComm.lib CustPrtPropComm.lib bmm.lib core.lib geom.lib gfx.lib mesh.lib maxutil.lib maxscrpt.lib manipsys.lib paramblk2.lib Particle.lib helpsys.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /libpath:"c:\3dsmax6\maxsdk\lib" /libpath:"../../../lib" /libpath:"../common/Release"

!ELSEIF  "$(CFG)" == "CustPrtPropMax - Win32 Debug Max 6 with Debug OpenExport"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "CustPrtPropMax___Win32_Debug_Max_6_with_Debug_OpenExport"
# PROP BASE Intermediate_Dir "CustPrtPropMax___Win32_Debug_Max_6_with_Debug_OpenExport"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_Max_6_with_Debug_OpenExport"
# PROP Intermediate_Dir "Debug_Max_6_with_Debug_OpenExport"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "$(STLPORTDIR)" /I "C:\3dsmax5\maxsdk\include" /I "..\..\..\include" /I "..\common" /I "..\..\..\include\common" /I "..\..\..\include\RwMax" /I "..\..\..\include\RwXML" /I "..\..\..\include\RwRf3" /I "..\..\..\include\RwMI" /I "..\..\..\include\RwComm" /I "..\..\..\include\RwExp" /I "..\..\..\..\rwsdk\include\null" /I "..\..\..\include\RwUtil" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CUSTPRTPROP_MAX_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\..\..\STLport-4.6\stlport" /I "C:\3dsmax6\maxsdk\include" /I "..\..\..\include" /I "..\common" /I "..\..\..\include\common" /I "..\..\..\include\RwMax" /I "..\..\..\include\RwXML" /I "..\..\..\include\RwRf3" /I "..\..\..\include\RwMI" /I "..\..\..\include\RwComm" /I "..\..\..\include\RwExp" /I "$(RWGSDK)\include\null" /I "..\..\..\include\RwUtil" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CUSTPRTPROP_MAX_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 RwUtild.lib RwMax5Prtd.lib RwMax5d.lib RwCommd.lib CustPrtPropComm.lib bmm.lib core.lib geom.lib gfx.lib mesh.lib maxutil.lib maxscrpt.lib manipsys.lib paramblk2.lib Particle.lib helpsys.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"C:\3dsmax5\plugins\OpenExportPlugins\CustPrtPropMax.dll" /pdbtype:sept /libpath:"../../../lib" /libpath:"../lib" /libpath:"C:\3dsmax5\maxsdk\lib"
# ADD LINK32 RwUtild.lib RwMax6Prtd.lib RwMax6d.lib RwCommd.lib CustPrtPropComm.lib bmm.lib core.lib geom.lib gfx.lib mesh.lib maxutil.lib maxscrpt.lib manipsys.lib paramblk2.lib Particle.lib helpsys.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept /libpath:"C:\3dsmax6\maxsdk\lib" /libpath:"../../../lib" /libpath:"../common/Debug_with_Debug_OpenExport"

!ENDIF 

# Begin Target

# Name "CustPrtPropMax - Win32 Debug Max 5"
# Name "CustPrtPropMax - Win32 Release Max 5"
# Name "CustPrtPropMax - Win32 Debug Max 5 with Debug OpenExport"
# Name "CustPrtPropMax - Win32 Debug Max 6"
# Name "CustPrtPropMax - Win32 Release Max 6"
# Name "CustPrtPropMax - Win32 Debug Max 6 with Debug OpenExport"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CustPrtPropMax.cpp
# End Source File
# Begin Source File

SOURCE=.\CustPrtPropMax.def
# End Source File
# Begin Source File

SOURCE=.\MyPrtMaxPrtSys.cpp
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

SOURCE=.\CustPrtPropMax.h
# End Source File
# Begin Source File

SOURCE=.\MyPrtMaxPrtSys.h
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
