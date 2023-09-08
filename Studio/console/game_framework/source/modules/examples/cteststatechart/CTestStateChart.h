/*****************************************************************************
*
* File :       Tutorial3.h
*
* Abstract :  Editing behavior attributes.
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

#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/core/eventhandler/ceventhandler.h"

#include "framework/toolkits/world/smartptrs/catomicptr.h"
#include "framework/mainloop/logic.h"

#include "framework/core/attributehandler/allocation policies/LinearAllocationPolicy.h"

#include "plugins/statecharts/include/StateChartHandler.h"

namespace RWS
{
   namespace Examples
   {
      /**
      *
      *  \ingroup Mod_Tutorials
      *
      *  Rotate an atomic object within a behavior, using RenderWare studio
      *  to control the amount of rotation.
      *
      *  \see CAttributeHandler
      *  \see CEventHandler
      *  \see CSystemCommands
      *  \see CAtomicPtr
      *
      */
      class CTestStateChart : public CSystemCommands,
          public CAttributeHandler ,
          public CEventHandler,
          public LinearAllocationPolicy,
          public CStateChartHandler
      {
      public:
         RWS_MAKENEWCLASS(CTestStateChart);
         RWS_DECLARE_CLASSID(CTestStateChart);
         RWS_CATEGORY("Examples");
         RWS_DESCRIPTION("CTestStateChart", "CTestStateChart");

         CTestStateChart(const CAttributePacket& attr);
         ~CTestStateChart(void);

         virtual void HandleEvents(CMsg &pMsg);
         virtual void HandleAttributes(const CAttributePacket& attr);

      protected:
         RwMatrix *m_mat;        /**< Matrix used for rotating object. */
         CAtomicPtr m_pAtomic;   /**< Behavior's Atomic. */
      };

   }//namespace Tutorial
}//namespace RWS