/*****************************************************************************
*
* File :     FPSPickup.cpp
*
* Abstract :  Implementation of FPSPickup class
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
* Copyright (c) 2002 Criterion Software Ltd.
* All Rights Reserved.
*
* RenderWare is a trademark of Canon Inc.
*
*****************************************************************************/
#include "precomp.h"
#ifdef __MWERKS__
#pragma force_active on // Do not deadstrip this class
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "fpspickup.h"
#include "framework/toolkits/math/maths.h"
#include "framework/toolkits/world/factory.h"

namespace RWS
{
   namespace FPS
   {
      RWS_IMPLEMENT_CLASSID(FPSPickup);

      CEventId FPSPickup::m_msg_rx;

      /**
      *
      *  \ref FPSPickup. Construct an FPSPickup object.
      *
      *  \see ~FPSPickup
      *
      */
      FPSPickup::FPSPickup (const CAttributePacket& attr)
         :  InitCEventHandler(&m_pAtomic)
      {
         RWS_FUNCTION("RWS::FPS::FPSPickup::FPSPickup");
         
         m_pAtomic = CreateAtomicInWorldFromResource(attr, this);
         
         RWS_ASSERT(m_pAtomic, "Failed to create atomic");
         
         RegisterMsg(m_msg_rx, "ACTN_PLAYERTOUCH", "CEventHandler*");
         LinkMsg(m_msg_rx, "CEventHandler*");

         RWS_RETURNVOID();
      }
      

      /**
      *
      *  \ref ~FPSPickup. Destroy a FPSPickup object.
      *
      *  \see FPSPickup
      *
      */
      FPSPickup::~FPSPickup () 
      {
         RWS_FUNCTION("RWS::FPS::FPSPickup::~FPSPickup");
         
         UnLinkMsg(m_msg_rx);
         
         UnRegisterMsg(m_msg_rx);
         UnRegisterMsg(m_msg_response_tx);
         
         RWS_RETURNVOID();
      }
      

      /**
      *
      *   \ref set_m_msg_response_tx. Specify the respond event, sent when trigger pickup event is received.
      *
      *   \param pEventName a string containing the name of the response event.
      *
      */
      void FPSPickup::Set_m_msg_response_tx(const RwChar *pEventName)
      {
         RWS_FUNCTION("RWS::FPS::FPSPickup::Set_m_msg_response_tx");
         RWS_PRE(pEventName);
         
         ReplaceRegisteredMsg(m_msg_response_tx, pEventName, "CEventHandler*");
         
         RWS_RETURNVOID();
      }
      

      /**
      *
      * \ref HandleEvents, when m_msg_rx is received m_msg_response_tx is sent back to the
      * originator of the event.
      *
      * \param pMsg reference to a CMsg.
      *
      */
      void FPSPickup::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::FPS::FPSPickup::HandleEvents");

         if (pMsg.Id == m_msg_rx && pMsg.pData)
         {
            // Send Response Message
            //
            CMsg Msg(m_msg_response_tx, static_cast<CEventHandler*>(this) );
            
            SendMsgToEventHandler (Msg, static_cast<CEventHandler*>(pMsg.pData));
         }

         RWS_RETURNVOID();
      }
      

      /**
      *
      * \ref HandleAttributes. Handle attribute updates.
      *
      * \param rAttr reference to a CAttributePacket.
      *
      */
      void FPSPickup::HandleAttributes(const CAttributePacket &rAttr)
      {
        RWS_FUNCTION("RWS::FPS::FPSPickup::HandleAttributes");

        m_pAtomic.HandleSystemCommands(rAttr);

        RWS_RETURNVOID();
      }
   }
}