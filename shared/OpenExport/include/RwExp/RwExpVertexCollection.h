// RwExpVertexCollection.h: interface for the RwExpVertexCollection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPVERTEXCOLLECTION_H__36AD59B8_19B9_4F21_A73F_55D598942120__INCLUDED_)
#define AFX_RWEXPVERTEXCOLLECTION_H__36AD59B8_19B9_4F21_A73F_55D598942120__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwExpVertexPipeline.h"
#include "RwCommShader.h"
#include "RwCommGeometry.h"



/**
*  \ingroup vertexpipeline
*
*   All vertices and primitives coming out of the vertex pipeline are stored in a vertex collection. A vertex
*   collection is a container of vertices and their corresponding primitives. Using the vertex collection you
*   can query all of the final optimized vertices and primitives.
*   
*   The class is responsible for storing the geometry's vertex and primitive data. Once the 
*   RwExpVertexCollection object is initialized, (using the RwExpVertexCollection::Init() method), it goes to
*   its assigned geometry, extracts its vertex and primitive information, pushes them into the vertex 
*   pipeline and stores the resulting vertices and remapped primitives.
*   
*   The class consists of a set of methods for querying the stored vertex and primitive information, after
*   it has gone out of the pipeline.
*
*   \see RwExpVertexPipeline
*/
class RWEXP_API RwExpVertexCollection  
{
public:
    RwExpVertexCollection(RwExpVertexPipeline &rVxPipeline);
    virtual ~RwExpVertexCollection();

    RwCommError Init(RwCommGeometry *pGeometry, const RwCommExportOptions *pExportOptions, bool bWorldSpace = false);
    RwCommError Append(RwCommGeometry *pGeometry, const RwExpVertexCollection &collection);
    RwCommError SortByShaders(RwVector<int> &vIndexMap);

    // Iterate on primitives
    int GetNumPrimitives() const;
    RwCommError ResetPrimIterator();
    RwCommError GetNextPrimitive(const RwCommPrimitive *&pPrimitive);
    RwCommError GetNextPrimitive(const RwCommPrimitive *&pPrimitive, RwCommGeometry *&pGeometry);

    // Iterate on vertices
    int GetNumVertices() const;
    RwCommError ResetVertexIterator();
    RwCommError GetNextVertex(const RwCommVertex *&pVertex);

    /** Get all the vertex IDs in the collection.*/
    const RwVector<RwCommVertexId> &GetVertexIds() const { return m_vVertexIds; }

    /** Get all vertices in the collection (in their original index order)*/
    const RwVector<RwCommVertex> &GetVertices() const { return m_vVertices; }

    /** Get all the primitives in the collection.*/
    const RwVector<RwCommPrimitive> &GetPrimitives() const { return m_vPrimitives; }

    /** Get the maximum number of texture channels in the collection.*/
    int GetMaxTexCoords() const { return m_iMaxTexCoords; }

    /** Calculate the maximum number of texture channels in the collection */
    void CalculateMaxTexCoords();

    /** Get the vertex data format in the collection.*/
    unsigned int GetVertexData() const { return m_iVertexFormat; }

    /** Get all source geometries in the collection. If the collection hasn't been merged 
        only the initialized source geometry will be returned,*/
    const RwVector<RwCommGeometry *> &GetSourceGeometries() const { return m_vGeometries; }

    /** Returns an index map, mapping the original geometry primitive order to the newly sorted
    *   primitive order.
    *
    *   \param pSourceGeometry a pointer to the source geometry of the collection
    *   \param vPrimIndices the primitive index map
    */
    void GetPrimitiveMap(RwCommGeometry *pSourceGeometry, RwVector<int> &vPrimIndices);

protected:

    void    TransformVertex(const RwCommMatrix &M, const RwCommMatrix &N, RwCommVertex &vertex) const;
    void    TransformWorldSpace(RwCommGeometry *pGeometry, RwVector<RwCommVertex> &vertices);
    void    Dump(const RwString &file, RwCommGeometry *pGeometry);

protected:

    RwExpVertexPipeline                     &m_rVxPipeline;
    RwVector<RwCommVertex>              m_vVertices;
    RwVector<RwCommPrimitive>           m_vPrimitives;
    RwVector<RwCommVertexId>            m_vVertexIds;

    RwVector<RwCommGeometry *>          m_vGeometries;
    RwVector<int>                       m_vGeomPrims;

    int                                     m_iPrimIterator;
    RwVector<RwCommVertex>::iterator    m_vxIterator;
    int                                     m_iMaxTexCoords;
    unsigned int                            m_iVertexFormat;
};

#endif // !defined(AFX_RWEXPVERTEXCOLLECTION_H__36AD59B8_19B9_4F21_A73F_55D598942120__INCLUDED_)
