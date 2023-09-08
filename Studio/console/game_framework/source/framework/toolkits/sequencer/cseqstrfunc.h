/*****************************************************************************
*
* File :      CSeqStrFunc.h
*
* Abstract :  Class to handle processing of the dynamic, real-time, sequence
*             data from the workspace.
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

#ifndef C_SEQ_STR_FUNC_HEADER
#define C_SEQ_STR_FUNC_HEADER

#include "cseqprocessor.h"

namespace RWS
{
   namespace SEQUENCE
   {

#ifdef RWS_DESIGN

      class CSeqStrFunc
      {
      public:

         static void Register(void);
         static void UnRegister(void);

      private:

         static CSeqProcessor *sm_pProcessor;

         static void HandleDynamicSeq(RwChunkHeaderInfo &rChunkHeader,
            RwStream *pStream);

         CSeqStrFunc(void) {}          /**< Stop direct construction. */
         ~CSeqStrFunc(void) {}         /**< Stop direct destruction. */
      };

#endif

   }     // SEQUENCE
}        // RWS
#endif   // #ifndef C_SEQ_STR_FUNC_HEADER
