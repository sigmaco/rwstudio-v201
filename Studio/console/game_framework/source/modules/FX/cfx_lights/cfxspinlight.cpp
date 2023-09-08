/*****************************************************************************
*
* File :     CFXSpinLight.cpp
*
* Abstract : Implementation of CFXSpinLight class. This type of light
*            can be triggered to turn on and off by triggering an event.  This
*            can be useful if you need to trigger a warning light within a 
*            game for example.
*            
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
#include "cfxspinlight.h"
#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/math/maths.h"
#include "framework/toolkits/world/clevel.h"


namespace RWS
{
   
   namespace FX
   {
      /**
      *
      * Using this behavior you can make beacon type effects,  like warning light etc. 
      * 
      */
      RWS_IMPLEMENT_CLASSID(CFXSpinLight);
      RWS_REGISTERCLASS(CFXSpinLight);
      
      /**
      *
      *  Construct a CFXSpinLight object.
      *
      */
      CFXSpinLight::CFXSpinLight(const CAttributePacket& rAttr): CFXBaseLight(rAttr),
         InitCEventHandler(&m_pLight)
      {
         RWS_FUNCTION("RWS::FX::CFXSpinLight::CFXSpinLight");
         
         m_mat = RwMatrixCreate();
         RWS_POST(m_mat);
         
         // Initialize rotation values
         m_rot[0] = 0.0f;
         m_rot[1] = 0.0f; 
         m_rot[2] = 0.0f;
         
         // Make sure light is not attached to the world. this is because the
         // light is triggered by an event.  If you want the light to appear
         // straight away,  use the iMsgStartSystem event.
         RemoveLightFromWorld();
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      *  Destroy a CFXSpinLight object.
      *
      */
      CFXSpinLight::~CFXSpinLight() 
      {
         RWS_FUNCTION("RWS::FX::CFXSpinLight::~CFXSpinLight");
         
         RWS_PRE(m_mat);
         RwMatrixDestroy(m_mat);
         m_mat = 0;
         UnLinkMsg(iMsgRunningTick);
         UnLinkMsg(m_trigger);
         UnRegisterMsg(m_trigger);
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      *  Set lights rotation matrix
      *
      */
      void CFXSpinLight::RotateLight( void )
      {
         RWS_FUNCTION("RWS::FX::CFXSpinLight::RotateLight");
         
         // Set lights rotation matrix
         RwMatrixRotate(m_mat, &XAxis, m_rot[0],rwCOMBINEREPLACE);
         RwMatrixRotate(m_mat, &YAxis, m_rot[1],rwCOMBINEPRECONCAT);
         RwMatrixRotate(m_mat, &ZAxis, m_rot[2],rwCOMBINEPRECONCAT);
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      *  Handle events.  Rotate light based on values within m_rot. This happens on the system running tick.
      *  
      */
      void CFXSpinLight::HandleEvents(CMsg &pMsg)
      {      
         RWS_FUNCTION("RWS::FX::CFXSpinLight::HandleEvents");
         
         // If linked to the running tick
         if (pMsg.Id == iMsgRunningTick)
         {
            // Trans form light
            RwFrameTransform(RpLightGetFrame(m_pLight.ptr()), m_mat, rwCOMBINEPRECONCAT);
         }
         // If received and event
         else if (pMsg.Id == m_trigger)
         {
            // Toggle Running tick
            if (IsLinked(iMsgRunningTick) == false)
            {
               // Add light to the world
               AddLightToWorld();
               LinkMsg(iMsgRunningTick);
            }
            else
            {
               // If running tick disabled remove light from the world
               RemoveLightFromWorld();
               UnLinkMsg(iMsgRunningTick);
            }
         }
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      *  Handle attributes.
      *
      */
      void CFXSpinLight::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::FX::CFXSpinLight::HandleAttributes");
         
         CFXBaseLight::HandleAttributes(attr);
         
         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(CFXSpinLight));
         
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
               
            case CMD_rot_x:
               // Get X rotation value
               attrIt->GetCommandData(m_rot[0]);
               m_rot[0] = RWDEG2RAD(m_rot[0]);
               RotateLight();
               break;
               
            case CMD_rot_y:
               // Get Y rotation value
               attrIt->GetCommandData(m_rot[1]);
               m_rot[1] = RWDEG2RAD(m_rot[1]);
               RotateLight();
               break;
               
            case CMD_rot_z:
               // Get Z rotation value
               attrIt->GetCommandData(m_rot[2]);
               m_rot[2] = RWDEG2RAD(m_rot[2]);
               RotateLight();
               break;
               
            case CMD_m_trigger:
               // Re-register new trigger event
               ReplaceLinkedMsg ( m_trigger,  attrIt->GetAs_RwChar_ptr(),0);
               break;
            }
            
            
            ++attrIt;
         }
         
         RWS_RETURNVOID();
      }
      
   } // namespace FX
   
} // namespace RWS
