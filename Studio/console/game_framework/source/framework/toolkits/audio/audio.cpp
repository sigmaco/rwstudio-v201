/*****************************************************************************
*
* File :     Audio.cpp
*
* Abstract : RenderWare Audio Integration.
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

#ifdef DOLPHIN
#include <dolphin.h>
#endif // DOLPHIN

//////////////////////////////////////////////////////////////////
//
// RenderWare Includes
//
#include <rtfsyst.h>
#include "rwacore.h"    /* RenderWare Audio core api.*/
#include "rwacoreobj.h" /* Core objects */
#include "rwaosobj.h"   /* Os objects */

// Left to its own devices, the MW linker will dead-strip the registrar ctors, so...
#ifdef __MWERKS__
#pragma force_active on
#pragma exceptions off // Turn off exceptions due to bug in MW Dwarf Generator for PS2, present in CW 3.04
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "audio.h"
#ifdef _WINDOWS
#include "framework/startup/win32/win.h"
#endif

namespace RWS
{
   namespace RwsAudio 
   {
      namespace
      {
         /*
         *
         * \ingroup RwsAudio
         *
         * This typedef is used to maintain virtual voice allocation in the framework.
         *
         */      
         typedef struct
         {
             RwaVirtualVoice *pVirtualVoice;  /**< Pointer to a virtual voice.*/
             RwBool           VoiceAlloc;     /**< Allocation status for virtual voices.*/
         }VirtualVoice;

         RwUInt32            g_RefCount = 0;
         RwaWaveDict        *g_pWaveDict[NUMGROUPS];
         RwaListener        *g_pListener;
         RwMemoryFunctions   g_MemFuncs = {RWSMalloc, RWSFree, RWSReAlloc, RWSCalloc}; 
         RwaOsOutput        *g_pOsOutput; 
         VirtualVoice        g_VirtualVoices[MAXVVOICES];
         RwaOsVoice         *g_pOsVoice[MAXVOICES];  
         RwaEnvironment      g_Environment = rwaENVIRONMENT_OFF;
         RwBool              g_ObjectsRegistered = FALSE;

         /*
         *
         * Create all virtual and hardware voices.
         *
         */        
         void CreateAllVoices( void )
         {
            RWS_FUNCTION("RWS::RwsAudio::NULL::CreateAllVoices")
 
            RwUInt32 i;
            RwBool Success;

             // Create the hardware voice pool.
            for (i = 0; i < MAXVOICES; i ++)
            {
               // Create the voice.
               g_pOsVoice[i] = RwaOsVoiceCreate(g_pOsOutput, rwaOBJFLAGS_INTERFACES, 0, 0);
               RWS_ASSERT(g_pOsVoice[i], "Invalid voice in hardware voice pool");
            }

            Success = RwaVoiceManagerRegister(reinterpret_cast<RwaObj*>(g_pOsOutput), 
                                              reinterpret_cast<RwaObj**>(g_pOsVoice), MAXVOICES);
            RWS_ASSERT(Success, "Can not register voice manager");
             
            // Create the virtual voices. 
            for (i = 0; i < MAXVVOICES; i ++)
            {
               // Create the virtual voice.
               g_VirtualVoices[i].pVirtualVoice = RwaVirtualVoiceCreate(0, 0, 0, 0, 0);
               RWS_ASSERT(g_VirtualVoices[i].pVirtualVoice, "Virtual voice " << i << " not created.");
                
               RwaVirtualVoiceSetGain(g_VirtualVoices[i].pVirtualVoice, 1.0f);
               RwaVirtualVoiceSetPan(g_VirtualVoices[i].pVirtualVoice, 0.0f);        
            }
             
            RWS_RETURNVOID();
         }

         /*
         *
         * Destroy all virtual and hardware voices.
         *
         */
         void DestroyAllVoices(void)
         {
            RWS_FUNCTION("RWS::RwsAudio::NULL::DestroyAllVoices")
               
            RwUInt32 i;
            
            // Destroy all virtual voices.
            for (i = 0; i < MAXVVOICES; i ++)
            {
               RwaVirtualVoiceDestroy(g_VirtualVoices[i].pVirtualVoice, 0, 0);
               g_VirtualVoices[i].VoiceAlloc = FALSE;
            }

            RwaVoiceManagerUnregister();
            
            // Can't be done until all virtual voices have been destroyed.
            for (i = 0; i < MAXVOICES; i ++)
            {
               RwaOsVoiceDestroy(g_pOsVoice[i], 0, 0);
            }
            
            RWS_RETURNVOID();
         }

         /*
         *
         * Set up RenderWare Audio memory callbacks. Then initialize RenderWare Audio and create the 3D listener 
         * object.
         *
         */
         void AudioInit(void)
         {
            RWS_FUNCTION("RWS::RwsAudio::NULL::AudioInit")
               
            RWS_PRE(g_RefCount == 0);
            
            memset (g_VirtualVoices, 0, sizeof(g_VirtualVoices));
            memset (g_pWaveDict, 0, sizeof(g_pWaveDict));
            memset (g_pOsVoice, 0, sizeof(g_pOsVoice));
            
               
#if defined (SKY) || defined (DOLPHIN)
            g_pOsOutput = RwaOsOutputCreate(rwaOBJFLAGS_FIXED | rwaOBJFLAGS_INTERFACES, 0, 0);
            RWS_ASSERT(g_pOsOutput, "Can not create output device"); 
#endif
               
#ifdef _WINDOWS
            RwaDXAudioOutputCreationParams creationParams;
            
            RwaObjDefGetDefaultCreationParams(RwaObjDefFindByUUID(&rwaDXAUDIOOUTPUTID, 0), &creationParams);
            
            creationParams.useSoftware  = FALSE;
            creationParams.fast3D       = FALSE;
            creationParams.enableReverb = TRUE; 
            
#ifdef RWS_DESIGN
            // In design mode it is useful to have the game window open at the same time as RwStudio,  by
            // setting the display flag to 0 you can edit sound properties and still hear the changes while 
            // the framework does not have the window focus.
            creationParams.display = 0;
#else
            creationParams.display = Win::GetMainWindow();
#endif
               
            g_pOsOutput = RwaDXAudioOutputCreate (0, &creationParams, 
                                    rwaOBJFLAGS_FIXED | rwaOBJFLAGS_INTERFACES, 0, 0);
            
            RWS_ASSERT(g_pOsOutput, "Can not create output device"); 
#endif 
                   
#ifdef _XBOX
            RwaXBoxOutputCreationParams creationParams;
            
            RwaObjDefGetDefaultCreationParams(RwaObjDefFindByUUID(&rwaXBOXOUTPUTID, 0), &creationParams);
            
            g_pOsOutput = RwaXBoxOutputCreate   (0, &creationParams, 
                                 rwaOBJFLAGS_FIXED | rwaOBJFLAGS_INTERFACES, 0, 0);
              
            RWS_ASSERT(g_pOsOutput, "Can not create output device"); 
#endif
            // Create the listener. 
            g_pListener = RwaListenerCreate(rwaOBJFLAGS_FIXED, 0, 0);
            RWS_ASSERT(g_pListener, "Can not create listener"); 
            
            // Attach the listener to the output object.
            RwaOsOutputSetListener(g_pOsOutput, g_pListener);
            
            RWS_RETURNVOID();
         }
         
         /*
         *
         * Close Audio Core. This will stop all the voices and then shut down the audio core.
         *
         * \see DestroyAllVoices
         *
         */
         void AudioClose(void)
         {
            RWS_FUNCTION("RWS::RwsAudio::NULL::AudioClose")
               
               RWS_PRE(g_RefCount >= 1);  
            
            DestroyAllVoices();

            // Destroy the output object.
            RwaOsOutputDestroy(g_pOsOutput, 0, 0);            
            RwaListenerDestroy(g_pListener, 0, 0);

            g_pOsOutput = 0;
            g_pListener = 0;
                       
            RWS_RETURNVOID();
         }
      } // namespace

      /**
      *
      * \ingroup RwsAudio
      *
      * Registers the RenderWare Audio objects. This needs to be called before the file system is
      * initialized on the PS2. This is because RenderWare Audio needs to setup the sifCMD buffers before
      * the file system initializes. If RenderWare Audio is disabled, the file system will set up the
      * sifCMD buffers instead.
      *
      * This function must be called before you call Open(), otherwise the RenderWare Audio core won't be
      * initialized.
      *
      * \return Return TRUE if the RenderWare Audio core is initialized successfully.
      *       
      * \see UnRegisterAudioObjects
      *
      */
      RwBool RegisterAudioObjects(void)
      {
         RWS_FUNCTION("RWS::RwsAudio::RegisterAudioObjects")

         RwBool Success;

         if (RwaCoreOpen(&RWSRCGLOBAL(memoryFuncs), TRUE))
         {

            // Register all default os independent objects.
            Success = RwaCoreRegisterDefaultObjects();
            RWS_ASSERT(Success, "Audio Core Register Failed.");

            // Register the os dependent objects. 
            Success = RwaOsObjectsRegisterUsingInterface();
            RWS_ASSERT(Success, "Audio Objects Register Failed.");

            g_ObjectsRegistered = TRUE;
         }
         else
         {
            g_ObjectsRegistered = FALSE;
         }

         RWS_RETURN (g_ObjectsRegistered);

      }

      /**
      *
      * \ingroup RwsAudio
      *
      * UnRegister the RenderWare Audio objects and shutdown the core. This function must be called
      * after Close() as the output object and voices must be destroyed first.
      *
      * \see RegisterAudioObjects
      *
      */
      void UnRegisterAudioObjects(void)
      {
         RWS_FUNCTION("RWS::RwsAudio::UnRegisterAudioObjects")

         RwBool Success = FALSE;

         RWS_ASSERT(g_ObjectsRegistered, "You can not un-register the audio objects as they have not  "
                                         "already been registered.");

         RWS_ASSERT(!g_pOsOutput, "Output object not destroyed. You must call Close() first.");
         
         Success = RwaCoreUnregisterDefaultObjects();   
         RWS_ASSERT(Success, "Audio Core UnRegister Failed."); 

         Success = RwaOsObjectsUnregister();
         RWS_ASSERT(Success, "Audio Objects UnRegister Failed.");

         RwaCoreClose();

         g_ObjectsRegistered = FALSE;

         RWS_RETURNVOID();
      }
      
      /**
      *
      * \ingroup RwsAudio
      *
      * Open the audio core. This will make sure the memory and file systems are 
      * initialized and that the sound output objects and voices are setup.
      *
      * \see Close
      *
      */
      void Open(void)
      {
         RWS_FUNCTION("RWS::RwsAudio::Open")
       
         RWS_ASSERT(g_ObjectsRegistered, "Your audio objects are not registered. "
                                         "Call UnRegisterAudioObjects() first.");

         if (g_RefCount == 0)
         {
            AudioInit();
            CreateAllVoices();           
         } 
  
         g_RefCount++;
  
         RWS_RETURNVOID();
      }

      /**
      *
      * \ingroup RwsAudio
      *
      * Shut down audio core. This will stop and clear all the voices.
      *
      * \see AudioClose
      * \see Open
      *
      */
      void Close(void)
      {
         RWS_FUNCTION("RWS::RwsAudio::Close")
            
         RWS_PRE(g_RefCount != 0);
         
         if (g_RefCount == 1)
         {
            AudioClose();
         }
         
         g_RefCount--;
         
         RWS_RETURNVOID();
      }
               
      /**
      *
      * \ingroup RwsAudio
      *
      * Remove a RenderWare Audio wave dictionary. First any voices that are still playing must be stopped 
      * and then destroyed. This is done by calling StopAllVoices and DestroyAllVoices. When the 
      * dictionary has been destroyed, the hardware voices and virtual voices are recreated with the
      * call to CreateAllVoices.
      *
      * \param pDict The dictionary to be unloaded.
      */
      void RemoveDictionary(const RwaWaveDict* const pDict)
      {
         RWS_FUNCTION("RWS::RwsAudio::RemoveDictionary")
            
         RWS_PRE(g_RefCount >= 1);  
         RWS_PRE(pDict);
         
         RwUInt32 i;
         
         StopAllVirtualVoices();
         RwaCoreUpdate();
         
         DestroyAllVoices();
         
         for (i = 0; i < NUMGROUPS; i++)
         {
            if (g_pWaveDict[i] == pDict)
            {
               RwaWaveDictDestroy(g_pWaveDict[i]);
               g_pWaveDict[i] = 0;
            }
         }
         
         CreateAllVoices();
         

         RWS_RETURNVOID();
      }
             
      /**
      *
      * \ingroup RwsAudio
      *
      * Add a dictionary to the list of dictionaries. It is possible to have more than one wave dictionary 
      * attached to a level. When these dictionaries get loaded, they are put into this list.
      *
      * \param pDict A pointer to a RenderWare Audio wave dictionary.
      *
      */
      void AddDictionary(RwaWaveDict* const pDict)
      {
         RWS_FUNCTION ("RWS::RwsAudio::AddDictionary")
            
         RWS_PRE(g_RefCount >= 1);  
         RWS_PRE(pDict);
         
         RwUInt32 i = 0;
         
         while (i < NUMGROUPS)
         {
            if (g_pWaveDict[i] == 0)
            {
               g_pWaveDict[i] = pDict;
               RWS_RETURNVOID();
            }
            
            i++;
         }
         
         RWS_ASSERTFAIL("Unable to add dictionary, increase RWS::Audio::NUMGROUPS, currently set at "
               << NUMGROUPS);
         
         RWS_RETURNVOID();
      }
      
      /**
      *
      * \ingroup RwsAudio
      *
      * Fades the environment to a specified gain.
      *
      * \param Destination The destination gain value.
      * \param Step The amount to add or subtract from the current reverb gain.
      *
      * \return Return TRUE when the fade has finished. FALSE when fade is in progress.
      *
      */
      RwBool FadeEnvironment(const RwInt32 Destination, const RwUInt32 Step)
      {
         RWS_FUNCTION("RWS::RwsAudio::FadeEnvironment")
            
         RWS_PRE(g_RefCount >= 1);  
         RWS_PRE(Step > 0);
         RWS_PRE(Destination >= -1000);
         
         // Fade Down.
         if (Destination < g_Environment.room) 
         {
            g_Environment.room = g_Environment.room - Step;
            
            // If gain goes out of range, then clamp.
            if (g_Environment.room < Destination) 
            {
               g_Environment.room = Destination;
            }
         }
         
         // Fade Up
         if (Destination > g_Environment.room) 
         {
            g_Environment.room = g_Environment.room + Step;
            
            // If gain goes out of range, then clamp.
            if (g_Environment.room >= Destination) 
            {
               g_Environment.room = Destination;
            }
         }
         
         RwaOsOutputSetEnvironment(g_pOsOutput, &g_Environment);
         
         RWS_RETURN((g_Environment.room == Destination));
      }

      /**
      *
      * \ingroup RwsAudio
      *
      * Fades the Output object to a specified gain. 
      *
      * \param Destination The destination gain value.
      * \param Step The amount to add or subtract from the current gain.
      *
      * \return Return TRUE when the fade has finished. FALSE when fade is in progress.
      *
      * \note See the RenderWare Audio api reference for RwaOsOutput.
      *
      */
      RwBool FadeOutputObject(const RwReal Destination, const RwReal Step)
      {
         RWS_FUNCTION("RWS::RwsAudio::FadeOutputObject")
            
         RWS_PRE(g_RefCount >= 1);  
         RWS_PRE(Step > 0);
         RWS_PRE(Destination >= 0);
         
         RwReal CurrentGain;
         
         CurrentGain = RwaOsOutputGetGain(g_pOsOutput);
         
         // Fade Down
         if (Destination < CurrentGain) 
         {
            CurrentGain = CurrentGain - Step;
            
            // If gain goes out of range,  then clamp.
            if (CurrentGain < Destination) 
            {
               CurrentGain = Destination;
            }
         }
         
         // Fade Up
         if (Destination > CurrentGain) 
         {
            CurrentGain = CurrentGain + Step;
            
            // If gain goes out of range, then clamp.
            if (CurrentGain >= Destination) 
            {
               CurrentGain = Destination;
            }
         }
         
         RwaOsOutputSetGain (g_pOsOutput, CurrentGain);
         
         RWS_RETURN((CurrentGain == Destination));
      }

      /**
      *
      * \ingroup RwsAudio
      *
      * Find a named wave.
      *
      * \param pName Pointer to a string containing the name of the wave object to find.
      *
      * \return Return pointer to an RwaOsWave, or 0 if wave not found.
      *   
      */
      RwaOsWave *FindWave(const RwChar* const pName)
      {
         RWS_FUNCTION("RWS::RwsAudio::FindWave")
            
         RWS_PRE(g_RefCount >= 1);
         
         RwaOsWave *pWave = 0;
         
         RwUInt32 i = 0;
         
         while (i < NUMGROUPS)
         {
            if (g_pWaveDict[i] != 0)
            {
               pWave = reinterpret_cast<RwaOsWave*>(RwaWaveDictFindWaveByName  (g_pWaveDict[i], pName));
               
               if (pWave) 
               {
                  break;
               }
            }
            
            i++;
         }
         
         // If wave is 0, then the sound could not be found in any of the groups.
         RWS_ASSERT (pWave, "The sound <" << pName << "> was not found in the wave dictionary.");
         
         RWS_RETURN (pWave);
      }

      /**
      *
      * \ingroup RwsAudio
      *
      * Find named wave and return its dictionary id number.
      *
      * \param pName A pointer to a string containing the name of the wave to find.
      *
      * \return Return id of the dictionary which contains the named wave. Return -1 if wave name is not 
      * in any dictionary.
      *
      */
      DictionaryId FindDictionaryId(const RwChar* const pName)
      {
         RWS_FUNCTION("RWS::RwsAudio::FindDictionaryId")
            
         RWS_PRE(g_RefCount >= 1);  
         
         RwaOsWave *pWave;
         
         for (RwUInt32 i = 0; i < NUMGROUPS; i++)
         {
            if (g_pWaveDict[i] != 0)
            {
               pWave = reinterpret_cast<RwaOsWave*>(RwaWaveDictFindWaveByName  (g_pWaveDict[i], pName));
               
               if (pWave)
               {
                  RWS_RETURN (i);
               }
            }
         }
         
         RWS_RETURN (-1);
      }

      /**
      *
      * \ingroup RwsAudio
      *
      * Find id of a wave dictionary.
      *
      * \param pDict A pointer to a wave dictionary.
      *
      * \return Return id of dictionary. Return -1 if dictionary has not been added \ref AddDictionary.
      *
      *
      */
      DictionaryId FindDictionaryId(const RwaWaveDict* const pDict)
      {
         RWS_FUNCTION("RWS::RwsAudio::FindDictionaryId")
            
         RWS_PRE(g_RefCount >= 1);  
         
         for (RwUInt32 i = 0; i < NUMGROUPS; i++)
         {
            if (g_pWaveDict[i] == pDict)
            {
               RWS_RETURN(i);
            }
         }
         
         RWS_RETURN (-1);
      }

      /**
      *
      * \ingroup RwsAudio
      *
      * Allocate next available virtual voice.
      *
      * \return Return pointer to a virtual voice,  NULL otherwise.
      *
      */
      RwaVirtualVoice *AllocateVirtualVoice(void)
      {
         RWS_FUNCTION("RWS::RwsAudio::AllocateVirtualVoice")
            
         RWS_PRE(g_RefCount >= 1);
         
         RwUInt32 i = 0;
         
         while (i < MAXVVOICES)
         {
            if (g_VirtualVoices[i].VoiceAlloc == FALSE)
            {
               g_VirtualVoices[i].VoiceAlloc = TRUE;
               RWS_RETURN(g_VirtualVoices[i].pVirtualVoice);
            }
            
            i++;
         }
         
         RWS_RETURN(0);
      }

      /**
      *
      * \ingroup RwsAudio
      *
      * Free virtual voice.
      *
      * \param pVirtualVoice Pointer to a virtual voice.
      *
      */
      void FreeVirtualVoice(RwaVirtualVoice* const pVirtualVoice)
      {
         RWS_FUNCTION("RWS::RwsAudio::FreeVirtualVoice")
            
         RWS_PRE(g_RefCount >= 1);  
         RWS_PRE(pVirtualVoice);
         
         RwUInt32 i = 0;
         
         while (i < MAXVVOICES)
         {
            if (g_VirtualVoices[i].pVirtualVoice == pVirtualVoice)
            {
               // Make sure virtual voice has stopped playing.
               RwaVirtualVoiceSetTrigger(g_VirtualVoices[i].pVirtualVoice, FALSE);
               g_VirtualVoices[i].VoiceAlloc = FALSE;
               
               // We have destroyed the voice, no need to continue searching.
               break;
            }
            
            i++;
         }
         
         RWS_RETURNVOID();
      }

      /**
      *
      * \ingroup RwsAudio
      *
      * Stop all virtual voices from playing.
      *
      */
      void StopAllVirtualVoices(void)
      {
         RWS_FUNCTION("RWS::RwsAudio::StopAllVirtualVoices")
            
         RWS_PRE(g_RefCount >= 1);  
         
         for (RwUInt32 i = 0; i < MAXVVOICES; i++)
         {
            RwaVirtualVoiceSetTrigger(g_VirtualVoices[i].pVirtualVoice, FALSE);
         }
         
         RWS_RETURNVOID();
      }

      /**
      *
      * \ingroup RwsAudio
      *
      * Fades the virtual voice to a specified gain.
      *
      * \param Destination The destination gain value.
      * \param Step The amount to add or subtract from the current gain.
      * \param pVoice The virtual voice to be faded.
      *
      * \return Return TRUE when the fade has finished. FALSE whilst fade is in progress.
      *
      */
      RwBool FadeVirtualVoice( const RwReal Destination, const RwReal Step, const RwaVirtualVoice* const pVoice)
      {
         RWS_FUNCTION("RWS::RwsAudio::FadeVirtualVoice")
            
         RWS_PRE(g_RefCount >= 1);
         RWS_PRE(pVoice);
         RWS_PRE(Step > 0);
         RWS_PRE(Destination >= 0.0f);
         
         RwReal CurrentGain = RwaVirtualVoiceGetGain (const_cast<RwaVirtualVoice*>(pVoice));
         
         // Fade Down
         if (Destination < CurrentGain ) 
         {
            CurrentGain = CurrentGain - Step;
            
            // If gain goes out of range,  then clamp.
            if (CurrentGain < Destination) 
            {
               CurrentGain = Destination;
            }
         }
         
         // Fade Up 
         if (Destination > CurrentGain ) 
         {
            CurrentGain = CurrentGain + Step;
            
            // If gain goes out of range, then clamp
            if (CurrentGain >= Destination) 
            {
               CurrentGain = Destination;
            }
         }
         
         RwaVirtualVoiceSetGain(const_cast<RwaVirtualVoice*>(pVoice),  CurrentGain);
         
         RWS_RETURN((CurrentGain == Destination));
      }

      /**
      *
      * \ingroup RwsAudio
      *
      * Return a pointer to the output object.
      *
      * \return Return pointer to RenderWare Audio output object.
      *
      * \note See the RenderWare Audio api reference for RwaOsOutput.
      *        
      */
      RwaOsOutput *GetOutputObject(void)
      {
         RWS_FUNCTION("RWS::RwsAudio::GetOutputObject")
            
         RWS_PRE(g_RefCount >= 1);  
         
         RWS_RETURN(g_pOsOutput);
      }
   
      /**
      *
      * \ingroup RwsAudio
      *
      * Get a pointer to the 3D listener.
      *
      * \return Returns a pointer to the RenderWare Audio 3D listener.
      *
      * \note See the RenderWare Audio api reference for RwaListener.
      *
      */
      RwaListener *GetListener(void)
      {
         RWS_FUNCTION("RWS::RwsAudio::GetListener")

         RWS_PRE(g_RefCount >= 1);
         
         RWS_RETURN(g_pListener);
      }

      /**
      *
      * \ingroup RwsAudio
      *
      * Get a pointer to a RenderWare Audio wave dictionary.
      *
      * \param Id of the group used to obtain the pointer to the RenderWare Audio wave dictionary.
      *
      * \return Returns a pointer to a wave dictionary.
      *
      * \note See the RenderWare Audio api reference for RwaWaveDict.
      *
      */
      RwaWaveDict *GetDictionary(const DictionaryId Id)
      {
         RWS_FUNCTION("RWS::RwsAudio::GetDictionary")

         RWS_PRE(g_RefCount >= 1);
         
         RWS_RETURN(g_pWaveDict[Id]);
      }

      /**
      *
      * \ingroup RwsAudio
      *
      * Get a pointer to a RenderWare Audio Environment Structure.
      *
      * \return Returns a pointer to a RenderWare Audio RwaEnvironment structure.
      *
      * \note See the RenderWare Audio api reference for RwaEnvironment.
      */
      RwaEnvironment *GetEnvironment(void)
      {
         RWS_FUNCTION("RWS::RwsAudio::GetEnvironment")

         RWS_PRE(g_RefCount >= 1);  
         
         RWS_RETURN(&g_Environment);
      }
   }
}
#endif// WITH_AUDIO

