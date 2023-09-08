/*****************************************************************************
*
* File :     FPSToggleInterface.h
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


#ifndef __FPS_TOGGLE_INTERFACE_H__
#define __FPS_TOGGLE_INTERFACE_H__

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/core/eventhandler/ceventhandler.h"
#include "framework/toolkits/world/smartptrs/catomicptr.h"
#include "framework/mainloop/logic.h"

namespace RWS
{
   namespace FPS
   {
      /**  
      *
      *  \ingroup Mod_FPS
      *
      *  Behavior implementation class. The FPSToggleInterface behavior is a 3D object which moves between
      *  two points in response to some event.
      *
      */
      class FPSToggleInterface :  public CSystemCommands, public CAttributeHandler , public CEventHandler
      {
      public:
         virtual void HandleEvents(CMsg &pMsg);
         virtual void HandleAttributes(const CAttributePacket& attr);
         
         FPSToggleInterface(const CAttributePacket& attr);
         virtual ~FPSToggleInterface(void);
         
      protected:
         
         /** Define entity states */
         enum State                      
         {
            State_off,
               State_off_to_on,
               State_off_to_on_reverse,
               State_on,
               State_on_to_off,
               State_on_to_off_reverse
         };
         
         /**< Generate the transformation matrices  */
         void GenerateTransformationMatrices(RwV3d &m_velocity, RwV3d &m_angvelocity);
         
         CAtomicPtr m_pAtomic;
         
         
         // Forward direction, state on to off events
         //
         /* Wait for this event */
         void Set_m_msg_rx_off_to_on(const RwChar *p);
         void Set_m_msg_rx_off_to_on(const CEventId &Id);
         
         /* Then send this event */
         void Set_m_msg_tx_off_to_on(const RwChar *p);
         void Set_m_msg_tx_off_to_on(const CEventId &Id);
         
         /* If this event, cancel motion */
         void Set_m_msg_rx_off_to_on_cancel(const RwChar *p);
         void Set_m_msg_rx_off_to_on_cancel(const CEventId &Id);
         
         /* Have completed the forward motion */
         void Set_m_msg_tx_finished_off_to_on(const RwChar *p);
         void Set_m_msg_tx_finished_off_to_on(const CEventId &Id);
         
         
         // Reverse direction, state off to on events
         //
         /* Wait for this event */
         void Set_m_msg_rx_on_to_off(const RwChar *p);
         void Set_m_msg_rx_on_to_off(const CEventId &Id);
         
         /* Then send this event */
         void Set_m_msg_tx_on_to_off(const RwChar *p);
         void Set_m_msg_tx_on_to_off(const CEventId &Id);
         
         /* If this event, cancel motion */
         void Set_m_msg_rx_on_to_off_cancel(const RwChar *p);
         void Set_m_msg_rx_on_to_off_cancel(const CEventId &Id);
         
         /**< Have completed the reverse motion */
         void Set_m_msg_tx_finished_on_to_off(const RwChar *p);
         void Set_m_msg_tx_finished_on_to_off(const CEventId &Id);
         
         
         // State _ Proc _ Message functions
         //
         //
         // State Off
         //            
         void State_off_Proc_m_msg_rx(void);                         /**< Trigger (off) Event */
         void State_off_to_on_Proc_MsgRunningTick(void);             /**< Moving from (off)->(on) */
         void State_off_to_on_Proc_m_msg_rx_off_to_on_cancel(void);  /**< Request Cancel (off)->(on) */
         void State_off_to_on_reverse_Proc_MsgRunningTick(void);     /**< Cancel Moving from (off)->(on) */
         
         
         // State On
         //
         void State_on_Proc_m_msg_rx(void);                          /**< Trigger (on) Event */
         void State_on_to_off_Proc_MsgRunningTick(void);             /**< Moving from (on)->(off) */
         void State_on_to_off_proc_m_msg_rx_on_to_off_cancel(void);  /**< Request Cancel (on)->(off) */
         void State_on_to_off_reverse_Proc_MsgRunningTick(void);     /**< Cancel Moving from (on)->(off) */         
         
         RwUInt32 m_num_msg_rx_off_to_on;                            /**< Number of messages off to on required to trigger */
         RwUInt32 m_num_msg_rx_on_to_off;                            /**< Number of messages on to off required to trigger */
         
         // Note changing m_frame_count_off_to_on requires the transformation matrices to be re-calculated 
         // i.e. GenerateTransformationMatrices
         //
         void Set_m_frame_count_off_to_on(RwUInt32 var) {m_frame_count_off_to_on = var;}
         
         // Note changing m_frame_count_on_to_off requires the transformation matrices to be re-calculated 
         // i.e. GenerateTransformationMatrices
         //
         void Set_m_frame_count_on_to_off(RwUInt32 var) {m_frame_count_on_to_off = var;}
         
         RwUInt32 Get_m_state() {return m_state;}
         
         CEventId &Get_m_msg_rx_off_to_on() {return m_msg_rx_off_to_on;}
         CEventId &Get_m_msg_tx_off_to_on() {return m_msg_tx_off_to_on;}
         CEventId &Get_m_msg_rx_off_to_on_cancel() {return m_msg_rx_off_to_on_cancel;}
         CEventId &Get_m_msg_tx_finished_on_to_off() {return m_msg_tx_finished_on_to_off;}
         CEventId &Get_m_msg_rx_on_to_off() {return m_msg_rx_on_to_off;}
         CEventId &Get_m_msg_tx_on_to_off() {return m_msg_tx_on_to_off;}
         CEventId &Get_m_msg_rx_on_to_off_cancel() {return m_msg_rx_on_to_off_cancel;}
         CEventId &Get_m_msg_tx_finished_off_to_on() {return m_msg_tx_finished_off_to_on;}
         
      private:
         // Note: If m_frame_count_off_to_on or m_frame_count_on_to_off are modified then the matrices will need to 
         // be regenerated, see GenerateTransformationMatrices
         //
         RwUInt32 m_frame_count_off_to_on;     /**< Number of frames the object will move for off to on */
         RwUInt32 m_frame_count_on_to_off;     /**< Number of frames the object will move for off to on */
         RwUInt32 m_state;                     /**< Current entity state */
         RwUInt32 m_num_msgs;                  /**< Counter to maintain number of rx messages received */
         
         CEventId m_msg_rx_off_to_on;          /**< Receive Message off to on */
         CEventId m_msg_tx_off_to_on;          /**< Transmit Message off to on */
         CEventId m_msg_rx_off_to_on_cancel;   /**< Receive Message off to on Reverse Event */    
         CEventId m_msg_tx_finished_on_to_off; /**< Message to send when reached off state from on state */
         CEventId m_msg_rx_on_to_off;          /**< Receive Message on to off */
         CEventId m_msg_tx_on_to_off;          /**< Transmit Message on to off */
         CEventId m_msg_rx_on_to_off_cancel;   /**< Receive Message off to on Reverse Event */
         CEventId m_msg_tx_finished_off_to_on; /**< Message to send when reached on state from off state */
         
         RwMatrix *m_matrix;                   /**< Transformation matrix */
         RwMatrix *m_matrix_rev;               /**< Transformation matrix, reverse */
         RwMatrix *m_imatrix;                  /**< Inverse Transformation matrix */
         RwMatrix *m_imatrix_rev;              /**< Inverse Transformation matrix, reverse */
         
         RwUInt32 m_counter;                   /**< Counter used when applying the transformation matrix m_matrix */
      };
   }//namespace FPS
}//namespace RWS
#endif