/*****************************************************************************
*
* File :     Math.h
*
* Abstract : General-purpose math utilities
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

#ifndef __MATH_H__
#define __MATH_H__

//////////////////////////////////////////////////////////////////
//
// RenderWare Includes
//
#include <rwcore.h>

#ifdef _MSC_VER
#pragma comment (lib, "rwcore.lib")
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
   * \ingroup MathsToolkit
   *
   * Converts an angle from degrees to radians.
   *
   * \param val A RwReal angle in degrees.
   *
   * \return Returns the same angle in radians.
   *
   * \note Based on the RenderWare Gfx macro RWRAD2DEG
   */
   #define RWDEG2RAD(_x) ((_x) * ( ((RwReal)rwPI) / ((RwReal)180) ))
   
   /**
   *
   * \ingroup MathsToolkit
   *
   * Clamps 'val' to the range ('min','max')
   *
   * \param val value to be clamped, type <T>
   * \param min minimum value to be used, type <T>
   * \param max maximum value to be used, type <T>
   *
   */

   template< class T >
   const T& Clamp( const T& val, const T& min, const T& max )
   {
      RWS_PRE( max >= min );

      if ( val > max ) return max;
      else if ( val < min ) return min;
      else return val;
   }

   ////////////////////////
   //
   // Useful trig values...
   //
   ////////////////////////

   /**
   *
   * \ingroup MathsToolkit
   *
   * Two times the radian value of Pi as a const RwReal
   *
   */

   const RwReal TWO_PI = (RwReal)(6.2831853);

   //////////////////////////////////
   //
   // Useful vectors that define axis
   //
   //////////////////////////////////

   /**
   *
   * \ingroup MathsToolkit
   *
   * X axis unit vector as a const RwV3d
   *
   */

   const RwV3d XAxis = {(RwReal)(1.0),  (RwReal)(0.0), (RwReal)(0.0)};

   /**
   *
   * \ingroup MathsToolkit
   *
   * Y axis unit vector as a const RwV3d
   *
   */

   const RwV3d YAxis = {(RwReal)(0.0), (RwReal)(1.0), (RwReal)(0.0)};

   /**
   *
   * \ingroup MathsToolkit
   *
   * Z axis unit vector as a const RwV3d
   *
   */

   const RwV3d ZAxis = {(RwReal)(0.0), (RwReal)(0.0), (RwReal)(1.0)};

   /**
   *
   * \ingroup MathsToolkit
   *
   * negative X axis unit vector as a const RwV3d
   *
   */

   const RwV3d negXAxis = {(RwReal)(-1.0),  (RwReal)(0.0), (RwReal)(0.0)};

   /**
   *
   * \ingroup MathsToolkit
   *
   * negative Y axis unit vector as a const RwV3d
   *
   */

   const RwV3d negYAxis = {(RwReal)(0.0),  (RwReal)(-1.0), (RwReal)(0.0)};

   /**
   *
   * \ingroup MathsToolkit
   *
   * negative Z axis unit vector as a const RwV3d
   *
   */

   const RwV3d negZAxis = {(RwReal)(0.0),  (RwReal)(0.0), (RwReal)(-1.0)};

   //////////////////////////////////////////////////////////////////
   // 
   // func RwReal | DegAngDiff | Determine angle difference between two angles...Degrees
   //
   //
   RwReal DegAngDiff(RwReal a0, RwReal a1);

   //////////////////////////////////////////////////////////////////
   // 
   // func RwReal | RadAngDiff | Determine angle difference between two angles...Radians
   //
   //
   RwReal RadAngDiff(RwReal a0, RwReal a1);

   //////////////////////////////////////////////////////////////////
   // 
   // func RwReal | LowPassFilter | Performs a low pass filter operation, where
   // y0 is the current input value, and x1 is the previous output value. k should be between 0 and 1
   //
   // out = x1 + (y0-x1)*k;
   //
   // i.e. the larger the difference between the y0 and x0 the greater the increase in the output
   //
   RwReal LowPassFilter(RwReal y0, RwReal x1, RwReal k);

   /**
   *
   * \ingroup MathsToolkit
   *
   * Tolerance of the comparisons within the FrameTrans class as a const RwReal
   *
   */

   const RwReal RWSFT_TOL = (RwReal)(0.0001);

   /**
   *
   * \ingroup MathsToolkit
   *
   * Provides a class to link to RwFrames together, via an adjustable translation and angular offsets.
   *
   */

   class FrameTrans
   {
   private:

      RwMatrix m_transMtx;    /**< transformation matrix from input frame to output frame */

      bool dirty;             /**< dirty flag, marks when recalculation is actually necessary */

      RwV3d m_offset,         /**< offset vector of linkage */
            m_upVect,         /**< up vector of linkage */
            m_rightVect,      /**< right vector of linkage */
            m_forwardVect;    /**< forward (look-at) vector of linkage */

   public:

      /**
      *
      * Set the XYZ offset between the two frames. The 'Recalc' function must be called after this
      * function, but calls to other 'Set' functions can be made before that. It is more efficient
      * to call several of these and then 'Recalc'.
      *
      * \param offset the 3D vector holding the XYZ offset values to be used, RwV3d.
      *
      * \see SetUp
      * \see SetRight
      * \see SetForward
      * \see SetFromAngles
      * \see Recalc
      *
      */

      void SetOffset(RwV3d &offset)
      {
         m_offset = offset;
         dirty = true;
      }

      /**
      *
      * Set the up vector mapping used by the output frame frames. The 'Recalc' function must be
      * called after this function, but calls to other 'Set' functions can be made before that.
      * It is more efficient to call several of these and then recalculate.
      *
      * \param up the new 3D up vector. This must be a unit vector within the +\- RWSFT_TOL range, RwV3d.
      *
      * \see SetOffset
      * \see SetRight
      * \see SetForward
      * \see SetFromAngles
      * \see Recalc
      *
      */

      void SetUp(RwV3d &up)
      {
         RWS_ASSERT(RwV3dLength(&up) >= (1.0f - RWSFT_TOL) && RwV3dLength(&up) <= (1.0f + RWSFT_TOL), \
               "MUST be a unit vector");

         m_upVect = up;
         dirty = true;
      }

      /**
      *
      * Set the right vector mapping used by the output frame frames. The 'Recalc' function must be
      * called after this function, but calls to other 'Set' functions can be made before that.
      * It is more Efficient to call several of these and then 'Recalc'.
      *
      * \param right the new 3D right vector. This must be a unit vector within the +\- RWSFT_TOL range, RwV3d.
      *
      * \see SetOffset
      * \see SetUp
      * \see SetForward
      * \see SetFromAngles
      * \see Recalc
      *
      */

      void SetRight(RwV3d &right)
      {
         RWS_ASSERT(RwV3dLength(&right) >= (1.0f - RWSFT_TOL) && RwV3dLength(&right) <= (1.0f + RWSFT_TOL), \
               "MUST be a unit vector");

         m_rightVect = right;
         dirty = true;
      }

      /**
      *
      * Set the forward (look-at) vector mapping used by the output frame frames. The 'Recalc' function
      * must be called after this function, but calls to other 'Set' functions can be made before that.
      * It is more efficient to call several of these and then recalculate.
      *
      * \param forward the new 3D forward vector. This must be a unit vector within the +\- RWSFT_TOL range, RwV3d.
      *
      * \see SetOffset
      * \see SetUp
      * \see SetRight
      * \see SetFromAngles
      * \see Recalc
      *
      */

      void SetForward(RwV3d &forward)
      {
         RWS_ASSERT(RwV3dLength(&forward) >= (1.0f - RWSFT_TOL) && RwV3dLength(&forward) <= (1.0f + RWSFT_TOL), \
               "MUST be a unit vector");

         m_forwardVect = forward;
         dirty = true;
      }

      /**
      *
      * Translates the input frame to output frame by the defined parameters (Set calls). Recalc MUST
      * have been called at least once (after setting parameters) BEFORE calling
      *
      * \param out reference to the output frame, RwFrame.
      * \param in reference to the input frame, RwFrame.
      *
      */

      void Trans(RwFrame &out, RwFrame &in)
      {
         RwMatrix *pOutMtx = RwFrameGetMatrix(&out),
               *pInMtx = RwFrameGetLTM(&in);

         RwMatrixMultiply(pOutMtx, &m_transMtx, pInMtx);
         RwFrameUpdateObjects(&out);
      }

      void SetFromAngles(RwReal deflect, RwReal heading);
      void Recalc(void);
   };

   /**
   *
   * Returns random number value between 0.0 and 1.0 as a RwReal (floating point) value.
   *
   * \retval a random floating point value between 0.0 and 1.0 as a RwReal
   *
   */
   
   inline RwReal RealRand(void)
   {
      RwReal val;
      
      val = static_cast<RwReal>(rand());
      val /= static_cast<RwReal>(RAND_MAX);
      
      return(val);
   }

   /**
   *
   * Returns true if the passed in value is a power of 2.
   *
   * \param a The number to text.
   * \return true if the parameter is a power of 2, false if it is not.
   *
   */

   inline bool IsPowerOf2(RwUInt32 a)
   {
      return(a != 0 ? (!((a - 1) & a)) : 0);
   }

   // Prototypes.

   RwReal DegAngDiffAndRange(RwReal &newVal, RwReal baseVal, RwReal lastVal);
}

#endif
