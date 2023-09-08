/****************************************************************************
*
* File :      CSeqTxAttrHand.cpp
*
* Abstract :  Transmission items and manager for sequence system using the
*             Attribute Handler system.
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

#include "precomp.h"

// Left to its own devices, the MW linker will dead strip the registrar ctors, so...

#ifdef __MWERKS__
#pragma force_active on
#pragma exceptions off     // Turn off exceptions due to bug in MW Dwarf
#endif                     // Generator for PS2, present in CW 3.04

//////////////////////////////////////////////////////////////////
//
// System Includes
//
#if (!defined (SKY) && !defined (DOLPHIN))
#include <memory.h>
#endif

#if defined(DOLPHIN) && defined(__MWERKS__)
   using namespace std;
#endif


//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//

#include "framework\core\attributehandler\cattributehandler.h"
#include "cseqtxattrhand.h"
#include "cseqstack.h"

namespace RWS
{
   namespace SEQUENCE
   {
      ////////////////////////////////
      //
      // CSeqAttrHandSlot functions...
      //
      //

      /**
      *
      * Constructor - just initialize the object. All setup is performed via the 
      *               CSeqAttrHandManager object.
      *
      */

      CSeqAttrHandSlot::CSeqAttrHandSlot(void) :
         m_pClass(0),
         m_pNext(0),
         m_pPrev(0),
         m_isHandlerConnected(FALSE)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqAttrHandSlot::CSeqAttrHandSlot");
         RWS_RETURNVOID();
      }

      /**
      *
      * Returns true if the slot is direct set. This effects which function calls must be made
      * on the slot, ie (pseudo code)...
      *
      * if (pSlot->IsDirectSet())
      * {
      *    // See 'GetDirectDataSlot' for more code information.
      * }
      * else
      * {
      *    RwUInt32 *pCalcVal;
      *    
      *    pCalcVal = pSlot->GetBuffDataSlot(sizeof(RwUInt32));
      *    *pCalcVal = 10;
      * }
      *
      * VITAL: This function MUST be called before calling "GetDirectDataSlot",
      *        "DirectUpdate" or "GetBuffDataSlot". If you do not do this,
      *        templated entities will fail.
      */

      RwBool CSeqAttrHandSlot::IsDirectSet(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqAttrHandSlot::IsDirectSet");

         ConnectToHandler();
         RWS_RETURN(m_pAttrTableThis != 0);
      }

      /**
      *
      * Give the address of the data slot to write information into OR zero if the value must be
      * set via the 'DirectUpdate' function. However, in EITHER CASE the 'DirectUpdate' function
      * MUST BE CALLED otherwise the attribute handling system may fail. This function MUST ONLY
      * be called if the 'IsDirectSet' function returned TRUE. For example (pseudo code)...
      *
      * if (pSlot->IsDirectSet())
      * {
      *    pBuff = pSlot->GetDirectDataSlot();
      *
      *    if (pBuff)
      *    {
      *       // Direct setting into a buffer (optional flag set & function call).
      *
      *       *pBuff = ...code to calc value directly in / copy from variable...;
      *       pSlot->DirectUpdate(pBuff);
      *    }
      *    else
      *    {
      *       // Direct set via function call (optional flag set).
      *
      *       MyType calcVal = ...code to calc value directly in...;
      *
      *       pSlot->DirectUpdate(calcVal);  // Or path in variable holding value instead.
      *    }
      * }
      * else
      * ...
      *
      * NOTE: This system does no data size checking, if the slot is too small for the data you
      *       are attempting to write, there will be memory corruption within the target behaviour.
      *       This system is provided for maximum performance.
      *
      * VITAL: You must call "IsDirectSet" EACH TIME, before this call.
      *
      */

      void *CSeqAttrHandSlot::GetDirectDataSlot(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqAttrHandSlot::GetDirectDataSlot");
         RWS_RETURN(m_pAttrTableData);
      }

      /**
      *
      * Call this function AFTER data has been written to the pointer returned by the
      * 'GetDirectDataSlot'. If data is written THIS FUNCTION MUST BE CALLED otherwise the
      * attribute handler system may fail. If no data is written, this function MUST NOT be
      * called. The data pointer MUST be either a) the pointer returned by the
      * 'GetDirectDataSlot' function OR if that is zero - a pointer to a variable holding
      * the update value. If the pointer returned from 'GetDirectDataSlot' is zero, the 
      * pointer to this function CANNOT BE zero.
      *
      * VITAL: You must call "IsDirectSet" EACH TIME, before this call.
      *
      */

      void CSeqAttrHandSlot::DirectUpdate(const void *pData)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqAttrHandSlot::GetDirectDataSlot");
         RWS_ASSERT(pData, "Data pointer must be valid - updates may not attribute handler may fail.");

         // Call the update function.

         if (m_pAttrTableFunc)
         {
            m_pAttrTableFunc(m_pAttrTableThis, pData);
         }

         // Add the entity this slot is within to the active list, needed so that
         // at the end of all the updates, the 'post update function' will be called
         // (if one is specified).

         m_pClass->MakeHoldingEntityActive();

         RWS_RETURNVOID();
      }

      /**
      *
      * Calls the class object inside the buffer manager to return a buffer that
      * data can be written into. This will be at least the size requested.
      *
      * NOTE: The transmission manager is responsible for setting up the class object
      *       pointer inside this object (along with using next/prev to link into lists
      *       etc).
      *
      * ALSO: Buffer sizes will be up sized to the nearest 4-byte aligned size. This is
      *       needed for compatibility on some platforms.
      *
      * VITAL: You must call "IsDirectSet" EACH TIME, before this call.
      *
      */

      void *CSeqAttrHandSlot::GetBuffDataSlot(RwUInt32 dataSize)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqAttrHandSlot::AddData");
         RWS_ASSERT(m_pClass, "Class group pointer invalid.");

         // Align buffer data size and return slot to user.

         dataSize += 3;
         dataSize -= dataSize % 4;

         RWS_RETURN(m_pClass->GetCommandDataSlot(m_commandID, dataSize));
      }

      /**
      *
      * Return next slot in list or zero if none.
      *
      */

      CSeqAttrHandSlot *CSeqAttrHandSlot::Next(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqAttrHandSlot::Next");
         RWS_RETURN(m_pNext);
      }

      /**
      *
      * Causes the buffer to disconnect from the entity specific linkages. This is needed when
      * an entity is deleted (such as in a template).
      *
      */

      void CSeqAttrHandSlot::DisconnectFromHandler(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqAttrHandSlot::DisconnectFromHandler");

         m_pClass->DisconnectFromHandler();
         m_isHandlerConnected = FALSE;
         m_pAttrTableData = 0;
         m_pAttrTableFunc = 0;
         m_pAttrTableThis = 0;

         RWS_RETURNVOID();
      }

      /**
      *
      * Causes slot to connect itself to the entity specific handlers.
      *
      */

      void CSeqAttrHandSlot::ConnectToHandler(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqAttrHandSlot::ConnectToHandler");

         // Need to reconnect ?

         if (!m_isHandlerConnected)
         {
            // Reconnect class / entity.

            m_pClass->ConnectToHandler();

            // Reconnect attribute table data and this pointers (if possible)...

            // Does this slot support direct setting ?

            RwUInt32 numTableEntries = 0;
            CAttributeTableEntry *pTable;
            void *pAttrTableThis = 0;

            pTable = m_pClass->GetEntityAttrHandler()->GetAttributeTable(m_pClass->GetName(),
               numTableEntries, pAttrTableThis);

            if (pTable)
            {
               RWS_ASSERT(numTableEntries > m_commandID, "Command index for unknown attribute.");
               RWS_ASSERT(pAttrTableThis,
                  "Failed to obtain this pointer for attribute table - should NEVER happen.");

               // Table exists, is there an entry for this command ID ?

               if (pTable[m_commandID].m_dataOffset != RWS_CMD_DATA_BLANK ||
                  pTable[m_commandID].m_pFunc != RWS_CMD_FUNC_BLANK)
               {
                  // Valid entry, so store the table entry for later use. Need to calculate the memory
                  // locations of the flag and data FOR THIS SPECIFIC INSTANCE. This stops them being
                  // calculated each time...

                  if (pTable[m_commandID].m_dataOffset != RWS_CMD_DATA_BLANK)
                  {
                     // Because the 'this' pointer may have been adjusted, the offset applied needs to
                     // be applied to the data address too, or it will be incorrect.

                     m_pAttrTableData = reinterpret_cast<void *>(reinterpret_cast<RwUInt8 *>
                        (pAttrTableThis) + pTable[m_commandID].m_dataOffset + 
                        pTable[m_commandID].m_thisOffset);
                  }
                  else
                  {
                     m_pAttrTableData = 0;
                  }

                  if (pTable[m_commandID].m_pFunc != RWS_CMD_FUNC_BLANK)
                  {
                     m_pAttrTableFunc = pTable[m_commandID].m_pFunc;
                  }
                  else
                  {
                     m_pAttrTableFunc = 0;
                  }

                  // This pointer needs to be adjusted by offset.

                  m_pAttrTableThis = reinterpret_cast<void *>(reinterpret_cast<RwUInt8 *>
                     (pAttrTableThis) + pTable[m_commandID].m_thisOffset);

#ifdef RWS_DS_DEBUG

                  if (m_pAttrTableData)
                  {
                     if (m_pAttrTableFunc)
                     {
                        RWS_DEBUGSTREAM("Using direct-set table entry for [" << m_pClass->GetName() <<
                           "]" << "command ID [" << m_commandID << "] data pointer and function pointer" <<
                           std::endl);
                     }
                     else
                     {
                        RWS_DEBUGSTREAM("Using direct-set table entry for [" << m_pClass->GetName() <<
                           "]" << "command ID [" << m_commandID << "] data pointer only" << std::endl);
                     }
                  }
                  else
                  {
                     RWS_DEBUGSTREAM("Using direct-set table entry for [" << m_pClass->GetName() << "]" <<
                        "command ID [" << m_commandID << "] function pointer only" << std::endl);
                  }
#endif
               }
               else
               {
                  // Not a valid entry, ie this particular command ID (attribute) is not setup to use
                  // direct setting.

                  m_pAttrTableData = 0;
                  m_pAttrTableFunc = 0;
                  m_pAttrTableThis = 0;

#ifdef RWS_DS_DEBUG

                  RWS_DEBUGSTREAM_WARN("No direct-set table entry for [" << m_pClass->GetName() << "]" <<
                     "command ID [" << m_commandID << "]" << std::endl);
#endif
               }
            }
            else
            {
               // No table, so set pointer to zero.

               m_pAttrTableData = 0;
               m_pAttrTableFunc = 0;
               m_pAttrTableThis = 0;

#ifdef RWS_DS_DEBUG

               RWS_DEBUGSTREAM_WARN("No direct-set table for [" << m_pClass->GetName() << "]" << std::endl);
#endif
            }

            // Set connection flag.

            m_isHandlerConnected = TRUE;
         }

         RWS_RETURNVOID();
      }
      
      ///////////////////////////////////
      //
      // CSeqAttrHandManager functions...
      //
      //
      
      /**
      *
      * Constructor - just initialize ready for adding buffers.
      *
      */

      CSeqAttrHandManager::CSeqAttrHandManager(CSeqStack &rSeqStack, RwUInt32 flags) :
         m_pEntityRoot(0),
         m_pActiveEntityRoot(0),
         m_pSeqStack(&rSeqStack),
         m_flags(flags)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqAttrHandManager::CSeqAttrHandManager");
         RWS_RETURNVOID();
      }

      /**
      *
      * Destructor - clean up any buffers used, etc.
      *
      */

      CSeqAttrHandManager::~CSeqAttrHandManager(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqAttrHandManager::~CSeqAttrHandManager");

         FreeItems();

         RWS_RETURNVOID();
      }

      /**
      *
      * Frees all the internally held buffers out of the systems.
      *
      */

      void CSeqAttrHandManager::FreeItems(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqAttrHandManager::FreeItems");

         while (m_pEntityRoot)
         {
            Entity *pDelEntity = m_pEntityRoot;

            m_pEntityRoot = m_pEntityRoot->m_pNext;
            delete pDelEntity;
         }

         RWS_RETURNVOID();
      }

      /**
      *
      * Called to cause the transmission handler to send all it's active buffers
      * to the appropriate attribute handlers.
      *
      */

      void CSeqAttrHandManager::Send(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqAttrHandManager::Send");

         Entity *pCurEntity = m_pActiveEntityRoot;

         // Do each entity in turn...

         while (pCurEntity)
         {
            Class *pCurClass = pCurEntity->m_pFirstActiveClass;

            // Do each class in the entity...

            while (pCurClass)
            {
               RWS_ASSERT(pCurEntity->m_pAttrHandler, "Invalid attribute handler.");

               // Put terminator on end of buffer, since sending, all data must have
               // been added...

               pCurClass->AddTerminator();

               // Send data to attribute handler...

               pCurEntity->m_pAttrHandler->HandleAttributes(
                  *(reinterpret_cast<CAttributePacket *>((pCurClass->m_pBuffer))));

               // Reset buffer ready for next load of adds...

               pCurClass->ResetForAdding();

               // Do next one...

               pCurClass = pCurClass->m_pNextActive;
            }

            // Clear the 'active root' so can start next set of adds on next time step.

            pCurEntity->m_pFirstActiveClass = 0;
            pCurEntity->m_dataAdded = FALSE;

            // Call (if exists) the post update function for the entity.

            if (pCurEntity->m_pAttrTablePostUpdateFunc)
            {
               pCurEntity->m_pAttrTablePostUpdateFunc(pCurEntity->m_pAttrTableUpdateThis);
            }

            // Do next entity...

            pCurEntity = pCurEntity->m_pNextActive;
         }

         // Clear the active list.

         m_pActiveEntityRoot = 0;

         RWS_RETURNVOID();
      }

      /**
      *
      * Creates a new slot and returns a pointer to it. Asserts if it fails.
      *
      */

      CSeqAttrHandSlot *CSeqAttrHandManager::CreateSlot(CSeqItem &rSeqItem,
         RwUInt32 dataSize)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqAttrHandManager::CreateSlot");
         RWS_ASSERT(rSeqItem.SeqStack() == m_pSeqStack, "Item not in same stack as manager.");

         Entity *pCurEntity = m_pEntityRoot;

         // Does the necessary entity id exist ?

         while (pCurEntity)
         {
            if (pCurEntity->m_guid == rSeqItem.GuidRef())
            {
               break;
            }
            pCurEntity = pCurEntity->m_pNext;
         }

         // Found entity id ?

         if (pCurEntity == 0)
         {
            // No, so add a new one...

            pCurEntity = RWS_NEW_TYPE(m_pSeqStack->MemType()) Entity();
            RWS_ASSERT(pCurEntity, "Failed to create new entity item.");

            pCurEntity->m_pManager = this;
            pCurEntity->m_guid = rSeqItem.GuidRef();
            pCurEntity->m_dataAdded = FALSE;

            // NOTE: connections to specific entity are now made in the
            //       call to the slot's "ConnectToHandler" function below.

            // Link into the list @ the head.

            pCurEntity->m_pNext = m_pEntityRoot;
            m_pEntityRoot = pCurEntity;
         }

         // Does the necessary class id exist ?

         Class *pCurClass = pCurEntity->m_pClassRoot;

         while (pCurClass)
         {
            if (!rwstrcmp(pCurClass->m_name, rSeqItem.ClassName()))
            {
               break;
            }
            pCurClass = pCurClass->m_pNext;
         }

         // Found the class id ?

         if (!pCurClass)
         {
            // No, so add a new one of the requested size.

            pCurClass = RWS_NEW_TYPE(m_pSeqStack->MemType())
               Class(rSeqItem.ClassName(), dataSize, *pCurEntity, rSeqItem.SeqStack()->MemType());
            RWS_ASSERT(pCurClass, "Failed to create new class item.");

            // Link this into the list of classes in the entity, at head.

            pCurClass->m_pNext = pCurEntity->m_pClassRoot;
            pCurEntity->m_pClassRoot = pCurClass;
         }

         // Allocate a new slot...

         CSeqAttrHandSlot *pSlot = RWS_NEW_TYPE(m_pSeqStack->MemType())
            CSeqAttrHandSlot;
         RWS_ASSERT(pSlot, "Failed to allocate slot.");
         
         // Setup the new slot...

         pSlot->m_commandID = rSeqItem.CommandID();
         pSlot->m_pClass = pCurClass;

         // Add to the list in class (at head)...

         pSlot->m_pPrev = 0;
         pSlot->m_pNext = pCurClass->m_pSlotRoot;

         if (pCurClass->m_pSlotRoot)
         {
            pCurClass->m_pSlotRoot->m_pPrev = pSlot;
         }

         pCurClass->m_pSlotRoot = pSlot;

         // Connect the slot to it's handlers...

         pSlot->ConnectToHandler();

         RWS_RETURN(pSlot);
      }

      /////////////////////////////////////////
      //
      // CSeqAttrHandManager::Entity functions...
      //
      //

      /**
      *
      * Constructor - just initialises the object.
      *
      */

      CSeqAttrHandManager::Entity::Entity(void) :
         m_pClassRoot(0),
         m_pFirstActiveClass(0),
         m_pNext(0),
         m_pAttrHandler(0),
         m_pAttrTableUpdateThis(0),
         m_pManager(0)
      {
         RWS_FUNCTION("CSeqAttrHandManager::Entity::Entity");
         RWS_RETURNVOID();
      }

      /**
      *
      * Destructor - release sub-object.
      *
      */

      CSeqAttrHandManager::Entity::~Entity(void)
      {
         RWS_FUNCTION("CSeqAttrHandManager::Entity::~Entity");

         while (m_pClassRoot)
         {
            Class *pDelClass = m_pClassRoot;

            m_pClassRoot = m_pClassRoot->Next();
            delete pDelClass;
         }

         RWS_RETURNVOID();
      }

      //////////////////////////////////////////
      //
      // CSeqAttrHandManager::Class functions...
      //
      //

      /**
      *
      * Constructs a new class object for the via attribute handler transmission
      * manger. Stores copy of passed in name for future reference.
      *
      */

      CSeqAttrHandManager::Class::Class(const RwChar *name, RwUInt32 size,
         CSeqAttrHandManager::Entity &rEntity, RwUInt32 memType) :
         m_pBuffer(0),
         m_pBuffIns(0),
         m_bufferSize(0),
         m_pNext(0),
         m_pNextActive(0),
         m_pSlotRoot(0),
         m_pFirstActiveSlot(0),
         m_pEntity(&rEntity),
         m_dataAdded(FALSE)
      {
         RWS_FUNCTION("CSeqAttrHandManager::Class::Class");
         RWS_ASSERT(name, "Invalid name pointer.");

         m_name = RWS_NEW_TYPE(m_pEntity->m_pManager->
            SeqStack()->MemType()) RwChar[rwstrlen(name) + 1];
         RWS_ASSERT(m_name, "Failed to get memory to copy name with.");

         rwstrcpy(m_name, name);

         // Inflate size to cope with addition packets needed in all class buffers.
         // ASSUME can use a fake GUID here, as ASSUMING they are fixed size. NOTE:
         // if the buffer requested is zero, then no allocation will occur at this
         // point, the allocation & initialisation will occur in GetCommandDataSlot
         // when/if the buffer is first used.

         if (size != 0)
         {
            size += CalcMinBuffHeadAndTailSize(name);
         }

         // Allocate initial buffer to requested size, and prepare for use.

         Resize(size);
         ResetForAdding();

         RWS_RETURNVOID();
      }

      /**
      *
      * Destructor frees up any used memory & releases sub-objects.
      *
      */

      CSeqAttrHandManager::Class::~Class(void)
      {
         RWS_FUNCTION("CSeqAttrHandManager::Class::~Class");

         CSeqAttrHandSlot *pCurSlot = m_pSlotRoot;

         // Release sub-objects.

         while (pCurSlot)
         {
            CSeqAttrHandSlot *pDelSlot = pCurSlot;

            // No need to unlink slot as deleting all of them.

            pCurSlot = pCurSlot->Next();
            delete pDelSlot;
         }

         // Release memory used by this object.

         delete[] m_name;

         if (m_pBuffer)
         {
            delete[] m_pBuffer;
         }

         RWS_RETURNVOID();
      }

      /**
      *
      * Returns a pointer to the slot for command data of the object, resizing the
      * buffer if needed. The buffer will be AT LEAST the size requested. The
      * return pointer will be valid (as function asserts if it cannot resize).
      *
      * This function ASSUMES that if you call it you WILL be placing data into the
      * returned buffer & therefore makes the buffer active so it's contents WILL BE
      * SENT.
      *
      * NOTE: Do NOT KEEP the pointer returned, it must be obtained IMMEDIATELY before
      *       inserting data. It may become invalid after another call to this function
      *       or other functions within the CSeqAttrHandManager class.
      *
      */

      void *CSeqAttrHandManager::Class::GetCommandDataSlot(RwUInt32 commandID, RwUInt32 dataSize)
      {
         RWS_FUNCTION("CSeqAttrHandManager::Class::AddCommandData");

         // Check there is room for this...

         RwUInt32 sizeNeeded = CAttributePacket::BuildCommandDataChunk(
               commandID, 0, dataSize, 0, 0),
            used = m_pBuffIns - m_pBuffer,
            dataSlotOffset;
         RwUInt8 *pDataSlot;

         // Check to see if there is a buffer - if not pre-sized the first use of the buffer
         // will need to size it.

         if (used == 0)
         {
            sizeNeeded += CalcMinBuffHeadAndTailSize(m_name);
         }

         if (m_bufferSize < (sizeNeeded + used))
         {
            // Going to need at LEAST a terminator on the end, so add this
            // to the size to stop instant resize when terminator is added.

            Resize(sizeNeeded + used + CAttributePacket::BuildTerminatorChunk(0));
         }

         // If first use, need to perform an initialisation on the buffer (add entity & class IDs, etc).

         if (used == 0)
         {
            InitBuffForUse();
         }

         // Insert the data header, etc, but NOT the data - caller will do that.

         pDataSlot = m_pBuffIns;

         m_pBuffIns += CAttributePacket::BuildCommandDataChunk(
               commandID, 0, dataSize, m_pBuffIns, &dataSlotOffset);

         pDataSlot += dataSlotOffset;

         // This class is now 'active' as it has some data to be sent. Need to
         // ensure it is within the 'active' list for the entity...

         if (!m_dataAdded)
         {
            // First time data has been added since reset, so add at HEAD of the
            // Entity's active class list...

            m_dataAdded = TRUE;
            m_pNextActive = m_pEntity->m_pFirstActiveClass;
            m_pEntity->m_pFirstActiveClass = this;
         }

         // Make the holding entity active too.

         MakeHoldingEntityActive();

         RWS_RETURN(pDataSlot);
      }

      /**
      *
      * Adds (if first data add) the entity which holds this class, to the buffer manager's
      * active list.
      *
      */

      void CSeqAttrHandManager::Class::MakeHoldingEntityActive(void)
      {
         RWS_FUNCTION("CSeqAttrHandManager::Class::MakeHoldingEntityActive");

         m_pEntity->AddToActiveList();

         RWS_RETURNVOID();
      }

      /**
      *
      * Returns pointer to next item in list or zero if none.
      *
      */

      CSeqAttrHandManager::Class *CSeqAttrHandManager::Class::Next(void)
      {
         RWS_FUNCTION("CSeqAttrHandManager::Class::Next");
         RWS_RETURN(m_pNext);
      }

      /**
      *
      * Resizes the buffer to the new size. If the new size is smaller than the current
      * size, the request is ignored.
      *
      */

      void CSeqAttrHandManager::Class::Resize(RwUInt32 size)
      {
         RWS_FUNCTION("CSeqAttrHandManager::Class::Resize");

         // Is requested size actually larger ?

         if (size > m_bufferSize)
         {
            RwUInt8 *pNewBuff;

            // Emit warning about this.

            if ((m_pEntity->m_pManager->GetFlags() & SEQ_FLAG_BUFFER_WARNINGS) != 0)
            {
               RWS_DEBUGSTREAM_WARN("Resizing buffer for instance " <<
                  m_pEntity->m_guid << " class [" << m_name <<
                  "]. This is likely to cause memory fragmentation. "
                  "Also slow because of need to preserve memory contents." <<
                  std::endl);
            }

            // Yep, so need to resize. Create new buffer first...

            pNewBuff = RWS_NEW_TYPE(m_pEntity->m_pManager->
               SeqStack()->MemType()) RwUInt8[size];
            RWS_ASSERT(pNewBuff, "Failed to get new enlarged buffer.");

            // Was there an old buffer ?

            if (m_pBuffer)
            {
               // Has any of the buffer been used ?

               if (m_pBuffIns > m_pBuffer && m_pBuffIns != 0)
               {
                  RwUInt32 copySize = m_pBuffIns - m_pBuffer;

                  // Copy data from this to the new one, ONLY up to the insertion point. Also
                  // need to move insertion pointer into new buffer.

                  memcpy(pNewBuff, m_pBuffer, copySize);
                  m_pBuffIns = pNewBuff + copySize;
               }

               // Free up old buffer now data has been copied, if any.

               delete[] m_pBuffer;
            }

            // Use new pointer...

            m_pBuffer = pNewBuff;
            m_bufferSize = size;
         }

         RWS_RETURNVOID();
      }

      /**
      *
      * Resets the buffer ready for adding of command data / terminator. MUST be called
      * BEFORE calling either of these functions (AddCommandData or AddTerminator).
      *
      */

      void CSeqAttrHandManager::Class::ResetForAdding(void)
      {
         RWS_FUNCTION("CSeqAttrHandManager::Class::ResetForAdding");

         InitBuffForUse();
         m_dataAdded = FALSE;

         RWS_RETURNVOID();
      }

      /**
      *
      * Returns the size necessary (minimum) to hold the header and trailer chunks for a buffer.
      *
      */

      RwUInt32 CSeqAttrHandManager::Class::CalcMinBuffHeadAndTailSize(const char *name)
      {
         RWS_FUNCTION("CSeqAttrHandManager::Class::CalcMinBuffHeadAndTailSize");

         RWSGUID fakeGuid;
         RwUInt32 size = 0;

         size += CAttributePacket::BuildInstanceIdChunk(fakeGuid, 0);
         size += CAttributePacket::BuildClassIdChunk(name, 0);
         size += CAttributePacket::BuildTerminatorChunk(0);

         RWS_RETURN(size);
      }

      /**
      *
      * Initialize the buffer for use, adding the entity and class ID chunks to it.
      *
      */

      void CSeqAttrHandManager::Class::InitBuffForUse(void)
      {
         RWS_FUNCTION("CSeqAttrHandManager::Class::InitBuffForUse");

         // If buffer, then add in instance and class ID chunks.

         m_pBuffIns = m_pBuffer;
         
         if (m_pBuffer)
         {
            m_pBuffIns += CAttributePacket::BuildInstanceIdChunk(m_pEntity->m_guid, m_pBuffIns);
            m_pBuffIns += CAttributePacket::BuildClassIdChunk(m_name, m_pBuffIns);
         }

         RWS_RETURNVOID();
      }

      /**
      *
      * Adds an attribute packet terminator chunk to the buffer. If there isn't room in the
      * buffer then it will be resized to make room.
      *
      */

      void CSeqAttrHandManager::Class::AddTerminator(void)
      {
         RWS_FUNCTION("CSeqAttrHandManager::Class::AddTerminator");

         // Check there is room for this...

         RwUInt32 sizeNeeded = CAttributePacket::BuildTerminatorChunk(0),
            used = m_pBuffIns - m_pBuffer;

         if (m_bufferSize < (sizeNeeded + used))
         {
            Resize(sizeNeeded + used);
         }

         // Insert the terminator....

         m_pBuffIns += CAttributePacket::BuildTerminatorChunk(m_pBuffIns);

         RWS_RETURNVOID();
      }
   }     // namespace SEQUENCE
}        // namespace RWS
