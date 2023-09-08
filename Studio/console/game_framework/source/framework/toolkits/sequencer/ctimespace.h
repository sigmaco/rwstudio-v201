/*****************************************************************************
*
* File :      CTimeSpace.h
*
* Abstract :  Class to allow time to flow at different rates.
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
* Copyright (c) 2004 Criterion Software Ltd.
* All Rights Reserved.
*
* RenderWare is a trademark of Canon Inc.
*
*****************************************************************************/

#ifndef C_TIME_SPACE_HEADER
#define C_TIME_SPACE_HEADER

namespace RWS
{
   namespace SEQUENCE
   {
      /**
      *
      * Allows time to be controlled. It is used to allow the flow of time within
      * objects which use it (the 'space') to be stopped, started, restarted or
      * slowed down / speeded up.
      *
      */

      class CTimeSpace
      {
      public:
         
         CTimeSpace(RwReal relativeSpeed = 1.0f, RwBool running = TRUE);
         CTimeSpace(RwUInt32 globalNowTime, RwReal relativeSpeed = 1.0f,
            RwBool running = TRUE);
         
         void Update(RwUInt32 globalNowTime);
         void UpdateUsingGlobalNow(void);
         void SetRelativeSpeed(RwReal relativeSpeed = 1.0f);
         void SetNow(RwUInt32 spaceNowTime = 0);
         void Sync(RwUInt32 globalNowTime, RwUInt32 spaceNowTime = 0);
         void SyncToGlobalNow(RwUInt32 spaceNowTime = 0);

         /**
         *
         * 
         *
         */

         RwUInt32 GetNow(void) { return(m_spaceNowTime); }

         /**
         *
         * 
         *
         */

         void SetRunning(RwBool running = TRUE) { m_runningFlag = running; }

         /**
         *
         * 
         *
         */


         RwBool IsRunning(void) { return(m_runningFlag); }

         /**
         *
         * 
         *
         */

         RwReal GetRelativeSpeed(void) { return(m_relativeSpeed); }

      private:

         void Init(RwUInt32 globalNowTime, RwReal relativeSpeed, RwBool running);

         RwUInt32 m_spaceNowTime,   /**< The time in milliseconds within this 'space'. */
            m_lastGlobalTime;       /**< Last global time passed into 'Update'. */
         RwReal m_relativeSpeed,    /**< If time in 'space' is running, indicates how fast. */
            m_msecFraction;         /**< Fractions of milliseconds held ready for next update. */
         RwBool m_runningFlag;      /**< Indicates if time in the 'space' is running. */
      };
   }     // SEQUENCE
}        // RWS
#endif   // #ifndef C_TIME_SPACE_HEADER
