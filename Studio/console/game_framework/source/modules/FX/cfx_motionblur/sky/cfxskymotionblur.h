/*****************************************************************************
 *
 * File :     CFXSkyMotionBlur.h
 *
 * Abstract : Sky motion blur implementation
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
#if (defined (SKY))

#ifndef __CFXSKYMOTIONBLUR_H__
#define __CFXSKYMOTIONBLUR_H__

//////////////////////////////////////////////////////////////////
//
// RenderWare Includes
//
#include "rwcore.h"
#include "rpworld.h"


#ifdef _MSC_VER
#pragma comment (lib, "rwcore.lib")
#pragma comment (lib, "rpworld.lib")
#endif

#include "../cfxmotionblurbase.h"

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
namespace RWS
{

   namespace FX
   {


      /**
      *
      *  \ingroup Mod_FX
      *
      *  CFXSkyMotionBlur is a class that can be used to provide motion blur type effects on the PS2.
      *  It does this without have to render the scene to a texture. This is achieved by creating a raster from
      *  the front buffer and using it as a texture on an alpha blended polygon that is drawn into the back buffer.
      *
      */
      class CFXSkyMotionBlur : public CFXMotionBlurBase
      {
      public:
         CFXSkyMotionBlur();
         ~CFXSkyMotionBlur();

         void  CreateBlendPoly(RwCamera *pCamera);
         void  BlendFrontBuffer(RwCamera *pCamera);

      protected:
         RwIm2DVertex   m_Vertex[4];         /**< Defines the blend polygon. */
         RwRaster       *m_pFrameRaster;     /**< Pointer to the frame buffer raster. */

      };

   } // namespace FX
      
} // namespace RWS

#endif
#endif