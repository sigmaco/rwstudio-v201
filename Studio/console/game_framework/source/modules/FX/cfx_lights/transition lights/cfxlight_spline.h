/*****************************************************************************
 *
 * File :     CFXLight_Spline.h
 *
 * Abstract : An entity wrapper for the RpLight type
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

#ifndef __CFXLIGHTSPLINE_H__
#define __CFXLIGHTSPLINE_H__

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "../cfxbaselight.h"
#include "framework/core/eventhandler/ceventhandler.h"
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
      *  CFXLight_Spline. This type of light can be triggered to turn on and 
      *  off by triggering an event.  This can be useful if you need to trigger 
      *  a warning light within a game for example. This light cycles through 
      *  a source and destination colors allowing  you to play once, loop and 
      *  ping pong the cycle.
      */
      class CFXLight_Spline: public CFXBaseLight, public CEventHandler
      { 
      public:
         RWS_MAKENEWCLASS(CFXLight_Spline);
         RWS_DECLARE_CLASSID(CFXLight_Spline);
         RWS_CATEGORY("FX");
         RWS_DESCRIPTION("Spline Light", "");

         enum LERPTYPE { PLAY_ONCE, PING_PONG, LOOP };
      
         CFXLight_Spline(const CAttributePacket&);
         virtual ~CFXLight_Spline(void);

         virtual void HandleAttributes(const CAttributePacket& attr);
         virtual void HandleEvents(CMsg &pMsg);
         void RotateLight( void );
         void UpdateLight( void );
         void EnableLight( void);

          void LERP_Lights(void);
          void EvaluateColor(const RwRGBAReal &c0, const RwRGBAReal &c1, const RwRGBAReal &c2, const RwRGBAReal &c3,
                             const RwRGBAReal &c4, const RwReal t, RwRGBAReal &cr);

         RWS_BEGIN_COMMANDS
            RWS_MESSAGE( CMD_m_trigger,             
                         "Trigger Event",  
                         "Event used to start/stop the light spinning",   
                         RECEIVE, 
                         0, 
                         0)

            RWS_SEPARATOR("Color Stages", 0) 
            RWS_ATTRIBUTE(CMD_Set_c0, 
                          "Color at time = 0.0",  
                          "Color spline c0,c1,c2,c3,c4 over lifetime of particle",   
                          COLOR,  
                          RwRGBA,   
                          DEFAULT(4294967295))

            RWS_ATTRIBUTE(CMD_Set_c1, 
                          "Color at time = 0.25", 
                          "Color spline c0,c1,c2,c3,c4 over lifetime of particle",   
                          COLOR,  
                          RwRGBA,   
                          DEFAULT(4294967295))

            RWS_ATTRIBUTE(CMD_Set_c2, 
                          "Color at time = 0.5",  
                          "Color spline c0,c1,c2,c3,c4 over lifetime of particle",   
                          COLOR,  
                          RwRGBA,   
                          DEFAULT(4294967295))

            RWS_ATTRIBUTE(CMD_Set_c3, 
                          "Color at time = 0.75", 
                          "Color spline c0,c1,c2,c3,c4 over lifetime of particle",   
                          COLOR,  
                          RwRGBA,   
                          DEFAULT(4294967295))

            RWS_ATTRIBUTE(CMD_Set_c4, 
                          "Color at time = 1.0",  
                          "Color spline c0,c1,c2,c3,c4 over lifetime of particle",   
                          COLOR,  
                          RwRGBA,   
                          DEFAULT(4294967295))

            RWS_ATTRIBUTE(CMD_step,                  
                          "lerp step",      
                          "Set amount to interpolate by",
                          SLIDER,
                          RwReal,
                          RANGE(0, 0.02 ,1))

            RWS_SEPARATOR("Rotation Values", 0)
            RWS_ATTRIBUTE( CMD_rot_x,                
                           "X rotation",
                           "X rotation",
                           SLIDER,
                           RwReal,
                           RANGE(-3600, 0 ,3600))

            RWS_ATTRIBUTE( CMD_rot_y,                
                           "Y rotation",
                           "Y rotation",
                           SLIDER,
                           RwReal,
                           RANGE(-3600, 0 ,3600))

            RWS_ATTRIBUTE( CMD_rot_z,                
                           "Z rotation",
                           "Z rotation",
                           SLIDER,
                           RwReal,
                           RANGE(-3600, 0 ,3600))
         RWS_END_COMMANDS;

      protected:
          void ColorCycle (void);
       

          RwMatrix     *m_mat;           /**< Matrix used for rotating light */
          RwReal       m_rot[3];         /**< RwReal array for storing rotation values*/
          CEventId     m_trigger;        /**< m_trigger is used to store the clients reference to an event*/ 
          RwRGBAReal   color[5];         /**< Color Stage 1*/
          RwBool       pingpong;         /**< Ping Pong state*/
          RwReal       step;             /**< Lerp step*/
          RwReal       tempstep;         /**< Lerp step*/
      };
   }
}

#endif

