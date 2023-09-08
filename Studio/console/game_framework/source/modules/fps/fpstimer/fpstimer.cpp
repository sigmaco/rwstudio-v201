/*****************************************************************************
*
* File :     FPSTimer.cpp
*
* Abstract : func_timer waits time + random time before triggering...
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
* Copyright (c) 2000 Criterion Software Ltd.
* All Rights Reserved.
*
* RenderWare is a trademark of Canon Inc.
*
*****************************************************************************/
#include "precomp.h"
//#define RWS_TRACEENABLE
//#define RWS_CALLSTACKENABLE

// Left to its own devices, the MW linker will dead-strip the registrar ctors, so...
#ifdef __MWERKS__
#pragma force_active on
#pragma exceptions off // Turn off exceptions due to bug in MW Dwarf Generator for PS2, present in CW 3.04
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Includes
//
#include "rprandom.h"
#ifdef _MSC_VER
#pragma comment (lib, "rprandom.lib")
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "fpstimer.h"
#include "framework/mainloop/logic.h"
#include "framework/toolkits/world/factory.h"
#include "framework/toolkits/debugtools/debugtools.h"

namespace RWS
{
   namespace FPS
   {
      RWS_IMPLEMENT_CLASSID(FPSTimer);
      RWS_REGISTERCLASS(FPSTimer);
      
      
      /**
      *
      * \ref FPSTimer. Constructor for FPSTimer.
      *
      * \see ~FPSTimer.
      *
      */
      FPSTimer::FPSTimer(const CAttributePacket& attr) :
      InitCEventHandler(&m_pAtomic),
         m_wait_counter(0),
         m_wait_num_frames(RWS::MainLoop::Logic::Rate*60),
         m_random(0)
      {
         RWS_FUNCTION("RWS::FPS::FPSTimer::FPSTimer");
         
         // Can't use initializer ('this' isn't valid) so...
         m_pAtomic = CreateAtomicInWorldFromResource(attr, this);
         RWS_ASSERT(m_pAtomic, "Failed to create atomic");
         
         LinkMsg(iMsgRunningTick);
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref ~FPSTimer. Destructor for FPSTimer.
      *
      * \see FPSTimer.
      *
      */
      FPSTimer::~FPSTimer(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSTimer::~FPSTimer");
         
         UnLinkMsg(iMsgRunningTick);
         UnRegisterMsg( m_target );
         UnRegisterMsg( m_killtarget );
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref HandleEvents(). Handle events.    
      *
      * \param pMsg Standard Message Package.
      *  
      * \ref iMsgRunningTick
      */
      void FPSTimer::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::FPS::FPSTimer::HandleEvents");
         
         if (pMsg.Id == iMsgRunningTick)
         {
#if defined (RWS_DEBUGTOOLS) && defined (RWS_DESIGN)
            if (m_Flags & uATTRIBUTEHANDLER_FLAG_DEBUG)
            {
               DebugTools::Printf("FPS_func_timer::HandleEvents\n\n");
               DebugTools::Printf("m_wait_counter %d\n", m_wait_counter);
               DebugTools::Printf("m_wait_num_frames %d\n", m_wait_num_frames);
               DebugTools::Printf("m_random %d\n", m_random);
            }
#endif
            m_wait_counter ++ ;
            
            if (m_wait_counter >= m_wait_num_frames)
            {
               RwInt32 rand = RpRandom()&0xffff;
               rand = m_random * rand;
               rand = rand >> 16;
               m_wait_counter = - rand;   // Reset m_wait_counter total time = m_wait_num_frames+rand
               
               CMsg msg(m_target, RpAtomicGetFrame(m_pAtomic.ptr()));
               
               SendMsg(msg);
               
               RegisterToDeleteEventHandlers(m_killtarget);
            }
         }
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref HandleAttributes. Handle attribute updates.
      *
      * \param attr reference to a CAttributePacket.
      *
      */ 
      void FPSTimer::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::FPS::FPSTimer::HandleAttributes");
         
         CAttributeHandler::HandleAttributes(attr);
         
         m_pAtomic.HandleSystemCommands(attr); // Initialize atomic/clump/frame
         
         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(FPSTimer));
         
         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
            case CMD_target:
               ReplaceRegisteredMsg(m_target, attrIt->GetAs_RwChar_ptr(), "RwFrame*");
               break;
               
            case CMD_killtarget:
               ReplaceRegisteredMsg(m_killtarget, attrIt->GetAs_RwChar_ptr(), 0);
               break;
               
            case CMD_wait:
               {
                  RwReal wait_num_frames;
                  attrIt->GetCommandData(wait_num_frames);
                  m_wait_num_frames = (RwInt32)((RwReal)wait_num_frames * RWS::MainLoop::Logic::Rate);
                  
                  m_wait_counter = 0;  // Reset wait time
               }
               break;
               
            case CMD_random:
               RwReal _random = attrIt->GetAs_RwReal();
               m_random = static_cast<RwUInt32>(RWS::MainLoop::Logic::Rate * (_random));
               RWS_TRACE("FPSTimer::HandleAttributes " << m_random );
               break;
            }
            ++attrIt;
         }
         RWS_RETURNVOID();
      }
   }// namespace FPS
}// namespace RWS
