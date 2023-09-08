/*****************************************************************************
 *
 * File :     CFXColorLight.h
 *
 * Abstract : Adds color control to CFXBaseLight
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

#ifndef __CFXCOLORLIGHT_H__
#define __CFXCOLORLIGHT_H__

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "cfxbaselight.h"


namespace RWS
{

   namespace FX
   {
      /**
      *
      *  \ingroup Mod_FX
      *
      *  This class defines a RenderWare Studio Workspace interface to a 
      *  CFXBaseLight which in turn is a wrapper to a RpLight struct.
      *
      */
      class CFXColorLight: public CFXBaseLight
      {
      public:
         RWS_MAKENEWCLASS(CFXColorLight);
         RWS_DECLARE_CLASSID(CFXColorLight);
         RWS_CATEGORY("FX");
         RWS_DESCRIPTION("Color Light", "");

         RWS_BEGIN_COMMANDS
            RWS_ATTRIBUTE(CMD_Set_t_color,          
                          "Color",      
                          "Set light color, see RpLightSetColor",  
                          COLOR,  
                          RwRGBA,   
                          DEFAULT(0xffffffff))
         RWS_END_COMMANDS;
      
         CFXColorLight(const CAttributePacket& rAttr) : CFXBaseLight(rAttr) {}

         virtual void HandleAttributes(const CAttributePacket& attr);
      };

   }
   
}

#endif
