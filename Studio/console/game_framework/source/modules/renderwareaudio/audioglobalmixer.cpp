/*****************************************************************************
*
* File :     AudioGlobalMixer.cpp
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
#include "precomp.h"

#ifdef WITH_AUDIO

// Left to its own devices, the MW linker will dead-strip the registrar ctors, so...
#ifdef __MWERKS__
#pragma force_active on
#pragma exceptions off // Turn off exceptions due to bug in MW Dwarf Generator for PS2, present in CW 3.04
#endif

#define RWS_TRACEENABLE

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/toolkits/debugtools/debugtools.h"
#include "framework/core/macros/debugmacros.h"
#include "framework/core/resourcemanager/cresourcemanager.h"
#include "framework/toolkits/world/factory.h"
#include "framework/mainloop/logic.h"
#include "framework/toolkits/audio/audio.h"
#include "framework/toolkits/world/clevel.h"
#include "audioglobalmixer.h"

namespace RWS
{
   namespace Audio
   {
      RWS_IMPLEMENT_CLASSID(AudioGlobalMixer);
      RWS_REGISTERCLASS(AudioGlobalMixer);
         
      /**
      *
      * Constructor for AudioGlobalMixer.
      *
      * \param attr  Reference to a CAttributePacket.
      *
      * \see AudioGlobalMixer()
      */
      AudioGlobalMixer::AudioGlobalMixer(const CAttributePacket& attr) : 
         InitCEventHandler(0),
         m_Gain(30.0f),
         m_DopplerScale(1.0f),
         m_DistanceFactor(1.0f),
         m_RollOffFactor(1.0f),
         m_DebugInfo(FALSE),
         m_FadeInc(0.03f),
         m_FadeState(FadeStopped)
      {
         RWS_FUNCTION("RWS::Audio::AudioGlobalMixer::AudioGlobalMixer")
            
         LinkMsg(iMsgRunningTick);
         
         RWS_RETURNVOID()
      }
      
      /**
      *
      * Destructor for AudioGlobalMixer.
      *
      * \see AudioGlobalMixer::AudioGlobalMixer
      */
      AudioGlobalMixer::~AudioGlobalMixer()
      {
         RWS_FUNCTION("RWS::Audio::AudioGlobalMixer::~AudioGlobalMixer")
               
         UnLinkMsg(iMsgRunningTick);
         UnLinkMsg(m_RenderIn);
         UnRegisterMsg(m_RenderIn);
            
         UnLinkMsg(m_FadeUp);
         UnRegisterMsg(m_FadeUp);
            
         UnLinkMsg(m_FadeDown);
         UnRegisterMsg(m_FadeDown);
            
         UnLinkMsg(m_EndFade);
         UnRegisterMsg(m_EndFade);
            
         RWS_RETURNVOID()
      }
        
      /**
      *
      * Handle attribute updates.
      *
      * \param attr The standard attribute packets.
      *
      */
      void AudioGlobalMixer::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::Audio::AudioGlobalMixer::HandleAttributes")
                
         CAttributeHandler::HandleAttributes(attr);

         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(AudioGlobalMixer));
            
         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
            case CMD_gain:
                attrIt->GetCommandData(m_Gain);
                RwaOsOutputSetGain(RwsAudio::GetOutputObject(), m_Gain);
                break;
                    
            case CMD_dopplerScale:
                attrIt->GetCommandData(m_DopplerScale);
                RwaListenerSetDopplerScale(RwsAudio::GetListener(), m_DopplerScale);
                break;
                      
                     
            case CMD_distanceFactor:
                attrIt->GetCommandData(m_DistanceFactor);
                RwaListenerSetDistanceFactor(RwsAudio::GetListener(), m_DistanceFactor);
                break;
                      
            case CMD_rolloffFactor:
                attrIt->GetCommandData(m_RollOffFactor);
                RwaListenerSetRolloffFactor(RwsAudio::GetListener(), m_RollOffFactor);
                break;

            case CMD_speakerConfig:
                RwUInt32  SpeakerConfig;
                attrIt->GetCommandData(SpeakerConfig);
                SetSpeakerConfig(SpeakerConfig);
                break;

            case CMD_In:
                UnLinkMsg(m_RenderIn);
                UnRegisterMsg(m_RenderIn);
                RegisterMsg(m_RenderIn, attrIt->GetAs_RwChar_ptr(), "RwCamera*");
                LinkMsg(m_RenderIn, "RwCamera*");
                break;

            case CMD_Debug:
                attrIt->GetCommandData(m_DebugInfo);
                break;
                      
            case CMD_StopAll:
                RwsAudio::StopAllVirtualVoices();
                break;

            case CMD_FadeUp:
                ReplaceLinkedMsg(m_FadeUp, attrIt->GetAs_RwChar_ptr(),0);
                break;

            case CMD_FadeDown:
                ReplaceLinkedMsg(m_FadeDown, attrIt->GetAs_RwChar_ptr(),0);
                break;

            case CMD_FadeStep:
                attrIt->GetCommandData(m_FadeInc);
                break;

            case CMD_FadeEnd:
                ReplaceRegisteredMsg(m_EndFade, attrIt->GetAs_RwChar_ptr(),0);
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
      * Handle events passed into the behavior.
      *
      * \param pMsg Standard message data object.
      *
      */
      void AudioGlobalMixer::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::Audio::AudioGlobalMixer::HandleEvents")
            
         if (pMsg.Id == iMsgRunningTick)
         {
            DisplayDebugInfo();
            ProcessFade();
            RwaCoreUpdate();
         }
         else if (pMsg.Id == m_RenderIn)
         {
            UpdateListener(pMsg);
         }
         else if (pMsg.Id == m_FadeDown)
         {
            // Trigger fade down.
            m_FadeState = FadeDown;
         }
         else if (pMsg.Id == m_FadeUp)
         {
            // Trigger fade up.
            m_FadeState = FadeUp;
         }
            
         RWS_RETURNVOID()
      }

      /**
      *
      * Display debugging information about the global mixer to console window.
      *
      */
#ifdef RWS_DEBUGTOOLS
      void AudioGlobalMixer::DisplayDebugInfo(void)
      {
         RWS_FUNCTION("RWS::Audio::AudioGlobalMixer::DisplayDebugInfo")
            
         // Display mixer debug info on target if debug tools selected.
         if (m_DebugInfo)
         {
            DebugTools::Printf("AUDIO MIXER\n");
            DebugTools::Printf("Global Gain %.3f\n", RwaOsOutputGetGain(RwsAudio::GetOutputObject()));
            DebugTools::Printf("Doppler Scale %.3f\n", RwaListenerGetDopplerScale(RwsAudio::GetListener()));
            DebugTools::Printf("Distance Factor %.3f\n", RwaListenerGetDistanceFactor(RwsAudio::GetListener()));
            DebugTools::Printf("Roll-off %.3f\n", RwaListenerGetRolloffFactor(RwsAudio::GetListener()));
         }
            
         RWS_RETURNVOID();
      }
#endif
      
      /**
      *
      * Process fade on output device.
      *
      */
      void AudioGlobalMixer::ProcessFade(void)
      {
         RWS_FUNCTION("RWS::Audio::AudioGlobalMixer::ProcessFade")
            
         // If fade event triggered,  perform fading operation.
         switch (m_FadeState)
         {
            case FadeDown:
               if (RwsAudio::FadeOutputObject(0.0f, m_FadeInc))
               {
                  m_FadeState = FadeStopped;
                  SendMsg(m_EndFade);
               }
               break;
               
            case FadeUp:
               if (RwsAudio::FadeOutputObject(m_Gain, m_FadeInc))
               {
                  m_FadeState = FadeStopped;
                  SendMsg(m_EndFade);
               }
               break;
         }
         
         RWS_RETURNVOID();
      }

      /**
      *
      * Update Listener with position based on incoming camera message.
      *
      * \param pMsg Incoming message data.
      *
      */
      void AudioGlobalMixer::UpdateListener(const CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::Audio::AudioGlobalMixer::UpdateListener")
            
         RwV3d *pPos;
         RwMatrix *pMatrix;
         
         // Update listener position based on incoming camera position.
         if (pMsg.pData != 0)
         {
            RwCamera *pInputCamera = reinterpret_cast<RwCamera*>(pMsg.pData);
            pMatrix = RwFrameGetLTM(RwCameraGetFrame(pInputCamera));
            pPos = RwMatrixGetPos(pMatrix);
            
            RwaListenerSetPosition(RwsAudio::GetListener(), pPos);
            RwaListenerSetOrientation(RwsAudio::GetListener(), pMatrix);   
         }
         
         RWS_RETURNVOID()
      }
      
      /**
      *
      * SetSpeakerConfig. Sets the speaker configuration for the output object. See RenderWare Audio Api reference 
      * for a description of speaker configurations.
      *
      * \param Config The speaker configuration.
      *
      */
      void AudioGlobalMixer::SetSpeakerConfig(const RwUInt32 Config)
      {
         RWS_FUNCTION("RWS::Audio::AudioGlobalMixer::SetSpeakerConfig")
            
         static RwaSpeakerConfig SpeakerConfig[] =
         {
            rwaSPEAKERCONFIG_STEREO, rwaSPEAKERCONFIG_MONO, rwaSPEAKERCONFIG_HEADPHONES, rwaSPEAKERCONFIG_SURROUND
         };   
         
         RwaOsOutputSetSpeakerConfig(RwsAudio::GetOutputObject(), SpeakerConfig[Config]);
         
         RWS_RETURNVOID();
      }
      
   }
}
#endif

