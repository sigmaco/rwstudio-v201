/*****************************************************************************
 *
 * File :     CEntity.h
 *
 * Abstract:   Declaration of CEntity class
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

#ifndef __CENTITY_H__
#define __CENTITY_H__

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/core/attributehandler/csystemcommands.h"
#include "framework/core/attributehandler/allocation policies/linearallocationpolicy.h"

namespace RWS
{
   namespace Generic
   {
      // Special case, the CEntity class has its own version of the MakeNew
      // function which differs from the standard function defined by RWS_MAKENEWCLASS.
      // The RenderWare Studio Workspace, looks for the RWS_MAKENEWCLASS in order to identify
      // the class as a behavior, removing the define allows the CEntity class to define a
      // custom MakeNew function and still expose the CEntity class to the Workspace.
      //
#define RWS_BEGIN_MACRO_DEFINITION
#undef RWS_MAKENEWCLASS
#define RWS_MAKENEWCLASS(var) enum{ObFuScAtE_RWS_MAKENEWCLASS = RWFORCEENUMSIZEINT}
#define RWS_END_MACRO_DEFINITION

      /**
      *
      *  \ingroup Mod_Generic
      *
      *  CEntity is the default behavior applied to an entity when it is added to the world. CEntity is a factory
      *  class and will where possible create a behavior class suitable for displaying any asset attached to it.
      *
      */
      class CEntity : public CAttributeHandler, public CSystemCommands, public LinearAllocationPolicy
      {
      public:
         RWS_MAKENEWCLASS(CEntity);
         RWS_DECLARE_CLASSID(CEntity);
         RWS_CATEGORY("Generic");
         RWS_DESCRIPTION("Default Behavior.", "This is the default behavior of all assets when they are first added to the world.");

         static RWS::CAttributeHandler* MakeNew(const CAttributePacket& attr);

         static RwUInt32 GetMaxClassSize(void);

      protected:
         CEntity() {}

      private:
         CEntity( const CEntity& );
         CEntity& operator=( const CEntity& );
      };
   }//namespace Generic
}//namespace RWS

#endif