/*****************************************************************************
*
* File :     CPipelines.cpp
*
* Abstract:   Implementation of pipeline behaviors
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

#include "precomp.h"

#if defined (SKY) // PS2 Only

// Left to its own devices, the MW linker will dead-strip the registrar ctors, so...
#ifdef __MWERKS__
#pragma force_active on
#pragma exceptions off // Turn off exceptions due to bug in MW Dwarf Generator for PS2, present in CW 3.04
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "cpipelines.h"
#include "framework/core/macros/debugmacros.h"

namespace RWS
{
   namespace Pipelines
   {
      //-------------------------------------------------------------------------------------
      // Material Pipelines
      //-------------------------------------------------------------------------------------
      RWS_REGISTERCLASS(CG3x_APL_MatPipeID);
      RWS_REGISTERCLASS(CG3x_ADL_MatPipeID);
      RWS_REGISTERCLASS(CG3x_A4D_MatPipeID);
      RWS_REGISTERCLASS(CG3x_APLDup_MatPipeID);
      RWS_REGISTERCLASS(CG3x_ADLDup_MatPipeID);
      RWS_REGISTERCLASS(CG3x_A4DDup_MatPipeID);
      RWS_REGISTERCLASS(CG3x_APLGem_MatPipeID);
      RWS_REGISTERCLASS(CG3x_ADLGem_MatPipeID);
      RWS_REGISTERCLASS(CG3x_A4DGem_MatPipeID);
      RWS_REGISTERCLASS(CG3x_APLSkin_MatPipeID);
      RWS_REGISTERCLASS(CG3x_ADLSkin_MatPipeID);
      RWS_REGISTERCLASS(CG3x_A4DSkin_MatPipeID);
      RWS_REGISTERCLASS(CG3x_APLSkinDup_MatPipeID);
      RWS_REGISTERCLASS(CG3x_ADLSkinDup_MatPipeID);
      RWS_REGISTERCLASS(CG3x_A4DSkinDup_MatPipeID);
      RWS_REGISTERCLASS(CG3x_APLSkinGem_MatPipeID);
      RWS_REGISTERCLASS(CG3x_ADLSkinGem_MatPipeID);
      RWS_REGISTERCLASS(CG3x_A4DSkinGem_MatPipeID);
      
      /**
      *
      *  \ingroup Mod_Pipelines
      *
      *  RpLight call back function used to locate 1 directional and 1 ambient light,
      *  used by G3x_ADL_DefaultLightingCB.
      *
      *  \param pLight Pointer to RenderWare Gfx RpLight object.
      *
      *  \param pData Pointer to custom data, in this case a ADL_LightingSetupCB_data structure.
      *
      *  \return Pointer to pLight, or 0 when 1 ambient and 1 directional light has been found.
      */
      RpLight *ADL_LightingSetupCB(RpLight *pLight, void *pData)
      {
         RWS_FUNCTION("RWS::Pipelines::NULL::ADL_LightingSetupCB");
         
         ADL_LightingSetupCB_data *pADL_LightingSetupCB_data =
            reinterpret_cast<ADL_LightingSetupCB_data*>(pData);
         
         switch (RpLightGetType(pLight))
         {
         case rpLIGHTDIRECTIONAL:
            pADL_LightingSetupCB_data->directional = pLight;
            break;
         case rpLIGHTAMBIENT:
            pADL_LightingSetupCB_data->ambient = pLight;
            break;
         default:
            break;
         }
         
         if (pADL_LightingSetupCB_data->directional && pADL_LightingSetupCB_data->ambient)
         {
            RWS_RETURN(0); // Found 1 directional + 1 ambient light early exit
         }
         
         RWS_RETURN(pLight);
      }
      
      /**
      *
      *  \ingroup Mod_Pipelines
      *
      *  RpAtomicRenderCallback used to setup 1 directional and 1 ambient light when using a G3x_ADL pipeline.
      *
      *  \param pAtomic Pointer to a RenderWare Gfx RpAtomic object.
      *
      *  \returns pAtomic.
      */
      RpAtomic *G3x_ADL_DefaultLightingCB(RpAtomic *pAtomic)
      {
         RWS_FUNCTION("RWS::Pipelines::G3x_ADL_MatPipeID_DefaultLightingCB");
         
         static RpWorld *renderWorld = 0;
         static RwUInt16 renderFrame = 0; 
         
         RWS_PRE(pAtomic);
         
         RpWorld *world = RpAtomicGetWorld(pAtomic);
         
         RWS_PRE(world);
         
         if (world != renderWorld || renderFrame != RWSRCGLOBAL(renderFrame))
         {
            renderWorld = world;
            renderFrame = RWSRCGLOBAL(renderFrame);
            
            ADL_LightingSetupCB_data tADL_LightingSetupCB_data;
            
            tADL_LightingSetupCB_data.ambient = 0;
            tADL_LightingSetupCB_data.directional = 0;
            
            RpWorldForAllLights(world, ADL_LightingSetupCB, &tADL_LightingSetupCB_data);
            
            RWS_ASSERT(tADL_LightingSetupCB_data.ambient,
               "World must contain at least 1 ambient light, to use a G3x_ADL pipeline.");
            
            RWS_ASSERT(tADL_LightingSetupCB_data.directional,
               "World must contain at least 1 directional light, to use a G3x_ADL pipeline.");
            
            RpPDS_G3x_ADL_LightingUpload(tADL_LightingSetupCB_data.ambient, tADL_LightingSetupCB_data.directional);
         }
         
         RWS_RETURN(AtomicDefaultRenderCallBack(pAtomic));
      }
      
      
      /**
      *
      *  \ingroup Mod_Pipelines
      *
      *  RpLight call back function used to locate 1 directional and 1 ambient light, used by
      *  G3x_ADL_DefaultLightingCB.
      *
      *  \param pLight Pointer to RenderWare Gfx RpLight object.
      *
      *  \param pData Pointer to custom data, in this case a ADL_LightingSetupCB_data structure.
      *
      *  \return Pointer to pLight, or 0 when 1 ambient and 1 directional light has been found.
      */
      RpLight *APL_LightingSetupCB(RpLight *pLight, void *pData)
      {
         RWS_FUNCTION("RWS::Pipelines::NULL::APL_LightingSetupCB");
         
         APL_LightingSetupCB_data *pAPL_LightingSetupCB_data = reinterpret_cast<APL_LightingSetupCB_data*>(pData);
         
         switch (RpLightGetType(pLight))
         {
         case rpLIGHTPOINT:
            pAPL_LightingSetupCB_data->point = pLight;
            break;
         case rpLIGHTAMBIENT:
            pAPL_LightingSetupCB_data->ambient = pLight;
            break;
         default:
            break;
         }
         
         if (pAPL_LightingSetupCB_data->point && pAPL_LightingSetupCB_data->ambient)
         {
            RWS_RETURN(0); // Found 1 point + 1 ambient light early exit
         }
         
         RWS_RETURN(pLight);
      }
      
      /**
      *
      *  \ingroup Mod_Pipelines
      *
      *  RpAtomicRenderCallback used to setup 1 point and 1 ambient light when using a G3x_APL pipeline.
      *
      *  \param pAtomic Pointer to a RenderWare Gfx RpAtomic object.
      *
      *  \returns pAtomic.
      */
      RpAtomic *G3x_APL_DefaultLightingCB(RpAtomic *pAtomic)
      {
         RWS_FUNCTION("RWS::Pipelines::G3x_ADL_MatPipeID_DefaultLightingCB");
         
         static RpWorld *renderWorld = 0;
         static RwUInt16 renderFrame = 0; 
         
         RWS_PRE(pAtomic);
         
         RpWorld *world = RpAtomicGetWorld(pAtomic);
         
         RWS_PRE(world);
         
         if (world != renderWorld || renderFrame != RWSRCGLOBAL(renderFrame))
         {
            renderWorld = world;
            
            renderFrame = RWSRCGLOBAL(renderFrame);
            
            APL_LightingSetupCB_data tAPL_LightingSetupCB_data;
            
            tAPL_LightingSetupCB_data.ambient = 0;
            tAPL_LightingSetupCB_data.point = 0;
            
            RpWorldForAllLights(world, APL_LightingSetupCB, &tAPL_LightingSetupCB_data);
            
            RWS_ASSERT(tAPL_LightingSetupCB_data.ambient,
               "World must contain at least 1 ambient light, to use a G3x_APL pipeline.");

            RWS_ASSERT(tAPL_LightingSetupCB_data.point,
               "World must contain at least 1 point light, to use a G3x_APL pipeline.");
            
            RpPDS_G3x_APL_LightingUpload (tAPL_LightingSetupCB_data.ambient, tAPL_LightingSetupCB_data.point);
         }
         
         RWS_RETURN(AtomicDefaultRenderCallBack(pAtomic));
      }
      
      
      /**
      *
      *  \ingroup Mod_Pipelines
      *
      *  RpLight call back function used to locate 1 directional and 1 ambient light, used
      *  by G3x_ADL_DefaultLightingCB.
      *
      *  \param pLight Pointer to RenderWare Gfx RpLight object.
      *
      *  \param pData Pointer to custom data, in this case a ADL_LightingSetupCB_data structure.
      *
      *  \return Pointer to pLight, or 0 when 1 ambient and 1 directional light has been found.
      */
      RpLight *A4D_LightingSetupCB(RpLight *pLight, void *pData)
      {
         RWS_FUNCTION("RWS::Pipelines::NULL::A4D_LightingSetupCB");
         
         A4D_LightingSetupCB_data *pA4D_LightingSetupCB_data = reinterpret_cast<A4D_LightingSetupCB_data*>(pData);
         
         switch (RpLightGetType(pLight))
         {
         case rpLIGHTDIRECTIONAL:
            if (pA4D_LightingSetupCB_data->uiIndex < 4)
            {
               pA4D_LightingSetupCB_data->directional[pA4D_LightingSetupCB_data->uiIndex] = pLight;
               pA4D_LightingSetupCB_data->uiIndex++;
            }
            break;
         case rpLIGHTAMBIENT:
            pA4D_LightingSetupCB_data->ambient = pLight;
            break;
         default:
            break;
         }
         
         if (pA4D_LightingSetupCB_data->uiIndex == 4 && pA4D_LightingSetupCB_data->ambient)
         {
            RWS_RETURN(0); // Found 4 directional + 1 ambient light early exit
         }
         
         RWS_RETURN(pLight);
      }
      
      
      /**
      *
      *  \ingroup Mod_Pipelines
      *
      *  RpAtomicRenderCallback used to setup 4 directional and 1 ambient light when using a G3x_A4D pipeline.
      *
      *  \param pAtomic Pointer to a RenderWare Gfx RpAtomic object.
      *
      *  \returns pAtomic.
      */
      RpAtomic *G3x_A4D_DefaultLightingCB(RpAtomic *pAtomic)
      {
         RWS_FUNCTION("RWS::Pipelines::G3x_A4D_DefaultLightingCB");
         
         static RpWorld *renderWorld = 0;
         static RwUInt16 renderFrame = 0; 
         
         RWS_PRE(pAtomic);
         
         RpWorld *world = RpAtomicGetWorld(pAtomic);
         
         RWS_PRE(world);
         
         if (world != renderWorld || renderFrame != RWSRCGLOBAL(renderFrame))
         {
            renderWorld = world;
            renderFrame = RWSRCGLOBAL(renderFrame);
            
            A4D_LightingSetupCB_data tA4D_LightingSetupCB_data;
            
            tA4D_LightingSetupCB_data.ambient = 0;
            tA4D_LightingSetupCB_data.uiIndex = 0;
            
            RpWorldForAllLights(world, A4D_LightingSetupCB, &tA4D_LightingSetupCB_data);
            
            RWS_ASSERT(tA4D_LightingSetupCB_data.ambient,
               "World must contain at least 1 ambient light, to use a G3x_A4D pipeline.");

            RWS_ASSERT(tA4D_LightingSetupCB_data.uiIndex,
               "World must contain at least 4 directional lights, to use a G3x_A4D pipeline.");
            
            RpPDS_G3x_A4D_LightingUpload(tA4D_LightingSetupCB_data.ambient, tA4D_LightingSetupCB_data.directional);
         }
         
         RWS_RETURN(AtomicDefaultRenderCallBack(pAtomic));
      }
   }
}

#endif
