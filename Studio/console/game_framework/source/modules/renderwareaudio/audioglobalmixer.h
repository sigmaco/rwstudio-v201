/*****************************************************************************
*
* File :    AudioGlobalMixer.h
*
* Abstract: Global interface behavior for audio toolkit. Sets up global
*           parameters such as output gain, Doppler scale, distance factor
*           roll-off factor and reverb type.
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
* Copyright (c) 2001 Criterion Software Ltd.
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
#include "framework/core/attributehandler/allocation policies/linearallocationpolicy.h"

namespace RWS
{
   namespace Audio
   {
      /**
      *
      * \ingroup Mod_Audio 
      *
      * Global mixer class for RenderWare Studio audio interface.  Allows setting 
      * of distance, roll-off factors, Doppler scale, and gain.  The position of the listener
      * is set from this behavior using an RwCamera passed via the render event.
      *
      * \li Render Camera Event: This is the camera that the listener will use for its position.
      *
      * \li Render Priority    : Sets the priority for the render camera event.
      *
      * \li Stop All Voices    : Zeros the gain for all the virtual voices registered with the output device.
      *
      * \li Fade Up Event      : Event to fade up the level of the output gain.
      *
      * \li Fade Down Event    : Event to fade down the level of the output gain.
      *
      * \li Fade End Message   : When a fading operation has completed, send out an event. This is so you can 
      *                          wait for a fade to finish in your game before performing other audio operations.
      *
      * \li Fade Step          : The increment step for the output voice fade.
      *
      * \li Gain               : Sets the output object gain.
      *
      * \li Doppler Scale      : Set the Doppler scale on the listener object. The Doppler scale 
      *                          is a scale of how much the Doppler effect is exaggerated. A large Doppler scale will 
      *                          cause a larger change in frequency in relation to the relative speed of a listener 
      *                          and a source. A smaller Doppler scale will cause a smaller change in frequency in 
      *                          relation to the relative speed of a listener to a source.
      *
      * \li Distance Factor    : Sets the 3d distance factor for the user's coordinate system.
      *
      * \li Roll-off Factor    : Sets the global 3d roll-off factor for each sound. A higher roll-off factor makes 
      *                          the sounds approach zero gain as they depart, quicker than if it was left at 1.0f.
      *
      * \li Speaker Config     : Change Speaker Configuration.
      *
      * \li Global Mixer Debug : If selected, display debug output in the framework.  You need to enable print in the 
      *                          CDebugTools behavior before debug info will be displayed.
      *
      * \see CAttributeHandler
      * \see CEventHandler
      * \see CSystemCommands
      *
      */
      class AudioGlobalMixer: public CSystemCommands, public CAttributeHandler, public CEventHandler, 
                              public LinearAllocationPolicy
      {
      public:
         RWS_MAKENEWCLASS(AudioGlobalMixer);
         RWS_DECLARE_CLASSID(AudioGlobalMixer);
         RWS_CATEGORY("Audio");
         RWS_DESCRIPTION("Global Audio Mixer", "Global audio mixer and listener.");

         AudioGlobalMixer(const CAttributePacket&);
         ~AudioGlobalMixer      ();
            
         RWS_BEGIN_COMMANDS
            RWS_MESSAGE  (CMD_In,
                         "Receive Render Camera Event",
                         "Updates the listener position.",
                         RECEIVE,
                         RwCamera*,
                         "iMsgDoRender")

            RWS_MESSAGE  (CMD_StopAll,
                         "Stop All Voices",
                         "Stop all voices from playing.",
                         RECEIVE,
                         0,
                         0)

            RWS_SEPARATOR("Global Voice Fade", 0)  
            RWS_MESSAGE  (CMD_FadeUp,
                         "Fade Voices Up",
                         "Fade all voices up to the level of the output gain.",
                         RECEIVE ,
                         0,
                         0)

            RWS_MESSAGE  (CMD_FadeDown,
                         "Fade Voices Down",
                         "Fade All voices down.",
                         RECEIVE,
                         0,
                         0)

            RWS_MESSAGE  (CMD_FadeEnd,
                         "Fade Ended Msg",
                         "Message sent out when a fade operation has completed.",
                         TRANSMIT,
                         0,
                         0)

            RWS_ATTRIBUTE(CMD_FadeStep,
                         "Fade Step",
                         "Specify the step for each fade increment.",
                         SLIDER,
                         RwReal,
                         RANGE(0.0, 0.03, 0.5))

            RWS_SEPARATOR("Audio Output Parameters", 0)  
            RWS_ATTRIBUTE(CMD_gain,
                         "Output Gain",
                         "Change Output gain",
                         SLIDER,    
                         RwReal, 
                         RANGE(0.0, 0.5, 1.0))

            RWS_ATTRIBUTE(CMD_dopplerScale,
                          "Change Doppler scale",
                          "Set the Doppler scale or Doppler factor on the Listener object. The Doppler scale is a " 
                          "scale of how much the Doppler effect is exaggerated. A large Doppler scale will cause " 
                          "larger change in frequency with relation to relative speed of a listener to a source. " 
                          "A smaller Doppler scale will cause smaller change in frequency with relation to relative "
                          "speed of a listener to a source.",
                          SLIDER,
                          RwReal,
                          RANGE(0.0, 1.0, 10.0))

            RWS_ATTRIBUTE(CMD_distanceFactor,
                          "Distance Factor",
                          "Sets the 3d distance factor for the user's coordinate system.",
                          SLIDER,
                          RwReal,
                          RANGE(0.0, 1.0, 100.0))

            RWS_ATTRIBUTE(CMD_rolloffFactor,
                          "Roll-off Factor",
                          "Sets the global 3d roll-off factor for each sound. A higher roll-off factor makes the "
                          "sounds approach zero gain as they depart, quicker than if it was left at 1.0f.",
                          SLIDER,
                          RwReal,
                          RANGE(0.1, 1.0, 10.0))

            RWS_ATTRIBUTE(CMD_speakerConfig,
                          "Speaker Configuration",
                          "Change Speaker Configuration",
                          LIST,
                          RwUInt32,
                          LIST("rwaSPEAKERCONFIG_STEREO|rwaSPEAKERCONFIG_MONO|rwaSPEAKERCONFIG_HEADPHONES| "
                               "rwaSPEAKERCONFIG_SURROUND"))
                
            RWS_SEPARATOR("Debugging", 0)  
            RWS_ATTRIBUTE(CMD_Debug,
                          "Mixer Debug Output",
                          "If selected, display debug output in target. You need to enable print in the "
                          "CDebugTools behavior before debug info will be displayed.",
                          BOOLEAN,
                          RwUInt32,
                          DEFAULT(0))                
            RWS_END_COMMANDS;           
            

         virtual void HandleAttributes(const CAttributePacket& attr);
         virtual void HandleEvents(CMsg &pMsg);

      private:
#ifdef RWS_DEBUGTOOLS
         void DisplayDebugInfo(void);
#else
         void DisplayDebugInfo(void){};
#endif

         void ProcessFade(void);
         void UpdateListener(const CMsg &pMsg);
         void SetSpeakerConfig(const RwUInt32 Config);

         enum Fade_State                     
         {
            FadeStopped = 0,
            FadeDown = 1,
            FadeUp = 2 
         };                               /**< State enumeration output device fading */

         RwReal       m_Gain;             /**< Output gain.*/
         RwReal       m_DopplerScale;     /**< Doppler scale.*/
         RwReal       m_DistanceFactor;   /**< Distance factor.*/
         RwReal       m_RollOffFactor;    /**< Rolloff factor.*/
         CEventId     m_RenderIn;         /**< Receive render event.*/
         RwBool       m_DebugInfo;        /**< Flag to display debugging info on target.*/
         RwReal       m_FadeInc;          /**< Step for each fade increment. */
         CEventId     m_FadeDown;         /**< Fade down event. */ 
         CEventId     m_FadeUp;           /**< Fade up event */ 
         Fade_State   m_FadeState;        /**< Fade state.*/
         CEventId     m_EndFade;          /**< Message sent out when fade has ended.*/
      };
   }
}
#endif
#endif
