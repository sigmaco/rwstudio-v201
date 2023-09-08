/*****************************************************************************
*
* File :     CMaterialEvent.h
*
* Abstract : Implementation of CMaterialEvent
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

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "cmaterialevent.h"
#include "framework/core/macros/debugmacros.h"


namespace RWS
{

   namespace MaterialHelper
   {

      struct CMaterialEventPlugin
      {
         CMaterialEvent  *m_ObjectToReceiveEvent;
      };            

      /**
      *
      *  \ingroup CMaterialEvent().
      *
      *  Filled in by ClumpCallForAllBaseMaterials().
      *  
      *
      */
      typedef struct s_ClumpCallForAllBaseMaterials_Info
      {
         void               *pCallBackData;   /**< Stores pointer to callback data used by AtomicCallForAllGeometry */
         RpMaterialCallBack MaterialCallBack; /**< Stores pointer to the callback that will be triggered from
                                                   AtomicCallForAllGeometry */
      }t_ClumpCallForAllBaseMaterials_Info;


      const RwUInt32 CMaterialEventPluginID = 0x1FE;

      RwUInt32 iCMaterialEventPlugin_Offset = 0;


      /**
      *
      *  \ingroup CMaterialEvent().
      *
      *  CMaterialListNode forms a node in a linked list of pointers to materials. A material  can only exist once
      *  in the list so m_RefCount is used to track how many times a material is contained in the list 
      *
      */
      class CMaterialListNode
      {
         private:

         // set CMaterialList to be a friend of CMaterialListNode
         friend class CMaterialList;

         CMaterialListNode      *m_Next;      
         CMaterialListNode      *m_Prev;
         const RpMaterial       *m_Material;
         RwInt32                 m_RefCount;

         //
         static RwUInt32         m_FreeListRefCount;
         static RwFreeList      *sm_pFreeList;
   
         public:
         CMaterialListNode();
         inline CMaterialListNode *GetNext()    {return m_Next;};   
         inline CMaterialListNode *GetPrev()    {return m_Prev;};   
         inline const RpMaterial  *GetMaterial(){return m_Material;};   
         inline RwInt32            GetRefCount(){return m_RefCount;};   

         static const RwUInt32     uiDefaultNumberOfMaterials;


         // overload new and delete to allocate from the freelist
         void *operator new(size_t size)
         {
            RWS_FUNCTION("RWS::MaterialHelper::CMaterialListNode::operator new");

            RWS_RETURN (RwFreeListAlloc(sm_pFreeList, rwMEMHINTDUR_GLOBAL));
         }

         void operator delete(void *pObj)
         {
            if (pObj)
            {
               RwFreeListFree(sm_pFreeList, pObj);
            }
         }

         // setup the free list
         static void OpenFreeList(RwUInt32 blockSize)
         {
            m_FreeListRefCount++;

            if (sm_pFreeList == 0)
            {
               sm_pFreeList = RwFreeListCreate(sizeof(CMaterialListNode), blockSize, 16, rwMEMHINTDUR_GLOBAL);
            }

         }

         // close the free list
         static void CloseFreeList(void)
         {
            m_FreeListRefCount--;

            if (m_FreeListRefCount == 0)
            {
               if (sm_pFreeList)
               {
                  RwFreeListDestroy(sm_pFreeList);
                  sm_pFreeList = 0;
               }
            }
         }
      };

      // the number of materials nodes to be pre allocated in the free list
      const RwUInt32 CMaterialListNode::uiDefaultNumberOfMaterials = 10;
      // reference count for the free list
      RwUInt32    CMaterialListNode::m_FreeListRefCount;
      // pointer to the CMaterialListNode free list
      RwFreeList *CMaterialListNode::sm_pFreeList;


      /**
      *
      * \ref CMaterialEvent(). Constructor for CMaterialEvent
      *
      * \see ~CMaterialEvent
      *
      */
      CMaterialEvent::CMaterialEvent()
      {
         RWS_FUNCTION("CMaterialEvent::CMaterialEvent");

         CMaterialListNode::OpenFreeList(CMaterialListNode::uiDefaultNumberOfMaterials);

         RWS_RETURNVOID();
      }

      /**
      *
      * \ref CMaterialEvent(). Destructor for CMaterialEvent
      *
      * \see CMaterialEvent
      *
      */
      CMaterialEvent::~CMaterialEvent()
      {
         RWS_FUNCTION("CMaterialEvent::~CMaterialEvent");

         CMaterialListNode::CloseFreeList();

         RWS_RETURNVOID();
      }
      
      /**
      *  Retrieves material plugin data.
      *
      *  \param Material Reference to an RpMaterial object.
      *
      *  \return A pointer to the CMaterialEventPlugin data.
      */
      CMaterialEventPlugin* CMaterialEvent::GetPluginData( RpMaterial& Material )
      {
         RWS_FUNCTION("CMaterialEvent::GetPluginData");

         RWS_PRE(iCMaterialEventPlugin_Offset);

         const RwUInt32 baseAddr = reinterpret_cast<RwUInt32>(&Material);
         const RwUInt32 pluginAddr = baseAddr + iCMaterialEventPlugin_Offset;

         RWS_RETURN(reinterpret_cast<CMaterialEventPlugin*>(pluginAddr));
      }

      /**
      *  Constructor for CMaterialEvent data.
      *
      *  \param pObject A pointer to the object to be constructed, the RpMaterial.
      *  \param offsetInObject A byte offset from object pointer to plugin data.
      *  \param sizeInObject The size of plugin data.
      *
      *  \returns Returns pointer to the constructed object.
      */
      void* CMaterialEvent::MaterialEventPlugin_Constructor(void* pObject,
         RwInt32 offsetInObject, RwInt32 sizeInObject)
      {
         RWS_FUNCTION("CMaterialEvent::MaterialEventPlugin_Constructor");

         RWS_PRE(pObject);

         CMaterialEventPlugin &pluginData = *GetPluginData(*(RpMaterial*)pObject);

         pluginData.m_ObjectToReceiveEvent = 0;
         RWS_RETURN(pObject);
      }


      /**
      *  Copy operator for CMaterialEvent.
      *
      *  \param pDstObject A void pointer to the destination object, the source RpMaterial.
      *  \param pSrcObject A void pointer to the source object, the destination RpMaterial.
      *  \param offset A byte offset from object pointers to plugin data.
      *  \param size The size of plugin data to copy.
      *
      *  \return Returns pDstObject.
      */
      void* CMaterialEvent::MaterialEventPlugin_Copy(void* pDstObject,
         const void* pSrcObject, RwInt32 offset, RwInt32 size)
      {
         RWS_FUNCTION("CMaterialEvent::MaterialEventPlugin_Copy");

         RWS_PRE(pDstObject);
         RWS_PRE(pSrcObject);

         CMaterialEventPlugin& srcData = *GetPluginData(*(RpMaterial*)pSrcObject);
         CMaterialEventPlugin& dstData = *GetPluginData(*(RpMaterial*)pDstObject);

         dstData = srcData;

         // tell the associated object that the material has been copied
         if (srcData.m_ObjectToReceiveEvent)
         {
            srcData.m_ObjectToReceiveEvent->MaterialCopyEvent(reinterpret_cast<const RpMaterial*>(pSrcObject),
               reinterpret_cast<const RpMaterial*>(pDstObject));
         }

         RWS_RETURN(pDstObject);
      }

      /**
      *  Destroy operator for CMaterialEvent.
      *
      *  \param pObject A void pointer to the RpMaterial object.
      *  \param offset A byte offset from object pointers to plugin data.
      *  \param size The size of plugin data to copy.
      *
      *  \return Returns pObject.
      */
      void* CMaterialEvent::MaterialEventPlugin_Destroy(void *pObject, RwInt32 offset, RwInt32 size)
      {
         RWS_FUNCTION("CMaterialEvent::MaterialEventPlugin_Destroy");

         CMaterialEventPlugin& pluginData = *GetPluginData(*(RpMaterial*)pObject);

         // tell the associated object that the material destructor has been called
         if (pluginData.m_ObjectToReceiveEvent)
         {
            pluginData.m_ObjectToReceiveEvent->MaterialDestroyEvent(reinterpret_cast<const RpMaterial*>(pObject));
         }

         RWS_RETURN(pObject);
      }

      /**
      *  Attaches the CMaterialEvent plugin - this must happen AFTER the 
      *  RenderWare engine is initialized but BEFORE it is opened. 
      *  Returns FALSE if the plugin fails to attach.
      *
      *  \returns TRUE if successful otherwise FALSE.
      */
      RwBool CMaterialEvent::MaterialEventPluginAttach(void)
      {
         RWS_FUNCTION("CMaterialEvent::MaterialEventPluginAttach");

         RWS_PRE( !iCMaterialEventPlugin_Offset );

         iCMaterialEventPlugin_Offset = RpMaterialRegisterPlugin (
               sizeof(CMaterialEventPlugin),
               MAKECHUNKID(rwVENDORID_CRITERIONRM, CMaterialEventPluginID),
               MaterialEventPlugin_Constructor,  
               MaterialEventPlugin_Destroy,
               MaterialEventPlugin_Copy
               );

         RWS_POST(iCMaterialEventPlugin_Offset);

         RWS_RETURN(0 != iCMaterialEventPlugin_Offset);
      }

      /**
      *
      * \ref SetInformObject. Creates a new node in the list pointed to by pHeadNode and adds pMaterial to it. 
      * 
      * \param pMaterial    Pointer to a material who's plugin data is to receive a pointer to pObject.
      * \param pObject      Pointer to an object derived from CMaterialEvent that contains the construction,
      *                     copy and destruction callbacks. 
      *
      * \return void
      *
      * \see CMaterialListNode::DeleteList(), CMaterialListNode::DeleteList().
      *
      */
      void CMaterialEvent::SetObjectToReceiveEvent(RpMaterial *pMaterial, CMaterialEvent *pObject)
      {
         RWS_FUNCTION("CMaterialEvent::SetObjectToReceiveEvent");

         CMaterialEventPlugin& pluginData = *GetPluginData(*(RpMaterial*)pMaterial);
         pluginData.m_ObjectToReceiveEvent  = pObject;

         RWS_RETURNVOID();
      }

      /**
      *
      * \ref CMaterialListNode(). Constructor for CMaterialListNode
      *
      * \see ~CMaterialListNode
      *
      */
      CMaterialListNode::CMaterialListNode()
      {
         RWS_FUNCTION("CMaterialListNode::CMaterialListNode");

         m_Next      = 0;
         m_Prev      = 0;
         m_Material  = 0;
         m_RefCount  = 0;

         RWS_RETURNVOID();
      }

      /**
      *
      * \ref DeleteNode.    Deletes pDeleteNode from the list pointed to by pHeadNode. 
      * 
      * \param pHeadNode    Pointer to the pointer of the head of the list.
      * \param pDeleteNode  Pointer to a node in the list to delete.
      *
      * \return void
      *
      * \see CMaterialListNode::DeleteList(), CMaterialListNode::NewNode.
      *
      */
      void CMaterialList::DeleteNodeFromList(CMaterialListNode *pDeleteNode)
      {
         RWS_FUNCTION("CMaterialList::DeleteNodeFromList");

         CMaterialListNode *NodeToDelete = pDeleteNode;

         pDeleteNode->m_RefCount--;

         if (pDeleteNode->m_RefCount>0)
         {
            RWS_RETURNVOID();
         }

         if (pDeleteNode->m_Prev == 0 && pDeleteNode->m_Next == 0)
         {
            // the list only contains one node
            m_Head = 0;
         }
            else
         {
            if (pDeleteNode->m_Prev == 0 && pDeleteNode->m_Next != 0)
            {
               // we are at the start of the list
               (pDeleteNode->m_Next)->m_Prev = 0;
               m_Head = pDeleteNode->m_Next;
            }
               else
            {
               if (pDeleteNode->m_Prev != 0 && pDeleteNode->m_Next == 0)
               {
                  // we are at the end of the list
                  (pDeleteNode->m_Prev)->m_Next = 0;
               }
                  else
               {
                  // we are in the middle of the list
                  (pDeleteNode->m_Prev)->m_Next = pDeleteNode->m_Next;
                  (pDeleteNode->m_Next)->m_Prev = pDeleteNode->m_Prev;
               }
            }
         
            delete NodeToDelete;
         }

         RWS_RETURNVOID();
      }

      /**
      *
      * \ref CMaterialList(). Constructor for CMaterialList
      *
      *
      */
      CMaterialList::CMaterialList() : m_Head(0)
      {
         RWS_FUNCTION("CMaterialList::CMaterialList");

         RWS_RETURNVOID();
      }

      /**
      *
      * \ref DeleteList.    Deletes the entire list pointed to by pHeadNode.
      * 
      * \param pHeadNode    Pointer to the pointer of the head of the list.
      *
      * \return void
      *
      * \see CMaterialListNode::DeleteNode(), CMaterialListNode::NewNode.
      *
      */
      void CMaterialList::DeleteList()
      {
         RWS_FUNCTION("CMaterialList::DeleteList");

         CMaterialListNode *Current, *CurrentNext;

         Current = m_Head;

         while (Current)
         {
            CurrentNext = Current->GetNext();
            delete Current;
            Current = CurrentNext;
         }

         m_Head = 0;
      
         RWS_RETURNVOID();
      }


      /**
      *
      * \ref DeleteFromMaterialList. Searches the list for a node containing a pointer to pMaterial and
      *      deletes the node if a match is found.
      * 
      * \param pHeadNode    Pointer to the pointer of the head of the list.
      *
      * \return void
      *
      * \see CMaterialListNode::DeleteNode(), CMaterialListNode::NewNode, CMaterialList::DeleteList(),
      *      CMaterialList::FindNodeWithMaterial.
      *
      */
      void CMaterialList::DeleteFromMaterialList(const RpMaterial *pMaterial)
      {
         RWS_FUNCTION("CMaterialList::DeleteFromMaterialList");

         // find pMaterial in list of materials
         CMaterialListNode *MaterialNode;

         if ( (MaterialNode = FindNodeWithMaterial(pMaterial)))
         {
            // we have found pMaterial in the list so we must remove the reference to the material from the list
            DeleteNodeFromList(MaterialNode);
         }

         RWS_RETURNVOID();
      }

      /**
      * 
      * \ref ForAllMaterials.  Calls pCallback for every material in the list.
      *
      * \param pCallback    A RpMaterialCallBack callback function.
      * \param pData        Data to pass into the callback function.
      *
      * \return void
      *
      * \see CMaterialListNode::NewNode().
      * \see CMaterialListNode::DeleteMode().
      *
      */
      void CMaterialList::ForAllMaterials(RpMaterialCallBack pCallback, void *pData)
      {
         RWS_FUNCTION("CMaterialList::ForAllMaterials");

         CMaterialListNode *Next;

         Next = m_Head;
         while (Next)
         {
            pCallback(const_cast<RpMaterial*>(Next->GetMaterial()), pData);
            Next = Next->GetNext();
         }

         RWS_RETURNVOID();
      }

      /**
      *
      * \ref FindNodeWithMaterial. Searches the list for a node that contains a
      *  reference to pMaterial. If a reference is found then a pointer to the 
      *  node is returned. If no reference is found a null pointer will be returned.
      * 
      * \param pMaterial  Material to find.
      *
      * \return Pointer to node containing pMaterial.
      *
      * \see CMaterialListNode::NewNode().
      *
      */
      CMaterialListNode *CMaterialList::FindNodeWithMaterial(const RpMaterial *pMaterial)
      {
         RWS_FUNCTION("CMaterialList::FindNodeWithMaterial");

         CMaterialListNode *Next;

         Next = m_Head;
         while (Next)
         {
            if (Next->GetMaterial() == pMaterial)
            {
               RWS_RETURN(Next);
            }
            Next = Next->GetNext();
         }

         RWS_RETURN(0);
      }

      /**
      *
      * \ref AddToList.     Creates a new node in the list pointed to by pHeadNode and adds pMaterial to it. 
      * 
      * \param pHeadNode    Pointer to the pointer of the head of the list.
      * \param pMaterial    Pointer to a material to store in the list.
      *
      * \return True if pMaterial was succesfully added to the list.
      *
      * \see CMaterialListNode::DeleteList(), CMaterialList::DeleteNodeFromList,
      *  CMaterialList::DeleteFromMaterialList().
      *
      */
      RwBool CMaterialList::AddToList(const RpMaterial *pMaterial)
      {
         RWS_FUNCTION("CMaterialList::AddToList");

         // check to see if the material already exists in the list
         CMaterialListNode *MatNode;
         if ( (MatNode = FindNodeWithMaterial(pMaterial))  )
         {
            MatNode->m_RefCount++;
            RWS_RETURN(true);
         }

         // the material is not already in the list so we need to create a new node in the list.
         CMaterialListNode *NewListNode = new CMaterialListNode;
         if (!NewListNode)
         {
            RWS_ASSERT(NewListNode, "Failed to create new node in material list");
            RWS_RETURN(FALSE);
         }

         if (m_Head)
         {
            NewListNode->m_Next  = m_Head;
            (m_Head)->m_Prev     = NewListNode;
            m_Head               = NewListNode;
            NewListNode->m_Prev  = 0;
         }
            else
         {
            m_Head = NewListNode;
         }

         NewListNode->m_Material = pMaterial;
         NewListNode->m_RefCount++;
         RWS_RETURN(TRUE);
      }

      /**
      *
      * \ref AtomicCallForAllGeometry. Triggers a callback for each material associated with the atomics geometry
      *                                geometry. Note. This function is intended to be called from
      *                                ClumpCallForAllBaseMaterials.
      *                    
      * \param pAtomic        Pointer to an atomic.
      * \param pCallBackData  Pointer to a t_ClumpCallForAllBaseMaterials_Info structure containing a pointer for the
      *                       material callback and a pointer to the data to pass to the material callback.
      *
      * \return returns pAtomic
      *
      * \see 
      *  
      *
      */
      RpAtomic *AtomicCallForAllGeometry(RpAtomic *pAtomic, void *pCallBackData)
      {
         RWS_FUNCTION("RWS::MaterialHelper::CMaterialList::AtomicCallForAllGeometry");

         t_ClumpCallForAllBaseMaterials_Info *pInfo;

         pInfo = static_cast<t_ClumpCallForAllBaseMaterials_Info*>(pCallBackData);

         RpGeometry *pGeom;
         pGeom = RpAtomicGetGeometry( pAtomic );

         if (pGeom)
         { 
            RpGeometry *Res;
            Res = RpGeometryForAllMaterials (pGeom, pInfo->MaterialCallBack, pInfo->pCallBackData); 
            RWS_ASSERT(Res, "AtomicCallForAllGeometry failed");
         }

         RWS_RETURN( pAtomic );
      }
 

      /**
      *
      * \ref ClumpCallForAllBaseMaterials. Calls pMaterialCallBack for each material in the clump.
      *                    
      * \param pClump               Pointer to a clump who's materials we wish to process.
      * \param pCallBackData        Pointer to the data to be passed to pMaterialCallBack.
      * \param pMaterialCallBack    Pointer to the material callback to be called for each material in the clump.
      *
      * \return pClump
      *
      * \see 
      *  
      *
      */
      RpClump *ClumpCallForAllBaseMaterials(RpClump *pClump, void *pCallBackData, RpMaterialCallBack pMaterialCallBack)
      {
         RWS_FUNCTION("RWS::MaterialHelper::CMaterialList::ClumpCallForAllBaseMaterials");

         t_ClumpCallForAllBaseMaterials_Info Info;

         Info.pCallBackData    = pCallBackData;
         Info.MaterialCallBack = pMaterialCallBack;

         RpClump *Res;

         Res = RpClumpForAllAtomics (pClump, AtomicCallForAllGeometry, &Info);

         RWS_ASSERT(Res, "ClumpCallForAllBaseMaterials failed");

         RWS_RETURN( pClump );

      }

   } //namespace
} //namespace
