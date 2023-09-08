/*****************************************************************************
 *
 * File :     CFXBaseLight.h
 *
 * Abstract : 
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

#ifndef __CFXBASELIGHT_H__
#define __CFXBASELIGHT_H__

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
#include "framework/toolkits/world/smartptrs/clightptr.h"

#include "framework/core/attributehandler/allocation policies/linearallocationpolicy.h"

namespace RWS
{

   namespace FX
   {
      /**
      *
      *  \ingroup Mod_FX
      *
      *  C++ wrapper for RpLight, see RenderWare Help RpLight.
      *
      */

      class CFXBaseLight:  public CSystemCommands, public CAttributeHandler, public LinearAllocationPolicy
      {
      public:
         RWS_DECLARE_CLASSID(CFXBaseLight);
         RWS_CATEGORY("FX");
         RWS_DESCRIPTION("Base Light", "");

         RWS_BEGIN_COMMANDS
            RWS_ATTRIBUTE(CMD_Set_t_Light_Type,     
                          "Type",        
                          "Set light type, see RpLightCreate",   
                          RADIO_BUTTON,    
                          RwUInt32,   
                          LIST("No Light|Directional|Ambient|Point|Spot|Soft Spot"))

            RWS_ATTRIBUTE(CMD_Set_t_LightConeAngle, 
                          "Cone angle",  
                          "Set light cone angle, see RpLightSetConeAngle",   
                          SLIDER,  
                          RwReal,     
                          RANGE(0, 45, 180))

            RWS_ATTRIBUTE(CMD_Set_t_radius,         
                          "Radius",      
                          "Set light radius, see RpLightSetRadius",   
                          SLIDER,  
                          RwReal,     
                          RANGE(0, 1000, 5000))

            RWS_ATTRIBUTE(CMD_Set_t_flag,           
                          "Flag",        
                          "Set light flags, see RpLightSetFlags",   
                          LIST,    
                          RwUInt32,   
                          LIST("Atomics+World|Atomics|World"))
         RWS_END_COMMANDS;

         virtual void HandleAttributes(const CAttributePacket& attr);

         CFXBaseLight(const CAttributePacket& rAttr);

         virtual ~CFXBaseLight();

      protected:

         void Create();
         void Destroy();

         void SetType( RwUInt32 type );

         void AddLightToWorld();
         void RemoveLightFromWorld();

         CAtomicPtr m_pAtomic;     /**< Behavior's Atomic */
         CLightPtr m_pLight;       /**< Pointer to a RpLight */
         RwUInt32 m_type;          /**< Type of the light (needed to be able to keep track of rpNALIGHTTYPE) */
      };

   } // namespace FX
   
} // namespace RWS

#endif
