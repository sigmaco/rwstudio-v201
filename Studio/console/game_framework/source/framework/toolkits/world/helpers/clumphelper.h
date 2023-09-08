/*****************************************************************************
*
* File :     ClumpHelper.h
*
* Abstract : Helper functions for use with RpClump
*
*****************************************************************************
*
* This file is a product of Criterion Software Ltd.
*
* This file is provided as is with no warranties of any kind and is
* provided without any obligation on criterion software ltd. or
* Canon Inc. to assist in its use or modification.
*
* Criterion Software Ltd. will not, under any
* circumstances, be liable for any lost revenue or other damages arising
* from the use of this file.
*
* Copyright (c) 2001 Criterion Software Ltd.
* All Rights Reserved.
*
* RenderWare is a trademark of Canon Inc.
*
*****************************************************************************/

#ifndef __CLUMPHELPER_H__
#define __CLUMPHELPER_H__

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/core/attributehandler/csystemcommands.h"
#include "framework/core/eventhandler/ceventhandler.h"

namespace RWS
{
   /**
   *
   *  \ingroup ClumpHelper
   *
   *  Helper functions for RenderWare Graphics RpClump object.
   *
   */
   namespace ClumpHelper
   {
      /**
      *  \ingroup SystemEvents
      *  \page ClumpHelperDeleteEvent Other Events
      *  \li iMsgPreRpClumpDestroy
      *  This event used to indicate that a frame hierarchy is about to be destroyed
      *  by RWS::CClumpPtr::Destroy. This is useful where behaviors
      *  such as a player can attach themselves to the hierarchy of other objects, 
      *  before the object can be destroyed it needs to detach
      *  itself from the object, for an example \see RWS::FPS::FPSPlayer.
      */
      RWS_DEFINE_EVENT( iMsgPreRpClumpDestroy,
                        "RwFrame*",
                        "Event used to indicate that a frame hierarchy is about"
                        "to be destroyed by ClumpHelper::Destroy.");

      void Open(void);

      void Close(void);

      RwBool Destroy(RpClump* pClump);

      void SetIsVisible(RpClump& clump, bool isVisible);

      void SetCanCollide(RpClump& clump, bool canCollide);
      
      void SetEventHandler(RpClump& clump, CEventHandler* pEventHandler);

      void RenderAllVisibleAtomics(RpClump * pClump);

      void HandleSystemCommands(RpClump& clump, const CAttributePacket& attr);
   }
}

#endif