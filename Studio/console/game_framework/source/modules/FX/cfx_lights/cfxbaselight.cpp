/*****************************************************************************
 *
 * File :     CFXBaseLight.cpp
 *
 * Abstract : An entity wrapper for the RpLight type
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
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "cfxbaselight.h"

#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/world/clevel.h"
#include "framework/toolkits/math/maths.h"
#include "framework/toolkits/world/factory.h"

namespace RWS
{
   namespace FX
   {
      RWS_IMPLEMENT_CLASSID(CFXBaseLight);
      
      /**
      *
      *  Construct a CFXBaseLight object.
      *
      */
      CFXBaseLight::CFXBaseLight(const CAttributePacket& rAttr) :
      m_type(rpNALIGHTTYPE)
      {
         RWS_FUNCTION("RWS::FX::CFXBaseLight::CFXBaseLight");
         
         m_pAtomic = CreateAtomicInWorldFromResource(rAttr, 0);
         
         RWS_ASSERT(m_pAtomic, "Failed to create atomic");
         
         // Create Light
         Create();
         
         RWS_RETURNVOID();
      }
     
      /**
      *
      *  Destroy a CFXBaseLight object.
      *
      */
      CFXBaseLight::~CFXBaseLight() 
      {
         // Destroy Light
         RWS_FUNCTION("RWS::FX::CFXBaseLight::~CFXBaseLight");
         Destroy();
         RWS_RETURNVOID();
      }
      
      /**
      *
      *	CFXBaseLight is a wrapper for a RpLight, this function creates an RpLight with a frame,
      *	and attaches it to the main world.
      *
      */
      void CFXBaseLight::Create()
      {
         RWS_FUNCTION("RWS::FX::CFXBaseLight::Create");
         
         // shouldn't be creating a RpLight if one already exists.
         //  (although the smart pointer will deal with destroying the existing RpLight first).
         RWS_PRE(!m_pLight);
         
         // Create light
         m_pLight = RpLightCreate(m_type);
         
         if (m_pLight)
         {
            //  Create a frame for the light
            RwFrame *frame = RwFrameCreate();
            
            if (frame)
            {
               // Set Identity matrix and frame then add to the world
               RwFrameSetIdentity(frame);
               RpLightSetFrame(m_pLight.ptr(), frame);
               
               RwRGBAReal t_color = {1, 1, 1, 1};
               
               // Set Light parameters cone, radius and color         
               RpLightSetConeAngle(m_pLight, RWDEG2RAD(45.0f));
               RpLightSetRadius(m_pLight, (1000.0f));
               RpLightSetColor(m_pLight, &t_color);
               RpLightSetFlags(m_pLight.ptr(), rpLIGHTLIGHTATOMICS | rpLIGHTLIGHTWORLD);
               
               // add the light to the world
               if ( m_type != rpNALIGHTTYPE )
               {
                  CLevel::AddLight(m_pLight);
               }
            }
         }
         
         RWS_RETURNVOID();
      }
      
      /**
      *
      *	CFXBaseLight is a wrapper for a RpLight, this function detaches the light from its world, 
      *	then it deletes the current RpLight and its associated RpFrame.
      *
      */
      void CFXBaseLight::Destroy()
      {
         RWS_FUNCTION("RWS::FX::CFXBaseLight::Destroy");
         
         m_pLight = 0;
         
         RWS_RETURNVOID();
      }
      
      /**
      *
      *  Set the type of the light.
      *
      *  \param type see RenderWare help for RpLightCreate().
      *
      */
      void CFXBaseLight::SetType( RwUInt32 type )
      {
         RWS_FUNCTION("RWS::FX::CFXBaseLight::SetType");
         
         m_type = type;
         
         if (type != rpNALIGHTTYPE)
         {
            RpLight *a_light = RpLightCreate(m_type);
            
            if (a_light)
            {
               // Get world that light is attached too
               RpWorld *world = RpLightGetWorld(m_pLight);
               
               // Get lights frame
               RwFrame *t_frame = RpLightGetFrame (m_pLight.ptr());
               
               // If world is valid remove light from world
               if (world)
               {
                  RpWorldRemoveLight(world, m_pLight);
               }
               
               // clear frame
               (void)RpLightSetFrame (m_pLight, 0);
               
               // set frame from world light and apply it to newly created light
               RpLightSetFrame (a_light, t_frame);
               
               // apply radius of world light to newly created light
               RpLightSetRadius(a_light, RpLightGetRadius(m_pLight));
               
               // apply color of world light to newly created light
               RpLightSetColor(a_light, RpLightGetColor(m_pLight));
               
               // apply cone angle of world light to newly created light
               RpLightSetConeAngle(a_light, RpLightGetConeAngle(m_pLight));
               
               // apply flags of world light to newly created light
               RpLightSetFlags(a_light, RpLightGetFlags(m_pLight.ptr()));
               
               // if valid world, add new light to world
               if (world)
               {
                  RpWorldAddLight(world, a_light);
               }
               
               // destroy old light
               RpLightDestroy (m_pLight);
               
               // copy new light to old light
               m_pLight = a_light;
            }            
         }
         else
         {
            RemoveLightFromWorld();
         }
         
         RWS_RETURNVOID();
      }
      
      /**
      *
      *  Add the light to the current RpWorld object. 
      *   Note: Use this instead of RpWorldAddLight as it also deals with the rpNALIGHTTYPE type
      *
      */
      void CFXBaseLight::AddLightToWorld()
      {
         RWS_FUNCTION("RWS::FX::CFXBaseLight::AddLightToWorld");
         
         // Never add lights of rpNALIGHTTYPE to the world
         if ( m_type != rpNALIGHTTYPE )
         {
            RpWorld * world = RpLightGetWorld( m_pLight );
            
            if (!world)
            {
               CLevel::AddLight(m_pLight);
            }
         }
         
         RWS_RETURNVOID();
      }
      
      /**
      *
      *  Remove the light from the current world.
      *   Note: use this instead of RpWorldRemoveLight as it also deals with the rpNALIGHTTYPE type
      *
      */
      void CFXBaseLight::RemoveLightFromWorld()
      {
         RWS_FUNCTION("RWS::FX::CFXBaseLight::RemoveLightFromWorld");
         
         RpWorld * world = RpLightGetWorld( m_pLight );
         
         if ( world )
         {
            RpWorldRemoveLight( world, m_pLight );
         }
         
         RWS_RETURNVOID();
      }
      
      /**
      *
      *	Update attributes.
      *
      */
      void CFXBaseLight::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::FX::CFXBaseLight::HandleAttributes");
         
         CAttributeHandler::HandleAttributes(attr);
         
         m_pAtomic.HandleSystemCommands(attr);
         m_pLight.HandleSystemCommands(attr);
         
         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(CFXBaseLight));
         
         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
            case CMD_Set_t_Light_Type:
               {
                  RwUInt32 type;
                  
                  attrIt->GetCommandData(type);
                  
                  // LIST sends the element to select, need to convert this to the 
                  // define for the light type as they are not 0, 1, 2, 3, 4
                  switch (type)
                  {
                  case 0: 
                     type = rpNALIGHTTYPE; 
                     break;
                  case 1: 
                     type = rpLIGHTDIRECTIONAL; 
                     break;
                  case 2: 
                     type = rpLIGHTAMBIENT; 
                     break;
                  case 3: 
                     type = rpLIGHTPOINT; 
                     break;
                  case 4: 
                     type = rpLIGHTSPOT; 
                     break;
                  case 5: 
                     type = rpLIGHTSPOTSOFT; 
                     break;
                  default:
                     RWS_ASSERTFAIL("CFXBaseLight::HandleAttributes illegal parameter value.");
                     break;
                  }
                  
                  SetType(type);
               }
               break;
               
            case CMD_Set_t_LightConeAngle:
               // Set cone angle
               RpLightSetConeAngle(m_pLight, RWDEG2RAD(attrIt->GetAs_RwReal()));
               break;
               
            case CMD_Set_t_radius:
               // Set radius
               RpLightSetRadius(m_pLight, attrIt->GetAs_RwReal());
               break;
               
            case CMD_Set_t_flag:
               {
                  const RwUInt32 LightFlags[] = 
                  {
                     rpLIGHTLIGHTATOMICS | rpLIGHTLIGHTWORLD,
                        rpLIGHTLIGHTATOMICS,
                        rpLIGHTLIGHTWORLD
                  };
                  
                  // Set light flags
                  RpLightSetFlags(m_pLight.ptr(), LightFlags[attrIt->GetAs_RwUInt32()]);
               }
               break;
            }
            ++attrIt;
         }
         
         RWS_RETURNVOID();
      }
   } // namespace FX
} //namespace RWS
