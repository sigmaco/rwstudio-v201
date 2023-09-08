// RwMaxNativeSkin.h: interface for the RwMaxNativeSkin class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWMAXNATIVESKIN_H__09D9B9CF_9262_4A0C_A72A_4B3C801C5883__INCLUDED_)
#define AFX_RWMAXNATIVESKIN_H__09D9B9CF_9262_4A0C_A72A_4B3C801C5883__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "RwMax.h"
#include "RwCommSkin.h"
#include "RwCommObjCreator.h"

/**
*  \ingroup maxscenegraph 
*
*  This is RwMax's native skin implementation of the RwCommSkin class. The class encapsulates an ISkin interface.
*
*   \see RwCommSkin
*/
class RWMAX_API RwMaxNativeSkin4 : public RwCommSkin 
{
public:
	RwMaxNativeSkin4(ISkin *pSkin, INode *pModifiableNode);
	virtual ~RwMaxNativeSkin4();

    // RwCommSkin methods
	virtual RwCommError GetSkinWeights(const RwCommVertexId &id, RwVector<RwCommSkinWeight> &vWeights) const;
    virtual RwCommError GetInfluenceBones(RwVector<RwCommTransform *> &vBones) const;

    //RwMaxNativeSkin4 methods
    RwCommError GetInfluenceBones(RwVector<INode *> &vBones) const;

    /** Get the ISkin interface.*/
    ISkin *GetISkin() const { return m_pSkin; }

    /** Get the ISkinContextData interface.*/
    ISkinContextData *GetISkinContextData() const { return m_pSkinData; }

protected:

	ISkin				*m_pSkin;
	ISkinContextData	*m_pSkinData;
};

/**
*  \ingroup maxbuilder 
*
*   This class is responsible for creating an RwMaxNativeSkin4 object.
*
*   \see RwMaxNativeSkin4
*/
class RWMAX_API RwMaxNativeSkin4Creator  : public RwCommObjCreator
{
public:
	RwMaxNativeSkin4Creator();
	virtual ~RwMaxNativeSkin4Creator();

	virtual RwCommObject *Create(const RwCommCreationData *pCreationData,
								 const RwCommExportOptions *pExportOptions) const;

	virtual void Init(	RwCommObject *pObject,
						const RwCommCreationData *pCreationData,
						const RwCommExportOptions *pExportOptions) const;

	virtual void Attach(RwCommObject *pObject, RwCommObject *pParent, RwCommScene *pScene,
                        const RwCommCreationData *pCreationData) const;

};

#endif // !defined(AFX_RWMAXNATIVESKIN_H__09D9B9CF_9262_4A0C_A72A_4B3C801C5883__INCLUDED_)
