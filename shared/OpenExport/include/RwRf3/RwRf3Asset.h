// RwRf3Asset.h: interface for the RwRf3Asset class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWRF3ASSET_H__F778A1B0_A6B6_4DB4_B71B_47B545F537FD__INCLUDED_)
#define AFX_RWRF3ASSET_H__F778A1B0_A6B6_4DB4_B71B_47B545F537FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwRf3.h"
#include "RwRf3Object.h"
#include "RwRf3ObjCreator.h"

class RwMIAsset;
class RwRf3Builder;
class RwRf3Param;

/**
*   \ingroup rf3write
*
*   This class is responsible for exporting RwCommAsset objects. In its Export() method the class
*   exports the asset's selection, template name, GUID and export options, using the ExportOptions()
*   method. The method attaches its root m_pNode to the XML scene node. 
*
*   \see RwCommAsset
*/
class RF3_API RwRf3ExpAsset  : public RwRf3ExpObject
{
public:
	RwRf3ExpAsset();
	virtual ~RwRf3ExpAsset();

    virtual RwCommError     Export( RwCommObject* pObj,
                                RwCommObject* pParent,
                                RwCommScene* pScene,
                                const RwCommExportOptions* pExportOptions );

protected:

    virtual RwCommError     ExportOptions(RwCommAsset *pAsset);

};

/**
*   \ingroup rf3read
*
*   This class builds an RwMIAsset from an XML node. 
*
*   \see RwMIAsset
*/
class RF3_API RwRf3AssetCreator : public RwRf3ObjCreator
{
public:
    RwRf3AssetCreator() {}
    virtual ~RwRf3AssetCreator() {}

	virtual RwCommObject* Create(const RwCommCreationData* pCreationData,
								const RwCommExportOptions* pExportOptions) const;

	virtual void Attach(RwCommObject* pObject, RwCommObject* pParent, RwCommScene* pScene,
                        const RwCommCreationData* pCreationData) const;

	virtual void Init(	RwCommObject* pObject,
						const RwCommCreationData* pCreationData,
						const RwCommExportOptions* pExportOptions) const;

protected:

    virtual void BuildSelection(RwMIAsset *pAsset, RwXMLNode *pNode, RwRf3Builder *pBuilder) const;
    virtual void BuildAssetName(RwMIAsset *pAsset, RwRf3Param *pNode) const;
    virtual void BuildTemplateName(RwMIAsset *pAsset, RwRf3Param *pNode) const;
    virtual void BuildGuid(RwMIAsset *pAsset, RwRf3Param *pNode) const;
    virtual void BuildCustomExportFile(RwMIAsset *pAsset, RwRf3Param *pNode) const;
    virtual void BuildOptions(RwRf3Param *pNode, RwCommExportOptions &options) const;

};

#endif // !defined(AFX_RWRF3ASSET_H__F778A1B0_A6B6_4DB4_B71B_47B545F537FD__INCLUDED_)
