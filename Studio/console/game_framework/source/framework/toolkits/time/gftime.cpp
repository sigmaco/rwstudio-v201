/*****************************************************************************
*
* File :     time.cpp
*
* Abstract : Time measurement services
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

//////////////////////////////////////////////////////////////////
//
// RenderWare studio Includes
//
#include "time.h"
#include "framework/core/macros/debugmacros.h"

//////////////////////////////////////////////////////////////////
//
// System Includes
//
#ifdef _XBOX
#include <xtl.h>
#endif

#ifdef _WINDOWS
#include <windows.h>
#include <mmsystem.h>
#endif

#ifdef SKY
#include <eeregs.h>
#endif

#ifdef DOLPHIN
#include <dolphin.h>
#endif

namespace RWS
{
   namespace Time
   {
      namespace
      {
#ifndef NDEBUG
         bool gIsInited = false;
#endif

#ifdef SKY
         int timerHandlerHid = -1;
         volatile unsigned long sweHighCount = 0;

         /*
         *
         *  \ingroup Time
         *
         *  PS2 only: Interrupt handler for time measurement services
         *
         */
         int TimerHandler(int ca)
         {
            if ((ca == INTC_TIM0) && (*T0_MODE & 0x800))
            {
               *T0_MODE |= 0x800;

               sweHighCount += 0x10000;
            }

            // This is a TRC requirement at the moment
            ExitHandler();

            return (0);
         }
#endif
      }

      ////////////////////////////////////////////////////////////////////////
      //
      void Open()
      {
         RWS_FUNCTION("RWS::Time::Open");
         RWS_PRE(!gIsInited)

#ifdef SKY
         timerHandlerHid = AddIntcHandler(INTC_TIM0, TimerHandler, 0);
         *T0_COUNT = 0;
         *T0_COMP = 0;
         *T0_HOLD = 0;
         *T0_MODE = 0x281;
         EnableIntc(INTC_TIM0);
#endif

#ifndef NDEBUG
         gIsInited = true;
#endif

         RWS_RETURNVOID();
      }

      ////////////////////////////////////////////////////////////////////////
      //
      void Close()
      {
         RWS_FUNCTION("RWS::Time::Close");
         RWS_PRE(gIsInited)

#ifdef SKY
         DisableIntc(INTC_TIM0);
         RemoveIntcHandler(INTC_TIM0, timerHandlerHid);
#endif

#ifndef NDEBUG
         gIsInited = false;
#endif

         RWS_RETURNVOID();
      }

#if defined(_WINDOWS)
      ////////////////////////////////////////////////////////////////////////
      //
      RwUInt32 GetTime()
      {
         RWS_PRE(gIsInited);

         TIMECAPS timecaps;
         timeGetDevCaps(&timecaps, sizeof(TIMECAPS));

         timeBeginPeriod(timecaps.wPeriodMin);
         RwUInt32 result = (RwUInt32) timeGetTime();
         timeEndPeriod(timecaps.wPeriodMin);

         return (result);
      }

#elif defined(_XBOX)
      ////////////////////////////////////////////////////////////////////////
      //
      RwUInt32 GetTime()
      {
         RWS_PRE(gIsInited);

         RwUInt32 result = (RwUInt32) timeGetTime();

         return (result);
      }

#elif defined(SKY)
      ////////////////////////////////////////////////////////////////////////
      //
      // Note: If sweHighCount reaches 0x100000000 * 1024 this will return the wrong result 
      // 0x100000000 * 1024
      //
      // = 4398046511104 / 9216
      // = 477218588 ms
      // = 477218 s
      // = 7953 m
      // = 132 hrs
      // = 5.5 days
      //
      RwUInt32 GetTime()
      {
         RWS_PRE(gIsInited);

         unsigned long high0, high1, low0, low1;

         /* Just to make sure that the compiler can't mess with this... */
         asm volatile (".set noat
                   .set noreorder
                   .set nomacro
                   sync.l
                   sync.p
                   lui $1, 0x1000
                   ld  %0, 0(%4)
                   sync.l
                   sync.p
                   lwu %1, 0($1)
                   sync.l
                   sync.p
                   ld  %2, 0(%4)
                   sync.l
                   sync.p
                   lwu %3, 0($1)
                   sync.l
                   sync.p
                   .set macro
                   .set reorder
                   .set at
                   nop"
                  : "=r&" (high0), "=r&" (low0), "=r&" (high1), "=r&" (low1)
                  : "r" (&sweHighCount));

         RwUInt32 result = 0;
         if(high0 == high1)
         {
            // Div (1024 * 9) = 9216
            //
            result = (RwUInt32) ((high0 | (low0 & 0xffff)) >>10);
            
            result = result / 9;
         }
         else
         {
            // Div (1024 * 9) = 9216
            //
            result = (RwUInt32) ((high1 | (low1 & 0xffff)) >>10); 

            result = result / 9;
         }
         
         return(result);
      }

#elif defined(DOLPHIN)
      ////////////////////////////////////////////////////////////////////////
      //
      RwUInt32 GetTime()
      {
         RWS_PRE(gIsInited);
         
         OSTime time;
         time = OSGetTime();

         return(OSTicksToMilliseconds(time));
      }
#else
   #error Your platform here...
#endif
   }
}
