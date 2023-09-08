/*****************************************************************************
*
* File :     FrameHelper.h
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


#ifndef __FRAMEHELPER_H__
#define __FRAMEHELPER_H__

//////////////////////////////////////////////////////////////////
//
// Renderware Studio Includes
//

namespace RWS
{
   /**
   *
   *  \ingroup FrameHelper
   *
   *  Helper functions for RenderWare Graphics RwFrame object.
   *
   */
   namespace FrameHelper
   {
      RwBool FrameDestroy(RwFrame *pFrame);
      RwFrame* RemoveChildFrame(RwFrame* pFrame, void*);
   }
}

#endif