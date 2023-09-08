/*****************************************************************************
*
* File :      Tutorial7.cpp
*
* Abstract :  Debug macros.
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
#define RWS_CALLSTACKENABLE
#define RWS_TRACEENABLE

// Left to its own devices, the MW linker will dead-strip the registrar ctors, so...
#ifdef __MWERKS__
#pragma force_active on
#pragma exceptions off // Turn off exceptions due to bug in MW Dwarf Generator for PS2, present in CW 3.04
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "tutorial7.h"
#include "framework/toolkits/math/maths.h"
#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/world/factory.h"

namespace RWS
{
   namespace Tutorial
   {
      RWS_IMPLEMENT_CLASSID(CTutorial7);
      RWS_REGISTERCLASS(CTutorial7);

      /**
      *
      *  Create CTutorial7 object.  Gets behaviors atomic by extracting information the behavior needs from the 
      *  attributes used to construct the behavior. Does not set default values for the attribute controls. This
      *  is because the values will be fed in as attribute packets and setup by the 'HandleAttributes' function
      *  instead.
      *
      *  \see HandleAttributes
      *  \param attr the attribute packets used to initialize. This includes the attribute control's values.
      *
      */
      CTutorial7::CTutorial7(const CAttributePacket& attr)
        : InitCEventHandler(&m_pAtomic)
      {
         RWS_FUNCTION("RWS::Tutorial::CTutorial::CTutorial7");

         m_pAtomic = CreateAtomicInWorldFromResource(attr, this);

         RWS_ASSERT(m_pAtomic, "Failed to create atomic");

         RWS_RETURNVOID();
      }
   
      /**
      *
      *  Destroy CTutorial7 object.
      *
      */
      CTutorial7::~CTutorial7(void)
      {
         RWS_FUNCTION("RWS::Tutorial::CTutorial7::~CTutorial7");
         RWS_RETURNVOID();
      }
   
      /**
      *
      *  Handle events. Would handle any message processing for the behavior, but this tutorial does not do
      *  any message processing. However, the function has to exist as it is a virtual.
      *
      *  \param pMsg standard message packets.
      *
      */
      void CTutorial7::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::Tutorial::CTutorial7::HandleEvents");
         RWS_RETURNVOID();
      }

      /**
      *
      *  Handle attributes and extract data for them from packets. Use this to update the internal variables of
      *  the behavior.
      *
      *  \param attr standard attribute packets, some of which contain updates to the behavior's attribute controls.
      *
      */
      void CTutorial7::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::Tutorial::CTutorial7::HandleAttributes");

         CAttributeHandler::HandleAttributes(attr);

         // Handle system commands,  e.g. if object moved from within RenderWare studio, then
         // move them on console.
         m_pAtomic.HandleSystemCommands(attr);

         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(CTutorial7));

         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
            case CMD_testmessage:
               RWS_TRACE(attrIt->GetAs_RwChar());
               break;

            case CMD_testboolean:
               RWS_TRACE(attrIt->GetAs_RwUInt32());
               break;

            case CMD_testslider:
               RWS_TRACE(attrIt->GetAs_RwReal());
               break;

            case CMD_testcolor:
               m_Color = attrIt->GetAs_RwRGBA();
                            
               RWS_TRACE(reinterpret_cast<RwUInt32*>(m_Color.alpha) <<" - ALPHA");
               RWS_TRACE(reinterpret_cast<RwUInt32*>(m_Color.red)   <<" - RED");
               RWS_TRACE(reinterpret_cast<RwUInt32*>(m_Color.green) <<" - GREEN");
               RWS_TRACE(reinterpret_cast<RwUInt32*>(m_Color.blue)  <<" - BLUE");
               break;

            case CMD_testlist:
               RwUInt32 type;

               attrIt->GetCommandData(type);

               switch (type)
               {
               case 0: 
                  RWS_TRACE("Selected List Element 1")
                  break;

               case 1: 
                  RWS_TRACE("Selected List Element 2")
                  break;

               case 2: 
                  RWS_TRACE("Selected List Element 3")
                  break;
               }

            break;
            }

         ++attrIt;
         }

         RWS_RETURNVOID();
      }
      
   }//namespace Tutorial
}//namespace RWS
