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

#include "stdafx.h"

#include "RwCommMesh.h"
#include "RwCommObjCreator.h"

class MaxSimpleMesh : public RwCommMesh
{
public:
    MaxSimpleMesh(INode* pNode);
    virtual ~MaxSimpleMesh();

    // Destroy the object - called by RwCommObject::Release()
    virtual void Destroy() { delete this; }

    // Comparison operator used for instancing
    virtual bool operator==(const RwCommGeometry& geom) const;

    virtual RwCommError GetUserData(RwCommUserDataCollection &userDataColl) { return RWCOMM_NOT_IMPLEMENTED; }
    virtual RwCommError GetVertexUserData(RwCommUserDataCollection &userDataColl,
                             const RwVector<RwCommVertexId>& vertIds) { return RWCOMM_NOT_IMPLEMENTED; }
    virtual RwCommError GetPrimitiveUserData(RwCommUserDataCollection &userDataColl) { return RWCOMM_NOT_IMPLEMENTED; }

    // Get attribute methods.
    virtual RwCommError GetAttribute(const RwString& name, int &value, int index = 0) { return RWCOMM_NOT_IMPLEMENTED; }
    virtual RwCommError GetAttribute(const RwString& name, float &value, int index = 0) { return RWCOMM_NOT_IMPLEMENTED; }
    virtual RwCommError GetAttribute(const RwString& name, RwString &value, int index = 0) { return RWCOMM_NOT_IMPLEMENTED; }
    virtual RwCommError GetAttributeNames(RwVector<RwString>& vAttributeNames, RwVector<AttributeType> &vAttributeTypes) { return RWCOMM_NOT_IMPLEMENTED; }

    // Animatable methods
    RwCommError OnSetFrame(RwCommReal frame);

    // Shape methods
    virtual RwCommError GetVisibility(bool& bIsVisible) const;

   // Bounding box
    virtual RwCommError GetWorldSpaceBoundingBox(RwCommBoundingBox&) const;

    // evaluate geometry
    virtual RwCommError ReEvaluate(RwCommReal frame);

    // Get vertex by id
    virtual RwCommError GetVertex(const RwCommVertexId& id, RwCommVertex& vertex, bool bWorldSpace = false);

    // Primitive methods
    virtual RwCommError GetNumPrimitives(int& nPrims) const;
    virtual RwCommError ResetPrimIterator();
    virtual RwCommError AdvancePrimIterator();
    virtual RwCommError GetPrimVertexCount(int& nVertices);
    virtual RwCommError GetPrimVertex(int index, RwCommVertex& vertex, RwCommVertexId& id, bool bWorldSpace = false);
    virtual RwCommError GetPrimIndex(int& nIndex);
    virtual RwCommError GetPrimShader(RwCommShader*& pShader);
    virtual RwCommError GetPrimNormal(RwCommVec3& normal, bool bWorldSpace = false);
    virtual RwCommError GetPrimTriangleCount(int& nTriangles);
    virtual RwCommError GetPrimTriangulation(RwVector< int >& triangleIndices);

    virtual RwCommError GetDefaultColor(RwCommVec3 &color) const;
    virtual RwCommError GetPrimEdges(RwVector<RwCommEdge>& edges) { return RWCOMM_NOT_IMPLEMENTED; }
    virtual RwCommError GetToonInks(RwVector<RwCommToonInk>& toonInks) { return RWCOMM_NOT_IMPLEMENTED; }

    Mesh* GetMesh() const { return m_pMesh; }
    INode* GetNode() const { return m_pINode; }

protected:

    //Get vertex data
    RwCommError GetVertex(int iFace, int iVertex, RwCommVertex& vertex, bool bWorldSpace) const;
    RwCommError GetVertexPosition(int iFace, int iVertex, RwCommVec3& vertex, bool bWorldSpace) const;
    RwCommError GetVertexPositionIndex(int iFace, int iVertex, int& iIndex) const;
    RwCommError GetVertexNormal(int iFace, int iVertex, RwCommVec3& normal, bool bWorldSpace) const;
    RwCommError GetVertexColor(int iFace, int iVertex, RwCommVec3& color) const;
    RwCommError GetVertexAlpha(int iFace, int iVertex, RwCommReal& alpha) const;
    RwCommError GetVertexIllumination(int iFace, int iVertex, RwCommVec3& vIllum) const;
    RwCommError GetVertexUVs(int iFace, int iVertex, RwVector< RwCommVec2 >& uvs) const;

    RwCommShader* GetFaceShader(int iFaceIndex) const;

    // utils
    RwCommBoundingBox Box3ToRwCommBoundingBox(const Box3& b) const;
    RwCommVec3 Point3ToRwCommVec3(const Point3& p) const;
    Matrix3 RwCommMatrixToMatrix3(const RwCommMatrix& m) const;
    Matrix3 GetObjOffsetMatrix(INode *iNode);

protected:
    Mesh*           m_pMesh;
    INode*          m_pINode;
    Object*         m_pObject;
    TriObject*      m_pTriObj;
    bool            m_bDelObj;
    int             m_iFaceIterator;

    // World space matrices
    Matrix3         m_worldMat;
    Matrix3         m_worldNormalMat;

    // Offset matrices
    Matrix3         m_offsetMat;
    Matrix3         m_offsetNormalMat;
};



/**
*  \ingroup maxbuilder 
*
*  This class is responsible for creating an RwMaxMesh object.
*
*   \see RwCommMesh
*/
class MaxSimpleMeshCreator  : public RwCommObjCreator
{
public:
    MaxSimpleMeshCreator();
    virtual ~MaxSimpleMeshCreator();

    virtual RwCommObject* Create(   const RwCommCreationData* pCreationData,
                                    const RwCommExportOptions* pExportOptions) const;

	virtual void Init(	RwCommObject* pObject,
						const RwCommCreationData* pCreationData,
						const RwCommExportOptions* pExportOptions) const;

	virtual void Attach(RwCommObject* pObject, RwCommObject* pParent, RwCommScene* pScene,
                        const RwCommCreationData* pCreationData) const;

};

#endif // !defined(AFX_RWMAXMESH_H__D11475EC_7705_4053_8DC2_5A721E203FB3__INCLUDED_)
