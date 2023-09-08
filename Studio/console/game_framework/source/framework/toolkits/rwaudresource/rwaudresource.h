/*****************************************************************************
*
* File :     rwaudresource.h
*
* Abstract : RenderWare Audio resource handler.
*
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

#ifndef _AUDIORESOURCE_H_
#define _AUDIORESOURCE_H_

#ifdef WITH_AUDIO

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/core/resourcemanager/cresourcehandler.h"
#include "framework/core/eventhandler/ceventhandler.h"

#ifdef WITH_AUDIO
#include "framework/toolkits/audio/audio.h"
#endif

namespace RWS
{
   RWS_DEFINE_EVENT(iMsgAudioDictionaryDelete,
      "RwUInt32 GroupIndex",
      "Sent when the audio resource handler tries to delete a dictionary from the group table."
      " AudioSound3D behaviors needs to be notified of the deletion so they can detach them selves"
      " from the dictionary.");

   /**
   *
   * \ingroup RwAudResource
   *
   *  Resource handler for handling RenderWare Audio Resources.
   *
   */
   class CAudioResource: public CResourceHandler
   {
      public:

         virtual void Open(void);

         virtual void Close(void);

         virtual void *Load(  const RwChar *psName,
                              const RwChar *psType,
                              const RwChar *psResourcePath,
                              RwStream* pStream,
                              RwUInt32 uiStreamSize,
                              RwUInt32 &uiResourceSize);

         virtual RwBool UnLoad(const RwChar *pStrType, void *pResource);

         virtual RwBool IsHandled(const RwChar *psType);
      private:
         RwaWaveDict* WaveDictStreamRead(const RwStream* const pRwStream);
   };
}

#endif
#endif
