/*****************************************************************************
*
* File :     ATBox.cpp
*
* Abstract : A basic box-shaped area trigger.
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

#include "atbox.h"


namespace RWS
{
   namespace
   {
      /*
      *  Clip a point against a plane. 
      *
      * \param pPoint Point to clip against the plane.
      * \param pPlane Plane to use.
      *
      *  \return TRUE if the point is on or behind the plane, FALSE otherwise.
      */
      RwBool PointPlaneClip( const RwV3d * pPoint, const RwPlane * pPlane )
      {
         return ((RwV3dDotProduct(pPoint, &pPlane->normal) + pPlane->distance) <= 0.0f);
      }
   }

   namespace AreaTrigger
   {
      RWS_IMPLEMENT_CLASSID(ATBox);
      RWS_REGISTERCLASS(ATBox);

      /*
      *  Constructor
      */
      ATBox::ATBox( const CAttributePacket& attr )
         : ATBase(attr)
      {
      }

      /*
      *  Destructor
      */
      ATBox::~ATBox()
      {
      }

      /**
      *  Do a point-box intersection test
      *
      *  \param pPoint Point to test, in world space.
      *  \return true if point is inside the box, false otherwise.
      */
      RwBool ATBox::PointInsideGeometry( const RwV3d * pPoint )
      {
         RWS_FUNCTION("ATBox::PointInsideGeometry");

         // Transform from world to local co-ordinates
         RwV3d LocalPoint;
         RwV3dTransformPoints(&LocalPoint, pPoint, 1, &m_InvMatrix);

         // Note: locally, the box is at (0,0,0) and is a unit box.
         if (LocalPoint.x < 0.5f && LocalPoint.x > -0.5f)
         {
            if (LocalPoint.y < 0.5f && LocalPoint.y > -0.5f)
            {
               if (LocalPoint.z < 0.5f && LocalPoint.z > -0.5f)
               {
                  RWS_RETURN(TRUE);
               }
            }
         }

         RWS_RETURN(FALSE);
      }

      /**
      *  Do a line-box intersection test
      *
      *  \param pLine Line to test, in world coordinates.
      *  \return TRUE if point is inside the box, FALSE otherwise.
      */
      RwBool ATBox::LineIntersectGeometry( const RwLine * pLine )
      {
         RWS_FUNCTION("ATBox::LineIntersectGeometry");

         // Since the local-space box is a unit one, already know the face normals.
         RwPlane FacePlanes[6] =
         {
            { { 1.0f, 0.0f, 0.0f }, -0.5f },
            { { -1.0f, 0.0f, 0.0f }, -0.5f },
            { { 0.0f, 1.0f, 0.0f }, -0.5f },
            { { 0.0f, -1.0f, 0.0f }, -0.5f },
            { { 0.0f, 0.0f, 1.0f }, -0.5f },
            { { 0.0f, 0.0f, -1.0f }, -0.5f }
         };
         RwLine LocalLine;
         RwBool bBehindStart;
         RwBool bBehindEnd;
         RwUInt32 uFace;

         // Transform from world to local co-ordinates
         RwV3dTransformPoints((RwV3d*)&LocalLine, (RwV3d*)pLine, 2, &m_InvMatrix);

         for (uFace = 0; uFace < 6; uFace++)
         {
            bBehindStart = PointPlaneClip(&LocalLine.start, &FacePlanes[uFace]);
            bBehindEnd = PointPlaneClip(&LocalLine.end, &FacePlanes[uFace]);

            if (!bBehindStart && !bBehindEnd)
            {
               RWS_RETURN(FALSE);
            }
         }

         RWS_RETURN(TRUE);
      }

      /**
      *  Render a wire frame box.
      *  Only works in builds with RWS_DEBUGTOOLS defined.
      *
      *  \see DebugTools::WireBox
      */
      void ATBox::DebugToolsRender()
      {
         RWS_FUNCTION("ATBox::DebugToolsRender");

#if defined(RWS_DEBUGTOOLS)

         DebugTools::WireBox( &m_Matrix, 255, 255, 0 );
      
#endif // RWS_DEBUGTOOLS

         RWS_RETURNVOID();
      }      

   } // namespace AreaTrigger

} // namespace RWS