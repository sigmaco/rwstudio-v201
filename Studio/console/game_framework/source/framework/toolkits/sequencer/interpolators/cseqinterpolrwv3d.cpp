/****************************************************************************
*
* File :      CSeqInterpolRwV3d.cpp
*
* Abstract :  Frame by frame sequence controller for RwV3d data types.
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
* Copyright (c) 2002 Criterion Software Ltd.
* All Rights Reserved.
*
* RenderWare is a trademark of Canon Inc.
*
*****************************************************************************/

#include "precomp.h"

// Left to its own devices, the MW linker will dead strip the registrar ctors, so...

#ifdef __MWERKS__
#pragma force_active on
#pragma exceptions off     // Turn off exceptions due to bug in MW Dwarf
#endif                     // Generator for PS2, present in CW 3.04

//////////////////////////////////////////////////////////////////
//
// System Includes
//
#include <new.h>

//////////////////////////////////////////////////////////////////
//
// RenderWare Includes
//
#include <rtquat.h>

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "cseqinterpolrwv3d.h"
#include "../cseqtxattrhand.h"

namespace RWS
{
   namespace SEQUENCE
   {
      /////////////////////////////////////////////////////////////////
      //
      // Type name -> interpolator mappings, default & build-in types.
      //
      // By creating a global variable, the mapping will be created and
      // stored for later searching.
      //
      //

      // Default - ALL type groups MUST have one of these.

      CSeqInterpolRegister gRwV3dDefaultReg(CSeqInterpolBase::MakeNew,
         CSeqInterpolBase::MaxSize, RWSTRING("RwV3d"), 0, FALSE);

      // Interpolator specific ones...

      CSeqInterpolRegister gRwV3dLinearInterpolReg(CSeqInterpolLinearRwV3d::MakeNew,
         CSeqInterpolLinearRwV3d::MaxSize, RWSTRING("RwV3d"),
         INTERPOL_TYPE_LINEAR_NAME, FALSE);

      CSeqInterpolRegister gRwV3dRotationInterpolReg(CSeqInterpolRotationRwV3d::MakeNew,
         CSeqInterpolRotationRwV3d::MaxSize, RWSTRING("RwV3d"),
         INTERPOL_TYPE_ROTATION_NAME, FALSE);

      /////////////////////////////////////
      //
      // CSeqInterpolLinearRwV3d functions.
      //
      //

      /**
      *
      * Constructor - sets up attribute packet buffer, with space for data change (RwV3d)
      *               and links to default set/prep/step functions.
      *
      */

      CSeqInterpolLinearRwV3d::CSeqInterpolLinearRwV3d(CSeqItem &rData) :
         CSeqInterpolBase()
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwV3d::CSeqInterpolLinearRwV3d");
         RWS_RETURNVOID();
      }

      /**
      *
      * Destructor - frees up memory used, etc...
      *
      */

      CSeqInterpolLinearRwV3d::~CSeqInterpolLinearRwV3d(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwV3d::~CSeqInterpolLinearRwV3d");
         RWS_RETURNVOID();
      }

      /**
      *
      * Allocates a new instance of the CSeqInterpolLinearRwV3d class and returns it via
      * a base class pointer. Asserts if fails to allocated.
      *
      */

      CSeqInterpolBase *CSeqInterpolLinearRwV3d::MakeNew(CSeqItem &rData)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwV3d::MakeNew");
         RWS_ASSERT(rData.GetInterpolMem(), "Memory buffer is invalid");
         RWS_ASSERT(rData.GetInterpolMemSize() >= sizeof(CSeqInterpolLinearRwV3d), 
            "Memory buffer too small");

         CSeqInterpolLinearRwV3d *pNew = new(rData.GetInterpolMem())
            CSeqInterpolLinearRwV3d(rData);
         RWS_ASSERT(pNew, "Could not allocate new object.");

         RWS_RETURN(pNew);
      }

      /**
      *
      * Returns the maximum size of memory need to hold this interpolator.
      *
      */

      RwUInt32 CSeqInterpolLinearRwV3d::MaxSize(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwV3d::MaxSize");
         RWS_RETURN(sizeof(CSeqInterpolLinearRwV3d));
      }

      /**
      *
      * Sets the attribute packet data and sets to appropriate attribute handler. Uses the
      * 'end' key of the passed in CSeqItem.
      *
      */

      void CSeqInterpolLinearRwV3d::Set(CSeqItem &rData)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwV3d::SetInterpol");

         // Call base class to do this.

         CSeqInterpolBase::Set(rData);

         RWS_RETURNVOID();
      }

      /**
      *
      * Prepares to interpolates between last and end keys in the passed in CSeqItem.
      *
      */

      void CSeqInterpolLinearRwV3d::Prep(CSeqItem &rData)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwV3d::Prep");
         RWS_ASSERT(rData.LengthTime(), "Invalid length time (zero)");
         RWS_ASSERT(rData.LastKey()->DataSize(), "Last key's data size invalid.");
         RWS_ASSERT(rData.LastKey()->Data(), "Last key's data pointer invalid.");
         RWS_ASSERT(rData.EndKey()->DataSize(), "End key's data size invalid.");
         RWS_ASSERT(rData.EndKey()->Data(), "End key's data pointer invalid.");

         const RwV3d *pEndVal = reinterpret_cast<const RwV3d *>
               (rData.EndKey()->Data()),
            *pStartVal = reinterpret_cast<const RwV3d *>
               (rData.LastKey()->Data());

         // Calculate step values per millisecond for each element of the vector.

         m_xInc = (pEndVal->x - pStartVal->x);
         m_yInc = (pEndVal->y - pStartVal->y);
         m_zInc = (pEndVal->z - pStartVal->z);

         m_oneOverSeqTime = 1.0f / static_cast<RwReal>(rData.LengthTime());

         RWS_RETURNVOID();
      }

      /**
      *
      * Prepares to interpolate between last and end keys in the passed in CSeqItem.
      * This is a special case where the same interpolator as last time is being used
      * and the last keyframe is the same as the end keyframe last time. Called to
      * allow the interpolator to re-use data, etc.
      *
      */

      void CSeqInterpolLinearRwV3d::PrepReuse(CSeqItem &rData)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwV3d::PrepReuse");

         // No special case, so call standard one.

         Prep(rData);

         RWS_RETURNVOID();
      }

      /**
      *
      * Calculates the value for the data slot used in 'Step'. Moved out for maintainability.
      *
      */

      inline void CalcLinearValue(RwV3d *pOutVal, const RwV3d *pStartVal, RwReal xInc,
         RwReal yInc, RwReal zInc, RwReal delta)
      {
         pOutVal->x = pStartVal->x + xInc * delta;
         pOutVal->y = pStartVal->y + yInc * delta;
         pOutVal->z = pStartVal->z + zInc * delta;
      }

      /**
      *
      * Calculates the intermediate value for the RwV3d given a time and the
      * data held in the CSeqItem passed in. ASSUMES that the 'Prep' has been
      * called before the first call to this function. Calls 'delta' version
      * to do actual work.
      *
      */

      void CSeqInterpolLinearRwV3d::Step(CSeqItem &rData, RwUInt32 nowTime)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwV3d::Step (time)");
         RWS_ASSERT(rData.LastKey()->DataSize(), "Last key's data size invalid.");
         RWS_ASSERT(rData.LastKey()->Data(), "Last key's data pointer invalid.");

         RwReal mSecsSinceStart = static_cast<RwReal>(nowTime - rData.StartTime()),
            delta = mSecsSinceStart * m_oneOverSeqTime;
         Step(rData, delta);

         RWS_RETURNVOID();
      }

      /**
      *
      * Calculates the intermediate value for the RwV3d given a delta and the
      * data held in the CSeqItem passed in. ASSUMES that the 'Prep' has been
      * called before the first call to this function. The data is ASSUMED to
      * be between 0.0 and 1.0 ONLY.
      *
      */

      void CSeqInterpolLinearRwV3d::Step(CSeqItem &rData, RwReal delta)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwV3d::Step (delta)");
         RWS_ASSERT(rData.LastKey()->DataSize(), "Last key's data size invalid.");
         RWS_ASSERT(rData.LastKey()->Data(), "Last key's data pointer invalid.");
         RWS_ASSERT(delta >= 0.0f && delta <= 1.0f, "Delta is out of range.");

         const RwV3d *pStartVal = reinterpret_cast<const RwV3d *>
               (rData.LastKey()->Data());
         RwV3d *pCalcVal;

         // Calculate the current value & insert into attribute data...

         RWS_ASSERT(rData.BuffSlot(), "no slot to set data with.");

         if (rData.BuffSlot()->IsDirectSet())
         {
            pCalcVal = reinterpret_cast<RwV3d *>(rData.BuffSlot()->GetDirectDataSlot());

            if (pCalcVal)
            {
               CalcLinearValue(pCalcVal, pStartVal, m_xInc, m_yInc, m_zInc, delta);
               rData.BuffSlot()->DirectUpdate(pCalcVal);
            }
            else
            {
               RwV3d calcVal;
               CalcLinearValue(&calcVal, pStartVal, m_xInc, m_yInc, m_zInc, delta);
               rData.BuffSlot()->DirectUpdate(&calcVal);
            }
         }
         else
         {
            pCalcVal = reinterpret_cast<RwV3d *>(rData.BuffSlot()->GetBuffDataSlot(sizeof(RwV3d)));
            CalcLinearValue(pCalcVal, pStartVal, m_xInc, m_yInc, m_zInc, delta);
         }

         RWS_RETURNVOID();
      }

      /////////////////////////////////////
      //
      // CSeqInterpolRotationRwV3d functions.
      //
      //

      /**
      *
      * Constructor - sets up attribute packet buffer, with space for data change (RwV3d)
      *               and links to default set/prep/step functions.
      *
      */

      CSeqInterpolRotationRwV3d::CSeqInterpolRotationRwV3d(CSeqItem &rData) :
         CSeqInterpolBase()
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolRotationRwV3d::CSeqInterpolRotationRwV3d");
         RWS_RETURNVOID();
      }

      /**
      *
      * Destructor - frees up memory used, etc...
      *
      */

      CSeqInterpolRotationRwV3d::~CSeqInterpolRotationRwV3d(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolRotationRwV3d::~CSeqInterpolRotationRwV3d");
         RWS_RETURNVOID();
      }

      /**
      *
      * Allocates a new instance of the CSeqInterpolRotationRwV3d class and returns it via
      * a base class pointer. Asserts if fails to allocated.
      *
      */

      CSeqInterpolBase *CSeqInterpolRotationRwV3d::MakeNew(CSeqItem &rData)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolRotationRwV3d::MakeNew");
         RWS_ASSERT(rData.GetInterpolMem(), "Memory buffer is invalid");
         RWS_ASSERT(rData.GetInterpolMemSize() >= sizeof(CSeqInterpolRotationRwV3d), 
            "Memory buffer too small");

         CSeqInterpolRotationRwV3d *pNew = new(rData.GetInterpolMem())
            CSeqInterpolRotationRwV3d(rData);
         RWS_ASSERT(pNew, "Could not allocate new object.");

         RWS_RETURN(pNew);
      }

      /**
      *
      * Returns the maximum size of memory need to hold this interpolator.
      *
      */

      RwUInt32 CSeqInterpolRotationRwV3d::MaxSize(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolRotationRwV3d::MaxSize");
         RWS_RETURN(sizeof(CSeqInterpolRotationRwV3d));
      }

      /**
      *
      * Sets the attribute packet data and sets to appropriate attribute handler. Uses the
      * 'end' key of the passed in CSeqItem.
      *
      */

      void CSeqInterpolRotationRwV3d::Set(CSeqItem &rData)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolRotationRwV3d::SetInterpol");

         // Call base class to do this.

         CSeqInterpolBase::Set(rData);

         RWS_RETURNVOID();
      }

      /**
      *
      * Prepares to interpolates between last and end keys in the passed in CSeqItem.
      *
      */

      void CSeqInterpolRotationRwV3d::Prep(CSeqItem &rData)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolRotationRwV3d::Prep");
         RWS_ASSERT(rData.LengthTime(), "Invalid length time (zero)");
         RWS_ASSERT(rData.LastKey()->DataSize(), "Last key's data size invalid.");
         RWS_ASSERT(rData.LastKey()->Data(), "Last key's data pointer invalid.");
         RWS_ASSERT(rData.EndKey()->DataSize(), "End key's data size invalid.");
         RWS_ASSERT(rData.EndKey()->Data(), "End key's data pointer invalid.");

         const RwV3d *pEndVal = reinterpret_cast<const RwV3d *>
               (rData.EndKey()->Data()),
            *pStartVal = reinterpret_cast<const RwV3d *>
               (rData.LastKey()->Data());
         RwV3d startNorm,
            endNorm;
         RwReal cosAngle,
            endLength;

         // Create normalised versions of the two vectors (keep original lengths).

         m_startLength = RwV3dLength(pStartVal);
         endLength = RwV3dLength(pEndVal);

         RwV3dScale(&startNorm, pStartVal, 1.0f / m_startLength);
         RwV3dScale(&endNorm, pEndVal, 1.0f / endLength);

         // Find angle between two. Using arc-cosine, which is slow, but only used once
         // per interpolation.

         cosAngle = RwV3dDotProduct(&startNorm, &endNorm);
         m_degAngle = RWRAD2DEG(RwACos(cosAngle));

         // Can the real axis be calculated ?

         if (m_degAngle > 0.0f)
         {
            RwV3dCrossProduct(&m_rotAxis, &startNorm, &endNorm);
            RwV3dNormalize(&m_rotAxis, &m_rotAxis);
         }

         // Setup the data needed.

         m_oneOverSeqTime = 1.0f / static_cast<RwReal>(rData.LengthTime());
         m_lengthInc = endLength - m_startLength;

         RWS_RETURNVOID();
      }

      /**
      *
      * Prepares to interpolate between last and end keys in the passed in CSeqItem.
      * This is a special case where the same interpolator as last time is being used
      * and the last keyframe is the same as the end keyframe last time. Called to
      * allow the interpolator to re-use data, etc.
      *
      */

      void CSeqInterpolRotationRwV3d::PrepReuse(CSeqItem &rData)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolRotationRwV3d::PrepReuse");

         // No special case, so call standard one.

         Prep(rData);

         RWS_RETURNVOID();
      }

      /**
      *
      * Calculates the value for the data slot used in 'Step'. Moved out for maintainability.
      *
      */

      inline void CalcRotationalValue(RwV3d *pOutVal, const RwV3d *pStartVal, const RtQuat &curQuat,
         const RwReal startLength, const RwReal lengthInc, const RwReal delta)
      {
         // Rotate start vector by this quaternion to get output vector, then
         // scale to be correct length.

         RtQuatTransformVectors(pOutVal, pStartVal, 1, &curQuat);

         // Modify the length of this vector.

         if (lengthInc != 0.0f)
         {
            RwV3dScale(pOutVal, pOutVal, startLength + lengthInc * delta);
         }
      }

      /**
      *
      * Performs actual calculation and storage of values. Does NOT do any checks and
      * therefore should ONLY be called from the 'Step' functions which do these checks.
      * Factored out for ease of maintenance.
      *
      */

      inline void CSeqInterpolRotationRwV3d::CalcAndStoreValue(CSeqItem &rData, RwReal delta)
      {
         const RwV3d *pStartVal = reinterpret_cast<const RwV3d *>
            (rData.LastKey()->Data());
         RwV3d *pCalcVal;
         RtQuat curQuat;
         RwReal curAngle;

         // Calculate the current value...

         curAngle = m_degAngle * delta;
         RtQuatRotate(&curQuat, &m_rotAxis, curAngle, rwCOMBINEREPLACE);

         // Insert into attribute data...

         RWS_ASSERT(rData.BuffSlot(), "no slot to set data with.");

         if (rData.BuffSlot()->IsDirectSet())
         {
            pCalcVal = reinterpret_cast<RwV3d *>(rData.BuffSlot()->GetDirectDataSlot());

            if (pCalcVal)
            {
               CalcRotationalValue(pCalcVal, pStartVal, curQuat, m_startLength,
                  m_lengthInc, delta);
               rData.BuffSlot()->DirectUpdate(pCalcVal);
            }
            else
            {
               RwV3d calcVal;
               CalcRotationalValue(&calcVal, pStartVal, curQuat, m_startLength, 
                  m_lengthInc, delta);
               rData.BuffSlot()->DirectUpdate(&calcVal);
            }
         }
         else
         {
            pCalcVal = reinterpret_cast<RwV3d *>(rData.BuffSlot()->GetBuffDataSlot(sizeof(RwV3d)));
            CalcRotationalValue(pCalcVal, pStartVal, curQuat, m_startLength, m_lengthInc, delta);
         }
      }

      /**
      *
      * Calculates the intermediate value for the RwV3d given the 'now' time and the
      * data held in the CSeqItem passed in. ASSUMES that the 'Prep' has been
      * called before the first call to this function.
      *
      */

      void CSeqInterpolRotationRwV3d::Step(CSeqItem &rData, RwUInt32 nowTime)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolRotationRwV3d::Step (time)");
         RWS_ASSERT(rData.LastKey()->DataSize(), "Last key's data size invalid.");
         RWS_ASSERT(rData.LastKey()->Data(), "Last key's data pointer invalid.");

         // Any interpolation ?

         if (m_degAngle > 0.0f)
         {
            RwReal timeStep = static_cast<RwReal>(nowTime - rData.StartTime()),
               delta;

            // Calculate the delta & call setup code.

            delta = timeStep * m_oneOverSeqTime;
            CalcAndStoreValue(rData, delta);
         }

         RWS_RETURNVOID();
      }
      /**
      *
      * Calculates the intermediate value for the RwV3d given a delta and the
      * data held in the CSeqItem passed in. ASSUMES that the 'Prep' has been
      * called before the first call to this function. The delta value is
      * ASSUMED to be between 0.0 and 1.0 ONLY.
      *
      */

      void CSeqInterpolRotationRwV3d::Step(CSeqItem &rData, RwReal delta)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolRotationRwV3d::Step (delta)");
         RWS_ASSERT(rData.LastKey()->DataSize(), "Last key's data size invalid.");
         RWS_ASSERT(rData.LastKey()->Data(), "Last key's data pointer invalid.");
         RWS_ASSERT(delta >= 0.0f && delta <= 1.0f, "Delta is out of range.");

         // Any interpolation ?

         if (m_degAngle > 0.0f)
         {
            // Set using delta value...

            CalcAndStoreValue(rData, delta);
         }

         RWS_RETURNVOID();
      }
   }  // namespace SEQUENCE
}     // namespace RWS
