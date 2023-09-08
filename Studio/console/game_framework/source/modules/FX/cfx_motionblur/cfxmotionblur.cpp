/*****************************************************************************
 *
 * File :     CFXMotionBlur.cpp
 *
 * Abstract : A behavior that can be used to blend the previous frame with the current
 *            frame to produce various motion blur effects.
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
#include "rwcore.h"

// Left to its own devices, the MW linker will dead-strip the registrar ctors, so...
#ifdef __MWERKS__
#pragma force_active on
#pragma exceptions off // Turn off exceptions due to bug in MW Dwarf Generator for PS2, present in CW 3.04
#endif


//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//

#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/world/clevel.h"
#include "framework/toolkits/math/maths.h"
#include "framework/mainloop/render.h"
#include "framework/core/resourcemanager/cresourcemanager.h"
#include "cfxmotionblur.h"


namespace RWS
{
                                                  
   namespace FX 
   {
                            
      RWS_IMPLEMENT_CLASSID(CFXMotionBlur);
      RWS_REGISTERCLASS(CFXMotionBlur);

      /**
      *
      *  Construct a CFXMotionBlur object.
      *
      */
      CFXMotionBlur::CFXMotionBlur(const CAttributePacket& rAttr) : InitCEventHandler(0)
      {
         RWS_FUNCTION("RWS::FX::CFXMotionBlur::CFXMotionBlur");
                    

         RWS_RETURNVOID();
      }

      /**
      *
      *  Destroy a CFXMotionBlur object.
      *
      */
      CFXMotionBlur::~CFXMotionBlur()
      {
         RWS_FUNCTION("RWS::FX::CFXMotionBlur::~CFXMotionBlur");

         UnLinkMsg(m_StartRender);
         UnRegisterMsg(m_StartRender);
         RWS_RETURNVOID();
      }


      /**
      *
      * \ref HandleEvents(). Handle events.  
      *
      * \param pMsg Standard Message Package
      *  
      * \ref iMsgRunningTick
      */
      void CFXMotionBlur::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::FX::CFXMotionBlur::HandleEvents");

         if (pMsg.Id == m_StartRender)
         {
            m_Target.BlendFrontBuffer(reinterpret_cast<RwCamera*>(pMsg.pData));
         }

         RWS_RETURNVOID();
      }

      /**
      *
      *  Update attributes.
      *
      */
      void CFXMotionBlur::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::FX::CFXMotionBlur::HandleAttributes");
         
         CAttributeHandler::HandleAttributes(attr);
         
         // deal with the attributes related to the environment map behaviour
         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(CFXMotionBlur));
         
         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
               case CMD_Start_Render:

                  // If already linked, unlink and unregister as we are about to register with
                  // a new event.
                  UnLinkMsg(m_StartRender);
                  UnRegisterMsg(m_StartRender);
                  // register the new event
                  RegisterMsg(m_StartRender, attrIt->GetAs_RwChar_ptr(), "RwCamera*");
                  LinkMsg(m_StartRender, "RwCamera*");
                  break;
               case CMD_Priority:
                  //////////////////////////////////////////////////////////////////
                  m_link_priority = attrIt->GetAs_RwUInt32();

                  // If already linked, unlink as were about to relink at a different priority
                  //
                  UnLinkMsg(m_StartRender);
                  LinkMsg(m_StartRender, "RwCamera*", m_link_priority);
                  break;

               case CMD_Set_m_BlurAmount:
                  attrIt->GetCommandData(m_Target.m_BlurAmount);
                  RWS_TRACE("CMD_Set_m_BlurAmount "<<m_Target.m_BlurAmount);
                  break;

               case CMD_Set_m_OffsetX:
                  attrIt->GetCommandData(m_Target.m_OffsetX);
                  RWS_TRACE("CMD_Set_m_OffsetX "<<m_Target.m_OffsetX);
                  break;
               case CMD_Set_m_OffsetY:
                  attrIt->GetCommandData(m_Target.m_OffsetY);
                  RWS_TRACE("CMD_Set_m_OffsetY "<<m_Target.m_OffsetY);
                  break;
               case CMD_Set_m_ScaleX:
                  attrIt->GetCommandData(m_Target.m_ScaleX);
                  RWS_TRACE("CMD_Set_m_ScaleX "<<m_Target.m_ScaleX);
                  break;
               case CMD_Set_m_ScaleY:
                  attrIt->GetCommandData(m_Target.m_ScaleY);
                  RWS_TRACE("CMD_Set_m_ScaleY "<<m_Target.m_ScaleY);
                  break;
               case CMD_Set_m_Scale:
                  attrIt->GetCommandData(m_Target.m_Scale);
                  RWS_TRACE("CMD_Set_m_Scale "<<m_Target.m_Scale);
                  break;

               case CMD_Set_m_Angle:
                  attrIt->GetCommandData(m_Target.m_Angle);
                  RWS_TRACE("CMD_Set_m_Angle "<<m_Target.m_Angle);
                  break;

            }
            ++attrIt;
         }

         // as the attributes has changed and the blend poly needs updating we set the flag telling
         // the platform specific motion blur code to construct a new polygon with the new attributes.
         m_Target.m_UpdateBlendPoly = TRUE;
         
         RWS_RETURNVOID();
      }

   } // namespace FX
} //namespace RWS
