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
 * iCMaestro.h - Implementation of Maestro behavior
 *
 ****************************************************************************/


#ifndef __iCMaestro_H__
#define __iCMaestro_H__


//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/core/eventhandler/ceventhandler.h"
#include "framework/core/attributehandler/csystemcommands.h"
#include "framework/toolkits/world/smartptrs/catomicptr.h"

#include "framework/core/attributehandler/allocation policies/LinearAllocationPolicy.h"

namespace RWS
{
   // Fwd. decls
   class CAttributePacket;

   /**
   *
   */
   namespace Maestro
   {

      /**
      *
      *  \ingroup Mod_Maestro
      *
      *  Implementation of Maestro behavior class.
      *
      *  If the Maestro animation (.anm) file requires a font then add a font
      *  asset (.fnt) first so that the font resource gets loaded before the
      *  Maestro resource.  Also, if the Maestro animation uses textures then add
      *  the texture dictionary (.txd) before the animation too.
      *
      *  Use rwID_2DMAESTRO as the type id for the Maestro asset in RenderWare Studio.
      *
      */
      class iCMaestro : public CSystemCommands, public LinearAllocationPolicy
      {
      public:

         iCMaestro();
         ~iCMaestro();

         void HandleSystemCommands( const CAttributePacket& attr );
         void Controller_Input(CMsg *pMsg, const char *pMsgName);
         void Update(RwReal rDeltaTime);
         void Render(CMsg *pMsg);
         RwCamera *GetCamera(void);
         Rt2dObject *GetScene(void);

         RwBool Controller_Action(const char *pMsgName);
         RwBool Stop(void);
         RwBool Play(void);
         RwBool Show(void);
         RwBool Hide(void);
         RwBool Restart(void);
         RwBool Interpolate(RwBool bInterp);
         RwBool GoToFrame(RwInt32 iFrame);
         RwBool SetCurrentAnim(RwInt32 iAnim);
         void SetPositionX(RwReal rPos);
         void SetPositionY(RwReal rPos);
         void SetPositionZ(RwReal rScale);
         void ShowBoundingBox(RwBool bShowBBox);
         void InitState(void);

      private:

         RwBool GetButtonLabelIndexFromEventName(const char *pMsgName, RwInt32 *pButIndex);
         void Calculate_Button_IDs(void);
         void Add(const void *pObject);
         void AddResource(const RWSGUID * pResourceId);
         void Clean(void);
         RwBool SetInterpolate(void);

         // Revoked ops
         iCMaestro( const iCMaestro& );
         iCMaestro& operator = ( const iCMaestro& );

         RwMatrix *m_pMatrix;                      /**< Matrix to store original position of Maestro object*/
         RwMatrix *m_pBBoxMatrix;                  /**< Matrix to store original position of Maestro object's bounding box*/
         RwV2d m_vPosition;                        /**< Position of Maestro object on screen*/
         RwReal m_rScale;                          /**< Scale of Maestro object on screen*/
         RwV2d m_MaxPos;                           /**< Maximum offset position of Maestro object on screen*/
         RwReal m_MaxScale;                        /**< Maximum scale of Maestro object on screen*/
         RwInt32 m_iWinHeight;                     /**< Height of window*/
         RwInt32 m_iWinWidth;                      /**< Width of window*/
         RwBool m_bViewChanged;                    /**< Flag to say that view needs to be refreshed*/
         RwReal m_rSpeed;                          /**< Used to change the playback speed, 1.0f is default*/
         RwBool m_bInterpolate;                    /**< Flag to turn on/ off interpolation for entire scene*/
         RwBool m_bShowBBox;                       /**< Flag to show/ hide bounding box for Maestro object*/

         Rt2dMaestro *m_pMaestro;                  /**< Main Maestro object*/
         Rt2dObject *m_pScene;                     /**< Maestro scene object*/
         Rt2dObject *m_pBBox;                      /**< Maestro bounding box object*/
         RwCamera *m_pCamera;                      /**< Camera used to render Maestro object*/

         enum Controller_State                     
         {
            eController_Up=0x01,
            eController_Down=0x02,
            eController_Left=0x04,
            eController_Right=0x08,
            eController_Select=0x10,
            eController_Cancel=0x12
         };                                        /**< State enumeration for buttons*/

         RwUInt32 m_iController_State_Reset_Timer;  /**< Used to determine the delay for repeat input*/
         RwInt32 m_iController_State;              /**< Stores the current state of the input controller*/
         RwInt32 m_iButDown;                       /**< Stores id for down button label*/
         RwInt32 m_iButUp;                         /**< Stores id for up button label*/
         RwInt32 m_iButLeft;                       /**< Stores id for left button label*/
         RwInt32 m_iButRight;                      /**< Stores id for right button label*/
         RwInt32 m_iButSelect;                     /**< Stores id for select button label*/
         RwInt32 m_iButCancel;                     /**< Stores id for cancel button label*/
      };
   } //namespace Maestro
} //namespace RWS

#endif /* __iCMaestro_H__ */
