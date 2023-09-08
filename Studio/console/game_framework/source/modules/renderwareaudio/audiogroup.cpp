/*****************************************************************************
*
* File :      AudioGroup.cpp
*
* Abstract :   Behavior to assign wave dictionaries to a group.
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
#include "framework/core/resourcemanager/cresourcemanager.h"
#include "audiogroup.h"

namespace RWS
{
   namespace Audio
   {
      CEventId        iMsgAudioGroupFadeDownGroup;    /**< Fade down event.*/ 
      CEventId        iMsgAudioGroupFadeUpGroup;      /**< Fade up event.*/ 
      CEventId        iMsgAudioGroupDisableGroup;     /**< Disable group event.*/ 
      CEventId        iMsgAudioGroupEnableGroup;      /**< Enable group event.*/ 

      RWS_IMPLEMENT_CLASSID(AudioGroup);
      RWS_REGISTERCLASS(AudioGroup);
        
      /**
      *
      * Constructor for AudioGroup
      *
      * \see ~AudioGroup()
      */
      AudioGroup::AudioGroup(const CAttributePacket& attr):
         InitCEventHandler(0),
         m_DictionaryId(0)
      {
         RWS_FUNCTION("RWS::Audio::AudioGroup::AudioGroup");
         
         RegisterMsg (iMsgAudioGroupFadeDownGroup, "iMsgAudioGroupFadeDownGroup", "RwUInt32");
         RegisterMsg (iMsgAudioGroupFadeUpGroup, "iMsgAudioGroupFadeUpGroup", "RwUInt32");
         RegisterMsg (iMsgAudioGroupEnableGroup, "iMsgAudioGroupEnableGroup", "RwUInt32");
         RegisterMsg (iMsgAudioGroupDisableGroup, "iMsgAudioGroupDisableGroup", "RwUInt32");
         
         RWS_RETURNVOID();
      }

      /**
      *
      * Destructor for AudioGroup
      *
      * \see AudioGroup
      */
      AudioGroup::~AudioGroup(void)
      {
         RWS_FUNCTION("RWS::Audio::AudioGroup::~AudioGroup");
            
         UnRegisterMsg(iMsgAudioGroupFadeDownGroup);
         UnRegisterMsg(iMsgAudioGroupFadeUpGroup);
         UnRegisterMsg(iMsgAudioGroupDisableGroup);
         UnRegisterMsg(iMsgAudioGroupEnableGroup);
            
         UnLinkMsg(m_FadeDown);
         UnRegisterMsg(m_FadeDown);
            
         UnLinkMsg(m_FadeUp);
         UnRegisterMsg(m_FadeUp);
            
         UnLinkMsg(m_Disable);
         UnRegisterMsg(m_Disable);
            
         UnLinkMsg(m_Enable);
         UnRegisterMsg(m_Enable);
            
         RWS_RETURNVOID();
      }

      /**
      *        
      * Handle events passed into the behavior.
      *        
      * \param pMsg Standard message data object.
      *  
      */
      void AudioGroup::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::Audio::AudioGroup::HandleEvents");
         
         if (pMsg.Id == m_FadeDown)
         {
            CMsg message(iMsgAudioGroupFadeDownGroup, static_cast<void*>(&m_DictionaryId));
            SendMsg(message);               
         }
         else if (pMsg.Id == m_FadeUp)
         {
            CMsg message(iMsgAudioGroupFadeUpGroup, static_cast<void*>(&m_DictionaryId));
            SendMsg(message);               
         }
         else if (pMsg.Id == m_Enable)
         {
            CMsg message(iMsgAudioGroupEnableGroup, static_cast<void*>(&m_DictionaryId));
            SendMsg(message);               
         }
         else if (pMsg.Id == m_Disable)
         {
            CMsg message(iMsgAudioGroupDisableGroup, static_cast<void*>(&m_DictionaryId));
            SendMsg(message);               
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
      void AudioGroup::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::Audio::AudioGroup::HandleAttributes");
            
         CAttributeHandler::HandleAttributes(attr);
            
         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(AudioGroup));

         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
               case CMD_FadeDown:   
                  ReplaceLinkedMsg(m_FadeDown, attrIt->GetAs_RwChar_ptr(),0);
                  break; 

               case CMD_FadeUp:
                  ReplaceLinkedMsg(m_FadeUp, attrIt->GetAs_RwChar_ptr(),0); 
                  break;

               case CMD_Enable:
                  ReplaceLinkedMsg(m_Enable, attrIt->GetAs_RwChar_ptr(),0);
                  break;

               case CMD_Disable:
                  ReplaceLinkedMsg(m_Disable, attrIt->GetAs_RwChar_ptr(),0);
                  break;

               default:
                  break;
            }

            ++attrIt;
         }

         // get a pointer to the wave dictionary in the resource manager which is associated with this behavior.
         CAttributeCommandIterator sysattrIt(attr, RWS_CLASSID_OF(CSystemCommands));
         while (!sysattrIt.IsFinished())
         {
            switch (sysattrIt->GetCommandId())
            {
               case CSystemCommands::CMD_AttachResource:
               {
                  const RWS::RWSGUID *pResourceID = 0;
                  sysattrIt->GetCommandData(&pResourceID);

                  const RwChar *pResourceType;
                  const void *pObject = CResourceManager::FindById(pResourceID, &pResourceType);

                  if (pResourceType)
                  {
                     if (!rwstrcmp("rwaID_WAVEDICT", pResourceType))
                     {
                        m_DictionaryId = RwsAudio::FindDictionaryId(
                                                  static_cast<RwaWaveDict *>(const_cast<void*>(pObject)));
                     }                           
                  }
               }
               break;
            };

            ++sysattrIt;
         }
         
         RWS_RETURNVOID();
      }
   }
}
#endif
