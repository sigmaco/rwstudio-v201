/*****************************************************************************
*
* File :     EventVisualization.h
*
* Abstract :
*
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
* Copyright (c) 2003 Criterion Software Ltd.
* All Rights Reserved.
*
* RenderWare is a trademark of Canon Inc.
*
*****************************************************************************/

#ifdef RWS_EVENTVISUALIZATION

#ifndef __EVENTVISUALIZATION_H__
#define __EVENTVISUALIZATION_H__

#include "framework\core\eventhandler\ceventhandler.h"

namespace RWS
{
   /**
   *
   *  \ingroup EventVisualizerToolKit
   *
   *  CEventVisualizer provides in game onscreen visualization of events. It does this by responding
   *  to the following system events iMsgEventSend, iMsgEventSent, iMsgEventReceived and iMsgEventUnRegistered.
   *
   */
   class CEventVisualizer: public CEventHandler
   {
      public:

         CEventVisualizer();
         ~CEventVisualizer();

         virtual void HandleEvents(CMsg &pMsg);

      private:

         void Process_iMsgEventSend(CMsg &pMsg);

         void Process_iMsgEventSent(CMsg &pMsg);

         void Process_iMsgEventReceived(CMsg &pMsg);

         void Process_iMsgEventUnRegistered(CMsg &pMsg);

         void Indent();

         static RwUInt32 m_uiDepth;
   };
}

#endif

#endif