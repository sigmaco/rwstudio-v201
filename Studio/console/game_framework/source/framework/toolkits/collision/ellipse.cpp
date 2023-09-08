/*****************************************************************************
*
* File :     ellipse.cpp
*
* Abstract : Collision detection ellipse
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

//////////////////////////////////////////////////////////////////
//
// Renderwarestudio Includes
//
#include "collide.h"
#include "framework/toolkits/world/clevel.h"
#include "framework/toolkits/debugtools/debugtools.h"
#include "framework/core/attributehandler/csystemcommands.h"
#include "framework/core/macros/debugmacros.h"

namespace RWS
{
   namespace CCollide
   {
      namespace CEllipsoid
      {
         /*
         *  \ref WorldTriangleIntersectCB called for each triangle in the world which
         *  intersects the intersection primitive.
         *
         *  \param intersection Pointer to an \ref RpIntersection object,
         *  the definition of the collision primitive e.g. a sphere
         *
         *  \param sector Pointer to an \ref RpWorldSector object, the 
         *  sector containing this collision.
         *
         *  \param collTriangle Pointer to an \ref RpCollisionTriangle object,
         *  the triangle which generated this collision. 
         *
         *  \param distance Distance from collision point to collision geometry.
         *  \param data Pointer to custom collision data.
         *
         *  \returns Pointer to the \ref RpCollisionTriangle object passed into the function.
         */
         RpCollisionTriangle *WorldTriangleIntersectCB
            (
            RpIntersection * intersection,  
            RpWorldSector * sector,
            RpCollisionTriangle * collTriangle,
            RwReal distance,
            void *data)
         {
            struct nearest_collision_tag *nc = (struct nearest_collision_tag*)data;
            
            RwV3d point = intersection->t.sphere.center;   // Note this point is in world co-ordinates
            
            RwBool onground = false;
            
            // Determine the shortest distance squared from the collision geometry to the triangle
            // note this includes the edges of the triangle, note point is modified.
            //
            RwReal tridistance = TrianglePointNearestPoint(
               collTriangle->vertices,    // Pointer to 3 vertices
               &collTriangle->normal, // Address of transformed normal
               &point);       // Point to test against
            
            RwV3d delta;
            RwReal Erad;
            
            // delta is the difference between the center of the sphere/ellipse and the intersection point
            //
            RwV3dSub(&delta, &intersection->t.sphere.center, &point);  
            
            // Check if the point of collision is below the ground level, specified by nc->gndradius
            //
            if (delta.y>nc->gndradius)
            {
               onground = true;
            }
            
            // Test within ellipse specified by E.x, E.y, E.z
            //
            delta.x = delta.x * delta.x * nc->E.x;
            delta.y = delta.y * delta.y * nc->E.y;
            delta.z = delta.z * delta.z * nc->E.z;
            
            Erad = (delta.x + delta.y + delta.z);
            
            if ( Erad < 1.0f )
            {
               // Yes
               //
               if (onground)
               {
                  *nc->onground = true;
#ifdef RWS_DEBUGTOOLS
                  DisplayCollisionTriangle(collTriangle->vertices,
                     &collTriangle->normal, 0,
                     (RwChar)255, 0);
#endif
               }
               else
               {
#ifdef RWS_DEBUGTOOLS
                  DisplayCollisionTriangle(collTriangle->vertices,
                     &collTriangle->normal, (RwChar)255,
                     0, 0);
#endif
               }
               
               nc->distance = tridistance;
               nc->point = point;
               nc->Erad = Erad;
               //
               //
               //
               RwV3d delta;
               RwV3d delta2;
               RwV3d v;
               
               // Calculate a vector from ellipsoid center to collision point
               //
               RwV3dSub(&delta, &intersection->t.sphere.center, &nc->point);  
               
               RwReal iErad;
               
               iErad = 1.0f / (nc->Erad);
               
               rwSqrt( &iErad , iErad );
               
               delta2.x = delta.x * iErad;
               delta2.y = delta.y * iErad;
               delta2.z = delta.z * iErad;
               
               RwV3dSub(&v, &delta2, &delta);  
               
               // If on the ground, then friction applies, need to determine the components of the force
               // along the plane (v.x, v.z) direction and vertically (v.y)
               //
               if (onground)
               {
                  // v represents the vector which will place the ellipsoid the correct distance from
                  // the collision plane.
                  //
                  // need to apply the v.y component, but scale the v.x & v.z components by friction
                  v.x = 0;
                  v.z = 0;
               }
               
               // Modify the frame
               //
               RwFrameTranslate (nc->pFrame, &v, rwCOMBINEPOSTCONCAT);
               
               RwFrameUpdateObjects(nc->pFrame);
               
               intersection->t.sphere.center = *RwMatrixGetPos(RwFrameGetLTM(nc->pFrame));
               
               if (nc->pcenter_offset)
               {
                  RwV3dAdd(&intersection->t.sphere.center, &intersection->t.sphere.center, nc->pcenter_offset);
               }
            }
            
            return collTriangle;
         }

         /*
         *
         *  \ref AtomicTriangleIntersectCB called for each triangle in the atomic which
         *  intersects the intersection primitive.
         *
         *  \param intersection Pointer to an \ref RpIntersection object, the definition
         *  of the collision primitive e.g. a sphere
         *
         *  \param collTriangle Pointer to an \ref RpCollisionTriangle object, the triangle
         *  which generated this collision. 
         *
         *  \param distance Distance from collision point to collision geometry.
         *
         *  \param data Pointer to custom collision data.
         *
         *  \returns Pointer to the \ref RpCollisionTriangle object passed into the function.
         *
         */
         RpCollisionTriangle *AtomicTriangleIntersectCB
         (
          RpIntersection * intersection,
          RpCollisionTriangle * collTriangle,
          RwReal distance,
          void *data)
         {
            RWS_FUNCTION("RWS::CCollide::CEllipsoid::AtomicTriangleIntersectCB");

            struct nearest_collision_tag *nc = (struct nearest_collision_tag*)data;
   
            RwV3d ivertices[3];     // Geometry vertices x, y, z and normal of surface
            RwV3d onormal;          // Transformed Normal
   
            static RwV3d overtices[3];     // Transformed Vertices 
   
            // Once only initialization, so overtices and povertices need to be static
            //
            static RwV3d *povertices[3] = 
            {
               &overtices[0],
               &overtices[1],
               &overtices[2]
            };   // Pointer to Transformed Vertices
   
            RwV3d point = intersection->t.sphere.center; // Note this point is in world co-ordinates
   
            ivertices[0] = *collTriangle->vertices[0];
            ivertices[1] = *collTriangle->vertices[1];
            ivertices[2] = *collTriangle->vertices[2];

            RWS_TRACE("ivertices[0] ["<<ivertices[0].x<<" "<<ivertices[0].y<<" "<<ivertices[0].z<<"]");
            RWS_TRACE("ivertices[1] ["<<ivertices[1].x<<" "<<ivertices[1].y<<" "<<ivertices[1].z<<"]");
            RWS_TRACE("ivertices[2] ["<<ivertices[2].x<<" "<<ivertices[2].y<<" "<<ivertices[1].z<<"]");
   
            // Need to apply forward transform to vertices and face normal
            //
            RwV3dTransformPoints(overtices, ivertices, 3, nc->matrix);

            RWS_TRACE("overtices[0] ["<<overtices[0].x<<" "<<overtices[0].y<<" "<<overtices[0].z<<"]");
            RWS_TRACE("overtices[1] ["<<overtices[1].x<<" "<<overtices[1].y<<" "<<overtices[1].z<<"]");
            RWS_TRACE("overtices[2] ["<<overtices[2].x<<" "<<overtices[2].y<<" "<<overtices[1].z<<"]");
            
            // Need to Apply Rotation to normal
            //
            RwV3dTransformVectors(&onormal, &collTriangle->normal, 1, nc->norm_matrix);
   
            RwBool onground = false;
   
            // Determine the shortest distance squared from the collision geometry to the triangle
            // note this includes the edges of the triangle.
            //
            RwReal tridistance = TrianglePointNearestPoint(
               povertices,    // Pointer to 3 vertices
               &onormal, // Address of transformed normal
               &point);       // Point to test against
   
            RwV3d delta;
            RwReal Erad;
   
            // Vector from collision center to collision point.
            //
            RwV3dSub(&delta, &intersection->t.sphere.center, &point);  

            RWS_TRACE("delta ["<<delta.x<<"]["<<delta.y<<"]["<<delta.z<<"]");
   
            // Check if collision can be treated as ground
            //
            if (delta.y > nc->gndradius && RpAtomicCollisionProperties::GetIsSolid(*nc->pRpAtomicCollide))
            {
               onground = true;
            }
   
            delta.x = delta.x * delta.x * nc->E.x;
            delta.y = delta.y * delta.y * nc->E.y;
            delta.z = delta.z * delta.z * nc->E.z;

            Erad = (delta.x + delta.y + delta.z);

            RWS_TRACE("Erad ["<<Erad<<"]");
   
         #ifdef RWS_DEBUGTOOLS
            if (DebugSwitches::GetSwitch(DebugSwitches::m_enable_collision_debug_info))
            {
               DebugTools::Line(&intersection->t.sphere.center, &point,0,0,255);
            }
         #endif
   
            if ( Erad < 1.0f )
            {
               nc->did_collide = true;
      
               // Check if solid collisions are required, solid collision repel
               // the collision hull.
               //
               if (nc->pRpAtomicCollide)
               {
                  if (!RpAtomicCollisionProperties::GetIsSolid(*nc->pRpAtomicCollide))
                  {
                     RWS_RETURN( 0 ); // Job done, if not solid exit after first collision triangle
                  }
                  else
                  {
                     if (onground)
                     {
                        *nc->onground = true;
               
                        // This is the entity that is considered the ground
                        //
                        nc->pCGndCEventHandler = nc->pCEventHandlerCollide;
                        nc->pGndRpAtomic = nc->pRpAtomicCollide;
         #ifdef RWS_DEBUGTOOLS
                        DisplayCollisionTriangle(povertices, &onormal, 0, (RwChar)255, 0);
         #endif
                     }
                     else
                     {
         #ifdef RWS_DEBUGTOOLS
                        DisplayCollisionTriangle(povertices, &onormal, (RwChar)255, 0, 0);
         #endif
                     }
            
                     nc->distance = tridistance;
                     nc->point = point;
                     nc->Erad = Erad;
            
                     RwV3d delta;
                     RwV3d delta2;
                     RwV3d v;
            
                     // Calculate a vector from ellipsoid center to collision point
                     //
                     RwV3dSub(&delta, &intersection->t.sphere.center, &nc->point);  

                     RwReal iErad;
            
                     iErad = 1.0f / (nc->Erad);
            
                     rwSqrt(&iErad, iErad);
            
                     delta2.x = delta.x * iErad;
                     delta2.y = delta.y * iErad;
                     delta2.z = delta.z * iErad;

                     RWS_TRACE("delta ["<<delta.x<<"]["<<delta.y<<"]["<<delta.z<<"]");
                     RWS_TRACE("delta2 ["<<delta2.x<<"]["<<delta2.y<<"]["<<delta2.z<<"]");
                     
                     RwV3dSub(&v, &delta2, &delta);  
            
                     RWS_TRACE("v ["<<v.x<<"]["<<v.y<<"]["<<v.z<<"]");

                     // If on the ground, then friction should apply
                     //
                     if (onground)
                     {
                        // v represents the vector which will place the ellipsoid the correct distance from
                        // the collision plane.
                        //
                        // need to apply the v.y component, but scale the v.x & v.z components by friction
                        v.x = 0;
                        v.z = 0;
                     }
            
                     // Check if need to work in local co-ordinate space not world space
                     //
                     if (RwFrameGetParent(nc->pFrame))
                     {
                        RwV3dTransformVectors(&v,
                           &v,
                           1,
                           nc->imatrix);
                     }
            
                     // Modify the frame
                     //
                     RwFrameTranslate (nc->pFrame, &v, rwCOMBINEPOSTCONCAT);
            
                     RwFrameUpdateObjects(nc->pFrame);
            
                     intersection->t.sphere.center = *RwMatrixGetPos(RwFrameGetLTM(nc->pFrame));
            
                     if (nc->pcenter_offset)
                     {
                        RwV3dAdd(&intersection->t.sphere.center, &intersection->t.sphere.center, nc->pcenter_offset);
                     }
                  }
               }
            }
            RWS_RETURN(collTriangle);
         }

         /*
         *
         *  \ref AtomicBoundryIntersectCB called if the boundary of the atomic which
         *  intersects the intersection primitive.
         *
         *  \param intersection Pointer to an \ref RpIntersection object, the definition
         *  of the collision primitive e.g. a sphere
         *
         *  \param sector Pointer to an \ref RpWorldSector object, the sector containing
         *  this collision.
         *
         *  \param atomic Pointer to an \ref RpAtomic object, the atomic thats boundary
         *  has been intersected.
         *
         *  \param distance Distance from collision point to collision geometry.
         *  \param data Pointer to custom collision data.
         *
         *  \returns Pointer to the \ref RpAtomic object passed into the function.
         *
         */
         RpAtomic *AtomicBoundryIntersectCB
         (
          RpIntersection * intersection,
          RpWorldSector * sector,
          RpAtomic * atomic,
          RwReal distance,
          void *data)
         {
            RWS_FUNCTION("RWS::CCollide::CEllipsoid::AtomicBoundryIntersectCB");

            RWS_PRE(atomic);
   
            struct nearest_collision_tag *nc = (struct nearest_collision_tag*)data;
   
            CEventHandler *pCEventHandler = RpAtomicToEventHandler::GetEventHandler(*atomic);
   
            // Need to check if its a different event handler to the one that requested the test
            //
            if (nc->pCEventHandler != pCEventHandler)  
            {
               nc->matrix = RwFrameGetLTM( RpAtomicGetFrame ( atomic ) );

               nc->norm_matrix = RwMatrixCreate(); // normalized forward matrix

               RwMatrixOrthoNormalize (nc->norm_matrix, nc->matrix);

               nc->imatrix = RwMatrixCreate(); // Inverse matrix
               RwMatrixInvert (nc->imatrix, nc->matrix );
      
               nc->pCEventHandlerCollide = pCEventHandler;
               nc->pRpAtomicCollide = atomic;
               nc->did_collide = false;
      

               const RwSphere     *boundingSphere;
               boundingSphere = RpAtomicGetWorldBoundingSphere(atomic);

               RWS_TRACE("boundingSphere ["
                  <<boundingSphere->center.x
                  <<" "
                  <<boundingSphere->center.y
                  <<" "
                  <<boundingSphere->center.z
                  <<" "
                  <<boundingSphere->radius
                  <<"]");

               // Check polygon collisions with the atomic
               //
               RpAtomicForAllIntersections(atomic, intersection, AtomicTriangleIntersectCB,nc);
      
               RwMatrixDestroy(nc->norm_matrix);
               RwMatrixDestroy(nc->imatrix);
      
               // If a polygon collision occurred send a message, if any to the entity
               //
               if (nc->did_collide)
               {
                  if (nc->CMsg) {
                     nc->pCEventHandler->SendMsgToEventHandler(*(nc->CMsg), pCEventHandler);
                  }
               }
            }
   
            RWS_RETURN( atomic );
         }

         /**
         *  CalcCollision is a high level collision detection function that performs
         *  a common form of game collision detection. An ellipse is specified by the
         *  RwFrame *CollidingObjectFrame providing a center point, and RwV3d *EllipseDimension
         *  which provides the dimension of the ellipse in each axis X, Y and Z. The center
         *  point can be offset using RwV3d *CenterOffset, this allows the center of the
         *  ellipse to be different to the center of the colliding object whose position
         *  is determined by the RwFrame *CollidingObjectFrame.\n
         *
         *  \note The ellipse collision calculation is an approximation, it first intersects
         *  the collision geometry with a sphere from this it calculates the closest point on
         *  the triangle to the center of the sphere. Finally it tests that this point is within
         *  the ellipse. As the ellipse becomes less like a sphere for example tall and thin this
         *  approximation breaks down.
         *
         *  CalcCollision can also be used to determine if the intersection with the ellipse is 
         *  with a wall or the ground. If the intersection point is below RwReal GndHeight then
         *  the RwBool *OnGround flag is set indicating that the ellipse is intersecting
         *  the ground.\n
         *
         *  If a ground collision occurs with another frame based object i.e. an Atomic, then
         *  the RwFrame *CollidingObjectFrame is modified making it relative to the Atomic
         *  i.e. it becomes a child frame. This is useful for effects such as moving platforms,
         *  and lifts where the colliding object needs to move when the parent object is moved.\n
         *
         *  Events can also be generated by the intersection test by providing CEventHandler *pCEventHandler
         *  and CMsg *CMsg. If an intersection with an object which is controlled by an event handler occurs
         *  CMsg is sent to that event handler.
         *
         *  \param pCEventHandler Pointer to the CEventHandler performing the test.
         *
         *  \param pCMsg Pointer to a CMsg sent to any atomics which collide during this test.
         *
         *  \param CollidingObjectFrame Pointer to the frame which the test is applied to.
         *
         *  \param center_offset Vector used to offset the center of the frame, used as the center of the ellipse.
         *
         *  \param GndHeight Distance from center of ellipse used to specify if the ellipse has collided
         *  with a "ground" triangle or a "wall" triangle.
         *
         *  \param OnGround Pointer to an flag filled in with true if an intersection with a ground triangle occurs
         *  or a wall triangle.
         *
         *  \param EllipseDimension A pointer to a vector used to specify the size of the ellipse on each of its axis.
         *
         */
         void CalcCollision(
            CEventHandler *pCEventHandler,
            CMsg *CMsg,
            RwFrame *CollidingObjectFrame,
            RwV3d *CenterOffset,
            RwReal GndHeight,
            RwBool *OnGround,
            RwV3d *EllipseDimension)
         {
            RWS_FUNCTION("RWS::CCollide::CEllipsoid::CalcCollision");
            RWS_PRE(CLevel::GetOpaqueWorld());
            RWS_PRE(OnGround);
            RWS_PRE(CollidingObjectFrame);
            RWS_PRE(EllipseDimension);
   
            // Store the nearest collision that occurs here
            //
            nearest_collision nc;
   
            // Collision detections, sphere to world collision
            //
            RpIntersection is;
   
            // Setup spherical collision test
            //
            is.type = rpINTERSECTSPHERE;
            is.t.sphere.center = *RwMatrixGetPos(RwFrameGetLTM(CollidingObjectFrame));
   
            if (CenterOffset)
            {
               RwV3dAdd(&is.t.sphere.center, &is.t.sphere.center, CenterOffset);
            }
   
            // Select dominant axis, 
            //
            if (EllipseDimension->x > EllipseDimension->y && EllipseDimension->x > EllipseDimension->z) 
            {
               is.t.sphere.radius  = EllipseDimension->x;
            }
            else
            if (EllipseDimension->y > EllipseDimension->x && EllipseDimension->y > EllipseDimension->z) 
            {
               is.t.sphere.radius  = EllipseDimension->y;
            }
            else 
            {
               is.t.sphere.radius  = EllipseDimension->z;
            }
   
            // Determine radius of sphere squared
            //
            RwReal distancepow2 = is.t.sphere.radius * is.t.sphere.radius;
   
            nc.pcenter_offset = CenterOffset;
            nc.CMsg = CMsg;
            nc.distance = distancepow2;
            nc.imatrix = 0;
            nc.matrix = 0;
            nc.pCEventHandler = pCEventHandler;
            nc.pFrame = CollidingObjectFrame;
   
            nc.E.x = 1.0f / ( EllipseDimension->x * EllipseDimension->x ); // Pre divide so we can use multiply later
            nc.E.y = 1.0f / ( EllipseDimension->y * EllipseDimension->y );
            nc.E.z = 1.0f / ( EllipseDimension->z * EllipseDimension->z );
   
            nc.gndradius = GndHeight;
            nc.onground = OnGround;
            nc.pCGndCEventHandler = 0;
            nc.pGndRpAtomic = 0;
   
         #ifdef RWS_DEBUGTOOLS
            if (DebugSwitches::GetSwitch(DebugSwitches::m_enable_collision_debug_info))
            {
               RwV3d v[4];
      
               DebugTools::Ellipsoid(&is.t.sphere.center,
                  EllipseDimension->x,
                  EllipseDimension->y,
                  EllipseDimension->z, 0, 128, 128);
      
               v[0] = 
               v[1] = 
               v[2] = 
               v[3] = is.t.sphere.center;
      
               v[0].x -= EllipseDimension->x;
               v[0].z -= EllipseDimension->z;
      
               v[1].x += EllipseDimension->x;
               v[1].z -= EllipseDimension->z;
      
               v[2].x += EllipseDimension->x;
               v[2].z += EllipseDimension->z;
      
               v[3].x -= EllipseDimension->x;
               v[3].z += EllipseDimension->z;
      
               v[0].y -= GndHeight;
               v[1].y -= GndHeight;
               v[2].y -= GndHeight;
               v[3].y -= GndHeight;
      
               DebugTools::Triangle(&v[0], &v[1], &v[2], 0, 0, 0);
               DebugTools::Triangle(&v[2], &v[1], &v[0], 255, 255, 255);
      
               DebugTools::Triangle(&v[2], &v[3], &v[0], 0, 0, 0);
               DebugTools::Triangle(&v[0], &v[3], &v[2], 255, 255, 255);
            }
         #endif
   
            if (CLevel::GetOpaqueWorld())
            {
               // Collide with atomics first
               RpWorldForAllAtomicIntersections(CLevel::GetOpaqueWorld(), &is, AtomicBoundryIntersectCB, &nc);
   
               // Fill in nc with nearest collision
               //
               RpCollisionWorldForAllIntersections(CLevel::GetOpaqueWorld(), &is, WorldTriangleIntersectCB, &nc);
            }

            if (CLevel::GetTranslucentWorld())
            {
               // Collide with atomics first
               RpWorldForAllAtomicIntersections(CLevel::GetTranslucentWorld(), &is, AtomicBoundryIntersectCB, &nc);
   
               // Fill in nc with nearest collision
               //
               RpCollisionWorldForAllIntersections(CLevel::GetTranslucentWorld(), &is, WorldTriangleIntersectCB, &nc);
            }

            // Check if need to link frames to a parent.
            //
            if (nc.pCGndCEventHandler != 0) // Link to that entity's frame
            {
               // Get the atomic of the entity the collision occurred with
               //
               RpAtomic *pAtomic = nc.pGndRpAtomic;//nc.pCGndCEventHandler->GetAtomic();
      
               // Get the frame of the atomic have collided with
               //
               if (pAtomic)
               {
                  RwFrame *entframe = RpAtomicGetFrame(pAtomic);
         
                  // Attach to this frame if not done already
                  //
                  if (RwFrameGetParent(CollidingObjectFrame) != entframe)
                  {
                     RwMatrix *inverseLTM = RwMatrixCreate();
            
                     // If already linked to something remove the link
                     //
                     if (RwFrameGetParent(CollidingObjectFrame))
                     {
                        RwMatrixCopy(RwFrameGetMatrix(CollidingObjectFrame), RwFrameGetLTM(CollidingObjectFrame));
                        RwFrameRemoveChild(CollidingObjectFrame);
                     }
            
                     rwMatrixInitialize(inverseLTM, rwMATRIXTYPENORMAL);
            
                     RwMatrixInvert(inverseLTM, RwFrameGetLTM(entframe));
            
                     RwFrameTransform(CollidingObjectFrame, inverseLTM,rwCOMBINEPOSTCONCAT);
            
                     RwFrameAddChild(entframe, CollidingObjectFrame);
            
                     RwMatrixDestroy(inverseLTM);
                  }
               }
            }
            else  // Link back to the world entity, i.e. absolute position
            {
               if (RwFrameGetParent(CollidingObjectFrame))
               {
                  RwMatrixCopy(RwFrameGetMatrix(CollidingObjectFrame), RwFrameGetLTM(CollidingObjectFrame));
                  RwFrameRemoveChild(CollidingObjectFrame);
               }
            }
            RWS_RETURNVOID();
         }
      }//namespace CEllipsoid
   }//namespace CCollide
}//namespace RWS
