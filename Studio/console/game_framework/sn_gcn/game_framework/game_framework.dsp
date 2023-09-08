# Microsoft Developer Studio Project File - Name="game_framework" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 60000
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=game_framework - Win32 NGC DDH DesignRelease Broadband
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "game_framework.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "game_framework.mak" CFG="game_framework - Win32 NGC DDH DesignRelease Broadband"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "game_framework - Win32 NGC DDH DesignDebug" (based on "Win32 (x86) Console Application")
!MESSAGE "game_framework - Win32 NGC DDH DesignRelease" (based on "Win32 (x86) Console Application")
!MESSAGE "game_framework - Win32 NGC DDH Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "game_framework - Win32 NGC DDH Release" (based on "Win32 (x86) Console Application")
!MESSAGE "game_framework - Win32 NGC GDEV DesignDebug" (based on "Win32 (x86) Console Application")
!MESSAGE "game_framework - Win32 NGC GDEV DesignRelease" (based on "Win32 (x86) Console Application")
!MESSAGE "game_framework - Win32 NGC GDEV Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "game_framework - Win32 NGC GDEV Release" (based on "Win32 (x86) Console Application")
!MESSAGE "game_framework - Win32 NGC GDEV DesignDebug Broadband" (based on "Win32 (x86) Console Application")
!MESSAGE "game_framework - Win32 NGC GDEV DesignRelease Broadband" (based on "Win32 (x86) Console Application")
!MESSAGE "game_framework - Win32 NGC DDH DesignDebug Broadband" (based on "Win32 (x86) Console Application")
!MESSAGE "game_framework - Win32 NGC DDH DesignRelease Broadband" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "game_framework"
# PROP Scc_LocalPath "."
CPP=snCl.exe
RSC=rc.exe

!IF  "$(CFG)" == "game_framework - Win32 NGC DDH DesignDebug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "game_framework___Win32_NGC_DDH_DesignDebug"
# PROP BASE Intermediate_Dir "game_framework___Win32_NGC_DDH_DesignDebug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DDH_Design_Debug"
# PROP Intermediate_Dir "DDH_Design_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "../../source/" /D "HW2" /D "_DEBUG" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWDEBUG" /D "RWTERMINAL" /D "DEBUG" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /FD /debug /c
# ADD CPP /nologo /W3 /GX /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "_DEBUG" /D "RWDEBUG" /D "DEBUG" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "RWS_MEMORY_PROFILING" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "RWS_FUNCTIONPROFILE" /FD /debug -fexceptions /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=snLink.exe
# ADD BASE LINK32 baseD.a osD.a exiD.a siD.a dbD.a mtxD.a dvdD.a viD.a demoD.a padD.a aiD.a arD.a axD.a axfxD.a mixD.a spD.a amD.a axartD.a synD.a seqD.a dspD.a dtkD.a cardD.a hioD.a mccD.a gxD.a G2DD.a texPaletteD.a fileCacheD.a supportD.a perfD.a AmcExi2D.a amcnotstubD.a odemuexi2D.a odenotstubD.a libstl.a rpdmorph.a rplodatm.a rpmatfx.a rpmipkl.a rppatch.a rppatchmatfx.a rppatchskin.a rppatchskinmatfx.a rpprtstd.a rpptank.a rpskinmatfx.a rpspline.a rpusrdat.a rtbezpat.a rtimport.a rtmipkmatfx.a rtmipkpatch.a rtmipkpatchmatfx.a rtpick.a rtpitexd.a rtray.a rtslerp.a rtsplpvs.a rttiff.a rttilerd.a rttoc.a rt2d.a rt2danim.a rpltmap.a rtltmap.a rtbary.a rpmorph.a rplogo.a rtmipk.a rphanim.a rpskin.a rtquat.a rtcharse.a rtras.a rtbmp.a rtpng.a rpcollis.a rtintsec.a rppvs.a rprandom.a rpworld.a rtworld.a rwcore.a rtanim.a libmcdconvex.a libmeassetfactory.a libmcdconvexcreatehull.a libmst.a libmcdframe.a libmcdprimitives.a libmdt.a libmdtarthur.a libmdtbcl.a libmdtkea.a libmdtsolverutils.a libmeapp.a libmeassetdb.a libmeassetdbxmlio.a libmeglobals.a libmexml.a libmcdcommon.a /nologo /pdb:none /debug /machine:IX86 /out:"DesignDebug\game_framework.elf" /libpath:"$(DOLPHIN_ROOT)\HW2\lib" /libpath:"$(ODEMUSDKBIN)\..\lib" /libpath:"$(AMCDDKBIN)\..\lib" /libpath:"$(RWGSDK)\lib\gcn\debug" /D:SN_TARGET_NGC
# ADD LINK32 odemustubsd.a amcnotstubd.a amcExi2d.a baseD.a osD.a exiD.a siD.a dbD.a mtxD.a dvdD.a viD.a demoD.a padD.a aiD.a arD.a axD.a axfxD.a mixD.a spD.a amD.a axartD.a synD.a seqD.a dspD.a dtkD.a cardD.a hioD.a mccD.a gxD.a G2DD.a texPaletteD.a fileCacheD.a supportD.a perfD.a libstl.a rpdmorph.a rplodatm.a rpmatfx.a rpmipkl.a rppatch.a rppatchmatfx.a rppatchskin.a rppatchskinmatfx.a rpprtstd.a rpptank.a rpskinmatfx.a rpspline.a rpusrdat.a rtbezpat.a rtimport.a rtmipkmatfx.a rtmipkpatch.a rtmipkpatchmatfx.a rtpick.a rtpitexd.a rtray.a rtslerp.a rtsplpvs.a rttiff.a rttilerd.a rttoc.a rt2d.a rt2danim.a rpltmap.a rtltmap.a rtbary.a rpmorph.a rplogo.a rtmipk.a rphanim.a rtquat.a rtcharse.a rtras.a rtbmp.a rtpng.a rpcollis.a rtintsec.a rppvs.a rprandom.a rpworld.a rtworld.a rwcore.a rtanim.a rtfsyst.a rtcmpkey.a /nologo /pdb:none /debug /machine:IX86 /out:"..\bin\DDH_Design_Debug\game_framework.elf" /libpath:"$(DOLPHIN_ROOT)\HW2\lib" /libpath:"$(AMCDDKBIN)\..\lib" /libpath:"$(RWGSDK)\lib\gcn\debug" /D:SN_TARGET_NGC

!ELSEIF  "$(CFG)" == "game_framework - Win32 NGC DDH DesignRelease"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "game_framework___Win32_NGC_DDH_DesignRelease"
# PROP BASE Intermediate_Dir "game_framework___Win32_NGC_DDH_DesignRelease"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DDH_Design_Release"
# PROP Intermediate_Dir "DDH_Design_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "../../source/" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "NDEBUG" /D "RWS_DISABLE_MEMORY_CHECKING" /FD /debug /c
# ADD CPP /nologo /W3 /GX /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "NDEBUG" /D "RWS_DISABLE_MEMORY_CHECKING" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "RWS_MEMORY_PROFILING" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "RWS_FUNCTIONPROFILE" /FD -fexceptions /debug /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=snLink.exe
# ADD BASE LINK32 base.a os.a exi.a si.a db.a mtx.a dvd.a vi.a demo.a pad.a ai.a ar.a ax.a axfx.a mix.a sp.a am.a axart.a syn.a seq.a dsp.a dtk.a card.a hio.a mcc.a gx.a G2D.a texPalette.a fileCache.a support.a perf.a AmcExi2.a amcnotstub.a odemuexi2.a odenotstub.a libstl.a rpdmorph.a rplodatm.a rpmatfx.a rpmipkl.a rppatch.a rppatchmatfx.a rppatchskin.a rppatchskinmatfx.a rpprtstd.a rpptank.a rpskinmatfx.a rpspline.a rpusrdat.a rtbezpat.a rtimport.a rtmipkmatfx.a rtmipkpatch.a rtmipkpatchmatfx.a rtpick.a rtpitexd.a rtray.a rtslerp.a rtsplpvs.a rttiff.a rttilerd.a rttoc.a rt2d.a rt2danim.a rpltmap.a rtltmap.a rtbary.a rpmorph.a rplogo.a rtmipk.a rphanim.a rpskin.a rtquat.a rtcharse.a rtras.a rtbmp.a rtpng.a rpcollis.a rtintsec.a rppvs.a rprandom.a rpworld.a rtworld.a rwcore.a rtanim.a /nologo /pdb:none /debug /machine:IX86 /out:"DesignRelease\game_framework.elf" /libpath:"$(DOLPHIN_ROOT)\HW2\lib" /libpath:"$(ODEMUSDKBIN)\..\lib" /libpath:"$(AMCDDKBIN)\..\lib" /libpath:"$(RWGSDK)\lib\gcn\release" /D:SN_TARGET_NGC
# ADD LINK32 odemustubs.a amcnotstub.a amcExi2.a base.a os.a exi.a si.a db.a mtx.a dvd.a vi.a demo.a pad.a ai.a ar.a ax.a axfx.a mix.a sp.a am.a axart.a syn.a seq.a dsp.a dtk.a card.a hio.a mcc.a gx.a G2D.a texPalette.a fileCache.a support.a perf.a libstl.a rpdmorph.a rplodatm.a rpmatfx.a rpmipkl.a rppatch.a rppatchmatfx.a rppatchskin.a rppatchskinmatfx.a rpprtstd.a rpptank.a rpskinmatfx.a rpspline.a rpusrdat.a rtbezpat.a rtimport.a rtmipkmatfx.a rtmipkpatch.a rtmipkpatchmatfx.a rtpick.a rtpitexd.a rtray.a rtslerp.a rtsplpvs.a rttiff.a rttilerd.a rttoc.a rt2d.a rt2danim.a rpltmap.a rtltmap.a rtbary.a rpmorph.a rplogo.a rtmipk.a rphanim.a rtquat.a rtcharse.a rtras.a rtbmp.a rtpng.a rpcollis.a rtintsec.a rppvs.a rprandom.a rpworld.a rtworld.a rwcore.a rtanim.a rtfsyst.a rtcmpkey.a /nologo /pdb:none /debug /machine:IX86 /out:"..\bin\DDH_Design_Release\game_framework.elf" /libpath:"$(RWGSDK)\lib\gcn\release" /libpath:"$(DOLPHIN_ROOT)\HW2\lib" /libpath:"$(AMCDDKBIN)\..\lib" /D:SN_TARGET_NGC

!ELSEIF  "$(CFG)" == "game_framework - Win32 NGC DDH Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "game_framework___Win32_NGC_DDH_Debug"
# PROP BASE Intermediate_Dir "game_framework___Win32_NGC_DDH_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DDH_Debug"
# PROP Intermediate_Dir "DDH_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "../../source/" /D "HW2" /D "_DEBUG" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWDEBUG" /D "RWTERMINAL" /D "DEBUG" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /FD /debug /c
# ADD CPP /nologo /W3 /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "_DEBUG" /D "RWDEBUG" /D "DEBUG" /D "RWS_DISABLE_MEMORY_CHECKING" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /FD -fexceptions /debug /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=snLink.exe
# ADD BASE LINK32 baseD.a osD.a exiD.a siD.a dbD.a mtxD.a dvdD.a viD.a demoD.a padD.a aiD.a arD.a axD.a axfxD.a mixD.a spD.a amD.a axartD.a synD.a seqD.a dspD.a dtkD.a cardD.a hioD.a mccD.a gxD.a G2DD.a texPaletteD.a fileCacheD.a supportD.a perfD.a AmcExi2D.a amcnotstubD.a odemuexi2D.a odenotstubD.a libstl.a rpdmorph.a rplodatm.a rpmatfx.a rpmipkl.a rppatch.a rppatchmatfx.a rppatchskin.a rppatchskinmatfx.a rpprtstd.a rpptank.a rpskinmatfx.a rpspline.a rpusrdat.a rtbezpat.a rtimport.a rtmipkmatfx.a rtmipkpatch.a rtmipkpatchmatfx.a rtpick.a rtpitexd.a rtray.a rtslerp.a rtsplpvs.a rttiff.a rttilerd.a rttoc.a rt2d.a rt2danim.a rpltmap.a rtltmap.a rtbary.a rpmorph.a rplogo.a rtmipk.a rphanim.a rpskin.a rtquat.a rtcharse.a rtras.a rtbmp.a rtpng.a rpcollis.a rtintsec.a rppvs.a rprandom.a rpworld.a rtworld.a rwcore.a rtanim.a /nologo /pdb:none /debug /machine:IX86 /out:"Debug\game_framework.elf" /libpath:"$(DOLPHIN_ROOT)\HW2\lib" /libpath:"$(ODEMUSDKBIN)\..\lib" /libpath:"$(AMCDDKBIN)\..\lib" /libpath:"$(RWGSDK)\lib\gcn\debug" /D:SN_TARGET_NGC
# ADD LINK32 odemustubsd.a amcnotstubd.a amcExi2d.a baseD.a osD.a exiD.a siD.a dbD.a mtxD.a dvdD.a viD.a demoD.a padD.a aiD.a arD.a axD.a axfxD.a mixD.a spD.a amD.a axartD.a synD.a seqD.a dspD.a dtkD.a cardD.a hioD.a mccD.a gxD.a G2DD.a texPaletteD.a fileCacheD.a supportD.a perfD.a libstl.a rpdmorph.a rplodatm.a rpmatfx.a rpmipkl.a rppatch.a rppatchmatfx.a rppatchskin.a rppatchskinmatfx.a rpprtstd.a rpptank.a rpskinmatfx.a rpspline.a rpusrdat.a rtbezpat.a rtimport.a rtmipkmatfx.a rtmipkpatch.a rtmipkpatchmatfx.a rtpick.a rtpitexd.a rtray.a rtslerp.a rtsplpvs.a rttiff.a rttilerd.a rttoc.a rt2d.a rt2danim.a rpltmap.a rtltmap.a rtbary.a rpmorph.a rplogo.a rtmipk.a rphanim.a rtquat.a rtcharse.a rtras.a rtbmp.a rtpng.a rpcollis.a rtintsec.a rppvs.a rprandom.a rpworld.a rtworld.a rwcore.a rtanim.a rtfsyst.a rtcmpkey.a /nologo /pdb:none /debug /machine:IX86 /out:"..\bin\DDH_Debug\game_framework.elf" /libpath:"$(RWGSDK)\lib\gcn\debug" /libpath:"$(DOLPHIN_ROOT)\HW2\lib" /libpath:"$(AMCDDKBIN)\..\lib" /D:SN_TARGET_NGC

!ELSEIF  "$(CFG)" == "game_framework - Win32 NGC DDH Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "game_framework___Win32_NGC_DDH_Release"
# PROP BASE Intermediate_Dir "game_framework___Win32_NGC_DDH_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DDH_Release"
# PROP Intermediate_Dir "DDH_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "../../source/" /D "RWS_DISABLE_MEMORY_CHECKING" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "NDEBUG" /FD /debug /c
# ADD CPP /nologo /W3 /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "NDEBUG" /D "RWS_DISABLE_MEMORY_CHECKING" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /FD -fexceptions /debug /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=snLink.exe
# ADD BASE LINK32 base.a os.a exi.a si.a db.a mtx.a dvd.a vi.a demo.a pad.a ai.a ar.a ax.a axfx.a mix.a sp.a am.a axart.a syn.a seq.a dsp.a dtk.a card.a hio.a mcc.a gx.a G2D.a texPalette.a fileCache.a support.a perf.a AmcExi2.a amcnotstub.a odemuexi2.a odenotstub.a libstl.a rpdmorph.a rplodatm.a rpmatfx.a rpmipkl.a rppatch.a rppatchmatfx.a rppatchskin.a rppatchskinmatfx.a rpprtstd.a rpptank.a rpskinmatfx.a rpspline.a rpusrdat.a rtbezpat.a rtimport.a rtmipkmatfx.a rtmipkpatch.a rtmipkpatchmatfx.a rtpick.a rtpitexd.a rtray.a rtslerp.a rtsplpvs.a rttiff.a rttilerd.a rttoc.a rt2d.a rt2danim.a rpltmap.a rtltmap.a rtbary.a rpmorph.a rplogo.a rtmipk.a rphanim.a rpskin.a rtquat.a rtcharse.a rtras.a rtbmp.a rtpng.a rpcollis.a rtintsec.a rppvs.a rprandom.a rpworld.a rtworld.a rwcore.a rtanim.a /nologo /pdb:none /debug /machine:IX86 /out:"Release\game_framework.elf" /libpath:"$(DOLPHIN_ROOT)\HW2\lib" /libpath:"$(ODEMUSDKBIN)\..\lib" /libpath:"$(AMCDDKBIN)\..\lib" /libpath:"$(RWGSDK)\lib\gcn\release" /D:SN_TARGET_NGC
# ADD LINK32 odemustubs.a amcnotstub.a amcExi2.a base.a os.a exi.a si.a db.a mtx.a dvd.a vi.a demo.a pad.a ai.a ar.a ax.a axfx.a mix.a sp.a am.a axart.a syn.a seq.a dsp.a dtk.a card.a hio.a mcc.a gx.a G2D.a texPalette.a fileCache.a support.a perf.a libstl.a rpdmorph.a rplodatm.a rpmatfx.a rpmipkl.a rppatch.a rppatchmatfx.a rppatchskin.a rppatchskinmatfx.a rpprtstd.a rpptank.a rpskinmatfx.a rpspline.a rpusrdat.a rtbezpat.a rtimport.a rtmipkmatfx.a rtmipkpatch.a rtmipkpatchmatfx.a rtpick.a rtpitexd.a rtray.a rtslerp.a rtsplpvs.a rttiff.a rttilerd.a rttoc.a rt2d.a rt2danim.a rpltmap.a rtltmap.a rtbary.a rpmorph.a rplogo.a rtmipk.a rphanim.a rtquat.a rtcharse.a rtras.a rtbmp.a rtpng.a rpcollis.a rtintsec.a rppvs.a rprandom.a rpworld.a rtworld.a rwcore.a rtanim.a rtfsyst.a rtcmpkey.a /nologo /pdb:none /debug /machine:IX86 /out:"..\bin\DDH_Release\game_framework.elf" /libpath:"$(RWGSDK)\lib\gcn\release" /libpath:"$(DOLPHIN_ROOT)\HW2\lib" /libpath:"$(AMCDDKBIN)\..\lib" /D:SN_TARGET_NGC

!ELSEIF  "$(CFG)" == "game_framework - Win32 NGC GDEV DesignDebug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "game_framework___Win32_NGC_GDEV_DesignDebug"
# PROP BASE Intermediate_Dir "game_framework___Win32_NGC_GDEV_DesignDebug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "GDEV_Design_Debug"
# PROP Intermediate_Dir "GDEV_Design_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "../../source/" /D "HW2" /D "_DEBUG" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWDEBUG" /D "RWTERMINAL" /D "DEBUG" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /FD /debug /c
# ADD CPP /nologo /W3 /GX /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "_DEBUG" /D "RWDEBUG" /D "DEBUG" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "RWS_MEMORY_PROFILING" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "RWS_FUNCTIONPROFILE" /FD -fexceptions /debug /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=snLink.exe
# ADD BASE LINK32 baseD.a osD.a exiD.a siD.a dbD.a mtxD.a dvdD.a viD.a demoD.a padD.a aiD.a arD.a axD.a axfxD.a mixD.a spD.a amD.a axartD.a synD.a seqD.a dspD.a dtkD.a cardD.a hioD.a mccD.a gxD.a G2DD.a texPaletteD.a fileCacheD.a supportD.a perfD.a AmcExi2D.a amcnotstubD.a odemuexi2D.a odenotstubD.a libstl.a rpdmorph.a rplodatm.a rpmatfx.a rpmipkl.a rppatch.a rppatchmatfx.a rppatchskin.a rppatchskinmatfx.a rpprtstd.a rpptank.a rpskinmatfx.a rpspline.a rpusrdat.a rtbezpat.a rtimport.a rtmipkmatfx.a rtmipkpatch.a rtmipkpatchmatfx.a rtpick.a rtpitexd.a rtray.a rtslerp.a rtsplpvs.a rttiff.a rttilerd.a rttoc.a rt2d.a rt2danim.a rpltmap.a rtltmap.a rtbary.a rpmorph.a rplogo.a rtmipk.a rphanim.a rpskin.a rtquat.a rtcharse.a rtras.a rtbmp.a rtpng.a rpcollis.a rtintsec.a rppvs.a rprandom.a rpworld.a rtworld.a rwcore.a rtanim.a libmcdconvex.a libmeassetfactory.a libmcdconvexcreatehull.a libmst.a libmcdframe.a libmcdprimitives.a libmdt.a libmdtarthur.a libmdtbcl.a libmdtkea.a libmdtsolverutils.a libmeapp.a libmeassetdb.a libmeassetdbxmlio.a libmeglobals.a libmexml.a libmcdcommon.a /nologo /pdb:none /debug /machine:IX86 /out:"DesignDebug\game_framework.elf" /libpath:"$(DOLPHIN_ROOT)\HW2\lib" /libpath:"$(ODEMUSDKBIN)\..\lib" /libpath:"$(AMCDDKBIN)\..\lib" /libpath:"$(RWGSDK)\lib\gcn\debug" /D:SN_TARGET_NGC
# ADD LINK32 odenotstubd.a amcstubsd.a OdemuExi2d.a baseD.a osD.a exiD.a siD.a dbD.a mtxD.a dvdD.a viD.a demoD.a padD.a aiD.a arD.a axD.a axfxD.a mixD.a spD.a amD.a axartD.a synD.a seqD.a dspD.a dtkD.a cardD.a hioD.a mccD.a gxD.a G2DD.a texPaletteD.a fileCacheD.a supportD.a perfD.a libstl.a rpdmorph.a rplodatm.a rpmatfx.a rpmipkl.a rppatch.a rppatchmatfx.a rppatchskin.a rppatchskinmatfx.a rpprtstd.a rpptank.a rpskinmatfx.a rpspline.a rpusrdat.a rtbezpat.a rtimport.a rtmipkmatfx.a rtmipkpatch.a rtmipkpatchmatfx.a rtpick.a rtpitexd.a rtray.a rtslerp.a rtsplpvs.a rttiff.a rttilerd.a rttoc.a rt2d.a rt2danim.a rpltmap.a rtltmap.a rtbary.a rpmorph.a rplogo.a rtmipk.a rphanim.a rtquat.a rtcharse.a rtras.a rtbmp.a rtpng.a rpcollis.a rtintsec.a rppvs.a rprandom.a rpworld.a rtworld.a rwcore.a rtanim.a rtfsyst.a rtcmpkey.a /nologo /pdb:none /debug /machine:IX86 /out:"..\bin\GDEV_Design_Debug\game_framework.elf" /libpath:"$(RWGSDK)\lib\gcn\debug" /libpath:"$(DOLPHIN_ROOT)\HW2\lib" /libpath:"$(ODEMUSDKBIN)\..\lib" /D:SN_TARGET_NGC

!ELSEIF  "$(CFG)" == "game_framework - Win32 NGC GDEV DesignRelease"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "game_framework___Win32_NGC_GDEV_DesignRelease"
# PROP BASE Intermediate_Dir "game_framework___Win32_NGC_GDEV_DesignRelease"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "GDEV_Design_Release"
# PROP Intermediate_Dir "GDEV_Design_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "../../source/" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "NDEBUG" /D "RWS_DISABLE_MEMORY_CHECKING" /FD /debug /c
# ADD CPP /nologo /W3 /GX /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "NDEBUG" /D "RWS_DISABLE_MEMORY_CHECKING" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "RWS_MEMORY_PROFILING" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "RWS_FUNCTIONPROFILE" /FD -fexceptions /debug /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=snLink.exe
# ADD BASE LINK32 base.a os.a exi.a si.a db.a mtx.a dvd.a vi.a demo.a pad.a ai.a ar.a ax.a axfx.a mix.a sp.a am.a axart.a syn.a seq.a dsp.a dtk.a card.a hio.a mcc.a gx.a G2D.a texPalette.a fileCache.a support.a perf.a AmcExi2.a amcnotstub.a odemuexi2.a odenotstub.a libstl.a rpdmorph.a rplodatm.a rpmatfx.a rpmipkl.a rppatch.a rppatchmatfx.a rppatchskin.a rppatchskinmatfx.a rpprtstd.a rpptank.a rpskinmatfx.a rpspline.a rpusrdat.a rtbezpat.a rtimport.a rtmipkmatfx.a rtmipkpatch.a rtmipkpatchmatfx.a rtpick.a rtpitexd.a rtray.a rtslerp.a rtsplpvs.a rttiff.a rttilerd.a rttoc.a rt2d.a rt2danim.a rpltmap.a rtltmap.a rtbary.a rpmorph.a rplogo.a rtmipk.a rphanim.a rpskin.a rtquat.a rtcharse.a rtras.a rtbmp.a rtpng.a rpcollis.a rtintsec.a rppvs.a rprandom.a rpworld.a rtworld.a rwcore.a rtanim.a /nologo /pdb:none /debug /machine:IX86 /out:"DesignRelease\game_framework.elf" /libpath:"$(DOLPHIN_ROOT)\HW2\lib" /libpath:"$(ODEMUSDKBIN)\..\lib" /libpath:"$(AMCDDKBIN)\..\lib" /libpath:"$(RWGSDK)\lib\gcn\release" /D:SN_TARGET_NGC
# ADD LINK32 odenotstub.a amcstubs.a OdemuExi2.a base.a os.a exi.a si.a db.a mtx.a dvd.a vi.a demo.a pad.a ai.a ar.a ax.a axfx.a mix.a sp.a am.a axart.a syn.a seq.a dsp.a dtk.a card.a hio.a mcc.a gx.a G2D.a texPalette.a fileCache.a support.a perf.a libstl.a rpdmorph.a rplodatm.a rpmatfx.a rpmipkl.a rppatch.a rppatchmatfx.a rppatchskin.a rppatchskinmatfx.a rpprtstd.a rpptank.a rpskinmatfx.a rpspline.a rpusrdat.a rtbezpat.a rtimport.a rtmipkmatfx.a rtmipkpatch.a rtmipkpatchmatfx.a rtpick.a rtpitexd.a rtray.a rtslerp.a rtsplpvs.a rttiff.a rttilerd.a rttoc.a rt2d.a rt2danim.a rpltmap.a rtltmap.a rtbary.a rpmorph.a rplogo.a rtmipk.a rphanim.a rtquat.a rtcharse.a rtras.a rtbmp.a rtpng.a rpcollis.a rtintsec.a rppvs.a rprandom.a rpworld.a rtworld.a rwcore.a rtanim.a rtfsyst.a rtcmpkey.a /nologo /pdb:none /debug /machine:IX86 /out:"..\bin\GDEV_Design_Release\game_framework.elf" /libpath:"$(RWGSDK)\lib\gcn\release" /libpath:"$(DOLPHIN_ROOT)\HW2\lib" /libpath:"$(ODEMUSDKBIN)\..\lib" /D:SN_TARGET_NGC

!ELSEIF  "$(CFG)" == "game_framework - Win32 NGC GDEV Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "game_framework___Win32_NGC_GDEV_Debug"
# PROP BASE Intermediate_Dir "game_framework___Win32_NGC_GDEV_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "GDEV_Debug"
# PROP Intermediate_Dir "GDEV_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "../../source/" /D "HW2" /D "_DEBUG" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWDEBUG" /D "RWTERMINAL" /D "DEBUG" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /FD /debug /c
# ADD CPP /nologo /W3 /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "RWS_BROADBAND" /D "RWS_DISABLE_MEMORY_CHECKING" /D "HW2" /D "_DEBUG" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWDEBUG" /D "RWTERMINAL" /D "DEBUG" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /FD -fexceptions /debug /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=snLink.exe
# ADD BASE LINK32 baseD.a osD.a exiD.a siD.a dbD.a mtxD.a dvdD.a viD.a demoD.a padD.a aiD.a arD.a axD.a axfxD.a mixD.a spD.a amD.a axartD.a synD.a seqD.a dspD.a dtkD.a cardD.a hioD.a mccD.a gxD.a G2DD.a texPaletteD.a fileCacheD.a supportD.a perfD.a AmcExi2D.a amcnotstubD.a odemuexi2D.a odenotstubD.a libstl.a rpdmorph.a rplodatm.a rpmatfx.a rpmipkl.a rppatch.a rppatchmatfx.a rppatchskin.a rppatchskinmatfx.a rpprtstd.a rpptank.a rpskinmatfx.a rpspline.a rpusrdat.a rtbezpat.a rtimport.a rtmipkmatfx.a rtmipkpatch.a rtmipkpatchmatfx.a rtpick.a rtpitexd.a rtray.a rtslerp.a rtsplpvs.a rttiff.a rttilerd.a rttoc.a rt2d.a rt2danim.a rpltmap.a rtltmap.a rtbary.a rpmorph.a rplogo.a rtmipk.a rphanim.a rpskin.a rtquat.a rtcharse.a rtras.a rtbmp.a rtpng.a rpcollis.a rtintsec.a rppvs.a rprandom.a rpworld.a rtworld.a rwcore.a rtanim.a /nologo /pdb:none /debug /machine:IX86 /out:"Debug\game_framework.elf" /libpath:"$(DOLPHIN_ROOT)\HW2\lib" /libpath:"$(ODEMUSDKBIN)\..\lib" /libpath:"$(AMCDDKBIN)\..\lib" /libpath:"$(RWGSDK)\lib\gcn\debug" /D:SN_TARGET_NGC
# ADD LINK32 odenotstubd.a amcstubsd.a OdemuExi2d.a baseD.a osD.a exiD.a siD.a dbD.a mtxD.a dvdD.a viD.a demoD.a padD.a aiD.a arD.a axD.a axfxD.a mixD.a spD.a amD.a axartD.a synD.a seqD.a dspD.a dtkD.a cardD.a hioD.a mccD.a gxD.a G2DD.a texPaletteD.a fileCacheD.a supportD.a perfD.a libstl.a rpdmorph.a rplodatm.a rpmatfx.a rpmipkl.a rppatch.a rppatchmatfx.a rppatchskin.a rppatchskinmatfx.a rpprtstd.a rpptank.a rpskinmatfx.a rpspline.a rpusrdat.a rtbezpat.a rtimport.a rtmipkmatfx.a rtmipkpatch.a rtmipkpatchmatfx.a rtpick.a rtpitexd.a rtray.a rtslerp.a rtsplpvs.a rttiff.a rttilerd.a rttoc.a rt2d.a rt2danim.a rpltmap.a rtltmap.a rtbary.a rpmorph.a rplogo.a rtmipk.a rphanim.a rtquat.a rtcharse.a rtras.a rtbmp.a rtpng.a rpcollis.a rtintsec.a rppvs.a rprandom.a rpworld.a rtworld.a rwcore.a rtanim.a rtfsyst.a rtcmpkey.a /nologo /pdb:none /debug /machine:IX86 /out:"..\bin\GDEV_Debug\game_framework.elf" /libpath:"$(RWGSDK)\lib\gcn\debug" /libpath:"$(DOLPHIN_ROOT)\HW2\lib" /libpath:"$(ODEMUSDKBIN)\..\lib" /D:SN_TARGET_NGC

!ELSEIF  "$(CFG)" == "game_framework - Win32 NGC GDEV Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "game_framework___Win32_NGC_GDEV_Release"
# PROP BASE Intermediate_Dir "game_framework___Win32_NGC_GDEV_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "GDEV_Release"
# PROP Intermediate_Dir "GDEV_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "../../source/" /D "RWS_DISABLE_MEMORY_CHECKING" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "NDEBUG" /FD /debug /c
# ADD CPP /nologo /W3 /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "NDEBUG" /D "RWS_DISABLE_MEMORY_CHECKING" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /FD -fexceptions /debug /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=snLink.exe
# ADD BASE LINK32 base.a os.a exi.a si.a db.a mtx.a dvd.a vi.a demo.a pad.a ai.a ar.a ax.a axfx.a mix.a sp.a am.a axart.a syn.a seq.a dsp.a dtk.a card.a hio.a mcc.a gx.a G2D.a texPalette.a fileCache.a support.a perf.a AmcExi2.a amcnotstub.a odemuexi2.a odenotstub.a libstl.a rpdmorph.a rplodatm.a rpmatfx.a rpmipkl.a rppatch.a rppatchmatfx.a rppatchskin.a rppatchskinmatfx.a rpprtstd.a rpptank.a rpskinmatfx.a rpspline.a rpusrdat.a rtbezpat.a rtimport.a rtmipkmatfx.a rtmipkpatch.a rtmipkpatchmatfx.a rtpick.a rtpitexd.a rtray.a rtslerp.a rtsplpvs.a rttiff.a rttilerd.a rttoc.a rt2d.a rt2danim.a rpltmap.a rtltmap.a rtbary.a rpmorph.a rplogo.a rtmipk.a rphanim.a rpskin.a rtquat.a rtcharse.a rtras.a rtbmp.a rtpng.a rpcollis.a rtintsec.a rppvs.a rprandom.a rpworld.a rtworld.a rwcore.a rtanim.a /nologo /pdb:none /debug /machine:IX86 /out:"Release\game_framework.elf" /libpath:"$(DOLPHIN_ROOT)\HW2\lib" /libpath:"$(ODEMUSDKBIN)\..\lib" /libpath:"$(AMCDDKBIN)\..\lib" /libpath:"$(RWGSDK)\lib\gcn\release" /D:SN_TARGET_NGC
# ADD LINK32 odenotstub.a amcstubs.a OdemuExi2.a base.a os.a exi.a si.a db.a mtx.a dvd.a vi.a demo.a pad.a ai.a ar.a ax.a axfx.a mix.a sp.a am.a axart.a syn.a seq.a dsp.a dtk.a card.a hio.a mcc.a gx.a G2D.a texPalette.a fileCache.a support.a perf.a libstl.a rpdmorph.a rplodatm.a rpmatfx.a rpmipkl.a rppatch.a rppatchmatfx.a rppatchskin.a rppatchskinmatfx.a rpprtstd.a rpptank.a rpskinmatfx.a rpspline.a rpusrdat.a rtbezpat.a rtimport.a rtmipkmatfx.a rtmipkpatch.a rtmipkpatchmatfx.a rtpick.a rtpitexd.a rtray.a rtslerp.a rtsplpvs.a rttiff.a rttilerd.a rttoc.a rt2d.a rt2danim.a rpltmap.a rtltmap.a rtbary.a rpmorph.a rplogo.a rtmipk.a rphanim.a rtquat.a rtcharse.a rtras.a rtbmp.a rtpng.a rpcollis.a rtintsec.a rppvs.a rprandom.a rpworld.a rtworld.a rwcore.a rtanim.a rtfsyst.a rtcmpkey.a /nologo /pdb:none /debug /machine:IX86 /out:"..\bin\GDEV_Release\game_framework.elf" /libpath:"$(RWGSDK)\lib\gcn\release" /libpath:"$(DOLPHIN_ROOT)\HW2\lib" /libpath:"$(ODEMUSDKBIN)\..\lib" /D:SN_TARGET_NGC

!ELSEIF  "$(CFG)" == "game_framework - Win32 NGC GDEV DesignDebug Broadband"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "game_framework___Win32_NGC_GDEV_DesignDebug_Broadband"
# PROP BASE Intermediate_Dir "game_framework___Win32_NGC_GDEV_DesignDebug_Broadband"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "GDEV_Design_Debug_Broadband"
# PROP Intermediate_Dir "GDEV_Design_Debug_Broadband"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "HW2" /D "_DEBUG" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWDEBUG" /D "RWTERMINAL" /D "DEBUG" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /FD /debug /c
# ADD CPP /nologo /W3 /GX /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "_DEBUG" /D "RWDEBUG" /D "DEBUG" /D "RWS_BROADBAND" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "RWS_MEMORY_PROFILING" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "RWS_FUNCTIONPROFILE" /FD -fexceptions /debug /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=snLink.exe
# ADD BASE LINK32 odenotstubd.a amcstubsd.a OdemuExi2d.a baseD.a osD.a exiD.a siD.a dbD.a mtxD.a dvdD.a viD.a demoD.a padD.a aiD.a arD.a axD.a axfxD.a mixD.a spD.a amD.a axartD.a synD.a seqD.a dspD.a dtkD.a cardD.a hioD.a mccD.a gxD.a G2DD.a texPaletteD.a fileCacheD.a supportD.a perfD.a libstl.a rpdmorph.a rplodatm.a rpmatfx.a rpmipkl.a rppatch.a rppatchmatfx.a rppatchskin.a rppatchskinmatfx.a rpprtstd.a rpptank.a rpskinmatfx.a rpspline.a rpusrdat.a rtbezpat.a rtimport.a rtmipkmatfx.a rtmipkpatch.a rtmipkpatchmatfx.a rtpick.a rtpitexd.a rtray.a rtslerp.a rtsplpvs.a rttiff.a rttilerd.a rttoc.a rt2d.a rt2danim.a rpltmap.a rtltmap.a rtbary.a rpmorph.a rplogo.a rtmipk.a rphanim.a rpskin.a rtquat.a rtcharse.a rtras.a rtbmp.a rtpng.a rpcollis.a rtintsec.a rppvs.a rprandom.a rpworld.a rtworld.a rwcore.a rtanim.a /nologo /pdb:none /debug /machine:IX86 /out:"..\bin\GDEV_Design_Debug\game_framework.elf" /libpath:"$(RWGSDK)\lib\gcn\debug" /libpath:"$(DOLPHIN_ROOT)\HW2\lib" /libpath:"$(ODEMUSDKBIN)\..\lib" /D:SN_TARGET_NGC
# ADD LINK32 odenotstubd.a amcstubsd.a OdemuExi2d.a baseD.a osD.a exiD.a siD.a dbD.a mtxD.a dvdD.a viD.a demoD.a padD.a aiD.a arD.a axD.a axfxD.a mixD.a spD.a amD.a axartD.a synD.a seqD.a dspD.a dtkD.a cardD.a hioD.a mccD.a gxD.a G2DD.a texPaletteD.a fileCacheD.a supportD.a perfD.a ipd.a ethd.a libstl.a rpdmorph.a rplodatm.a rpmatfx.a rpmipkl.a rppatch.a rppatchmatfx.a rppatchskin.a rppatchskinmatfx.a rpprtstd.a rpptank.a rpskinmatfx.a rpspline.a rpusrdat.a rtbezpat.a rtimport.a rtmipkmatfx.a rtmipkpatch.a rtmipkpatchmatfx.a rtpick.a rtpitexd.a rtray.a rtslerp.a rtsplpvs.a rttiff.a rttilerd.a rttoc.a rt2d.a rt2danim.a rpltmap.a rtltmap.a rtbary.a rpmorph.a rplogo.a rtmipk.a rphanim.a rtquat.a rtcharse.a rtras.a rtbmp.a rtpng.a rpcollis.a rtintsec.a rppvs.a rprandom.a rpworld.a rtworld.a rwcore.a rtanim.a rtfsyst.a rtcmpkey.a /nologo /pdb:none /debug /machine:IX86 /out:"..\bin\GDEV_Design_Debug_Broadband\game_framework.elf" /libpath:"$(RWGSDK)\lib\gcn\debug" /libpath:"$(DOLPHIN_ROOT)\HW2\lib" /libpath:"$(ODEMUSDKBIN)\..\lib" /D:SN_TARGET_NGC

!ELSEIF  "$(CFG)" == "game_framework - Win32 NGC GDEV DesignRelease Broadband"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "game_framework___Win32_NGC_GDEV_DesignRelease_Broadband"
# PROP BASE Intermediate_Dir "game_framework___Win32_NGC_GDEV_DesignRelease_Broadband"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "GDEV_Design_Release_Broadband"
# PROP Intermediate_Dir "GDEV_Design_Release_Broadband"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "NDEBUG" /D "RWS_DISABLE_MEMORY_CHECKING" /FD /debug /c
# ADD CPP /nologo /W3 /GX /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "NDEBUG" /D "RWS_BROADBAND" /D "RWS_DISABLE_MEMORY_CHECKING" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "RWS_MEMORY_PROFILING" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "RWS_FUNCTIONPROFILE" /FD -fexceptions /debug /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=snLink.exe
# ADD BASE LINK32 odenotstub.a amcstubs.a OdemuExi2.a base.a os.a exi.a si.a db.a mtx.a dvd.a vi.a demo.a pad.a ai.a ar.a ax.a axfx.a mix.a sp.a am.a axart.a syn.a seq.a dsp.a dtk.a card.a hio.a mcc.a gx.a G2D.a texPalette.a fileCache.a support.a perf.a libstl.a rpdmorph.a rplodatm.a rpmatfx.a rpmipkl.a rppatch.a rppatchmatfx.a rppatchskin.a rppatchskinmatfx.a rpprtstd.a rpptank.a rpskinmatfx.a rpspline.a rpusrdat.a rtbezpat.a rtimport.a rtmipkmatfx.a rtmipkpatch.a rtmipkpatchmatfx.a rtpick.a rtpitexd.a rtray.a rtslerp.a rtsplpvs.a rttiff.a rttilerd.a rttoc.a rt2d.a rt2danim.a rpltmap.a rtltmap.a rtbary.a rpmorph.a rplogo.a rtmipk.a rphanim.a rpskin.a rtquat.a rtcharse.a rtras.a rtbmp.a rtpng.a rpcollis.a rtintsec.a rppvs.a rprandom.a rpworld.a rtworld.a rwcore.a rtanim.a /nologo /pdb:none /debug /machine:IX86 /out:"..\bin\GDEV_Design_Release\game_framework.elf" /libpath:"$(RWGSDK)\lib\gcn\release" /libpath:"$(DOLPHIN_ROOT)\HW2\lib" /libpath:"$(ODEMUSDKBIN)\..\lib" /D:SN_TARGET_NGC
# ADD LINK32 odenotstub.a amcstubs.a OdemuExi2.a base.a os.a exi.a si.a db.a mtx.a dvd.a vi.a demo.a pad.a ai.a ar.a ax.a axfx.a mix.a sp.a am.a axart.a syn.a seq.a dsp.a dtk.a card.a hio.a mcc.a gx.a G2D.a texPalette.a fileCache.a support.a perf.a ip.a eth.a libstl.a rpdmorph.a rplodatm.a rpmatfx.a rpmipkl.a rppatch.a rppatchmatfx.a rppatchskin.a rppatchskinmatfx.a rpprtstd.a rpptank.a rpskinmatfx.a rpspline.a rpusrdat.a rtbezpat.a rtimport.a rtmipkmatfx.a rtmipkpatch.a rtmipkpatchmatfx.a rtpick.a rtpitexd.a rtray.a rtslerp.a rtsplpvs.a rttiff.a rttilerd.a rttoc.a rt2d.a rt2danim.a rpltmap.a rtltmap.a rtbary.a rpmorph.a rplogo.a rtmipk.a rphanim.a rtquat.a rtcharse.a rtras.a rtbmp.a rtpng.a rpcollis.a rtintsec.a rppvs.a rprandom.a rpworld.a rtworld.a rwcore.a rtanim.a rtfsyst.a rtcmpkey.a /nologo /pdb:none /debug /machine:IX86 /out:"..\bin\GDEV_Design_Release_Broadband\game_framework.elf" /libpath:"$(RWGSDK)\lib\gcn\release" /libpath:"$(DOLPHIN_ROOT)\HW2\lib" /libpath:"$(ODEMUSDKBIN)\..\lib" /D:SN_TARGET_NGC

!ELSEIF  "$(CFG)" == "game_framework - Win32 NGC DDH DesignDebug Broadband"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "game_framework___Win32_NGC_DDH_DesignDebug_Broadband"
# PROP BASE Intermediate_Dir "game_framework___Win32_NGC_DDH_DesignDebug_Broadband"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DDH_Design_Debug_Broadband"
# PROP Intermediate_Dir "DDH_Design_Debug_Broadband"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "HW2" /D "_DEBUG" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWDEBUG" /D "RWTERMINAL" /D "DEBUG" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /FD /debug /c
# ADD CPP /nologo /W3 /GX /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "_DEBUG" /D "RWDEBUG" /D "DEBUG" /D "RWS_BROADBAND" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "RWS_MEMORY_PROFILING" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "RWS_FUNCTIONPROFILE" /FD -fexceptions /debug /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=snLink.exe
# ADD BASE LINK32 odemustubsd.a amcnotstubd.a amcExi2d.a baseD.a osD.a exiD.a siD.a dbD.a mtxD.a dvdD.a viD.a demoD.a padD.a aiD.a arD.a axD.a axfxD.a mixD.a spD.a amD.a axartD.a synD.a seqD.a dspD.a dtkD.a cardD.a hioD.a mccD.a gxD.a G2DD.a texPaletteD.a fileCacheD.a supportD.a perfD.a libstl.a rpdmorph.a rplodatm.a rpmatfx.a rpmipkl.a rppatch.a rppatchmatfx.a rppatchskin.a rppatchskinmatfx.a rpprtstd.a rpptank.a rpskinmatfx.a rpspline.a rpusrdat.a rtbezpat.a rtimport.a rtmipkmatfx.a rtmipkpatch.a rtmipkpatchmatfx.a rtpick.a rtpitexd.a rtray.a rtslerp.a rtsplpvs.a rttiff.a rttilerd.a rttoc.a rt2d.a rt2danim.a rpltmap.a rtltmap.a rtbary.a rpmorph.a rplogo.a rtmipk.a rphanim.a rpskin.a rtquat.a rtcharse.a rtras.a rtbmp.a rtpng.a rpcollis.a rtintsec.a rppvs.a rprandom.a rpworld.a rtworld.a rwcore.a rtanim.a /nologo /pdb:none /debug /machine:IX86 /out:"..\bin\DDH_Design_Debug\game_framework.elf" /libpath:"$(DOLPHIN_ROOT)\HW2\lib" /libpath:"$(AMCDDKBIN)\..\lib" /libpath:"$(RWGSDK)\lib\gcn\debug" /D:SN_TARGET_NGC
# ADD LINK32 odemustubsd.a amcnotstubd.a amcExi2d.a baseD.a osD.a exiD.a siD.a dbD.a mtxD.a dvdD.a viD.a demoD.a padD.a aiD.a arD.a axD.a axfxD.a mixD.a spD.a amD.a axartD.a synD.a seqD.a dspD.a dtkD.a cardD.a hioD.a mccD.a gxD.a G2DD.a texPaletteD.a fileCacheD.a supportD.a perfD.a ipd.a ethd.a libstl.a rpdmorph.a rplodatm.a rpmatfx.a rpmipkl.a rppatch.a rppatchmatfx.a rppatchskin.a rppatchskinmatfx.a rpprtstd.a rpptank.a rpskinmatfx.a rpspline.a rpusrdat.a rtbezpat.a rtimport.a rtmipkmatfx.a rtmipkpatch.a rtmipkpatchmatfx.a rtpick.a rtpitexd.a rtray.a rtslerp.a rtsplpvs.a rttiff.a rttilerd.a rttoc.a rt2d.a rt2danim.a rpltmap.a rtltmap.a rtbary.a rpmorph.a rplogo.a rtmipk.a rphanim.a rtquat.a rtcharse.a rtras.a rtbmp.a rtpng.a rpcollis.a rtintsec.a rppvs.a rprandom.a rpworld.a rtworld.a rwcore.a rtanim.a rtfsyst.a rtcmpkey.a /nologo /pdb:none /debug /machine:IX86 /out:"..\bin\DDH_Design_Debug_Broadband\game_framework.elf" /libpath:"$(DOLPHIN_ROOT)\HW2\lib" /libpath:"$(AMCDDKBIN)\..\lib" /libpath:"$(RWGSDK)\lib\gcn\debug" /D:SN_TARGET_NGC

!ELSEIF  "$(CFG)" == "game_framework - Win32 NGC DDH DesignRelease Broadband"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "game_framework___Win32_NGC_DDH_DesignRelease_Broadband"
# PROP BASE Intermediate_Dir "game_framework___Win32_NGC_DDH_DesignRelease_Broadband"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DDH_Design_Release_Broadband"
# PROP Intermediate_Dir "DDH_Design_Release_Broadband"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "NDEBUG" /D "RWS_DISABLE_MEMORY_CHECKING" /FD /debug /c
# ADD CPP /nologo /W3 /GX /Od /I "..\..\source\framework\core\MemoryHandler" /I "$(DOLPHIN_ROOT)\include" /I "$(RWGSDK)\include\gcn" /I "..\..\source\\" /D "NDEBUG" /D "RWS_BROADBAND" /D "RWS_DISABLE_MEMORY_CHECKING" /D "RWS_EVENTVISUALIZATION" /D "RWS_DEBUGTOOLS" /D "RWS_DESIGN" /D "RWS_DEBUGSWITCHES" /D "RWS_MEMORY_PROFILING" /D "HW2" /D "SN_TARGET_NGC" /D "__SN__" /D "SNSYS" /D __HWSIM=0 /D "EPPC" /D MARLIN=DI /D "ORCA" /D "GEKKO" /D GX_REV=2 /D "BUG_CLR_LOAD_DLY" /D "BUG_XF_STALL" /D "DOLPHIN" /D "RWTERMINAL" /D "TRK_INTEGRATION" /D "GXFIFO_SERIAL" /D "NGC" /D "RWS_FUNCTIONPROFILE" /FD -fexceptions /debug /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=snBsc.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=snLink.exe
# ADD BASE LINK32 odemustubs.a amcnotstub.a amcExi2.a base.a os.a exi.a si.a db.a mtx.a dvd.a vi.a demo.a pad.a ai.a ar.a ax.a axfx.a mix.a sp.a am.a axart.a syn.a seq.a dsp.a dtk.a card.a hio.a mcc.a gx.a G2D.a texPalette.a fileCache.a support.a perf.a libstl.a rpdmorph.a rplodatm.a rpmatfx.a rpmipkl.a rppatch.a rppatchmatfx.a rppatchskin.a rppatchskinmatfx.a rpprtstd.a rpptank.a rpskinmatfx.a rpspline.a rpusrdat.a rtbezpat.a rtimport.a rtmipkmatfx.a rtmipkpatch.a rtmipkpatchmatfx.a rtpick.a rtpitexd.a rtray.a rtslerp.a rtsplpvs.a rttiff.a rttilerd.a rttoc.a rt2d.a rt2danim.a rpltmap.a rtltmap.a rtbary.a rpmorph.a rplogo.a rtmipk.a rphanim.a rpskin.a rtquat.a rtcharse.a rtras.a rtbmp.a rtpng.a rpcollis.a rtintsec.a rppvs.a rprandom.a rpworld.a rtworld.a rwcore.a rtanim.a /nologo /pdb:none /debug /machine:IX86 /out:"..\bin\DDH_Design_Release\game_framework.elf" /libpath:"$(RWGSDK)\lib\gcn\release" /libpath:"$(DOLPHIN_ROOT)\HW2\lib" /libpath:"$(AMCDDKBIN)\..\lib" /D:SN_TARGET_NGC
# ADD LINK32 odemustubs.a amcnotstub.a amcExi2.a base.a os.a exi.a si.a db.a mtx.a dvd.a vi.a demo.a pad.a ai.a ar.a ax.a axfx.a mix.a sp.a am.a axart.a syn.a seq.a dsp.a dtk.a card.a hio.a mcc.a gx.a G2D.a texPalette.a fileCache.a support.a perf.a ip.a eth.a libstl.a rpdmorph.a rplodatm.a rpmatfx.a rpmipkl.a rppatch.a rppatchmatfx.a rppatchskin.a rppatchskinmatfx.a rpprtstd.a rpptank.a rpskinmatfx.a rpspline.a rpusrdat.a rtbezpat.a rtimport.a rtmipkmatfx.a rtmipkpatch.a rtmipkpatchmatfx.a rtpick.a rtpitexd.a rtray.a rtslerp.a rtsplpvs.a rttiff.a rttilerd.a rttoc.a rt2d.a rt2danim.a rpltmap.a rtltmap.a rtbary.a rpmorph.a rplogo.a rtmipk.a rphanim.a rtquat.a rtcharse.a rtras.a rtbmp.a rtpng.a rpcollis.a rtintsec.a rppvs.a rprandom.a rpworld.a rtworld.a rwcore.a rtanim.a rtfsyst.a rtcmpkey.a /nologo /pdb:none /debug /machine:IX86 /out:"..\bin\DDH_Design_Release_Broadband\game_framework.elf" /libpath:"$(RWGSDK)\lib\gcn\release" /libpath:"$(DOLPHIN_ROOT)\HW2\lib" /libpath:"$(AMCDDKBIN)\..\lib" /D:SN_TARGET_NGC

!ENDIF 

# Begin Target

# Name "game_framework - Win32 NGC DDH DesignDebug"
# Name "game_framework - Win32 NGC DDH DesignRelease"
# Name "game_framework - Win32 NGC DDH Debug"
# Name "game_framework - Win32 NGC DDH Release"
# Name "game_framework - Win32 NGC GDEV DesignDebug"
# Name "game_framework - Win32 NGC GDEV DesignRelease"
# Name "game_framework - Win32 NGC GDEV Debug"
# Name "game_framework - Win32 NGC GDEV Release"
# Name "game_framework - Win32 NGC GDEV DesignDebug Broadband"
# Name "game_framework - Win32 NGC GDEV DesignRelease Broadband"
# Name "game_framework - Win32 NGC DDH DesignDebug Broadband"
# Name "game_framework - Win32 NGC DDH DesignRelease Broadband"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "modules"

# PROP Default_Filter ""
# Begin Group "Examples"

# PROP Default_Filter ""
# Begin Group "CCameralookatpoint"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\modules\Examples\CCameraLookAtPoint\CCameralookatpoint.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\Examples\CCameraLookAtPoint\CCameralookatpoint.h
# End Source File
# End Group
# Begin Group "CMorphAnimate"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\modules\Examples\CMorphAnimate\CMorphAnimate.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\Examples\CMorphAnimate\CMorphAnimate.h
# End Source File
# End Group
# Begin Group "CMultiAnimate"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\modules\Examples\CMultiAnimate\CMultiAnimate.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\Examples\CMultiAnimate\CMultiAnimate.h
# End Source File
# End Group
# Begin Group "LinkPriority"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\modules\Examples\LinkPriority\CLinkPriority.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\Examples\LinkPriority\CLinkPriority.h
# End Source File
# End Group
# Begin Group "CCameraSpline"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\modules\examples\ccameraspline\ccameraspline.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\examples\ccameraspline\ccameraspline.h
# End Source File
# End Group
# End Group
# Begin Group "FPS"

# PROP Default_Filter ""
# Begin Group "FPSButton"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSButton\FPSButton.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSButton\FPSButton.h
# End Source File
# End Group
# Begin Group "FPSDoors"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSDoors\FPSDoor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSDoors\FPSDoor.h
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSDoors\FPSSecretDoor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSDoors\FPSSecretDoor.h
# End Source File
# End Group
# Begin Group "FPSMonsters"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSMonsters\FPSExampleMonster.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSMonsters\FPSExampleMonster.h
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSMonsters\FPSMonster.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSMonsters\FPSMonster.h
# End Source File
# End Group
# Begin Group "FPSPath"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSPath\FPSPathNode.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSPath\FPSPathNode.h
# End Source File
# End Group
# Begin Group "FPSPickups"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSPickups\FPSBouncyPickup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSPickups\FPSBouncyPickup.h
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSPickups\FPSExamplePickup.h
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSPickups\FPSPickup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSPickups\FPSPickup.h
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSPickups\FPSPickupClassReg.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSPickups\FPSSpinningPickup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSPickups\FPSSpinningPickup.h
# End Source File
# End Group
# Begin Group "FPSPlatforms"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSPlatforms\FPSEscalator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSPlatforms\FPSEscalator.h
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSPlatforms\FPSPlatform.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSPlatforms\FPSPlatform.h
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSPlatforms\FPSSimplePlatform.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSPlatforms\FPSSimplePlatform.h
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSPlatforms\FPSTrain.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSPlatforms\FPSTrain.h
# End Source File
# End Group
# Begin Group "FPSPlayer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSPlayer\FPSPlayer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSPlayer\FPSPlayer.h
# End Source File
# End Group
# Begin Group "FPSRenderer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSRenderer\FPSRender.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSRenderer\FPSRender.h
# End Source File
# End Group
# Begin Group "FPSTeleporters"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSTeleporters\FPSTeleportDestination.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSTeleporters\FPSTeleportDestination.h
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSTeleporters\FPSTeleporter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSTeleporters\FPSTeleporter.h
# End Source File
# End Group
# Begin Group "FPSTimer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSTimer\FPSTimer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSTimer\FPSTimer.h
# End Source File
# End Group
# Begin Group "FPSToggle"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSToggle\FPSToggle.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSToggle\FPSToggle.H
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSToggle\FPSToggleInterface.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSToggle\FPSToggleInterface.h
# End Source File
# End Group
# Begin Group "FPSTriggers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSTriggers\FPSTriggerCounter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSTriggers\FPSTriggerCounter.h
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSTriggers\FPSTriggerMultiple.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSTriggers\FPSTriggerMultiple.h
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSTriggers\FPSTriggerOnce.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSTriggers\FPSTriggerOnce.h
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSTriggers\FPSTriggerRelay.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSTriggers\FPSTriggerRelay.h
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSTriggers\FPSTriggerSecret.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSTriggers\FPSTriggerSecret.h
# End Source File
# End Group
# Begin Group "FPSSkyDome"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSSkyDome\FPSSkyDome.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FPS\FPSSkyDome\FPSSkyDome.h
# End Source File
# End Group
# End Group
# Begin Group "FX"

# PROP Default_Filter ""
# Begin Group "CFX_Lights"

# PROP Default_Filter ""
# Begin Group "Transition_Lights"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\source\modules\FX\CFX_Lights\Transition Lights\CFXLight_2Stage.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\source\modules\FX\CFX_Lights\Transition Lights\CFXLight_2Stage.h"
# End Source File
# Begin Source File

SOURCE="..\..\source\modules\FX\CFX_Lights\Transition Lights\CFXLight_Spline.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\source\modules\FX\CFX_Lights\Transition Lights\CFXLight_Spline.h"
# End Source File
# End Group
# Begin Group "Wave_Lights"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\source\modules\FX\CFX_Lights\Wave Lights\CFX_WaveLight.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\source\modules\FX\CFX_Lights\Wave Lights\CFX_WaveLight.h"
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\source\modules\FX\CFX_Lights\CFXBaseLight.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FX\CFX_Lights\CFXBaseLight.h
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FX\CFX_Lights\CFXColorLight.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FX\CFX_Lights\CFXColorLight.h
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FX\CFX_Lights\CFXSpinLight.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FX\CFX_Lights\CFXSpinLight.h
# End Source File
# End Group
# Begin Group "CFX_Particles"

# PROP Default_Filter ""
# Begin Group "PartSpray"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\modules\FX\CFX_Particles\PartSpray\CFXPartSpray.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FX\CFX_Particles\PartSpray\CFXPartSpray.h
# End Source File
# End Group
# Begin Group "Replicator"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\modules\FX\CFX_Particles\Replicator\CFXReplicator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FX\CFX_Particles\Replicator\CFXReplicator.h
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FX\CFX_Particles\Replicator\CFXReplicatorGenerator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FX\CFX_Particles\Replicator\CFXReplicatorGenerator.h
# End Source File
# End Group
# End Group
# Begin Group "CFX_EnvironmentMap"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\modules\FX\CFX_EnvironmentMap\CFXBaseEnvironmentMap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FX\CFX_EnvironmentMap\CFXBaseEnvironmentMap.h
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FX\CFX_EnvironmentMap\CFXEnvironmentMap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FX\CFX_EnvironmentMap\CFXEnvironmentMap.h
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FX\cfx_environmentmap\cfxreflectenvironment.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FX\cfx_environmentmap\cfxreflectenvironment.h
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FX\cfx_environmentmap\cfxreflectenvironmentgcn.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FX\cfx_environmentmap\cfxreflectenvironmentgcn.h
# End Source File
# End Group
# Begin Group "CFX_MotionBlur"

# PROP Default_Filter ""
# Begin Group "GameCube"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\modules\FX\CFX_MotionBlur\GameCube\CFXGameCubeMotionBlur.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FX\CFX_MotionBlur\GameCube\CFXGameCubeMotionBlur.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\source\modules\FX\CFX_MotionBlur\CFXMotionBlur.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FX\CFX_MotionBlur\CFXMotionBlur.h
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FX\CFX_MotionBlur\CFXMotionBlurBase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\FX\CFX_MotionBlur\CFXMotionBlurBase.h
# End Source File
# End Group
# End Group
# Begin Group "Generic"

# PROP Default_Filter ""
# Begin Group "CDebugTools"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\modules\Generic\CDebugTools\CDebugTools.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\Generic\CDebugTools\CDebugTools.h
# End Source File
# End Group
# Begin Group "CDirectorsCamera"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\modules\Generic\CDirectorsCamera\CDirectorsCamera.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\Generic\CDirectorsCamera\CDirectorsCamera.h
# End Source File
# End Group
# Begin Group "CEntity"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\modules\Generic\CEntity\CEntity.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\Generic\CEntity\CEntity.h
# End Source File
# End Group
# Begin Group "CSubRasterCamera"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\modules\Generic\CSubRasterCamera\CSubRasterCamera.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\Generic\CSubRasterCamera\CSubRasterCamera.h
# End Source File
# End Group
# Begin Group "CSetCLevelHint"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\modules\generic\csetclevelhint\csetclevelhint.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\generic\csetclevelhint\csetclevelhint.h
# End Source File
# End Group
# End Group
# Begin Group "InputDevices"

# PROP Default_Filter ""
# Begin Group "CGCNStandardController"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\modules\InputDevices\CGCNStandardController\CGCNStandardController.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\InputDevices\CGCNStandardController\CGCNStandardController.h
# End Source File
# End Group
# End Group
# Begin Group "Logic"

# PROP Default_Filter ""
# Begin Group "AND"

# PROP Default_Filter ""
# End Group
# Begin Group "OR"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\modules\Logic\OR\CLogic_OR.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\Logic\OR\CLogic_OR.h
# End Source File
# End Group
# End Group
# Begin Group "Maestro"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\modules\Maestro\CMaestro.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\Maestro\CMaestro.h
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\Maestro\iCMaestro.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\Maestro\iCMaestro.h
# End Source File
# End Group
# Begin Group "RenderwareAudio"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\modules\RenderwareAudio\AudioGlobalMixer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\RenderwareAudio\AudioGlobalMixer.h
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\RenderwareAudio\audiogroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\RenderwareAudio\audiogroup.h
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\RenderwareAudio\audioreverb.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\RenderwareAudio\audioreverb.h
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\RenderwareAudio\AudioSound3D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\RenderwareAudio\AudioSound3D.h
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\RenderwareAudio\AudioSound3DInterface.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\RenderwareAudio\AudioSound3DInterface.h
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\RenderwareAudio\AudioStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\RenderwareAudio\AudioStream.h
# End Source File
# End Group
# Begin Group "ToolBox"

# PROP Default_Filter ""
# Begin Group "TBMakeTexDict"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\modules\ToolBox\TBMakeTexDict\TBMakeTexDict.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\ToolBox\TBMakeTexDict\TBMakeTexDict.h
# End Source File
# End Group
# End Group
# Begin Group "Tutorials"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\modules\Tutorials\Tutorial1.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\Tutorials\Tutorial1.h
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\Tutorials\Tutorial2.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\Tutorials\Tutorial2.h
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\Tutorials\Tutorial3.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\Tutorials\Tutorial3.h
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\Tutorials\Tutorial4.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\Tutorials\Tutorial4.h
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\Tutorials\Tutorial5.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\Tutorials\Tutorial5.h
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\Tutorials\Tutorial6.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\Tutorials\Tutorial6.h
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\Tutorials\Tutorial7.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\Tutorials\Tutorial7.h
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\Tutorials\Tutorial8.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\Tutorials\Tutorial8.h
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\Tutorials\Tutorial9.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\Tutorials\Tutorial9.h
# End Source File
# End Group
# Begin Group "AreaTriggers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\modules\areatriggers\atbase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\areatriggers\atbase.h
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\areatriggers\atbox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\areatriggers\atbox.h
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\areatriggers\atcylinder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\areatriggers\atcylinder.h
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\areatriggers\atlogic.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\areatriggers\atlogic.h
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\areatriggers\atsphere.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\areatriggers\atsphere.h
# End Source File
# End Group
# Begin Group "Sequencer"

# PROP Default_Filter ""
# Begin Group "Interpolators"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\framework\toolkits\sequencer\interpolators\cseqinterpolrwint16.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\sequencer\interpolators\cseqinterpolrwint16.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\sequencer\interpolators\cseqinterpolrwint32.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\sequencer\interpolators\cseqinterpolrwint32.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\sequencer\interpolators\cseqinterpolrwint8.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\sequencer\interpolators\cseqinterpolrwint8.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\sequencer\interpolators\cseqinterpolrwmatrix.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\sequencer\interpolators\cseqinterpolrwmatrix.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\sequencer\interpolators\cseqinterpolrwreal.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\sequencer\interpolators\cseqinterpolrwreal.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\sequencer\interpolators\cseqinterpolrwrgba.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\sequencer\interpolators\cseqinterpolrwrgba.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\sequencer\interpolators\cseqinterpolrwuint16.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\sequencer\interpolators\cseqinterpolrwuint16.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\sequencer\interpolators\cseqinterpolrwuint32.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\sequencer\interpolators\cseqinterpolrwuint32.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\sequencer\interpolators\cseqinterpolrwuint8.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\sequencer\interpolators\cseqinterpolrwuint8.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\sequencer\interpolators\cseqinterpolrwv3d.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\sequencer\interpolators\cseqinterpolrwv3d.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\source\framework\toolkits\sequencer\cseqctrldata.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\sequencer\cseqctrldata.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\sequencer\cseqstack.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\sequencer\cseqstack.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\sequencer\cseqtxattrhand.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\sequencer\cseqtxattrhand.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\sequencer\seqctrldataresource.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\sequencer\seqctrldataresource.h
# End Source File
# End Group
# End Group
# Begin Group "toolkits"

# PROP Default_Filter ""
# Begin Group "Animation"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\framework\toolkits\Animation\AnimHelper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\Animation\AnimHelper.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\Animation\CAnimSet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\Animation\CAnimSet.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\Animation\SkinSafe.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\Animation\SkinSafe.h
# End Source File
# End Group
# Begin Group "Audio"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\framework\toolkits\Audio\Audio.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\Audio\Audio.h
# End Source File
# End Group
# Begin Group "Collision"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\framework\toolkits\Collision\collide.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\Collision\collide.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\Collision\ellipse.cpp
# End Source File
# End Group
# Begin Group "Math"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\framework\toolkits\Math\maths.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\Math\Maths.h
# End Source File
# End Group
# Begin Group "Particles"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\framework\toolkits\Particles\Particles.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\Particles\Particles.h
# End Source File
# End Group
# Begin Group "RwAudResource"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\framework\toolkits\rwaudresource\rwaudresource.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\rwaudresource\rwaudresource.h
# End Source File
# End Group
# Begin Group "TxtResource"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\framework\toolkits\txtresource\txtresource.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\txtresource\txtresource.h
# End Source File
# End Group
# Begin Group "RwGfxResource"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\framework\toolkits\rwgfxresource\rwgfxresource.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\rwgfxresource\rwgfxresource.h
# End Source File
# End Group
# Begin Group "Time"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\framework\toolkits\time\gftime.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\time\gftime.h
# End Source File
# End Group
# Begin Group "World"

# PROP Default_Filter ""
# Begin Group "Helpers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\framework\toolkits\world\helpers\atomichelper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\world\helpers\atomichelper.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\world\helpers\camerahelper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\world\helpers\camerahelper.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\world\helpers\clumphelper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\world\helpers\clumphelper.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\world\helpers\framehelper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\world\helpers\framehelper.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\world\helpers\lighthelper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\world\helpers\lighthelper.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\world\helpers\skinhelper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\world\helpers\skinhelper.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\world\helpers\worldhelper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\world\helpers\worldhelper.h
# End Source File
# End Group
# Begin Group "SmartPtrs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\framework\toolkits\world\smartptrs\catomicptr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\world\smartptrs\catomicptr.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\world\smartptrs\ccameraptr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\world\smartptrs\ccameraptr.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\world\smartptrs\cclumpptr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\world\smartptrs\cclumpptr.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\world\smartptrs\clightptr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\world\smartptrs\clightptr.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\world\smartptrs\crwobjectptr.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\source\framework\toolkits\world\clevel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\world\clevel.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\world\cmaterialevent.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\world\cmaterialevent.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\world\factory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\world\factory.h
# End Source File
# End Group
# Begin Group "EventVisualizer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\framework\toolkits\eventvisualization\eventvisualization.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\eventvisualization\eventvisualization.h
# End Source File
# End Group
# Begin Group "DebugTools"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\framework\toolkits\debugtools\debugswitches.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\debugtools\debugswitches.h
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\debugtools\debugtools.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\debugtools\debugtools.h
# End Source File
# End Group
# Begin Group "rwsplineresource"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\framework\toolkits\rwsplineresource\rwsplineresource.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\framework\toolkits\rwsplineresource\rwsplineresource.h
# End Source File
# End Group
# Begin Group "SeqCtrl"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\modules\seqctrl\cconductor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\seqctrl\cconductor.h
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\seqctrl\cseqcamera.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\seqctrl\cseqcamera.h
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\seqctrl\cseqpos.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\modules\seqctrl\cseqpos.h
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
