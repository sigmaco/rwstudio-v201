// RwMaxSpline.h: interface for the RwMaxSpline class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWMAXSPLINE_H__B7A97B33_6AD1_490F_9B32_D3C6FD0B8806__INCLUDED_)
#define AFX_RWMAXSPLINE_H__B7A97B33_6AD1_490F_9B32_D3C6FD0B8806__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma warning(disable : 4786)
#pragma warning(disable : 4251)


#include "RwCommSpline.h"
#include "RwMaxCustomAttributes.h"
#include "RwMaxGeometry.h"

/**
*  \ingroup maxscenegraph 
*
*   This is the RwMax implementation of the RwCommSpline class. The class encapsulates an INode object and its
*   SplineShape object.
*
*   \see RwCommSpline
*/
class RWMAX_API RwMaxSpline  : public RwCommSpline, public RwMaxGeometry
{
public:
	RwMaxSpline(INode *pNode);
	virtual ~RwMaxSpline();

    virtual RwCommError GetUserData(RwCommUserDataCollection &userDataColl);

	//Animatable methods
	RwCommError OnSetFrame(RwCommReal frame);

   // Shape methods
    virtual RwCommError     GetVisibility(bool &bIsVisible) const;

   // Bounding box
	virtual RwCommError GetWorldSpaceBoundingBox(RwCommBoundingBox &) const;

    // Reevaluate geometry
	virtual RwCommError ReEvaluate(RwCommReal frame);

    // Get vertex by id
    virtual RwCommError GetVertex(const RwCommVertexId &id, RwCommVertex &vertex, bool bWorldSpace = false) { return RWCOMM_NOT_IMPLEMENTED; }

    // Primitive methods
    virtual RwCommError GetNumPrimitives(int &nPrims) const;
    virtual RwCommError ResetPrimIterator();
    virtual RwCommError AdvancePrimIterator();
    virtual RwCommError GetPrimVertexCount(int &nVertices);
    virtual RwCommError GetPrimVertex(int index, RwCommVertex& vertex, RwCommVertexId &id, bool bWorldSpace = false);
    virtual RwCommError GetPrimIndex(int &nIndex);
    virtual RwCommError GetPrimShader(RwCommShader *&pShader) ;
    virtual RwCommError GetPrimNormal(RwCommVec3 &normal, bool bWorldSpace = false) { return RWCOMM_NOT_IMPLEMENTED; }
    virtual RwCommError GetDefaultColor(RwCommVec3 &color) const;

    // Spline methods
    virtual RwCommError PrimIsClosed(bool &bClosed) const;

    /**
     *  Get the encapsualted SplineShape object
     *  \return 3ds max SplineShape
     */
    SplineShape     *GetSplineShape() const { return m_pSplineShape; }

    /** Get the INode object*/
    INode           *GetINode() const { return m_pINode; }

protected:

	SplineShape     *m_pSplineShape;
    INode           *m_pINode;
    int             m_iSplineIterator;
};


/**
*  \ingroup maxbuilder 
*
*  This class is responsible for creating an RwMaxSpline object.
*
*   \see RwMaxSplineCreator
*/
class RWMAX_API RwMaxSplineCreator  : public RwMaxGeometryCreator
{
public:
	RwMaxSplineCreator();
	virtual ~RwMaxSplineCreator();

	virtual RwCommObject *Create(const RwCommCreationData *pCreationData,
								 const RwCommExportOptions *pExportOptions) const;


};

#endif // !defined(AFX_RWMAXSPLINE_H__B7A97B33_6AD1_490F_9B32_D3C6FD0B8806__INCLUDED_)
