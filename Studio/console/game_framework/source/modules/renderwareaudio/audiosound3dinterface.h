/*****************************************************************************
*
* File :     AudioSound3DInterface.h
*
* Abstract:  Interface class for 3d sounds.
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

#ifndef __AUDIOSOUND3DI_H__
#define __AUDIOSOUND3DI_H__

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
      * Interface class for playing sounds. Provides basic functionality for opening and playing 
      * a sound in a level.
      *
      * \see CAttributeHandler
      * \see CEventHandler
      * \see CSystemCommands
      *
      */
      class AudioSound3DInterface: public CSystemCommands, public CAttributeHandler, public LinearAllocationPolicy
      {
      public:
         AudioSound3DInterface(const CAttributePacket&);
         ~AudioSound3DInterface();
           
         virtual void HandleAttributes(const CAttributePacket& attr);     
         void SetVirtualVoicePosition(const RwMatrix* const pMatrix);            
        protected:
         RwaOsWave       *m_pOsWave;      /**< Pointer to selected sound.*/
         CAtomicPtr       m_pAtomic;      /**< Behaviors atomic.*/
         RwaVirtualVoice *m_pVoice;       /**< Pointer to a virtual voice that this entity uses
                                               to play a sound.*/
      };
   }
}
#endif
#endif
