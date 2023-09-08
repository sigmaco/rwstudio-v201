/*****************************************************************************
*
* File :     SkyRegisterPipes.cpp
*
* Abstract : Register G3 Pipelines
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
// Renderware Includes
//
#include "rt2danim.h"
#include "rppds.h"

//////////////////////////////////////////////////////////////////
//
// Renderware Studio Includes
//
#include "skyregisterpipes.h"
#include "../../core/macros/debugmacros.h"

namespace RWS
{
   namespace Pipelines
   {
      /**
      *
      *  \ingroup StartUp
      *
      *  Register pipelines that need to be registered before plugins are attached.
      *
      */
      void PrePluginAttachRegisterPipeLines(void)
      {
         RWS_FUNCTION("PrePluginAttachRegisterPipeLines");

         RpWorldPipesAttach();

         RWS_RETURNVOID();
      }
      
      /**
      *
      *  \ingroup StartUp
      *
      *  Register pipelines that need to be registered after plugins are attached.
      *
      */
      void PostPluginAttachRegisterPipeLines(void)
      {
         RWS_FUNCTION("PostPluginAttachRegisterPipeLines");
         
         RpPatchPipesAttach();
         RpMatfxPipesAttach();
         Rt2DPipesAttach();
         RpLtMapPipesAttach();
         RpSkinfxPipesAttach();
         
         // These pipelines are used by the rf3 example where the data is being pre-instanced
         // 
         rwPDS_G3_Generic_MatPipeRegister(); 
         rwPDS_G3_Generic_SctPipeRegister();
         rwPDS_G3_Skin_MatPipeRegister();
         rwPDS_G3_SkinGem_MatPipeRegister();
         rwPDS_G3_Skin_AtmPipeRegister();
         rwPDS_G3_Generic_AtmPipeRegister();

         // These pipelines are used by the custom pipelines example
         //
         rwPDS_G3x_APL_MatPipeRegister();
         rwPDS_G3x_ADL_MatPipeRegister();
         rwPDS_G3x_A4D_MatPipeRegister();
         rwPDS_G3x_APLDup_MatPipeRegister();
         rwPDS_G3x_ADLDup_MatPipeRegister();
         rwPDS_G3x_A4DDup_MatPipeRegister();
         rwPDS_G3x_APLGem_MatPipeRegister();
         rwPDS_G3x_ADLGem_MatPipeRegister();
         rwPDS_G3x_A4DGem_MatPipeRegister();
         rwPDS_G3x_APLSkin_MatPipeRegister();
         rwPDS_G3x_ADLSkin_MatPipeRegister();
         rwPDS_G3x_A4DSkin_MatPipeRegister();
         rwPDS_G3x_APLSkinDup_MatPipeRegister();
         rwPDS_G3x_ADLSkinDup_MatPipeRegister();
         rwPDS_G3x_A4DSkinDup_MatPipeRegister();
         rwPDS_G3x_APLSkinGem_MatPipeRegister();
         rwPDS_G3x_ADLSkinGem_MatPipeRegister();
         rwPDS_G3x_A4DSkinGem_MatPipeRegister();

         rwPDS_G3x_Generic_AtmPipeRegister();
         rwPDS_G3x_Skin_AtmPipeRegister();

         RWS_RETURNVOID();
      }
   }
}