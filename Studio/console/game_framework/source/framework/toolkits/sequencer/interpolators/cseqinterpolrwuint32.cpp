/****************************************************************************
*
* File :      CSeqInterpolRwUInt32.cpp
*
* Abstract :  Frame by frame sequence controller for RwUInt32 data types.
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
#include "cseqinterpolrwuint32.h"
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

      CSeqInterpolRegister gRwUInt32DefaultReg(CSeqInterpolBase::MakeNew,
         CSeqInterpolBase::MaxSize, RWSTRING("RwUInt32"), 0, FALSE);

      // Interpolator specific ones...

//      CSeqInterpolRegister gRwUInt32InterpolReg(CSeqInterpolLinearRwUInt32::MakeNew,
//         CSeqInterpolLinearRwUInt32::MaxSize, RWSTRING("RwUInt32"),
//         INTERPOL_TYPE_LINEAR_NAME, FALSE);

      ////////////////////////////////////////
      //
      // CSeqInterpolLinearRwUInt32 functions.
      //
      //

      /**
      *
      * Constructor - sets up attribute packet buffer, with space for data change (RwUInt32)
      *               and links to default set/prep/step functions.
      *
      */

      CSeqInterpolLinearRwUInt32::CSeqInterpolLinearRwUInt32(CSeqItem &rData) :
         CSeqInterpolBase()
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwUInt32::CSeqInterpolLinearRwUInt32");
         RWS_RETURNVOID();
      }

      /**
      *
      * Destructor - frees up memory used, etc...
      *
      */

      CSeqInterpolLinearRwUInt32::~CSeqInterpolLinearRwUInt32(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwUInt32::~CSeqInterpolLinearRwUInt32");

         RWS_RETURNVOID();
      }

      /**
      *
      * Allocates a new instance of the CSeqInterpolLinearRwUInt32 class and returns it via
      * a base class pointer. Asserts if fails to allocated.
      *
      */

      CSeqInterpolBase *CSeqInterpolLinearRwUInt32::MakeNew(CSeqItem &rData)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwUInt32::MakeNew");
         RWS_ASSERT(rData.GetInterpolMem(), "Memory buffer is invalid");
         RWS_ASSERT(rData.GetInterpolMemSize() >= sizeof(CSeqInterpolLinearRwUInt32),
            "Memory buffer too small");

         CSeqInterpolLinearRwUInt32 *pNew = new(rData.GetInterpolMem())
            CSeqInterpolLinearRwUInt32(rData);
         RWS_ASSERT(pNew, "Could not allocate new object.");

         RWS_RETURN(pNew);
      }

      /**
      *
      * Returns the maximum size of memory need to hold this interpolator.
      *
      */

      RwUInt32 CSeqInterpolLinearRwUInt32::MaxSize(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwUInt32::MaxSize");
         RWS_RETURN(sizeof(CSeqInterpolLinearRwUInt32));
      }

      /**
      *
      * Sets the attribute packet data and sets to appropriate attribute handler. Uses the
      * 'end' key of the passed in CSeqItem.
      *
      */

      void CSeqInterpolLinearRwUInt32::Set(CSeqItem &rData)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwUInt32::SetInterpol");

         // Call base class to do this.

         CSeqInterpolBase::Set(rData);

         RWS_RETURNVOID();
      }

      /**
      *
      * Prepares to interpolates between last and end keys in the passed in CSeqItem.
      *
      */

      void CSeqInterpolLinearRwUInt32::Prep(CSeqItem &rData)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwUInt32::Prep");
         RWS_ASSERT(rData.LengthTime(), "Invalid length time (zero)");

         // Calculate divisor for total sequence time, as a multiplier...

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

      void CSeqInterpolLinearRwUInt32::PrepReuse(CSeqItem &rData)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwUInt32::PrepReuse");

         // No special case, so call standard one.

         Prep(rData);

         RWS_RETURNVOID();
      }

      /**
      *
      * Calculates the value for the data slot used in 'Step'. Moved out for maintainability.
      *
      */

      inline RwUInt32 CalcValue(const RwUInt32 *pStartVal, const RwUInt32 *pEndVal, RwReal tVal)
      {
         return(*pStartVal + static_cast<RwUInt32>(static_cast<RwReal>
            (*pEndVal - *pStartVal) * tVal));
      }

      /**
      *
      * Calculates the intermediate value for the RwUInt32 given the 'now' time and the
      * data held in the CSeqItem passed in. ASSUMES that the 'Prep' has been
      * called before the first call to this function. Calls the
      * 'Step(CSeqItem &rData, RwReal delta)' version to do the work.
      *
      */

      void CSeqInterpolLinearRwUInt32::Step(CSeqItem &rData, RwUInt32 nowTime)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwUInt32::Step (time)");

         RwReal delta = static_cast<RwReal>(nowTime - rData.StartTime()) * m_oneOverSeqTime;
         Step(rData, delta);

         RWS_RETURNVOID();
      }

      /**
      *
      * Calculates the intermediate value for the RwUInt32 given a delta value and the
      * data held in the CSeqItem passed in. ASSUMES delta will be in the range of 0.0
      * to 1.0 ONLY.
      *
      */

      void CSeqInterpolLinearRwUInt32::Step(CSeqItem &rData, RwReal delta)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwUInt32::Step (delta)");
         RWS_ASSERT(rData.LastKey()->DataSize(), "Last key's data size invalid.");
         RWS_ASSERT(rData.LastKey()->Data(), "Last key's data pointer invalid.");
         RWS_ASSERT(rData.EndKey()->DataSize(), "End key's data size invalid.");
         RWS_ASSERT(rData.EndKey()->Data(), "End key's data pointer invalid.");
         RWS_ASSERT(delta >= 0.0f && delta < 1.0f, "Delta is out of range.");

         const RwUInt32 *pEndVal = reinterpret_cast<const RwUInt32 *>
               (rData.EndKey()->Data()),
            *pStartVal = reinterpret_cast<const RwUInt32 *>
               (rData.LastKey()->Data());
         RwUInt32 *pCalcVal;

         // Calculate the current value & insert into attribute data...

         RWS_ASSERT(rData.BuffSlot(), "no slot to set data with.");

         if (rData.BuffSlot()->IsDirectSet())
         {
            pCalcVal = reinterpret_cast<RwUInt32 *>(rData.BuffSlot()->GetDirectDataSlot());

            if (pCalcVal)
            {
               *pCalcVal = CalcValue(pStartVal, pEndVal, delta);
               rData.BuffSlot()->DirectUpdate(pCalcVal);
            }
            else
            {
               RwUInt32 calcVal = CalcValue(pStartVal, pEndVal, delta);
               rData.BuffSlot()->DirectUpdate(&calcVal);
            }
         }
         else
         {
            pCalcVal = reinterpret_cast<RwUInt32 *>(rData.BuffSlot()->GetBuffDataSlot(sizeof(RwUInt32)));
            *pCalcVal = CalcValue(pStartVal, pEndVal, delta);
         }

         RWS_RETURNVOID();
      }
   }  // namespace SEQUENCE
}     // namespace RWS
