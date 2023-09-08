/*****************************************************************************
*
* File :     CAttributeHandler.cpp
*
* Abstract : Provides functionality for receiving and processing attribute
*            commands
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

#ifdef _MSC_VER
#pragma warning (disable: 4786)
#endif

#include "cattributehandler.h"
#include "../macros/debugmacros.h"
#include "../misc/idfromstring.h"
#include <map>
#include <list>

#if defined(DOLPHIN) && defined(__MWERKS__)
using namespace std;
#endif

namespace RWS
{
   namespace
   {
      /*
      *
      *  Predicate for attribute handler map. Allows RWSGUID type to be used 
      *  as the key
      *
      */
      struct RWSGUIDless : std::binary_function<const RWSGUID, const RWSGUID, bool>
      {
         bool operator() (const RWSGUID x, const RWSGUID y) const
         {
            return (x) < (y);
         }
      };
      
      /**
      *
      *  A global map relating RenderWare Studio Workspace instance IDs to 
      *  CAttributeHandler pointers. We need the map in order to allow fast searching
      *  for an instance Id, fast searching is key to the operation of the sequencer.
      *
      */
      typedef std::map< RWSGUID , RWS::CAttributeHandler*, RWSGUIDless > Id2AttributeHandler_Map;

      Id2AttributeHandler_Map theMap;

      /**
      *  A global list of CAttributeHandlers added to allow CAttributeHandler::Purge
      *  to delete entities in reverse order to the creation order.
      */
      typedef std::list< RWS::CAttributeHandler* > CAttributeHandler_List;

      CAttributeHandler_List theList;
   }

   /**
   *
   *  RenderWare Studio Workspace fills out CAttributeDataChunk::type_ with 
   *  either a class-specific attribute command or one of the following chunk 
   *  type identifiers...
   *
   */

   enum ChunkType {
      CLASSID = 0x80000000,         /**<  This chunk identifies the target class for any commands
                                          that follow the chunk. [CLASSID][Size][String] */

      INSTANCEID = 0x40000000,      /**<  This chunk identifies the target instance for all 
                                          commands in the sequence. [INSTANCEID][Size][unsigned int]*/

      CREATECLASSID = 0x20000000    /**<  This chunk identifies the class that should be created/instanced
                                          [CREATECLASSID][Size][String] */
   };

   /**
   *
   *  \return Returns true if this is the terminating chunk in the sequence, otherwise false.
   *
   */

   bool CAttributeDataChunk::IsEndChunk() const
   {
      RWS_FUNCTION("RWS::CAttributeDataChunk::IsEndChunk");

      RWS_RETURN( 0 == size_ );
   }

   /**
   *
   *  \return Returns a pointer to the next data chunk in the sequence.
   *
   */

   const CAttributeDataChunk* CAttributeDataChunk::pNextChunk() const
   {
      RWS_FUNCTION("RWS::CAttributeDataChunk::pNextChunk");

      RWS_PRE(!IsEndChunk());

      RWS_RETURN( reinterpret_cast<CAttributeDataChunk*>(reinterpret_cast<RWS_DWORD>(this) + size_) );
   }

   /**
   *
   *  \return Returns true if the packet is to be used to create a new instance of a class.
   *
   */

   bool CAttributePacket::IsInstanceCreationPacket() const
   {
      RWS_FUNCTION("RWS::CAttributePacket::IsInstanceCreationPacket");

      // By convention, the chunk telling us that this is an instance
      // creation packet is the first in the sequence. However, we still
      // use an iterator to access it because the sequence COULD be empty.
      CAttributeDataChunkIterator chunkIt(firstChunk_);
      
      RWS_RETURN( !chunkIt.IsFinished() && CREATECLASSID == chunkIt->type_ );
   }

   /**
   *
   *  \return Returns the name of the class to be created (provided it's an instance creation packet).
   *
   */

   const char* CAttributePacket::GetNameOfClassToCreate() const
   {
      RWS_FUNCTION("RWS::CAttributePacket::GetNameOfClassToCreate");

      RWS_PRE(IsInstanceCreationPacket());

      CAttributeDataChunkIterator chunkIt(firstChunk_);

      RWS_RETURN( reinterpret_cast<const char*>(&chunkIt->data_) );
   }

   /**
   *
   *  All CAttributePackets contain a CAttributeHandler instance ID. In
   *  the case of an instance creation packet, it specifies the ID that is
   *  to be assigned to the new instance. For all other packets, it specifies
   *  the ID of an existing instance that is the target of the packet.
   *
   *  \return Returns the instance ID extracted from the CAttributePacket.
   */

   RWSGUID CAttributePacket::GetInstanceId() const
   {
      RWS_FUNCTION("RWS::CAttributePacket::GetInstanceId");

      // By convention, the chunk telling us the packet's instance ID can
      // appear anywhere in the sequence. The packet should contain exactly
      // one such chunk (though currently, we only check that it contains
      // AT LEAST one).
      bool found = false;
      RWSGUID result;

      CAttributeDataChunkIterator chunkIt(firstChunk_);
      while (!found)
      {
         RWS_ASSERT(!chunkIt.IsFinished(), "Packet does not contain an instance ID");

         if (INSTANCEID == chunkIt->type_)
         {
            result = *reinterpret_cast<const RWSGUID *>(&chunkIt->data_);            
            found = true;
         }

         ++chunkIt;
      }

      RWS_RETURN( result );
   }

   /**
   *
   *  \return Returns the ID of the command - the interpretation of this will
   *  depend upon the class that the command is directed.
   *
   */

   unsigned int CAttributeCommand::GetCommandId() const
   {
      RWS_FUNCTION("RWS::CAttributeCommand::GetCommandId");

      RWS_RETURN( *(reinterpret_cast<const unsigned int*>(&chunk_.type_)) );
   }

   /**
   *
   *  Constructs a CAttributeDataChunkIterator object that can be used
   *  to iterate over a sequence of CAttributeDataChunks.
   *
   *  \param begin The first chunk in the sequence
   *
   */

   CAttributeDataChunkIterator::CAttributeDataChunkIterator(const CAttributeDataChunk& begin)
      : pCurrChunk_(&begin) {}

   /**
   *
   *  \return Returns true when the iterator points just beyond the last chunk in
   *  the sequence, otherwise false.
   *
   */

   bool CAttributeDataChunkIterator::IsFinished() const
   {
      RWS_FUNCTION("RWS::CAttributeDataChunkIterator::IsFinished");

      RWS_RETURN( pCurrChunk_->IsEndChunk() );
   }

   /**
   *
   *  Advances the iterator to the next chunk
   *
   *  \return Returns a reference to the CAttributeDataChunkIterator.
   *
   */

   CAttributeDataChunkIterator& CAttributeDataChunkIterator::operator++()
   {
      RWS_PRE(!IsFinished());
      pCurrChunk_ = pCurrChunk_->pNextChunk();
      return *this;
   }

   /**
   *
   *  Dereferences the iterator, returning a reference to the current chunk.
   *
   *  \return Returns a reference to the current CAttributeDataChunk.
   *
   */

   const CAttributeDataChunk& CAttributeDataChunkIterator::operator*() const
   {
      RWS_PRE(!IsFinished());
      return *pCurrChunk_;
   }

   /**
   *
   *  Dereferences the iterator, returning a pointer to the current chunk.
   *
   *  \return Returns a reference to the current CAttributeDataChunk.
   *
   */

   const CAttributeDataChunk* CAttributeDataChunkIterator::operator->() const
   {
      RWS_PRE(!IsFinished());
      return pCurrChunk_;
   }

   /**
   *
   *  Constructs a CAttributeCommandIterator object. After construction, the
   *  iterator will point to the first attribute command in the packet that
   *  is directed at the target class. If there are no such commands, the
   *  iterator will be 'finished'.
   *
   *  \param packet        The attribute packet from which attribute commands are to be retrieved
   *  \param targetClass   The target class - only commands directed at the target class will be retrieved
   *
   *  \see IsFinished()
   */

   CAttributeCommandIterator::CAttributeCommandIterator(
      const CAttributePacket& packet,
      const RwChar *targetClass
      )
      :  chunkIt_(packet.firstChunk_),
         pTargetClass_(targetClass)
   {
      // Skip to the first attribute command - the only valid
      // states for this iterator are to point at an attribute
      // command for the target class or to be finished. We've
      // only just been constructed so there's no way we've
      // encountered any CLASSID chunks yet...
      SkipToNextAttributeCommand(false);
   }

   /**
   *
   *  \return True when the iterator points just beyond the last command in the sequence.
   *
   */

   bool CAttributeCommandIterator::IsFinished() const
   {
      return chunkIt_.IsFinished();
   }

   /**
   *
   *  Finds the first data chunk that is an attribute command directed at the target class.
   *  It is a client responsibility to specify whether the last class ID chunk encountered
   *  was for the target class.
   *
   *  \param lastClassIdChunkWasForTargetClass Flag used to indicate whether the CAttributeCommandIterator is
   *  already at an AttributeCommand, (lastClassIdChunkWasForTargetClass == true).
   *
   */

   void CAttributeCommandIterator::SkipToNextAttributeCommand( bool lastClassIdChunkWasForTargetClass )
   {
      RWS_FUNCTION("RWS::CAttributeCommandIterator::SkipToNextAttributeCommand");

      bool found = false;
      while (!chunkIt_.IsFinished() && !found)
      {
         switch (chunkIt_->type_)
         {
            case INSTANCEID:
            case CREATECLASSID:
               // Skip
               ++chunkIt_;
               break;
            case CLASSID:
               // Record whether it matches the target class and skip
               lastClassIdChunkWasForTargetClass = 
                  (0 == rwstrcmp(pTargetClass_, reinterpret_cast<const char*>(&chunkIt_->data_)));
               ++chunkIt_;
               break;
            default:
               // Must be an attribute command. If our target class was
               // the last CLASSID chunk encountered then we've found our
               // chunk. If not then skip.
               if (lastClassIdChunkWasForTargetClass) found = true;
               else ++chunkIt_;
               break;
         }
      }
      RWS_RETURNVOID();
   }

   /**
   *
   *  Advances the iterator to the next command
   *
   *  \return Returns a reference to the CAttributeCommandIterator.
   *
   */

   CAttributeCommandIterator& CAttributeCommandIterator::operator++()
   {
      RWS_PRE(!IsFinished());
      
      ++chunkIt_;

      // We WERE pointing to an attribute command for the target class,
      // but now we might not be, so skip until we find one again.
      SkipToNextAttributeCommand(true);

      return *this;
   }

   /**
   *
   *  Dereferences the iterator, returning a reference to the current command.
   *
   *  \return Returns a reference to the CAttributeCommand.
   *
   */

   const CAttributeCommand& CAttributeCommandIterator::operator*() const
   {
      RWS_PRE(!IsFinished());

      return *reinterpret_cast<const CAttributeCommand*>(&(*chunkIt_));
   }

   /**
   *
   *  Dereferences the iterator, returning a pointer to the current command.
   *
   *  \return Returns a reference to the CAttributeCommand.
   */

   const CAttributeCommand* CAttributeCommandIterator::operator->() const
   {
      RWS_PRE(!IsFinished());

      return reinterpret_cast<const CAttributeCommand*>(&(*chunkIt_));
   }

   /**
   *
   *  Deconstruct a CAttributeHandler object.
   *
   */
   
   CAttributeHandler::~CAttributeHandler()
   {
      RWS_FUNCTION("RWS::CAttributeHandler::~CAttributeHandler");

      Remove(m_InstanceId);

      RWS_RETURNVOID();
   }

   /**
   *
   *  Creates an entry in the global map linking an instanceId to an instance of a CAttributeHandler.
   *
   *  \param instanceId The instance Id of this CAttributeHandler.
   *
   */
   
   void CAttributeHandler::Add(const RWSGUID & instanceId)
   {
      RWS_FUNCTION("RWS::CAttributeHandler::Add");
      
      RWS_ASSERT( !Find(instanceId), "Instance ID (" << instanceId << ") already in use" );

      m_InstanceId = instanceId;

      theMap.insert(Id2AttributeHandler_Map::value_type(instanceId, this));

      theList.push_front (this);
      
      RWS_RETURNVOID();
   }

  /**
   *
   *  Delete the entry in the global map linking the specified instanceId to an instance of
   *  a CAttributeHandler.
   *
   *  \param instanceId The instance Id of this CAttributeHandler.
   *
   */

   void CAttributeHandler::Remove(const RWSGUID & instanceId)
   {
      RWS_FUNCTION("RWS::CAttributeHandler::Remove");
      
      CAttributeHandler *pCAttributeHandler = CAttributeHandler::Find(instanceId);

      if (pCAttributeHandler)
      {
         CAttributeHandler_List::iterator it = theList.begin();

         while (it != theList.end())
         {
            if (*it == pCAttributeHandler)
            {
               theList.erase(it);
               break;
            }

            it++;
         }

         theMap.erase(instanceId);
      }
      
      RWS_RETURNVOID();
   }

  /**
   *
   *  Searches the global map to find the specified instance of a CAttributeHandler, if the
   *  specified instanceId is found the pointer to the instance of the CAttributeHandler is
   *  returned, otherwise FindInMap returns 0.
   *
   *  \param instanceId The instance Id of this CAttributeHandler.
   *
   *  \return Returns a pointer to the instance of the CAttributeHandler or 0.
   *
   */

   CAttributeHandler* CAttributeHandler::Find(const RWSGUID & instanceId)
   {
      RWS_FUNCTION("RWS::CAttributeHandler::Find");

      CAttributeHandler* pResult = 0;
      const Id2AttributeHandler_Map::iterator it = theMap.find(instanceId);
      if (it != theMap.end()) pResult = it->second;
      
      RWS_RETURN( pResult );
   }


   /**
   *
   *  Deletes all instances of CAttributeHandler that have not been locked
   *  by calling CAttributeHandler::Lock. Deleted instances are removed from 
   *  the map that links an InstanceId to its associated CAttributeHandler.
   *
   *  \See CAttributeHandler::PurgeAll
   *  \See CAttributeHandler::Lock
   *  \See CAttributeHandler::UnLock
   *  \see CAttributeHandler::IsLocked
   *
   */
   void CAttributeHandler::Purge()
   {
      RWS_FUNCTION("RWS::CAttributeHandler::Purge");

      CAttributeHandler_List::iterator it = theList.begin();

      while (it != theList.end())
      {
         CAttributeHandler *pCAttributeHandler = *it;

         if (pCAttributeHandler->IsLocked())
         {
            it++;
         }
         else
         {
            const RWSGUID *pRWSGUID = pCAttributeHandler->InqInstanceId();

            if (pRWSGUID)
            {
               const Id2AttributeHandler_Map::iterator mapit = theMap.find(*pRWSGUID);
               
               if (mapit != theMap.end()) 
               {
                  theMap.erase(mapit);
               }
            }

            it = theList.erase(it);

            delete pCAttributeHandler;
         }
      }
      RWS_RETURNVOID();
   }

   /**
   *
   *  Deletes all instances of CAttributeHandler, even those that have been 
   *  locked by calling CAttributeHandler::Lock. Deleted instances are removed 
   *  from the map that links an InstanceId to its associated CAttributeHandler.
   *
   *  \See CAttributeHandler::Purge
   *  \See CAttributeHandler::Lock
   *  \See CAttributeHandler::UnLock
   *  \see CAttributeHandler::IsLocked
   *
   */
   void CAttributeHandler::PurgeAll()
   {
      RWS_FUNCTION("RWS::CAttributeHandler::PurgeAll");
      
      CAttributeHandler_List::iterator it = theList.begin();
      
      while (it != theList.end())
      {
         CAttributeHandler *pCAttributeHandler = *it;
         
         const RWSGUID *pRWSGUID = pCAttributeHandler->InqInstanceId();
         
         if (pRWSGUID)
         {
            const Id2AttributeHandler_Map::iterator mapit = theMap.find(*pRWSGUID);
            
            if (mapit != theMap.end()) 
            {
               theMap.erase(mapit);
            }
         }
         
         it = theList.erase(it);
         
         delete pCAttributeHandler;
      }
      RWS_RETURNVOID();
   }

#ifdef RWS_DESIGN
   RWS_IMPLEMENT_CLASSID(CAttributeHandler);
#endif

   /**
   *
   *  HandleAttributes Handle CAttributeHandler attributes, as a debugging tool all CAttributeHandler's
   *  contain a member variable called m_debug which can be used by any class derived from CAttributeHandler.
   *
   */
   void CAttributeHandler::HandleAttributes(const CAttributePacket& attr)
   {
#ifdef RWS_DESIGN
      CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(CAttributeHandler));

      while (!attrIt.IsFinished())
      {
         switch (attrIt->GetCommandId())
         {
            case CMD_SetDebug:
               if (attrIt->GetAs_RwUInt32())
               {
                  m_Flags |= uATTRIBUTEHANDLER_FLAG_DEBUG;
               }
               else
               {
                  m_Flags &= ~uATTRIBUTEHANDLER_FLAG_DEBUG;
               }
               break;
         }
         ++attrIt;
      }
#endif
   }

   /**
   *
   * This method Clones an existing CAttributePacket. The cloned CAttributePacket is allocated
   * using RwMalloc because of this the client is responsible for releasing this memory using RwFree.
   *  
   * \return Returns a pointer to the cloned CAttributePacket.
   */
   CAttributePacket *CAttributePacket::Clone(void) const
   {
      RWS_FUNCTION("RWS::CAttributePacket::Clone");

      CAttributeDataChunkIterator DatatChunkIttr(this->firstChunk_);

#ifdef _DEBUG
      // Check contents of packet, just displays the contents of the original packet.
      //
      CAttributeDataChunkIterator CheckIttr(this->firstChunk_); 

      while (!CheckIttr.IsFinished())
      {
         const CAttributeDataChunk *pCheckChunk = CheckIttr.GetDataChunk();

         switch (pCheckChunk->type_)
         {
            case INSTANCEID:
               RWS_TRACE("INSTANCEID "<<(reinterpret_cast<const RWSGUID>(&pCheckChunk->data_)));
            break;
            case CREATECLASSID:
               RWS_TRACE("CREATECLASSID "<<(reinterpret_cast<const RwChar*>(&pCheckChunk->data_)));
            break;
            case CLASSID:
               RWS_TRACE("CLASSID "<<(reinterpret_cast<const RwChar*>(&pCheckChunk->data_)));
            break;
            default:
               RWS_TRACE("PARAM "<<pCheckChunk->type_);
            break;
         }
         ++CheckIttr;
      }
#endif

      // DatatChunkIttr is just used to traverse over the data, but we need to know the size of it
      // so that we can copy it.
      //
      const void *pStart = static_cast<const void*>(DatatChunkIttr.GetDataChunk());
     
      while (!DatatChunkIttr.IsFinished())
      {
         ++DatatChunkIttr;
      }

      const void *pEnd = static_cast<const void*>(DatatChunkIttr.GetDataChunk());

      // pStart, and pEnd point to the start and end of the data, use these to calculate the size
      //
      RwUInt32 size = reinterpret_cast<RwUInt32>(pEnd) - reinterpret_cast<RwUInt32>(pStart);

      // We need to include the terminate DataChunk
      // which is RWS_DWORD size_ = 0
      //
      size += sizeof(RWS_DWORD);

      CAttributePacket *pAttr = reinterpret_cast<CAttributePacket*>(RwMalloc(size, rwMEMHINTDUR_GLOBAL));

      RWS_ASSERT(pAttr, "Failed to allocate memory for attribute packet.");

#if defined(DOLPHIN) && defined(__MWERKS__)
      std::memcpy(pAttr, pStart, size);
#else
      memcpy(pAttr, pStart, size);
#endif

      RWS_TRACE("Total size of Cloned Packet "<<size);

      RWS_RETURN (pAttr);
   }

   /**
   *
   * This method updates a cloned CAttributePacket based on the data contained within an update
   * CAttributePacket, note the original clone is freed using RwFree (therefore it must have
   * been allocated with RwMalloc see CAttributePacket::Clone).
   *
   * \param rAttr A reference to the update CAttributePacket.
   *
   * \return Returns a pointer to an updated CAttributePacket.
   */
   CAttributePacket *CAttributePacket::Update(const CAttributePacket &rAttr)
   {
      RWS_FUNCTION("RWS::CAttributePacket::Update");
      
      CAttributePacket *pAttr = 0;
      
      // First determine the size of the updated attribute packet
      //
      
      // This is tricky as it requires in depth knowledge of the packet structure
      // used by CAttributePacket, be careful with any modifications.
      //
      {
         // The original attribute packet, (Must have been cloned)
         //
         CAttributeDataChunkIterator CloneIttr(this->firstChunk_);
         
         RwUInt32 size = 0;   // Size of new Attribute packet
         
         const RwChar *CloneClassId = 0; // Name of class, attributes are for this class
         
         RwBool copyClone;  // flag indicates whether data is taken from original or update. 
         
         while (!CloneIttr.IsFinished())
         {
            const CAttributeDataChunk *pCloneChunk = CloneIttr.GetDataChunk();
            const CAttributeDataChunk *pUpdateChunk = 0;
            
            // Assume we will be copying from the original.
            //
            copyClone = true;
            
            switch (pCloneChunk->type_)
            {
            case INSTANCEID:
               RWS_TRACE("INSTANCEID "<<(reinterpret_cast<const RWSGUID>(&pCloneChunk->data_)));
               break;
            case CREATECLASSID:
               RWS_TRACE("CREATECLASSID "<<(reinterpret_cast<const RwChar*>(&pCloneChunk->data_)));
               break;
            case CLASSID:
               RWS_TRACE("CLASSID "<<(reinterpret_cast<const RwChar*>(&pCloneChunk->data_)));
               CloneClassId = reinterpret_cast<const RwChar*>(&pCloneChunk->data_);
               break;
            default:
               RWS_TRACE("PARAM "<<pCloneChunk->type_);
               // Check update attribute packet, does it replace original packet ?
               //
               if (CloneClassId)
               {
                  // The update attribute packet, data from this is used to
                  //
                  CAttributeCommandIterator UpdatekIttr(rAttr, CloneClassId);
                  // update the original attribute packet.
                  while (!UpdatekIttr.IsFinished())
                  {
                     // Match original attribute to update attribute, if true copy the updated attribute
                     // not the original.
                     //
                     if (UpdatekIttr->GetCommandId() == pCloneChunk->type_)
                     {
                        pUpdateChunk = UpdatekIttr.GetDataChunk();
                        
                        // Update contains newer data, flag that we should copy from the update
                        //
                        copyClone = false;
                        break;
                     }
                     ++UpdatekIttr;
                  }
               }
               break;
            }
            
            // If copyClone is true we need to copy the data from the original cloned
            // attribute packet.
            //
            if (copyClone)
            {
               size = size + pCloneChunk->size_;
            }
            else
            {
               size = size + pUpdateChunk->size_;
            }
            
            ++CloneIttr;
         }
         
         // Need to add terminator to end of new attribute packet, size = 0, type = not_required,
         // data = not_required
         //
         const RwUInt32 TerminatorSize = sizeof(RWS_DWORD);
         size = size + TerminatorSize;
         
         RWS_TRACE("Total size of Updated Packet "<<size);
         
         pAttr = reinterpret_cast<CAttributePacket*>(RwMalloc(size, rwMEMHINTDUR_GLOBAL ));
         
         RWS_ASSERT(pAttr, "Unable to allocate memory for updated attribute packet");
      }

      /*==================================================================================================*/
      
      // Now do the copy of the attribute data.
      //
      //
      {
         // The original attribute packet, (Must have been cloned)
         //
         CAttributeDataChunkIterator CloneIttr(this->firstChunk_);
         
         RwUInt32 size = 0;   // Size of new Attribute packet
         
         const RwChar *CloneClassId = 0; // Name of class, attributes are for this class
         
         RwBool copyClone;  // flag indicates whether data is taken from original or update. 
         
         while (!CloneIttr.IsFinished())
         {
            const CAttributeDataChunk *pCloneChunk = CloneIttr.GetDataChunk();
            const CAttributeDataChunk *pUpdateChunk = 0;
            
            // Assume we will be copying from the original.
            //
            copyClone = true;
            
            switch (pCloneChunk->type_)
            {
            case INSTANCEID:
               RWS_TRACE("INSTANCEID "<<(reinterpret_cast<const RWSGUID>(&pCloneChunk->data_)));
               break;
            case CREATECLASSID:
               RWS_TRACE("CREATECLASSID "<<(reinterpret_cast<const RwChar*>(&pCloneChunk->data_)));
               break;
            case CLASSID:
               RWS_TRACE("CLASSID "<<(reinterpret_cast<const RwChar*>(&pCloneChunk->data_)));
               CloneClassId = reinterpret_cast<const RwChar*>(&pCloneChunk->data_);
               break;
            default:
               RWS_TRACE("PARAM "<<pCloneChunk->type_);
               // Check update attribute packet, does it replace original packet ?
               //
               if (CloneClassId)
               {
                  // The update attribute packet, data from this is used to
                  //
                  CAttributeCommandIterator UpdatekIttr(rAttr, CloneClassId);
                  // update the original attribute packet.
                  while (!UpdatekIttr.IsFinished())
                  {
                     // Match original attribute to update attribute, if true copy the updated attribute
                     // not the original.
                     //
                     if (UpdatekIttr->GetCommandId() == pCloneChunk->type_)
                     {
                        pUpdateChunk = UpdatekIttr.GetDataChunk();
                        
                        // Update contains newer data, flag that we should copy from the update
                        //
                        copyClone = false;
                        break;
                     }
                     ++UpdatekIttr;
                  }
               }
               break;
            }
            
            // If copyClone is true we need to copy the data from the original cloned
            // attribute packet.
            //
            if (copyClone)
            {
               RWS_TRACE("Copying attribute from original.");
               
               const char *pSrc = reinterpret_cast<const char*>(pCloneChunk);
               char *pDst = reinterpret_cast<char*>(pAttr) + size;
               
#if defined(DOLPHIN) && defined(__MWERKS__)
               std::memcpy(const_cast<char*>(pDst), pSrc, size);
#else
               memcpy(const_cast<char*>(pDst), pSrc, pCloneChunk->size_);
#endif
               size = size + pCloneChunk->size_;
            }
            else
            {
               RWS_TRACE("Copying attribute from update.");
               
               const char *pSrc = reinterpret_cast<const char*>(pUpdateChunk);
               char *pDst = reinterpret_cast<char*>(pAttr) + size;
               
#if defined(DOLPHIN) && defined(__MWERKS__)
               std::memcpy(const_cast<char*>(pDst), pSrc, size);
#else
               memcpy(const_cast<char*>(pDst), pSrc, pUpdateChunk->size_);
#endif
               size = size + pUpdateChunk->size_;
            }
            
            ++CloneIttr;
         }
         
         // Need to add terminator to end of new attribute packet, size = 0, type = not_required,
         // data = not_required
         //
         {
            const RwUInt32 TerminatorSize = sizeof(RWS_DWORD);
            
            char *pDst = reinterpret_cast<char*>(pAttr) + size;
            
#if defined(DOLPHIN) && defined(__MWERKS__)
            std::memset(const_cast<char*>(pDst), 0, TerminatorSize);
#else
            memset(const_cast<char*>(pDst), 0, TerminatorSize);
#endif
            size = size + TerminatorSize;
         }
      }
      
      RwFree(this);        // Free old AttributePacket
      
      RWS_RETURN(pAttr);   // Return new AttributePacket
   }

   /**
   *
   * Creates a new entity from this attribute packet - must be a packet for this purpose.
   * If there is an entity with the GUID already in the system, it will be deleted first.
   *
   * \see RWS::CreateEntity
   * \return A pointer to the new entity's attribute handler or zero on failure.
   *
   */

   CAttributeHandler *CAttributePacket::CreateEntity(void) const
   {
      RWS_FUNCTION("RWS::CAttributePacket::CreateEntity");

      CAttributeHandler *pAttrHand = 0;

      // Get Instance Id
      //
      RWSGUID InstanceId = GetInstanceId();
      
      // Check if it is already in use, if it is release it and delete 
      // the AttributeHandler/Behavior
      //
      CAttributeHandler *pCAttributeHandler = CAttributeHandler::Find(InstanceId);
      delete pCAttributeHandler;

      const char *cclassname = GetNameOfClassToCreate();
      
      // Create a new instance of a class
      //
      RWS_TRACE("Create Instance Of ["<<cclassname<<"] Id ["<<InstanceId<<"]"); 
      
#ifdef RWS_DESIGN

      try
      {

#endif
         pAttrHand =  CClassFactory::MakeNew(cclassname, *this);
         
         if (pAttrHand)
         {
            pAttrHand->Add(InstanceId);
            CAttributeHandler::NewAttributeHandlerCreated();
         }
         
#ifdef RWS_DESIGN

      }
      catch(...)
      {
         // Placeholder for code that will send back the Id to the workspace for bug reporting...
         //
         RWS_DEBUGSTREAM("RWS::NULL::CreateEntity failed, entity Id "
            << InstanceId << " behavior type " << cclassname << std::endl);
      }

#endif
      
      RWS_RETURN(pAttrHand);
   }

   /**
   *
   * Builds an instance id chunk for the attribute packet into the passed in buffer.
   * If the buffer pointer is zero then does not write into it, but does still return
   * the size of the data that would have been written. If the buffer pointer is non-
   * zero then the data is written and size used returned. If buffer pointer is non-
   * zero the function will assume that the buffer pointed to is large enough to hold
   * the data being written.
   *
   * \param rGuid The GUID of the instance to select via the chunk.
   * \param pBuffer The buffer to write to or zero if size is being requested.
   * \return The number of bytes that were (or would be) used in the buffer.
   *
   */

   RwUInt32 CAttributePacket::BuildInstanceIdChunk(const RWSGUID &rGuid, void *pBuffer)
   {
      RWS_FUNCTION("RWS::CAttributePacket::BuildInstanceIdChunk");

      RwUInt32 size = 0;

      // In this case, size if fixed.

      size = 4 + 4 + 16;

      // Insert data...?

      if (pBuffer)
      {
         RwUInt32 *pData32 = reinterpret_cast<RwUInt32 *>(pBuffer);

         // Size of chunk.

         pData32[0] = size;

         // Chunk identifier.

         pData32[1] = INSTANCEID;

         // GUID data.

         pData32[2] = rGuid.m_Data[0];
         pData32[3] = rGuid.m_Data[1];
         pData32[4] = rGuid.m_Data[2];
         pData32[5] = rGuid.m_Data[3];
      }

      RWS_RETURN(size);
   }

   /**
   *
   * Builds a class id chunk for the attribute packet into the passed in buffer.
   * If the buffer pointer is zero then does not write into it, but does still return
   * the size of the data that would have been written. If the buffer pointer is non-
   * zero then the data is written and size used returned. If buffer pointer is non-
   * zero the function will assume that the buffer pointed to is large enough to hold
   * the data being written.
   *
   * \param pClassID The class ID to select via the chunk. MUST NOT BE ZERO and MUST
   * point to a valid (null terminated) string.
   * \param pBuffer The buffer to write to or zero if size is being requested.
   * \return The number of bytes that were (or would be) used in the buffer.
   *
   */

   RwUInt32 CAttributePacket::BuildClassIdChunk(const RwChar *pClassID,
      void *pBuffer)
   {
      RWS_FUNCTION("RWS::CAttributePacket::BuildClassIdChunk");
      RWS_ASSERT(pClassID, "Invalid class ID.");

      RwUInt32 size = 0;

      // Calculate the size needed and ensure it's a multiple of 4.

      size = 4 + 4 + rwstrlen(pClassID) + 1;

      size += size + 3;
      size -= size % 4;

      // Add data into buffer...?

      if (pBuffer)
      {
         RwUInt32 *pData32 = reinterpret_cast<RwUInt32 *>(pBuffer);

         // Insert chunk size.

         pData32[0] = size;

         // Insert chunk ID.

         pData32[1] = CLASSID;

         // Insert the string itself (class ID).

         rwstrcpy(reinterpret_cast<char *>(&pData32[2]), pClassID);
      }

      RWS_RETURN(size);
   }

   /**
   *
   * Builds a create class id chunk for the attribute packet into the passed in buffer.
   * If the buffer pointer is zero then does not write into it, but does still return
   * the size of the data that would have been written. If the buffer pointer is non-
   * zero then the data is written and size used returned. If buffer pointer is non-
   * zero the function will assume that the buffer pointed to is large enough to hold
   * the data being written.
   *
   * \param pClassID The class ID to create via the chunk. MUST NOT BE ZERO and MUST
   * point to a valid (null terminated) string.
   * \param pBuffer The buffer to write to or zero if size is being requested.
   * \return The number of bytes that were (or would be) used in the buffer.
   *
   */

   RwUInt32 CAttributePacket::BuildCreateClassIdChunk(const RwChar *pClassID,
      void *pBuffer)
   {
      RWS_FUNCTION("RWS::CAttributePacket::BuildCreateClassIdChunk");
      RWS_ASSERT(pClassID, "Invalid class ID.");

      RwUInt32 size = 0;

      // Calculate the size needed and ensure it's a multiple of 4.

      size = 4 + 4 + rwstrlen(pClassID) + 1;

      size += size + 3;
      size -= size % 4;

      // Add data into buffer...?

      if (pBuffer)
      {
         RwUInt32 *pData32 = reinterpret_cast<RwUInt32 *>(pBuffer);

         // Insert chunk size.

         pData32[0] = size;

         // Insert chunk ID.

         pData32[1] = CREATECLASSID;

         // Insert the string itself (class ID).

         rwstrcpy(reinterpret_cast<char *>(&pData32[2]), pClassID);
      }

      RWS_RETURN(size);
   }

   /**
   *
   * Builds a command data chunk for the attribute packet into the passed in buffer.
   * If the buffer pointer is zero then does not write into it, but does still return
   * the size of the data that would have been written. If the buffer pointer is non-
   * zero then the data is written and size used returned. If buffer pointer is non-
   * zero the function will assume that the buffer pointed to is large enough to hold
   * the data being written.
   *
   * NOTE: Data pointer can be zero, in this case space will be left for the data, but
   *       this 'space' will NOT be initialized in ANY way and will contain totally
   *       undefined values.
   *
   * ALSO: The data size will be aligned to a 4 byte boundary as some platforms require
   *       this alignment.
   *
   * \param commandID The command ID to attribute to send data to.
   * \param pData Pointer to the data to insert, can be zero.
   * \param dataSize The size of the data to insert - can be zero, needed (validly) for
   *        message link name updates.
   * \param pBuffer The buffer to write to or zero if size is being requested.
   * \param pDataOffset On exit the value this points to is set the the offset from the
   *        start of the chunk to the point where the data insertion started. If zero, 
   *        value is not returned.
   * \return The number of bytes that were (or would be) used in the buffer.
   *
   */

   RwUInt32 CAttributePacket::BuildCommandDataChunk(RwUInt32 commandID, const void *pData,
      RwUInt32 dataSize, void *pBuffer, RwUInt32 *pDataOffset)
   {
      RWS_FUNCTION("RWS::CAttributePacket::BuildCommandDataChunk");
      RWS_ASSERT(commandID != CLASSID, "Invalid command ID, clashes with system value.")
      RWS_ASSERT(commandID != INSTANCEID, "Invalid command ID, clashes with system value.")
      RWS_ASSERT(commandID != CREATECLASSID, "Invalid command ID, clashes with system value.")

      // NOTE: Do NOT check data size, as message link name updates can VALIDLY have a zero
      //       data size.

      RwUInt32 size = 0;

      // Calculate size of chunk, header only...

      size = 4 + 4;

      // Return this as offset to data, if requested...

      if (pDataOffset)
      {
         *pDataOffset = size;
      }

      // Add on data size (and align to 4 byte boundary).

      size += dataSize + 3;
      size -= size % 4;

      // Add data into buffer...?

      if (pBuffer)
      {
         RwUInt32 *pData32 = reinterpret_cast<RwUInt32 *>(pBuffer);

         // Insert chunk size.

         pData32[0] = size;

         // Insert command ID.

         pData32[1] = commandID;

         // Insert the string itself (class ID) - if specified.

         if (pData)
         {
            memcpy(&pData32[2], pData, dataSize);
         }
      }

      RWS_RETURN(size);
   }

   /**
   *
   * Builds a terminator chunk for the attribute packet into the passed in buffer.
   * If the buffer pointer is zero then does not write into it, but does still return
   * the size of the data that would have been written. If the buffer pointer is non-
   * zero then the data is written and size used returned. If buffer pointer is non-
   * zero the function will assume that the buffer pointed to is large enough to hold
   * the data being written.
   *
   * \param pBuffer The buffer to write to or zero if size is being requested.
   * \return The number of bytes that were (or would be) used in the buffer.
   *
   */
   
   RwUInt32 CAttributePacket::BuildTerminatorChunk(void *pBuffer)
   {
      RWS_FUNCTION("RWS::CAttributePacket::BuildCommandDataChunk");

      RwUInt32 size = 4;

      // Write into buffer...?

      if (pBuffer)
      {
         RwUInt32 *pData32 = reinterpret_cast<RwUInt32 *>(pBuffer);

         // Terminating block has a size (within the chunk header) of zero. The
         // actual size in the buffer is NOT zero.

         pData32[0] = 0;
      }

      RWS_RETURN(size);
   }
}
