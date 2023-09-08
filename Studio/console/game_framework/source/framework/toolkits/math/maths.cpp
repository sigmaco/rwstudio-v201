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
#include "precomp.h"
#include "maths.h"

namespace RWS
{
   /**
   *
   * \ingroup MathsToolkit
   *
   * Determine angle difference between two angles in degrees
   *
   * \param a0 first angle as a RwReal
   * \param a1 second angle as a RwReal
   *
   * \return Returns the angular difference between the two angles as a RwReal (-180 to +180).
   *
   */

   RwReal DegAngDiff(RwReal a0, RwReal a1) 
   {
      RWS_FUNCTION("RWS::DegAngDiff");

      RwReal a2;
   
      a2 = a1 - a0;
      while(a2 < 0.0f) a2 += 360.0f;
      while(a2 >= 360.0f) a2 -= 360.0f;
      if(a2 >= 180.0f) a2 -= 360.0f;
   
      RWS_RETURN (a2);
   }

   /**
   *
   * \ingroup MathsToolkit
   *
   * Determine angle difference between two angles in degrees. New angle is normally in the range
   * of -180 to +180 (normal calculation range). Base is assumed to be in range of -180 to +180 too,
   * but can be anything. The Last value can be any angular value. The new value will translated into
   * the same range (as the last value) and the difference between this ranged value and the base value
   * will be returned. Therefore the return value can also be OUTSIDE the range of -180 to +180.
   *
   * The code assumes that the new value cannot be out by more than 180 degrees from the last value.
   * If it is/can be, this function is likely to fail. That is to say, the code ranges the new value
   * so that it IS within -180 to +180 degrees of the last value.
   * 
   * \param newVal the calculated angular value (can be any value, but normally in the range of -180 to
   * +180 on INPUT). Ranged value will be returned, so any angle can be returned. i.e., it will be within
   * (last value - 180) to (last value + 180).
   * \param baseVal the base or reference angle. Normally in range of -180 to +180, but can be any.
   * \param lastVal the previous angular value. Used for ranging. Can be any angular value.
   *
   * \return the difference between baseVal and newVal, after newVal had been ranged to match lastVal's range.
   *
   */

   RwReal DegAngDiffAndRange(RwReal &newVal, RwReal baseVal, RwReal lastVal)
   {
      RWS_FUNCTION("RWS::DegAngDiffAndRange");

      RwReal temp;

      // First off, need to range the new value, so it matches the lastVal range. Assume that
      // they cannot be out be a linear value of greater than 180 degrees.

      temp = lastVal - newVal;

      if (RwFabs(temp) > 180)
      {
         RwInt32 turns;

         // Out of range, so adjust it...

         if (temp > 0)
         {
            turns = static_cast<RwInt32>((temp + 180.0f) / 360.0f);
         }
         else
         {
            turns = static_cast<RwInt32>((temp - 180.0f) / 360.0f);
         }

         newVal += 360.0f * static_cast<RwReal>(turns);
         temp = lastVal - newVal;
      }

      // Return the difference

      RWS_RETURN(baseVal - newVal);
   }

   /**
   *
   * \ingroup MathsToolkit
   *
   * Determine angle difference between two angles in radians
   *
   * \param a0 first angle as a RwReal
   * \param a1 second angle as a RwReal
   *
   * \return the difference between the two angles as a RwReal.
   *
   */

   RwReal RadAngDiff(RwReal a0, RwReal a1)
   {
      RWS_FUNCTION("RWS::RadAngDiff");

      RwReal a2;

      a2 = a1 - a0;
      while(a2 < 0.0f) a2 += (rwPI*2.0f);
      while(a2 >= (rwPI*2.0f)) a2 -= (rwPI*2.0f);
      if(a2 >= rwPI) a2 -= (rwPI*2.0f);

      RWS_RETURN (a2);
   }

   /**
   *
   * Performs a low pass filter operation, where y0 is the current input value
   * and x1 is the previous output value. Output = x1 + (y0 - x1) * k. Therefore, the larger
   * the difference between the y0 and x0 the greater the increase in the output
   *
   * \param y0 input value as RwReal
   * \param x1 second input value as RwReal
   * \param k third input value, should be between 0 and 1, RwReal
   *
   * \return new value as RwReal
   *
   */

   RwReal LowPassFilter(RwReal y0,RwReal x1,RwReal k)
   {
      RWS_FUNCTION("RWS::LowPassFilter");

      RWS_RETURN (x1+(y0-x1)*k);
   }

   //////////////////////////////
   //
   // FrameTrans class functions.
   //
   //////////////////////////////

   /**
   *
   * Sets the up, forward and right vectors from the passed in deflection and heading. The 'Recalc'
   * function must be called after this function. However, several other 'Set' functions can be
   * called before. It is more efficient to call them all and then make one call to 'Recalc'.
   *
   * \see SetOffset
   * \see SetUp
   * \see SetRight
   * \see SetForward
   * \see Recalc
   *
   * \param deflect angle off z axis in degrees, RwReal
   * \param heading angle round y axis in degrees, RwReal
   *
   */

   void FrameTrans::SetFromAngles(RwReal deflect, RwReal heading)
   {
      RWS_FUNCTION("RWS::FrameTrans::SetFromAngles");

      RwMatrix mtx;
      RwV3d axis;

      // From matrix by rotating around heading on z (look at) axis.

      axis.x = 0;
      axis.y = 0;
      axis.z = 1;
      RwMatrixRotate(&mtx, &axis, heading, rwCOMBINEREPLACE);

      // Extract the x (right) axis from the matrix (will have been changed by above).

      axis = *RwMatrixGetRight(&mtx);
      RwMatrixRotate(&mtx, &axis, deflect, rwCOMBINEPOSTCONCAT);

      // Form the axis from this...

      axis.x = 1;
      axis.y = 0;
      axis.z = 0;
      RwV3dTransformPoints(&m_rightVect, &axis, 1, &mtx);

      axis.x = 0;
      axis.y = 1;
      axis.z = 0;
      RwV3dTransformPoints(&m_upVect, &axis, 1, &mtx);

      axis.x = 0;
      axis.y = 0;
      axis.z = 1;
      RwV3dTransformPoints(&m_forwardVect, &axis, 1, &mtx);

      dirty = true;

      RWS_RETURNVOID();
   }

   /**
   *
   * Called after changing the axis values, via the 'Set' functions. Must be called before using the
   * 'FrameTrans' function or old/invalid values will be used. A call to this function will only actually
   * recalculate if the data is dirty. So calling each frame will only result in recalculation if the data
   * has changed.
   *
   * NOTE: This function uses "optimize matrix" and could be slow if used very frequently.
   *
   * \see SetOffset
   * \see SetUp
   * \see SetRight
   * \see SetForward
   * \see SetFromAngles
   * \see FrameTrans
   *
   */

   void FrameTrans::Recalc(void)
   {
      RWS_FUNCTION("RWS::FrameTrans::Recalc");

      if (dirty)
      {
         // Build view matrix to calculate frame change. Right, I've been told my RW dev to do it
         // by replacing the vectors returned by up, right, at and pos and then call update, so...

         *RwMatrixGetRight(&m_transMtx) = m_rightVect;
         *RwMatrixGetUp(&m_transMtx) = m_upVect;
         *RwMatrixGetAt(&m_transMtx) = m_forwardVect;
         *RwMatrixGetPos(&m_transMtx) = m_offset;

         // Use optimize and not update, same but will detect cases there performance can be
         // increased. Slow, but since only call recalc when changing connection on objects
         // should be okay. If this is too slow, then add a 'FastRecalc' function and use
         // '...Update' instead of '...Optimize'.

         RwMatrixOptimize(&m_transMtx, 0);

         dirty = false;
      }

      RWS_RETURNVOID();
   }
}
