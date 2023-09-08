 /*****************************************************************************
*
* File :     CMaterialEvent.h
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

#ifndef __CMaterialEvent_H__
#define __CMaterialEvent_H__

//////////////////////////////////////////////////////////////////
//
// RenderWare Includes
//
#include <rwcore.h>
#include <rpworld.h>

#ifdef _MSC_VER
#pragma comment (lib, "rwcore.lib")
#pragma comment (lib, "rpworld.lib")
#endif

namespace RWS
{
  /**
   *
   *  \ingroup CMaterialList
   *
   *  Helper functions for RenderWare Graphics RpMaterial object.
   *
   */
   namespace MaterialHelper
   {

      class  CMaterialListNode;
      struct CMaterialEventPlugin;

      /**
      *
      *  \ingroup CMaterialEvent
      *
      *  CMaterialList is used to store a list materials whose construction, destruction and copy events
      *  need to be monitored. 
      *  
      *
      */
      class  CMaterialList
      {
         private:
         CMaterialListNode  *m_Head;

         public:
         CMaterialList();

         void               DeleteList             ();
         void               DeleteFromMaterialList (const RpMaterial  *pMaterial);
         void               ForAllMaterials        (RpMaterialCallBack pCallback, void *pData);
         CMaterialListNode *FindNodeWithMaterial   (const RpMaterial  *pMaterial);
         RwBool             AddToList              (const RpMaterial  *pMaterial);
         void               DeleteNodeFromList     (CMaterialListNode *pDeleteNode);
      };

      /**
      *
      *  \ingroup CMaterialEvent
      *
      *  CMaterialEvent can be use to derive a class that can track the construction, copying and
      *  destruction of materials
      *
      */
      class CMaterialEvent
      {
         public:
         
            CMaterialEvent();
            virtual ~CMaterialEvent();

            static RwBool           MaterialEventPluginAttach       (void);
            static void             SetObjectToReceiveEvent         (RpMaterial *pMaterial, CMaterialEvent *pObject);
            CMaterialList           m_MatList;

         private:

            static void            *MaterialEventPlugin_Constructor (void *pObject, RwInt32 offsetInObject, RwInt32 sizeInObject);
            static void            *MaterialEventPlugin_Copy        (void *pDstObject, const void *pSrcObject, RwInt32 offset, RwInt32 size);
            static void            *MaterialEventPlugin_Destroy     (void *pObject, RwInt32 offset, RwInt32 size);
            static CMaterialEventPlugin  *GetPluginData             (RpMaterial &Material);
            virtual void            MaterialConstructEvent          (const RpMaterial *pMaterial) = 0;
            virtual void            MaterialCopyEvent               (const RpMaterial *pSrcMaterial, const RpMaterial *pDstMaterial) = 0;
            virtual void            MaterialDestroyEvent            (const RpMaterial *pMaterial) = 0;
      };
   }
   
} //namespace

#endif