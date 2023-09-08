// RwMIMesh.h: interface for the RwMIMesh class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RwMIMesh_H__8FADC0A8_CC61_4722_B64B_6D39CC9B7D2A__INCLUDED_)
#define AFX_RwMIMesh_H__8FADC0A8_CC61_4722_B64B_6D39CC9B7D2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwMI.h"
#include "RwCommMesh.h"
#include "RwMIGeometry.h"
#include "RwMIAttributes.h"

/**
*   \ingroup miscenegraph
*
*   This is the MI implementation of RwCommMesh.. The class caches the mesh's
*   triangulation data.
*
*/
class RWMI_API RwMIMesh : public RwCommMesh, public RwMIAttributes, public RwMIGeometry 
{
public:
    RwMIMesh();
    virtual ~RwMIMesh();

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
    virtual RwCommError GetVisibility(bool &bIsVisible) const;

   // Bounding box
    virtual RwCommError GetWorldSpaceBoundingBox(RwCommBoundingBox&) const;

    // Get vertex by id
    virtual RwCommError GetVertex(const RwCommVertexId &id, RwCommVertex &vertex,
                                  bool bWorldSpace = false);

    // Primitive methods
    virtual RwCommError GetNumPrimitives(int &nPrims) const;
    virtual RwCommError ResetPrimIterator();
    virtual RwCommError AdvancePrimIterator();
    virtual RwCommError GetPrimVertexCount(int &nVertices);
    virtual RwCommError GetPrimVertex(int index, RwCommVertex &vertex, RwCommVertexId &id,
                                      bool bWorldSpace = false);
    virtual RwCommError GetPrimIndex(int &nIndex);
    virtual RwCommError GetPrimShader(RwCommShader *&pShader);
    virtual RwCommError GetPrimNormal(RwCommVec3 &normal, bool bWorldSpace = false);
    virtual RwCommError GetPrimTriangleCount(int &nTriangles);
    virtual RwCommError GetPrimTriangulation(RwVector< int > &triangleIndices);
    virtual RwCommError GetPrimEdges(RwVector<RwCommEdge> &edges);
    virtual RwCommError GetDefaultColor(RwCommVec3 &color) const;
    virtual RwCommError GetToonInks(RwVector<RwCommToonInk> &toonInks);

    // RwMIMesh methods

    RwCommError AddTriangulation(const RwVector<int> &vTriangulation);
    RwCommError AddPrimEdges(const RwVector<RwCommEdge> &vEdges);
    RwCommError SetToonInks(const RwVector<RwCommToonInk> &vToonInks);

protected:
    RwCommError ApplyModifiers(RwCommVertex &vertex, int index) const;
    virtual RwCommError OnSetFrame(RwCommReal frame);

protected:
    RwVector< RwVector<int> >           m_vTriangulation;
    RwVector< RwVector<RwCommEdge> >    m_vEdges;
    RwVector<RwCommToonInk>                 m_vToonInks;
};

#endif // !defined(AFX_RwMIMesh_H__8FADC0A8_CC61_4722_B64B_6D39CC9B7D2A__INCLUDED_)
