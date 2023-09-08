/*****************************************************************************
*
* File :     ATCylinder.cpp
*
* Abstract : A basic cylinder shaped area trigger.
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
// RenderWare Studio Includes
//
#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/debugtools/debugtools.h"

#include "atcylinder.h"


namespace RWS
{
   namespace
   {
      #define ZERO_THRESHOLD  0.00001f

      /*
      *  Test whether a 2D line intersects a circle.
      *
      *  \param pStart Start point of the line.
      *  \param pEnd End point of the line.
      *  \param pCenter Center of the circle.
      *  \param Radius Radius of the circle.
      *
      *  \return The number of intersections found with the circle.
      */
      RwUInt32 LineCircleIntersect( const RwV2d * pStart, const RwV2d * pEnd, 
                                    const RwV2d * pCenter, RwReal Radius, RwReal * paRatios )
      {
         RWS_FUNCTION("RWS::NULL::LineCircleIntersect");

         RwV2d Along;
         RwV2d Diff;
         RwReal AlongMagSqr;
         RwReal Root;
         RwReal FourAC;
         RwReal FxGy;
         RwReal FGInv;

         RwV2dSub(&Along, pEnd, pStart);
         AlongMagSqr = RwV2dDotProduct(&Along, &Along);

         if (AlongMagSqr > ZERO_THRESHOLD)
         {
            RwV2dSub(&Diff, pCenter, pStart);
            FourAC = (Along.x * Diff.y) - (Along.y * Diff.x);
            FourAC = (FourAC * FourAC);
            Root = ((Radius * Radius) * AlongMagSqr) - FourAC;

            if (Root < ( - ZERO_THRESHOLD))
            {
               // Imaginary roots to the equation - line does not intersect.
               RWS_RETURN(0);
            }
            else
            {
               FxGy = RwV2dDotProduct(&Along, &Diff);
               if (Root < ZERO_THRESHOLD)
               {
                  // Only one solution to the quadratic (line is tangential to circle).
                  paRatios[0] = FxGy / AlongMagSqr;
                  RWS_RETURN(1);
               }
               else
               {
                  rwSqrt(&Root, Root);
                  FGInv = 1.0f / AlongMagSqr;
                  paRatios[0] = (FxGy - Root) * FGInv;
                  paRatios[1] = (FxGy + Root) * FGInv;
                  RWS_RETURN(2);
               }
            }
         }

         RWS_RETURN(FALSE);
      }

   } // namespace <anonymous>

   namespace AreaTrigger
   {
      RWS_IMPLEMENT_CLASSID(ATCylinder);
      RWS_REGISTERCLASS(ATCylinder);

      /*
      *  Constructor
      */
      ATCylinder::ATCylinder( const CAttributePacket& attr )
         : ATBase(attr)
      {
      }

      /*
      *  Destructor
      */
      ATCylinder::~ATCylinder()
      {
      }

      /**
      *  Do a point-cylinder intersection test.
      *
      *  \param pWorldPoint Point to test, in world space.
      *  \return TRUE if the point is inside the cylinder, FALSE otherwise.
      */
      RwBool ATCylinder::PointInsideGeometry( const RwV3d * pPoint )
      {
         RWS_FUNCTION("ATCylinder::PointInsideGeometry");

         // transform from world to local co-ordinates
         RwV3d LocalPoint;
         RwV3dTransformPoints(&LocalPoint, pPoint, 1, &m_InvMatrix);

         // Note: locally, the cylinder is at (0,0,0) so the vector from 
         //       the center of the cylinder to the point is the same 
         //       as LocalPoint.
         if (LocalPoint.y < 0.5f && LocalPoint.y > -0.5f)
         {
            LocalPoint.y = 0.0f;

            RwReal fDist = RwV3dLength(&LocalPoint);
            if ( fDist < 0.5f )
            {
               RWS_RETURN(TRUE);
            }
         }

         RWS_RETURN(FALSE);
      }

      /**
      *  Do a line-cylinder intersection test.
      *
      *  \param pLine Line to test, in world space.
      *  \return TRUE if the point is inside the cylinder, FALSE otherwise.
      */
      RwBool ATCylinder::LineIntersectGeometry( const RwLine * pLine )
      {
         RWS_FUNCTION("ATCylinder::LineIntersectGeometry");

         RwLine LocalLine;
         RwV2d LineStart2D;
         RwV2d LineEnd2D;
         RwV2d Center = { 0.0f, 0.0f };
         RwReal Ratios[2];
         RwUInt32 NumIntersections;
         RwUInt32 Loop;

         // Transform from world to local co-ordinates
         RwV3dTransformPoints(   reinterpret_cast<RwV3d*>(&LocalLine),
                                 reinterpret_cast<const RwV3d*>(pLine),
                                 2, &m_InvMatrix);

         // Clip the line to the end planes of the cylinder.
         // Note: using a unit cylinder with its center at (0,0,0).
         if (LocalLine.start.y > 0.5f)
         {
            if (LocalLine.end.y > 0.5f)
            {
               // Both ends of the line are outside the top end of the
               // cylinder, so it can't intersect.
               RWS_RETURN(FALSE);
            }

            LocalLine.start.y = 0.5f;
         }
         else
         {
            if (LocalLine.start.y < - 0.5f)
            {
               if (LocalLine.end.y < - 0.5f)
               {                  
                  // Both ends of the line are outside the bottom end of the
                  // cylinder, so it can't intersect.
                  RWS_RETURN(FALSE);
               }

               LocalLine.start.y = - 0.5f;
            }
         }

         if (LocalLine.end.y > 0.5f)
         {
            LocalLine.end.y = 0.5f;
         }
         else
         {
            if (LocalLine.end.y < - 0.5f)
            {
               LocalLine.end.y = - 0.5f;
            }
         }

         // Test the line against the curved part of the cylinder.
         // This can be simplified to a 2D line-circle test.
         LineStart2D.x = LocalLine.start.x;
         LineStart2D.y = LocalLine.start.z;
         LineEnd2D.x = LocalLine.end.x;
         LineEnd2D.y = LocalLine.end.z;

         NumIntersections = LineCircleIntersect(&LineStart2D, &LineEnd2D, &Center, 0.5f, &Ratios[0]);
         for (Loop = 0; Loop < NumIntersections; Loop++)
         {
            if (Ratios[Loop] >= 0.0f && Ratios[Loop] <= 1.0f)
            {
               RWS_RETURN(TRUE);
            }
         }

         RWS_RETURN(FALSE);
      }

      /**
      *  Render a wire frame cylinder.
      *  Only works in builds with RWS_DEBUGTOOLS defined.
      *
      *  \see DebugTools::WireCylinder
      */
      void ATCylinder::DebugToolsRender()
      {
         RWS_FUNCTION("ATCylinder::DebugToolsRender");

#if defined(RWS_DEBUGTOOLS)

         DebugTools::WireCylinder(&m_Matrix, 255, 255, 0);
         
#endif // RWS_DEBUGTOOLS
         
         RWS_RETURNVOID();
      }

   } // namespace AreaTrigger

} // namespace RWS