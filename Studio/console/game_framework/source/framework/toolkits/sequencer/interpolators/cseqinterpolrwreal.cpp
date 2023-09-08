/****************************************************************************
*
* File :      CSeqInterpolRwReal.cpp
*
* Abstract :  Frame by frame sequence controller for RwReal data types.
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
#include "cseqinterpolrwreal.h"
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

      CSeqInterpolRegister gRwRealDefaultReg(CSeqInterpolBase::MakeNew,
         CSeqInterpolBase::MaxSize, RWSTRING("RwReal"), 0, FALSE);

      // Interpolator specific ones...

      CSeqInterpolRegister gRwRealInterpolReg(CSeqInterpolLinearRwReal::MakeNew,
         CSeqInterpolLinearRwReal::MaxSize, RWSTRING("RwReal"),
         INTERPOL_TYPE_LINEAR_NAME, FALSE);

      ////////////////////////////////////////
      //
      // CSeqInterpolLinearRwReal functions.
      //
      //

      /**
      *
      * Constructor - sets up attribute packet buffer, with space for data change (RwReal)
      *               and links to default set/prep/step functions.
      *
      */

      CSeqInterpolLinearRwReal::CSeqInterpolLinearRwReal(CSeqItem &rData) :
         CSeqInterpolBase()
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwReal::CSeqInterpolLinearRwReal");
         RWS_RETURNVOID();
      }

      /**
      *
      * Destructor - frees up memory used, etc...
      *
      */

      CSeqInterpolLinearRwReal::~CSeqInterpolLinearRwReal(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwReal::~CSeqInterpolLinearRwReal");

         RWS_RETURNVOID();
      }

      /**
      *
      * Allocates a new instance of the CSeqInterpolLinearRwReal class and returns it via
      * a base class pointer. Asserts if fails to allocated.
      *
      */

      CSeqInterpolBase *CSeqInterpolLinearRwReal::MakeNew(CSeqItem &rData)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwReal::MakeNew");
         RWS_ASSERT(rData.GetInterpolMem(), "Memory buffer is invalid");
         RWS_ASSERT(rData.GetInterpolMemSize() >= sizeof(CSeqInterpolLinearRwReal), 
            "Memory buffer too small");

         CSeqInterpolLinearRwReal *pNew = new(rData.GetInterpolMem())
            CSeqInterpolLinearRwReal(rData);
         RWS_ASSERT(pNew, "Could not allocate new object.");

         RWS_RETURN(pNew);
      }

      /**
      *
      * Returns the maximum size of memory need to hold this interpolator.
      *
      */

      RwUInt32 CSeqInterpolLinearRwReal::MaxSize(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwReal::MaxSize");
         RWS_RETURN(sizeof(CSeqInterpolLinearRwReal));
      }

      /**
      *
      * Sets the attribute packet data and sets to appropriate attribute handler. Uses the
      * 'end' key of the passed in CSeqItem.
      *
      */

      void CSeqInterpolLinearRwReal::Set(CSeqItem &rData)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwReal::SetInterpol");

         // Call base class to do this.

         CSeqInterpolBase::Set(rData);

         RWS_RETURNVOID();
      }

      /**
      *
      * Prepares to interpolates between last and end keys in the passed in CSeqItem.
      *
      */

      void CSeqInterpolLinearRwReal::Prep(CSeqItem &rData)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwReal::Prep");
         RWS_ASSERT(rData.LengthTime(), "Invalid length time (zero)");
         RWS_ASSERT(rData.LastKey()->DataSize(), "Last key's data size invalid.");
         RWS_ASSERT(rData.LastKey()->Data(), "Last key's data pointer invalid.");
         RWS_ASSERT(rData.EndKey()->DataSize(), "End key's data size invalid.");
         RWS_ASSERT(rData.EndKey()->Data(), "End key's data pointer invalid.");

         const RwReal *pEndVal = reinterpret_cast<const RwReal *>
               (rData.EndKey()->Data()),
            *pStartVal = reinterpret_cast<const RwReal *>
               (rData.LastKey()->Data());

         // Calculate the step value...

         m_incPerMilliSec = (*pEndVal - *pStartVal) / static_cast<RwReal>(rData.LengthTime());

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

      void CSeqInterpolLinearRwReal::PrepReuse(CSeqItem &rData)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwReal::PrepReuse");

         // No special case, so call standard one.

         Prep(rData);

         RWS_RETURNVOID();
      }

      /**
      *
      * Calculates the value placed into the buffer within the 'Step' function. Separated out
      * for easier maintenance.
      *
      */

      inline RwReal CalcValue(const RwReal *pStartVal, RwUInt32 nowTime,
         RwUInt32 startTime, RwReal incPerMilliSec)
      {
         return(*pStartVal + static_cast<RwReal>(nowTime - startTime) * incPerMilliSec);
      }

      /**
      *
      * Calculates the intermediate value for the RwReal given the 'now' time and the
      * data held in the CSeqItem passed in. ASSUMES that the 'Prep' has been
      * called before the first call to this function.
      *
      */

      void CSeqInterpolLinearRwReal::Step(CSeqItem &rData, RwUInt32 nowTime)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwReal::Step");
         RWS_ASSERT(rData.LastKey()->DataSize(), "Last key's data size invalid.");
         RWS_ASSERT(rData.LastKey()->Data(), "Last key's data pointer invalid.");

         const RwReal *pStartVal = reinterpret_cast<const RwReal *>
               (rData.LastKey()->Data());
         RwReal *pCalcVal;

         // Calculate the current value & insert into attribute data...

         RWS_ASSERT(rData.BuffSlot(), "no slot to set data with.");

         if (rData.BuffSlot()->IsDirectSet())
         {
            pCalcVal = reinterpret_cast<RwReal *>(rData.BuffSlot()->GetDirectDataSlot());

            if (pCalcVal)
            {
               *pCalcVal = CalcValue(pStartVal, nowTime, rData.StartTime(), m_incPerMilliSec);
               rData.BuffSlot()->DirectUpdate(pCalcVal);
            }
            else
            {
               RwReal calcVal = CalcValue(pStartVal, nowTime, rData.StartTime(), m_incPerMilliSec);
               rData.BuffSlot()->DirectUpdate(&calcVal);
            }
         }
         else
         {
            pCalcVal = reinterpret_cast<RwReal *>(rData.BuffSlot()->GetBuffDataSlot(sizeof(RwReal)));
            *pCalcVal = CalcValue(pStartVal, nowTime, rData.StartTime(), m_incPerMilliSec);
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

      inline RwReal CalcValue(const RwReal *pStartVal, const RwReal *pEndVal, RwReal delta)
      {
         return(*pStartVal + (*pEndVal - *pStartVal) * delta);
      }

      /**
      *
      * Calculates the intermediate value for the RwReal given a delta value and the
      * data held in the CSeqItem passed in. ASSUMES delta will be in the range of 0.0
      * to 1.0 ONLY.
      *
      */

      void CSeqInterpolLinearRwReal::Step(CSeqItem &rData, RwReal delta)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwReal::Step");
         RWS_ASSERT(rData.LastKey()->DataSize(), "Last key's data size invalid.");
         RWS_ASSERT(rData.LastKey()->Data(), "Last key's data pointer invalid.");
         RWS_ASSERT(rData.EndKey()->DataSize(), "End key's data size invalid.");
         RWS_ASSERT(rData.EndKey()->Data(), "End key's data pointer invalid.");
         RWS_ASSERT(delta >= 0.0f && delta <= 1.0f, "Delta out of range.");

         const RwReal *pStartVal = reinterpret_cast<const RwReal *>
               (rData.LastKey()->Data()),
            *pEndVal = reinterpret_cast<const RwReal *>(rData.EndKey()->Data());
         RwReal *pCalcVal;

         // Calculate the current value & insert into attribute data...

         RWS_ASSERT(rData.BuffSlot(), "no slot to set data with.");

         if (rData.BuffSlot()->IsDirectSet())
         {
            pCalcVal = reinterpret_cast<RwReal *>(rData.BuffSlot()->GetDirectDataSlot());

            if (pCalcVal)
            {
               *pCalcVal = CalcValue(pStartVal, pEndVal, delta);
               rData.BuffSlot()->DirectUpdate(pCalcVal);
            }
            else
            {
               RwReal calcVal = CalcValue(pStartVal, pEndVal, delta);
               rData.BuffSlot()->DirectUpdate(&calcVal);
            }
         }
         else
         {
            pCalcVal = reinterpret_cast<RwReal *>(rData.BuffSlot()->GetBuffDataSlot(sizeof(RwReal)));
            *pCalcVal = CalcValue(pStartVal, pEndVal, delta);
         }

         RWS_RETURNVOID();
      }
   }  // namespace SEQUENCE
}     // namespace RWS
