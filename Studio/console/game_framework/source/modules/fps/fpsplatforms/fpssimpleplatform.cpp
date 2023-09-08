/*****************************************************************************
*
* File :      FPSPlatform.cpp
*
* Abstract :
*
*  Implementation of a basic moving platform.  To move the platform you specify a maximum offset vector from the players
*  current position.  There are too modes: 
*           ONCE,  where the platform moves to its target and returns,  then stops.
*           LOOP,  where the platform moved to its target and returns,  and then repeats
*   
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
// Left to its own devices, the MW linker will dead-strip the registrar ctors, so...
#ifdef __MWERKS__
#pragma force_active on
#pragma exceptions off // Turn off exceptions due to bug in MW Dwarf Generator for PS2, present in CW 3.04
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "fpssimpleplatform.h"
#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/math/maths.h"
#include "framework/toolkits/world/factory.h"

namespace RWS
{
   namespace FPS
   {
      RWS_IMPLEMENT_CLASSID(FPSSimplePlatform);
      RWS_REGISTERCLASS(FPSSimplePlatform);
      
      /**
      *
      * \ref FPSSimplePlatform(). Create FPSSimplePlatform object.
      *
      * \see ~FPSSimplePlatform
      *
      */
      FPSSimplePlatform::FPSSimplePlatform(const CAttributePacket& attr)
         : InitCEventHandler(&m_pAtomic),
         m_playstate (LOOP)
      {
         RWS_FUNCTION("RWS::FPS::FPSSimplePlatform::FPSSimplePlatform");
         
         m_pAtomic = CreateAtomicInWorldFromResource(attr, this);
         RWS_ASSERT(m_pAtomic, "Failed to create atomic");
         
         m_mat = RwMatrixCreate();
         RWS_POST(m_mat);
         
         // Target max offset
         m_direction.x = 0.0f;
         m_direction.y = 0.0f;  
         m_direction.z = 0.0f;
         
         // current position along max offset
         m_attribDir.x = 0.0f;
         m_attribDir.y = 0.0f;
         m_attribDir.z = 0.0f;
         
         ResetPlatform();
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref ~FPSSimplePlatform(). Destroy FPSSimplePlatform object
      *
      * \see FPSSimplePlatform
      */
      FPSSimplePlatform::~FPSSimplePlatform(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSSimplePlatform::~FPSSimplePlatform");
         
         RWS_PRE(m_mat);
         RwMatrixDestroy(m_mat);
         m_mat = 0;
         UnLinkMsg(iMsgRunningTick);
         UnLinkMsg(m_trigger);
         UnRegisterMsg(m_trigger);
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref FPSSimplePlatform(). Reset platform to initial state
      *
      */
      void FPSSimplePlatform::ResetPlatform( void )
      {
         RWS_FUNCTION("RWS::FPS::FPSSimplePlatform::ResetPlatform");
         
         m_toX.x = 0.0f;
         m_toX.y = 0.0f;
         m_toX.z = 0.0f;
         
         m_toY.x = 0.0f;  
         m_toY.y = 0.0f;  
         m_toY.z = 0.0f;  
         
         m_toZ.x = 0.0f;
         m_toZ.y = 0.0f;
         m_toZ.z = 0.0f;
         
         m_actual.x = 0.0f;
         m_actual.y = 0.0f;
         m_actual.z = 0.0f;
         
         m_direction.x = m_attribDir.x;
         m_direction.y = m_attribDir.y;
         m_direction.z = m_attribDir.z;
         m_state    = START;
         
         m_dest[0]  = false;
         m_dest[1]  = false;
         m_dest[2]  = false;   
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref HandleEvents(). Handle events for platform.  Handles movement 
      * of platform on the game tick, and handle trigger for turning platform on and off.
      *
      * \param msg Standard Message Package
      *  
      * \ref iMsgRunningTick
      */
      void FPSSimplePlatform::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::FPS::FPSSimplePlatform::HandleEvents");
         
         RwFrame* pFrame = RpAtomicGetFrame(m_pAtomic.ptr());
         
         // If linked to the running tick
         if (pMsg.Id == iMsgRunningTick)
         {
            MovePlatform(pFrame);
         }
         // If received and event
         else if (pMsg.Id == m_trigger)
         {
            ToggleON_OFF();
         }
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref ToggleON_OFF(). Turn platform on or off
      *
      * \ref iMsgRunningTick
      */
      void FPSSimplePlatform::ToggleON_OFF( void )
      {
         RWS_FUNCTION("RWS::FPS::FPSSimplePlatform::ToggleON_OFF");
         
         // Toggle Running tick
         if (IsLinked(iMsgRunningTick) == false)
         {
            LinkMsg(iMsgRunningTick);
            
            // if platform stopped, reset and start
            if (m_state == STOP)
            {
               ResetPlatform();   
               m_state = START;
            }
         }
         else
         {
            UnLinkMsg(iMsgRunningTick);
            
            // if platform stopped,  reset
            if (m_state == STOP)
            {
               ResetPlatform();   
            }
         }
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref FPSSimplePlatform. Move platform in direction of m_direction attribute.  
      * This function acts as a state machine for the platform handling start and destination states.
      *
      * \param RwFrame *frame: Frame of atomic to move.
      *
      * \see MovePlatform, ProcessPlatformX, ProcessPlatformY, ProcessPlatformZ, TransformPlatform
      *
      */
      void FPSSimplePlatform::MovePlatform( RwFrame *frame )
      {
         RWS_FUNCTION("RWS::FPS::FPSSimplePlatform::MovePlatform");
         
         switch (m_state)
         {
         case START:
            // All axis have reached target destination,  set new destinion to original starting position
            if ( (m_dest[0]) && (m_dest[1]) && (m_dest[2]) )
            {
               m_state = DESTINATION;
               
               m_direction.x = m_direction.x - m_direction.x;
               m_direction.y = m_direction.y - m_direction.y;
               m_direction.z = m_direction.z - m_direction.z;
               
               m_dest[0]  = false;
               m_dest[1]  = false;
               m_dest[2]  = false;
            }
            else
            {
               ProcessPlatformX(frame);
               ProcessPlatformY(frame);
               ProcessPlatformZ(frame);
            }
            break;
            
         case DESTINATION:
            // All axis have reached original
            if ( (m_dest[0]) && (m_dest[1]) && (m_dest[2]) )
            {
               // If LOOPED: Set new destinion to original starting position
               if (m_playstate == LOOP)
               {
                  m_state = START;
                  
                  m_direction.x = m_attribDir.x;
                  m_direction.y = m_attribDir.y;
                  m_direction.z = m_attribDir.z;
                  
                  m_dest[0]  = false;
                  m_dest[1]  = false;
                  m_dest[2]  = false;
               }
               
               // If PLAY ONCE: Stop platform and unlink from running tick
               if (m_playstate == ONCE)
               {
                  m_state = STOP;
                  UnLinkMsg(iMsgRunningTick);
               }
            }
            else
            {
               ProcessPlatformX(frame);
               ProcessPlatformY(frame);
               ProcessPlatformZ(frame);
            }
            break;
            
         case  STOP:
            break;
         }
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref ProcessPlatformX. Move platform on X axis
      *
      * \param RwFrame *frame: Frame of atomic to move
      *
      * \see MovePlatform, ProcessPlatformX, ProcessPlatformY, ProcessPlatformZ, TransformPlatform
      *
      */
      void FPSSimplePlatform::ProcessPlatformX(RwFrame *frame)
      {
         RWS_FUNCTION("RWS::FPS::FPSSimplePlatform::ProcessPlatformX");
         
         // If destination reached,  set flag
         if (m_actual.x ==  m_direction.x)
         {
            m_dest[0] = true;
         }
         // if platform not reached destination,  move platform  
         else if (m_actual.x <=  m_direction.x )
         {
            m_toX.x = 1.0f;
            m_actual.x = m_actual.x + 1.0f;
            
            TransformPlatform(frame, &m_toX);
         }
         else if (m_actual.x >=  m_direction.x )
         {
            m_toX.x = -1.0f;
            m_actual.x = m_actual.x - 1.0f;
            
            TransformPlatform(frame, &m_toX);
         }
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref ProcessPlatformY. mMove platform on Y axis
      *
      * \param RwFrame *frame: Frame of atomic to move
      *
      * \see MovePlatform, ProcessPlatformX, ProcessPlatformY, ProcessPlatformZ, TransformPlatform
      *
      */    
      void FPSSimplePlatform::ProcessPlatformY(RwFrame *frame)
      {
         RWS_FUNCTION("RWS::FPS::FPSSimplePlatform::ProcessPlatformY");
         
         // If destination reached,  set flag
         if (m_actual.y ==  m_direction.y)
         {
            m_dest[1] = true;
         }
         // if platform not reached destination,  move platform  
         else if (m_actual.y <=  m_direction.y )
         {
            m_toY.y = 1.0f;
            m_actual.y = m_actual.y + 1.0f;
            
            TransformPlatform(frame, &m_toY);
         }
         else if (m_actual.y >  m_direction.y )
         {
            m_toY.y = -1.0f;
            m_actual.y = m_actual.y - 1.0f;
            
            TransformPlatform(frame, &m_toY);
         }
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref ProcessPlatformZ. Move platform on Zaxis
      *
      * \param RwFrame *frame: Frame of atomic to move
      *
      * \see MovePlatform, ProcessPlatformX, ProcessPlatformY, ProcessPlatformZ, TransformPlatform
      *
      */        
      void FPSSimplePlatform::ProcessPlatformZ(RwFrame *frame)
      {
         RWS_FUNCTION("RWS::FPS::FPSSimplePlatform::ProcessPlatformZ");
         
         // If destination reached,  set flag
         if (m_actual.z ==  m_direction.z)
         {
            m_dest[2] = true;
         }
         // if platform not reached destination,  move platform  
         else if (m_actual.z <=  m_direction.z )
         {
            m_toZ.z = 1.0f;
            m_actual.z = m_actual.z + 1.0f;
            
            TransformPlatform(frame, &m_toZ);
         }
         else if (m_actual.z >  m_direction.z )
         {
            m_toZ.z = -1.0f;
            m_actual.z = m_actual.z - 1.0f;
            
            TransformPlatform(frame, &m_toZ);
         }
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref TransformPlatform: Apply Transformation to platform
      *
      * \param RwFrame *frame: Frame of atomic to move.
      * \param RwV3d *vec: Vector to transform by.
      *
      * \see MovePlatform, ProcessPlatformX, ProcessPlatformY, ProcessPlatformZ, TransformPlatform
      *
      */
      void FPSSimplePlatform::TransformPlatform( RwFrame *frame, RwV3d *vec )
      {
         RWS_FUNCTION("RWS::FPS::FPSSimplePlatform::TransformPlatform");
         
         RwMatrixTranslate (m_mat, vec, rwCOMBINEREPLACE);
         RwFrameTransform(frame, m_mat, rwCOMBINEPRECONCAT);
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      *  \ref HandleAttributes.  HandleAttributes
      *
      *  \param attr Standard Attribute Package
      *  
      */
      void FPSSimplePlatform::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::FPS::FPSSimplePlatform::HandleAttributes");
         
         CAttributeHandler::HandleAttributes(attr);
         m_pAtomic.HandleSystemCommands(attr);
         
         RespondToWorkspace(attr);
         RespondToAttributes(attr);
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref RespondToWorkspace. Handle system commands.  if atomic reposition in workspace,  reset platform
      *
      * \param attr Standard Attribute Package
      *
      */
      void FPSSimplePlatform::RespondToWorkspace( const CAttributePacket& attr )
      {
         CAttributeCommandIterator attrItb(attr, RWS_CLASSID_OF(CSystemCommands));
         
         while (!attrItb.IsFinished())
         {
            switch (attrItb->GetCommandId())
            {
            case CSystemCommands::CMD_LoadMatrix:
               {
                  ResetPlatform();
               }
               break;
            }
            ++attrItb;
         }
      }
      
      
      /**
      *
      * \ref RespondToAttributes. Handle standard attributes form
      *
      * \param attr Standard Attribute Package
      *
      */
      void FPSSimplePlatform::RespondToAttributes( const CAttributePacket& attr )
      {
         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(FPSSimplePlatform));
         
         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
            case CMD_direction:
               attrIt->GetCommandData(m_attribDir);
               
               m_direction.x = m_attribDir.x;
               m_direction.y = m_attribDir.y;
               m_direction.z = m_attribDir.z;
               break;
               
            case CMD_m_trigger:
               ReplaceLinkedMsg ( m_trigger, attrIt->GetAs_RwChar_ptr(), 0);
               break;
               
            case CMD_State:
               RwUInt32 type;
               
               attrIt->GetCommandData(type);
               
               switch (type)
               {
               case 0: 
                  m_playstate = LOOP; 
                  break;
                  
               case 1: 
                  m_playstate = ONCE; 
                  break;
                  
               default:
                  RWS_ASSERTFAIL("FPSSimplePlatform::HandleAttributes illegal parameter value.");
                  break;
               }
               break;
            }
            
            ++attrIt;
         }
      }
      
   }//namespace FPS
}//namespace RWS
