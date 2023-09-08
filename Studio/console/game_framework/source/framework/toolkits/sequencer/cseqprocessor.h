/*****************************************************************************
*
* File :      CSeqProcessor.h
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

#ifndef C_SEQ_PROCESSOR_HEADER
#define C_SEQ_PROCESSOR_HEADER

#include "cseqstack.h"
#include "ctimespace.h"

namespace RWS
{
   namespace SEQUENCE
   {
      /**
      *
      * Base class for processing the sequence instruction data and controlling
      * the CSeqStack, etc.
      *
      */

      class CSeqProcessor
      {
      public:

         /**
         *
         * States returned by the 'Step' function.
         *
         * \see StepPrepare.
         * \see Step.
         *
         */

         enum StepState
         {
            SS_SEND_EVENT,          /**< Need to send an event, use returned event name.
                                         Send this event and then return to processing the
                                         sequence via the 'Step' function. */
            SS_LOOPED,              /**< Sequence has looped. Just informational, continue
                                         to call the 'Step' function. */

            //------

            SS_KEEP_CALLING,        /**< Never returned, just to allow caller to tell
                                         if they should continue to call 'Step' or not.
                                         If returned state < SS_KEEP_CALLING, then do
                                         keep calling. If > SS_KEEP_CALLING, then stop. */

            //------

            SS_UP_TO_DATE,          /**< Sequence is up-to-date, there are no more
                                         instructions to update at this time. */
            SS_WAIT_ON_EVENT,       /**< Sequence has been halted because it needs to wait
                                         for an event, use returned event name. Link to this
                                         and exit the sequence processing loop. When the
                                         event occurs, call 'ClearWaitEvent' and then start
                                         to process the sequence again. */
            SS_ENDED                /**< The end of the sequence has been reached (it's not
                                         looping) so no more processing is possible. */
         };

         CSeqProcessor(RwUInt32 memType = RWS_MEMORY_TYPE_RWS, RwUInt32 delBuffSize = 0,
            RwUInt32 flags = 0);
         virtual ~CSeqProcessor(void);

         virtual void Init(const CSeqCtrlData &dataToUse, RwUInt32 strBuffSize = 0,
            RwUInt32 guidBuffSize = 0);
         virtual void Start(RwUInt32 *pStartTime = 0);
         virtual void StepPrepare(void);
         virtual StepState Step(const char *&rpWaitEventName);

         virtual void FreeBuffers(void);
         virtual void FreeAll(void);

         virtual void Freeze(RwBool freeze = TRUE);

         virtual void DebugDisp(const char *name, const RWSGUID *pGuid);

         // Inline functions...

         /**
         *
         * Returns the statue of the frozen flag.
         *
         * \return TRUE if sequence playback is frozen as a result of a call to
         *         Freeze(TRUE) - does NOT indicate any internal frozen states,
         *         and FALSE if not frozen as a result of this call.
         *
         */

         virtual RwBool IsFrozen(void) { return(m_isFrozen); }

         /**
         *
         * Returns TRUE if the sequence is running. If this is true, it does NOT
         * indicate the actual progression is being made, the sequence may be
         * waiting on time, an event or be frozen.
         *
         */

         virtual RwBool IsRunning(void) { return(m_isRunning); }

         /**
         *
         * Stops the sequence from playing. Does NOT free up any buffers or
         * sub-objects. See the free functions for this.
         *
         * \see FreeBuffers
         * \see FreeAll
         *
         */

         virtual void Stop(void) { m_isRunning = FALSE; }

         /**
         *
         * Returns event name pointer if the system is waiting for an event. If not
         * waiting on an event, zero will be returned.
         *
         * \see Step.
         * \see ClearWaitEvent.
         *
         */

         virtual const char *GetWaitEventName(void) { return(m_waitEventName); }

         /**
         *
         * Clears the wait event. NOTE: function will ASSERT if NOT waiting for an event.
         *
         * \see WaitEventName
         * \see Step
         *
         */

         virtual void ClearWaitEvent(void)
         {
            RWS_ASSERT(m_waitEventName && m_isWaitingForEvent, "Not waiting for an event.");
            
            m_isWaitingForEvent = FALSE;
            m_waitEventName = 0;
         }

         /**
         *
         * Modifies the relative speed of the clock (used by the sequence) in relation to
         * the global clock. Default is 1.0, is the same speed.
         *
         * \param relativeRate Relative rate of the sequences clock to the global clock.
         *                     1.0f = the same speed, 0.1f = 1/10 global clock speed,
         *                     10.0f = 10x faster than global clock.
         *
         */

         virtual void SetRelativeSpeed(RwReal relativeRate = 1.0f)
         {
            m_timeSpace.SetRelativeSpeed(relativeRate);
         }

         /**
         *
         * Sets if the sequence is looping or not via flag (default is to enable looping
         * or TRUE).
         *
         */

         virtual void Loop(RwBool flag = TRUE) { m_isLooping = flag; }

         /**
         *
         * Returns TRUE if the sequence is set to loop and FALSE if not.
         *
         */

         virtual RwBool IsLooping(void) { return(m_isLooping); }

      protected:

         void TestEnableClock(void);
         void ResizeDelBuffer(RwUInt32 newSize);
         void DeleteEntitiesInList(void);

         RwBool m_isLooping,                       /**< Indicates if sequence is to loop. */
            m_isRunning,                           /**< Indicates if the sequence is running. */
            m_isWaiting,                           /**< Indicates if the sequence is waiting for time-out. */
            m_isWaitingForEvent,                   /**< Indicates if the sequence is waiting for an event. */
            m_isFrozen,                            /**< Indicates if the sequence has be requested to
                                                        freeze. */
            m_flags;
         CTimeSpace m_timeSpace;                   /**< Internal clock use by processor. */
         CSeqStack *m_pSeqStack;                   /**< The sequence stack to use for playback. */
         CSeqCtrlDataIttr *m_pSeqIttr;             /**< A pointer to the data iterator. */
         RWSGUID m_curEntityGuid;                  /**< The current entity being sequenced. */
         const char *m_curClassName,               /**< The current class being sequenced. */
            *m_waitEventName;                      /**< A pointer to the event name (held in string table)
                                                        that the system is waiting for. Zero if none. */
         RwUInt32 m_curCommandID,                  /**< The current command being sequenced. */
            m_memType,                             /**< Memory type being used for allocations. */
            m_waitTimeOut,                         /**< The (local) time at which the time based
                                                        wait ends. */
            m_localNowTime,                        /**< The sequence's "now time", used in
                                                        'Step' function. */
            m_curNumEntityDels,
            m_maxNumEntityDels;
         RwUInt32 *m_pEntityDelGuidOffsetList;
      };
   }     // SEQUENCE
}        // RWS
#endif   // #ifndef C_SEQ_PROCESSOR_HEADER
