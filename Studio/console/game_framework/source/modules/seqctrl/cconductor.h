/*****************************************************************************
*
* File :      CConductor.h
*
* Abstract :  Sequence controller behaviour.
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

#ifndef C_CONDUCTOR_HEADER
#define C_CONDUCTOR_HEADER

#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/core/eventhandler/ceventhandler.h"
#include "framework/core/attributehandler/allocation policies/LinearAllocationPolicy.h"
#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/world/smartptrs/catomicptr.h"
#include "framework/toolkits/sequencer/cseqprocessor.h"

namespace RWS
{
   namespace SEQUENCE
   {
      /**
      *
      * Sequence instruction block must be attached as a sub-asset.
      *
      */

      class CConductor : public CSystemCommands, public CAttributeHandler, public CEventHandler,
         public LinearAllocationPolicy
      {
      public:

         RWS_MAKENEWCLASS(CConductor);
         RWS_DECLARE_CLASSID(CConductor);
         RWS_CATEGORY("Sequence");
         RWS_DESCRIPTION("Sequence conductor",
            "Controls the sequencing of other object via control instructions.");

         RWS_BEGIN_COMMANDS
            RWS_SEPARATOR("Messages", 0)
               RWS_MESSAGE(MSG_START,
                  "Start event",
                  "Name of event which starts sequence. Data is pointer to RwUInt32 of start "
                     "time in milliseconds. If zero, then takes current time.",
                  RECEIVE,
                  RwUInt32*,
                  0)
            RWS_SEPARATOR("Sequence", 0)
               RWS_ATTRIBUTE(CMD_LOOP_SEQ,
                  "Loop sequence",
                  "Repeatedly loop the sequence.",
                  BOOLEAN,
                  RwUInt32,
                  DEFAULT(0))
               RWS_ATTRIBUTE(CMD_SEQ_SPEED,
                  "Speed",
                  "Speed of playback of sequence.",
                  SLIDER,
                  RwReal,
                  RANGE(0.1, 1, 10))
         RWS_END_COMMANDS;

         CConductor(const CAttributePacket &attr);
         ~CConductor(void);

         virtual void HandleEvents(CMsg &msg);
         virtual void HandleAttributes(const CAttributePacket &attr);

      private:

         void HandleSystemCommands(const CAttributePacket &attr);
         void AddResource(const RWSGUID *pResourceID);

         CEventId m_startMsg,                /**< Event used to detect sequence start. */
            m_waitMsg;                       /**< The event to wait for - specified by instruction. */
         CSeqProcessor m_seqProcessor;       /**< The object used to process the sequence. */
      };
   }     // SEQUENCE
}        // RWS
#endif   // #ifndef C_CONDUCTOR_HEADER
