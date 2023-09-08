/****************************************************************************
*
* File :      CSeqInterpolRwMatrix.h
*
* Abstract :  Frame by frame sequence controller for RwMatrix values.
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

#ifndef CSEQINTERPOLRWMATRIX_HEADER
#define CSEQINTERPOLRWMATRIX_HEADER

#include "rtquat.h"
#include "rtslerp.h"

#include "../cseqstack.h"

namespace RWS
{
   namespace SEQUENCE
   {
      ///////////////////////////////////////////////////////////
      //
      // Linear interpolator - individually interpolates x, y & z.
      //

      class CSeqInterpolLinearRwMatrix : public CSeqInterpolBase
      {
      public:

         CSeqInterpolLinearRwMatrix(CSeqItem &rData);
         virtual ~CSeqInterpolLinearRwMatrix(void);

         virtual void Set(CSeqItem &rData);
         virtual void Prep(CSeqItem &rData);
         virtual void PrepReuse(CSeqItem &rData);
         virtual void Step(CSeqItem &rData, RwUInt32 nowTime);
         virtual void Step(CSeqItem &rData, RwReal delta);

         static CSeqInterpolBase *MakeNew(CSeqItem &rData);
         static RwUInt32 MaxSize(void);

      private:
         
         void CalcValues(CSeqItem &rData, RwBool CalcStartQuat);

         RwReal m_xStartPos,
            m_yStartPos,
            m_zStartPos,
            m_xPosStep,
            m_yPosStep,
            m_zPosStep,
            m_xStartScale,
            m_yStartScale,
            m_zStartScale,
            m_xScaleStep,
            m_yScaleStep,
            m_zScaleStep,
            m_degTotal,
            m_oneOverSeqTime;
         RtQuat m_startQuat,
            m_endQuat;
         RtQuatSlerpCache m_slerpCache;
         RwMatrix m_posScaleMatrix,
            m_rotMatrix,
            m_outMatrix;                     /**< Used if no direct-set data variable pointer. */
      };
   }     // namespace SEQUENCE
}        // namespace RWS
#endif   // #ifndef CSEQINTERPOLRWREAL_HEADER
