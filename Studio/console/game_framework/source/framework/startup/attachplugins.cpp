/*****************************************************************************
 *
 * File :     attachplugins.cpp
 *
 * Abstract : Utility function used to attach RenderWare plugin data.
 *
 *****************************************************************************
 *
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
 * Copyright (c) 2000 Criterion Software Ltd.
 * All Rights Reserved.
 *
 * RenderWare is a trademark of Canon Inc.
 *
 *****************************************************************************/

#include "precomp.h"

//////////////////////////////////////////////////////////////////
// 
// RenderWare Studio Includes
//
#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/world/clevel.h"
#include "framework/toolkits/world/cmaterialevent.h"
#include "framework/core/attributehandler/csystemcommands.h"
#include "framework/core/eventhandler/ceventhandler.h"

//////////////////////////////////////////////////////////////////
//
// RenderWare Includes
//
#include "rpcollis.h"
#include "rtintsec.h"
#include "rprandom.h"
#include "rpskin.h"
#include "rtcharse.h"
#include "rppvs.h"
#include "rpmorph.h"
#include "rpdmorph.h"
#include "rplodatm.h"
#include "rppatch.h"
#include "rpmatfx.h"
#include "rpptank.h"
#include "rpprtstd.h"
#include "rpspline.h"
#include "rpltmap.h"
#include "rt2danim.h"
#include "rpusrdat.h"
#include "rtcmpkey.h"


#if (defined (_XBOX))
#include "rpanisot.h"
#endif

// RenderWare Version Exceeds 3.4
//      
#if (rwLIBRARYCURRENTVERSION >= 0x34002)
#if (defined(SKY))
#include "rppds.h"
#endif
#endif

// Auto Include Libs
//
#ifdef _MSC_VER
#pragma comment (lib, "rpusrdat.lib")
#pragma comment (lib, "rtquat.lib")
#pragma comment (lib, "rtslerp.lib")
#pragma comment (lib, "rpskinmatfx.lib")
#pragma comment (lib, "rwcore.lib")
#pragma comment (lib, "rphanim.lib")
#pragma comment (lib, "rppvs.lib")
#pragma comment (lib, "rprandom.lib")
#pragma comment (lib, "rpmorph.lib")
#pragma comment (lib, "rplodatm.lib")
#pragma comment (lib, "rpltmap.lib")
#pragma comment (lib, "rpptank.lib")
#pragma comment (lib, "rpprtstd.lib")
#pragma comment (lib, "rtpick.lib")
#pragma comment (lib, "rpdmorph.lib")
#pragma comment (lib, "rtpitexd.lib")
#pragma comment (lib, "rprandom.lib")
#pragma comment (lib, "rppatchskinmatfx.lib")
#pragma comment (lib, "rtbezpat.lib")
#pragma comment (lib, "rt2d.lib")
#pragma comment (lib, "rt2danim.lib")
#pragma comment (lib, "rtpng.lib")
#pragma comment (lib, "rtbmp.lib")
#pragma comment (lib, "rtcharse.lib")
#pragma comment (lib, "rtintsec.lib")
#pragma comment (lib, "rtworld.lib")
#pragma comment (lib, "rpmatfx.lib")
#pragma comment (lib, "rpcollis.lib")
#pragma comment (lib, "rppvs.lib")
#pragma comment (lib, "rpspline.lib")
#pragma comment (lib, "rplogo.lib")
#pragma comment (lib, "rpworld.lib")
#pragma comment (lib, "rwcore.lib")
#pragma comment (lib, "rtanim.lib")
#pragma comment (lib, "rtcmpkey.lib")
#if (defined (_XBOX))
#pragma comment (lib, "rpanisot.lib")
#endif
#endif

#if (defined (XBOX_DRVMODEL_H))
#include "modules\fx\cfx_xbox_vertexshader\cfxxboxvertexshaderbase.h"
#endif

#if (rwLIBRARYCURRENTVERSION >= 0x34002)
#if (defined(SKY))      
#include "sky\skyregisterpipes.h"
#endif
#endif

namespace RWS
{
   namespace StartUp
   {
      /*
      *
      *  \ingroup StartUp
      *
      *  Attaches all plugins.
      *
      *  \return Returns true on success.
      */
      bool AttachPlugins()
      {
         RWS_FUNCTION("AttachPlugins");
         
#if (defined(__MWERKS__) || defined(SN_TARGET_PS2)) || defined(SN_TARGET_NGC)
         typedef RwBool (*functor)(void);
#else
         typedef RwBool (__cdecl *functor)(void);
#endif
         
         typedef struct pluginlist_tag
         {
            functor pFunc;
#ifdef RWS_TRACEENABLE
            char *pName;
#define ADD_PLUGIN(a) {a, #a}
#else
#define ADD_PLUGIN(a) {a}
#endif
         } pluginlist;
         
         static const pluginlist plugins[] = 
         {
            ADD_PLUGIN(RpWorldPluginAttach),
            ADD_PLUGIN(RpCollisionPluginAttach),
            ADD_PLUGIN(RpPVSPluginAttach),
            ADD_PLUGIN(RpMorphPluginAttach),
            ADD_PLUGIN(RpDMorphPluginAttach),
               
            ADD_PLUGIN(RpSkinPluginAttach),
            ADD_PLUGIN(RpHAnimPluginAttach),
               
            ADD_PLUGIN(RpRandomPluginAttach),
               
            ADD_PLUGIN(RpPatchPluginAttach),
            ADD_PLUGIN(RpMatFXPluginAttach),
               
            ADD_PLUGIN(RpPTankPluginAttach),
            ADD_PLUGIN(RpLtMapPluginAttach),
            ADD_PLUGIN(RpLODAtomicPluginAttach),
            ADD_PLUGIN(RpSplinePluginAttach),
            ADD_PLUGIN(RpUserDataPluginAttach),
               
#if (defined (XBOX_DRVMODEL_H))
            ADD_PLUGIN(RpAnisotPluginAttach),
            ADD_PLUGIN(FX::CFXXBoxBaseVertexShader::PluginAttach),
#endif
            // Finally Add RenderWare Studio RpAtomic -> CEventHandler Plugin
            //
            ADD_PLUGIN(RpAtomicToEventHandler::PluginAttach),
            ADD_PLUGIN(RpAtomicCollisionProperties::PluginAttach),
            ADD_PLUGIN(CLevel::PluginAttach),
            ADD_PLUGIN(MaterialHelper::CMaterialEvent::MaterialEventPluginAttach)
         };

#if (rwLIBRARYCURRENTVERSION >= 0x34002)
         /*
         *
         *  Must be done before attaching plugins.
         *
         */
         if (!RtAnimInitialize())
         {
            RWS_ASSERTFAIL ("Unable to initialize RtAnim");

            RWS_RETURN(false);
         }
#endif
         
         // RenderWare Version Exceeds 3.4
         //      
#if (rwLIBRARYCURRENTVERSION >= 0x34002)
#if (defined(SKY))      
         RpPDSPluginAttach(RpWorldNumPipes);
         
         Pipelines::PrePluginAttachRegisterPipeLines();
#endif
#endif
         
         bool flag = true;
         for (RwUInt32 i = 0;i < sizeof(plugins) / sizeof(pluginlist);i++)
         {
#ifdef RWS_TRACEENABLE
            RWS_TRACE("Attaching Pluging : "<<plugins[i].pName);
#endif
            if (!plugins[i].pFunc())
            {
               RWS_ASSERTFAIL ("Unable to Attach Plugin: Index ["<<i<<"]");
               
               flag = false;
            }
         }
         
         // RenderWare Version Exceeds 3.4
         //      
#if (rwLIBRARYCURRENTVERSION >= 0x34002)
#if (defined(SKY))      
         Pipelines::PostPluginAttachRegisterPipeLines();
#endif      
#endif
         
         // Register the compressed keyframe animation scheme.
         //
         if (!RtCompressedKeyFrameRegister())
         {
            RWS_ASSERTFAIL("Compressed keyframe scheme registration failed.");
         }

         RWS_RETURN(flag);
      }
   }
}
