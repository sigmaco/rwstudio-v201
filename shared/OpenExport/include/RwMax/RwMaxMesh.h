// RwMaxMesh.h: interface for the RwMaxMesh class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWMAXMESH_H__D11475EC_7705_4053_8DC2_5A721E203FB3__INCLUDED_)
#define AFX_RWMAXMESH_H__D11475EC_7705_4053_8DC2_5A721E203FB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable : 4786)
#pragma warning(disable : 4251)

#include "RwCommMesh.h"
#include "RwMaxCustomAttributes.h"
#include "RwMaxGeometry.h"
#include "RwMax.h"


struct RWMAX_API RwMaxModContextEnumProc : ModContextEnumProc
{
    ModContext *m_mc;

    RwMaxModContextEnumProc() : m_mc(0) {}

    virtual BOOL proc(ModContext *mc)
    {
        m_mc = mc;
        return TRUE;
    }
};

#if (MAX_RELEASE >= 5000)
struct RwNormalModTri
{
    RwNormalModTri( void );
    RwNormalModTri(int a, int b, int c);

    bool Matches(int a, int b, int c);

    bool operator <(const RwNormalModTri &that);
    bool operator == (const RwNormalModTri &that) const;
    bool operator == (RwNormalModTri &that);
    
    int m_vertexID[3];

    Point3 m_specifedNormals[3];

};

struct NormalFace
{
    Point3 normals[3];
};

#endif 

/**
*  \ingroup maxscenegraph 
*
*   This is the RwMax implementation of the RwCommMesh class. The class encapsulates an INode object
*   and a Mesh object.
*
*   \see RwCommMesh
*/
class RWMAX_API RwMaxMesh : public RwCommMesh, public RwMaxGeometry
{
public:
    RwMaxMesh(INode *pNode);
    virtual ~RwMaxMesh();

    // Comparison operator used for instancing
    virtual bool operator==(const RwCommGeometry &geom) const;

    virtual RwCommError GetUserData(RwCommUserDataCollection &userDataColl);
    virtual RwCommError GetVertexUserData(RwCommUserDataCollection &userDataColl,
                             const RwVector<RwCommVertexId> &vertIds);
    virtual RwCommError GetPrimitiveUserData(RwCommUserDataCollection &userDataColl);

    // Get attribute methods.
    virtual RwCommError GetAttribute(const RwWString &name, int &value, int index = 0);
    virtual RwCommError GetAttribute(const RwWString &name, float &value, int index = 0);
    virtual RwCommError GetAttribute(const RwWString &name, RwWString &value, int index = 0);
    virtual RwCommError GetAttributeNames(RwVector<RwWString> &vAttributeNames, RwVector<AttributeType> &vAttributeTypes);

    // Animatable methods
    RwCommError OnSetFrame(RwCommReal frame);

    // Shape methods
    virtual RwCommError GetVisibility(bool &bIsVisible) const;

   // Bounding box
    virtual RwCommError GetWorldSpaceBoundingBox(RwCommBoundingBox &) const;

    // evaluate geometry
    virtual RwCommError ReEvaluate(RwCommReal frame);

    // Get vertex by id
    virtual RwCommError GetVertex(const RwCommVertexId &id, RwCommVertex &vertex, bool bWorldSpace = false);

    // Primitive methods
    virtual RwCommError GetNumPrimitives(int &nPrims) const;
    virtual RwCommError ResetPrimIterator();
    virtual RwCommError AdvancePrimIterator();
    virtual RwCommError GetPrimVertexCount(int &nVertices);
    virtual RwCommError GetPrimVertex(int index, RwCommVertex &vertex, RwCommVertexId &id, bool bWorldSpace = false);
    virtual RwCommError GetPrimIndex(int &nIndex);
    virtual RwCommError GetPrimShader(RwCommShader *&pShader);
    virtual RwCommError GetPrimNormal(RwCommVec3 &normal, bool bWorldSpace = false);
    virtual RwCommError GetPrimTriangleCount(int &nTriangles);
    virtual RwCommError GetPrimTriangulation(RwVector< int > &triangleIndices);
    virtual RwCommError GetPrimEdges(RwVector<RwCommEdge> &edges);
    virtual RwCommError GetDefaultColor(RwCommVec3 &color) const;
    virtual RwCommError GetToonInks(RwVector<RwCommToonInk> &toonInks);

    /** \return The 3ds max mesh object. */
    Mesh *GetMesh() const { return m_pMesh; }
    /** \return The 3ds max INode object this mesh belongs too. */
    INode *GetNode() const { return m_pINode; }

protected:

    virtual void OnAddShader(RwCommShader *pShader);

    //Get vertex data
    RwCommError GetVertex(int iFace, int iVertex, RwCommVertex& vertex, bool bWorldSpace) const;
    RwCommError GetVertexPosition(int iFace, int iVertex, RwCommVec3 &vertex, bool bWorldSpace) const;
    RwCommError GetVertexPositionIndex(int iFace, int iVertex, int &iIndex) const;
    RwCommError GetVertexNormal(int iFace, int iVertex, RwCommVec3 &normal, bool bWorldSpace) const;
    RwCommError GetVertexColor(int iFace, int iVertex, RwCommVec3 &color) const;
    RwCommError GetVertexAlpha(int iFace, int iVertex, RwCommReal &alpha) const;
    RwCommError GetVertexIllumination(int iFace, int iVertex, RwCommVec3 &vIllum) const;
    RwCommError GetVertexUVs(int iFace, int iVertex, RwVector< RwCommVec2 > &uvs) const;


    RwCommShader *GetFaceShader(int iFaceIndex) const;

protected:
    Mesh            *m_pMesh;
    INode           *m_pINode;
    Object          *m_pObject;
    TriObject       *m_pTriObj;
    bool            m_bDelObj;
    int             m_iFaceIterator;
    Matrix3         m_worldNormalMat;
    
    #if (MAX_RELEASE >= 5000)
        bool                m_EditNormalInterface;
        RwCommError BuildNormalCache(MNNormalSpec *specNormals, MNMesh *mnMesh);
        RwVector<NormalFace>   m_NMeshNormals;

    #endif
};



/**
*  \ingroup maxbuilder 
*
*  This class is responsible for creating an RwMaxMesh object.
*
*   \see RwCommMesh
*/
class RWMAX_API RwMaxMeshCreator  : public RwMaxGeometryCreator
{
public:
    RwMaxMeshCreator();
    virtual ~RwMaxMeshCreator();

    virtual RwCommObject *Create(   const RwCommCreationData *pCreationData,
                                    const RwCommExportOptions *pExportOptions) const;

};

#endif // !defined(AFX_RWMAXMESH_H__D11475EC_7705_4053_8DC2_5A721E203FB3__INCLUDED_)
