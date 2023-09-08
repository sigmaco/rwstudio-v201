/*****************************************************************************
*
* File :     CPipelines.h
*
* Abstract:   Declaration of pipeline behaviors.
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
* Copyright (c) 2001 Criterion Software Ltd.
* All Rights Reserved.
*
* RenderWare is a trademark of Canon Inc.
*
*****************************************************************************/


#ifndef __CG3PIPESELECT_H__
#define __CG3PIPESELECT_H__

#if defined (SKY)

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "cpipeselectbase.h"
#include "framework/core/eventhandler/ceventhandler.h"

namespace RWS
{
   namespace Pipelines
   {
      struct ADL_LightingSetupCB_data
      {
         RpLight *ambient;
         RpLight *directional;
      };
      
      struct APL_LightingSetupCB_data
      {
         RpLight *ambient;
         RpLight *point;
      };
      
      struct A4D_LightingSetupCB_data
      {
         RpLight *ambient;
         RpLight *directional[4];
         RwUInt32 uiIndex;
      };
      
      RpLight *ADL_LightingSetupCB(RpLight *pLight, void *pData);   // for pData use ADL_LightingSetupCB_data
      RpLight *APL_LightingSetupCB(RpLight *pLight, void *pData);   // for pData use APL_LightingSetupCB_data
      RpLight *A4D_LightingSetupCB(RpLight *pLight, void *pData);   // for pData use A4D_LightingSetupCB_data
      
      RpAtomic *G3x_ADL_DefaultLightingCB(RpAtomic *);
      RpAtomic *G3x_APL_DefaultLightingCB(RpAtomic *);
      RpAtomic *G3x_A4D_DefaultLightingCB(RpAtomic *);

      /**
      *
      *  \ingroup Mod_Pipelines
      *
      *  \page Mod_PipelinesOverview Overview
      *
      *  RenderWare Gfx provides in addition to the generic pipelines a library of custom pipelines. These
      *  pipelines can be used to significantly
      *  improve rendering performance under certain conditions. These conditions are typically lighting
      *  policies, that is the lighting of the object
      *  can be considered fixed i.e. (i) lights of type A and (j) lights of type B. For a more detailed
      *  explanation of each of the custom pipelines
      *  please refer to the RenderWare Gfx documentation.
      *
      *  RenderWare studio provides behaviors for attaching pipelines to RenderWare assets, once the pipelines
      *  have been attached any instances of that
      *  object will use the new pipeline i.e. if you know the world contains 1 ambient and 1 directional light
      *  and contains 10 instances of a skinned characters, then 
      *  by attaching the CG3x_ADLSkin_MatPipeID to the skinned character asset, (has the effect of applying the
      *  pipeline 
      *  to each of the assets) each of the 10 instances of the character will use the custom pipeline.
      *
      *  As part of the pipeline behaviors, code to setup the lights for each of the custom pipelines has been
      *  included \ref G3x_ADL_DefaultLightingCB, 
      *  \ref G3x_APL_DefaultLightingCB and \ref G3x_A4D_DefaultLightingCB. Although these provide a generic
      *  solution. Further performance
      *  benefits can be achieved if more of the scene can be assumed constant, i.e. the lighting can be setup
      *  once for all atomics using
      *  a particular lighting pipeline rather than once for each atomic as is done by the generic solution.
      *  
      *  \note Custom pipes are included as part of the executable by referencing their register macros these
      *  have been collected into a single place see
      *  \ref PrePluginAttachRegisterPipeLines and \ref PostPluginAttachRegisterPipeLines. Once the required
      *  pipelines have been determined the unused
      *  pipelines should be removed i.e. comment out the register macros, to reduce the size of the executable.
      *
      *  \note The pipeline behaviors should be called to modify the asset before the asset is cloned
      *  \ref CreateAtomicInWorldFromResource, i.e. make sure the pipeline behaviors are
      *  near the start of the level in the hierarchy view in the game database shown within the workspace
      *  of RenderWare Studio.
      *
      *  \note The naming convention for the behaviors is based on the name of the pipeline ID, for example
      *  the behavior which attaches the 
      *  rwPDS_G3_Generic_MatPipeID pipe is called CG3_Generic_MatPipeID.
      */
      
      //-------------------------------------------------------------------------------------
      // Material Pipelines
      //-------------------------------------------------------------------------------------
      
      /**
      *
      *  \ingroup Mod_Pipelines
      *
      *  Behavior used to attach the rwPDS_G3x_APL_MatPipeID pipe to an asset.
      *
      */
      class CG3x_APL_MatPipeID : public CPipeSelectBase
      {
      public:
         
         RWS_MAKENEWCLASS(CG3x_APL_MatPipeID);
         RWS_DECLARE_CLASSID(CG3x_APL_MatPipeID);
         
         RWS_CATEGORY("Material Pipeline");
         
         RWS_DESCRIPTION("Ambient and Point Light Generic rendering.",
            "Attaches the rwPDS_G3x_APL_MatPipeID pipeline to each asset.");
         
         CG3x_APL_MatPipeID(const CAttributePacket &rAttr) :
         CPipeSelectBase(rAttr, rwPDS_G3x_APL_MatPipeID, rwPDS_G3x_Generic_AtmPipeID, G3x_APL_DefaultLightingCB, (RpPDSPipeID)0, 0)
         {
         }
      };
      
      /**
      *
      *  \ingroup Mod_Pipelines
      *
      *  Behavior used to attach the rwPDS_G3x_ADL_MatPipeID pipe to an asset.
      *
      */
      class CG3x_ADL_MatPipeID : public CPipeSelectBase
      {
      public:
         
         RWS_MAKENEWCLASS(CG3x_ADL_MatPipeID);
         RWS_DECLARE_CLASSID(CG3x_ADL_MatPipeID);
         
         RWS_CATEGORY("Material Pipeline");
         
         RWS_DESCRIPTION("Ambient and Direction Light Generic rendering.",
            "Attaches the rwPDS_G3x_ADL_MatPipeID pipeline to each asset.");
         
         CG3x_ADL_MatPipeID(const CAttributePacket &rAttr) :
         CPipeSelectBase(rAttr, rwPDS_G3x_ADL_MatPipeID, rwPDS_G3x_Generic_AtmPipeID, G3x_ADL_DefaultLightingCB, (RpPDSPipeID)0, 0)
         {
         }
      };
      
      /**
      *
      *  \ingroup Mod_Pipelines
      *
      *  Behavior used to attach the rwPDS_G3x_A4D_MatPipeID pipe to an asset.
      *
      */
      class CG3x_A4D_MatPipeID : public CPipeSelectBase
      {
      public:
         
         RWS_MAKENEWCLASS(CG3x_A4D_MatPipeID);
         RWS_DECLARE_CLASSID(CG3x_A4D_MatPipeID);
         
         RWS_CATEGORY("Material Pipeline");
         
         RWS_DESCRIPTION("Ambient and 4 Direction Lights Generic rendering.",
            "Attaches the rwPDS_G3x_A4D_MatPipeID pipeline to each asset.");
         
         CG3x_A4D_MatPipeID(const CAttributePacket &rAttr) :
         CPipeSelectBase(rAttr, rwPDS_G3x_A4D_MatPipeID, rwPDS_G3x_Generic_AtmPipeID, G3x_A4D_DefaultLightingCB, (RpPDSPipeID)0, 0)
         {
         }
      };
      
      /**
      *
      *  \ingroup Mod_Pipelines
      *
      *  Behavior used to attach the rwPDS_G3x_APLDup_MatPipeID pipe to an asset.
      *
      */
      class CG3x_APLDup_MatPipeID : public CPipeSelectBase
      {
      public:
         
         RWS_MAKENEWCLASS(CG3x_APLDup_MatPipeID);
         RWS_DECLARE_CLASSID(CG3x_APLDup_MatPipeID);
         
         RWS_CATEGORY("Material Pipeline");
         
         RWS_DESCRIPTION("Ambient and Point Light DUal Pass rendering.",
            "Attaches the rwPDS_G3x_APLDup_MatPipeID pipeline to each asset.");
         
         CG3x_APLDup_MatPipeID(const CAttributePacket &rAttr) :
         CPipeSelectBase(rAttr, rwPDS_G3x_APLDup_MatPipeID, rwPDS_G3x_Generic_AtmPipeID, G3x_APL_DefaultLightingCB, (RpPDSPipeID)0, 0)
         {
         }
      };
      
      /**
      *
      *  \ingroup Mod_Pipelines
      *
      *  Behavior used to attach the rwPDS_G3x_ADLDup_MatPipeID pipe to an asset.
      *
      */
      class CG3x_ADLDup_MatPipeID : public CPipeSelectBase
      {
      public:
         
         RWS_MAKENEWCLASS(CG3x_ADLDup_MatPipeID);
         RWS_DECLARE_CLASSID(CG3x_ADLDup_MatPipeID);
         
         RWS_CATEGORY("Material Pipeline");
         
         RWS_DESCRIPTION("Ambient and Direction Light DUal Pass rendering.",
            "Attaches the rwPDS_G3x_ADLDup_MatPipeID pipeline to each asset.");
         
         CG3x_ADLDup_MatPipeID(const CAttributePacket &rAttr) :
         CPipeSelectBase(rAttr, rwPDS_G3x_ADLDup_MatPipeID, rwPDS_G3x_Generic_AtmPipeID, G3x_ADL_DefaultLightingCB, (RpPDSPipeID)0, 0)
         {
         }
      };
      
      /**
      *
      *  \ingroup Mod_Pipelines
      *
      *  Behavior used to attach the rwPDS_G3x_A4DDup_MatPipeID pipe to an asset.
      *
      */
      class CG3x_A4DDup_MatPipeID : public CPipeSelectBase
      {
      public:
         
         RWS_MAKENEWCLASS(CG3x_A4DDup_MatPipeID);
         RWS_DECLARE_CLASSID(CG3x_A4DDup_MatPipeID);
         
         RWS_CATEGORY("Material Pipeline");
         
         RWS_DESCRIPTION("Ambient and 4 Direction Lights DUal Pass rendering",
            "Attaches the rwPDS_G3x_A4DDup_MatPipeID pipeline to each asset.");
         
         CG3x_A4DDup_MatPipeID(const CAttributePacket &rAttr) :
         CPipeSelectBase(rAttr, rwPDS_G3x_A4DDup_MatPipeID, rwPDS_G3x_Generic_AtmPipeID, G3x_A4D_DefaultLightingCB, (RpPDSPipeID)0, 0)
         {
         }
      };
      
      /**
      *
      *  \ingroup Mod_Pipelines
      *
      *  Behavior used to attach the rwPDS_G3x_APLGem_MatPipeID pipe to an asset.
      *
      */
      class CG3x_APLGem_MatPipeID : public CPipeSelectBase
      {
      public:
         
         RWS_MAKENEWCLASS(CG3x_APLGem_MatPipeID);
         RWS_DECLARE_CLASSID(CG3x_APLGem_MatPipeID);
         
         RWS_CATEGORY("Material Pipeline");
         
         RWS_DESCRIPTION("Ambient and Point Light Gloss Environment Map rendering.",
            "Attaches the rwPDS_G3x_APLGem_MatPipeID pipeline to each asset.");
         
         CG3x_APLGem_MatPipeID(const CAttributePacket &rAttr) :
         CPipeSelectBase(rAttr, rwPDS_G3x_APLGem_MatPipeID, rwPDS_G3x_Generic_AtmPipeID, G3x_APL_DefaultLightingCB, (RpPDSPipeID)0, 0)
         {
         }
      };
      
      /**
      *
      *  \ingroup Mod_Pipelines
      *
      *  Behavior used to attach the rwPDS_G3x_ADLGem_MatPipeID pipe to an asset.
      *
      */
      class CG3x_ADLGem_MatPipeID : public CPipeSelectBase
      {
      public:
         
         RWS_MAKENEWCLASS(CG3x_ADLGem_MatPipeID);
         RWS_DECLARE_CLASSID(CG3x_ADLGem_MatPipeID);
         
         RWS_CATEGORY("Material Pipeline");
         
         RWS_DESCRIPTION("Ambient and Direction Light Gloss Environment Map rendering.",
            "Attaches the rwPDS_G3x_ADLGem_MatPipeID pipeline to each asset.");
         
         CG3x_ADLGem_MatPipeID(const CAttributePacket &rAttr) :
         CPipeSelectBase(rAttr, rwPDS_G3x_ADLGem_MatPipeID, rwPDS_G3x_Generic_AtmPipeID, G3x_ADL_DefaultLightingCB, (RpPDSPipeID)0, 0)
         {
         }
      };
      
      /**
      *
      *  \ingroup Mod_Pipelines
      *
      *  Behavior used to attach the rwPDS_G3x_A4DGem_MatPipeID pipe to an asset.
      *
      */
      class CG3x_A4DGem_MatPipeID : public CPipeSelectBase
      {
      public:
         
         RWS_MAKENEWCLASS(CG3x_A4DGem_MatPipeID);
         RWS_DECLARE_CLASSID(CG3x_A4DGem_MatPipeID);
         
         RWS_CATEGORY("Material Pipeline");
         
         RWS_DESCRIPTION("Ambient and 4 Direction Lights Gloss Environment Map rendering.",
            "Attaches the rwPDS_G3x_A4DGem_MatPipeID pipeline to each asset.");
         
         CG3x_A4DGem_MatPipeID(const CAttributePacket &rAttr) :
         CPipeSelectBase(rAttr, rwPDS_G3x_A4DGem_MatPipeID, rwPDS_G3x_Generic_AtmPipeID, G3x_A4D_DefaultLightingCB, (RpPDSPipeID)0, 0)
         {
         }
      };
      
      /**
      *
      *  \ingroup Mod_Pipelines
      *
      *  Behavior used to attach the rwPDS_G3x_APLSkin_MatPipeID pipe to an asset.
      *
      */
      class CG3x_APLSkin_MatPipeID : public CPipeSelectBase
      {
      public:
         
         RWS_MAKENEWCLASS(CG3x_APLSkin_MatPipeID);
         RWS_DECLARE_CLASSID(CG3x_APLSkin_MatPipeID);
         
         RWS_CATEGORY("Material Pipeline");
         
         RWS_DESCRIPTION("Ambient and Point Light Skin Generic rendering.",
            "Attaches the rwPDS_G3x_APLSkin_MatPipeID pipeline to each asset.");
         
         CG3x_APLSkin_MatPipeID(const CAttributePacket &rAttr) :
         CPipeSelectBase(rAttr, rwPDS_G3x_APLSkin_MatPipeID, rwPDS_G3x_Skin_AtmPipeID, G3x_APL_DefaultLightingCB, (RpPDSPipeID)0, 0)
         {
         }
      };
      
      /**
      *
      *  \ingroup Mod_Pipelines
      *
      *  Behavior used to attach the rwPDS_G3x_ADLSkin_MatPipeID pipe to an asset.
      *
      */
      class CG3x_ADLSkin_MatPipeID : public CPipeSelectBase
      {
      public:
         
         RWS_MAKENEWCLASS(CG3x_ADLSkin_MatPipeID);
         RWS_DECLARE_CLASSID(CG3x_ADLSkin_MatPipeID);
         
         RWS_CATEGORY("Material Pipeline");
         
         RWS_DESCRIPTION("Ambient and Direction Light Skin Generic rendering.",
            "Attaches the rwPDS_G3x_ADLSkin_MatPipeID pipeline to each asset.");
         
         CG3x_ADLSkin_MatPipeID(const CAttributePacket &rAttr) :
         CPipeSelectBase(rAttr, rwPDS_G3x_ADLSkin_MatPipeID, rwPDS_G3x_Skin_AtmPipeID, G3x_ADL_DefaultLightingCB, (RpPDSPipeID)0, 0)
         {
         }
      };
      
      /**
      *
      *  \ingroup Mod_Pipelines
      *
      *  Behavior used to attach the rwPDS_G3x_A4DSkin_MatPipeID pipe to an asset.
      *
      */
      class CG3x_A4DSkin_MatPipeID : public CPipeSelectBase
      {
      public:
         
         RWS_MAKENEWCLASS(CG3x_A4DSkin_MatPipeID);
         RWS_DECLARE_CLASSID(CG3x_A4DSkin_MatPipeID);
         
         RWS_CATEGORY("Material Pipeline");
         
         RWS_DESCRIPTION("Ambient and 4 Direction Lights Skin Generic rendering.",
            "Attaches the rwPDS_G3x_A4DSkin_MatPipeID pipeline to each asset.");
         
         CG3x_A4DSkin_MatPipeID(const CAttributePacket &rAttr) :
         CPipeSelectBase(rAttr, rwPDS_G3x_A4DSkin_MatPipeID, rwPDS_G3x_Skin_AtmPipeID, G3x_A4D_DefaultLightingCB, (RpPDSPipeID)0, 0)
         {
         }
      };
      
      /**
      *
      *  \ingroup Mod_Pipelines
      *
      *  Behavior used to attach the rwPDS_G3x_APLSkinDup_MatPipeID pipe to an asset.
      *
      */
      class CG3x_APLSkinDup_MatPipeID : public CPipeSelectBase
      {
      public:
         
         RWS_MAKENEWCLASS(CG3x_APLSkinDup_MatPipeID);
         RWS_DECLARE_CLASSID(CG3x_APLSkinDup_MatPipeID);
         
         RWS_CATEGORY("Material Pipeline");
         
         RWS_DESCRIPTION("Ambient and Point Light Skin DUal Pass rendering",
            "Attaches the rwPDS_G3x_APLSkinDup_MatPipeID pipeline to each asset.");
         
         CG3x_APLSkinDup_MatPipeID(const CAttributePacket &rAttr) :
         CPipeSelectBase(rAttr, rwPDS_G3x_APLSkinDup_MatPipeID, rwPDS_G3x_Skin_AtmPipeID, G3x_APL_DefaultLightingCB, (RpPDSPipeID)0, 0)
         {
         }
      };
      
      /**
      *
      *  \ingroup Mod_Pipelines
      *
      *  Behavior used to attach the rwPDS_G3x_ADLSkinDup_MatPipeID pipe to an asset.
      *
      */
      class CG3x_ADLSkinDup_MatPipeID : public CPipeSelectBase
      {
      public:
         
         RWS_MAKENEWCLASS(CG3x_ADLSkinDup_MatPipeID);
         RWS_DECLARE_CLASSID(CG3x_ADLSkinDup_MatPipeID);
         
         RWS_CATEGORY("Material Pipeline");
         
         RWS_DESCRIPTION("Ambient and Direction Light Skin DUal Pass rendering.",
            "Attaches the rwPDS_G3x_ADLSkinDup_MatPipeID pipeline to each asset.");
         
         CG3x_ADLSkinDup_MatPipeID(const CAttributePacket &rAttr) :
         CPipeSelectBase(rAttr, rwPDS_G3x_ADLSkinDup_MatPipeID, rwPDS_G3x_Skin_AtmPipeID, G3x_ADL_DefaultLightingCB, (RpPDSPipeID)0, 0)
         {
         }
      };
      
      /**
      *
      *  \ingroup Mod_Pipelines
      *
      *  Behavior used to attach the rwPDS_G3x_A4DSkinDup_MatPipeID pipe to an asset.
      *
      */
      class CG3x_A4DSkinDup_MatPipeID : public CPipeSelectBase
      {
      public:
         
         RWS_MAKENEWCLASS(CG3x_A4DSkinDup_MatPipeID);
         RWS_DECLARE_CLASSID(CG3x_A4DSkinDup_MatPipeID);
         
         RWS_CATEGORY("Material Pipeline");
         
         RWS_DESCRIPTION("Ambient and 4 Direction Lights Skin DUal Pass rendering",
            "Attaches the rwPDS_G3x_A4DSkinDup_MatPipeID pipeline to each asset.");
         
         CG3x_A4DSkinDup_MatPipeID(const CAttributePacket &rAttr) :
         CPipeSelectBase(rAttr, rwPDS_G3x_A4DSkinDup_MatPipeID, rwPDS_G3x_Skin_AtmPipeID, G3x_A4D_DefaultLightingCB, (RpPDSPipeID)0, 0)
         {
         }
      };
      
      /**
      *
      *  \ingroup Mod_Pipelines
      *
      *  Behavior used to attach the rwPDS_G3x_APLSkinGem_MatPipeID pipe to an asset.
      *
      */
      class CG3x_APLSkinGem_MatPipeID : public CPipeSelectBase
      {
      public:
         
         RWS_MAKENEWCLASS(CG3x_APLSkinGem_MatPipeID);
         RWS_DECLARE_CLASSID(CG3x_APLSkinGem_MatPipeID);
         
         RWS_CATEGORY("Material Pipeline");
         
         RWS_DESCRIPTION("Ambient and Point Light Skin Gloss Environment Map rendering.",
            "Attaches the rwPDS_G3x_APLSkinGem_MatPipeID pipeline to each asset.");
         
         CG3x_APLSkinGem_MatPipeID(const CAttributePacket &rAttr) :
         CPipeSelectBase(rAttr, rwPDS_G3x_APLSkinGem_MatPipeID, rwPDS_G3x_Skin_AtmPipeID, G3x_APL_DefaultLightingCB, (RpPDSPipeID)0, 0)
         {
         }
      };
      
      /**
      *
      *  \ingroup Mod_Pipelines
      *
      *  Behavior used to attach the rwPDS_G3x_ADLSkinGem_MatPipeID pipe to an asset.
      *
      */
      class CG3x_ADLSkinGem_MatPipeID : public CPipeSelectBase
      {
      public:
         
         RWS_MAKENEWCLASS(CG3x_ADLSkinGem_MatPipeID);
         RWS_DECLARE_CLASSID(CG3x_ADLSkinGem_MatPipeID);
         
         RWS_CATEGORY("Material Pipeline");
         
         RWS_DESCRIPTION("Ambient and Direction Light Skin Gloss Environment Map rendering.",
            "Attaches the specified pipeline to each asset.");
         
         CG3x_ADLSkinGem_MatPipeID(const CAttributePacket &rAttr) :
         CPipeSelectBase(rAttr, rwPDS_G3x_ADLSkinGem_MatPipeID, rwPDS_G3x_Skin_AtmPipeID, G3x_ADL_DefaultLightingCB, (RpPDSPipeID)0, 0)
         {
         }
      };
      
      /**
      *
      *  \ingroup Mod_Pipelines
      *
      *  Behavior used to attach the rwPDS_G3x_A4DSkinGem_MatPipeID pipe to an asset.
      *
      */
      class CG3x_A4DSkinGem_MatPipeID : public CPipeSelectBase
      {
      public:
         
         RWS_MAKENEWCLASS(CG3x_A4DSkinGem_MatPipeID);
         RWS_DECLARE_CLASSID(CG3x_A4DSkinGem_MatPipeID);
         
         RWS_CATEGORY("Material Pipeline");
         
         RWS_DESCRIPTION("Ambient and 4 Direction Lights Skin Gloss Environment Map rendering.",
            "Attaches the rwPDS_G3x_A4DSkinGem_MatPipeID pipeline to each asset.");
         
         CG3x_A4DSkinGem_MatPipeID(const CAttributePacket &rAttr) :
         CPipeSelectBase(rAttr, rwPDS_G3x_A4DSkinGem_MatPipeID, rwPDS_G3x_Skin_AtmPipeID, G3x_A4D_DefaultLightingCB, (RpPDSPipeID)0, 0)
         {
         }
      };
   }
}

#endif
#endif

//rwPDS_G3x_Generic_AtmPipeID
//rwPDS_G3x_Skin_AtmPipeID
