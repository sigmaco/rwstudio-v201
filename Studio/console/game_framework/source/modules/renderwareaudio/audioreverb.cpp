/*****************************************************************************
*
* File :      AudioReverb.cpp
*
* Abstract :  Environment object for RenderWare Audio output object.
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
#include "framework/toolkits/audio/audio.h"
#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/math/maths.h"
#include "audioreverb.h"

namespace RWS
{
   namespace Audio
   {
      RWS_IMPLEMENT_CLASSID(AudioReverb);
      RWS_REGISTERCLASS(AudioReverb);
       
      /**
      *
      * Constructor for AudioReverb
      *
      * \see ~AudioReverb()
      */
      AudioReverb::AudioReverb(const CAttributePacket& attr)
          : InitCEventHandler(0),
            m_EnvironmentType(0),
            m_Override(0),
            m_DebugInfo(FALSE),
            m_Active(FALSE),
            m_FadeInc(10),
            m_FadeState(FadeStopped),
            m_FadeUpDest(0)

      {
         RWS_FUNCTION("RWS::Audio::AudioReverb::AudioReverb");

         m_Environment.decayHFRatio       = 0.5f;
         m_Environment.decayTime          = 1.0f;
         m_Environment.density            = 100.0f;
         m_Environment.diffusion          = 100.0f;
         m_Environment.hfReference        = 5000.f;
         m_Environment.reflections        = 1000;
         m_Environment.reflectionsDelay   = 0.02f;
         m_Environment.reverb             = -10000;
         m_Environment.reverbDelay        = 0.04f;
         m_Environment.room               = -10000;
         m_Environment.roomHF             = 0;
         m_Environment.roomRolloffFactor  = 0.0f;

         LinkMsg(iMsgRunningTick);

         RWS_RETURNVOID();
      }
        
      /**
      *
      * Destructor for AudioReverb
      *
      * \see AudioReverb
      */
      AudioReverb::~AudioReverb(void)
      {
         RWS_FUNCTION("RWS::Audio::AudioReverb::~AudioReverb");
         
         UnLinkMsg(m_StartEnvironment);
         UnRegisterMsg(m_StartEnvironment);
          
         UnLinkMsg(m_StopEnvironment);
         UnRegisterMsg(m_StopEnvironment);

         UnLinkMsg(m_FadeUp);
         UnRegisterMsg(m_FadeUp);

         UnLinkMsg(m_FadeDown);
         UnRegisterMsg(m_FadeDown);

         UnLinkMsg(m_EndFade);
         UnRegisterMsg(m_EndFade);

         UnLinkMsg(iMsgRunningTick);
         
         RWS_RETURNVOID();
      }
        
      /**
      *        
      * Handle events passed into the behavior.
      *        
      * \param pMsg Standard message data object.
      *  
      */
      void AudioReverb::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::Audio::AudioReverb::HandleEvents");
           
         if (pMsg.Id == m_StartEnvironment)
         {
            StartEnvironment();
         }
         else if (pMsg.Id == m_StopEnvironment)
         {
            m_Active = FALSE;
            SetEnvironment(0, m_Environment.room);
         }
         else if (pMsg.Id == m_FadeDown)
         {
            m_FadeState = FadeDown;
         }
         else if (pMsg.Id == m_FadeUp)
         {
            m_FadeState = FadeUp;
         }
         else if (pMsg.Id == iMsgRunningTick)
         {
            ProcessFade();
            DisplayDebugInfo();
         }
           
         RWS_RETURNVOID();
      }

      /**
      *
      * Update overridden environment parameters.
      *
      */
      void AudioReverb::ApplyEnvironment(void)
      {
         RWS_FUNCTION("RWS::Audio::AudioReverb::ApplyEnvironment")

         if ((m_Active) && (m_Override))
         {
             RwaOsOutputSetEnvironment(RwsAudio::GetOutputObject(), &m_Environment);
         }

         RWS_RETURNVOID();
      }
        
      /**
      *
      * Handle attribute updates.
      *
      * \param attr The standard attribute packets.
      *  
      */
      void AudioReverb::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::Audio::AudioReverb::HandleAttributes");
            
         CAttributeHandler::HandleAttributes(attr);
          
         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(AudioReverb));

         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
            case CMD_TriggerEnvironment:
               ReplaceLinkedMsg(m_StartEnvironment, attrIt->GetAs_RwChar_ptr(),0);
               break;

            case CMD_StopEnvironment:
               ReplaceLinkedMsg(m_StopEnvironment, attrIt->GetAs_RwChar_ptr(),0);
               break;

            case CMD_environmentType:
               attrIt->GetCommandData(m_EnvironmentType);

               if ((!m_Override) && (m_Active))
               {                          
                  SetEnvironment( m_EnvironmentType, m_Environment.room );
               }
               break;
                      
            case CMD_environmentGain:
               attrIt->GetCommandData(m_FadeUpDest); 
               UpdateEnvironmentGain();
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

            case CMD_Override:
               attrIt->GetCommandData(m_Override);
               OverrideEnvironment();                    
               break;

            case CMD_decayHFRatio:
               attrIt->GetCommandData(m_Environment.decayHFRatio);                    
               ApplyEnvironment();
               break;

            case CMD_decayTime:
               attrIt->GetCommandData(m_Environment.decayTime);
               ApplyEnvironment();
               break;

            case CMD_density:
               attrIt->GetCommandData(m_Environment.density);
               ApplyEnvironment();
               break;

            case CMD_diffusion:
               attrIt->GetCommandData(m_Environment.diffusion);
               ApplyEnvironment();
               break;

            case CMD_hfReference:
               attrIt->GetCommandData(m_Environment.hfReference);
               ApplyEnvironment();
               break;

            case CMD_reflections:
               attrIt->GetCommandData(m_Environment.reflections);
               ApplyEnvironment();
               break;

            case CMD_reflectionsDelay:
               attrIt->GetCommandData(m_Environment.reflectionsDelay);
               ApplyEnvironment();
               break;

            case CMD_reverb:
               attrIt->GetCommandData(m_Environment.reverb);
               ApplyEnvironment();
               break;

            case CMD_reverbDelay:
               attrIt->GetCommandData(m_Environment.reverbDelay);
               ApplyEnvironment();
               break;

            case CMD_roomHF:
               attrIt->GetCommandData(m_Environment.roomHF);
               ApplyEnvironment();
               break;

            case CMD_roomRolloffFactor:
               attrIt->GetCommandData(m_Environment.roomRolloffFactor);
               ApplyEnvironment();
               break;
                       
            case CMD_Debug:
               attrIt->GetCommandData(m_DebugInfo);
               break;
                      
            default:
               break;
            }

            ++attrIt;
         }
            
         RWS_RETURNVOID();
      }

      /**
      *
      * Process the fading of the environment gain.
      *
      */
      void AudioReverb::ProcessFade(void)
      {
         RWS_FUNCTION("RWS::Audio::AudioReverb::ProcessFade");
         
         RwaEnvironment *pEnvironment = RwsAudio::GetEnvironment();
         
         switch (m_FadeState)
         {
         case FadeDown:
            if (RwsAudio::FadeEnvironment(-1000, m_FadeInc))
            {
               m_FadeState = FadeStopped;
               SendMsg(m_EndFade);
            }
            
            m_Environment.room = pEnvironment->room;
            break;
            
         case FadeUp:
            if (RwsAudio::FadeEnvironment(m_FadeUpDest, m_FadeInc))
            {
               m_FadeState = FadeStopped;
               SendMsg(m_EndFade);
            }
            
            m_Environment.room = pEnvironment->room;
            break;
            
         case FadeStopped:
            break;
         }

         
         RWS_RETURNVOID();
      }

      /**
      *
      * Display debugging info on the console.      
      *
      */
#ifdef RWS_DEBUGTOOLS
      void AudioReverb::DisplayDebugInfo(void)
      {
         RWS_FUNCTION("RWS::Audio::AudioReverb::DisplayDebugInfo");
         
         if (m_DebugInfo)
         {
            DebugTools::Printf("I3DL2 ENVIRONMENT PARAMETERS\n");
            DebugTools::Printf("decayHFRatio = %.2f\n", m_Environment.decayHFRatio );
            DebugTools::Printf("decayTime = %.2f\n", m_Environment.decayTime);
            DebugTools::Printf("density = %.2f\n", m_Environment.density );
            DebugTools::Printf("diffusion = %.2f\n", m_Environment.diffusion );
            DebugTools::Printf("hfReference = %.2f\n", m_Environment.hfReference );
            DebugTools::Printf("reflections = %.2d\n", m_Environment.reflections);
            DebugTools::Printf("reflectionsDelay = %.2f\n", m_Environment.reflectionsDelay);
            DebugTools::Printf("reverb = %.2d\n", m_Environment.reverb );
            DebugTools::Printf("reverbDelay = %.2f\n", m_Environment.reverbDelay );
            DebugTools::Printf("room = %.2d\n", m_Environment.room);
            DebugTools::Printf("roomHF = %.2d\n", m_Environment.roomHF);
            DebugTools::Printf("roomRolloffFactor = %.2f\n", m_Environment.roomRolloffFactor );
         }
         
         RWS_RETURNVOID();
      }
#endif

      /**
      *
      * Enable the environment. If the override flag has not been set, use the default environment effects.
      * Otherwise use the custom created environment effect.
      *
      */
      void AudioReverb::StartEnvironment(void)
      {
         RWS_FUNCTION("RWS::Audio::AudioReverb::StartEnvironment");
         
         m_Active = TRUE;
         
         if (!m_Override)
         {
            SetEnvironment(m_EnvironmentType, m_Environment.room );
         }
         else
         {
            RwaEnvironment *pEnvironment = RwsAudio::GetEnvironment();
            *pEnvironment = m_Environment;
            RwaOsOutputSetEnvironment(RwsAudio::GetOutputObject(), pEnvironment);
         }
         
         RWS_RETURNVOID();
      }

      /**
      *
      * Allows you to override the default environment types so you can create your own with the entity attributes.
      *
      */
      void AudioReverb::OverrideEnvironment(void)
      {
         RWS_FUNCTION("RWS::Audio::AudioReverb::OverrideEnvironment");
         
         RwaEnvironment *pEnvironment = RwsAudio::GetEnvironment();
         
         if (m_Active)
         {
            if (m_Override)
            {
               // Set the overridden environment for the output device.
               *pEnvironment = m_Environment;
               RwaOsOutputSetEnvironment(RwsAudio::GetOutputObject(), pEnvironment);
            }
            else
            {
               // Set default environment type. The entity attributes set the environment gain,
               // so this needs to be passed in to set the correct gain level.
               SetEnvironment(m_EnvironmentType, m_Environment.room);
            }
         }
         
         RWS_RETURNVOID();
      }

      /**
      *
      * Update the gain for the environment.
      *
      */
      void AudioReverb::UpdateEnvironmentGain(void)
      {
         RWS_FUNCTION("RWS::Audio::AudioReverb::UpdateEnvironmentGain");
         
         RwaEnvironment *pEnvironment = RwsAudio::GetEnvironment();
         
         pEnvironment->room = m_FadeUpDest;
         m_Environment.room = m_FadeUpDest;
         
         if (m_Active)
         {
            RwaOsOutputSetEnvironment(RwsAudio::GetOutputObject(), pEnvironment);
         }
         
         RWS_RETURNVOID();
      }

      /**
      *
      * Set environment effect on output object.
      *
      * \param Rev An index into a table of default reverbs effects that come with RenderWare Audio. 
      * See documentation for RwaEnvironment in the RenderWare Audio Api reference guide.
      *
      * \param Gain Sets the gain for mixing the environment effect with a voice.
      *
      */
      void AudioReverb::SetEnvironment(const RwUInt32 Rev, const RwUInt32 Gain)
      {
         RWS_FUNCTION("RWS::Audio::RwsReverb::SetEnvironment")
            
            /*RenderWare Audio environment presets (I3DL2 compatible)*/
            static RwaEnvironment Environment[] =
         {
               rwaENVIRONMENT_OFF             ,
               rwaENVIRONMENT_GENERIC         ,
               rwaENVIRONMENT_PADDEDCELL      ,
               rwaENVIRONMENT_ROOM            ,
               rwaENVIRONMENT_BATHROOM        ,
               rwaENVIRONMENT_LIVINGROOM      ,
               rwaENVIRONMENT_STONEROOM       ,
               rwaENVIRONMENT_AUDITORIUM      ,
               rwaENVIRONMENT_CONCERTHALL     ,
               rwaENVIRONMENT_CAVE            ,
               rwaENVIRONMENT_ARENA           ,
               rwaENVIRONMENT_HANGAR          ,
               rwaENVIRONMENT_CARPETEDHALLWAY ,
               rwaENVIRONMENT_HALLWAY         ,
               rwaENVIRONMENT_STONECORRIDOR   ,
               rwaENVIRONMENT_ALLEY           ,
               rwaENVIRONMENT_FOREST          ,
               rwaENVIRONMENT_CITY            ,
               rwaENVIRONMENT_MOUNTAINS       ,
               rwaENVIRONMENT_QUARRY          ,
               rwaENVIRONMENT_PLAIN           ,
               rwaENVIRONMENT_PARKINGLOT      ,
               rwaENVIRONMENT_SEWERPIPE       ,
               rwaENVIRONMENT_UNDERWATER      
         };   
         
         RWS_PRE(Rev >= 0);
         RWS_PRE(Rev < (sizeof(Environment) / sizeof(RwaEnvironment)));
         
         RwaEnvironment *pEnvironment = RwsAudio::GetEnvironment();
         
         *pEnvironment = Environment[Rev];
         pEnvironment->room = Gain;
         
         RwaOsOutputSetEnvironment(RwsAudio::GetOutputObject(), pEnvironment);
         
         RWS_RETURNVOID();
      }
   }
}
#endif
