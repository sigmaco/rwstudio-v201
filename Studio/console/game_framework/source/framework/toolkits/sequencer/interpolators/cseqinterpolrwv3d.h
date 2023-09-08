/****************************************************************************
*
* File :      CSeqInterpolRwV3d.h
*
* Abstract :  Frame by frame sequence controller for RwV3d values.
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

#ifndef CSEQINTERPOLRWV3D_HEADER
#define CSEQINTERPOLRWV3D_HEADER

#include "../cseqstack.h"

namespace RWS
{
   namespace SEQUENCE
   {
      ///////////////////////////////////////////////////////////
      //
      // Linear interpolator - individually interpolates x, y & z.
      //

      class CSeqInterpolLinearRwV3d : public CSeqInterpolBase
      {
      public:

         CSeqInterpolLinearRwV3d(CSeqItem &rData);
         virtual ~CSeqInterpolLinearRwV3d(void);

         virtual void Set(CSeqItem &rData);
         virtual void Prep(CSeqItem &rData);
         virtual void PrepReuse(CSeqItem &rData);
         virtual void Step(CSeqItem &rData, RwUInt32 nowTime);
         virtual void Step(CSeqItem &rData, RwReal delta);

         static CSeqInterpolBase *MakeNew(CSeqItem &rData);
         static RwUInt32 MaxSize(void);

      private:

         void CalcAndStoreValue(CSeqItem &rData, RwReal delta);

         RwReal m_xInc,
            m_yInc,
            m_zInc,
            m_oneOverSeqTime;
      };

      ///////////////////////////////////////////////////////////
      //
      // Rotational interpolator - converts to angle & magnitude.
      //

      class CSeqInterpolRotationRwV3d : public CSeqInterpolBase
      {
      public:

         CSeqInterpolRotationRwV3d(CSeqItem &rData);
         virtual ~CSeqInterpolRotationRwV3d(void);

         virtual void Set(CSeqItem &rData);
         virtual void Prep(CSeqItem &rData);
         virtual void PrepReuse(CSeqItem &rData);
         virtual void Step(CSeqItem &rData, RwUInt32 nowTime);
         virtual void Step(CSeqItem &rData, RwReal delta);

         static CSeqInterpolBase *MakeNew(CSeqItem &rData);
         static RwUInt32 MaxSize(void);

      private:

         void CalcAndStoreValue(CSeqItem &rData, RwReal delta);

         RwReal m_startLength,      /**< Length of start vector. Valid if m_rotStep non-zero. */
            m_lengthInc,            /**< Amount length increases. Valid if m_rotStep non-zero. */
            m_oneOverSeqTime,
            m_degAngle;             /**< Total angular rotation needed. 0 = just set to start key. */
         RwV3d m_rotAxis;
      };
   }     // namespace SEQUENCE
}        // namespace RWS
#endif   // #ifndef CSEQINTERPOLRWREAL_HEADER
