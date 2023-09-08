/*****************************************************************************
*
* File :       Tutorial8.h
*
* Abstract :   Inheritance.
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

namespace RWS
{
   namespace Tutorial
   {
      /**
      *
      *  \ingroup Mod_Tutorials
      *
      *  Show how to inherit a behavior. This tutorial is inherited from tutorial 2. It adds a user adjustable
      *  counter value. When this count is completed, the behavior disables / enables the linkage (of tutorial 2's
      *  code) to the running tick. This behavior never unlinks / relinks to the running tick (except during
      *  construction and destruction).
      *
      *  This controlled access of tutorial 2's code to the running tick, causes the object to stop and start
      *  spinning. The object also continuously (i.e. not under counter control) changes it's scale. This makes it
      *  expand and contract. This new functionality is added by tutorial 8 itself.
      *
      *  \see CTutorial2
      * 
      */
      class CTutorial8 : public CTutorial2
      {
      public:
         RWS_MAKENEWCLASS(CTutorial8);
         RWS_DECLARE_CLASSID(CTutorial8);
         RWS_CATEGORY("Tutorial");
         RWS_DESCRIPTION("Tutorial8", "Inherited behavior (from CTutorial2)");

         RWS_BEGIN_COMMANDS
            RWS_ATTRIBUTE( CMD_counter_max, 
                           "Counter Limit", 
                           "Specify the time taken to change state", 
                           SLIDER, 
                           RwUInt32, 
                           RANGE(0, 30, 60))
         RWS_END_COMMANDS;
  
         CTutorial8(const CAttributePacket& attr);
         ~CTutorial8(void);

         virtual void HandleEvents(CMsg &pMsg);
         virtual void HandleAttributes(const CAttributePacket& attr); 

      protected:
         void Scale_Object(void);

         RwUInt32 m_counter;           /**< Counter to control delay for next state change. */
         RwUInt32 m_counter_max;       /**< Maximum delay to change state and turn on/ off rotation. */
         RwBool m_rotate_flag;         /**< Flag used to turn on/ off rotation. */

         RwReal m_scale;               /**< Current scaling of the asset. */
         RwBool m_scale_flag;          /**< Flag used to control scale direction. */
      };
   }//namespace Tutorial
}//namespace RWS
