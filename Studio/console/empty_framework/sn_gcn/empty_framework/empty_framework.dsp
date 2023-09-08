# Microsoft Developer Studio Project File - Name="empty_framework" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 60000
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=empty_framework - Win32 NGC DDH DesignRelease Broadband
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "empty_framework.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "empty_framework.mak" CFG="empty_framework - Win32 NGC DDH DesignRelease Broadband"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "empty_framework - Win32 NGC DDH DesignDebug" (based on "Win32 (x86) Console Application")
!MESSAGE "empty_framework - Win32 NGC DDH DesignRelease" (based on "Win32 (x86) Console Application")
!MESSAGE "empty_framework - Win32 NGC DDH Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "empty_framework - Win32 NGC DDH Release" (based on "Win32 (x86) Console Application")
!MESSAGE "empty_framework - Win32 NGC GDEV DesignDebug" (based on "Win32 (x86) Console Application")
!MESSAGE "empty_framework - Win32 NGC GDEV DesignRelease" (based on "Win32 (x86) Console Application")
!MESSAGE "empty_framework - Win32 NGC GDEV Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "empty_framework - Win32 NGC GDEV Release" (based on "Win32 (x86) Console Application")
!MESSAGE "empty_framework - Win32 NGC GDEV DesignDebug Broadband" (based on "Win32 (x86) Console Application")
!MESSAGE "empty_framework - Win32 NGC GDEV DesignRelease Broadband" (based on "Win32 (x86) Console Application")
!MESSAGE "empty_framework - Win32 NGC DDH DesignDebug Broadband" (based on "Win32 (x86) Console Application")
!MESSAGE "empty_framework - Win32 NGC DDH DesignRelease Broadband" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "empty_framework"
# PROP Scc_LocalPath "."
CPP=snCl.exe
RSC=rc.exe

!IF  "$(CFG)" == "empty_framework - Win32 NGC DDH DesignDebug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "empty_framework___Win32_NGC_DDH_DesignDebug"
# PROP BASE Intermediate_Dir "empty_framework___Win32_NGC_DDH_DesignDebug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DDH_Design_Debug"
# PROP Intermediate_Dir "DDH_Design_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "../../source/" /D "HW2" /D "_DEBUG" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWDEBUG" /D "RWTERMINAL" /D "DEBUG" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /FD /debug /c
# ADD CPP /nologo /W3 /GX /Od /I "..\..\..\game_framework\source\framework\core\MemoryHandler" /I "../../../game_framework/source" /I "../../source" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /D "_DEBUG" /D "RWDEBUG" /D "DEBUG" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "RWS_FUNCTIONPROFILE" /FD /debug -fexceptions /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=snLink.exe
# ADD BASE LINK32 baseD.a osD.a exiD.a siD.a dbD.a mtxD.a dvdD.a viD.a demoD.a padD.a aiD.a arD.a axD.a axfxD.a mixD.a spD.a amD.a axartD.a synD.a seqD.a dspD.a dtkD.a cardD.a hioD.a mccD.a gxD.a G2DD.a texPaletteD.a fileCacheD.a supportD.a perfD.a AmcExi2D.a amcnotstubD.a odemuexi2D.a odenotstubD.a libstl.a rpdmorph.a rplodatm.a rpmatfx.a rpmipkl.a rppatch.a rppatchmatfx.a rppatchskin.a rppatchskinmatfx.a rpprtstd.a rpptank.a rpskinmatfx.a rpspline.a rpusrdat.a rtbezpat.a rtimport.a rtmipkmatfx.a rtmipkpatch.a rtmipkpatchmatfx.a rtpick.a rtpitexd.a rtray.a rtslerp.a rtsplpvs.a rttiff.a rttilerd.a rttoc.a rt2d.a rt2danim.a rpltmap.a rtltmap.a rtbary.a rpmorph.a rplogo.a rtmipk.a rphanim.a rpskin.a rtquat.a rtcharse.a rtras.a rtbmp.a rtpng.a rpcollis.a rtintsec.a rppvs.a rprandom.a rpworld.a rtworld.a rwcore.a rtanim.a libmcdconvex.a libmeassetfactory.a libmcdconvexcreatehull.a libmst.a libmcdframe.a libmcdprimitives.a libmdt.a libmdtarthur.a libmdtbcl.a libmdtkea.a libmdtsolverutils.a libmeapp.a libmeassetdb.a libmeassetdbxmlio.a libmeglobals.a libmexml.a libmcdcommon.a /nologo /pdb:none /debug /machine:IX86 /out:"DesignDebug\empty_framework.elf" /libpath:"$(DOLPHIN_ROOT)\HW2\lib" /libpath:"$(ODEMUSDKBIN)\..\lib" /libpath:"$(AMCDDKBIN)\..\lib" /libpath:"$(RWGSDK)\lib\gcn\debug" /D:SN_TARGET_NGC
# ADD LINK32 odemustubsd.a amcnotstubd.a amcExi2d.a baseD.a osD.a exiD.a siD.a dbD.a mtxD.a dvdD.a viD.a demoD.a padD.a aiD.a arD.a axD.a axfxD.a mixD.a spD.a amD.a axartD.a synD.a seqD.a dspD.a dtkD.a cardD.a hioD.a mccD.a gxD.a G2DD.a texPaletteD.a fileCacheD.a supportD.a perfD.a libstl.a rpdmorph.a rplodatm.a rpmatfx.a rpmipkl.a rppatch.a rppatchmatfx.a rppatchskin.a rppatchskinmatfx.a rpprtstd.a rpptank.a rpskinmatfx.a rpspline.a rpusrdat.a rtbezpat.a rtimport.a rtmipkmatfx.a rtmipkpatch.a rtmipkpatchmatfx.a rtpick.a rtpitexd.a rtray.a rtslerp.a rtsplpvs.a rttiff.a rttilerd.a rttoc.a rt2d.a rt2danim.a rpltmap.a rtltmap.a rtbary.a rpmorph.a rplogo.a rtmipk.a rphanim.a rtquat.a rtcharse.a rtras.a rtbmp.a rtpng.a rpcollis.a rtintsec.a rppvs.a rprandom.a rpworld.a rtworld.a rwcore.a rtanim.a rtfsyst.a /nologo /pdb:none /debug /machine:IX86 /out:"..\bin\DDH_Design_Debug\empty_framework.elf" /libpath:"$(DOLPHIN_ROOT)\HW2\lib" /libpath:"$(AMCDDKBIN)\..\lib" /libpath:"$(RWGSDK)\lib\gcn\debug" /D:SN_TARGET_NGC

!ELSEIF  "$(CFG)" == "empty_framework - Win32 NGC DDH DesignRelease"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "empty_framework___Win32_NGC_DDH_DesignRelease"
# PROP BASE Intermediate_Dir "empty_framework___Win32_NGC_DDH_DesignRelease"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DDH_Design_Release"
# PROP Intermediate_Dir "DDH_Design_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "../../source/" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "NDEBUG" /D "RWS_DISABLE_MEMORY_CHECKING" /FD /debug /c
# ADD CPP /nologo /W3 /GX /Od /I "..\..\..\game_framework\source\framework\core\MemoryHandler" /I "../../../game_framework/source" /I "../../source" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "NDEBUG" /D "RWS_DISABLE_MEMORY_CHECKING" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "RWS_FUNCTIONPROFILE" /FD -fexceptions /debug /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=snLink.exe
# ADD BASE LINK32 base.a os.a exi.a si.a db.a mtx.a dvd.a vi.a demo.a pad.a ai.a ar.a ax.a axfx.a mix.a sp.a am.a axart.a syn.a seq.a dsp.a dtk.a card.a hio.a mcc.a gx.a G2D.a texPalette.a fileCache.a support.a perf.a AmcExi2.a amcnotstub.a odemuexi2.a odenotstub.a libstl.a rpdmorph.a rplodatm.a rpmatfx.a rpmipkl.a rppatch.a rppatchmatfx.a rppatchskin.a rppatchskinmatfx.a rpprtstd.a rpptank.a rpskinmatfx.a rpspline.a rpusrdat.a rtbezpat.a rtimport.a rtmipkmatfx.a rtmipkpatch.a rtmipkpatchmatfx.a rtpick.a rtpitexd.a rtray.a rtslerp.a rtsplpvs.a rttiff.a rttilerd.a rttoc.a rt2d.a rt2danim.a rpltmap.a rtltmap.a rtbary.a rpmorph.a rplogo.a rtmipk.a rphanim.a rpskin.a rtquat.a rtcharse.a rtras.a rtbmp.a rtpng.a rpcollis.a rtintsec.a rppvs.a rprandom.a rpworld.a rtworld.a rwcore.a rtanim.a /nologo /pdb:none /debug /machine:IX86 /out:"DesignRelease\empty_framework.elf" /libpath:"$(DOLPHIN_ROOT)\HW2\lib" /libpath:"$(ODEMUSDKBIN)\..\lib" /libpath:"$(AMCDDKBIN)\..\lib" /libpath:"$(RWGSDK)\lib\gcn\release" /D:SN_TARGET_NGC
# ADD LINK32 odemustubs.a amcnotstub.a amcExi2.a base.a os.a exi.a si.a db.a mtx.a dvd.a vi.a demo.a pad.a ai.a ar.a ax.a axfx.a mix.a sp.a am.a axart.a syn.a seq.a dsp.a dtk.a card.a hio.a mcc.a gx.a G2D.a texPalette.a fileCache.a support.a perf.a libstl.a rpdmorph.a rplodatm.a rpmatfx.a rpmipkl.a rppatch.a rppatchmatfx.a rppatchskin.a rppatchskinmatfx.a rpprtstd.a rpptank.a rpskinmatfx.a rpspline.a rpusrdat.a rtbezpat.a rtimport.a rtmipkmatfx.a rtmipkpatch.a rtmipkpatchmatfx.a rtpick.a rtpitexd.a rtray.a rtslerp.a rtsplpvs.a rttiff.a rttilerd.a rttoc.a rt2d.a rt2danim.a rpltmap.a rtltmap.a rtbary.a rpmorph.a rplogo.a rtmipk.a rphanim.a rtquat.a rtcharse.a rtras.a rtbmp.a rtpng.a rpcollis.a rtintsec.a rppvs.a rprandom.a rpworld.a rtworld.a rwcore.a rtanim.a rtfsyst.a /nologo /pdb:none /debug /machine:IX86 /out:"..\bin\DDH_Design_Release\empty_framework.elf" /libpath:"$(RWGSDK)\lib\gcn\release" /libpath:"$(DOLPHIN_ROOT)\HW2\lib" /libpath:"$(AMCDDKBIN)\..\lib" /D:SN_TARGET_NGC

!ELSEIF  "$(CFG)" == "empty_framework - Win32 NGC DDH Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "empty_framework___Win32_NGC_DDH_Debug"
# PROP BASE Intermediate_Dir "empty_framework___Win32_NGC_DDH_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DDH_Debug"
# PROP Intermediate_Dir "DDH_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "../../source/" /D "HW2" /D "_DEBUG" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWDEBUG" /D "RWTERMINAL" /D "DEBUG" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /FD /debug /c
# ADD CPP /nologo /W3 /Od /I "..\..\..\game_framework\source\framework\core\MemoryHandler" /I "../../../game_framework/source" /I "../../source" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /D "_DEBUG" /D "RWDEBUG" /D "DEBUG" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "RWS_DISABLE_MEMORY_CHECKING" /FD -fexceptions /debug /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=snLink.exe
# ADD BASE LINK32 baseD.a osD.a exiD.a siD.a dbD.a mtxD.a dvdD.a viD.a demoD.a padD.a aiD.a arD.a axD.a axfxD.a mixD.a spD.a amD.a axartD.a synD.a seqD.a dspD.a dtkD.a cardD.a hioD.a mccD.a gxD.a G2DD.a texPaletteD.a fileCacheD.a supportD.a perfD.a AmcExi2D.a amcnotstubD.a odemuexi2D.a odenotstubD.a libstl.a rpdmorph.a rplodatm.a rpmatfx.a rpmipkl.a rppatch.a rppatchmatfx.a rppatchskin.a rppatchskinmatfx.a rpprtstd.a rpptank.a rpskinmatfx.a rpspline.a rpusrdat.a rtbezpat.a rtimport.a rtmipkmatfx.a rtmipkpatch.a rtmipkpatchmatfx.a rtpick.a rtpitexd.a rtray.a rtslerp.a rtsplpvs.a rttiff.a rttilerd.a rttoc.a rt2d.a rt2danim.a rpltmap.a rtltmap.a rtbary.a rpmorph.a rplogo.a rtmipk.a rphanim.a rpskin.a rtquat.a rtcharse.a rtras.a rtbmp.a rtpng.a rpcollis.a rtintsec.a rppvs.a rprandom.a rpworld.a rtworld.a rwcore.a rtanim.a /nologo /pdb:none /debug /machine:IX86 /out:"Debug\empty_framework.elf" /libpath:"$(DOLPHIN_ROOT)\HW2\lib" /libpath:"$(ODEMUSDKBIN)\..\lib" /libpath:"$(AMCDDKBIN)\..\lib" /libpath:"$(RWGSDK)\lib\gcn\debug" /D:SN_TARGET_NGC
# ADD LINK32 odemustubsd.a amcnotstubd.a amcExi2d.a baseD.a osD.a exiD.a siD.a dbD.a mtxD.a dvdD.a viD.a demoD.a padD.a aiD.a arD.a axD.a axfxD.a mixD.a spD.a amD.a axartD.a synD.a seqD.a dspD.a dtkD.a cardD.a hioD.a mccD.a gxD.a G2DD.a texPaletteD.a fileCacheD.a supportD.a perfD.a libstl.a rpdmorph.a rplodatm.a rpmatfx.a rpmipkl.a rppatch.a rppatchmatfx.a rppatchskin.a rppatchskinmatfx.a rpprtstd.a rpptank.a rpskinmatfx.a rpspline.a rpusrdat.a rtbezpat.a rtimport.a rtmipkmatfx.a rtmipkpatch.a rtmipkpatchmatfx.a rtpick.a rtpitexd.a rtray.a rtslerp.a rtsplpvs.a rttiff.a rttilerd.a rttoc.a rt2d.a rt2danim.a rpltmap.a rtltmap.a rtbary.a rpmorph.a rplogo.a rtmipk.a rphanim.a rtquat.a rtcharse.a rtras.a rtbmp.a rtpng.a rpcollis.a rtintsec.a rppvs.a rprandom.a rpworld.a rtworld.a rwcore.a rtanim.a rtfsyst.a /nologo /pdb:none /debug /machine:IX86 /out:"..\bin\DDH_Debug\empty_framework.elf" /libpath:"$(RWGSDK)\lib\gcn\debug" /libpath:"$(DOLPHIN_ROOT)\HW2\lib" /libpath:"$(AMCDDKBIN)\..\lib" /D:SN_TARGET_NGC

!ELSEIF  "$(CFG)" == "empty_framework - Win32 NGC DDH Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "empty_framework___Win32_NGC_DDH_Release"
# PROP BASE Intermediate_Dir "empty_framework___Win32_NGC_DDH_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DDH_Release"
# PROP Intermediate_Dir "DDH_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "../../source/" /D "RWS_DISABLE_MEMORY_CHECKING" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "NDEBUG" /FD /debug /c
# ADD CPP /nologo /W3 /Od /I "..\..\..\game_framework\source\framework\core\MemoryHandler" /I "../../../game_framework/source" /I "../../source" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /D "RWS_DISABLE_MEMORY_CHECKING" /D "NDEBUG" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /FD -fexceptions /debug /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=snLink.exe
# ADD BASE LINK32 base.a os.a exi.a si.a db.a mtx.a dvd.a vi.a demo.a pad.a ai.a ar.a ax.a axfx.a mix.a sp.a am.a axart.a syn.a seq.a dsp.a dtk.a card.a hio.a mcc.a gx.a G2D.a texPalette.a fileCache.a support.a perf.a AmcExi2.a amcnotstub.a odemuexi2.a odenotstub.a libstl.a rpdmorph.a rplodatm.a rpmatfx.a rpmipkl.a rppatch.a rppatchmatfx.a rppatchskin.a rppatchskinmatfx.a rpprtstd.a rpptank.a rpskinmatfx.a rpspline.a rpusrdat.a rtbezpat.a rtimport.a rtmipkmatfx.a rtmipkpatch.a rtmipkpatchmatfx.a rtpick.a rtpitexd.a rtray.a rtslerp.a rtsplpvs.a rttiff.a rttilerd.a rttoc.a rt2d.a rt2danim.a rpltmap.a rtltmap.a rtbary.a rpmorph.a rplogo.a rtmipk.a rphanim.a rpskin.a rtquat.a rtcharse.a rtras.a rtbmp.a rtpng.a rpcollis.a rtintsec.a rppvs.a rprandom.a rpworld.a rtworld.a rwcore.a rtanim.a /nologo /pdb:none /debug /machine:IX86 /out:"Release\empty_framework.elf" /libpath:"$(DOLPHIN_ROOT)\HW2\lib" /libpath:"$(ODEMUSDKBIN)\..\lib" /libpath:"$(AMCDDKBIN)\..\lib" /libpath:"$(RWGSDK)\lib\gcn\release" /D:SN_TARGET_NGC
# ADD LINK32 odemustubs.a amcnotstub.a amcExi2.a base.a os.a exi.a si.a db.a mtx.a dvd.a vi.a demo.a pad.a ai.a ar.a ax.a axfx.a mix.a sp.a am.a axart.a syn.a seq.a dsp.a dtk.a card.a hio.a mcc.a gx.a G2D.a texPalette.a fileCache.a support.a perf.a libstl.a rpdmorph.a rplodatm.a rpmatfx.a rpmipkl.a rppatch.a rppatchmatfx.a rppatchskin.a rppatchskinmatfx.a rpprtstd.a rpptank.a rpskinmatfx.a rpspline.a rpusrdat.a rtbezpat.a rtimport.a rtmipkmatfx.a rtmipkpatch.a rtmipkpatchmatfx.a rtpick.a rtpitexd.a rtray.a rtslerp.a rtsplpvs.a rttiff.a rttilerd.a rttoc.a rt2d.a rt2danim.a rpltmap.a rtltmap.a rtbary.a rpmorph.a rplogo.a rtmipk.a rphanim.a rtquat.a rtcharse.a rtras.a rtbmp.a rtpng.a rpcollis.a rtintsec.a rppvs.a rprandom.a rpworld.a rtworld.a rwcore.a rtanim.a rtfsyst.a /nologo /pdb:none /debug /machine:IX86 /out:"..\bin\DDH_Release\empty_framework.elf" /libpath:"$(RWGSDK)\lib\gcn\release" /libpath:"$(DOLPHIN_ROOT)\HW2\lib" /libpath:"$(AMCDDKBIN)\..\lib" /D:SN_TARGET_NGC

!ELSEIF  "$(CFG)" == "empty_framework - Win32 NGC GDEV DesignDebug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "empty_framework___Win32_NGC_GDEV_DesignDebug"
# PROP BASE Intermediate_Dir "empty_framework___Win32_NGC_GDEV_DesignDebug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "GDEV_Design_Debug"
# PROP Intermediate_Dir "GDEV_Design_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "../../source/" /D "HW2" /D "_DEBUG" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWDEBUG" /D "RWTERMINAL" /D "DEBUG" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /FD /debug /c
# ADD CPP /nologo /W3 /GX /Od /I "..\..\..\game_framework\source\framework\core\MemoryHandler" /I "../../../game_framework/source" /I "../../source" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /D "_DEBUG" /D "RWDEBUG" /D "DEBUG" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "RWS_FUNCTIONPROFILE" /FD -fexceptions /debug /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=snLink.exe
# ADD BASE LINK32 baseD.a osD.a exiD.a siD.a dbD.a mtxD.a dvdD.a viD.a demoD.a padD.a aiD.a arD.a axD.a axfxD.a mixD.a spD.a amD.a axartD.a synD.a seqD.a dspD.a dtkD.a cardD.a hioD.a mccD.a gxD.a G2DD.a texPaletteD.a fileCacheD.a supportD.a perfD.a AmcExi2D.a amcnotstubD.a odemuexi2D.a odenotstubD.a libstl.a rpdmorph.a rplodatm.a rpmatfx.a rpmipkl.a rppatch.a rppatchmatfx.a rppatchskin.a rppatchskinmatfx.a rpprtstd.a rpptank.a rpskinmatfx.a rpspline.a rpusrdat.a rtbezpat.a rtimport.a rtmipkmatfx.a rtmipkpatch.a rtmipkpatchmatfx.a rtpick.a rtpitexd.a rtray.a rtslerp.a rtsplpvs.a rttiff.a rttilerd.a rttoc.a rt2d.a rt2danim.a rpltmap.a rtltmap.a rtbary.a rpmorph.a rplogo.a rtmipk.a rphanim.a rpskin.a rtquat.a rtcharse.a rtras.a rtbmp.a rtpng.a rpcollis.a rtintsec.a rppvs.a rprandom.a rpworld.a rtworld.a rwcore.a rtanim.a libmcdconvex.a libmeassetfactory.a libmcdconvexcreatehull.a libmst.a libmcdframe.a libmcdprimitives.a libmdt.a libmdtarthur.a libmdtbcl.a libmdtkea.a libmdtsolverutils.a libmeapp.a libmeassetdb.a libmeassetdbxmlio.a libmeglobals.a libmexml.a libmcdcommon.a /nologo /pdb:none /debug /machine:IX86 /out:"DesignDebug\empty_framework.elf" /libpath:"$(DOLPHIN_ROOT)\HW2\lib" /libpath:"$(ODEMUSDKBIN)\..\lib" /libpath:"$(AMCDDKBIN)\..\lib" /libpath:"$(RWGSDK)\lib\gcn\debug" /D:SN_TARGET_NGC
# ADD LINK32 odenotstubd.a amcstubsd.a OdemuExi2d.a baseD.a osD.a exiD.a siD.a dbD.a mtxD.a dvdD.a viD.a demoD.a padD.a aiD.a arD.a axD.a axfxD.a mixD.a spD.a amD.a axartD.a synD.a seqD.a dspD.a dtkD.a cardD.a hioD.a mccD.a gxD.a G2DD.a texPaletteD.a fileCacheD.a supportD.a perfD.a libstl.a rpdmorph.a rplodatm.a rpmatfx.a rpmipkl.a rppatch.a rppatchmatfx.a rppatchskin.a rppatchskinmatfx.a rpprtstd.a rpptank.a rpskinmatfx.a rpspline.a rpusrdat.a rtbezpat.a rtimport.a rtmipkmatfx.a rtmipkpatch.a rtmipkpatchmatfx.a rtpick.a rtpitexd.a rtray.a rtslerp.a rtsplpvs.a rttiff.a rttilerd.a rttoc.a rt2d.a rt2danim.a rpltmap.a rtltmap.a rtbary.a rpmorph.a rplogo.a rtmipk.a rphanim.a rtquat.a rtcharse.a rtras.a rtbmp.a rtpng.a rpcollis.a rtintsec.a rppvs.a rprandom.a rpworld.a rtworld.a rwcore.a rtanim.a rtfsyst.a /nologo /pdb:none /debug /machine:IX86 /out:"..\bin\GDEV_Design_Debug\empty_framework.elf" /libpath:"$(RWGSDK)\lib\gcn\debug" /libpath:"$(DOLPHIN_ROOT)\HW2\lib" /libpath:"$(ODEMUSDKBIN)\..\lib" /D:SN_TARGET_NGC

!ELSEIF  "$(CFG)" == "empty_framework - Win32 NGC GDEV DesignRelease"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "empty_framework___Win32_NGC_GDEV_DesignRelease"
# PROP BASE Intermediate_Dir "empty_framework___Win32_NGC_GDEV_DesignRelease"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "GDEV_Design_Release"
# PROP Intermediate_Dir "GDEV_Design_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "../../source/" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "NDEBUG" /D "RWS_DISABLE_MEMORY_CHECKING" /FD /debug /c
# ADD CPP /nologo /W3 /GX /Od /I "..\..\..\game_framework\source\framework\core\MemoryHandler" /I "../../../game_framework/source" /I "../../source" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "NDEBUG" /D "RWS_DISABLE_MEMORY_CHECKING" /D "RWS_FUNCTIONPROFILE" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /FD -fexceptions /debug /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=snLink.exe
# ADD BASE LINK32 base.a os.a exi.a si.a db.a mtx.a dvd.a vi.a demo.a pad.a ai.a ar.a ax.a axfx.a mix.a sp.a am.a axart.a syn.a seq.a dsp.a dtk.a card.a hio.a mcc.a gx.a G2D.a texPalette.a fileCache.a support.a perf.a AmcExi2.a amcnotstub.a odemuexi2.a odenotstub.a libstl.a rpdmorph.a rplodatm.a rpmatfx.a rpmipkl.a rppatch.a rppatchmatfx.a rppatchskin.a rppatchskinmatfx.a rpprtstd.a rpptank.a rpskinmatfx.a rpspline.a rpusrdat.a rtbezpat.a rtimport.a rtmipkmatfx.a rtmipkpatch.a rtmipkpatchmatfx.a rtpick.a rtpitexd.a rtray.a rtslerp.a rtsplpvs.a rttiff.a rttilerd.a rttoc.a rt2d.a rt2danim.a rpltmap.a rtltmap.a rtbary.a rpmorph.a rplogo.a rtmipk.a rphanim.a rpskin.a rtquat.a rtcharse.a rtras.a rtbmp.a rtpng.a rpcollis.a rtintsec.a rppvs.a rprandom.a rpworld.a rtworld.a rwcore.a rtanim.a /nologo /pdb:none /debug /machine:IX86 /out:"DesignRelease\empty_framework.elf" /libpath:"$(DOLPHIN_ROOT)\HW2\lib" /libpath:"$(ODEMUSDKBIN)\..\lib" /libpath:"$(AMCDDKBIN)\..\lib" /libpath:"$(RWGSDK)\lib\gcn\release" /D:SN_TARGET_NGC
# ADD LINK32 odenotstub.a amcstubs.a OdemuExi2.a base.a os.a exi.a si.a db.a mtx.a dvd.a vi.a demo.a pad.a ai.a ar.a ax.a axfx.a mix.a sp.a am.a axart.a syn.a seq.a dsp.a dtk.a card.a hio.a mcc.a gx.a G2D.a texPalette.a fileCache.a support.a perf.a libstl.a rpdmorph.a rplodatm.a rpmatfx.a rpmipkl.a rppatch.a rppatchmatfx.a rppatchskin.a rppatchskinmatfx.a rpprtstd.a rpptank.a rpskinmatfx.a rpspline.a rpusrdat.a rtbezpat.a rtimport.a rtmipkmatfx.a rtmipkpatch.a rtmipkpatchmatfx.a rtpick.a rtpitexd.a rtray.a rtslerp.a rtsplpvs.a rttiff.a rttilerd.a rttoc.a rt2d.a rt2danim.a rpltmap.a rtltmap.a rtbary.a rpmorph.a rplogo.a rtmipk.a rphanim.a rtquat.a rtcharse.a rtras.a rtbmp.a rtpng.a rpcollis.a rtintsec.a rppvs.a rprandom.a rpworld.a rtworld.a rwcore.a rtanim.a rtfsyst.a /nologo /pdb:none /debug /machine:IX86 /out:"..\bin\GDEV_Design_Release\empty_framework.elf" /libpath:"$(RWGSDK)\lib\gcn\release" /libpath:"$(DOLPHIN_ROOT)\HW2\lib" /libpath:"$(ODEMUSDKBIN)\..\lib" /D:SN_TARGET_NGC

!ELSEIF  "$(CFG)" == "empty_framework - Win32 NGC GDEV Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "empty_framework___Win32_NGC_GDEV_Debug"
# PROP BASE Intermediate_Dir "empty_framework___Win32_NGC_GDEV_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "GDEV_Debug"
# PROP Intermediate_Dir "GDEV_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "../../source/" /D "HW2" /D "_DEBUG" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWDEBUG" /D "RWTERMINAL" /D "DEBUG" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /FD /debug /c
# ADD CPP /nologo /W3 /Od /I "..\..\..\game_framework\source\framework\core\MemoryHandler" /I "../../../game_framework/source" /I "../../source" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /D "_DEBUG" /D "RWDEBUG" /D "DEBUG" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "RWS_DISABLE_MEMORY_CHECKING" /FD -fexceptions /debug /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=snLink.exe
# ADD BASE LINK32 baseD.a osD.a exiD.a siD.a dbD.a mtxD.a dvdD.a viD.a demoD.a padD.a aiD.a arD.a axD.a axfxD.a mixD.a spD.a amD.a axartD.a synD.a seqD.a dspD.a dtkD.a cardD.a hioD.a mccD.a gxD.a G2DD.a texPaletteD.a fileCacheD.a supportD.a perfD.a AmcExi2D.a amcnotstubD.a odemuexi2D.a odenotstubD.a libstl.a rpdmorph.a rplodatm.a rpmatfx.a rpmipkl.a rppatch.a rppatchmatfx.a rppatchskin.a rppatchskinmatfx.a rpprtstd.a rpptank.a rpskinmatfx.a rpspline.a rpusrdat.a rtbezpat.a rtimport.a rtmipkmatfx.a rtmipkpatch.a rtmipkpatchmatfx.a rtpick.a rtpitexd.a rtray.a rtslerp.a rtsplpvs.a rttiff.a rttilerd.a rttoc.a rt2d.a rt2danim.a rpltmap.a rtltmap.a rtbary.a rpmorph.a rplogo.a rtmipk.a rphanim.a rpskin.a rtquat.a rtcharse.a rtras.a rtbmp.a rtpng.a rpcollis.a rtintsec.a rppvs.a rprandom.a rpworld.a rtworld.a rwcore.a rtanim.a /nologo /pdb:none /debug /machine:IX86 /out:"Debug\empty_framework.elf" /libpath:"$(DOLPHIN_ROOT)\HW2\lib" /libpath:"$(ODEMUSDKBIN)\..\lib" /libpath:"$(AMCDDKBIN)\..\lib" /libpath:"$(RWGSDK)\lib\gcn\debug" /D:SN_TARGET_NGC
# ADD LINK32 odenotstubd.a amcstubsd.a OdemuExi2d.a baseD.a osD.a exiD.a siD.a dbD.a mtxD.a dvdD.a viD.a demoD.a padD.a aiD.a arD.a axD.a axfxD.a mixD.a spD.a amD.a axartD.a synD.a seqD.a dspD.a dtkD.a cardD.a hioD.a mccD.a gxD.a G2DD.a texPaletteD.a fileCacheD.a supportD.a perfD.a libstl.a rpdmorph.a rplodatm.a rpmatfx.a rpmipkl.a rppatch.a rppatchmatfx.a rppatchskin.a rppatchskinmatfx.a rpprtstd.a rpptank.a rpskinmatfx.a rpspline.a rpusrdat.a rtbezpat.a rtimport.a rtmipkmatfx.a rtmipkpatch.a rtmipkpatchmatfx.a rtpick.a rtpitexd.a rtray.a rtslerp.a rtsplpvs.a rttiff.a rttilerd.a rttoc.a rt2d.a rt2danim.a rpltmap.a rtltmap.a rtbary.a rpmorph.a rplogo.a rtmipk.a rphanim.a rtquat.a rtcharse.a rtras.a rtbmp.a rtpng.a rpcollis.a rtintsec.a rppvs.a rprandom.a rpworld.a rtworld.a rwcore.a rtanim.a rtfsyst.a /nologo /pdb:none /debug /machine:IX86 /out:"..\bin\GDEV_Debug\empty_framework.elf" /libpath:"$(RWGSDK)\lib\gcn\debug" /libpath:"$(DOLPHIN_ROOT)\HW2\lib" /libpath:"$(ODEMUSDKBIN)\..\lib" /D:SN_TARGET_NGC

!ELSEIF  "$(CFG)" == "empty_framework - Win32 NGC GDEV Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "empty_framework___Win32_NGC_GDEV_Release"
# PROP BASE Intermediate_Dir "empty_framework___Win32_NGC_GDEV_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "GDEV_Release"
# PROP Intermediate_Dir "GDEV_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "../../source/" /D "RWS_DISABLE_MEMORY_CHECKING" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "NDEBUG" /FD /debug /c
# ADD CPP /nologo /W3 /Od /I "..\..\..\game_framework\source\framework\core\MemoryHandler" /I "../../../game_framework/source" /I "../../source" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /D "RWS_DISABLE_MEMORY_CHECKING" /D "NDEBUG" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /FD -fexceptions /debug /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=snLink.exe
# ADD BASE LINK32 base.a os.a exi.a si.a db.a mtx.a dvd.a vi.a demo.a pad.a ai.a ar.a ax.a axfx.a mix.a sp.a am.a axart.a syn.a seq.a dsp.a dtk.a card.a hio.a mcc.a gx.a G2D.a texPalette.a fileCache.a support.a perf.a AmcExi2.a amcnotstub.a odemuexi2.a odenotstub.a libstl.a rpdmorph.a rplodatm.a rpmatfx.a rpmipkl.a rppatch.a rppatchmatfx.a rppatchskin.a rppatchskinmatfx.a rpprtstd.a rpptank.a rpskinmatfx.a rpspline.a rpusrdat.a rtbezpat.a rtimport.a rtmipkmatfx.a rtmipkpatch.a rtmipkpatchmatfx.a rtpick.a rtpitexd.a rtray.a rtslerp.a rtsplpvs.a rttiff.a rttilerd.a rttoc.a rt2d.a rt2danim.a rpltmap.a rtltmap.a rtbary.a rpmorph.a rplogo.a rtmipk.a rphanim.a rpskin.a rtquat.a rtcharse.a rtras.a rtbmp.a rtpng.a rpcollis.a rtintsec.a rppvs.a rprandom.a rpworld.a rtworld.a rwcore.a rtanim.a /nologo /pdb:none /debug /machine:IX86 /out:"Release\empty_framework.elf" /libpath:"$(DOLPHIN_ROOT)\HW2\lib" /libpath:"$(ODEMUSDKBIN)\..\lib" /libpath:"$(AMCDDKBIN)\..\lib" /libpath:"$(RWGSDK)\lib\gcn\release" /D:SN_TARGET_NGC
# ADD LINK32 odenotstub.a amcstubs.a OdemuExi2.a base.a os.a exi.a si.a db.a mtx.a dvd.a vi.a demo.a pad.a ai.a ar.a ax.a axfx.a mix.a sp.a am.a axart.a syn.a seq.a dsp.a dtk.a card.a hio.a mcc.a gx.a G2D.a texPalette.a fileCache.a support.a perf.a libstl.a rpdmorph.a rplodatm.a rpmatfx.a rpmipkl.a rppatch.a rppatchmatfx.a rppatchskin.a rppatchskinmatfx.a rpprtstd.a rpptank.a rpskinmatfx.a rpspline.a rpusrdat.a rtbezpat.a rtimport.a rtmipkmatfx.a rtmipkpatch.a rtmipkpatchmatfx.a rtpick.a rtpitexd.a rtray.a rtslerp.a rtsplpvs.a rttiff.a rttilerd.a rttoc.a rt2d.a rt2danim.a rpltmap.a rtltmap.a rtbary.a rpmorph.a rplogo.a rtmipk.a rphanim.a rtquat.a rtcharse.a rtras.a rtbmp.a rtpng.a rpcollis.a rtintsec.a rppvs.a rprandom.a rpworld.a rtworld.a rwcore.a rtanim.a rtfsyst.a /nologo /pdb:none /debug /machine:IX86 /out:"..\bin\GDEV_Release\empty_framework.elf" /libpath:"$(RWGSDK)\lib\gcn\release" /libpath:"$(DOLPHIN_ROOT)\HW2\lib" /libpath:"$(ODEMUSDKBIN)\..\lib" /D:SN_TARGET_NGC

!ELSEIF  "$(CFG)" == "empty_framework - Win32 NGC GDEV DesignDebug Broadband"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "empty_framework___Win32_NGC_GDEV_DesignDebug_Broadband"
# PROP BASE Intermediate_Dir "empty_framework___Win32_NGC_GDEV_DesignDebug_Broadband"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "GDEV_Design_Debug_Broadband"
# PROP Intermediate_Dir "GDEV_Design_Debug_Broadband"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "HW2" /D "_DEBUG" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWDEBUG" /D "RWTERMINAL" /D "DEBUG" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /FD /debug /c
# ADD CPP /nologo /W3 /GX /Od /I "..\..\..\game_framework\source\framework\core\MemoryHandler" /I "../../../game_framework/source" /I "../../source" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /D "_DEBUG" /D "RWDEBUG" /D "DEBUG" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "RWS_BROADBAND" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "RWS_FUNCTIONPROFILE" /FD -fexceptions /debug /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=snLink.exe
# ADD BASE LINK32 odenotstubd.a amcstubsd.a OdemuExi2d.a baseD.a osD.a exiD.a siD.a dbD.a mtxD.a dvdD.a viD.a demoD.a padD.a aiD.a arD.a axD.a axfxD.a mixD.a spD.a amD.a axartD.a synD.a seqD.a dspD.a dtkD.a cardD.a hioD.a mccD.a gxD.a G2DD.a texPaletteD.a fileCacheD.a supportD.a perfD.a libstl.a rpdmorph.a rplodatm.a rpmatfx.a rpmipkl.a rppatch.a rppatchmatfx.a rppatchskin.a rppatchskinmatfx.a rpprtstd.a rpptank.a rpskinmatfx.a rpspline.a rpusrdat.a rtbezpat.a rtimport.a rtmipkmatfx.a rtmipkpatch.a rtmipkpatchmatfx.a rtpick.a rtpitexd.a rtray.a rtslerp.a rtsplpvs.a rttiff.a rttilerd.a rttoc.a rt2d.a rt2danim.a rpltmap.a rtltmap.a rtbary.a rpmorph.a rplogo.a rtmipk.a rphanim.a rpskin.a rtquat.a rtcharse.a rtras.a rtbmp.a rtpng.a rpcollis.a rtintsec.a rppvs.a rprandom.a rpworld.a rtworld.a rwcore.a rtanim.a /nologo /pdb:none /debug /machine:IX86 /out:"..\bin\GDEV_Design_Debug\empty_framework.elf" /libpath:"$(RWGSDK)\lib\gcn\debug" /libpath:"$(DOLPHIN_ROOT)\HW2\lib" /libpath:"$(ODEMUSDKBIN)\..\lib" /D:SN_TARGET_NGC
# ADD LINK32 odenotstubd.a amcstubsd.a OdemuExi2d.a baseD.a osD.a exiD.a siD.a dbD.a mtxD.a dvdD.a viD.a demoD.a padD.a aiD.a arD.a axD.a axfxD.a mixD.a spD.a amD.a axartD.a synD.a seqD.a dspD.a dtkD.a cardD.a hioD.a mccD.a gxD.a G2DD.a texPaletteD.a fileCacheD.a supportD.a perfD.a ipd.a ethd.a libstl.a rpdmorph.a rplodatm.a rpmatfx.a rpmipkl.a rppatch.a rppatchmatfx.a rppatchskin.a rppatchskinmatfx.a rpprtstd.a rpptank.a rpskinmatfx.a rpspline.a rpusrdat.a rtbezpat.a rtimport.a rtmipkmatfx.a rtmipkpatch.a rtmipkpatchmatfx.a rtpick.a rtpitexd.a rtray.a rtslerp.a rtsplpvs.a rttiff.a rttilerd.a rttoc.a rt2d.a rt2danim.a rpltmap.a rtltmap.a rtbary.a rpmorph.a rplogo.a rtmipk.a rphanim.a rtquat.a rtcharse.a rtras.a rtbmp.a rtpng.a rpcollis.a rtintsec.a rppvs.a rprandom.a rpworld.a rtworld.a rwcore.a rtanim.a rtfsyst.a /nologo /pdb:none /debug /machine:IX86 /out:"..\bin\GDEV_Design_Debug_Broadband\empty_framework.elf" /libpath:"$(RWGSDK)\lib\gcn\debug" /libpath:"$(DOLPHIN_ROOT)\HW2\lib" /libpath:"$(ODEMUSDKBIN)\..\lib" /D:SN_TARGET_NGC

!ELSEIF  "$(CFG)" == "empty_framework - Win32 NGC GDEV DesignRelease Broadband"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "empty_framework___Win32_NGC_GDEV_DesignRelease_Broadband"
# PROP BASE Intermediate_Dir "empty_framework___Win32_NGC_GDEV_DesignRelease_Broadband"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "GDEV_Design_Release_Broadband"
# PROP Intermediate_Dir "GDEV_Design_Release_Broadband"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "NDEBUG" /D "RWS_DISABLE_MEMORY_CHECKING" /FD /debug /c
# ADD CPP /nologo /W3 /GX /Od /I "..\..\..\game_framework\source\framework\core\MemoryHandler" /I "../../../game_framework/source" /I "../../source" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "NDEBUG" /D "RWS_DISABLE_MEMORY_CHECKING" /D "RWS_BROADBAND" /D "RWS_FUNCTIONPROFILE" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /FD -fexceptions /debug /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=snLink.exe
# ADD BASE LINK32 odenotstub.a amcstubs.a OdemuExi2.a base.a os.a exi.a si.a db.a mtx.a dvd.a vi.a demo.a pad.a ai.a ar.a ax.a axfx.a mix.a sp.a am.a axart.a syn.a seq.a dsp.a dtk.a card.a hio.a mcc.a gx.a G2D.a texPalette.a fileCache.a support.a perf.a libstl.a rpdmorph.a rplodatm.a rpmatfx.a rpmipkl.a rppatch.a rppatchmatfx.a rppatchskin.a rppatchskinmatfx.a rpprtstd.a rpptank.a rpskinmatfx.a rpspline.a rpusrdat.a rtbezpat.a rtimport.a rtmipkmatfx.a rtmipkpatch.a rtmipkpatchmatfx.a rtpick.a rtpitexd.a rtray.a rtslerp.a rtsplpvs.a rttiff.a rttilerd.a rttoc.a rt2d.a rt2danim.a rpltmap.a rtltmap.a rtbary.a rpmorph.a rplogo.a rtmipk.a rphanim.a rpskin.a rtquat.a rtcharse.a rtras.a rtbmp.a rtpng.a rpcollis.a rtintsec.a rppvs.a rprandom.a rpworld.a rtworld.a rwcore.a rtanim.a /nologo /pdb:none /debug /machine:IX86 /out:"..\bin\GDEV_Design_Release\empty_framework.elf" /libpath:"$(RWGSDK)\lib\gcn\release" /libpath:"$(DOLPHIN_ROOT)\HW2\lib" /libpath:"$(ODEMUSDKBIN)\..\lib" /D:SN_TARGET_NGC
# ADD LINK32 odenotstub.a amcstubs.a OdemuExi2.a base.a os.a exi.a si.a db.a mtx.a dvd.a vi.a demo.a pad.a ai.a ar.a ax.a axfx.a mix.a sp.a am.a axart.a syn.a seq.a dsp.a dtk.a card.a hio.a mcc.a gx.a G2D.a texPalette.a fileCache.a support.a perf.a ip.a eth.a libstl.a rpdmorph.a rplodatm.a rpmatfx.a rpmipkl.a rppatch.a rppatchmatfx.a rppatchskin.a rppatchskinmatfx.a rpprtstd.a rpptank.a rpskinmatfx.a rpspline.a rpusrdat.a rtbezpat.a rtimport.a rtmipkmatfx.a rtmipkpatch.a rtmipkpatchmatfx.a rtpick.a rtpitexd.a rtray.a rtslerp.a rtsplpvs.a rttiff.a rttilerd.a rttoc.a rt2d.a rt2danim.a rpltmap.a rtltmap.a rtbary.a rpmorph.a rplogo.a rtmipk.a rphanim.a rtquat.a rtcharse.a rtras.a rtbmp.a rtpng.a rpcollis.a rtintsec.a rppvs.a rprandom.a rpworld.a rtworld.a rwcore.a rtanim.a rtfsyst.a /nologo /pdb:none /debug /machine:IX86 /out:"..\bin\GDEV_Design_Release_Broadband\empty_framework.elf" /libpath:"$(RWGSDK)\lib\gcn\release" /libpath:"$(DOLPHIN_ROOT)\HW2\lib" /libpath:"$(ODEMUSDKBIN)\..\lib" /D:SN_TARGET_NGC

!ELSEIF  "$(CFG)" == "empty_framework - Win32 NGC DDH DesignDebug Broadband"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "empty_framework___Win32_NGC_DDH_DesignDebug_Broadband"
# PROP BASE Intermediate_Dir "empty_framework___Win32_NGC_DDH_DesignDebug_Broadband"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DDH_Design_Debug_Broadband"
# PROP Intermediate_Dir "DDH_Design_Debug_Broadband"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "HW2" /D "_DEBUG" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWDEBUG" /D "RWTERMINAL" /D "DEBUG" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /FD /debug /c
# ADD CPP /nologo /W3 /GX /Od /I "..\..\..\game_framework\source\framework\core\MemoryHandler" /I "../../../game_framework/source" /I "../../source" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /D "_DEBUG" /D "RWDEBUG" /D "DEBUG" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "RWS_BROADBAND" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "RWS_FUNCTIONPROFILE" /FD -fexceptions /debug /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=snLink.exe
# ADD BASE LINK32 odemustubsd.a amcnotstubd.a amcExi2d.a baseD.a osD.a exiD.a siD.a dbD.a mtxD.a dvdD.a viD.a demoD.a padD.a aiD.a arD.a axD.a axfxD.a mixD.a spD.a amD.a axartD.a synD.a seqD.a dspD.a dtkD.a cardD.a hioD.a mccD.a gxD.a G2DD.a texPaletteD.a fileCacheD.a supportD.a perfD.a libstl.a rpdmorph.a rplodatm.a rpmatfx.a rpmipkl.a rppatch.a rppatchmatfx.a rppatchskin.a rppatchskinmatfx.a rpprtstd.a rpptank.a rpskinmatfx.a rpspline.a rpusrdat.a rtbezpat.a rtimport.a rtmipkmatfx.a rtmipkpatch.a rtmipkpatchmatfx.a rtpick.a rtpitexd.a rtray.a rtslerp.a rtsplpvs.a rttiff.a rttilerd.a rttoc.a rt2d.a rt2danim.a rpltmap.a rtltmap.a rtbary.a rpmorph.a rplogo.a rtmipk.a rphanim.a rpskin.a rtquat.a rtcharse.a rtras.a rtbmp.a rtpng.a rpcollis.a rtintsec.a rppvs.a rprandom.a rpworld.a rtworld.a rwcore.a rtanim.a /nologo /pdb:none /debug /machine:IX86 /out:"..\bin\DDH_Design_Debug\empty_framework.elf" /libpath:"$(DOLPHIN_ROOT)\HW2\lib" /libpath:"$(AMCDDKBIN)\..\lib" /libpath:"$(RWGSDK)\lib\gcn\debug" /D:SN_TARGET_NGC
# ADD LINK32 odemustubsd.a amcnotstubd.a amcExi2d.a baseD.a osD.a exiD.a siD.a dbD.a mtxD.a dvdD.a viD.a demoD.a padD.a aiD.a arD.a axD.a axfxD.a mixD.a spD.a amD.a axartD.a synD.a seqD.a dspD.a dtkD.a cardD.a hioD.a mccD.a gxD.a G2DD.a texPaletteD.a fileCacheD.a supportD.a perfD.a ipd.a ethd.a libstl.a rpdmorph.a rplodatm.a rpmatfx.a rpmipkl.a rppatch.a rppatchmatfx.a rppatchskin.a rppatchskinmatfx.a rpprtstd.a rpptank.a rpskinmatfx.a rpspline.a rpusrdat.a rtbezpat.a rtimport.a rtmipkmatfx.a rtmipkpatch.a rtmipkpatchmatfx.a rtpick.a rtpitexd.a rtray.a rtslerp.a rtsplpvs.a rttiff.a rttilerd.a rttoc.a rt2d.a rt2danim.a rpltmap.a rtltmap.a rtbary.a rpmorph.a rplogo.a rtmipk.a rphanim.a rtquat.a rtcharse.a rtras.a rtbmp.a rtpng.a rpcollis.a rtintsec.a rppvs.a rprandom.a rpworld.a rtworld.a rwcore.a rtanim.a rtfsyst.a /nologo /pdb:none /debug /machine:IX86 /out:"..\bin\DDH_Design_Debug_Broadband\empty_framework.elf" /libpath:"$(DOLPHIN_ROOT)\HW2\lib" /libpath:"$(AMCDDKBIN)\..\lib" /libpath:"$(RWGSDK)\lib\gcn\debug" /D:SN_TARGET_NGC

!ELSEIF  "$(CFG)" == "empty_framework - Win32 NGC DDH DesignRelease Broadband"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "empty_framework___Win32_NGC_DDH_DesignRelease_Broadband"
# PROP BASE Intermediate_Dir "empty_framework___Win32_NGC_DDH_DesignRelease_Broadband"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DDH_Design_Release_Broadband"
# PROP Intermediate_Dir "DDH_Design_Release_Broadband"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "NDEBUG" /D "RWS_DISABLE_MEMORY_CHECKING" /FD /debug /c
# ADD CPP /nologo /W3 /GX /Od /I "..\..\..\game_framework\source\framework\core\MemoryHandler" /I "../../../game_framework/source" /I "../../source" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "NDEBUG" /D "RWS_DISABLE_MEMORY_CHECKING" /D "RWS_BROADBAND" /D "RWS_FUNCTIONPROFILE" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /FD -fexceptions /debug /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=snLink.exe
# ADD BASE LINK32 odemustubs.a amcnotstub.a amcExi2.a base.a os.a exi.a si.a db.a mtx.a dvd.a vi.a demo.a pad.a ai.a ar.a ax.a axfx.a mix.a sp.a am.a axart.a syn.a seq.a dsp.a dtk.a card.a hio.a mcc.a gx.a G2D.a texPalette.a fileCache.a support.a perf.a libstl.a rpdmorph.a rplodatm.a rpmatfx.a rpmipkl.a rppatch.a rppatchmatfx.a rppatchskin.a rppatchskinmatfx.a rpprtstd.a rpptank.a rpskinmatfx.a rpspline.a rpusrdat.a rtbezpat.a rtimport.a rtmipkmatfx.a rtmipkpatch.a rtmipkpatchmatfx.a rtpick.a rtpitexd.a rtray.a rtslerp.a rtsplpvs.a rttiff.a rttilerd.a rttoc.a rt2d.a rt2danim.a rpltmap.a rtltmap.a rtbary.a rpmorph.a rplogo.a rtmipk.a rphanim.a rpskin.a rtquat.a rtcharse.a rtras.a rtbmp.a rtpng.a rpcollis.a rtintsec.a rppvs.a rprandom.a rpworld.a rtworld.a rwcore.a rtanim.a /nologo /pdb:none /debug /machine:IX86 /out:"..\bin\DDH_Design_Release\empty_framework.elf" /libpath:"$(RWGSDK)\lib\gcn\release" /libpath:"$(DOLPHIN_ROOT)\HW2\lib" /libpath:"$(AMCDDKBIN)\..\lib" /D:SN_TARGET_NGC
# ADD LINK32 odemustubs.a amcnotstub.a amcExi2.a base.a os.a exi.a si.a db.a mtx.a dvd.a vi.a demo.a pad.a ai.a ar.a ax.a axfx.a mix.a sp.a am.a axart.a syn.a seq.a dsp.a dtk.a card.a hio.a mcc.a gx.a G2D.a texPalette.a fileCache.a support.a perf.a ip.a eth.a libstl.a rpdmorph.a rplodatm.a rpmatfx.a rpmipkl.a rppatch.a rppatchmatfx.a rppatchskin.a rppatchskinmatfx.a rpprtstd.a rpptank.a rpskinmatfx.a rpspline.a rpusrdat.a rtbezpat.a rtimport.a rtmipkmatfx.a rtmipkpatch.a rtmipkpatchmatfx.a rtpick.a rtpitexd.a rtray.a rtslerp.a rtsplpvs.a rttiff.a rttilerd.a rttoc.a rt2d.a rt2danim.a rpltmap.a rtltmap.a rtbary.a rpmorph.a rplogo.a rtmipk.a rphanim.a rtquat.a rtcharse.a rtras.a rtbmp.a rtpng.a rpcollis.a rtintsec.a rppvs.a rprandom.a rpworld.a rtworld.a rwcore.a rtanim.a rtfsyst.a /nologo /pdb:none /debug /machine:IX86 /out:"..\bin\DDH_Design_Release_Broadband\empty_framework.elf" /libpath:"$(RWGSDK)\lib\gcn\release" /libpath:"$(DOLPHIN_ROOT)\HW2\lib" /libpath:"$(AMCDDKBIN)\..\lib" /D:SN_TARGET_NGC

!ENDIF 

# Begin Target

# Name "empty_framework - Win32 NGC DDH DesignDebug"
# Name "empty_framework - Win32 NGC DDH DesignRelease"
# Name "empty_framework - Win32 NGC DDH Debug"
# Name "empty_framework - Win32 NGC DDH Release"
# Name "empty_framework - Win32 NGC GDEV DesignDebug"
# Name "empty_framework - Win32 NGC GDEV DesignRelease"
# Name "empty_framework - Win32 NGC GDEV Debug"
# Name "empty_framework - Win32 NGC GDEV Release"
# Name "empty_framework - Win32 NGC GDEV DesignDebug Broadband"
# Name "empty_framework - Win32 NGC GDEV DesignRelease Broadband"
# Name "empty_framework - Win32 NGC DDH DesignDebug Broadband"
# Name "empty_framework - Win32 NGC DDH DesignRelease Broadband"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "modules"

# PROP Default_Filter ""
# Begin Group "Generic"

# PROP Default_Filter ""
# Begin Group "CEntity"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\modules\generic\CEntity\CEntity.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\Generic\CEntity\CEntity.h
# End Source File
# End Group
# End Group
# End Group
# Begin Group "toolkits"

# PROP Default_Filter ""
# Begin Group "RwGfxResource"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\game_framework\source\framework\toolkits\rwgfxresource\rwgfxresource.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\game_framework\source\framework\toolkits\rwgfxresource\rwgfxresource.h
# End Source File
# End Group
# Begin Group "Time"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\game_framework\source\framework\toolkits\time\gftime.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\game_framework\source\framework\toolkits\time\gftime.h
# End Source File
# End Group
# Begin Group "World"

# PROP Default_Filter ""
# Begin Group "Helpers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\game_framework\source\framework\toolkits\world\helpers\camerahelper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\game_framework\source\framework\toolkits\world\helpers\camerahelper.h
# End Source File
# Begin Source File

SOURCE=..\..\..\game_framework\source\framework\toolkits\world\helpers\framehelper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\game_framework\source\framework\toolkits\world\helpers\framehelper.h
# End Source File
# End Group
# Begin Group "SmartPtrs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\game_framework\source\framework\toolkits\world\smartptrs\ccameraptr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\game_framework\source\framework\toolkits\world\smartptrs\ccameraptr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\game_framework\source\framework\toolkits\world\smartptrs\crwobjectptr.h
# End Source File
# End Group
# End Group
# Begin Group "EventVisualizer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\game_framework\source\framework\toolkits\eventvisualization\eventvisualization.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\game_framework\source\framework\toolkits\eventvisualization\eventvisualization.h
# End Source File
# End Group
# Begin Group "DebugTools"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\game_framework\source\framework\toolkits\debugtools\debugswitches.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\game_framework\source\framework\toolkits\debugtools\debugswitches.h
# End Source File
# Begin Source File

SOURCE=..\..\..\game_framework\source\framework\toolkits\debugtools\debugtools.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\game_framework\source\framework\toolkits\debugtools\debugtools.h
# End Source File
# End Group
# End Group
# Begin Group "MainLoop"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\framework\mainloop\connectionscreen.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\mainloop\connectionscreen.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\mainloop\logic.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\mainloop\logic.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\mainloop\mainloop.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\mainloop\mainloop.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\mainloop\network.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\mainloop\network.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\mainloop\render.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\mainloop\render.h
# End Source File
# End Group
# Begin Group "StartUp"

# PROP Default_Filter ""
# Begin Group "GCN"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\framework\startup\gcn\dlfs.c
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\startup\gcn\dlfs.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\startup\gcn\dlmem.c
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\startup\gcn\dlmem.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\startup\gcn\gcn.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\startup\gcn\gcn.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\source\framework\startup\attachplugins.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\startup\attachplugins.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\startup\debugmsghandler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\startup\debugmsghandler.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\startup\loadgamedatabase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\startup\loadgamedatabase.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\startup\registerimageloaders.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\startup\registerimageloaders.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\startup\startup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\startup\startup.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\startup\videomode.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\startup\videomode.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\missing.c
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
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\ngc.ld
# End Source File
# End Target
# End Project
