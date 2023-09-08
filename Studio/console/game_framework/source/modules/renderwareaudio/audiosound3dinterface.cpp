/*****************************************************************************
*
* File :     AudioSound3DInterface.cpp
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
#include "framework/core/resourcemanager/cresourcemanager.h"
#include "framework/toolkits/world/factory.h"
#include "framework/mainloop/logic.h"
#include "framework/toolkits/audio/audio.h"
#include "framework/toolkits/world/clevel.h"
#include "audiosound3dinterface.h"

namespace RWS
{
   namespace Audio
   {
      /**
      *
      * Constructor for 3D sound interface.
      *
      * \see ~AudioSound3DInterface
      *
      */
      AudioSound3DInterface::AudioSound3DInterface(const CAttributePacket& attr):
            m_pOsWave(0) 
      {
         RWS_FUNCTION("RWS::Audio::AudioSound3DInterface::AudioSound3DInterface")
                      
         // Can't use initializer ('this' isn't valid) so...
         m_pAtomic = CreateAtomicInWorldFromResource(attr, 0);
         RWS_ASSERT(m_pAtomic, "Failed to create atomic");

         m_pAtomic.HandleSystemCommands(attr);
         m_pVoice = RwsAudio::AllocateVirtualVoice();
         RWS_ASSERT (m_pVoice, "Run out of virtual voices, increase MAXVVOICES.");

         RwaVirtualVoiceSetLoop(m_pVoice, FALSE);
         RwaVirtualVoiceSetGain(m_pVoice, 30.0f);
         RwaVirtualVoiceSetPan(m_pVoice, 0.5f);

         RWS_RETURNVOID()
      }
    
      /**
      *
      * Destroy Audio 3D object.
      *
      * \see AudioSound3DInterface
      */
      AudioSound3DInterface::~AudioSound3DInterface()
      {
         RWS_FUNCTION("RWS::Audio::AudioSound3DInterface::~AudioSound3DInterface")
                  
         RwsAudio::FreeVirtualVoice(m_pVoice);
         m_pOsWave = 0;
               
         RWS_RETURNVOID()        
      }        
    
      /**
      *
      * Handle attribute updates.
      *
      * \param attr The standard attribute packets.
      *  
      */
      void AudioSound3DInterface::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::Audio::AudioSound3DInterface::HandleAttributes")
           
         CAttributeHandler::HandleAttributes(attr);
        
         // Initialize contained class first
         m_pAtomic.HandleSystemCommands(attr); 

         CAttributeCommandIterator attrIt2(attr, RWS_CLASSID_OF(CSystemCommands));
         while (!attrIt2.IsFinished())
         {
            switch (attrIt2->GetCommandId())
            {
               case CSystemCommands::CMD_LoadMatrix:
               {
                  RwFrame* pFrame = RpAtomicGetFrame(m_pAtomic.ptr());
                  if (pFrame) CSystemCommands::UpdateFrame(*pFrame, *attrIt2);

                  SetVirtualVoicePosition(RwFrameGetLTM(RpAtomicGetFrame(m_pAtomic.ptr())));
               }
               break;
            }
                
            ++attrIt2;
         }
        
         RWS_RETURNVOID()
      }

      /**
      *
      * Set virtual voice position based on incoming message data.
      *
      * \param pMatrix The behaviors atomic gets set to this matrix.
      *
      */
      void AudioSound3DInterface::SetVirtualVoicePosition(const RwMatrix* const pMatrix)
      {
         RWS_FUNCTION("RWS::Audio::AudioSound3DInterface::SetVirtualVoicePosition")
            
         // If we have a valid matrix here,  then the sound is linked to an event,  to we want
         // to apply the matrix to the atomics frame.
         if (pMatrix)
         {
            RwFrame *pFrame = RpAtomicGetFrame(m_pAtomic.ptr());
            RwFrameTransform(pFrame, pMatrix, rwCOMBINEREPLACE);
         }
            
         // If we don't have a frame then use the atomics position.
         RwaVirtualVoiceSetPos(m_pVoice, RwMatrixGetPos(RwFrameGetMatrix(RpAtomicGetFrame(m_pAtomic.ptr()))));
            
         RWS_RETURNVOID();
      }
   }
}
#endif // WITH_AUDIO
