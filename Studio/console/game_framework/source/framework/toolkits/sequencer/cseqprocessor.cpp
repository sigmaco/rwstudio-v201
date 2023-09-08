/****************************************************************************
*
* File :      CSeqProcessor.cpp
*
* Abstract :  Class to handle the vast majority of the sequence ctrl data
*             processing & calls to CSeqStack, etc.
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

#include "cseqprocessor.h"
#include <cstring>

namespace RWS
{
   namespace SEQUENCE
   {
      /**
      *
      * Initialises the data structures, etc. Does not prepare for playing, the
      * 'Init' function must be called to specify the sequence data to use.
      *
      * \param memType The memory type to use for allocations, default is the
      *                standard RWS memory pool (RWS_MEMORY_TYPE_RWS). This same
      *                memory type will be used by the sub-objects (sequence stack
      *                and sequence iterator).
      *
      */

      CSeqProcessor::CSeqProcessor(RwUInt32 memType, RwUInt32 delBuffSize,
            RwUInt32 flags) :
         m_isLooping(FALSE),
         m_isRunning(FALSE),
         m_isWaiting(FALSE),
         m_isWaitingForEvent(FALSE),
         m_isFrozen(FALSE),
         m_flags(flags),
         m_pSeqStack(0),
         m_pSeqIttr(0),
         m_waitEventName(0),
         m_memType(memType),
         m_curNumEntityDels(0),
         m_maxNumEntityDels(0),
         m_pEntityDelGuidOffsetList(0)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqProcessor::CSeqProcessor");

         ResizeDelBuffer(delBuffSize);

         RWS_RETURNVOID();
      }

      /**
      *
      * Releases memory used by internal structures, etc.
      *
      */

      CSeqProcessor::~CSeqProcessor(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqProcessor::~CSeqProcessor");

         FreeAll();

         RWS_RETURNVOID();
      }

      /**
      *
      * Call to specify the data block to be used. MUST be called before
      * calling the 'Start' function. If called for a second time, the
      * system will be cleared before the new data is used. If sequence
      * is running, it will be stopped.
      *
      * Allocates the iterator and the sequence stack sub objects. Memory allocated
      * during the playback of the sequence are freed by the 'Stop' function. The
      * sub-objects themselves are released by a call to 'FreeAll'.
      *
      * \param dataToUse Sequence control data to be used.
      * \param strBuffSize Size of the string table buffer to use, default = 0,
      *                    will be resized as needed. NOTE: this may cause
      *                    fragmentation of memory if a size is specified and
      *                    proves to be too small, resulting in a resize.
      * \param guidBuffSize Size of the GUID table buffer to use, default = 0,
      *                    will be resized as needed. NOTE: this may cause
      *                    fragmentation of memory if a size is specified and
      *                    proves to be too small, resulting in a resize.
      *
      */

      void CSeqProcessor::Init(const CSeqCtrlData &dataToUse, RwUInt32 strBuffSize,
         RwUInt32 guidBuffSize)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqProcessor::Init");

         // Stop sequence & free any memory used by old sequence...

         FreeAll();

         // Allocate new iterator & setup data for it to use.

         m_pSeqIttr = RWS_NEW_TYPE(m_memType) CSeqCtrlDataIttr(dataToUse,
            m_memType, strBuffSize, guidBuffSize, m_flags);
         RWS_ASSERT(m_pSeqIttr, "Failed to allocate new sequence iterator.");

         // Allocate the sequence stack...

         m_pSeqStack = RWS_NEW_TYPE(m_memType) CSeqStack(m_memType, m_flags);
         RWS_ASSERT(m_pSeqStack, "Failed to allocate new sequence stack.");

         // Clear flags, etc...

         m_isRunning = FALSE;
         m_isWaiting = FALSE;
         m_isWaitingForEvent = FALSE;
         m_isFrozen = FALSE;

         RWS_RETURNVOID();
      }

      /**
      *
      * Starts a sequence playing or restarts a sequence playing. The 'init' function MUST
      * have been called before the first call to this function (asserts if not). After
      * starting the 'step' function must be called to progress the sequence.
      *
      * NOTE: calling this function does NOT unfreeze the clock system. Call 'Freeze' with FALSE
      *       first if you wish to do this.
      *
      * NOTE: If the sequence is running when this function is called, it will be reset and
      *       restarted.
      *
      * \param pStartTime A pointer to a millisecond start time. If this pointer is 0, which
      *                   is the default, then the current time is used instead.
      *
      */

      void CSeqProcessor::Start(RwUInt32 *pStartTime)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqProcessor::Init");
         RWS_ASSERT(m_pSeqStack, "Sequence stack not allocated - need to call 'init'?");
         RWS_ASSERT(m_pSeqIttr, "Sequence iterator not allocated - need to call 'init'?");

         // Reset the sequence iterator to the start of the instructions.

         m_pSeqIttr->Reset();
         m_isWaiting = FALSE;
         m_isWaitingForEvent = FALSE;
         m_isRunning = TRUE;
         m_waitEventName = 0;

         // Restart the clock (if waiting was stalling it).

         TestEnableClock();

         // Start time specified ? MUST sync after enable call, as that may cause
         // a sync to the current time.

         if (pStartTime)
         {
            m_timeSpace.Sync(*pStartTime);
         }
         else
         {
            m_timeSpace.SyncToGlobalNow();
         }

         m_localNowTime = m_timeSpace.GetNow();

         RWS_RETURNVOID();
      }

      /**
      *
      * Called at the start of the sequence progression code. This function is called
      * once to update the time and then the 'Step' function is called one or more
      * times.
      *
      * NOTE: If 'GetWaitEventName' returns non-zero, there is no point in calling
      *       this or the 'Step' function as no progression will occur.
      *
      * \see GetWaitEventName
      * \see Step
      *
      */

      void CSeqProcessor::StepPrepare(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqProcessor::StepPrepare");

         // Update the internal time space & get time space's now time...

         m_timeSpace.UpdateUsingGlobalNow();
         m_localNowTime = m_timeSpace.GetNow();

         RWS_RETURNVOID();
      }

      /**
      *
      * Called within the processing loop. The 'StepPrepare' function must have been
      * called before starting this loop. See code extract below for details on the
      * processing loop.
      *
      * Each call to the function progresses the sequence. Unless it is waiting for
      * an event - this can be detect by calling 'GetWaitEventName', if this returns
      * non-zero, there is no point in calling 'StepPrepare' or this function. Again,
      * see code example below.
      *
      * The function will process a block of instructions and return a status. The
      * possible return values are...
      *
      * SS_SEND_EVENT      = returned when a 'send event' instruction has been reached.
      *                      The calling code needs to send an event with a data value
      *                      of 0 to the event returned via the rpWaitEventName param.
      *                      After sending processing should CONTINUE.
      * SS_LOOPED          = returned when the sequence loops, this is just to inform
      *                      the caller of the event. The caller does not need to do
      *                      anything other than CONTINUE to process.
      * SS_UP_TO_DATE      = returned when all processing to bring the sequence up to
      *                      date has been completed. The processing loop should end
      *                      for the current update.
      * SS_WAIT_ON_EVENT   = returned when the sequence has stalled waiting for an
      *                      event. The caller should link to the event named via the
      *                      rpWaitEventName param and stop processing. Once this event
      *                      is received, the code must call 'ClearWaitEvent' and then
      *                      start processing the update loop again. A check for if the
      *                      update loop can be called or not, can be made via the
      *                      'GetWaitEventName' function, if this returns ZERO, then the
      *                      processing loop SHOULD be run.
      * SS_ENDED           = returned once the sequence is complete. That is all the
      *                      instructions have been processed and there is no looping.
      *                      Once received the processing of the sequence can be totally
      *                      stopped until the next time the sequence is started. The
      *                      'FreeBuffers' function can be called to free up memory or
      *                      if necessary the 'FreeAll' function - but if this is called
      *                      the 'Init' function MUST be re-used before 'Start' function
      *                      can be called again.
      * 
      * The enum includes another value SS_KEEP_CALLING - but this is never returned.
      * It's purpose is to allow the caller to more easily see if they should continue
      * to call the 'Step' function in the processing loop. If the returned state is
      * less than SS_KEEP_CALLING, then the loop should continue. If greater, then the
      * loop must end. Again, see code example below.
      *
      * The processor has it's own built-in clock. Using the processing loop (see code
      * below) will cause the processor to catch up to the 'now' time of the global
      * clock. However, the processor's clock can be set to run at a relative speed to
      * the global clock, either faster, slower or the same - see the 'SetRelativeSpeed'
      * function. The fact that the processor uses an internal clock means that it
      * provides 'frame compensation' as standard.
      *
      * Code sample (handle update event)...
      *

         if (msg.Id == <tick event>)
         {
            if (processor.IsRunning() && processor.GetWaitEventName() == 0)
            {
               CSeqProcessor::StepState state;
               const char *pEventName;

               processor.StepPrepare();

               do
               {
                  state = processor.Step(pEventName);

                  if (state == CSeqProcessor::SS_SEND_EVENT)
                  {
                     RegisterMsg(<send event id>, pEventName);
                     CMsg msg(<send event id>);
                     SendMsg(msg);
                     UnRegisterMsg(<send event id>);
                  }
                  else if (state == CSeqProcessor::SS_WAIT_EVENT)
                  {
                     RegisterMsg(<wait for event id>, pEventName);
                     LinkMsg(<wait for event id>);
                  }
                  else if (state == CSeqProcessor::SS_LOOPED)
                  {
                     RWS_DEBUGSTREAM("Sequence looped...!" << std::endl);
                  }
               }
               while (state < CSeqProcessor::SS_KEEP_CALLING);
            }
         }

      *
      * Code sample (handle waiting for event)...
      *

         if (msg.Id == <wait for event id>)
         {
            processor.ClearWaitEvent();
            UnLinkMsg(<wait for event id>);
            UnRegister(<wait for event id>);
         }

      *
      * \param rpWaitEventName On exit this will be set to either zero or the address
      *                        of the wait event name or send event name.
      * \return Returns a state code (see above) indicating what point the processing
      *         has reached.
      * \see ClearWaitEvent
      * \see FreeBuffers
      * \see FreeAll
      * \see StepPrepare
      * \see GetWaitEventName
      * \see SetRelativeSpeed
      * \see Init
      * \see Start
      * \see Stop
      *
      */

      CSeqProcessor::StepState CSeqProcessor::Step(const char *&rpWaitEventName)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqProcessor::Step");
         RWS_ASSERT(m_pSeqIttr, "Iterator not setup, not called Init and Start?");
         RWS_ASSERT(m_pSeqStack, "Sequence stack not setup, not called Init and Start?");

         RwUInt32 waitOverShoot = 0;
         const char *pSendEventName = 0;

         // Handle the sequence update using this time... Paused or waiting for event ?

         if (m_isWaiting || m_isWaitingForEvent)
         {
            // Completed wait ?

            if (m_localNowTime >= m_waitTimeOut && !m_isWaitingForEvent)
            {
               // Cancel flag. Need to keep a record of any
               // overshooting which happened.

               m_isWaiting = FALSE;
               waitOverShoot = m_localNowTime - m_waitTimeOut;
            }
            else
            {
               // No, but sequence is still running. Waiting for event ?

               if (m_isWaitingForEvent)
               {
                  // No time will have passed, as clock is paused, so just return state.
                  // Must return name of event being waited for too...

                  rpWaitEventName = m_waitEventName;

                  RWS_RETURN(SS_WAIT_ON_EVENT);
               }
               else
               {
                  // Time may have (likely to have) passed, so update sequence stack, etc...

                  m_pSeqStack->ProcessItems(m_localNowTime);
                  m_pSeqStack->Flush();

                  RWS_RETURN(SS_UP_TO_DATE);
               }
            }
         }

         // Do ALL instructions until either
         // A) end-of-file; B) wait for period or C) pause waiting
         // for message.

         RwBool run = TRUE;

         while (run)
         {
            // Get next instruction...

            RwUInt32 instruction = m_pSeqIttr->GetInstruction();

            // Handle it...

            switch (instruction)
            {
            case CSeqCtrlDataIttr::IT_EOF:

               // Reached end of sequence. DO NOT want to step passed this command,
               // so just exit. Have all on-going sequences also finished ?

               if (m_pSeqStack->IsActiveEmpty())
               {
                  // Yep, so finished interpolations and no new commands - finished. BUT
                  // there may be data added that needs to be flushed (if only using
                  // keyframes, etc).

                  m_pSeqStack->Flush();

                  // If any entities need deleting, do so now...

                  DeleteEntitiesInList();

                  // Is the sequenced flagged as looping ?

                  if (m_isLooping)
                  {

                     // 'Wait time' is when the loop occurred. So just need to
                     // reset the iterator and inform the processor caller. On
                     // next call the overshoot will be the amount of the next
                     // loop of the sequence instructions that should be processed.

                     m_pSeqIttr->Reset();
                     RWS_RETURN(SS_LOOPED);
                  }
                  else
                  {
                     RWS_RETURN(SS_ENDED);
                  }               
               }
               else
               {
                  // No, still going, need up do update on any running interpolations
                  // and flush this data...

                  m_pSeqStack->ProcessItems(m_localNowTime);
                  m_pSeqStack->Flush();

                  RWS_RETURN(SS_UP_TO_DATE);
               }
               break;

            case CSeqCtrlDataIttr::IT_WAIT:

               // Waits demarcates the period between blocks of key-frames being
               // processed. Therefore, need to a) process any on-going interpolations
               // (up to the processing time ONLY) and b) flush any data build up by
               // these / keyframe data added to the entities.

               m_pSeqStack->ProcessItems(m_localNowTime - waitOverShoot);
               m_pSeqStack->Flush();

               // If any entities need deleting, do so now...

               DeleteEntitiesInList();

               // Going into a 'wait for event' state ?

               if (m_isWaitingForEvent)
               {
                  // Disable clock and set to last processed time...

                  m_timeSpace.SetRunning(FALSE);
                  m_timeSpace.SetNow(m_localNowTime - waitOverShoot);

                  // Set wait time to occur at correct time (just effectively removed
                  // any over shoot time). Need to do this as when clock is set to
                  // running again (event is received) need to wait required period before
                  // processing any new keyframes, etc...

                  m_waitTimeOut = m_timeSpace.GetNow() + m_pSeqIttr->GetWaitTime();
                  m_isWaiting = TRUE;

                  // Now force exit of processing loop...

                  run = FALSE;
               }
               else
               {
                  // Enter waiting period. Since unlikely to have come into
                  // sequence exactly on time, need to cope with over shooting
                  // last wait (if one). Therefore, take over shoot value off
                  // wait time.
                  //
                  // NOTE: if the overshoot time (left over time from previous
                  //       wait completion) is larger than this new wait, then
                  //       just step straight over it.

                  if (m_pSeqIttr->GetWaitTime() > waitOverShoot)
                  {
                     run = FALSE;
                     m_waitTimeOut = m_localNowTime + m_pSeqIttr->GetWaitTime();
                     m_waitTimeOut -= waitOverShoot;
                     m_isWaiting = TRUE;
                  }
                  else
                  {
                     // Wait shorter than over shoot time, so need to continue
                     // processing to 'catch up', BUT the overshoot time needs
                     // to be accurate for next set, so take off the wait time...

                     waitOverShoot -= m_pSeqIttr->GetWaitTime();
                  }
               }
               break;

            case CSeqCtrlDataIttr::IT_ENTITY:

               // Update current entity & get it's function pointer (if necessary).

               m_curEntityGuid = *m_pSeqIttr->GetEntityGuid();
               break;

            case CSeqCtrlDataIttr::IT_CLASS:

               // Update class name pointer.

               m_curClassName = m_pSeqIttr->GetClassName();
               break;

            case CSeqCtrlDataIttr::IT_COMMAND:

               // Update command number value.

               m_curCommandID = m_pSeqIttr->GetCommand();
               break;

            case CSeqCtrlDataIttr::IT_KEYFRAME:
               {
                  CSeqNode *pSeqNode;

                  // Does the 'current' GUID, name & command no exist in sequence stack ?

                  pSeqNode = m_pSeqStack->FindItem(m_curEntityGuid, m_curClassName, m_curCommandID);

                  if (pSeqNode == 0)
                  {
                     // Not found, so add it - must be added at correct time in the sequence, so
                     // take off any slack time (overshoot) due to not processing fast enough...

                     pSeqNode = m_pSeqStack->AddItem(m_curEntityGuid, m_curClassName, m_curCommandID,
                        m_pSeqIttr->GetKeyframeTypeName(), m_localNowTime - waitOverShoot);
                  }

                  // Again, need to add key at correct time (in case it's an interpolation) so
                  // need to take off any slack / overshoot time. Check if it's an interpolated
                  // value (function returns TRUE)...

                  if (pSeqNode->AddKey(*m_pSeqIttr, m_localNowTime - waitOverShoot))
                  {
                     // It's an interpolation, so move to active list. Will be up-to-date in
                     // terms of the current point in the sequence - as just has it's keyframe
                     // set. This occurs because the system HAS TO HAVE the interpolation data
                     // at the beginning of an interpolation (so it can do the interpolation
                     // over the time in which is occurs). Therefore data would be...
                     //
                     // <start keyframe><interpolation data><end keyframe>.

                     m_pSeqStack->MoveToActive(pSeqNode);
                  }
               }
               break;

            case CSeqCtrlDataIttr::IT_INTERPOL:
            case CSeqCtrlDataIttr::IT_DYNAMIC_INTERPOL:
               {
                  CSeqItem *pSeqItem;

                  // Does the 'current' GUID, name & command no exist in sequence stack ?

                  pSeqItem = m_pSeqStack->FindItem(m_curEntityGuid, m_curClassName, m_curCommandID);
                  RWS_ASSERT(pSeqItem, "attempting interpolation before setting start keyframe. "
                     "There MUST be a keyframe instruction before and after an interpolator "
                     "instruction.");

                  // Add this interpolator data...

                  pSeqItem->AddInterpol(*m_pSeqIttr);
               }
               break;

            case CSeqCtrlDataIttr::IT_SEND_EVENT:

               // Store name and cause processing loop exit...
               //
               // NOTE: this is NOT a wait event and will ONLY be reported to the processor
               //       caller once.

               pSendEventName = m_pSeqIttr->GetEventName();
               run = FALSE;
               break;

            case CSeqCtrlDataIttr::IT_WAIT_EVENT:
               {
                  // Wait for event. NOTE: any events, keyframes or interpolations will be updated
                  // FIRST and then the wait will become active. Wait name is from string table, so
                  // safe to store value.

                  m_waitEventName = m_pSeqIttr->GetWaitEventName();

                  // Flag that, at the end of the processing of this 'time', the system has to
                  // wait for an event...

                  m_isWaitingForEvent = TRUE;
               }
               break;
               
            case CSeqCtrlDataIttr::IT_CREATE:
               {
                  const CAttributePacket *pAttrPack;
                  CAttributeHandler *pAttrHand;

                  // Get attribute packet and build new instance using it.

                  pAttrPack = m_pSeqIttr->GetCreateData();
                  pAttrHand = pAttrPack->CreateEntity();
                  RWS_ASSERT(pAttrHand, "Failed to create new entity in sequence.");
               }
               break;

            case CSeqCtrlDataIttr::IT_DELETE:
               {
                  // In debug mode, check to see if the offset is already in the list.

#ifndef NDEBUG
                  RwUInt32 count = m_curNumEntityDels;

                  while (count)
                  {
                     count--;
                     RWS_ASSERT(m_pEntityDelGuidOffsetList[count] !=
                        m_pSeqIttr->GetDeleteGuidOffset(),
                        "Deleting the same entity twice will cause at least an assert.");
                  }
#endif
                  // Since, the GUID is from the GUID table, the pointer to it can be
                  // stored ready for deletion after all updates have been made. Resize
                  // if necessary...

                  ResizeDelBuffer(m_curNumEntityDels + 1);

                  m_pEntityDelGuidOffsetList[m_curNumEntityDels] =
                     m_pSeqIttr->GetDeleteGuidOffset();
                  m_curNumEntityDels++;
               }
               break;

            case CSeqCtrlDataIttr::IT_ERROR:
            default:

               RWS_ASSERTFAIL("Error or unknown / unhandled sequence control instruction.");
               break;
            }

            // Step to next instruction.

            (*m_pSeqIttr)++;
         }

         // Return and wait for event / wait time-out. Sequence is STILL in progress.

         if (m_isWaitingForEvent)
         {
            // Must return name of event being waited on...

            rpWaitEventName = m_waitEventName;

            RWS_RETURN(SS_WAIT_ON_EVENT);
         }
         else
         {
            if (pSendEventName)
            {
               // Need to return name of event being waited for.

               rpWaitEventName = pSendEventName;
               RWS_RETURN(SS_SEND_EVENT);
            }
            else
            {
               RWS_RETURN(SS_UP_TO_DATE);
            }
         }
      }

      /**
      *
      * Causes the entities in the deletion list to be removed.
      *
      */

      void CSeqProcessor::DeleteEntitiesInList(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqProcessor::DeleteEntitiesInList");

         while (m_curNumEntityDels)
         {
            CAttributeHandler *pAttrHand;
            RWSGUID delGuid;
            CSeqStackFindToken findToken;

            // Find & delete it...

            m_curNumEntityDels--;
            delGuid = *m_pSeqIttr->GetGuid(m_pEntityDelGuidOffsetList[m_curNumEntityDels]);
            pAttrHand = CAttributeHandler::Find(delGuid);

            // This pointer may be NULL (0) if the dynamic sequencer is being used with templates.
            // Since the fact that it is NULL does not cause any problems, do NOT assert.

            delete pAttrHand;          // Delete will cope with NULL pointer.

            // Find in sequence stack and mark as deleted, this is needed in looping
            // sequences or ones where the sequence stack (and buffer handler) are not
            // freed, so that they can correctly reconnect to the entities in templates.
            // This has to be performed for every class / attribute within the deleted
            // entity...

            m_pSeqStack->FindFirstItem(findToken, delGuid);

            while (findToken.IsFound())
            {
               findToken.GetSeqItem()->DisconnectFromHandler();
               m_pSeqStack->FindNextItem(findToken);
            }
         }

         RWS_RETURNVOID();
      }

      /**
      *
      * Frees the buffer used by the sub-objects responsible for handling playback.
      * Safe to call at any point, if the sub-objects have not been allocated, will
      * have no effect.
      *
      */

      void CSeqProcessor::FreeBuffers(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqProcessor::Stop");

         if (m_pSeqStack)
         {
            m_pSeqStack->FreeItems();
         }

         RWS_RETURNVOID();
      }

      /**
      *
      * Called to free up all memory used by the system and remove any sequence
      * data attached to the processor. After a call to this function 'Init' MUST
      * be called before attempting to start the sequence again.
      *
      */

      void CSeqProcessor::FreeAll(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqProcessor::FreeAll");

         // Stop playback.

         Stop();

         // And remove the sequence data & iterator too. Destructors should remove
         // any buffers too, so no need to call 'FreeBuffers'.

         delete m_pSeqStack;                    // Delete copes with NULL pointer.
         delete m_pSeqIttr;                     // Delete copes with NULL pointer.
         delete[] m_pEntityDelGuidOffsetList;   // Delete copes with NULL pointer.

         m_pSeqStack = 0;
         m_pSeqIttr = 0;
         m_pEntityDelGuidOffsetList = 0;
         m_curNumEntityDels = 0;
         m_maxNumEntityDels = 0;
         
         RWS_RETURNVOID();
      }

      /**
      *
      * Called to stop the sequence from progressing (default) or to restart
      * the sequence. It is safe to call this function repeatedly with enable
      * or disable, as it will only perform the state change once (ie enabled ->
      * disabled or disabled -> enabled). This freeze acts independently of the
      * sequences internal pausing for events or timer.
      *
      * \param freeze Flag to indicate if freeze should be enabled or not. Default
      *        is to enable (TRUE) the freeze.
      *
      */

      void CSeqProcessor::Freeze(RwBool freeze)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqProcessor::Freeze");

         // Process only if state has changed.

         if (freeze != m_isFrozen)
         {
            if (freeze)
            {
               // Stop the clock, if already stopped as a result of an internal
               // wait state, this won't cause any problems.

               m_isFrozen = TRUE;
               m_timeSpace.SetRunning(FALSE);
            }
            else
            {
               // Need to check it's okay to restart the clock.

               m_isFrozen = FALSE;
               TestEnableClock();
            }
         }

         RWS_RETURNVOID();
      }

      /**
      *
      * not documented. Name may be 0, RWSGUID MUST be valid.
      *
      */

      void CSeqProcessor::DebugDisp(const char *name, const RWSGUID *pGuid)
      {

/*
                  // Inform log of what's been loaded (DEBUG)... NOTE: the resource
                  // name can be zero, if the workspace has not been asked to send
                  // names to the console. In that case, use the GUID instead.

                  if (ResourceName != 0)
                  {
                     RWS_DEBUGSTREAM("Loaded sequence [" << ResourceName << "].\n" <<
                        "Contents of sequence data are...\n" << std::endl);
                  }
                  else
                  {
                     RWS_DEBUGSTREAM("Loaded sequence [" << *pResourceID << "].\n" <<
                        "Contents of sequence data are...\n" << std::endl);
                  }

                  CSeqCtrlDataIttr ittr(*m_pSeqData, RWS_MEMORY_TYPE_RWS);

                  ittr.DebugDispStringTable();
                  ittr.DebugDispGuidTable();

                  while (ittr.GetInstruction() != CSeqCtrlDataIttr::IT_EOF)
                  {
                     ittr.DebugDispCurrent();
                     ittr++;
                  }
*/
      }

      /**
      *
      * Checks to see if all locks on the clock are free and if so enables
      * the internal clock. ONLY CALL if the clock is currently disabled, if the
      * clock is already enabled, calling this function may result if time being
      * lost.
      *
      */

      void CSeqProcessor::TestEnableClock(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqProcessor::TestEnableClock");

         if (!m_isFrozen && !m_isWaitingForEvent)
         {
            // Before setting the time space to running, catch up with the global
            // time, this will stop the system from skipping forward.
            //
            // NOTE: this will cause the clock to skip forward if this function is
            //       being called when the wait events are potentially changing
            //       from paused -> running. More specifically, if the clock is
            //       already enabled and the function is called, then time will be
            //       lost.

            m_timeSpace.UpdateUsingGlobalNow();
            m_timeSpace.SetRunning(TRUE);            
         }

         RWS_RETURNVOID();
      }

      /**
      *
      * not documented
      *
      */

      void CSeqProcessor::ResizeDelBuffer(RwUInt32 newSize)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqProcessor::ResizeDelBuffer");

         // ONLY resize if new size is larger than current one.

         if (newSize > m_maxNumEntityDels)
         {
            RwUInt32 *pNewList;

            // Allocate new list, need to copy across data if old list.

            pNewList = RWS_NEW_TYPE(m_memType) RwUInt32[newSize];
            RWS_ASSERT(pNewList, "Failed to resize entity deletion buffer.");

            // Is there an old list ?

            if (m_maxNumEntityDels != 0)
            {
               // If requested to do so, warn the user.

               if ((m_flags & SEQ_FLAG_BUFFER_WARNINGS) != 0)
               {
                  RWS_DEBUGSTREAM_WARN("Resizing entity deletion buffer in sequence processor. "
                     "This is slow (due to data copying) and may cause memory fragmentation." <<
                     std::endl);
               }

               // Copy across old data and delete old list.

               memcpy(pNewList, m_pEntityDelGuidOffsetList, sizeof(RwUInt32) * m_maxNumEntityDels);
               delete[] m_pEntityDelGuidOffsetList;
            }

            // Store new buffer and new size.

            m_pEntityDelGuidOffsetList = pNewList;
            m_maxNumEntityDels = newSize;
         }

         RWS_RETURNVOID();
      }
   }  // SEQUENCE
}     // RWS
