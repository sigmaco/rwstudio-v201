/****************************************************************************
*
* File :      CSeqInterpolRwUInt8.h
*
* Abstract :  Frame by frame sequence controller for RwUInt8 values.
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

#ifndef CSEQINTERPOLRWUINT8_HEADER
#define CSEQINTERPOLRWUINT8_HEADER

#include "../cseqstack.h"

namespace RWS
{
   namespace SEQUENCE
   {
      class CSeqInterpolLinearRwUInt8 : public CSeqInterpolBase
      {
      public:

         CSeqInterpolLinearRwUInt8(CSeqItem &rData);
         virtual ~CSeqInterpolLinearRwUInt8(void);

         virtual void Set(CSeqItem &rData);
         virtual void Prep(CSeqItem &rData);
         virtual void PrepReuse(CSeqItem &rData);
         virtual void Step(CSeqItem &rData, RwUInt32 nowTime);
         virtual void Step(CSeqItem &rData, RwReal delta);

         static CSeqInterpolBase *MakeNew(CSeqItem &rData);
         static RwUInt32 MaxSize(void);

      private:

         RwUInt32 m_scaledStep;        /**< Since only counting 8 bit value, the 24 bits of decimal
                                            should be okay for all cases. */
      };
   }     // namespace SEQUENCE
}        // namespace RWS
#endif   // #ifndef CSEQINTERPOLRWUINT8_HEADER
