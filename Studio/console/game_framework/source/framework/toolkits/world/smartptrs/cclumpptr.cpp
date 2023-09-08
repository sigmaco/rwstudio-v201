/*****************************************************************************
*
* File :     CClumpPtr.cpp
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
#include "cclumpptr.h"
#include "../factory.h"
#include "framework/core/eventhandler/ceventhandler.h"

/**
*
*  \ingroup WorldSmartPointers
*
* Function overload for RwBool RpClumpDestroy(RpAtomic *).
* See RWS::CClumpPtr::Destroy for details.
*
*/
RwBool RpClumpDestroy( RWS::CClumpPtr &rCClumpPtr )
{
   RWS_FUNCTION("RWS::RpClumpDestroy");
   
   RpClump *pClump = rCClumpPtr;

   RwBool ret = RWS::ClumpHelper::Destroy(pClump);
   
   rCClumpPtr.setptr(0);

   RWS_RETURN(ret);
}
