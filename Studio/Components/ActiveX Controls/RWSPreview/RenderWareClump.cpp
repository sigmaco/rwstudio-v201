#include "stdafx.h"
#include "RenderWareClump.h"
#include "RenderWareHelpers.h"
#include "rtquat.h"

namespace RW
{
   ////////////////////////////////////////////////////////////////////////////
   //
   RenderWareClump::RenderWareClump (const RwChar * szName,
                                     const RwChar * szTextures) : m_pClump (0)
   {
      m_NearClip = 1.0f;
      m_FarClip = 100.0f;

      RwImageSetPath (szTextures);

      if (RwStream* pStream = RwStreamOpen (rwSTREAMFILENAME, rwSTREAMREAD, szName))
      {
         RwChunkHeaderInfo _RwChunkHeaderInfo;

         RwStreamReadChunkHeaderInfo(pStream, &_RwChunkHeaderInfo);

         // Find clump chunk
         if (_RwChunkHeaderInfo.type == rwID_CLUMP)
         {
            // Load clump
            m_pClump = RpClumpStreamRead (pStream);

            // Move the clump to the origin
            RwV3d Zero = {0.0f, 0.0f, 0.0f};
            RwFrameTranslate (RpClumpGetFrame (m_pClump), &Zero, rwCOMBINEREPLACE);

            // Calculate the bounding sphere for the clump
            ClumpGetBoundingSphere (m_pClump, &m_BoundingSphere);

            // Add the clump to the animation
            m_Animation.SetClump (m_pClump);
         }

         // Find atomic chunk
         if (_RwChunkHeaderInfo.type == rwID_ATOMIC)
         {
            RpAtomic *atomic;

            // Load atomic
            atomic = RpAtomicStreamRead(pStream);
            
            if (atomic)
            {
               RwFrame* pFrame = RwFrameCreate();
                       
               RwFrameSetIdentity( pFrame );

               RpAtomicSetFrame( atomic, pFrame );

               m_pClump = RpClumpCreate();
               
               if (IsValidClump())
               {
                  RpClumpAddAtomic( m_pClump, atomic);

                  RpClumpSetFrame( m_pClump, pFrame );
              
                  // Move the clump to the origin
                  RwV3d Zero = {0.0f, 0.0f, 0.0f};
                  RwFrameTranslate (RpClumpGetFrame (m_pClump), &Zero, rwCOMBINEREPLACE);

                  // Calculate the bounding sphere for the clump
                  ClumpGetBoundingSphere (m_pClump, &m_BoundingSphere);

                  // Add the clump to the animation
                  m_Animation.SetClump (m_pClump);
               }
            }
         }

         RwStreamClose (pStream, 0);
      }
   }

   ////////////////////////////////////////////////////////////////////////////
   //
   RenderWareClump::~RenderWareClump ()
   {
      if (IsValidClump())
      {
         RpClumpDestroy (m_pClump);
      }
   }

   ////////////////////////////////////////////////////////////////////////////
   //
   RwBool RenderWareClump::Rotate (RwReal x, RwReal y, RenderWareCamera& Camera)
   {
      if (IsValidClump())
      {
         if (RwFrame *pFrame = RpClumpGetFrame (m_pClump))
         {
            RwMatrix *cameraMatrix = RwFrameGetMatrix (RwCameraGetFrame (Camera.CameraPtr ()));
            RwV3d clumpPos, right = *RwMatrixGetRight (cameraMatrix);
            RwV3d up = *RwMatrixGetUp(cameraMatrix);

            // Rotate about the clump's bounding sphere center...
            RwV3dTransformPoints(&clumpPos, &(m_BoundingSphere.center), 1, RwFrameGetLTM (pFrame));

            // First translate back to the origin...
            RwV3dScale(&clumpPos, &clumpPos, -1.0f);
            RwFrameTranslate(pFrame, &clumpPos, rwCOMBINEPOSTCONCAT);

            // ...do the rotations...
            RwFrameRotate(pFrame, &up, -x, rwCOMBINEPOSTCONCAT);
            RwFrameRotate(pFrame, &right, y, rwCOMBINEPOSTCONCAT);

            // ...and translate back...
            RwV3dScale(&clumpPos, &clumpPos, -1.0f);
            RwFrameTranslate(pFrame, &clumpPos, rwCOMBINEPOSTCONCAT);
         }

         return TRUE;
      }

      return FALSE;
   }

   ////////////////////////////////////////////////////////////////////////////
   //
   RwBool RenderWareClump::TranslateBy (RwReal x, RwReal y, RwReal z)
   {
      if (IsValidClump())
      {
         // Translate clump by a specified offset.
         if (RwFrame *pFrame = RpClumpGetFrame (m_pClump))
         {
            RwV3d Translation = {-x, -y, -z};
            RwFrameTranslate (pFrame, &Translation, rwCOMBINEPOSTCONCAT);
            return TRUE;
         }
      }

      return FALSE;
   }
   
   ////////////////////////////////////////////////////////////////////////////
   //
   RwBool RenderWareClump::TranslateTo (RwReal x, RwReal y, RwReal z)
   {
      if (IsValidClump())
      {
         RwV3d Pos = {x, y, z};
         RwFrameTranslate (RpClumpGetFrame (m_pClump), &Pos, rwCOMBINEREPLACE);

         return TRUE;
      }

      return FALSE;
   }

   ////////////////////////////////////////////////////////////////////////////
   //
   RwBool RenderWareClump::IsValidClump ()
   {
      return (m_pClump != 0);
   }

   ////////////////////////////////////////////////////////////////////////////
   //
   RenderWareClump::operator RpClump* () const
   {
      return m_pClump;
   }

   
   void RenderWareClump::GetClumpStats( void )
   {
      m_ClumpStats.numAtomics = RpClumpGetNumAtomics(m_pClump);
      m_ClumpStats.numTriangles = 0;
      m_ClumpStats.numVertices = 0;

      RpClumpForAllAtomics(m_pClump, GeometryProperties, reinterpret_cast<void*>(&m_ClumpStats));
   }

   void RenderWareClump::FitClumpInView(RwCamera *camera, RwSphere *worldSphere)
   {
      RwFrame *cameraFrame, *baseFrame;
      RwMatrix *baseMatrix;
      RwV3d yAxis = {0.0f, 1.0f, 0.0f}; 

      // Reset to stop asserts when setting in clip planes...
      RwCameraSetNearClipPlane(camera, 1.0f);
      RwCameraSetFarClipPlane(camera, 10.0f);

      m_FarClip = worldSphere->radius * CLIPFACTOR;
      if( m_FarClip < m_NearClip + MINNEARTOFAR )
      {
         m_FarClip = m_NearClip + MINNEARTOFAR;
      }

      m_NearClip = m_FarClip * 0.01f;
      if( m_NearClip < 0.01f )
      {
         m_NearClip = 0.01f;
      }

      RwCameraSetFarClipPlane(camera, m_FarClip);
      RwCameraSetNearClipPlane(camera, m_NearClip);

      // Make the camera's frame coincident with the base-frame...
      cameraFrame = RwCameraGetFrame(camera);
      RwFrameSetIdentity(cameraFrame);

      baseFrame = RwFrameGetParent(cameraFrame);
      baseMatrix = RwFrameGetMatrix(baseFrame);

      RwV3d *right, at;
      RwReal cameraDistance;

      // Orient the camera...
      RwFrameSetIdentity(baseFrame);
      right = RwMatrixGetRight(baseMatrix);

      RwFrameRotate(baseFrame, right, -INITTILTCLUMP, rwCOMBINEREPLACE);
      RwFrameRotate(baseFrame, &yAxis, INITTURNCLUMP, rwCOMBINEPOSTCONCAT);

      // ...and pull it back, from the center of the sphere, along the
      // -ve at-axis so that the world's bounding-sphere subtends 
      // twice the HFoV...
      cameraDistance = 1.0f * worldSphere->radius / (RwReal)RwTan(DEGTORAD(INITHFOV * 0.5f));

      at = *RwMatrixGetAt(baseMatrix);
      RwV3dScale(&at, &at, -cameraDistance);
      RwV3dAdd(&at, &at, &worldSphere->center);
      RwFrameTranslate(baseFrame, &at, rwCOMBINEPOSTCONCAT);
   }
}
      
