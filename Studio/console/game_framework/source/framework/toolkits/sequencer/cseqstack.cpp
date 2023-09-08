/****************************************************************************
*
* File :      CSeqStack.cpp
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

#include <new.h>

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//

#include "cseqstack.h"
#include "cseqtxattrhand.h"

namespace RWS
{
   namespace SEQUENCE
   {
      /////////////////////////////////////////////////////////////////
      //
      // Type name -> interpolator mappings, default & build-in types.
      //
      // By creating a global variable, the mapping will be created and
      // stored for later searching.
      //
      //

      CSeqInterpolRegister gDefaultInterpolReg(CSeqInterpolBase::MakeNew,
         CSeqInterpolBase::MaxSize, 0, 0, FALSE);

      // Handler for message link name update, cannot interpolate these (by
      // default) so just use base interpolator (which can only set values).

      CSeqInterpolRegister gMsgLinkNameDefaultReg(CSeqInterpolBase::MakeNew,
         CSeqInterpolBase::MaxSize, MSG_LINK_NAME_TYPE, 0, FALSE);

      //////////////////////////////
      //
      // CSeqInterpolBase functions.
      //
      //

      /**
      *
      * Constructor - sets up interpolator functions.
      *
      */

      CSeqInterpolBase::CSeqInterpolBase(CSeqItem &rData)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolBase::CSeqInterpolBase");
         RWS_RETURNVOID();
      }

      /**
      *
      * Constructor - initialises only. Called by inherited versions.
      *
      */

      CSeqInterpolBase::CSeqInterpolBase(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolBase::CSeqInterpolBase - default");
         RWS_RETURNVOID();
      }

      /**
      *
      * Destructor.
      *
      */

      CSeqInterpolBase::~CSeqInterpolBase(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolBase::~CSeqInterpolBase");
         RWS_RETURNVOID();
      }

      /**
      *
      * Creates a new instances of the CSeqInterpolBase object (this should be overridden
      * by inherited classes) and returns it to the caller. Asserts if fails to do so.
      * This function is used in conjunction with the 'CSeqInterpolRegister' class.
      *
      * NOTE: This function SHOULD use the block of memory pre-allocated within the CSeqItem
      *       to reduce the chance of memory fragmentation. This can be obtained using the
      *       CSeqItem functions 'GetInterpolMem' and its size via 'GetInterpolMemSize'.
      *
      */

      CSeqInterpolBase *CSeqInterpolBase::MakeNew(CSeqItem &rData)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolBase::MakeNew");
         RWS_ASSERT(rData.GetInterpolMem(), "Memory buffer is invalid");
         RWS_ASSERT(rData.GetInterpolMemSize() >= sizeof(CSeqInterpolBase), "Memory buffer too small");

         // Use placement new to create the object within the memory buffer...

         CSeqInterpolBase *pBase = new(rData.GetInterpolMem()) CSeqInterpolBase(rData);
         RWS_ASSERT(pBase, "Failed to allocate new instance.");

         RWS_RETURN(pBase);
      }
      
      /**
      *
      * Returns the maximum size the object will every be. This is used to allow the sequence
      * item to allocate a buffer large enough for the all interpolators it will use. This
      * should include any sub-blocks the interpolator wants to use as the interpolator SHOULD
      * NOT allocate blocks on the heap directly, this could cause memory fragmentation. It
      * should instead request a size equal to itself + max buffer size and then offset the
      * buffer it needs from the passed in memory pointer. That is, get's a block of memory
      * from 'CSeqItem' and...
      *
      * +--------------------+  <- START (of buffer provided by CSeqItem)
      * | Instance of class. |
      * +--------------------+  <- START + 'sizeof' class
      * |                    |
      * | Use this space for |
      * / buffer it would    /
      * . otherwise directly .
      * / allocate with new  /
      * | and delete         |
      * |                    |
      * +--------------------+  <- START + (at least) size returned by this function.
      *
      */

      RwUInt32 CSeqInterpolBase::MaxSize(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolBase::MaxSize");
         RWS_RETURN(sizeof(CSeqInterpolBase));
      }

      /**
      *
      * Basic set function, just copies data into buffer (resizing if necessary) and
      * sends to the target behaviour. Resizing is very bad, only does this if forced
      * to do so, ie data will not fit in space. If data size is smaller the buffer is
      * NOT shrunk. Resizing can cause memory fragmentation.
      *
      * Uses data held in the CSeqItem's END keyframe.
      *
      */

      void CSeqInterpolBase::Set(CSeqItem &rData)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolBase::Set");
         RWS_ASSERT(rData.EndKey(), "No end keyframe to use.");
         RWS_ASSERT(rData.EndKey()->Name(), "End-keyframe type name is invalid.");
         RWS_ASSERT(rData.EndKey()->DataSize() ||
            !rwstrcmp(rData.EndKey()->Name(), MSG_LINK_NAME_TYPE),
            "Zero end-keyframe data size - only valid for message link name data.");
         RWS_ASSERT(rData.EndKey()->Data() ||
            !rwstrcmp(rData.EndKey()->Name(), MSG_LINK_NAME_TYPE),
            "Zero data pointer in end-keyframe - only valid for message link name data.");

         void *pBuff;

         // Write data to this buffer...

         RWS_ASSERT(rData.BuffSlot(), "no slot to set data with.");

         if (rData.BuffSlot()->IsDirectSet())
         {
            pBuff = rData.BuffSlot()->GetDirectDataSlot();

            if (pBuff)
            {
               memcpy(pBuff, rData.EndKey()->Data(), rData.EndKey()->DataSize());
               rData.BuffSlot()->DirectUpdate(pBuff);
            }
            else
            {
               rData.BuffSlot()->DirectUpdate(rData.EndKey()->Data());
            }
         }
         else
         {
            pBuff = rData.BuffSlot()->GetBuffDataSlot(rData.EndKey()->DataSize());
            memcpy(pBuff, rData.EndKey()->Data(), rData.EndKey()->DataSize());
         }

         RWS_RETURNVOID();
      }

      /**
      *
      * Basic prepare for interpolation function. This one ASSERTS if called, because the
      * basic object has no idea how to interpolate the data in it sending and so cannot
      * prepare for an interpolation between two values.
      *
      * Normally this function (in an inherited class) would prepare step rates, etc ready
      * for use in the one or more following calls to the 'Step' function.
      *
      */

      void CSeqInterpolBase::Prep(CSeqItem &rData)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolBase::Prep");
         RWS_ASSERTFAIL("Do not know how to handle data interpolation.");
         RWS_RETURNVOID();
      }

      /**
      *
      * Basic prepare for interpolation function. This one ASSERTS if called, because the
      * basic object has no idea how to interpolate the data in it sending and so cannot
      * prepare for an interpolation between two values.
      *
      * Normally this function (in an inherited class) would prepare step rates, etc ready
      * for use in the one or more following calls to the 'Step' function.
      *
      */

      void CSeqInterpolBase::PrepReuse(CSeqItem &rData)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolBase::PrepReuse");
         RWS_ASSERTFAIL("Do not know how to handle data interpolation.");
         RWS_RETURNVOID();
      }

      /**
      *
      * Basic step interpolation function. This one ASSERTS if called, because the
      * basic object has no idea how to interpolate the data in it sending and so cannot
      * step an interpolation between two values.
      *
      * Normally this function would use data prepared in the 'Prep' to step the
      * interpolation to the specified time.
      *
      * This function is called DIRECTLY for running of pre-prepared sequences. However,
      * it is NOT called (directly) for dynamically edited (real-time) sequences. Obviously,
      * the individual interpolators may call it from the 'Step(CSeqItem &rData, RwReal delta)'
      * version of the function.
      *
      */

      void CSeqInterpolBase::Step(CSeqItem &rData, RwUInt32 nowTime)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolBase::Step");
         RWS_ASSERTFAIL("Do not know how to handle data interpolation.");
         RWS_RETURNVOID();
      }

      /**
      *
      * Basic step interpolation function for a delta value. This one ASSERTS if called,
      * because the basic object has no idea how to interpolate the data in it sending
      * and so cannot step an interpolation between two values.
      *
      * Normally this function would use data prepared in the 'Prep' to step the
      * interpolation to the specified time.
      *
      * The delta value may be anything between 0.0 (start key) and 1.0 (end key). Other
      * values must be calculated.
      *
      * This function is called DIRECTLY for dynamically edited (real-time) sequences and
      * is NOT called (directly) for pre-prepared sequences. Obviously, the individual
      * interpolators may call it from the 'Step(CSeqItem &rData, RwUInt32 nowTime)' version
      * of the function.
      *
      */

      void CSeqInterpolBase::Step(CSeqItem &rData, RwReal delta)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolBase::Step");
         RWS_ASSERTFAIL("Do not know how to handle data interpolation.");
         RWS_RETURNVOID();
      }

      //////////////////////////////////
      //
      // CSeqInterpolRegGroup functions.
      //
      //

      /**
      *
      * Constructor - stores type name and initialises the object. Type name must be either
      *               zero or point to a valid STATICALLY allocated string. If zero, then
      *               this group will be a default handler group - which will only be allowed
      *               entries which are themselves default handlers (ie don't interpolate and
      *               have a type name pointer of zero too).
      *
      *         NOTE: The string MUST be static as the class only takes a copy of the address
      *               of the string, not the string itself. It cannot do this as the 
      *               RenderWare string function pointers are not setup at this point, because
      *               the class is used as globally allocated objects (normally).
      *
      *               The 'Sort' function should be called after the string functions have been
      *               setup - if this is not called, it will occur automatically, but this may
      *               result in a stall before the first sequence is played.
      *
      */

      CSeqInterpolRegGroup::CSeqInterpolRegGroup(const RwChar *typeName) :
         m_pRootReg(0),
         m_pNext(0),
         m_pPrev(0)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolRegGroup::CSeqInterpolRegGroup");

         // Store pointer to type name string.

         m_typeName = typeName;
         
         RWS_RETURNVOID();
      }

      /**
      *
      * Returns the type name string - this pointer may be zero if this is a default
      * handler group.
      *
      */

      const RwChar *CSeqInterpolRegGroup::TypeName(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolRegGroup::TypeName");
         RWS_RETURN(m_typeName);
      }

      /**
      *
      * Returns the pointer to the next group in the list or zero if there isn't one.
      *
      */

      CSeqInterpolRegGroup *CSeqInterpolRegGroup::Next(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolRegGroup::Next");
         RWS_RETURN(m_pNext);
      }

      /**
      *
      * Returns the pointer to the previous group in the list or zero if there isn't one.
      *
      */

      CSeqInterpolRegGroup *CSeqInterpolRegGroup::Prev(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolRegGroup::Prev");
         RWS_RETURN(m_pPrev);
      }

      /**
      *
      * Links this interpolator group into the passed in list.
      *
      * NOTE: that 'default' entries will ALWAYS be placed at the top of the list.
      *
      * Also, items will be inserted in alphabetical (via rwstrcmp) order on type name.
      * Therefore the RenderWare string functions MUST have been setup before this
      * function is called.
      *
      */

      void CSeqInterpolRegGroup::LinkInOrder(CSeqInterpolRegGroup *&rpRoot)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolRegGroup::LinkInOrder");

         // Default function ?

         if (m_typeName == 0)
         {
            // Insert at root of list.

            if (rpRoot && rpRoot->m_typeName == 0)
            {
               RWS_ASSERTFAIL("Already have default, cannot add second."
                  " Original default must be removed first.");
            }
            else
            {
               m_pNext = rpRoot;
               m_pPrev = 0;

               if (rpRoot)
               {
                  rpRoot->m_pPrev = this;
               }

               rpRoot = this;
            }
         }
         else
         {
            // Normal handler - insert in alphabetical order. Skip over default, if one.

            if (rpRoot)
            {
               CSeqInterpolRegGroup *pCur = rpRoot;
               int cmpRes;

               // Is there a default to step over ?

               if (pCur && pCur->m_typeName == 0)
               {
                  if (pCur->m_pNext)
                  {
                     // There is an item after it (non-default) to check, so move past default.

                     pCur = pCur->m_pNext;
                  }
                  else
                  {
                     // There is no next, so just link after it.

                     pCur->m_pNext = this;
                     m_pPrev = pCur;
                     m_pNext = 0;

                     RWS_RETURNVOID();
                  }
               }

               // Scan list for insertion point, need cmpRes calculated EACH and EVERY
               // time, so must not be conditionally-short-circuited, ie as a result of
               // and && or ||.

               while ((cmpRes = rwstrcmp(m_typeName, pCur->m_typeName)) > 0
                  && pCur->m_pNext)
               {
                  pCur = pCur->m_pNext;
               }

               // Insert before or after ? Or is it the same (error) ?

               RWS_ASSERT(cmpRes, "Attempt to register second interpolator with the same name.");

               if (cmpRes < 0)
               {
                  // Before...

                  if (pCur->m_pPrev)
                  {
                     pCur->m_pPrev->m_pNext = this;
                  }
                  else
                  {
                     // It's the root.

                     rpRoot = this;
                  }

                  m_pPrev = pCur->m_pPrev;
                  m_pNext = pCur;
                  pCur->m_pPrev = this;
               }
               else
               {
                  // After...

                  if (pCur->m_pNext)
                  {
                     pCur->m_pNext->m_pPrev = this;
                  }
                  m_pNext = pCur->m_pNext;
                  m_pPrev = pCur;
                  pCur->m_pNext = this;
               }
            }
            else
            {
               // First item.

               rpRoot = this;
               m_pPrev = 0;
               m_pNext = 0;
            }
         }

         RWS_RETURNVOID();
      }

      /**
      *
      *
      *
      */

      void CSeqInterpolRegGroup::Unlink(CSeqInterpolRegGroup *&rpRoot)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolRegGroup::Unlink");

         if (rpRoot == this)
         {
            rpRoot = rpRoot->m_pNext;
         }

         if (m_pPrev)
         {
            m_pPrev->m_pNext = m_pNext;
         }

         if (m_pNext)
         {
            m_pNext->m_pPrev = m_pPrev;
         }

         m_pPrev = m_pNext = 0;

         RWS_RETURNVOID();
      }

      /**
      *
      * Looks through the list of interpolators and returns a pointer to the one matching
      * the passed in name string. Return zero if one cannot be found OR the default
      * (handle all keyframes but no interpolation) object, if there is one. The caller
      * can specifically request the default (non-interpolator, key frame only) function
      * by setting the interpolName pointer to zero.
      *
      * If the 'specifiedOnly' flag is TRUE then if the interpolName is specified (not == 0)
      * and the specified interpolator cannot be found, the default interpolator will NOT
      * be returned - zero will instead.
      *
      * Uses the RenderWare string functions and MUST NOT be called until these functions
      * have been initialised.
      *
      */

      CSeqInterpolRegister *CSeqInterpolRegGroup::FindInterpol(const RwChar *interpolName,
         RwBool specifiedOnly)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolRegGroup::FindInterpol");

         // Is caller requesting the default interpolator ?

         if (interpolName == 0)
         {
            // Is there a default ?

            if (m_pRootReg && m_pRootReg->InterpolName() == 0)
            {
               RWS_RETURN(m_pRootReg);
            }
            else
            {
               RWS_RETURN(0);
            }
         }
         else
         {
            CSeqInterpolRegister *pCurReg = m_pRootReg;
            int cmpRes;

            // Skip default group, if one, for now...

            if (pCurReg && pCurReg->InterpolName() == 0)
            {
               pCurReg = pCurReg->Next();
            }

            // Scan the groups looking for a matching one...

            while (pCurReg && (cmpRes = rwstrcmp(interpolName, pCurReg->InterpolName())) > 0)
            {
               pCurReg = pCurReg->Next();
            }

            // Was one found ?

            if (!pCurReg || (pCurReg && cmpRes != 0))
            {
               // No, so is there a default that can be used ?

               if (!specifiedOnly && m_pRootReg && m_pRootReg->InterpolName() == 0)
               {
                  // Yep, so use this.

                  pCurReg = m_pRootReg;

                  // Inform user that requested interpolator could not be found...

                  RWS_DEBUGSTREAM_ERR("Could not find [" << interpolName <<
                     "] interpolator for [" <<
                     (m_typeName ? m_typeName : "DEFAULT") << 
                     "] type - using default interpolator." << std::endl);

                  CSeqInterpolRegister *pReportReg = m_pRootReg;

                  while (pReportReg)
                  {
                     RWS_DEBUGSTREAM("Did find [" << (pReportReg->InterpolName() ?
                        pReportReg->InterpolName() : "DEFAULT") << 
                        "] interpolator." << std::endl);
                     pReportReg = pReportReg->Next();
                  }
               }
               else
               {
                  pCurReg = 0;
               }
            }

            RWS_RETURN(pCurReg);
         }
      }

      /**
      *
      * Adds the passed in interpolator registry entry into this group. If this group is
      * a 'default' group (typename == 0) then the added entry can ONLY be a default one
      * too (interpolator name == 0) and there can only be one of these.
      *
      * Requires that the RenderWare string functions have been initialised BEFORE being
      * called.
      *
      */

      void CSeqInterpolRegGroup::Add(CSeqInterpolRegister *pRegister)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolRegGroup::Add");
         RWS_ASSERT(pRegister, "Interpolator registry entry cannot be zero.");
         RWS_ASSERT(!(m_typeName == 0 && pRegister->InterpolName() != 0),
            "Cannot add a non-default interpolator into a default group.");

         // Link into the list...

         pRegister->LinkInOrder(m_pRootReg);

         RWS_RETURNVOID();
      }

      /**
      *
      * Removes the passed in interpolator registry entry from this group, which is MUST
      * be part of. Returns TRUE if the removal of this entry results in the group becoming
      * completely empty.
      *
      */

      RwBool CSeqInterpolRegGroup::Remove(CSeqInterpolRegister *pRegister)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolRegGroup::Remove");

         // Remove from list.

         pRegister->Unlink(m_pRootReg);

         // Is the list now empty ?

         RWS_RETURN(m_pRootReg == 0);
      }
         
      //////////////////////////////////
      //
      // CSeqInterpolRegister functions.
      //
      //

      CSeqInterpolRegGroup *CSeqInterpolRegister::sm_pRootGroup = 0;
      CSeqInterpolRegister *CSeqInterpolRegister::sm_pRootUnsorted = 0;

      /**
      *
      * Constructor - stores pointer to 'make' function and type name for which this
      *               make function applies. This can be 0 = default (used for
      *               unmatched types) - in this case the interpolator name MUST also
      *               be 0, as the default type group can only have default interpolators
      *               - since the type is unknown the interpolator CANNOT calculate
      *               intermediate values. THERE MUST BE A DEFAULT TYPE, DEFAULT
      *               INTERPOLATOR - without this non-specified types would not be able
      *               to be keyframed.
      *
      *               If the type name is valid, then the interpolator name can be either
      *               valid (adding a specific interpolator type) or zero to add a default
      *               interpolator (sets keys only). THERE MUST BE A DEFAULT INTERPOLATOR
      *               FOR EVERY TYPE - without this it would not be possible to set the
      *               first keyframe for a specified type.
      *
      *               The 'override' flag should be FALSE for ALL default code, ie
      *               supplied versions. When TRUE this allows the 'default'
      *               interpolator to be replaced with a user version. If a overridden
      *               version has already been specified then an assert will occur.
      *               Likewise an assert will occur if attempting to override a default
      *               version with another default version.
      *
      *         NOTE: Objects of this type are normally created as global objects and that
      *               the type and interpolator names MUST BE STATICALLY ALLOCATED as the
      *               strings are not copied, only the pointer to them.
      *
      */

      CSeqInterpolRegister::CSeqInterpolRegister(CSeqInterpolBase *(*pMakeFunc)(
            CSeqItem &rData), RwUInt32 (*pSizeFunc)(void), const RwChar *typeName, 
            const RwChar *interpolName, RwBool override) :
         m_pGroup(0)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolRegister::CSeqInterpolRegister");
         RWS_ASSERT(!(typeName == 0 && interpolName != 0),
            "Cannot specify interpolator name if no type name.");
         RWS_ASSERT(pMakeFunc, "make function MUST BE VALID.");
         RWS_ASSERT(pSizeFunc, "size function MUST BE VALID.");

         // Copy string pointers, 'make' function, etc.

         m_interpolName = interpolName;
         m_typeName = typeName;
         m_pMakeFunc = pMakeFunc;
         m_pSizeFunc = pSizeFunc;
         m_overridden = override;

         // Store this object at the head of the 'unsorted' list. This will be
         // sorted into group / alphabetical order when the 'Sort' function is called.

         m_pNext = sm_pRootUnsorted;
         m_pPrev = 0;

         if (sm_pRootUnsorted)
         {
            sm_pRootUnsorted->m_pPrev = this;
         }

         sm_pRootUnsorted = this;

         RWS_RETURNVOID();
      }

      /**
      *
      * Destructor - remove this registered type to interpolator mapping & release memory.
      *
      */

      CSeqInterpolRegister::~CSeqInterpolRegister(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolRegister::~CSeqInterpolRegister");

         // Unlink from group / unsorted list.

         if (m_pGroup)
         {
            // Unlink from group.

            if (m_pGroup->Remove(this))
            {
               // Group is now empty, so delete it.

               m_pGroup->Unlink(sm_pRootGroup);
               delete m_pGroup;
            }
         }
         else
         {
            // Unlink from unsorted list.

            Unlink(sm_pRootUnsorted);
         }

         RWS_RETURNVOID();
      }

      /**
      *
      * Attempts to make an instance of the interpolator for this registry entry.
      * Returns a pointer to it or zero on failure. In debug mode the latter will
      * result in an assert.
      *
      */

      CSeqInterpolBase *CSeqInterpolRegister::MakeInterpol(CSeqItem &rData)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolRegister::MakeInterpol");

         CSeqInterpolBase *pInterpol = (*m_pMakeFunc)(rData);
         RWS_ASSERT(pInterpol, "Failed to create interpolator instance.");

         RWS_RETURN(pInterpol);
      }

      /**
      *
      * Calls the interpolator's size function and thereby returns the maximum size
      * of memory block that the interpolator needs.
      *
      */

      RwUInt32 CSeqInterpolRegister::MaxInterpolMemSize(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolRegister::MaxInterpolMemSize");
         RWS_RETURN((*m_pSizeFunc)());
      }

      /**
      *
      * Returns the name of the interpolator, this pointer MAY be ZERO if the
      * interpolator is a default one.
      *
      */

      const RwChar *CSeqInterpolRegister::InterpolName(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolRegister::InterpolName");
         RWS_RETURN(m_interpolName);
      }

      /**
      *
      * Returns a pointer to the next interpolator register entry in the list
      * or zero if there isn't one.
      *
      */

      CSeqInterpolRegister *CSeqInterpolRegister::Next(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolRegister::Next");
         RWS_RETURN(m_pNext);
      }

      /**
      *
      * Returns a pointer to the previous interpolator register entry in the list
      * or zero if there isn't one.
      *
      */

      CSeqInterpolRegister *CSeqInterpolRegister::Prev(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolRegister::Prev");
         RWS_RETURN(m_pPrev);
      }

      /**
      *
      * Links this interpolator registry entry into the passed in list.
      *
      * NOTE: that 'default' entries will ALWAYS be placed at the top of the list.
      *
      * Also, items will be inserted in alphabetical (via rwstrcmp) order on interpolator
      * name. Therefore the RenderWare string functions MUST HAVE BEEN INITIALISED before
      * calling this function.
      *
      */

      void CSeqInterpolRegister::LinkInOrder(CSeqInterpolRegister *&rpRoot)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolRegister::LinkInOrder");

         // Default function ?

         if (m_interpolName == 0)
         {
            // Insert at root of list.

            if (rpRoot && rpRoot->m_interpolName == 0)
            {
               RWS_ASSERTFAIL("Already have default, cannot add second."
                  " Original default must be removed first.");
            }
            else
            {
               m_pNext = rpRoot;
               m_pPrev = 0;

               if (rpRoot)
               {
                  rpRoot->m_pPrev = this;
               }

               rpRoot = this;
            }
         }
         else
         {
            // Normal handler - insert in alphabetical order. Skip over default, if one.

            if (rpRoot)
            {
               CSeqInterpolRegister *pCur = rpRoot;
               int cmpRes;

               // Is there a default to step over ?

               if (pCur && pCur->m_interpolName == 0)
               {
                  if (pCur->m_pNext)
                  {
                     // There is an item after it (non-default) to check, so move past default.

                     pCur = pCur->m_pNext;
                  }
                  else
                  {
                     // There is no next, so just link after it.

                     pCur->m_pNext = this;
                     m_pPrev = pCur;
                     m_pNext = 0;

                     RWS_RETURNVOID();
                  }
               }

               // Scan list for insertion point, need cmpRes calculated EACH and EVERY
               // time, so must not be conditionally-short-circuited, ie as a result of
               // and && or ||.

               while ((cmpRes = rwstrcmp(m_interpolName, pCur->m_interpolName)) > 0
                  && pCur->m_pNext)
               {
                  pCur = pCur->m_pNext;
               }

               // Insert before or after ? Or is it the same (error) ?

               RWS_ASSERT(cmpRes, "Attempt to register second interpolator with the same name.");

               if (cmpRes < 0)
               {
                  // Before...

                  if (pCur->m_pPrev)
                  {
                     pCur->m_pPrev->m_pNext = this;
                  }
                  else
                  {
                     // It's the root.

                     rpRoot = this;
                  }

                  m_pPrev = pCur->m_pPrev;
                  m_pNext = pCur;
                  pCur->m_pPrev = this;
               }
               else
               {
                  // After...

                  if (pCur->m_pNext)
                  {
                     pCur->m_pNext->m_pPrev = this;
                  }
                  m_pNext = pCur->m_pNext;
                  m_pPrev = pCur;
                  pCur->m_pNext = this;
               }
            }
            else
            {
               // First item.

               rpRoot = this;
               m_pPrev = 0;
               m_pNext = 0;
            }
         }

         RWS_RETURNVOID();
      }

      /**
      *
      * Unlinks the entry from the passed in list (which is MUST be in) and updates
      * the root pointer if necessary.
      *
      */

      void CSeqInterpolRegister::Unlink(CSeqInterpolRegister *&rpRoot)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolRegister::Unlink");

         if (rpRoot == this)
         {
            rpRoot = rpRoot->m_pNext;
         }

         if (m_pPrev)
         {
            m_pPrev->m_pNext = m_pNext;
         }

         if (m_pNext)
         {
            m_pNext->m_pPrev = m_pPrev;
         }

         m_pPrev = m_pNext = 0;

         RWS_RETURNVOID();
      }

      /**
      *
      * Looks through the list of type groups and returns a pointer to the one matching
      * the passed in type name string. This can be a zero pointer - in this case the
      * 'default' type group will be returned (if one). Returns zero if one cannot be
      * found OR the default (handle all keyframes but no interpolation) object, if 
      * there is one.
      *
      * If the 'specifiedOnly' flag is TRUE then if the typeName is specified (not == 0)
      * and the specified type group cannot be found, the default type group will NOT
      * be returned - zero will instead.
      *
      * NOTE: Uses the RenderWare string functions and these MUST HAVE BEEN initialised
      *       before calling this function. Also, if the 'Sort' function has not been
      *       called since new instances of the class were created, it will be called
      *       by this function - which could result in a delay. It is therefore advisable
      *       to call the 'Sort' function yourself after all instances have been created.
      *
      */

      CSeqInterpolRegGroup *CSeqInterpolRegister::FindGroup(const RwChar *typeName,
         RwBool specifiedOnly)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolRegister::FindGroup");

         CSeqInterpolRegGroup *pCurGroup;
         int cmpRes;

         // Any unsorted items?

         if (sm_pRootUnsorted)
         {
            // Yep, these MUST be sorted before the find group operation can work
            // correctly.

            Sort();
         }

         pCurGroup = sm_pRootGroup;

         // Is caller asking for default ?

         if (typeName == 0)
         {
            // Is there a default ?

            if (sm_pRootGroup && sm_pRootGroup->TypeName() == 0)
            {
               RWS_RETURN(sm_pRootGroup);
            }
            else
            {
               RWS_RETURN(0);
            }
         }
         else
         {
            // Skip default group, if one, for now...

            if (pCurGroup && pCurGroup->TypeName() == 0)
            {
               pCurGroup = pCurGroup->Next();
            }

            // Scan the groups looking for a matching one...

            while (pCurGroup && (cmpRes = rwstrcmp(typeName, pCurGroup->TypeName())) > 0)
            {
               pCurGroup = pCurGroup->Next();
            }

            // Was one found ?

            if (!pCurGroup || (pCurGroup && cmpRes != 0))
            {
               // No, so is there a default that can be used ?

               if (!specifiedOnly && sm_pRootGroup && sm_pRootGroup->TypeName() == 0)
               {
                  // Yep, so use this.

                  pCurGroup = sm_pRootGroup;

                  // Do not inform use of inability to find specific group as this is not
                  // really an error, the default system will keyframe (only) any kind of
                  // data, so it is only a error if they request a particular interpolator
                  // and it does not exist (see CSeqInterpolRegGroup::FindInterpol).
               }
               else
               {
                  pCurGroup = 0;
               }
            }

            RWS_RETURN(pCurGroup);
         }
      }

      /**
      *
      * Called to sort the instances of this class into groups / alphabetical order. This must be
      * completed before any group finding can occur. If the function is not explicitly called it
      * will be called by the 'FindGroup' function before it searches if (and ONLY IF) there are
      * any unsorted items.
      *
      * NOTE: This function REQUIRES that the RenderWare string functions have been initialised
      *       BEFORE it is called.
      *
      */

      void CSeqInterpolRegister::Sort(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqInterpolRegister::Sort");

         CSeqInterpolRegister *pNewReg = sm_pRootUnsorted,
            *pNextReg;

         // All items in about to be sorted, so clear the pointer. Have copy of it
         // for processing and will need to recursively call the 'FindGroup' function - it
         // the pointer is NOT cleared here, then infinite recursion would occur.

         sm_pRootUnsorted = 0;
         
         // Process each item in the unsorted list (via local pointer).

         while (pNewReg)
         {
            // Store 'next' new registry object, as linking the present one into a group
            // will destroy the 'next' pointer.

            pNextReg = pNewReg->m_pNext;

            // Find existing group...

            pNewReg->m_pGroup = FindGroup(pNewReg->m_typeName, TRUE);

            // Was it found ?

            if (!pNewReg->m_pGroup)
            {
               // No, so add a new one.
               //
               // NOTE: CANNOT use RWS_NEW here. Although this function will be called
               //       after main() has been (ie is not called from 'global' space),
               //       the memory here will not be released until after main() has
               //       exited - ie from 'global' space calls to the destructors of the
               //       global registion instances of the class.

               pNewReg->m_pGroup = new CSeqInterpolRegGroup(pNewReg->m_typeName);
               RWS_ASSERT(pNewReg->m_pGroup, "Failed to create needed new group.");

               // Link into list...

               pNewReg->m_pGroup->LinkInOrder(sm_pRootGroup);
            }

            // Add this object to the group - is there one already here?

            CSeqInterpolRegister *pCurReg = pNewReg->m_pGroup->FindInterpol(
               pNewReg->m_interpolName, TRUE);

            // If one already exists check validity of 'overriding' it.

            RWS_ASSERT(!pCurReg || !pCurReg->m_overridden,
               "cannot override already overridden interpolator. Type [" <<
               (pNewReg->m_typeName ? pNewReg->m_typeName : "<< Default >>") <<
               "] interpol [" << (pNewReg->m_interpolName ? pNewReg->m_interpolName :
               "<< Default >>") << "].");

            RWS_ASSERT(!pCurReg || pNewReg->m_overridden,
               "cannot have two interpolators for type when not overriding. Type [" <<
               (pNewReg->m_typeName ? pNewReg->m_typeName : "<< Default >>") << 
               "] interpol [" << (pNewReg->m_interpolName ? pNewReg->m_interpolName : 
               "<< Default >>") << "].");

            // Remove old one...

            if (pCurReg)
            {
               pNewReg->m_pGroup->Remove(pCurReg);
            }

            // Add this one...

            pNewReg->m_pGroup->Add(pNewReg);

            // Go to next in the unsorted list.

            pNewReg = pNextReg;
         }

         RWS_RETURNVOID();
      }

      ///////////////////////
      //
      // CSeqItem functions.
      //

      /**
      *
      * Constructor - sets up a sequence track for an attribute. Pass in the initial keyframe
      *               of the track's sequence. Causes the interpolator's initialize function
      *               to be called.
      *
      */

      CSeqItem::CSeqItem(RWSGUID &rGuid, const RwChar *pClassName,
         RwUInt32 commandID, const RwChar *typeName, RwUInt32 addTime,
         CSeqStack &rSeqStack, RwUInt32 flags) :
            m_startTime(0),
            m_lastUpdateTime(0),
            m_isActive(FALSE),
            m_pInterpolReg(0),
            m_pInterpol(0),
            m_interpolNextKey(FALSE),
            m_interpolMemSize(0),
            m_flags(flags),
            m_pInterpolMem(0),
            m_pBuffSlot(0)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqItem::CSeqItem");
         RWS_ASSERT(pClassName, "No class name provided.");
         RWS_ASSERT(typeName, "Invalid type name.");

         // Setup the data for the keyframes and interpolator data.

         m_pLastKeyData = RWS_NEW_TYPE(rSeqStack.MemType())
            CSeqCtrlKeyData(0, rSeqStack.MemType(), m_flags);
         RWS_ASSERT(m_pLastKeyData, "Failed to allocate keyframe data object for 'last'.");

         m_pEndKeyData = RWS_NEW_TYPE(rSeqStack.MemType())
            CSeqCtrlKeyData(0, rSeqStack.MemType(), m_flags);
         RWS_ASSERT(m_pEndKeyData, "Failed to allocate keyframe data object for 'cur'.");
         
         m_pInterpolData = RWS_NEW_TYPE(rSeqStack.MemType())
            CSeqCtrlInterpolData(0, rSeqStack.MemType(), m_flags);
         RWS_ASSERT(m_pInterpolData, "Failed to allocate interpolator data object.");

         // Initialize the interpolator data - MUST do before finding an interpolator.

         m_pClassName = pClassName;
         m_commandID = commandID;
         m_guid = rGuid;

         m_pSeqStack = &rSeqStack;

         // Find an interpolator group for the data type in question. Built-in types first...

         m_pInterpolRegGroup = CSeqInterpolRegister::FindGroup(typeName, FALSE);
         RWS_ASSERT(m_pInterpolRegGroup, "Failed to find interpolator group for type [" <<
            typeName << "].");

         // Create the slot for this item within the buffer manager.

         m_pBuffSlot = rSeqStack.AddBuffSlot(*this, 0);
         RWS_ASSERT(m_pBuffSlot, "Failed to obtain a buffer slot for the sequence item.");

         // Get the 'keyframe only' interpolator.

         SetInterpol(0, TRUE);

         RWS_ASSERT(m_pInterpol, "Failed to find interpolator for type [" <<
            typeName << "].");

         RWS_RETURNVOID();
      }

      /**
      *
      * Destructor - call interpolator's close function to allow it to tidy up.
      *
      */

      CSeqItem::~CSeqItem(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqItem::~CSeqItem");

         // Release keyframe & interpolator data objects.

         delete m_pLastKeyData;
         delete m_pEndKeyData;
         delete m_pInterpolData;

         // Call the 'close' function for the interpolator.

         if (m_pInterpol)
         {
            // NOTE: all 'MakeInterpol' functions MUST use placement new, so
            //       have not really allocated memory (not in normal way) so
            //       CANNOT use 'delete' here, must directly call the destructor
            //       but NOT delete the memory (as memory will be used for next
            //       interpolator & was not allocated via 'new' & last interpolator).

            m_pInterpol->~CSeqInterpolBase();
         }

         if (m_pInterpolMem)
         {
            operator delete(m_pInterpolMem);
         }

         RWS_RETURNVOID();
      }

      /**
      *
      * Called to add a new keyframe to the sequence track without performing
      * an interpolation - this is achieved by doing the interpolation in
      * zero time. Returns TRUE if the added key is interpolated to or FALSE
      * if it is simply instantaneously 'set'.
      *
      * If the function returns 'TRUE' and the item is within a sequence stack
      * then the stack must be told to 'MoveToActive' on the item.
      *
      */

      RwBool CSeqItem::AddKey(CSeqCtrlDataIttr &rIttr, RwUInt32 nowTime)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqItem::AddKey");

         // Swap the (last = end) and get new keyframe data from iterator.

         CSeqCtrlKeyData *pTempKey = m_pLastKeyData;
         
         m_pLastKeyData = m_pEndKeyData;
         m_pEndKeyData = pTempKey;

         rIttr.GetKeyframe(*m_pEndKeyData, m_pSeqStack->MemType());
         RWS_ASSERT(m_pEndKeyData->Name(), "No keyframe data type");
         RWS_ASSERT(m_pEndKeyData->Data() || !rwstrcmp(m_pEndKeyData->Name(),
            MSG_LINK_NAME_TYPE),
            "No keyframe data to use - illegal for none message link name types.");
         RWS_ASSERT(m_pEndKeyData->DataSize() || !rwstrcmp(m_pEndKeyData->Name(),
            MSG_LINK_NAME_TYPE),
            "Keyframe data zero size - illegal for none message link name types.");

         m_lastUpdateTime = nowTime;
         
         // Doing keyframe set or interpolating to it?

         if (m_interpolNextKey)
         {
            // Interpolating...

            m_interpolNextKey = FALSE;
            m_isActive = TRUE;
            m_startTime = nowTime;

            // Reusing data ? Only occurs if there are two consecutive interpolations (with
            // no keyframe only sets in between) using the same interpolator. Allows the
            // interpolator class to potentially re-use data it has expensively calculated.

            if (m_canReuse)
            {
               m_pInterpol->PrepReuse(*this);
            }
            else
            {
               m_pInterpol->Prep(*this);
            }

            // Can potentially reuse, so set flag.

            m_canReuse = TRUE;

            // After adding, if there is slack or overshoot time, the caller should
            // then call the 'Process' function with the most up-to-date time to get
            // the object to the correct place.

            RWS_RETURN(TRUE);
         }
         else
         {
            // Just setting...

            m_pInterpol->Set(*this);
            m_canReuse = FALSE;

            RWS_RETURN(FALSE);
         }
      }

      /**
      *
      * Called to add a new keyframe interpolation over the specified time. This sets up
      * the interpolator to be used with the next keyframe received. Calls interpolator
      * functions to setup the require function calls for the interpolator type.
      *
      * Interpolator CANNOT HAVE ZERO TIME (unless it is a design build 'dynamic'
      * interpolator - in which case the delta value MUST be between 0.0 and 1.0).
      *
      */

      void CSeqItem::AddInterpol(CSeqCtrlDataIttr &rIttr)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqItem::AddInterpol");

         // Get new interpolator data from the iterator.

         rIttr.GetInterpol(*m_pInterpolData, m_pSeqStack->MemType());
         RWS_ASSERT(m_pInterpolData->LengthTime(),
            "Zero time interpolator (includes dynamic interpolators).");
         RWS_ASSERT(m_pInterpolData->Name(), "Invalid interpolator name.");

#ifdef RWS_DESIGN

         RWS_ASSERT(m_pInterpolData->GetDelta() >= 0.0f && m_pInterpolData->GetDelta() <= 1.0f,
            "Invalid delta value in dynamic interpolator.");
#endif

         // Check to see if new interpolator is the same as the currently selected one.

         if (SetInterpol(m_pInterpolData->Name(), FALSE))
         {
            // Changed interpolator, so the 'reuse' of data from the old one is not valid.

            m_canReuse = FALSE;
         }

         // Setup data ready for when 'end keyframe' is added...

         m_interpolNextKey = TRUE;
         
         RWS_RETURNVOID();
      }

      /**
      *
      * Called to set or check for change in the interpolator name. Pass in the interpolator name
      * or set pointer to zero to use a default. Code will always use a default if cannot find exact
      * match. If the 'forceSet' flag is TRUE the interpolator will always be changed, if it's FALSE
      * then the interpolator will only change if it is not the same name as the one passed in. If
      * the interpolator changes, the old one (if one) will be released.
      *
      * Function returns TRUE if the interpolator changes and FALSE if not.
      *
      */

      RwBool CSeqItem::SetInterpol(const RwChar *interpolName, RwBool forceSet)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqItem::SetInterpol");
         RWS_ASSERT(m_pInterpolRegGroup, "Interpolator registry group pointer MUST be setup first.");

         CSeqInterpolRegister *pNewReg = m_pInterpolRegGroup->FindInterpol(interpolName, FALSE);
         RWS_ASSERT(pNewReg, "Could not find interpolator to function registry entry.");

         // Has interpolator changed or is it being forced.

         if (pNewReg != m_pInterpolReg || forceSet)
         {            
            // Yep, delete old one and create new one...

            if (m_pInterpol)
            {
               // NOTE: all 'MakeInterpol' functions MUST use placement new, so
               //       have not really allocated memory (not in normal way) so
               //       CANNOT use 'delete' here, must directly call the destructor
               //       but NOT delete the memory (as memory will be used for next
               //       interpolator & was not allocated via 'new' & last interpolator).

               m_pInterpol->~CSeqInterpolBase();
            }

            // Is the memory block big enough?

            m_pInterpolReg = pNewReg;

            RwUInt32 memNeeded = m_pInterpolReg->MaxInterpolMemSize();

            if (m_interpolMemSize < memNeeded)
            {
               // Need to resize buffer...

               ResizeInterpolMem(memNeeded);
            }

            // Now know memory is large enough, can create interpolator in it & check on setup.

            m_pInterpol = m_pInterpolReg->MakeInterpol(*this);
            RWS_ASSERT(m_pInterpol, "Failed to create necessary interpolator.");

            RWS_RETURN(TRUE);
         }
         else
         {
            RWS_RETURN(FALSE);
         }
      }

      /**
      *
      * Resizes the buffer used for the interpolators. The code DOES NOT PRESERVE the memory's
      * contents and so MUST be called when the memory is not in use by an interpolator object.
      *
      */

      void CSeqItem::ResizeInterpolMem(RwUInt32 memNeeded)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqItem::ResizeInterpolMem");

         // Deleting old and emit warning about resizing operation (if previous buffer).

         if (m_pInterpolMem)
         {
            operator delete(m_pInterpolMem);

            if ((m_flags & SEQ_FLAG_BUFFER_WARNINGS) != 0)
            {
               RWS_DEBUGSTREAM_WARN("Resizing sequence item's interpolator memory for guid [" <<
                  m_guid << " class [" << m_pClassName <<
                  "] command [" << m_commandID << "]. This is likely to cause memory fragmentation." <<
                  std::endl);
            }
         }

         // Allocate new memory block.

         m_pInterpolMem = RWS_OP_NEW_TYPE(memNeeded, m_pSeqStack->MemType());
         RWS_ASSERT(m_pInterpolMem, "Failed to allocate interpolator memory.");

         m_interpolMemSize = memNeeded;

         RWS_RETURNVOID();
      }

      /**
      *
      * Returns TRUE if the item has an active sequence under way.
      *
      */

      RwBool CSeqItem::IsActive(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqItem::IsActive");
         RWS_RETURN(m_isActive);
      }

      /**
      *
      * Processes the sequence to the passed in 'nowTime' will clear the
      * active flag if the sequence is then complete.
      *
      * In the case of dynamic interpolators, the value is set and then
      * interpolator IMMEDIATELY becomes inactive.
      *
      */

      void CSeqItem::Process(RwUInt32 nowTime)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqItem::Process");
         RWS_ASSERT(m_pInterpol, "No interpolator object setup.");
         RWS_ASSERT(m_pInterpolData->LengthTime(),
            "Zero time interpolator in non-design build are illegal. "
            "This includes dynamic interpolators.");

#ifdef RWS_DESIGN

         if (m_pInterpolData->IsDynamic())
         {
            // Dynamic interpolator - set to specified interpolation value or
            // keyframe and make inactive immediately. The dynamic interpolator
            // effectively 'sets' to a given delta value.

            m_pInterpol->Step(*this, m_pInterpolData->GetDelta());
            m_isActive = FALSE;
         }
         else
         {
            // Standard (non-dynamic) interpolator...
#endif
            // Check if finished and step to next.

            if (nowTime >= (m_startTime + m_pInterpolData->LengthTime()))
            {
               // SET to end key value & deactivate. Need to ensure end value is
               // reached in all cases.

               m_pInterpol->Set(*this);
               m_isActive = FALSE;
            }
            else
            {
               // Process...

               m_pInterpol->Step(*this, nowTime);
            }

#ifdef RWS_DESIGN

         }
#endif
         m_lastUpdateTime = nowTime;

         RWS_RETURNVOID();
      }

      /**
      *
      * Returns the start time (in milliseconds) of the sequence.
      *
      */

      RwUInt32 CSeqItem::StartTime(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqItem::StartTime");
         RWS_RETURN(m_startTime);
      }

      /**
      *
      * Returns the length of the sequence (in milliseconds).
      *
      */

      RwUInt32 CSeqItem::LengthTime(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqItem::LengthTime");
         RWS_RETURN(m_pInterpolData->LengthTime());
      }

      /**
      *
      * Returns a pointer to the last keyframe data. Should never
      * be zero.
      *
      */

      CSeqCtrlKeyData *CSeqItem::LastKey(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqItem::LastKey");
         RWS_ASSERT(m_pLastKeyData->Name(), "Last keyframe type name is invalid.");
         RWS_ASSERT(m_pLastKeyData->DataSize() || !rwstrcmp(m_pEndKeyData->Name(),
            MSG_LINK_NAME_TYPE),
            "Last keyframe datasize is invalid - can only be zero for message link names.");
         RWS_ASSERT(m_pLastKeyData->Data() || !rwstrcmp(m_pEndKeyData->Name(),
            MSG_LINK_NAME_TYPE),
            "Last keyframe data pointer is invalid - can only be zero for message link names.");
         RWS_RETURN(m_pLastKeyData);
      }

      /**
      *
      * Returns a pointer to the end keyframe data or 0 if there isn't
      * any.
      *
      */

      CSeqCtrlKeyData *CSeqItem::EndKey(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqItem::EndKey");
         RWS_ASSERT(m_pEndKeyData->Name(), "end keyframe type name is invalid.");
         RWS_ASSERT(m_pEndKeyData->DataSize() || !rwstrcmp(m_pEndKeyData->Name(),
            MSG_LINK_NAME_TYPE),
            "End keyframe datasize is invalid - can only be zero for message link names.");
         RWS_ASSERT(m_pEndKeyData->Data() || !rwstrcmp(m_pEndKeyData->Name(),
            MSG_LINK_NAME_TYPE),
            "End keyframe data pointer is invalid - can only be zero for message link names.");
         RWS_RETURN(m_pEndKeyData);
      }

      /**
      *
      * Returns the 4 32bit values for the guid via the passed in 32bit variables.
      *
      */

      void CSeqItem::Guid(RwUInt32 &rGuidData0, RwUInt32 &rGuidData1, RwUInt32 &rGuidData2,
         RwUInt32 &rGuidData3)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqItem::Guid");

         rGuidData0 = m_guid.m_Data[0];
         rGuidData1 = m_guid.m_Data[1];
         rGuidData2 = m_guid.m_Data[2];
         rGuidData3 = m_guid.m_Data[3];

         RWS_RETURNVOID();
      }

      /**
      *
      * Returns constant reference to GUID being used.
      *
      */

      const RWSGUID &CSeqItem::GuidRef(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqItem::GuidRef");
         RWS_RETURN(m_guid);
      }

      /**
      *
      * Returns TRUE if the passed in GUID and the internally held one match.
      *
      */

      RwBool CSeqItem::IsSameGuid(RWSGUID &rGuid)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqItem::IsSameGuid");
         RWS_RETURN(rGuid == m_guid);
      }

      /**
      *
      * Returns a pointer to the class name being controlled by the sequence. Should
      * never be zero.
      *
      */

      const RwChar *CSeqItem::ClassName(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqItem::ClassName");
         RWS_ASSERT(m_pClassName, "Invalid class name.");
         RWS_RETURN(m_pClassName);
      }

      /**
      *
      * Returns the command ID (attribute number) being controlled by the sequence.
      *
      */

      RwUInt32 CSeqItem::CommandID(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqItem::CommandID");
         RWS_RETURN(m_commandID);
      }

      /**
      *
      * Returns the sequence stack this sequence item was created via.
      *
      */

      CSeqStack *CSeqItem::SeqStack(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqItem::SeqStack");
         RWS_RETURN(m_pSeqStack);
      }

      /**
      *
      * Returns a pointer to the buffer manager's slot for this item or zero if
      * one has not been stored.
      *
      */

      CSeqAttrHandSlot *CSeqItem::BuffSlot(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqItem::BuffSlot");
         RWS_RETURN(m_pBuffSlot);
      }

      /**
      *
      * Sets the stored buffer manager's slot to the passed in reference.
      *
      */

      void CSeqItem::SetBuffSlot(CSeqAttrHandSlot &rBuffSlot)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqItem::SetBuffSlot");

         m_pBuffSlot = &rBuffSlot;

         RWS_RETURNVOID();
      }

      /**
      *
      * Marks the buffer used by this sequence item as being disconnected from it's
      * attribute handler. This needs to be called if the entity in question is deleted.
      *
      */

      void CSeqItem::DisconnectFromHandler(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqItem::DisconnectFromHandler");

         if (m_pBuffSlot)
         {
            m_pBuffSlot->DisconnectFromHandler();
         }

         RWS_RETURNVOID();
      }

      /**
      *
      * Returns the time at which the last update to the sequence item was make. This is set
      * by the 'AddKey' and 'Process' functions.
      *
      */

      RwUInt32 CSeqItem::LastUpdateTime(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqItem::TxManToken");
         RWS_RETURN(m_lastUpdateTime);
      }

      /**
      *
      * Returns a pointer to the memory the sequencer item is using for interpolators or
      * zero if none is yet setup.
      *
      */

      void *CSeqItem::GetInterpolMem(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqItem::GetInterpolMem");
         RWS_RETURN(m_pInterpolMem);
      }

      /**
      *
      * Returns the size of the memory the sequencer item is using for interpolators. This may
      * be zero (if not initialised yet).
      *
      */

      RwUInt32 CSeqItem::GetInterpolMemSize(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqItem::GetInterpolMemSize");
         RWS_RETURN(m_interpolMemSize);
      }

      ////////////////////////
      //
      // SeqStack functions...
      //
      //

      /**
      *
      * Constructor - just initialises.
      *
      */

      CSeqStack::CSeqStack(RwUInt32 memType, RwUInt32 flags) :
         m_memType(memType),
         m_flags(flags),
         m_pInactiveRoot(0),
         m_pActiveRoot(0)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqStack::CSeqStack");

         // Allocate the buffer manager...

         m_pBuffManager = RWS_NEW_TYPE(memType) CSeqAttrHandManager(*this, flags);
         RWS_ASSERT(m_pBuffManager, "Failed to allocate buffer manager.");

         RWS_RETURNVOID();
      }

      /**
      *
      * Destructor - free up list of remaining sequence nodes.
      *
      */

      CSeqStack::~CSeqStack(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqStack::~CSeqStack");

         // Free all resources used by the system.

         FreeItems();

         // Free up sub-classes now no longer needed.

         delete m_pBuffManager;

         RWS_RETURNVOID();
      }

      /**
      *
      * Empties (completely) the passed in list and deletes all the links
      * within it.
      *
      */

      void CSeqStack::EmptyList(CSeqNode *pRoot)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqStack::EmptyList");

         CSeqNode *pCur = pRoot;

         // Since deleting entire list, don't need to unlink...

         while (pCur)
         {
            CSeqNode *pDel = pCur;
            
            pCur = pCur->m_ptrs.Next();
            delete pDel;
         }

         RWS_RETURNVOID();
      }

      /**
      *
      * Adds new sequence track to the stack - get's added to the inactive list.
      *
      */

      CSeqNode *CSeqStack::AddItem(RWSGUID &rGuid, const RwChar *pClassName, RwUInt32 commandID,
         const RwChar *typeName, RwUInt32 addTime)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqStack::AddItem");

         CSeqNode *pNode;

         // Allocate new item.

         pNode = RWS_NEW_TYPE(m_memType) CSeqNode(rGuid, pClassName, commandID,
            typeName, addTime, *this, m_flags);
         RWS_ASSERT(pNode, "Failed to allocate memory for node.");

         // Insert into the list.

         pNode->m_ptrs.Link(m_pInactiveRoot, *pNode);

         RWS_RETURN(pNode);
      }

      /**
      *
      * Finds an existing sequence track and returns a pointer to it or
      * zero if it cannot be found.
      *
      */

      CSeqNode *CSeqStack::FindItem(RWSGUID &rGuid, const RwChar *pClassName,
         RwUInt32 commandID)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqStack::FindItem");

         CSeqNode *pFound;

         // Attempt to find in the inactive list first...

         pFound = FindItemInList(rGuid, pClassName, commandID, m_pInactiveRoot);

         if (!pFound)
         {
            pFound = FindItemInList(rGuid, pClassName, commandID, m_pActiveRoot);
         }

         RWS_RETURN(pFound);
      }

      /**
      *
      * Finds an item in the specified link-list. Returns pointer to the item within the link's
      * node or 0 if a match cannot be found.
      *
      */

      CSeqNode *CSeqStack::FindItemInList(RWSGUID &rGuid, const RwChar *pClassName,
         RwUInt32 commandID, CSeqNode *pRoot)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqStack::FindItemInList");

         CSeqNode *pCur = pRoot;

         while (pCur)
         {
            if (pCur->IsSameGuid(rGuid) && !rwstrcmp(pClassName, pCur->ClassName()) &&
               commandID == pCur->CommandID())
            {
               break;
            }

            pCur = pCur->m_ptrs.Next();
         }

         RWS_RETURN(pCur);
      }

      /**
      *
      * Finds the first item which is an attribute of the passed in entity GUID.
      * Sets up data in the passed in token.
      *
      */

      void CSeqStack::FindFirstItem(CSeqStackFindToken &rToken, RWSGUID &rGuid)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqStack::FindFirstItem");

         // Setup data...

         rToken.m_guid = rGuid;

         if (m_pActiveRoot)
         {
            // Start in active root.

            rToken.m_pSeqNode = m_pActiveRoot;
            rToken.m_fromActiveList = TRUE;

            // Find first match...

            FindNextItemMatch(rToken, FALSE);
         }
         else if (m_pInactiveRoot)
         {
            // Start in in-active root.

            rToken.m_pSeqNode = m_pInactiveRoot;
            rToken.m_fromActiveList = FALSE;

            // Find first match...

            FindNextItemMatch(rToken, FALSE);
         }
         else
         {
            // Nothing to search for, both lists empty.

            rToken.m_fromActiveList = FALSE;
            rToken.m_pSeqNode = 0;
         }

         RWS_RETURNVOID();
      }

      /**
      *
      * Finds the next item with the entity GUID specified in the 'FindFirstItem'
      * call. This call MUST BE MAKE for the token to be validated. Sets the data
      * in the passed in token (uses this to keep track of search).
      *
      */

      void CSeqStack::FindNextItem(CSeqStackFindToken &rToken)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqStack::FindNextItem");

         // Find next match...

         FindNextItemMatch(rToken, TRUE);

         RWS_RETURNVOID();
      }

      /**
      *
      * Finds next item in the list. The moveToNext flag is set to say if it should
      * move to the next item before testing for match or just use the current item
      * immediately (moveToNext == FALSE).
      *
      */

      void CSeqStack::FindNextItemMatch(CSeqStackFindToken &rToken, RwBool moveToNext)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqStack::FindNextItemMatch");

         // Check current value for match ?

         if (!moveToNext)
         {
            if (rToken.m_pSeqNode && rToken.m_pSeqNode->m_guid == rToken.m_guid)
            {
               // Matches, so return this one...

               RWS_RETURNVOID();
            }
         }

         do
         {
            // Move to next...

            if (rToken.m_pSeqNode)
            {
               rToken.m_pSeqNode = rToken.m_pSeqNode->m_ptrs.Next();
            }
            else
            {
               // Move to inactive list IF in active list...

               if (rToken.m_fromActiveList)
               {
                  rToken.m_fromActiveList = FALSE;
                  rToken.m_pSeqNode = m_pInactiveRoot;
               }
            }
         }
         while(rToken.m_pSeqNode && rToken.m_pSeqNode->m_guid != rToken.m_guid);
         	
         RWS_RETURNVOID();
      }

      /**
      *
      * Processes the sequences in the stack. Only active ones are processed.
      * Also updates the 'active' status of any sequences which may end as a
      * result of the processing.
      *
      * NOTE: Calling this function does NOT send the data onto the handling
      *       code. Call Flush to do this.
      *
      */

      void CSeqStack::ProcessItems(RwUInt32 nowTime)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqStack::ProcessItems");

         CSeqNode *pCur = m_pActiveRoot,
            *pNext;

         // Process all the sequence items...

         while (pCur)
         {
            // Get next pointer, needed as move list for current and
            // otherwise the list being processed will be flipped.

            pNext = pCur->m_ptrs.Next();

            // If active process it & needs updating...

            if (pCur->IsActive() && nowTime > pCur->LastUpdateTime())
            {
               pCur->Process(nowTime);
            }

            // May have become (or originally been) inactive, so
            // check and remove from list.
            
            if (!pCur->IsActive())
            {
               // No longer active, move to inactive list.

               pCur->m_ptrs.Unlink(m_pActiveRoot, *pCur);
               pCur->m_ptrs.Link(m_pInactiveRoot, *pCur);
            }

            pCur = pNext;
         }

         RWS_RETURNVOID();
      }

      /**
      *
      * Moves node from the inactive list to the active list. Also copes if
      * item is moved from active to active. The passed in item MUST be one
      * that was returned from either 'AddItem' or 'FindItem'.
      *
      */

      void CSeqStack::MoveToActive(CSeqNode *pNode)
      {
         // The node could be in either the active or inactive lists, just
         // check it's not at the root of the active list, as code will only
         // cope naturally if it's not. If it is already at the head of the
         // active list, then it's in the right list already, so leave it there.

         if (pNode != m_pActiveRoot)
         {
            // Can safely move as code will cope. Code can only check 1 root
            // collision, but the above check will provide the 2nd root
            // collision check.
            //
            // That is, if actually in in-active list, then unlink + link into
            // active is fine. If in active, it can't be the root, so unlink
            // pretending it's in in-active (but not at root) will be fine and
            // re-link into active will be fine. If actually in root of active,
            // the above check will stop this code from being run - which it
            // needs to, because otherwise the root of the active list would
            // become corrupted.

            pNode->m_ptrs.Unlink(m_pInactiveRoot, *pNode);
            pNode->m_ptrs.Link(m_pActiveRoot, *pNode);
         }
      }

      /**
      *
      * Called after the sequence has completed to free up objects used during the
      * processing of the sequence.
      *
      */

      void CSeqStack::FreeItems(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqStack::FreeItems");

         // Empty lists of interpolator objects.

         EmptyList(m_pInactiveRoot);
         EmptyList(m_pActiveRoot);

         m_pInactiveRoot = m_pActiveRoot = 0;

         // Delete the buffer manager's internal data...

         m_pBuffManager->FreeItems();

         RWS_RETURNVOID();
      }
         
      /**
      *
      * Causes the transmission managers to be called to send the data built
      * up to the handling code at the entities, etc.
      *
      */

      void CSeqStack::Flush(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqStack::Flush");
         RWS_ASSERT(m_pBuffManager, "No buffer manager allocated.");

         // Call the buffer manager to send the data on...

         m_pBuffManager->Send();

         RWS_RETURNVOID();
      }

      /**
      *
      * Returns a pointer to a new buffer slot item allocated from the buffer manager
      * held within the sequence stack or 0 on failure.
      *
      */

      CSeqAttrHandSlot *CSeqStack::AddBuffSlot(CSeqItem &rSeqItem, RwUInt32 dataSize)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqStack::AddBuffSlot");
         RWS_ASSERT(m_pBuffManager, "No buffer manager allocated.");
         RWS_RETURN(m_pBuffManager->CreateSlot(rSeqItem, dataSize));
      }

      /**
      *
      * Returns the memory type number, to be passed into allocations, for this
      * CSeqStack instance.
      *
      */

      RwUInt32 CSeqStack::MemType(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqStack::MemType");
         RWS_RETURN(m_memType);
      }

      ////////////////////////
      //
      // CSeqNode functions...
      //
      //

      /**
      *
      * Constructor - create base-object and init.
      *
      */

      CSeqNode::CSeqNode(RWSGUID &rGuid, const RwChar *pClassName,
            RwUInt32 commandID, const RwChar *typeName, RwUInt32 addTime,
            CSeqStack &rSeqStack, RwUInt32 flags) :
         CSeqItem(rGuid, pClassName, commandID, typeName, addTime, rSeqStack, flags)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqStack::CSeqNode::CSeqNode");

         // All initialisation in initialiser list.

         RWS_RETURNVOID();
      }

      /**
      *
      * Destructor - clean up.
      *
      */

      CSeqNode::~CSeqNode(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqStack::CSeqNode::~CSeqNode");

         // Needed as base-object has virtual destructor.

         RWS_RETURNVOID();
      }

      ////////////////////////////
      //
      // CSeqNodeLink functions...
      //
      //

      /**
      *
      * Links this object at the head of the passed in link list.
      *
      */

      void CSeqNodeLink::Link(CSeqNode *&rpRoot, CSeqNode &pNode)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqNodeLink::Link");

         m_pNext = rpRoot;
         if (rpRoot)
         {
            rpRoot->m_ptrs.m_pPrev = &pNode;
         }
         rpRoot = &pNode;

         RWS_RETURNVOID();
      }

      /**
      *
      * Unlinks this object from the list it's in. Check if it's the
      * root of the passed in list and maintains the root pointer if
      * it is.
      *
      */

      void CSeqNodeLink::Unlink(CSeqNode *&rpRoot, CSeqNode &pNode)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqNodeLink::Unlink");

         if (rpRoot == &pNode)
         {
            rpRoot = rpRoot->m_ptrs.m_pNext;
         }

         if (m_pNext)
         {
            m_pNext->m_ptrs.m_pPrev = m_pPrev;
         }

         if (m_pPrev)
         {
            m_pPrev->m_ptrs.m_pNext = m_pNext;
         }

         m_pNext = m_pPrev = 0;

         RWS_RETURNVOID();
      }
   }     // namespace SEQUENCE
}        // namespace RWS
