#ifndef __RENDERWAREWORLD_H__
#define __RENDERWAREWORLD_H__

#include <vector>
#include "RenderWareCamera.h"
#include "RenderWareClump.h"

extern RwReal NormalsScaleFactor;
namespace RW
{
   const int LINE_SPACING = 12;
 
   class RenderWareWorld
   {
   public:
      typedef std::basic_string <RWSChar> RWSString;

      RenderWareWorld ();

      RwBool Initialise (HWND hWnd);
      RwBool Shutdown ();
      RwBool IsInitialised ();
      
      void Render (HWND hWnd, RwBool DrawSkeleton, RwBool DrawSkelAxis, RwBool m_DrawText,  
                   RwBool DrawWorld, RwBool DrawSectors, RwBool DrawWireFrame, RwBool DrawNormals, 
                   RwInt32 CurrentSector, RwBool ZTest);

      // Clump creation/destruction/iterators
      RenderWareClump* CreateClump (const RwChar* szName, const RwChar* szDependancies);
      RwBool DestroyClump (RenderWareClump* pClump);
      RwBool DestroyAllClumps ();
      RwInt32 GetClumpCount ();
      RenderWareClump* GetClumpAtIndex (RwUInt32 nIndex);
      void SetIsVisible( RwBool visible);
      typedef std::vector <RenderWareClump*> ClumpVector;
      typedef ClumpVector::iterator ClumpIterator;
      RenderWareCamera& Camera ();
      operator RpWorld* () const;
      SkeletonParams   m_params;
      RwSphere m_WorldSphere;
      void CameraPoint( RwReal tilt, RwReal turn);
      RwBool IsBspLoaded();
      void   SetBspFlag(RwBool flag);
      RwBool LoadWorld(RWSString strFile, RWSString strTextures);
      void FitWorldInView(RwCamera *camera, RwSphere *worldSphere);
      RwInt32 m_LastFramesNumberOfSectors;

   protected:
      void AllocateWorld ( void );
      void ClumpRender(RwBool DrawSkeleton, RwBool DrawSkelAxis, RwBool m_DrawText);
      void WorldRender(RwBool DrawSectors,RwBool DrawWorld, RwBool m_DrawText, 
      RwBool DrawWireFrame, RwBool DrawNormals, RwInt32 CurrentSector, RwBool ZTest);
      void WorldGetBoundingSphere(RpWorld *world, RwSphere *sphere);
      void RenderWorldBoundingBox(RpWorld *world);
      RwBool ResizeIm3DVertexArray(RpWorld *world);

      RwBool           m_bspLoaded;    // Is the BSP loaded.
      ClumpVector      m_ClumpVector;  // Vector used to storing clumps.
      RenderWareCamera m_Camera;       // RenderWare Camera.
      RpWorld*         m_pWorld;       // Pointer to the world.
      RwReal           m_NearClip;     // Near clip plane for the camera.
      RwReal           m_FarClip;      // Far clip plane for the camera.
      RpLight*         m_Light;        // Ambient light for the world.
   };
}

#endif // __RENDERWAREWORLD_H__
