/*****************************************************************************
 *
 * File :     CMultiAnimate.cpp
 *
 * Abstract : A simple animation behavior
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
 * Copyright (c) 2001 Criterion Software Ltd.
 * All Rights Reserved.
 *
 * RenderWare is a trademark of Canon Inc.
 *
 *****************************************************************************/
#include "precomp.h"
//#define RWS_CALLSTACKENABLE
//#define RWS_TRACEENABLE

// Left to its own devices, the MW linker will dead-strip the registrar ctors, so...
#ifdef __MWERKS__
#pragma force_active on
#pragma exceptions off // Turn off exceptions due to bug in MW Dwarf Generator for PS2, present in CW 3.04
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "cmultianimate.h"

#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/world/factory.h"

namespace
{
   const RwReal kFrameTime = 1.0f / RWS::MainLoop::Logic::Rate;
}

namespace RWS
{
   namespace Examples
   {
      RWS_IMPLEMENT_CLASSID(CMultiAnimate);
      RWS_REGISTERCLASS(CMultiAnimate);

      /**
      *
      * Constructor for CMultiAnimate. Creates the clump for the entity, attaches the animations,
      * prepares the first animation to be run (if one) and links to the running tick to allow
      * animation update.
      *
      */

      CMultiAnimate::CMultiAnimate(const CAttributePacket& attr) :
         InitCEventHandler(&m_pClump)
      {
         RWS_FUNCTION("RWS::Examples::CMultiAnimate::CMultiAnimate")

         // Can't use initializer ('this' isn't valid) so...
         m_pClump = CreateClumpInWorldFromResource(attr, this);

         RWS_ASSERT(m_pClump, "Failed to create atomic");

         // Wire the clump to the anim set
         m_animSet.SetTarget(*m_pClump);

         // It'd be better to link to the running tick once the first animation
         // has kicked off. However, we'd need a flag to tell us that we haven't
         // linked yet - do we really want the overhead of that extra memory?
         LinkMsg( iMsgRunningTick, 0 );

         RWS_RETURNVOID()
      }

      /**
      *
      *  Destructor for CMultiAnimate - unlinks / unregisters messages.
      *
      */

      CMultiAnimate::~CMultiAnimate()
      {
         RWS_FUNCTION("RWS::Examples::CMultiAnimate::~CMultiAnimate")

         UnLinkMsg( iMsgRunningTick );
         UnLinkMsg(m_msgNextAnimTrigger);
         UnRegisterMsg(m_msgNextAnimTrigger);

         RWS_RETURNVOID()
      }

      /**
      *
      * Mandatory CAttributeHandler interface - handles new attribute packets received from the workspace.
      * These contain changes to the frame rate, trigger name, looping and blend duration controls.
      * Also calls the handlers for the clump and animation objects.
      *
      */

      void CMultiAnimate::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::Examples::CMultiAnimate::HandleAttributes")

         CAttributeHandler::HandleAttributes(attr);

         // Give clump a look
         m_pClump.HandleSystemCommands(attr);

         // Now give anim set a look
         m_animSet.HandleSystemCommands(attr);

         CAttributeCommandIterator SysattrIt(attr, RWS_CLASSID_OF(CSystemCommands));

         while (!SysattrIt.IsFinished())
         {
            // Intercept CMD_AttachResource's only
            if ( CMD_AttachResource == SysattrIt->GetCommandId() )
            {
               // Next anim, if possible
               if (m_animSet.HasTarget() && m_animSet.GetAnimationCount() )
               {
                  m_animSet.SetDstAnimation(m_animSet.GetAnimationCount() - 1);
               }
            }

            // Next section
            ++SysattrIt;
         }

         // Handle CMultiAnimate commands
         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(CMultiAnimate));
         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
               //////////////////////////////////////////////////////////////////
            case CMD_MsgNextAnimTrigger:
               //////////////////////////////////////////////////////////////////
               {
                  const RwChar *name;

                  attrIt->GetCommandData(&name);

                  UnLinkMsg(m_msgNextAnimTrigger);
                  UnRegisterMsg(m_msgNextAnimTrigger);
                  RegisterMsg(m_msgNextAnimTrigger, name, 0);
                  LinkMsg(m_msgNextAnimTrigger, 0);
               }
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_Rate:
               //////////////////////////////////////////////////////////////////
               {
                  RwReal rate;

                  attrIt->GetCommandData(rate);

                  SetRate(rate);
               }
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_BlendDuration:
               //////////////////////////////////////////////////////////////////
               {
                  RwReal blend;

                  attrIt->GetCommandData(blend);

                  m_animSet.SetBlendDuration(blend);
               }
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_Loop:
               //////////////////////////////////////////////////////////////////
               {
                  RwUInt32 loopFlag;

                  attrIt->GetCommandData(loopFlag);
               
                  SetLooping(0 != loopFlag);
               }
               break;
            }
            ++attrIt;
         }

         RWS_RETURNVOID()
      }

      /*
      *
      *  Sets the rate multiplier for all animations.
      *
      */

      void CMultiAnimate::SetRate( RwReal rate )
      {
         RWS_FUNCTION("RWS::Examples::CMultiAnimate::SetRate")

         for (RwUInt32 i = 0; i < m_animSet.GetAnimationCount(); ++i )
         {
            m_animSet.SetRateMultiplier( i, rate );
         }

         RWS_RETURNVOID()
      }

      /*
      *
      *  Enables / disables looping for all animations.
      *
      */

      void CMultiAnimate::SetLooping( bool doLoop )
      {
         RWS_FUNCTION("RWS::Examples::CMultiAnimate::SetLooping")

         const CAnimSet::EndBehavior behavior = doLoop ? CAnimSet::LOOP : CAnimSet::HOLD;
         for (RwUInt32 i = 0; i < m_animSet.GetAnimationCount(); ++i )
         {
            m_animSet.SetEndBehavior(i, behavior);
         }

         RWS_RETURNVOID()
      }

      /**
      *
      * Mandatory CEventHandler interface - handles the incoming messages for the update of the animations
      * and the switching of animations (blending, etc).
      *
      */

      void CMultiAnimate::HandleEvents(CMsg &msg)
      {
         RWS_FUNCTION("RWS::Examples::CMultiAnimate::HandleEvents")

         if (m_msgNextAnimTrigger == msg.Id )
         {
            // Next anim, if possible
            const RwUInt32 animCount = m_animSet.GetAnimationCount();
            if (m_animSet.HasTarget() && animCount )
            {
               RwUInt32 nextAnim = 0;
               nextAnim = (m_animSet.GetDstAnimation() + 1) % animCount;
               m_animSet.SetDstAnimation(nextAnim);
            }
         }
         else if (iMsgRunningTick == msg.Id )
         {
            if (!m_animSet.Update(kFrameTime))
            {
               // Anim Complete
               //
//               RWS_TRACE("Animation Complete");
            }

            if (m_animSet.IsAnimating())
            {
//               RWS_TRACE("IsAnimating True");
            }
            else
            {
//               RWS_TRACE("IsAnimating False");
            }
         }

         RWS_RETURNVOID()
      }
   }//namespace Examples
}//namespace RWS
   