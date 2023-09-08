/*****************************************************************************
*
* File :     CCameralookatpoint.cpp
*
* Abstract : Implementation of CCameraLookAtPoint class / behavior
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
* Copyright (c) 2001 Criterion Software Ltd.
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

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "ccameralookatpoint.h"

#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/world/factory.h"
#include "framework/toolkits/world/helpers/camerahelper.h"
#include "framework/toolkits/world/helpers/atomichelper.h"
#include "framework/toolkits/debugtools/debugswitches.h"
#include "framework/toolkits/math/maths.h"
#include "framework/toolkits/debugtools/debugtools.h"
#include "framework/core/netstream/nettools.h"


namespace RWS
{
   namespace Examples
   {
      RWS_IMPLEMENT_CLASSID(CCameraLookAtPoint);
      RWS_REGISTERCLASS(CCameraLookAtPoint);

      /**
      *
      *  Construct a CCameraLookAtPoint object - initializes values and links to appropriate messages.
      *
      * \param attr the standard attribute packets used for setup.
      *
      */

      CCameraLookAtPoint::CCameraLookAtPoint(const CAttributePacket& attr) : 
         InitCEventHandler(&m_pAtomic),
         m_this_camera_active(true),
         m_distance(100.0f),
         m_max_distance(100.0f),
         m_min_distance(25.0f),
         m_pref_distance(75.0f),
         m_smooth_distance(0.1f),
         m_elevation(10.0f),
         m_max_elevation(10.0f),
         m_min_elevation(-10.0f),
         m_pref_elevation(5.0f),
         m_smooth_elevation(0.1f),
         m_angle(0.0f),
         m_lastAngle(0.0f),
         m_max_angle(180.0f),
         m_min_angle(-180.0f),
         m_pref_angle(180.0f),
         m_smooth_angle(0.025f)
#ifdef RWS_DESIGN
         , m_respondtocamerarequest(true)
#endif
      {
         RWS_FUNCTION("RWS::Examples::CCameraLookAtPoint::CCameraLookAtPoint");

         // Can't use initializer ('this' isn't valid) so...
         m_pAtomic = CreateAtomicInWorldFromResource(attr, this);
         RWS_ASSERT(m_pAtomic, "Failed to create atomic");
   
         m_ViewWindow = static_cast<RwReal>(RwTan(RWDEG2RAD(45.0f * 0.5f)));
   
         LinkMsg(iMsgRunningPostTick, 0);
         LinkMsg(iMsg_SetEnableDirectorsCamera, 0);
         LinkMsg(iMsg_SetDisableDirectorsCamera, 0);

#ifdef RWS_DESIGN
         m_cameraFrame = RwFrameCreate();
         LinkMsg(iMsgRequestCurrentCameraMatrix, "RwUInt32 InstanceID");
#endif

         RWS_RETURNVOID();
      }

      /**
      *
      *  Destroy a CCameraLookAtPoint object and unlinks / unregisters messages.
      *
      */

      CCameraLookAtPoint::~CCameraLookAtPoint()
      {
         RWS_FUNCTION("RWS::Examples::CCameraLookAtPoint::~CCameraLookAtPoint");

#ifdef RWS_DESIGN
         RwFrameDestroy(m_cameraFrame);
         UnLinkMsg(iMsgRequestCurrentCameraMatrix);
#endif

         UnLinkMsg(m_Msg_Rx_Pos);
         UnLinkMsg(m_RxEnableCameraEvent);
         UnLinkMsg(m_TxEnableCameraEvent);
         UnLinkMsg(m_RxDisableCameraEvent);
         UnLinkMsg(iMsgRunningPostTick);
         UnLinkMsg(iMsg_SetEnableDirectorsCamera);
         UnLinkMsg(iMsg_SetDisableDirectorsCamera);
         UnLinkMsg(m_renderInMsg);
         UnLinkMsg(m_renderOutMsg);

         UnRegisterMsg(m_Msg_Rx_Pos);
         UnRegisterMsg(m_RxEnableCameraEvent);
         UnRegisterMsg(m_TxEnableCameraEvent);
         UnRegisterMsg(m_RxDisableCameraEvent);
         UnRegisterMsg(m_renderInMsg);
         UnRegisterMsg(m_renderOutMsg);

         RWS_RETURNVOID();
      }

      /**
      *
      *  Process m_Msg_Rx_Pos message. That is receive a new target position and modify the camera
      *  position and orientation.
      *
      * \param pMsg the standard message structure.
      *
      */

      void CCameraLookAtPoint::Proc_m_Msg_Rx_Pos(const CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::Examples::CCameraLookAtPoint::Proc_m_Msg_Rx_Pos");

         RwFrame *frame = static_cast<RwFrame*>(pMsg.pData);

         if (frame) 
         {
            // We won't modify it, honest!
            //
            RwMatrix &msgmatrix = *RwFrameGetLTM(frame); // Use reference 
      
            RwMatrix rotmatrix = msgmatrix; // rot matrix
            RwMatrix irotmatrix; // Inverse rot matrix
            RwV3d v;
      
            // Assume our own entity has a frame
            //
            RwFrame *frame = RpAtomicGetFrame(m_pAtomic.ptr());
            RWS_ASSERT( frame, "Atomic has no frame" );
            const RwMatrix *matrix = RwFrameGetLTM(frame);
      
            // Remove the translation from the rot matrix
            //
            rotmatrix.pos.x = 0;
            rotmatrix.pos.y = 0;
            rotmatrix.pos.z = 0;
      
            RwMatrixInvert(&irotmatrix, &rotmatrix);

            RwV3dSubMacro(&v, &matrix->pos, &msgmatrix.pos);

            RwV3dTransformVectors(&v, &v, 1, &irotmatrix);  // Remove the target objects rotation
      
            rwSqrt(&m_distance, (v.x * v.x) + (v.z * v.z));        // Distance X/Z only

#if defined (RWS_DEBUGTOOLS) && defined (RWS_DESIGN)
            if (m_Flags & uATTRIBUTEHANDLER_FLAG_DEBUG)
            {
               DebugTools::Printf("m_distance += (m_pref_distance - m_distance) * m_smooth_distance;\n");

               DebugTools::Printf("%f = (%f - %f) * %f;\n",
                  (m_pref_distance - m_distance) * m_smooth_distance, m_pref_distance, m_distance,m_smooth_distance);

            }
#endif
            
            // Head towards preferred distance ?
            //
            m_distance += (m_pref_distance - m_distance) * m_smooth_distance;
      
#if defined (RWS_DEBUGTOOLS) && defined (RWS_DESIGN)
            if (m_Flags & uATTRIBUTEHANDLER_FLAG_DEBUG)
            {
               DebugTools::Printf("Head towards preferred distance %f\n", m_distance);
            }
#endif
            // Clamp min/max distance
            //
            if (m_distance>m_max_distance) m_distance = m_max_distance;
            if (m_distance<m_min_distance) m_distance = m_min_distance;

#if defined (RWS_DEBUGTOOLS) && defined (RWS_DESIGN)
            if (m_Flags & uATTRIBUTEHANDLER_FLAG_DEBUG)
            {
               DebugTools::Printf("Clamp min/max distance %f %f %f\n", m_min_distance, m_distance, m_max_distance);
            }
#endif
            
            m_elevation = v.y;
      
            // Head towards preferred elevation, may need to change this to be an angle diff not sure, as atan2 should
            // always return +-180
            //

            m_elevation += (m_pref_elevation - m_elevation) * m_smooth_elevation;
      
#if defined (RWS_DEBUGTOOLS) && defined (RWS_DESIGN)
            if (m_Flags & uATTRIBUTEHANDLER_FLAG_DEBUG)
            {
               DebugTools::Printf("Head towards preferred elevation %f\n", m_elevation);
            }
#endif

            // Clamp Elevation
            //
            if (m_elevation>m_max_elevation) m_elevation = m_max_elevation;
            if (m_elevation<m_min_elevation) m_elevation = m_min_elevation;

#if defined (RWS_DEBUGTOOLS) && defined (RWS_DESIGN)
            if (m_Flags & uATTRIBUTEHANDLER_FLAG_DEBUG)
            {
               DebugTools::Printf("Clamp min/max elevation %f %f %f\n", m_min_elevation, m_elevation, m_max_elevation);
            }
#endif
            
            m_angle =  RWRAD2DEG( static_cast<RwReal>( RwATan2 (v.x, v.z) ));
      
            // Head towards preferred angle
            //
            RwReal d_angle = DegAngDiffAndRange(m_angle, m_pref_angle, m_lastAngle);
      
            m_angle += d_angle * m_smooth_angle;
      
            // Clamp angle
            //
            if (m_angle>m_max_angle) m_angle = m_max_angle;
            if (m_angle<m_min_angle) m_angle = m_min_angle;

            m_lastAngle = m_angle;
      
            // Generate a position vector relative to the character, this is the desired place to put the camera
            //
            v.x = m_distance * static_cast<RwReal>(RwSin(RWDEG2RAD(m_angle)));
            v.y = m_elevation;
            v.z = m_distance * static_cast<RwReal>(RwCos(RWDEG2RAD(m_angle)));
      
            // Apply the target objects rotation 
            //
            RwV3dTransformVectors(&v, &v, 1, &rotmatrix);
      
            RwReal y_angle = 180.0f + RWRAD2DEG ( static_cast<RwReal>( RwATan2 (v.x, v.z) ));

            RwReal temp;

            rwSqrt (&temp, (v.x * v.x) + (v.z * v.z));

            RwReal x_angle = RWRAD2DEG( static_cast<RwReal> (RwATan2 (v.y, temp)));

            // Add back to original position, to create our new camera position
            //
            RwV3dAddMacro(&v, &v, &msgmatrix.pos);

            RwFrameRotate (frame, &XAxis, x_angle, rwCOMBINEREPLACE);   // Pitch
      
            RwFrameRotate (frame, &YAxis, y_angle, rwCOMBINEPOSTCONCAT);   // Heading
      
            RwFrameTranslate (frame, &v, rwCOMBINEPOSTCONCAT); // Position
         }

         RWS_RETURNVOID();
      }

      /**
      *
      *  Process iMsg_SetEnableDirectorsCamera event, makes the atomic/clump visible.
      *
      */

      void CCameraLookAtPoint::Proc_iMsg_SetEnableDirectorsCamera(void)
      {
         RWS_FUNCTION("RWS::Examples::CCameraLookAtPoint::Proc_iMsg_SetEnabledDirectorsCamera");

         // Unhide the camera atomic
         //
         AtomicHelper::SetIsVisible(*m_pAtomic, true);

         RWS_RETURNVOID();
      }

      /**
      *
      *  Process iMsg_SetDisableDirectorsCamera event, makes the atomic/clump invisible.
      *
      */

      void CCameraLookAtPoint::Proc_iMsg_SetDisableDirectorsCamera(void)
      {
         RWS_FUNCTION("RWS::Examples::CCameraLookAtPoint::Proc_iMsg_SetDisableDirectorsCamera");

         // Hide the camera atomic
         //
         AtomicHelper::SetIsVisible(*m_pAtomic, false);

         RWS_RETURNVOID();
      }

      /**
      *
      *  Process m_RxEnableCameraEvent event, enables update of the rendering camera and sends
      *  m_TxEnableCameraEvent event.
      *
      */

      void CCameraLookAtPoint::Proc_m_RxEnableCameraEvent(void)
      {
         RWS_FUNCTION("RWS::Examples::CCameraLookAtPoint::Proc_m_RxEnableCameraEvent");

         SendMsg(m_TxEnableCameraEvent); 

         m_this_camera_active = true;

         RWS_RETURNVOID();
      }

      /**
      *
      *  Process m_RxDisableCameraEvent event, disables updating the rendering camera..
      *
      */

      void CCameraLookAtPoint::Proc_m_RxDisableCameraEvent(void)
      {
         RWS_FUNCTION("RWS::Examples::CCameraLookAtPoint::Proc_m_RxDisableCameraEvent");

         m_this_camera_active = false;

         RWS_RETURNVOID();
      }

      /**
      *
      *  Handle events for the CCameraLookAtPoint class.
      *
      *  \see m_Msg_Rx_Pos
      *  \see Proc_iMsg_SetEnableDirectorsCamera
      *  \see Proc_iMsg_SetDisableDirectorsCamera
      *  \see m_RxEnableCameraEvent
      *  \see m_RxDisableCameraEvent
      *  \param pMsg standard message data object
      *
      */

      void CCameraLookAtPoint::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::Examples::CCameraLookAtPoint::HandleEvents");

         // Receive position from another entity
         //
         if (pMsg.Id == m_Msg_Rx_Pos && m_pAtomic) Proc_m_Msg_Rx_Pos(pMsg);
   
         // NOTE: user may use the same message for their events, so do not use else-if, because
         //       second, third, etc, user selectable messages will not be picked up.

         // Enable the directors camera
         //
         if (pMsg.Id == iMsg_SetEnableDirectorsCamera && m_pAtomic) Proc_iMsg_SetEnableDirectorsCamera();
   
         // Disable the directors camera
         //
         if (pMsg.Id == iMsg_SetDisableDirectorsCamera && m_pAtomic) Proc_iMsg_SetDisableDirectorsCamera();
   
         // Enable this camera
         //
         if (pMsg.Id == m_RxEnableCameraEvent) Proc_m_RxEnableCameraEvent();

         // Disable this camera
         //
         if (pMsg.Id == m_RxDisableCameraEvent) Proc_m_RxDisableCameraEvent();

         // Rendering input message...

         if (m_this_camera_active && pMsg.Id == m_renderInMsg && pMsg.pData && m_renderOutMsg.Get_registered_count())
         {
            RwCamera *pOutCam;
            RwFrame *pFrame;

            // Copy camera data into local copy, so original attributes are kept, but
            // original is not overwritten in any way...

            pOutCam = RwCameraClone(static_cast<RwCamera *>(pMsg.pData));
            RWS_ASSERT(pOutCam, "Failed to clone incoming camera");

            // Copy frame into temporary local one and attach (as will update matrix below).

            pFrame = RwFrameCreate();
            RwCameraSetFrame(pOutCam, pFrame);

            // Set the matrix inside this camera to that of the look-at-camera.

            RwMatrixCopy(RwFrameGetMatrix(RwCameraGetFrame(pOutCam)),
                  RwFrameGetMatrix(RpAtomicGetFrame(m_pAtomic.ptr())));

            CameraHelper::SetFieldOfView(m_ViewWindow, pOutCam);

            // Then send the modified camera onward...

            CMsg msg(m_renderOutMsg, pOutCam);
            SendMsg(msg);

#ifdef RWS_DESIGN
            RwMatrix *cameraMatrix = RwFrameGetLTM(pFrame);
            RwFrameTransform(m_cameraFrame, cameraMatrix, rwCOMBINEREPLACE);
#endif

            // Tidy up, destroy the camera - DOES NOT destroy frame (local) or z-buffer, etc, as these
            // are shared (via the clone) with the incoming camera.

            RwCameraSetFrame(pOutCam, 0);
            RwFrameDestroy(pFrame);
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
      *  Intercepts CSystemCommands commands in order to make the atomic/clump invisible if the
      *  directors camera is active.
      *
      *  \param attr standard attribute data packet.
      *
      */

      void CCameraLookAtPoint::HandleSystemCommands(const CAttributePacket &attr)
      {
         RWS_FUNCTION("RWS::Examples::CCameraLookAtPoint::HandleSystemCommands");

         // Entity gets first dibs
         m_pAtomic.HandleSystemCommands(attr);

         // Trap the entity creation command - we need to synchronize the entity's
         // visibility
         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(CSystemCommands));

         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
               case CMD_AttachResource:
                  // If the directors camera is active, this atomic/clump is invisible
                  //
   #ifdef RWS_DEBUGTOOLS
                  if (DebugSwitches::GetSwitch (DebugSwitches::m_enable_DirectorsCamera) == true)
                  {
                     AtomicHelper::SetIsVisible(*m_pAtomic, false);
                  }
                  else
                  {
                     AtomicHelper::SetIsVisible(*m_pAtomic, true);
                  }
   #endif               
                  break;
            }

            ++attrIt;
         }

         RWS_RETURNVOID();
      }

      /**
      *
      *  Handle updating attributes for the CCameraLookAtPoint class.
      *
      *  \param attr standard attribute packets sent from the workspace / stream.
      *
      */

      void CCameraLookAtPoint::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::Examples::CCameraLookAtPoint::HandleAttributes");

         CAttributeHandler::HandleAttributes(attr);

         HandleSystemCommands(attr);
      
         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(CCameraLookAtPoint));
         while (!attrIt.IsFinished())
         {
               switch (attrIt->GetCommandId())
               {
                  //////////////////////////////////////////////////////////////////
               case CMD_m_max_distance:
                  //////////////////////////////////////////////////////////////////
                  attrIt->GetCommandData(m_max_distance);
                  break;
                  //////////////////////////////////////////////////////////////////
               case CMD_m_pref_distance:
                  //////////////////////////////////////////////////////////////////
                  attrIt->GetCommandData(m_pref_distance);
                  break;
                  //////////////////////////////////////////////////////////////////
               case CMD_m_min_distance:
                  //////////////////////////////////////////////////////////////////
                  attrIt->GetCommandData(m_min_distance);
                  break;
                  //////////////////////////////////////////////////////////////////
               case CMD_m_smooth_distance:
                  //////////////////////////////////////////////////////////////////
                  attrIt->GetCommandData(m_smooth_distance);
                  break;
                  //////////////////////////////////////////////////////////////////
               case CMD_m_max_elevation:
                  //////////////////////////////////////////////////////////////////
                  attrIt->GetCommandData(m_max_elevation);
                  break;
                  //////////////////////////////////////////////////////////////////
               case CMD_m_pref_elevation:
                  //////////////////////////////////////////////////////////////////
                  attrIt->GetCommandData(m_pref_elevation);
                  break;
                  //////////////////////////////////////////////////////////////////
               case CMD_m_min_elevation:
                  //////////////////////////////////////////////////////////////////
                  attrIt->GetCommandData(m_min_elevation);
                  break;
                  //////////////////////////////////////////////////////////////////
               case CMD_m_smooth_elevation:
                  //////////////////////////////////////////////////////////////////
                  attrIt->GetCommandData(m_smooth_elevation);
                  break;
                  //////////////////////////////////////////////////////////////////
               case CMD_m_max_angle:
                  //////////////////////////////////////////////////////////////////
                  attrIt->GetCommandData(m_max_angle);
                  break;
                  //////////////////////////////////////////////////////////////////
               case CMD_m_pref_angle:
                  //////////////////////////////////////////////////////////////////
                  attrIt->GetCommandData(m_pref_angle);
                  break;
                  //////////////////////////////////////////////////////////////////
               case CMD_m_min_angle:
                  //////////////////////////////////////////////////////////////////
                  attrIt->GetCommandData(m_min_angle);
                  break;
                  //////////////////////////////////////////////////////////////////
               case CMD_m_smooth_angle:
                  //////////////////////////////////////////////////////////////////
                  attrIt->GetCommandData(m_smooth_angle);
                  break;
                  //////////////////////////////////////////////////////////////////
               case CMD_Set_m_Msg_Rx_Pos:
                  //////////////////////////////////////////////////////////////////
                  {
                     const RwChar *name;
                  
                     attrIt->GetCommandData(&name);

                     UnLinkMsg (m_Msg_Rx_Pos);
                     UnRegisterMsg(m_Msg_Rx_Pos);
                     RegisterMsg(m_Msg_Rx_Pos, name, "RwFrame*");
                     LinkMsg(m_Msg_Rx_Pos, "RwFrame*");
                  }
                  break;
                  //////////////////////////////////////////////////////////////////
               case CMD_Set_m_RxEnableCameraEvent:
                  //////////////////////////////////////////////////////////////////
                  {
                     const RwChar *name;
                  
                     attrIt->GetCommandData(&name);

                     UnLinkMsg (m_RxEnableCameraEvent);
                     UnRegisterMsg(m_RxEnableCameraEvent);
                     RegisterMsg(m_RxEnableCameraEvent, name, 0);
                     LinkMsg(m_RxEnableCameraEvent, 0);
                  }
                  break;
                  //////////////////////////////////////////////////////////////////
               case CMD_Set_m_TxEnableCameraEvent:
                  //////////////////////////////////////////////////////////////////
                  {
                     const RwChar *name;
                  
                     attrIt->GetCommandData(&name);

                     UnRegisterMsg(m_TxEnableCameraEvent);
                     RegisterMsg(m_TxEnableCameraEvent, name, 0);
                  }
                  break;
                  //////////////////////////////////////////////////////////////////
               case CMD_Set_m_RxDisableCameraEvent:
                  //////////////////////////////////////////////////////////////////
                  {
                     const RwChar *name;
                  
                     attrIt->GetCommandData(&name);

                     UnLinkMsg (m_RxDisableCameraEvent);
                     UnRegisterMsg(m_RxDisableCameraEvent);
                     RegisterMsg(m_RxDisableCameraEvent, name, 0);
                     LinkMsg(m_RxDisableCameraEvent, 0);
                  }
                  break;
                  //////////////////////////////////////////////////////////////////
               case CMD_Set_View_Angle:
                  //////////////////////////////////////////////////////////////////
                  {
                     RwReal angle;

                     attrIt->GetCommandData(angle);

                     m_ViewWindow = static_cast<RwReal>(RwTan(RWDEG2RAD(angle * 0.5f)));
                  }
                  break;
            
                  // Enable/Disable this camera
                  //
               case CMD_Set_StartsActive:

                  attrIt->GetCommandData(m_this_camera_active);
                  break;

                  // Set incoming render message.

               case CMD_Set_m_InRender:

                  UnLinkMsg(m_renderInMsg);
                  UnRegisterMsg(m_renderInMsg);
                  RegisterMsg(m_renderInMsg, attrIt->GetAs_RwChar_ptr(), "RwCamera*");
                  LinkMsg(m_renderInMsg, "RwCamera*", m_renderInPriority);
                  break;

                  // Set priority of the rendering message.

               case CMD_priority:

                  attrIt->GetCommandData(m_renderInPriority);
                  UnLinkMsg(m_renderInMsg);
                  LinkMsg(m_renderInMsg, "RwCamera*", m_renderInPriority);
                  break;

                  // Set outgoing render message.

               case CMD_Set_m_OutRender:

                  UnRegisterMsg(m_renderOutMsg);
                  RegisterMsg(m_renderOutMsg, attrIt->GetAs_RwChar_ptr(), "RwCamera*");
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
