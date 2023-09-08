/*****************************************************************************
*
* File :       AudioGroup.h
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
#ifndef __AUDIOGROUP_H__
#define __AUDIOGROUP_H__

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
      extern CEventId        iMsgAudioGroupFadeDownGroup;    /**< Fade down event.*/ 
      extern CEventId        iMsgAudioGroupFadeUpGroup;      /**< Fade up event.*/ 
      extern CEventId        iMsgAudioGroupDisableGroup;     /**< Disable group event.*/ 
      extern CEventId        iMsgAudioGroupEnableGroup;      /**< Enable group event.*/ 

      /**
      *
      * \ingroup Mod_Audio 
      *
      * This behavior lets you assign a RenderWare Audio wave dictionary as a separate group. You may want to 
      * have a group for all your menu sounds, another group for gun sounds, and monster sounds etc. By putting
      * them into a separate groups, you could fade down all the menu sounds and then fade up the games sounds.
      *
      * To put dictionaries into a group, need to drop an AudioGroup behavior into the level, then attach a 
      * wave dictionary as an asset of that entity.
      * 
      * \see CAttributeHandler
      * \see CEventHandler
      * \see CSystemCommands
      *
      */
      class AudioGroup : public CSystemCommands, public CAttributeHandler , public CEventHandler, 
                         public LinearAllocationPolicy
      {
      public:
         RWS_MAKENEWCLASS(AudioGroup);
         RWS_DECLARE_CLASSID(AudioGroup);
         RWS_CATEGORY("Audio");
         RWS_DESCRIPTION("AudioGroup", "Define Wave dictionary group");

         RWS_BEGIN_COMMANDS   
                RWS_SEPARATOR("Group Voice Fade", 1)
                RWS_MESSAGE  (CMD_FadeUp,
                              "Fade group up",
                              "Event to fade group up.",
                              RECEIVE ,
                              RwUInt32*,
                              0)

                RWS_MESSAGE  (CMD_FadeDown,
                              "Fade group down",
                              "Event to fade group down.",
                              RECEIVE,
                              RwUInt32*,
                              0)

                RWS_SEPARATOR("Group Voice Enable/Disable", 1)
                RWS_MESSAGE  (CMD_Enable,
                              "Enable group voices.",
                              "Enable all voices attached to this group.",
                              RECEIVE ,
                              RwUInt32*,
                              0)

                RWS_MESSAGE  (CMD_Disable,
                              "Disable group voices",
                              "Disable all voices attached to this group.",
                              RECEIVE,
                              RwUInt32*,
                              0)

         RWS_END_COMMANDS;    

         AudioGroup(const CAttributePacket& attr);
         ~AudioGroup(void);

         virtual void HandleEvents(CMsg &pMsg);
         virtual void HandleAttributes(const CAttributePacket& attr);

      private:
         RwsAudio::DictionaryId    m_DictionaryId;   /**< Id of wave dictionary.*/
         RwaWaveDict     *m_pDictionary;             /**< Pointer to a RenderWare Audio wave dictionary in the 
                                                          resource manager.*/
         CEventId        m_FadeDown;                 /**< Fade down event.*/ 
         CEventId        m_FadeUp;                   /**< Fade up event.*/ 
         CEventId        m_Disable;                  /**< Disable group event.*/ 
         CEventId        m_Enable;                   /**< Enable group event.*/ 
      };
   }
}

#endif
#endif

