// RwRf3Morph.h: interface for the RwRf3Morph class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWRF3MORPH_H__A1CCFB14_D9ED_4A19_81FF_D6598CFA304D__INCLUDED_)
#define AFX_RWRF3MORPH_H__A1CCFB14_D9ED_4A19_81FF_D6598CFA304D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwRf3.h"
#include "RwRf3Object.h"

#include "RwRf3ObjCreator.h"

#define RWRF3_MORPH_TAG   L"morph"

class RF3_API RwRf3ExpMorph : public RwRf3ExpObject  
{
public:
    RwRf3ExpMorph() {}
    virtual ~RwRf3ExpMorph() {}

    virtual RwCommError     Export( RwCommObject* pObj,
                                RwCommObject* pParent,
                                RwCommScene* pScene,
                                const RwCommExportOptions* pExportOptions );

protected:

    virtual RwCommError ExportMorphTargets( RwCommGeometry* pGeometry, const RwCommExportOptions* pExportOptions );

};

/**
*   \ingroup rf3read
*
*   This class builds an RwMIMorph from an XML node. 
*
*   \see RwMISkin
*/
class RF3_API RwRf3MorphCreator : public RwRf3ObjCreator
{
public:
    RwRf3MorphCreator() {}
    virtual ~RwRf3MorphCreator() {}

	virtual RwCommObject* Create(const RwCommCreationData* pCreationData,
								const RwCommExportOptions* pExportOptions) const;

	virtual void Attach(RwCommObject* pObject, RwCommObject* pParent, RwCommScene* pScene,
                        const RwCommCreationData* pCreationData) const;

    virtual void Init(	RwCommObject* pObject,
						const RwCommCreationData* pCreationData,
						const RwCommExportOptions* pExportOptions) const;
};

#endif // !defined(AFX_RWRF3MORPH_H__A1CCFB14_D9ED_4A19_81FF_D6598CFA304D__INCLUDED_)
