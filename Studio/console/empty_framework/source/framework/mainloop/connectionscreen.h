/*****************************************************************************
*
* File :     connectionscreen.h
*
* Notes : This file contains the behaviour code which is used to display the
* waiting to connect screen. The bulk of the code being in the LoadingScreen
* project.
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
* Copyright (c) 2000 Criterion Software Ltd.
* All Rights Reserved.
*
* RenderWare is a trademark of Canon Inc.
*
*****************************************************************************/

#ifdef RWS_DESIGN  

#include "framework/core/eventhandler/ceventhandler.h"

namespace RWS
{
   namespace ConnectionScreen
   {
      /**
      *
      *  \ingroup ConnectionScreen
      *
      *  The CRenderConnectionScreen class defines a very simple event handler that displays the waiting for 
      *  connection screen.
      *
      */
      class CRenderConnectionScreen : public CEventHandler
      {
      public:
         
         CRenderConnectionScreen();
         ~CRenderConnectionScreen();
         
         virtual void HandleEvents(CMsg &pMsg);

      private:

         RwBool m_MemoryProfilingEnabled;
      };
   }
}
#endif
