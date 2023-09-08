/*****************************************************************************
* This file is a product of Criterion Software Ltd.
*
* This file is provided as is with no warranties of any kind and is
* provided without any obligation on Criterion Software Ltd. or
* Canon Inc. to assist in its use or modification.
*
* Criterion Software Ltd. will not, under any
* circumstances, be liable for any lost revenue or other damages arising
* from the use of this file.
*
* Copyright (c) 2002 Criterion Software Ltd.
* All Rights Reserved.
*
* RenderWare is a trademark of Canon Inc.
*
*****************************************************************************/

#ifndef __SKYIOP_H__
#define __SKYIOP_H__

#if (defined(CDROM) || defined(DVDROM))
#define IOP_MODULEPATH_CDROM    "\\"
#define IOP_RWSCOMMSPATH_CDROM  "\\"
#define RWS_MODULEPATH_CDROM    "\\"
#endif // #if (defined(CDROM) || defined(DVDROM))

#ifdef WITH_VISUALIZER
#define IOP_MODULEPATH          "modules/"
#define IOP_RWSCOMMSPATH        "modules/"
#else
// Sony IOP Modules
//
#define IOP_MODULEPATH          "/usr/local/sce/iop/modules/"
// RenderWare Studio network IOP modules, the IOP modules are typically built to the following folder
// (INSTALL)/console/game_framework/bin/modules/ Due to the empty framework linking using the gfCore
// (which is within the game framework folders) a relative path CANNOT be used. If it were the empty
// framework would fail as the relative path to the IOP modules would be wrong from the empty framework
// when any gfCore code loaded a module. For this reason an absolute path must be used for both the
// empty and game frameworks.
//
#define IOP_RWSCOMMSPATH        "/rwgsdk_pc_v3.7_eval/Studio/Console/game_framework/bin/modules/"

#endif // WITH_VISUALIZER

// RenderWare Audio IOP modules
//
#ifdef RWADEBUG
#define  RWS_AUDIO_MODULEPATH   "/rw/audio/lib/ps2/debug/"
#elif  RWAMETRICS
#define  RWS_AUDIO_MODULEPATH   "/rw/audio/lib/ps2/metrics/"
#else
#define  RWS_AUDIO_MODULEPATH   "/rw/audio/lib/ps2/release/"
#endif // RWDEBUG

namespace RWS
{
   const char * GetRootFileServerPath();
   void SetRootFileServerPath( const char * szRootFileServerPath );
}

#endif // __SKYIOP_H__
