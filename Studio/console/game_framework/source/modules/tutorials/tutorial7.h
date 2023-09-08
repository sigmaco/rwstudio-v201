/*****************************************************************************
*
* File :       Tutorial7.h
*
* Abstract :   Debug macros.
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
   namespace Tutorial
   {
      /**
      *
      *  \ingroup Mod_Tutorials
      *
      *  Gives examples of how to use attribute controls. Checkbox, slider, color picker and list.
      * 
      *  \see CAttributeHandler
      *  \see CEventHandler
      *  \see CSystemCommands
      *  \see CAtomicPtr
      *
      */
      class CTutorial7 : public CSystemCommands, public CAttributeHandler , public CEventHandler, public LinearAllocationPolicy
      {
      public:
         RWS_MAKENEWCLASS(CTutorial7);
         RWS_DECLARE_CLASSID(CTutorial7);
         RWS_CATEGORY("Tutorial");
         RWS_DESCRIPTION("Tutorial7", "Tutorial 7");

         CTutorial7(const CAttributePacket& attr);
         ~CTutorial7(void);

         virtual void HandleEvents(CMsg &pMsg);
         virtual void HandleAttributes(const CAttributePacket& attr);

         /*
          * Set up one of each attributes, a message receive box, boolean check box, slider, color picker and 
          * a list box.
          */
         RWS_BEGIN_COMMANDS
            RWS_SEPARATOR("Attribute List", 0)
            RWS_MESSAGE( CMD_testmessage, "Receive Event", "Type in event name.", RECEIVE, "RwUInt32", 0)

            RWS_ATTRIBUTE(CMD_testboolean, "Test Boolean", "Check box to send event.", BOOLEAN, RwUInt32, DEFAULT(0))
            RWS_ATTRIBUTE(CMD_testslider, "Test Slider", "Set rotation.", SLIDER, RwReal, RANGE(-4000, 0, 4000))
            RWS_ATTRIBUTE(CMD_testcolor, "Test Color", "Object color.", COLOR, RwRGBA, DEFAULT(0))
            RWS_ATTRIBUTE(CMD_testlist, "Test List", "List Selection", LIST, RwUInt32, LIST("item1|item2|item3"))
         RWS_END_COMMANDS;

      protected:

         CAtomicPtr m_pAtomic;     /**< Behavior's Atomic. */
         CEventId m_message1;      /**< m_message1 is used to store the clients reference to an event. */ 
         RwRGBA m_Color;           /**< Red, green, blue, alpha structure. */
     };
   }//namespace Tutorial
}//namespace RWS