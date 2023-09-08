/****************************************************************************
*
* File :      CTimeSpace.cpp
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

#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/time/gftime.h"
#include "ctimespace.h"

namespace RWS
{
   namespace SEQUENCE
   {
      //
      // CTimeSpace class's functions.
      //

      /**
      *
      * not documented
      *
      */

      CTimeSpace::CTimeSpace(RwReal relativeSpeed, RwBool running)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CTimeSpace::CTimeSpace (2 param)");

         Init(Time::GetTime(), relativeSpeed, running);

         RWS_RETURNVOID();
      }

      /**
      *
      * not documented
      *
      */

      CTimeSpace::CTimeSpace(RwUInt32 globalNowTime, RwReal relativeSpeed, RwBool running)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CTimeSpace::CTimeSpace");

         Init(globalNowTime, relativeSpeed, running);

         RWS_RETURNVOID();
      }

      /**
      *
      * not documented
      *
      */

      void CTimeSpace::Init(RwUInt32 globalNowTime, RwReal relativeSpeed, RwBool running)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CTimeSpace::CTimeSpace");

         m_spaceNowTime = 0;
         m_relativeSpeed = relativeSpeed;
         m_msecFraction = 0.0f;
         m_runningFlag = running;
         m_lastGlobalTime = globalNowTime;

         RWS_RETURNVOID();
      }

      /**
      *
      * not documented
      *
      */

      void CTimeSpace::SetRelativeSpeed(RwReal relativeSpeed)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CTimeSpace::SetRelativeSpeed");
         RWS_ASSERT(relativeSpeed >= 0.0f,
            "Time cannot flow backwards, relative speed must be >= 0.");

         m_relativeSpeed = relativeSpeed;

         RWS_RETURNVOID();
      }

      /**
      *
      * not documented
      *
      */

      void CTimeSpace::SetNow(RwUInt32 spaceNowTime)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CTimeSpace::SetNow");

         m_spaceNowTime = spaceNowTime;
         m_msecFraction = 0.0f;

         RWS_RETURNVOID();
      }

      /**
      *
      * not documented
      *
      */

      void CTimeSpace::Sync(RwUInt32 globalNowTime, RwUInt32 spaceNowTime)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CTimeSpace::Sync");

         m_spaceNowTime = spaceNowTime;
         m_msecFraction = 0.0f;
         m_lastGlobalTime = globalNowTime;

         RWS_RETURNVOID();
      }

      /**
      *
      * not documented
      *
      */

      void CTimeSpace::SyncToGlobalNow(RwUInt32 spaceNowTime)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CTimeSpace::SyncToGlobalNow");

         Sync(Time::GetTime(), spaceNowTime);

         RWS_RETURNVOID();
      }

      /**
      *
      * not documented
      *
      */

      void CTimeSpace::Update(RwUInt32 globalNowTime)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CTimeSpace::Update");
         RWS_ASSERT(globalNowTime >= m_lastGlobalTime,
            "The global (real world) clock cannot flow backwards.");

         // Is this 'space' running ?

         if (m_runningFlag)
         {
            RwUInt32 globalTimePassed,
               spaceTimePassed;
            RwReal spaceTimePassedExact;

            // How much time has gone past in global time. Scale this for this
            // time space.

            globalTimePassed = globalNowTime - m_lastGlobalTime;
            spaceTimePassedExact = static_cast<RwReal>(globalTimePassed) * m_relativeSpeed;

            // Add on any left over fractions - they may cause roll over to next millisecond.

            spaceTimePassedExact += m_msecFraction;

            // Need to convert this to millisecond ticks. The fraction (if any) must be
            // kept though to keep the clock accurate.

            spaceTimePassed = static_cast<RwUInt32>(spaceTimePassedExact);
            m_msecFraction = spaceTimePassedExact - static_cast<RwReal>(spaceTimePassed);

            // Update the space's now time.

            m_spaceNowTime += spaceTimePassed;
         }

         m_lastGlobalTime = globalNowTime;

         RWS_RETURNVOID();
      }

      /**
      *
      * not documented
      *
      */

      void CTimeSpace::UpdateUsingGlobalNow(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CTimeSpace::UpdateUsingGlobalNow");

         Update(Time::GetTime());

         RWS_RETURNVOID();
      }

   }  // SEQUENCE
}     // RWS
