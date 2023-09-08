/*****************************************************************************
 *
 * File :     CSetCLevelHint.h
 *
 * Abstract:   Declaration of CSetCLevelHint class
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

#ifndef __CSETCLEVELHINT_H__
#define __CSETCLEVELHINT_H__

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/core/attributehandler/csystemcommands.h"
#include "framework/toolkits/world/clevel.h"
#include "framework/core/attributehandler/allocation policies/LinearAllocationPolicy.h"


namespace RWS
{
   namespace Generic
   {
      /**
      *
      *  \ingroup Mod_Generic
      *
      *  Behavior used to set the CLevelHint used by CLevel to determine whether a cloned instance of an asset
      *  should be added to the opaque world in preference to the translucent world \see CLevel.
      *
      */
      class CSetCLevelHint : public CAttributeHandler, public CSystemCommands, public LinearAllocationPolicy
      {
         public:

            RWS_MAKENEWCLASS(CSetCLevelHint);
            RWS_DECLARE_CLASSID(CSetCLevelHint);
            RWS_CATEGORY("Generic");
            RWS_DESCRIPTION("Set CLevelHint", "Sets the CevelHint for attached assets.");

            RWS_BEGIN_COMMANDS

               RWS_ATTRIBUTE(CMD_CLevelHint, "Set the Level Hint", 
                  "Specify the CLevelHint for each of the assets attached to this instance of CSetCLevelHint behavior,"
                  " typically for this to take effect the level should be reset. The level hint is used by the level"
                  " manager CLevel to determine the preferred world to add an instance of the atomic/clump to.",
                  LIST, RwUInt32, LIST("@HINT_OPAQUE_WORLD|HINT_TRANSLUCENT_WORLD"))

            RWS_END_COMMANDS;

            CSetCLevelHint(const CAttributePacket&);

            virtual void HandleAttributes(const CAttributePacket& attr);

         private:

            CLevel::CLevelHint m_CLevelHint;
      };
   }
}
#endif