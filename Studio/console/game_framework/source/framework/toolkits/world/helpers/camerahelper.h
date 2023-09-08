/*****************************************************************************
*
* File :     CameraHelper.h
*
* Abstract : Helper functions for use with RwCamera
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

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/core/attributehandler/csystemcommands.h"

namespace RWS
{
  /**
   *
   *  \ingroup CameraHelper
   *
   *  Helper functions for RenderWare Graphics RwCamera object.
   *
   */
   namespace CameraHelper
   {
      RwCamera *CreateMainRasterCamera(RwUInt32 Width, RwUInt32 Height, RwUInt32 ZDepth);

      RwCamera *CreateSubRasterCamera(RwRect &rect);

      RwBool Destroy(RwCamera *pCamera);

      void SetFieldOfView(RwReal rViewWindow, RwCamera *pCamera);

      void HandleSystemCommands(RwCamera &pCamera, const CAttributePacket& rAttr);
   }
}//namespace
