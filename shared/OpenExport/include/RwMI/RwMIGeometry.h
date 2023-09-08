// RwMIGeometry.h: interface for the RwMIGeometry class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWMIGEOMETRY_H__23CED5FC_B4C9_4952_B82C_875AC19577D7__INCLUDED_)
#define AFX_RWMIGEOMETRY_H__23CED5FC_B4C9_4952_B82C_875AC19577D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwMI.h"
#include "RwCommGeometry.h"
#include "RwCommVertex.h"
#include "RwCommUserData.h"

/**
*   \ingroup miscenegraph
*
*   This is the base class for all MI geometry objects. The class caches all the
*   vertex and primitive information.
*
*/
class RWMI_API RwMIGeometry
{
public:
    RwMIGeometry();
    virtual ~RwMIGeometry();

    // User data methods
    RwCommError GetVertexUserData(RwCommUserDataCollection &userDataColl,
                                  const RwVector<RwCommVertexId> &vertIds);
    RwCommError GetPrimitiveUserData(RwCommUserDataCollection &userDataColl);

    // RwCommShape methods
    RwCommError GetVisibility(bool &bIsVisible) const;

    // Bounding box
    RwCommError GetBoundingBox(RwCommBoundingBox &) const;

    // Re evaluate geometry
    RwCommError ReEvaluate(RwCommReal frame) { return RWCOMM_SUCCESS; }

    // Get vertex by id
    RwCommError GetVertex(const RwCommVertexId &id, RwCommVertex &vertex);

    // Primitive methods

    // Get Number of primitives
    RwCommError GetNumPrimitives(int &nPrims) const;

    // Reset the primitive iterator
    RwCommError ResetPrimIterator();

    // Advance the primitive iterator
    RwCommError AdvancePrimIterator();

    // Get number of vertices per current primitive
    RwCommError GetPrimVertexCount(int &nVertices);

    // Get vertex by index within the current primitive
    RwCommError GetPrimVertex(int index, RwCommVertex &vertex, RwCommVertexId &id);

    // Get the current primitive's index
    RwCommError GetPrimIndex(int &nIndex);

    // Get the current primitive's shader
    RwCommError GetPrimShader(RwCommShader *&pShader);

    // Get the current primitive's normal
    RwCommError GetPrimNormal(RwCommVec3 &normal);

    // Get the node's wire-frame color.
    RwCommError GetDefaultColor(RwCommVec3 &color) const;

    /** Sets the visibility state.
    *   \param visibility the visibility state
    */
    void SetVisibility(bool visibility) { m_bVisibility = visibility; }

    void AddVertex(const RwCommVertex &vertex);
    void AddPrimitive(const RwCommPrimitive &primitive);

    /** Sets the geometry's default color.
    *   \param color the input color
    */
    void SetDefaultColor(const RwCommVec3 &color) { m_color = color; }

    /** Set the geometry's ref name.*/
    void SetRefName(const RwWString &ref) { m_refName = ref; }
    RwWString  GetRefName() const { return m_refName; }

    RwCommUserDataCollection &GetVertexUserDataCollection() { return m_vxUsrDataCol; }
    RwCommUserDataCollection &GetPrimUserDataCollection()   { return m_primUsrDataCol; }


protected:

        // Transform a vertex
    RwCommError     TransformVertex(const RwCommMatrix &M, RwCommVertex &vertex) const; 
    void            CalcWorldSpaceMatrix(RwCommTransform *pTransform);

protected:

    /** visibility state*/
    bool                        m_bVisibility;

    /** the vertex array*/
    RwVector<RwCommVertex>  m_vVertices;

    /** the primitive array*/
    RwVector<RwCommPrimitive> m_vPrimitives;

    /** prim iterator*/
    int                         m_primIter;

    /** default color*/
    RwCommVec3                  m_color;

    // user data
    /** vertex user data collection*/
    RwCommUserDataCollection    m_vxUsrDataCol;

    /** primitive user data collection*/
    RwCommUserDataCollection    m_primUsrDataCol; 

    /** the world space matrix, calculated each time the frame is changed */
    RwCommMatrix                m_worldSpaceMatrix;

    /** Reference name, used for geometry instancing*/
    RwWString                m_refName;
    
};

#endif // !defined(AFX_RWMIGEOMETRY_H__23CED5FC_B4C9_4952_B82C_875AC19577D7__INCLUDED_)
