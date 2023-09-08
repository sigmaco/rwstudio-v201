// RwMISpline.h: interface for the RwMISpline class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWMISPLINE_H__3FD7237B_2C77_457E_AE12_D3616C7A4137__INCLUDED_)
#define AFX_RWMISPLINE_H__3FD7237B_2C77_457E_AE12_D3616C7A4137__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwMI.h"
#include "RwCommSpline.h"
#include "RwMIGeometry.h"
#include "RwMIAttributes.h"

/**
*   \ingroup miscenegraph
*
*   This is the MI implementation of RwCommSpline. The class caches the prim state and
*   vertex type.
*
*/
class RWMI_API RwMISpline : public RwCommSpline, public RwMIAttributes, public RwMIGeometry 
{
public:
    RwMISpline();
    virtual ~RwMISpline();

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

    // RwCommSpline methods
    virtual RwCommError PrimIsClosed(bool &bClosed) const;
    virtual RwCommError SetVertexType(VertexType type);

    // RwMISpline methods
    RwCommError SetPrimIsClosed(int nPrim, bool state);

protected:
    virtual RwCommError OnSetFrame(RwCommReal frame);

protected:
    RwVector<bool>  m_vPrimState;
    VertexType          m_vertexType;
};

#endif // !defined(AFX_RWMISPLINE_H__3FD7237B_2C77_457E_AE12_D3616C7A4137__INCLUDED_)
