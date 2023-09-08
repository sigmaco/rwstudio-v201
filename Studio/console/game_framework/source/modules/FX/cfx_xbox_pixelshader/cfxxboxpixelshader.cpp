/*****************************************************************************
 *
 * File :     CFXXBoxPixelShader.cpp
 *
 * Abstract : A class that can be used to apply pixel shaders to clumps and atomics
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
#include "rwcore.h"

// Left to its own devices, the MW linker will dead-strip the registrar ctors, so...
#ifdef __MWERKS__
#pragma force_active on
#endif


//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "cfxxboxpixelshader.h"
#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/world/clevel.h"
#include "framework/toolkits/math/maths.h"
#include "framework/mainloop/render.h"


namespace RWS
{

   namespace FX
   {
      // structure used to pass data into the material callbacks
      typedef struct
      {
         RpXboxPixelShader *m_PixelShader;
         
      }t_MATERIAL_PS_CALLBACK_DATA;

      /**
      *
      *  Applies the pixel shader stored in pData to the materials used by pAtomic.
      * \param RpAtomic                      Pointer to an RpAtomic object.
      * \param t_MATERIAL_PS_CALLBACK_DATA   Pointer to a call back structure containing the pixel shader
      * \return                              RpAtomic. 
      *
      */
      RpAtomic * CFXXBoxPixelShader::AtomicCallBack(RpAtomic *pAtomic, void *pData)
      {
         RWS_FUNCTION("RWS::FX::CFXXBoxPixelShader::AtomicCallBack");

         RpGeometry  *Geom;

         Geom = RpAtomicGetGeometry(pAtomic);
         RWS_ASSERT(Geom, "Unable to get a pointer to the atomics geometry");

         RpGeometryForAllMaterials(Geom, MaterialCallBack, pData);

         RWS_RETURN(pAtomic);
      }

      /**
      *
      *  Applies the pixel shader stored in pData to pMaterial
      * \param RpMaterial                    Pointer to an RpMaterial object.
      * \param t_MATERIAL_PS_CALLBACK_DATA   Pointer to a call back structure containing the pixel shader
      * \return                              pMaterial. 
      *
      */
      RpMaterial * CFXXBoxPixelShader::MaterialCallBack(RpMaterial *pMaterial, void *pData)
      {
         RWS_FUNCTION("RWS::FX::CFXXBoxPixelShader::MaterialCallBack");

         t_MATERIAL_PS_CALLBACK_DATA *PSInfo;

         PSInfo = static_cast<t_MATERIAL_PS_CALLBACK_DATA*>(pData); 
        
         RpXboxMaterialSetPixelShader(pMaterial, PSInfo->m_PixelShader);
         RWS_RETURN(pMaterial);
      }

      /**
      *
      *  Set any materials used by the pAtomic to use the pixel shader pPixelShader  
      * \param RpAtomic             Pointer to an RpAtomic object.
      * \param RpXboxPixelShader    Pointer to an RenderWare pixel shader
      * \return                     TRUE. 
      *
      */
      RwBool CFXXBoxPixelShader::AtomicSetPixelShader(const RpAtomic *pAtomic, RpXboxPixelShader  *pPixelShader)
      {
         RWS_FUNCTION("RWS::FX::CFXXBoxPixelShader::SetAtomicMaterialsPixelShader");
         t_MATERIAL_PS_CALLBACK_DATA CallBackData;

         CallBackData.m_PixelShader = pPixelShader;

         AtomicCallBack(const_cast<RpAtomic *>(pAtomic), &CallBackData);
         RWS_RETURN(TRUE);
      }


      /**
      *
      *  Set any materials used by the pClump to use the pixel shader pPixelShader  
      * \param RpClump              Pointer to an RpClump object.
      * \param RpXboxPixelShader    Pointer to an RenderWare pixel shader
      * \return                     TRUE. 
      *
      */
      RwBool CFXXBoxPixelShader::ClumpSetPixelShader (const RpClump *pClump, RpXboxPixelShader  *pPixelShader)
      {
         RWS_FUNCTION("RWS::FX::CFXXBoxPixelShader::SetClumpMaterialsPixelShader");
         t_MATERIAL_PS_CALLBACK_DATA CallBackData;

         CallBackData.m_PixelShader = pPixelShader;

         RpClumpForAllAtomics(const_cast<RpClump *>(pClump), AtomicCallBack, &CallBackData);
         RWS_RETURN(TRUE);
      }
   } // namespace FX
} //namespace RWS
