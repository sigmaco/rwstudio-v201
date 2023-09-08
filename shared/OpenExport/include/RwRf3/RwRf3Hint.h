// RwRf3Hint.h: interface for the RwRf3Hint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWRF3HINT_H__B79D5E4A_5FDF_486D_A30E_E80EF7FEBD38__INCLUDED_)
#define AFX_RWRF3HINT_H__B79D5E4A_5FDF_486D_A30E_E80EF7FEBD38__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwRf3.h"
#include "RwRf3Object.h"

#include "RwRf3ObjCreator.h"

#include  "RwMIHint.h"

#define RWRF3_HINT_TAG              L"hint"
#define RWRF3_HINT_HINTTYPE         L"type"
#define RWRF3_HINT_SHIELD           L"shield"
#define RWRF3_HINT_PARTITION        L"partition"
#define RWRF3_HINT_DIMENSIONS       L"dimensions"


class RF3_API RwRf3ExpHint :  public RwRf3ExpObject
{
public:
	RwRf3ExpHint();
	virtual ~RwRf3ExpHint();

    virtual RwCommError     Export( RwCommObject* pObj,
                                RwCommObject* pParent,
                                RwCommScene* pScene,
                                const RwCommExportOptions* pExportOptions );

protected:

    virtual RwCommError     ExportHintType(RwCommHint *pHint);
    virtual RwCommError     ExportHintStrengths(RwCommHint *pHint);
    virtual RwCommError     ExportHintDimensions(RwCommHint *pHint);

};

// Class RwRf3HintCreator

class RF3_API RwRf3HintCreator : public RwRf3ObjCreator
{
public:
    RwRf3HintCreator() {}
    virtual ~RwRf3HintCreator() {}

	virtual RwCommObject* Create(const RwCommCreationData* pCreationData,
								const RwCommExportOptions* pExportOptions) const;

    virtual void Init(	RwCommObject* pObject,
						const RwCommCreationData* pCreationData,
						const RwCommExportOptions* pExportOptions) const;

	virtual void Attach(RwCommObject* pObject, RwCommObject* pParent, RwCommScene* pScene,
                        const RwCommCreationData* pCreationData) const;

protected:

    virtual void    BuildHintType(RwRf3Param *pParam, RwMIHint* pHint) const;
    virtual void    BuildHintStrength(RwRf3Param *pShield, RwRf3Param *pPartition, RwMIHint* pHint) const;
    virtual void    BuildHintDiemsions(RwRf3Param *pParam, RwMIHint* pHint) const;
};

#endif // !defined(AFX_RWRF3HINT_H__B79D5E4A_5FDF_486D_A30E_E80EF7FEBD38__INCLUDED_)
