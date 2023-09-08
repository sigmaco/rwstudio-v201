// RwExpRemapFilter.h: interface for the RwExpRemapFilter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPREMAPFILTER_H__1AD9D036_E876_4654_B695_7513EDF3A522__INCLUDED_)
#define AFX_RWEXPREMAPFILTER_H__1AD9D036_E876_4654_B695_7513EDF3A522__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable : 4786)
#pragma warning(disable : 4251)

#include <set>
#include <map>

#include "RwExpVertexFilter.h"

/**
*
*  \ingroup vertexpipeline
*   Vertex comparison function. The function defines a relative sort order between the two given vertices.
*   The function should return true if v1 strictly precedes v2 in the sort order.
*   If v1 and v2 are equivalent, then both RwExpVxCompareFunc(v1,v2) and RwExpVxCompareFunc(v2,v1) should return
*   false.
*
*   \param v1 the first vertx
*   \param v2 the second vertex
*
*   \returns true if v1 strictly precedes v2 in the sort order, false otherwise.
*
*   \see RwExpRemapFilter
*   \see RwExpVxCompare
*/
typedef bool (*RwExpVxCompareFunc)(const RwCommVertex &v1 , const RwCommVertex &v2);

/**
*
*  \ingroup vertexpipeline
*   The default vertex comparison function, used by RwExpRemapFilter.
*
*   \see RwExpRemapFilter
*   \see RwExpVxCompare
*/
RWEXP_API bool RwExpVxCompareDefault(const RwCommVertex &a, const RwCommVertex &b);


struct RWEXP_API RwExpVertexInfo
{
    RwExpVertexInfo(int _nVertex, int _nPrimitive, RwCommVertex *_vertex, int _index) :
        nVertex(_nVertex), nPrimitive(_nPrimitive), vertex(_vertex), index(_index) {}

    RwExpVertexInfo& operator=(const RwExpVertexInfo &that)
    {
        vertex = that.vertex;
        index = that.index;
        return *this;
    }

    /** The local index of the vertex in the primitive*/
    const int                   nVertex;

    /** The index of the vertex's primitive in the vertex pipeline*/
    const int                   nPrimitive;

    /**  A pointer to the vertex.*/
    RwCommVertex                *vertex;

    /** The global index of the vertex in the pipeline's vertex array*/
    int                         index;
};

/**
*
*  \ingroup structures
*   The RwExpRemapFilter comparison criteria. Use the RwExpVxCompare::SetComparisonFunc() to set
*   your own custom vertex comparison criteria.
*
*   \see RwExpVxCompareFunc
*   \see RwExpRemapFilter
*/
struct RWEXP_API RwExpVxCompare
{
    bool operator() (const RwExpVertexInfo &, const RwExpVertexInfo &) const;

    /** Set the comparison function.*/
    static void SetComparisonFunc(RwExpVxCompareFunc pCompFunc) { m_pCompareFunc = pCompFunc; }

    /** The vertex field mask, which is updated according to the export options. The mask can be used
    by comparison functions to check remapping on the masked fields only.*/
    static unsigned             m_iVertexFieldMask;
private:

    static RwExpVxCompareFunc   m_pCompareFunc;
};






const RwCommType RwExpRemapFilterType("RwExpRemapFilter");
/**
*
*  \ingroup vertexpipeline
*
*   This vertex filter is responsible for remapping vertices to be shared across geometry primitives. The filter
*   tests for each vertex, if it is equivalent to a previously processed vertex, using a comparison criteria. If
*   the vertices are equivalent, the second vertex is remapped to the first one, and all primitives using that vertex
*   will now use the first one. Users can supply their own custom comparison criteria by using the RwExpVxCompare::SetComparisonFunc()
*   static method, and specifying the comparison function.
*
*   \see RwExpVxCompare
*   \see RwExpVxCompareFunc
*/
class RWEXP_API RwExpRemapFilter : public RwExpVertexFilter  
{
public:
    RwExpRemapFilter();
    virtual ~RwExpRemapFilter();

    virtual RwCommType GetType() const { return RwExpRemapFilterType; }

    virtual unsigned GetVertexDataMask() const { return REMAPPER_MASK; }

    virtual bool Init(RwCommGeometry *pGeometry, const RwCommExportOptions *pExportOptions);

    // Apply the filter 
    virtual void Do(RwVector<RwCommVertex *> &vertices, RwVector<RwCommPrimitive> &primitives,
        RwCommGeometry *pGeometry, const RwCommExportOptions *pExportOptions);

    // Clean up the filter. Called at the end of the pipeline.
    virtual void CleanUp(RwCommGeometry *pGeometry, const RwCommExportOptions *pExportOptions);

public:
    static unsigned                         m_iRemapField;

protected:

    void PrintStats(const RwString &name, int nInputVertices, int nNormals, int nUvs, int nColors);
    void UnifyVertex(RwCommVertex &vertex, unsigned int mask, int nUvs);
    void DumpVertices(const RwString &filename, const RwString &object);

protected:
    RwSet<RwExpVertexInfo, RwExpVxCompare>    m_sVertices;
};

#endif // !defined(AFX_RWEXPREMAPFILTER_H__1AD9D036_E876_4654_B695_7513EDF3A522__INCLUDED_)
