/****************************************************************************
*
* File :      CSeqStrFunc.cpp
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

#include "precomp.h"

// Left to its own devices, the MW linker will dead strip the registrar ctors, so...

#ifdef __MWERKS__
#pragma force_active on
#pragma exceptions off     // Turn off exceptions due to bug in MW Dwarf
#endif                     // Generator for PS2, present in CW 3.04

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//

#include "cseqstrfunc.h"
#include "framework/core/streamhandler/streamhandler.h"
#include "framework/core/streamhandler/strfunc.h"
#include "framework/core/eventhandler/ceventhandler.h"

namespace RWS
{
   namespace SEQUENCE
   {

#ifdef RWS_DESIGN

      ///////////////////////////////////////////////////
      //
      // CSeqStrFunc - only compiled in on Design Builds.

      CSeqProcessor *CSeqStrFunc::sm_pProcessor = 0;

      /**
      *
      * Static function used to register the linkage to the string function needed
      * to allow the processing of 'dynamic sequences' from the Workspace.
      *
      */

      void CSeqStrFunc::Register(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqStrFunc::Register");

         // If not already already done, allocate the sequence processor.

         if (!sm_pProcessor)
         {
            sm_pProcessor = new CSeqProcessor;
            RWS_ASSERT(sm_pProcessor, "Failed to create global sequence processor.");

            // Link to data from workspace.

            CStreamHandler::RegisterChunkHandler(MAKECHUNKID (rwVENDORID_CRITERIONRM,
               strfunc_DynamicSequence), HandleDynamicSeq);
         }

         RWS_RETURNVOID();
      }

      /**
      *
      * Static function used to un-register the linkage to the string function needed
      * to allow the processing of 'dynamic sequences' from the Workspace. Also frees
      * up objects used in that processing.
      *
      */

      void CSeqStrFunc::UnRegister(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqStrFunc::UnRegister");

         // Close linkage.

         CStreamHandler::UnRegisterChunkHandler(MAKECHUNKID(rwVENDORID_CRITERIONRM,
            strfunc_DynamicSequence));

         // Free up the processor.

         delete sm_pProcessor;
         sm_pProcessor = 0;

         RWS_RETURNVOID();
      }

      /**
      *
      * Handles the incoming data block from the Workspace. Assumes the 'Register'
      * function has been called and the 'UnRegister' function has NOT been called.
      *
      */

      void CSeqStrFunc::HandleDynamicSeq(RwChunkHeaderInfo &rChunkHeader,
         RwStream *pStream)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqStrFunc::HandleDynamicSeq");
         RWS_ASSERT(sm_pProcessor, "No processor allocated - cannot process data.");

         CSeqCtrlData ctrlData(pStream, 0, &rChunkHeader);
         CSeqProcessor::StepState state;
         const char *pEventName;

         // Link this data to the processor and make sure it does NOT loop and
         // is not frozen.

         sm_pProcessor->Init(ctrlData);
         sm_pProcessor->Loop(FALSE);
         sm_pProcessor->Freeze(FALSE);

         // Process the data... Will do ALL data in one go. ASSUMES there are no
         // wait instruction in this stream - test cannot be handled correctly as
         // there is no real clock in this part of the code. Waiting for events is
         // ignored here too.

         sm_pProcessor->Start();

         do
         {
            // Do processing. In this case (only) because it's a dynamic sequence, need
            // to progress the clock so that the items on the active list will drop off
            // after being processed. This means the loop should only be run twice, once
            // to set the values and the second time to drop the 'active' dynamic interpolators
            // off the active list. NOTE: that if any events are waited for (skipped) or sent
            // then the loop will be run more than once anyway.

            sm_pProcessor->StepPrepare();
            state = sm_pProcessor->Step(pEventName);

            // Handle return state.

            switch (state)
            {
            case CSeqProcessor::SS_LOOPED:

               // Illegal, sequence told NOT to loop.

               RWS_ASSERTFAIL("Sequence looping in dynamic sequence - illegal.");
               break;

            case CSeqProcessor::SS_SEND_EVENT:
               {
                  CEventId eventID;

                  CEventHandler::RegisterMsg(eventID, pEventName, 0);
                  SendMsg(eventID);
                  CEventHandler::UnRegisterMsg(eventID);
               }
               break;

            case CSeqProcessor::SS_WAIT_ON_EVENT:

               // Clear event immediately.

               sm_pProcessor->ClearWaitEvent();
               break;

            case CSeqProcessor::SS_UP_TO_DATE:
            case CSeqProcessor::SS_KEEP_CALLING:
            case CSeqProcessor::SS_ENDED:

               // Ignore these here. Only needed due to 'catch unknown' default.

               break;

            default:

               RWS_ASSERTFAIL("Unknown sequence processing return state.");
               break;
            }
         }
         while (state != CSeqProcessor::SS_ENDED);

         // Free up memory used by processor.

         sm_pProcessor->FreeAll();

         RWS_RETURNVOID();
      }

#endif

   }  // SEQUENCE
}     // RWS
