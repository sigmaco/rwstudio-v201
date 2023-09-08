/*****************************************************************************
*
* File :     ATSphere.cpp
*
* Abstract : A basic sphere-shaped area trigger.
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
#include "rtray.h"

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/debugtools/debugtools.h"

#include "atsphere.h"


namespace RWS
{
   namespace AreaTrigger
   {
      RWS_IMPLEMENT_CLASSID(ATSphere);
      RWS_REGISTERCLASS(ATSphere);

      /*
      *  Constructor
      */
      ATSphere::ATSphere( const CAttributePacket& attr )
         : ATBase(attr)
      {
      }

      /*
      *  Destructor
      */
      ATSphere::~ATSphere()
      {
      }

      /**
      *  Do a point-sphere intersection test.
      *
      *  \param pPoint Point to test, in world coordinates.
      *  \return TRUE if point is inside the sphere, FALSE otherwise.
      */
      RwBool ATSphere::PointInsideGeometry( const RwV3d * pPoint )
      {
         RWS_FUNCTION("ATSphere::PointInsideGeometry");

         // transform from world to local co-ordinates
         RwV3d LocalPoint;
         RwV3dTransformPoints(&LocalPoint, pPoint, 1, &m_InvMatrix);

         // Note: locally, the sphere is at (0,0,0) so the vector from 
         //       the center of the sphere to the point is the same as 
         //       LocalPoint.
         RwReal fDist = RwV3dLength(&LocalPoint);
         if ( fDist < 0.5f )
         {
            RWS_RETURN(TRUE);
         }

         RWS_RETURN(FALSE);
      }

      /**
      *  Do a line-sphere intersection test.
      *
      *  \param pLine Line to test, in world coordinates.
      *  \return TRUE if line intersects the sphere, FALSE otherwise.
      */
      RwBool ATSphere::LineIntersectGeometry( const RwLine * pLine )
      {
         RWS_FUNCTION("ATSphere::LineIntersectGeometry");

         RwLine LocalLine;
         RwSphere Sphere;
         RwBool bIntersect;
         RwReal Ratio;

         // Testing against a unit sphere, in local co-ordinates
         Sphere.center.x = 0.0f;
         Sphere.center.y = 0.0f;
         Sphere.center.z = 0.0f;
         Sphere.radius = 0.5f;

         // Transform from world to local co-ordinates
         RwV3dTransformPoints((RwV3d*)&LocalLine, (RwV3d*)pLine, 2, &m_InvMatrix);

         bIntersect = RtLineSphereIntersectionTest(&LocalLine, &Sphere, &Ratio);
         if (bIntersect && (Ratio <= 1.0f))
         {
            RWS_RETURN(TRUE);
         }

         RWS_RETURN(FALSE);
      }

      /**
      *  Render a wire frame sphere. 
      *  Only works in builds with RWS_DEBUGTOOLS defined.
      *
      *  \see DebugTools::WireSphere
      */
      void ATSphere::DebugToolsRender()
      {
         RWS_FUNCTION("ATSphere::DebugToolsRender");

#if defined(RWS_DEBUGTOOLS)

         DebugTools::WireSphere(&m_Matrix, 255, 255, 0);

#endif // RWS_DEBUGTOOLS

         RWS_RETURNVOID();
      }

   } // namespace AreaTrigger

} // namespace RWS