/*****************************************************************************
*
* File :     AudioStream.h
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

#ifndef __AUDIOSTREAM_H__
#define __AUDIOSTREAM_H__

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
      * Behavior to play streamed sound file.
      *
      * \see CAttributeHandler
      * \see CEventHandler
      * \see CSystemCommands
      *
      */
      class AudioStream: public CSystemCommands, public CAttributeHandler , public CEventHandler, 
                         public LinearAllocationPolicy
      {
      public:
         RWS_MAKENEWCLASS(AudioStream);
         RWS_DECLARE_CLASSID(AudioStream);
         RWS_CATEGORY("Audio");
         RWS_DESCRIPTION("Stream Player", "Audio stream player.");

         AudioStream(const CAttributePacket&);
         ~AudioStream();
            
         RWS_BEGIN_COMMANDS
            RWS_SEPARATOR("Play/Stop Events", 1)
            RWS_MESSAGE( CMD_m_play,      
                         "Play stream.",
                         "Play a loaded stream file.",
                         RECEIVE,
                         0,
                         0)

            RWS_MESSAGE( CMD_m_stop,      
                         "Stop stream",
                         "Stop a stream from playing.",
                         RECEIVE, 
                         0, 
                         0)

            RWS_ATTRIBUTE( CMD_selectText,
                           "Select stream",
                           "Select stream. You must enter a filename for the stream file.",
                           EDIT,
                           RwChar,
                           DEFAULT("Enter Filename Here ..."))
         RWS_END_COMMANDS;

         virtual void HandleAttributes(const CAttributePacket& attr);
         virtual void HandleEvents(CMsg &pMsg);
         void OpenStream(const RwChar* const pName);
      private:
         CEventId             m_Play;              /**< Event to start a stream playing.*/ 
         CEventId             m_Stop;              /**< Event to stop a stream from playing.*/
         RwaOsStream         *m_pStream;           /**< Pointer to a RenderWare Audio stream.*/
         RwChar              *m_pStreamName;       /**< File name for stream to be loaded.*/
         RwaStreamMediaParams m_MediaParams;       /**< Media parameters for RenderWare Audio streams.
                                                        See RenderWare Audio api reference for 
                                                        information on RwaStreamMediaParams.*/
      };
   }
}
#endif
#endif
