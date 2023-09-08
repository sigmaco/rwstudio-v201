/*****************************************************************************
*
* File : 	 CFXColorLight.cpp
*
* Abstract : Implementation of CFXColorLight class
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
// Left to its own devices, the MW linker will dead-strip the registrar ctors, so...
#ifdef __MWERKS__
#pragma force_active on
#pragma exceptions off // Turn off exceptions due to bug in MW Dwarf Generator for PS2, present in CW 3.04
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "cfxcolorlight.h"
#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/math/maths.h"

namespace RWS
{

   namespace FX
   {
      RWS_IMPLEMENT_CLASSID(CFXColorLight);
      RWS_REGISTERCLASS(CFXColorLight);
      
      /**
      *
      *	Handle attributes.
      *
      */
      void CFXColorLight::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::FX::CFXColorLight::HandleAttributes");
         
         CFXBaseLight::HandleAttributes(attr);
         
         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(CFXColorLight));
         
         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
            case CMD_Set_t_color:
               {
                  RwRGBA Color = attrIt->GetAs_RwRGBA();
                  RwRGBAReal t_color;
                  RwRGBARealFromRwRGBA  (&t_color, &Color);  

                  // Set light
                  RpLightSetColor(m_pLight, &t_color);                                  
               }
               break;
            }
            ++attrIt;
         }
         
         // Need to make sure the light is attached to the world, because CFXBaseLight doesn't handle that
         AddLightToWorld();
         
         RWS_RETURNVOID();
      }

   } // namespace FX

} // namespace RWS
