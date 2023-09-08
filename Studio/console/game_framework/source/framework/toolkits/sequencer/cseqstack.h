/****************************************************************************
*
* File :      CSeqStack.h
*
* Abstract :  Frame by frame sequence controller for active objects.
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

#ifndef CSEQSTACK_HEADER
#define CSEQSTACK_HEADER

#include "framework\core\macros\debugmacros.h"
#include "framework\core\misc\rwsguid.h"

#include "cseqctrldata.h"
#include "cseqtxattrhand.h"

namespace RWS
{
   namespace SEQUENCE
   {
      // Predefines...

      class CSeqItem;
      class CSeqStack;
      class CSeqNode;
      class CSeqInterpolRegister;

      /**
      *
      * Interpolator 'type name' for linear interpolation. Used in the
      * CSeqCtrlInterpolData.m_typeName string.
      *
      */

      const RwChar INTERPOL_TYPE_LINEAR_NAME[] = RWSTRING("CLinearInterpolator");
      const RwChar INTERPOL_TYPE_ROTATION_NAME[] = RWSTRING("CRotationInterpolator");

      /**
      *
      * Name of type used for message link name sequencing.
      *
      */

      const RwChar MSG_LINK_NAME_TYPE[] = RWSTRING("#msg#");

      /**
      *
      * CSeqInterpolBase can be inherited from to provided specific interpolator
      * objects for each type, including user types. This base object does NOT
      * provide interpolation functionality and ONLY allows keyframes to be set.
      *
      */

      class CSeqInterpolBase
      {
      public:

         CSeqInterpolBase(CSeqItem &rData);
         virtual ~CSeqInterpolBase(void);

         static CSeqInterpolBase *MakeNew(CSeqItem &rData);
         static RwUInt32 MaxSize(void);

         virtual void Set(CSeqItem &rData);
         virtual void Prep(CSeqItem &rData);
         virtual void PrepReuse(CSeqItem &rData);
         virtual void Step(CSeqItem &rData, RwUInt32 nowTime);
         virtual void Step(CSeqItem &rData, RwReal delta);

      protected:

         CSeqInterpolBase(void);
      };

      /**
      *
      * Holds a group of 'same typed' interpolator objects. Used by the 
      * 'CSeqInterpolRegister' class. Holds each interpolator type (by name)
      * for the type. CSeqInterpolRegister class holds groups for each type.
      *
      * NOTE: These are instantiated as a result of instantiating
      *       'CSeqInterpolRegister' objects. They do NOT need to delete
      *       any CSeqInterpolRegister objects held internally, as this will
      *       happen when those objects are deleted by their creating code.
      *       In other words, objects of this type are created / destroyed as
      *       a biproduct of creating 'CSeqInterpolRegister' objects.
      *
      */

      class CSeqInterpolRegGroup
      {
      public:

         CSeqInterpolRegGroup(const RwChar *typeName);

         const RwChar *TypeName(void);
         CSeqInterpolRegGroup *Next(void);
         CSeqInterpolRegGroup *Prev(void);
         void LinkInOrder(CSeqInterpolRegGroup *&rpRoot);
         void Unlink(CSeqInterpolRegGroup *&rpRoot);
         CSeqInterpolRegister *FindInterpol(const RwChar *interpolName,
            RwBool specifiedOnly);
         void Add(CSeqInterpolRegister *pRegister);
         RwBool Remove(CSeqInterpolRegister *pRegister);
         
      private:

         const RwChar *m_typeName;              /**< Copy of pointer ONLY, does not copy string. */
         CSeqInterpolRegister *m_pRootReg;
         CSeqInterpolRegGroup *m_pNext,
            *m_pPrev;
      };

      /**
      *
      * Used to register interpolator objects (derived from CSeqInterpolBase) to
      * handle particular types of data. Holds static internal list of type name
      * to interpolator mapping. To use, create global instance of each type name
      * to interpolator link object, ie each 'CSeqInterpolRegister' object for each
      * of the specified types. If the 'typeName' is 0, then the interpolator is a
      * 'default' one. There can be only one of these and it (by definition) will not
      * understand data types, ie it can only SET keyframes and not interpolate
      * between them.
      *
      */

      class CSeqInterpolRegister
      {
      public:

         CSeqInterpolRegister(CSeqInterpolBase *(*pMakeFunc)(CSeqItem &rData),
            RwUInt32 (*pSizeFunc)(void), const RwChar *typeName,
            const RwChar *interpolName, RwBool override);
         ~CSeqInterpolRegister(void);

         CSeqInterpolBase *MakeInterpol(CSeqItem &rData);
         RwUInt32 MaxInterpolMemSize(void);
         const RwChar *InterpolName(void);
         CSeqInterpolRegister *Next(void);
         CSeqInterpolRegister *Prev(void);
         void LinkInOrder(CSeqInterpolRegister *&rpRoot);
         void Unlink(CSeqInterpolRegister *&rpRoot);

         static CSeqInterpolRegGroup *FindGroup(const RwChar *typeName,
            RwBool specifiedOnly);
         static void Sort(void);

      private:

         const RwChar *m_interpolName,          /**< Copy of pointer ONLY, does not copy string. */
            *m_typeName;                        /**< Copy of pointer ONLY, does not copy string. */
         RwBool m_overridden;
         CSeqInterpolBase *(*m_pMakeFunc)(CSeqItem &rData);
         RwUInt32 (*m_pSizeFunc)(void);
         CSeqInterpolRegGroup *m_pGroup;
         CSeqInterpolRegister *m_pNext,
            *m_pPrev;

         static CSeqInterpolRegGroup *sm_pRootGroup;
         static CSeqInterpolRegister *sm_pRootUnsorted;
      };

      /**
      *
      * CSeqItem is used to control the interpolation of keyframes in an
      * sequence controller object. Most data stored in this object is
      * interpolator/object type specific. It is therefore stored via void
      * pointers.
      *
      */

      class CSeqItem
      {
      public:
         
         CSeqItem(RWSGUID &rGuid, const RwChar *pClassName, RwUInt32 commandID,
            const RwChar *typeName, RwUInt32 nowTimer, CSeqStack &rSeqStack,
            RwUInt32 flags = 0);
         virtual ~CSeqItem(void);

         RwBool AddKey(CSeqCtrlDataIttr &rIttr, RwUInt32 nowTime);
         void AddInterpol(CSeqCtrlDataIttr &rIttr);
         RwBool IsActive(void);
         void Process(RwUInt32 nowTime);

         RwUInt32 StartTime(void);
         RwUInt32 LengthTime(void);
         CSeqCtrlKeyData *LastKey(void);
         CSeqCtrlKeyData *EndKey(void);
         void Guid(RwUInt32 &rGuidData0, RwUInt32 &rGuidData1, RwUInt32 &rGuidData2,
            RwUInt32 &rGuidData3);
         const RWSGUID &GuidRef(void);
         RwBool IsSameGuid(RWSGUID &rGuid);
         const RwChar *ClassName(void);
         RwUInt32 CommandID(void);
         RwUInt32 LastUpdateTime(void);

         void *GetInterpolMem(void);
         RwUInt32 GetInterpolMemSize(void);

         CSeqStack *SeqStack(void);
         CSeqAttrHandSlot *BuffSlot(void);
         void SetBuffSlot(CSeqAttrHandSlot &rBuffSlot);
         void DisconnectFromHandler(void);

      protected: 

         RWSGUID m_guid;
         const RwChar *m_pClassName;
         RwUInt32 m_commandID;

      private:

         RwBool SetInterpol(const RwChar *interpolName, RwBool forceSet);
         void ResizeInterpolMem(RwUInt32 memNeeded);

         RwUInt32 m_startTime,                  /**< Current interpolation start time. */
            m_lastUpdateTime;                   /**< Last 'now time' used for updating. */
         RwBool m_isActive;

         // Sub-buffing objects...

         CSeqCtrlKeyData *m_pLastKeyData,
            *m_pEndKeyData;
         CSeqCtrlInterpolData *m_pInterpolData;       /**< Holds length time, etc. */

         // Interpolator stuff...

         CSeqInterpolRegGroup *m_pInterpolRegGroup;
         CSeqInterpolRegister *m_pInterpolReg;
         CSeqInterpolBase *m_pInterpol;
         RwBool m_interpolNextKey,              /**< Indicates if interpolator before next key. */
            m_canReuse;                         /**< Set to indicate the 'PrepReuse' can be called. */
         RwUInt32 m_interpolMemSize,
            m_flags;                            /**< Flags used to modify behaviour, OR'ed flags
                                                     from the SEQ_FLAG_... set. */
         void *m_pInterpolMem;

         // Transmission handler stuff...

         CSeqAttrHandSlot *m_pBuffSlot;         /**< The slot supplied by this buffer manager. */
         CSeqStack *m_pSeqStack;                /**< The stack this item is part of. */
      };

      /**
      *
      * Holds next and previous pointers for CSeqNode, but also allows the access to the
      * functions to be limited - see CSeqNode for more info.
      *
      */

      class CSeqNodeLink
      {
      public:
         
         /**
         *
         * Constructor - just initialize the object.
         *
         */

         CSeqNodeLink(void) :
            m_pNext(0),
            m_pPrev(0)
         {
            RWS_FUNCTION("RWS::SEQUENCE::CSeqNodeLink::CSeqNodeLink");
            RWS_RETURNVOID();
         }
         
         /**
         *
         * Returns next item in list or 0 if none.
         *
         */

         CSeqNode *Next(void)
         {
            RWS_FUNCTION("RWS::SEQUENCE::CSeqNodeLink::Next");
            RWS_RETURN(m_pNext);
         }

         /**
         *
         * Returns previous item in list or 0 if none.
         *
         */

         CSeqNode *Prev(void)
         {
            RWS_FUNCTION("RWS::SEQUENCE::CSeqNodeLink::Prev");
            RWS_RETURN(m_pPrev);
         }

         void Link(CSeqNode *&rpRoot, CSeqNode &pNode);
         void Unlink(CSeqNode *&rpRoot, CSeqNode &pNode);

      private:
         
         CSeqNode *m_pNext,
            *m_pPrev;
      };

      /**
      *
      * Class to hold sequence items in a link list. This needs to be a publicly
      * 'visible' class, but all functionality in it is inaccessible unless from
      * the 'CSeqStack' class. This is the only one which should be able to manipulate
      * the objects / crate / destroy them.
      *
      * Friends : CSeqStack needs this as it manages these items.
      *           CSeqNodeLink needs this as must access next & prev pointers in m_ptrs;
      *
      * Why? This class is returned from the CSeqStack by the 'find' and 'add' functions
      * but the code calling these functions MUST NOT have access to the link list
      * variables (in CSeqNodeLink) as this could compromise the list's validity. However,
      * the calling code should be able to access the CSeqItem base class's functions.
      * This is complicated by the need to hide the implementation of the link list in the
      * CSeqNodeLink class - hence it being mad another class.
      *
      */

      class CSeqNode : public CSeqItem
      {
         friend class CSeqStack;
         friend class CSeqNodeLink;

      private:

         CSeqNode(RWSGUID &rGuid, const RwChar *pClassName, RwUInt32 commandID,
            const RwChar *typeName, RwUInt32 nowTime, CSeqStack &rSeqStack,
            RwUInt32 flags = 0);
         ~CSeqNode(void);

         CSeqNodeLink m_ptrs;
      };

      /**
      *
      * Token used during 'FindFirstItem' and 'FindNextItem' calls in the CSeqStack class.
      *
      */

      class CSeqStackFindToken
      {
         friend class CSeqStack;

      public:

         RwBool IsFound(void) { return(m_pSeqNode != 0); }
         CSeqItem *GetSeqItem(void) { return(m_pSeqNode); }

      private:

         CSeqNode *m_pSeqNode;
         RwBool m_fromActiveList;
         RWSGUID m_guid;
      };

      /**
      *
      * SeqStack which holds a series of sequence items.
      *
      */

      class CSeqStack
      {
      public:

         CSeqStack(RwUInt32 memType, RwUInt32 flags = 0);
         ~CSeqStack(void);

         CSeqNode *AddItem(RWSGUID &rGuid, const RwChar *pClassName, RwUInt32 commandID,
            const RwChar *typeName, RwUInt32 nowTime);
         CSeqNode *FindItem(RWSGUID &rGuid, const RwChar *pClassName, RwUInt32 commandID);
         void ProcessItems(RwUInt32 nowTime);
         void MoveToActive(CSeqNode *pItem);
         void FreeItems(void);
         void FindFirstItem(CSeqStackFindToken &rToken, RWSGUID &rGuid);
         void FindNextItem(CSeqStackFindToken &rToken);

         void Flush(void);
         CSeqAttrHandSlot *AddBuffSlot(CSeqItem &rSeqItem, RwUInt32 dataSize);
         RwUInt32 MemType(void);

         /**
         *
         * Returns TRUE if the stack is empty, ie there are no on-going sequences.
         *
         */

         RwBool IsActiveEmpty(void)
         {
            RWS_FUNCTION("RWS::SEQUENCE::CSeqItem::IsActiveEmpty");
            RWS_RETURN(m_pActiveRoot == 0);
         }
         
      private:

         void EmptyList(CSeqNode *pRoot);
         CSeqNode *FindItemInList(RWSGUID &rGuid, const RwChar *pClassName,
            RwUInt32 commandID, CSeqNode *pRoot);
         void FindNextItemMatch(CSeqStackFindToken &rToken, RwBool moveToNext);

         RwUInt32 m_memType,              /**< The memory 'type' passed in to allocation functions. */
            m_flags;
         CSeqNode *m_pInactiveRoot,
            *m_pActiveRoot;
         CSeqAttrHandManager *m_pBuffManager;
      };
   }        // namespace SEQUENCE
}           // namespace RWS
#endif      // #ifndef CSEQSTACK_HEADER
