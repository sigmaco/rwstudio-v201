/*****************************************************************************
*
* File :     CSubRasterCamera.cpp
*
* Notes :
*
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

#include "precomp.h"

//#define RWS_CALLSTACKENABLE
//#define RWS_TRACEENABLE

#ifdef __MWERKS__
#pragma force_active on // Left to its own devices, the MW linker will dead-strip the registrar ctors, so...
#pragma exceptions off // Turn off exceptions due to bug in MW Dwarf Generator for PS2, present in CW 3.04
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "csubrastercamera.h"

#include "framework/core/macros/debugmacros.h"
#include "framework/mainloop/render.h"
#include "framework/toolkits/world/helpers/camerahelper.h"
#include "framework/toolkits/math/maths.h"

#ifdef _WINDOWS
#include "framework/startup/win32/win.h"
#endif

namespace RWS
{
   namespace Generic
   {
      RWS_IMPLEMENT_CLASSID(CSubRasterCamera);
      RWS_REGISTERCLASS(CSubRasterCamera);
      
      /**
      *
      *  Create CSubRasterCamera object.
      *
      *  \params attr reference to a CAttributePacket object.
      *
      */
      CSubRasterCamera::CSubRasterCamera(const CAttributePacket& attr) : InitCEventHandler(&m_pCamera),
         m_top(0.25f),
         m_bottom(0.75f),
         m_left(0.25f),
         m_right(0.75f),
         m_link_priority(65535)
      {
         RWS_FUNCTION("RWS::Generic::CSubRasterCamera::CSubRasterCamera");

#ifdef _WINDOWS
         CEventHandler::RegisterMsg(Win::iMsgResizeCameraRasters, Win::iMsgResizeCameraRastersStr, 0);

         LinkMsg(Win::iMsgResizeCameraRasters);
#endif

         RWS_RETURNVOID();
      }

      /**
      *
      *  Destroy CSubRasterCamera object
      *
      */
      CSubRasterCamera::~CSubRasterCamera(void)
      {
         RWS_FUNCTION("RWS::Generic::CSubRasterCamera::~CSubRasterCamera");

#ifdef _WINDOWS
         UnLinkMsg(Win::iMsgResizeCameraRasters);

         CEventHandler::UnRegisterMsg(Win::iMsgResizeCameraRasters);
#endif

         UnLinkMsg(m_Render_In);
         UnRegisterMsg(m_Render_In);

         UnLinkMsg(m_Render_Out);
         UnRegisterMsg(m_Render_Out);

         RWS_RETURNVOID();
      }

      /**
      *
      *  Create a camera based on the current dimensions. Dimensions are specified by m_top, m_bottom,
      *  m_left, m_right as a factor of the dimension of the main camera's raster. For example if the 
      *  main view or screen is set to 640x480 then if m_top is set to 0.5 the top of this camera's
      *  window will be 480 * 0.5 = 240.
      *
      */
      void CSubRasterCamera::CreateCamera(void)
      {
         RWS_FUNCTION("RWS::Generic::CSubRasterCamera::CreateCamera");

         // Get the main camera, this is needed to determine the size of the main camera's window i.e. the screen
         //
         RwCamera *pCamera = MainLoop::Render::GetCamera();

         RwRaster *pRaster = RwCameraGetRaster(pCamera);

         RwReal width = static_cast<RwReal>(RwRasterGetWidth(pRaster));
         RwReal height = static_cast<RwReal>(RwRasterGetHeight(pRaster));

         RwReal tmp_left, tmp_right, tmp_top, tmp_bottom;

         // Check valid co-ords, left < right and top < bottom
         //
         // 0 <= tmp_left <= 1
         // 0 <= tmp_right <= 1
         // 0 <= tmp_top <= 1
         // 0 <= tmp_bottom <= 1
         //

         // if right is less than left, make left = right
         //
         tmp_left = m_left; if (m_right < m_left) tmp_left = m_right;

         // if left is greater than right, make right = left
         //
         tmp_right = m_right; if (m_left > m_right) tmp_right = m_left;

         // if bottom is less than top then top = bottom
         //
         tmp_top = m_top; if (m_bottom < m_top) tmp_top = m_bottom;

         // if top is greater than bottom then bottom = top
         //
         tmp_bottom = m_bottom; if (m_top > m_bottom) tmp_bottom = m_top;

         RwRect rect;

         rect.x = static_cast<RwUInt32>(width * tmp_left);
         rect.y = static_cast<RwUInt32>(height * tmp_top);

         rect.w = static_cast<RwUInt32>(width * tmp_right - width * tmp_left);
         rect.h = static_cast<RwUInt32>(height * tmp_bottom - height * tmp_top);

         if (rect.w != 0 && rect.h != 0)
         {
            RWS_TRACE("rect "<<rect.x<<" "<<rect.y<<" "<<rect.w<<" "<<rect.h);

            m_pCamera = CameraHelper::CreateSubRasterCamera(rect);
         }
         else
         {
            RWS_TRACE("WARNING: raster is either 0 pixels wide or 0 pixels high");

            m_pCamera = 0;
         }
         
         RWS_RETURNVOID();
      }
      
      /**
      *
      *  Handle events:
      *
      *  Responds to m_Render_In event, which is typically a render event specifying a camera, 
      *  cameras transformation is copied into the RwCamera object owned by this behavior. The RwCamera
      *  object owned by this behavior specifies the extent of the rendering surface. Broadcasts a m_Render_Out
      *  event containing a pointer to the RwCamera object.
      *
      *  Also responds to iMsgResizeCameraRasters, this event is sent by the framework if the lowest level 
      *  rendering surface changes, i.e. on a PC if the application window is resized or on a console if the
      *  screen resolution is modified. The sub-raster is resized accordingly.
      *
      *  \params pMsg reference to a CMsg object.
      *
      */
      void CSubRasterCamera::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::Generic::CSubRasterCamera::HandleEvents");

         if (pMsg.Id == m_Render_In && m_pCamera)
         {
            RWS_ASSERT(m_Render_In != m_Render_Out, "Error Input event cannot equal output event");

            // Take the frame of the input camera, and copy it into the output camera
            //
            if (pMsg.pData)
            {
               RwCamera *pInputCamera = reinterpret_cast<RwCamera*>(pMsg.pData);

               RWS_ASSERTE(pInputCamera);

               RwFrame *pInputCameraFrame = RwCameraGetFrame(pInputCamera);

               RWS_ASSERTE(pInputCameraFrame);

               RwMatrix *pDirectorsCameraMatrix = RwFrameGetMatrix(pInputCameraFrame);

               RWS_ASSERTE(pDirectorsCameraMatrix);

               RwFrame *pCameraFrame = RwCameraGetFrame(m_pCamera.ptr());

               RWS_ASSERTE(pCameraFrame);

               RwFrameTransform(pCameraFrame, pDirectorsCameraMatrix, rwCOMBINEREPLACE);

               CameraHelper::SetFieldOfView(pInputCamera->viewWindow.x, m_pCamera);
            }

            RwCamera *pCamera = m_pCamera;

            RWS_ASSERTE(pCamera);

            // Send this camera out
            //
            CMsg Msg(m_Render_Out, pCamera);

            SendMsg(Msg);
         }
#ifdef _WINDOWS
         else if (pMsg.Id == Win::iMsgResizeCameraRasters)
         {
            CreateCamera();
         }
#endif
         RWS_RETURNVOID();
      }
      
      /**
      *
      *  Handle attributes
      *
      *  \params attr reference to a CAttributePacket object.
      *
      */
      void CSubRasterCamera::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::Generic::CSubRasterCamera:HandleAttributes");
         
         CAttributeHandler::HandleAttributes(attr);
         
         m_pCamera.HandleSystemCommands(attr);
         
         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(CSubRasterCamera));
         
         bool flag = false;   // Camera needs to be re-created

         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
                  //////////////////////////////////////////////////////////////////
               case CMD_In:
                  //////////////////////////////////////////////////////////////////
                  UnLinkMsg(m_Render_In);
                  UnRegisterMsg(m_Render_In);
                  RegisterMsg(m_Render_In, attrIt->GetAs_RwChar_ptr(), "RwCamera*");
                  LinkMsg(m_Render_In, "RwCamera*", m_link_priority);
               break;
                  //////////////////////////////////////////////////////////////////
               case CMD_priority:
                  //////////////////////////////////////////////////////////////////
                  m_link_priority = attrIt->GetAs_RwUInt32();
                  // If already linked, unlink as were about to relink at a different priority
                  //
                  UnLinkMsg(m_Render_In);
                  LinkMsg(m_Render_In, "RwCamera*", m_link_priority);
                  break;
                  //////////////////////////////////////////////////////////////////
               case CMD_Out:
                  //////////////////////////////////////////////////////////////////
                  ReplaceRegisteredMsg(m_Render_Out, attrIt->GetAs_RwChar_ptr(), "RwCamera*");
               break;
                  //////////////////////////////////////////////////////////////////
               case CMD_Left:
                  //////////////////////////////////////////////////////////////////
                  attrIt->GetCommandData(m_left);
                  flag = true;
                  break;
                  //////////////////////////////////////////////////////////////////
               case CMD_Right:
                  //////////////////////////////////////////////////////////////////
                  attrIt->GetCommandData(m_right);
                  flag = true;
                  break;
                  //////////////////////////////////////////////////////////////////
               case CMD_Top:
                  //////////////////////////////////////////////////////////////////
                  attrIt->GetCommandData(m_top);
                  flag = true;
                  break;
                  //////////////////////////////////////////////////////////////////
               case CMD_Bottom:
                  //////////////////////////////////////////////////////////////////
                  attrIt->GetCommandData(m_bottom);
                  flag = true;
                  break;
            }
            ++attrIt;
         }

         if (flag) CreateCamera();

         RWS_RETURNVOID();
      }
   }//namespace Tutorial
}//namespace RWS