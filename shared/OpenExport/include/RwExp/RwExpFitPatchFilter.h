// RwExpFitPatch.h: interface for the RwExpFitPatch class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPFITPATCH_H__66735492_DDBF_417D_A847_9F7FC21A3D2B__INCLUDED_)
#define AFX_RWEXPFITPATCH_H__66735492_DDBF_417D_A847_9F7FC21A3D2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwExp.h"
#include "RwExpVertexFilter.h"

#include "RwCommPatchMesh.h"

#include "rtbezpat.h"

const RwCommType RwExpFitPatchFilterType("RwExpFitPatchFilter");

/**
*
*  \ingroup vertexpipeline
*
*   This vertex filter is responsible for fitting all patches, with degree different than 4, or
*   with no available normal data.   
*/
class RWEXP_API RwExpFitPatchFilter : public RwExpVertexFilter  
{
public:
    RwExpFitPatchFilter();
    virtual ~RwExpFitPatchFilter();

    virtual RwCommType GetType() const { return RwExpFitPatchFilterType; }

    virtual unsigned GetVertexDataMask() const { return RwCommVertex::POSITION | RwCommVertex::NORMAL; }

    // Apply the filter 
    virtual void Do(RwVector<RwCommVertex *> &vertices, RwVector<RwCommPrimitive> &primitives,
        RwCommGeometry *pGeometry, const RwCommExportOptions *pExportOptions);

    /**
     *  Mirror flag - set to true if the patch should be mirrored
     *  \param a bool indicating the mirror state to be set.
     **/
    void SetMirrorFlag(bool bIsMirrored) { m_bIsMirrored = bIsMirrored; }

protected:

    void FitTriPatch(RwVector<RwCommVertex *> &vertices, RwCommPatchMesh *pPatchMesh,
        int iVertexIndex, RtBezierMatrix &P);

    void FitNormals(RwVector<RwCommVertex *> &vertices, RwCommPatchMesh *pPatchMesh,
        int iVertexIndex, int nVxsPerPrim, RtBezierMatrix &P);

protected:

    bool    m_bIsMirrored;
};

#endif // !defined(AFX_RWEXPFITPATCH_H__66735492_DDBF_417D_A847_9F7FC21A3D2B__INCLUDED_)
