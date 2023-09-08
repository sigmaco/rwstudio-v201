// RwExpColorOps.h: interface for the RwExpColorOps class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPCOLOROPS_H__F3F7FB8F_0507_410B_8A83_F9667A297974__INCLUDED_)
#define AFX_RWEXPCOLOROPS_H__F3F7FB8F_0507_410B_8A83_F9667A297974__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwExp.h"
#include "RwExpVertexFilter.h"

const RwCommType RwExpSelfIllumFilterType("RwExpSelfIllumFilter");

/**
*
*  \ingroup vertexpipeline
*
*   This vertex filter is responsible for baking the vertex illumination and self illumination values into the
*   vertex's color values. The filter is enabled only if the COLORVERTEXPRELIGHT option or the 
*   DFF_PRELIGHTCLUMP option are turned on.   
*/
class RWEXP_API RwExpSelfIllumFilter : public RwExpVertexFilter  
{
public:
    RwExpSelfIllumFilter() {}
    virtual ~RwExpSelfIllumFilter() {}

    virtual RwCommType GetType() const { return RwExpSelfIllumFilterType; }

    virtual unsigned GetVertexDataMask() const { return RwCommVertex::COLOR; }

    // Initialise the filter. Called at the beginning of the pipeline.
    virtual bool Init(RwCommGeometry *pGeometry, const RwCommExportOptions *pGlobalData);

    // Apply the filter 
     virtual void Do(RwVector<RwCommVertex *> &vertices, RwVector<RwCommPrimitive> &primitives,
        RwCommGeometry *pGeometry, const RwCommExportOptions *pExportOptions);

};

#endif // !defined(AFX_RWEXPCOLOROPS_H__F3F7FB8F_0507_410B_8A83_F9667A297974__INCLUDED_)
