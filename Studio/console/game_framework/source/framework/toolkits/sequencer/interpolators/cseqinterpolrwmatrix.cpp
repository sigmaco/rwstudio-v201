/****************************************************************************
*
* File :      CSeqInterpolRwMatrix.cpp
*
* Abstract :  Frame by frame sequence controller for RwMatrix data types.
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
#if (!defined (SKY) && !defined (DOLPHIN))
#include <memory.h>
#endif

#if defined(DOLPHIN) && defined(__MWERKS__)
   using namespace std;
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include <rtquat.h>

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "cseqinterpolrwmatrix.h"
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

      CSeqInterpolRegister gRwMatrixDefaultReg(CSeqInterpolBase::MakeNew,
         CSeqInterpolBase::MaxSize, RWSTRING("RwMatrix"), 0, FALSE);

      // Interpolator specific ones...

      CSeqInterpolRegister gRwMatrixLinearInterpolReg(CSeqInterpolLinearRwMatrix::MakeNew,
         CSeqInterpolLinearRwMatrix::MaxSize, RWSTRING("RwMatrix"),
         INTERPOL_TYPE_LINEAR_NAME, FALSE);

      ////////////////////////////////////////
      //
      // CSeqInterpolLinearRwMatrix functions.
      //
      //

      /**
      *
      * Constructor - sets up attribute packet buffer, with space for data change (RwMatrix)
      *               and links to default set/prep/step functions.
      *
      */

      CSeqInterpolLinearRwMatrix::CSeqInterpolLinearRwMatrix(CSeqItem &rData) :
         CSeqInterpolBase()
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwMatrix::CSeqInterpolLinearRwMatrix");
         RWS_RETURNVOID();
      }

      /**
      *
      * Destructor - frees up memory used, etc...
      *
      */

      CSeqInterpolLinearRwMatrix::~CSeqInterpolLinearRwMatrix(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwMatrix::~CSeqInterpolLinearRwMatrix");
         RWS_RETURNVOID();
      }

      /**
      *
      * Allocates a new instance of the CSeqInterpolLinearRwMatrix class and returns it via
      * a base class pointer. Asserts if fails to allocated.
      *
      */

      CSeqInterpolBase *CSeqInterpolLinearRwMatrix::MakeNew(CSeqItem &rData)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwMatrix::MakeNew");
         RWS_ASSERT(rData.GetInterpolMem(), "Memory buffer is invalid");
         RWS_ASSERT(rData.GetInterpolMemSize() >= sizeof(CSeqInterpolLinearRwMatrix), 
            "Memory buffer too small");

         CSeqInterpolLinearRwMatrix *pNew = new(rData.GetInterpolMem())
            CSeqInterpolLinearRwMatrix(rData);
         RWS_ASSERT(pNew, "Could not allocate new object.");

         RWS_RETURN(pNew);
      }

      /**
      *
      * Returns the maximum size of memory need to hold this interpolator.
      *
      */

      RwUInt32 CSeqInterpolLinearRwMatrix::MaxSize(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwMatrix::MaxSize");
         RWS_RETURN(sizeof(CSeqInterpolLinearRwMatrix));
      }

      /**
      *
      * Sets the attribute packet data and sets to appropriate attribute handler. Uses the
      * 'end' key of the passed in CSeqItem.
      *
      */

      void CSeqInterpolLinearRwMatrix::Set(CSeqItem &rData)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwMatrix::SetInterpol");

         // Call base class to do this.

         CSeqInterpolBase::Set(rData);

         RWS_RETURNVOID();
      }

      /**
      *
      * Prepares to interpolates between last and end keys in the passed in CSeqItem.
      *
      */

      void CSeqInterpolLinearRwMatrix::Prep(CSeqItem &rData)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwMatrix::Prep");

         // Calculate everything...

         CalcValues(rData, TRUE);

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

      void CSeqInterpolLinearRwMatrix::PrepReuse(CSeqItem &rData)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwMatrix::PrepReuse");

         // Copy over end values into start values.

         m_startQuat = m_endQuat;

         // Calculate everything EXCEPT this copied value.

         CalcValues(rData, FALSE);

         RWS_RETURNVOID();
      }

      /**
      *
      * Calculates the intermediate value for the RwMatrix given the 'now' time and the
      * data held in the CSeqItem passed in. ASSUMES that the 'Prep' has been
      * called before the first call to this function. Calls the 
      * 'Step(CSeqItem &rData, RwReal delta)' to do all the work.
      *
      */

      void CSeqInterpolLinearRwMatrix::Step(CSeqItem &rData, RwUInt32 nowTime)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwMatrix::Step (time)");

         RwReal delta = static_cast<RwReal>(nowTime - rData.StartTime()) *
            m_oneOverSeqTime;

         Step(rData, delta);

         RWS_RETURNVOID();
      }

      /**
      *
      * Calculates the intermediate value for the RwMatrix given a delta and the
      * data held in the CSeqItem passed in. ASSUMES that the 'Prep' has been
      * called before the first call to this function. ASSUMES delta value is in the
      * range of 0.0 to 1.0 ONLY.
      *
      * NOTE: If writing to a 'Direct-Set' pointer, ie a variable in a class instance, this
      *       code ASSUMES that the matrix at that location is correctly aligned and can be
      *       safely used with the RwMatrix... functions. This provides a speed up my removing
      *       the need for a temporary 'output matrix' variable and the copy operations
      *       involved as a result.
      *
      */

      void CSeqInterpolLinearRwMatrix::Step(CSeqItem &rData, RwReal delta)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwMatrix::Step (delta)");
         RWS_ASSERT(rData.LastKey()->DataSize(), "Last key's data size invalid.");
         RWS_ASSERT(rData.LastKey()->Data(), "Last key's data pointer invalid.");
         RWS_ASSERT(delta >= 0.0f && delta <= 1.0f, "Delta out of range.");

         RwV3d vect,
            *pPos;
         RtQuat curQuat;

         // Setup a matrix with appropriate scale & position.

         vect.x = m_xStartScale + m_xScaleStep * delta;
         vect.y = m_yStartScale + m_yScaleStep * delta;
         vect.z = m_zStartScale + m_zScaleStep * delta;
         RwMatrixScale(&m_posScaleMatrix, &vect, rwCOMBINEREPLACE);

         pPos = RwMatrixGetPos(&m_posScaleMatrix);
         pPos->x = m_xStartPos + m_xPosStep * delta;
         pPos->y = m_yStartPos + m_yPosStep * delta;
         pPos->z = m_zStartPos + m_zPosStep * delta;

         RwMatrixUpdate(&m_posScaleMatrix);

         // Calculate the current value & insert into attribute data, do rotation first...

         RtQuatSlerp(&curQuat, &m_startQuat, &m_endQuat, delta, &m_slerpCache);
         RtQuatConvertToMatrix(&curQuat, &m_rotMatrix);

         // Calculate the current value & insert into attribute data...

         RWS_ASSERT(rData.BuffSlot(), "no slot to set data with.");

         if (rData.BuffSlot()->IsDirectSet())
         {
            RwMatrix *pWriteMatrix = reinterpret_cast<RwMatrix *>(rData.BuffSlot()->GetDirectDataSlot());

            if (pWriteMatrix)
            {
               // ASSUMES pWriteMatrix is correctly aligned.

               RwMatrixMultiply(pWriteMatrix, &m_rotMatrix, &m_posScaleMatrix);
               rData.BuffSlot()->DirectUpdate(pWriteMatrix);
            }
            else
            {
               // Setup dummy and pass to direct-set update function.

               RwMatrixMultiply(&m_outMatrix, &m_rotMatrix, &m_posScaleMatrix);
               rData.BuffSlot()->DirectUpdate(&m_outMatrix);
            }
         }
         else
         {
            // Setup dummy and copy into UNALIGNED buffer...

            void *pWrite = rData.BuffSlot()->GetBuffDataSlot(sizeof(RwMatrix));

            RwMatrixMultiply(&m_outMatrix, &m_rotMatrix, &m_posScaleMatrix);
            memcpy(pWrite, &m_outMatrix, sizeof(RwMatrix));
         }
         
         RWS_RETURNVOID();
      }

      /**
      *
      *
      *
      */

      void CSeqInterpolLinearRwMatrix::CalcValues(CSeqItem &rData, RwBool CalcStartQuat)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolLinearRwMatrix::CalcValues");
         RWS_ASSERT(rData.LengthTime(), "Invalid length time (zero)");
         RWS_ASSERT(rData.LastKey()->DataSize(), "Last key's data size invalid.");
         RWS_ASSERT(rData.LastKey()->Data(), "Last key's data pointer invalid.");
         RWS_ASSERT(rData.EndKey()->DataSize(), "End key's data size invalid.");
         RWS_ASSERT(rData.EndKey()->Data(), "End key's data pointer invalid.");

         RwMatrix *pEndVal = reinterpret_cast<RwMatrix *>(const_cast<void *>(rData.EndKey()->Data())),
            *pStartVal = reinterpret_cast<RwMatrix *>(const_cast<void *>(rData.LastKey()->Data()));
         RwV3d *pStartVect,
            *pEndVect,
            vect;
         RwReal endXScale,
            endYScale,
            endZScale;

         // Setup data...

         m_oneOverSeqTime = 1.0f / static_cast<RwReal>(rData.LengthTime());

         // Calculate the positional changes...

         pStartVect = RwMatrixGetPos(pStartVal);
         pEndVect = RwMatrixGetPos(pEndVal);

         m_xPosStep = pEndVect->x - pStartVect->x;
         m_yPosStep = pEndVect->y - pStartVect->y;
         m_zPosStep = pEndVect->z - pStartVect->z;

         m_xStartPos = pStartVect->x;
         m_yStartPos = pStartVect->y;
         m_zStartPos = pStartVect->z;

         // Get scale changes.

         m_xStartScale = RwV3dLength(RwMatrixGetRight(pStartVal));
         m_yStartScale = RwV3dLength(RwMatrixGetUp(pStartVal));
         m_zStartScale = RwV3dLength(RwMatrixGetAt(pStartVal));

         endXScale = RwV3dLength(RwMatrixGetRight(pEndVal));
         endYScale = RwV3dLength(RwMatrixGetUp(pEndVal));
         endZScale = RwV3dLength(RwMatrixGetAt(pEndVal));

         m_xScaleStep = endXScale - m_xStartScale;
         m_yScaleStep = endYScale - m_yStartScale;
         m_zScaleStep = endZScale - m_zStartScale;

         // Remove scale from matrices & convert to quaternions - start matrix first.
         // Don't need to re-copy from keyframe, just copy previous data (as will
         // modify copy).

         if (CalcStartQuat)
         {
            *RwMatrixGetPos(&m_rotMatrix) = *RwMatrixGetPos(pStartVal);
            *RwMatrixGetUp(&m_rotMatrix) = *RwMatrixGetUp(pStartVal);
            *RwMatrixGetAt(&m_rotMatrix) = *RwMatrixGetAt(pStartVal);
            *RwMatrixGetRight(&m_rotMatrix) = *RwMatrixGetRight(pStartVal);

            RwMatrixUpdate(&m_rotMatrix);

            vect.x = 1.0f / m_xStartScale;
            vect.y = 1.0f / m_yStartScale;
            vect.z = 1.0f / m_zStartScale;

            RwMatrixScale(&m_rotMatrix, &vect, rwCOMBINEPRECONCAT);
            RtQuatConvertFromMatrix(&m_startQuat, &m_rotMatrix);
         }

         // Now end matrix. Again, have to copy data from keyframe as not aligned.

         *RwMatrixGetPos(&m_rotMatrix) = *RwMatrixGetPos(pEndVal);
         *RwMatrixGetUp(&m_rotMatrix) = *RwMatrixGetUp(pEndVal);
         *RwMatrixGetAt(&m_rotMatrix) = *RwMatrixGetAt(pEndVal);
         *RwMatrixGetRight(&m_rotMatrix) = *RwMatrixGetRight(pEndVal);

         RwMatrixUpdate(&m_rotMatrix);

         vect.x = 1.0f / endXScale;
         vect.y = 1.0f / endYScale;
         vect.z = 1.0f / endZScale;

         RwMatrixScale(&m_rotMatrix, &vect, rwCOMBINEPRECONCAT);
         RtQuatConvertFromMatrix(&m_endQuat, &m_rotMatrix);

         // Setup slerp between these two...

         RtQuatSetupSlerpCache(&m_startQuat, &m_endQuat, &m_slerpCache);

         RWS_RETURNVOID();
      }
   }  // namespace SEQUENCE
}     // namespace RWS
