// RwRf3Mesh.h: interface for the RwRf3Mesh class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RwRf3MESH_H__156F3AD7_3CDF_4A22_9535_71A7E7AC7169__INCLUDED_)
#define AFX_RwRf3MESH_H__156F3AD7_3CDF_4A22_9535_71A7E7AC7169__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwRf3.h"
#include "RwRf3Geometry.h"

#include "RwCommMesh.h"

// Mesh
#define RWRF3_MESH_TAG                 L"polymesh"

/**
*   \ingroup rf3write
*
*   This class is responsible for exporting RwCommMesh objects. In its Export() method the class calls
*   the base class implementation of RwRf3ExpGeometry::Export() for generating all vertex and primitive
*   information. It then goes on and adds triangulation data to the primitive lists, using the
*   RwRf3Triangulation helper class.
*   
*   In its constructor, the class registers an RwRf3RemapFilter in its vertex pipeline, for remapping
*   all exported vertices. 
*
*   \see RwRf3ExpGeometry
*   \see RwRf3Triangulation
*   \see RwExpRemapFilter
*/
class RF3_API RwRf3ExpMesh : public RwRf3ExpGeometry 
{
public:
	RwRf3ExpMesh();
	virtual ~RwRf3ExpMesh();

    virtual RwCommError     Export( RwCommObject* pObject,
                                RwCommObject* pParent,
                                RwCommScene* pScene,
                                const RwCommExportOptions* pExportOptions );

protected:

    virtual RwCommError     ExportToonInfo(RwCommMesh *pMesh);
};

/**
*   \ingroup rf3read
*
*   This class builds an RwMIMesh from an XML node. 
*
*   \see RwMIMesh
*   \see RwRf3GeometryCreator
*/
class RF3_API RwRf3MeshCreator : public RwRf3GeometryCreator
{
public:
    RwRf3MeshCreator() {}
    virtual ~RwRf3MeshCreator() {}

	virtual RwCommObject* Create(const RwCommCreationData* pCreationData,
								const RwCommExportOptions* pExportOptions) const;

protected:

    virtual void BuildTriangulation(RwRf3Triangulation *pTriangulation, RwMIGeometry *pGeometry, int nPrims) const;
    virtual void BuildEdgeList(RwRf3EdgeList *pEdgeList, RwMIGeometry *pGeometry, int nPrims) const;
    virtual void BuildToonInks(RwVector<RwRf3Param*> &vToonInks, RwMIGeometry *pGeometry) const;
};

#endif // !defined(AFX_RwRf3MESH_H__156F3AD7_3CDF_4A22_9535_71A7E7AC7169__INCLUDED_)
