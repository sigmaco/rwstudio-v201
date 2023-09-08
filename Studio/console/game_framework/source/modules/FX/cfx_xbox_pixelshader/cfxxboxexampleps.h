/*****************************************************************************
 *
 * File :     CFXXBoxExamplePS.h
 *
 * Abstract : Example of how to apply a pixel shader to an atomic.
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
 * Copyright (c) 2000 Criterion Software Ltd.
 * All Rights Reserved.
 *
 * RenderWare is a trademark of Canon Inc.
 *
 *****************************************************************************/
#if (defined (_XBOX))

#ifndef __CFXXBOXEXAMPLEPS_H__
#define __CFXXBOXEXAMPLEPS_H__

//////////////////////////////////////////////////////////////////
//
// RenderWare Includes
//
#include "rwcore.h"
#include "rpworld.h"


#ifdef _MSC_VER
#pragma comment (lib, "rwcore.lib")
#pragma comment (lib, "rpworld.lib")
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "cfxxboxpixelshader.h"
#include "framework/core/eventhandler/ceventhandler.h"
#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/core/attributehandler/csystemcommands.h"
#include "framework/toolkits/world/smartptrs/catomicptr.h"

namespace RWS
{
   namespace FX
   {
      /**
      *
      *  \ingroup Mod_FX
      * 
      * This example pixel shader will make an atomic fade between it normal color and gray.
      * The effect could be used on an enemy to signal they are about to die or have been hit.
      *
      */
      class CFXXBoxExamplePS: public CFXXBoxPixelShader, public CSystemCommands, public CAttributeHandler,
                              public CEventHandler
      {
      public:
         RWS_MAKENEWCLASS(CFXXBoxExamplePS);
         RWS_DECLARE_CLASSID(CFXXBoxExamplePS);
         RWS_CATEGORY("FX_XBOX");
         RWS_DESCRIPTION("Example Pixel Shader", "Example of how to use a pixel shader using RenderWare");

         RWS_BEGIN_COMMANDS
            RWS_ATTRIBUTE(CMD_Set_m_Speed, "Speed",  "Speed of fade",     SLIDER,  RwReal,RANGE(0, 0, 1.0))
         RWS_END_COMMANDS;

         CFXXBoxExamplePS(const CAttributePacket& rAttr);
         ~CFXXBoxExamplePS();
         virtual void         HandleEvents               (CMsg &pMsg);
         virtual void         HandleAttributes           (const CAttributePacket& attr);

      protected:
         RwReal               m_Speed;

         RpXboxPixelShader    *m_PixelShader;
         RwUInt8              m_RampDir;   // 0 up 1 down
         RwReal               m_Blend;

      };

   } // namespace FX
      
} // namespace RWS

#endif
#endif