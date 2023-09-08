/****************************************************************************
 *
 * This file is a product of Criterion Software Ltd.
 *
 * This file is provided as is with no warranties of any kind and is
 * provided without any obligation on Criterion Software Ltd.
 * or Canon Inc. to assist in its use or modification.
 *
 * Criterion Software Ltd. and Canon Inc. will not, under any
 * circumstances, be liable for any lost revenue or other damages
 * arising from the use of this file.
 *
 * Copyright (c) 2002 Criterion Software Ltd.
 * All Rights Reserved.
 *
 */

/****************************************************************************
 *
 * CMaestro.cpp - Maestro behavior
 *
 ****************************************************************************/

#include "precomp.h"

// Left to its own devices, the MW linker will dead-strip the registrar ctors, so...
#ifdef __MWERKS__
#pragma force_active on
#pragma exceptions off // Turn off exceptions due to bug in MW Dwarf Generator for PS2, present in CW 3.04
#endif

// RenderWare Includes
#include "rwcore.h"
#include "rt2d.h"
#include "rt2danim.h"

//#define RWS_CALLSTACKENABLE
//#define RWS_TRACEENABLE

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//

#include "cmaestro.h"
#include "framework/core/macros/debugmacros.h"
#include "framework/mainloop/render.h"
#include "framework/toolkits/world/clevel.h"
#include "framework/toolkits/math/maths.h"

#ifdef _WINDOWS
#include "framework/startup/win32/win.h"
#endif

namespace
{
   const RwReal kFrameTime = 1.0f / RWS::MainLoop::Logic::Rate;
}

namespace RWS
{
   /**
   *
   */
   namespace Maestro
   {
      RWS_IMPLEMENT_CLASSID(CMaestro);
      RWS_REGISTERCLASS(CMaestro);

      /**
      *
      *
      *  \ref CMaestro
      *
      *  Create CMaestro object.  Gets behaviors atomic by extracting information the behavior needs from the 
      *  attributes used to construct the behavior.
      *
      *  \param attr  Reference to a CAttributePacket.
      *
      *  \see ~CMaestro
      */
      CMaestro::CMaestro(const CAttributePacket& attr) :
         InitCEventHandler(0), 
         m_Link_Priority(0)
      {
         RWS_FUNCTION("RWS::Maestro::CMaestro::CMaestro");

#ifdef _WINDOWS
         CEventHandler::RegisterMsg(Win::iMsgResizeCameraRasters, Win::iMsgResizeCameraRastersStr, 0);

         LinkMsg(Win::iMsgResizeCameraRasters);
#endif

         LinkMsg( iMsgRunningTick, 0 );

         RegisterMsg(m_Msg_Button[eBut_up], "INQ_MENU_BTN_UP", "return RwReal");
         RegisterMsg(m_Msg_Button[eBut_down], "INQ_MENU_BTN_DOWN", "return RwReal");
         RegisterMsg(m_Msg_Button[eBut_left], "INQ_MENU_BTN_LEFT", "return RwReal");
         RegisterMsg(m_Msg_Button[eBut_right], "INQ_MENU_BTN_RIGHT", "return RwReal");
         RegisterMsg(m_Msg_Button[eBut_select], "INQ_MENU_BTN_SELECT", "return RwReal");
         RegisterMsg(m_Msg_Button[eBut_cancel], "INQ_MENU_BTN_CANCEL", "return RwReal");

         RegisterMsg(m_StartRender, iMsgDoRenderStr, "RwCamera*");
         LinkMsg(m_StartRender, "RwCamera*", m_Link_Priority);

         RWS_RETURNVOID();
      }
   

      /**
      *
      *
      *  \ref ~CMaestro
      *
      *  Destroy CMaestro object.
      *
      *  \see CMaestro
      */
      CMaestro::~CMaestro(void)
      {
         RWS_FUNCTION("RWS::Maestro::CMaestro::~CMaestro");

#ifdef _WINDOWS
         UnLinkMsg(Win::iMsgResizeCameraRasters);

         CEventHandler::UnRegisterMsg(Win::iMsgResizeCameraRasters);
#endif

         UnLinkMsg( iMsgRunningTick );

         for (RwInt32 i = 0; i < eBut_total; i++)
         {
            UnRegisterMsg(m_Msg_Button[i]);
         }

         UnLinkMsg(m_TriggerCntrlButton);
         UnRegisterMsg(m_TriggerCntrlButton);

         UnLinkMsg(m_StartRender);
         UnRegisterMsg(m_StartRender);
      
         UnLinkMsg(m_EndRender);
         UnRegisterMsg(m_EndRender);

         RWS_RETURNVOID();
      }


      /**
      *
      *
      *  \ref InquireInput
      *
      *  Each tick the input event messages are sent to the Maestro controller method to be queried.
      *
      */
      void CMaestro::InquireInput(void)
      {
         RWS_FUNCTION("RWS::Maestro::CMaestro::InquireInput");

         for (RwInt32 i = 0; i < eBut_total; i++)
         {
            CMsg CMsg(m_Msg_Button[i]);

            SendMsg(CMsg);

            m_Maestro.Controller_Input(&CMsg, m_Msg_Button[i].GetName());
         }

         RWS_RETURNVOID();
      }


      /**
      *
      *
      *  \ref HandleEvents
      *
      *  Event Handler for CMaestro.
      *
      *  \param pMsg
      *
      */
      void CMaestro::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::Maestro::CMaestro::HandleEvents");

         if (iMsgRunningTick == pMsg.Id)
         {
            InquireInput();

            m_Maestro.Update(kFrameTime);
         }
         else if (m_StartRender == pMsg.Id)
         {
            m_Maestro.Render(&pMsg);

            // End Camera Event
            //
            CMsg Msg(m_EndRender, pMsg.pData);

            SendMsg(Msg);
         }
         else if (m_TriggerCntrlButton == pMsg.Id)
         {
            m_Maestro.Controller_Action(m_TriggerCntrlButton.GetName());

            UnLinkMsg(m_TriggerCntrlButton);
            UnRegisterMsg(m_TriggerCntrlButton);
         }
#ifdef _WINDOWS
         else if (Win::iMsgResizeCameraRasters == pMsg.Id)
         {
            // A resize has been done so we need to reset the 2d camera
            Rt2dDeviceSetCamera(m_Maestro.GetCamera());
            // We also need to reset the maestro scales to the new size of
            // the camera's raster.
            m_Maestro.InitState();
         }
#endif

         RWS_RETURNVOID();
      }
  

      /**
      *
      *
      *  \ref HandleAttributes
      *
      *  Attribute Handler for CMaestro.
      *
      *  \param attr  Reference to a CAttributePacket.
      *
      */
      void CMaestro::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::Maestro::CMaestro:HandleAttributes");

         CAttributeHandler::HandleAttributes(attr);

         // Now give Maestro a look
         m_Maestro.HandleSystemCommands(attr);

         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(CMaestro));

         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
               case CMD_Start_Render:
                  //////////////////////////////////////////////////////////////////

                  // If already linked, unlink and unregister as we're about to register with
                  // a new event.
                  //
                  UnLinkMsg(m_StartRender);
                  UnRegisterMsg(m_StartRender);

                  //
                  //
                  RegisterMsg(m_StartRender, attrIt->GetAs_RwChar_ptr(), "RwCamera*");
                  LinkMsg(m_StartRender, "RwCamera*", m_Link_Priority);
                  break;
                  //////////////////////////////////////////////////////////////////
               case CMD_End_Render:
                  //////////////////////////////////////////////////////////////////
                  ReplaceRegisteredMsg(m_EndRender, attrIt->GetAs_RwChar_ptr(), "RwCamera*");
                  break;

               case CMD_Priority:
                  //////////////////////////////////////////////////////////////////
                  m_Link_Priority = attrIt->GetAs_RwUInt32();

                  // If already linked, unlink as we're about to re-link at a different priority
                  //
                  UnLinkMsg(m_StartRender);
                  LinkMsg(m_StartRender, "RwCamera*", m_Link_Priority);
                  break;

               case CMD_ShowHide:
                  // Show/ hide animation
                  if (attrIt->GetAs_RwUInt32())
                     Rt2dObjectSetVisible(m_Maestro.GetScene(), TRUE);
                  else
                     Rt2dObjectSetVisible(m_Maestro.GetScene(), FALSE);
                  break;

               case CMD_PlayMessage:
                  // Play/ stop current animation
                  if (attrIt->GetAs_RwUInt32())
                     m_Maestro.Play();
                  else
                     m_Maestro.Stop();
                  break;

               case CMD_Interp:
                  // Turn on/ off interpolation
                  if (attrIt->GetAs_RwUInt32())
                     m_Maestro.Interpolate(TRUE);
                  else
                     m_Maestro.Interpolate(FALSE);
                  break;
               
               case CMD_SetPosX:
                  // Position slider to adjust the x-coordinate
                  m_Maestro.SetPositionX(attrIt->GetAs_RwReal());
                  break;

               case CMD_SetPosY:
                  // Position slider to adjust the y-coordinate
                  m_Maestro.SetPositionY(attrIt->GetAs_RwReal());
                  break;

               case CMD_SetPosZ:
                  // Position slider to adjust the z-coordinate
                  m_Maestro.SetPositionZ(attrIt->GetAs_RwReal());
                  break;

               case CMD_TriggerCntrlButton:
                  // Test to activate up button
                  ReplaceLinkedMsg(m_TriggerCntrlButton, attrIt->GetAs_RwChar_ptr(), "return RwReal");
                  break;
            }
            ++attrIt;
         }

         RWS_RETURNVOID();
      }
   } // namespace Maestro
} // namespace RWS
