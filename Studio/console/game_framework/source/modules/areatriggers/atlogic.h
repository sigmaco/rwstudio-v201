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

#ifndef ATLOGIC_H__
#define ATLOGIC_H__

#include "framework/core/eventhandler/ceventhandler.h"


namespace RWS
{
   namespace AreaTrigger
   {
      /*
      *  Forward definition
      */
      class ATBase;

      /**
      *
      *  \ingroup Mod_AreaTriggers
      *
      *  Base logic class. This simply fires the m_EnterEvent each time ProcessLogic() is called
      *  with bIntersecting as true. 
      *  
      *  Subclasses should implement their own ProcessLogic() and add any extra members that are
      *  needed. However, you can't add any extra RWS Commands because the ATLogic commands are
      *  explicitly included in the ATBase class - no commands from sub-classes will show up in RWS.
      *
      */
      class ATLogic
      {
      public:

         ATLogic( ATBase * pBase );
         virtual ~ATLogic();

         virtual void ProcessLogic( CEventHandler * pEventHandler, RwBool bIntersecting );

      protected:

         virtual void OnEnter( CEventHandler * pEntered );
         virtual void OnExit( CEventHandler * pExited );

         void SendEnterMsg();
         void SendExitMsg();

      protected:
      
         ATBase * m_pBase;       /**< ATBase-derived object that this logic belongs to */

      };

      /**
      *
      *  \ingroup Mod_AreaTriggers
      *
      *  The maximum number of objects that can be tracked by ATFireOnceOnEntry.
      *
      */
      static const RwUInt32 MAX_OBJECTS_TRACKED = 8;

      /**
      *
      *  \ingroup Mod_AreaTriggers
      *
      *  Logic that only fires when an object first intersects with it. 
      *  The m_EnterEvent won't fire again until the object leaves and 
      *  re-enters the trigger area.
      *
      */
      class ATFireOnceOnEntry : public ATLogic
      {
      public:

         ATFireOnceOnEntry( ATBase * pBase );
         virtual ~ATFireOnceOnEntry() {}

         void ProcessLogic( CEventHandler * pEventHandler, RwBool bIntersecting );

      protected:
      
         void OnEnter( CEventHandler * pEntered );
         void OnExit( CEventHandler * pExited );

      protected:

         CEventHandler * m_IntersectingObjects[MAX_OBJECTS_TRACKED];
         RwUInt32 m_NumObjects;
      };

      /**
      *
      *  \ingroup Mod_AreaTriggers
      *
      *  Logic that fires the enter event when a CEventHandler enters an
      *  area trigger and the exit event when it leaves it.
      *
      */
      class ATFireOnceOnEntryAndExit : public ATLogic
      {
      public:

         ATFireOnceOnEntryAndExit( ATBase * pBase );
         virtual ~ATFireOnceOnEntryAndExit() {}

         void ProcessLogic( CEventHandler * pEventHandler, RwBool bIntersecting );

      protected:
      
         void OnEnter( CEventHandler * pEntered );
         void OnExit( CEventHandler * pExited );

      protected:

         CEventHandler * m_IntersectingObjects[MAX_OBJECTS_TRACKED];
         RwUInt32 m_NumObjects;
      };      
   } // namespace AreaTrigger

} // namespace RWS

#endif // __ATLOGIC_H_