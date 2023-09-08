/*****************************************************************************
*
* File :     rwgfxresource.cpp
*
* Abstract : This file handles loading/unloading of RenderWare resources
* which are all stored within RenderWare streams.
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
// Left to its own devices, the MW linker will dead-strip the registrar ctors, so...
#ifdef __MWERKS__
#pragma force_active on
#endif

//#define RWS_CALLSTACKENABLE
//#define RWS_TRACEENABLE

//////////////////////////////////////////////////////////////////
//
// RenderWare Gfx Includes
//
#ifdef SKY
#include "rtmipk.h"
#endif
#include "rt2danim.h"
#include "rtpitexd.h"

#ifdef _MSC_VER
#pragma comment(lib, "rt2danim.lib")
#pragma comment(lib, "rtpitexd.lib")
#endif

/* Some plug-in IDs that we can't get from public headers */
#define rwID_BINMESHPLUGIN      MAKECHUNKID(rwVENDORID_CRITERIONWORLD, 0x0E)
#define rwID_NATIVEDATAPLUGIN   MAKECHUNKID(rwVENDORID_CRITERIONWORLD, 0x10)

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "rwgfxresource.h"
#include "framework/core/macros/debugmacros.h"
#include "framework/core/resourcemanager/cresourcemanager.h"
#include "framework/mainloop/render.h"

namespace RWS
{
      namespace
      {
         /*
         *
         *  Callback to override the default font reading method used by RenderWare.
         *  This is called by RenderWare when a Maestro animation is read or after a call to Rt2dFontRead.
         *
         *  \param pFontName Pointer to a string containing the name of the font to be used.
         *
         *  \return A pointer to a RenderWare Graphics Rt2dFont object on success, otherwise 0.
         */
         Rt2dFont *FontReaderCB(const RwChar *pFontName)
         {
            RWS_FUNCTION("RWS::NULL::FontReaderCB");
            RWS_ASSERT(pFontName, "No font name supplied.");

            Rt2dFont *pFont;

            // Find the font object in the resource manager
            const void* pObject = CResourceManager::FindByName(pFontName, 0, 0, 0);

            pFont = static_cast<Rt2dFont*>(const_cast<void*>(pObject));

            RWS_ASSERT(pFont, "Unable to find named font ( " << pFontName << " )");

            RWS_RETURN (pFont);
         }


         /*
         *
         *  Callback to override the default font reading method used by RenderWare.
         *  This is called by RenderWare when a Maestro animation is read.
         *
         *  \param pFontName Pointer to a string containing the name of the font to be used.
         *
         *  \returns A pointer to a RenderWare Graphics Rt2dFont object on success, otherwise 0.
         */
         Rt2dFont *MaestroFontReaderCB(const RwChar *pFontName)
         {
            RWS_FUNCTION("RWS::NULL::MaestroFontReaderCB");
            RWS_ASSERT(pFontName, "No font name supplied.");

            Rt2dFont *pFont;

            // Find the font object in the resource manager
            const void* pObject = CResourceManager::FindByName(pFontName, 0, 0, 0);

            pFont = static_cast<Rt2dFont*>(const_cast<void*>(pObject));

            RWS_ASSERT(pFont, "Unable to find named Maestro font ( " << pFontName << " )");

            // The returned font is now managed by rt2d.
            // Remove the font from the resource manager
            CResourceManager::Remove(pObject);
            
            RWS_RETURN (pFont);
         }
      }

      /**
      *
      *  Reads an RpWorld object from a binary RenderWare stream.
      *
      *  \param stream, pointer to a RenderWare Stream Object
      *
      *  \returns If successful returns a pointer to the RpWorld object, otherwise 0.
      */
      RpWorld* CRenderwareResource::WorldStreamRead (RwStream *stream)
      {
         RWS_FUNCTION("RWS::CRenderwareResource::WorldStreamRead");
         RWS_PRE(stream);
         
         RpWorld *world;

         // Read the new world from the stream
         //
         world = RpWorldStreamRead(stream);

         RWS_POST(world);

#if defined (SKY)
         RtMipKWorldCalculateKValues(world, MainLoop::Render::GetCamera());
#endif

         RWS_RETURN( world );
      }

      /**
      *
      *  Reads an RpAtomic object from a binary RenderWare stream.
      *
      *  \param stream, pointer to a RenderWare Stream Object
      *
      *  \returns If successful returns a pointer to the RpAtomic object, otherwise 0.
      */
      RpAtomic* CRenderwareResource::AtomicStreamRead (RwStream *stream)
      {
         RWS_FUNCTION("RWS::CRenderwareResource::AtomicStreamRead");
         RWS_PRE(stream);

         RpAtomic *atomic;

         atomic = RpAtomicStreamRead(stream);

         RWS_POST(atomic);

#if defined (SKY) 
         RpClump *clump = RpClumpCreate();

         if (clump)
         {
            RwFrame *pFrame = RwFrameCreate();
            RpAtomicSetFrame(atomic, pFrame);

            RpClumpAddAtomic(clump, atomic);
            RtMipKClumpCalculateKValues(clump, MainLoop::Render::GetCamera());
            RpClumpRemoveAtomic(clump, atomic);
            RpClumpDestroy(clump);

            RpAtomicSetFrame(atomic, 0);
            RwFrameDestroy(pFrame);
         }
#endif
         RWS_RETURN ( atomic );
      }

      /**
      *
      *  Reads an RpClump object from a binary RenderWare stream.
      *
      *  \param stream, pointer to a RenderWare Stream Object
      *
      *  \returns If successful returns a pointer to the RpClump object, otherwise 0.
      */
      RpClump *CRenderwareResource::ClumpStreamRead (RwStream *stream)
      {
         RWS_FUNCTION("RWS::CRenderwareResource::ClumpStreamRead");
         RWS_PRE(stream);

         RpClump *clump;

         clump = RpClumpStreamRead(stream);

         RWS_POST(clump);

#if defined (SKY)
         RtMipKClumpCalculateKValues(clump, MainLoop::Render::GetCamera());
#endif

         RWS_RETURN( clump );
      }

      /**
      *
      *  Reads a Rt2dMaestro object from a binary RenderWare stream.
      *
      *  \param stream Pointer to a RenderWare Stream Object
      *
      *  \returns Returns a pointer to a Rt2dMaestro object.
      */
      Rt2dMaestro *CRenderwareResource::MaestroStreamRead (RwStream *stream)
      {
         RWS_FUNCTION ("RWS::CRenderwareResource::MaestroStreamRead");
         RWS_PRE(stream);

         Rt2dMaestro *maestro;

         Rt2dDeviceSetCamera(RWS::MainLoop::Render::GetCamera());

         maestro = Rt2dMaestroStreamRead ((Rt2dMaestro *)NULL, stream);

         RWS_POST(maestro);

         RWS_RETURN ( maestro );
      }


      /**
      *
      *  Reads a Rt2dFont object from a binary RenderWare stream.
      *
      *  \param pStream Pointer to a RenderWare Stream Object
      *
      *  \returns If successful returns a pointer to the Rt2dFont object, otherwise 0.
      */
      Rt2dFont *CRenderwareResource::FontStreamRead (RwStream *pStream)
      {
         RWS_FUNCTION ("RWS::CRenderwareResource::FontStreamRead");
         RWS_PRE(pStream);

         // Assume last read texture dictionary is the one for the fonts
         RwTexDictionary* pTexDict = RwTexDictionaryGetCurrent();
         Rt2dFontTexDictionarySet(pTexDict);

         Rt2dFont *pFont;

         Rt2dFontSetReadCallBack(MaestroFontReaderCB);

         pFont = Rt2dFontStreamRead(pStream);

         RWS_POST(pFont);

         RWS_RETURN(pFont);
      }

      /**
      *
      *  Read a RwTexDictionary object from a binary RenderWare stream,
      *  also selects the RwTexDictionary object as the current texture
      *  dictionary.
      *
      *  \param stream Pointer to a RenderWare Stream Object
      *
      *  \returns If successful returns a pointer to the RwTexDictionary object, otherwise 0.
      */
      RwTexDictionary* CRenderwareResource::TexDictionaryStreamRead (RwStream *stream)
      {
         RWS_FUNCTION("RWS::CRenderwareResource::TexDictionaryStreamRead");
         RWS_PRE(stream);
         
         RwTexDictionary *tex_dictionary;
         
         tex_dictionary = RwTexDictionaryStreamRead (stream);
         
         RWS_POST(tex_dictionary);
         
         RwTexDictionarySetCurrent (tex_dictionary);
         
         RWS_RETURN( tex_dictionary );
      }


      /**
      *
      *  Read a platform independent RwTexDictionary object from a binary RenderWare stream,
      *  also selects the RwTexDictionary object as the current texture
      *  dictionary.
      *
      *  \param stream Pointer to a RenderWare Stream Object
      *
      *  \returns If successful returns a pointer to the RwTexDictionary object, otherwise 0.
      */
      RwTexDictionary* CRenderwareResource::PiTexDictionaryStreamRead (RwStream *stream)
      {         
         RWS_FUNCTION("RWS::CRenderwareResource::PiTexDictionaryStreamRead");
         RWS_PRE(stream);

#if defined (D3D8_DRVMODEL_H) || defined (D3D9_DRVMODEL_H) || defined (OPENGL_DRVMODEL_H) || defined(GERBERA_DRVMODEL_H)
         
         RwTexDictionary *tex_dictionary;
         
         tex_dictionary = RtPITexDictionaryStreamRead (stream);
         
         RWS_POST(tex_dictionary);
         
         RwTexDictionarySetCurrent (tex_dictionary);
         
         RWS_RETURN( tex_dictionary );
#else
         RWS_ASSERTFAIL("CRenderwareResource::Platform independent texture dictionary's"
            " are only recommended for use with OpenGL and DirectX");

         RWS_RETURN(0);
#endif
      }


#if defined(DOLPHIN) || defined(_XBOX)
      /**
      *
      *  Read a RpMTEffectDict object from a binary RenderWare stream,
      *  also selects the RpMTEffectDict object as the current material effect
      *  dictionary.
      *
      *  \param stream Pointer to a RenderWare Stream Object
      *
      *  \returns If successful returns a pointer to the RpMTEffectDict object, otherwise 0.
      */
      RpMTEffectDict* CRenderwareResource::MTEffectDictionaryStreamRead (RwStream *stream)
      {
         RWS_FUNCTION("RWS::CRenderwareResource::MTEffectDictionaryStreamRead");
         RWS_PRE(stream);

         RpMTEffectDict *MTE_dictionary;

         MTE_dictionary = RpMTEffectDictStreamRead  (stream);

         RWS_POST(MTE_dictionary);

         RpMTEffectDictSetCurrent (MTE_dictionary);

         RWS_RETURN( MTE_dictionary );
      }
#endif
      
      /**
      *
      *  Loads a RenderWare Graphics Resource.
      *
      *  \param psName pointer to a string defining the resource name.
      *
      *  \param psType pointer to a string defining the resource type.
      *
      *  \param psResourcePath pointer to a string containing a path where files referenced
      *  by this file can be found.
      *
      *  \param pStream pointer to a RenderWare stream.
      *
      *  \param uiStreamSize size of the file associated with this resource.
      *
      *  \returns If successful returns a pointer to the loaded resource, otherwise 0.
      *
      */
      void *CRenderwareResource::Load( const RwChar *psName,
                                       const RwChar *psType,
                                       const RwChar *psResourcePath,
                                       RwStream* pStream,
                                       RwUInt32 uiStreamSize,
                                       RwUInt32 &uiResourceSize)
      {
         RWS_FUNCTION("RWS::CRenderwareResource::Load");
         RWS_PRE(psType);
         RWS_PRE(psResourcePath);
         RWS_PRE(pStream);

         void *ret = 0;

         if (uiStreamSize == 0)
         {
            RWS_RETURN(ret);
         }

         if (psResourcePath) RwImageSetPath(psResourcePath);

         RwChunkHeaderInfo _RwChunkHeaderInfo;

         RwStreamReadChunkHeaderInfo(pStream, &_RwChunkHeaderInfo);

         switch (_RwChunkHeaderInfo.type)
         {
            default:
            case rwID_NAOBJECT:
               RWS_ASSERTFAIL("CRenderwareResource::Load doesn't support this RenderWare type ( "
                     << _RwChunkHeaderInfo.type << " )");
               break;

            case rwID_WORLD:           ret = WorldStreamRead(pStream);            break;
            case rwID_CLUMP:           ret = ClumpStreamRead(pStream);            break;
            case rwID_ATOMIC:          ret = AtomicStreamRead(pStream);           break;
            case rwID_TEXDICTIONARY:   ret = TexDictionaryStreamRead(pStream);    break;
            case rwID_PITEXDICTIONARY: ret = PiTexDictionaryStreamRead(pStream);  break;
            case rwID_HANIMANIMATION:  ret = RpHAnimAnimationStreamRead(pStream); break;
            case rwID_2DMAESTRO:       ret = MaestroStreamRead(pStream);          break;
            case rwID_2DFONT:          ret = FontStreamRead(pStream);             break;
#if defined(DOLPHIN) || defined(_XBOX)
            case rwID_MTEFFECTDICT:    ret = MTEffectDictionaryStreamRead(pStream); break;
#endif
         }

         RWS_RETURN( ret );
      }

      /**
      *
      *  UnLoad a RenderWare Graphics Resource.
      *
      *  \param pStrType A pointer to a string defining the resource type.
      *
      *  \param pResource A pointer to the resource.
      *
      *  \returns True if the resource was successfully unloaded otherwise false.
      *
      */
      RwBool CRenderwareResource::UnLoad (const RwChar *pStrType, void *pResource)
      {
         RWS_FUNCTION("RWS::CRenderwareResource::UnLoad");

         RwBool ret = FALSE;

         const RwChar *_pStrType = &pStrType[5];

         if (rwstrcmp(_pStrType, RWSTRING("WORLD")) == 0)
         {
            ret = RpWorldDestroy(reinterpret_cast<RpWorld*>(pResource));
         }
         else if ((rwstrcmp(_pStrType, RWSTRING("TEXDICTIONARY")) == 0)
               || (rwstrcmp(_pStrType, RWSTRING("PITEXDICTIONARY")) == 0))
         {
            ret = RwTexDictionaryDestroy(reinterpret_cast<RwTexDictionary*>(pResource));
         }
         else if (rwstrcmp(_pStrType, RWSTRING("CLUMP")) == 0)
         {
            ret = RpClumpDestroy(reinterpret_cast<RpClump*>(pResource));
         }
         else if (rwstrcmp(_pStrType, RWSTRING("ATOMIC")) == 0)
         {
            ret = RpAtomicDestroy(reinterpret_cast<RpAtomic*>(pResource));
         }
         else if (rwstrcmp(_pStrType, RWSTRING("HANIMANIMATION")) == 0)
         {
            ret = RpHAnimAnimationDestroy (reinterpret_cast<RpHAnimAnimation*>(pResource));
         }
         else if (rwstrcmp(_pStrType, RWSTRING("2DMAESTRO")) == 0)
         {
            ret = ( Rt2dMaestroDestroy (reinterpret_cast<Rt2dMaestro*>(pResource)) );
         }
         else if (rwstrcmp(_pStrType, RWSTRING("2DFONT")) == 0)
         {
            ret = TRUE;
         }     
#if defined(DOLPHIN) || defined(_XBOX)
         else if (rwstrcmp(_pStrType, RWSTRING("MTEFFECTDICT")) == 0)
         {
            RpMTEffectDictDestroy(static_cast<RpMTEffectDict*>(pResource));
            ret = TRUE;
         }     
#endif
         else
         {
            RWS_TRACE ("OnFreeResource - Unimplemented resource type \"" << pStrType << "\"");
         }

         RWS_RETURN(ret);
      }

      /**
      *
      * Check if the specified resource is a RenderWare Graphics Resource.
      *
      * \param psType pointer to a string defining the resource type.
      *
      * \returns TRUE if the resource is a RenderWare Graphics resource, otherwise returns FALSE.
      *
      */
      RwBool CRenderwareResource::IsHandled(const RwChar *psType)
      {
         RWS_FUNCTION("RWS::CRenderwareResource::IsHandled");
         RWS_PRE(psType);

         if (psType[0] == RWSTRING('r')
         && psType[1] == RWSTRING('w')
         && psType[2] == RWSTRING('I')
         && psType[3] == RWSTRING('D'))
         {
            const RwChar *_pStrType = &psType[5];

            if (rwstrcmp(_pStrType, RWSTRING("WORLD")) == 0) {RWS_RETURN(TRUE);}
            else if (rwstrcmp(_pStrType, RWSTRING("TEXDICTIONARY")) == 0) {RWS_RETURN(TRUE);}
            else if (rwstrcmp(_pStrType, RWSTRING("CLUMP")) == 0) {RWS_RETURN(TRUE);}
            else if (rwstrcmp(_pStrType, RWSTRING("ATOMIC")) == 0) {RWS_RETURN(TRUE);}
            else if (rwstrcmp(_pStrType, RWSTRING("HANIMANIMATION")) == 0) {RWS_RETURN(TRUE);}
            else if (rwstrcmp(_pStrType, RWSTRING("2DMAESTRO")) == 0) {RWS_RETURN(TRUE);}
            else if (rwstrcmp(_pStrType, RWSTRING("2DFONT")) == 0) {RWS_RETURN(TRUE);}
            else if (rwstrcmp(_pStrType, RWSTRING("PITEXDICTIONARY")) == 0) {RWS_RETURN(TRUE);}
#if defined(DOLPHIN) || defined(_XBOX)
            else if (rwstrcmp(_pStrType, RWSTRING("MTEFFECTDICT")) == 0) {RWS_RETURN(TRUE);}
#endif
         }

         RWS_RETURN(FALSE);
      }
      /*
      *
      * Create an instance of the RenderWare Resource handler, this is required in order to register the handler.
      *
      */
      namespace { CRenderwareResource gCRenderwareResource; }
}//namespace RWS

