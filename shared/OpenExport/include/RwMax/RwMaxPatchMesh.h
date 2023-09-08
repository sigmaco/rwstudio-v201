// RwMaxPatchMesh.h: interface for the RwMaxPatchMesh class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWMAXPATCHMESH_H__728D84BB_E455_4759_8AC9_EE622A05EA5F__INCLUDED_)
#define AFX_RWMAXPATCHMESH_H__728D84BB_E455_4759_8AC9_EE622A05EA5F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "RwMax.h"
#include "RwCommPatchMesh.h"
#include "RwMaxCustomAttributes.h"
#include "RwMaxGeometry.h"

/**
*  \ingroup maxscenegraph 
*
*   This is the RwMax implementation of the RwCommPatchMesh class. The class encapsulates an INode object
*   and a PatchMesh object.
*
*   \see RwCommPatchMesh
*/
class RWMAX_API RwMaxPatchMesh  : public RwCommPatchMesh, public RwMaxGeometry
{
public:
	RwMaxPatchMesh(INode * pNode);
	virtual ~RwMaxPatchMesh();

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
    virtual RwCommError     GetVisibility(bool &bIsVisible) const;

   // Bounding box
	virtual RwCommError GetWorldSpaceBoundingBox(RwCommBoundingBox &) const;

    // evaluate geometry
	virtual RwCommError ReEvaluate(RwCommReal frame);

    // Get vertex by id
    virtual RwCommError GetVertex(const RwCommVertexId &id, RwCommVertex &vertex, bool bWorldSpace = false);

    // Geometry methods
    virtual RwCommError GetNumPrimitives(int &nPrims) const;
    virtual RwCommError ResetPrimIterator();
    virtual RwCommError AdvancePrimIterator();
    virtual RwCommError GetPrimVertexCount(int &nVertices);
    virtual RwCommError GetPrimVertex(int index, RwCommVertex &vertex, RwCommVertexId &id, bool bWorldSpace = false);
    virtual RwCommError GetPrimIndex(int &nIndex);
    virtual RwCommError GetPrimShader(RwCommShader *&pShader);
    virtual RwCommError GetPrimNormal(RwCommVec3 &normal, bool bWorldSpace = false) { return RWCOMM_NOT_IMPLEMENTED; }
    virtual RwCommError GetDefaultColor(RwCommVec3 &color) const;

    // Patch methods
    virtual RwCommError GetPatchType(PatchType &type) const;
    virtual RwCommError GetPatchDegree(int &degree) const;
    virtual RwCommError InterpolatePatch(RwCommReal u, RwCommReal v, RwCommReal w, RwCommVec3& p) const;
    virtual RwCommError Tessellate(RwCommMesh *&pMesh);

    /**
     * Get the PatchMesh instance
     *  \return a pointer to the PatchMesh.
     */
	PatchMesh *GetMesh() const { return m_pMesh; }

    /**
     *  Get the patch mesh INode
     *  \return a pointer to the INode.
     */
	INode *GetNode() const { return m_pINode; }

protected:
    virtual void OnAddShader(RwCommShader *pShader);

	//Get control point data
	RwCommError GetVertexPosition(int iPatchIndex, int x, int y, RwCommVec3& vertex, bool bWorldSpace) const;
	RwCommError GetVertexPositionIndex(int iPatchIndex, int x, int y, int& iIndex) const;
	RwCommError GetVertexNormal(int iPatchIndex, int x, int y, RwCommVec3& normal) const;
	RwCommError GetVertexColor(int iPatchIndex, int x, int y, RwCommVec3& color) const;
	RwCommError	GetVertexAlpha(int iPatchIndex, int x, int y, RwCommReal& alpha) const;
	RwCommError	GetVertexIllumination(int iPatchIndex, int x, int y, RwCommVec3& vIllum) const;
	RwCommError	GetVertexUVs(int iPatchIndex, int x, int y, RwVector< RwCommVec2 >& uvs) const;
    RwCommShader *GetPatchShader(int iPatchIndex) const;

protected:
	PatchMesh       *m_pMesh;
    INode           *m_pINode;
    Object          *m_pObject;
    PatchObject     *m_pPatchObject;
	RwCommMesh      *m_pTessellatedMesh;
    int			    m_iPatchIterator;
    TriObject       *m_pTriObject;

    bool            m_bDelObj;
};

/**
*  \ingroup maxbuilder 
*
*  This class is responsible for creating an RwMaxPatchMesh object.
*
*   \see RwCommPatchMesh
*/
class RWMAX_API RwMaxPatchMeshCreator  : public RwMaxGeometryCreator
{
public:
	RwMaxPatchMeshCreator() : RwMaxGeometryCreator() {}
	virtual ~RwMaxPatchMeshCreator() {}

	virtual RwCommObject *Create(const RwCommCreationData *pCreationData, const RwCommExportOptions *pExportOptions) const;

};

#endif // !defined(AFX_RWMAXPATCHMESH_H__728D84BB_E455_4759_8AC9_EE622A05EA5F__INCLUDED_)
