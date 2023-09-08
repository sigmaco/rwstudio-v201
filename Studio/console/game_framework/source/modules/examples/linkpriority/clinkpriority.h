/*****************************************************************************
 *
 * File :     CLinkPriority.h
 *
 * Abstract :  
 *
 * Example of using the Link Priority feature of a CEventHandler
 *
 * A Priority value can be specified as a parameter to LinkMsg 
 *
 * The higher the priority (max 0xffff) the earlier it will be called
 * when that event is triggered.
 *
 * Default Priority is 0x8000
 *
 * Lowest Priority is 0x0000
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

#ifndef __CLinkPriority_H__
#define __CLinkPriority_H__

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/core/eventhandler/ceventhandler.h"
#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/toolkits/world/smartptrs/catomicptr.h"
#include "framework/core/attributehandler/allocation policies/LinearAllocationPolicy.h"


namespace RWS
{
   namespace Examples
   {
      /**
      *
      * \ingroup Mod_Examples
      *
      * A simple behavior class to demonstrate the use of the Link Priority features of the \ref CEventHandler.
      *
      * The priority value can be edited using a slider within the attributes. The higher the priority, the
      * earlier the entity will be called, after the linked event is triggered. The highest priority is 65535
      * (0xFFFF), the lowest is 0 and the default value is 32768 (0x8000).
      *
      * \see CAttributeHandler, CEventHandler
      *
      */
      class CLinkPriority: public CEventHandler, public CAttributeHandler, public LinearAllocationPolicy
      {
         public:

            RWS_MAKENEWCLASS(CLinkPriority);
            RWS_DECLARE_CLASSID(CLinkPriority);
            RWS_CATEGORY("Examples");
            RWS_DESCRIPTION("Link Priority", "Example of a behavior where the link priority can be specified to control the order in which events are processed.");

            /** 
            *
            * The attribute definition for CMD_priority allows you too alter the link priority of a
            * behavior from within RenderWare Studio.
            *
            */

            RWS_BEGIN_COMMANDS

               RWS_ATTRIBUTE( CMD_priority,   "Priority","Specify the link priority, see LinkMsg", SLIDER, RwUInt32, RANGE(0, 32768, 65535))

            RWS_END_COMMANDS;

            //////////////////////////////////////////////////////////////////
            // 
            virtual void HandleEvents(CMsg &pMsg);

            //////////////////////////////////////////////////////////////////
            // 
            virtual void HandleAttributes(const CAttributePacket& attr);

            //////////////////////////////////////////////////////////////////
            // 
            CLinkPriority(const CAttributePacket&);
            ~CLinkPriority(void);

         protected:

            CAtomicPtr m_pAtomic; /**< Behaviors atomic */
            RwUInt16  m_priority; /**< Link Priority, current value */
      };
   }//namespace Examples
}//namespace RWS
#endif