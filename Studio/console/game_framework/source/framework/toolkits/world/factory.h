/*****************************************************************************
*
* File :     Factory.h
*
* Abstract : Factory functions for atomics and clumps
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


#ifndef __FACTORY_H__
#define __FACTORY_H__

/**
*
*  \ingroup Factory
*  \page FactoryOverview Overview
*
*  Provides functionality for creating and destroying clumps and atomics and linking them
*  to their parent event handler.
*
*/

struct RpClump;
struct RpAtomic;
struct RwFrame;

//////////////////////////////////////////////////////////////////
//
// Renderware Studio Includes
//

namespace RWS
{
   // Fwd. decls
   class CEventHandler;
   class CAttributePacket;

   RpAtomic *CreateAtomicInWorldFromResource( const CAttributePacket& attr, CEventHandler* eventHandler = 0);
   RpAtomic *CreateAtomicFromResource( const CAttributePacket& attr, CEventHandler* eventHandler = 0);

   RpClump *CreateClumpInWorldFromResource( const CAttributePacket& attr, CEventHandler* eventHandler = 0);
   RpClump *CreateClumpFromResource( const CAttributePacket& attr, CEventHandler* eventHandler = 0);
}

#endif