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
 * iCMaestro.cpp - Implementation of Maestro behavior
 *
 ****************************************************************************/

#include "precomp.h"

#ifdef __MWERKS__
#pragma force_active on
#endif

// RenderWare Includes
#include "rwcore.h"
#include "rt2d.h"
#include "rt2danim.h"

#include "icmaestro.h"

//#define RWS_CALLSTACKENABLE
//#define RWS_TRACEENABLE

//#define DUMP_MAESTRO_MESSAGE // define this to get debug messages for Maestro message system

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/core/macros/debugmacros.h"
#include "framework/core/resourcemanager/cresourcemanager.h"
#include "framework/toolkits/world/clevel.h"
#include "framework/toolkits/world/helpers/camerahelper.h"
#include "framework/mainloop/render.h"

static RwInt32 g_iNum_Anims = 0;
static RwInt32 g_iCurrentAnimation = 0;

const RwUInt32 CONTROLLER_STATE_RESET_TIME = 50;

namespace RWS
{
   /**
   *
   */
   namespace Maestro
   {

      /**
      *
      *
      *  \ref iCMaestro
      *
      *  Constructor for iCMaestro
      *
      *  \see ~iCMaestro
      */
      iCMaestro::iCMaestro():
         m_pMatrix(0),
         m_rScale(0.0f),
         m_MaxScale(1.0f), // Maximum scale should correspond to max range for CMD_SetPosZ

         m_iWinHeight(480),
         m_iWinWidth(640),

         m_bViewChanged(FALSE),
         m_rSpeed(1.0f),
         m_bInterpolate(TRUE),
         m_bShowBBox(FALSE),
         m_pMaestro(0),
         m_pScene(0),
         m_pBBox(0),

         m_iController_State_Reset_Timer(0),
         m_iController_State(0),
         m_iButDown(0),
         m_iButUp(0)

      {
         RWS_FUNCTION("RWS::Maestro::iCMaestro::iCMaestro");

         m_vPosition.x = 0.0f;
         m_vPosition.y = 0.0f;

         m_MaxPos.x = 500.0f; // Maximum position should correspond to max range for CMD_SetPosX
         m_MaxPos.y = 500.0f; // Maximum position should correspond to max range for CMD_SetPosY

         RWS_RETURNVOID();
      }

      
      /**
      *
      *
      *  \ref ~iCMaestro
      *
      *  Destructor for iCMaestro
      *
      *  \see iCMaestro
      */
      iCMaestro::~iCMaestro()
      {
         RWS_FUNCTION("RWS::Maestro::iCMaestro::~iCMaestro");

         Clean();

         RWS_RETURNVOID();
      }


      /**
      *
      *  \ref AllAnimGoToFrame is a callback which runs through all visible anims
      *  and sends a message defining the transition required to go to the given frame
      *  in the current animation.
      *
      *  \param pMaestro
      *  \param pAnim
      *  \param pProps
      *  \param pData
      *
      *  \return A Pointer to the Maestro object.
      */
      static Rt2dMaestro* AllAnimGoToFrame(  Rt2dMaestro *pMaestro,
                                             Rt2dAnim *pAnim,
                                             Rt2dAnimProps *pProps,
                                             void *pData)
      {
         RWS_FUNCTION("RWS::Maestro::AllAnimRestart");

         Rt2dMessage message;
         RwInt32 iNumFrames;
         RwInt32 iFrame;

         iFrame = reinterpret_cast<RwInt32>(pData); // frame to go to

         iNumFrames = Rt2dAnimGetNumberOfKeyFrames(Rt2dMaestroGetAnimationsByIndex(pMaestro, g_iCurrentAnimation));

         if (iFrame >= iNumFrames)
         {
            RWS_RETURN(pMaestro);
         }

         message.messageType = rt2dMESSAGETYPEGOTOFRAME;
         message.index = g_iCurrentAnimation;
         message.intParam1 = iFrame; 
         message.intParam2 = 0;

         // Post the message and process.
         Rt2dMaestroPostMessages(pMaestro, &message, 1);

         RWS_RETURN(pMaestro);
      }


      /**
      *
      *  \ref AllAnimRestart is a callback which runs through all visible anims
      *  and sends a message defining the transition required to restart the current animation.
      *
      *  \param pMaestro
      *  \param pAnim
      *  \param pProps
      *  \param pData
      *
      *  \return A Pointer to the Maestro object.
      */
      static Rt2dMaestro* AllAnimRestart(Rt2dMaestro *pMaestro, Rt2dAnim *pAnim, Rt2dAnimProps *pProps, void *pData)
      {
         RWS_FUNCTION("RWS::Maestro::AllAnimRestart");

         Rt2dMessage message;

         message.messageType = rt2dMESSAGETYPEGOTOFRAME;
         message.index = reinterpret_cast<RwInt32>(pData); //root animation label
         message.intParam1 = 0;
         message.intParam2 = 0;

         // Post the message and process.
         Rt2dMaestroPostMessages(pMaestro, &message, 1);

         RWS_RETURN(pMaestro);
      }


      /**
      *
      *  \ref AllAnimStop is a callback which runs through all visible anims
      *  and sends a message defining the transition required to stop the current animation.
      *
      *  \param pMaestro
      *  \param pAnim
      *  \param pProps
      *  \param pData
      *
      *  \return A Pointer to the Maestro object.
      */
      static Rt2dMaestro* AllAnimStop(Rt2dMaestro *pMaestro, Rt2dAnim *pAnim, Rt2dAnimProps *pProps, void *pData)
      {
         RWS_FUNCTION("RWS::Maestro::AllAnimStop");

         Rt2dMessage message;

         message.messageType = rt2dMESSAGETYPESTOP;
         message.index = -1; // No animation index needed as Maestro will find the right one
         message.intParam1 = 0;
         message.intParam2 = 0;

         // Post the message and process.
         Rt2dMaestroPostMessages(pMaestro, &message, 1);

         RWS_RETURN(pMaestro);
      }


      /**
      *
      *  \ref AllAnimPlay is a callback which runs through all visible anims
      *  and sends a message defining the transition required to play the current animation.
      *
      *  \param pMaestro
      *  \param pAnim
      *  \param pProps
      *  \param pData
      *
      *  \return A Pointer to the Maestro object.
      */
      static Rt2dMaestro* AllAnimPlay(Rt2dMaestro *pMaestro,
         Rt2dAnim *pAnim, Rt2dAnimProps *pProps, void *pData)
      {
         RWS_FUNCTION("RWS::Maestro::AllAnimPlay");

         Rt2dMessage message;

         message.messageType = rt2dMESSAGETYPEPLAY;
         message.index = -1; // No animation index needed as Maestro will find the right one
         message.intParam1 = 0;
         message.intParam2 = 0;

         // Post the message and process.
         Rt2dMaestroPostMessages(pMaestro, &message, 1);

         RWS_RETURN(pMaestro);
      }


      /**
      *
      *  \ref AllAnimActivateButton is a callback which runs through all visible anims
      *  and sends a message defining the transition required.
      *
      *  \param pMaestro
      *  \param pAnim
      *  \param pProps
      *  \param pData
      *
      *  \return A Pointer to the Maestro object.
      */
      static Rt2dMaestro* AllAnimActivateButton(Rt2dMaestro *pMaestro,
         Rt2dAnim *pAnim, Rt2dAnimProps *pProps, void *pData)
      {
         RWS_FUNCTION("RWS::Maestro::AllAnimActivateButton");

         Rt2dMessage message;

         // Trigger button using it's label id
         message.messageType = rt2dMESSAGETYPEBUTTONBYLABEL;
         message.index = -1;  // No animation index needed as Maestro will find the right one
         message.intParam1 = reinterpret_cast<RwInt32>(pData); // button label index
         message.intParam2 = rt2dANIMBUTTONSTATEOVERDOWNTOOVERUP;

         // Pass on the message to the Maestro
         Rt2dMaestroPostMessages(pMaestro, &message, 1);

         RWS_RETURN(pMaestro);
      }


      /**
      *
      *
      *  \ref AllAnimSetInterpolate sets interpolation on all animations.
      *
      *  \param pMaestro
      *  \param pAnim
      *  \param pProps
      *  \param pData
      *
      *  \return A Pointer to the Maestro object.
      */
      static Rt2dMaestro* AllAnimSetInterpolate(Rt2dMaestro *pMaestro,
         Rt2dAnim *pAnim, Rt2dAnimProps *pProps __RWUNUSED__, void *pData)
      {
         RWS_FUNCTION("RWS::Maestro::AllAnimSetInterpolate");

         Rt2dAnimSetInterpolate(pAnim, (RwBool)pData);

         RWS_RETURN(pMaestro);
      }

      
      /**
      *
      *
      *  \ref AllAnimCountAnims counts the total number of anims in the Maestro file.
      *
      *  \param pMaestro
      *  \param pAnim
      *  \param pProps
      *  \param pData
      *
      *  \return A Pointer to the Maestro object.
      */
      static Rt2dMaestro* AllAnimCountAnims(Rt2dMaestro *pMaestro,
         Rt2dAnim *pAnim __RWUNUSED__,
         Rt2dAnimProps *pProps __RWUNUSED__,
         void *pData __RWUNUSED__)
      {
         RWS_FUNCTION("RWS::Maestro::AllAnimCountAnims");

         g_iNum_Anims++;  

         RWS_RETURN(pMaestro);
      }
   
#ifdef DUMP_MAESTRO_MESSAGE
      /**
      *
      *
      *  \ref MessageDumper prints out which messages are being sent by Maestro message system.
      *
      *  \param pMaestro
      *  \param pMessage
      *
      */
      static void MessageDumper(Rt2dMaestro *pMaestro, Rt2dMessage *pMessage)
      {
         RWS_FUNCTION("RWS::Maestro::MessageDumper");

         RwChar str[256];

         switch (pMessage->messageType)
         {
            case rt2dMESSAGETYPEPLAY:
            {
               RwDebugSendMessage(rwDEBUGMESSAGE, RWSTRING("Message :"),
                              RWSTRING("rt2dMESSAGETYPEPLAY"));

               rwsprintf(str, RWSTRING("  anim %d"), pMessage->index);
               RwDebugSendMessage(rwDEBUGMESSAGE, RWSTRING("Message :"),
                              str);
            }
            break;
            case rt2dMESSAGETYPESTOP:
            {
               RwDebugSendMessage(rwDEBUGMESSAGE, RWSTRING("Message :"),
                              RWSTRING("rt2dMESSAGETYPESTOP"));

               rwsprintf(str, RWSTRING("  anim %d"), pMessage->index);
               RwDebugSendMessage(rwDEBUGMESSAGE, RWSTRING("Message :"),
                              str);

            }
            break;
            case rt2dMESSAGETYPENEXTFRAME:
            {
               RwDebugSendMessage(rwDEBUGMESSAGE, RWSTRING("Message :"),
                              RWSTRING("rt2dMESSAGETYPENEXTFRAME"));

               rwsprintf(str, RWSTRING("  anim %d"), pMessage->index);
               RwDebugSendMessage(rwDEBUGMESSAGE, RWSTRING("Message :"),
                              str);

               rwsprintf(str, RWSTRING("  param1 %d"), pMessage->intParam1);
               RwDebugSendMessage(rwDEBUGMESSAGE, RWSTRING("Message :"),
                              str);
               rwsprintf(str, RWSTRING("  param2 %d"), pMessage->intParam2);
               RwDebugSendMessage(rwDEBUGMESSAGE, RWSTRING("Message :"),
                              str);
            }
            break;
            case rt2dMESSAGETYPEPREVFRAME:
            {
               RwDebugSendMessage(rwDEBUGMESSAGE, RWSTRING("Message :"),
                              RWSTRING("rt2dMESSAGETYPEPREVFRAME"));

               rwsprintf(str, RWSTRING("  anim %d"), pMessage->index);
               RwDebugSendMessage(rwDEBUGMESSAGE, RWSTRING("Message :"),
                              str);

               rwsprintf(str, RWSTRING("  param1 %d"), pMessage->intParam1);
               RwDebugSendMessage(rwDEBUGMESSAGE, RWSTRING("Message :"),
                              str);
               rwsprintf(str, RWSTRING("  param2 %d"), pMessage->intParam2);
               RwDebugSendMessage(rwDEBUGMESSAGE, RWSTRING("Message :"),
                              str);
            }
            break;
            case rt2dMESSAGETYPEGOTOFRAME:
            {
               RwDebugSendMessage(rwDEBUGMESSAGE, RWSTRING("Message :"),
                              RWSTRING("rt2dMESSAGETYPEGOTOFRAME"));

               rwsprintf(str, RWSTRING("  anim %d"), pMessage->index);
               RwDebugSendMessage(rwDEBUGMESSAGE, RWSTRING("Message :"),
                              str);

               rwsprintf(str, RWSTRING("  frame %d"), pMessage->intParam1);
               RwDebugSendMessage(rwDEBUGMESSAGE, RWSTRING("Message :"),
                              str);
            }
            break;
            case rt2dMESSAGETYPEGOTOLABEL:
            {
               Rt2dStringLabel *stringLabel;
               const RwChar *labelName;

               RwDebugSendMessage(rwDEBUGMESSAGE, RWSTRING("Message :"),
                              RWSTRING("rt2dMESSAGETYPEGOTOLABEL"));

               rwsprintf(str, RWSTRING("  anim %d"), pMessage->index);
               RwDebugSendMessage(rwDEBUGMESSAGE, RWSTRING("Message :"),
                              str);
               stringLabel = Rt2dMaestroGetStringLabelByIndex(pMaestro, 
                              static_cast<RwInt32>(pMessage->intParam1));

               labelName = Rt2dMaestroGetStringLabelName(
                              pMaestro,
                              stringLabel);

               rwsprintf(str, RWSTRING("  frame name %s"), labelName);
               RwDebugSendMessage(rwDEBUGMESSAGE, RWSTRING("Message :"),
                              str);

            }
            break;
            case rt2dMESSAGETYPEGETURL:
            {
               RwDebugSendMessage(rwDEBUGMESSAGE, RWSTRING("Message :"),
                              RWSTRING("rt2dMESSAGETYPEGETURL"));

               rwsprintf(str, RWSTRING("  anim %d"), pMessage->index);
               RwDebugSendMessage(rwDEBUGMESSAGE, RWSTRING("Message :"),
                              str);

               rwsprintf(str, RWSTRING("  param1 %d"), pMessage->intParam1);
               RwDebugSendMessage(rwDEBUGMESSAGE, RWSTRING("Message :"),
                              str);
               rwsprintf(str, RWSTRING("  param2 %d"), pMessage->intParam2);
               RwDebugSendMessage(rwDEBUGMESSAGE, RWSTRING("Message :"),
                              str);
            }
            break;
            case rt2dMESSAGETYPEDOACTION:
            {
               RwDebugSendMessage(rwDEBUGMESSAGE, RWSTRING("Message :"),
                              RWSTRING("rt2dMESSAGETYPEDOACTION"));

               rwsprintf(str, RWSTRING("  anim %d"), pMessage->index);
               RwDebugSendMessage(rwDEBUGMESSAGE, RWSTRING("Message :"),
                              str);

               rwsprintf(str, RWSTRING("  param1 %d"), pMessage->intParam1);
               RwDebugSendMessage(rwDEBUGMESSAGE, RWSTRING("Message :"),
                              str);
               rwsprintf(str, RWSTRING("  param2 %d"), pMessage->intParam2);
               RwDebugSendMessage(rwDEBUGMESSAGE, RWSTRING("Message :"),
                              str);
            }
            break;
            case rt2dMESSAGETYPEFOREIGN:
            {
               RwDebugSendMessage(rwDEBUGMESSAGE, RWSTRING("Message :"),
                              RWSTRING("rt2dMESSAGETYPEFOREIGN"));

               rwsprintf(str, RWSTRING("  anim %d"), pMessage->index);
               RwDebugSendMessage(rwDEBUGMESSAGE, RWSTRING("Message :"),
                              str);

               rwsprintf(str, RWSTRING("  param1 %d"), pMessage->intParam1);
               RwDebugSendMessage(rwDEBUGMESSAGE, RWSTRING("Message :"),
                              str);
               rwsprintf(str, RWSTRING("  param2 %d"), pMessage->intParam2);
               RwDebugSendMessage(rwDEBUGMESSAGE, RWSTRING("Message :"),
                              str);
            }
            break;
            case rt2dMESSAGETYPEMOUSEMOVETO:
            {
            #ifdef DUMP_MOUSE_MESSAGE
               RwDebugSendMessage(rwDEBUGMESSAGE, RWSTRING("Message :"),
                              RWSTRING("rt2dMESSAGETYPEMOUSEMOVETO"));

               rwsprintf(str, RWSTRING("  anim %d"),pMessage->index);
               RwDebugSendMessage(rwDEBUGMESSAGE, RWSTRING("Message :"),
                              str);

               rwsprintf(str, RWSTRING("  Move To %d,%d"),pMessage->intParam1,pMessage->intParam2);
               RwDebugSendMessage(rwDEBUGMESSAGE, RWSTRING("Message :"),
                              str);
            #endif
            }
            break;
            case rt2dMESSAGETYPEMOUSEBUTTONSTATE:
            {
            #ifdef DUMP_MOUSE_MESSAGE
               RwDebugSendMessage(rwDEBUGMESSAGE, RWSTRING("Message :"),
                              RWSTRING("rt2dMESSAGETYPEMOUSEBUTTONSTATE"));

               rwsprintf(str, RWSTRING("  anim %d"), pMessage->index);
               RwDebugSendMessage(rwDEBUGMESSAGE, RWSTRING("Message :"),
                              str);

               if (pMessage->intParam1 == static_cast<RwInt32>(TRUE))
               {
                   rwsprintf(str, RWSTRING("  Button Down"));
               }
               else
               {
                   rwsprintf(str, RWSTRING("  Button Up"));
               }
               RwDebugSendMessage(rwDEBUGMESSAGE, RWSTRING("Message :"),str);
            #endif
            }
            break;
            case rt2dMESSAGETYPESPECIALTELLTARGET:
            {
               RwDebugSendMessage(rwDEBUGMESSAGE, RWSTRING("Message :"),
                              RWSTRING("rt2dMESSAGETYPESPECIALTELLTARGET"));

               rwsprintf(str, RWSTRING("  anim %d"), pMessage->index);
               RwDebugSendMessage(rwDEBUGMESSAGE, RWSTRING("Message :"),
                              str);

               rwsprintf(str, RWSTRING("  param1 %d"), pMessage->intParam1);
               RwDebugSendMessage(rwDEBUGMESSAGE, RWSTRING("Message :"),
                              str);
               rwsprintf(str, RWSTRING("  param2 %d"), pMessage->intParam2);
               RwDebugSendMessage(rwDEBUGMESSAGE, RWSTRING("Message :"),
                              str);
            }
            break;
            case rt2dMESSAGETYPENULL:
               RwDebugSendMessage(rwDEBUGMESSAGE, RWSTRING("Message :"),
                              RWSTRING("rt2dMESSAGETYPENULL"));
            default:
            {
               RwDebugSendMessage(rwDEBUGMESSAGE, RWSTRING("Message :"),
                              RWSTRING("Unknown pMessage type"));

               rwsprintf(str, RWSTRING("  anim %d"), pMessage->index);
               RwDebugSendMessage(rwDEBUGMESSAGE, RWSTRING("Message :"),
                              str);

               rwsprintf(str, RWSTRING("  param1 %d"), pMessage->intParam1);
               RwDebugSendMessage(rwDEBUGMESSAGE, RWSTRING("Message :"),
                              str);
               rwsprintf(str, RWSTRING("  param2 %d"), pMessage->intParam2);
               RwDebugSendMessage(rwDEBUGMESSAGE, RWSTRING("Message :"),
                              str);
            }
            break;
         }

         RWS_RETURNVOID();
      }
#endif
            
      /**
      *
      *
      *  \ref MessageHandler
      *
      *  Custom handler for sending messages to Maestro
      *
      *  \param pMaestro
      *  \param pMessage
      *
      *  \return A pointer to the resultant message.
      */
      Rt2dMessage* MessageHandler(Rt2dMaestro *pMaestro, Rt2dMessage *pMessage)
      {
         RWS_FUNCTION("RWS::Maestro::MessageHandler");

         Rt2dMessage *pResult = 0;

         // Check to make sure the index is valid
         if (pMessage->index >= 0)
         {

#ifdef DUMP_MAESTRO_MESSAGE
            MessageDumper(pMaestro, pMessage);
#endif

            if (pMessage->messageType == rt2dMESSAGETYPEBUTTONBYLABEL)
            {
               // This should prevent a stall in the default message handler
               // which may occur if you try to post a button transition
               // to a button which doesn't exist

               pResult = pMessage;
               Rt2dMessageHandlerDefaultCallBack(pMaestro, pMessage);
            }
            else
            {
               pResult = Rt2dMessageHandlerDefaultCallBack(pMaestro, pMessage);
            }
         }

         RWS_RETURN(pResult);
      }


      /**
      *
      *
      *  \ref Controller_Action triggers a button transition from the behavior.
      *
      *  \param pMsgName  Name use to determine which button transition to send.
      *
      *  \return TRUE if successful, FALSE otherwise.
      */
      RwBool iCMaestro::Controller_Action(const char *pMsgName)
      {
         RWS_FUNCTION("RWS::Maestro::iCMaestro::Controller_Action");

         RwInt32 iParam1 = 0;

         if (!m_pMaestro)
         {
            RWS_RETURN(FALSE);
         }

         m_iController_State = 0;

         if (GetButtonLabelIndexFromEventName(pMsgName, &iParam1))
         {
            Rt2dMaestroForAllVisibleAnimations(m_pMaestro, AllAnimActivateButton, (void*)iParam1);
         }

         RWS_RETURN(TRUE);
      }


      /**
      *
      *
      *  \ref Stop stops Maestro playback on current animation.
      *
      *  \return TRUE if successful, FALSE otherwise.
      */
      RwBool iCMaestro::Stop(void)
      {
         RWS_FUNCTION("RWS::Maestro::iCMaestro::Stop");

         if (!m_pMaestro)
         {
            RWS_RETURN(FALSE);
         }

         Rt2dMaestroForAllVisibleAnimations(m_pMaestro, AllAnimStop, 0);

         RWS_RETURN(TRUE);
      }


      /**
      *
      *
      *  \ref Play starts playing the current Maestro animation.
      *
      *  \return TRUE if successful, FALSE otherwise.
      */
      RwBool iCMaestro::Play(void)
      {
         RWS_FUNCTION("RWS::Maestro::iCMaestro::Play");

         if (!m_pMaestro)
         {
            RWS_RETURN(FALSE);
         }

         Rt2dMaestroForAllVisibleAnimations(m_pMaestro, AllAnimPlay, 0);

         RWS_RETURN(TRUE);

      }


      /**
      *
      *
      *  \ref Restart restarts the Maestro animation from the beginning.
      *
      *  \return TRUE if successful, FALSE otherwise.
      */
      RwBool iCMaestro::Restart(void)
      {
         RWS_FUNCTION("RWS::Maestro::iCMaestro::Restart");

         RwChar cRootAnimName[] = RWSTRING("/");
         RwInt32 iRootAnimLabel;
         RwInt32 iParam1 = 0;

         if (!m_pMaestro)
         {
            RWS_RETURN(FALSE);
         }

         if (0 == Rt2dMaestroFindStringLabel(m_pMaestro,
                                       rt2dANIMLABELTYPEANIM,
                                       cRootAnimName,
                                       &iRootAnimLabel))
         {
            RWS_RETURN(FALSE);
         }

         iParam1 = iRootAnimLabel;

         Rt2dMaestroForAllVisibleAnimations(m_pMaestro, AllAnimRestart, (void*)iParam1);

         RWS_RETURN(TRUE);
      }


      /**
      *
      *
      *  \ref MaestroGoToFrame jumps to the iFrame frame of current animation.
      *
      *  \param iFrame  Frame to jump to in the current animation.
      *
      *  \return TRUE if successful, FALSE otherwise.
      */
      RwBool iCMaestro::GoToFrame(RwInt32 iFrame)
      {
         RWS_FUNCTION("RWS::Maestro::iCMaestro::GoToFrame");

         RwInt32 iParam1 = 0;

         if (!m_pMaestro)
         {
            RWS_RETURN(FALSE);
         }

         iParam1 = iFrame;

         Rt2dMaestroForAllVisibleAnimations(m_pMaestro, AllAnimGoToFrame, (void*)iParam1);

         RWS_RETURN(TRUE);
      }


      /**
      *
      *
      *  \ref SetCurrentAnim sets the current animation which can be used to view different
      *  parts of a Maestro animation object.
      *
      *  \param iAnim  Animation index to specify which animation to use.
      *
      *  \return TRUE if successful, FALSE otherwise.
      */
      RwBool iCMaestro::SetCurrentAnim(RwInt32 iAnim)
      {
         RWS_FUNCTION("RWS::Maestro::iCMaestro::SetCurrentAnim");

         if (!m_pMaestro)
         {
            RWS_RETURN(FALSE);
         }

         if (iAnim >= g_iNum_Anims)
         {
            RWS_RETURN(FALSE);
         }

         g_iCurrentAnimation = iAnim;

         RWS_RETURN(TRUE);
      }


      /**
      *
      *
      *  \ref SetInterpolate sets interpolation on or off for all
      *  animations using m_bInterpolate.
      *
      *  \return TRUE if successful, FALSE otherwise.
      */
      RwBool iCMaestro::SetInterpolate(void)
      {
         RWS_FUNCTION("RWS::Maestro::iCMaestro::SetInterpolate");

         if (!m_pMaestro)
         {
            RWS_RETURN(FALSE);
         }

         Rt2dMaestroForAllAnimations(m_pMaestro, AllAnimSetInterpolate, (void*)m_bInterpolate);

         RWS_RETURN(TRUE);
      }


      /**
      *
      *
      *  \ref Interpolate
      *
      *  \param bInterp  If TRUE, turns on interpolation on all Maestro animations.
      *
      *  \return TRUE if successful, FALSE otherwise.
      */
      RwBool iCMaestro::Interpolate(RwBool bInterp)
      {
         RWS_FUNCTION("RWS::Maestro::iCMaestro::Interpolate");

         if (!m_pMaestro)
         {
            RWS_RETURN(FALSE);
         }

         if ((bInterp == (RwBool)TRUE))
         {
            m_bInterpolate = TRUE;
         }
         else
         {
            m_bInterpolate = FALSE;
         }

         RWS_RETURN(SetInterpolate());
      }


      /**
      *
      *
      *  \ref ShowBoundingBox
      *
      *  \param bShowBBox  If TRUE, shows the bounding box of the entire Maestro Scene.
      *
      */
      void iCMaestro::ShowBoundingBox(RwBool bShowBBox)
      {
         RWS_FUNCTION("RWS::Maestro::iCMaestro::ShowBoundingBox");

         if (bShowBBox)
            m_bShowBBox = TRUE;
         else
            m_bShowBBox = FALSE;

         RWS_RETURNVOID();
      }


      /**
      *
      *
      *  \ref Clean
      *
      *  Used to reset parameters and destroy any objects used for Maestro playback.
      *
      */
      void iCMaestro::Clean(void)
      {
         RWS_FUNCTION("RWS::Maestro::iCMaestro::Clean");

         if (m_pMaestro)
         {
            if (m_pScene)
            {
               Rt2dObjectMTMSetIdentity(m_pScene);
            }

            // Called from CRenderwareResource::UnLoad
            //Rt2dMaestroDestroy(m_pMaestro);

            m_pMaestro = 0;
            m_pScene = 0;

            Rt2dShapeDestroy(m_pBBox);
            m_pBBox = 0;

            g_iNum_Anims = 0;

            if (m_pMatrix)
            {
               RwMatrixDestroy(m_pMatrix);
            }

            if (m_pBBoxMatrix)
            {
               RwMatrixDestroy(m_pBBoxMatrix);
            }
         }

          RWS_RETURNVOID();
      }


      /**
      *
      *
      *  \ref AddMaestro creates and adds a new Maestro object,
      *  starts playback automatically.
      *
      * \param pObject  New Maestro object to set up.
      *
      */
      void iCMaestro::Add(const void *pObject)
      {
         RWS_FUNCTION("RWS::Maestro::iCMaestro::Add");

         const Rt2dMaestro *pMaestro = 0;

         if (pObject)
         {
            // We found some Maestro data - assume it's compatible with any existing target
            pMaestro = static_cast<const Rt2dMaestro*>(pObject);
         }

         if (0 != pMaestro)
         {
            Rt2dBBox *pBBox;            
            Rt2dPath *pPath;
            Rt2dBrush *pStrokeBrush;
            RwRGBA Red = {255, 0, 0, 255}; // 0: Red

            m_pMaestro = (Rt2dMaestro *) pMaestro;
            m_pScene = Rt2dMaestroGetScene(m_pMaestro);

            // bounding box of all objects in scene
            pBBox = Rt2dMaestroGetBBox(m_pMaestro);
            m_pBBox = Rt2dShapeCreate();
            pPath = Rt2dPathCreate();
            Rt2dPathRect(pPath, pBBox->x, pBBox->y, pBBox->w, pBBox->h);
            Rt2dPathUnlock(pPath);

            // brush
            pStrokeBrush = Rt2dBrushCreate();
            Rt2dBrushSetRGBA(pStrokeBrush, &Red, &Red, &Red, &Red);
            Rt2dBrushSetWidth(pStrokeBrush, 2.000f);

            Rt2dShapeAddNode(m_pBBox, 0, pPath, pStrokeBrush);
            Rt2dBrushDestroy(pStrokeBrush);
            Rt2dObjectSetDepth(m_pBBox, 1);

            // Message handler for passing various messages to Maestro
            Rt2dMaestroSetMessageHandler(m_pMaestro, MessageHandler);

            // Total up number of anims contained within Maestro Scene
            Rt2dMaestroForAllAnimations(m_pMaestro, AllAnimCountAnims, 0);

            m_pCamera = MainLoop::Render::GetCamera();

            Rt2dDeviceSetCamera(m_pCamera);

            Calculate_Button_IDs();

            // Set initial transformations and scale
            InitState();

            Play();
         }

         RWS_RETURNVOID();
      }


      /**
      *
      *
      *  \ref InitState initializes the position, and scale of Maestro object
      *  and bounding boc.
      *
      */
      void iCMaestro::InitState(void)
      {
         RWS_FUNCTION("RWS::Maestro::iCMaestro::InitState");

         if (m_pMaestro)
         {
            Rt2dBBox *pBBox;
            RwReal rScaleH, rScaleW, rScale;
            RwReal rScaledWinHeight, rScaledWinWidth;
            RwV2d vXStep, vYStep, vOrigin;

            Rt2dDeviceGetStep(&vXStep, &vYStep, &vOrigin);

            // If the scene has been setup already, reset the
            // MTM to identity for scale to be applied later.
            if (m_pScene)
            {
               Rt2dObjectMTMSetIdentity(m_pScene);
            }

            // bounding box of all objects in scene
            pBBox = Rt2dMaestroGetBBox(m_pMaestro);

            // Get the height and width from the current camera
            RwCamera *pCamera = MainLoop::Render::GetCamera();
            RwRaster *pRaster = RwCameraGetRaster(pCamera);

            m_iWinWidth = RwRasterGetWidth(pRaster);
            m_iWinHeight = RwRasterGetHeight(pRaster);

            rScaledWinHeight = m_iWinHeight*vYStep.y + m_iWinHeight*vYStep.x;
            rScaledWinWidth = m_iWinWidth*vXStep.x + m_iWinWidth*vXStep.y;

            rScaleH = static_cast<RwReal>(rScaledWinHeight) / (pBBox->h);
            rScaleW = static_cast<RwReal>(rScaledWinWidth) / (pBBox->w);

            rScale = RwRealMin2(rScaleH, rScaleW);

            // Scale and position scene
            Rt2dObjectMTMScale(m_pScene, rScale, rScale);
            Rt2dObjectMTMTranslate(m_pScene,
                                  (m_iWinWidth * vXStep.x / rScale - pBBox->w) / 2.0f,
                                  pBBox->h + (m_iWinHeight * vYStep.y / rScale - pBBox->h) / 2.0f);

            // Scale and position bounding box
            Rt2dObjectMTMScale(m_pBBox, rScale, rScale);
            Rt2dObjectMTMTranslate(m_pBBox,
                              (m_iWinWidth * vXStep.x / rScale - pBBox->w) / 2.0f,
                              (m_iWinHeight * vYStep.y / rScale - pBBox->h) / 2.0f);

            Rt2dSceneUpdateLTM(m_pScene);

            
            // Only create the 2 matrices if they have not already been created
            if (!m_pMatrix)
            {
               m_pMatrix = RwMatrixCreate();
               m_pBBoxMatrix = RwMatrixCreate();
            }

            // Reset the view
            m_bViewChanged = TRUE;
         
            RwMatrixCopy(m_pMatrix, Rt2dObjectGetMTM(m_pScene));
            RwMatrixCopy(m_pBBoxMatrix, Rt2dObjectGetMTM(m_pBBox));

            Rt2dCTMSetIdentity();

         }

         RWS_RETURNVOID();
      }


      /**
      *
      *
      *  \ref Calculate_Button_IDs
      *
      *  Determines the labels for the input buttons.\n
      *  btnUp - label in Maestro file for the button which moves you up to the previous menu item.\n
      *  btnDown - label in Maestro file for the button which moves you down to the next menu item.\n
      *  btnSelect - label in Maestro file for the button which moves you to the next menu animation.\n
      *  btnCancel - label in Maestro file for the button which moves you to the previous menu animation.
      *
      */
      void iCMaestro::Calculate_Button_IDs(void)
      {
         RWS_FUNCTION("RWS::Maestro::iCMaestro::TraverseSceneAndCheckForStringObject");

         Rt2dMaestroFindStringLabel(m_pMaestro, rt2dANIMLABELTYPEBUTTON, "btnUp", &m_iButUp);
         Rt2dMaestroFindStringLabel(m_pMaestro, rt2dANIMLABELTYPEBUTTON, "btnDown", &m_iButDown);
         Rt2dMaestroFindStringLabel(m_pMaestro, rt2dANIMLABELTYPEBUTTON, "btnLeft", &m_iButLeft);
         Rt2dMaestroFindStringLabel(m_pMaestro, rt2dANIMLABELTYPEBUTTON, "btnRight", &m_iButRight);
         Rt2dMaestroFindStringLabel(m_pMaestro, rt2dANIMLABELTYPEBUTTON, "btnSelect", &m_iButSelect);
         Rt2dMaestroFindStringLabel(m_pMaestro, rt2dANIMLABELTYPEBUTTON, "btnCancel", &m_iButCancel);

         RWS_RETURNVOID();
      }


      /**
      *
      *
      *  \ref AddResource
      *
      *  A Maestro resource has been added.
      *
      *  \param pResourceID  Pointer to determine which resource to add.
      *
      */
      void iCMaestro::AddResource(const RWSGUID *pResourceId)
      {
         RWS_FUNCTION("RWS::Maestro::iCMaestro::AddResource");

         // Look up the object in the global store
         const RwChar *ResourceType;
         const RwChar *ResourceName;
         const void *pObject = RWS::CResourceManager::FindById (pResourceId, &ResourceType, 0, &ResourceName);

         if (pObject)
         {
            if (!rwstrcmp ("rwID_2DMAESTRO", ResourceType))
            {
               Add(pObject);
            }
         }

         RWS_RETURNVOID();
      }


      /**
      *
      *
      *  \ref SetPositionX
      *
      *  Changes the offset for the Maestro object in the x-axis.
      *
      * \param rPos  Amount to move Maestro object by.
      *
      */
      void iCMaestro::SetPositionX( RwReal rPos )
      {
         RWS_FUNCTION("RWS::Maestro::iCMaestro::SetPositionX");

         if (!m_pMaestro)
         {
            RWS_RETURNVOID();
         }

         if (rPos > m_MaxPos.x)
         {
            // Don't move further cause max pos encountered
            RWS_RETURNVOID();
         }

         m_vPosition.x = rPos;

         m_bViewChanged = TRUE;
         
         RWS_RETURNVOID();
      }


      /**
      *
      *
      *  \ref SetPositionY
      *
      *  Changes the offset for the Maestro object in the y-axis.
      *
      *  \param rPos  Amount to move Maestro object by.
      *
      */
      void iCMaestro::SetPositionY( RwReal rPos )
      {
         RWS_FUNCTION("RWS::Maestro::iCMaestro::SetPositionY");

         if (!m_pMaestro)
         {
            RWS_RETURNVOID();
         }

         if (rPos > m_MaxPos.y)
         {
            // Don't move further cause max pos encountered
            RWS_RETURNVOID();
         }

         m_vPosition.y = rPos;

         m_bViewChanged = TRUE;
         
         RWS_RETURNVOID();
      }


      /**
      *
      *
      *  \ref SetPositionZ
      *
      *  Changes the offset for the Maestro object in the z-axis, scales the Maestro object.
      *
      *  \param rScale  Amount to scale Maestro object by.
      *
      */
      void iCMaestro::SetPositionZ( RwReal rScale )
      {
         RWS_FUNCTION("RWS::Maestro::iCMaestro::SetPositionZ");

         if (!m_pMaestro)
         {
            RWS_RETURNVOID();
         }

         if (rScale > m_MaxScale)
         {
            // Don't move further cause max pos encountered
            RWS_RETURNVOID();
         }

         RwReal d_scale = 0.0f;

         if (rScale > 0)
         {
            // Scale up
            d_scale = (1.0f) + (rScale);
         }
         else
         {
            // Scale down
            d_scale = (1.0f) / (1.0f) + (rScale);            
         }

         m_rScale = d_scale;

         m_bViewChanged = TRUE;
         
         RWS_RETURNVOID();
      }


      /**
      *
      *
      *  \ref GetButtonLabelIndexFromEventName sets the index for the button.
      *
      *  \param pMsgName  String containing pointer to the name of the message.
      *  \param pButIndex  Pointer to index to store button index if button is valid.
      *
      *  \return TRUE if the event name matches a valid button, FALSE otherwise.
      */
      RwBool iCMaestro::GetButtonLabelIndexFromEventName(const char *pMsgName, RwInt32 *pButIndex)
      {
         RWS_FUNCTION("RWS::Maestro::iCMaestro::GetButtonLabelIndexFromEventName");

         RwBool ret = FALSE;

         if (rwstrcmp(pMsgName, RWSTRING("INQ_MENU_BTN_UP")) == 0 &&
            (m_iController_State & eController_Up) == 0)
         {
            // Up button activated

            m_iController_State |= eController_Up;

            *pButIndex = m_iButUp;
         }
         else if (rwstrcmp(pMsgName, RWSTRING("INQ_MENU_BTN_DOWN")) == 0 &&
             (m_iController_State & eController_Down) == 0)
         {
            // Down button activated

            m_iController_State |= eController_Down;

            *pButIndex = m_iButDown;
         }
         else if (rwstrcmp(pMsgName, RWSTRING("INQ_MENU_BTN_LEFT")) == 0 &&
             (m_iController_State & eController_Left) == 0)
         {
            // Down button activated

            m_iController_State |= eController_Left;

            *pButIndex = m_iButLeft;
         }
         else if (rwstrcmp(pMsgName, RWSTRING("INQ_MENU_BTN_RIGHT")) == 0 &&
             (m_iController_State & eController_Right) == 0)
         {
            // Down button activated

            m_iController_State |= eController_Right;

            *pButIndex = m_iButRight;
         }
         else if (rwstrcmp(pMsgName, RWSTRING("INQ_MENU_BTN_SELECT")) == 0 &&
             (m_iController_State & eController_Select) == 0)
         {
            // Select button activated

            m_iController_State |= eController_Select;

            *pButIndex = m_iButSelect;
         }
         else if (rwstrcmp(pMsgName, RWSTRING("INQ_MENU_BTN_CANCEL")) == 0 &&
             (m_iController_State & eController_Cancel) == 0)
         {
            // Cancel button activated

            m_iController_State |= eController_Cancel;

            *pButIndex = m_iButCancel;
         }

         if (*pButIndex)
         {
            ret = TRUE;
         }

         RWS_RETURN(ret);
      }


      /**
      *
      *
      *  \ref Controller_Input
      *
      *  Uses a message from the behavior to trigger a buttons transition
      *  depending on what input is given.
      *
      *  \param pMsg
      *  \param pMsgName
      *
      */
      void iCMaestro::Controller_Input(CMsg *pMsg, const char *pMsgName)
      {
         RWS_FUNCTION("RWS::Maestro::iCMaestro::Controller_Input");

         RwInt32 iParam1 = 0;

         if (!m_pMaestro)
         {
            RWS_RETURNVOID();
         }

         if (m_iController_State_Reset_Timer > CONTROLLER_STATE_RESET_TIME)
         {
            m_iController_State_Reset_Timer = 0;

            // Reset state
            m_iController_State = 0;
         }
         else
         {
            m_iController_State_Reset_Timer++;
         }

         RwReal input = *(reinterpret_cast<RwReal*>(&pMsg->pData));

         if (input > 0)
         {
            if (GetButtonLabelIndexFromEventName(pMsgName, &iParam1))
            {
               Rt2dMaestroForAllVisibleAnimations(m_pMaestro, AllAnimActivateButton, (void*)iParam1);
            }
         }

         RWS_RETURNVOID();
      }         


      /**
      *
      *
      *  \ref Update
      *
      *  Next tick for Maestro object.
      *
      *  \param rDeltaTime  Time elapsed since last update.
      *
      */
      void iCMaestro::Update(RwReal rDeltaTime)
      {
         RWS_FUNCTION("RWS::Maestro::iCMaestro::Update");

         if (!m_pMaestro)
         {
            RWS_RETURNVOID();
         }

         Rt2dDeviceSetCamera(m_pCamera);

         Rt2dMaestroAddDeltaTime(m_pMaestro,  rDeltaTime * m_rSpeed);
         Rt2dMaestroUpdateAnimations(m_pMaestro);

         Rt2dMaestroProcessMessages(m_pMaestro);

         if (m_bViewChanged)
         {
            // Reset Matrix
            Rt2dObjectSetMTM(m_pScene, m_pMatrix);
            Rt2dObjectSetMTM(m_pBBox, m_pBBoxMatrix);

            // Position
            Rt2dObjectMTMTranslate(m_pScene, m_vPosition.x, m_vPosition.y);
            Rt2dObjectMTMTranslate(m_pBBox, m_vPosition.x, m_vPosition.y);

            // Scale
            Rt2dObjectMTMScale(m_pScene, m_rScale, m_rScale);
            Rt2dObjectMTMScale(m_pBBox, m_rScale, m_rScale);
         }

         RWS_RETURNVOID();
      }


      /**
      *
      *
      *  \ref Render
      *
      *  Next render tick for Maestro object.
      *  This sets the camera to the one currently displayed from iMsgDoRender.
      *
      *  \param pMsg
      *
      */
      void iCMaestro::Render(CMsg *pMsg)
      {
         RWS_FUNCTION("RWS::Maestro::iCMaestro::Render");

         if (!m_pMaestro)
         {
            RWS_RETURNVOID();
         }

         RwCamera *m_pCamera = (reinterpret_cast<RwCamera*>(pMsg->pData));

         if (m_pCamera)
         {
            Rt2dDeviceSetCamera(m_pCamera);

            // Store current clipping planes
            RwReal rNearClip = RwCameraGetNearClipPlane(m_pCamera);
            RwReal rFarClip = RwCameraGetFarClipPlane(m_pCamera);

            // Set new clipping planes for Maestro rendering
            RwCameraSetNearClipPlane(m_pCamera, 0.1f);
            RwCameraSetFarClipPlane(m_pCamera, 500.0f);

            // Add camera to world
            RpWorldAddCamera (CLevel::GetOpaqueWorld(), m_pCamera);
            

            // Begin camera update
            if (RwCameraBeginUpdate(m_pCamera))
            {
               RwBool ZWrite;
               RwBool ZTest;
               RwShadeMode ShadeMode;
               RwTextureFilterMode TextureFilterMode;
               RwBool VertexAlpha;
               RwCullMode CullMode;

               RwRenderStateGet(rwRENDERSTATEZWRITEENABLE, (void *) &ZWrite);
               RwRenderStateGet(rwRENDERSTATEZTESTENABLE, (void *) &ZTest);
               RwRenderStateGet(rwRENDERSTATESHADEMODE, (void *) &ShadeMode);
               RwRenderStateGet(rwRENDERSTATETEXTUREFILTER, (void *) &TextureFilterMode);
               RwRenderStateGet(rwRENDERSTATEVERTEXALPHAENABLE, (void *) &VertexAlpha);
               RwRenderStateGet(rwRENDERSTATECULLMODE, (void*) &CullMode);

               RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *) FALSE);
               RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *) FALSE);
               RwRenderStateSet(rwRENDERSTATESHADEMODE, (void *) rwSHADEMODEGOURAUD);
               RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void *) rwFILTERLINEAR);
               RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *) TRUE);
               RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLNONE );

               Rt2dCTMPush();

               // As the CTM contains the view manipulations (zoom/translation)
               //  we need to force the pObject to recalculate the LTM
               //

               if (m_bViewChanged)
               {
                  Rt2dObjectMTMChanged(m_pScene);
                  Rt2dObjectMTMChanged(m_pBBox);
                  m_bViewChanged = FALSE;
               }

               Rt2dMaestroRender(m_pMaestro);

               // Render bounding box
               if (m_bShowBBox)
               {
                  Rt2dShapeRender(m_pBBox);
               }

               Rt2dCTMPop();

               // Restore the previous renderstates
               RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *) ZWrite);
               RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *) ZTest);
               RwRenderStateSet(rwRENDERSTATESHADEMODE, (void *) ShadeMode);
               RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void *) TextureFilterMode);
               RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *) VertexAlpha);
               RwRenderStateSet(rwRENDERSTATECULLMODE, (void*) CullMode);

               // End camera update
               RwCameraEndUpdate (m_pCamera);

            } //RwCameraBeginUpdate

            // Remove Camera from world
            RpWorldRemoveCamera (CLevel::GetOpaqueWorld(), m_pCamera);

            // Restore clipping planes just in case
            RwCameraSetNearClipPlane(m_pCamera, rNearClip);
            RwCameraSetFarClipPlane(m_pCamera, rFarClip);
         }

         RWS_RETURNVOID();
      }


      /**
      *
      *
      *  \ref GetCamera
      *
      *  Gets the camera used for the Maestro object.
      *
      *  \return A pointer to the camera used to render the Maestro object.
      *
      */
      RwCamera* iCMaestro::GetCamera(void)
      {
         RWS_FUNCTION("RWS::Maestro::iCMaestro::GetCamera");
         RWS_RETURN(m_pCamera);
      }


      /**
      *
      *
      *  \ref GetScene
      *
      *  Gets the top level scene for the Maestro object
      *
      *  \return A pointer to the scene of the Maestro object.
      *
      */
      Rt2dObject* iCMaestro::GetScene(void)
      {
         RWS_FUNCTION("RWS::Maestro::iCMaestro::GetScene");
         RWS_RETURN(m_pScene);
      }


      /**
      *
      *
      *  \ref HandleSystemCommands
      *
      *  \param attr  Reference to a CAttributePacket.
      *
      */
      void iCMaestro::HandleSystemCommands( const CAttributePacket& attr )
      {
         RWS_FUNCTION("RWS::Maestro::iCMaestro::HandleSystemCommands");

         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(CSystemCommands));
         
         while (!attrIt.IsFinished())
         {
            // Intercept CMD_AttachResource's only
            if (CMD_AttachResource == attrIt->GetCommandId())
            {
               const RWSGUID * pResourceId;
               attrIt->GetCommandData(&pResourceId);

               AddResource(pResourceId);
            }

            // Next section
            ++attrIt;
         }

         RWS_RETURNVOID();
      }

   } // namespace Maestro
} // namespace RWS
