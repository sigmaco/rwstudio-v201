// RwExpSortPrimFilter.h: interface for the RwExpSortPrimFilter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPSORTPRIMFILTER_H__4D7A209A_6E1F_4E27_AEF1_0F9F8C735945__INCLUDED_)
#define AFX_RWEXPSORTPRIMFILTER_H__4D7A209A_6E1F_4E27_AEF1_0F9F8C735945__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwExpVertexFilter.h"

const RwCommType RwExpSortPrimFilterType("RwExpSortPrimFilter");
/**
*
*  \ingroup vertexpipeline
*
*   This vertex filter is responsible for sorting all the primitives according to their assigned shaders,
*   and spatially, in 3D space, within the same shader group.
*/
class RwExpSortPrimFilter  : public RwExpVertexFilter  
{
public:
    RwExpSortPrimFilter();
    virtual ~RwExpSortPrimFilter();

    virtual RwCommType GetType() const { return RwExpSortPrimFilterType; }

    virtual unsigned GetVertexDataMask() const { return REMAPPER_MASK; }

    // Apply the filter 
    virtual void Do(RwVector<RwCommVertex *> &vertices, RwVector<RwCommPrimitive> &primitives,
        RwCommGeometry *pGeometry, const RwCommExportOptions *pExportOptions);

};

#endif // !defined(AFX_RWEXPSORTPRIMFILTER_H__4D7A209A_6E1F_4E27_AEF1_0F9F8C735945__INCLUDED_)
