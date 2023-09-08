/*****************************************************************************
*
* File :     Audio.h
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

#ifndef __AUDIO_H__
#define __AUDIO_H__

#ifdef WITH_AUDIO


//////////////////////////////////////////////////////////////////
//
// RenderWare Includes
//
#include <rtfsyst.h>
#include <rwcore.h>

#ifdef _MSC_VER
#pragma comment (lib, "rwcore.lib")

#ifdef WITH_AUDIO
#pragma comment (lib, "rwacore.lib")

#ifdef _WINDOWS
#pragma comment (lib, "rwadxaudio.lib")

#pragma comment (lib, "dsound.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dmoguids.lib")
#pragma comment (lib, "strmiids.lib")
#pragma comment (lib, "winmm.lib")
#endif

#ifdef _XBOX
#pragma comment (lib, "rwaxbox.lib")

#ifdef _DEBUG
#pragma comment (lib, "dsoundd.lib")
#pragma comment (lib, "dmusicd.lib")
#else
#pragma comment (lib, "dsound.lib")
#pragma comment (lib, "dmusic.lib")
#endif
#endif
#endif
#endif

#ifdef DOLPHIN
#include <dolphin.h>
#endif // DOLPHIN

#include <rwcore.h>

#include "rwacore.h"    /* RenderWare Audio core api.*/
#include "rwacoreobj.h" /* Core objects.*/
#include "rwaosobj.h"   /* Os objects.*/

#ifdef SKY
#include "rwaspu2.h"    /* Platform specific objects */
#endif

#ifdef _WINDOWS
#include "rwadxaudio.h"
#endif

#ifdef _XBOX
#include "rwaxbox.h"
#endif

#ifdef DOLPHIN
#include "rwaax.h"
#endif
//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/core/macros/debugmacros.h"

namespace RWS
{
   /**
   *
   *  \ingroup RwsAudio
   *  \page RwsAudioOverview Overview
   *
   * This toolkit integrates RenderWare Studio with RenderWare Audio.  
   * Using this toolkit and behaviors, you can attach wave dictionaries 
   * to your level and then play sounds contained in them.
   *
   * To create wave dictionaries, you must use the Audio Management Tool that comes 
   * with RenderWare Audio. For instructions on how to create wave 
   * dictionaries, see the RenderWare Audio help.
   *
   * \note
   *
   * By default, the audio toolkit is not built into the RenderWare Studio Framework.
   * To include it, see the 'Building the Audio Toolkit' section below.
   *
   * <BR> <B>PLAYING SOUNDS</B>
   *
   * To play a sound in the framework, drag an object into the scene and place 
   * the AudioGlobalMixer behavior onto it.  Then add a RenderWare Audio wave dictionary asset 
   * to the level. Make sure the asset type is set to rwaID_WAVEDICT. Then drop an 
   * AudioSound3D behavior onto an object in the level. To get the behavior to reference a sound
   * in a wave dictionary, you must type in the sound name in the attributes. This sound name 
   * must correspond with a name in the dictionary.
   *
   * You can play sounds in either 3D or 2D.  This is set when you create the dictionary 
   * in the RWAEditor tool. When the sound has been set to 3D, the sound you place 
   * in the level will be positional, so the place you drop the sound 
   * in the scene is were you would hear it when the framework is running.
   *
   * When using 3D sounds, you can change the global roll-off factor, distance factor 
   * and Doppler for the scene in the AudioGlobalMixer behavior.
   *
   * If the sound has been set to 2D, the sound will not be positional, and will be 
   * audible wherever you are in the level.
   *
   *
   * <BR> <B>STREAMED AUDIO</B>
   *
   * To play streamed audio you need to specify the stream file to use in the AudioStream 
   * behavior, there is a text box for this, for example "host0:c:/examples/mopdels/music.rws". 
   *
   * This will send the path name to the console, and the AudioStream behavior will load 
   * up the stream file.  The reason the path name is sent is because it would not be 
   * sensible to send the whole stream as an asset due to the size of the file. Once the 
   * stream has been loaded you can use this behavior to start and stop the stream.
   *
   * <BR> <B>BUILDING THE AUDIO TOOLKIT</B>
   *
   * For the PS2 builds, in Skyiop.h there are some #define's which reference the irx files needed for 
   * RenderWare Audio. They reference c:\rw\audio and the relevant build directories 
   * contained within. If you install RenderWare Audio to any other directory you 
   * will need to modify these defines.
   *
   * In all your projects you will need to make sure you have RWADEBUG defined in the preprocessor for 
   * DesignDebug/Debug builds and RWAMETRICS for the Design Metrics/Metrics builds.
   *
   * <BR><B>Metrowerks Codewarrior for PS2</B>
   *
   * Add the following to the project settings for each build configuration:
   *
   * \li #define \ref WITH_AUDIO in PREFIX_PS2_RW.h
   *
   * \li Add the RenderWare Audio path to the source tree (e.g. C:\rw\audio), or use the RWASDK environment
   * variable.
   *
   * \li Add the libraries librwacore.a and librwaspu2.a to each build target. Note: Remember to use the 
   * debug version of the libraries for debug builds, release for release, etc.
   *
   * <BR><B>SN Systems ProDG (Using Visual C++ Integration) for PS2</B>
   *
   * Add the following to the project settings for each build configuration:
   *
   * \li \ref WITH_AUDIO define to the list of preprocessor defines.
   *
   * \li The RenderWare Audio include path (e.g. C:\rw\audio\include\ps2) to the include paths.
   *
   * \li The RenderWare Audio library path for the set of libraries to use,
   *     (e.g. C:\rw\audio\lib\ps2\debug for debug builds) to the library paths.
   *
   * \li librwacore.a and librwaspu2.a to the list of library files used. Note: librwacore.a should come 
   * before librwaspu2.a in the list.
   *
   * <BR><B>Visual C++ for Win32</B>
   *
   * Add the following to the project settings for each build configuration:
   *
   * \li \ref WITH_AUDIO define to the list of preprocessor defines.
   *
   * \li The RenderWare Audio include path (e.g. C:\rw\audio\include\win) to the include paths.
   *
   * \li The RenderWare Audio library path for the set of libraries to use,
   *     (e.g. C:\rw\audio\lib\win\debug for debug builds) to the library paths.
   *
   * \li The following libraries are used by RenderWare Audio:
   *     rwacore.lib 
   *     rwadxaudio.lib 
   *     dsound.lib 
   *     dxguid.lib 
   *     dmoguids.lib 
   *     strmiids.lib 
   *     winmm.lib 
   *
   *     For the Win32 build these libraries are included using a #pragma lib directive at the top of audio.h.
   *
   * <BR><B>Visual C++ for Xbox</B>
   *
   * Add the following to the project settings for each build configuration:
   *
   * \li \ref WITH_AUDIO define to the list of preprocessor defines.
   *
   * \li The RenderWare Audio include path (e.g. C:\rw\audio\include\xbox) to the include paths.
   *
   * \li The RenderWare Audio library path for the set of libraries to use,
   *     (e.g. C:\rw\audio\lib\xbox\debug for debug builds) to the library paths.
   *
   * \li The following libraries are used by RenderWare Audio:
   *     rwacore.lib 
   *     rwaxbox.lib 
   *     dsoundd.lib for debug
   *     dmusicd.lib for debug           
   *     dsound.lib for release
   *     dmusic.lib for release           
   *
   *     For the Xbox build these libraries are included using a #pragma lib directive at the top of audio.h. 
   *
   * <BR><B>Metrowerks Codewarrior for GCN</B>
   *
   * Add the following to the project settings for each build configuration:
   *
   * \li #define \ref WITH_AUDIO in HW2_PREFIX.h
   *
   * \li Add the RenderWare Audio path to the source tree (e.g. C:\rw\audio), or use the RWASDK environment 
   * variable if that is set. You will need to set the path for the libs and includes for each target platform.
   * This can be set by going into the Edit menu, and then the target settings.
   *
   * \li Add rwacore.a and rwaax.a to the libraries used by each build target. Remember to use the debug version
   * of the libraries for debug builds, release for release, etc.
   *
   * <BR><B>SN Systems ProDG (Using Visual C++ Integration) for GCN</B>
   *
   * Add the following to the project settings for each build configuration:
   *
   * \li \ref WITH_AUDIO define to the list of preprocessor defines.
   *
   * \li The RenderWare Audio include path (e.g. C:\rw\audio\include\gcn) to the include paths.
   *
   * \li The RenderWare Audio library path for the set of libraries to use,
   *     (e.g. C:\rw\audio\lib\gcn\debug for debug builds) to the library paths.
   *
   * \li rwacore.a and rwaax.a to the list of library files used. Note: rwacore.a should come before rwaax.a in 
   * the list.
   *
   */ 
   namespace RwsAudio
   {
      const RwUInt32    MAXVOICES       =    15;    /** Maximum number of voices.*/
      const RwUInt32    MAXVVOICES      =    50;    /** Maximum number of virtual voices.*/        
      const RwUInt32    NUMSTREAMVOICES =    16;    /** Maximum number of channels for streams.*/
      const RwUInt32    NUMGROUPS       =    16;    /** Maximum number of wave groups.*/

#ifdef DOLPHIN
      const RwUInt32    WAVEDATAALIGNMENT = 32;
#endif

#ifdef SKY
      const RwUInt32    WAVEDATAALIGNMENT = rwaMALLOCHWALIGN;
#endif

#if defined(_WINDOWS) || defined(_XBOX)
      const RwUInt32    WAVEDATAALIGNMENT = 1;
#endif

      const RwUInt32    WAVEUPLOADBUFFSIZE = 0x40000;

      typedef RwInt32 DictionaryId;

      RwBool RegisterAudioObjects(void);
      void UnRegisterAudioObjects(void);
      void Open(void);
      void Close(void);

      RwaOsOutput *GetOutputObject(void);
      //
      RwBool FadeOutputObject(const RwReal Destination, const RwReal Step);

      RwaEnvironment *GetEnvironment(void);
      //
      RwBool FadeEnvironment(const RwInt32 Destination, const RwUInt32 Step);

      RwaListener *GetListener(void);

      void AddDictionary(RwaWaveDict* const pDict);
      void RemoveDictionary(const RwaWaveDict* const pDict);
      //
      DictionaryId FindDictionaryId(const RwChar* const pName);
      DictionaryId FindDictionaryId(const RwaWaveDict* const pDict);
      RwaWaveDict *GetDictionary(const DictionaryId Id);
      //
      RwaOsWave *FindWave(const RwChar* const pName);

      RwaVirtualVoice *AllocateVirtualVoice(void);
      void FreeVirtualVoice(RwaVirtualVoice* const pVirtualVoice);
      //
      RwBool FadeVirtualVoice(const RwReal Destination, const RwReal Step, const RwaVirtualVoice* const pVoice);
      //
      void StopAllVirtualVoices(void);

      RwaOsStream *OpenStream(const RwChar* const pName);
   }
}
#endif
#endif //WITH_AUDIO
