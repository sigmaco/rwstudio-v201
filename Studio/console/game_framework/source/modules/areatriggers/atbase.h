/*****************************************************************************
*
* File :     ATBase.cpp
*
* Abstract : Base class for an area trigger.
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

#ifndef ATBASE_H__
#define ATBASE_H__

//////////////////////////////////////////////////////////////////
//
// RenderWare Includes
//
#include "rpcollis.h"

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/core/attributehandler/csystemcommands.h"
#include "framework/core/eventhandler/ceventhandler.h"

#include "framework/core/attributehandler/allocation policies/LinearAllocationPolicy.h"

#include "atlogic.h"

namespace RWS
{
   namespace AreaTrigger
   {
      /**
      *
      *  \ingroup Mod_AreaTriggers
      *
      *  Structure used when making a collision query with an area trigger.
      *  You need to pass a pointer to a variable of this type as the data
      *  member of the message.
      *
      *  \note Currently, only rpINTERSECTPOINT and rpINTERSECTLINE intersection
      *        types are supported.
      *
      */
      struct ATCollisionQuery
      {
         CEventHandler * pEventHandler;   /**< Pointer to a CEventHandler-derived class that is making the query */
         RpIntersection Data;             /**< Data for the intersection test, including what type of intersection
                                               to perform. */
      };


      /**
      *
      *  \ingroup Mod_AreaTriggers
      *
      *  Represents the base class for an area trigger.
      *
      */
      class ATBase : public CEventHandler, public CAttributeHandler, public CSystemCommands, public LinearAllocationPolicy
      {
      public:

         RWS_DECLARE_CLASSID(ATBase);

         RWS_BEGIN_COMMANDS

            RWS_MESSAGE(CMD_QueryEvent, "Message", "Specify the message to query for a collision with this entity", 
                        RECEIVE, ATCollisionQuery*, 0 )

         RWS_SEPARATOR("Logic", 0)
            RWS_ATTRIBUTE(CMD_Logic, "Logic Type", "Specify the algorithm to use with this trigger", 
                          LIST, RwUInt32, LIST(FireContinuouslyOnEntry|@FireOnceOnEntry|FireOnceOnEntryAndExit) )
            RWS_MESSAGE(CMD_EnterEvent, "Enter Event", "Message sent when an object enters the area", TRANSMIT, 0, 0 )
            RWS_MESSAGE(CMD_ExitEvent, "Exit Event", "Message sent when an object exits the area", TRANSMIT, 0, 0 )

         RWS_SEPARATOR("Debug Tools", 0)
            RWS_ATTRIBUTE(CMD_DebugRender, "Render the area", "Uses DebugTools to render a wire frame representation "
                          "of the trigger area", BOOLEAN, RwUInt32, DEFAULT(0) )

         RWS_END_COMMANDS;
      
         ATBase( const CAttributePacket& attr );
         virtual ~ATBase();

         void HandleEvents( CMsg& Msg );
         void HandleAttributes( const CAttributePacket& attr );

         /**
         *
         *  Function used to test for intersections between a given point
         *  and this area trigger. Derived classes must implement this themselves.
         *
         *  \param pWorldPoint point to test, in world coordinates.
         *  \return TRUE if the point is inside or intersects the area, FALSE otherwise.
         *
         */
         virtual RwBool PointInsideGeometry( const RwV3d * pPoint ) = 0;

         /**
         *
         *  Function used to test for line intersections with the area trigger.
         *
         *  \param pWorldLine Line to test, in world coordinates.
         *  \return TRUE if the line intersets the area, FALSE otherwise.
         *
         */
         virtual RwBool LineIntersectGeometry( const RwLine * pLine ) = 0;

         /**
         *
         *  Draw an in-game representation of the area contained by this
         *  area trigger. Only ever called if RWS_DEBUGTOOLS is defined when the framework is built.
         *
         *  \see RWS_DEBUGTOOLS DebugTools
         *
         */
         virtual void DebugToolsRender() = 0;
      
      protected:
      
         friend class ATLogic;     /* To allow the contained ATLogic access to the Enter/Exit events */

         RwMatrix m_Matrix;         /**< World matrix of the area. */
         RwMatrix m_InvMatrix;      /**< Inverse of the world matrix for the trigger area. Cached for speed. */
         CEventId m_QueryEvent;     /**< Event that this is linked to query for a collision */
         ATLogic * m_pLogic;        /**< Logic for how this trigger responds to things entering or leaving it */
         CEventId m_EnterEvent;     /**< Event that is fired when an object 'enters' a trigger area */
         CEventId m_ExitEvent;      /**< Event that gets fired when an object exits a trigger area */

      };

   } // namespace AreaTrigger

} // namespace RWS

#endif // __ATBASE_H__
