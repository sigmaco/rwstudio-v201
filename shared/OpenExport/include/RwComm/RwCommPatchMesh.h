// RwExpPatchMesh.h: interface for the RwExpPatchMesh class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPPATCHMESH_H__336EF785_FA08_4B51_BBA3_8044B7DE45A9__INCLUDED_)
#define AFX_RWEXPPATCHMESH_H__336EF785_FA08_4B51_BBA3_8044B7DE45A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwCommGeometry.h"

class RwCommMesh;

const RwCommType RwCommPatchMeshType("RwCommPatchMesh");

/**
*  \ingroup commscenegraph
*
*   This is the class representing a 3D Bezier patch mesh. Its basic geometric primitive
*   is a Bezier patch, which can be a triangular patch, consisting of ten control points,
*   or a quad patch, consisting of sixteen control points. You can query the patch type
*   by using the GetPatchType() method. To query all of the patch's control points use
*   the standard RwCommGeomtry interface for querying primitives and vertices. You can
*   also tessellate the patch mesh using the Tesselate() method, which returns an
*   instance of an RwCommMesh object, representing the tessellated patch mesh. 
*/
class RWCOMM_API RwCommPatchMesh : public RwCommGeometry
{
public:
    /** Patch type. */
    enum PatchType
    {
        /** triangular patch */
        TRI = 0,
        /** quad patch */
        QUAD,
    };

    RwCommPatchMesh();
    virtual ~RwCommPatchMesh();

   // Type methods
    virtual RwCommType  GetType() const { return RwCommPatchMeshType; }
    virtual void        GetTypeHierarchy(RwVector<RwCommType> &vTypeVec) const;

    // Patch methods
    /**
     *  Get the patch type
     *  \param type returns the type of the current patch primitive.
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetPatchType(PatchType &type) const = 0;
    
    /**
     *  Get the patch degree
     *  \param degree returns the degree of the current patch primitive.
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetPatchDegree(int &degree) const = 0;
    
    /**
    *   This method returns a point on the surface of the current patch
    *   primitive based on the specified u, v and w values. The u, v, w
    *   values are barycentric coordinates. u+v+w = 1.0. If u is 1, and
    *   v and w are 0, the point is at the first vertex. If u is 0, v is
    *   1, and w is 0, then the point is at the second vertex. If u and
    *   v are 0 and w is 1 then the point is at the third vertex.
    *   Varying positions between these values represent different
    *   positions on the patch.
    *
    *   \note This method only makes sense on triangular patch.
    */
    virtual RwCommError InterpolatePatch(RwCommReal u, RwCommReal v, RwCommReal w, RwCommVec3 &p) const = 0;
    /** 
    *   \param pMesh returns the RwCommMesh representing the tessellated patch mesh.
    *       The mesh that is returned is attached to the RwCommScene.
    */
    virtual RwCommError Tessellate(RwCommMesh *&pMesh) = 0;
};

#endif // !defined(AFX_RWEXPPATCHMESH_H__336EF785_FA08_4B51_BBA3_8044B7DE45A9__INCLUDED_)
