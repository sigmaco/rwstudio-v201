/*****************************************************************************
 *
 * File :     CFXGameCubeMotionBlur.h
 *
 * Abstract : A class specifically for the GameCube that can be used to blend the
 *            previous frame with the current frame to produce various motion
 *            blur effects.
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
#if (defined (DOLPHIN))

#ifndef __CFXGAMECUBEMOTIONBLUR_H__
#define __CFXGAMECUBEMOTIONBLUR_H__

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
      *  CFXGameCubeMotionBlur is a class that can be used to provide motion blur type effects on the GameCube.
      *  To achieve this a texture has to be created in system memory. After each frame is rendered a copy of the
      *  the frame is sent from the EBF to the texture. When the next frame is rendered the copy of the previous
      *  frame is blended with the current frame to produce motion blur type effects.
      *
      */
      class CFXGameCubeMotionBlur : public CFXMotionBlurBase
      {
      public:
         CFXGameCubeMotionBlur();
         ~CFXGameCubeMotionBlur();

         void  CreateBlendPoly(RwCamera *pCamera);
         void  BlendFrontBuffer(RwCamera *pCamera);

      protected:
         RwIm2DVertex   m_Vertex[4];      /**< Pointer to the frame buffer raster. */
         RwRaster       *m_pFrameRaster;  /**< Defines the blend polygon. */         
      };

   } // namespace FX
      
} // namespace RWS

#endif
#endif