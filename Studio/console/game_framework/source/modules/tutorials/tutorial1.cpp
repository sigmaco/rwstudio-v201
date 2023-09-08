/*****************************************************************************
*
* File :      Tutorial1.cpp
*
* Abstract :  Creating a behavior.
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
#include "tutorial1.h"
#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/math/maths.h"
#include "framework/toolkits/world/factory.h"

namespace RWS
{
   namespace Tutorial
   {
      RWS_IMPLEMENT_CLASSID(CTutorial1);
      RWS_REGISTERCLASS(CTutorial1);

      /**
      *
      *  Create CTutorial1 object.  Gets behaviors atomic by extracting information the behavior needs from the 
      *  attributes used to construct the behavior.
      *
      *  \param attr the standard attribute data
      *
      */
      CTutorial1::CTutorial1(const CAttributePacket& attr)
         : InitCEventHandler(&m_pAtomic)
      {
         RWS_FUNCTION("RWS::Tutorial::CTutorial1::CTutorial1");

         m_pAtomic = CreateAtomicInWorldFromResource(attr, this);

         RWS_ASSERT(m_pAtomic, "Failed to create atomic");

         RWS_RETURNVOID();
      }
   
      /**
      *
      *  Destroy CTutorial1 object
      *
      */
      CTutorial1::~CTutorial1(void)
      {
         RWS_FUNCTION("RWS::Tutorial::CTutorial1::~CTutorial1");
         RWS_RETURNVOID();
      }
 
      /**
      *
      *  Handle events, processes message events.
      *
      * \param pMsg standard message data.
      *
      */
      void CTutorial1::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::Tutorial::CTutorial1::HandleEvents");
         RWS_RETURNVOID();
      }
  
      /**
      *
      *  Handle attributes, processes attribute changes.
      *
      * \param attr standard attribute packets.
      *
      */
      void CTutorial1::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::Tutorial::CTutorial1:HandleAttributes");

         CAttributeHandler::HandleAttributes(attr);

         // handle system commands,  e.g. if object moved from within RenderWare Studio, then
         // move them on console

         m_pAtomic.HandleSystemCommands(attr);

         RWS_RETURNVOID();
      }
      
   }//namespace Tutorial
}//namespace RWS
