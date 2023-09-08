/*****************************************************************************
*
* File :     CClumpPtr.h
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

#ifndef __CCLUMPPTR_H__
#define __CCLUMPPTR_H__

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
#include "framework/core/eventhandler/ceventhandler.h"
#include "crwobjectptr.h"
#include "../helpers/clumphelper.h"

namespace RWS
{
   /**
   *
   *  \ingroup WorldSmartPointers
   *
   *  Smart pointer for RpClump. Also provides support for event Visualization \see CEventVisualization
   *  and handling CSystemCommands.
   *
   */
   
   class CClumpPtr : public CRwObjectPtr<RpClump>
#ifdef RWS_EVENTVISUALIZATION
      , public CEventVisualization
#endif
   {
   public:

      /**
      * Default constructor, constructs a NULL CClumpPtr pointer
      */
      CClumpPtr() : CRwObjectPtr<RpClump>() {}

      /**
      * Constructor, constructs a CClumpPtr pointer that points to \a p;
      *
      * \param p Pointer to an RpClump object.
      */
      CClumpPtr(RpClump* p) : CRwObjectPtr<RpClump>(p) {}

      /**
      * Destructor for CClumpPtr object, if this CClumpPtr is still assigned to an RpClump calls CClumpPtr::Destroy.
      */
      ~CClumpPtr() { if (p_) ClumpHelper::Destroy(p_); }
      
      /**
      *
      * Copy operator, if the pointer is already assigned to an RpClump, calls ClumpHelper::Destroy
      * before assigning the new RpClump pointer, this ensures that an object pointed to
      * by the smart pointer is never left dangling.
      *
      * \param p Pointer to an RpClump object.
      */
      CClumpPtr& operator = (RpClump* p)
      {
         if (p_ != p)
         {
            if (p_) {
               ClumpHelper::Destroy(p_);
            }
            p_ = p;
         }
         
         return *this;
      }
      
      /**
      *
      * Mandatory CEventVisualization interface
      *
      * \param v Reference to a RwV3d object, filled in with the position of the frame of the RpClump.
      * \note Get3dPos is only available if RWS_EVENTVISUALIZATION is defined, this function is
      * only intended to be used during development by the event Visualization system.
      *
      */
#ifdef RWS_EVENTVISUALIZATION
      virtual RwV3d *GetWorldPos(void) 
      {
         if (p_)
         {
            RwFrame *pFrame = RpClumpGetFrame(p_);

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
            ClumpHelper::HandleSystemCommands(*p_, rAttr);
         }
      }
      
   private:
      
      // Revoked ops
      CClumpPtr(const CClumpPtr&);
      CClumpPtr& operator = (const CClumpPtr&);
   };
}

extern RwBool RpClumpDestroy( RWS::CClumpPtr &rCClumpPtr );

#endif
