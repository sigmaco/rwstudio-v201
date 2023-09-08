// RwExpGeometry.h: interface for the RwExpGeometry class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPGEOMETRY_H__AD133A1C_54B9_41D8_B7C5_E0D96356A8C3__INCLUDED_)
#define AFX_RWEXPGEOMETRY_H__AD133A1C_54B9_41D8_B7C5_E0D96356A8C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwCommShape.h"

const RwCommType RwCommGeometryType("RwCommGeometry");

class RwCommVertex;
struct RwCommVertexId;

/**
*  \ingroup commlowlevelobjects
*   This structure represents a primitive. A primitive is a generic 3D geometrical
*   shape, which can vary depending on the implementing class. Primitives can be
*   polygons, patches, splines etc. 
*/
struct RWCOMM_API RwCommPrimitive
{
    /** Array of vertex indices belonging to the primitive.*/
    RwVector<int>   m_vVxIndices;
    /** Shader of the primitive.*/
    RwCommShader        *m_pShader;
    /** The original index of the primitive.*/
    int                 m_index;
};

/**
*  \ingroup commscenegraph
*
*   This is the base class for all objects that are geometries, i.e. that have
*   any sort of associated vertex information. The RwCommGeometry class derives
*   from RwCommShape and implements all of its interface methods. 
*   To extract all of the vertex information from a geometry, the geometry exposes a primitive
*   iterator interface. A primitive is a generic 3D geometrical shape, which can vary depending on
*   the implementing class. (Primitives can be polygons, patches, splines etc.) The geometry manages an
*   iterator that steps through all of its primitives and for each one exposes a set of primitive related
*   querying methods. To reset and advance the primitive iterator use the ResetPrimIterator() and 
*   AdvancePrimIterator() methods. A typical primitive iteration loop will look like this:
*
\code
RwCommError result = pGeometry->ResetPrimIterator();
while (result == RWCOMM_SUCCESS)
{
    int nVerticesPerPrim = 0;
    pGeometry->GetPrimVertexCount(nVerticesPerPrim);
    for (int i = 0; i < nVerticesPerPrim; i++)
    {
        RwCommVertex vertex;
        RwCommVertexId id;

        if (pGeometry->GetPrimVertex(i, vertex, id) == RWCOMM_SUCCESS)
        {
            // Do something with the vertex
        }
    }
    result = pGeometry->AdvancePrimIterator();
}
\endcode

*   Each vertex you extract from the geometry contains all the relevant vertex information,
*   such as position, normal, texture coordinates, color information etc. The GetPrimVertex() method also 
*   return a vertex Id object, which is used to identify that vertex in subsequent queries, such as the 
*   GetVertex() method, which returns the vertex associated with that vertex Id.
*   Each primitive in the geometry can have an associated shader attached to it, determining the render
*   state of that primitive. To extract a primitive's associated shader use the GetPrimShader() method. 
*   To get a list of all of the geometry's associated shaders you can use RwCommShape's GetShader() method.
*/
class RWCOMM_API RwCommGeometry : public RwCommShape
{
public:
    RwCommGeometry();
    virtual ~RwCommGeometry();

    // Type methods
    virtual RwCommType      GetType() const { return RwCommGeometryType; }
    virtual void            GetTypeHierarchy(RwVector<RwCommType> &vTypeVec) const;

    // User data methods
    /**
    *   Extract vertex user data for this object.
    *
    *   \param userDataColl collection to hold the user data.
    *   \param vertIds vector of RwCommVertexId vertex indices
    *          for which to extract user data.
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError     GetVertexUserData(RwCommUserDataCollection &userDataColl,
                                              const RwVector<RwCommVertexId> &vertIds)
                                              { return RWCOMM_NOT_IMPLEMENTED; }
    /**
    *   Extract primitive user data for this object.
    *
    *   \param userDataColl collection to hold the user data.
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError     GetPrimitiveUserData(RwCommUserDataCollection &userDataColl)
                                                { return RWCOMM_NOT_IMPLEMENTED; }

    /**
    *   Comparison operator. This method is used to determine
    *   if two geometries are identical.
    *   \param the RwCommGeometry to be compared against
    *   \return a bool indicating equality.
    */
    virtual bool            operator==(const RwCommGeometry &geom) const { return false; }


    /**
     *  Get the geometry bounding box.
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError     GetWorldSpaceBoundingBox(RwCommBoundingBox &) const = 0;

    /**
    *   Re-evaluates the geometry on the given frame. 
    *   
    *
    *   \param frame at which to rebuild the geometry
    */
    virtual RwCommError     ReEvaluate(RwCommReal frame) { return RWCOMM_SUCCESS; }

    /**
    *   Use this method to get a vertex from its RwCommVertexId. Vertex Ids are obtained
    *   using the GetPrimVertex() method, called inside a primitive iteration loop. This method
    *   does not depend on the current iterated primitive (in the prim iteration mechanism).
    *
    *   \param id of the vertex you query
    *   \param vertex returns the vertex
    *   \param bWorldSpace indicates if you need vertex in world or local space.
    *          Local is default.
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError     GetVertex(const RwCommVertexId &id, RwCommVertex &vertex,
                                      bool bWorldSpace = false) = 0;

    // Primitive methods

    /**
     *  Get number of primitives. 
    *   \param nPrims a reference to an int to be filled with the number of primitives
    *   \return RWCOMM_SUCCESS if the current primitive is valid, RWCOMM_ITERATOR_END otherwise.
    */
    virtual RwCommError     GetNumPrimitives(int &nPrims) const = 0;

    /** 
    *   Reset the primitive iterator.
    *
    *   \return RWCOMM_SUCCESS if the current primitive is valid, RWCOMM_ITERATOR_END otherwise.
    */
    virtual RwCommError     ResetPrimIterator() = 0;

    /** 
    *   Advance the primitive iterator.
    *
    *   \return RWCOMM_SUCCESS if the current primitive, after the advance operation, is valid,
    *       RWCOMM_ITERATOR_END otherwise.
    */
    virtual RwCommError     AdvancePrimIterator() = 0;

    /** 
    *   Get number of vertices per current primitive.
     *  \param nVertices an reference to an int to be filled with the number of indices.
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError     GetPrimVertexCount(int &nVertices) = 0;

    /**
    *   Get vertex by local index within the current primitive.
    *
    *   \param index of the vertex you query
    *   \param vertex returns the vertex
    *   \param id returns the vertex id
    *   \param bWorldSpace indicates if you need vertex in world or local space.
    *          Local is default.
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError     GetPrimVertex(int index, RwCommVertex &vertex,
                                          RwCommVertexId &id, bool bWorldSpace = false) = 0;

    /**
     *  Get the current primitive's index.
     *  \param nIndex an reference to an int to be filled with the prim index.
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError     GetPrimIndex(int &nIndex) = 0;

    /**
     *  Get the current primitive's shader.
     *  \param pShader an reference to RwCommShader to be filled int with the shader
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError     GetPrimShader(RwCommShader *&pShader) = 0;

    /**
    *   Get the current primitive's normal.
    *
    *   \param normal returns the vertex normal
    *   \param bWorldSpace indicates if you need vertex in world or local space.
    *          Local is default.
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError     GetPrimNormal(RwCommVec3 &normal, bool bWorldSpace = false) = 0;

    /**
     *  Get the geometry's default color.
     *  \param color the color to be filled in.
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError     GetDefaultColor(RwCommVec3 &color) const = 0;
};

#endif // !defined(AFX_RWEXPGEOMETRY_H__AD133A1C_54B9_41D8_B7C5_E0D96356A8C3__INCLUDED_)
