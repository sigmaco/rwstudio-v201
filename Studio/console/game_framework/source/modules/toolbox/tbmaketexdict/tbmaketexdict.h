/*****************************************************************************
*
* File : TBMakeTexDict.h
*
* Abstract : This isn't really a game behavior, although it does work within
* the gameframework. This behavior class is used recalculate lightmaps on a
* target platform.
*
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
* Copyright (c) 2002 Criterion Software Ltd.
* All Rights Reserved.
*
* RenderWare is a trademark of Canon Inc.
*
*****************************************************************************/
#ifdef RWS_DESIGN

#ifndef __TOOLBOX_MAKETEXDICT__
#define __TOOLBOX_MAKETEXDICT__

//////////////////////////////////////////////////////////////////
//
// RenderWare Includes
//
#include <rpcollis.h>
#include <rtintsec.h>

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/core/eventhandler/ceventhandler.h"
#include "framework/core/attributehandler/cattributehandler.h"

#include "framework/core/attributehandler/allocation policies/LinearAllocationPolicy.h"

namespace RWS
{
   namespace ToolBox
   {
      /**
      *
      * \ingroup ToolBox
      *
      * ATBMakeTexDict is a tool box behavior, it is design to be used during the production of a game to
      * save texture dictionaries generated on the target console back to the workspace running on the host
      * PC.
      */
      class ATBMakeTexDict: public CEventHandler, public CAttributeHandler, public LinearAllocationPolicy
      {
      public:
         
         RWS_MAKENEWCLASS(ATBMakeTexDict);
         RWS_DECLARE_CLASSID(ATBMakeTexDict);
         RWS_CATEGORY("Tools");
         RWS_DESCRIPTION("Save Texture Dictionary", "Save the current texture dictionary from the console to the workspace.");

         RWS_BEGIN_COMMANDS
            RWS_MESSAGE( CMD_GenerateTextDict, "Generate Texture Dict", "Generate and save texture dictionary.", RECEIVE ,0, 0)
         RWS_END_COMMANDS;
         
         virtual void HandleEvents(CMsg &pMsg);
         virtual void HandleAttributes(const CAttributePacket& attr);
         
         ATBMakeTexDict(const CAttributePacket&);
         ~ATBMakeTexDict(void);
         
      private:
         
         CEventId m_MakeTexDict;  /**< Event use to trigger generation of the Texture Dict */
         
      protected:
      };
   }
}

#endif
#endif
