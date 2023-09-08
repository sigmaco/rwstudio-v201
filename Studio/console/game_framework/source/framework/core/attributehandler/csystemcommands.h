/*****************************************************************************
*
* File :     CSystemCommands.h
*
* Abstract : ABC for classes that responds to RWS system commands
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


#ifndef _CSystemCommands_H__
#define _CSystemCommands_H__

//////////////////////////////////////////////////////////////////
//
// Renderware Studio Includes
//
#include "../attributehandler/cattributehandler.h"

namespace RWS
{
   /**
   *
   *  \ingroup CSystemCommands
   *
   *  \page CSystemCommandsOverview Overview
   *
   *  This module declares a class that defines a Renderware Studio Interface 
   *  used to manipulate 3D objects on the console.
   *
   */

   /**
   *
   *  \ingroup CSystemCommands
   *
   *  This class defines a set of common attributes used by the RenderWare 
   *  Studio Workspace. 
   *
   *  \note Not all behaviors will make use of these attributes, 
   *  hence they are not part of the CAttributeHandler class.
   *
   *  As an example, the position and orientation of an entity in the 
   *  game database is sent to the framework via the CMD_LoadMatrix attribute. 
   *  To get access to this attribute a behaviour must either publicly inherit 
   *  from CSystemCommands, or use RWS_IMPORT_COMMANDS to import the 
   *  attributes defined in CSystemCommands.
   *
   */

   class CSystemCommands
   {
   public:

      RWS_DECLARE_CLASSID(CSystemCommands);

      RWS_BEGIN_COMMANDS

           RWS_ATTRIBUTE(CMD_AttachResource,    "Resource",             "Specify a resource attached to this entity",   NULL,    RwUInt32,   DEFAULT(0))
           RWS_ATTRIBUTE(CMD_LoadMatrix,        "Matrix",               "Specify a transformation matrix associated with this entity",   MATRIX,  RwMatrix,   DEFAULT(0))
           RWS_ATTRIBUTE(CMD_SetSolidFlag,      "Solid Collisions",     "Collisions repel intersecting objects. (If Enable Collisions is true)",   BOOLEAN, RwUInt32,   DEFAULT(1))
           RWS_ATTRIBUTE(CMD_SetCollisionFlag,  "Enable Collisions",    "Enable/Disable Collisions. If collisions are enabled but solid collisions is disabled then the object will be sent collision events",    BOOLEAN, RwUInt32,   DEFAULT(1))
           RWS_ATTRIBUTE(CMD_SetInvisibleFlag,  "Invisible",            "Set visible/invisible flag, invisible objects are not rendered but may be used to generate collisions.",   BOOLEAN, RwUInt32,   DEFAULT(0))

      RWS_END_COMMANDS;

      static const RWSGUID * FindFirstResource( const CAttributePacket& attr );
      static const void* FindFirstResourceOfType( const CAttributePacket& attr, const RwChar* pType );

      // RWS_PRE( CMD_LoadMatrix == attrCmd.GetCommandId() )
      static void UpdateFrame( RwFrame& frame, const CAttributeCommand& attrCmd );
      static void UpdateFrame( RwFrame& frame, const RwMatrix *pUnalignedMat);

   private:

      // Revoked ops
      CSystemCommands& operator = ( const CSystemCommands& );
   };
   
   /**
   *
   *  Functions relating to RpAtomic Collision Properties plugin.
   *
   */
   namespace RpAtomicCollisionProperties
   {

      /**
      *
      *  Plugin for adding extra collision data to an RpAtomic
      *
      */
      RwBool PluginAttach();

      void SetIsSolid( RpAtomic& atomic, bool isSolid );

      bool GetIsSolid( const RpAtomic& atomic );

      void SetIsSolid( RpClump& clump, bool isSolid );

      bool GetIsSolid( const RpClump& atomic );
   }
}

#endif
