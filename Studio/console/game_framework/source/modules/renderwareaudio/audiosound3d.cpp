/*****************************************************************************
*
* File :     AudioSound3D.cpp
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
#include "precomp.h"

#ifdef WITH_AUDIO

// Left to its own devices, the MW linker will dead-strip the registrar ctors, so...
#ifdef __MWERKS__
#pragma force_active on
#pragma exceptions off // Turn off exceptions due to bug in MW Dwarf Generator for PS2, present in CW 3.04
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/toolkits/debugtools/debugtools.h"
#include "framework/core/resourcemanager/cresourcemanager.h"
#include "framework/mainloop/logic.h"
#include "framework/toolkits/audio/audio.h"
#include "framework/toolkits/rwaudresource/rwaudresource.h"
#include "audiosound3dinterface.h"
#include "audiosound3d.h"
#include "audiogroup.h"

#define RWS_TRACEENABLE

namespace RWS
{
   namespace Audio
   {
      RWS_IMPLEMENT_CLASSID(AudioSound3D);
      RWS_REGISTERCLASS(AudioSound3D);
            
      /**
      *
      * Constructor for 3D sound object.
      *
      * \see ~AudioSound3D
      *
      */
      AudioSound3D::AudioSound3D(const CAttributePacket& attr):
                AudioSound3DInterface(attr), 
                InitCEventHandler(&m_pAtomic) ,
                m_DestGain(0.0f),
                m_SampleRate(48000.0f),
                m_DebugInfo(FALSE),
                m_RestartVoice(TRUE),
                m_FadeInc(0.03f),
                m_FadeState(FadeStopped),
                m_DictIndex(-1),
                m_GroupFadeNotify(FALSE),
                m_EndEvent(FALSE)
      {
         RWS_FUNCTION("RWS::Audio::AudioSound3D::AudioSound3D")
                      
         RegisterMsg(iMsgAudioGroupFadeDownGroup, "iMsgAudioGroupFadeDownGroup", "RwUInt32");
         LinkMsg(iMsgAudioGroupFadeDownGroup);

         RegisterMsg(iMsgAudioGroupFadeUpGroup, "iMsgAudioGroupFadeUpGroup", "RwUInt32");
         LinkMsg(iMsgAudioGroupFadeUpGroup);

         RegisterMsg(iMsgAudioGroupEnableGroup, "iMsgAudioGroupEnableGroup", "RwUInt32");
         LinkMsg(iMsgAudioGroupEnableGroup);

         RegisterMsg(iMsgAudioGroupDisableGroup, "iMsgAudioGroupDisableGroup", "RwUInt32");
         LinkMsg(iMsgAudioGroupDisableGroup);
             
#ifdef RWS_DESIGN
         LinkMsg(iMsgAudioDictionaryDelete, "RwUInt32 GroupIndex");
#endif

         LinkMsg(iMsgRunningTick);
         RWS_RETURNVOID()
      }
        
      /**
      *        
      * Destroy 3D sound object.
      *
      * \see AudioSound3D
      *
      */
      AudioSound3D::~AudioSound3D()
      {
         RWS_FUNCTION("RWS::Audio::AudioSound3D::~AudioSound3D")

         UnLinkMsg(m_Play);
         UnRegisterMsg(m_Play);

         UnLinkMsg(m_Stop);
         UnRegisterMsg(m_Stop);

         UnLinkMsg(m_EndTarget);
         UnRegisterMsg(m_EndTarget);

         UnLinkMsg(m_FadeUp);
         UnRegisterMsg(m_FadeUp);

         UnLinkMsg(m_FadeDown);
         UnRegisterMsg(m_FadeDown);

         UnLinkMsg(m_EndFade);
         UnRegisterMsg(m_EndFade);

         UnLinkMsg(iMsgAudioGroupFadeDownGroup);
         UnRegisterMsg(iMsgAudioGroupFadeDownGroup);

         UnLinkMsg(iMsgAudioGroupFadeUpGroup);
         UnRegisterMsg(iMsgAudioGroupFadeUpGroup);

         UnLinkMsg(iMsgAudioGroupEnableGroup);
         UnRegisterMsg(iMsgAudioGroupEnableGroup);

         UnLinkMsg(iMsgAudioGroupDisableGroup);
         UnRegisterMsg(iMsgAudioGroupDisableGroup);

#ifdef RWS_DESIGN
         UnLinkMsg(iMsgAudioDictionaryDelete);
#endif

         UnLinkMsg(iMsgRunningTick);

         RWS_RETURNVOID()   
      }
        
      /**
      *
      * Handle attribute updates.
      *
      * \param attr The standard attribute packets.
      *  
      */
      void AudioSound3D::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::Audio::AudioSound3D::HandleAttributes")
             
         AudioSound3DInterface::HandleAttributes(attr);
             
         CAttributeHandler::HandleAttributes(attr);
           
         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(AudioSound3D));
            
         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
               case CMD_m_play:
                  ReplaceLinkedMsg(m_Play, attrIt->GetAs_RwChar_ptr(), "RwFrame*");
                  break;
                    
               case CMD_m_stop:
                  ReplaceLinkedMsg(m_Stop, attrIt->GetAs_RwChar_ptr(), 0);
                  break;
                    
               case CMD_m_endtarget:
                  ReplaceRegisteredMsg(m_EndTarget, attrIt->GetAs_RwChar_ptr(), 0);
                  break;

               case CMD_selectText:
                  SelectWave(attrIt->GetAs_RwChar_ptr());                    
                  break;
                    
               case CMD_Voice_Restart:
                  attrIt->GetCommandData(m_RestartVoice);
                  break;

               case CMD_FadeUp:
                  ReplaceLinkedMsg(m_FadeUp, attrIt->GetAs_RwChar_ptr(), 0);
                  break;

               case CMD_FadeDown:
                  ReplaceLinkedMsg(m_FadeDown, attrIt->GetAs_RwChar_ptr(), 0);
                  break;


               case CMD_FadeStep:
                  attrIt->GetCommandData(m_FadeInc);
                  break;

               case CMD_FadeEnd:
                  ReplaceRegisteredMsg(m_EndFade, attrIt->GetAs_RwChar_ptr(), 0);
                  break;

               case CMD_looped:
                  RwaVirtualVoiceSetLoop (m_pVoice, attrIt->GetAs_RwInt32());
                  break;
                    
               case CMD_gain:                    
                  RwaVirtualVoiceSetGain(m_pVoice, attrIt->GetAs_RwReal());

                  // Store gain for fade destination.
                  m_DestGain = RwaVirtualVoiceGetGain(m_pVoice);
                  break;
                    
               case CMD_pan:
                  RwaVirtualVoiceSetPan(m_pVoice, attrIt->GetAs_RwReal());
                  break;
                               
               case CMD_Priority:
                  RwaVirtualVoiceSetPriority(m_pVoice, attrIt->GetAs_RwReal());
                  break;

               case CMD_sample:
                  m_SampleRate = SelectSampleRate(attrIt->GetAs_RwUInt32());
                  RwaVirtualVoiceSetFreq(m_pVoice, m_SampleRate);  
                  break;

               case CMD_Min_Atten:
                  RwaVirtualVoiceSetMinDistance(m_pVoice, attrIt->GetAs_RwReal());
                  break;

               case CMD_Max_Atten:
                  RwaVirtualVoiceSetMaxDistance(m_pVoice, attrIt->GetAs_RwReal());
                  break;

               case CMD_Debug:
                  attrIt->GetCommandData(m_DebugInfo);
                  break;

               case CMD_Bias:
                  RwaVirtualVoiceSetBias(m_pVoice, attrIt->GetAs_RwReal());
                  break;
                    
               default:
                  break;
            }

            ++attrIt;
         } 
           
         RWS_RETURNVOID()
      }
        
      /**
      *        
      * Handle events.
      *
      * \param pMsg Standard message data object.
      *
      */
      void AudioSound3D::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::Audio::AudioSound3D::HandleEvents")

         if ((pMsg.Id == m_Play))
         {
            if (m_RestartVoice)
            {
               Play(pMsg);
            }
            else
            {
               // If the wave associated with the voice is null, then don't play. This could happen
               // when a dictionary is deleted from the workspace, the virtual voice is still valid
               // but the wave would no longer exist.
               if (m_pOsWave)
               {
                  // If the sound is still playing, don't play it again until it has finished.
                  if (!RwaVirtualVoiceGetPlaying(m_pVoice))
                  {
                     Play(pMsg);
                  }
               }
            }
         }             
         else if (pMsg.Id == m_Stop)
         {
            RwaVirtualVoiceSetTrigger(m_pVoice, FALSE);
            SendMsg(m_EndTarget);
         }
         else if (pMsg.Id == iMsgRunningTick)
         {
            if (m_pOsWave)
            {
               if ((!RwaVirtualVoiceGetPlaying(m_pVoice)) && (m_EndEvent))
               {
                  SendMsg(m_EndTarget);
                  m_EndEvent = FALSE;
               }
            }

            ProcessFade();
            DisplayDebugInfo();
         }
         else if (pMsg.Id == m_FadeDown)
         {
            m_FadeState = FadeDown;
         }
         else if (pMsg.Id == m_FadeUp)
         {
            m_FadeState = FadeUp;
         }
         else if (pMsg.Id == iMsgAudioGroupFadeDownGroup)
         {
            TriggerGroupFadeDown(pMsg);
         }
         else if (pMsg.Id == iMsgAudioGroupFadeUpGroup)
         {
            TriggerGroupFadeUp(pMsg);
         }
         else if (pMsg.Id == iMsgAudioGroupEnableGroup)
         {
            TriggerGroupVoiceEnable(pMsg);

         }
         else if (pMsg.Id == iMsgAudioGroupDisableGroup)
         {
            TriggerGroupVoiceDisable(pMsg);
         }
#ifdef RWS_DESIGN
         else if (pMsg.Id == iMsgAudioDictionaryDelete)
         {
            HandleDictionaryDelete(pMsg);
         }
#endif               
         RWS_RETURNVOID()
      }
        
      /**
      *
      * Play a sound.
      *
      * \see Stop
      */
      void AudioSound3D::Play(const CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::Audio::AudioSound3D::Play")
         RWS_PRE(m_pVoice);
         RWS_PRE(m_pOsWave);
         RwMatrix *pMatrix = 0;
         
         m_EndEvent = TRUE;
         RwaVirtualVoiceSetWave(m_pVoice, reinterpret_cast<RwaWave*>(m_pOsWave)); 
         RwaVirtualVoiceSetTrigger(m_pVoice, TRUE);
         RwaVirtualVoiceSetFreq(m_pVoice, m_SampleRate);  
         
         if (pMsg.pData)
         {
            pMatrix = RwFrameGetMatrix(reinterpret_cast<RwFrame*>(pMsg.pData));
         }
         
         SetVirtualVoicePosition(pMatrix);
         
         RWS_RETURNVOID()
      }

      /**
      *
      * Process fade on virtual voice.
      *
      */
      void AudioSound3D::ProcessFade(void)
      {
         RWS_FUNCTION("RWS::Audio::AudioSound3D::ProcessFade")
            
         static RwReal Gain;
         
         // If fade event triggered, perform fading operation.
         switch (m_FadeState)
         {
         case FadeDown:
            if (RwsAudio::FadeVirtualVoice(0.0f, m_FadeInc, m_pVoice))
            {
               m_FadeState = FadeStopped;
               Gain = RwaVirtualVoiceGetGain(m_pVoice);
               
               // If the fade has not been triggered by the AudioGroup behavior, 
               // then send out the EndFade event when fade has completed.
               if (!m_GroupFadeNotify)
               {
                  SendMsg(m_EndFade);
               }
               else
               {
                  // If this is a fade triggered by the AudioGroup, then we don't want 
                  // to send out a fade event for every sound.
                  m_GroupFadeNotify = FALSE;
               }
            }
            break;
            
         case FadeUp:
            if (RwsAudio::FadeVirtualVoice(m_DestGain, m_FadeInc, m_pVoice))
            {
               m_FadeState = FadeStopped;
               Gain = RwaVirtualVoiceGetGain(m_pVoice);
               
               // If the fade has not been triggered by the AudioGroup behavior, 
               // then send out the EndFade event when fade has completed.
               if (!m_GroupFadeNotify)
               {
                  SendMsg(m_EndFade);
               }
               else
               {
                  // If this is a fade triggered by the AudioGroup, then we don't want 
                  // to send out a fade event for every sound.
                  m_GroupFadeNotify = FALSE;
               }                        
            }
            break;
            
         case FadeStopped:
            break;
         }
         
         RWS_RETURNVOID();
      }
        
      /**
      *
      * Display debug information on console.
      *
      */
#ifdef RWS_DEBUGTOOLS
      void AudioSound3D::DisplayDebugInfo(void)
      {
         RWS_FUNCTION("RWS::Audio::AudioSound3D::DisplayDebugInfo")
            
         // Display debug info on target only if debug tools selected.
         if (m_DebugInfo)
         {
            DebugTools::Printf("Event = %s  ", m_Play.GetName());
            
            if (RwaVirtualVoiceGetLoop(m_pVoice))
            {
               DebugTools::Printf("Loop = TRUE ");
            }
            else
            {
               DebugTools::Printf("Loop = FALSE ");
            }
               
            DebugTools::Printf("Gain = %.1f   Pan = %.1f\n", RwaVirtualVoiceGetGain(m_pVoice), 
               RwaVirtualVoiceGetPan(m_pVoice));     
         }
         RWS_RETURNVOID();
      }
#endif

      /**

      *
      * Trigger fade up if this voice's sound is in the selected group.
      *
      * \param pMsg Incoming message, containing message data.
      *
      */
      void AudioSound3D::TriggerGroupFadeUp(const CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::Audio::AudioSound3D::TriggerGroupFadeUp")
            
         if (pMsg.pData)
         {
            if (m_DictIndex == *(static_cast<RwInt32*>(pMsg.pData)))
            {
               m_FadeState = FadeUp;
               m_GroupFadeNotify = TRUE;
            }
         }
         
         RWS_RETURNVOID();
      }

      /**
      *
      * Trigger fade down if this voice's sound is in the selected group.
      *
      * \param pMsg Incoming message, containing message data.
      *
      */
      void AudioSound3D::TriggerGroupFadeDown(const CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::Audio::AudioSound3D::TriggerGroupFadeDown")
            
         if (pMsg.pData)
         {
            if (m_DictIndex == *(static_cast<RwInt32*>(pMsg.pData)))
            {
               m_FadeState = FadeDown;
               m_GroupFadeNotify = TRUE;
            }
         }
          
         RWS_RETURNVOID();
      }

      /**
      *
      * Disable voice if part of intended group.
      *
      * \param pMsg Incoming message, containing message data.
      *
      */
      void AudioSound3D::TriggerGroupVoiceDisable(const CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::Audio::AudioSound3D::TriggerGroupVoiceDisable")
            
         if (pMsg.pData)
         {
            if (m_DictIndex == *(static_cast<RwInt32*>(pMsg.pData)))
            {
               RwaVirtualVoiceSetTrigger(m_pVoice, FALSE);
               SendMsg(m_EndTarget);
            }
         }
            
         RWS_RETURNVOID();
      }

      /**
      *
      * Enable voice if part of intended group.
      *
      * \param pMsg Incoming message, containing message data.
      *
      */
      void AudioSound3D::TriggerGroupVoiceEnable(const CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::Audio::AudioSound3D::TriggerGroupVoiceEnable")
            
         if (pMsg.pData)
         {
            if (m_DictIndex == *(static_cast<RwInt32*>(pMsg.pData)))
            {
               Play(pMsg);
            }
         }
            
         RWS_RETURNVOID();
      }

      /**
      *
      * Select sound from dictionary and assign it a group number.
      *
      */
      void AudioSound3D::SelectWave(const RwChar* const pWavName)
      {
         RWS_FUNCTION("RWS::Audio::AudioSound3D::SelectWave")
         RWS_ASSERT(pWavName, "No wave name specified.")
            
         // If we have a wave already assigned then we can stop the voice,            
         if (m_pOsWave)
         {
            RwaVirtualVoiceSetTrigger(m_pVoice, FALSE);
            SendMsg(m_EndTarget);
         }
            
         m_pOsWave = RwsAudio::FindWave(pWavName);    
         RWS_ASSERT(m_pOsWave, "Sound not found in any group, name = " << pWavName); 
         
         m_DictIndex = RwsAudio::FindDictionaryId(pWavName);
         RWS_ASSERT(m_DictIndex >= 0, "Sound not found in any group, name = " << pWavName); 
         
         RWS_RETURNVOID();
      }

      /**
      *
      * When a dictionary is selected in design time, the audio resource manager sends out a message
      * notifying the AudioSound3D behavior that the dictionary is being deleted. If the AudioSound3D behavior is 
      * using a wave from that dictionary, then it must stop playing it and dereference itself from the sound.
      *
      * \param pMsg Incoming message, containing message data.
      *
      */
      void AudioSound3D::HandleDictionaryDelete(const CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::Audio::AudioSound3D::HandleDictionaryDelete")
            
#ifdef RWS_DESIGN
         // If the group index matches that of the dictionary being deleted, then stop sound and clear wave.

         if (m_DictIndex == reinterpret_cast<RwInt32>(pMsg.pData))
         {
            RwaVirtualVoiceSetTrigger(m_pVoice, FALSE);
            m_pOsWave = 0;
            m_DictIndex = -1;
            SendMsg(m_EndTarget);
            
            RwaCoreUpdate();
         }
#endif
         RWS_RETURNVOID();
      }

      /**
      *
      * Select sample rate. This is called when selecting one of the default sample rates in the attributes.
      * The sample rates that can be selected are, 11025.0f, 22050.0f, 32000.0f, 44100.0f, 48000.0f.
      *
      * \param Rate Index into table of sample rates.        
      *
      * \return Return sample rate.
      *
      */
      RwReal AudioSound3D::SelectSampleRate(const RwUInt32 Index)
      {
         RWS_FUNCTION("RWS::Audio::AudioSound3D::SelectSampleRate")
            
         static RwReal SampleRate[] =
         {
            11025.0f, 22050.0f, 32000.0f, 44100.0f, 48000.0f
         };   
         
         RWS_RETURN(SampleRate[Index]);
      }
   }
}

#endif
