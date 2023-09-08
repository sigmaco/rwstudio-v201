/*****************************************************************************
 *
 * File :     CFXMotionBlurBase.h
 *
 * Abstract : Base class used by platform specific implementations of motion blur
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

#ifndef _CFXMOTIONBLURBASE_H_
#define _CFXMOTIONBLURBASE_H_

//////////////////////////////////////////////////////////////////
//
// RenderWare Includes
//
#include "rwcore.h"

namespace RWS
{

   namespace FX
   {

      /**
      *
      *  \ingroup Mod_FX
      *
      *  CFXMotionBlurBase is a base class that can be derived from to create a motion blur effect behaviour. 
      *  The platform specific class used to implement the motion blur effect will inherit from CFXMotionBlurBase.
      *
      */
      class CFXMotionBlurBase
      {
      public:
         CFXMotionBlurBase();
         ~CFXMotionBlurBase();

         RwReal        m_BlurAmount;   /**<  amount of alpha used to blend frames. */
         RwReal        m_OffsetX;      /**<  offset applied to the blend poly. */
         RwReal        m_OffsetY;
         RwReal        m_ScaleX;       /**<  scale in x applied to the blend poly. */
         RwReal        m_ScaleY;       /**<  scale in y applied to the blend poly. */
         RwReal        m_Scale;        /**<  overall scale applied to the blend poly. */
         RwReal        m_Angle;        /**<  angle of the blend poly. */
         RwBool        m_UpdateBlendPoly;

         RwV3d TransformMotionVert     (RwMatrix *RotMat, float x, float y, float z);
         void  CalcTransformationMatrix(RwMatrix *pMatrix, RwReal HalfWidth, RwReal HalfHeight);

      protected:
         RwMatrix     *m_RotMat;       /**< Stores the rotation matrix used to transform the blend polygon. */

      };
   } // namespace FX
      
} // namespace RWS

#endif
