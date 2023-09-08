/*****************************************************************************
*
* File :     CCameraSpline.cpp
*
* Abstract : Implementation of CCameraSpline class / behavior
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
* Copyright (c) 2003 Criterion Software Ltd.
* All Rights Reserved.
*
* RenderWare is a trademark of Canon Inc.
*
*****************************************************************************/
#include "precomp.h"
// Left to its own devices, the MW linker will dead-strip the registrar ctors, so...
#ifdef __MWERKS__
#pragma force_active on
#pragma exceptions off // Turn off exceptions due to bug in MW Dwarf Generator for PS2, present in CW 3.04
#endif

#include <rpspline.h>
#include <rtslerp.h>

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "ccameraspline.h"

#include "framework/core/macros/debugmacros.h"
#include "framework/core/resourcemanager/cresourcemanager.h"
#include "framework/toolkits/world/helpers/camerahelper.h"
#include "framework/toolkits/world/factory.h"
#include "framework/toolkits/math/maths.h"
#include "framework/toolkits/debugtools/debugtools.h"
#include "framework/core/netstream/nettools.h"

namespace RWS
{
   namespace Examples
   {
      namespace
      {
         const RwReal kFrameTime = (1.0f / RWS::MainLoop::Logic::Rate) * 1000.0f;
         const RwInt32 uiMinControlPoints = 4;
         const RwUInt32 uiOrthoLimit = 128;         
      }

      RWS_IMPLEMENT_CLASSID(CCameraSpline);
      RWS_REGISTERCLASS(CCameraSpline);

      /**
      *
      *  Construct a CCameraSpline object - initializes values and links to appropriate messages.
      *
      * \param attr the standard attribute packets used for setup.
      *
      */
      CCameraSpline::CCameraSpline(const CAttributePacket& attr) : 
         InitCEventHandler(0),
         m_pSpline(0),
         m_bDirectorsCam(FALSE),
         m_iCurrentControlPoint(0),
         m_rControlPointRatio(0.0f),
         m_rFrameTimeDelta(0.0f),
         m_rSpeed(0.0f),
         m_bMoving(TRUE),
         m_bLoop(TRUE),
         m_bThisCameraActive(TRUE),
         m_RenderInPriority(0)
      {
         RWS_FUNCTION("RWS::Examples::CCameraSpline::CCameraSpline");

         m_rViewWindow = static_cast<RwReal>(RwTan(RWDEG2RAD(45.0f * 0.5f)));

#ifdef RWS_DESIGN
         LinkMsg(iMsg_SetEnableDirectorsCamera, 0);
         LinkMsg(iMsg_SetDisableDirectorsCamera, 0);

         m_cameraFrame = RwFrameCreate();

         LinkMsg(iMsgRequestCurrentCameraMatrix, "RwUInt32 InstanceID");
#endif

         RWS_RETURNVOID();
      }

      /**
      *
      *  Destroy a CCameraSpline object and unlinks / unregisters messages.
      *
      */
      CCameraSpline::~CCameraSpline(void)
      {
         RWS_FUNCTION("RWS::Examples::CCameraSpline::~CCameraSpline");

         UnLinkMsg(m_RxEnableCameraEvent);
         UnLinkMsg(m_TxEnableCameraEvent);
         UnLinkMsg(m_RxDisableCameraEvent);
#ifdef RWS_DESIGN
         UnLinkMsg(iMsg_SetEnableDirectorsCamera);
         UnLinkMsg(iMsg_SetDisableDirectorsCamera);

         RwFrameDestroy(m_cameraFrame);

         UnLinkMsg(iMsgRequestCurrentCameraMatrix);
#endif
         UnLinkMsg(m_RenderInMsg);
         UnLinkMsg(m_RenderOutMsg);

         UnRegisterMsg(m_RxEnableCameraEvent);
         UnRegisterMsg(m_TxEnableCameraEvent);
         UnRegisterMsg(m_RxDisableCameraEvent);
         UnRegisterMsg(m_RenderInMsg);
         UnRegisterMsg(m_RenderOutMsg);

         RWS_RETURNVOID();
      }

      /**
      *
      *  Process m_RxEnableCameraEvent event, enables update of the rendering camera and sends
      *  m_TxEnableCameraEvent event.
      *
      */
      void CCameraSpline::Proc_m_RxEnableCameraEvent(void)
      {
         RWS_FUNCTION("RWS::Examples::CCameraSpline::Proc_m_RxEnableCameraEvent");

         SendMsg(m_TxEnableCameraEvent); 

         m_bThisCameraActive = TRUE;

         RWS_RETURNVOID();
      }

      /**
      *
      *  Process m_RxDisableCameraEvent event, disables updating the rendering camera..
      *
      */
      void CCameraSpline::Proc_m_RxDisableCameraEvent(void)
      {
         RWS_FUNCTION("RWS::Examples::CCameraSpline::Proc_m_RxDisableCameraEvent");

         m_bThisCameraActive = FALSE;

         RWS_RETURNVOID();
      }

#ifdef RWS_DESIGN
      /**
      *
      *  Process iMsg_SetEnableDirectorsCamera event
      *
      */
      void CCameraSpline::Proc_iMsg_SetEnableDirectorsCamera(void)
      {
         RWS_FUNCTION("RWS::Examples::CCameraSpline::Proc_iMsg_SetEnabledDirectorsCamera");

         m_bDirectorsCam = TRUE;

         RWS_RETURNVOID();
      }

      /**
      *
      *  Process iMsg_SetDisableDirectorsCamera event
      *
      */
      void CCameraSpline::Proc_iMsg_SetDisableDirectorsCamera(void)
      {
         RWS_FUNCTION("RWS::Examples::CCameraSpline::Proc_iMsg_SetDisableDirectorsCamera");

         m_bDirectorsCam = FALSE;

         RWS_RETURNVOID();
      }
#endif

      /**
      *
      *  Handle events for the CCameraSpline class.
      *
      *  \param pMsg standard message data object
      *
      */
      void CCameraSpline::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::Examples::CCameraSpline::HandleEvents");

         // Enable this camera
         //
         if (pMsg.Id == m_RxEnableCameraEvent)
         {
            Proc_m_RxEnableCameraEvent();
         }

         // Disable this camera
         //
         if (pMsg.Id == m_RxDisableCameraEvent)
         {
            Proc_m_RxDisableCameraEvent();
         }

#ifdef RWS_DESIGN
         // Enable the directors camera
         //
         if (pMsg.Id == iMsg_SetEnableDirectorsCamera)
         {
            Proc_iMsg_SetEnableDirectorsCamera();
         }
   
         // Disable the directors camera
         //
         if (pMsg.Id == iMsg_SetDisableDirectorsCamera)
         {
            Proc_iMsg_SetDisableDirectorsCamera();
         }
#endif

         // Rendering input message...
         //
         if (m_bThisCameraActive &&
             pMsg.Id == m_RenderInMsg &&
             pMsg.pData &&
             m_RenderOutMsg.Get_registered_count() &&
             m_pSpline)
         {
            RwCamera *pOutCam;

            // Copy camera data into local copy, so original attributes are kept, but
            // original is not overwritten in any way...

            pOutCam = RwCameraClone(static_cast<RwCamera *>(pMsg.pData));
            RWS_ASSERT(pOutCam, "Failed to clone incoming camera");

            RwMatrix *pOutMat = RwMatrixCreate();

            RwMatrixCopy(pOutMat, RwFrameGetMatrix(RwCameraGetFrame(pOutCam)));

            // Set the position of the camera to the position on the spline.

            RwInt32 rNumPoints = RpSplineGetNumControlPoints(m_pSpline);

            if (rNumPoints >= uiMinControlPoints)
            {
               if (m_bMoving)
               {
                  // Move the camera automatically
                  m_rFrameTimeDelta += (m_rSpeed / kFrameTime);
               }

               if (m_iCurrentControlPoint < rNumPoints)
               {
                  // We are somewhere on the spline between
                  // m_iCurrentControlPoint and m_iCurrentControlPoint + 1                     

                  RwReal rCurrentNodeRatio = m_iCurrentControlPoint * m_rControlPointRatio;

                  RwReal rSplinePos = rCurrentNodeRatio + 
                                      (m_rControlPointRatio * m_rFrameTimeDelta);

                  static const RwV3d vUp = {0.0f, 1.0f, 0.0f};

                  // Get the Frenet matrix to find the up, right and look-at vectors for the
                  // given point on the spline.
                  RpSplineFindMatrix(m_pSpline,
                                     rpSPLINEPATHSMOOTH,
                                     rSplinePos,
                                     const_cast<RwV3d *>(&vUp),
                                     pOutMat);

                  RwReal rNextNodeRatio = (m_iCurrentControlPoint + 1) * m_rControlPointRatio;

                  if (m_bMoving && rSplinePos >= rNextNodeRatio)
                  {
                     // Move to next node
                     m_iCurrentControlPoint++;

                     m_rFrameTimeDelta = 0;
                  }
               }
               else
               {
                  // At end of spline so go back to beginning
                  m_iCurrentControlPoint = 0;

                  m_rFrameTimeDelta = 0;

                  if (!m_bLoop)
                  {
                     m_bMoving = FALSE;
                  }
               }
            }

            m_uiOrthoCount++;

            if (m_uiOrthoCount >= uiOrthoLimit)
            {
               RwMatrixOrthoNormalize(pOutMat, pOutMat);

               m_uiOrthoCount = 0;
            }

            if (!m_bDirectorsCam)
            {
               RwMatrixCopy(RwFrameGetMatrix(RwCameraGetFrame(pOutCam)), pOutMat);
            }            

            RwMatrixDestroy(pOutMat);

            CameraHelper::SetFieldOfView(m_rViewWindow, pOutCam);

            // Then send the modified camera onward...
            CMsg msg(m_RenderOutMsg, pOutCam);
            SendMsg(msg);

#ifdef RWS_DESIGN
            RwMatrix *cameraMatrix = RwFrameGetLTM( RwCameraGetFrame(pOutCam) );
            RwFrameTransform(m_cameraFrame, cameraMatrix, rwCOMBINEREPLACE);
#endif

            // Tidy up, destroy the camera
            RwCameraDestroy(pOutCam);
         }

#ifdef RWS_DESIGN
         // Workspace has requested the current camera, send current player camera as possible camera
         //
         if (pMsg.Id == iMsgRequestCurrentCameraMatrix && m_respondtocamerarequest)
         {
            NetTools::SendTransform(m_cameraFrame);
         }
#endif

         RWS_RETURNVOID();
      }

      /**
      *
      *  A Spline resource has been added.
      *
      *  \param pResourceID  Pointer to determine which resource to add.
      *
      */
      void CCameraSpline::AddResource(const RWSGUID *pResourceID)
      {
         RWS_FUNCTION("RWS::Examples::CCameraSpline::AddResource");

         // Look up the object in the global store
         const RwChar *ResourceType;
         const RwChar *ResourceName;
         const void *pObject = RWS::CResourceManager::FindById (pResourceID, &ResourceType, 0, &ResourceName);

         if (pObject)
         {
            if (*static_cast<RwInt32 *>(const_cast<void *>(pObject)) == -1)
            {
               m_pSpline = 0;
            }
            else
            {
               if (!rwstrcmp ("rwID_SPLINE", ResourceType))
               {
                  // Store RenderWare Graphics spline data.
                  m_pSpline = static_cast<RpSpline*>(const_cast<void *>(pObject));

                  if (m_pSpline)
                  {
                     RwInt32 rNumPoints = RpSplineGetNumControlPoints(m_pSpline);

                     if (rNumPoints > uiMinControlPoints)
                     {
                        m_rControlPointRatio = 1 / (static_cast<RwReal>(rNumPoints - 1));
                     }
                     else
                     {
                        m_rControlPointRatio = 0;
                     }
                  }

                  m_iCurrentControlPoint = 0;
               }
            }
         }

         RWS_RETURNVOID();
      }

      /**
      *
      *  Intercepts CSystemCommands commands.
      *
      *  \param attr standard attribute data packet.
      *
      */
      void CCameraSpline::HandleSystemCommands(const CAttributePacket &attr)
      {
         RWS_FUNCTION("RWS::Examples::CCameraSpline::HandleSystemCommands");

         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(CSystemCommands));

         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
               case CMD_AttachResource:
                  const RWSGUID * pResourceId;
                  attrIt->GetCommandData(&pResourceId);

                  AddResource(pResourceId);
                  break;
            }

            ++attrIt;
         }

         RWS_RETURNVOID();
      }

      /**
      *
      *  Handle updating attributes for the CCameraSpline class.
      *
      *  \param attr standard attribute packets sent from the workspace / stream.
      *
      */
      void CCameraSpline::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::Examples::CCameraSpline::HandleAttributes");

         CAttributeHandler::HandleAttributes(attr);

         HandleSystemCommands(attr);
      
         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(CCameraSpline));
         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
                  // Set incoming event to enable the camera
               case CMD_Set_m_RxEnableCameraEvent:
                  ReplaceLinkedMsg(m_RxEnableCameraEvent, attrIt->GetAs_RwChar_ptr(), 0);
                  break;

                  // Set outgoing event to tell camera has been enabled
               case CMD_Set_m_TxEnableCameraEvent:
                  ReplaceRegisteredMsg(m_TxEnableCameraEvent, attrIt->GetAs_RwChar_ptr(), 0);
                  break;

                  // Set incoming event to disable the camera
               case CMD_Set_m_RxDisableCameraEvent:
                  ReplaceLinkedMsg(m_RxDisableCameraEvent, attrIt->GetAs_RwChar_ptr(), 0);
                  break;

                  // Set incoming render message.
               case CMD_Set_m_InRender:
                  ReplaceLinkedMsg(m_RenderInMsg, attrIt->GetAs_RwChar_ptr(), "RwCamera*");
                  break;

                  // Set priority of the rendering message.
               case CMD_priority:

                  attrIt->GetCommandData(m_RenderInPriority);
                  UnLinkMsg(m_RenderInMsg);
                  LinkMsg(m_RenderInMsg, "RwCamera*", m_RenderInPriority);
                  break;

                  // Set outgoing render message.
               case CMD_Set_m_OutRender:
                  ReplaceRegisteredMsg(m_RenderOutMsg, attrIt->GetAs_RwChar_ptr(), 0);
                  break;

                  // Set speed to move camera along the spline
               case CMD_Speed:
                  attrIt->GetCommandData(m_rSpeed);
                  break;

                  // Play/ pause the movement along the spline
               case CMD_Move_Camera:                  
                  if (attrIt->GetAs_RwUInt32())
                  {
                     m_bMoving = TRUE;
                  }
                  else
                  {
                     m_bMoving = FALSE;
                  }
                  break;

                  // Set camera movement looping on or off
               case CMD_Loop:
                  if (attrIt->GetAs_RwUInt32())
                  {
                     m_bLoop = TRUE;
                  }
                  else
                  {
                     m_bLoop = FALSE;
                  }
                  break;               

                  // Move to the next control point on the spline
               case CMD_Set_Next_Control_Point:
                  if (!m_bMoving && m_pSpline)
                  {
                     if (m_iCurrentControlPoint < RpSplineGetNumControlPoints(m_pSpline))
                     {
                        m_iCurrentControlPoint++;
                     }
                     else
                     {
                        m_iCurrentControlPoint = 0;
                     }
                  }
                  break;
#ifdef RWS_DESIGN
               case CMD_Set_m_respondtocamerarequest:
                  attrIt->GetCommandData(m_respondtocamerarequest);
                  break;
#endif
            }
            ++attrIt;
         }

         RWS_RETURNVOID();
      }
   }//namespace Examples
}//namespace RWS
