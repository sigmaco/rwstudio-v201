/*****************************************************************************
*
* File :     CLinkPriority.cpp
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
#include "clinkpriority.h"
#include "modules/generic/cdebugtools/cdebugtools.h"
#include "framework/mainloop/logic.h"
#include "framework/toolkits/debugtools/debugtools.h"
#include "framework/toolkits/world/factory.h"

namespace RWS
{
   namespace Examples
   {
      RWS_IMPLEMENT_CLASSID(CLinkPriority);
      RWS_REGISTERCLASS(CLinkPriority);

      /**
      *
      * Constructor for CLink Priority, gets behaviors atomic and registers to iMsgRunningTick.
      * The value of m_priority is NOT defaulted here, because the attribute handler will be called
      * during creation and this will default the value to the one in the workspace, i.e. it will be
      * set only once.
      *
      */
      CLinkPriority::CLinkPriority(const CAttributePacket& attr) :
         InitCEventHandler(0)
      {
         RWS_FUNCTION("RWS::Examples::CLinkPriority::CLinkPriority");

         // Can't use initializer ('this' isn't valid) so...
         m_pAtomic = CreateAtomicInWorldFromResource(attr, this);

         RWS_ASSERT(m_pAtomic, "Failed to create atomic");

         LinkMsg(iMsgRunningTick);

         RWS_RETURNVOID();
      }
   

      /**
      *
      * Destructor for CLinkPriority. Unlinks from the iMsgRunningTick message.
      *
      */
      CLinkPriority::~CLinkPriority(void)
      {
         RWS_FUNCTION("RWS::Examples::CLinkPriority::~CLinkPriority");

         UnLinkMsg(iMsgRunningTick);

         RWS_RETURNVOID();
      }
   

      /**
      *
      * Handle events for CLinkPriority. Displays a debug message every time the iMsgRunningTick is received (debug mode).
      *
      */
      void CLinkPriority::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::Examples::CLinkPriority::HandleEvents");

         if (pMsg.Id == iMsgRunningTick)
         {
   #ifdef RWS_DEBUGTOOLS
            DebugTools::Printf("CLinkPriority::HandleEvents 0x%x Priority 0x%x\n", this, m_priority);
   #endif
         }

         RWS_RETURNVOID();
      }
   

      /**
      *
      * Handle attributes for CLinkPriority.  When the link priority slider is updated in the workspace, this function
      * receives a new attribute packet for it. The code then changes the priority of the linkage to the iMsgRunningTick
      * to match that specified in the workspace.
      *
      */
      void CLinkPriority::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::Examples::CLinkPriority::HandleAttributes");

         CAttributeHandler::HandleAttributes(attr);

         // Initialize the contained class first.
         //
         m_pAtomic.HandleSystemCommands(attr);

         // Find the parameters for this class "CLinkPriority" if any
         //
         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(CLinkPriority));
      
         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
               //////////////////////////////////////////////////////////////////
               case CMD_priority:
               //////////////////////////////////////////////////////////////////
               {
                  attrIt->GetCommandData(m_priority);
               
                  UnLinkMsg(iMsgRunningTick);
                  LinkMsg(iMsgRunningTick, 0, m_priority);
               }
               break;
            }
            ++attrIt;
         }

         RWS_RETURNVOID();
      }
   }//namespace Examples
}//namespace RWS
