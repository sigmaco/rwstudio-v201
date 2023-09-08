/*****************************************************************************
 *
 * File :     CFXMotionBlurBase.cpp
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


#include "precomp.h"
#include "rwcore.h"

// Left to its own devices, the MW linker will dead-strip the registrar ctors, so...
#ifdef __MWERKS__
#pragma force_active on
#endif


//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//

#include "framework/core/macros/debugmacros.h"
#include "framework/mainloop/render.h"
#include "cfxmotionblurbase.h"

namespace RWS
{

   namespace FX
   {


      /**
      *
      *  Construct a CFXMotionBlurBase object.
      *
      */
      CFXMotionBlurBase::CFXMotionBlurBase()
      {
         RWS_FUNCTION("RWS::FX::CFXMotionBlurBase::CFXMotionBlurBase");

         m_RotMat = RwMatrixCreate();

         m_UpdateBlendPoly = TRUE;

         RWS_RETURNVOID();
      }

      /**
      *
      *  Destroy a CFXMotionBlurBase object.
      *
      */
      CFXMotionBlurBase::~CFXMotionBlurBase()
      {
         RWS_FUNCTION("RWS::FX::CFXMotionBlurBase::~CFXMotionBlurBase");

         if (m_RotMat)
         {
            RwMatrixDestroy(m_RotMat);
         }

         RWS_RETURNVOID();
      }

      /**
      *
      *  Transforms x,y,z by the RwMatrix pointed to by RotMat.
      *  \param RotMat Pointer to a RwMatrix used in the transformation.
      *  \param x      X coordinate to transform.
      *  \param y      Y coordinate to transform.
      *  \param z      Z coordinate to transform.
      *  \return       RwV3d containing the result of the transformation.
      */
      RwV3d CFXMotionBlurBase::TransformMotionVert(RwMatrix *RotMat, float x, float y, float z)
      {
         RWS_FUNCTION("RWS::FX::CFXMotionBlurBase::TransformMotionVert");

         RwV3d TransformedVert;

         TransformedVert.x = (x * RotMat->right.x) + (y * RotMat->up.x) + (z * RotMat->at.x) + RotMat->pos.x;
         TransformedVert.y = (x * RotMat->right.y) + (y * RotMat->up.y) + (z * RotMat->at.y) + RotMat->pos.y;
         TransformedVert.z = (x * RotMat->right.z) + (y * RotMat->up.z) + (z * RotMat->at.z) + RotMat->pos.z;

         RWS_RETURN(TransformedVert);
      }

      /**
      *
      *  Calculates the translation matrix the for motion blur based on the offset and scale attributes.
      *
      *  \param pMatrix    Pointer to a RwMatrix used store the resulting transformation.
      *  \param HalfWidth  Half the width of the camera raster.
      *  \param HalfHeight Half the height of the camera raster.
      */
      void CFXMotionBlurBase::CalcTransformationMatrix(RwMatrix *pMatrix, RwReal HalfWidth, RwReal HalfHeight)
      {
         RWS_FUNCTION("RWS::FX::CFXMotionBlurBase::CalcTransformationMatrix");
         RwV3d Axis = {0.0f, 0.0f, 1.0f};
         RwV3d Pos;
         RwV3d Scale;


         Pos.x = (m_OffsetX * HalfWidth);
         Pos.y = (m_OffsetY * HalfHeight);
         Pos.z = 0.0f;
         RwMatrixTranslate(pMatrix, &Pos, rwCOMBINEREPLACE); 

         RwMatrixRotate(pMatrix, &Axis, m_Angle, rwCOMBINEPOSTCONCAT); 

         Scale.x = m_ScaleX * m_Scale;
         Scale.y = m_ScaleY * m_Scale;          
         Scale.z = 1.0f;
         RwMatrixScale (pMatrix, &Scale, rwCOMBINEPOSTCONCAT); 

         // translate to the center of the screen
         Pos.x = HalfWidth;
         Pos.y = HalfHeight;
         Pos.z = 0.0f;
         RwMatrixTranslate(pMatrix, &Pos, rwCOMBINEPOSTCONCAT); 
                     
         RWS_RETURNVOID();
      }


   } // namespace FX
      
} // namespace RWS
