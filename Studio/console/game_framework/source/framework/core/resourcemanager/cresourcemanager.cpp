/*****************************************************************************
*
* File :     CResourceManager.cpp
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
// Standard Includes
//
#include <stdio.h>

//////////////////////////////////////////////////////////////////
//
// RenderWare Includes
//
#include <rtfsyst.h>

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "cresourcemanager.h"
#include "cresourcehandler.h"
#include "../macros/debugmacros.h"


namespace RWS
{
   namespace
   {
      /**
      *
      *  LoadResource selects the CResourceHandler to load the resource, and adds it to the resource manager.
      *
      *  \param pStrName A pointer to a string containing the name of the resource.
      *
      *  \param pResId A pointer to a string specifying the unique resource id.
      *
      *  \param pStrType A pointer to a string specifying the resource type.
      *
      *  \param pResourcePath A pointer to a string specifying the resource path, RenderWare streams
      *  often contain references to other files, this path is used to locate these additional
      *  resources i.e. textures.
      *
      *  \param pStream A pointer to a RenderWare stream.
      *
      *  \param uiStreamSize The size of the file associated with this resource, note this may
      *  be a section of a larger file.
      */
      void *LoadResource (const RwChar *pStrName,
                          const RWSGUID *pResId,
                          const RwChar *pStrType,
                          RwChar* pResourcePath,
                          RwStream* pStream, 
                          RwUInt32 uiStreamSize)
      {
         RWS_FUNCTION("RWS::NULL::LoadResource" );
   
         void* ret = 0;

         RWS_PRE(pStrName);
         RWS_PRE(pResId);
         RWS_PRE(pStrType);
   
         // Find the resource handler registered with this type
         //
         CResourceHandler *pCResourceHandler = CResourceHandler::FindHandler(pStrType);

         RWS_DEBUGSTREAM( "-------------------------------------------------------------------------" << std::endl );
         RWS_DEBUGSTREAM( "LOADING " << RWS_VALIDSTRING(pStrName) << " " << RWS_VALIDSTRING(pStrType) << std::endl );
         RWS_DEBUGSTREAM( "-------------------------------------------------------------------------" << std::endl );

         if (pCResourceHandler)
         {
            RwUInt32 _uiResDataSize = 0;

            if ( (ret = pCResourceHandler->Load(pStrName,
                                                pStrType,
                                                pResourcePath,
                                                pStream,
                                                uiStreamSize,
                                                _uiResDataSize)) )
            {
               CResourceManager::Add(pStrName, pResId, pStrType, ret, _uiResDataSize);
            }
            else
            {
               RWS_ASSERTFAIL("pCResourceHandler->Load failed\npStrName ["
                  <<RWS_VALIDSTRING(pStrName)<<"]\npResId ["<<pResId
                  <<"]\npStrType ["<<RWS_VALIDSTRING(pStrType)<<"]\npResourcePath ["
                  <<RWS_VALIDSTRING(pResourcePath)<<"]");
            }
         }
         else
         {
            RWS_ASSERTFAIL ("OnLoadResource - Unable to locate resource handler for resource type\npStrName ["
               <<RWS_VALIDSTRING(pStrName)<<"]\npResId ["<<pResId<<"]\npStrType ["
               <<RWS_VALIDSTRING(pStrType)<<"]\npResourcePath ["<<RWS_VALIDSTRING(pResourcePath)<<"]");
         }

         RWS_DEBUGSTREAM( "-------------------------------------------------------------------------" << std::endl );
         RWS_DEBUGSTREAM( "DONE " << RWS_VALIDSTRING(pStrName) << " " << RWS_VALIDSTRING(pStrType) << std::endl );
         RWS_DEBUGSTREAM( "-------------------------------------------------------------------------" << std::endl );

         RWS_RETURN(ret);
      }

      /**
      *
      *  FreeResource selects the registered resource handler to unload a resource of a specific type. 
      * 
      *  \param pStrType A pointer to a string specifying the resource type.
      *
      *  \param pResource A pointer to a block of memory containing the resource.
      *
      */
      RwBool FreeResource (const RwChar *pStrType, void *pResource)
      {
         RWS_FUNCTION("RWS::NULL::FreeResource");
   
         RWS_PRE(pStrType);
         RWS_PRE(pResource);

         RwBool ret = FALSE;

         // Find the resource handler registered with this type
         //
         CResourceHandler *pCResourceHandler = CResourceHandler::FindHandler(pStrType);

         if (pCResourceHandler)
         {
            ret = pCResourceHandler->UnLoad(pStrType, pResource);
         }
         // ---------------------------------------------------------------------------------
         //
         // Add UnLoading of custom resource types here
         //
         // ---------------------------------------------------------------------------------
         else
         {
            RWS_ASSERTFAIL ("Unable to locate resource handler for resource type, pStrType [" << pStrType << "]");
         }
   
         RWS_RETURN(ret);
      }
   }


  /*
   *
   * Global pointer to head of Resource list.
   *
   */
   CResource *CResource::sm_ResourceHead = 0;
   
   /**
   *
   *  Construct a Resource object. 
   *
   */
   CResource::CResource(void) : 
      m_pNextResource(0),
      m_pPrevResource(0),
      m_pResName (0),
      m_pResType(0),
      m_uiLockRefCount(0)
   {
      RWS_FUNCTION("RWS::CResource::Resource");

      m_Guid.Clear();
      
      if (sm_ResourceHead == 0)
      {
         sm_ResourceHead = this;
      }
      else
      {
         this->m_pNextResource = sm_ResourceHead;
         sm_ResourceHead->m_pPrevResource = this;
         
         sm_ResourceHead = this;
      }

      RWS_RETURNVOID();
   }
   
   /**
   *
   *  Destroy a Resource object.
   *
   */
   CResource::~CResource(void)
   {
      RWS_FUNCTION("RWS::CResource::~CResource");
      
      RWS_TRACE("m_pResId ["<<RWS_VALIDSTRING(m_pResId)<<"] m_pResName ["
         <<RWS_VALIDSTRING(m_pResName)<<"] m_pResType ["
         <<RWS_VALIDSTRING(m_pResType)<<"]");
      
      m_pResData = 0;

      operator delete (m_pResName);
      operator delete (m_pResType);
      
      CResource *_pNextResource = m_pNextResource;
      CResource *_pPrevResource = m_pPrevResource;

      if (m_pPrevResource == 0)  // Head of List
      {
         sm_ResourceHead = _pNextResource;
         
         if (_pNextResource)
         {
            _pNextResource->m_pPrevResource = m_pPrevResource;
         }
      }
      else  // Middle of List
      {
         if (_pPrevResource)
         {
            _pPrevResource->m_pNextResource = m_pNextResource;
         }
         
         if (_pNextResource)
         {
            _pNextResource->m_pPrevResource = m_pPrevResource;
         }
      }
      
      RWS_RETURNVOID();
   }
   
   /**
   *
   *  Add a resource to the resource manager.
   *
   *  \param _pResName A string used to name the resource.
   *  \param _pResId A string used to uniquely identify the resource.
   *  \param _pResType A string used to identify the type of the resource.
   *  \param _pResData A pointer to the data associated with this resource.
   *  \param _uiResDataSize The size of the data associated with this resource.
   *
   *  \see CResourceManager::Remove
   */
   void CResourceManager::Add(const RwChar *_pResName,
                              const RWSGUID *_pResId,
                              const RwChar *_pResType,
                              const void *_pResData,
                              RwUInt32 _uiResDataSize)
   {
      RWS_FUNCTION("RWS::CResourceManager::Add");
      RWS_PRE(_pResId);
      RWS_PRE(_pResType);
      RWS_PRE(_pResData);
      
      RWS_TRACE("_pResId "<<_pResId<<" _pResType "<<_pResType);

      CResource *pResource = ::RWS_NEW CResource();

      pResource->SetName(_pResName);
      pResource->SetId(*_pResId);
      pResource->SetType(_pResType);
      pResource->SetResource(_pResData);
      pResource->SetSize(_uiResDataSize);
      
      RWS_RETURNVOID();
   }
   
   /**
   *
   *  Remove the specified resource from the resource manager.
   *
   *  \param _pResData A pointer to the data associated with the resource.
   *
   *  \return Returns a pointer to the resource if successful otherwise 0, failure may occur
   *  if the resource is locked.
   *
   *  \see CResourceManager::Add
   *  \see CResourceManager::PurgeItem
   *
   */
   const void *CResourceManager::Remove(const void *_pResData)
   {
      RWS_FUNCTION("RWS::CResourceManager::Remove");
      RWS_ASSERT(_pResData, "No resource name specified.");
      
      CResourceIterator Ittr;
      
      while (!Ittr.IsDone())
      {
         if (Ittr->GetResource() == _pResData)
         {
            if (PurgeItem(Ittr))
            {             
               RWS_RETURN( _pResData );
            }
            else
            {
               RWS_WARNING(false, "Attempting to remove resource failed locked count = " << Ittr->GetLockRefCount());

               // It is possible for purge to fail if the resource is locked
               //
               RWS_RETURN( 0 ); 
            }
         }  
         
         ++Ittr;
      }
      
      RWS_ASSERTFAIL("Unable to find specified resource ( " << _pResData << " )");
      
      RWS_RETURN( 0 );
   }
   
   /**
   *
   *  Find a specified resource and return a pointer to the data associated with that resource.
   *
   *  \param _pResId A pointer to a string used to identify the resource.
   *
   *  \param _pResName A pointer to a string, filled in if the specified resource is found with the 
   *  name of the resource.
   *
   *  \param _pResType A pointer to a string, filled in if the specified resource is found with the 
   *  type of the resource.
   *
   *  \param _uiResDataSize A pointer to a RwUInt32 filled in if the specified resource is found.
   *
   *  \return If the resource manager locates the resource a pointer to the associated data,
   *  otherwise null.
   *
   */
   const void *CResourceManager::FindById(const RWSGUID *_pResId,
                                          RwChar const **_pResType,
                                          RwUInt32 *_uiResDataSize,
                                          RwChar const **_pResName)
   {
      RWS_FUNCTION("RWS::CResourceManager::FindById");
      
      CResourceIterator Ittr;

      while (!Ittr.IsDone())
      {
         if (Ittr->GetId() == (*_pResId))
         {
            if (_pResType) *_pResType = Ittr->GetType();
            if (_pResName) *_pResName = Ittr->GetName();
            if (_uiResDataSize) *_uiResDataSize = Ittr->GetSize();
            
            RWS_RETURN( Ittr->GetResource() );
         }

         ++Ittr;
      }

      RWS_TRACE("Not found: "<<_pResId);

      RWS_RETURN( 0 );
   }

#ifdef _DEBUG
   /**
   *
   *  Debugging function used to output a list of all available resources, their GUID, Name and Type.
   *
   */
   void CResourceManager::Debug_ListAvailableResources(void)
   {
      RWS_FUNCTION("RWS::CResourceManager::Debug_ListAvailableResources");

      CResourceIterator Ittr;

      RWS_TRACE("-------------------------------------------------------------------------");
      
      while (!Ittr.IsDone())
      {
         RWS_TRACE("Resource Id ["<<RWS_VALIDSTRING(Ittr->GetId())<<"] Name ["
            <<RWS_VALIDSTRING(Ittr->GetName())<<"] Type ["<<RWS_VALIDSTRING(Ittr->GetType())<<"]");

         RwChar const *_pResType = 0;
         RWSGUID Guid;
         RwUInt32 _ResDataSize = 0;
         const void *pResource = 0;

         Guid = Ittr->GetId();
         pResource = FindById(&Guid, &_pResType, &_ResDataSize);

         RWS_TRACEIF (pResource,
            "FindById found ["<<pResource<<"]["<<RWS_VALIDSTRING(_pResType)<<"]["
            <<_ResDataSize<<"]");

         pResource = FindByName(Ittr->GetName(), &Guid, &_pResType, &_ResDataSize);

         RWS_TRACEIF (pResource,
            "FindByName found ["<<pResource<<"]["<<RWS_VALIDSTRING(_pResType)<<"]["
            <<_ResDataSize<<"]");

         ++Ittr;
      }

      RWS_TRACE("-------------------------------------------------------------------------");

      RWS_RETURNVOID();
   }
#endif   
   /**
   *
   *  Find a named resource and return a pointer to the data associated with that resource.
   *
   *  \param _pResName A pointer to a Resource Name string used to specify the resource name to search for.
   *
   *  \param _pResId A pointer to an ID string, filled in if the specified resource is found.
   *
   *  \param _pResType A pointer to a Type string, filled in if the specified resource is found.
   *
   *  \param _uiResDataSize A pointer to a RwUInt32 filled in if the specified resource is found.
   *
   *  \return If the resource manager locates the resource a pointer to the associated data,
   *  otherwise null.
   *
   */
   const void *CResourceManager::FindByName( const RwChar * const _pResName,
                                             RWSGUID *_pResId,
                                             RwChar const **_pResType,
                                             RwUInt32 *_uiResDataSize)
   {
      RWS_FUNCTION("RWS::CResourceManager::FindByName");

      if (_pResName == 0) RWS_RETURN(0);   // Fail if name is 0

      CResourceIterator Ittr;

      while (!Ittr.IsDone())
      {
         if (Ittr->GetName() && rwstrcmp(Ittr->GetName(), _pResName) == 0)
         {
            if (_pResId) *_pResId = Ittr->GetId();
            if (_pResType) *_pResType = Ittr->GetType();
            if (_uiResDataSize) *_uiResDataSize = Ittr->GetSize();

            RWS_RETURN( Ittr->GetResource() );
         }

         ++Ittr;
      }

      RWS_TRACE("Not found: "<<_pResName);
      RWS_RETURN( 0 );
   }

   /**
   *
   *  Removes a resource from the resource manager.
   *
   *  \param Ittr A reference to a CResourceIterator object.
   *
   *  \return Returns true if the resource has been freed otherwise returns false.
   *
   */
   bool CResourceManager::PurgeItem(CResourceIterator &Ittr)
   {
      RWS_FUNCTION("RWS::CResourceManager::PurgeItem");

      if (Ittr->GetLockRefCount() == 0)
      {
         if (FreeResource (Ittr->GetType(), const_cast<void*>(Ittr->GetResource()) ))
         {
            Ittr.Remove();

            RWS_RETURN(true);
         }
         else
         {
            RWS_TRACE("FreeResource failed ["<<Ittr->GetId()<<"]");
         }
      }
      
      RWS_TRACE("Locked ["<<Ittr->GetLockRefCount()<<"]");

      RWS_RETURN(false);
   }

   /**
   *
   *  Remove all non-global resources from the resource manager.
   *
   *  \return Returns true if all resources are released otherwise false, failure may occur
   *  if one or more resources are locked.
   *
   *  \see CResource::Remove
   *
   */
   bool CResourceManager::Purge(void)
   {
      RWS_FUNCTION("RWS::CResourceManager::Purge");

      bool flag = true;

      CResourceIterator Ittr;

      while (!Ittr.IsDone())
      {
         RWS_TRACE("Id ["<<RWS_VALIDSTRING(Ittr->GetId())<<"] Name"<<RWS_VALIDSTRING(Ittr->GetName())<<"]");

         if (!PurgeItem(Ittr))
         {
            flag = false;  // Failed to free a resource

            ++Ittr;
         }
      }
      
      RWS_RETURN(flag);
   }

   /**
   *
   *  Remove all resources from the resource manager.
   *
   *  \return Returns true if all resources are released otherwise false, failure may occur
   *  if one or more resources are locked.
   *
   *  \see CResource::Remove
   *
   */
   bool CResourceManager::PurgeAll(void)
   {
      RWS_FUNCTION("RWS::CResourceManager::Purge");

      bool flag = true;

      CResourceIterator Ittr;

      while (!Ittr.IsDone())
      {
         RWS_TRACE("Id ["<<RWS_VALIDSTRING(Ittr->GetId())<<"] Name"<<RWS_VALIDSTRING(Ittr->GetName())<<"]");

         // Force the resource to be purged by setting the lock count to zero.
         Ittr->SetLockRefCount(0);

         if (!PurgeItem(Ittr))
         {
            flag = false;  // Failed to free a resource

            ++Ittr;
         }
      }
      
      RWS_RETURN(flag);
   }

   /**
   *
   *  Lock the specified resource, if a resource is locked it cannot be purged from the resource manager
   *
   *  \param ptr A void pointer to the resource to be locked
   *
   */
   void CResourceManager::Lock(const void *ptr)
   {
      RWS_FUNCTION("RWS::CResourceManager::Lock");

      CResourceIterator Ittr;
      
      while (!Ittr.IsDone())
      {
         if (Ittr->GetResource() == ptr)
         {
            Ittr->SetLockRefCount(Ittr->GetLockRefCount() + 1);

            RWS_TRACE("m_uiLockRefCount ["<<Ittr->GetLockRefCount()<<"]");

            break;
         }
         ++Ittr;
      }
      RWS_RETURNVOID();
   }

  /**
   *
   *  UnLock the specified resource, \see CResource::Lock
   *
   *  \param ptr A void pointer to the resource to be unlocked.
   *
   */
   void CResourceManager::UnLock(const void *ptr)
   {
      RWS_FUNCTION("RWS::CResourceManager::UnLock");

      CResourceIterator Ittr;
      
      while (!Ittr.IsDone())
      {
         if (Ittr->GetResource() == ptr)
         {
            Ittr->SetLockRefCount(Ittr->GetLockRefCount() - 1);

            RWS_TRACE("m_uiLockRefCount ["<<Ittr->GetLockRefCount()<<"]");

            break;
         }
         ++Ittr;
      }

      RWS_RETURNVOID();
   }
   
   /**
   *
   *  Load a resource.
   *
   *  \param pStrName A pointer to a string containing the name of the resource.
   *
   *  \param pResId A pointer to a string used to uniquely identify a resource.
   *
   *  \param pStrType A pointer to a string used to specify the type of the resource.
   *
   *  \param pStrFName A pointer to a string containing the file name of the resource.
   *
   *  \param pStrLoc A pointer to a string containing the path to any additional files
   *  that are loaded with this resource.
   *
   *  \return if successful returns a pointer to the resource, otherwise 0.
   */
   void *CResourceManager::Load(
      const RwChar *pStrName,
      const RWSGUID *pResId,
      const RwChar *pStrType,
      const RwChar *pStrFName,
      const RwChar *pStrLoc)
   {
      RWS_FUNCTION("RWS::CResourceManager::Load");
      
      RwChar ResourcePath[255];
      RwChar FilePath[255];
      
      RWS_PRE(pStrLoc);

      *ResourcePath = 0;

      rwstrcat(ResourcePath, pStrLoc);

      *FilePath = 0;

      rwstrcat(FilePath, pStrFName);

     // Make sure we have the correct forward or back slashes for the system
     //
#if (defined SKY) || (defined DOLPHIN)

      RwChar *pResourcePath = ResourcePath;
      RwChar *pFilePath = FilePath;

      while (*pResourcePath != 0)
      {
         if (*pResourcePath == '\\') *pResourcePath = '/';

         pResourcePath++;
      }

      while (*pFilePath != 0)
      {
         if (*pFilePath == '\\') *pFilePath = '/';

         pFilePath++;
      }
#endif

      RWS_TRACE("\n\n Id ["<<pResId<<"]\n Type ["<<pStrType<<"]\n FilePath ["<<FilePath
         <<"]\n ResourcePath ["<<ResourcePath<<"]\n\n");

      /*
      *
      * Determine the size of the file
      *
      */
      RwUInt32 uiStreamSize;
      void *In = RwFopen(FilePath, "rb");

      RWS_ASSERT(In,
         "RwFopen ["<<RWS_VALIDSTRING(FilePath)<<"] failed, check the resource path in the target connection.");

      RwFseek(In, 0, SEEK_END);

      uiStreamSize = RwFtell(In);

      RwFclose(In);

      /*
      *
      *  Open and Load the stream
      *
      */
      RwStream* pStream = RwStreamOpen(rwSTREAMFILENAME, rwSTREAMREAD, FilePath);

      RWS_ASSERT(pStream, "RwStreamOpen failed\nFile ["<<FilePath<<"]");

      void *ptrRes = LoadResource(pStrName, pResId, pStrType, ResourcePath, pStream, uiStreamSize);

      RwStreamClose(pStream, FilePath);

      RWS_ASSERT(ptrRes, "CResourceManager::Load Failed");

      RWS_RETURN(ptrRes);
   }

   /**
   *
   *  UpLoad a resource.
   *
   *  \param pStrName A pointer to a string containing the name of the resource.
   *
   *  \param pResId A pointer to a string used to uniquely identify a resource.
   *
   *  \param pStrType A pointer to a string containing the type of the resource.
   *
   *  \param pStrLoc A pointer to a string containing the path to the additional files
   *  that are loaded with this resource.
   *
   *  \param pStream A pointer to a RenderWare stream.
   *
   *  \param nEmbDataSize The size of the embedded data.
   */
   void *CResourceManager::UpLoad(
      const RwChar *pStrName,
      const RWSGUID *pResId,
      const RwChar *pStrType,
      const RwChar *pStrLoc,
      RwStream* pStream,
      RwUInt32 nEmbDataSize)
   {
      RWS_FUNCTION("RWS::CResourceManager::UpLoad");
      
      void *ptrRes = LoadResource (pStrName, pResId, pStrType, "", pStream, nEmbDataSize);

      RWS_ASSERT(ptrRes, "CResourceManager::UpLoad Failed");

      RWS_RETURN(ptrRes);
   }

   /**
   *
   *  Initialize the resource manager system, and each registered resource handler.
   *
   */
   void CResourceManager::Open(void)
   {
      RWS_FUNCTION("RWS::CResourceManager::Open");
      
      CResourceHandler::OpenAll();
      
      RWS_RETURNVOID();
   }
   
   /**
   *
   *  Shutdown the resource manager system, and each registered resource handler.
   *
   */
   void CResourceManager::Close(void)
   {
      RWS_FUNCTION("RWS::CResourceManager::Close");
      
      CResourceHandler::CloseAll();

      RWS_RETURNVOID();
   }
   

   /**
   *  
   *  Update Receives an update resource, data packet from the workspace which is used to modify a resource
   *  while connected.
   *  
   *  \param pStrType A pointer to a string containing the type of the resource.
   *  \param pResData A pointer to the resource's data.
   *  \param pData A pointer to data used to update the resource.
   */
   void CResourceManager::Update(const RwChar *pStrType,
                                 const void *pResData,
                                 const void *pData)
   {
      RWS_FUNCTION("RWS::CResourceManager::Update");

      RWS_PRE(pStrType);

      // Find the resource handler registered with this type
      //
      CResourceHandler *pCResourceHandler = CResourceHandler::FindHandler(pStrType);

      if (pCResourceHandler)
      {
         pCResourceHandler->Update(pResData, pData, pStrType);
      }

      RWS_RETURNVOID();
   }
}
