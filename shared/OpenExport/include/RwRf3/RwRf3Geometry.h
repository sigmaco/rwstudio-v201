// RwRf3Geometry.h: interface for the RwRf3Geometry class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RwRf3GEOMETRY_H__30FBB54A_1FD7_4DC8_96E3_09BBAEA9EFFD__INCLUDED_)
#define AFX_RwRf3GEOMETRY_H__30FBB54A_1FD7_4DC8_96E3_09BBAEA9EFFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwRf3.h"
#include "RwRf3Object.h"
#include "RwRf3ObjCreator.h"

#include "RwExpVertexPipeline.h"
#include "RwExpVertexCollection.h"
#include "RwExpRemapFilter.h"

class RwMIGeometry;
class RwRf3Triangulation;
class RwRf3EdgeList;
class RwRf3VertexList;
class RwRf3PrimList;

#define RWRF3_GEOMETRY_COLOR                L"color"
#define RWRF3_GEOMETRY_TOON_INK             L"toonink"        
#define RWRF3_GEOMETRY_TOON_INK_TYPE        L"type"
#define RWRF3_GEOMETRY_TOON_INK_NAME        L"inkname"

// Class RwRf3ExpGeometry
/**
*   \ingroup rf3write
*
*   This is the base class for all Rf3 geometry exporters. In its Export() method the class uses a vertex pipeline
*   and a vertex collection for extracting all of the RwCommGeometry object's vertex and primitive information.
*   It then calls ExportVertices() and ExportPrimitives() to export this information.
*
*   \see RwCommGeometry
*   \see RwExpVertexCollection
*   \see RwExpVertexPipeline
*/
class RF3_API RwRf3ExpGeometry  : public RwRf3ExpObject
{
public:
	RwRf3ExpGeometry();
	virtual ~RwRf3ExpGeometry();

    virtual RwCommError     Export( RwCommObject* pObject,
                                RwCommObject* pParent,
                                RwCommScene* pScene,
                                const RwCommExportOptions* pExportOptions );

    /** Get the RwCommGeometry vertex collection*/
    const RwExpVertexCollection &GetVertexCollection() const { return m_collection; }

    /** Get the vertex pipeline, applied on the extracted vertices*/
    RwExpVertexPipeline &GetVertexPipeline() { return m_vertexPipeline; }

protected:

    virtual RwCommError ExportVertices(  RwCommObject* pObject );
    virtual RwCommError ExportPrimitives(  RwCommObject* pObject );
    virtual RwCommError ExportMorphTargets( RwCommGeometry* pGeometry, const RwCommExportOptions* pExportOptions );
    virtual RwCommError ExportColor(RwCommGeometry*  pGeometry);

    RwCommGeometry      *GetInstance(RwCommGeometry *pGeometry);
    bool EnableAttachedBlendShape(bool bEnable, RwCommGeometry* pGeometry, RwVector<RwCommReal>& vWeights);

protected:

    RwExpVertexPipeline     m_vertexPipeline;
    RwExpVertexCollection   m_collection;

    RwVector<int>        m_vPrimIndices;
    RwVector<RwXMLNode*> m_vPrimLists;

};



// Class RwRf3GeometryCreator
/**
*   \ingroup rf3read
*
*   This is the base class for all geometry creators. The class reads in all vertex and primitive data
*   from an XML node, and sets it to the RwCommGeometry object being built. Since this is a base class it
*   does not explicitly create the object, but handles the Init() and Attach() process.
*
*   \see RwCommGeometry
*/
class RF3_API RwRf3GeometryCreator : public RwRf3ObjCreator
{
public:
    RwRf3GeometryCreator() {}
    virtual ~RwRf3GeometryCreator() {}

	virtual void Attach(RwCommObject* pObject, RwCommObject* pParent, RwCommScene* pScene,
                        const RwCommCreationData* pCreationData) const;

	virtual void Init(	RwCommObject* pObject,
						const RwCommCreationData* pCreationData,
						const RwCommExportOptions* pExportOptions) const;

protected:

    virtual void BuildVertexList(RwRf3VertexList *pVertexList, RwMIGeometry *pGeometry) const;
    virtual void BuildPrimList(RwRf3PrimList *pPrimList, RwMIGeometry *pGeometry, RwCommScene *pScene) const;
    virtual void BuildColor(RwRf3Param *pColor, RwMIGeometry *pGeometry) const;

    // Overridable by triangulated geometries
    virtual void BuildTriangulation(RwRf3Triangulation *pTriangulation, RwMIGeometry *pGeometry, int nPrims) const {}

    // Overridable by toon geometries 
    virtual void BuildEdgeList(RwRf3EdgeList *pEdgeList, RwMIGeometry *pGeometry, int nPrims) const {}
    virtual void BuildToonInks(RwVector<RwRf3Param*> &vToonInks, RwMIGeometry *pGeometry) const {}

};

// Class RwRf3RemapFilter
const RwCommType RwRf3RemapFilterType("RwRf3RemapFilter");

/**
*   \ingroup rf3write
*
*   This filter remaps all shared vertices, to avoid exporting duplicate vertex data into the rf3 file.
*
*   \see RwRf3ExpMesh
*   \see RwRf3ExpPatchMesh
*   \see RwRf3ExpSpline
*/
class RF3_API RwRf3RemapFilter   : public RwExpRemapFilter
{
public:
    RwRf3RemapFilter() {}
    virtual ~RwRf3RemapFilter() {}

    virtual RwCommType GetType() const { return RwRf3RemapFilterType; }

    virtual unsigned GetVertexDataMask() const { return REMAPPER_MASK; }

    virtual bool Init(RwCommGeometry *pGeometry, const RwCommExportOptions *pExportOptions)
    {
        //Update the remap field mask
        RwExpVxCompare::m_iVertexFieldMask =    (RwCommVertex::POSITION |
                                                 RwCommVertex::NORMAL |
                                                 RwCommVertex::COLOR |
                                                 RwCommVertex::ALPHA |
                                                 RwCommVertex::ILLUM |
                                                 RwCommVertex::UVS |
                                                 RwCommVertex::ID);
        return true;
    }
};
#endif // !defined(AFX_RwRf3GEOMETRY_H__30FBB54A_1FD7_4DC8_96E3_09BBAEA9EFFD__INCLUDED_)
