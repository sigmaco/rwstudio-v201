
#include "stdafx.h"
#include "RenderWareCamera.h"
#include "RenderWareHelpers.h"

namespace RW
{
   namespace
   {
      const RwReal fDefaultNearClip = 0.1f;
      const RwReal fDefaultFarClip = 999.0f;

      const RwV3d  XAxis = {1.0f, 0.0f, 0.0f};
      const RwV3d  YAxis = {0.0f, 1.0f, 0.0f};
      const RwV3d  ZAxis = {0.0f, 0.0f, 1.0f};
   }

   ////////////////////////////////////////////////////////////////////////////
   //
   RenderWareCamera::RenderWareCamera () : m_pCamera (0)
   {

   }

   ////////////////////////////////////////////////////////////////////////////
   //
   RwBool RenderWareCamera::Initialise (HWND hWnd)
   {
      ATLASSERT (!m_pCamera);

      if (m_pCamera = RwCameraCreate ()) // Assignment intentional
      {
         if (RwFrame* pFrame = RwFrameCreate ()) // Assignment intentional
            RwCameraSetFrame (m_pCamera, pFrame);

         RwFrame *baseFrame = (RwFrame *)NULL;
         baseFrame = RwFrameCreate();
         RwFrameAddChild(baseFrame, RwCameraGetFrame(m_pCamera));

         RECT rcClient;
         ::GetWindowRect (hWnd, &rcClient);

         // Create the rasters to render to.
         RwInt32 nWidth = rcClient.right - rcClient.left;
         RwInt32 nHeight = rcClient.bottom - rcClient.top;
         RwRaster* pRaster = RwRasterCreate (nWidth, nHeight, 0, rwRASTERTYPECAMERA);
         RwRaster* pZRaster = RwRasterCreate (nWidth, nHeight, 0, rwRASTERTYPEZBUFFER);
         RwCameraSetRaster (m_pCamera, pRaster);
         RwCameraSetZRaster (m_pCamera, pZRaster);

         // Set camera clip planes.
         RwCameraSetNearClipPlane(m_pCamera, fDefaultNearClip);
         RwCameraSetFarClipPlane(m_pCamera, fDefaultFarClip);
      }

      // Clear the background colour.
      m_BackgroundColour.red = m_BackgroundColour.green = m_BackgroundColour.blue = 0;

      return IsInitialised ();
   }

   ////////////////////////////////////////////////////////////////////////////
   //
   RwBool RenderWareCamera::Shutdown ()
   {
      if (IsInitialised())
      {
         // Remove the frame from the camera
         if (RwFrame* pFrame = RwCameraGetFrame (m_pCamera))
         {
            RwCameraSetFrame (m_pCamera, 0);
            RwFrameDestroy (pFrame);
         }

         // Remove and destroy the cameras raster
         if (RwRaster* pRaster = RwCameraGetRaster(m_pCamera))
         {
            RwCameraSetRaster (m_pCamera, 0);
            RwRasterDestroy (pRaster);
         }

         // Remove and destroy the cameras z-raster
         if (RwRaster* pZRaster = RwCameraGetZRaster (m_pCamera))
         {
            RwCameraSetZRaster (m_pCamera, 0);
            RwRasterDestroy (pZRaster);
         }

         // Destroy the camera
         RwCameraDestroy (m_pCamera);
         m_pCamera = 0;

         return TRUE;
      }

      return FALSE;
   }

   ////////////////////////////////////////////////////////////////////////////
   //
   RwBool RenderWareCamera::IsInitialised ()
   {
      return (m_pCamera != 0);
   }

   ////////////////////////////////////////////////////////////////////////////
   //
   RwBool RenderWareCamera::Clear ()
   {
      return (RwCameraClear (m_pCamera, &m_BackgroundColour,
                             rwCAMERACLEARZ | rwCAMERACLEARIMAGE) == m_pCamera);
   }

   ////////////////////////////////////////////////////////////////////////////
   //
   RwBool RenderWareCamera::BeginUpdate ()
   {
      return (RwCameraBeginUpdate (m_pCamera) == m_pCamera);
   }

   ////////////////////////////////////////////////////////////////////////////
   //
   RwBool RenderWareCamera::EndUpdate ()
   {
      return (RwCameraEndUpdate (m_pCamera) == m_pCamera);
   }

   ////////////////////////////////////////////////////////////////////////////
   //
   RwBool RenderWareCamera::ShowRaster (HWND hWnd)
   {
      return (RwCameraShowRaster (m_pCamera, hWnd, 0) == m_pCamera);
   }

   ////////////////////////////////////////////////////////////////////////////
   //
   void RenderWareCamera::Resize (RwUInt32 nWidth, RwUInt32 nHeight)
   {
      RwRect ClientSize = {0, 0, nWidth, nHeight};

      CameraSize (m_pCamera, &ClientSize, 1.0f, 1.0f / 3.14159f);
   }

   ////////////////////////////////////////////////////////////////////////////
   //
   RwBool RenderWareCamera::TranslateBy (RwReal x, RwReal y, RwReal z)
   {
      // Translate camera by a specified offset.
      if (IsInitialised())
      {
         RwV3d at;
         RwV3d right;
         RwV3d up;

         RwFrame *frame = RwFrameGetParent(RwCameraGetFrame(m_pCamera));

         right = *RwMatrixGetRight(RwFrameGetMatrix(frame));
         up = *RwMatrixGetUp(RwFrameGetMatrix(frame));
         at = *RwMatrixGetAt(RwFrameGetMatrix(frame));

         RwV3dScale(&right, &right, x);
         RwV3dScale(&up, &up, y);
         RwV3dScale(&at, &at, z);

         RwFrameTranslate(frame, &right, rwCOMBINEPOSTCONCAT);
         RwFrameTranslate(frame, &up, rwCOMBINEPOSTCONCAT);
         RwFrameTranslate(frame, &at, rwCOMBINEPOSTCONCAT);

         return TRUE;
      }

      return FALSE;
   }

   ////////////////////////////////////////////////////////////////////////////
   //
   RwBool RenderWareCamera::TranslateTo (RwReal x, RwReal y, RwReal z)
   {
      // Translate camera to specific position.
      if (IsInitialised())
      {
         RwFrame* pFrame = RwCameraGetFrame (m_pCamera);
         RwV3d* pPos = RwMatrixGetPos (RwFrameGetMatrix (pFrame));

         RwV3d v = {x - pPos->x, y - pPos->y, z - pPos->z};
         RwFrameTranslate (pFrame, &v, rwCOMBINEPOSTCONCAT);
         return TRUE;
      }

      return FALSE;
   }


   ////////////////////////////////////////////////////////////////////////////
   //
   RenderWareCamera::operator RwCamera* () const
   {
      return m_pCamera;
   }

   ////////////////////////////////////////////////////////////////////////////
   //
   void RenderWareCamera::SetBackGroundColor( unsigned int Red, unsigned int Green, unsigned int Blue)
   {
      if (IsInitialised())
      {
         m_BackgroundColour.red   = Red;
         m_BackgroundColour.green = Green;
         m_BackgroundColour.blue  = Blue;
      }
   }

   ////////////////////////////////////////////////////////////////////////////
   //
   void RenderWareCamera::CameraPoint(RwReal TotalTilt, RwReal tilt, RwReal turn)
   {
      RwFrame *frame;
      RwV3d delta, pos, *right;
      RwV3d yAxis = {0.0f, 1.0f, 0.0f};

      if (IsInitialised())
      {
         // Limit the camera's tilt so that it never quite reaches
         // exactly +90 or -90 degrees...         
         if( TotalTilt + tilt > 89.0f )
         {
            tilt = 89.0f - TotalTilt;

         }
         else if( TotalTilt + tilt < -89.0f )
         {
            tilt = -89.0f - TotalTilt;
         }

         TotalTilt += tilt;

         // Use the base-frame to rotate the camera.
         frame = RwFrameGetParent(RwCameraGetFrame(m_pCamera));

         // Remember where the camera is...
         pos = *RwMatrixGetPos(RwFrameGetMatrix(frame));

         // Remove the translation component...
         RwV3dScale(&delta, &pos, -1.0f);
         RwFrameTranslate(frame, &delta, rwCOMBINEPOSTCONCAT);

         // Rotate to the new direction...
         right = RwMatrixGetRight(RwFrameGetMatrix(frame));
         RwFrameRotate(frame, right, -tilt, rwCOMBINEPOSTCONCAT);
         RwFrameRotate(frame, &yAxis, turn, rwCOMBINEPOSTCONCAT);

         // Put the camera back to where it was...
         RwFrameTranslate(frame, &pos, rwCOMBINEPOSTCONCAT);
      }
   }
}
