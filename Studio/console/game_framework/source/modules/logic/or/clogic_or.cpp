/*****************************************************************************
*
* File :      CCLogic_OR.cpp
*
* Abstract :  Implement bare minimum for a behavior class
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
#include "clogic_or.h"
#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/math/maths.h"

namespace RWS
{
   namespace Logic
   {
      RWS_IMPLEMENT_CLASSID(CLogic_OR);
      RWS_REGISTERCLASS(CLogic_OR);
      
      /**
      *
      *  Create CLogic_OR object.
      *
      */
      CLogic_OR::CLogic_OR(const CAttributePacket& attr) : InitCEventHandler(0)
      {
         RWS_FUNCTION("RWS::Logic::CLogic_OR::CLogic_OR");
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      *  Destroy CLogic_OR object
      *
      */
      CLogic_OR::~CLogic_OR(void)
      {
         RWS_FUNCTION("RWS::Logic::CLogic_OR::~CLogic_OR");

         UnLinkMsg(m_InputA);
         UnLinkMsg(m_InputB);

         UnRegisterMsg(m_InputA);
         UnRegisterMsg(m_InputB);
         UnRegisterMsg(m_Output);

         RWS_RETURNVOID();
      }
      
      
      /**
      *
      *  Handle events
      *
      */
      void CLogic_OR::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::Logic::CLogic_OR::HandleEvents");

         if (pMsg.Id == m_InputA || pMsg.Id == m_InputB)
         {
            CMsg Msg(m_Output, pMsg.pData);

            SendMsg(Msg);
         }

         RWS_RETURNVOID();
      }
      
      
      /**
      *
      *  Handle attributes
      *
      */
      void CLogic_OR::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::Logic::CLogic_OR:HandleAttributes");
         
         CAttributeHandler::HandleAttributes(attr);
         
         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(CLogic_OR));
         
         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
            case CMD_InputA:
               ReplaceLinkedMsg(m_InputA, attrIt->GetAs_RwChar_ptr(), 0);
               break;
               
            case CMD_InputB:
               ReplaceLinkedMsg(m_InputB, attrIt->GetAs_RwChar_ptr(), 0);
               break;
               
            case CMD_Output:
               ReplaceRegisteredMsg(m_Output, attrIt->GetAs_RwChar_ptr(), 0);
               break;
            }
            ++attrIt;
         }
         
         RWS_RETURNVOID();
      }
   }//namespace Logic
}//namespace RWS