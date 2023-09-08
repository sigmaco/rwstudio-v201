/*****************************************************************************
*
* File :      TBClumpView.cpp
*
* Abstract :  Rotate an atomic object within a behavior, using RenderWare studio
*             to control the amount of rotation
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
// Left to its own devices, the MW linker will dead-strip the registrar ctors, so...
#ifdef __MWERKS__
#pragma force_active on
#pragma exceptions off // Turn off exceptions due to bug in MW Dwarf Generator for PS2, present in CW 3.04
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Includes
//
#include "rprandom.h"
#include "rpskin.h"
#include "rppatch.h"

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/math/maths.h"
#include "tbclumpview.h"
#include "framework/toolkits/world/factory.h"

namespace RWS
{
   namespace ToolBox
   {
      namespace
      {
         typedef struct _TriStripData
         {
            RwMatrix *matrix;
            RwV3d *vertices;
            RwBool lengths;
            RwInt32 numTriStripTriangles;
         } TriStripData;

         static RwRGBA Cyan = { 0, 255, 255, 255 };
         static RwRGBA Green = { 0, 255, 0, 255 };
         static RwV3d Zero = {0.0f, 0.0f, 0.0f};

         static RwRGBA Color;
         #define DISPATCHSIZE (1000)
         
         /*
         *  Render randomly colored, flat-shaded triangle strips to highlight the
         *  strips in the given RpMesh.
         */
         RpMesh * MeshRenderTriStrip(RpMesh * mesh, RpMeshHeader * __RWUNUSED__ meshHeader, void * data)
         {
            RwMatrix        *matrix       = ((TriStripData *)data)->matrix;
            RwV3d           *vertices     = ((TriStripData *)data)->vertices;
            RwBool           lengths      = ((TriStripData *)data)->lengths;
            RwInt32          numTriStripTriangles = ((TriStripData *)data)->numTriStripTriangles;
            RwImVertexIndex *currentIndex = mesh->indices;
            RwUInt32         vertCounter  = 0;

            RwIm3DVertex    *TriStrip;
            RwUInt32         i, j;

            TriStrip = (RwIm3DVertex *)RwMalloc(mesh->numIndices * sizeof(RwIm3DVertex), rwMEMHINTDUR_FRAME);

            Color.red   = 0;
            Color.green = 0;
            Color.blue  = 0;
            Color.alpha = 255;

            if (!lengths)
            {
               RpRandomSeed(mesh->numIndices);
            }

            /* Process the index array, test for duplicate vertices */
            for (i = 0; i < mesh->numIndices; i++)
            {
               RwIm3DVertexSetPos( &TriStrip[i], 
               vertices[*currentIndex].x,
               vertices[*currentIndex].y,
               vertices[*currentIndex].z );

               /* Check indices for join-strip degenerates */
               if ((i > 0) && ((*currentIndex) == (*(currentIndex-1))))
               {
                  /* This is degenerate - start a new strip if we've got a triangle */
                  if (vertCounter >= 3)
                  {
                     if (lengths)
                     {
                        if ((vertCounter-3)<(RwUInt32)numTriStripTriangles)
                        {
                           Color.red = 
                              (RwUInt8)(255 - (255*(vertCounter-3))/numTriStripTriangles);
                        }
                        else
                        {
                           Color.red = 0;
                        }
                     }
                     else
                     {
                        Color.red   = (RwUInt8)(RpRandom() % 200) + 50;
                        Color.green = (RwUInt8)(RpRandom() % 200) + 50;
                        Color.blue  = (RwUInt8)(RpRandom() % 200) + 50;
                     }

                     /* Colour my verts */
                     for (j = 0; j < vertCounter; j++)
                     {
                        RwIm3DVertexSetRGBA( &TriStrip[i - (j+1)],
                        Color.red,
                        Color.green,
                        Color.blue,
                        Color.alpha );
                     }

                     /* Zero counter */
                     vertCounter = 0;
                  }
               }
               else
               {
                  /* count vertices. Need 3 to make a triangle. */
                  vertCounter++;
               }

               /* Point to the next source vertex */
               currentIndex++;
            }

	         /* final geometry */
	         currentIndex--;
	         if (vertCounter >= 3)
	         {
		         if (lengths)
		         {
			         if ((vertCounter-3)<(RwUInt32)numTriStripTriangles)
			         {
				         Color.red = 
					         (RwUInt8)(255 - (255*(vertCounter-3))/numTriStripTriangles);
			         }
			         else
			         {
				         Color.red = 0;
			         }
		         }
		         else
		         {
			         Color.red   = (RwUInt8)(RpRandom() % 200) + 50;
			         Color.green = (RwUInt8)(RpRandom() % 200) + 50;
			         Color.blue  = (RwUInt8)(RpRandom() % 200) + 50;
		         }

		         /* Colour my verts */
		         for (j = 0; j < vertCounter; j++)
		         {
			         RwIm3DVertexSetRGBA( &TriStrip[i - (j+1)],
								          Color.red,
								          Color.green,
								          Color.blue,
								          Color.alpha );
		         }
	         }

	         RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
	         RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *)TRUE);
	         RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *)TRUE);
	         RwRenderStateSet(rwRENDERSTATESRCBLEND, (void *)rwBLENDONE);
	         RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void *)rwBLENDZERO);

	         if (RwIm3DTransform(TriStrip, mesh->numIndices, matrix, 0))
	         {                         
		         RwIm3DRenderPrimitive(rwPRIMTYPETRISTRIP); 
		         RwIm3DEnd();
	         }   

	         RwFree(TriStrip);

	         return mesh;
         }

         /*
         *  Render each of the tristrips in a RpAtomic untextured and
         *  flatshaded, using a random color for each strip.
         */
         RpAtomic * AtomicRenderTriStrip(RpAtomic * atomic, void * data)
         {
	         RpGeometry *geometry = RpAtomicGetGeometry(atomic);
	         TriStripData *info = (TriStripData *)data;

	         // We don't current support skinned objects
	         if (RpSkinAtomicGetSkin(atomic) ||
		         //or patches
		         RpPatchAtomicGetPatchMesh(atomic) ||
		         //or native data
		         (geometry && ((RpGeometryGetFlags(geometry)&rpGEOMETRYNATIVE)))
		         )
	         {
		         return atomic;
	         }

	         info->matrix = RwFrameGetLTM(RpAtomicGetFrame(atomic));
	         info->vertices = RpMorphTargetGetVertices(RpGeometryGetMorphTarget(geometry, 0));

	         /* Is this tristripped? */
	         if(RpGeometryGetFlags(geometry) & rpGEOMETRYTRISTRIP)
	         {
		         /* Enumerate all meshes */
		         RpGeometryForAllMeshes(geometry, MeshRenderTriStrip, info);
	         }

	         return atomic;
         }

		   /*
         *  Render the given RpMesh flat-shaded and untextured.
         */
         RpMesh * MeshRenderMeshes(RpMesh * mesh, 
				                       RpMeshHeader * __RWUNUSED__ meshHeader, 
				                       void * data)
         {
	         RwMatrix        *matrix       = ((TriStripData *)data)->matrix;
	         RwV3d           *vertices     = ((TriStripData *)data)->vertices;
	         RwImVertexIndex *currentIndex = mesh->indices;
 
	         RwIm3DVertex    *TriStrip;
	         RwUInt32         i;

	         TriStrip = (RwIm3DVertex *)RwMalloc(mesh->numIndices * sizeof(RwIm3DVertex), rwMEMHINTDUR_FRAME);
 

	         RpRandomSeed(mesh->numIndices);
	         Color.red   = (RwUInt8)(RpRandom() % 200) + 50;
	         Color.green = (RwUInt8)(RpRandom() % 200) + 50;
	         Color.blue  = (RwUInt8)(RpRandom() % 200) + 50;
	         Color.alpha = 255;

	         /* Process the index array, test for duplicate vertices */
	         for(i = 0; i < mesh->numIndices; i++, currentIndex++)
	         {
		         RwIm3DVertexSetPos( &TriStrip[i], 
							         vertices[*currentIndex].x,
							         vertices[*currentIndex].y,
							         vertices[*currentIndex].z );

		         RwIm3DVertexSetRGBA( &TriStrip[i],
							          Color.red,
							          Color.green,
							          Color.blue,
							          Color.alpha );
	         }

	         RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
	         RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *)TRUE);
	         RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *)TRUE);
	         RwRenderStateSet(rwRENDERSTATESRCBLEND, (void *)rwBLENDONE);
	         RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void *)rwBLENDZERO);

	         if( RwIm3DTransform(TriStrip, mesh->numIndices, matrix, 0) )
	         {                         
		         RwIm3DRenderPrimitive(rwPRIMTYPETRISTRIP); 
		         RwIm3DEnd();
	         }   

	         RwFree(TriStrip);

	         return mesh;
         }

         /*
         *  Render the geometry for a RpAtomic. If the geometry is
         *  tristripped, it is rendered untextured and flat shaded.
         */
         RpAtomic * AtomicRenderMeshes(RpAtomic * atomic, void * __RWUNUSED__ data)
         {
	         RpGeometry *geometry = RpAtomicGetGeometry(atomic);
	         TriStripData info;

	         info.matrix = RwFrameGetLTM(RpAtomicGetFrame(atomic));
	         info.vertices = RpMorphTargetGetVertices(RpGeometryGetMorphTarget(geometry, 0));
	         info.lengths = FALSE;

	         /* Is this tristripped? */
	         if(RpGeometryGetFlags(geometry) & rpGEOMETRYTRISTRIP)
	         {
		         /* Enumerate all meshes */
		         RpGeometryForAllMeshes(geometry, MeshRenderMeshes, &info);
	         }
	         else
	         {
		         /* Just render it. */
		         RpAtomicRender(atomic);
	         }

	         return atomic;
         }

         /*
         *  Render the given RpAtomic in wireframe, to display the polygon mesh.
         */
         RpAtomic * AtomicRenderWireMesh(RpAtomic *atomic, void * __RWUNUSED__ data)
         {
	         RpGeometry *geometry;

	         geometry = RpAtomicGetGeometry(atomic);

	         if( geometry )
	         {
		         RwReal interpPos = 1.0f, invInterpPos = 0.0f;
		         RpMorphTarget *morphTarget;
		         RwInt32 nkf;
		         RpTriangle *triangle;
		         RwV3d *vertPosStart = (RwV3d *)NULL;
		         RwV3d *vertPosEnd = (RwV3d *)NULL;
		         RwMatrix *LTM;
		         RwInt32 numTri, numImmVert, i;
		         RwIm3DVertex *imVertex;

		         nkf = RpGeometryGetNumMorphTargets(geometry);
		         numTri = RpGeometryGetNumTriangles(geometry);
		         imVertex = (RwIm3DVertex *)RwMalloc(numTri * 6 * sizeof(RwIm3DVertex), rwMEMHINTDUR_FRAME);

		         if( nkf > 1 )
		         {
			         RpInterpolator *interp;
			         RwInt32 startMorphTarget, endMorphTarget;

			         interp = RpAtomicGetInterpolator(atomic);

			         interpPos = RpInterpolatorGetValue(interp) / RpInterpolatorGetScale(interp);

			         invInterpPos = 1.0f - interpPos;

			         startMorphTarget = RpInterpolatorGetStartMorphTarget(interp);
			         endMorphTarget = RpInterpolatorGetEndMorphTarget(interp);

			         morphTarget = RpGeometryGetMorphTarget(geometry, startMorphTarget);
			         vertPosStart = RpMorphTargetGetVertices(morphTarget);

			         morphTarget = RpGeometryGetMorphTarget(geometry, endMorphTarget);
			         vertPosEnd = RpMorphTargetGetVertices(morphTarget);
		         }
		         else
		         {
			         morphTarget = RpGeometryGetMorphTarget(geometry, 0);
			         vertPosStart = RpMorphTargetGetVertices(morphTarget);
		         }

			         Color = Cyan;

		         triangle = RpGeometryGetTriangles(geometry);

		         for( i = 0; i < numTri; i++ )
		         {
			         RwUInt16 vert0, vert1, vert2;
			         RwV3d vertPos[3];

			         RpGeometryTriangleGetVertexIndices(geometry, triangle, &vert0, &vert1, &vert2);

			         if( nkf > 1 )
			         {
				         RwV3d tempVec1, tempVec2;

				         RwV3dScale(&tempVec1, &vertPosStart[vert0], invInterpPos);
				         RwV3dScale(&tempVec2, &vertPosEnd[vert0], interpPos);
				         RwV3dAdd(&vertPos[0], &tempVec1, &tempVec2);

				         RwV3dScale(&tempVec1, &vertPosStart[vert1], invInterpPos);
				         RwV3dScale(&tempVec2, &vertPosEnd[vert1], interpPos);
				         RwV3dAdd(&vertPos[1], &tempVec1, &tempVec2);

				         RwV3dScale(&tempVec1, &vertPosStart[vert2], invInterpPos);
				         RwV3dScale(&tempVec2, &vertPosEnd[vert2], interpPos);
				         RwV3dAdd(&vertPos[2], &tempVec1, &tempVec2);

			         }
			         else
			         {
				         vertPos[0] = vertPosStart[vert0];
				         vertPos[1] = vertPosStart[vert1];
				         vertPos[2] = vertPosStart[vert2];
			         }

			         RwIm3DVertexSetPos(&imVertex[i*6+0], vertPos[0].x, vertPos[0].y, vertPos[0].z);
			         RwIm3DVertexSetRGBA (&imVertex[i*6+0], Color.red, Color.green, Color.blue, Color.alpha);
			         RwIm3DVertexSetPos(&imVertex[i*6+1], vertPos[1].x, vertPos[1].y, vertPos[1].z);
			         RwIm3DVertexSetRGBA(&imVertex[i*6+1], Color.red, Color.green, Color.blue, Color.alpha);


			         RwIm3DVertexSetPos(&imVertex[i*6+2], vertPos[1].x, vertPos[1].y, vertPos[1].z);
			         RwIm3DVertexSetRGBA(&imVertex[i*6+2], Color.red, Color.green, Color.blue, Color.alpha);
			         RwIm3DVertexSetPos(&imVertex[i*6+3], vertPos[2].x, vertPos[2].y, vertPos[2].z);
			         RwIm3DVertexSetRGBA(&imVertex[i*6+3], Color.red, Color.green, Color.blue, Color.alpha);

			         RwIm3DVertexSetPos(&imVertex[i*6+4], vertPos[2].x, vertPos[2].y, vertPos[2].z);
			         RwIm3DVertexSetRGBA(&imVertex[i*6+4], Color.red, Color.green, Color.blue, Color.alpha);
			         RwIm3DVertexSetPos(&imVertex[i*6+5], vertPos[0].x, vertPos[0].y, vertPos[0].z);
			         RwIm3DVertexSetRGBA(&imVertex[i*6+5], Color.red, Color.green, Color.blue, Color.alpha);

			         triangle++;
		         }

		         RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
		         RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *)TRUE);
		         RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *)TRUE);

		         i = 0;
		         numImmVert = numTri * 6;
		         LTM = RwFrameGetLTM(RpAtomicGetFrame(atomic));
		         while (numImmVert > DISPATCHSIZE)
		         {
			         if( RwIm3DTransform(&imVertex[i], DISPATCHSIZE, LTM, 0) )
			         {
				         RwIm3DRenderPrimitive(rwPRIMTYPELINELIST);

				         RwIm3DEnd();
			         }

			         numImmVert -= DISPATCHSIZE;
			         i += DISPATCHSIZE;
		         }

		         if( RwIm3DTransform(&imVertex[i], numImmVert, LTM, 0) )
		         {
			         RwIm3DRenderPrimitive(rwPRIMTYPELINELIST);

			         RwIm3DEnd();
		         }

		         RwFree(imVertex);
	         }

	         return atomic;
         }

         /*
         *  Render lines representing the vertex normals for a RpAtomic.
         *  
         *  \param atomic Pointer to the atomic to render the normals for.
         *  \param data Pointer to a RwReal which gives the length of the
         *              line drawn to represent a normal.
         */
         RpAtomic * AtomicRenderVertexNormals(RpAtomic *atomic, void * data)
         {
	         RpGeometry *geometry;
            RwReal normalScaleFactor = 1.0f;

            if (data)
            {
               normalScaleFactor = *(RwReal*)data;
            }

	         geometry = RpAtomicGetGeometry(atomic);

	         if( geometry && (RpGeometryGetFlags(geometry) & rpGEOMETRYNORMALS) )
	         {
		         RpMorphTarget *morphTarget;
		         RwInt32 numVert, nkf, numImmVert;
		         RwV3d *vertPosStart = (RwV3d *)NULL;
		         RwV3d *vertPosEnd = (RwV3d *)NULL;
		         RwV3d *vertNormalStart = (RwV3d *)NULL;
		         RwV3d *vertNormalEnd = (RwV3d *)NULL;
		         RwReal interpPos = 1.0f, invInterpPos = 0.0f;
		         RwMatrix *LTM;
		         RwInt32 i;
		         RwIm3DVertex *imVertex;

		         nkf = RpGeometryGetNumMorphTargets(geometry);
		         numVert = RpGeometryGetNumVertices(geometry);

		         imVertex = (RwIm3DVertex *)RwMalloc(numVert * 2 * sizeof(RwIm3DVertex), rwMEMHINTDUR_FRAME);

		         if( nkf > 1 )
		         {
			         RpInterpolator *interp;
			         RwInt32 startMorphTarget, endMorphTarget;

			         interp = RpAtomicGetInterpolator(atomic);
			         interpPos = RpInterpolatorGetValue (interp) / RpInterpolatorGetScale (interp);

			         invInterpPos = 1.0f - interpPos;

			         startMorphTarget = RpInterpolatorGetStartMorphTarget(interp);
			         endMorphTarget = RpInterpolatorGetEndMorphTarget(interp);

			         morphTarget = RpGeometryGetMorphTarget(geometry, startMorphTarget);
			         vertPosStart = RpMorphTargetGetVertices(morphTarget);
			         vertNormalStart = RpMorphTargetGetVertexNormals(morphTarget);

			         morphTarget = RpGeometryGetMorphTarget(geometry, endMorphTarget);
			         vertPosEnd = RpMorphTargetGetVertices(morphTarget);
			         vertNormalEnd = RpMorphTargetGetVertexNormals(morphTarget);
		         }
		         else
		         {
			         morphTarget = RpGeometryGetMorphTarget(geometry, 0);
			         vertPosStart = RpMorphTargetGetVertices(morphTarget);
			         vertNormalStart = RpMorphTargetGetVertexNormals(morphTarget);
		         }

		         Color = Green;

		         for( i = 0; i < numVert; i++ )
		         {
			         RwV3d vertPos, vertNormal;

			         if( nkf > 1 )
			         {
				         RwV3d tempVec1, tempVec2;

				         RwV3dScale(&tempVec1, &vertPosStart[i], invInterpPos);
				         RwV3dScale(&tempVec2, &vertPosEnd[i], interpPos);
				         RwV3dAdd(&vertPos, &tempVec1, &tempVec2);

				         RwV3dScale(&tempVec1, &vertNormalStart[i], invInterpPos);
				         RwV3dScale(&tempVec2, &vertNormalEnd[i], interpPos);
				         RwV3dAdd(&vertNormal, &tempVec1, &tempVec2);
			         }
			         else
			         {
				         vertPos = vertPosStart[i];
				         vertNormal = vertNormalStart[i];
			         }

			         RwV3dScale(&vertNormal, &vertNormal, normalScaleFactor);

			         RwIm3DVertexSetPos(&imVertex[i*2+0], vertPos.x, vertPos.y, vertPos.z);
			         RwIm3DVertexSetRGBA(&imVertex[i*2+0], Color.red, Color.green, Color.blue, Color.alpha);

			         RwV3dAdd(&vertPos, &vertPos, &vertNormal);
			         RwIm3DVertexSetPos(&imVertex[i*2+1], vertPos.x, vertPos.y, vertPos.z);
			         RwIm3DVertexSetRGBA (&imVertex[i*2+1], Color.red, Color.green, Color.blue, Color.alpha);
		         }

		         RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
		         RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *)TRUE);
		         RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *)TRUE);

		         i = 0;
		         numImmVert = numVert * 2;
		         LTM = RwFrameGetLTM(RpAtomicGetFrame(atomic));
		         while (numImmVert > DISPATCHSIZE)
		         {
			         if( RwIm3DTransform(&imVertex[i], DISPATCHSIZE, LTM, 0) )
			         {
				         RwIm3DRenderPrimitive(rwPRIMTYPELINELIST);

				         RwIm3DEnd();
			         }

			         numImmVert -= DISPATCHSIZE;
			         i += DISPATCHSIZE;
		         }

		         if( RwIm3DTransform(&imVertex[i], numImmVert, LTM, 0) )
		         {
			         RwIm3DRenderPrimitive(rwPRIMTYPELINELIST);

			         RwIm3DEnd();
		         }

		         RwFree(imVertex);
	         }

	         return atomic;
         }

         // ------------------------------------------------------
         // --- Bounding Sphere ----------------------------------
         // ------------------------------------------------------

         RpAtomic * AtomicCompareBSphere(RpAtomic *atomic, void *data)
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

         RpAtomic *	AtomicAddBSphereCentre(RpAtomic *atomic, void *data)
         {
            RpGeometry *geometry;

            geometry = RpAtomicGetGeometry(atomic);

            if (geometry)
            {
               RwV3d center;
               RwMatrix *LTM;
               RpMorphTarget *morphTarget;
               RwInt32 i, numMorphTargets;
               RwV3d atomicCentre;
               RwSphere *clumpSphere;

               clumpSphere = (RwSphere *)data;

               /*
               * Establish the average center of this atomic over all morph targets
               */
               atomicCentre = Zero;

               numMorphTargets = RpGeometryGetNumMorphTargets (geometry);

               for( i = 0; i < numMorphTargets; i++ )
               {
                  morphTarget = RpGeometryGetMorphTarget(geometry, i);
                  center = RpMorphTargetGetBoundingSphere(morphTarget)->center;
                  RwV3dAdd(&atomicCentre, &atomicCentre, &center);
               }

               RwV3dScale(&atomicCentre, &atomicCentre, 1.0f / numMorphTargets);

               /*
               * Transform the average center of the atomic to world space
               */
               LTM = RwFrameGetLTM(RpAtomicGetFrame(atomic));
               RwV3dTransformPoints(&atomicCentre, &atomicCentre, 1, LTM);

               /*
               * Add the average center of the atomic up in order to calculate the center of the clump
               */
               RwV3dAdd(&clumpSphere->center, &clumpSphere->center, &atomicCentre);
            }

            return atomic;
         }

         /*
         *  Find the bounding sphere that contains all the RpAtomics in a RpClump.
         */
         void ClumpGetBoundingSphere(RpClump *clump, RwSphere *clumpSphere)
         {
            RwInt32 numAnimSamples;

            /*
            * First find the mean of all the atomics' bounding sphere centers.
            * All morph targets of all atomics and all frame animations are taken into account.
            * The result is the clump's bounding sphere center...
            */
            clumpSphere->center = Zero;
            numAnimSamples = 0;

            {
               RwSphere curClumpSphere;

               /*
               * average over morph targets and atomics
               */
               curClumpSphere.center = Zero;
               RpClumpForAllAtomics(clump, AtomicAddBSphereCentre, &curClumpSphere);

               RwV3dScale(&curClumpSphere.center, 
                          &curClumpSphere.center, 1.0f / RpClumpGetNumAtomics (clump));

               /*
               * Sum up the above average in order to calculate the overall
               * average over the frame animation...
               */
               RwV3dAdd(&clumpSphere->center,
                        &clumpSphere->center, &curClumpSphere.center);

               numAnimSamples++;

            }

            RwV3dScale(&clumpSphere->center, &clumpSphere->center, 1.0f / numAnimSamples);

            /*
            * Now, given the clump's bounding sphere center, determine the radius
            * by finding the greatest distance from the center that encloses all
            * the atomics' bounding spheres.  All morph targets, atomics and animations
            * are taken into account
            */
            clumpSphere->radius = -RwRealMAXVAL;
            numAnimSamples = 0;

            RpClumpForAllAtomics(clump, AtomicCompareBSphere, clumpSphere);

            numAnimSamples++;

            return;
         }

      } // namespace <anonymous>

      
      RWS_IMPLEMENT_CLASSID(ATBClumpView);
      RWS_REGISTERCLASS(ATBClumpView);

      /**
      *
      *  Create ATBClumpView object.
      *
      *  \param attr The standard attribute data used for initialization.
      *
      */
      ATBClumpView::ATBClumpView(const CAttributePacket& attr)
         : InitCEventHandler(&m_pClump),
         m_RenderPriority(65535),
         m_NormalsScaleFactor(1.0f),
         m_NumTriStripTriangles(1)
      {
         RWS_FUNCTION("ATBClumpView::ATBClumpView");
         
         m_pClump = CreateClumpInWorldFromResource(attr,this);
         RWS_ASSERT(m_pClump,"Failed to create atomic");

         RwSphere ClumpSphere;
         ClumpGetBoundingSphere(m_pClump, &ClumpSphere);
         m_NormalsScaleFactor = 0.15f * ClumpSphere.radius;

         RwMatrixSetIdentity(&m_Matrix);

         LinkMsg(iMsgRunningTick, 0);
         LinkMsg(m_StartRender);
         
         RWS_RETURNVOID();
      }
      
      /**
      *
      *  Destroy ATBClumpView object.
      *
      */
      ATBClumpView::~ATBClumpView(void)
      {
         RWS_FUNCTION("ATBClumpView::~ATBClumpView");
         
         UnLinkMsg(iMsgRunningTick);
         UnLinkMsg(m_StartRender);
         
         RWS_RETURNVOID();
      }      
      
      /**
      *
      *  Handle events.
      *  
      *  \param pMsg The standard RenderWare Studio message.
      *
      */
      void ATBClumpView::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("ATBClumpView::HandleEvents");
         
         RwFrame* pFrame = RpClumpGetFrame(m_pClump.ptr());
         
         if (pMsg.Id == iMsgRunningTick && m_bSpinToggle)
         {
            RwMatrixSetIdentity(&m_Matrix);
            RwMatrixRotate(&m_Matrix, &XAxis, m_Rot[0], rwCOMBINEPRECONCAT);
            RwMatrixRotate(&m_Matrix, &YAxis, m_Rot[1], rwCOMBINEPRECONCAT);
            RwMatrixRotate(&m_Matrix, &ZAxis, m_Rot[2], rwCOMBINEPRECONCAT);
            
            RwFrameTransform(pFrame, &m_Matrix, rwCOMBINEPRECONCAT);
         }

         if(pMsg.Id == m_StartRender)
         {
            RwCamera * pRenderCam = 0;

            RWS_ASSERT(pMsg.pData, "The RwCamera pointer sent with the render messages is 0.");

            pRenderCam = RwCameraBeginUpdate(reinterpret_cast<RwCamera *>(pMsg.pData));
            if (pRenderCam)
            {
               if (m_bStripShow)
               {
                  TriStripData info;
                  info.matrix = 0;
                  info.vertices = 0;
                  info.lengths = m_bStripShowLength;
                  info.numTriStripTriangles = m_NumTriStripTriangles;
                  RpClumpForAllAtomics(m_pClump, AtomicRenderTriStrip, (void *)&info);
               }

               if (m_bMeshShow)
               {
                  RpClumpForAllAtomics(m_pClump, AtomicRenderMeshes, (void *)FALSE);
               }

               if (m_bWireShow)
               {
                  RpClumpForAllAtomics(m_pClump, AtomicRenderWireMesh, (void *)FALSE);		
               }

               if (m_bNormalShow)
               {
                  RpClumpForAllAtomics(m_pClump, AtomicRenderVertexNormals, (void *)&m_NormalsScaleFactor);		
               }

               RwCameraEndUpdate(pRenderCam);
            }
         }
		         
         RWS_RETURNVOID();
      }      
      
      /**
      *
      *  Handle attributes.  This function receives the attribute values from RenderWare Studio.
      *
      *  \param attr The standard attribute packet.
      *
      */
      void ATBClumpView::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("ATBClumpView::HandleAttributes");
         
         CAttributeHandler::HandleAttributes(attr);
         
         m_pClump.HandleSystemCommands(attr);
         
         CAttributeCommandIterator attrIt(attr,RWS_CLASSID_OF(ATBClumpView));
         
         while(!attrIt.IsFinished())
         {
            switch(attrIt->GetCommandId())
            {
            case CMD_rot_x:
               attrIt->GetCommandData(m_Rot[0]);
               m_Rot[0] = RWDEG2RAD(m_Rot[0]);
               break;
               
            case CMD_rot_y:
               attrIt->GetCommandData(m_Rot[1]);
               m_Rot[1] = RWDEG2RAD(m_Rot[1]);
               break;
               
            case CMD_rot_z:
               attrIt->GetCommandData(m_Rot[2]);
               m_Rot[2] = RWDEG2RAD(m_Rot[2]);
               break;

            case CMD_StripSize:
               attrIt->GetCommandData(m_NumTriStripTriangles);
               break;

            case CMD_SpinToggle:
               m_bSpinToggle = attrIt->GetAs_RwUInt32() ? TRUE : FALSE;
               break;

            case CMD_MeshShow:
               m_bMeshShow = attrIt->GetAs_RwUInt32() ? TRUE : FALSE;
               break;

            case CMD_WireShow:
               m_bWireShow = attrIt->GetAs_RwUInt32() ? TRUE : FALSE;
               break;

            case CMD_NormalShow:
               m_bNormalShow = attrIt->GetAs_RwUInt32() ? TRUE : FALSE;
               break;

            case CMD_ScaleFactor:
               attrIt->GetCommandData(m_NormalsScaleFactor);
               break;

            case CMD_StripShow:
               m_bStripShow = attrIt->GetAs_RwUInt32() ? TRUE : FALSE;
               break;

            case CMD_StripShowLength:               
               m_bStripShowLength = attrIt->GetAs_RwUInt32() ? TRUE : FALSE;
               break;

            case CMD_Start_Render:
               {
                  UnLinkMsg(m_StartRender);
                  UnRegisterMsg(m_StartRender);

                  RegisterMsg(m_StartRender, attrIt->GetAs_RwChar_ptr(), "RwCamera*");
                  LinkMsg(m_StartRender, "RwCamera*", m_RenderPriority);
               }
               break;

            case CMD_RenderPriority:
               m_RenderPriority = attrIt->GetAs_RwUInt32();

               // Relink to the message at the new priority
               UnLinkMsg(m_StartRender);
               LinkMsg(m_StartRender, "RwCamera*", m_RenderPriority);
               break;

            }
            ++attrIt;
         }
         
         RWS_RETURNVOID();
      }

   } // namespace ToolBox

} // namespace RWS