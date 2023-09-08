/*****************************************************************************
*
* File :     LightHelper.h
*
* Abstract : Helper functions for use with RpLights
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

#ifndef __LIGHTHELPER_H__
#define __LIGHTHELPER_H__

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
   *  \ingroup LightHelper
   *
   *  Helper functions for RenderWare Graphics RpLight object.
   *
   */
   namespace LightHelper
   {
      void HandleSystemCommands(RpLight& light, const CAttributePacket& attr);

      RwBool Destroy(RpLight* pLight);
   }
}

#endif