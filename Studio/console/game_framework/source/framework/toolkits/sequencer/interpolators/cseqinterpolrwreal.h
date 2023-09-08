/****************************************************************************
*
* File :      CSeqInterpolRwReal.h
*
* Abstract :  Frame by frame sequence controller for RwReal values.
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

#ifndef CSEQINTERPOLRWREAL_HEADER
#define CSEQINTERPOLRWREAL_HEADER

#include "../cseqstack.h"

namespace RWS
{
   namespace SEQUENCE
   {
      class CSeqInterpolLinearRwReal : public CSeqInterpolBase
      {
      public:

         CSeqInterpolLinearRwReal(CSeqItem &rData);
         virtual ~CSeqInterpolLinearRwReal(void);

         virtual void Set(CSeqItem &rData);
         virtual void Prep(CSeqItem &rData);
         virtual void PrepReuse(CSeqItem &rData);
         virtual void Step(CSeqItem &rData, RwUInt32 nowTime);
         virtual void Step(CSeqItem &rData, RwReal delta);

         static CSeqInterpolBase *MakeNew(CSeqItem &rData);
         static RwUInt32 MaxSize(void);

      private:

         RwReal m_incPerMilliSec;
      };
   }     // namespace SEQUENCE
}        // namespace RWS
#endif   // #ifndef CSEQINTERPOLRWREAL_HEADER
