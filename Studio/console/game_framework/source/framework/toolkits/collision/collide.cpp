/*****************************************************************************
*
* File :     collide.cpp
*
* Abstract : Collision detection behavior
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
//////////////////////////////////////////////////////////////////
//
// Renderwarestudio Includes
//
#include "collide.h"
#include "framework/toolkits/debugtools/debugtools.h"

namespace RWS
{
   namespace CCollide
   {
#ifdef RWS_DEBUGTOOLS
      /**
      *  \ingroup CollisionToolkit
      *  Debug function used to display collision polygons and normals, note normals are scaled by size of triangle.
      *  
      *  \param vpaVertices Pointer to array of 3 RwV3d vertices representing the 3 corners of the triangle.
      *  \param normal Pointer to normal of triangle.
      *  \param uiRed Red color value range 0 to 255.
      *  \param uiGreen Green color value range 0 to 255.
      *  \param uiBlue Blue color value range 0 to 255.
      */
      void DisplayCollisionTriangle(RwV3d *pVertices[3], RwV3d *pNormal, const RwUInt8 uiRed, const RwUInt8 uiGreen, const RwUInt8 uiBlue)
      {
         if(DebugSwitches::GetSwitch(DebugSwitches::m_enable_collision_debug_info))
         {
            RwV3d center;           // Center of triangle
            RwV3d centerpnormal;    // Center of triangle plus normal
            RwUInt32 i;
            
            // Find center of the polygon
            //
            //
            center = *pVertices[0];
            
            for(i = 1;i < 3;i++)
            {
               RwV3dAdd(&center, &center, pVertices[i]);
            }
            
            RwV3dScale(&center, &center, (1.0f/3.0f));
            
            RwV3d delta01;
            RwV3d delta12;
            RwV3d delta20;
            
            RwV3dSub(&delta01, pVertices[0], pVertices[1]);
            RwV3dSub(&delta12, pVertices[1], pVertices[2]);
            RwV3dSub(&delta20, pVertices[2], pVertices[0]);
            
            RwReal l01 = RwV3dLength(&delta01);
            RwReal l12 = RwV3dLength(&delta12);
            RwReal l20 = RwV3dLength(&delta20);
            
            RwReal avgl = (l01 + l12 + l20) / (3.0f*2.0f); // Use half the average legth of the sides to scale the normal.
            
            if(pNormal)
            {
               RwV3d scaled_normal;
               
               RwV3dScale(&scaled_normal, pNormal, avgl);
               
               centerpnormal = center;
               
               RwV3dAdd(&centerpnormal, &centerpnormal, &scaled_normal);
               
               DebugTools::Line(&centerpnormal, &center, 255, 255, 255);
            }
            
            DebugTools::Triangle(pVertices[0], pVertices[1], pVertices[2], uiRed, uiGreen, uiBlue);
            
            DebugTools::Line(pVertices[0], pVertices[1], 255, 0, 0);
            DebugTools::Line(pVertices[1], pVertices[2], 255, 0, 0);
            DebugTools::Line(pVertices[2], pVertices[0], 255, 0, 0);
         }
      }
#endif

#define RWS_CONSTREALASINT(r)   ( ((const RwSplitBits *)&(r))->nInt )
#define RWS_REALSIGNSNEQ(a, b)  ( (RWS_CONSTREALASINT(a) ^ RWS_CONSTREALASINT(b)) < 0 )
      
#define RWS_TRIAREA(vpA, vpB, vpC, _X, _Y)                                   \
   ( (((vpB)->_X) - ((vpA)->_X)) * (((vpC)->_Y) - ((vpA)->_Y)) -        \
      (((vpB)->_Y) - ((vpA)->_Y)) * (((vpC)->_X) - ((vpA)->_X)) )
      
#define RWS_SNAPEDGE(_rResult, _A, _B, _X, _Y, _Z)                         \
   MACRO_START                                                              \
      {                                                                        \
      RwV3d               vEdge;                                           \
      \
      vEdge._X = vpaVertices[_B]->_X -  vpaVertices[_A]->_X;               \
      vEdge._Y = vpaVertices[_B]->_Y -  vpaVertices[_A]->_Y;               \
      \
      MACRO_START                                                          \
      {                                                                    \
      const RwReal        rSubtend =                                   \
      ( vEdge._X * ((vProj._Y) - (vpaVertices[_A]->_Y)) -          \
      vEdge._Y * ((vProj._X) - (vpaVertices[_A]->_X)) );         \
      const RwBool        outsideEdge =                                \
      RWS_REALSIGNSNEQ(rSubtend, rTriangle);                           \
      \
      if (outsideEdge)                                                 \
      {                                                                \
      RwReal              mu;                                      \
      RwV3d               vCandidate;                              \
      \
      vEdge._Z = vpaVertices[_B]->_Z -  vpaVertices[_A]->_Z;       \
      \
      mu = RwV3dDotProduct(&vProj, &vEdge) -                       \
      RwV3dDotProduct(vpaVertices[_A], &vEdge);               \
      \
      if (mu <= 0)                                                 \
            {                                                            \
            vCandidate = *vpaVertices[_A];                           \
      }                                                            \
      else                                                         \
            {                                                            \
            RwReal       denom = RwV3dDotProduct(&vEdge, &vEdge);    \
            \
            bEdgeInternal = ((0 < denom) && (mu < denom));           \
            if (bEdgeInternal)                                       \
                {                                                        \
                mu /= denom;                                         \
                RwV3dScale(&vCandidate, &vEdge, mu);                 \
                RwV3dAdd(&vCandidate, &vCandidate, vpaVertices[_A]); \
      }                                                        \
      else                                                     \
                {                                                        \
                vCandidate = *vpaVertices[_B];                       \
      }                                                        \
      }                                                            \
      \
      RwV3dSub(&vEdge, &vPtAsPassed, &vCandidate);                 \
      rDist2 = RwV3dDotProduct(&vEdge, &vEdge);                    \
      \
      if ((!bSnapped) || ((_rResult) > rDist2))                    \
            {                                                            \
            *vpPt = vCandidate;                                      \
            (_rResult) = rDist2;                                     \
      }                                                            \
      }                                                                \
      bSnapped |= outsideEdge;                                         \
      }                                                                    \
      MACRO_STOP;                                                          \
      }                                                                        \
      MACRO_STOP
      
#define RWS_RWPLANEPROCESS(_result, _X, _Y, _Z)                              \
   MACRO_START                                                              \
{                                                                        \
   RwBool              bEdgeInternal = 0;                               \
   RwReal              rTriangle = RWS_TRIAREA(vpaVertices[0],              \
   vpaVertices[1],              \
   vpaVertices[2],              \
   _X, _Y);                     \
   \
   RWS_SNAPEDGE(_result, 1, 2, _X, _Y, _Z);                               \
   if (!bEdgeInternal)                                                  \
    {                                                                    \
    RWS_SNAPEDGE(_result, 2, 0, _X, _Y, _Z);                           \
    if (!bEdgeInternal)                                              \
        {                                                                \
        RWS_SNAPEDGE(_result, 0, 1, _X, _Y, _Z);                       \
        }                                                                \
    }                                                                    \
}                                                                        \
MACRO_STOP

      /**
      *  \ingroup CollisionToolkit
      *                                                                      
      * Given a point and a description of a triangle, return the point within the triangle
      * that is closest to that point.
      * 
      * \param vpaVertices Pointer to an array containing the triangles vertices.
      * \param vpNormal Pointer to the triangles normal.
      * \param vpPt Pointer to the test point, this is modified
      *
      * \returns Returns the distance from the triangle to the supplied reference point,                
      * vpPt now holds the near point of the triangle to the input point.
      *
      */
      RwReal TrianglePointNearestPoint(RwV3d * vpaVertices[3],
                                                 RwV3d * vpNormal, RwV3d * vpPt)
      {
         RwReal              rResult = (RwReal) 0;
         RwV3d               vPtAsPassed;
         RwV3d               vProj;
         RwReal              rDistPt2Plane;
         RwBool              bSnapped = 0;
         RwReal              rDist2;
   
         /* work in 2-D plane of greatest projection */
         const RwInt32       nAbsX = RWS_CONSTREALASINT(vpNormal->x) & 0x7FFFFFFF;
         const RwInt32       nAbsY = RWS_CONSTREALASINT(vpNormal->y) & 0x7FFFFFFF;
         const RwInt32       nAbsZ = RWS_CONSTREALASINT(vpNormal->z) & 0x7FFFFFFF;
   
         vPtAsPassed = *vpPt;
   
         /* project point onto plane of triangle */
         /* (cost of using two dot products vs. V3dSub and one is 3 multiplies,
         * but loss of precision is minimized ) */
         rDistPt2Plane =
            RwV3dDotProduct(vpaVertices[0], vpNormal) -
            RwV3dDotProduct(vpPt, vpNormal);
   
         RwV3dScale(&vProj, vpNormal, rDistPt2Plane);
         RwV3dAdd(&vProj, &vProj, vpPt);
   
         if (nAbsZ > nAbsY)
         {
            if (nAbsZ > nAbsX)
            {
               RWS_RWPLANEPROCESS(rResult, x, y, z);
            }
            else
            {
               RWS_RWPLANEPROCESS(rResult, y, z, x);
            }
         }
         else
         {
            if (nAbsY > nAbsX)
            {
               RWS_RWPLANEPROCESS(rResult, z, x, y);
            }
            else
            {
               RWS_RWPLANEPROCESS(rResult, y, z, x);
            }
         }
   
         if (!bSnapped)
         {
            *vpPt = vProj;
            rDist2 = rDistPt2Plane * rDistPt2Plane;
            rResult = rDist2;
         }
   
         return rResult;
      }
   }//namespace CCollide
}//namespace RWS