// RwExpSpline.h: interface for the RwExpSpline class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPSPLINE_H__C80384E6_9CCE_44D1_9129_501BF457995D__INCLUDED_)
#define AFX_RWEXPSPLINE_H__C80384E6_9CCE_44D1_9129_501BF457995D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwCommGeometry.h"

const RwCommType RwCommSplineType("RwCommSpline");

/**
*  \ingroup commscenegraph  
*
*   This is a base class for the spline objects.
*/
class RWCOMM_API RwCommSpline : public RwCommGeometry  
{
public:
    enum VertexType
    {
        EDIT    = 0,
        CONTROL = 1
    };

    RwCommSpline();
    virtual ~RwCommSpline();

    // Type methods
    virtual RwCommType  GetType() const { return RwCommSplineType; }
    virtual void        GetTypeHierarchy(RwVector<RwCommType> &vTypeVec) const;

    // Spline methods
    /**
     *  \param bClosed will be true if the spline in closed and false if not.
     *
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError PrimIsClosed(bool &bClosed) const = 0;

    virtual RwCommError SetVertexType(VertexType type);
};

#endif // !defined(AFX_RWEXPSPLINE_H__C80384E6_9CCE_44D1_9129_501BF457995D__INCLUDED_)
