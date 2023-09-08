// RwRf3PatchMesh.h: interface for the RwRf3PatchMesh class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RwRf3PATCHMESH_H__E094F35F_2D17_4813_BA83_50A571387187__INCLUDED_)
#define AFX_RwRf3PATCHMESH_H__E094F35F_2D17_4813_BA83_50A571387187__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwRf3.h"
#include "RwRf3Geometry.h"

#include "RwCommPatchMesh.h"

#define RWRF3_PATCH_TAG                 L"patchmesh"
#define RWRF3_PATCH_GEOMTYPE            L"type"
#define RWRF3_PATCH_DEGREE              L"degree"

/**
*   \ingroup rf3write
*
*   This class is responsible for exporting RwCommPatchMesh objects. In its Export() method the class calls
*   the base class implementation of RwRf3ExpGeometry::Export() for generating all vertex and primitive
*   information. 
*   
*   In its constructor, the class registers an RwExpFitPatchFilter, for fitting patches, and
*   an RwRf3RemapFilter, for remapping vertices, in its vertex pipeline.
*
*   \see RwRf3ExpGeometry
*   \see RwExpFitPatchFilter
*   \see RwExpRemapFilter
*/
class RF3_API RwRf3ExpPatchMesh  : public RwRf3ExpGeometry 
{
public:
	RwRf3ExpPatchMesh();
	virtual ~RwRf3ExpPatchMesh();

    virtual RwCommError     Export( RwCommObject* pObject,
                                RwCommObject* pParent,
                                RwCommScene* pScene,
                                const RwCommExportOptions* pExportOptions );

};

/**
*   \ingroup rf3read
*
*   This class builds an RwMIPatchMesh from an XML node. 
*
*   \see RwMIPatchMesh
*   \see RwRf3GeometryCreator
*/
class RF3_API RwRf3PatchMeshCreator : public RwRf3GeometryCreator
{
public:
    RwRf3PatchMeshCreator() {}
    virtual ~RwRf3PatchMeshCreator() {}

	virtual RwCommObject* Create(const RwCommCreationData* pCreationData,
								const RwCommExportOptions* pExportOptions) const;

};

#endif // !defined(AFX_RwRf3PATCHMESH_H__E094F35F_2D17_4813_BA83_50A571387187__INCLUDED_)
