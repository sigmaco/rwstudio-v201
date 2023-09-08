#ifndef __RENDERWARECAMERA_H__
#define __RENDERWARECAMERA_H__

#include "RenderWareClump.h"

namespace RW
{
   class RenderWareClump;

   class RenderWareCamera
   {
   public:
      RenderWareCamera ();
      RwBool Initialise (HWND hWnd);
      RwBool Shutdown ();
      RwBool IsInitialised ();
      RwBool Clear ();
      RwBool BeginUpdate ();
      RwBool EndUpdate ();
      RwBool ShowRaster (HWND hWnd);
      void Resize (RwUInt32 nWidth, RwUInt32 nHeight);
      RwBool TranslateBy (RwReal x, RwReal y, RwReal z);
      RwBool TranslateTo (RwReal x, RwReal y, RwReal z);
      void CameraPoint(RwReal TotalTilt, RwReal tilt, RwReal turn);
      operator RwCamera* () const;
      RwCamera* CameraPtr () const { return m_pCamera; }
      void SetBackGroundColor( unsigned int Red, unsigned int Green, unsigned int Blue);

   protected:
      RwRGBA    m_BackgroundColour; // Background color of the camera viewport.
      RwCamera* m_pCamera;          // Pointer to a RenderWare camera.
   };
}

#endif // __RENDERWARECAMERA_H__
