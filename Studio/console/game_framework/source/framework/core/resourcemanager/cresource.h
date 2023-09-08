/*****************************************************************************
*
* File :     CResource.h
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

#ifndef _CRESOURCEMANAGER_H_
#error "cresource.h should no longer be included directly, please include cresourcemanager.h instead"
#endif

/**
*
*  \ingroup Resource
*
*  \page ResourceOverview Overview
*
*  The CResourceManager and associated classes provide a way of managing
*  the data resources for a game level. Typically this will be graphics,
*  audio and animation data but could be just about anything.
*
*/

#ifndef _RESOURCE_H_
#define _RESOURCE_H_

//////////////////////////////////////////////////////////////////
//
// RenderWare Includes
//
#include <rwcore.h>

#ifdef _MSC_VER
#pragma comment (lib, "rwcore.lib")
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "../misc/idfromstring.h"
#include "../misc/rwsguid.h"

namespace RWS
{
   /**
   *
   *  \ingroup Resource
   *
   *  The CResource class stores an entry for each resource containing several fields.
   *
   *  [pResId][pResType][pResData][uiResDataSize]
   *
   *  This is expected to change to...
   *
   *  [pResUId][pResName][pResType][pResData][uiResDataSize]
   *
   *  Where pResUId is a GUID
   *        pResName is an optional string specifying the name.
   *        pResType is the pointer to the resource
   *        uiResDataSize size of the resource data, if known.
   */
   class CResource
   {
      friend class CResourceIterator;
      
   public:
      
      CResource(void); 
      ~CResource(void);
      
      /*
      *  Get the Guid of the resource managed by this CResource object.
      *
      *  \returns A RWSGUID containg the unique Id for the resource.
      */
      const RWSGUID & GetId() {return m_Guid;}
     
      /**
      *  Get the type of the resource, managed by this CResource object.
      *
      *   \return A string containing the type of the resource.
      */
      const RwChar *GetType(void) const {return m_pResType;}
      
      /**
      *  Get the name of the resource, managed by this CResource object.
      *
      *   \return A string containing the type of the resource.
      */
      const RwChar *GetName(void) const {return m_pResName;}
      
      /**
      *  Get resource pointer to the resource, managed by this CResource object.
      *
      *   \return A pointer to the resource
      */
      const void *GetResource(void) const {return m_pResData;}
      
      /**
      *  Get the size, managed by this CResource object.
      *
      *   \returns The size of the resource, if avaialable.otherwise 0
      */
      RwUInt32 GetSize(void) const {return m_uiResDataSize;}
      /**
      *  Get the lock refcount of the resource, managed by this CResource object.
      *
      *   \returns The lock refcount of this resource
      */
      RwUInt32 GetLockRefCount(void) const {return m_uiLockRefCount;}

      /*
      *  Set the Guid of the resource managed by this CResource object.
      *
      *  \param rGuid A reference to a RWSGUID containing the unique Id of the resource.
      */
      void SetId( const RWSGUID &rGuid ) {m_Guid = rGuid;}
    
      /**
      *  Set the type of the resource, managed by this CResource object.
      *
      *   \param pType A string containing the type of the resource.
      */
      void SetType(const RwChar *pType) 
      {
         operator delete (m_pResType);
         
         if (pType && rwstrlen(pType) > 0)
         {
            m_pResType = static_cast<RwChar *>(::RWS_OP_NEW( rwstrlen (pType) + 1));
            
            rwstrcpy (m_pResType, pType);
         }
         else
         {
            m_pResType = 0;
         }
      }
      
      /**
      *  Set the name of the resource, managed by this CResource object.
      *
      *  \param pName A string containing the name of the resource.
      */
      void SetName(const RwChar *pName) {
         operator delete (m_pResName);
         
         if (pName && rwstrlen(pName) > 0)
         {
            m_pResName = static_cast<RwChar *>(::RWS_OP_NEW( rwstrlen (pName) + 1));
            
            rwstrcpy (m_pResName, pName);
         }
         else
         {
            m_pResName = 0;
         }
      }
      
      /**
      *  Set resource pointer to the resource, managed by this CResource object.
      *
      *   \param pResource A pointer to the resource
      */
      void SetResource(const void *pResource) {m_pResData = pResource;}
      
      /**
      *  Set the size, managed by this CResource object.
      *
      *  \param uiSize The size of the resource.
      */
      void SetSize(RwUInt32 uiSize) {m_uiResDataSize = uiSize;}
      
      /**
      *  Set the lock refcount of the resource, managed by this CResource object.
      *
      *  \param uiLockRefCount The lock refcount of this resource.
      */
      void SetLockRefCount(RwUInt32 uiLockRefCount) {m_uiLockRefCount = uiLockRefCount;}
      
         protected:
            
            CResource *m_pNextResource;   /**< Pointer to the next resource */
            CResource *m_pPrevResource;   /**< Pointer to the previous resource */
            static CResource *sm_ResourceHead;   /**< Pointer to head of resource list */
            
         private:
            
            RwChar *m_pResName;           /**< String describing the resource Name */
            RwChar *m_pResType;           /**< String describing the resource Type */
            
            const void *m_pResData;       /**< Pointer to resource */
            
            RwUInt32 m_uiResDataSize;     /**< Size of resource in bytes, if known otherwise 0 */
            RwUInt32 m_uiLockRefCount;    /**< Reference count used to lock the resource */


            RWSGUID m_Guid;               /**< Id of the resource, in the form of an integer
                                               guid. */
      };
      
      /**
      *
      *  \ingroup Resource
      *
      *  A unidirectional iterator for a CResource object.
      */
      class CResourceIterator
      {
      public:
         CResourceIterator()
         {
            m_pCResource = CResource::sm_ResourceHead;
         }
         
         void First() {
            m_pCResource = CResource::sm_ResourceHead;
         }
         
         bool IsDone(void) 
         {
            if (m_pCResource == 0 ) return true;
            else return false;
         }
         
         CResource& operator*() const 
         {
            return *m_pCResource;
         }
         
         CResource* operator->() const 
         {
            return m_pCResource;
         }
         
         CResourceIterator& operator++()
         {
            m_pCResource = m_pCResource->m_pNextResource;
            return *this;
         }
         
         void Remove(void)
         {
            if (m_pCResource)
            {
               CResource *next = m_pCResource->m_pNextResource;
            
               delete m_pCResource;
            
               m_pCResource = next;
            }
         }
         
      private:
         
         CResource *m_pCResource;
      };
}
#endif
