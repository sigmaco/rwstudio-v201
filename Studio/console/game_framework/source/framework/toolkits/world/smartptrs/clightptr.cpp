/*****************************************************************************
*
* File :     CLightPtr.cpp
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
#include "clightptr.h"
#include "../factory.h"

/**
*
*  \ingroup WorldSmartPointers
*
* Function overload for RwBool RpLightDestroy(RpLight *).
* See RWS::CLightPtr::Destroy for details.
*
*/
RwBool RpLightDestroy(RWS::CLightPtr &rCLightPtr)
{
   RWS_FUNCTION("RWS::RpLightDestroy");
   
   RpLight* pLight = rCLightPtr;
   
   RwBool ret = RWS::LightHelper::Destroy(pLight);
   
   // Update the smart pointer
   //
   rCLightPtr.setptr(0);
   
   RWS_RETURN(ret);
}