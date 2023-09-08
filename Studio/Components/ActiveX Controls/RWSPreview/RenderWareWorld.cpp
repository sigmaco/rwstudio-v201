#include "stdafx.h"
#include "RenderWareWorld.h"
#include "RenderWareHelpers.h"


namespace RW
{
   namespace
   {
      RwReal fBoundingBoxExtents = 4000.0f;
   }

   ////////////////////////////////////////////////////////////////////////////
   //
   RenderWareWorld::RenderWareWorld () : m_pWorld (0), 
      m_NearClip (0.01f), m_FarClip (100.0f),m_bspLoaded (false), m_LastFramesNumberOfSectors(0), m_Light(0)
   {
      CurrentSector = 0;
   }

   ////////////////////////////////////////////////////////////////////////////
   //
   RwBool RenderWareWorld::Initialise (HWND hWnd)
   {
      ATLASSERT (!m_pWorld);

      if (m_Camera.Initialise (hWnd))
      {
         AllocateWorld();
      }

      return IsInitialised ();
   }


   ////////////////////////////////////////////////////////////////////////////
   //
   void RenderWareWorld::AllocateWorld ( void )
   {
      RwBBox BoundingBox;
      BoundingBox.inf.x = BoundingBox.inf.y = BoundingBox.inf.z = -fBoundingBoxExtents;
      BoundingBox.sup.x = BoundingBox.sup.y = BoundingBox.sup.z = fBoundingBoxExtents;

      if (m_pWorld = RpWorldCreate(&BoundingBox))
      {
         RpWorldAddCamera(m_pWorld, m_Camera);
      }
   }

   ////////////////////////////////////////////////////////////////////////////
   //
   RwBool RenderWareWorld::Shutdown ()
   {
      if (m_pWorld)
      {
         ClumpIterator Iter = m_ClumpVector.begin ();

         while (Iter != m_ClumpVector.end ())
         {
            RpWorldRemoveClump (m_pWorld, **Iter);
            delete *Iter;
            ++Iter;
         }

         m_ClumpVector.clear ();

         RpWorldRemoveCamera (m_pWorld, m_Camera);
         RpWorldDestroy (m_pWorld);
      }

      m_Camera.Shutdown ();
      m_pWorld = 0;

      return FALSE;
   }

   ////////////////////////////////////////////////////////////////////////////
   //
   RwBool RenderWareWorld::IsInitialised ()
   {
      return (m_pWorld && m_Camera.IsInitialised ());
   }

   ////////////////////////////////////////////////////////////////////////////
   //
   void RenderWareWorld::Render (HWND hWnd, RwBool DrawSkeleton, RwBool DrawSkelAxis, RwBool m_DrawText,  
      RwBool DrawWorld, RwBool DrawSectors, RwBool DrawWireFrame, RwBool DrawNormals, 
      RwInt32 CurrentSector, RwBool ZTest)
   {
      if (m_Camera.IsInitialised ())
      {
         m_Camera.Clear ();

         if (m_Camera.BeginUpdate ())
         {
            if (m_bspLoaded)
            {
               WorldRender(DrawSectors,DrawWorld, m_DrawText, DrawWireFrame, DrawNormals, CurrentSector, ZTest);
            }
            else
            {
               ClumpRender(DrawSkeleton, DrawSkelAxis, m_DrawText);
            }

            m_Camera.EndUpdate ();
         }

         m_Camera.ShowRaster (hWnd);
      }

   }

   ////////////////////////////////////////////////////////////////////////////
   //
   void RenderWareWorld::WorldRender(RwBool DrawSectors,RwBool DrawWorld, RwBool m_DrawText, 
                                     RwBool DrawWireFrame, RwBool DrawNormals, RwInt32 CurrentSector, RwBool ZTest)
   {
      RwUInt32 y = 10;
      RwBool SavedZTest;

      WorldStats worldStats;
      worldStats.NumSectors = 0;
      worldStats.NumVertices = 0;
      worldStats.NumTriangles = 0;
      worldStats.DrawSectors = DrawSectors;
      worldStats.Currentsector = CurrentSector;
      worldStats.DrawWireFrame = DrawWireFrame;
      worldStats.DrawNormals = DrawNormals;

      if (DrawWorld)
      {
         RpWorldRender (m_pWorld); 
      }
      
      if ((!DrawSectors) && (!DrawWireFrame) && (!DrawNormals) && (!DrawWorld))
      {
         RenderWorldBoundingBox(m_pWorld);  
      }

      // Set Z test, based on toolbar button
      RwRenderStateGet (rwRENDERSTATEZTESTENABLE  , (void*)&SavedZTest);
      RwRenderStateSet (rwRENDERSTATEZTESTENABLE  , (void*)ZTest);

      RpWorldForAllWorldSectors(m_pWorld, SectorsRenderCallBack, &worldStats );

      // restore the z write to previous setting.
      RwRenderStateSet (rwRENDERSTATEZTESTENABLE  , (void*)SavedZTest);

      // Because RenderWare doesn't store the total number of sectors, you need to store off the count
      // from this frame, so you can do checks to see if the current sector has overrun the total number of 
      // sectors.
      m_LastFramesNumberOfSectors = worldStats.NumSectors;

      if (m_DrawText)
      {
         PrintfXY(10, y, "World Render");
         y = y + LINE_SPACING;

         PrintfXY(10, y, "------------");
         y = y + LINE_SPACING;

         PrintfXY(10, y, "Sectors             : %d", worldStats.NumSectors);
         y = y + LINE_SPACING;

         PrintfXY(10, y, "Vertices            : %d", worldStats.NumVertices);
         y = y + LINE_SPACING;

         PrintfXY(10, y, "Triangles           : %d", worldStats.NumTriangles);
         y = y + LINE_SPACING;

         PrintfXY(10, y, "Total Num Materials : %d", RpWorldGetNumMaterials  ( m_pWorld ) );
         y = y + (LINE_SPACING*2);

         if (worldStats.Currentsector >= 1)
         {
            PrintfXY(10, y, "Current Sector      : %d", worldStats.Currentsector);
            y = y + LINE_SPACING;

            PrintfXY(10, y, "Sector %d Vertices  : %d", worldStats.Currentsector, worldStats.CurrentSectorNumVertices);
            y = y + LINE_SPACING;

            PrintfXY(10, y, "Sector %d Triangles : %d", worldStats.Currentsector, worldStats.CurrentSectorNumTriangles);
            y = y + LINE_SPACING;
         }
      }
   }

   ////////////////////////////////////////////////////////////////////////////
   //
   void RenderWareWorld::ClumpRender(RwBool DrawSkeleton, RwBool DrawSkelAxis, RwBool m_DrawText)
   {
      RwUInt32 y = 10;

      RW::RenderWareClump * pClump = GetClumpAtIndex (0);

      if (pClump)
      {
         RpClumpRender (*pClump);
      }

      if ((pClump) && DrawSkeleton)
      {
         m_params.DrawSkeleton = DrawSkelAxis;

         RpClumpForAllAtomics(pClump->Anim ().GetClump(), SkeletonRender, reinterpret_cast<void*>(&m_params));
      }

      if (pClump && m_DrawText)
      {
         PrintfXY(10, y, "Clump Render");
         y = y + LINE_SPACING;
         PrintfXY(10, y, "------------");
         y = y + LINE_SPACING;

         PrintfXY(10, y, "Atomics       : %d", pClump->m_ClumpStats.numAtomics);
         y = y + LINE_SPACING;
         PrintfXY(10, y, "Vertices      : %d", pClump->m_ClumpStats.numVertices);
         y = y + LINE_SPACING;
         PrintfXY(10, y, "Triangles     : %d", pClump->m_ClumpStats.numTriangles);
         y = y + LINE_SPACING;

         if (m_params.HasHierarchy)
         {                  
            PrintfXY(10, y, "Bones         : %d", m_params.NumBones);
            y = y + LINE_SPACING;

            PrintfXY(10, y, "Anim Duration : %.2f", pClump->Anim ().GetAnimationDuration() );
            y = y + LINE_SPACING;              

            PrintfXY(10, y, "Anim Position : %.2f", pClump->Anim ().GetAnimationTime() );
            y = y + LINE_SPACING;              
         }

         if (pClump->Anim().HasMorph())
         {   

            PrintfXY(10, y, "Anim Duration : %.2f", pClump->Anim ().m_MorphParameters.duration );
            y = y + LINE_SPACING;              

            PrintfXY(10, y, "Anim Position : %.2f", pClump->Anim ().m_MorphParameters.position );
            y = y + LINE_SPACING;   

            RwUInt32 NumMorph = 0;
            RpClumpForAllAtomics(pClump->Anim ().GetClump(), AtomicMorphGetNumMorph, &NumMorph);
            PrintfXY(10, y, "Morph Targets : %d", NumMorph );
            y = y + LINE_SPACING;              

            PrintfXY(10, y, "Interpolators : %d", (NumMorph-1) * 2);
            y = y + LINE_SPACING;              
         }
      }
   }

   ////////////////////////////////////////////////////////////////////////////
   //
   RenderWareClump* RenderWareWorld::CreateClump (const RwChar* szName,
                                                  const RwChar* szTextures)
   {
      if (RenderWareClump* pClump = new RenderWareClump (szName, szTextures))
      {
         if (pClump->IsValidClump ())
         {
            RpWorldAddClump (m_pWorld, *pClump);
            m_ClumpVector.push_back (pClump);
            return pClump;
         }
         else
         {
            delete pClump;
            return 0;
         }
      }

      return 0;
   }

   void RenderWareWorld::SetIsVisible( RwBool visible)
   {
      RW::RenderWareClump * pClump = GetClumpAtIndex (0);
      RpClumpForAllAtomics(pClump->Anim ().GetClump(), SetIsVisibleCallback, reinterpret_cast<void*>(visible));
   }

   ////////////////////////////////////////////////////////////////////////////
   //
   RwBool RenderWareWorld::DestroyClump (RenderWareClump* pClump)
   {
      ClumpIterator Iter = m_ClumpVector.begin ();
      while (Iter != m_ClumpVector.end ())
      {
         if (pClump == *Iter)
         {
            RpWorldRemoveClump (m_pWorld, **Iter);
            delete pClump;

            m_ClumpVector.erase (Iter);
            return TRUE;
         }

         ++Iter;
      }
 
      return FALSE;
   }

   ////////////////////////////////////////////////////////////////////////////
   //
   RwBool RenderWareWorld::DestroyAllClumps ()
   {
      ClumpIterator Iter = m_ClumpVector.begin ();
      while (Iter != m_ClumpVector.end ())
      {
         RpWorldRemoveClump (m_pWorld, **Iter);
         delete *Iter;
         ++Iter;
      }

      m_ClumpVector.clear ();
      return TRUE;
   }

   ////////////////////////////////////////////////////////////////////////////
   //
   RwInt32 RenderWareWorld::GetClumpCount ()
   {
      return m_ClumpVector.size ();
   }

   ////////////////////////////////////////////////////////////////////////////
   //
   RenderWareClump* RenderWareWorld::GetClumpAtIndex (RwUInt32 nIndex)
   {
      if (nIndex < m_ClumpVector.size ())
      {
         return m_ClumpVector [nIndex];
      }
      else
      {
         return 0;
      }
   }

   ////////////////////////////////////////////////////////////////////////////
   //
   RenderWareCamera& RenderWareWorld::Camera ()
   {
      return m_Camera;
   }

   ////////////////////////////////////////////////////////////////////////////
   //
   RenderWareWorld::operator RpWorld* () const
   {
      return m_pWorld;
   }
   
   ////////////////////////////////////////////////////////////////////////////
   //
   RwBool RenderWareWorld::IsBspLoaded()
   {
      return m_bspLoaded;
   }

   ////////////////////////////////////////////////////////////////////////////
   //
   void RenderWareWorld::SetBspFlag(RwBool flag)
   {
      m_bspLoaded = flag;
   }

   ////////////////////////////////////////////////////////////////////////////
   //
   RwBool RenderWareWorld::LoadWorld(RWSString strFile, RWSString strTextures)
   {
      USES_CONVERSION;
      RwImageSetPath(T2A(strTextures.c_str()));
      RwTexDictionarySetCurrent(RwTexDictionaryCreate());

      if ( m_pWorld )  
      {
         DestroyAllClumps();

         if( m_Light )
         {
            RpWorldRemoveLight(m_pWorld, m_Light);
            RpLightDestroy(m_Light);
         }

         RpWorldRemoveCamera(m_pWorld, m_Camera);
         RpWorldDestroy(m_pWorld);

         // Recreate the world
         AllocateWorld();
      }

      if(RwStream* stream = RwStreamOpen(rwSTREAMFILENAME, rwSTREAMREAD, T2A(strFile.c_str())))
      {
         if( RwStreamFindChunk(stream, rwID_WORLD, (RwUInt32 *)NULL, (RwUInt32 *)NULL) )
         {
            m_pWorld = RpWorldStreamRead(stream);

            RwStreamClose (stream, NULL);
         }
      }

      if (m_pWorld)
      {
         m_bspLoaded = true;
         ResizeIm3DVertexArray(m_pWorld);

         m_Light = RpLightCreate(rpLIGHTAMBIENT);
         if( m_Light )
         {
            RwRGBAReal color;

            color.red = color.green = color.blue = 0.5f;
            color.alpha = 1.0f;

            RpLightSetColor(m_Light, &color);
            RpWorldAddLight(m_pWorld, m_Light);
         }

         RpWorldAddCamera (m_pWorld, m_Camera);
         WorldGetBoundingSphere(m_pWorld, &m_WorldSphere);
         FitWorldInView(m_Camera, &m_WorldSphere);
         NormalsScaleFactor = m_WorldSphere.radius * NORMALSFACTOR;

         if (RpLtMapWorldLightMapsQuery(m_pWorld) > 0)
         {
            RpLtMapSetRenderStyle(static_cast<RpLtMapStyle >(rpLTMAPSTYLERENDERBASE | 
               rpLTMAPSTYLERENDERLIGHTMAP), m_pWorld);
         }
      }

      return (m_bspLoaded);
   }

   ////////////////////////////////////////////////////////////////////////////
   //
   void RenderWareWorld::WorldGetBoundingSphere(RpWorld *world, RwSphere *sphere)
   {
       const RwBBox *bbox = 0;
       RwV3d temp;

       bbox = RpWorldGetBBox(world);

       RwV3dAdd(&temp, &bbox->sup, &bbox->inf);
       RwV3dScale(&sphere->center, &temp, 0.5f);

       RwV3dSub(&temp, &bbox->sup, &bbox->inf);
       sphere->radius = RwV3dLength(&temp) * 0.5f;

   }

   ////////////////////////////////////////////////////////////////////////////
   //
   void RenderWareWorld::FitWorldInView(RwCamera *camera, RwSphere *worldSphere)
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

      RwFrameRotate(baseFrame, right, -INITTILTWORLD, rwCOMBINEREPLACE);
      RwFrameRotate(baseFrame, &yAxis, INITTURNWORLD, rwCOMBINEPOSTCONCAT);

      // ...and pull it back, from the center of the sphere, along the
      // -ve at-axis so that the world's bounding-sphere subtends 
      // twice the HFoV...      
      cameraDistance = 0.5f * worldSphere->radius / (RwReal)RwTan(DEGTORAD(INITHFOV * 0.5f));

      at = *RwMatrixGetAt(baseMatrix);
      RwV3dScale(&at, &at, -cameraDistance);
      RwV3dAdd(&at, &at, &worldSphere->center);
      RwFrameTranslate(baseFrame, &at, rwCOMBINEPOSTCONCAT);
   }

   ////////////////////////////////////////////////////////////////////////////
   //
   void RenderWareWorld::CameraPoint( RwReal tilt, RwReal turn)
   {
      m_Camera.CameraPoint(INITTILTWORLD, tilt, turn);
   }

   ////////////////////////////////////////////////////////////////////////////
   //
   void RenderWareWorld::RenderWorldBoundingBox(RpWorld *world)
   {
       const RwBBox *bbox;
       RwIm3DVertex *vertex;
       RwRGBA color = {255, 255,   0, 255};
       RwInt32 i;

       static RwImVertexIndex indices[24] =
       {
           0, 1, 1, 3, 3, 2, 2, 0, 4, 5, 5, 7,
           7, 6, 6, 4, 0, 4, 1, 5, 2, 6, 3, 7
       };

       bbox = RpWorldGetBBox(world);
       vertex = WorldIm3DVertices;

       for(i=0; i<8; i++)
       {
           RwIm3DVertexSetPos(vertex, 
               i & 1 ? bbox->sup.x : bbox->inf.x,
               i & 2 ? bbox->sup.y : bbox->inf.y,
               i & 4 ? bbox->sup.z : bbox->inf.z);

           RwIm3DVertexSetRGBA(vertex, color.red, color.green, color.blue, color.alpha);

           vertex++;
       }

       if( RwIm3DTransform(WorldIm3DVertices, 8, (RwMatrix *)NULL, rwIM3D_ALLOPAQUE) )
       {
           RwIm3DRenderIndexedPrimitive(rwPRIMTYPELINELIST, indices, 24);

           RwIm3DEnd();
       }
   }

   ////////////////////////////////////////////////////////////////////////////
   //
   RwBool RenderWareWorld::ResizeIm3DVertexArray(RpWorld *world)
   {
       RwInt32 numVerts;
       RwBool retVal = FALSE;

       static RwInt32 numIm3DVerts = 0;

       // At least, we need 8 vertices for each world-sector bounding-box...
       numVerts = 8;

       RpWorldForAllWorldSectors(world,  WorldSectorCompareSize, (void *)&numVerts);

       // Resize only if we need more memory...
       if( numVerts > numIm3DVerts )
       {
           RwIm3DVertex *vertices = 0;

           vertices = (RwIm3DVertex *)RwRealloc(WorldIm3DVertices, numVerts * sizeof(RwIm3DVertex), rwID_NAOBJECT);

           if( vertices )
           {
               WorldIm3DVertices = vertices;
               numIm3DVerts = numVerts;

               retVal = TRUE;
           }
           else
           {
               retVal = FALSE;
           }
       }

       return retVal;
   }

}

