/*****************************************************************************
*
* File :     CLightPtr.h
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

#ifndef __CLightPtr_H__
#define __CLightPtr_H__

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "crwobjectptr.h"
#include "../helpers/lighthelper.h"

namespace RWS
{
   /**
   *
   *  \ingroup WorldSmartPointers
   *
   *  Smart pointer for RpLight. Also provides support for event visualization \see CEventVisualization
   *  and handling CSystemCommands.        
   *
   */
   class CLightPtr : public CRwObjectPtr<RpLight>
#ifdef RWS_EVENTVISUALIZATION
   , public CEventVisualization
#endif
   {
   public:
      /**
      * Default constructor, constructs a NULL CLightPtr pointer
      */
      CLightPtr() : CRwObjectPtr<RpLight>() {}

      /**
      * Constructor, constructs a CLightPtr pointer that points to \a p;
      *
      * \param p Pointer to an RpLight object.
      */
      CLightPtr(RpLight* p) : CRwObjectPtr<RpLight>(p) {}

      /**
      * Destructor for CLightPtr object, if this CLightPtr is still assigned to an RpLight calls CLightPtr::Destroy.
      */
      ~CLightPtr() { if (p_) LightHelper::Destroy(p_); }

      /**
      *
      * Copy operator, if the pointer is already assigned to an RpLight, calls LightHelper::Destroy
      * before assigning the new RpLight pointer, this ensures that an object pointed to
      * by the smart pointer is never left dangling.
      *
      * \param p Pointer to an RpLight object.
      */
      CLightPtr& operator = (RpLight* p)
      {
         if (p_ != p)
         {
            if (p_) LightHelper::Destroy(p_);

            p_ = p;
         }

         return *this;
      }
      
      /**
      *
      * Mandatory CEventVisualization interface
      *
      * \param v Reference to a RwV3d object, filled in with the position of the RpLight.
      * \note Get3dPos is only available if RWS_EVENTVISUALIZATION is defined, this function is
      * only intended to be used during development by the event visualization system.
      *
      */
#ifdef RWS_EVENTVISUALIZATION
      virtual RwV3d *GetWorldPos(void) 
      {
         if (p_)
         {
            RwFrame *pFrame = RpLightGetFrame(p_);

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
      * Handles system commands, CMD_LoadMatrix
      *
      * \param rAttr A reference to a CAttributePacket.
      */
      void HandleSystemCommands(const CAttributePacket& rAttr)
      {
         if (*this)
         {
            LightHelper::HandleSystemCommands(*p_, rAttr);
         }
      }

   private:

      // Revoked ops
      CLightPtr(const CLightPtr&);
      CLightPtr& operator = (const CLightPtr&);
   };
}

extern RwBool RpLightDestroy(RWS::CLightPtr &rCLightPtr);

#endif
