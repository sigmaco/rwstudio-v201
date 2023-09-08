/*****************************************************************************
*
* File :     AudioSound3D.h
*
* Abstract:  3D audio object.
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

#ifndef __AUDIOSOUND3D_H__
#define __AUDIOSOUND3D_H__

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
      * \ingroup Mod_Audio 
      *
      * This behavior lets you play a sound from a dictionary that has been assigned to a virtual voice. By using
      * this interface on to the RenderWare Audio api, the games designer can control sounds by using the event
      * system. So for example if a player fires a gun, an event can be sent to an instance of the AudioSound3d 
      * behavior to start a bang sound.
      *
      * \li Play Sound Event  : This event starts a sound playing. You can send an RwFrame* with this event so 
      *                         that the correct position is applied to the sound. By default everytime you send 
      *                         the play event the sound will restart, but you can set the 'No Restart' flag below, 
      *                         so that the sound will not restart until the current sound has finished playing.
      *
      * \li Stop Sound Event  : This event stops a sound from playing.
      *
      * \li End target        : Event sent when sound finishes playing.
      *
      * \li Select sound      : Select sound from dictionary.
      *
      * \li No restart on play: When selected, do not restart the sound when the play button is pressed until the 
      *                         sound has stopped.  Even though it won't restart the sound it will still update the 
      *                         RwFrame position. If you have attached the sound to an object that is moving around 
      *                         the level, you can update its position in the sound engine by continually sending the 
      *                         play event.
      *
      * \li Loop              : Set whether the sound loops or not.
      *
      * \li Set voice gain    : Set voice gain.
      *
      * \li Set voice pan     : Set the pan parameter of a VirtualVoice. A pan value of -1.0 sets the VirtualVoice 
      *                         to full volume in the left stereo channel and zero volume in the right stereo channel. 
      *                         A pan of 1.0 set the VirtualVoice to full volume in the right stereo channel and zero 
      *                         volume in the left stereo channel. A pan of 0.0 sets the VirtualVoice to full volume 
      *                         in both channels. Where the full volume is determined from the gain of the 
      *                         VirtualVoice. Setting the pan parameter will have no effect on 3D voices.
      *
      * \li Voice priority    : Set priority for virtual voice.
      *
      * \li Min distance      : This function sets the minimum distance in 3d space, between the virtual voice and 
      *                         the listener before the sound starts to attenuate.
      *
      * \li Max distance      : Sets the maximum distance of a virtual voice. It is the distance the sound should 
      *                         be away from the listener before it is cut off.
      *
      * \li Voice bias        : Set the bias of the virtual voice object. A bias value of 0.0 makes audibility 
      *                         of a virtual voice more important to the voice manager when assigning real 
      *                         voices to virtual voice. Where a bias value of 1.0 makes the priority of a 
      *                         virtual voice more important.
      *
      * \li Rate override      : Use preset sample rates,  e.g., 11025, 22050, 32000, 44100, 48000. 
      *
      * \li Debug output      : If selected, display debug output in target. You need to enable print in the 
      *                         CDebugTools behavior before debug info will be displayed.
      *
      * \see CAttributeHandler
      * \see CEventHandler
      * \see CSystemCommands
      * \see AudioSound3DInterface
      *
      */
      class AudioSound3D: public AudioSound3DInterface, public CEventHandler
      {
      public:
         RWS_MAKENEWCLASS(AudioSound3D);
         RWS_DECLARE_CLASSID(AudioSound3D);
         RWS_CATEGORY("Audio");
         RWS_DESCRIPTION("3D Sound Player", "3D sound interface.");

         AudioSound3D(const CAttributePacket&);
         ~AudioSound3D();
            
         RWS_BEGIN_COMMANDS
             RWS_SEPARATOR("Play/Stop Events", 1)  
             RWS_MESSAGE( CMD_m_play,  
                          "Play sound",
                          "Play a sound. This event can be used to update the position of a sound in the level.",
                          RECEIVE,
                          RwFrame*, 0)

             RWS_MESSAGE( CMD_m_stop,
                          "Stop sound",
                          "Stop a sound from playing.",
                          RECEIVE,
                          0,
                          0)

             RWS_MESSAGE( CMD_m_endtarget,
                          "End target",
                          "Event sent when sound finishes",
                          TRANSMIT,
                          0,
                          0)

             RWS_ATTRIBUTE( CMD_selectText,
                            "Select sound",
                            "Select sound from dictionary.",
                            EDIT,
                            RwChar,
                            DEFAULT("Enter Text Here ..."))

             RWS_ATTRIBUTE( CMD_Voice_Restart,
                            "Restart voice on play",
                            "If selected, restart the sound when play button is pressed, otherwise wait for the sound "
                            "to finish playing before restarting.",
                            BOOLEAN,
                            RwUInt32,
                            DEFAULT(0))                


             RWS_SEPARATOR("Voice Fade", 1)
             RWS_MESSAGE  (CMD_FadeUp,
                           "Fade voice up",
                           "Fade up to level of current voice gain.",
                           RECEIVE ,
                           0,
                           0)

             RWS_MESSAGE  (CMD_FadeDown,
                           "Fade voice down",
                           "Fade voice down.",
                           RECEIVE,
                           0,
                           0)

             RWS_MESSAGE  (CMD_FadeEnd,
                           "Fade ended Message",
                           "Message sent out when a fade operation has completed.",
                           TRANSMIT,
                           0,
                           0)

             RWS_ATTRIBUTE(CMD_FadeStep,
                           "Fade step",
                           "Specify the step for each fade increment.",
                           SLIDER,
                           RwReal,
                           RANGE(0.0, 0.03, 0.5))

             RWS_SEPARATOR("Sound controls", 1)  
             RWS_ATTRIBUTE( CMD_looped,
                            "Loop",
                            "Set whether sound loops or not.",
                            LIST,
                            RwUInt32,
                            LIST("False|True"))

             RWS_ATTRIBUTE( CMD_gain,
                            "Set voice gain",
                            "Set voice gain.",
                            SLIDER,
                            RwReal,
                            RANGE(0.0, 0.3, 1.0))

             RWS_ATTRIBUTE( CMD_pan,
                            "Set voice pan",
                            "Set the pan parameter of a VirtualVoice. A pan value of -1.0 sets the VirtualVoice to "
                            "full volume in the left stereo channel and zero volume in the right stereo channel. "
                            "A pan of 1.0 set the VirtualVoice to full volume in the right stereo channel and zero "
                            "volume in the left stereo channel. A pan of 0.0 sets the VirtualVoice to full volume "
                            "in both channels. Where the full volume is determined from the gain of the "
                            "VirtualVoice. Setting the pan parameter will have no effect on 3D voices.",
                            SLIDER, 
                            RwReal,
                            RANGE(-1.0, 0.0, 1.0))

             RWS_ATTRIBUTE( CMD_Priority,
                            "Select voice priority",
                            "Set priority for virtual voice",
                            SLIDER, RwReal, 
                            RANGE(0.0, 0.5, 1.0))

             RWS_ATTRIBUTE( CMD_Bias, 
                            "Set Voice Bias",
                            "Set the bias of the virtual voice object. A bias value of 0.0 makes audibility of "
                            "a virtual voice more important to the voice manager when assigning real voices to "
                            "virtual voice. Where a bias value of 1.0 makes the priority of a virtual voice "
                            "more important.",
                            SLIDER, 
                            RwReal, 
                            RANGE(0.0, 0.0, 1.0))

             RWS_SEPARATOR("Min/Max voice attenuation", 1)  
             RWS_ATTRIBUTE( CMD_Min_Atten,
                            "Select min distance" ,
                            "This function sets the minimum distance in 3d space, between the virtual voice and "
                            "the listener before the sound starts to attenuate.",   
                            SLIDER, 
                            RwReal, 
                            RANGE(0.0, 1.0, 1000.0))
                               
             RWS_ATTRIBUTE( CMD_Max_Atten, 
                            "Select max distance",  
                            "Sets the 'maximum distance' of a virtual voice. It is the distance the sound should "
                            "be away before it is cut off.",   
                            SLIDER, 
                            RwReal, 
                            RANGE(0.0, 10000.0, 20000.0))

             RWS_SEPARATOR("Sample Rate", 1)  

             RWS_ATTRIBUTE( CMD_sample,  
                            "Select sample rate",
                            "Select preset sample rate",
                            LIST,    
                            RwUInt32, 
                            LIST("11025|22050|32000|44100|48000")) 

             RWS_SEPARATOR("Debugging", 0)  
             RWS_ATTRIBUTE( CMD_Debug,  
                            "3D sound debug output",
                            "If selected, display debug output in target. You need to enable print in the CDebugTools "
                            "behavior before debug info will be displayed.", 
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
         void TriggerGroupFadeUp(const CMsg &pMsg);
         void TriggerGroupFadeDown(const CMsg &pMsg);
         void TriggerGroupVoiceDisable(const CMsg &pMsg);
         void TriggerGroupVoiceEnable(const CMsg &pMsg);

         void SelectWave(const RwChar* const pWavName);
         void Play(const CMsg &pMsg);

         void HandleDictionaryDelete(const CMsg &pMsg);

         RwReal SelectSampleRate(const RwUInt32 Index);

         void ProcessFade(void);

         enum Fade_State                     
         {
            FadeStopped = 0,
            FadeDown = 1,
            FadeUp = 2 
         };                                  /**< State enumeration output device fading.*/

         CEventId    m_Play;                 /**< Event to start a sound playing.*/ 
         CEventId    m_Stop;                 /**< Event to stop a sound from playing.*/ 
         CEventId    m_EndTarget;            /**< Event sent when sound finishes.*/  
         RwReal      m_DestGain;             /**< Desination value for fading.*/
         RwReal      m_SampleRate;           /**< Sample rate of virtual voice.*/
         RwBool      m_DebugInfo;            /**< Flag to display debugging info on target.*/
         RwBool      m_RestartVoice;         /**< If selected, do not restart the sound when play button is 
                                                  pressed until sound has stopped.*/
         RwReal      m_FadeInc;              /**< Step for each fade increment.*/
         CEventId    m_FadeDown;             /**< Fade down event.*/ 
         CEventId    m_FadeUp;               /**< Fade up event.*/ 
         Fade_State  m_FadeState;            /**< Fade state.*/
         CEventId    m_EndFade;              /**< Message sent out when fade has ended.*/
         RwInt32     m_DictIndex;            /**< Stores the index of the dictionary to which this behaviors 
                                                  selected sound comes from.*/
         RwBool      m_GroupFadeNotify;      /**< If the fade event had been received from a wave dictionary 
                                                  group, then we do not want to send out an 'End' message 
                                                  for each voice that is affected.*/ 
         RwBool      m_EndEvent;             /**< Flag used to stop behavior sending out the end event all the time.  
                                                  Only sends out the event when the sound first stops playing.*/
      };
   }
}
#endif
#endif
