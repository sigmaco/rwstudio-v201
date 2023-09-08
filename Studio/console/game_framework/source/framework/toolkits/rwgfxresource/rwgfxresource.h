/*****************************************************************************
*
* File :     rwgfxresource.h
*
* Abstract : 
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

/*
*
*  \ingroup RwResource
*
*  \page RwResourceOverview Overview
*
*/

#ifndef _RWRESOURCE_H_
#define _RWRESOURCE_H_

//////////////////////////////////////////////////////////////////
//
// Renderware Includes
//
#include <rwcore.h>
#include <rpworld.h>
#include <rphanim.h>
#include <rpskin.h>
#include <rpmatfx.h>
#include <rt2d.h>
#include <rt2danim.h>

#ifdef _MSC_VER
#pragma comment (lib, "rwcore.lib")
#pragma comment (lib, "rpworld.lib")
#pragma comment (lib, "rphanim.lib")
#pragma comment (lib, "rtanim.lib")
#pragma comment (lib, "rpskin.lib")
#pragma comment (lib, "rpmatfx.lib")
#pragma comment (lib, "rt2d.lib")
#pragma comment (lib, "rt2danim.lib")
#endif

//////////////////////////////////////////////////////////////////
//
// Renderware Studio Includes
//
#include "framework/core/resourcemanager/cresourcehandler.h"

namespace RWS
{

   /**
   *
   *  \ingroup RwGfxResource
   *
   *  Resource handler for handling Renderware resources.
   *
   */
   class CRenderwareResource: public CResourceHandler
   {
      public:

         virtual void *Load(const RwChar *psName, const RwChar *psType, const RwChar *psResourcePath, RwStream* pStream, RwUInt32 uiStreamSize, RwUInt32 &uiResourceSize);

         virtual RwBool UnLoad(const RwChar *pStrType, void *pResource);

         virtual RwBool IsHandled(const RwChar *psType);

         virtual void Update(const void *pResData,
                             const void *pData,
                             const RwChar *pStrType) {};

      private:

         // RpWorld Load/Unload
         //
         RpWorld* WorldStreamRead (RwStream *stream);

         // RpTexture Load/Unload
         //
         RwTexture* ReadTextureFromImage (RwStream *stream);

         // RpAtomic
         //
         RpAtomic* AtomicStreamRead (RwStream *stream);

         // RpClump
         //
         RpClump *ClumpStreamRead (RwStream *stream);

         // Rt2dMaestro
         //
         Rt2dMaestro *MaestroStreamRead (RwStream *stream);

         // Rt2dFont
         //
         Rt2dFont *FontStreamRead (RwStream *pStream);

         // RwTexDictionary
         //
         RwTexDictionary* TexDictionaryStreamRead (RwStream *stream);

         // RwTexDictionary (Platform independent)
         //
         RwTexDictionary* PiTexDictionaryStreamRead (RwStream *stream);

         // RpMTEffectDict Multi Texture Effect Dictionary
         //
#if defined(DOLPHIN) || defined(_XBOX)
         RpMTEffectDict* MTEffectDictionaryStreamRead (RwStream *stream);

#endif
         
   };
}

#endif
