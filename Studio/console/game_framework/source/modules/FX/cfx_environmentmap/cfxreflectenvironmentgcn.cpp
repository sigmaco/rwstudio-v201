/*****************************************************************************
 *
 * File :     CFXReflectEnvironment.cpp
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
#pragma exceptions off // Turn off exceptions due to bug in MW Dwarf Generator for PS2, present in CW 3.04
#endif
#include "rwcore.h"


#include "framework/toolkits/world/clevel.h"
#include "CFXReflectEnvironmentGCN.h"
#include "framework/core/resourcemanager/cresourcemanager.h"

extern GXRenderModeObj  *_RwDlRenderMode;  /* Display mode info ( defined in RenderWare )*/

namespace RWS
{                    

   namespace FX
   {

      /**
      *
      *  Construct a CFXReflectEnvironmentGCN object.
      *
      */
      CFXReflectEnvironmentGCN::CFXReflectEnvironmentGCN()
      {
         RWS_FUNCTION("RWS::FX::CFXReflectEnvironmentGCN::CFXReflectEnvironmentGCN");

         RWS_RETURNVOID();
      }

      /**
      *
      *  Destroy a CFXReflectEnvironmentGCN object.
      *
      */
      CFXReflectEnvironmentGCN::~CFXReflectEnvironmentGCN()
      {
         RWS_FUNCTION("RWS::FX::CFXReflectEnvironmentGCN::~CFXReflectEnvironmentGCN");

         RWS_RETURNVOID();
      }

      void CFXReflectEnvironmentGCN::CameraTextureFlush(const RwRaster *raster, RwBool boxFilter, RwUInt32 X,
                                                        RwUInt32 Y, RwUInt32 Width, RwUInt32 Height)
      {
         RwUInt32                    offset = 0;
         RwRaster                    *dstParent;
         RwGameCubeRasterExtension   *dstRasExt;

         RWS_FUNCTION("RWS::FX::CFXReflectEnvironmentGCN::CameraTextureFlush");
         RWS_ASSERT(raster != 0, "Raster object pointer invalid.");
         RWS_ASSERTE (((raster->parent == raster) &&
                      !(rwRASTERDONTALLOCATE & raster->cFlags)) ||
                      ((raster->parent != raster) &&
                      (rwRASTERDONTALLOCATE & raster->cFlags)));

         dstParent = RwRasterGetParent(raster);
         dstRasExt = RwGameCubeRasterGetExtension(dstParent);

#if defined(RWDEBUG)
         if ((0x01 & raster->nOffsetX) ||
             (0x01 & raster->nOffsetY))
            {
               RWS_ASSERT( 0, RWSTRING("X & Y Offset must be a multiple of 2") );
            }

         if ((0x01 & RwRasterGetWidth(raster)) ||
            (0x01 & RwRasterGetHeight(raster)))
            {
               RWS_ASSERT(0, RWSTRING("Width & Height must be a multiple of 2") );
            }
#endif /* defined(RWDEBUG) */

         /*
         * Turn off vertical de-flicker filter temporary
         * (to avoid filtering during framebuffer-to-texture copy).
         */
         GXSetCopyFilter(GX_FALSE, 0, GX_FALSE, 0);

         /* Src rect to copy from EFB */
         if (boxFilter)
         {
             GXSetTexCopySrc(
                 X << 1,
                 Y << 1,
                 Width  << 1,
                 Height << 1);
         }
         else
         {
             GXSetTexCopySrc(
                 X,
                 Y,
                 Width,
                 Height );
         }

         /* Destination raster */
         GXSetTexCopyDst(
             RwRasterGetWidth(dstParent),
             RwRasterGetHeight(dstParent),
             (GXTexFmt)dstRasExt->format,
             (GXBool)boxFilter);


         GXCopyTex(dstRasExt->pixels + offset, FALSE);//(GXBool)_RwDlCopyClear);

         /*
          * Wait for the copy to finish before trying to use the texture.
          */
         GXPixModeSync();

         /*
          * Restore vertical de-flicker filter mode
          */
         GXSetCopyFilter(_RwDlRenderMode->aa,
                         _RwDlRenderMode->sample_pattern,
                         GX_TRUE,
                         _RwDlRenderMode->vfilter);

#if defined(EPPC)
         /* Invalidate the texture cache */
         if (dstRasExt->region)
         {
            GXInvalidateTexRegion( (GXTexRegion*)dstRasExt->region);
         }
         else
         {
            GXInvalidateTexAll();
         }
#endif /* defined(EPPC) */

         RWS_RETURNVOID();
      }

      /**
      * Copies an area of the back buffer specified by TopLeftUV and BottomRightUV to pRaster.
      *
      * \param TopLeftUV      Coordinates of the top left area of the screen to copy to environment map texture.
      * \param BottomRightUV  Coordinates of the bottom right area of the screen to copy to environment map texture. 
      * \param pRaster        Pointer to the raster for which the area of the screen will be copied.
      * \return               void. 
      *
      */
      void CFXReflectEnvironmentGCN::UpdateTextureFromDisplay(const RwV2d &TopLeftUV, const RwV2d &BottomRightUV,
                                                              const RwRaster *pRaster)
      {
         RWS_FUNCTION("RWS::FX::CFXReflectEnvironmentGCN::UpdateTextureFromDisplay");

         RwVideoMode VideoMode;

         RwEngineGetVideoModeInfo(&VideoMode, RwEngineGetCurrentVideoMode());

         RwUInt32 X,     Y;
         RwUInt32 Width, Height;

         X      = static_cast<RwUInt32> ( (static_cast<float>(VideoMode.width ) * TopLeftUV.x) );
         Y      = static_cast<RwUInt32> ( (static_cast<float>(VideoMode.height) * TopLeftUV.y) );

         Width  = static_cast<RwUInt32> ( ( (static_cast<float>(VideoMode.width ) * BottomRightUV.x) ) - X );
         Height = static_cast<RwUInt32> ( ( (static_cast<float>(VideoMode.height) * BottomRightUV.y) ) - Y );

         CameraTextureFlush(pRaster, FALSE, X & 0xfffffffc, Y & 0xfffffffc, Width & 0xfffffffc, Height & 0xfffffffc  );


         RWS_RETURNVOID();
      }

   } // namespace FX

} //namespace RWS

