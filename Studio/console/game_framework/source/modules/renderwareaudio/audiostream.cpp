/*****************************************************************************
*
* File :     AudioStream.cpp
*
* Abstract:  Sound stream behavior.
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
#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/audio/audio.h"
#include "audiostream.h"

namespace RWS
{
   namespace Audio
   {
      RWS_IMPLEMENT_CLASSID(AudioStream);
      RWS_REGISTERCLASS(AudioStream);
 
      /**
      *
      * Constructor for audio stream.
      *
      * \see ~AudioStream
      *
      */
      AudioStream::AudioStream(const CAttributePacket& attr) : InitCEventHandler(0), m_pStream(0), m_pStreamName(0)
      {
          RWS_FUNCTION("RWS::Audio::AudioStream::AudioStream")
          RWS_RETURNVOID();
      }
        
      /**
      *
      * Destroy audio stream object.
      *
      * \see AudioStream
      */
      AudioStream::~AudioStream()
      {
         RWS_FUNCTION("RWS::Audio::AudioStream::~AudioStream")
            
         UnLinkMsg(m_Play);
         UnRegisterMsg(m_Play);

         UnLinkMsg(m_Stop);
         UnRegisterMsg(m_Stop);

         RwaOsStreamDestroy(m_pStream, 0, 0);
         RwFree(m_pStreamName);

         RWS_RETURNVOID();
      }

      /**
      *
      * Handle attribute updates.
      *
      * \param attr The standard attribute packets.
      *  
      */
      void AudioStream::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::Audio::AudioStream::HandleAttributes")

         CAttributeHandler::HandleAttributes(attr);
                       
         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(AudioStream));
          
         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
            case CMD_m_play:
               ReplaceLinkedMsg(m_Play, attrIt->GetAs_RwChar_ptr(),0);
               break;
                
            case CMD_m_stop:
               ReplaceLinkedMsg(m_Stop, attrIt->GetAs_RwChar_ptr(),0);
               break;   
                
            case CMD_selectText:
            {
               RwChar *pFileName = const_cast<RwChar*>(attrIt->GetAs_RwChar_ptr());

               if (m_pStream)
               {                            
                  RwaOsStreamSetPlayState(m_pStream, rwaSTREAMPLAYSTATE_STOP);

                  RwaOsStreamDestroy(m_pStream, 0, 0);
                  m_pStream = 0;
               }

               if (m_pStreamName != 0)
               {
                  RwFree(m_pStreamName);
               }
               
               m_pStreamName = (RwChar*)RwMalloc(strlen(pFileName) + 1,rwMEMHINTDUR_FUNCTION);
               strncpy(m_pStreamName, pFileName, sizeof(RwChar) * (strlen(pFileName) + 1));

               OpenStream(m_pStreamName);
            }
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
      * Handle Events
      *
      * \param pMsg Standard message data object.
      *
      */
      void AudioStream::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::Audio::AudioStream::HandleEvents")

         if (pMsg.Id == m_Play)
         {
            RwaOsStreamSetPlayState(m_pStream, rwaSTREAMPLAYSTATE_PLAY);
         }
         else if (pMsg.Id == m_Stop)
         {
            RwaOsStreamSetPlayState(m_pStream, rwaSTREAMPLAYSTATE_STOP);
         }
            
         RWS_RETURNVOID();
      }

      /**
      *
      * Open an audio stream.
      *
      * \param pName Filename for the stream.
      *
      */
      void AudioStream::OpenStream(const RwChar* const pName)
      {  
         RWS_FUNCTION("RWS::RwsAudio::AudioStream::OpenStream")
                           
         m_MediaParams.mediaID                  = const_cast<RwChar*>(pName);
         m_MediaParams.mediaIDType              = rwaSTREAMMEDIATYPE_FILENAME;
         m_MediaParams.audioStreamID            = 0;
         m_MediaParams.audioStreamName          = 0;
         m_MediaParams.format                   = 0;
         m_MediaParams.mediaBufferSize          = 0;
         m_MediaParams.mediaNumPacketBuffers    = 8;
         m_MediaParams.pipelineNumPacketBuffers = 4;
         m_MediaParams.allowReinterleaving      = TRUE;
         
         m_pStream = RwaOsStreamCreate(RWS::RwsAudio::GetOutputObject(), 0, 0, 0);
         RWS_ASSERTE(m_pStream);
         
         RwaOsStreamSetMedia(m_pStream, &m_MediaParams);
         RWS_RETURNVOID();   
      } 
   }
}
#endif // WITH_AUDIO
