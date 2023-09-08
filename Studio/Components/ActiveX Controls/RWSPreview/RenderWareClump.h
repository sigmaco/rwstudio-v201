#ifndef __RENDERWARECLUMP_H__
#define __RENDERWARECLUMP_H__

#include "RenderWareCamera.h"
#include "RenderWareAnim.h"

namespace RW
{
   class RenderWareCamera;

   class RenderWareClump
   {
   public:
      RenderWareClump (const RwChar * szName, const RwChar * szTextures);
      ~RenderWareClump ();

      RwSphere& BoundingSphere () {return m_BoundingSphere; }
      RwBool Rotate (RwReal x, RwReal y, RenderWareCamera& Camera);
      RwBool TranslateBy (RwReal x, RwReal y, RwReal z);
      RenderWareAnim& Anim () { return m_Animation; }
      void FitClumpInView(RwCamera *camera, RwSphere *worldSphere);
      RwBool TranslateTo (RwReal x, RwReal y, RwReal z);
      void GetClumpStats( void );
      operator RpClump* () const;
      RwBool IsValidClump ();
      ClumpStats     m_ClumpStats;

   protected:
      RenderWareAnim m_Animation;         // Clumps animation.
      RwSphere       m_BoundingSphere;    // Clumps bounding sphere.
      RpClump*       m_pClump;            // Pointer to a RenderWare clump.
      RwReal         m_NearClip;          // Near clip plane of camera when fitting clump in view.     
      RwReal         m_FarClip;           // Far clip plane of camera when fitting clump in view.
   };
}

#endif // __RENDERWARECLUMP_H__