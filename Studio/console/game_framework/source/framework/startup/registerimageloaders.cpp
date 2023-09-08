/*****************************************************************************
 *
 * File :     registerimageloaders.h
 *
 * Abstract : Utility function used to register RenderWare Gfx image loaders.
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

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/core/macros/debugmacros.h"

//////////////////////////////////////////////////////////////////
//
// RenderWare Includes
//
#include "rtbmp.h"
#include "rtpng.h"
#include "rtras.h"
#if (!defined _XBOX) && (!defined DOLPHIN)
#include "rttiff.h"
#endif

#ifdef _MSC_VER
#pragma comment (lib, "rtbmp.lib")
#pragma comment (lib, "rtpng.lib")
#pragma comment (lib, "rtras.lib")
#if (!defined _XBOX) && (!defined DOLPHIN)
#pragma comment (lib, "rttiff.lib")
#endif
#endif

namespace RWS
{
   namespace
   {
#if defined (_XBOX) || defined (DOLPHIN)
      RwTextureCallBackRead OldCB;
      RwTexture * DDSTextureRead(const RwChar * szName, const RwChar * szMaskName)
      {    
         RwTexture * pTexture = NULL;    // first try to load the dds texture
      
#if defined (_XBOX)
         if (pTexture = RwXboxDDSTextureRead(szName, szMaskName))
#elif defined (DOLPHIN)
         if (pTexture = RwGameCubeDDSTextureRead(szName, szMaskName))
#else
         if (0)
#endif
         {
            return pTexture;    
         }
      
         // failed, so try to load using normal texture loader
         return OldCB(szName, szMaskName);
      }
   
      // make RW try to load dds textures first of all
      void HookDDSTextureRead(void)
      {    
         OldCB = RwTextureGetReadCallBack(); 
         RwTextureSetReadCallBack(DDSTextureRead);
      }
#endif
   }

   namespace StartUp
   {
     /*
      *
      *  \ingroup StartUp
      *
      *  Register RenderWare Graphics Image loaders.
      *
      *  \returns Returns true on success otherwise false.
      */
      RwBool RegisterImageLoaders(void)
      {
         RWS_FUNCTION("RegisterImageLoaders");
         bool flag = true;

#if (defined(__MWERKS__) || defined(SN_TARGET_PS2)) || defined(SN_TARGET_NGC)
         typedef struct RwImage *(*functorR)(const RwChar *imageName);
         typedef struct RwImage *(*functorW)(RwImage * image, const RwChar *imageName);
#else
         typedef struct RwImage *(__cdecl *functorR)(const RwChar *imageName);
         typedef struct RwImage *(__cdecl *functorW)(RwImage * image, const RwChar *imageName);
#endif
      
         typedef struct ImageFormats_tag
         {
            functorR _functorR;
            functorW _functorW;
            RwChar *imagename;
         } ImageFormats;
      
         // Declare which image loaders to use.
         //
         static const ImageFormats _ImageFormats[] =
         {
            {RtPNGImageRead, RtPNGImageWrite, RWSTRING("png")},
            {RtBMPImageRead, RtBMPImageWrite, RWSTRING("bmp")},
            {RtRASImageRead, RtRASImageWrite, RWSTRING("ras")},
#if (!defined _XBOX) && (!defined DOLPHIN)         
            {RtTIFFImageRead, 0, RWSTRING("tif")},
#endif
         };

         for (RwUInt32 i = 0; i < sizeof(_ImageFormats) / sizeof(ImageFormats); i++)
         {
            if (!RwImageRegisterImageFormat(_ImageFormats[i].imagename,
                                            _ImageFormats[i]._functorR, 
                                            (RwImageCallBackWrite) _ImageFormats[i]._functorW))
            {
               RWS_ASSERTFAIL("Unable to register image loader: "<<_ImageFormats[i].imagename);

               flag = false;
            }
         }

#if defined(_XBOX) || defined(DOLPHIN)
         // Add support for loading DDS textures.
         //
         HookDDSTextureRead();
#endif

         RWS_RETURN(flag);
      }
   }
}