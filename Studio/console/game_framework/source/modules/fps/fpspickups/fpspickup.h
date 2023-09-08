/*****************************************************************************
*
* File : FPSPickup.h
*
* Abstract : Declaration of FPSPickup class
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
#ifndef __FPS_PICKUP_H__
#define __FPS_PICKUP_H__

#include "framework/core/eventhandler/ceventhandler.h"
#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/core/attributehandler/csystemcommands.h"
#include "framework/toolkits/world/smartptrs/catomicptr.h"

namespace RWS
{
   namespace FPS
   { 
      /**
      *
      *  \ingroup Mod_FPS
      *
      *  Basic class for creating "pickup" behaviors. Pickups are typically objects that can be collected by the 
      *  player. Typically the pickup knows it is being collected when the player touches the object and receives the
      *  trigger event, the source of the touch event is sent an event which identifies the type of pickup i.e. health,
      *  ammo etc
      *
      *  The player behavior is responsible for updating the properties of the player e.g. Health, Ammo,
      *  Speed. The player is also responsible for deleting the pickup, this is to allow the player behavior to check if
      *  it can collect the pickup, for example if the player has 100% health it doesn't collect the next health pickup.
      *
      *  By default FPSPickup responds to the event ACTN_PLAYER_TOUCH that passes a pointer to a CEventHandler usually the player
      *  behavior.
      *
      *  The only way to use FPSPickup is to derive it into another class,  see FPSExamplePickup,  and initialize the events from there.
      *  You will then need to add the following entries into FPSPlayer so that the player knows what to do once a pickup has
      *  been collected.
      *
      *  IN FPSPlayer add (for example);
      *
      *  FPSPLAYER.H
      *    \verbatim 
            CEventId m_Event_FPS_Example_Pickup;
           \endverbatim
      *
      *    This specifies an event for the pickup
      *
      *
      *
      *  FPSPLAYER.CPP  in FPSPlayer()        
      *    \verbatim 
            RegisterMsg ( m_Event_FPS_Example_Pickup,"FPS_Example_Pickup","CEventHandler*");
           \endverbatim 
      *
      *     This registers the message with its name and type
      *
      *
      *  FPSPLAYER.CPP in HandleEvents
      *
      *    \verbatim
            if(pMsg.Id == m_Event_FPS_Example_Pickup)
            {
              RWS_TRACE("FPSPlayer::HandleEvents FPS_Example_Pickup Collected");

              // Do Pickup code here,  e.g. increase health,  pickup new gun etc

              DeletePickup(pMsg);
            }
            \endverbatim
      *
      *
      *
      *    FPSPLAYER.CPP in ~FPSPlayer
      *
      *     \verbatim
             UnRegisterMsg (m_Event_FPS_Example_Pickup);
            \endverbatim
      *
      *       This will unregister the pickup event indicating the player behavior is no longer using it.
      *
      *
      */
      class FPSPickup : public CSystemCommands, public CEventHandler, public CAttributeHandler
      {
      public:
         RWS_DECLARE_CLASSID(FPSPickup);
         
         FPSPickup (const CAttributePacket& attr);
         ~FPSPickup (void);
         
         // Process messages/events
         //
         virtual void HandleEvents(CMsg &pMsg);
         void HandleAttributes(const CAttributePacket& rAttr);

      protected:
         void Set_m_msg_response_tx(const RwChar *p);
         
         CAtomicPtr m_pAtomic;            /**< \ref CAtomicPtr */         
         static CEventId m_msg_rx;        /**< Receive Message, with pointer to entity */
         CEventId m_msg_response_tx;      /**< Transmit Message, sent the entity specified by m_msg_rx. */
      };
   }
}

#endif
