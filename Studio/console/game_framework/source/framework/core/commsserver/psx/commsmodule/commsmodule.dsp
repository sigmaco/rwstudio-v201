# Microsoft Developer Studio Project File - Name="COMMSMODULE" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 60000
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=COMMSMODULE - Win32 PS2 Design Release CDROM
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "commsmodule.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "commsmodule.mak" CFG="COMMSMODULE - Win32 PS2 Design Release CDROM"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "COMMSMODULE - Win32 PS2 Debug" (based on "Win32 (x86) Application")
!MESSAGE "COMMSMODULE - Win32 PS2 Release" (based on "Win32 (x86) Application")
!MESSAGE "COMMSMODULE - Win32 PS2 Metrics" (based on "Win32 (x86) Application")
!MESSAGE "COMMSMODULE - Win32 PS2 Design Debug" (based on "Win32 (x86) Application")
!MESSAGE "COMMSMODULE - Win32 PS2 Design Metrics" (based on "Win32 (x86) Application")
!MESSAGE "COMMSMODULE - Win32 PS2 Design Release" (based on "Win32 (x86) Application")
!MESSAGE "COMMSMODULE - Win32 PS2 Release CDROM" (based on "Win32 (x86) Application")
!MESSAGE "COMMSMODULE - Win32 PS2 Design Release CDROM" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "COMMSMODULE"
# PROP Scc_LocalPath ".."
CPP=snCl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "COMMSMODULE - Win32 PS2 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "COMMSMODULE___Win32_PS2_IOP_Debug"
# PROP BASE Intermediate_Dir "COMMSMODULE___Win32_PS2_IOP_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "PS2_IOP_Debug"
# PROP Intermediate_Dir "PS2_IOP_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /GZ /c
# ADD CPP /nologo /W4 /Od /I "..\\" /I "C:\usr\local\sce\iop\gcc\mipsel-scei-elfl\include\inet" /D "SN_TARGET_PS2_IOP" /FD -G0 /debug /c
# SUBTRACT CPP /WX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=snLink.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 inet.ilb inetctl.ilb /nologo /pdb:none /debug /machine:IX86 /out:"..\..\..\..\..\..\bin\Modules\rwscommsd.irx" /D:SN_TARGET_PS2_IOP

!ELSEIF  "$(CFG)" == "COMMSMODULE - Win32 PS2 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "COMMSMODULE___Win32_PS2_IOP_Release"
# PROP BASE Intermediate_Dir "COMMSMODULE___Win32_PS2_IOP_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "PS2_IOP_Release"
# PROP Intermediate_Dir "PS2_IOP_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /GZ /c
# ADD CPP /nologo /W4 /O2 /I "..\\" /I "C:\usr\local\sce\iop\gcc\mipsel-scei-elfl\include\inet" /D "SN_TARGET_PS2_IOP" /FD -G0 /c
# SUBTRACT CPP /WX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=snLink.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 inet.ilb inetctl.ilb /nologo /pdb:none /machine:IX86 /out:"..\..\..\..\..\..\bin\Modules\rwscomms.irx" /D:SN_TARGET_PS2_IOP

!ELSEIF  "$(CFG)" == "COMMSMODULE - Win32 PS2 Metrics"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "COMMSMODULE___Win32_PS2_IOP_Metrics"
# PROP BASE Intermediate_Dir "COMMSMODULE___Win32_PS2_IOP_Metrics"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "PS2_IOP_Release"
# PROP Intermediate_Dir "PS2_IOP_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /w /W0 /WX /O2 /I "..\\" /I "C:\usr\local\sce\iop\gcc\mipsel-scei-elfl\include\inet" /D "SN_TARGET_PS2_IOP" /FD -G0 /c
# ADD CPP /nologo /W4 /O2 /I "..\\" /I "C:\usr\local\sce\iop\gcc\mipsel-scei-elfl\include\inet" /D "SN_TARGET_PS2_IOP" /FD -G0 /c
# SUBTRACT CPP /WX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=snLink.exe
# ADD BASE LINK32 inet.ilb inetctl.ilb /nologo /pdb:none /machine:IX86 /out:"..\..\..\..\..\..\bin\Modules\rwscomms.irx" /D:SN_TARGET_PS2_IOP
# ADD LINK32 inet.ilb inetctl.ilb /nologo /pdb:none /machine:IX86 /out:"..\..\..\..\..\..\bin\Modules\rwscomms.irx" /D:SN_TARGET_PS2_IOP

!ELSEIF  "$(CFG)" == "COMMSMODULE - Win32 PS2 Design Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "COMMSMODULE___Win32_PS2_IOP_Design_Debug"
# PROP BASE Intermediate_Dir "COMMSMODULE___Win32_PS2_IOP_Design_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "PS2_IOP_Debug"
# PROP Intermediate_Dir "PS2_IOP_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /w /W0 /WX /Od /I "..\\" /I "C:\usr\local\sce\iop\gcc\mipsel-scei-elfl\include\inet" /D "SN_TARGET_PS2_IOP" /FD -G0 /debug /c
# ADD CPP /nologo /W4 /Od /I "..\\" /I "C:\usr\local\sce\iop\gcc\mipsel-scei-elfl\include\inet" /D "SN_TARGET_PS2_IOP" /FD -G0 /debug /c
# SUBTRACT CPP /WX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=snLink.exe
# ADD BASE LINK32 inet.ilb inetctl.ilb /nologo /pdb:none /debug /machine:IX86 /out:"..\..\..\..\..\..\bin\Modules\rwscommsd.irx" /D:SN_TARGET_PS2_IOP
# ADD LINK32 inet.ilb inetctl.ilb /nologo /pdb:none /debug /machine:IX86 /out:"..\..\..\..\..\..\bin\Modules\rwscommsd.irx" /D:SN_TARGET_PS2_IOP

!ELSEIF  "$(CFG)" == "COMMSMODULE - Win32 PS2 Design Metrics"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "COMMSMODULE___Win32_PS2_IOP_Design_Metrics"
# PROP BASE Intermediate_Dir "COMMSMODULE___Win32_PS2_IOP_Design_Metrics"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "PS2_IOP_Release"
# PROP Intermediate_Dir "PS2_IOP_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /w /W0 /WX /O2 /I "..\\" /I "C:\usr\local\sce\iop\gcc\mipsel-scei-elfl\include\inet" /D "SN_TARGET_PS2_IOP" /FD -G0 /c
# ADD CPP /nologo /W4 /O2 /I "..\\" /I "C:\usr\local\sce\iop\gcc\mipsel-scei-elfl\include\inet" /D "SN_TARGET_PS2_IOP" /FD -G0 /c
# SUBTRACT CPP /WX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=snLink.exe
# ADD BASE LINK32 inet.ilb inetctl.ilb /nologo /pdb:none /machine:IX86 /out:"..\..\..\..\..\..\bin\Modules\rwscomms.irx" /D:SN_TARGET_PS2_IOP
# ADD LINK32 inet.ilb inetctl.ilb /nologo /pdb:none /machine:IX86 /out:"..\..\..\..\..\..\bin\Modules\rwscomms.irx" /D:SN_TARGET_PS2_IOP

!ELSEIF  "$(CFG)" == "COMMSMODULE - Win32 PS2 Design Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "COMMSMODULE___Win32_PS2_IOP_Design_Release"
# PROP BASE Intermediate_Dir "COMMSMODULE___Win32_PS2_IOP_Design_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "PS2_IOP_Release"
# PROP Intermediate_Dir "PS2_IOP_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /w /W0 /WX /O2 /I "..\\" /I "C:\usr\local\sce\iop\gcc\mipsel-scei-elfl\include\inet" /D "SN_TARGET_PS2_IOP" /FD -G0 /c
# ADD CPP /nologo /W4 /O2 /I "..\\" /I "C:\usr\local\sce\iop\gcc\mipsel-scei-elfl\include\inet" /D "SN_TARGET_PS2_IOP" /FD -G0 /c
# SUBTRACT CPP /WX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=snLink.exe
# ADD BASE LINK32 inet.ilb inetctl.ilb /nologo /pdb:none /machine:IX86 /out:"..\..\..\..\..\..\bin\Modules\rwscomms.irx" /D:SN_TARGET_PS2_IOP
# ADD LINK32 inet.ilb inetctl.ilb /nologo /pdb:none /machine:IX86 /out:"..\..\..\..\..\..\bin\Modules\rwscomms.irx" /D:SN_TARGET_PS2_IOP

!ELSEIF  "$(CFG)" == "COMMSMODULE - Win32 PS2 Release CDROM"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "COMMSMODULE___Win32_PS2_IOP_Release_CDROM"
# PROP BASE Intermediate_Dir "COMMSMODULE___Win32_PS2_IOP_Release_CDROM"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "PS2_IOP_Release"
# PROP Intermediate_Dir "PS2_IOP_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /w /W0 /WX /O2 /I "..\\" /I "C:\usr\local\sce\iop\gcc\mipsel-scei-elfl\include\inet" /D "SN_TARGET_PS2_IOP" /FD -G0 /c
# ADD CPP /nologo /W4 /O2 /I "..\\" /I "C:\usr\local\sce\iop\gcc\mipsel-scei-elfl\include\inet" /D "SN_TARGET_PS2_IOP" /FD -G0 /c
# SUBTRACT CPP /WX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=snLink.exe
# ADD BASE LINK32 inet.ilb inetctl.ilb /nologo /pdb:none /machine:IX86 /out:"..\..\..\..\..\..\bin\Modules\rwscomms.irx" /D:SN_TARGET_PS2_IOP
# ADD LINK32 inet.ilb inetctl.ilb /nologo /pdb:none /machine:IX86 /out:"..\..\..\..\..\..\bin\Modules\rwscomms.irx" /D:SN_TARGET_PS2_IOP

!ELSEIF  "$(CFG)" == "COMMSMODULE - Win32 PS2 Design Release CDROM"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "COMMSMODULE___Win32_PS2_IOP_Design_Release_CDROM"
# PROP BASE Intermediate_Dir "COMMSMODULE___Win32_PS2_IOP_Design_Release_CDROM"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "PS2_IOP_Release"
# PROP Intermediate_Dir "PS2_IOP_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /w /W0 /WX /O2 /I "..\\" /I "C:\usr\local\sce\iop\gcc\mipsel-scei-elfl\include\inet" /D "SN_TARGET_PS2_IOP" /FD -G0 /c
# ADD CPP /nologo /W4 /O2 /I "..\\" /I "C:\usr\local\sce\iop\gcc\mipsel-scei-elfl\include\inet" /D "SN_TARGET_PS2_IOP" /FD -G0 /c
# SUBTRACT CPP /WX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=snLink.exe
# ADD BASE LINK32 inet.ilb inetctl.ilb /nologo /pdb:none /machine:IX86 /out:"..\..\..\..\..\..\bin\Modules\rwscomms.irx" /D:SN_TARGET_PS2_IOP
# ADD LINK32 inet.ilb inetctl.ilb /nologo /pdb:none /machine:IX86 /out:"..\..\..\..\..\..\bin\Modules\rwscomms.irx" /D:SN_TARGET_PS2_IOP

!ENDIF 

# Begin Target

# Name "COMMSMODULE - Win32 PS2 Debug"
# Name "COMMSMODULE - Win32 PS2 Release"
# Name "COMMSMODULE - Win32 PS2 Metrics"
# Name "COMMSMODULE - Win32 PS2 Design Debug"
# Name "COMMSMODULE - Win32 PS2 Design Metrics"
# Name "COMMSMODULE - Win32 PS2 Design Release"
# Name "COMMSMODULE - Win32 PS2 Release CDROM"
# Name "COMMSMODULE - Win32 PS2 Design Release CDROM"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\main.c
# End Source File
# Begin Source File

SOURCE=.\Server.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\PS2_in_VC.h
# End Source File
# Begin Source File

SOURCE=..\RPCMessages.h
# End Source File
# Begin Source File

SOURCE=.\Server.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
