/*****************************************************************************
*
* File :     CCameraPtr.cpp
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

//#define RWS_CALLSTACKENABLE
//#define RWS_TRACEENABLE

//////////////////////////////////////////////////////////////////
//
// RenderWare Includes
//
#include "rwcore.h"
#include "rpworld.h"

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "ccameraptr.h"

/**
*
* \ingroup WorldSmartPointers
*
* Function overload for RwBool RwCameraDestroy(RwCamera *).
* See RWS::CCameraPtr::Destroy for details.
*
*/
RwBool RwCameraDestroy( RWS::CCameraPtr &rCCameraPtr )
{
   RWS_FUNCTION("RWS::RwCameraDestroy");
   
   RwCamera *pCamera = rCCameraPtr;

   RwBool ret = RWS::CameraHelper::Destroy(pCamera);

   rCCameraPtr.setptr(0);
   
   RWS_RETURN(ret);
}