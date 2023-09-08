/*****************************************************************************
*
* File :     rwaudresource.cpp
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
#include "precomp.h"

#ifdef WITH_AUDIO

// Left to its own devices, the MW linker will dead-strip the registrar ctors, so...
#ifdef __MWERKS__
#pragma force_active on
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "rwaudresource.h"
#include "framework/core/macros/debugmacros.h"

namespace RWS
{
   /**
   *
   * When deleting a dictionary, you have to make sure no behaviors are referencing waves 
   * in that dictionary. If the AudioSound3D behavior is using a wave from the dictionary, 
   * then it must stop playing it and dereference itself from the wave.
   *
   * This message gets sent out when the wave dictionary resource get unloaded from the
   * resource manager.
   *
   */
   CEventId iMsgAudioDictionaryDelete;

   /**
   *
   *  Open CAudioResource resource handler.
   *
   */
   void CAudioResource::Open(void)
   {
      RWS_FUNCTION("RWS::CAudioResource::Open");

#ifdef RWS_DESIGN
      CEventHandler::RegisterMsg(iMsgAudioDictionaryDelete,
         iMsgAudioDictionaryDeleteStr, "RwUInt32 GroupIndex");
#endif

      RWS_RETURNVOID();
   }

   /**
   *
   *  Close CAudioResource resource handler.
   *
   */
   void CAudioResource::Close(void)
   {
      RWS_FUNCTION("RWS::CAudioResource::Close");

#ifdef RWS_DESIGN
      CEventHandler::UnRegisterMsg(iMsgAudioDictionaryDelete);
#endif

      RWS_RETURNVOID();
   }


   /**
   *
   * Open a RenderWare Audio wave dictionary from a stream.
   *
   * \param pRwStream Pointer to a RenderWare audio stream.
   *
   * \return Returns pointer to a valid RenderWare Audio wave dictionary, otherwise 0. 
   *
   * \see AddDictionary
   *
   */
   RwaWaveDict* CAudioResource::WaveDictStreamRead(const RwStream* const pRwStream)
   {
      RWS_FUNCTION("RWS::CAudioResource::WaveDictStreamRead")

      RwaWaveDict *pDictionary;
      void        *pUpLoadBuffer; // Memory area for uploading waves.
      void        *pUpLoadAlign;  // Aligned area for uploading.
      
      // Allocate memory for the upload buffer.
      pUpLoadBuffer = RwCalloc(RwsAudio::WAVEUPLOADBUFFSIZE + RwsAudio::WAVEDATAALIGNMENT, 1, rwMEMHINTDUR_FUNCTION);

      // Align the read buffer pointer.
      pUpLoadAlign = (void*)rwaAlign(pUpLoadBuffer, RwsAudio::WAVEDATAALIGNMENT);

      // Point the wave dictionary loader to use this buffer.
      RwaWaveDictSetStreamIOBuffer(pUpLoadAlign, RwsAudio::WAVEUPLOADBUFFSIZE);
    
      // Find the wave dictionary in the stream. 
      if (RwStreamFindChunk(const_cast<RwStream*>(pRwStream), rwaID_WAVEDICT, 0, 0))
      {          
         pDictionary = RwaWaveDictStreamReadUsingObject(0, const_cast<RwStream*>(pRwStream), 
                                                        reinterpret_cast<RwaObj*>(RwsAudio::GetOutputObject()), 0, 0);

         RWS_ASSERT(pDictionary, "Wave Dictionary Invalid");
      }

      RwFree (pUpLoadBuffer);

      RWS_RETURN(pDictionary)
   }

   /**
   *
   * Load a RenderWare Audio resource.
   *
   * \param psName Pointer to a string defining the resource name.
   *
   * \param psType Pointer to a string defining the resource type.
   *
   * \param psResourcePath Pointer to a string containing a path where files referenced
   *  by this file can be found.
   *
   * \param pStream Pointer to a RenderWare stream.
   *
   * \param uiStreamSize Size of the file associated with this resource.
   *
   * \returns If successful returns a pointer to the loaded resource, otherwise 0.
   *
   */
   void *CAudioResource::Load(const RwChar *psName,
                              const RwChar *psType,
                              const RwChar *psResourcePath,
                              RwStream* pStream,
                              RwUInt32 uiStreamSize,
                              RwUInt32 &uiResourceSize)
   {
      RWS_FUNCTION("RWS::CAudioResource::Load");
      
      RWS_PRE(pStream);
      
      if (uiStreamSize == 0)
      {
         RWS_RETURN(0);
      }

      RwaWaveDict* temp;

      if (!rwstrcmp(psType, RWSTRING("rwaID_WAVEDICT")))
      {     
         temp = WaveDictStreamRead(pStream);
         RwsAudio::AddDictionary(temp);

         RWS_RETURN (temp)
      }
   
      RWS_RETURN(0);
   }

   /**
   *
   * Unload RenderWare Audio resource.
   *
   * \param pStrType Pointer to a string defining the resource type.
   *
   * \param pResource Pointer to the resource.
   *
   * \returns True if the resource was successfully unloaded otherwise false.
   *
   */
   RwBool CAudioResource::UnLoad(const RwChar *pStrType, void *pResource)
   {
      RWS_FUNCTION("RWS::CAudioResource::UnLoad");
      RwaWaveDict *dict = static_cast<RwaWaveDict*>(pResource);

#ifdef RWS_DESIGN
      // When we delete a dictionary from a group, we have to make sure no
      // behaviors are referencing waves in that group.
      //
      RwUInt32 DictionaryId = RwsAudio::FindDictionaryId(dict);

      CMsg msg (iMsgAudioDictionaryDelete, reinterpret_cast<void*> (DictionaryId));
         
      SendMsg(msg);
#endif

      RwsAudio::RemoveDictionary(dict);

      RWS_RETURN(TRUE);
   }

   /**
   *
   * Check if this resource is handled by this resource handler.
   *
   * \param psType Pointer to a string defining the resource type.
   *
   * \returns TRUE if the resource is of type "rwaID_WAVEDICT" otherwise returns FALSE.
   *
   */
   RwBool CAudioResource::IsHandled(const RwChar *psType)
   {
      RWS_FUNCTION("RWS::CAudioResource::IsHandled");
      RWS_PRE(psType);

      if (!rwstrcmp(psType, RWSTRING("rwaID_WAVEDICT")))
      {
         RWS_RETURN(TRUE);
      }

      RWS_RETURN(FALSE);
   }

   /*
   *
   * Create an instance of the Audio Resource handler, this is required in order to register the handler.
   *
   */
   namespace { CAudioResource gCAudioResource; }
}
#endif

