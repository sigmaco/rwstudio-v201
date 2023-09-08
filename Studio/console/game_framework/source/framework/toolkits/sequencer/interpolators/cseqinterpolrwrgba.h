/****************************************************************************
*
* File :      CSeqInterpolRwRGBA.h
*
* Abstract :  Frame by frame sequence controller for RwRGBA values.
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

#ifndef CSEQINTERPOLRWRGBA_HEADER
#define CSEQINTERPOLRWRGBA_HEADER

#include "../cseqstack.h"

namespace RWS
{
   namespace SEQUENCE
   {
      class CSeqInterpolLinearRwRGBA : public CSeqInterpolBase
      {
      public:

         CSeqInterpolLinearRwRGBA(CSeqItem &rData);
         virtual ~CSeqInterpolLinearRwRGBA(void);

         virtual void Set(CSeqItem &rData);
         virtual void Prep(CSeqItem &rData);
         virtual void PrepReuse(CSeqItem &rData);
         virtual void Step(CSeqItem &rData, RwUInt32 nowTime);
         virtual void Step(CSeqItem &rData, RwReal delta);

         static CSeqInterpolBase *MakeNew(CSeqItem &rData);
         static RwUInt32 MaxSize(void);

         struct Data
         {
            RwUInt32 m_startR,
               m_startG,
               m_startB,
               m_startA,
               m_stepR,
               m_stepG,
               m_stepB,
               m_stepA;
            RwBool m_incR,
               m_incG,
               m_incB,
               m_incA;
         };

      private:

         Data m_data;         
      };
   }     // namespace SEQUENCE
}        // namespace RWS
#endif   // #ifndef CSEQINTERPOLRWRGBA_HEADER
