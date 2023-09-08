/*****************************************************************************
*
* File :      CEntity.cpp
*
* Abstract :   Implement very simple behavior provided as a starting point
*              for the empty framework.
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
#include "CEntity.h"
#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/math/maths.h"
#include "framework/toolkits/world/helpers/framehelper.h"
#include "framework/mainloop/render.h"

#include "framework/core/attributehandler/clinearmemorymanager.h"
#include "framework/core/attributehandler/allocation policies/dynamicallocationpolicy.h"

namespace RWS
{
   namespace Generic
   {
      RWS_IMPLEMENT_CLASSID(CEntity);
      RWS_REGISTERCLASS(CEntity);
      
      /*
      *
      *  Constructor
      *
      *  \param attr the standard attribute data
      *
      */
      
      CEntity::CEntity(const CAttributePacket& attr) : InitCEventHandler(0)
      {
         RWS_FUNCTION("RWS::Generic::CEntity::CEntity");

         // Add code to construct instance of CEntity here, note attr contains all attributes
         // that are then passed to HandleAttributes. Later updates to attributes at design time
         // are send directly to HandleAttributes.
         //
          
         RWS_RETURNVOID();
      }
      
      
      /*
      *
      *  Destructor
      *
      */
      CEntity::~CEntity(void)
      {
         RWS_FUNCTION("RWS::Generic::CEntity::~CEntity");
         
         // Add code to deconstruct instance of CEntity here.
         //
         
         RWS_RETURNVOID();
      }
      
      
      /*
      *
      *  Handle events, processes message events.
      *
      * \param pMsg standard message data.
      *
      */
      void CEntity::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::Generic::CEntity::HandleEvents");

         // Add code to handle events here.
         //
         
         RWS_RETURNVOID();
      }
      
      
      /*
      *
      *  Handle attributes, processes attribute changes.
      *
      * \param attr standard attribute packets.
      *
      */
      void CEntity::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::Generic::CEntity:HandleAttributes");
         
         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(CEntity));
         while (!attrIt.IsFinished())
         {
            // Add code to handle attributes here.
            //
            
            ++attrIt;
         }
         
         RWS_RETURNVOID();
      }
   }//namespace Tutorial
}//namespace RWS