// RwMIPatchMesh.h: interface for the RwMIPatchMesh class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWMIPATCHMESH_H__331E460C_4391_4622_87D4_827A2B3DB8CD__INCLUDED_)
#define AFX_RWMIPATCHMESH_H__331E460C_4391_4622_87D4_827A2B3DB8CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwMI.h"
#include "RwCommPatchMesh.h"
#include "RwMIGeometry.h"
#include "RwMIAttributes.h"

/**
*   \ingroup miscenegraph
*
*   This is the MI implementation of RwCommPatchMesh.
*
*/
class RWMI_API RwMIPatchMesh : public RwCommPatchMesh, public RwMIAttributes, public RwMIGeometry 
{
public:
    RwMIPatchMesh();
    virtual ~RwMIPatchMesh();

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
    virtual RwCommError GetAttributeNames(RwVector<RwWString> &vAttributeNames,
                                          RwVector<AttributeType> &vAttributeTypes);

    // Shape methods
    virtual RwCommError     GetVisibility(bool &bIsVisible) const;

   // Bounding box
    virtual RwCommError GetWorldSpaceBoundingBox(RwCommBoundingBox &) const;

    // Get vertex by id
    virtual RwCommError GetVertex(const RwCommVertexId &id, RwCommVertex &vertex,
                                  bool bWorldSpace = false);

    // Geometry methods
    virtual RwCommError GetNumPrimitives(int &nPrims) const;
    virtual RwCommError ResetPrimIterator();
    virtual RwCommError AdvancePrimIterator();
    virtual RwCommError GetPrimVertexCount(int &nVertices);
    virtual RwCommError GetPrimVertex(int index, RwCommVertex &vertex, RwCommVertexId &id,
                                      bool bWorldSpace = false);
    virtual RwCommError GetPrimIndex(int &nIndex);
    virtual RwCommError GetPrimShader(RwCommShader *&pShader);
    virtual RwCommError GetPrimNormal(RwCommVec3 &normal, bool bWorldSpace = false)
                                     { return RWCOMM_NOT_IMPLEMENTED; }
    virtual RwCommError GetDefaultColor(RwCommVec3 &color) const;

    // Patch methods
    virtual RwCommError GetPatchType(PatchType &type) const;
    virtual RwCommError GetPatchDegree(int &degree) const;
    virtual RwCommError InterpolatePatch(RwCommReal u, RwCommReal v, RwCommReal w,
                                         RwCommVec3 &p) const;
    virtual RwCommError Tessellate(RwCommMesh *&pMesh);

protected:
    virtual RwCommError OnSetFrame(RwCommReal frame);
};

#endif // !defined(AFX_RWMIPATCHMESH_H__331E460C_4391_4622_87D4_827A2B3DB8CD__INCLUDED_)
