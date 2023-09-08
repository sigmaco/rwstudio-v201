/*****************************************************************************
*
* File :      CSeqCtrlData.h
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

#ifndef SEQUENCE_DATA_HEADER
#define SEQUENCE_DATA_HEADER

#include "framework\core\macros\debugmacros.h"
#include "framework\core\misc\rwsguid.h"
#include "framework\core\attributehandler\cattributehandler.h"

namespace RWS
{
   namespace SEQUENCE
   {
      /**
      *
      * Defines the chunk id for the stream containing the sequence control data.
      *
      * NOTE: This value MUST match the one used in the workspace build rules.
      *
      */

      const RwUInt32 rwID_SEQUENCE = MAKECHUNKID (rwVENDORID_CRITERIONRM, 0x80);

      /**
      *
      * Flag used to control output of warnings from the sequence classes.
      *
      */

      const RwUInt32 SEQ_FLAG_BUFFER_WARNINGS = 0x00000001;

      /**
      *
      * Used to hold an sequence controller data block.
      *
      */

      class CSeqCtrlData
      {
      public:

         CSeqCtrlData(RwStream* pStream, const RwChar *pName,
            RwChunkHeaderInfo *pChunckHeader = 0);
         ~CSeqCtrlData(void);

         const RwChar *Name(void);
         const void *RawData(void) const;
         RwUInt32 DataSize(void) const;

      private:

         RwUInt8 *m_pRawData;
         RwChar *m_pName;
         RwUInt32 m_dataSize;
      };

      /**
      *
      * Used to hold information on keyframe data from the sequence data. This provides a
      * re-usable buffer for the information, so as to be stream compatible. Should be
      * allocated once per sequence item (track) of the sequence and re-used.
      *
      */

      class CSeqCtrlKeyData
      {
      public:
         
         CSeqCtrlKeyData(RwUInt32 maxDataSize, RwUInt32 memType, RwUInt32 flags = 0);
         virtual ~CSeqCtrlKeyData(void);

         void Set(const RwChar *name, const void *pData, RwUInt32 dataSize, RwUInt32 memType);
         const char *Name(void);
         const void *Data(void);
         RwUInt32 DataSize(void);

      protected:

         void Resize(RwUInt32 newDataSize, RwUInt32 memType);

         const char *m_name;
         RwUInt32 m_dataSize,
            m_maxNameLen,
            m_maxDataSize,
            m_flags;
         void *m_pData;
      };

      /**
      *
      * Used to hold information on interpolation from the sequence data. This provides a
      * re-usable buffer for the information, so as to be stream compatible. Should be
      * allocated once per sequence item (track) of the sequence and re-used.
      *
      */

      class CSeqCtrlInterpolData : public CSeqCtrlKeyData
      {
      public:

         CSeqCtrlInterpolData(RwUInt32 maxDataSize, RwUInt32 memType, RwUInt32 flags = 0);
         virtual ~CSeqCtrlInterpolData(void);

         void Set(const RwChar *name, const void *pData, RwUInt32 dataSize,
            RwUInt32 lengthTime, RwUInt32 memType);
         RwUInt32 LengthTime(void);

#ifdef RWS_DESIGN

         void Set(const RwChar *name, const void *pData, RwUInt32 dataSize,
            RwReal delta, RwUInt32 memType);
#else
         void Set(const RwChar *name, const void *pData, RwUInt32 dataSize,
            RwReal delta, RwUInt32 memType) {}
#endif

         // Inline functions.

         /**
         *
         * Returns if the interpolator is a dynamic interpolator or not.
         *
         */

#ifdef RWS_DESIGN

         RwBool IsDynamic(void) { return(m_isDynamic); }
#else
         RwBool IsDynamic(void) { return(FALSE); }
#endif

         /**
         *
         * Returns the dynamic delta value or zero if not a dynamic interpolator.
         *
         */

#ifdef RWS_DESIGN

         RwReal GetDelta(void) { return((IsDynamic() ? m_dynamicDelta : 0.0f)); }
#else
         RwReal GetDelta(void) { return(0.0f); }
#endif

      protected:

         RwUInt32 m_lengthTime;

#ifdef RWS_DESIGN

         RwBool m_isDynamic;
         RwReal m_dynamicDelta;
#endif

      private:

         void Set(const RwChar *name, const void *pData,
            RwUInt32 dataSize, RwUInt32 memType) {};     /**< To invalidate calls, FORCES
                                                              call extra param version. */
      };

      /**
      *
      * Used to iterate over the instructions in a sequence controller data block. Base type
      * for doing this type of itterator. Can be user overridden to allow new instructions to
      * be handled by inheriting from it. Calling code would need to be extended too, thereby
      * allowing it to understand the new instructions.
      *
      */

      class CSeqCtrlDataIttr
      {
      public:

         /**
         *
         * Lists the types of instructions. Only uses 1 byte, so values from 0 to 255 ONLY can
         * be used - this will become an issue if user extended. In this case the user types
         * MUST be offset from IT_USERBASE, as the number of basic instructions may change.
         *
         */

         enum InstructionType
         {
            IT_ERROR = 0,           /**< Error, unknown instruction type. Very bad! Corruption? */

            IT_STRING_TABLE,        /**< Internal use only, this should never be seen by the user.
                                         Pointer via 'GetStringTablePtr'. Size via 'GetStringTableSize'. */
            IT_GUID_TABLE,          /**< Internal use only, this should never be seen by the user.
                                         Pointer via 'GetGuidTablePtr'. Size via 'GetGuidTableSize'. */

            IT_RESERVED,            /**< Not yet used. */

            IT_WAIT,                /**< Get data via 'GetWaitTime'. */
            IT_ENTITY,              /**< Get data via 'GetEntityGuid'. */
            IT_CLASS,               /**< Get data via 'GetClassName'. */
            IT_COMMAND,             /**< Get data via 'GetCommand'. */
            IT_KEYFRAME,            /**< Get data via 'GetKeyframe'. */
            IT_INTERPOL,            /**< Get data via 'GetInterpol'. */
            IT_SEND_EVENT,          /**< Get data via 'GetEventName'. */
            IT_WAIT_EVENT,          /**< Get data via 'GetWaitEventName. */
            IT_CREATE,              /**< Get guid via 'GetCreateGuid', data via 'GetCreateData'. */
            IT_DELETE,              /**< Get guid via 'GetDeleteGuidOffset'. */

            IT_EOF,                 /**< End of file (instruction data) reached. */

            IT_DYNAMIC_INTERPOL,    /**< Get delta via 'GetDynamicInterpolDelta', data via
                                         'GetInterpol' and name via 'GetInterpolName'.
                                         ONLY supported in design builds. */

            IT_USERBASE             /**< The base value for the user instruction types. */
         };

         CSeqCtrlDataIttr(RwUInt32 memType, RwUInt32 strBuffSize = 0, RwUInt32 guidBuffSize = 0,
            RwUInt32 flags = 0);
         CSeqCtrlDataIttr(const CSeqCtrlData &dataObj, RwUInt32 memType,
            RwUInt32 strBuffSize = 0, RwUInt32 guidBuffSize = 0, RwUInt32 flags = 0);
         virtual ~CSeqCtrlDataIttr(void);

         void Use(const CSeqCtrlData &dataObj);
         void Reset(void);
         RwUInt32 GetInstruction(void);            /**< Returns 'InstructionType' values. */

         void PreAllocMem(RwUInt32 strBuffSize, RwUInt32 guidBuffSize);
         void FreeMem(void);
         
         void DebugDispCurrent(void);
         void DebugDispStringTable(void);
         void DebugDispGuidTable(void);

         const RwUInt32 GetWaitTime(void);
         const RWSGUID *GetEntityGuid(void);
         const RwChar *GetClassName(void);
         RwUInt32 GetClassNameOffset(void);
         const RwUInt32 GetCommand(void);
         const RwChar *GetKeyframeTypeName(void);
         RwUInt32 GetKeyframeTypeNameOffset(void);
         const void *GetKeyframeDataPtr(void);
         RwUInt32 GetKeyframeDataSize(void);
         const char *GetInterpolName(void);
         RwUInt32 GetInterpolNameOffset(void);
         RwUInt32 GetInterpolTime(void);
         const char *GetEventName(void);
         RwUInt32 GetEventNameOffset(void);
         void GetCreateGuid(RWSGUID &rGuid);
         const CAttributePacket *GetCreateData(void);
         RwUInt32 GetDeleteGuidOffset(void);
         const char *GetWaitEventName(void);
         RwUInt32 GetWaitEventNameOffset(void);
         const char *GetString(RwUInt32 offset);
         const RWSGUID *GetGuid(RwUInt32 offset);

#ifdef RWS_DESIGN

         RwReal GetDynamicInterpolDelta(void);
#else
         RwReal GetDynamicInterpolDelta(void) { return(0.0f); }
#endif

         virtual void GetKeyframe(CSeqCtrlKeyData &rKeyData,              /**< Called inside CSeqItem. */
            RwUInt32 memType);
         virtual void GetInterpol(CSeqCtrlInterpolData &rInterpolData,    /**< Called inside CSeqItem. */
            RwUInt32 memType);

         // Inline functions.

         /**
         *
         * Moves to next instruction within the data block. When at end of block
         * the 'GetInstruction' function will return CT_EOF.
         *
         */

         void CSeqCtrlDataIttr::operator++(int) { MoveToNext(); }

      protected:

         void MoveToNext(void);
         const char *GetStringTablePtr(void);
         RwUInt32 GetStringTableSize(void);
         const RWSGUID *GetGuidTablePtr(void);
         RwUInt32 GetGuidTableSize(void);
         void ResizeStr(RwUInt32 newSize);
         void ResizeGuid(RwUInt32 newSize);

         const RwUInt8 *m_pDataByte,         /**< Points to the current sequence instruction start. */
            *m_pUserDataByteStart,           /**< Points to the first byte of user data in the sequence. */
            *m_pEndDataByte;                 /**< Points to the first byte after the end of the data. */
         RWSGUID *m_pGuidTable;              /**< Points to the first byte of the guid table (copy). */
         char *m_pStringTable;               /**< Points to the first byte of the string table (copy). */
         RwUInt32 m_stringTableSize,         /**< Size of string table, in bytes. This may be smaller than
                                                  the m_strTableBuffSize in which it is allocated. But it
                                                  ALWAYS starts at the beginning of the buffer. If zero, 
                                                  there is no string table. */
            m_guidTableSize,                 /**< Size of guid table, in bytes. This may be smaller than
                                                  the m_guidTableBuffSize in which it is allocated. But it
                                                  ALWAYS starts at the beginning of the buffer. If zero,
                                                  there is no guid table. */
            m_userMemType,                   /**< The type of memory the user asked allocations to use. */
            m_strTableBuffSize,              /**< The size of the buffer that is used to hold strings. */
            m_guidTableBuffSize,             /**< The size of the buffer that is used to hold GUIDs. */
            m_flags;                         /**< Flags used to control aspects of behaviour. OR'ed
                                                  list from the SEQ_ITTR_FLAG_... constants. */
      };

      /**
      *
      * Used to hold a pointer to a CSeqCtrlData object in a link list.
      *
      */

      class CSeqCtrlDataNode
      {
      public:

         CSeqCtrlDataNode(CSeqCtrlData &rData);

         // NOTE: lack of destructor means that the node does NOT delete the
         //       data object it points to on destruction. This MUST be
         //       performed manual, if needed.

         void LinkTo(CSeqCtrlDataNode &rNode);
         CSeqCtrlDataNode *UnLink(void);
         CSeqCtrlDataNode *Next(void);
         CSeqCtrlData *Data(void);

      private:

         CSeqCtrlDataNode *m_pNext;
         CSeqCtrlData *m_pData;
      };
   }        // namespace SEQUENCE
}           // namespace RWS

#endif      // #ifndef SEQUENCE_DATA_HEADER
