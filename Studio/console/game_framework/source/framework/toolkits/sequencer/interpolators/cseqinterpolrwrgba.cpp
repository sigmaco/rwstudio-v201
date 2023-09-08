/****************************************************************************
*
* File :      CSeqInterpolRwRGBA.cpp
*
* Abstract :  Frame by frame sequence controller for RwRGBA data types.
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
#include "cseqinterpolrwrgba.h"
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

      CSeqInterpolRegister gRwRGBADefaultReg(CSeqInterpolBase::MakeNew,
         CSeqInterpolBase::MaxSize, RWSTRING("RwRGBA"), 0, FALSE);

      // Interpolator specific ones...

      CSeqInterpolRegister gRwRGBAInterpolReg(CSeqInterpolLinearRwRGBA::MakeNew,
         CSeqInterpolLinearRwRGBA::MaxSize, RWSTRING("RwUInt32"),
         INTERPOL_TYPE_LINEAR_NAME, FALSE);

      //////////////////////////////////////
      //
      // CSeqInterpolLinearRwRGBA functions.
      //
      //

      /**
      *
      * Constructor - sets up attribute packet buffer, with space for data change (RwRGBA)
      *               and links to default set/prep/step functions.
      *
      */

      CSeqInterpolLinearRwRGBA::CSeqInterpolLinearRwRGBA(CSeqItem &rData) :
         CSeqInterpolBase()
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwRGBA::CSeqInterpolLinearRwRGBA");
         RWS_RETURNVOID();
      }

      /**
      *
      * Destructor - frees up memory used, etc...
      *
      */

      CSeqInterpolLinearRwRGBA::~CSeqInterpolLinearRwRGBA(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwRGBA::~CSeqInterpolLinearRwRGBA");
         RWS_RETURNVOID();
      }

      /**
      *
      * Allocates a new instance of the CSeqInterpolLinearRwRGBA class and returns it via
      * a base class pointer. Asserts if fails to allocated.
      *
      */

      CSeqInterpolBase *CSeqInterpolLinearRwRGBA::MakeNew(CSeqItem &rData)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwRGBA::MakeNew");
         RWS_ASSERT(rData.GetInterpolMem(), "Memory buffer is invalid");
         RWS_ASSERT(rData.GetInterpolMemSize() >= sizeof(CSeqInterpolLinearRwRGBA),
            "Memory buffer too small");

         CSeqInterpolLinearRwRGBA *pNew = new(rData.GetInterpolMem())
            CSeqInterpolLinearRwRGBA(rData);
         RWS_ASSERT(pNew, "Could not allocate new instance.");

         RWS_RETURN(pNew);
      }

      /**
      *
      * Returns the maximum size of memory need to hold this interpolator.
      *
      */

      RwUInt32 CSeqInterpolLinearRwRGBA::MaxSize(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwRGBA::MaxSize");
         RWS_RETURN(sizeof(CSeqInterpolLinearRwRGBA));
      }

      /**
      *
      * Sets the attribute packet data and sets to appropriate attribute handler. Uses the
      * 'end' key of the passed in CSeqItem.
      *
      */

      void CSeqInterpolLinearRwRGBA::Set(CSeqItem &rData)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwRGBA::SetInterpol");

         // Just call base object to do this...

         CSeqInterpolBase::Set(rData);

         RWS_RETURNVOID();
      }

      /**
      *
      * Prepares to interpolates between last and end keys in the passed in CSeqItem.
      *
      */

      void CSeqInterpolLinearRwRGBA::Prep(CSeqItem &rData)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwRGBA::Prep");
         RWS_ASSERT(rData.LengthTime(), "Invalid length time (zero)");
         RWS_ASSERT(rData.LastKey()->DataSize(), "Last key's data size invalid.");
         RWS_ASSERT(rData.LastKey()->Data(), "Last key's data pointer invalid.");
         RWS_ASSERT(rData.EndKey()->DataSize(), "End key's data size invalid.");
         RWS_ASSERT(rData.EndKey()->Data(), "End key's data pointer invalid.");

         const RwRGBA *pEndVal = reinterpret_cast<const RwRGBA *>
               (rData.EndKey()->Data()),
            *pStartVal = reinterpret_cast<const RwRGBA *>
               (rData.LastKey()->Data());

         // Store start value for each individual channel of the color (using fixed 24 bit decimal).

         m_data.m_startR = static_cast<RwUInt32>(pStartVal->red) << 24;
         m_data.m_startG = static_cast<RwUInt32>(pStartVal->green) << 24;
         m_data.m_startB = static_cast<RwUInt32>(pStartVal->blue) << 24;
         m_data.m_startA = static_cast<RwUInt32>(pStartVal->alpha) << 24;

         // Is red channel going up ?

         if (pEndVal->red >= pStartVal->red)
         {
            m_data.m_stepR = ((static_cast<RwUInt32>(pEndVal->red) << 24) - 
               m_data.m_startR) / rData.LengthTime();
            m_data.m_incR = TRUE;
         }
         else
         {
            m_data.m_stepR = (m_data.m_startR - (static_cast<RwUInt32>(pEndVal->red) << 24)) / 
               rData.LengthTime();
            m_data.m_incR = FALSE;
         }

         // Is green channel going up ?

         if (pEndVal->green >= pStartVal->green)
         {
            m_data.m_stepG = ((static_cast<RwUInt32>(pEndVal->green) << 24) - m_data.m_startG) / 
               rData.LengthTime();
            m_data.m_incG = TRUE;
         }
         else
         {
            m_data.m_stepG = (m_data.m_startG - (static_cast<RwUInt32>(pEndVal->green) << 24)) / 
               rData.LengthTime();
            m_data.m_incG = FALSE;
         }

         // Is blue channel going up ?

         if (pEndVal->blue >= pStartVal->blue)
         {
            m_data.m_stepB = ((static_cast<RwUInt32>(pEndVal->blue) << 24) - m_data.m_startB) / 
               rData.LengthTime();
            m_data.m_incB = TRUE;
         }
         else
         {
            m_data.m_stepB = (m_data.m_startB - (static_cast<RwUInt32>(pEndVal->blue) << 24)) / 
               rData.LengthTime();
            m_data.m_incB = FALSE;
         }

         // Is alpha channel going up ?

         if (pEndVal->alpha >= pStartVal->alpha)
         {
            m_data.m_stepA = ((static_cast<RwUInt32>(pEndVal->alpha) << 24) - m_data.m_startA) / 
               rData.LengthTime();
            m_data.m_incA = TRUE;
         }
         else
         {
            m_data.m_stepA = (m_data.m_startA - (static_cast<RwUInt32>(pEndVal->alpha) << 24)) / 
               rData.LengthTime();
            m_data.m_incA = FALSE;
         }

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

      void CSeqInterpolLinearRwRGBA::PrepReuse(CSeqItem &rData)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwRGBA::PrepReuse");

         // No special case, so call standard one.

         Prep(rData);

         RWS_RETURNVOID();
      }

      /**
      *
      * Calculates the value for the data slot used in 'Step'. Moved out for maintainability.
      *
      */

      inline void CalcValue(RwRGBA *pResult, CSeqInterpolLinearRwRGBA::Data &rData,
         RwUInt32 mSecsPassed)
      {
         // Calc red...

         if (rData.m_incR)
         {
            pResult->red = static_cast<RwUInt8>((rData.m_startR + rData.m_stepR * 
               mSecsPassed) >> 24);
         }
         else
         {
            pResult->red = static_cast<RwUInt8>((rData.m_startR - rData.m_stepR * 
               mSecsPassed) >> 24);
         }

         // Calc green...

         if (rData.m_incG)
         {
            pResult->green = static_cast<RwUInt8>((rData.m_startG + rData.m_stepG * 
               mSecsPassed) >> 24);
         }
         else
         {
            pResult->green = static_cast<RwUInt8>((rData.m_startG - rData.m_stepG * 
               mSecsPassed) >> 24);
         }

         // Calc blue...

         if (rData.m_incB)
         {
            pResult->blue = static_cast<RwUInt8>((rData.m_startB + rData.m_stepB * 
               mSecsPassed) >> 24);
         }
         else
         {
            pResult->blue = static_cast<RwUInt8>((rData.m_startB - rData.m_stepB * 
               mSecsPassed) >> 24);
         }

         // Calc alpha...

         if (rData.m_incA)
         {
            pResult->alpha = static_cast<RwUInt8>((rData.m_startA + rData.m_stepA * 
               mSecsPassed) >> 24);
         }
         else
         {
            pResult->alpha = static_cast<RwUInt8>((rData.m_startA - rData.m_stepA * 
               mSecsPassed) >> 24);
         }
      }

      /**
      *
      * Calculates the intermediate value for the RwRGBA given the 'now' time and the
      * data held in the CSeqItem passed in. ASSUMES that the 'Prep' has been
      * called before the first call to this function.
      *
      */

      void CSeqInterpolLinearRwRGBA::Step(CSeqItem &rData, RwUInt32 nowTime)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwRGBA::Step");

         RwRGBA *pCalcVal;
         RwUInt32 mSecsPassed = nowTime - rData.StartTime();

         // Calculate the current value & insert into attribute data...

         RWS_ASSERT(rData.BuffSlot(), "no slot to set data with.");

         if (rData.BuffSlot()->IsDirectSet())
         {
            pCalcVal = reinterpret_cast<RwRGBA *>(rData.BuffSlot()->GetDirectDataSlot());

            if (pCalcVal)
            {
               CalcValue(pCalcVal, m_data, mSecsPassed);
               rData.BuffSlot()->DirectUpdate(pCalcVal);
            }
            else
            {
               RwRGBA calcVal;

               CalcValue(&calcVal, m_data, mSecsPassed);
               rData.BuffSlot()->DirectUpdate(&calcVal);
            }
         }
         else
         {
            pCalcVal = reinterpret_cast<RwRGBA *>(rData.BuffSlot()->GetBuffDataSlot(sizeof(RwRGBA)));
            CalcValue(pCalcVal, m_data, mSecsPassed);
         }

         RWS_RETURNVOID();
      }

      /**
      *
      * Calculates an colour element value, between two passed in values at a given
      * delta. 0 = all start, 1 = all end. Values inbetween are blended. The delta
      * can ONLY be between 0.0 and 1.0.
      *
      * NOTE: function is written for accuracy and NOT speed.
      *
      * \return The calculated value at the given delta.
      *
      */

      inline RwUInt8 CalcColElement(RwUInt8 startVal, RwUInt8 endVal, RwReal delta)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CalcValues (RGBA, delta)");
         RWS_ASSERT(delta >= 0.0f && delta <= 1.0f, "Delta is out of range.");

         RwReal realStartVal = static_cast<RwReal>(startVal),
            realEndVal = static_cast<RwReal>(endVal);
         RwUInt8 result = static_cast<RwUInt8>(realStartVal +
            (realEndVal - realStartVal) * delta);

         RWS_RETURN(result);
      }

      /**
      *
      * Calculates the value for the data slot used in 'Step'. Moved out for maintainability.
      * This version uses a delta value, which is ASSUMED to be between 0.0 and 1.0 ONLY.
      *
      * NOTE: This function is NOT written for speed. It should only be used for dynamic
      *       interpolation in design builds (and such like) ONLY.
      *
      */

      inline void CalcValue(RwRGBA *pResult, CSeqItem &rData, RwReal delta)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CalcValues (RGBA, delta)");
         RWS_ASSERT(rData.LastKey()->DataSize(), "Last key's data size invalid.");
         RWS_ASSERT(rData.LastKey()->Data(), "Last key's data pointer invalid.");
         RWS_ASSERT(rData.EndKey()->DataSize(), "End key's data size invalid.");
         RWS_ASSERT(rData.EndKey()->Data(), "End key's data pointer invalid.");
         RWS_ASSERT(delta >= 0.0f && delta <= 1.0f, "Delta is out of range.");

         const RwRGBA *pEndVal = reinterpret_cast<const RwRGBA *>
               (rData.EndKey()->Data()),
            *pStartVal = reinterpret_cast<const RwRGBA *>
               (rData.LastKey()->Data());

         pResult->red = CalcColElement(pStartVal->red, pEndVal->red, delta);
         pResult->green = CalcColElement(pStartVal->green, pEndVal->green, delta);
         pResult->blue = CalcColElement(pStartVal->blue, pEndVal->blue, delta);
         pResult->alpha = CalcColElement(pStartVal->alpha, pEndVal->alpha, delta);

         RWS_RETURNVOID();
      }

      /**
      *
      * Calculates the intermediate value for the RwRGBA given a delta and the
      * data held in the CSeqItem passed in.
      *
      */

      void CSeqInterpolLinearRwRGBA::Step(CSeqItem &rData, RwReal delta)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwRGBA::Step");

         RwRGBA *pCalcVal;

         // Calculate the current value & insert into attribute data...

         RWS_ASSERT(rData.BuffSlot(), "no slot to set data with.");

         if (rData.BuffSlot()->IsDirectSet())
         {
            pCalcVal = reinterpret_cast<RwRGBA *>(rData.BuffSlot()->GetDirectDataSlot());

            if (pCalcVal)
            {
               CalcValue(pCalcVal, rData, delta);
               rData.BuffSlot()->DirectUpdate(pCalcVal);
            }
            else
            {
               RwRGBA calcVal;

               CalcValue(&calcVal, rData, delta);
               rData.BuffSlot()->DirectUpdate(&calcVal);
            }
         }
         else
         {
            pCalcVal = reinterpret_cast<RwRGBA *>(rData.BuffSlot()->GetBuffDataSlot(sizeof(RwRGBA)));
            CalcValue(pCalcVal, rData, delta);
         }

         RWS_RETURNVOID();
      }
   }  // namespace SEQUENCE
}     // namespace RWS
