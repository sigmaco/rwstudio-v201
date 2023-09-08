/****************************************************************************
*
* File :      CSeqTxAttrHand.h
*
* Abstract :  Transmission items and manager for the sequencer using the
*             attribute handler system.
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

#ifndef CSEQTX_ATTR_HAND_HEADER
#define CSEQTX_ATTR_HAND_HEADER

#include "framework\core\attributehandler\cattributehandler.h"
#include "framework\core\macros\debugmacros.h"

namespace RWS
{
   namespace SEQUENCE
   {
      /**
      *
      * Class pre-defines.
      *
      */

      class CSeqAttrHandSlot;
      class CSeqStack;
      class CSeqItem;

      /**
      *
      * Transmission manager used for building buffers of attribute packets to be
      * sent to entities via the attribute handler function.
      *
      */

      class CSeqAttrHandManager
      {
      public:
         
         CSeqAttrHandManager(CSeqStack &rSeqStack, RwUInt32 flags = 0);
         ~CSeqAttrHandManager(void);

         void FreeItems(void);
         void Send(void);
         CSeqAttrHandSlot *CreateSlot(CSeqItem &rSeqItem, RwUInt32 dataSize);
         CSeqStack *SeqStack(void) { return(m_pSeqStack); }
         RwUInt32 GetFlags(void) { return(m_flags); }

      private:
         
         class Entity;                             // Pre-define.
         
         Entity *m_pEntityRoot,
            *m_pActiveEntityRoot;
         CSeqStack *m_pSeqStack;
         RwUInt32 m_flags;
         
      public:

         void MakeEntityActive(Entity *pEntity)
         {
            pEntity->m_pNextActive = m_pActiveEntityRoot;
            m_pActiveEntityRoot = pEntity;
         }

         /**
         *
         * Points to the start of the class data within an entity buffer. All
         * slots holding attribute data for that class will be after this.
         *
         */

         class Class
         {
            friend class CSeqAttrHandManager;

         public:

            ~Class(void);

            void *GetCommandDataSlot(RwUInt32 commandID, RwUInt32 dataSize);
            void MakeHoldingEntityActive(void);
            Class *Next(void);
            void DisconnectFromHandler(void) { m_pEntity->DisconnectFromHandler(); }
            void ConnectToHandler(void) { m_pEntity->ConnectToHandler(); }
            CAttributeHandler *GetEntityAttrHandler(void) { return(m_pEntity->m_pAttrHandler); }
            const RwChar *GetName(void) { return(m_name); }

         private:

            Class(const RwChar *name, RwUInt32 size, Entity &rEntity, RwUInt32 memType);

            void Resize(RwUInt32 size);
            void ResetForAdding(void);
            void AddTerminator(void);
            void InitBuffForUse(void);
            RwUInt32 CalcMinBuffHeadAndTailSize(const char *name);

            RwUInt8 *m_pBuffer,                    /**< The buffer for the class attribute packet. */
               *m_pBuffIns;                        /**< The point in the buffer to insert data at. */
            RwUInt32 m_bufferSize;                 /**< Size of all the command packets in class. */
            Class *m_pNext,
               *m_pNextActive;
            RwChar *m_name;                        /**< Copy of class name. */
            CSeqAttrHandSlot *m_pSlotRoot,
               *m_pFirstActiveSlot;                /**< Points to first active (in address order) slot. */
            Entity *m_pEntity;                     /**< A pointer to the entity buffer this class
                                                        group is held in. */
            RwBool m_dataAdded;                    /**< Cleared during 'ResetForAdding'. Set when first
                                                        add occurs. At this point class should be added 
                                                        to the parent entity's active list. */
         };

      private:

         /**
         *
         * Holds the buffer for an entity, all classes and attributes for that
         * entity are held within this buffer.
         *
         */

         class Entity
         {
         public:

            Entity(void);
            ~Entity(void);

            Class *m_pClassRoot,                /**< Points to list of class items (no order). */
               *m_pFirstActiveClass;            /**< Points to first (in address order) active class. */
            Entity *m_pNext,
               *m_pNextActive;
            CAttributeHandler *m_pAttrHandler;  /**< The attribute handler for the entity. If zero,
                                                     need to reconnect. */

            CAttributeTablePostUpdateFunction m_pAttrTablePostUpdateFunc;  /**< The post update function
                                                                                for the top-most level
                                                                                behaviour which needs 
                                                                                it. May be zero if
                                                                                no behaviour needs
                                                                                it. */
            void *m_pAttrTableUpdateThis;                                  /**< If post update function
                                                                                is used, holds this
                                                                                pointer for the 
                                                                                top-most level 
                                                                                behaviour which uses 
                                                                                it. If zero, need to
                                                                                reconnect. */
            CSeqAttrHandManager *m_pManager;    /**< Pointer to manager holding entity buffer. */
            RWSGUID m_guid;                     /**< GUID of entity buffer is for. */
            RwBool m_dataAdded;                 /**< Cleared during the processing of the 'Send' function. */

            void AddToActiveList(void)
            {
               if (!m_dataAdded)
               {
                  m_pManager->MakeEntityActive(this);
                  m_dataAdded = TRUE;
               }
            }

            void DisconnectFromHandler(void)
            {
               // Blank pointers, indicates that the linkages are broken too.

               m_pAttrHandler = 0;
               m_pAttrTableUpdateThis = 0;
            }

            void ConnectToHandler(void)
            {
               RWS_FUNCTION("RWS::SEQUENCE::CSeqAttrHandManager::Entity::ConnectToHandler");

               if (m_pAttrHandler == 0)
               {
                  // Connect to attribute handler...

                  m_pAttrHandler = CAttributeHandler::Find(m_guid);
                  RWS_ASSERT(m_pAttrHandler, "Failed to find entity's attribute handler.");
               }

               if (m_pAttrTableUpdateThis == 0)
               {
                  // Get top-level call back function, needed (optionally) if using direct-setting
                  // on the attributes.

                  m_pAttrTablePostUpdateFunc = m_pAttrHandler->
                     GetAttrPostUpdateFunc(m_pAttrTableUpdateThis);
               }

               RWS_RETURNVOID();
            }
         };
      };

      /**
      *
      * Slot object used by the CSeqAttrHandManager class. Holds information
      * need to request operations from that manager and identify the manager
      * in question / the item being managed.
      *
      */

      class CSeqAttrHandSlot
      {
         friend class CSeqAttrHandManager;

      public:

         RwBool IsDirectSet(void);
         void *GetDirectDataSlot(void);
         void DirectUpdate(const void *pData);
         void *GetBuffDataSlot(RwUInt32 dataSize);
         CSeqAttrHandSlot *Next(void);
         void DisconnectFromHandler(void);

      private:

         CSeqAttrHandSlot(void);
         void ConnectToHandler(void);

         void *m_pAttrTableData;                               /**< If attr table is used,
                                                                    holds data pointer. */
         CAttributeTableEntryUpdateFunction m_pAttrTableFunc;  /**< If attr table is used, 
                                                                    holds callback func. */
         void *m_pAttrTableThis;                               /**< If attr table is used, 
                                                                    holds instance 'this'.
                                                                    This is set (always) if
                                                                    direct set enabled. */
         CSeqAttrHandManager::Class *m_pClass;     /**< The manager class group this slot is in. */
         CSeqAttrHandSlot *m_pNext,                /**< The next slot inside the buffer manager. */
            *m_pPrev;                              /**< The previous slot inside the buffer manager. */
         RwUInt32 m_commandID;                     /**< The command ID being used. */
         RwBool m_isHandlerConnected;              /**< Indicates if entity specific handlers are
                                                        connected or not. */
      };                                           
   }     // namespace SEQUENCE
}        // namespace RWS
#endif   // #ifndef CSEQTX_ATTR_HAND_HEADER
