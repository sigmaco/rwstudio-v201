/*****************************************************************************
 *
 * File :     CFXLight_2Stage.h
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

#ifndef __CFXLIGHT2STAGE_H__
#define __CFXLIGHT2STAGE_H__

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "../cfxbaselight.h"
#include "../../mainloop/logic.h"


namespace RWS
{

   namespace FX
   {
      /**
      *
      *  \ingroup Mod_FX
      *
      *  This class defines a RenderWare Studio Workspace interface to a 
      *  CFXLight_2Stage. This type of light can be triggered to turn on and 
      *  off by an event.  This can be useful if you need to 
      *  trigger a warning light within a game for example. This light cycles 
      *  through a source and destination colors allowing you to play once, 
      *  loop and ping pong the cycle.
      */
      class CFXLight_2Stage: public CFXBaseLight, public CEventHandler
      {
      public:
         RWS_MAKENEWCLASS(CFXLight_2Stage);
         RWS_DECLARE_CLASSID(CFXLight_2Stage);
         RWS_CATEGORY("FX");
         RWS_DESCRIPTION("2 Stage Light", "");

         enum LERPTYPE { PLAY_ONCE, PING_PONG, LOOP };
         
         CFXLight_2Stage(const CAttributePacket&);
         virtual ~CFXLight_2Stage(void);

         virtual void HandleAttributes(const CAttributePacket& attr);
         virtual void HandleEvents(CMsg &pMsg);
         void RotateLight( void );
         void UpdateLight( void );
         bool EnableLight( CMsg &pMsg );
         bool DisableLight( CMsg &pMsg );

         RWS_BEGIN_COMMANDS
            RWS_SEPARATOR("Light Events", 0) 

            RWS_MESSAGE( CMD_m_trigger_enable,
                         "Trigger Event, Enable Light",  
                         "Event used to start/stop the light spinning",   
                         RECEIVE, 
                         RwFrame*, 
                         0)

            RWS_ATTRIBUTE( CMD_m_reset_pos_on_trigger,        
                           "Enable update position,", 
                           "Enable update the position of the light when triggered.", 
                           BOOLEAN, 
                           RwUInt32, 
                           1)

            RWS_ATTRIBUTE( CMD_m_reset_on_trigger,        
                           "Enable reset effect.", 
                           "Enable reset of the effect,lerp between color1 and color2 when triggered", 
                           BOOLEAN, 
                           RwUInt32, 
                           1)

            RWS_MESSAGE( CMD_m_trigger_disable,           
                         "Trigger Event, Disable Light",  
                         "Event used to start/stop the light spinning",   
                         RECEIVE, 
                         RwFrame*, 
                         0)

            RWS_SEPARATOR("Color Stages", 0) 
            RWS_ATTRIBUTE(CMD_Set_t_color1,          
                          "Color1",        
                          "Set light color, see RpLightSetColor",   
                          COLOR,  
                          RwRGBA,   
                          DEFAULT(0xffffffff))

            RWS_ATTRIBUTE(CMD_Set_t_color2,          
                          "Color2",        
                          "Set light color, see RpLightSetColor",   
                          COLOR,  
                          RwRGBA,   
                          DEFAULT(0xffffffff))

            RWS_ATTRIBUTE(CMD_step,                  
                          "lerp step",      
                          "Set amount to interpolate by",
                          SLIDER,
                          RwReal,
                          RANGE(0, 0.02 ,1))

            RWS_ATTRIBUTE(CMD_Playback_type,         
                          "Playback type",  
                          "Set the playback type",   
                          RADIO_BUTTON,    
                          RwUInt32,   
                          LIST("PLAY ONCE|PING PONG|LOOP"))

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
          void LERP_Lights(void);
          void ColorCycle (void);
          RwMatrix     *m_mat;                 /**< Matrix used for rotating light */
          RwReal       m_rot[3];               /**< RwReal array for storing rotation values*/

          CEventId     m_trigger_enable;       /**< m_trigger_enable used to enable the light*/ 
          CEventId     m_trigger_disable;      /**< m_trigger_disable used to disable the light*/ 

          RwUInt32     m_reset_pos_on_trigger; /**< Enable/disable reset of position on receipt of 
                                                    m_trigger_enable event */

          RwUInt32     m_reset_on_trigger;     /**< Enable/disable reset of effect on receipt of 
                                                    m_trigger_enable event */

          RwRGBAReal   color1;           /**< Color Stage 1*/
          RwRGBAReal   color2;           /**< Color Stage 2*/
          RwRGBAReal   temp;             /**< Color Stage temp storage*/

          RwRGBAReal * pColor1;
          RwRGBAReal * pColor2;

          RwBool       pingpong;         /**< Ping Pong state*/
          RwReal       step;             /**< Lerp step*/
          RwBool       running;          /**< Is color lerp running*/
          LERPTYPE     LerpType;         /**< Animation type */
          RwBool       LerpFinishRed;    /**< Determine whether a lerp has finished on RED*/
          RwBool       LerpFinishGreen;  /**< Determine whether a lerp has finished on GREEN*/
          RwBool       LerpFinishBlue;   /**< Determine whether a lerp has finished on BLUE*/
      };

   } // namespace FX

} // namespace RWS

#endif

