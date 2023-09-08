/****************************************************************************
*
* File :      CSeqCtrlData.cpp
*
* Abstract :  Sequence controller data objects.
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
#include <CString>
#include <memory.h>
#endif

#if defined(DOLPHIN) && defined(__MWERKS__)
   using namespace std;
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//

#include "framework/toolkits/time/gftime.h"
#include "framework/mainloop/logic.h"
#include "framework/toolkits/debugtools/debugtools.h"
#include "cseqctrldata.h"
#include "cseqstack.h"

namespace RWS
{
   namespace SEQUENCE
   {
      /**
      *
      * Defines the size of instruction and data blocks in the sequence control data.
      *
      */

      const RwUInt32 SEQ_CTRL_OP_SIZE = sizeof(RwUInt32);         // MUST be 4 bytes.

      ///////////////////////////
      //
      // CSeqCtrlData functions.
      //
      //

      /**
      *
      * Constructor - initialises the object and COPIES the passed in data and name.
      *
      * \param pChunkHeader A pointer to a supplied chunk header. Default is 0. If
      *                     zero, the code will search for the appropriate chunk
      *                     header in the stream. If non-zero, no search occurs, the
      *                     supplied data is used instead.
      *
      */

      CSeqCtrlData::CSeqCtrlData(RwStream *pStream, const RwChar *pName,
            RwChunkHeaderInfo *pChunckHeader) :
         m_pRawData(0),
         m_pName(0),
         m_dataSize(0)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlData::CSeqCtrlData");

         // Copy in name.

         if (pName)
         {
            RwUInt32 nameSize = rwstrlen(pName) + 1;

            m_pName = RWS_NEW RwChar[nameSize];
            RWS_ASSERT(m_pName, "Failed to allocate space for object's name.");
            rwstrcpy(m_pName, pName);
         }

         // Copy in data.

         if (pStream)
         {
            RwUInt32 dataSize = 0;

            // Header supplied ?

            if (pChunckHeader)
            {
               RWS_ASSERT(pChunckHeader->type != rwID_SEQUENCE,
                  "Chunk is not a sequence according to type information in header.");

               dataSize = pChunckHeader->length;
            }
            else
            {
               // Find chunk and read in stream.

               RwStreamFindChunk(pStream, rwID_SEQUENCE, &dataSize, 0);
            }

            // Allocate buffer or necessary size.

            RWS_ASSERT(dataSize, "data size cannot be zero.");
            m_pRawData = RWS_NEW RwUInt8[dataSize];
            RWS_ASSERT(m_pRawData, "Failed to allocate space for object's data.");
            
            RwStreamRead(pStream, m_pRawData, dataSize);
            m_dataSize = dataSize;
         }

         RWS_RETURNVOID();
      }

      /**
      *
      * Destructor - deallocates used memory and shuts down object.
      *
      */

      CSeqCtrlData::~CSeqCtrlData(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlData::~CSeqCtrlData");

         // Free up name & data, if allocated.

         if (m_pName)
         {
            delete[] m_pName;
         }
         if (m_pRawData)
         {
            delete[] m_pRawData;
         }

         RWS_RETURNVOID();
      }

      /**
      *
      * Returns the name of the data block. Do NOT modify this string.
      *
      */

      const RwChar *CSeqCtrlData::Name(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlData::Name");
         RWS_RETURN(m_pName);
      }

      /**
      *
      * Returns a pointer to the raw data held in the block. Do NOT modify this data.
      *
      */

      const void *CSeqCtrlData::RawData(void) const
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlData::RawData");
         RWS_RETURN(static_cast<void *>(m_pRawData));
      }

      /**
      *
      * Returns the size of the raw data held within the block.
      *
      */

      RwUInt32 CSeqCtrlData::DataSize(void) const
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlData::DataSize");
         RWS_RETURN(m_dataSize);
      }

      ///////////////////////////////
      //
      // CSeqCtrlDataIttr functions.
      //
      //

      const RwUInt32 ITTR_INT_STRING_TABLE                  = 1;
      const RwUInt32 ITTR_INT_GUID_TABLE                    = 2;
      const RwUInt32 ITTR_INT_RESERVED_TABLE                = 3;
      const RwUInt32 ITTR_INT_WAIT_INSTRUCTION              = 4;
      const RwUInt32 ITTR_INT_ENTITY_INSTRUCTION            = 5;
      const RwUInt32 ITTR_INT_CLASS_INSTRUCTION             = 6;
      const RwUInt32 ITTR_INT_COMMAND_INSTRUCTION           = 7;
      const RwUInt32 ITTR_INT_KEYFRAME_INSTRUCTION          = 8;
      const RwUInt32 ITTR_INT_INTERPOL_INSTRUCTION          = 9;
      const RwUInt32 ITTR_INT_SEND_EVENT_INSTRUCTION        = 10;
      const RwUInt32 ITTR_INT_WAIT_EVENT_INSTRUCTION        = 11;
      const RwUInt32 ITTR_INT_CREATE_INSTRUCTION            = 12;
      const RwUInt32 ITTR_INT_DELETE_INSTRUCTION            = 13;
      const RwUInt32 ITTR_INT_DYNAMIC_INTERPOL_INSTRUCTION  = 14;

      /**
      *
      * Constructor - sets the data block to be iterated over. Version in which data
      *               block is passed in initially.
      *
      */

      CSeqCtrlDataIttr::CSeqCtrlDataIttr(const CSeqCtrlData &dataObj, RwUInt32 memType,
         RwUInt32 strBuffSize, RwUInt32 guidBuffSize, RwUInt32 flags) :
         m_pGuidTable(0),
         m_pStringTable(0),
         m_userMemType(memType),
         m_strTableBuffSize(0),
         m_guidTableBuffSize(0),
         m_flags(flags)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlDataIttr::CSeqCtrlDataIttr(CSeqCtrlData &)");

         // Pre-allocate the buffers...

         ResizeStr(strBuffSize);
         ResizeGuid(guidBuffSize);

         Use(dataObj);

         RWS_RETURNVOID();
      }

      /**
      *
      * Constructor - just initialises, no initial data block to process. To set data
      *               block to use (MUST do this before use) call the 'Use' function.
      *
      */

      CSeqCtrlDataIttr::CSeqCtrlDataIttr(RwUInt32 memType, RwUInt32 strBuffSize,
            RwUInt32 guidBuffSize, RwUInt32 flags) :
         m_pDataByte(0),
         m_pUserDataByteStart(0),
         m_pEndDataByte(0),
         m_pGuidTable(0),
         m_pStringTable(0),
         m_stringTableSize(0),
         m_guidTableSize(0),
         m_userMemType(memType),
         m_strTableBuffSize(0),
         m_guidTableBuffSize(0),
         m_flags(flags)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlDataIttr::CSeqCtrlDataIttr");
         RWS_ASSERT(SEQ_CTRL_OP_SIZE == 4, "SEQ_CTRL_OP_SIZE is not 4 bytes.");

         // Pre-allocate the buffers...

         ResizeStr(strBuffSize);
         ResizeGuid(guidBuffSize);

         RWS_RETURNVOID();
      }

      /**
      *
      * Destructor - frees up memory used for string table, etc.
      *
      */

      CSeqCtrlDataIttr::~CSeqCtrlDataIttr(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlDataIttr::~CSeqCtrlDataIttr");

         FreeMem();

         RWS_RETURNVOID();
      }

      /**
      *
      * Factored-out function which is called by operator++ and some internal functions.
      * Moves to the next instruction in the sequencer instruction block. When reaches
      * the end, 'GetInstruction' will return CT_EOF.
      *
      */

      void CSeqCtrlDataIttr::MoveToNext(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlDataIttr::MoveToNext");
         RWS_ASSERT((reinterpret_cast<RwUInt32>(m_pDataByte) & 0x03) == 0,
            "Data pointer is not 4 byte aligned.");

         RwUInt32 curInst = GetInstruction();

         // At end of instructions ?

         if (curInst != IT_EOF)
         {
            // Need to move onto next instruction, sizing is dependent on type of instruction.

            switch (curInst)
            {
            case IT_STRING_TABLE:
               {
                  const RwUInt32 tableLen = GetStringTableSize();
                  RWS_ASSERT((tableLen & 0x3) == 0,
                     "String table length not a multiple of 4 bytes.");

                  m_pDataByte += (SEQ_CTRL_OP_SIZE + SEQ_CTRL_OP_SIZE + tableLen);
               }
               break;

            case IT_GUID_TABLE:
               {
                  const RwUInt32 tableLen = GetGuidTableSize();
                  RWS_ASSERT((tableLen & 0xF) == 0,
                     "Guid table length not a multiple of 16 bytes.");

                  m_pDataByte += (SEQ_CTRL_OP_SIZE + SEQ_CTRL_OP_SIZE + tableLen);
               }
               break;

            case IT_WAIT:

               m_pDataByte += (SEQ_CTRL_OP_SIZE + SEQ_CTRL_OP_SIZE);
               break;

            case IT_ENTITY:

               m_pDataByte += (SEQ_CTRL_OP_SIZE + SEQ_CTRL_OP_SIZE);
               break;

            case IT_CLASS:

               m_pDataByte += (SEQ_CTRL_OP_SIZE + SEQ_CTRL_OP_SIZE);
               break;

            case IT_COMMAND:

               m_pDataByte += (SEQ_CTRL_OP_SIZE + SEQ_CTRL_OP_SIZE);
               break;

            case IT_KEYFRAME:
               {
                  const RwUInt32 dataLen = *reinterpret_cast<const RwUInt32 *>(
                     &m_pDataByte[SEQ_CTRL_OP_SIZE + SEQ_CTRL_OP_SIZE]);
                  RWS_ASSERT((dataLen & 0x3) == 0,
                     "Keyframe data length not a multiple of 4 bytes.");

                  m_pDataByte += (SEQ_CTRL_OP_SIZE + SEQ_CTRL_OP_SIZE +
                     SEQ_CTRL_OP_SIZE + dataLen);
               }
               break;

            case IT_INTERPOL:

               m_pDataByte += (SEQ_CTRL_OP_SIZE + SEQ_CTRL_OP_SIZE + 
                  SEQ_CTRL_OP_SIZE);
               break;

            case IT_SEND_EVENT:

               m_pDataByte += (SEQ_CTRL_OP_SIZE + SEQ_CTRL_OP_SIZE);
               break;

            case IT_WAIT_EVENT:

               m_pDataByte += (SEQ_CTRL_OP_SIZE + SEQ_CTRL_OP_SIZE);
               break;

            case IT_CREATE:
               {
                  const RwUInt32 dataLen = *reinterpret_cast<const RwUInt32 *>(
                     &m_pDataByte[SEQ_CTRL_OP_SIZE]);
                  RWS_ASSERT((dataLen & 0x3) == 0,
                     "Creation data length not a multiple of 4 bytes.");

                  m_pDataByte += (SEQ_CTRL_OP_SIZE + SEQ_CTRL_OP_SIZE + dataLen);
               }
               break;

            case IT_DELETE:

               m_pDataByte += SEQ_CTRL_OP_SIZE + SEQ_CTRL_OP_SIZE;
               break;

            case IT_DYNAMIC_INTERPOL:

               m_pDataByte += SEQ_CTRL_OP_SIZE + SEQ_CTRL_OP_SIZE + SEQ_CTRL_OP_SIZE;
               break;

            case IT_ERROR:

               RWS_ASSERTFAIL("Error instruction found - problem with instruction block.");
               break;
            
            default:

               RWS_ASSERTFAIL("Unknown instruction type in data block.");
               break;
            }
         }

         RWS_ASSERT((reinterpret_cast<RwUInt32>(m_pDataByte) & 0x03) == 0,
            "Data pointer (after moving to next instruction) is not 4 byte aligned.");

         RWS_RETURNVOID();
      }

      /**
      *
      * Sets the data object to be iterated over and resets to the start of the data
      * block.
      *
      */

      void CSeqCtrlDataIttr::Use(const CSeqCtrlData &dataObj)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlDataIttr::Use");

         m_pDataByte = static_cast<const RwUInt8 *>(dataObj.RawData());
         m_pEndDataByte = m_pDataByte + dataObj.DataSize();
         m_pUserDataByteStart = 0;

         // Initialize sizes of stored data (NOT buffer sizes).

         m_guidTableSize = 0;
         m_stringTableSize = 0;

         // Process internal instructions...

         RwUInt32 curInst = GetInstruction();
         RwBool run = TRUE;

         while (run)
         {
            switch (curInst)
            {
            case IT_STRING_TABLE:

               // To support streaming this data needs to be copied, as the
               // data may be unloaded from memory during the processing of
               // the stream.

               m_stringTableSize = GetStringTableSize();
               ResizeStr(m_stringTableSize);
               memcpy(m_pStringTable, GetStringTablePtr(), m_stringTableSize);
               break;

            case IT_GUID_TABLE:

               // To support streaming this data needs to be copied, as the
               // data may be unloaded from memory during the processing of
               // the stream.

               m_guidTableSize = GetGuidTableSize();
               ResizeGuid(m_guidTableSize);
               memcpy(m_pGuidTable, GetGuidTablePtr(), m_guidTableSize);
               break;

            // None internal instruction, exit loop.

            default:

               run = FALSE;
               break;
            }

            // If not exiting, get the next instruction...

            if (run)
            {
               MoveToNext();
               curInst = GetInstruction();
            }
         }

         // Check data has been setup - do NOT check to see if the GUID and string tables have
         // been created, as they are not required to be in the sequence data for dynamic
         // sequences.

         RWS_ASSERT((m_guidTableSize & 0xF) == 0, "GUID table is NOT a multiple of 16.");

         // Set the 'user start' to the current instruction, used during resetting of
         // the iterator.

         m_pUserDataByteStart = m_pDataByte;

         RWS_RETURNVOID();
      }

      /**
      *
      * Resets the current instruction pointer to the first user (ie not internal) instruction
      * in the sequence control data. The 'Use' function or constructor specifying a CSeqCtrlData
      * object to use MUST have been called before calling this function.
      *
      */

      void CSeqCtrlDataIttr::Reset(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlDataIttr::Reset");
         RWS_ASSERT(m_pUserDataByteStart, "User data pointer invalid, data block not specified?");

         m_pDataByte = m_pUserDataByteStart;

         RWS_RETURNVOID();
      }

      /**
      *
      * Allows the size of the string and GUID table buffers to be pre-allocated. This is
      * useful if several sequences are to be run, each with difference sizes of string &
      * or GUID tables - it stops the system having to reallocate memory (and hence fragmenting
      * memory). The memory can be release using the 'FreeMem' function. If this function is
      * called a second time, the buffers will only reallocated if a larger number is passed in
      * the second time (independent for each buffer).
      *
      * NOTE: Do NOT CALL if the iterator has begun processing data as this will cause the
      *       potential LOST of the string and/or GUID tables.
      *
      * \param strBuffSize The maximum size the string buffer will be. Cannot be zero. If not
      *                    known, do not call pre-alloc function.
      * \param guidBuffSize The maximum size the GUID buffer will be. Cannot be zero. If not
      *                     known, do not call pre-alloc function.
      *
      */

      void CSeqCtrlDataIttr::PreAllocMem(RwUInt32 strBuffSize, RwUInt32 guidBuffSize)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlDataIttr::PreAllocMem");
         RWS_ASSERT(strBuffSize, "String table buffer pre-alloc size cannot be zero.");
         RWS_ASSERT(guidBuffSize, "GUID table buffer pre-alloc size cannot be zero.");
         RWS_ASSERT(m_stringTableSize,
            "Cannot resize if iterator is in operation. This WILL cause problems.");
         RWS_ASSERT(m_guidTableSize,
            "Cannot resize if iterator is in operation. This WILL cause problems.");

         ResizeStr(strBuffSize);
         ResizeGuid(guidBuffSize);

         RWS_RETURNVOID();
      }

      /**
      *
      * Frees the memory used internally by the object and sets all sizes and pointers to
      * safe values.
      *
      */

      void CSeqCtrlDataIttr::FreeMem(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlDataIttr::FreeMem");

         delete[] m_pStringTable;            // Delete checks if NULL.
         m_pStringTable = 0;
         m_stringTableSize = 0;
         m_strTableBuffSize = 0;

         delete[] m_pGuidTable;              // Delete checks if NULL.
         m_pGuidTable = 0;
         m_guidTableSize = 0;
         m_guidTableBuffSize = 0;

         RWS_RETURNVOID();
      }

      /**
      *
      * Resizes buffer used for string table. If resize occurs the contents of the table
      * WILL BE LOST. Therefore it cannot be assumed the data will persist across a call
      * to this function.
      *
      */

      void CSeqCtrlDataIttr::ResizeStr(RwUInt32 newSize)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlDataIttr::ResizeStr");

         // Need to resize ?

         if (newSize > m_strTableBuffSize)
         {
            if (m_strTableBuffSize != 0)
            {
               // Emit warning...

               if ((m_flags & SEQ_FLAG_BUFFER_WARNINGS) != 0)
               {
                  RWS_DEBUGSTREAM_WARN("Resizing string buffer inside sequence data "
                     "iterator - likely to cause memory fragmentation." << std::endl);
               }

               // Remove old buffer...

               delete[] m_pStringTable;
            }

            // Allocate the new one...

            m_pStringTable = RWS_NEW_TYPE(m_userMemType) char[newSize];
            RWS_ASSERT(m_pStringTable, "Failed to resize string table buffer.");
            m_strTableBuffSize = newSize;
         }
         RWS_RETURNVOID();
      }

      /**
      *
      * Resizes buffer used for GUID table. If resize occurs the contents of the table
      * WILL BE LOST. Therefore it cannot be assumed the data will persist across a call
      * to this function.
      *
      * NOTE: Size is in BYTES and NOT in GUIDs. And the new size MUST be a multiple of 16.
      *
      */

      void CSeqCtrlDataIttr::ResizeGuid(RwUInt32 newSize)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlDataIttr::ResizeGuid");
         RWS_ASSERT((newSize & 0xF) == 0, "The new buffer size is not a multiple of 16.");

         // Need to resize ?

         if (newSize > m_guidTableBuffSize)
         {
            if (m_guidTableBuffSize != 0)
            {
               // Emit warning...

               if ((m_flags & SEQ_FLAG_BUFFER_WARNINGS) != 0)
               {
                  RWS_DEBUGSTREAM_WARN("Resizing GUID buffer inside sequence data "
                     "iterator - likely to cause memory fragmentation." << std::endl);
               }

               // Remove old buffer...

               delete[] m_pGuidTable;
            }

            // Allocate the new one...

            m_pGuidTable = RWS_NEW_TYPE(m_userMemType) RWSGUID[newSize >> 4];
            RWS_ASSERT(m_pGuidTable, "Failed to resize GUID table buffer.");
            m_guidTableBuffSize = newSize;
         }
         RWS_RETURNVOID();
      }

      /**
      *
      * Get instruction, returns the current instruction type. Returns RwUInt32, not
      * 'InstructionType' so user can more easily add there own instructions in an
      * inherited class.
      *
      */

      RwUInt32 CSeqCtrlDataIttr::GetInstruction(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlDataIttr::GetInstruction");

         // Is the end of the buffer reached ?

         if (m_pDataByte < m_pEndDataByte)
         {
            RwUInt32 srcInst = *reinterpret_cast<const RwUInt32 *>(m_pDataByte);
            InstructionType instruction;

            switch (srcInst)
            {
            case ITTR_INT_STRING_TABLE:
               instruction = IT_STRING_TABLE;
               break;

            case ITTR_INT_GUID_TABLE:
               instruction = IT_GUID_TABLE;
               break;

            case ITTR_INT_WAIT_INSTRUCTION:

               instruction = IT_WAIT;
               break;

            case ITTR_INT_ENTITY_INSTRUCTION:

               instruction = IT_ENTITY;
               break;

            case ITTR_INT_CLASS_INSTRUCTION:

               instruction = IT_CLASS;
               break;

            case ITTR_INT_COMMAND_INSTRUCTION:

               instruction = IT_COMMAND;
               break;

            case ITTR_INT_KEYFRAME_INSTRUCTION:

               instruction = IT_KEYFRAME;
               break;

            case ITTR_INT_INTERPOL_INSTRUCTION:

               instruction = IT_INTERPOL;
               break;

            case ITTR_INT_SEND_EVENT_INSTRUCTION:

               instruction = IT_SEND_EVENT;
               break;

            case ITTR_INT_WAIT_EVENT_INSTRUCTION:

               instruction = IT_WAIT_EVENT;
               break;

            case ITTR_INT_CREATE_INSTRUCTION:

               instruction = IT_CREATE;
               break;

            case ITTR_INT_DELETE_INSTRUCTION:

               instruction = IT_DELETE;
               break;

            case ITTR_INT_DYNAMIC_INTERPOL_INSTRUCTION:

               instruction = IT_DYNAMIC_INTERPOL;
               break;

            default:

               RWS_ASSERTFAIL("Unknown instruction found [" << srcInst <<
                  "] - setting to IT_ERROR.");
               instruction = IT_ERROR;
               break;
            }
         
            RWS_RETURN(instruction);
         }

         RWS_RETURN(IT_EOF);
      }

      /**
      *
      * Returns data from a wait instruction.
      *
      * Format of instruction...
      *
      * [RwUInt32] = instruction, must be ITTR_INT_WAIT_INSTRUCTION, asserts if not.
      * [RwUInt32] = wait period in milliseconds.
      *
      * Purpose of instruction, instructs playback system to wait the specified
      * number of milliseconds before processing any more instructions. NOTE: any
      * sequence currently under way, will continue to be processed, this
      * simply stops any new one from being started until the time is up.
      *
      */

      const RwUInt32 CSeqCtrlDataIttr::GetWaitTime(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlDataIttr::GetEntityGuid");
         RWS_ASSERT(GetInstruction() == IT_WAIT, "Not a wait instruction.");
         RWS_RETURN(*reinterpret_cast<const RwUInt32 *>(
            &m_pDataByte[SEQ_CTRL_OP_SIZE]));
      }

      /**
      *
      * Returns data from a entity instruction.
      *
      * Format of instruction...
      *
      * [RwUInt32] = instruction, must be ITTR_INT_ENTITY_INSTRUCTION, asserts if not.
      * [RwUint32] = Offset into the GUID table, this MUST be a multiple of 16 and is
      *              a BYTE offset, not a GUID index.
      *
      * Purpose of instruction, sets the GUID of the entity for which subsequent
      * instructions apply.
      *
      */

      const RWSGUID *CSeqCtrlDataIttr::GetEntityGuid(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlDataIttr::GetEntityGuid");
         RWS_ASSERT(GetInstruction() == IT_ENTITY, "Not an entity instruction.");

         const RwUInt32 *pOffset = reinterpret_cast<const RwUInt32 *>(
            &m_pDataByte[SEQ_CTRL_OP_SIZE]);
         RWS_ASSERT(((*pOffset) & 0xF) == 0, "GUID table offset is not a multiple of 16.");

         RWS_RETURN(GetGuid(*pOffset));
      }

      /**
      *
      * Returns the class name from a class instruction.
      *
      * Format of instruction...
      *
      * [RwUInt32] = instruction, must be IT_CLASS, asserts if not.
      * [RwUInt32] = offset from start of string table where name is held.
      *
      * Purpose of instruction, sets the class name (behaviour or sub-behaviour)
      * of entity which the following sequence data applies to.
      *
      */

      const RwChar *CSeqCtrlDataIttr::GetClassName(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlDataIttr::GetClassName");
         RWS_ASSERT(GetInstruction() == IT_CLASS, "Not a class instruction.");
         RWS_RETURN(GetString(GetClassNameOffset()));
      }

      /**
      *
      * Returns the string table offset for the class name string. See 'GetClassName'
      * for more information on instruction format. Instruction MUST BE the
      * IT_CLASS, asserts if not.
      *
      */

      RwUInt32 CSeqCtrlDataIttr::GetClassNameOffset(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlDataIttr::GetClassNameOffset");
         RWS_ASSERT(GetInstruction() == IT_CLASS, "Not a class instruction.");
         RWS_RETURN(*reinterpret_cast<const RwUInt32 *>(&m_pDataByte[SEQ_CTRL_OP_SIZE]));
      }

      /**
      *
      * Returns the command number from the command instruction.
      *
      * Format of instruction...
      *
      * [RwUInt32] = instruction, must be ITTR_INT_COMMAND_INSTRUCTION, asserts if not.
      * [RwUInt32] = command number.
      *
      * Purpose of instruction, indicates which command the following keyframe, etc
      * instructions apply to. The 'command' is the also know as the 'attribute number'.
      *
      */

      const RwUInt32 CSeqCtrlDataIttr::GetCommand(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlDataIttr::GetCommand");
         RWS_ASSERT(GetInstruction() == IT_COMMAND, "Not a command instruction.");
         RWS_RETURN(*reinterpret_cast<const RwUInt32 *>(&m_pDataByte[SEQ_CTRL_OP_SIZE]));
      }

      /**
      *
      * Returns only the type name from the keyframe data, see 'GetKeyframe' for more
      * information on instruction format, etc...
      *
      */

      const RwChar *CSeqCtrlDataIttr::GetKeyframeTypeName(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlDataIttr::GetKeyframeTypeName");
         RWS_ASSERT(GetInstruction() == IT_KEYFRAME, "Not a keyframe instruction.");
         RWS_RETURN(GetString(GetKeyframeTypeNameOffset()));
      }

      /**
      *
      * Returns the string table offset for the keyframe type name string. See 'GetKeyframe'
      * for more information on instruction format. Instruction MUST BE the
      * IT_KEYFRAME, asserts if not.
      *
      */

      RwUInt32 CSeqCtrlDataIttr::GetKeyframeTypeNameOffset(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlDataIttr::GetKeyframeTypeNameOffset");
         RWS_ASSERT(GetInstruction() == IT_KEYFRAME, "Not a keyframe instruction.");
         RWS_RETURN(*reinterpret_cast<const RwUInt32 *>(&m_pDataByte[SEQ_CTRL_OP_SIZE]));
      }

      /**
      *
      * Returns the name of the event to send. The data pointers are ONLY valid
      * while this is the current iterator command. Once stepped past the validity
      * of the pointers cannot be guaranteed.
      *
      * Format of send event command is...
      *
      * [RwUInt32] = instruction, must be ITTR_INT_SEND_EVENT_INSTRUCTION, asserts if not.
      * [RwUInt32] = offset from start of string table where name is held.
      *
      */

      const char *CSeqCtrlDataIttr::GetEventName(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlDataIttr::GetEventName");
         RWS_ASSERT(GetInstruction() == IT_SEND_EVENT, "Not a 'send event' instruction.");
         RWS_RETURN(GetString(GetEventNameOffset()));
      }

      /**
      *
      * Returns the string table offset for the send event name string. See 'GetEventName'
      * for more information on instruction format. Instruction MUST BE the
      * IT_SEND_EVENT, asserts if not.
      *
      */

      RwUInt32 CSeqCtrlDataIttr::GetEventNameOffset(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlDataIttr::GetEventNameOffset");
         RWS_ASSERT(GetInstruction() == IT_SEND_EVENT, "Not a 'send event' instruction.");
         RWS_RETURN(*reinterpret_cast<const RwUInt32 *>(&m_pDataByte[SEQ_CTRL_OP_SIZE]));
      }

      /**
      *
      * Returns the keyframe data to the caller. The data pointers are ONLY valid
      * while this is the current iterator command. Once stepped past the validity
      * of the pointers cannot be guaranteed.
      *
      * Format of instruction...
      *
      * [RwUInt32] = instruction, must be ITTR_INT_KEYFRAME_INSTRUCTION, asserts if not.
      * [RwUInt32] = offset from start of string table where name is held.
      * [RwUInt32] = data length.
      * [RwUInt8 ]
      * ...
      * [RwUInt8 ] = data - not of any particular fixed format.
      *
      */

      void CSeqCtrlDataIttr::GetKeyframe(CSeqCtrlKeyData &rKeyData, RwUInt32 memType)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlDataIttr::GetKeyframe");
         RWS_ASSERT(GetInstruction() == IT_KEYFRAME, "Not a keyframe instruction.");

         const RwUInt32 dataSize = GetKeyframeDataSize();
         const RwChar *name = GetKeyframeTypeName();
         const void *pData = GetKeyframeDataPtr();

         rKeyData.Set(name, pData, dataSize, memType);

         RWS_RETURNVOID();
      }

      /**
      *
      * Returns the keyframe data pointer. See 'GetKeyframe' for more information on instruction
      * format. Instruction MUST BE the IT_KEYFRAME, asserts if not.
      *
      */

      const void *CSeqCtrlDataIttr::GetKeyframeDataPtr(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlDataIttr::GetKeyframeDataPtr");
         RWS_ASSERT(GetInstruction() == IT_KEYFRAME, "Not a keyframe instruction.");
         RWS_RETURN(static_cast<const void *>(&m_pDataByte[SEQ_CTRL_OP_SIZE + 
            SEQ_CTRL_OP_SIZE + SEQ_CTRL_OP_SIZE]));
      }

      /**
      *
      * Returns the keyframe data size. See 'GetKeyframe' for more information on instruction
      * format. Instruction MUST BE the IT_KEYFRAME, asserts if not.
      *
      */

      RwUInt32 CSeqCtrlDataIttr::GetKeyframeDataSize(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlDataIttr::GetKeyframeDataSize");
         RWS_ASSERT(GetInstruction() == IT_KEYFRAME, "Not a keyframe instruction.");
         RWS_RETURN(*reinterpret_cast<const RwUInt32 *>(
            &m_pDataByte[SEQ_CTRL_OP_SIZE + SEQ_CTRL_OP_SIZE]));
      }

      /**
      *
      * Returns the interpolator data to the caller. The data pointers are ONLY valid
      * while this is the current iterator command. Once stepped past the validity
      * of the pointers cannot be guaranteed.
      *
      * Format of instruction...
      *
      * [RwUInt32] = instruction, must be IT_INTERPOL, asserts if not.
      * [RwUInt32] = interpolation time in milliseconds.
      * [RwUInt32] = offset from start of string table where name is held.
      *
      * NOTE: This instruction is assumed to be surrounded by two keyframes, the start
      *       and end ones. These must be sent AT THE SAME TIME as the interpolator needs
      *       to know the value it is going to at the beginning, so it can a) setup data
      *       and b) interpolate at intermediate times. The format is therefore...
      *
      * <start keyframe instruction><interpolation instruction><end keyframe instruction>
      *
      * In Design builds (ONLY) the dynamic interpolator instruction is also supported. The
      * SAME RULES apply, but the value must be set immediately - use the GetDynamicInterpolDelta
      * function to get the position between the two keyframes. Instruction format for the
      * dynamic interpolator is...
      *
      * [RwUInt32] = instruction, must be IT_DYNAMIC_INTERPOL, asserts if not.
      * [RwReal]   = interpolator value, will be between 0 (fully start keyframe) and 1 (fully
      *              end keyframe). Values in between require interpolation.
      * [RwUInt32] = offset from start of string table where name is held.
      *
      * NOTE: Dynamic updates are immediate, so the interpolator time is zero.
      * 
      */
 
      void CSeqCtrlDataIttr::GetInterpol(CSeqCtrlInterpolData &rInterpolData, RwUInt32 memType)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlDataIttr::GetInterpol");

#ifdef RWS_DESIGN

         RWS_ASSERT(GetInstruction() == IT_INTERPOL || GetInstruction() == IT_DYNAMIC_INTERPOL,
            "Not an interpolation (dynamic or otherwise) instruction.");

         if (GetInstruction() == IT_DYNAMIC_INTERPOL)
         {
            const RwChar *name = GetInterpolName();

            rInterpolData.Set(name, 0, 0, GetDynamicInterpolDelta(), memType);
         }
         else
         {
#else
            RWS_ASSERT(GetInstruction() == IT_INTERPOL, "Not an interpolation instruction.");
#endif

            const RwUInt32 time = GetInterpolTime();
            const RwChar *name = GetInterpolName();

            rInterpolData.Set(name, 0, 0, time, memType);

#ifdef RWS_DESIGN

         }
#endif
         RWS_RETURNVOID();
      }

      /**
      *
      * Returns the interpolator type name. See 'GetInterpol' for more information on instruction
      * format. Instruction MUST BE the IT_INTERPOL(or IT_DYNAMIC_INTERPOL in design builds),
      * asserts if not.
      *
      */

      const char *CSeqCtrlDataIttr::GetInterpolName(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlDataIttr::GetInterpolName");

#ifdef RWS_DESIGN

         RWS_ASSERT(GetInstruction() == IT_INTERPOL || GetInstruction() == IT_DYNAMIC_INTERPOL,
            "Not an interpolation (dynamic or otherwise) instruction.");
#else
         RWS_ASSERT(GetInstruction() == IT_INTERPOL, "Not an interpolation instruction.");
#endif
         RWS_RETURN(GetString(GetInterpolNameOffset()));
      }

      /**
      *
      * Returns the string table offset for the interpolator type name string. See 'GetInterpol'
      * for more information on instruction format. Instruction MUST BE the
      * IT_INTERPOL (or IT_DYNAMIC_INTERPOL in design builds), asserts if not.
      *
      * See GetInterpol for more information.
      *
      */

      RwUInt32 CSeqCtrlDataIttr::GetInterpolNameOffset(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlDataIttr::GetInterpolNameOffset");

#ifdef RWS_DESIGN

         RWS_ASSERT(GetInstruction() == IT_INTERPOL || GetInstruction() == IT_DYNAMIC_INTERPOL,
            "Not an interpolation (dynamic or otherwise) instruction.");
#else
         RWS_ASSERT(GetInstruction() == IT_INTERPOL, "Not an interpolation instruction.");
#endif
         // NOTE: Dynamic and non-dynamic interpolator instructions have the offset into the
         //       string table for their interpolator name at the same offset in the instruction
         //       block. Therefore, the same code can be used...

         RWS_RETURN(*reinterpret_cast<const RwUInt32 *>(
            &m_pDataByte[SEQ_CTRL_OP_SIZE + SEQ_CTRL_OP_SIZE]));
      }

      /**
      *
      * Returns the interpolation time. See 'GetInterpol' for more information on instruction
      * format. Instruction MUST BE the IT_INTERPOL, asserts if not.
      *
      * NOTE: Dynamic interpolations do NOT have time.
      *
      */

      RwUInt32 CSeqCtrlDataIttr::GetInterpolTime(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlDataIttr::GetInterpolTime");
         RWS_ASSERT(GetInstruction() == IT_INTERPOL, "Not an interpolation instruction.");
         RWS_RETURN(*reinterpret_cast<const RwUInt32 *>(&m_pDataByte[SEQ_CTRL_OP_SIZE]));
      }

      /**
      *
      * Returns the GUID from a create command, via the passed in RWSGUID variable.
      *
      * \param rGuid The RWSGUID to set to the GUID of the entity being created.
      *
      */

      void CSeqCtrlDataIttr::GetCreateGuid(RWSGUID &rGuid)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlDataIttr::GetCreateGuid");
         RWS_ASSERT(GetInstruction() == IT_CREATE, "Not a create instruction.");

         rGuid = GetCreateData()->GetInstanceId();

         RWS_RETURNVOID();
      }

      /**
      *
      * Returns the creation data, in the form of an attribute packet, for the
      * new entity.
      *
      * \return The attribute packet used to create the new entity.
      *
      */

      const CAttributePacket *CSeqCtrlDataIttr::GetCreateData(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlDataIttr::GetCreateData");
         RWS_ASSERT(GetInstruction() == IT_CREATE, "Not a create instruction.");
         RWS_RETURN(reinterpret_cast<const CAttributePacket *>(&m_pDataByte[
            SEQ_CTRL_OP_SIZE + SEQ_CTRL_OP_SIZE]));
      }

      /**
      *
      * Returns the offset in the GUID table for a delete command.
      *
      */

      RwUInt32 CSeqCtrlDataIttr::GetDeleteGuidOffset(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlDataIttr::GetDeleteGuidOffset");
         RWS_ASSERT(GetInstruction() == IT_DELETE, "Not a delete instruction.");

         const RwUInt32 *pOffset = reinterpret_cast<const RwUInt32 *>
            (&m_pDataByte[SEQ_CTRL_OP_SIZE]);

         RWS_RETURN(*pOffset);
      }

      /**
      *
      * Returns the name of the event to wait for. The data pointers are ONLY valid
      * while this is the current iterator command. Once stepped past the validity
      * of the pointers cannot be guaranteed.
      *
      * Format of wait event command is...
      *
      * [RwUInt32] = instruction, must be IT_WAIT_EVENT, asserts if not.
      * [RwUInt32] = offset from start of string table where name is held.
      *
      */

      const char *CSeqCtrlDataIttr::GetWaitEventName(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlDataIttr::GetWaitEventName");
         RWS_ASSERT(GetInstruction() == IT_WAIT_EVENT, "Not a wait event instruction.");
         RWS_RETURN(GetString(GetWaitEventNameOffset()));
      }

      /**
      *
      * Returns the string table offset for the wait event name string. See 'GetWaitEventName'
      * for more information on instruction format. Instruction MUST BE the
      * IT_WAIT_EVENT, asserts if not.
      *
      */

      RwUInt32 CSeqCtrlDataIttr::GetWaitEventNameOffset(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlDataIttr::GetWaitEventNameOffset");
         RWS_ASSERT(GetInstruction() == IT_WAIT_EVENT, "Not a wait event instruction.");
         RWS_RETURN(*reinterpret_cast<const RwUInt32 *>(&m_pDataByte[SEQ_CTRL_OP_SIZE]));
      }

      /**
      *
      * Returns the delta value of the dynamic interpolator. This is zero for the last keyframe,
      * one for the current keyframe and any other value between these to indicate an interpolation
      * between the two keyframes. Instruction type MUST BE the IT_DYNAMIC_INTERPOL, asserts if not.
      *
      * NOTE: Only active in design builds, outside this mode the function will always return zero.
      *
      * See GetInterpol for more information.
      *
      */

#ifdef RWS_DESIGN

      RwReal CSeqCtrlDataIttr::GetDynamicInterpolDelta(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlDataIttr::GetDynamicInterpolDelta");
         RWS_ASSERT(GetInstruction() == IT_DYNAMIC_INTERPOL,
            "Not a dynamic Interpolator instruction.");
         RWS_RETURN(*reinterpret_cast<const RwReal *>(&m_pDataByte[SEQ_CTRL_OP_SIZE]));
      }

#endif

      /**
      *
      * Returns a pointer to the start of the string table. The current instruction MUST
      * be IT_STRING_TABLE. For internal use ONLY.
      *
      * Format of string table command is...
      *
      * [RwUInt32] = instruction, must be IT_STRING_TABLE, asserts if not.
      * [RwUInt32] = size of the string table in bytes. MUST be a multiple of 4.
      * [Char    ] = Strings, each ending with a '/0'. There is then a zero length string
      * ...          (ie a single '\0' character) and several padding characters (NOT
      *              defined as '\0') to make up to a multiple of 4 bytes.
      * [Char    ]
      *
      */

      const char *CSeqCtrlDataIttr::GetStringTablePtr(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlDataIttr::GetStringTablePtr");
         RWS_ASSERT(GetInstruction() == IT_STRING_TABLE, "Not a string table instruction.");
         RWS_RETURN(reinterpret_cast<const char *>(
            &m_pDataByte[SEQ_CTRL_OP_SIZE + SEQ_CTRL_OP_SIZE]));
      }

      /**
      *
      * Returns the size of the string table. The current instruction MUST be
      * IT_STRING_TABLE. For internal use ONLY.
      *
      * See 'GetStringTablePtr' for more information.
      *
      */

      RwUInt32 CSeqCtrlDataIttr::GetStringTableSize(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlDataIttr::GetStringTableSize");
         RWS_ASSERT(GetInstruction() == IT_STRING_TABLE, "Not a string table instruction.");
         RWS_RETURN(*reinterpret_cast<const RwUInt32 *>(&m_pDataByte[SEQ_CTRL_OP_SIZE]));
      }

      /**
      *
      * Returns a pointer to a string at the specified offset in the string table.
      * Will assert if the offset is off the end of the string table or the string
      * is zero length.
      *
      * See 'GetStringTablePtr' for more information.
      *
      */

      const char *CSeqCtrlDataIttr::GetString(RwUInt32 offset)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlDataIttr::GetString");
         RWS_ASSERT(m_stringTableSize != 0, "String table NOT setup.");
         RWS_ASSERT(offset < m_stringTableSize, "Offset off end of string table.");
         RWS_ASSERT(m_pStringTable[offset] != '\0', "String at offset is zero length.");
         RWS_RETURN(m_pStringTable + offset);
      }

      /**
      *
      * Returns a pointer to the start of the GUID table. The current instruction MUST
      * be IT_GUID_TABLE. For internal use ONLY. Can return RWSGUID pointer here as GUID
      * is being pulled out of the (stored in memory not stream) GUID table.
      *
      * Format of guid table command is...
      *
      * [RwUInt32]     = instruction, must be IT_GUID_TABLE, asserts if not.
      * [RwUInt32]     = size of the GUID table in BYTES. MUST be a multiple of 16.
      * [RwUInt32 x 4] = Guid table entries (4 x 32 bit values each).
      * ...
      * [RwUInt32 x 4]
      *
      */

      const RWSGUID *CSeqCtrlDataIttr::GetGuidTablePtr(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlDataIttr::GetGuidTablePtr");
         RWS_ASSERT(GetInstruction() == IT_GUID_TABLE, "Not a guid table instruction.");
         RWS_RETURN(reinterpret_cast<const RWSGUID *>(
            &m_pDataByte[SEQ_CTRL_OP_SIZE + SEQ_CTRL_OP_SIZE]));
      }

      /**
      *
      * Returns the size of the guid table. The current instruction MUST be
      * IT_GUID_TABLE. For internal use ONLY.
      *
      * See 'GetGuidTablePtr' for more information.
      *
      */

      RwUInt32 CSeqCtrlDataIttr::GetGuidTableSize(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlDataIttr::GetGuidTableSize");
         RWS_ASSERT(GetInstruction() == IT_GUID_TABLE, "Not a guid table instruction.");
         RWS_RETURN(*reinterpret_cast<const RwUInt32 *>(&m_pDataByte[SEQ_CTRL_OP_SIZE]));
      }

      /**
      *
      * Returns a pointer to a GUID at the specified offset in the GUID table. NOTE that
      * the offset is in BYTES and NOT in GUIDs. That is the first one is at 0, next at 16,
      * then 32, etc... Will assert if the offset is off the end of the guid table or the
      * offset is not a multiple of 16. Returned pointer can be safely kept (when iterator
      * exists) as it points to a stored memory address, not the current contents of a stream.
      *
      * See 'GetGuidTablePtr' for more information.
      *
      */

      const RWSGUID *CSeqCtrlDataIttr::GetGuid(RwUInt32 offset)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlDataIttr::GetGuid");
         RWS_ASSERT(m_guidTableSize != 0, "GUID table NOT setup.");
         RWS_ASSERT(offset < m_guidTableSize, "Offset off end of GUID table.");
         RWS_ASSERT((offset & 0xF) == 0, "Offset is not a multiple of 16.");
         RWS_RETURN(&m_pGuidTable[offset >> 4]);
      }

      /**
      *
      * Displays the current instruction in a data block to the debug console.
      * ONLY works in debug mode. Information is repeated in workspace's log.
      *
      */

      void CSeqCtrlDataIttr::DebugDispCurrent(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlDataIttr::DebugDispCurrent");

         // Which instruction type ?
         //
         // NOTE: String and other tables not shown here. Call separate functions,
         //       such as 'DebugDispStringTable' to show these. This function only
         //       displays user illegal instruction, not internal instructions.

         switch (GetInstruction())
         {
         case IT_ERROR:

            RWS_ASSERTFAIL("ERROR - error found in stream.");
            break;

         case IT_WAIT:

            RWS_DEBUGSTREAM("WAIT - time = [" << GetWaitTime() <<
               "] milliseconds." << std::endl);
            break;

         case IT_ENTITY:

            RWS_DEBUGSTREAM("ENTITY - guid = [" << *GetEntityGuid() << std::endl);
            break;

         case IT_CLASS:

            RWS_DEBUGSTREAM("CLASS - name [" << GetClassName() << "] string table offset [" <<
               GetClassNameOffset() << "]." << std::endl);
            break;

         case IT_COMMAND:

            RWS_DEBUGSTREAM("COMMAND - number [" << GetCommand() << "]." << std::endl);
            break;

         case IT_KEYFRAME:
            {
               RWS_DEBUGSTREAM("KEYFRAME - data type [" << GetKeyframeTypeName() <<
                  "] (string table offset [" << GetKeyframeTypeNameOffset() << "]) address [" <<
                  std::hex << GetKeyframeDataPtr() << "] size [" << std::dec << 
                  GetKeyframeDataSize() << "]." << std::endl);
            }
            break;

         case IT_INTERPOL:
            {
               RWS_DEBUGSTREAM("INTERPOLATION - interpolator type [" <<
                  GetInterpolName() << "] (string table offset [" <<
                  GetInterpolNameOffset() << "]) over [" << GetInterpolTime() << 
                  "] msecs." << std::endl);
            }
            break;

         case IT_SEND_EVENT:

            RWS_DEBUGSTREAM("SEND EVENT - name [" << GetEventName() << "] string table offset [" << 
               GetEventNameOffset() << "]." << std::endl);
            break;

         case IT_WAIT_EVENT:

            RWS_DEBUGSTREAM("WAIT EVENT - name [" << GetWaitEventName() << "] string table offset [" <<
               GetWaitEventNameOffset() << "]." << std::endl);
            break;
            
         case IT_CREATE:
            {
               RWSGUID tempGuid;

               GetCreateGuid(tempGuid);
               RWS_DEBUGSTREAM("CREATE - GUID [" << tempGuid << "]." << std::endl);
            }
            break;

         case IT_DELETE:

            RWS_DEBUGSTREAM("DELETE - GUID [" << *GetGuid(GetDeleteGuidOffset()) << 
               "]." << std::endl);
            break;

         case IT_EOF:

            RWS_DEBUGSTREAM("EOF - end of file reached.\n" << std::endl);
            break;

         case IT_DYNAMIC_INTERPOL:

            RWS_DEBUGSTREAM("DYNAMIC INTERPOL - " << std::endl);
            break;
            
         default:

            RWS_DEBUGSTREAM_WARN("UNKNOWN - unknown instruction type specified." << std::endl);
            break;
         }
         
         RWS_RETURNVOID();
      }

      /**
      *
      * Displays all the strings in the string table to the debug console, in the order in
      * which they appear in the table, with the offset value.
      *
      */

      void CSeqCtrlDataIttr::DebugDispStringTable(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlDataIttr::DebugDispCurrent");

         // Is there a table ?

         RWS_DEBUGSTREAM("String table within sequence instruction block is...\n" << std::endl);

         if (m_pStringTable && m_stringTableSize)
         {
            RwUInt32 offset = 0;
            const char *pString;

            // Do each string in turn - NOTE: these are standard string and NOT RenderWare ones.

            while (offset < m_stringTableSize)
            {
               pString = m_pStringTable + offset;

               if (*pString == '\0')
               {
                  // Only zero length string is at the end of the table (unless damaged data)
                  // so just exit the loop.
                  
                  break;
               }
               else
               {
                  RWS_DEBUGSTREAM("   [" << offset << "] = [" << (m_pStringTable + offset) <<
                     "]" << std::endl);
               }

               offset += strlen(pString) + 1;
            }

            RWS_DEBUGSTREAM(std::endl);
         }
         else
         {
            RWS_DEBUGSTREAM_ERR("There is no string table - illegal." << std::endl);
         }

         RWS_RETURNVOID();
      }

      /**
      *
      * not documented
      *
      */

      void CSeqCtrlDataIttr::DebugDispGuidTable(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlDataIttr::DebugDispGuidTable");
         RWS_ASSERT((m_guidTableSize & 0xF) == 0, "Guid table is NOT a multiple of 16.");

         // Is there a table ?

         RWS_DEBUGSTREAM("GUID table within sequence instruction block is...\n" << std::endl);

         if (m_pGuidTable && m_guidTableSize)
         {
            RwUInt32 offset = 0;

            // Do each guid in turn...

            while (offset < m_guidTableSize)
            {
               const RWSGUID *pGuid = GetGuid(offset);

               RWS_DEBUGSTREAM("   [" << offset << "] = [" << *pGuid << "]" << std::endl);
               offset += 16;
            }

            RWS_DEBUGSTREAM(std::endl);
         }
         else
         {
            RWS_DEBUGSTREAM_ERR("There is no GUID table - illegal." << std::endl);
         }

         RWS_RETURNVOID();
      }

      ///////////////////////////////
      //
      // CSeqCtrlDataNode functions.
      //
      //

      /**
      *
      * Constructor - initialize object and sets the internal reference to the passed
      * in data object - this data object is NOT copied.
      *
      */
      
      CSeqCtrlDataNode::CSeqCtrlDataNode(CSeqCtrlData &rData)
         : m_pNext(0),
         m_pData(&rData)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlDataNode::CSeqCtrlDataNode");
         RWS_RETURNVOID();
      }

      /**
      *
      * Links this node BEFORE the passed in node, ie the passed in node will become
      * the 'next' node after this one.
      *
      */

      void CSeqCtrlDataNode::LinkTo(CSeqCtrlDataNode &rNode)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlDataNode::LinkTo");

         m_pNext = &rNode;

         RWS_RETURNVOID();
      }

      /**
      *
      * Unlinks this node from the next node and returns the node that used to be next
      * in the list or 0 if there wasn't one.
      *
      */

      CSeqCtrlDataNode *CSeqCtrlDataNode::UnLink(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlDataNode::UnLink");

         CSeqCtrlDataNode *pOriginalNext = m_pNext;
         m_pNext = 0;

         RWS_RETURN(pOriginalNext);
      }

      /**
      *
      * Returns a pointer to the next node in the link list or 0 if there isn't one.
      *
      */

      CSeqCtrlDataNode *CSeqCtrlDataNode::Next(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlDataNode::Next");
         RWS_RETURN(m_pNext);
      }

      /**
      *
      * Returns a pointer to the data object held by the node.
      *
      */

      CSeqCtrlData *CSeqCtrlDataNode::Data(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlDataNode::Data");
         RWS_RETURN(m_pData);
      }

      /////////////////////////////
      //
      // CSeqCtrlKeyData functions.
      //
      //

      /**
      *
      * Constructor - pre-allocates the buffer for data, can be zero. If the 'Set' function is 
      *               called with data that exceeds this size, the buffer will be resized.
      *
      *         NOTE: the name length INCLUDES the terminator.
      *
      */

      CSeqCtrlKeyData::CSeqCtrlKeyData(RwUInt32 maxDataSize, RwUInt32 memType, RwUInt32 flags) :
         m_name(0),
         m_dataSize(0),
         m_maxNameLen(0),
         m_maxDataSize(0),
         m_flags(flags),
         m_pData(0)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlKeyData::CSeqCtrlKeyData");

         Resize(maxDataSize, memType);

         RWS_RETURNVOID();
      }

      /**
      *
      * Destructor - free up any used memory.
      *
      */

      CSeqCtrlKeyData::~CSeqCtrlKeyData(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlKeyData::~CSeqCtrlKeyData");

         // Release memory, may not have been allocated.

         operator delete(m_pData);        // Should cope with NULL ptr.

         RWS_RETURNVOID();
      }

      /**
      *
      * Sets the values to the passed in data, if necessary the buffers will be resized to
      * cope (buffer are, however, never reduced in size).
      *
      * NOTE : this function has been written to work with a system which uses a string
      *        table. It therefore does NOT copy the passed in name, only the pointer value.
      *
      */

      void CSeqCtrlKeyData::Set(const RwChar *name, const void *pData,
         RwUInt32 dataSize, RwUInt32 memType)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlKeyData::Set");
         RWS_ASSERT(name, "Name must be valid.");
         RWS_ASSERT(pData, "Data pointer must be valid.");
         RWS_ASSERT(dataSize || !rwstrcmp(name, MSG_LINK_NAME_TYPE),
            "Zero data size is illegal for none 'message link name' keys.");

         // Call resize to handle any necessary up-sizing of the buffer.

         Resize(dataSize, memType);

         // Copy the data (necessary as may drop off the end of the stream buffer during an
         // interpolation). DO NOT NEED to copy name, as using a string table which is copied
         // out of the stream - ie the memory address is ASSUMED TO STAY VALID.

         m_name = name;
         memcpy(m_pData, pData, dataSize);
         m_dataSize = dataSize;

         RWS_RETURNVOID();
      }

      /**
      *
      * Returns a pointer to the name stored in the object or zero if none set.
      *
      */

      const RwChar *CSeqCtrlKeyData::Name(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlKeyData::Name");
         RWS_RETURN(m_name);
      }

      /**
      *
      * Returns a pointer to the data held in the object or zero if none set.
      *
      */

      const void *CSeqCtrlKeyData::Data(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlKeyData::Data");
         RWS_RETURN(m_pData);
      }

      /**
      *
      * Returns the size of the data held or zero if none held.
      *
      */

      RwUInt32 CSeqCtrlKeyData::DataSize(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlKeyData::DataSize");
         RWS_RETURN(m_dataSize);
      }

      /**
      *
      * Checks and if necessary up sizes the buffers. If not the first time, warning
      * will be emitted to indicate possible memory fragmentation. In case of data,
      * the old data is NOT copied on resize.
      *
      */

      void CSeqCtrlKeyData::Resize(RwUInt32 newDataSize, RwUInt32 memType)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCtrlKeyData::Resize");

         if (newDataSize > m_maxDataSize)
         {
            void *pNewData = RWS_OP_NEW_TYPE(newDataSize, memType);
            RWS_ASSERT(pNewData, "Failed to resize data buffer.");

            // Free up old data ?

            if (m_pData)
            {
               if ((m_flags & SEQ_FLAG_BUFFER_WARNINGS) != 0)
               {
                  RWS_DEBUGSTREAM_WARN("Resizing keyframe data buffer - "
                     "may cause memory fragmentation." << std::endl);
               }
               operator delete(m_pData);
            }

            m_pData = pNewData;
            m_maxDataSize = newDataSize;
         }

         RWS_RETURNVOID();
      }

      //////////////////////////////////
      //
      // CSeqCtrlInterpolData functions.
      //
      //

      /**
      *
      * Constructor - pre-allocates the buffer for the data, these can be zero. If the 'Set'
      *               function is called with data that exceeds these sizes, the buffers
      *               will be resized.
      *
      *         NOTE: the name length INCLUDES the terminator.
      *
      */

      CSeqCtrlInterpolData::CSeqCtrlInterpolData(RwUInt32 maxDataSize,
            RwUInt32 memType, RwUInt32 flags) :
         CSeqCtrlKeyData(maxDataSize, memType, flags),
         m_lengthTime(0)
         
#ifdef RWS_DESIGN

         ,
         m_isDynamic(FALSE),
         m_dynamicDelta(0.0f)
#endif
      {
         RWS_FUNCTION("CSeqCtrlInterpolData::CSeqCtrlInterpolData");
         RWS_RETURNVOID();
      }

      /**
      *
      * Destructor - release memory, etc.
      *
      */

      CSeqCtrlInterpolData::~CSeqCtrlInterpolData(void)
      {
         RWS_FUNCTION("CSeqCtrlInterpolData::~CSeqCtrlInterpolData");
         RWS_RETURNVOID();
      }

      /**
      *
      * Sets the values to the passed in data, if necessary the buffers will be resized to
      * cope (buffer are, however, never reduced in size). Length time CANNOT be zero. In
      * the interpolator it is legal to have a zero data pointer & data size.
      *
      * NOTE : this function has been written to work with a system which uses a string
      *        table. It therefore does NOT copy the passed in name, only the pointer value.
      *
      */

      void CSeqCtrlInterpolData::Set(const RwChar *name, const void *pData, RwUInt32 dataSize,
         RwUInt32 lengthTime, RwUInt32 memType)
      {
         RWS_FUNCTION("CSeqCtrlInterpolData::Set");
         RWS_ASSERT(lengthTime, "Cannot have zero length interpolations.");
         RWS_ASSERT(name, "Name must be valid.");

         // Call resize to handle any necessary up-sizing of the buffer.

         Resize(dataSize, memType);

         // Copy the data (necessary as may drop off the end of the stream buffer during an
         // interpolation). DO NOT NEED to copy name, as using a string table which is copied
         // out of the stream - ie the memory address is ASSUMED TO STAY VALID.

#ifdef RWS_DESIGN

         m_isDynamic = FALSE;
#endif
         m_name = name;
         memcpy(m_pData, pData, dataSize);
         m_dataSize = dataSize;
         m_lengthTime = lengthTime;

         RWS_RETURNVOID();
      }

      /**
      *
      * Version of set function for dynamic interpolators. Only available in design builds. See
      * standard 'Set' function for more information.
      *
      * NOTE: The length time from a dynamic interpolator is ALWAYS zero.
      *
      */

#ifdef RWS_DESIGN

      void CSeqCtrlInterpolData::Set(const RwChar *name, const void *pData, RwUInt32 dataSize,
         RwReal delta, RwUInt32 memType)
      {
         RWS_FUNCTION("CSeqCtrlInterpolData::Set");
         RWS_ASSERT(name, "Name must be valid.");

         // Call resize to handle any necessary up-sizing of the buffer.

         Resize(dataSize, memType);

         // Copy the data (necessary as may drop off the end of the stream buffer during an
         // interpolation). DO NOT NEED to copy name, as using a string table which is copied
         // out of the stream - ie the memory address is ASSUMED TO STAY VALID.

         m_isDynamic = TRUE;
         m_dynamicDelta = delta;
         m_name = name;
         memcpy(m_pData, pData, dataSize);
         m_dataSize = dataSize;

         // NOTE: Time CANNOT be zero, as interpolators use it for division. Set to smallest
         //       possible VALID number.

         m_lengthTime = 1;

         RWS_RETURNVOID();
      }

#endif

      /**
      *
      * Returns the length time, which may be zero if it has never been set.
      *
      */

      RwUInt32 CSeqCtrlInterpolData::LengthTime(void)
      {
         RWS_FUNCTION("CSeqCtrlInterpolData::LengthTime");
         RWS_RETURN(m_lengthTime);
      }
   }  // namespace SEQUENCE
}     // namespace RWS
