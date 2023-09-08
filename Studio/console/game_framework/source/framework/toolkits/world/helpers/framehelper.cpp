/*****************************************************************************
*
* File :     FrameHelper.cpp
*
* Abstract : RwFrame helper functions.
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
#include "precomp.h"
#include "framework/core/macros/debugmacros.h"

namespace RWS
{
   namespace FrameHelper
   {
      /**
      *
      *  \ingroup FrameHelper
      *
      *  Safe destruction of a RwFrame object, checks if it is fully detached from any other
      *  objects before attempting to delete.
      *
      *  \return Returns TRUE if frame is deleted otherwise FALSE.
      *
      */
      RwBool FrameDestroy(RwFrame *pFrame)
      {
         RWS_FUNCTION("RWS::FrameHelper::FrameDestroy");
         RWS_PRE(pFrame);

         // Is the list empty ?
         //
         if (reinterpret_cast<void*>(pFrame->objectList.link.next) == reinterpret_cast<void*>(&pFrame->objectList))
         {
            RwFrameDestroy(pFrame);

            RWS_RETURN(TRUE);
         }

         RWS_RETURN(FALSE);
      }

      /**
      *
      *  \ingroup FrameHelper
      *
      * RenderWare Graphics callback that removes a child frame from its parent. 
      * Its LTM is copied to its modelling matrix prior to removal - this ensures
      * that any attached objects maintain their position in the world.
      *
      */
      RwFrame* RemoveChildFrame(RwFrame* pFrame, void*)
      {
         RWS_FUNCTION("RWS::FrameHelper::RemoveChildFrame");
         RWS_PRE(pFrame);
      
         RwMatrixCopy( RwFrameGetMatrix(pFrame), RwFrameGetLTM(pFrame) );
         RwFrameRemoveChild(pFrame);
      
         RWS_RETURN(pFrame);
      }
   }
}