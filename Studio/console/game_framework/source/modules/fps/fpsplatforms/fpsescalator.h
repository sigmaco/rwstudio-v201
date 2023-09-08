/*****************************************************************************
 *
 * File :     FPSEscalator.h
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
 * Copyright (c) 2000 Criterion Software Ltd.
 * All Rights Reserved.
 *
 * RenderWare is a trademark of Canon Inc.
 *
 *****************************************************************************/

#ifndef _FPSEscalator_H_
#define _FPSEscalator_H_

#include "framework/core/eventhandler/ceventhandler.h"
#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/toolkits/world/smartptrs/catomicptr.h"
#include "framework/toolkits/world/smartptrs/cclumpptr.h"
#include "framework/core/attributehandler/allocation policies/LinearAllocationPolicy.h"

namespace RWS
{
   namespace FPS
   {
      class EscalatorStep;

      /**
      *
      *  \ingroup Mod_FPS
      *
      *  This class implements an escalator behavior, a single atomic or clump is used to specify the geometry
      *  of the steps that make up the escalator, this is cloned multiple times to form the steps of the escalator.
      *  Attributes are provided which modify the lead in lead out region, the speed and the number of steps. An event
      *  
      *  is exposed which can be used to toggle the escalator between its idle and active state.
      *
      *  \see CAttributeHandler, CEventHandler, CSystemCommands
      */
      class FPSEscalator : public CSystemCommands, public CEventHandler, public CAttributeHandler, public LinearAllocationPolicy
      {
         public:
            RWS_MAKENEWCLASS(FPSEscalator);
            RWS_DECLARE_CLASSID(FPSEscalator);
            RWS_CATEGORY("FPS");
            RWS_DESCRIPTION("Escalator", "Constructs an escalator from a single 'step' atomic.");

            RWS_BEGIN_COMMANDS

              RWS_ATTRIBUTE( CMD_limitlow,   "Lead In Steps","Specifies the lower clamp value, determines the number of lead in steps bottom of escalator",SLIDER,RwReal,RANGE(0,0.1,1.0))
              RWS_ATTRIBUTE( CMD_limithig,   "Lead Out Steps","Specifies the higher clamp value, determines the number of lead out steps top of escalator",SLIDER,RwReal,RANGE(0,0.9,1.0))

              RWS_ATTRIBUTE( CMD_height_mod, "Step Height Modifier","Multiplies the height of the step by this factor to determine the height of the escalator",SLIDER,RwReal,RANGE(0,1,1.0f))
              RWS_ATTRIBUTE( CMD_width_mod,  "Step Width Modifier","Multiplies the width of the step by this factor to determine the height of the escalator",SLIDER,RwReal,RANGE(0,1,1.0f))

              RWS_ATTRIBUTE( CMD_velocity,   "Velocity","Determines the speed/direction of the escalator",SLIDER,RwReal,RANGE(-0.01,0.001,0.01))

              RWS_ATTRIBUTE( CMD_num_steps,  "Number Of Steps","Determines the number of steps in the escalator",SLIDER,RwUInt32,RANGE(0,20,100))

              RWS_MESSAGE( CMD_toggle,
                        "Start/Stop Event",
                        "Receive Event, start escalator",
                        RECEIVE,
                        0,
                        0)

            RWS_END_COMMANDS;

            virtual void HandleAttributes(const CAttributePacket& attr);
            virtual void HandleEvents(CMsg &pMsg);

            FPSEscalator(const CAttributePacket&);
            ~FPSEscalator(void);

         protected:

            void InitSteps(void);
            void UpdateSteps(void);

            CClumpPtr m_pClump;           /**< \see CClumpPtr clump used to contain escalator steps.*/

            EscalatorStep *m_pSteps;      /**< Pointer to an array of \see EscalatorStep.*/

            RwReal m_rLimitLow;           /**< Lower steps limit, determines the length of the lead in, lower steps.*/
            RwReal m_rLimitHig;           /**< Upper steps limit, determines the length of the lead out, upper steps.*/

            RwReal m_rHeightMod;          /**< Height modifier, modifies height of each step in escalator.*/
            RwReal m_rWidthMod;           /**< Width modifier, modifies width of each step in escalator.*/

            RwReal m_rHeight;             /**< Total height of escalator.*/
            RwReal m_rWidth;              /**< Total width of escalator.*/

            RwReal m_rVelocity;           /**< Speed/Direction of escalator.*/

            RwUInt32 m_uiNumSteps;        /**< Number of steps in escalator.*/

            RpAtomic *m_pAtomicTemplate;  /**< Pointer to atomic which is used as a template for the steps.*/

            CEventId m_ToggleEvent;       /**< Toggle escalator event.*/

            enum States
            {
               Idle,
               Active
            };

            States eState;                /**< Current state of the escalator, either idle or active.*/
      };
   }
}

#endif