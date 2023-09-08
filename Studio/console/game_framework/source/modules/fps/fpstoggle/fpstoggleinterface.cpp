/*****************************************************************************
*
* File :     FPSToggleInterface.cpp
*
* Abstract : Behavior implementation class. The FPSToggleInterface behavior 
*            is a 3D object which moves between two points in response to 
*            some event.
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
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "fpstoggleinterface.h"
#include "framework/toolkits/world/factory.h"
#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/math/maths.h"

namespace RWS
{
   namespace FPS
   { 
      /**
      *
      *  Constructor for FPSTriggerCounter
      *      
      */
      FPSToggleInterface::FPSToggleInterface(const CAttributePacket& attr) :
                                             InitCEventHandler(&m_pAtomic),
                                             m_num_msg_rx_off_to_on(0),
                                             m_num_msg_rx_on_to_off(0),
                                             m_frame_count_off_to_on(RWS::MainLoop::Logic::Rate),
                                             m_frame_count_on_to_off(RWS::MainLoop::Logic::Rate),
                                             m_state(State_off),
                                             m_num_msgs(0),
                                             m_counter(0)
      {
         RWS_FUNCTION("RWS::FPS::FPSToggleInterface::FPSToggleInterface");
      
         // Can't use initializer ('this' isn't valid) so...
         m_pAtomic = CreateAtomicInWorldFromResource(attr, this);
         RWS_ASSERT(m_pAtomic, "Failed to create atomic");
      
         // Create the matrices that will be used to define the movement of this object
         m_matrix = RwMatrixCreate();
         m_matrix_rev = RwMatrixCreate();
         m_imatrix = RwMatrixCreate();
         m_imatrix_rev = RwMatrixCreate();
      
         RWS_RETURNVOID();
      }
   
   
      /**
      *
      * Destructor for FPSTriggerCounter
      *
      */
      FPSToggleInterface::~FPSToggleInterface(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSToggleInterface::~FPSToggleInterface");
      
         // Need to unlink from any events that this CEventHandler may have linked to
      
         UnLinkMsg(m_msg_rx_off_to_on);
         UnLinkMsg(m_msg_rx_off_to_on_cancel);
         UnLinkMsg(m_msg_rx_on_to_off);
         UnLinkMsg(m_msg_rx_on_to_off_cancel);
         UnLinkMsg(m_msg_tx_finished_on_to_off);
         UnLinkMsg(m_msg_tx_finished_off_to_on);
      
         UnLinkMsg(iMsgRunningTick);
      
         // Need to unregister any events that this CEventHandler may have registered
         UnRegisterMsg( m_msg_rx_off_to_on );            // Receive Message off to on
         UnRegisterMsg( m_msg_tx_off_to_on );            // Transmit Message off to on
         UnRegisterMsg( m_msg_rx_off_to_on_cancel );     // Receive Message off to on Reverse Event
         UnRegisterMsg( m_msg_tx_finished_on_to_off );   // Message to send when reached off state from on state
         UnRegisterMsg( m_msg_rx_on_to_off );            // Receive Message on to off
         UnRegisterMsg( m_msg_tx_on_to_off );            // Transmit Message on to off
         UnRegisterMsg( m_msg_rx_on_to_off_cancel );     // Receive Message off to on Reverse Event
         UnRegisterMsg( m_msg_tx_finished_off_to_on );   // Message to send when reached on state from off state
      
         RWS_RETURNVOID();
      }
   
   
      /**
      *
      *  Prepare switch to move to the on position
      *
      */
      void FPSToggleInterface::State_off_Proc_m_msg_rx(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSToggleInterface::State_off_Proc_m_msg_rx");
      
         m_num_msgs++;
      
         if (m_num_msgs >= m_num_msg_rx_off_to_on)
         {
            m_num_msgs = 0;
            m_state = State_off_to_on;
            UnLinkMsg(m_msg_rx_off_to_on);
            LinkMsg(iMsgRunningTick, 0);
            LinkMsg(m_msg_rx_off_to_on_cancel, 0);
            SendMsg(m_msg_tx_off_to_on);
         }
      
         RWS_RETURNVOID();
      }
   
   
      /**
      *
      * Move switch from the off position to the on position
      * 
      */        
      void FPSToggleInterface::State_off_to_on_Proc_MsgRunningTick(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSToggleInterface::State_off_to_on_Proc_MsgRunningTick");
      
         if (m_counter < m_frame_count_off_to_on)
         {
            // Update the frame
            RwFrame *frame = RpAtomicGetFrame(m_pAtomic.ptr());
            RwFrameTransform (frame, m_matrix, rwCOMBINEPRECONCAT);
            RwFrameUpdateObjects(frame);
         
            m_counter++;
         }
         else
         {
            m_counter = 0;
         
            m_state = State_on;
            UnLinkMsg(iMsgRunningTick);
            UnLinkMsg(m_msg_rx_off_to_on_cancel);
         
            // Send our pressed message
            SendMsg(m_msg_tx_finished_off_to_on); 
            LinkMsg(m_msg_rx_on_to_off, 0);
         }
         RWS_RETURNVOID();
      }
   
   
      /**
      *
      * Process m_msg_rx_off_to_on_cancel event while in state off_to_on
      *
      */        
      void FPSToggleInterface::State_off_to_on_Proc_m_msg_rx_off_to_on_cancel(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSToggleInterface::State_off_to_on_Proc_m_msg_rx_off_to_on_cancel");
      
         m_state = State_off_to_on_reverse;
         UnLinkMsg(m_msg_rx_off_to_on_cancel);
      
         RWS_RETURNVOID();
      }
   
   
      /**
      *
      * Cancel move switch from the off position to the on position
      *
      */
      void FPSToggleInterface::State_off_to_on_reverse_Proc_MsgRunningTick(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSToggleInterface::State_off_to_on_reverse_Proc_MsgRunningTick");
      
         if (m_counter > 0)
         {
            // Update the frame
            RwFrame *frame = RpAtomicGetFrame(m_pAtomic.ptr());
            RwFrameTransform (frame, m_matrix_rev, rwCOMBINEPRECONCAT);
            RwFrameUpdateObjects(frame);
         
            m_counter--;
         }
         else
         {
            m_state = State_off;
            UnLinkMsg(iMsgRunningTick);
            LinkMsg(m_msg_rx_off_to_on, 0);
         }
      
         RWS_RETURNVOID();
      }
   
   
      /**
      *
      * Prepare switch to move to the off position
      *
      */
      void FPSToggleInterface::State_on_Proc_m_msg_rx(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSToggleInterface::State_on_Proc_m_msg_rx");
      
         m_num_msgs++;
      
         if (m_num_msgs >= m_num_msg_rx_on_to_off)
         {
            m_num_msgs = 0;
            m_state = State_on_to_off;
            UnLinkMsg(m_msg_rx_on_to_off);
            LinkMsg(iMsgRunningTick, 0);
            LinkMsg(m_msg_rx_on_to_off_cancel, 0);
            SendMsg(m_msg_tx_on_to_off);
         }
      
         RWS_RETURNVOID();
      }
   
   
      /**
      *
      * Move switch from the on position to the off position
      *
      */
      void FPSToggleInterface::State_on_to_off_Proc_MsgRunningTick(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSToggleInterface::State_on_to_off_Proc_MsgRunningTick");
      
         if (m_counter < m_frame_count_on_to_off)
         {
            // Update the frame
            RwFrame *frame = RpAtomicGetFrame(m_pAtomic.ptr());
            RwFrameTransform (frame, m_imatrix, rwCOMBINEPRECONCAT);
            RwFrameUpdateObjects(frame);
         
            m_counter++;
         }
         else
         {
            m_counter = 0;
         
            m_state = State_off;
            UnLinkMsg(iMsgRunningTick);
            UnLinkMsg(m_msg_rx_on_to_off_cancel);
         
            // Send our pressed message
            SendMsg(m_msg_tx_finished_on_to_off); 
            LinkMsg(m_msg_rx_off_to_on, 0);
         }
      
         RWS_RETURNVOID();
      }
   
   
      /**
      *
      * Process m_msg_rx_on_to_off_cancel event while in state on_to_off
      *
      */        
      void FPSToggleInterface::State_on_to_off_proc_m_msg_rx_on_to_off_cancel(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSToggleInterface::State_on_to_off_proc_m_msg_rx_on_to_off_cancel");
      
         m_state = State_on_to_off_reverse;
         UnLinkMsg(m_msg_rx_on_to_off_cancel);
      
         RWS_RETURNVOID();
      }
   
   
      /**
      *
      * Cancel Move switch from the on position to the off position
      *
      */        
      void FPSToggleInterface::State_on_to_off_reverse_Proc_MsgRunningTick(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSToggleInterface::State_on_to_off_reverse_Proc_MsgRunningTick");
      
         if (m_counter > 0)
         {
            // Update the frame
            RwFrame *frame = RpAtomicGetFrame(m_pAtomic.ptr());
            RwFrameTransform (frame, m_imatrix_rev, rwCOMBINEPRECONCAT);
            RwFrameUpdateObjects(frame);
         
            m_counter--;
         }
         else
         {
            m_state = State_on;
            UnLinkMsg(iMsgRunningTick);
            LinkMsg(m_msg_rx_on_to_off, 0);
         }
      
         RWS_RETURNVOID();
      }
   
   
      /**
      *
      * Process messages based on current state
      *
      */
      void FPSToggleInterface::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::FPS::FPSToggleInterface::HandleEvents");
      
         switch (m_state)
         {
            // Wait
         case State_off:
            if (pMsg.Id == m_msg_rx_off_to_on) 
            {
               State_off_Proc_m_msg_rx();
            }
            break;
         
            // Move from off state to on state
         case State_off_to_on:
            if (pMsg.Id == m_msg_rx_off_to_on_cancel)
            {
               State_off_to_on_Proc_m_msg_rx_off_to_on_cancel();
            }
            else if (pMsg.Id == iMsgRunningTick)  
            {
               State_off_to_on_Proc_MsgRunningTick();
            }
            break;
         
            // Cancel moving from off state to on state
         case State_off_to_on_reverse:
            if (pMsg.Id == iMsgRunningTick) 
            {
               State_off_to_on_reverse_Proc_MsgRunningTick();
            }
            break;
         
            // Wait for event
         case State_on:
            if (pMsg.Id == m_msg_rx_on_to_off) 
            {
               State_on_Proc_m_msg_rx();
            }
            break;
         
            // Move from on state to off state
         case State_on_to_off:
            if (pMsg.Id == m_msg_rx_on_to_off_cancel)
            {
               State_on_to_off_proc_m_msg_rx_on_to_off_cancel();
            }
            else if (pMsg.Id == iMsgRunningTick)  
            {
               State_on_to_off_Proc_MsgRunningTick();
            }
            break;
         
            // Cancel moving from on state to off state                
         case State_on_to_off_reverse:
            if (pMsg.Id == iMsgRunningTick) 
            {
               State_on_to_off_reverse_Proc_MsgRunningTick();
            }
            break;
         }
      
         RWS_RETURNVOID();
      }
   
   
      /**
      *
      * Process messages from RenderWare Studio to set entity parameters
      *
      */    
      void FPSToggleInterface::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::FPS::FPSToggleInterface::HandleAttributes");
      
         CAttributeHandler::HandleAttributes(attr);
      
         m_pAtomic.HandleSystemCommands(attr);
      
         RWS_RETURNVOID();
      }
   
   
      /**
      *
      * Generate forward and reverse transformation matrices given a translation and rotation destination 
      * vector, i.e. if the translation is 0,25,0 then the transformation will move the object 0,25,0 
      * in m_frame_count_off_to_on frames
      *
      */        
      void FPSToggleInterface::GenerateTransformationMatrices(RwV3d &m_velocity, RwV3d &m_angvelocity)
      {
         RWS_FUNCTION("RWS::FPS::FPSToggleInterface::GenerateTransformationMatrices");
      
         RwV3d velocity;
      
         RwUInt32 _frame_count_off_to_on = m_frame_count_off_to_on;
         RwUInt32 _frame_count_on_to_off = m_frame_count_on_to_off;
      
         if (_frame_count_off_to_on == 0) _frame_count_off_to_on = 1;
         if (_frame_count_on_to_off == 0) _frame_count_on_to_off = 1;
      
         // Calculate forward transform, off to on
         //
         RwV3dScale(&velocity, &m_velocity, 1.0f / _frame_count_off_to_on);

         RwMatrixTranslate (m_matrix, &velocity, rwCOMBINEREPLACE);
      
         RwMatrixRotate (m_matrix, &YAxis, m_angvelocity.y / _frame_count_off_to_on, rwCOMBINEPRECONCAT);
         RwMatrixRotate (m_matrix, &ZAxis, m_angvelocity.z / _frame_count_off_to_on, rwCOMBINEPRECONCAT);
         RwMatrixRotate (m_matrix, &XAxis, m_angvelocity.x / _frame_count_off_to_on, rwCOMBINEPRECONCAT);
      
         // Calculate reverse transform, on to off
         //
         RwV3dScale(&velocity, &m_velocity, -1.0f / _frame_count_on_to_off);
         RwMatrixTranslate (m_imatrix, &velocity, rwCOMBINEREPLACE);
      
         RwMatrixRotate (m_imatrix, &YAxis, -m_angvelocity.y / _frame_count_on_to_off, rwCOMBINEPRECONCAT);
         RwMatrixRotate (m_imatrix, &ZAxis, -m_angvelocity.z / _frame_count_on_to_off, rwCOMBINEPRECONCAT);
         RwMatrixRotate (m_imatrix, &XAxis, -m_angvelocity.x / _frame_count_on_to_off, rwCOMBINEPRECONCAT);
      
         // Calculate, backup matrices
         //
         RwMatrixInvert (m_matrix_rev, m_matrix);
         RwMatrixInvert (m_imatrix_rev, m_imatrix);
      
         RWS_RETURNVOID();
      }
   
   
      /**
      *
      * Specify the name of the (m_msg_rx_off_to_on) event. FPSToggleInterface waits until it has received 
      * (m_num_msg_rx_off_to_on) of these events, when that occurs it uses the forward 
      * transform to move the object from the off to the on position.
      *
      */
      void FPSToggleInterface::Set_m_msg_rx_off_to_on(const RwChar *p)
      {
         RWS_FUNCTION("RWS::FPS::FPSToggleInterface::Set_m_msg_rx_off_to_on");
      
         UnLinkMsg (m_msg_rx_off_to_on);
         UnRegisterMsg(m_msg_rx_off_to_on);
         RegisterMsg(m_msg_rx_off_to_on, p, 0);
      
         if (m_state == State_off) 
            LinkMsg(m_msg_rx_off_to_on, 0);
      
         RWS_RETURNVOID();
      }
   
   
      /**
      *
      * Specify the event to use as the (m_msg_rx_off_to_on) event,
      * see Set_m_msg_rx_off_to_on(const RwChar *p)
      *         
      */        
      void FPSToggleInterface::Set_m_msg_rx_off_to_on(const CEventId &Id)
      {
         RWS_FUNCTION("RWS::FPS::FPSToggleInterface::Set_m_msg_rx_off_to_on");
      
         UnLinkMsg (m_msg_rx_off_to_on);
         UnRegisterMsg(m_msg_rx_off_to_on);
         m_msg_rx_off_to_on = Id;
      
         if (m_state == State_off) 
            LinkMsg(m_msg_rx_off_to_on, 0);
      
         RWS_RETURNVOID();
      }
   
   
      /**
      *
      * Specify the name of the (m_msg_tx_off_to_on event).FPSToggleInterface waits until it has received 
      * (m_num_msg_rx_off_to_on) of these events, when that occurs it uses the forward transform to move 
      * the object from the off to the on position. It also sends the (m_msg_tx_off_to_on) event as it
      * begins the transform.
      *
      */
      void FPSToggleInterface::Set_m_msg_tx_off_to_on(const RwChar *p)
      {
         RWS_FUNCTION("RWS::FPS::FPSToggleInterface::Set_m_msg_tx_off_to_on");
         UnRegisterMsg(m_msg_tx_off_to_on);
         RegisterMsg(m_msg_tx_off_to_on, p, 0);
         RWS_RETURNVOID();
      }
   
   
      /**
      *
      * Specify the name of the (m_msg_tx_finished_off_to_on) event. FPSToggleInterface waits until it has
      * received (m_num_msg_rx_on_to_off) of these events, when that occurs it uses the reverse transform 
      * to move the object from the on to the off position. It also sends the (m_msg_tx_on_to_off) event 
      * as it begins the transform.
      */
      void FPSToggleInterface::Set_m_msg_tx_finished_off_to_on(const RwChar *p)
      {
         RWS_FUNCTION("RWS::FPS::FPSToggleInterface::Set_m_msg_tx_finished_off_to_on");
         UnRegisterMsg(m_msg_tx_finished_off_to_on);
         RegisterMsg(m_msg_tx_finished_off_to_on, p, 0);
         RWS_RETURNVOID();
      }
   
   
      /**
      *
      * Specify the event to use as the (m_msg_tx_finished_off_to_on) event,
      * see Set_m_msg_tx_finished_off_to_on(const RwChar *p)
      * 
      */
      void FPSToggleInterface::Set_m_msg_tx_finished_off_to_on(const CEventId &Id)
      {
         RWS_FUNCTION("RWS::FPS::FPSToggleInterface::Set_m_msg_tx_finished_off_to_on");
         UnRegisterMsg(m_msg_tx_finished_off_to_on);
         m_msg_tx_finished_off_to_on = Id;
         RWS_RETURNVOID();
      }
   
   
      /**
      *
      * Specify the event to use as the (m_msg_tx_off_to_on) event,
      * see Set_m_msg_tx_off_to_on(const RwChar *p)
      * 
      */
      void FPSToggleInterface::Set_m_msg_tx_off_to_on(const CEventId &Id)
      {
         RWS_FUNCTION("RWS::FPS::FPSToggleInterface::Set_m_msg_tx_off_to_on");
         UnRegisterMsg(m_msg_tx_off_to_on);
         m_msg_tx_off_to_on = Id;
         RWS_RETURNVOID();
      }
   
   
      /**
      *
      * Specify the name of the (m_msg_rx_off_to_on_cancel) event. Once the object has started 
      * the movement from the off to the on position if this event is received the motion is 
      * canceled and the object returns to the off position.
      *
      */
      void FPSToggleInterface::Set_m_msg_rx_off_to_on_cancel(const RwChar *p)
      {
         RWS_FUNCTION("RWS::FPS::FPSToggleInterface::Set_m_msg_rx_off_to_on_cancel");
      
         UnLinkMsg (m_msg_rx_off_to_on_cancel);
         UnRegisterMsg(m_msg_rx_off_to_on_cancel);
         RegisterMsg(m_msg_rx_off_to_on_cancel, p, 0);
      
         if (m_state == State_off_to_on) 
            LinkMsg(m_msg_rx_off_to_on_cancel, 0);
      
         RWS_RETURNVOID();
      }
   
   
      /**
      *
      * Specify the event to use as the (m_msg_rx_off_to_on_cancel) event
      * see Set_m_msg_rx_off_to_on_cancel(const RwChar *p)
      *
      */
      void FPSToggleInterface::Set_m_msg_rx_off_to_on_cancel(const CEventId &Id)
      {
         RWS_FUNCTION("RWS::FPS::FPSToggleInterface::Set_m_msg_rx_off_to_on_cancel");
      
         UnLinkMsg (m_msg_rx_off_to_on_cancel);
         UnRegisterMsg(m_msg_rx_off_to_on_cancel);
         m_msg_rx_off_to_on_cancel = Id;
      
         if (m_state == State_off_to_on)
            LinkMsg(m_msg_rx_off_to_on_cancel, 0);
      
         RWS_RETURNVOID();
      }
   
   
      /**
      *
      * Specify the name of the (m_msg_rx_on_to_off) event. FPSToggleInterface waits until it has 
      * received (m_num_msg_rx_on_to_off) of these events, when that occurs it uses the reverse 
      * transform to move the object from the on to the off position.
      *
      */
      void FPSToggleInterface::Set_m_msg_rx_on_to_off(const RwChar *p)
      {
         RWS_FUNCTION("RWS::FPS::FPSToggleInterface::Set_m_msg_rx_on_to_off");
      
         UnLinkMsg(m_msg_rx_on_to_off);
         UnRegisterMsg(m_msg_rx_on_to_off);
         RegisterMsg(m_msg_rx_on_to_off, p, 0);
      
         if (m_state == State_on)
            LinkMsg(m_msg_rx_on_to_off, 0);
      
         RWS_RETURNVOID();
      }
   
   
      /**
      *
      * Specify the event to use as the (m_msg_rx_on_to_off) event,
      * see Set_m_msg_rx_on_to_off(const RwChar *p)
      * 
      */        
      void FPSToggleInterface::Set_m_msg_rx_on_to_off(const CEventId &Id)
      {
         RWS_FUNCTION("RWS::FPS::FPSToggleInterface::Set_m_msg_rx_on_to_off");
      
         UnLinkMsg(m_msg_rx_on_to_off);
         UnRegisterMsg(m_msg_rx_on_to_off);
         m_msg_rx_on_to_off = Id;
      
         if (m_state == State_on) 
            LinkMsg(m_msg_rx_on_to_off, 0);
      
         RWS_RETURNVOID();
      }
   
   
      void FPSToggleInterface::Set_m_msg_tx_on_to_off(const RwChar *p)
      {
         RWS_FUNCTION("RWS::FPS::FPSToggleInterface::Set_m_msg_rx_on_to_off");
         UnRegisterMsg(m_msg_tx_on_to_off);
         RegisterMsg(m_msg_tx_on_to_off, p, 0);
         RWS_RETURNVOID();
      }
   
   
      void FPSToggleInterface::Set_m_msg_tx_on_to_off(const CEventId &Id)
      {
         RWS_FUNCTION("RWS::FPS::FPSToggleInterface::Set_m_msg_rx_on_to_off");
         UnRegisterMsg(m_msg_tx_on_to_off);
         m_msg_tx_on_to_off = Id;
         RWS_RETURNVOID();
      }
   
   
      void FPSToggleInterface::Set_m_msg_rx_on_to_off_cancel(const RwChar *p)
      {
         RWS_FUNCTION("RWS::FPS::FPSToggleInterface::Set_m_msg_rx_on_to_off_cancel");
      
         UnLinkMsg(m_msg_rx_on_to_off_cancel);
         UnRegisterMsg(m_msg_rx_on_to_off_cancel);
         RegisterMsg(m_msg_rx_on_to_off_cancel, p, 0);
      
         if (m_state == State_on_to_off)
            LinkMsg(m_msg_rx_on_to_off_cancel, 0);
      
         RWS_RETURNVOID();
      }
   
   
      void FPSToggleInterface::Set_m_msg_rx_on_to_off_cancel(const CEventId &Id)
      {
         RWS_FUNCTION("RWS::FPS::FPSToggleInterface::Set_m_msg_rx_on_to_off_cancel");
      
         UnLinkMsg(m_msg_rx_on_to_off_cancel);
         UnRegisterMsg(m_msg_rx_on_to_off_cancel);
         m_msg_rx_on_to_off_cancel = Id;
      
         if (m_state == State_on_to_off) 
            LinkMsg(m_msg_rx_on_to_off_cancel, 0);
      
         RWS_RETURNVOID();
      }
   
   
      void FPSToggleInterface::Set_m_msg_tx_finished_on_to_off(const RwChar *p)
      {
         RWS_FUNCTION("RWS::FPS::FPSToggleInterface::Set_m_msg_rx_on_to_off_cancel");
         UnRegisterMsg(m_msg_tx_finished_on_to_off);
         RegisterMsg(m_msg_tx_finished_on_to_off, p, 0);
         RWS_RETURNVOID();
      }
   
   
      void FPSToggleInterface::Set_m_msg_tx_finished_on_to_off(const CEventId &Id)
      {
         RWS_FUNCTION("RWS::FPS::FPSToggleInterface::Set_m_msg_tx_finished_on_to_off");
         UnRegisterMsg(m_msg_tx_finished_on_to_off);
         m_msg_tx_finished_on_to_off = Id;
         RWS_RETURNVOID();
      }
   
   }//namespace FPS
}//namespace RWS

