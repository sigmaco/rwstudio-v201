/*****************************************************************************
*
* File :     EventVisualization.cpp
*
* Abstract :
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
* Copyright (c) 2003 Criterion Software Ltd.
* All Rights Reserved.
*
* RenderWare is a trademark of Canon Inc.
*
*****************************************************************************/
#include "precomp.h"


#ifdef RWS_EVENTVISUALIZATION

//////////////////////////////////////////////////////////////////
//
// RenderWare Includes
//
#include "rprandom.h"
#ifdef _MSC_VER
#pragma comment (lib, "rprandom.lib")
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "eventvisualization.h"
#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/debugtools/debugtools.h"
#include "framework/toolkits/debugtools/debugswitches.h"
#include "framework/toolkits/math/maths.h"

namespace RWS
{
   RwUInt32 CEventVisualizer::m_uiDepth = 0;

   /**
   *
   *  Constructor fo CEventVisualizer class.
   */
   CEventVisualizer::CEventVisualizer() :InitCEventHandler(0)
   {
      RWS_FUNCTION("RWS::CEventVisualizer::CEventVisualizer");

      LinkMsg(iMsgEventSend, "CRegisteredMsgs*", 0);
      LinkMsg(iMsgEventSent, "CRegisteredMsgs*", 0);
      LinkMsg(iMsgEventReceived, "EventData*", 0);
      LinkMsg(iMsgEventUnRegistered, "CRegisteredMsgs*", 0);

      RWS_RETURNVOID();
   }

   /**
   *
   *  Destructor for CEventVisualizer class.
   *
   */
   CEventVisualizer::~CEventVisualizer()
   {
      RWS_FUNCTION("RWS::CEventVisualizer::~CEventVisualizer");

      UnLinkMsg(iMsgEventUnRegistered);
      UnLinkMsg(iMsgEventReceived);
      UnLinkMsg(iMsgEventSend);
      UnLinkMsg(iMsgEventSent);

      RWS_RETURNVOID();
   }

   /**
   *
   *  Handle events function for CEventVisualizer class, listens to event generated by the event system
   *  for visualizing events. In detail \ref iMsgEventReceived and \ref iMsgEventUnRegistered
   *
   *  \param pMsg Reference to a CMsg object containing the event that caused HandleEvents to be called.
   *
   */
   void CEventVisualizer::HandleEvents(CMsg &pMsg)
   {
      RWS_FUNCTION("RWS::CEventVisualizer::HandleEvents");

      if (pMsg.Id == iMsgEventSend)
      {
         Process_iMsgEventSend(pMsg);
      }
      else if (pMsg.Id == iMsgEventReceived)
      {
         Process_iMsgEventReceived(pMsg);
      }
      else if (pMsg.Id == iMsgEventSent)
      {
         Process_iMsgEventSent(pMsg);
      }
      else if (pMsg.Id == iMsgEventUnRegistered)
      {
         Process_iMsgEventUnRegistered(pMsg);
      }

      RWS_RETURNVOID();
   }

   /**
   *
   *  Process a iMsgEventReceived event, this event is received each time an event is received by an event handler.
   *
   */
   void CEventVisualizer::Process_iMsgEventReceived(CMsg &pMsg)
   {
      RWS_FUNCTION("RWS::CEventVisualizer::Process_iMsgEventFired");

      if (DebugSwitches::GetSwitch(DebugSwitches::m_enable_message_flow_debug_info))
      {
         EventData *pEventData = static_cast<EventData*>(pMsg.pData);

         RwV3d DummySrcVect;

         RwV3d *pSrcVect;
         RwV3d *pDstVect;

         if (pEventData->m_pDst && pEventData->m_pDst->Get_m_pCEventVisualization())
         {
            pDstVect = pEventData->m_pDst->Get_m_pCEventVisualization()->GetWorldPos();
         }
         else
         {
            // If no world co-ords can be obtained for the destination then
            // it cannot be visualized.
            //
            RWS_RETURNVOID ();
         }

         if (pEventData->m_pSrc && pEventData->m_pSrc->Get_m_pCEventVisualization())
         {
            pSrcVect = pEventData->m_pSrc->Get_m_pCEventVisualization()->GetWorldPos();
         }
         else
         {
            pSrcVect = 0;
         }

         if (pSrcVect == 0)
         {
            DummySrcVect = *pDstVect;

            DummySrcVect.y += 100.0f;

            pSrcVect = &DummySrcVect;
         }

         RwV3d TextVect;
      
         RwV3dAdd(&TextVect, pSrcVect, pDstVect);
         RwV3dScale(&TextVect, &TextVect, 0.5f);
      
#ifdef RWS_DEBUGTOOLS
         if (pEventData->m_pRegisteredEvent && pEventData->m_pRegisteredEvent->p_msgname)
         {
            DebugTools::PrintfXYZ(&TextVect, pEventData->m_pRegisteredEvent->p_msgname);
         }

         DebugTools::Arrow(pSrcVect, pDstVect, 255, 255, 255);
#endif
      }

      RWS_RETURNVOID();
   }

   /**
   *
   *  Process a iMsgEventUnRegistered event, this event is received when and event
   *  is completely unregistered from the event system.
   *
   */
   void CEventVisualizer::Process_iMsgEventUnRegistered(CMsg &pMsg)
   {
      RWS_FUNCTION("RWS::CEventVisualizer::Process_iMsgEventDeleted");

#ifdef _DEBUG
      CRegisteredMsgs *pCRegisteredMsgs = static_cast<CRegisteredMsgs*>(pMsg.pData);

      pCRegisteredMsgs = pCRegisteredMsgs; // Prevent unused variable warning
#endif

      RWS_RETURNVOID();
   }

   /**
   *
   *  Indent the event with m_uiDepth characters of >
   *
   */
   void CEventVisualizer::Indent()
   {
#ifdef RWS_DEBUGTOOLS
      for (RwUInt32 i = 0; i < m_uiDepth; i++)
      {
         DebugTools::Printf(">");
      }
#endif
   }
   /**
   *
   *  Process a iMsgEventSend event, this event is received when an event is send
   *  using SendMsg or SendMsgToEventHandler
   */
   void CEventVisualizer::Process_iMsgEventSend(CMsg &pMsg)
   {
      RWS_FUNCTION("RWS::CEventVisualizer::Process_iMsgEventSend");

      if (DebugSwitches::GetSwitch(DebugSwitches::m_enable_message_flow_debug_info))
      {
         CRegisteredMsgs *pCRegisteredMsgs = static_cast<CRegisteredMsgs*>(pMsg.pData);

#ifdef RWS_DEBUGTOOLS
         m_uiDepth++;

         Indent();

         DebugTools::Printf("%s\n", pCRegisteredMsgs->p_msgname);
#endif
      }
      RWS_RETURNVOID();
   }

   /**
   *
   *  Process a iMsgEventSent event, this event is received when an event has been sent
   *  using SendMsg or SendMsgToEventHandler
   */
   void CEventVisualizer::Process_iMsgEventSent(CMsg &pMsg)
   {
      RWS_FUNCTION("RWS::CEventVisualizer::Process_iMsgEventSend");

      if (DebugSwitches::GetSwitch(DebugSwitches::m_enable_message_flow_debug_info))
      {
#ifdef _DEBUG
         CRegisteredMsgs *pCRegisteredMsgs = static_cast<CRegisteredMsgs*>(pMsg.pData);

         pCRegisteredMsgs = pCRegisteredMsgs;   // Prevent unused variable warning
#endif

#ifdef RWS_DEBUGTOOLS
         m_uiDepth--;
#endif
      }
      RWS_RETURNVOID();
   }
}

#endif