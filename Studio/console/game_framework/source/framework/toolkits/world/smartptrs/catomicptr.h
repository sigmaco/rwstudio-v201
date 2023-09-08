/*****************************************************************************
*
* File :     CAtomicPtr.h
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
* Copyright (c) 2001 Criterion Software Ltd.
* All Rights Reserved.
*
* RenderWare is a trademark of Canon Inc.
*
*****************************************************************************/

#ifndef __CATOMICPTR_H__
#define __CATOMICPTR_H__

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

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "crwobjectptr.h"
#include "../helpers/atomichelper.h"

namespace RWS
{
   /**
   *
   *  \ingroup WorldSmartPointers
   *
   *  Smart pointer for RpAtomic. Provides support for event Visualization \see CEventVisualization
   *  and handling CSystemCommands.        
   *
   */
   
   class CAtomicPtr : public CRwObjectPtr<RpAtomic>
#ifdef RWS_EVENTVISUALIZATION
      , public CEventVisualization
#endif
   {
   public:
      /**
      * Default constructor, constructs a NULL CAtomicPtr pointer
      */
      CAtomicPtr() : CRwObjectPtr<RpAtomic>() {}

      /**
      * Constructor, constructs a CAtomicPtr pointer that points to \a p;
      *
      * \param p Pointer to an RpAtomic object.
      */
      CAtomicPtr(RpAtomic* p) : CRwObjectPtr<RpAtomic>(p) {}

      /**
      * Destructor for CAtomicPtr object, if this CAtomicPtr is still assigned to an
      * RpAtomic calls CAtomicPtr::Destroy.
      */
      ~CAtomicPtr() { if (p_) AtomicHelper::Destroy( p_ ); }
      
      /**
      *
      * Copy operator, if the pointer is already assigned to an RpAtomic, calls Destroy
      * before assigning the new RpAtomic pointer, this ensures that an object pointed to
      * by the smart pointer is never left dangling.
      *
      * \param p Pointer to an RpAtomic object.
      */
      CAtomicPtr& operator = (RpAtomic* p)
      {
         if (p_ != p)
         {
            if (p_) AtomicHelper::Destroy( p_ );
            
            p_ = p;
         }
         
         return *this;
      }

#ifdef RWS_EVENTVISUALIZATION      
      /**
      *
      * Mandatory CEventVisualization interface.
      *
      * \return A pointer to a RwV3d object, containing the world co-ordinates of the RpAtomic.
      *
      * \note Get3dPos is only available if RWS_EVENTVISUALIZATION is defined, this function is
      * only intended to be used during development by the event Visualization system.
      *
      */
      virtual RwV3d *GetWorldPos(void) 
      {
         if (p_)
         {
            RwFrame *pFrame = RpAtomicGetFrame(p_);

            if (pFrame)
            {
               RwMatrix *pMatrix = RwFrameGetLTM(pFrame);

               return(RwMatrixGetPos(pMatrix));
            }
         }
         return(0);
      }
#endif
      
      /**
      *
      * Handles system commands, CMD_LoadMatrix, CMD_SetSolidFlag, CMD_SetInvisibleFlag, CMD_SetCollisionFlag
      *
      * \param rAttr A reference to a CAttributePacket.
      */
      void HandleSystemCommands(const CAttributePacket& rAttr)
      {
         if (*this)
         {
            AtomicHelper::HandleSystemCommands(*p_, rAttr);
         }
      }
      
   private:
      
      // Revoked ops
      CAtomicPtr(const CAtomicPtr&);
      CAtomicPtr& operator = (const CAtomicPtr&);
   };
}

extern RwBool RpAtomicDestroy( RWS::CAtomicPtr &rCAtomicPtr );

#endif
