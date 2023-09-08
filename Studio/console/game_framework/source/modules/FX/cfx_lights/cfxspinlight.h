/*****************************************************************************
 *
 * File :     CFXSpinLight.h
 *
 * Abstract : Rotating light source
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

#ifndef __CFXSPINLIGHT_H__
#define __CFXSPINLIGHT_H__

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "cfxbaselight.h"
#include "framework/toolkits/world/smartptrs/catomicptr.h"
#include "framework/mainloop/logic.h"


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
   *  This type of light can be triggered to turn on and off by triggering 
   *  an event.  This can be useful if you need to trigger a warning light 
   *  within a game for example.   
      */
      class CFXSpinLight: public CFXBaseLight, public CEventHandler
      {
      public:
         RWS_MAKENEWCLASS(CFXSpinLight);
         RWS_DECLARE_CLASSID(CFXSpinLight);
         RWS_CATEGORY("FX");
         RWS_DESCRIPTION("Spinning Light", "");
         
         CFXSpinLight(const CAttributePacket&);
         virtual ~CFXSpinLight(void);
         
         virtual void HandleAttributes(const CAttributePacket& attr);
         virtual void HandleEvents(CMsg &pMsg);
         void RotateLight( void );
         
         
         RWS_BEGIN_COMMANDS
            RWS_SEPARATOR("Light Parameters", 0)
            RWS_MESSAGE( CMD_m_trigger,
                        "Trigger Event",  
                        "Event used to start/stop the light spinning",   
                        RECEIVE, 
                        0, 
                        0)
            
            RWS_ATTRIBUTE(CMD_Set_t_color,          
                          "Color",         
                          "Set light color, see RpLightSetColor",   
                          COLOR,  
                          RwRGBA,   
                          DEFAULT(0xffffffff))
            
            RWS_SEPARATOR("Rotation Values", 0)
            RWS_ATTRIBUTE( CMD_rot_x,   
                           "X rotation",
                           "",
                           SLIDER,
                           RwReal,
                           RANGE(-3600, 0 ,3600))

            RWS_ATTRIBUTE( CMD_rot_y,   "Y rotation","",SLIDER,RwReal,RANGE(-3600, 0 ,3600))
            RWS_ATTRIBUTE( CMD_rot_z,   "Z rotation","",SLIDER,RwReal,RANGE(-3600, 0 ,3600))
            RWS_END_COMMANDS;
         
      protected:
         RwMatrix *m_mat;       /**< Matrix used for rotating light */
         RwReal m_rot[3];       /**< RwReal array for storing rotation values*/
         CEventId m_trigger;    /**< m_trigger is used to store the clients reference to an event*/ 
      };
      
   } // namespace FX
   
} // namespace RWS

#endif
