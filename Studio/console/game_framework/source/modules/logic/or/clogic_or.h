/*****************************************************************************
*
* File :       CLogic_OR.h
*
* Abstract :   Implement bare minimum for a behavior class
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

#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/core/eventhandler/ceventhandler.h"
#include "framework/toolkits/world/smartptrs/catomicptr.h"
#include "framework/mainloop/logic.h"

#include "framework/core/attributehandler/allocation policies/LinearAllocationPolicy.h"

namespace RWS
{
   namespace Logic
   {
      /**
      *
      * \ingroup Mod_Logic
      *
      *  Implementation of an event based or class, If Input Event A or B are received send Output Event.
      *
      *  \see CAttributeHandler, CEventHandler
      *
      */
      class CLogic_OR : public CAttributeHandler , public CEventHandler, public LinearAllocationPolicy
      {
         public:
            RWS_MAKENEWCLASS(CLogic_OR);
            RWS_DECLARE_CLASSID(CLogic_OR);
            RWS_CATEGORY("Logic");
            RWS_DESCRIPTION("OR", "Output Event is triggered if event A or B is received.");
         
            RWS_BEGIN_COMMANDS
               RWS_MESSAGE  (CMD_InputA, "Input Event A", "When Input Event A or B are received send Output Event", RECEIVE , 0 , 0)
               RWS_MESSAGE  (CMD_InputB, "Input Event B", "When Input Event A or B are received send Output Event", RECEIVE , 0 , 0)
               RWS_MESSAGE  (CMD_Output, "Output Event", "When Input Event A or B are received send Output Event", TRANSMIT , 0 , 0)
            RWS_END_COMMANDS;
            
            CLogic_OR(const CAttributePacket& attr);
            ~CLogic_OR(void);
         
            virtual void HandleEvents(CMsg &pMsg);
            virtual void HandleAttributes(const CAttributePacket& attr);

         protected:

            CEventId m_InputA;
            CEventId m_InputB;
            CEventId m_Output;
      };
   }//namespace Logic
}//namespace RWS