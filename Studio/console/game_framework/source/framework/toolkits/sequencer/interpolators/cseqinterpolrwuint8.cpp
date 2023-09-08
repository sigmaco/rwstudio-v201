/****************************************************************************
*
* File :      CSeqInterpolRwUInt8.cpp
*
* Abstract :  Frame by frame sequence controller for RwUInt8 data types.
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
// RenderWare Studio Includes
//

#include "cseqinterpolrwuint8.h"
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

      CSeqInterpolRegister gRwUInt8DefaultReg(CSeqInterpolBase::MakeNew,
         CSeqInterpolBase::MaxSize, RWSTRING("RwUInt8"), 0, FALSE);

      // Interpolator specific ones...

      CSeqInterpolRegister gRwUInt8InterpolReg(CSeqInterpolLinearRwUInt8::MakeNew,
         CSeqInterpolLinearRwUInt8::MaxSize, RWSTRING("RwUInt8"),
         INTERPOL_TYPE_LINEAR_NAME, FALSE);

      ///////////////////////////////////////
      //
      // CSeqInterpolLinearRwUInt8 functions.
      //
      //

      /**
      *
      * Constructor - sets up attribute packet buffer, with space for data change (RwUInt8)
      *               and links to default set/prep/step functions.
      *
      */

      CSeqInterpolLinearRwUInt8::CSeqInterpolLinearRwUInt8(CSeqItem &rData) :
         CSeqInterpolBase()
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwUInt8::CSeqInterpolLinearRwUInt8");
         RWS_RETURNVOID();
      }

      /**
      *
      * Destructor - frees up memory used, etc...
      *
      */

      CSeqInterpolLinearRwUInt8::~CSeqInterpolLinearRwUInt8(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwUInt8::~CSeqInterpolLinearRwUInt8");

         RWS_RETURNVOID();
      }

      /**
      *
      * Allocates a new instance of the CSeqInterpolLinearRwUInt8 class and returns it via
      * a base class pointer. Asserts if fails to allocated.
      *
      */

      CSeqInterpolBase *CSeqInterpolLinearRwUInt8::MakeNew(CSeqItem &rData)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwUInt8::MakeNew");
         RWS_ASSERT(rData.GetInterpolMem(), "Memory buffer is invalid");
         RWS_ASSERT(rData.GetInterpolMemSize() >= sizeof(CSeqInterpolLinearRwUInt8), 
            "Memory buffer too small");

         // Use placement new to create the object within the memory buffer...

         CSeqInterpolLinearRwUInt8 *pNew = new(rData.GetInterpolMem())
            CSeqInterpolLinearRwUInt8(rData);
         RWS_ASSERT(pNew, "Could not allocate new object.");

         RWS_RETURN(pNew);
      }

      /**
      *
      * Returns the maximum size of memory need to hold this interpolator.
      *
      */

      RwUInt32 CSeqInterpolLinearRwUInt8::MaxSize(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwUInt8::MaxSize");
         RWS_RETURN(sizeof(CSeqInterpolLinearRwUInt8));
      }

      /**
      *
      * Sets the attribute packet data and sets to appropriate attribute handler. Uses the
      * 'end' key of the passed in CSeqItem.
      *
      */

      void CSeqInterpolLinearRwUInt8::Set(CSeqItem &rData)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwUInt8::SetInterpol");

         // Call base class to do this.

         CSeqInterpolBase::Set(rData);

         RWS_RETURNVOID();
      }

      /**
      *
      * Prepares to interpolates between last and end keys in the passed in CSeqItem.
      *
      */

      void CSeqInterpolLinearRwUInt8::Prep(CSeqItem &rData)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwUInt8::Prep");
         RWS_ASSERT(rData.LengthTime(), "Invalid length time (zero)");
         RWS_ASSERT(rData.LastKey()->DataSize(), "Last key's data size invalid.");
         RWS_ASSERT(rData.LastKey()->Data(), "Last key's data pointer invalid.");
         RWS_ASSERT(rData.EndKey()->DataSize(), "End key's data size invalid.");
         RWS_ASSERT(rData.EndKey()->Data(), "End key's data pointer invalid.");

         const RwUInt8 *pEndVal = reinterpret_cast<const RwUInt8 *>
               (rData.EndKey()->Data()),
            *pStartVal = reinterpret_cast<const RwUInt8 *>
               (rData.LastKey()->Data());

         // Calculate per-millisecond step values...

         m_scaledStep = (static_cast<RwUInt32>(*pEndVal - *pStartVal) << 24) /
            rData.LengthTime();

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

      void CSeqInterpolLinearRwUInt8::PrepReuse(CSeqItem &rData)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwUInt8::PrepReuse");

         // No special case, so call standard one.

         Prep(rData);

         RWS_RETURNVOID();
      }

      /**
      *
      * Calculates the value for the data slot used in 'Step'. Moved out for maintainability.
      *
      */

      inline RwUInt8 CalcValue(const RwUInt8 *pStartVal, RwUInt32 scaledStep,
         RwUInt32 nowTime, RwUInt32 startTime)
      {
         return(*pStartVal + static_cast<RwUInt8>((scaledStep *
            (nowTime - startTime)) >> 24));
      }

      /**
      *
      * Calculates the intermediate value for the RwUInt8 given the 'now' time and the
      * data held in the CSeqItem passed in. ASSUMES that the 'Prep' has been
      * called before the first call to this function.
      *
      */

      void CSeqInterpolLinearRwUInt8::Step(CSeqItem &rData, RwUInt32 nowTime)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwUInt8::Step (time)");
         RWS_ASSERT(rData.LastKey()->DataSize(), "Last key's data size invalid.");
         RWS_ASSERT(rData.LastKey()->Data(), "Last key's data pointer invalid.");

         const RwUInt8 *pStartVal = reinterpret_cast<const RwUInt8 *>
               (rData.LastKey()->Data());
         RwUInt8 *pCalcVal;

         // Calculate the current value & insert into attribute data...

         RWS_ASSERT(rData.BuffSlot(), "no slot to set data with.");

         if (rData.BuffSlot()->IsDirectSet())
         {
            pCalcVal = reinterpret_cast<RwUInt8 *>(rData.BuffSlot()->GetDirectDataSlot());

            if (pCalcVal)
            {
               *pCalcVal = CalcValue(pStartVal, m_scaledStep, nowTime, rData.StartTime());
               rData.BuffSlot()->DirectUpdate(pCalcVal);
            }
            else
            {
               RwUInt8 calcVal = CalcValue(pStartVal, m_scaledStep, nowTime, rData.StartTime());
               rData.BuffSlot()->DirectUpdate(&calcVal);
            }
         }
         else
         {
            pCalcVal = reinterpret_cast<RwUInt8 *>(rData.BuffSlot()->GetBuffDataSlot(sizeof(RwUInt8)));
            *pCalcVal = CalcValue(pStartVal, m_scaledStep, nowTime, rData.StartTime());
         }

         RWS_RETURNVOID();
      }

      /**
      *
      * Calculates the value for the data slot used in 'Step'. Moved out for maintainability.
      * Calculates the value 'on-the-fly' from the passed in delta and does not require the
      * data setup in the 'Prep' function.
      *
      * NOTE: Function is not designed for speed. Should only be used for design build dynamic
      *       editing and such like.
      *
      */

      inline RwUInt8 CalcValue(const RwUInt8 *pStartVal, const RwUInt8 *pEndVal, RwReal delta)
      {
         RwReal realStartVal = static_cast<RwReal>(*pStartVal),
            realEndVal = static_cast<RwReal>(*pEndVal);

         return(static_cast<RwUInt8>(realStartVal + (realEndVal - realStartVal) * delta));
      }

      /**
      *
      * Calculates the intermediate value for the RwUInt8 given a delta value and the
      * data held in the CSeqItem passed in. ASSUMES delta will be in the range of 0.0
      * to 1.0 ONLY.
      *
      */

      void CSeqInterpolLinearRwUInt8::Step(CSeqItem &rData, RwReal delta)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwUInt8::Step (delta)");
         RWS_ASSERT(rData.LastKey()->DataSize(), "Last key's data size invalid.");
         RWS_ASSERT(rData.LastKey()->Data(), "Last key's data pointer invalid.");
         RWS_ASSERT(rData.EndKey()->DataSize(), "End key's data size invalid.");
         RWS_ASSERT(rData.EndKey()->Data(), "End key's data pointer invalid.");
         RWS_ASSERT(delta >= 0.0f && delta <= 1.0f, "Delta is out of range.");

         const RwUInt8 *pStartVal = reinterpret_cast<const RwUInt8 *>
               (rData.LastKey()->Data()),
            *pEndVal =  reinterpret_cast<const RwUInt8 *>(rData.EndKey()->Data());
         RwUInt8 *pCalcVal;

         // Calculate the current value & insert into attribute data...

         RWS_ASSERT(rData.BuffSlot(), "no slot to set data with.");

         if (rData.BuffSlot()->IsDirectSet())
         {
            pCalcVal = reinterpret_cast<RwUInt8 *>(rData.BuffSlot()->GetDirectDataSlot());

            if (pCalcVal)
            {
               *pCalcVal = CalcValue(pStartVal, pEndVal, delta);
               rData.BuffSlot()->DirectUpdate(pCalcVal);
            }
            else
            {
               RwUInt8 calcVal = CalcValue(pStartVal, pEndVal, delta);
               rData.BuffSlot()->DirectUpdate(&calcVal);
            }
         }
         else
         {
            pCalcVal = reinterpret_cast<RwUInt8 *>(rData.BuffSlot()->GetBuffDataSlot(sizeof(RwUInt8)));
            *pCalcVal = CalcValue(pStartVal, pEndVal, delta);
         }

         RWS_RETURNVOID();
      }
   }  // namespace SEQUENCE
}     // namespace RWS
