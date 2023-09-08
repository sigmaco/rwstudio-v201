/*****************************************************************************
*
* File :     FPSExampleMonster.h
*
* Abstract : 
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

#ifndef __FPSExampleMonster_H__
#define __FPSExampleMonster_H__

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "fpsmonster.h"

namespace RWS
{
   namespace FPS
   {
      /**
      *
      * \ingroup Mod_FPS
      *
      * Example Monster that uses FPSMonster.  This class derives from FPSMonster and then sets 
      * up some default parameters,  like height,  width and height on ground.  This lets you set up
      * different types of monster that all inherit the basic characteristics of FPSMonster
      *
      * \see FPSMonster.
      *
      */
      class FPSExampleMonster : public FPSMonster
      {
      public:
         RWS_MAKENEWCLASS(FPSExampleMonster);
         RWS_DECLARE_CLASSID(FPSExampleMonster);
         RWS_CATEGORY("FPS");
         RWS_DESCRIPTION("Example Monster", "A generic path following monster");
         
         void HandleAttributes(const CAttributePacket& attr);
         
         FPSExampleMonster(const CAttributePacket&);
         
         RWS_BEGIN_COMMANDS                
            RWS_ATTRIBUTE(CMD_m_height, "Height", "Height of monster", SLIDER, RwReal,RANGE(0, 40, 40))
            RWS_ATTRIBUTE(CMD_m_height_on_ground, "Height On Ground","", SLIDER, RwReal,RANGE(0, 20, 40))
            RWS_ATTRIBUTE(CMD_m_width, "Width", "Width of Monster", SLIDER, RwReal,RANGE(0, 20, 40))                
            RWS_END_COMMANDS;
         
      protected:
      };
      
   } // namespace FPS
}// namespace RWS

#endif