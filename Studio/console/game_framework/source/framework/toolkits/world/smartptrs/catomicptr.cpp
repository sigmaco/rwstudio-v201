/*****************************************************************************
*
* File :     CAtomicPtr.cpp
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
#include "catomicptr.h"

/**
*
* \ingroup WorldSmartPointers
*
* Function overload for RwBool RpAtomicDestroy(RpAtomic *).
* See RWS::CAtomicPtr::Destroy for details.
*
*  \param rCAtomicPtr Reference to a CAtomicPtr, contains a pointer to the RpAtomic to be
*  destroyed.
*/
RwBool RpAtomicDestroy( RWS::CAtomicPtr &rCAtomicPtr )
{
   RWS_FUNCTION("RWS::RpAtomicDestroy");

   RpAtomic *pAtomic = rCAtomicPtr;

   RwBool ret = RWS::AtomicHelper::Destroy(pAtomic);

   rCAtomicPtr.setptr(0);

   RWS_RETURN(ret);
}