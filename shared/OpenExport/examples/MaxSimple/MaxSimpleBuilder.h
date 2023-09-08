// RwMaxBuilder.h: interface for the RwMaxBuilder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWMAXBUILDER_H__C4007D85_C05A_4522_815D_A38A3830EFCD__INCLUDED_)
#define AFX_RWMAXBUILDER_H__C4007D85_C05A_4522_815D_A38A3830EFCD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"

#pragma warning(disable : 4786)
#pragma warning(disable : 4251)

#include "RwCommBuilder.h"
#include "RwCommObjCreator.h"

struct MaxSimpleCreationData : public RwCommCreationData
{
	INode*  m_pNode;
    Mtl*    m_pMtl;
};

class MaxSimpleBuilder : public RwCommBuilder 
{
public:
	MaxSimpleBuilder();
	virtual ~MaxSimpleBuilder();

    // Destroy the object - called by RwCommObject::Release()
    virtual void Destroy() { delete this; }

	// Scene build function
	RwCommError         Build(RwCommScene *&pScene);

	// Creator Methods
	RwCommError RegisterCreator(const SClass_ID& cid, RwCommObjCreator* pCreator);

	BOOL Traverse(INode* node, RwCommObject* pParent, RwCommScene* pScene);
    BOOL TraverseMaterials(Mtl* pMtl, INode* node, RwCommObject* pParent, RwCommScene* pScene);

	RwCommObject* BuildObject(	SClass_ID& cid, 
								RwCommCreationData* pCreationData,
								RwCommObject* pParent,
								ReferenceTarget* pObj,
                                RwCommScene* pScene);

private:

	RwMap<SClass_ID, RwCommObjCreator* >   m_mCreators;

};

#endif // !defined(AFX_RWMAXBUILDER_H__C4007D85_C05A_4522_815D_A38A3830EFCD__INCLUDED_)
