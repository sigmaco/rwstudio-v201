/*****************************************************************************
 *
 * File :     CFXXBOXMotionBlur.h
 *
 * Abstract : XBox motion blur implementation
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
#if (defined (_XBOX))

#ifndef __CFXXBOXMOTIONBLUR_H__
#define __CFXXBOXMOTIONBLUR_H__

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

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "../cfxmotionblurbase.h"


namespace RWS
{

   namespace FX
   {

      /**
      *
      *  \ingroup Mod_FX
      *
      *  CFXXBOXMotionBlur is a class that can be used to provide motion blur type effects on the XBOX.
      *  It does this without having to render the scene to a texture. This is achieved by tricking D3D into
      *  thinking the front buffer is a texture and alpha blending the front buffer with the back buffer.
      *  In order to perform this trick it is necessary to use DirectX rather than straight RenderWare calls.
      *  State blocks have been used to ensure that the state of the GPU is the same on exit as it was on entry
      *  to the call to BlendFrontBuffer.
      *
      */
      class CFXXBOXMotionBlur : public CFXMotionBlurBase
      {
      public:

         /**
         *
         *  \ingroup Mod_FX
         *  Vertex structure used to define the blend polygon
         *
         */
         typedef struct s_FullScreenVert
         {
            RwV3d    Pos;   /**< Position of the vertex.*/
            RwReal   u;     /**< u coordinate for the vertex.*/
            RwReal   v;     /**< v coordinate for the vertex.*/
         }t_FullScreenVert;

         CFXXBOXMotionBlur();
         ~CFXXBOXMotionBlur();

         void CreateSavedRenderState      (DWORD *SB);
         void RestoreRenderState          (DWORD SB);
         void DeleteSavedRenderState      (DWORD SB);
         void CreateBlendPoly             (RwCamera *pCamera);
         void ApplyMotionBlurRenderStates ();
         void BlendFrontBuffer            (RwCamera *pCamera);

      protected:
         DWORD             m_RenderState; /**< Handle of state block used to preserve the render state. */
         LPDIRECT3DDEVICE8 m_Dev;         /**< D3D device. */
         t_FullScreenVert  m_Verts[4];    /**< The four vertices used to describe the blend polygon. */

      };

   } // namespace FX
      
} // namespace RWS

#endif
#endif