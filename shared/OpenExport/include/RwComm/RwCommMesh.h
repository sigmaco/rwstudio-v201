// RwExpMesh.h: interface for the RwExpMesh class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPMESH_H__7DE18B0E_449B_441D_B97C_D53CB51ACD86__INCLUDED_)
#define AFX_RWEXPMESH_H__7DE18B0E_449B_441D_B97C_D53CB51ACD86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwCommGeometry.h"

/**
*  \ingroup commlowlevelobjects
*   This structure represents am edge. All indices refer to the original 3D modeling tool
*   geometry.
*/
struct RWCOMM_API RwCommEdge
{
    /** Edge index in the geometry.*/
    int     m_edgeIndex;
    /** Global position vertex index in the geometry of the first edge vertex.*/
    int     m_vert0Index;
    /** Global position vertex index in the geometry of the second edge vertex.*/
    int     m_vert1Index;
};

/**
*  \ingroup commlowlevelobjects
*   This structure represents am edge. All indices refer to the original 3D modeling tool
*   geometry.
*/
struct RWCOMM_API RwCommToonInk
{
    /** Ink type. There are two kinds of edges, silhouette (or outline)
    *   edges and crease (or hard) edges.
    */
    enum InkType
    {
        /** silhouette edges */
        RPTOON_INKTYPE_SILHOUETTE = 0,
        /** crease edges */
        RPTOON_INKTYPE_CREASE,
    };

    /** Holds the type of the ink. */
    InkType                     m_type;
    /** Name of the toon ink. */
    RwWString                m_name;
    /** Vector of all edges belonging to this ink. */
    RwVector<RwCommEdge>    m_edges;
};

const RwCommType RwCommMeshType("RwCommMesh");

/**
*  \ingroup commscenegraph
*
*   This is the class representing a 3D polygonal mesh. Its basic geometric primitive
*   is a polygon of an arbitrary size. In addition to all of RwCommGeometry's methods,
*   the mesh adds the GetPrimTriangleCount() method for querying the number of triangles
*   for the current primitive and GetPrimTriangulation(), returning a list of index
*   triplets, representing the current primitive's triangulation.
*/
class RWCOMM_API RwCommMesh : public RwCommGeometry
{
public:
    RwCommMesh();
    virtual ~RwCommMesh();

   // Type methods
    virtual RwCommType  GetType() const { return RwCommMeshType; }
    virtual void        GetTypeHierarchy(RwVector<RwCommType> &vTypeVec) const;

    /**
     *  Get number of triangles making up the current primitive.
     *  \param nTriangles a reference to an int to be filled with the number of triangles
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetPrimTriangleCount(int &nTriangles) = 0;

    /** 
    *   Get vertex indices of triangles making up the current primitive. 
    *   The indices are local indices into the vertex array in the current RwCommPrimitive.
    *   \param triangleIndices the vector of ints representing the triangulation
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError GetPrimTriangulation(RwVector<int> &triangleIndices) = 0;

    /**
     *  Get edges for the current primitive.
     *  \param edges the vector of edges inks to be filled in
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetPrimEdges(RwVector<RwCommEdge> &edges) = 0;

    /**
     *  Get the toon inks from this mesh.
     *  \param toonInks the vector of toon inks to be filled in
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetToonInks(RwVector<RwCommToonInk> &toonInks) = 0;
};

#endif // !defined(AFX_RWEXPMESH_H__7DE18B0E_449B_441D_B97C_D53CB51ACD86__INCLUDED_)
