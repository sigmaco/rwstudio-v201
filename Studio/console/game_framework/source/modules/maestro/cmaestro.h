/****************************************************************************
 *
 * This file is a product of Criterion Software Ltd.
 *
 * This file is provided as is with no warranties of any kind and is
 * provided without any obligation on Criterion Software Ltd.
 * or Canon Inc. to assist in its use or modification.
 *
 * Criterion Software Ltd. and Canon Inc. will not, under any
 * circumstances, be liable for any lost revenue or other damages
 * arising from the use of this file.
 *
 * Copyright (c) 2002 Criterion Software Ltd.
 * All Rights Reserved.
 *
 */

/****************************************************************************
 *
 * CMaestro.h - Maestro behavior
 *
 ****************************************************************************/


/**
*
*  \ingroup Mod_Maestro
*
*  \page MaestroOverview Overview
*
*  Maestro is a high level coordinator of the playback of user-interactive
*  animations incorporating buttons and hierarchical animations of 2d scenes. 
*
*  Maestro animations are generated using RenderWare tools which convert from Macromedia
*  Shockwave Flash animations.  The tool '2dconvrt' produces a .anm file which can be
*  used in RenderWare Studio to playback the animation using Maestro functions.
*
*  To use a simple Maestro animation in RenderWare Studio, add the .anm asset to an entity
*  using the Maestro behavior.  To use a Maestro animation that contains font information
*  you will need to export the font data using the Maestro Exporter tool in the Tools directory.
*  Load in the Maestro animation that uses the font data and export the .fnt file to be used
*  alongside the .anm file in RenderWare Studio.  The font data has to be loaded before the
*  animation data so make sure it is located above the animation asset in the Game Database.
*  To use a Maestro animation that contains textures you will need to export a Texture Dictionary
*  using the Maestro Exporter.  Export the Texture Dictionary .txd file and make sure it is
*  located above the animation asset too.
*
*  Further details on the Maestro toolkit can be found in the RenderWare documentation.
*
*/

/**
*
*  \ingroup Mod_Maestro
*
*  \page MaestroFeatures Features
*
*  The Maestro behavior includes various controls to interact with the Maestro animation and
*  move it around the screen.
*
*  You can move the Maestro animation around using the sliders, this allows you to move left
*  and right across the screen, up and down the screen and scaled in and out of the screen.
*
*  The animation controls are useful when you need to step through the hierarchy of animations
*  contained in the Maestro animation data.  If you stop the animation playing then you can move
*  to a certain animation index and a frame within that animation using the provided sliders.
*
*  Interpolation between animation frames can be turned on and off to give smoother animations
*  when required.
*
*  Some Maestro animations may contain buttons and menu hierarchies.  The animations within these
*  can be triggered using button events.  An event trigger is provided to test the button activation.
*  Normally you would link these button events to controller events which provides a useful way of
*  generating menu systems from Maestro animations.
*
*/


/**
*
*  \ingroup Mod_Maestro
*
*  \page MaestroMenuAnimations Menu Animations
*
*  You can hook into the Maestro Menu Animations in order to activate buttons within them and trigger
*  new states in the animation.
*
*  An example of this is the Main Menu animation example (mainmenu.anm).  This has four buttons you can
*  interact with, this behavior uses the names of these buttons to send events to the Maestro system
*  and activate the associated buttons.  The buttons are called:
*
*  \li btnUp - Moves the menu highlight up.
*  \li btnDown - Moves the menu highlight down.
*  \li btnSelect - Moves to the next menu in the animation if possible.
*  \li btnCancel - Moves to the previous menu in the animation if possible.
*
*  Some IDs are generated in \ref Calculate_Button_IDs which then get used to specify which button should be
*  triggered.  \ref GetButtonLabelIndexFromEventName compares the strings sent from the events to determine
*  which ID should be set. The strings corresponding to the button names above are:
*
*  \li Up: INQ_MENU_BTN_UP
*  \li Down: INQ_MENU_BTN_DOWN
*  \li Select: INQ_MENU_BTN_SELECT
*  \li Cancel: INQ_MENU_BTN_CANCEL
*
*  Using these strings with the Main Menu animation you will be able to interact with the menu.
*/


#ifndef __CMaestro_H__
#define __CMaestro_H__

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/core/eventhandler/ceventhandler.h"
#include "framework/toolkits/world/smartptrs/catomicptr.h"
#include "framework/mainloop/logic.h"
#include "icmaestro.h"

#include "framework/core/attributehandler/allocation policies/LinearAllocationPolicy.h"

namespace RWS
{
   /**
   *
   */
   namespace Maestro
   {
      /**
      *
      *  \ingroup Mod_Maestro
      *
      *  Maestro behavior class.
      *
      *  The Maestro behavior allows playback of 2D animations within RenderWare Studio.
      *  See the overview and features for a more detailed description of the Maestro functionality.
      *  
      *
      */
      class CMaestro : public CSystemCommands, public CAttributeHandler, public CEventHandler, public LinearAllocationPolicy
      {
      public:

         RWS_MAKENEWCLASS(CMaestro);
         RWS_DECLARE_CLASSID(CMaestro);
         RWS_CATEGORY("Examples");
         RWS_DESCRIPTION("Maestro Player", "Example of using the Maestro plugin, to playback 2D anims.");

         CMaestro(const CAttributePacket& attr);
         ~CMaestro(void);

         virtual void HandleEvents(CMsg &pMsg);
         virtual void HandleAttributes(const CAttributePacket& attr);

         RWS_BEGIN_COMMANDS

            RWS_SEPARATOR( "Rendering", 0 )

            RWS_MESSAGE( 
                         CMD_Start_Render,
                         "Start Render Event",
                         "Begin rendering specified camera.",
                         RECEIVE ,
                         RwCamera*,
                         "iMsgDoRender"
                       )

            RWS_MESSAGE( CMD_End_Render,
                         "End Render Event",
                         "Pass Camera on so that any other rendering can be done.",
                         TRANSMIT,
                         RwCamera*,
                         0
                       )

            RWS_ATTRIBUTE( 
                           CMD_Priority,
                           "Render Priority",
                           "Specify the render priority.",
                           SLIDER,
                           RwUInt32,
                           RANGE(0, 0, 65535)
                         )

            RWS_ATTRIBUTE( 
                           CMD_ShowHide,
                           "Show/ Hide Current Animation",
                           "Turn this on to make the current animation visible and off to make it invisible.",
                           BOOLEAN,
                           RwUInt32,
                           DEFAULT(1)
                         )

            RWS_SEPARATOR( "Animation Controls", 0 )

            RWS_ATTRIBUTE( 
                           CMD_PlayMessage,
                           "Play Current Animation",
                           "Turn this on to play the current animation and off to stop the current animation.",
                           BOOLEAN,
                           RwUInt32,
                           DEFAULT(1)
                         )

            RWS_ATTRIBUTE(
                           CMD_Interp,          
                           "Interpolation",
                           "Turn this on to enable blending in-between frames of the animation. This results in a smoother animation.",
                           BOOLEAN, 
                           RwUInt32,
                           DEFAULT(1)
                         )

            RWS_SEPARATOR( "Position and Scaling", 0 )

            RWS_ATTRIBUTE( CMD_SetPosX,
                           "Position Across",
                           "Set the offset to move the Maestro animation left or right on the screen. Set as zero to move to default position.",
                           SLIDER,
                           RwReal,
                           RANGE(-500.0, 0.0, 500.0))

            RWS_ATTRIBUTE( CMD_SetPosY,
                           "Position Up And Down",
                           "Set the offset to move the Maestro animation up or down on the screen. Set as zero to move to default position.",
                           SLIDER,
                           RwReal,
                           RANGE(-500.0, 0.0, 500.0)
                         )

            RWS_ATTRIBUTE( CMD_SetPosZ,
                           "Scaling",
                           "Set the offset to move the Maestro animation in or out of the screen. This scales the Maestro animation accordingly. Set as zero to move to default scale.",
                           SLIDER,
                           RwReal,
                           RANGE(-1.0, 0.0, 1.0)
                         )

            RWS_SEPARATOR( "Menu Buttons", 0 )

            RWS_MESSAGE( CMD_TriggerCntrlButton,
                         "Send Button Event",
                         "Specify a button event to trigger which also test fires the event to the Maestro animation.",
                         RECEIVE,
                         return RwReal,
                         "INQ_MENU_BTN_DOWN"
                        )

         RWS_END_COMMANDS;

      private:

         void InquireInput(void); /**< Each tick the input is inquired to test if there are any button messages to be sent */

         iCMaestro m_Maestro; /**< Maestro object */

         enum but_ids
         {
            eBut_up = 0,
            eBut_down,
            eBut_left,
            eBut_right,
            eBut_select,
            eBut_cancel,
            eBut_total
         };

         CEventId m_Msg_Button[eBut_total];       /**< Event used to trigger a button transition */
         CEventId m_TriggerCntrlButton;           /**< Event used to trigger a test controller button transition */
         CEventId m_StartRender;                  /**< Event used to specify when rendering should begin with the
                                                       specified RwCamera */
         CEventId m_EndRender;                    /**< Event used to specify when rendering should begin with the
                                                       specified RwCamera */
         CEventId m_RenderCamera;                 /**< Event used to specify rendering with the specified RwCamera */

         RwUInt32 m_Link_Priority;                /**< Current Link Priority, Used to determine rendering order */
      };
   } //namespace Maestro
} //namespace RWS

#endif // __CMaestro_H__
