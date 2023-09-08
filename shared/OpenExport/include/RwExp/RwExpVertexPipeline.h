// RwExpVertexPipeline.h: interface for the RwExpVertexPipeline class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPVERTEXPIPELINE_H__F2088D4C_0E36_45CC_A437_730F8074C518__INCLUDED_)
#define AFX_RWEXPVERTEXPIPELINE_H__F2088D4C_0E36_45CC_A437_730F8074C518__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma warning(disable : 4786)
#pragma warning(disable : 4251)

#include <list>

#include "RwExp.h"
#include "RwExpVertexFilter.h"

#include "RwCommGeometry.h"


/**
*  \ingroup vertexpipeline
*
*   This class is the vertex pipeline manager. It manages a queue of vertex filters, to which all the vertex and
*   primitive information is passed. Each vertex filter, in turn, applies a certain action on the vertices.
*   These actions are usually vertex or primitive modification actions, changing any data field in the vertices
*   or in their associated primitives. Besides modifying the vertices, the vertex filters
*   can also re-map vertices to be shared across primitives, by modifiying the primitive data.
*   
*   The class consists of two groups of methods. One for pushing the vertex information into the pipeline
*   and the other for managing the vertex filters. Since the RwExpVertexCollection class pushes the
*   vertex information into the pipeline, using the first group, most users will only be interested in the second
*   group of methods, i.e. the vertex filters methods.
*   
*   Most vertex pipeline instances are static members of geometry exporters, shared among the geometry exporters.
*   Users can access them directly, without referencing a certain geometry exporter instance.
*
*   \see RwExpVertexCollection
*   \see RwExpVertexFilter
*/

class RWEXP_API RwExpVertexPipeline  
{
public:
    RwExpVertexPipeline();
    virtual ~RwExpVertexPipeline();

    // Initialise vertex pipeline
    void Init(RwCommGeometry *pGeometry, const RwCommExportOptions *pExportOptions, 
        unsigned filterMask = 0xffff);

    // Flush the vertex data down the pipeline
    void Flush(RwVector<RwCommVertex> &vVertexBuffer, RwVector<RwCommVertexId> &vVertexIds,
               RwVector<RwCommPrimitive> &vPrimitives);

    // Flush the vertices, without passing any primitive data
    void Flush(RwVector<RwCommVertex> &vVertexBuffer);

    // Filter methods

    // Get all filter types
    void GetFilterTypes(RwVector<RwCommType> &vFilterTypes) const;

    // Get filter by type
    RwExpVertexFilter *GetFilter(const RwCommType &type) const;

    // Enable/disable filter by type
    RwCommError EnableFilter(const RwCommType &type, bool bEnable);

    // Add filter at end/start of the pipeline
    RwCommError AddFilter(RwExpVertexFilter *pFilter, bool bAtEnd = true);

    // Insert filter before/after given filter type
    RwCommError InsertFilter(const RwCommType &type, RwExpVertexFilter *pFilter, bool bBefore = true);

    // Insert filter before given index
    RwCommError InsertFilter(int index, RwExpVertexFilter *pFilter);

   // Replaces a filter specified by type with a new filter
    RwCommError ReplaceFilter(const RwCommType &type, RwExpVertexFilter *pFilter);

    // Removes a filter by type
    RwCommError RemoveFilter(const RwCommType &type);

protected:

    // Clean up the filters. Called at the end of the pipeline.
    void CleanUp(RwCommGeometry *pGeometry, const RwCommExportOptions *pExportOptions);

protected:

    RwList<RwExpVertexFilter *>     m_lFilters;
    RwVector<RwExpVertexFilter *>   m_vActiveFilters;

    RwCommGeometry                      *m_pGeometry;
    const RwCommExportOptions           *m_pExportOptions;

    bool                                m_bInitPrimData;
};

#endif // !defined(AFX_RWEXPVERTEXPIPELINE_H__F2088D4C_0E36_45CC_A437_730F8074C518__INCLUDED_)
