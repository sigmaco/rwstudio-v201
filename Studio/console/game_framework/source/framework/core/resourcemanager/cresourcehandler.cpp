/*****************************************************************************
*
* File :     CResourceHandler.cpp
*
* Abstract : 
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
* Copyright (c) 2000 Criterion Software Ltd.
* All Rights Reserved.
*
* RenderWare is a trademark of Canon Inc.
*
*****************************************************************************/
#include "precomp.h"
//#define RWS_CALLSTACKENABLE
//#define RWS_TRACEENABLE

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "cresourcehandler.h"
#include "../macros/debugmacros.h"

//////////////////////////////////////////////////////////////////
//
// Standard Includes
//
#ifdef _MSC_VER
#pragma warning (disable: 4786)
#endif

#include <list>

namespace RWS
{
   namespace 
   {
      typedef std::list<CResourceHandler*> CResourceHandlerList;

      /*
      *
      * This function encapsulates the global CResourceHandlerList, this ensures that the 
      * the_CResourceHandlerList is initialized the first time GetCResourceHandlerList is
      * called.
      *
      */
      CResourceHandlerList& GetCResourceHandlerList(void)
      {
         RWS_FUNCTION("RWS::NULL::GetCResourceHandlerList");

         static CResourceHandlerList the_CResourceHandlerList;

         RWS_RETURN(the_CResourceHandlerList);
      }
   }

   /**
   *
   * Construct a CResourceHandler object, register resource handler.
   *
   */
   CResourceHandler::CResourceHandler(void)
   {
      RWS_FUNCTION("RWS::CResourceHandler::CResourceHandler");

      GetCResourceHandlerList().push_front(this);

      RWS_RETURNVOID();
   }

   /**
   *
   * Destroy a CResourceHandler object unregister resource handler.
   *
   */
   CResourceHandler::~CResourceHandler(void)
   {
      RWS_FUNCTION("RWS::CResourceHandler::~CResourceHandler");

      GetCResourceHandlerList().remove(this);

      RWS_RETURNVOID();
   }

   /**
   *
   * Check each registered resource handler and return the handler associated with this
   * resource type.
   *
   */
   CResourceHandler *CResourceHandler::FindHandler(const RwChar * const psType)
   {
      RWS_FUNCTION("RWS::CResourceHandler::FindHandler");

      CResourceHandlerList::iterator it = GetCResourceHandlerList().begin();

      while (it != GetCResourceHandlerList().end())
      {
         if ((*it)->IsHandled(psType))
         {
            RWS_RETURN (*it);
         }
         it++;
      }

      RWS_RETURN( 0 );  // None found
   }

   /**
   *
   *  Calls CResourceHandler::Open for each of the registered resource handlers.
   *
   *
   */
   void CResourceHandler::OpenAll(void)
   {
      RWS_FUNCTION("RWS::CResourceHandler::OpenAll");

      CResourceHandlerList::iterator it = GetCResourceHandlerList().begin();

      while (it != GetCResourceHandlerList().end())
      {
         (*it)->Open();

         it++;
      }

      RWS_RETURNVOID();
   }
   
   /**
   *
   *  Calls CResourceHandler::Close for each of the registered resource handlers.
   *
   */
   void CResourceHandler::CloseAll(void)
   {
      RWS_FUNCTION("RWS::CResourceHandler::CloseAll");

      CResourceHandlerList::iterator it = GetCResourceHandlerList().begin();

      while (it != GetCResourceHandlerList().end())
      {
         (*it)->Close();

         it++;
      }

      RWS_RETURNVOID();
   }
}