/*****************************************************************************
*
* File :     ATLogic.cpp
*
* Abstract : Base class for an Area Trigger's logic.
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

// Left to its own devices, the MW linker will dead-strip the registrar ctors, so...
#ifdef __MWERKS__
#pragma force_active on
#endif

#include <string.h>  // for memset()

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/core/macros/debugmacros.h"

#include "atlogic.h"
#include "atbase.h"


namespace RWS
{
   namespace AreaTrigger
   {
      /**
      *
      *  Constructor
      *
      */
      ATLogic::ATLogic( ATBase * pBase )
      {
         m_pBase = pBase;
      }
   
      /**
      *
      *  Destructor
      *
      */
      ATLogic::~ATLogic()
      {
      }

      /**
      *  
      *  Process the trigger logic for an object. The default implementation simply
      *  sends ATBase::m_EnterEvent whenever bIntersecting is true.
      *
      *  \param pEventHandler Pointer to the object to process the logic for.
      *  \param bIntersecting true if the object is intersecting the trigger area, false otherwise.
      *
      */
      void ATLogic::ProcessLogic( CEventHandler * pEventHandler, RwBool bIntersecting )
      {
         RWS_FUNCTION("ATLogic::ProcessLogic");

         if (bIntersecting)
         {
            OnEnter(pEventHandler);
         }

         RWS_RETURNVOID();
      }

      /**
      *
      *  Handler for when an object enters the trigger area
      *
      *  \param pEntered Pointer to the object that has entered the trigger area.
      *
      */
      void ATLogic::OnEnter( CEventHandler * pEntered )
      {
         RWS_FUNCTION("ATLogic::OnEnter");

         SendEnterMsg();

         RWS_RETURNVOID();
      }

      /**
      *
      *  Handler for when an object leaves the trigger area
      *
      *  \param pExited Pointer to the object which has left the trigger area.
      *
      */
      void ATLogic::OnExit( CEventHandler* pExited )
      {
         RWS_FUNCTION("ATLogic::OnExit");

         SendExitMsg();

         RWS_RETURNVOID();
      }

      /**
      *
      *  Send an 'enter' message. Note: needs to access a protected member of ATBase, which is 
      *  restricted to ATLogic only, none of its derived classes.
      *
      */
      void ATLogic::SendEnterMsg()
      {
         RWS_FUNCTION("ATLogic::SendEnterMsg");

         SendMsg(m_pBase->m_EnterEvent);

         RWS_RETURNVOID();
      }

      /**
      *
      *  Send an 'exit' message. Note: needs to access a protected member of ATBase, which is 
      *  restricted to ATLogic only, none of its derived classes.
      *
      */
      void ATLogic::SendExitMsg()
      {
         RWS_FUNCTION("ATLogic::SendExitMsg");

         SendMsg(m_pBase->m_ExitEvent);

         RWS_RETURNVOID();
      }


      /**
      *
      *  Constructor
      *
      */
      ATFireOnceOnEntry::ATFireOnceOnEntry( ATBase * pBase )
         : ATLogic(pBase),
           m_NumObjects(0)
      {
         memset(m_IntersectingObjects, 0, sizeof(m_IntersectingObjects));
      }

      /**
      *
      *  This behaviour only fires the ATBase::m_EnterEvent when an object
      *  first enters an area. The ATBase::m_ExitEvent is never fired.
      *  Up to MAX_OBJECTS_TRACKED objects can be intersecting the trigger
      *  area at any one time, any more will not cause any events to be sent.
      *
      *  \param pEventHandler Pointer to the object to process the logic for.
      *  \param bIntersecting true if the object is intersecting the trigger 
      *                       area, false otherwise.
      *
      */
      void ATFireOnceOnEntry::ProcessLogic( CEventHandler * pEventHandler, RwBool bIntersecting )
      {
         RWS_FUNCTION("ATFireOnceOnEntry::ProcessLogic");

         // Is the event handler currently intersecting this?
         bool bCurrentlyIntersecting = false;
         for (RwUInt32 Index = 0; Index < m_NumObjects; Index++)
         {
            if (m_IntersectingObjects[Index] == pEventHandler)
            {
               bCurrentlyIntersecting = true;
               break;
            }
         }

         if (bCurrentlyIntersecting)
         {
            if (!bIntersecting)
            {
               OnExit(pEventHandler);
            }
         }
         else
         {
            if (bIntersecting)
            {
               OnEnter(pEventHandler);
            }
         }

         RWS_RETURNVOID();
      }

      /**
      *
      */
      void ATFireOnceOnEntry::OnEnter( CEventHandler * pEntered )
      {
         RWS_FUNCTION("ATFireOnceOnEntry::OnEnter");

         if (m_NumObjects < MAX_OBJECTS_TRACKED && pEntered)
         {
            m_IntersectingObjects[m_NumObjects] = pEntered;
           ++m_NumObjects;
         }

         SendEnterMsg();

         RWS_RETURNVOID();
      }

      /**
      *
      */
      void ATFireOnceOnEntry::OnExit( CEventHandler * pExited )
      {
         RWS_FUNCTION("ATFireOnceOnEntry::OnExit");

         // Remove the object from the list of intersecting objects
         RwUInt32 Index = 0;
         while (Index < m_NumObjects && pExited)
         {
            if (m_IntersectingObjects[Index] == pExited)
            {
               // swap the element for the last one and decrease the number of intersecting objects
               CEventHandler * pTemp = m_IntersectingObjects[Index];
               m_IntersectingObjects[Index] = m_IntersectingObjects[m_NumObjects - 1];
               m_IntersectingObjects[m_NumObjects - 1] = pTemp;
               --m_NumObjects;

               break;
            }
         }

         RWS_RETURNVOID();
      }

      
      /**
      *
      *  Constructor
      *
      */
      ATFireOnceOnEntryAndExit::ATFireOnceOnEntryAndExit( ATBase * pBase )
         : ATLogic(pBase),
           m_NumObjects(0)
      {
         memset(m_IntersectingObjects, 0, sizeof(m_IntersectingObjects));
      }

      /**
      *
      *  This behaviour only fires the ATBase::m_EnterEvent when an object
      *  first enters an area. The ATBase::m_ExitEvent is never fired.
      *  Up to MAX_OBJECTS_TRACKED objects can be intersecting the trigger
      *  area at any one time, any more will not cause any events to be sent.
      *
      *  \param pEventHandler Pointer to the object to process the logic for.
      *  \param bIntersecting true if the object is intersecting the trigger 
      *                       area, false otherwise.
      *
      */
      void ATFireOnceOnEntryAndExit::ProcessLogic( CEventHandler * pEventHandler, RwBool bIntersecting )
      {
         RWS_FUNCTION("ATFireOnceOnEntry::ProcessLogic");

         // Is the event handler currently intersecting this?
         bool bCurrentlyIntersecting = false;
         for (RwUInt32 Index = 0; Index < m_NumObjects; Index++)
         {
            if (m_IntersectingObjects[Index] == pEventHandler)
            {
               bCurrentlyIntersecting = true;
               break;
            }
         }

         if (bCurrentlyIntersecting)
         {
            if ( !bIntersecting )
            {
               OnExit(pEventHandler);
            }
         }
         else
         {
            if (bIntersecting)
            {
               OnEnter(pEventHandler);
            }
         }

         RWS_RETURNVOID();
      }

      /**
      *
      */
      void ATFireOnceOnEntryAndExit::OnEnter( CEventHandler * pEntered )
      {
         RWS_FUNCTION("ATFireOnceOnEntry::OnEnter");

         if ( m_NumObjects < MAX_OBJECTS_TRACKED && pEntered )
         {
            m_IntersectingObjects[m_NumObjects] = pEntered;
            ++m_NumObjects;
         }

         SendEnterMsg();

         RWS_RETURNVOID();
      }

      /**
      *
      */
      void ATFireOnceOnEntryAndExit::OnExit( CEventHandler * pExited )
      {
         RWS_FUNCTION("ATFireOnceOnEntry::OnExit");

         // Remove the object from the list of intersecting objects
         RwUInt32 Index = 0;
         while (Index < m_NumObjects && pExited)
         {
            if (m_IntersectingObjects[Index] == pExited)
            {
               // swap the element for the last one and decrease the number of intersecting objects
               CEventHandler * pTemp = m_IntersectingObjects[Index];
               m_IntersectingObjects[Index] = m_IntersectingObjects[m_NumObjects - 1];
               m_IntersectingObjects[m_NumObjects - 1] = pTemp;
               --m_NumObjects;

               break;
            }
         }

         SendExitMsg();

         RWS_RETURNVOID();
      }
      
   } // namespace AreaTrigger

} // namespace RWS