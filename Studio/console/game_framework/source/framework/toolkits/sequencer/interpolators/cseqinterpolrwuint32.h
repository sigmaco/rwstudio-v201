/****************************************************************************
*
* File :      CSeqInterpolRwUInt32.h
*
* Abstract :  Frame by frame sequence controller for RwUInt32 values.
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

#ifndef CSEQINTERPOLRWUINT32_HEADER
#define CSEQINTERPOLRWUINT32_HEADER

#include "../cseqstack.h"

namespace RWS
{
   namespace SEQUENCE
   {
      class CSeqInterpolLinearRwUInt32 : public CSeqInterpolBase
      {
      public:

         CSeqInterpolLinearRwUInt32(CSeqItem &rData);
         virtual ~CSeqInterpolLinearRwUInt32(void);

         virtual void Set(CSeqItem &rData);
         virtual void Prep(CSeqItem &rData);
         virtual void PrepReuse(CSeqItem &rData);
         virtual void Step(CSeqItem &rData, RwUInt32 nowTime);
         virtual void Step(CSeqItem &rData, RwReal delta);

         static CSeqInterpolBase *MakeNew(CSeqItem &rData);
         static RwUInt32 MaxSize(void);

      private:

         RwReal m_oneOverSeqTime;
      };
   }     // namespace SEQUENCE
}        // namespace RWS
#endif   // #ifndef CSEQINTERPOLRWUINT32_HEADER
