/*****************************************************************************
*
* File :       Audioreverb.h
*
* Abstract :   Environment object for RenderWare Audio output object.
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
#ifndef __AUDIOMIXER_H__
#define __AUDIOMIXER_H__

#ifdef WITH_AUDIO

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/core/eventhandler/ceventhandler.h"
#include "framework/toolkits/world/smartptrs/catomicptr.h"
#include "framework/mainloop/logic.h"
#include "framework/core/attributehandler/allocation policies/linearallocationpolicy.h"

namespace RWS
{
   namespace Audio
   {
      /**
      *
      * \ingroup Mod_Audio 
      *
      * This behavior sets up a level's environment parameters. Environment effects simulate the reflections and 
      * echos caused by a real environment.
      *
      * \li Trigger Environment : Event to enable this behaviors environment settings.
      *
      * \li Stop Environment    : Event to disable this behaviors environment settings.
      *
      * \li Environment Type    : Select one of the default environment types.
      *
      * \li Environment Gain    : Change the gain of the environment. This modifies the room setting in the 
      *                           I3DL2 structure. See the RenderWare Audio Api reference for a description 
      *                           of the RwaEnvironment structure.
      *
      * \li Fade Environment Up : Event to fade up the environment gain.
      *
      * \li Fade Environment Down: Event to fade down the environment gain.
      *
      * \li Fade End Message    : When a fade operation has completed, send out an event. This is so you can 
      *                           wait for a fade to finish in your game before performing other audio operations.
      *
      * \li Fade Step           : The increment step for the environment fade.
      *
      * \li Override Defaults    : Override default reverb types and allow you to create your own using I3DL2
      *                           environment parameters. 
      *
      * \li decayHFRatio        : Decay high frequency ratio. Sets environment ratio of high frequency decay 
      *                           time relative to the environment decay time parameter. This controls the spectral
      *                           quality of the late reverberation decay.
      *
      * \li decayTime           : Decay Time.
      *
      * \li density             : Modal density of the late reverberation decay.
      *
      * \li diffusion           : Diffusion of the late reverberation decay. This controls the rate at which discrete 
      *                           echos in the late reverberation decay into diffuse reverberation. With a diffusion 
      *                           of 0.0f, late reverberation sounds like a series of distinct echoes.
      *
      * \li hfReference         : The frequency at which all high frequency attenuation effects apply. roomHF and 
      *                           decayHFRatio refer to this frequency.
      *
      * \li reflections         : The reflections level controls the attenuation of early reflections relative 
      *                           environment level.
      *
      * \li reflectionsDelay    : Reflections delay time.
      *
      * \li reverb              : Reverberation level. The reverberation level controls the attenuation of subsequent
      *                           reverberation relative to the environment level.
      *
      * \li reverbDelay         : Reverb delay time.
      *
      * \li roomHF              : High frequency room level.
      *
      * \li roomRolloffFactor   : Room roll-off factor. The roll off factor is an attenuation factor applied to reverb 
      *                           effects with distance to exaggerate or reduce them.
      *
      * \li Environment Debug Output : If selected, display debug output in target. You need to enable print in the 
      *                                CDebugTools behavior before debug info will be displayed.
      *
      * \see CAttributeHandler
      * \see CEventHandler
      * \see CSystemCommands
      *
      */
      class AudioReverb : public CSystemCommands, public CAttributeHandler , public CEventHandler, 
                          public LinearAllocationPolicy
      {
      public:
         RWS_MAKENEWCLASS(AudioReverb);
         RWS_DECLARE_CLASSID(AudioReverb);
         RWS_CATEGORY("Audio");
         RWS_DESCRIPTION("AudioReverb", "Global reverberation");

         RWS_BEGIN_COMMANDS   

            RWS_SEPARATOR("Reverb Triggers", 0)  
            RWS_MESSAGE  (CMD_TriggerEnvironment,
                          "Trigger environment",
                          "Start environment effect for the game level.",
                          RECEIVE,
                          0,
                          0)

            RWS_MESSAGE  (CMD_StopEnvironment,
                          "Stop environment",
                          "Stop environment effect for the game level.",
                          RECEIVE,
                          0,
                          0)

             RWS_SEPARATOR("Reverb Parameters", 0)  
             RWS_ATTRIBUTE( CMD_environmentType,
                            "Environment type",
                            "Change environment type from RenderWare Audio defaults.",
                            LIST,
                            RwUInt32,
                            LIST("OFF|GENERIC|PADDEDCELL|ROOM|BATHROOM|LIVINGROOM|STONE ROOM|AUDITORIUM|CONCERT HALL "
                            "|CAVE|ARENA|HANGAR|CARPETED HALLWAY|HALLWAY|STONE CORRIDOR|ALLEY|FOREST|CITY|MOUNTAINS| "
                            "QUARRY|PLAIN|PARKING LOT|SEWER PIPE|UNDERWATER")) 


             RWS_ATTRIBUTE( CMD_environmentGain,
                            "Environment gain",
                            "Change environment gain",
                            SLIDER,
                            RwInt32,
                            RANGE(-1000, -1000, 0))

             RWS_SEPARATOR("Environment Fade", 0)  
             RWS_MESSAGE  (CMD_FadeUp,
                          "Fade environment up",
                          "Fade environment up to current gain.",
                          RECEIVE ,
                          0,
                          0)

             RWS_MESSAGE  (CMD_FadeDown,
                          "Fade environment down",
                          "Fade environment down.",
                          RECEIVE,
                          0,
                          0)

             RWS_MESSAGE  (CMD_FadeEnd,
                          "Fade end message",
                          "Message sent out when a fade operation has completed.",
                          TRANSMIT,
                          0,
                          0)

            RWS_ATTRIBUTE(CMD_FadeStep,
                          "Fade step",
                          "Specify the step for each fade increment.",
                          SLIDER,
                          RwUInt32,
                          RANGE(0, 10, 100))

             RWS_SEPARATOR("I3DL2 Environment Parameters", 0)  
             RWS_ATTRIBUTE( CMD_Override,  
                            "Override defaults",  
                            "Override default environment types and allow you to create your own using I3DL2 "
                            "environment parameters.", 
                            BOOLEAN, 
                            RwUInt32,   
                            DEFAULT(0))                


             RWS_ATTRIBUTE( CMD_decayHFRatio, 
                            "Decay HF Ratio", 
                            "Decay high frequency ratio. Sets ratio of high frequency decay time relative to "
                            "the reverb decay time parameter. This controls the spectral quality of the "
                            "late reverberation decay.",
                            SLIDER,
                            RwReal,
                            RANGE(0.1, 0.5, 2.0))            


             RWS_ATTRIBUTE( CMD_decayTime , 
                            "Decay Time",
                            "Decay time.",
                            SLIDER,
                            RwReal,
                            RANGE(0.1, 1.0, 20.0))


             RWS_ATTRIBUTE( CMD_density , 
                            "Density",
                            "Modal density of the late reverberation decay.",
                            SLIDER,
                            RwReal,
                            RANGE(0.0, 100.0, 100.0))


             RWS_ATTRIBUTE( CMD_diffusion , 
                            "Diffusion",
                            "Diffusion of the late reverberation decay. This controls the rate at which "
                            "discrete echos in the late reverberation decay into diffuse reverberation. "
                            "With a diffusion of 0.0f, late reverberation sounds like a series of distinct echoes.",
                            SLIDER,
                            RwReal,
                            RANGE(0.0, 100.0, 100.0))


             RWS_ATTRIBUTE( CMD_hfReference , 
                            "High Frequency Reference ",
                            "The frequency at which all high frequency attenuation effects apply. roomHF "
                            "and decayHFRatio refer to this frequency.",
                            SLIDER,
                            RwReal,
                            RANGE(20.0, 5000.0, 20000.0))


             RWS_ATTRIBUTE( CMD_reflections, 
                            "Reflections ",
                            "The reflections level controls the attenuation of early reflections relative "
                            "environment level.",
                            SLIDER,
                            RwInt32,
                            RANGE(-10000, -10000, 1000))


             RWS_ATTRIBUTE( CMD_reflectionsDelay , 
                            "Reflection Delay",
                            "Reflection's delay time.",
                            SLIDER,
                            RwReal,
                            RANGE(0.0, 0.002, 0.3))

             RWS_ATTRIBUTE( CMD_reverb , 
                            "Reverb",
                            "Reverberation level. The reverberation level controls the attenuation of subsequent "
                            "reverberation relative to the environment level.",
                            SLIDER,
                            RwInt32,
                            RANGE(-10000, -10000, 2000))


             RWS_ATTRIBUTE( CMD_reverbDelay, 
                            "Reverb Delay",
                            "Reverb delay time.",
                            SLIDER,
                            RwReal,
                            RANGE(0.0, 0.004, 0.1))


             RWS_ATTRIBUTE( CMD_roomHF  , 
                            "Room HF",
                            "High frequency room level.",
                            SLIDER,
                            RwInt32,
                            RANGE(-10000, 0, 0))


             RWS_ATTRIBUTE( CMD_roomRolloffFactor , 
                            "Room Roll-off Factor  ",
                            "Room roll-off factor The roll off factor is an attenuation factor applied to reverb "
                            "effects with distance to exaggerate or reduce them.",
                            SLIDER,
                            RwReal,
                            RANGE(0.0, 0.0, 10.0))


            RWS_SEPARATOR("Debugging", 0)  
            RWS_ATTRIBUTE( CMD_Debug,  
                           "Environment Debug Output",
                           "If selected, display debug output in target. You need to enable print in the CDebugTools "
                           "behavior before debug info will be displayed.", 
                           BOOLEAN, 
                           RwBool,   
                           DEFAULT(0))                

         RWS_END_COMMANDS;    

         AudioReverb(const CAttributePacket& attr);
         ~AudioReverb(void);

         virtual void HandleEvents(CMsg &pMsg);
         virtual void HandleAttributes(const CAttributePacket& attr);

      private:
         void ApplyEnvironment(void);
         void ProcessFade(void);

#ifdef RWS_DEBUGTOOLS
         void DisplayDebugInfo(void);
#else
         void DisplayDebugInfo(void){};
#endif

         void StartEnvironment(void);
         void OverrideEnvironment(void);
         void UpdateEnvironmentGain(void);
         void SetEnvironment(const RwUInt32 Rev, const RwUInt32 Gain);
       
         enum Fade_State                     
         {
            FadeStopped = 0,
            FadeDown = 1,
            FadeUp = 2 
         };                                  /**< State enumeration for reverb device fading.*/

         RwUInt32        m_EnvironmentType;  /**< Reverb Type.*/
         RwaEnvironment  m_Environment;      /**< Holds parameters for overidden I3DL2 reverbs.*/
         RwBool          m_Override;          /**< Flag to override the default reverb types.  0 = Use defaults types, 
                                                  1 = Override and create your own*/
         RwBool          m_DebugInfo;        /**< Flag to display debugging information on target.*/
         CEventId        m_StartEnvironment; /**< Event to start a reverb effect.*/ 
         CEventId        m_StopEnvironment;  /**< Event to stop a reverb effect.*/ 
         RwBool          m_Active;           /**< Flag to determine if the reverb is active.*/
         RwUInt32        m_FadeInc;          /**< Step for each fade increment.*/
         CEventId        m_FadeDown;         /**< Fade down event.*/ 
         CEventId        m_FadeUp;           /**< Fade up event */ 
         Fade_State      m_FadeState;        /**< Fade state.*/
         CEventId        m_EndFade;          /**< Message sent out when fade has ended.*/
         RwInt32         m_FadeUpDest;       /**< This value is the destination gain when fading up.*/
      };
   }
}

#endif
#endif
