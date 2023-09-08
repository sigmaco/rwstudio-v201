/*****************************************************************************
 *
 * File :     collide.h
 *
 * Abstract : Collision detection behavior, very incomplete
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

#ifndef __GFCOLLIDE_H__
#define __GFCOLLIDE_H__

//////////////////////////////////////////////////////////////////
//
// RenderWare Includes
//
#include <rwcore.h>
#include <rpcollis.h>
#include <rtintsec.h>

#ifdef _MSC_VER
#pragma comment (lib, "rwcore.lib")
#pragma comment (lib, "rpcollis.lib")
#pragma comment (lib, "rtintsec.lib")
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework\core\eventhandler\ceventhandler.h"

namespace RWS
{
   /**
   *
   *  \ingroup CollisionToolkit
   *
   *  CCollide provides utility functions used by each of the collision methods.
   *  The collision toolkit is based on the RenderWare plugins rpcollis and rtintsec.
   *  
   *
   */
   namespace CCollide
   {
      void DisplayCollisionTriangle(RwV3d *pVertices[3], RwV3d *pNormal, const RwUInt8 uiRed, const RwUInt8 uiGreen, const RwUInt8 uiBlue);

      RwReal TrianglePointNearestPoint(RwV3d * vpaVertices[3], RwV3d * vpNormal, RwV3d * vpPt);
      /**
      *
      *  \ingroup CollisionToolkit
      *
      *  CEllipsoid provides functionality for performing ellipsoid collision detection.
      *
      */
      namespace CEllipsoid
      {
         /**
         *
         * \ingroup CollisionToolkit
         *
         * Data structure used by the CEllipsoid intersection functions.
         *
         * \see WorldTriangleIntersectCB
         * \see AtomicTriangleIntersectCB
         * \see AtomicBoundryIntersectCB
         *
         */

         typedef struct nearest_collision_tag
         {
            CEventHandler *pCEventHandler;        /**< Pointer to the EventHandler testing for a collision. */
            CEventHandler *pCEventHandlerCollide; /**< Pointer to the EventHandler with which a collision is being tested. */

            RpAtomic *pRpAtomicCollide;         /**< Atomic of instance. */
            class CMsg *CMsg;                   /**< Message to send. */
            RwFrame *pFrame;                    /**< center position of collision object. */
            RwMatrix *matrix;                   /**< Forward transformation matrix of atomic, atomics local transformation. */
            RwMatrix *norm_matrix;              /**< Normalized forward transformation matrix of atomic, atomics local transformation. */
            RwMatrix *imatrix;                  /**< Inverse matrix of atomic. */
            RwReal distance;                    /**< Minimum distance. */
            RwV3d point;                        /**< Point of intersection. */
            RwV3d E;                            /**< Used by ellipse check. */
            RwReal Erad;                        /**< Result of ellipse equations. */
            RwReal gndradius;                   /**< Ellipse collision, collisions below this height are considered to be with the ground and set the onground flag. */
            RwBool *onground;                   /**< Flag specifying if a ground collision occurred. */
            CEventHandler *pCGndCEventHandler;  /**< If On Ground is set then this is the entity that caused the collision. */
            RpAtomic *pGndRpAtomic;             /**< If On Ground this is the RpAtomic considered the ground. */

            RwBool did_collide;                 /**< Specifies whether a polygon collision did occur or not. */
            RwV3d *pcenter_offset;              /**< Offset for center of collision. */
         } nearest_collision;

         RpCollisionTriangle *WorldTriangleIntersectCB(
             RpIntersection * intersection,
             RpWorldSector * sector,       
             RpCollisionTriangle * collTriangle,
             RwReal distance,
             void *data);

         RpCollisionTriangle *AtomicTriangleIntersectCB(
             RpIntersection * intersection,
             RpCollisionTriangle * collTriangle,
             RwReal distance __RWUNUSED__,
             void *data);

         RpAtomic *AtomicBoundryIntersectCB(
             RpIntersection * intersection,
             RpWorldSector * sector,       
             RpAtomic * atomic,
             RwReal distance,
             void *data);

         //
         // (x^2)+(y^2)+(z^2)<radius
         //
         // And
         //
         // (x^2)/(E.x^2) + (y^2/E.y^2) + (z2/E/z^2) = 1
         //
         void CalcCollision(
            CEventHandler *pCEventHandler,
            CMsg *CMsg,
            RwFrame *CollidingObjectFrame,
            RwV3d *CenterOffset,
            RwReal GndHeight,
            RwBool *OnGround,
            RwV3d *EllipseDimension);
      }
   }//namespace CCollide
}//namespace RWS
#endif
