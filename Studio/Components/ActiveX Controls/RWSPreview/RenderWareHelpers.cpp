#include "stdafx.h"
#include "RenderWareAnim.h"
#include "RenderWareHelpers.h"
#include "rpmorph.h"
#include <rtcharse.h>

RwIm3DVertex *WorldIm3DVertices = (RwIm3DVertex *)NULL;
RwReal NormalsScaleFactor;
RwUInt32 CurrentSector = 0;

namespace
{
   const RwV3d Zero = {0.0f, 0.0f, 0.0f};

   RwBool g_bComplete = FALSE;
   RtCharset *charset;
   RtCharsetDesc charsetDesc;
}

///////////////////////////////////////////////////////////////////////////////
//
void CameraSize(RwCamera *camera, RwRect *rect, RwReal viewWindow, RwReal aspectRatio)
{
    if( camera )
    {
        RwVideoMode videoMode;
        static RwRect origSize = {0, 0, 0, 0};

        if( origSize.w == 0 || origSize.h == 0 )
        {
            origSize.x = origSize.y = 0;
            origSize.w = RwRasterGetWidth(RwCameraGetRaster(camera));
            origSize.h = RwRasterGetHeight(RwCameraGetRaster(camera));
        }

        RwEngineGetVideoModeInfo(&videoMode, RwEngineGetCurrentVideoMode());

        if( !rect )
        {
            static RwRect r;

            rect = &r;

            // Rect not specified - reuse current values...
            rect->w = RwRasterGetWidth(RwCameraGetRaster(camera));
            rect->h = RwRasterGetHeight(RwCameraGetRaster(camera));
            rect->x = rect->y = 0;
        }

        // For full screen applications, resizing the camera just doesn't
        // make sense, use the video mode size...        
        if( videoMode.flags & rwVIDEOMODEEXCLUSIVE )
        {
            rect->x = rect->y = 0;
            rect->w = videoMode.width;
            rect->h = videoMode.height;
        }

        if( (rect->w > 0) && (rect->h > 0) )
        {
            RwV2d vw;
            RwRaster *raster = NULL;
            RwRaster *zRaster = NULL;

            // In OpenGl & D3D8 you don't need to care about the rasters
			   // of the cameras            
#if (defined(D3D8_DRVMODEL_H)) || (defined(OPENGL_DRVMODEL_H))
            raster = RwCameraGetRaster(camera);
			   zRaster = RwCameraGetZRaster(camera);

			   raster->width = rect->w;
			   raster->height= rect->h;

			   zRaster->width = rect->w;
			   zRaster->height= rect->h;
#else
            // Destroy rasters...
            raster = RwCameraGetRaster(camera);
            if( raster )
            {
                RwRasterDestroy(raster);
                
                raster = NULL;
            }

            zRaster = RwCameraGetZRaster(camera);
            if( zRaster )
            {
                RwRasterDestroy(zRaster);
                
                zRaster = NULL;
            }

            // Create new rasters... 
            raster = RwRasterCreate(rect->w, rect->h, 0, rwRASTERTYPECAMERA);
            zRaster = RwRasterCreate(rect->w, rect->h, 0, rwRASTERTYPEZBUFFER);

            if( raster && zRaster )
            {
                RwCameraSetRaster(camera, raster);
                RwCameraSetZRaster(camera, zRaster);
            }
            else
            {
                if( raster )
                {
                    RwRasterDestroy(raster);
                    
                    raster = NULL;
                }

                if( zRaster )
                {
                    RwRasterDestroy(zRaster);
                    
                    zRaster = NULL;
                }

                rect->w = origSize.w;
                rect->h = origSize.h;

                // Use default values... 
                raster = RwRasterCreate(rect->w, rect->h, 0, rwRASTERTYPECAMERA);

                zRaster = RwRasterCreate(rect->w, rect->h, 0, rwRASTERTYPEZBUFFER);

                RwCameraSetRaster(camera, raster);
                RwCameraSetZRaster(camera, zRaster);

                return;
            }
#endif

            // Figure out the view window... 
            if( videoMode.flags & rwVIDEOMODEEXCLUSIVE )
            {
                // Derive ratio from aspect ratio...
                vw.x = viewWindow;
                vw.y = viewWindow / aspectRatio;
            }
            else
            {
               // In OpenGl & D3D8 you don't need to care about the rasters
			      // of the cameras, just change the aspect ratio
#if (defined(D3D8_DRVMODEL_H)) || (defined(OPENGL_DRVMODEL_H))
#else
                rect->w = RwRasterGetWidth(RwCameraGetRaster(camera));
                rect->h = RwRasterGetHeight(RwCameraGetRaster(camera));
#endif
                // Derive from pixel ratios... 
                if( rect->w > rect->h )
                {
                    vw.x = viewWindow;
                    vw.y = (rect->h * viewWindow) / rect->w;
                }
                else
                {
                    vw.x = (rect->w * viewWindow) / rect->h;
                    vw.y = viewWindow;
                }
            }

            RwCameraSetViewWindow(camera, &vw);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
//
RpClump* DestroyClumpCallback(RpClump *c, void *d)
{
    RpWorldRemoveClump(reinterpret_cast<RpWorld*>(d), c);
    RpClumpDestroy(c);

    return c;
}

///////////////////////////////////////////////////////////////////////////////
//
RpAtomic* AtomicAddBSphereCentre ( RpAtomic *pAtomic, void *data)
{
    if (RpGeometry *pGeometry = RpAtomicGetGeometry (pAtomic))
    {
        RwV3d center;
        RwMatrix *LTM;
        RpMorphTarget *morphTarget;
        RwInt32 i, numMorphTargets;
        RwV3d atomicCentre;
        RwSphere *clumpSphere;

        clumpSphere = (RwSphere *)data;

        // Establish the average centre of this atomic over all morph targets
        atomicCentre = Zero;

        numMorphTargets = RpGeometryGetNumMorphTargets (pGeometry);

        for( i = 0; i < numMorphTargets; i++ )
        {
            morphTarget = RpGeometryGetMorphTarget (pGeometry, i);
            center = RpMorphTargetGetBoundingSphere (morphTarget)->center;
            RwV3dAdd (&atomicCentre, &atomicCentre, &center);
        }

        RwV3dScale (&atomicCentre, &atomicCentre, 1.0f / numMorphTargets);

        // Tranform the average centre of the atomic to world space
        LTM = RwFrameGetLTM (RpAtomicGetFrame (pAtomic));
        RwV3dTransformPoints (&atomicCentre, &atomicCentre, 1, LTM);

        // Add the average centre of the atomic up in order to calculate the centre of the clump
        RwV3dAdd (&clumpSphere->center, &clumpSphere->center, &atomicCentre);
    }

    return pAtomic;
}

///////////////////////////////////////////////////////////////////////////////
//
RpAtomic * AtomicCompareBSphere (RpAtomic *atomic, void *data)
{
    RpGeometry *geometry;

    geometry = RpAtomicGetGeometry(atomic);

    if( geometry )
    {
        RwSphere *sphere, morphTargetSphere;
        RwV3d tempVec;
        RpMorphTarget *morphTarget;
        RwReal dist;
        RwMatrix *LTM;
        RwInt32 i, numMorphTargets;

        sphere = (RwSphere *)data;

        LTM = RwFrameGetLTM(RpAtomicGetFrame(atomic));

        numMorphTargets = RpGeometryGetNumMorphTargets(geometry);

        for( i = 0; i < numMorphTargets; i++ )
        {
            morphTarget = RpGeometryGetMorphTarget(geometry, i);
            morphTargetSphere = *RpMorphTargetGetBoundingSphere(morphTarget);

            RwV3dTransformPoints(&morphTargetSphere.center,
                &morphTargetSphere.center, 1, LTM);

            RwV3dSub(&tempVec, &morphTargetSphere.center, &sphere->center);

            dist = RwV3dLength(&tempVec) + morphTargetSphere.radius;
            if( dist > sphere->radius )
            {
                sphere->radius = dist;
            }
        }
    }

    return atomic;
}

///////////////////////////////////////////////////////////////////////////////
//
void ClumpGetBoundingSphere (RpClump *clump, RwSphere *clumpSphere)
{
   RwInt32 numAnimSamples;

   // First find the mean of all the atomics' bounding sphere centers.
   // All morph targets of all atomics and all frame animations are taken into account.
   // The result is the clump's bounding sphere center...    
   clumpSphere->center = Zero;
   numAnimSamples = 0;

   RwSphere curClumpSphere;

   // average over morph targets and atomics
   curClumpSphere.center = Zero;
   RpClumpForAllAtomics(clump, AtomicAddBSphereCentre, &curClumpSphere);

   RwV3dScale(&curClumpSphere.center, &curClumpSphere.center, 1.0f / RpClumpGetNumAtomics (clump));

   // Sum up the above average in order to calculate the overall
   // average over the frame animation...        
   RwV3dAdd(&clumpSphere->center, &clumpSphere->center, &curClumpSphere.center);

   numAnimSamples++;

   RwV3dScale(&clumpSphere->center, &clumpSphere->center, 1.0f / numAnimSamples);

   // Now, given the clump's bounding sphere center, determine the radius
   // by finding the greatest distance from the center that encloses all
   // the atomics' bounding spheres.  All morph targets, atomics and animations
   // are taken into account    
   clumpSphere->radius = -RwRealMAXVAL;
   numAnimSamples = 0;

   RpClumpForAllAtomics(clump, AtomicCompareBSphere, clumpSphere);

   numAnimSamples++;
}

///////////////////////////////////////////////////////////////////////////////
// NOTE: The behavior of the default callback is simply to execute
// the interpolator following this one (or the first if this is the
// last). For instructional purposes we explicitly reproduce this 
// behavior in a custom callback...
//
RwReal InterpolatorCallback(RpAtomic *atomic, RwReal position)
{
    RwInt32 curIpIndex, newIpIndex, NumInterpolators;
    RwReal newPosition, scale;

    scale = RpInterpolatorGetScale(RpAtomicGetInterpolator(atomic));
    newPosition = position - scale;

    RpGeometry* pGeometry = RpAtomicGetGeometry (atomic);
    NumInterpolators = (RpGeometryGetNumMorphTargets (pGeometry) - 1) * 2;

    curIpIndex = RpMorphAtomicGetCurrentInterpolator(atomic);

    newIpIndex = (curIpIndex + 1) % NumInterpolators;

    RpMorphAtomicSetCurrentInterpolator(atomic, newIpIndex);

    if (!newIpIndex)
       g_bComplete = TRUE;

    return newPosition;
}

///////////////////////////////////////////////////////////////////////////////
//
RpAtomic * AtomicSetupInterpolators(RpAtomic *atomic, void* pbHasMorph)
{
    RpGeometry *geometry;

    geometry = RpAtomicGetGeometry(atomic);

    if( geometry )
    {
        RwInt32 NumMorphTargets = RpGeometryGetNumMorphTargets(geometry);

        if( NumMorphTargets > 1 )
        {
            RwInt32 i;

            // Create interpolators...
            RpMorphGeometryCreateInterpolators(geometry, (NumMorphTargets*2)-1);

            // Link all morph targets...            
            for( i=0; i < (NumMorphTargets - 1); i++ )
            {
                RpMorphGeometrySetInterpolator(geometry, i, i, i+1, 1.0f);
            }

            // Link final morph target to fisrt creating continuos loop...             
            RpMorphGeometrySetInterpolator(geometry, i, i, 0, 1.0f);            

            RpMorphAtomicSetCurrentInterpolator(atomic, 0);

            RpMorphGeometrySetCallBack(geometry, InterpolatorCallback);

            if (pbHasMorph)
            {
               *reinterpret_cast <RwBool*>(pbHasMorph) = TRUE;
            }
        }
    }
    
    return atomic;
}


///////////////////////////////////////////////////////////////////////////////
//
RwBool ClumpSetupInterpolators(RpClump *clump)
{
   RwBool bRetVal = FALSE;
   RpClumpForAllAtomics(clump, AtomicSetupInterpolators, &bRetVal);
   return bRetVal;
}

///////////////////////////////////////////////////////////////////////////////
//
RpAtomic * AtomicMorphAddTime (RpAtomic * atomic, void *data)
{
   g_bComplete = FALSE;
   MorphIncTime * pIncTime = reinterpret_cast<MorphIncTime*>(data);

   if ( pIncTime->m_Time > 0.0f )
      RpMorphAtomicAddTime(atomic, pIncTime->m_Time);

   pIncTime->m_bComplete = g_bComplete;

   return atomic;
}

///////////////////////////////////////////////////////////////////////////////
//
RpAtomic * AtomicMorphSetTime (RpAtomic * atomic, void *data)
{
   g_bComplete = FALSE;
   MorphIncTime * pIncTime = reinterpret_cast<MorphIncTime*>(data);
   
   RpMorphAtomicSetTime(atomic, 0);
   RpMorphAtomicSetCurrentInterpolator (atomic, 0);

   if ( pIncTime->m_Time > 0.0f )
   {
      RpMorphAtomicAddTime(atomic, pIncTime->m_Time);
   }

   pIncTime->m_bComplete = g_bComplete;

   return atomic;
}

///////////////////////////////////////////////////////////////////////////////
//
RpAtomic * AtomicMorphGetDuration(RpAtomic * atomic, void * data)
{
    MorphParams *params;
    RpInterpolator *interp;

    params = static_cast<MorphParams *>(data);

    params->numMorph = RpGeometryGetNumMorphTargets(RpAtomicGetGeometry(atomic));
    params->numInterp = (params->numMorph-1) * 2;
    params->duration = 0;
    
    for (RwUInt32 i = 0; i < params->numInterp; i++)
    {
      RpMorphAtomicSetCurrentInterpolator (atomic, i);

      interp = RpAtomicGetInterpolator(atomic);
      params->duration = params->duration + RpInterpolatorGetScale(interp);
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
RpAtomic * AtomicMorphGetPosition(RpAtomic * atomic, void * data)
{
    MorphParams *params;
    RpInterpolator *interp;

    params = static_cast<MorphParams *>(data);

    RwUInt32 CurrentInterp = RpMorphAtomicGetCurrentInterpolator(atomic);
   
    // Add the length of the previous interpolators to the position.
    for (RwUInt32 i = 0; i < CurrentInterp; i++)
    {
      RpMorphAtomicSetCurrentInterpolator (atomic, i);

      interp = RpAtomicGetInterpolator(atomic);
      params->position = params->position + RpInterpolatorGetScale(interp);
    }

    // Add the position of the current interpolator to the animations position.
    RpMorphAtomicSetCurrentInterpolator (atomic, CurrentInterp);
    interp = RpAtomicGetInterpolator(atomic);
    params->position = params->position + RpInterpolatorGetValue(interp);

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
RpAtomic * AtomicMorphGetTime(RpAtomic * atomic, void * data)
{
   return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
RpAtomic *SkeletonRender(RpAtomic *atom, void *data)
{
    RpHAnimHierarchy *hier;

    if (RpAtomicGetFrame(atom)->child == NULL)
    {
        return(atom);
    }

    hier = RpHAnimGetHierarchy(RpAtomicGetFrame(atom));

    if(!hier)
    {
       hier = RpHAnimGetHierarchy(RpAtomicGetFrame(atom)->child);
    }

    SkeletonParams *params = static_cast<SkeletonParams*>(data);

    if (hier)
    {
        RwMatrix *matrices;
        RpHAnimNodeInfo *finfo;
        RwMatrix *mstack[MAX_BONES],*parent,*root;
        RwInt32 tos = 0;
        RwInt32 i = 0;
        RwIm3DVertex *im3DVertices;
        RwIm3DVertex *imVertex;

        matrices = RpHAnimHierarchyGetMatrixArray(hier);
        finfo = hier->pNodeInfo;

        root = RwFrameGetLTM(RpAtomicGetFrame(atom));
        parent = mstack[0] = RwMatrixCreate();

        RwMatrixSetIdentity (parent);

        im3DVertices = (RwIm3DVertex *)RwMalloc(sizeof(RwIm3DVertex) * MAX_BONES * 8, rwID_NAOBJECT);
        imVertex = im3DVertices;
        for (i=0; i<hier->numNodes; i++)
        {
            RwV3d tmp;

            RwIm3DVertexSetPos(imVertex, parent->pos.x,parent->pos.y,parent->pos.z);
            RwIm3DVertexSetRGBA(imVertex, 255,255,0,255);
            imVertex++;

            RwIm3DVertexSetPos(imVertex, matrices[i].pos.x,matrices[i].pos.y,matrices[i].pos.z);
            RwIm3DVertexSetRGBA(imVertex, 255,255,0,255);
            imVertex++;

            // Find length of bone.  This is needed to scale the axis handles.
            RwV3d len; 
            RwV3dSub (&len, &parent->pos, &matrices[i].pos);

            RwReal length = RwV3dLength(&len);

            if (params->DrawSkeleton)
            {
               //// Drawn bone axes 

               // Right Vector
               RwIm3DVertexSetPos(imVertex, matrices[i].pos.x,matrices[i].pos.y,matrices[i].pos.z);
               RwIm3DVertexSetRGBA(imVertex, 255,0,0,255);
               imVertex++;

               RwV3d right;
               right.x = matrices[i].right.x;
               right.y = matrices[i].right.y;
               right.z = matrices[i].right.z;
               RwV3dScale  (  &right, &right,  length * AXIS_SCALE );
               RwV3dAdd(&tmp, &matrices[i].pos, &right);
               RwIm3DVertexSetPos(imVertex, tmp.x,tmp.y,tmp.z);
               RwIm3DVertexSetRGBA(imVertex, 255,0,0,255);
               imVertex++;

               // Up vector
               RwIm3DVertexSetPos(imVertex, matrices[i].pos.x,matrices[i].pos.y,matrices[i].pos.z);
               RwIm3DVertexSetRGBA(imVertex, 0,255,0,255);
               imVertex++;

               RwV3d up;
               up.x = matrices[i].up.x;
               up.y = matrices[i].up.y;
               up.z = matrices[i].up.z;
               RwV3dScale  (  &up, &up,  length * AXIS_SCALE );

               RwV3dAdd(&tmp, &matrices[i].pos, &up);
               RwIm3DVertexSetPos(imVertex, tmp.x,tmp.y,tmp.z);
               RwIm3DVertexSetRGBA(imVertex, 0,255,0,255);
               imVertex++;

               // Left vector
               RwIm3DVertexSetPos(imVertex, matrices[i].pos.x,matrices[i].pos.y,matrices[i].pos.z);
               RwIm3DVertexSetRGBA(imVertex, 0,0,255,255);
               imVertex++;

               RwV3d at;
               at.x = matrices[i].at.x;
               at.y = matrices[i].at.y;
               at.z = matrices[i].at.z;
               RwV3dScale  (  &at, &at,  length * AXIS_SCALE );

               RwV3dAdd(&tmp, &matrices[i].pos, &at);
               RwIm3DVertexSetPos(imVertex, tmp.x,tmp.y,tmp.z);
               RwIm3DVertexSetRGBA(imVertex, 0,0,255,255);
               imVertex++;
            }

            if (finfo[i].flags & rpHANIMPUSHPARENTMATRIX)
            {
                mstack[++tos] = &matrices[i];
            }

            if (finfo[i].flags & rpHANIMPOPPARENTMATRIX)
            {
                parent = mstack[tos--];
            }
            else
            {
                parent = &matrices[i];
            }
        }

        RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
        RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *)FALSE);

        if( RwIm3DTransform(im3DVertices, imVertex - im3DVertices, (RwMatrix *)root, 0) )
        {
            RwIm3DRenderPrimitive(rwPRIMTYPELINELIST);
            RwIm3DEnd();
        }

        RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *)TRUE);

        RwMatrixDestroy(mstack[0]);
        RwMatrixDestroy(parent);

        RwFree(im3DVertices);
    }

    return(atom);
}

///////////////////////////////////////////////////////////////////////////////
//
RpAtomic *HasHierarchy(RpAtomic *atom, void *data)
{
    RpHAnimHierarchy *hier;
    SkeletonParams *params = static_cast<SkeletonParams*>(data);

    if (RpAtomicGetFrame(atom)->child == NULL)
    {
        return(atom);
    }

    hier = RpHAnimGetHierarchy(RpAtomicGetFrame(atom));
    
    if(!hier)
    {
       hier = RpHAnimGetHierarchy(RpAtomicGetFrame(atom)->child);
    }

    if (hier)
    {
       params->NumBones = hier->numNodes;
       params->HasHierarchy = TRUE;
    }
    else
    {
       params->HasHierarchy = FALSE;
    }

    return(atom);
}

///////////////////////////////////////////////////////////////////////////////
//
void SetIsVisible( RpAtomic& pAtomic, bool isVisible )
{
   RwUInt32 flags = RpAtomicGetFlags(&pAtomic);

   if (isVisible) flags |= static_cast<RwUInt32>(rpATOMICRENDER);
   else flags &= ~ static_cast<RwUInt32>(rpATOMICRENDER);

   RpAtomicSetFlags( &pAtomic, flags);
}

///////////////////////////////////////////////////////////////////////////////
//
RpAtomic* SetIsVisibleCallback(RpAtomic* pAtomic, void* pData)
{
   SetIsVisible(*pAtomic, 0 != pData);

   return(pAtomic);
}

///////////////////////////////////////////////////////////////////////////////
//
void InitTextPrint( void )
{
   static RwRGBA ForegroundColor = {255, 255, 255, 255};
   static RwRGBA BackgroundColor = {0, 0,0, 0};

   RtCharsetOpen ();
   charset = RtCharsetCreate (&ForegroundColor, &BackgroundColor);
   RtCharsetGetDesc(charset, &charsetDesc);      
}

///////////////////////////////////////////////////////////////////////////////
//
void DestroyTextPrint( void)
{
   RtCharsetDestroy (charset);
   RtCharsetClose ();
}

///////////////////////////////////////////////////////////////////////////////
//
void PrintfXY (const RwInt32 x, const RwInt32 y, const char *fmt, ...)
{
   const int temp_buffer_size = MAX_DEBUG_TOOLS_BUFFER_SIZE;

   char buffer[temp_buffer_size];
   va_list argptr;

   // Process the varargs part and create a string
   va_start (argptr, fmt);
   vsprintf (buffer, fmt, argptr);

   va_end (argptr);

   // Display the string at the given screen coordinates
   RtCharsetPrint  (charset, (const RwChar *)buffer, x, y) ;

   RwInt32 ty = y;
}


///////////////////////////////////////////////////////////////////////////////
//
RpAtomic* GeometryProperties(RpAtomic* pAtomic, void* pData)
{
   ClumpStats *pAtomicData = static_cast<ClumpStats*>(pData);
   RpGeometry *pGeometry = RpAtomicGetGeometry  ( pAtomic ); 
   
   if (pGeometry) 
   {
      pAtomicData->numTriangles += RpGeometryGetNumTriangles (pGeometry);
      pAtomicData->numVertices  += RpGeometryGetNumVertices (pGeometry); 
   }

   return(pAtomic);
}

///////////////////////////////////////////////////////////////////////////////
//
RpAtomic * AtomicMorphGetNumMorph(RpAtomic * atomic, void * data)
{
    *reinterpret_cast <RwUInt32*>(data) =RpGeometryGetNumMorphTargets(RpAtomicGetGeometry(atomic));
    RwUInt32 temp = RpGeometryGetNumMorphTargets(RpAtomicGetGeometry(atomic));

    return atomic;
}

////////////////////////////////////////////////////////////////////////////
//
void RenderWorldSectorBoundingBox(RpWorldSector *worldSector)
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

   bbox = RpWorldSectorGetBBox(worldSector);

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
void RenderWorldSectorWireMesh(RpWorldSector *worldSector)
{
   RwV3d *worldVertices;
   RpTriangle *worldTriangles;
   RwIm3DVertex *vertex;
   RwInt32 numPolygons, i;
   RwRGBA color = {0, 255, 255, 255};;

   numPolygons = RpWorldSectorGetNumTriangles(worldSector);
   if( numPolygons == 0 )
   {
      return;
   }

   worldVertices = worldSector->vertices;
   worldTriangles = worldSector->triangles;
   vertex = WorldIm3DVertices;

   for(i=0; i<numPolygons; i++)
   {
      RwV3d vert[3];

      vert[0] = worldVertices[worldTriangles->vertIndex[0]];
      vert[1] = worldVertices[worldTriangles->vertIndex[1]];
      vert[2] = worldVertices[worldTriangles->vertIndex[2]];

      RwIm3DVertexSetPos(vertex, vert[0].x, vert[0].y, vert[0].z);
      RwIm3DVertexSetRGBA(vertex, color.red, color.green, color.blue, color.alpha);
      vertex++;

      RwIm3DVertexSetPos(vertex, vert[1].x, vert[1].y, vert[1].z);
      RwIm3DVertexSetRGBA(vertex, color.red, color.green, color.blue, color.alpha);
      vertex++;

      RwIm3DVertexSetPos(vertex, vert[1].x, vert[1].y, vert[1].z);
      RwIm3DVertexSetRGBA(vertex, color.red, color.green, color.blue, color.alpha);
      vertex++;

      RwIm3DVertexSetPos(vertex, vert[2].x, vert[2].y, vert[2].z);
      RwIm3DVertexSetRGBA(vertex, color.red, color.green, color.blue, color.alpha);
      vertex++;

      RwIm3DVertexSetPos(vertex, vert[2].x, vert[2].y, vert[2].z);
      RwIm3DVertexSetRGBA(vertex, color.red, color.green, color.blue, color.alpha);
      vertex++;

      RwIm3DVertexSetPos(vertex, vert[0].x, vert[0].y, vert[0].z);
      RwIm3DVertexSetRGBA(vertex, color.red, color.green, color.blue, color.alpha);
      vertex++;

      worldTriangles++;
   }

   if( RwIm3DTransform(WorldIm3DVertices, numPolygons*6, (RwMatrix *)NULL, rwIM3D_ALLOPAQUE) )
   {
      RwIm3DRenderPrimitive(rwPRIMTYPELINELIST);

      RwIm3DEnd();
   }
}

////////////////////////////////////////////////////////////////////////////
//
void RenderWorldSectorVertexNormals(RpWorldSector *worldSector)
{
   RwV3d *worldVertices;
   RpVertexNormal *worldNormals;
   RwIm3DVertex *vertex;
   RwInt32 numVertices, i;
   RwRGBA color = {0, 200, 55, 255};;

   numVertices = RpWorldSectorGetNumVertices(worldSector);
   if( numVertices == 0 )
   {
      return;
   }

   worldVertices = worldSector->vertices;
   worldNormals = worldSector->normals;
   vertex = WorldIm3DVertices;

   for(i=0; i<numVertices; i++)
   {
      RwV3d normal;

      RPV3DFROMVERTEXNORMAL(normal, *worldNormals);

      RwIm3DVertexSetPos(vertex, worldVertices->x, worldVertices->y, worldVertices->z);

      RwIm3DVertexSetRGBA(vertex, color.red, color.green, color.blue, color.alpha);

      vertex++;

      RwIm3DVertexSetPos(vertex, 
         worldVertices->x + NormalsScaleFactor * normal.x, 
         worldVertices->y + NormalsScaleFactor * normal.y,
         worldVertices->z + NormalsScaleFactor * normal.z);

      RwIm3DVertexSetRGBA(vertex, color.red, color.green, color.blue, color.alpha);

      vertex++;
      worldVertices++;
      worldNormals++;
   }

   if( RwIm3DTransform(WorldIm3DVertices, 
      numVertices*2, (RwMatrix *)NULL, rwIM3D_ALLOPAQUE) )
   {
      RwIm3DRenderPrimitive(rwPRIMTYPELINELIST);

      RwIm3DEnd();
   }
}

////////////////////////////////////////////////////////////////////////////
//
RpWorldSector *WorldSectorCompareSize(RpWorldSector *worldSector, void *data)
{
      RwInt32 *currentNumVerts = (RwInt32 *)data;
      RwInt32 temp;

      // For the triangle mesh...
      temp = 6 * RpWorldSectorGetNumTriangles(worldSector);

      if( temp > *currentNumVerts )
      {
         *currentNumVerts = temp;
      }

      // For the vertex normals...        
      temp = 2 * RpWorldSectorGetNumVertices(worldSector);

      if( temp > *currentNumVerts )
      {
         *currentNumVerts = temp;
      }

      return worldSector;
}

////////////////////////////////////////////////////////////////////////////
//
RpWorldSector *SectorsRenderCallBack(RpWorldSector *worldSector, void *pData)
{
   WorldStats *pWorldStats = static_cast<WorldStats*>(pData);
   pWorldStats->NumSectors++;
   pWorldStats->NumVertices  += RpWorldSectorGetNumVertices(worldSector);
   pWorldStats->NumTriangles += RpWorldSectorGetNumPolygons(worldSector);

   if ((pWorldStats->Currentsector == 0) || (pWorldStats->Currentsector == pWorldStats->NumSectors))
   {
      pWorldStats->CurrentSectorNumVertices  = RpWorldSectorGetNumVertices(worldSector);
      pWorldStats->CurrentSectorNumTriangles = RpWorldSectorGetNumPolygons(worldSector);

      RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void *)NULL);
      RwRenderStateSet(rwRENDERSTATESHADEMODE, (void *)rwSHADEMODEFLAT);
      if (pWorldStats->DrawSectors)
      {
         // Draw the world sector bounding-box, triangle mesh and 
         // vertex normals if required...
         RenderWorldSectorBoundingBox(worldSector);
      }

      if (pWorldStats->DrawWireFrame)
      { 
         RenderWorldSectorWireMesh(worldSector);
      }

      if (pWorldStats->DrawNormals)
      {
         RenderWorldSectorVertexNormals(worldSector);
      }
   }

   return worldSector;
}
