// RwRf3Skin.h: interface for the RwRf3Skin class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWRF3SKIN_H__9A1482DC_54ED_44CD_A5DE_0A23254082A4__INCLUDED_)
#define AFX_RWRF3SKIN_H__9A1482DC_54ED_44CD_A5DE_0A23254082A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwRf3.h"
#include "RwRf3Object.h"
#include "RwRf3ObjCreator.h"

class RwCommSkin;
class RwCommTransform;
class RwMISkin;
class RwRf3Builder;

#define RWRF3_SKIN_TAG              L"skin"
#define RWRF3_SKIN_SKELETON         L"skeleton"
#define RWRF3_SKIN_SKELETON_BONES   L"bones"
#define RWRF3_SKIN_WEIGHT_LIST      L"weightlist"
#define RWRF3_SKIN_WEIGHT_VERTICES  L"vertices"
#define RWRF3_SKIN_WEIGHT           L"w"
#define RWRF3_SKIN_WEIGHT_VERTEXID  L"vertex"
#define RWRF3_SKIN_WEIGHT_BONE      L"bone"

/**
*   \ingroup rf3write
*
*   This class is responsible for exporting RwCommSkin modifiers. In its Export() method the class
*   exports the skin's skeleton, using the ExportSkeleton() method, and the vertex weights, using
*   the ExportWeights() method. At the end of the method the root m_pNode is attached to its parent 
*   XML geometry node.
*
*   \see RwCommSkin
*/
class RF3_API RwRf3ExpSkin  : public RwRf3ExpObject
{
public:
	RwRf3ExpSkin();
	virtual ~RwRf3ExpSkin();

    virtual RwCommError     Export( RwCommObject* pObj,
                                RwCommObject* pParent,
                                RwCommScene* pScene,
                                const RwCommExportOptions* pExportOptions );

protected:

    virtual RwCommError ExportSkeleton(RwCommSkin* pSkin);
    virtual RwCommError ExportWeights(RwCommSkin* pSkin);

private:

    RwMap<RwCommTransform*, int>  m_mBones;

};


/**
*   \ingroup rf3read
*
*   This class builds an RwMISkin from an XML node. 
*
*   \see RwMISkin
*/
class RF3_API RwRf3SkinCreator : public RwRf3ObjCreator
{
public:
    RwRf3SkinCreator() {}
    virtual ~RwRf3SkinCreator() {}

	virtual RwCommObject* Create(const RwCommCreationData* pCreationData,
								const RwCommExportOptions* pExportOptions) const;

	virtual void Attach(RwCommObject* pObject, RwCommObject* pParent, RwCommScene* pScene,
                        const RwCommCreationData* pCreationData) const;

	virtual void Init(	RwCommObject* pObject,
						const RwCommCreationData* pCreationData,
						const RwCommExportOptions* pExportOptions) const;

protected:

    virtual void BuildSkeleton(RwXMLNode *pSkeleton, RwMISkin *pSkin, RwRf3Builder* pBuilder) const;
    virtual void BuildWeights(RwXMLNode *pWeightList, RwMISkin *pSkin) const;

};
#endif // !defined(AFX_RWRF3SKIN_H__9A1482DC_54ED_44CD_A5DE_0A23254082A4__INCLUDED_)
