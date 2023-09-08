// RwExpVertexDataFilter.h: interface for the RwExpVertexDataFilter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPVERTEXDATAFILTER_H__13D3A8B3_971B_49BC_8816_36CF0BAC6AAC__INCLUDED_)
#define AFX_RWEXPVERTEXDATAFILTER_H__13D3A8B3_971B_49BC_8816_36CF0BAC6AAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwExp.h"
#include "RwExpVertexFilter.h"


const RwCommType RwExpVertexDataFilterType("RwExpVertexDataFilter");

/**
*
*  \ingroup vertexpipeline
*
*   This vertex filter is responsible for filtering out vertex data fields, according to the export options.
*   It first checks if its associated geometry has the Export Normals/Export Pre-Lights enabled/disabled. If not,
*   it checks the export options value. 
*/
class RWEXP_API RwExpVertexDataFilter  : public RwExpVertexFilter  
{
public:
    RwExpVertexDataFilter();
    virtual ~RwExpVertexDataFilter();

    virtual RwCommType GetType() const { return RwExpVertexDataFilterType; }

    virtual unsigned GetVertexDataMask() const { return RwCommVertex::NORMAL | RwCommVertex::COLOR; }

    // Initialise the filter. Called at the beginning of the pipeline.
    virtual bool Init(RwCommGeometry *pGeometry, const RwCommExportOptions *pGlobalData);

    // Apply the filter 
    virtual void Do(RwVector<RwCommVertex *> &vertices, RwVector<RwCommPrimitive> &primitives,
        RwCommGeometry *pGeometry, const RwCommExportOptions *pExportOptions);

protected:

    int m_bNormals;
    int m_bColors;

};

#endif // !defined(AFX_RWEXPVERTEXDATAFILTER_H__13D3A8B3_971B_49BC_8816_36CF0BAC6AAC__INCLUDED_)
