/*****************************************************************************
 *
 * File :     CFXXBoxPixelShader.h
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
#if (defined (_XBOX))

#ifndef __CFXXBOXPIXELSHADER_H__
#define __CFXXBOXPIXELSHADER_H__

//////////////////////////////////////////////////////////////////
//
// RenderWare Includes
//
#include "rwcore.h"
#include "rpworld.h"


#ifdef _MSC_VER
#pragma comment (lib, "rwcore.lib")
#pragma comment (lib, "rpworld.lib")
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/core/eventhandler/ceventhandler.h"
#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/core/attributehandler/csystemcommands.h"
#include "framework/toolkits/world/smartptrs/catomicptr.h"

#include "framework/core/attributehandler/allocation policies/LinearAllocationPolicy.h"

namespace RWS
{
   namespace FX
   {
      /**
      *
      *  \ingroup Mod_FX
      *
      *  CFXXBoxPixelShader can be used as a base class to form behaviors that render atomics with a pixel shader.
      *  Once your behavior has created the pixel shaders it requires, making a call to either 
      *  SetAtomicMaterialsPixelShader or SetClumpMaterialsPixelShader will apply the pixel shader to all the materials
      *  used by the clump or atomic that was attach to the behaviour.
      */
      class CFXXBoxPixelShader:  public LinearAllocationPolicy
      {

      public:
         static RpAtomic     *AtomicCallBack       (RpAtomic *pAtomic, void *pData);
         static RpMaterial   *MaterialCallBack     (RpMaterial *pMaterial, void *pData);

         RwBool               AtomicSetPixelShader (const RpAtomic *pAtomic, RpXboxPixelShader  *pPixelShader);
         RwBool               ClumpSetPixelShader  (const RpClump  *pClump, RpXboxPixelShader  *pPixelShader);

      protected:
         CFXXBoxPixelShader(){};
         virtual ~CFXXBoxPixelShader(){};
      };

   } // namespace FX
      
} // namespace RWS

#endif
#endif