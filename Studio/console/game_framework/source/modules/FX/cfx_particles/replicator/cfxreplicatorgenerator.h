/*****************************************************************************
 *
 * File :     CFXReplicatorGenerator.h
 *
 * Abstract : General purpose particle entity
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

#ifndef __CReplicatorGENERATOR_H__
#define __CReplicatorGENERATOR_H__

#include "rwcore.h"
#ifdef _MSC_VER
#pragma comment (lib, "rwcore.lib")
#endif

#include "cfxreplicator.h"
#include "framework/core/eventhandler/ceventhandler.h"
#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/toolkits/world/smartptrs/catomicptr.h"
#include "framework/mainloop/logic.h"

#include "framework/core/attributehandler/allocation policies/LinearAllocationPolicy.h"

namespace RWS
{
   namespace FX
   {
      class CFXReplicatorGenerator : public CSystemCommands, public CAttributeHandler, public CEventHandler, public LinearAllocationPolicy
      {
      public:
         RWS_MAKENEWCLASS(CFXReplicatorGenerator);
         RWS_DECLARE_CLASSID(CFXReplicatorGenerator);
         RWS_CATEGORY("FX");
         RWS_DESCRIPTION("Atomic Replicator", "Using an atomic as a template, creates clones of the atomic and throws them outwards.");

   
         virtual void HandleEvents(CMsg &pMsg);
         virtual void HandleAttributes(const CAttributePacket& attr); 

         RWS_BEGIN_COMMANDS

            RWS_SEPARATOR("Messages", 1)

            RWS_MESSAGE  (CMD_Start_Render, "Start Render Event", "Begin Rendering Of any atomics created by this replicator generator.", RECEIVE , RwCamera*,"iMsgDoRender")

            RWS_ATTRIBUTE(CMD_priority,  "Render Priority",   "Specify the render priority",   SLIDER,  RwUInt32,   RANGE(0, 32768, 65535))

            RWS_MESSAGE( CMD_Set_m_Msg_Rx_Trigger_Particles,
                     "Trigger particles, Set Position",
                     "When this is received a counter is incremented when this reaches (messages to trigger) then (number of particles) are created.",
                     RECEIVE,
                     RwFrame*,
                     0)

            RWS_MESSAGE( CMD_Set_m_Msg_Rx_Alt_Trigger_Particles,
                     "Trigger particles",
                     "When this is received a counter is incremented when this reaches (messages to trigger) then (number of particles) are created.",
                     RECEIVE,
                     0,
                     0)

            RWS_ATTRIBUTE(  CMD_Set_m_number_of_particles_to_create,
                        "Number of particles to create",
                        "Number of particles to create each time the generator is triggered",
                        SLIDER,
                        RwUInt32,
                        RANGE(0, 10, 100))

            RWS_ATTRIBUTE(  CMD_Set_m_total_number_of_particles_to_create,
                        "Total Number of particles",
                        "Total Number of particles to create, particle generator is deleted when this number is reached, note 0 implies no limit",
                        SLIDER,
                        RwUInt32,
                        RANGE(0, 0, 1000))

            RWS_ATTRIBUTE(  CMD_Set_m_number_of_message_to_trigger,
                        "Messages required to trigger",
                        "Number of messages required to trigger generation of particles",
                        SLIDER,
                        RwUInt32,
                        RANGE(0, 1, 3600))

            RWS_ATTRIBUTE(  CMD_Set_m_ExpireTime,
                        "Random Expire time",
                        "Number of frames that each particle remains active until it is deleted (total time = random time + bias time)",
                        SLIDER,
                        RwUInt32,
                        RANGE(1, 120, 3600))

            RWS_ATTRIBUTE(  CMD_Set_m_ExpireTime_Bias,
                        "Expire time Bias",
                        "Number of frames that each particle remains active until it is deleted (total time = random time + bias time)",
                        SLIDER,
                        RwUInt32,
                        RANGE(0, 60, 3600))

            RWS_SEPARATOR("Initial Velocity", 0)
            RWS_ATTRIBUTE(CMD_Set_m_initial_velocity,          "Velocity (Random +- 0.5 *var)",         "velocity = Velocity Random + Velocity Bias",   VECTOR,  RwV3d,   RANGES((-8, 4, 8), (-8, 4, 8), (-8, 4, 8)))
            RWS_ATTRIBUTE(CMD_Set_m_initial_velocity_bias,     "Velocity bias",         "velocity = Velocity Random + Velocity Bias",   VECTOR,  RwV3d,   RANGES((-8, 0, 8), (-8, 0, 8), (-8, 0, 8)))
            RWS_ATTRIBUTE(CMD_Set_m_initial_angvelocity,       "Angular velocity (Random +- 0.5 *var)", "velocity = Velocity Random + Velocity Bias",   VECTOR,  RwV3d,   RANGES((-8, 3, 8), (-8, 3, 8), (-8, 3, 8)))
            RWS_ATTRIBUTE(CMD_Set_m_initial_angvelocity_bias,  "Angular velocity bias", "velocity = Velocity Random + Velocity Bias",   VECTOR,  RwV3d,   RANGES((-8,0,8), (-8,0,8), (-8,0,8)))

            RWS_SEPARATOR("Acceleration/Friction", 0)
            RWS_ATTRIBUTE(CMD_Set_m_initial_acceleration,      "Acceleration", "velocity = velocity + acceleration",   VECTOR,  RwV3d,   RANGES((-1, 0, 1), (-1, -0.01, 1), (-1, 0, 1)))
            RWS_ATTRIBUTE(CMD_Set_m_initial_friction,          "Friction",     "velocity = velocity * friction",   VECTOR,  RwV3d,   RANGES((0, 0.98, 2), (0, 0.98, 2), (0, 0.98, 2)))
            RWS_ATTRIBUTE(CMD_Set_m_initial_angacceleration,   "Angular acceleration", "velocity = velocity + acceleration",   VECTOR,  RwV3d,   RANGES((-1, 0, 1), (-1, -0.01, 1), (-1, 0, 1)))
            RWS_ATTRIBUTE(CMD_Set_m_initial_angfriction,       "Angular friction",     "velocity = velocity * friction",   VECTOR,  RwV3d,   RANGES((0, 0.98, 2), (0, 0.98, 2), (0, 0.98, 2)))

            RWS_SEPARATOR("Scale", 0)

            RWS_ATTRIBUTE(CMD_Set_iScale,           "Initial Scale value",    "Scale applied when Particle is created.",   VECTOR,  RwV3d,   RANGES((-20, 1, 20), (-20, 1, 20), (-20, 1, 20)))

            RWS_ATTRIBUTE(CMD_Set_vScale,           "Scale value",    "Scale applied to particle each frame.",   VECTOR,  RwV3d,   RANGES((0.9, 1, 1.1), (0.9, 1, 1.1), (0.9, 1, 1.1)))

            RWS_SEPARATOR("Particle Messages", 0)
            RWS_MESSAGE( CMD_Set_m_Msg_Particle_Expired,
                     "Particle expire",
                     "Sent by the particle when it expires.",
                     TRANSMIT ,
                     RwFrame*,
                     0)

            RWS_MESSAGE( CMD_Set_m_Msg_Particle_Active,
                     "Particle active",
                     "Sent when a particle is active, each time the particle is moved i.e. each frame",
                     TRANSMIT ,
                     RwFrame*,
                     0)


            RWS_ATTRIBUTE(CMD_m_modify_paths,           "Modify particle paths based on, trigger objects orientation",    "",   BOOLEAN,  RwUInt32,DEFAULT(0))

            RWS_SEPARATOR("Color", 0)

            RWS_ATTRIBUTE(CMD_Set_c0, "Color at time = 0.0",  "Color spline c0,c1,c2,c3,c4 over lifetime of particle",   COLOR,  RwRGBA,   DEFAULT(4294967295))
            RWS_ATTRIBUTE(CMD_Set_c1, "Color at time = 0.25", "Color spline c0,c1,c2,c3,c4 over lifetime of particle",   COLOR,  RwRGBA,   DEFAULT(4294967295))
            RWS_ATTRIBUTE(CMD_Set_c2, "Color at time = 0.5",  "Color spline c0,c1,c2,c3,c4 over lifetime of particle",   COLOR,  RwRGBA,   DEFAULT(4294967295))
            RWS_ATTRIBUTE(CMD_Set_c3, "Color at time = 0.75", "Color spline c0,c1,c2,c3,c4 over lifetime of particle",   COLOR,  RwRGBA,   DEFAULT(4294967295))
            RWS_ATTRIBUTE(CMD_Set_c4, "Color at time = 1.0",  "Color spline c0,c1,c2,c3,c4 over lifetime of particle",   COLOR,  RwRGBA,   DEFAULT(4294967295))

            // Render state flags
            RWS_SEPARATOR("Render state flags", 0)

            RWS_ATTRIBUTE(CMD_rwRENDERSTATECULLMODE, "rwRENDERSTATECULLMODE", "", LIST, RwUInt32, LIST("rwCULLMODECULLNONE|@rwCULLMODECULLBACK|rwCULLMODECULLFRONT"))
            RWS_ATTRIBUTE(CMD_rwRENDERSTATESRCBLEND,  "rwRENDERSTATESRCBLEND","", LIST, RwUInt32, LIST("rwBLENDZERO|rwBLENDONE|rwBLENDSRCCOLOR|rwBLENDINVSRCCOLOR|@rwBLENDSRCALPHA|rwBLENDINVSRCALPHA|rwBLENDDESTALPHA|rwBLENDINVDESTALPHA|rwBLENDDESTCOLOR|rwBLENDINVDESTCOLOR|rwBLENDSRCALPHASAT"))
            RWS_ATTRIBUTE(CMD_rwRENDERSTATEDESTBLEND, "rwRENDERSTATEDESTBLEND", "", LIST, RwUInt32, LIST("rwBLENDZERO|rwBLENDONE|rwBLENDSRCCOLOR|rwBLENDINVSRCCOLOR|rwBLENDSRCALPHA|@rwBLENDINVSRCALPHA|rwBLENDDESTALPHA|rwBLENDINVDESTALPHA|rwBLENDDESTCOLOR|rwBLENDINVDESTCOLOR|rwBLENDSRCALPHASAT"))
            RWS_ATTRIBUTE(CMD_rwRENDERSTATEVERTEXALPHAENABLE, "rwRENDERSTATEVERTEXALPHAENABLE", "", BOOLEAN, RwUInt32, DEFAULT(0))
            RWS_ATTRIBUTE(CMD_rwRENDERSTATEZWRITEENABLE, "rwRENDERSTATEZWRITEENABLE", "",BOOLEAN, RwUInt32, DEFAULT(1))
            RWS_ATTRIBUTE(CMD_rwRENDERSTATEZTESTENABLE,  "rwRENDERSTATEZTESTENABLE", "",BOOLEAN, RwUInt32, DEFAULT(1))

         RWS_END_COMMANDS;

         CFXReplicatorGenerator(const CAttributePacket&);
         ~CFXReplicatorGenerator();

         RwBlendFunction Map_AttributeListToBlendMode(RwUInt32 var) const;
         void TriggerParticles ( CMsg &pMsg );

      private:
         static RpAtomic* RemoveAtomic(RpAtomic* pAtomic, void *data);

         static void ClumpRender(RpClump *pClump);

         CFXReplicatorDynamics m_ParticleDynamics;         /**< Particle Dynamics vars */
         RwUInt32 m_number_of_particles_to_create;         /**< Number of particles to create each time the generator is triggered */
         RwUInt32 m_total_number_of_particles_to_create;   /**< Total number of particles to generate, 0 implies imfinite*/
         CEventId m_Msg_Rx_Trigger_Particles;              /**<  Message used to control particle generators */
         CEventId m_Msg_Rx_Alt_Trigger_Particles;         
         RwUInt32 m_number_of_message_to_trigger;          /**<  Number of messages (m_Msg_Rx_Trigger_Particles) to receive in order to trigger particle effect*/
         RwUInt32 m_ExpireTime;                            /**< Expire time of particles (Random) */   
         RwUInt32 m_ExpireTime_Bias;                       /**<  Expire time of particles*/
         RwV3d m_initial_velocity;                         /**< Initial velocity's random */
         RwV3d m_initial_angvelocity;
         RwV3d m_initial_velocity_bias;                    /**< Initial velocity's bias  */
         RwV3d m_initial_angvelocity_bias;
         RwUInt32 m_message_count;                         /**< Current number of messages that have been received, reset when count reaches m_number_of_message_to_trigger */
         RwUInt32 m_total_number_of_particles;             /**< Total number of particles generated */
         RwUInt32 m_modify_paths;                          /**< Modify particle paths based on orientation of object generating event */
      
         CAtomicPtr m_pAtomic;                             /**< behaviors atomic */

         // The event below is used to start rendering the clump
         //
         CEventId m_StartRender;

         RwUInt32 m_link_priority;                        /**< Current Link Priority, Used to determine rendering order */


         // The clump below is used to render all the atomics generated by this replicator generator
         //
         CClumpPtr m_pClump;                               /**< This clump is used to render all the atomics */

         // Render State is the same for all atomics generated by this replicator generator, therefore when the 
         // clump above is rendered use these.
         //
         RwCullMode m_rwRENDERSTATECULLMODE;              /**< see RenderWare help, rwRENDERSTATECULLMODE */
         RwBlendFunction m_rwRENDERSTATESRCBLEND;         /**< see Renderware help, rwRENDERSTATESRCBLEND */
         RwBlendFunction m_rwRENDERSTATEDESTBLEND;        /**< see RenderWare help, rwRENDERSTATEDESTBLEND */
         RwUInt32 m_rwRENDERSTATEVERTEXALPHAENABLE;       /**< see Renderware help, rwRENDERSTATEVERTEXALPHAENABLE */
         RwUInt32 m_rwRENDERSTATEZWRITEENABLE;            /**< see RenderWare help, rwRENDERSTATEZWRITEENABLE */
         RwUInt32 m_rwRENDERSTATEZTESTENABLE;             /**< see RenderWare help, rwRENDERSTATEZTESTENABLE */
      };       
   }//namespace
}

#endif