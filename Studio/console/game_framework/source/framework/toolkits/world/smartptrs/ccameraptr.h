/*****************************************************************************
*
* File :     CCameraPtr.h
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

#ifndef __CCameraPtr_H__
#define __CCameraPtr_H__

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
#include "../helpers/camerahelper.h"
#include "framework/core/attributehandler/csystemcommands.h"

namespace RWS
{
   /**
   *
   *  \ingroup WorldSmartPointers
   *
   *  Smart pointer for RwCamera. Also provides support for event Visualization \see CEventVisualization
   *  and handling CSystemCommands.        
   *
   */

   class CCameraPtr : public CRwObjectPtr<RwCamera>
#ifdef RWS_EVENTVISUALIZATION
   , public CEventVisualization
#endif
   {
   public:
      /**
      * Default constructor, constructs a NULL CCameraPtr pointer
      */
      CCameraPtr() : CRwObjectPtr<RwCamera>() {}

      /**
      * Constructor, constructs a CCameraPtr pointer that points to \a p;
      *
      * \param p Pointer to an RwCamera object.
      */
      CCameraPtr(RwCamera* p) : CRwObjectPtr<RwCamera>(p) {}

      /**
      * Destructor for CCameraPtr object, if this CCameraPtr is still assigned to an RwCamera
      * calls CameraHelper::Destroy.
      */
      ~CCameraPtr() { if (p_) CameraHelper::Destroy(p_); }

      /**
      *
      * Copy operator, if the pointer is already assigned to an RwCamera, calls Destroy
      * before assigning the new RwCamera pointer, this ensures that an object pointed to
      * by the smart pointer is never left dangling.
      *
      * \param p Pointer to an RwCamera object.
      */
      CCameraPtr& operator = (RwCamera* p)
      {
         if (p_ != p)
         {
            if (p_) CameraHelper::Destroy(p_);
            p_ = p;
         }

         return *this;
      }
     
      /**
      *
      * Mandatory CEventVisualization interface.
      *
      * \param v Reference to a RwV3d object, filled in with the center point of the RwCamera.
      * \note Get3dPos is only available if RWS_EVENTVISUALIZATION is defined, this function is
      * only intended to be used during development by the event Visualization system.
      *
      */
#ifdef RWS_EVENTVISUALIZATION
      virtual RwV3d *GetWorldPos(void) 
      {
         if (p_)
         {
            RwFrame *pFrame = RwCameraGetFrame(p_);

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
      * Handles system commands, CMD_LoadMatrix for an RwCamera
      *
      * \param rAttr A reference to a CAttributePacket.
      */
      void HandleSystemCommands(const CAttributePacket& rAttr)
      {
         if (*this)
         {
            CameraHelper::HandleSystemCommands(*p_, rAttr);
         }
      }

   private:

      // Revoked ops
      CCameraPtr(const CCameraPtr&);
      CCameraPtr& operator = (const CCameraPtr&);
   };
}

extern RwBool RwCameraDestroy( RWS::CCameraPtr &rCCameraPtr );


#endif
