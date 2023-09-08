/*****************************************************************************
*
* File :     CAttributeHandler.h
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

/**
*
*  \ingroup CAttributeHandler
*
*  \page CAttributeHandlerOverview Overview
*
*  The attribute handler forms one of the components of the console framework. 
*  This section describes some of the motivations and design decisions that 
*  have contributed to the development of this component.
*
*  Games development is a highly recursive activity, careful tuning is critical 
*  to producing high quality titles. Tuning typically involves modifying 
*  parameters or attributes of a particular behavior, testing and repeating 
*  the process. Games are often highly interconnected systems. This means that 
*  modifying the attributes of one game component has a knock on effect on 
*  another section of the game, which then requires further tuning to correct 
*  the problem.
*
*  Several methods are available to the developer for tuning their game but 
*  each of these is often time consuming, for example;
*
*  \li Having parameters hard coded requires the code to be re-compiled, 
*      re-downloaded and re-run each time an edit is made, parameters typically 
*      limited to global data not individual entities within a game level.
*
*  \li Parameters are sometimes stored in a text file loaded at run-time,
*      easier to edit than having the parameters stored in the code but 
*      parameters are still typically limited to global data not individual
*      entities within a game level.
*
*  \li Parameters modified in a custom game editor, typically requiring a 
*      time-consuming export process before the changes can be tested within 
*      the game.
*
*  The attribute handler provides a formalized method of initializing and 
*  modifying parameters of individual or global game data. The source of the 
*  parameter data is not determined by the attribute handler. The source could 
*  be a game data file, memory card, a network source or during development 
*  the network link to the RenderWare Studio Workspace. Using the link to the 
*  RenderWare Studio Workspace enables the developer to tune their game in 
*  real-time.
*
*  Another time consuming and often bottleneck activity for games developers 
*  is the creation of user interfaces within their game editor. The game editor 
*  typically being either a complete proprietary tool or a customized third 
*  party modelling package.
*
*  The RenderWare Studio Workspace in conjunction with the attribute handler 
*  enables the games programmer to design and build his or her own user 
*  interface from pre-built components very quickly. 
*
*  These components are specified effectively as comments with the game code 
*  thus keeping the code for the particular game component and definition of 
*  the user interface required to use that component together. Building a user 
*  interface from scratch or modifying it doesn’t require the RenderWare Studio 
*  Framework to be compiled. The RenderWare Studio Workspace scans the source 
*  code of the game extracting the definitions of the user interface thus 
*  enabling them to be displayed. Modifying an attribute within the RenderWare 
*  Studio Editor generates a network packet containing the commands used by 
*  the attribute handler to modify the attribute on the console while it is 
*  running.
*
*  The \ref RWS::CAttributeHandler class maintains the link between an instance 
*  of a behavior in the RenderWare Studio Workspace with the behavior
*  code on the console using Ids.
*
*/

#ifndef __CATTRIBUTEHANDLER_H__
#define __CATTRIBUTEHANDLER_H__

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "../streamhandler/strfunc.h"
#include "attributemacros.h"
#include "cclassfactory.h"
#include "../misc/rwsguid.h"

namespace RWS
{
   typedef void (*NewAttributeHandlerCallBack)(void);

   /**
   *
   *  \ingroup CAttributeHandler
   *
   *  Declares an ID for a class. RWS_DECLARE_CLASSID() should be placed in
   *  the public section of your class declaration (typically in your class's
   *  header file).
   *
   */

   #define RWS_DECLARE_CLASSID(className) static const RwChar* p##className##ClassID

   /**
   *
   *  \ingroup CAttributeHandler
   *
   *  Implements an ID for a class. RWS_IMPLEMENT_CLASSID() should typically be
   *  placed at global scope in your class's CPP file.
   *
   */
   #define RWS_IMPLEMENT_CLASSID(className) const RwChar* className::p##className##ClassID = #className

   /**
   *
   *  \ingroup CAttributeHandler
   *
   *  Retrieves the ID of a class.
   *
   */

   #define RWS_CLASSID_OF(className) (className::p##className##ClassID)

   // Definition of RWS_DWORD and RWS_FLOAT which are used within the workspace comms
   // code to build up attribute data packets.
   //
   typedef RwUInt32 RWS_DWORD;
   typedef RwReal RWS_FLOAT;

   // If this fails then the size of the defined type in bytes is incorrect, and the workspace
   // will be unable to communicate with the console.
   //
   extern const char check_RWS_DWORD_size[sizeof(RWS_DWORD) == 4];
   extern const char check_RWS_FLOAT_size[sizeof(RWS_FLOAT) == 4];

   /**
   *
   *  \ingroup CAttributeHandler
   *
   *  Encapsulates the format of a single chunk in a packet of 
   *  incoming attribute data. The data can be accessed directly 
   *  for maximum efficiency. The CAttributeDataChunkIterator class
   *  allows for iterating through all of the data chunks within a 
   *  given packet of attribute data.
   *
   */
   class CAttributeDataChunk
   {
   public:

      const RWS_DWORD size_;   /**< Total size of chunk + data  */
      const RWS_DWORD type_;   /**< Class-specific attribute command or chunk type identifier */
      const RWS_DWORD data_;   /**< First RWS_DWORD of data */

   private:

      friend class CAttributeDataChunkIterator;

      bool IsEndChunk() const;

      const CAttributeDataChunk* pNextChunk() const;

      // Revoked ctor/copy/assign - not a 'normal' constructable class
      CAttributeDataChunk();
      CAttributeDataChunk( const CAttributeDataChunk& );
      CAttributeDataChunk& operator = ( const CAttributeDataChunk& );
   };

   /**
   *
   *  \ingroup CAttributeHandler
   *
   *  A packet of incoming attribute data,
   *  effectively a sequence of CAttributeDataChunks. The ordering of the
   *  different chunks is expected to conform to certain conventions which
   *  are encapsulated and (to some extent) checked by CAttributePacket.
   *
   */

   class CAttributePacket
   {
   public:

      bool IsInstanceCreationPacket() const;

      const char* GetNameOfClassToCreate() const;

      RWSGUID GetInstanceId() const;

      CAttributePacket *Clone(void) const;

      CAttributePacket *Update(const CAttributePacket &rAttr);
      
      CAttributeHandler *CreateEntity(void) const;

      static RwUInt32 BuildInstanceIdChunk(const RWSGUID &rGuid, void *pBuffer);
      static RwUInt32 BuildClassIdChunk(const RwChar *pClassID, void *pBuffer);
      static RwUInt32 BuildCreateClassIdChunk(const RwChar *pClassID, void *pBuffer);
      static RwUInt32 BuildCommandDataChunk(RwUInt32 commandID, const void *pData,
         RwUInt32 dataSize, void *pBuffer, RwUInt32 *pDataOffset);
      static RwUInt32 BuildTerminatorChunk(void *pBuffer);

   private:

      // Revoked ctor/copy/assign - not a 'normal' constructable class
      CAttributePacket();
      CAttributePacket( const CAttributePacket& );
      CAttributePacket& operator = ( const CAttributePacket& );

      // Implementation
      friend class CAttributeCommandIterator;
      const CAttributeDataChunk firstChunk_;
   };

   /**
   *
   *  \ingroup CAttributeHandler
   *
   *  A single attribute command directed at an instance of a particular
   *  class. Consists of a command ID and some command-specific data.
   *  Multiple helper functions are provided for acessing the command data.
   *
   */

   class CAttributeCommand
   {
   public:

      unsigned int GetCommandId() const;

      /**
      *
      *  Template function to copy command data to an arbitrary type, for example:
      *
      *  \code
      
         RwUInt32 var1;
         RwReal var2;
         RwV3d var3

         cmd.GetCommandData(var1);  // Copy RwUInt32 into var1
         cmd.GetCommandData(var2);  // Copy RwReal into var2
         cmd.GetCommandData(var3);  // Copy RwV3d into var3

         \endcode
      *
      */

      template<class T> void GetCommandData(T &dest) const
      { dest = *(reinterpret_cast<const T*>(&chunk_.data_)); }

      /*
      *  Specialization of the above template to workaround a flaw when
      *  compiled with MetroWerks on Gamecube. A bool type is 8bits on MW,
      *  and because the Gamecube is big endian the pointer cast points to
      *  the MSB of chunk_.data_, rather than the LSB. So dest is set to the 
      *  wrong value.
      */
      void GetCommandData(bool &dest) const
      {
         const RwUInt32 temp = *(reinterpret_cast<const RwUInt32*>(&chunk_.data_));
         if (temp)
         {
            dest = true;
         }
         else
         {
            dest = false;
         }
      }

      /**
      *
      *  Template function to direct a pointer of arbitrary type to command data. Pass
      *  in the address of the pointer as the argument, for example:
      *
      *  \code

         const RwChar* pvar1;
         const RwUInt32* pvar2;
         const RwV3d* pvar3;
         
         cmd.GetCommandData(&pvar1);   // Copy pointer to RwChar into pvar1
         cmd.GetCommandData(&pvar2);   // Copy pointer to RwUInt32 into pvar1
         cmd.GetCommandData(&pvar3);   // Copy pointer to RwV3d into pvar1
      
         \endcode
      *
      */

      template<class T> void GetCommandData(T* dest) const
      { *dest = reinterpret_cast<T>(&chunk_.data_); }

      /*
      *  Specialization of the above template to workaround a flaw when
      *  compiled with MetroWerks on Gamecube. A bool type is 8bits on MW,
      *  and because the Gamecube is big endian the pointer cast points to
      *  the MSB of chunk_.data_, rather than the LSB. So dest is set to the 
      *  wrong value.
      */
      void GetCommandData(bool* dest) const
      {
         const RwUInt32 temp = *(reinterpret_cast<const RwUInt32*>(&chunk_.data_));
         if (temp)
         {
            *dest = true;
         }
         else
         {
            *dest = false;
         }
      }
      

      /**   Used to return void pointer to data */

      const void *GetAs_void_ptr(void) const {return reinterpret_cast<const void *>(&chunk_.data_);}

      /**   Used to return unsigned char pointer to data */

      const unsigned char *GetAs_unsigned_char_ptr(void) const
         {return reinterpret_cast<const unsigned char *>(&chunk_.data_);}

      /**   Used to return char pointer to data */

      const char *GetAs_char_ptr(void) const {return reinterpret_cast<const char *>(&chunk_.data_);}

      /**   Used to return unsigned short pointer to data */

      const unsigned short *GetAs_unsigned_short_ptr(void) const
         {return reinterpret_cast<const unsigned short *>(&chunk_.data_);}

      /**   Used to return short pointer to data */

      const short *GetAs_short_ptr(void) const {return reinterpret_cast<const short *>(&chunk_.data_);}

      /**   Used to return unsigned int pointer to data */

      const unsigned int *GetAs_unsigned_int_ptr(void) const
         {return reinterpret_cast<const unsigned int *>(&chunk_.data_);}

      /**   Used to return int pointer to data */

      const int *GetAs_int_ptr(void) const {return reinterpret_cast<const int *>(&chunk_.data_);}

      /**   Used to return unsigned long pointer to data */

      const unsigned long *GetAs_unsigned_long_ptr(void) const
         {return reinterpret_cast<const unsigned long *>(&chunk_.data_);}

      /**   Used to return long pointer to data */

      const long *GetAs_long_ptr(void) const {return reinterpret_cast<const long *>(&chunk_.data_);}

      /**   Used to return float pointer to data */

      const float *GetAs_float_ptr(void) const {return reinterpret_cast<const float *>(&chunk_.data_);}

      /**   Used to return double pointer to data */

      const double *GetAs_double_ptr(void) const {return reinterpret_cast<const double *>(&chunk_.data_);}

      /**   Used to return data as unsigned char */

      const unsigned char GetAs_unsigned_char(void) const
         {return *(reinterpret_cast<const unsigned char *>(&chunk_.data_));}

      /**   Used to return data as char */

      const char GetAs_char(void) const {return *(reinterpret_cast<const char *>(&chunk_.data_));}

      /**   Used to return data as unsigned short */

      const unsigned short GetAs_unsigned_short(void) const
         {return *(reinterpret_cast<const unsigned short *>(&chunk_.data_));}

      /**   Used to return data as short */

      const short GetAs_short(void) const {return *(reinterpret_cast<const short *>(&chunk_.data_));}

      /**   Used to return data as unsigned int */

      const unsigned int GetAs_unsigned_int(void) const
         {return *(reinterpret_cast<const unsigned int *>(&chunk_.data_));}

      /**   Used to return data as int */

      const int GetAs_int(void) const {return *(reinterpret_cast<const int *>(&chunk_.data_));}

      /**   Used to return data as unsigned long */

      const unsigned long GetAs_unsigned_long(void) const
         {return *(reinterpret_cast<const unsigned long *>(&chunk_.data_));}

      /**   Used to return data as long */

      const long GetAs_long(void) const
         {return *(reinterpret_cast<const long *>(&chunk_.data_));}

      /**   Used to return data as float */

      const float GetAs_float(void) const
         {return *(reinterpret_cast<const float *>(&chunk_.data_));}

      /**   Used to return data as double */

      const double GetAs_double(void) const
         {return *(reinterpret_cast<const double *>(&chunk_.data_));}

      /**   Used to return RwReal pointer to data */

      const RwReal *GetAs_RwReal_ptr(void) const
         {return reinterpret_cast<const RwReal *>(&chunk_.data_);}

      /**   Used to return RwChar pointer to data */

      const RwChar *GetAs_RwChar_ptr(void) const
         {return reinterpret_cast<const RwChar *>(&chunk_.data_);}

      /**   Used to return RwUInt8 pointer to data */

      const RwUInt8 *GetAs_RwUInt8_ptr(void) const
         {return reinterpret_cast<const RwUInt8 *>(&chunk_.data_);}

      /**   Used to return RwUInt16 pointer to data */

      const RwUInt16 *GetAs_RwUInt16_ptr(void) const
         {return reinterpret_cast<const RwUInt16 *>(&chunk_.data_);}

      /**   Used to return RwUInt32 pointer to data */

      const RwUInt32 *GetAs_RwUInt32_ptr(void) const
         {return reinterpret_cast<const RwUInt32 *>(&chunk_.data_);}

      /**   Used to return RwInt8 pointer to data */

      const RwInt8 *GetAs_RwInt8_ptr(void) const
         {return reinterpret_cast<const RwInt8 *>(&chunk_.data_);}

      /**   Used to return RwInt16 pointer to data */

      const RwInt16 *GetAs_RwInt16_ptr(void) const
         {return reinterpret_cast<const RwInt16 *>(&chunk_.data_);}

      /**   Used to return RwInt32 pointer to data */

      const RwInt32 *GetAs_RwInt32_ptr(void) const
         {return reinterpret_cast<const RwInt32 *>(&chunk_.data_);}
    
      /**   Used to return data as RwReal */

      const RwReal GetAs_RwReal(void) const
         {return *(reinterpret_cast<const RwReal *>(&chunk_.data_));}

      /**   Used to return data as RwChar */

      const RwChar GetAs_RwChar(void) const
         {return *(reinterpret_cast<const RwChar *>(&chunk_.data_));}

      /**   Used to return data as RwUInt8 */

      const RwUInt8 GetAs_RwUInt8(void) const
         {return *(reinterpret_cast<const RwUInt8 *>(&chunk_.data_));}
      
      /**   Used to return data as RwUInt16 */

      const RwUInt16 GetAs_RwUInt16(void) const
         {return *(reinterpret_cast<const RwUInt16 *>(&chunk_.data_));}

      /**   Used to return data as RwUInt32 */

      const RwUInt32 GetAs_RwUInt32(void) const
         {return *(reinterpret_cast<const RwUInt32 *>(&chunk_.data_));}
 
      /**   Used to return data as RwInt8 */

      const RwInt8 GetAs_RwInt8(void) const
         {return *(reinterpret_cast<const RwInt8 *>(&chunk_.data_));}
      
      /**   Used to return data as RwInt16 */

      const RwInt16 GetAs_RwInt16(void) const
         {return *(reinterpret_cast<const RwInt16 *>(&chunk_.data_));}

      /**   Used to return data as RwInt32 */

      const RwInt32 GetAs_RwInt32(void) const
         {return *(reinterpret_cast<const RwInt32 *>(&chunk_.data_));}

      /**   Used to return data as RwRGBA */

      const RwRGBA GetAs_RwRGBA(void) const
         {return *(reinterpret_cast<const RwRGBA *>(&chunk_.data_));}

    protected:

      // Revoked ctor/copy/assign - not a 'normal' constructable class
      CAttributeCommand();
      CAttributeCommand( const CAttributeCommand& );
      CAttributeCommand& operator = ( const CAttributeCommand& );

    private:

      // Implementation
      const CAttributeDataChunk chunk_;
   };

   /**
   *  \ingroup CAttributeHandler
   *
   *  A unidirectional iterator for a sequence of CAttributeDataChunks.
   *
   */

   class CAttributeDataChunkIterator
   {
   public:

      CAttributeDataChunkIterator(const CAttributeDataChunk& begin); 

      bool IsFinished() const;
      
      // RWS_PRE(!IsFinished())
      CAttributeDataChunkIterator& operator++();

      // RWS_PRE(!IsFinished())
      const CAttributeDataChunk& operator*() const;

      // RWS_PRE(!IsFinished())
      const CAttributeDataChunk* operator->() const;

      const CAttributeDataChunk *GetDataChunk(void) {return pCurrChunk_;}

   private:

      // Implementation
      const CAttributeDataChunk* pCurrChunk_;
   };

   /**
   *  \ingroup CAttributeHandler
   *
   *  A unidirectional iterator for retrieving all the CAttributeCommands in a CAttributePacket
   *  that are directed at a specified class, the 'target class'.
   *
   */

   class CAttributeCommandIterator
   {
   public:

      CAttributeCommandIterator(
         const CAttributePacket& packet,
         const RwChar *targetClass
         ); 

      bool IsFinished() const;
      
      // RWS_PRE(!IsFinished())
      CAttributeCommandIterator& operator++();

      // RWS_PRE(!IsFinished())
      const CAttributeCommand& operator*() const;

      // RWS_PRE(!IsFinished())
      const CAttributeCommand* operator->() const;

      const CAttributeDataChunk *GetDataChunk(void) {return chunkIt_.GetDataChunk();}

   private:

      // Implementation
      void SkipToNextAttributeCommand( bool lastClassIdChunkWasForTargetClass );
      CAttributeDataChunkIterator chunkIt_;
      const RwChar* pTargetClass_;
   };

   /**
   *  \ingroup CAttributeHandler
   *
   *  Abstract base class for anything that wishes to be capable of receiving attribute commands.
   *
   *  The RenderWare Studio Workspace uses unique IDs to identify instances 
   *  of behaviors that have been instantiated on the console. 
   *  On the console, a global map is maintained relating RenderWare Studio 
   *  Workspace unique IDs to CAttributeHandler pointers. The CAttributeHandler 
   *  class provides methods which are used to maintain this mapping.
   *
   */

   class CAttributeHandler
   {
   public:

#ifdef RWS_DESIGN
      RWS_DECLARE_CLASSID(CAttributeHandler);

      RWS_BEGIN_COMMANDS

           RWS_ATTRIBUTE(CMD_SetDebug,
            "Debug",
            "Enable/Disable debugging, each attribute handler has a Debug flag"
            " which is available for use by any derived class to enable/disable behavior"
            " specific per instance debugging (Programmers should look at "
            " uATTRIBUTEHANDLER_FLAG_DEBUG in CAttributeHandler.h)",
            BOOLEAN,
            RwUInt32,
            DEFAULT(0))

      RWS_END_COMMANDS;
#endif

      virtual ~CAttributeHandler();

      /**
      *
      *  Hook that allows derived classes to receive attribute commands 
      *
      */

      virtual void HandleAttributes(const CAttributePacket& attr);

      /**
      *
      * Returns a pointer to the attribute offset / update function table or zero if there
      * is no such table defined for the class. If a table is returned, then the number of
      * entries in that table will be returned via the 'rNumEntries' parameter.
      *
      * \return A pointer to an attribute offset table or zero.
      * \param classID The classID (name) of the class to find the table for.
      * \param rNumEntries On exit set to the number of entries in the table or zero if no table.
      * \param rpThis The pointer to the instance to be used when setting data via the table.
      * \param ignoreTopLevel A flag indicating if top level class checking should be ignored, if
      *                       this is true the top-most implementation will not check itself and
      *                       simply pass the check onto any base classes (and down the tree to
      *                       the most base class) instead. The default is to check the top level.
      */
      virtual CAttributeTableEntry *GetAttributeTable(const char *classID,
         RwUInt32 &rNumEntries, void *&rpThis, RwBool ignoreTopLevel = FALSE)
      {
         rNumEntries = 0;
         rpThis = 0;
         return(0);
      }

      /**
      * 
      * Returns a pointer to the update function for the behaviour along with the 'this' pointer
      * to use when calling the returned function. The function should be (is called by the
      * sequencer) after ALL other updates to attributes have been made.
      *
      * NOTE: This is a virtual function and as such if the top-level behaviour does not implement
      *       the function, a base behaviour may respond instead. To avoid this, always implement
      *       the function if the base behaviour does. If the base behaviour does return a function
      *       the 'this' pointer will also be for the base behaviour and NOT the top level behaviour.
      *
      * \return A pointer to the post update function for the behaviour or 0 if there isn't one.
      * \param rpThis Used to return the 'this' pointer which must be passed when the funciton is
      *        called.
      *
      */
      virtual CAttributeTablePostUpdateFunction GetAttrPostUpdateFunc(void *&rpThis)
      {
         rpThis = 0;
         return(0);
      }

      void Add(const RWSGUID & instanceId);

      static void Remove(const RWSGUID & instanceId);

      static CAttributeHandler* Find(const RWSGUID & instanceId);

      static void Purge();

      static void PurgeAll();

      /**
      *  
      *  Initialize the attribute handler system - typically called once during system startup
      *
      */

      static void Open(NewAttributeHandlerCallBack pNewAttributeHandlerCallBack)
      {
         sm_pNewAttributeHandlerCallBack = pNewAttributeHandlerCallBack;
      }

      /**
      *
      *  Shutdown the attribute handler system - typically called once during system shutdown 
      *
      */

      static void Close() { Purge(); sm_pNewAttributeHandlerCallBack = 0;}

      static void RegisterStreamChunkHandlers(void);

      static void UnRegisterStreamChunkHandlers(void);

      /**
      *  Not Documented
      */
      static void NewAttributeHandlerCreated(void)
      {
         if (sm_pNewAttributeHandlerCallBack) sm_pNewAttributeHandlerCallBack();
      }

      /*
      *
      *  Bit field values for various flags that are stored in m_Flags.
      *
      */
      #define uATTRIBUTEHANDLER_FLAG_DEBUG    (static_cast<RwUInt32>(0x1 << 0))
      #define uATTRIBUTEHANDLER_FLAG_LOCKED   (static_cast<RwUInt32>(0x1 << 1))

      /**
      *
      *  Locks the object to stop it being destroyed by CAttributeHandler::Purge().
      *  Calling CAttributeHandler::PurgeAll() will still destroy it.
      *
      *  \see CAttributeHandler::UnLock
      *  \see CAttributeHandler::IsLocked
      *  \see CAttributeHandler::Purge
      *  \see CAttributeHandler::PurgeAll
      *
      */
      void Lock()
      {
         m_Flags |= uATTRIBUTEHANDLER_FLAG_LOCKED;
      }

      /**
      *
      *  Unlocks the object to allow it to be destroyed by CAttributeHandler::Purge().
      *
      *  \see CAttributeHandler::Lock
      *  \see CAttributeHandler::IsLocked
      *  \see CAttributeHandler::Purge
      *  \see CAttributeHandler::PurgeAll
      *
      */
      void UnLock()
      {
         m_Flags &= ~uATTRIBUTEHANDLER_FLAG_LOCKED;
      }

      /**
      *
      *  Returns TRUE if the attribute handler is locked, otherwise FALSE
      *
      *  \see CAttributeHandler::Lock
      *  \see CAttributeHandler::UnLock
      *  \see CAttributeHandler::Purge
      *  \see CAttributeHandler::PurgeAll
      *
      */
      RwBool IsLocked()
      {
         if (m_Flags & uATTRIBUTEHANDLER_FLAG_LOCKED)
         {
            return TRUE;
         }
         else
         {
            return FALSE;
         }
      }
      
      /*
      *  \return Returns the unique Id, of this instance, of this class which corresponds to the GUID
      *  used by the workspace to identify the instance of the behavior.
      */
      const RWSGUID *InqInstanceId()
      {
         return &m_InstanceId;
      }

   protected:

      RwUInt32 m_Flags;    /**< Flags \ref uATTRIBUTEHANDLER_FLAG_DEBUG, \ref uATTRIBUTEHANDLER_FLAG_LOCKED*/

      /** 
      *
      *  Construct a CAttributeHandler object.
      *
      */
      CAttributeHandler() : m_Flags(0)
      {
         m_InstanceId.Clear();
      }

   private:

      // Revoked ops
      //
      CAttributeHandler( const CAttributeHandler& );
      CAttributeHandler& operator = ( const CAttributeHandler& );

      static NewAttributeHandlerCallBack sm_pNewAttributeHandlerCallBack;

   protected:
      RWSGUID m_InstanceId;  /**< The unique instanceId of this CAttributeHandler */
   };
}

#endif
