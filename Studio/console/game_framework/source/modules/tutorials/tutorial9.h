/*****************************************************************************
*
* File :       Tutorial9.h
*
* Abstract :   Creating a behavior without using the CAtomicPtr object.
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

#include "framework/core/attributehandler/csystemcommands.h"
#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/core/eventhandler/ceventhandler.h"
#include "framework/toolkits/world/factory.h"
#include "framework/toolkits/world/helpers/atomichelper.h"

#include "framework/mainloop/logic.h"

#include "framework/core/attributehandler/allocation policies/LinearAllocationPolicy.h"

namespace RWS
{
   namespace Tutorial
   {
      /**
      *
      * \ingroup Mod_Tutorials
      *
      *  Implementation of bare minimum for a behavior class.
      *
      *  \see CAttributeHandler
      *  \see CEventHandler
      *  \see CSystemCommands
      *  \see CAtomicPtr
      *
      */
      class CTutorial9 : public CSystemCommands, public CAttributeHandler , public CEventHandler, public LinearAllocationPolicy
#ifdef RWS_EVENTVISUALIZATION
      , public CEventVisualization
#endif
      {
      public:
         RWS_MAKENEWCLASS(CTutorial9);
         RWS_DECLARE_CLASSID(CTutorial9);
         RWS_CATEGORY("Tutorial");
         RWS_DESCRIPTION("Tutorial9", "Tutorial 9");

         CTutorial9(const CAttributePacket& attr);
         ~CTutorial9(void);

         virtual void HandleEvents(CMsg &pMsg);
         virtual void HandleAttributes(const CAttributePacket& attr);

#ifdef RWS_EVENTVISUALIZATION
         virtual RwV3d *GetWorldPos(void);
#endif

      protected:
         RpAtomic  *m_pAtomic; /**< Behaviors atomic. */
      };
   }//namespace Tutorial
}//namespace RWS