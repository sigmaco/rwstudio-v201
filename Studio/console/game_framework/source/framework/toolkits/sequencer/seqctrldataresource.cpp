/****************************************************************************
*
* File :     SeqCtrlDataResource.cpp
*
* Abstract : This file handles loading/unloading of RenderWareStudio
*            sequence resources which are all stored within RenderWare
*            streams.
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

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//

#include "framework/core/resourcemanager/cresourcemanager.h"
#include "seqctrldataresource.h"

namespace RWS
{
   /**
   *
   * Used to hold a linked list of all the sequence control data
   * resources loaded.
   *
   */

   CSeqCtrlDataNode *CSeqCtrlDataResource::sm_pDataRoot = 0;

   /**
   *
   * Constructor - registers the handler into the system (via sub-class).
   *
   */

   CSeqCtrlDataResource::CSeqCtrlDataResource(void)
   {
      RWS_FUNCTION("RWS::CSeqCtrlDataResource::CSeqCtrlDataResource");
      RWS_ASSERT(!sm_pDataRoot, "data root is not starting empty. Multiple instances?");
      RWS_RETURNVOID();
   }

   /**
   *
   * Destructor.
   *
   */

   CSeqCtrlDataResource::~CSeqCtrlDataResource(void)
   {
      RWS_FUNCTION("RWS::CSeqCtrlDataResource::~CSeqCtrlDataResource");

      // Ensure the system has been closed.

      Close();

      RWS_RETURNVOID();
   }

   /**
   *
   * Called when the resource manager is 'opened'.
   *
   */

   void CSeqCtrlDataResource::Open(void)
   {
      RWS_FUNCTION("RWS::CSeqCtrlDataResource::Open");

      // If items in node list, remove them.

      Empty();

      RWS_RETURNVOID();
   }

   /**
   *
   * Called when the resource manager is 'closed'.
   *
   */

   void CSeqCtrlDataResource::Close(void)
   {
      RWS_FUNCTION("RWS::CSeqCtrlDataResource::Close");

      // If items in node list, remove them.

      Empty();

      RWS_RETURNVOID();
   }

   /**
   *
   * Returns TRUE if the passed in type is an sequence control data resource,
   * otherwise returns FALSE.
   *
   */

   RwBool CSeqCtrlDataResource::IsHandled(const RwChar *psType)
   {
      RWS_FUNCTION("RWS::CSeqCtrlDataResource::IsHandled");

      if (!rwstrcmp(psType, SEQUENCE_DATA_RESOURCE_STRING))
      {
         RWS_RETURN(TRUE);
      }
      else
      {
         RWS_RETURN(FALSE);
      }
   }

   /**
   *
   * Loads the passed in resource into the sequence control data list. Returns pointer
   * to sequence control data object. If the load fails this pointer will zero.
   *
   */

   void *CSeqCtrlDataResource::Load(const RwChar *psName, const RwChar *psType,
      const RwChar *psResourcePath, RwStream* pStream, RwUInt32 uiStreamSize,
      RwUInt32 &uiResourceSize)
   {
      RWS_FUNCTION("RWS::CSeqCtrlDataResource::Load");
      RWS_ASSERT(pStream, "Stream pointer is invalid.");
      
      // Is there something to load?

      if (uiStreamSize == 0)
      {
         RWS_RETURN(0);
      }

      // Is it the correct type?

      if (IsHandled(psType))
      {
         CSeqCtrlData *pSeqData;
         CSeqCtrlDataNode *pSeqNode;

         // Proceed with load, need to copy all necessary data as incoming pointers
         // do NOT point to static data.

         pSeqData = RWS_NEW CSeqCtrlData(pStream, psName);
         RWS_ASSERT(pSeqData, "Failed to allocate sequence data object.");

         pSeqNode = RWS_NEW CSeqCtrlDataNode(*pSeqData);
         RWS_ASSERT(pSeqNode, "Failed to allocate sequence link-list node.");

         // Place the new node at the head of the link list.

         if (sm_pDataRoot)
         {
            pSeqNode->LinkTo(*sm_pDataRoot);
         }
         sm_pDataRoot = pSeqNode;
         uiResourceSize = sizeof(CSeqCtrlData);
         
         RWS_RETURN(static_cast<void *>(pSeqData));
      }

      RWS_RETURN(0);
   }

   /**
   *
   * Unloads the requested resource. Code assumes the pResource is the pointer returned
   * by the loading function.
   *
   */

   RwBool CSeqCtrlDataResource::UnLoad(const RwChar *pStrType, void *pResource)
   {
      RWS_FUNCTION("RWS::CSeqCtrlDataResource::UnLoad");

      // This type of resource ?

      if (IsHandled(pStrType))
      {
         CSeqCtrlDataNode *pNode = sm_pDataRoot,
            *pLast = 0;
         CSeqCtrlData *pRefData = static_cast<CSeqCtrlData *>(pResource);

         // Find a resource with the same address in the link list...

         while (pNode)
         {
            if (pNode->Data() == pRefData)
            {
               // Found item to unload, so remove from list and delete it.

               if (pLast)
               {
                  pLast->UnLink();

                  if (pNode->Next())
                  {
                     pLast->LinkTo(*pNode->UnLink());
                  }
               }
               else
               {
                  sm_pDataRoot = pNode->UnLink();
               }
               delete pNode->Data();
               delete pNode;

               RWS_RETURN(TRUE);
            }
            
            pLast = pNode;
            pNode = pNode->Next();
         }
      }

      RWS_RETURN(FALSE);
   }
   
   /**
   *
   * Called to provide live update of information - this resource does not support this.
   *
   */

   void CSeqCtrlDataResource::Update(const void *pResData, const void *pData, const RwChar *pStrType)
   {
      RWS_FUNCTION("RWS::CSeqCtrlDataResource::Update");
      RWS_RETURNVOID();
   }


   /**
   *
   * Static function to search the link list for the specified data object and
   * return a pointer to it or 0 if the object is not found. The name is the
   * resource's original name.
   *
   */

   CSeqCtrlData *CSeqCtrlDataResource::Find(const RwChar *psName)
   {
      RWS_FUNCTION("RWS::CSeqCtrlDataResource::Find");

      CSeqCtrlDataNode *pCur = sm_pDataRoot;

      while(pCur)
      {
         if (!rwstrcmp(pCur->Data()->Name(), psName))
         {
            RWS_RETURN(pCur->Data());
         }
         pCur = pCur->Next();
      }

      RWS_RETURN(0);
   }

   /**
   *
   * Empties the static list of data nodes.
   *
   */

   void CSeqCtrlDataResource::Empty(void)
   {
      RWS_FUNCTION("RWS::CSeqCtrlDataResource::Empty");

      while (sm_pDataRoot)
      {
         CSeqCtrlDataNode *pDel = sm_pDataRoot;

         sm_pDataRoot = sm_pDataRoot->Next();
         delete pDel->Data();
         delete pDel;
      }

      RWS_RETURNVOID();
   }

   /*
   *
   * Create an instance of the Sequence Control Data Resource handler, this is required
   * in order to register the handler.
   *
   */

   namespace
   {
      CSeqCtrlDataResource gCSeqCtrlDataResource;
   }
}     // namespace RWS
