/****************************************************************************
*
* File :      CSeqInterpolRwUInt16.cpp
*
* Abstract :  Frame by frame sequence controller for RwUInt16 data types.
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

#include "cseqinterpolrwuint16.h"
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

      CSeqInterpolRegister gRwUInt16DefaultReg(CSeqInterpolBase::MakeNew,
         CSeqInterpolBase::MaxSize, RWSTRING("RwUInt16"), 0, FALSE);

      // Interpolator specific ones...

      CSeqInterpolRegister gRwUInt16InterpolReg(CSeqInterpolLinearRwUInt16::MakeNew,
         CSeqInterpolLinearRwUInt16::MaxSize, RWSTRING("RwUInt16"),
         INTERPOL_TYPE_LINEAR_NAME, FALSE);

      ////////////////////////////////////////
      //
      // CSeqInterpolLinearRwUInt16 functions.
      //
      //

      /**
      *
      * Constructor - sets up attribute packet buffer, with space for data change (RwUInt16)
      *               and links to default set/prep/step functions.
      *
      */

      CSeqInterpolLinearRwUInt16::CSeqInterpolLinearRwUInt16(CSeqItem &rData) :
         CSeqInterpolBase()
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwUInt16::CSeqInterpolLinearRwUInt16");
         RWS_RETURNVOID();
      }

      /**
      *
      * Destructor - frees up memory used, etc...
      *
      */

      CSeqInterpolLinearRwUInt16::~CSeqInterpolLinearRwUInt16(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwUInt16::~CSeqInterpolLinearRwUInt16");

         RWS_RETURNVOID();
      }

      /**
      *
      * Allocates a new instance of the CSeqInterpolLinearRwUInt16 class and returns it via
      * a base class pointer. Asserts if fails to allocated.
      *
      */

      CSeqInterpolBase *CSeqInterpolLinearRwUInt16::MakeNew(CSeqItem &rData)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwUInt16::MakeNew");
         RWS_ASSERT(rData.GetInterpolMem(), "Memory buffer is invalid");
         RWS_ASSERT(rData.GetInterpolMemSize() >= sizeof(CSeqInterpolLinearRwUInt16), 
            "Memory buffer too small");

         // Use placement new to create the object within the memory buffer...

         CSeqInterpolLinearRwUInt16 *pNew = new(rData.GetInterpolMem())
            CSeqInterpolLinearRwUInt16(rData);
         RWS_ASSERT(pNew, "Could not allocate new object.");

         RWS_RETURN(pNew);
      }

      /**
      *
      * Returns the maximum size of memory need to hold this interpolator.
      *
      */

      RwUInt32 CSeqInterpolLinearRwUInt16::MaxSize(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwUInt16::MaxSize");
         RWS_RETURN(sizeof(CSeqInterpolLinearRwUInt16));
      }

      /**
      *
      * Sets the attribute packet data and sets to appropriate attribute handler. Uses the
      * 'end' key of the passed in CSeqItem.
      *
      */

      void CSeqInterpolLinearRwUInt16::Set(CSeqItem &rData)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwUInt16::SetInterpol");

         // Call base class to do this.

         CSeqInterpolBase::Set(rData);

         RWS_RETURNVOID();
      }

      /**
      *
      * Prepares to interpolates between last and end keys in the passed in CSeqItem.
      *
      */

      void CSeqInterpolLinearRwUInt16::Prep(CSeqItem &rData)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwUInt16::Prep");
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

      void CSeqInterpolLinearRwUInt16::PrepReuse(CSeqItem &rData)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwUInt16::PrepReuse");

         // No special case, so call standard one.

         Prep(rData);

         RWS_RETURNVOID();
      }

      /**
      *
      * Calculates the value for the data slot used in 'Step'. Moved out for maintainability.
      *
      */

      inline RwUInt16 CalcValue(const RwUInt16 *pStartVal, const RwUInt16 *pEndVal, RwReal tVal)
      {
         return(*pStartVal + static_cast<RwUInt16>(static_cast<RwReal>
               (*pEndVal - *pStartVal) * tVal));
      }

      /**
      *
      * Calculates the intermediate value for the RwUInt16 given the 'now' time and the
      * data held in the CSeqItem passed in. ASSUMES that the 'Prep' has been
      * called before the first call to this function. Calls the
      * 'Step(CSeqItem &rData, RwReal delta)' function to do work.
      *
      */

      void CSeqInterpolLinearRwUInt16::Step(CSeqItem &rData, RwUInt32 nowTime)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwUInt16::Step (time)");

         RwReal tVal = static_cast<RwReal>(nowTime - rData.StartTime()) * m_oneOverSeqTime;
         Step(rData, nowTime);

         RWS_RETURNVOID();
      }

      /**
      *
      * Calculates the intermediate value for the RwUInt16 given a delta and the
      * data held in the CSeqItem passed in. ASSUMES that the 'Prep' has been
      * called before the first call to this function. ASSUMES delta value is in the
      * range of 0.0 to 1.0 ONLY.
      *
      */

      void CSeqInterpolLinearRwUInt16::Step(CSeqItem &rData, RwReal delta)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwUInt16::Step (delta)");
         RWS_ASSERT(rData.LastKey()->DataSize(), "Last key's data size invalid.");
         RWS_ASSERT(rData.LastKey()->Data(), "Last key's data pointer invalid.");
         RWS_ASSERT(rData.EndKey()->DataSize(), "End key's data size invalid.");
         RWS_ASSERT(rData.EndKey()->Data(), "End key's data pointer invalid.");
         RWS_ASSERT(delta >= 0.0f && delta <= 1.0f, "Delta is out of range.");

         const RwUInt16 *pEndVal = reinterpret_cast<const RwUInt16 *>
               (rData.EndKey()->Data()),
            *pStartVal = reinterpret_cast<const RwUInt16 *>
               (rData.LastKey()->Data());
         RwUInt16 *pCalcVal;

         // Calculate the current value & insert into attribute data...

         RWS_ASSERT(rData.BuffSlot(), "no slot to set data with.");

         if (rData.BuffSlot()->IsDirectSet())
         {
            pCalcVal = reinterpret_cast<RwUInt16 *>(rData.BuffSlot()->GetDirectDataSlot());

            if (pCalcVal)
            {
               *pCalcVal = CalcValue(pStartVal, pEndVal, delta);
               rData.BuffSlot()->DirectUpdate(pCalcVal);
            }
            else
            {
               RwUInt16 calcVal = CalcValue(pStartVal, pEndVal, delta);
               rData.BuffSlot()->DirectUpdate(&calcVal);
            }
         }
         else
         {
            pCalcVal = reinterpret_cast<RwUInt16 *>(rData.BuffSlot()->GetBuffDataSlot(sizeof(RwUInt16)));
            *pCalcVal = CalcValue(pStartVal, pEndVal, delta);
         }

         RWS_RETURNVOID();
      }
   }  // namespace SEQUENCE
}     // namespace RWS
