/*****************************************************************************
*
* File :       FPSSimplePlatform.h
*
* Abstract :
*
*  Implementation of a basic moving platform.  To move the platform you specify a maximum offset vector from the players
*  current position.  There are two modes: 
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
#ifndef _FPS_SIMPLE_PLATFORM_H_
#define _FPS_SIMPLE_PLATFORM_H_


#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/core/eventhandler/ceventhandler.h"

#include "framework/toolkits/world/smartptrs/catomicptr.h"
#include "framework/mainloop/logic.h"
#include "framework/core/attributehandler/allocation policies/LinearAllocationPolicy.h"

namespace RWS
{
   namespace FPS
   {
      /**
      *
      *  \ingroup Mod_FPS
      *
      *  Implementation of a basic moving platform.  To move the platform you specify a maximum offset vector from the
      *  current position.  There are two modes: 
      *           ONCE,  where the platform moves to its target and returns,  then stops.
      *           LOOP,  where the platform moved to its target and returns,  and then repeats
      *
      *
      *  \see CAttributeHandler, CEventHandler, CSystemCommands
      *
      */
      class FPSSimplePlatform : public CSystemCommands, public CAttributeHandler , public CEventHandler, public LinearAllocationPolicy
      {
      public:
         RWS_MAKENEWCLASS(FPSSimplePlatform);
         RWS_DECLARE_CLASSID(FPSSimplePlatform);
         RWS_CATEGORY("FPS");
         RWS_DESCRIPTION("Alternate Platform", "A Platform that's movement is defined by a direction vector.");
         
         RWS_BEGIN_COMMANDS
            RWS_MESSAGE  ( CMD_m_trigger, "Trigger Event", "Event used to start/stop the platform moving", RECEIVE, 0, 0)
            RWS_ATTRIBUTE( CMD_direction, "Max Offset"    , "Determine the max offset of the platform from its original position"    , VECTOR,RwV3d,RANGES((-1000.0, 0.0, 1000.0),(-1000, 1, 1000),(-1000, 1, 1000)))
            RWS_ATTRIBUTE( CMD_State,     "Play State",    "Set playing state,  ONCE, LOOP"             ,   LIST,    RwUInt32,   LIST("LOOP|ONCE"))
            
            RWS_END_COMMANDS;
         
         enum STATE { START, DESTINATION, STOP };
         enum PLAYSTATE { ONCE, LOOP };
         
         
         FPSSimplePlatform(const CAttributePacket& attr);
         ~FPSSimplePlatform(void);
         
         virtual void HandleEvents(CMsg &pMsg);
         virtual void HandleAttributes(const CAttributePacket& attr);
         void MovePlatform( RwFrame *frame );
         
         
      private:
         void RespondToWorkspace( const CAttributePacket& attr );
         void RespondToAttributes( const CAttributePacket& attr );
         
         void TransformPlatform( RwFrame *frame, RwV3d *vec );
         void ProcessPlatformX(RwFrame *frame);
         void ProcessPlatformY(RwFrame *frame);
         void ProcessPlatformZ(RwFrame *frame);
         void ResetPlatform( void );
         void ToggleON_OFF( void );
         
         
      protected:
         RwMatrix  *m_mat;         /**< Matrix used for transforming object object */
         RwV3d     *m_OriginalPos; /**< Original position */
         CAtomicPtr m_pAtomic;     /**< Behavior's Atomic */
         RwV3d      m_direction;   /**< Position that is used as target for m_actual*/
         RwV3d      m_attribDir;   /**< RwReal array for storing translation values from the workspace */
         RwV3d      m_toX;         /**< X Position to be modified before transforming */
         RwV3d      m_toY;         /**< Y Position to be modified before transforming */
         RwV3d      m_toZ;         /**< Z Position to be modified before transforming */
         STATE      m_state;       /**< State of platform */  
         PLAYSTATE  m_playstate;   /**< Play state,  play once,  or loop */
         RwV3d      m_actual;      /**< Actual position */
         CEventId   m_trigger;     /**< m_trigger is used to store the clients reference to an event*/ 
         RwBool     m_dest[3];     /**< has platform reached its destination */
      };
   }//namespace FPS
}//namespace RWS

#endif
