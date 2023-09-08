// RwMaxMorph.h: interface for the RwMaxMorph class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWMAXMORPH_H__848C0C85_36D9_449D_AB47_35CECCC8C53B__INCLUDED_)
#define AFX_RWMAXMORPH_H__848C0C85_36D9_449D_AB47_35CECCC8C53B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "wM3.h"

#include "RwMax.h"
#include "RwCommBlendShape.h"
#include "RwCommObjCreator.h"
#include "RwMaxTraverseControllers.h"

/**
*  \ingroup maxscenegraph 
*
*  This is the RwMax implementation of the RwCommBlendShape class. The class encapsulates a MorphR3 object.
*
*   \see RwCommBlendShape
*/
class RWMAX_API RwMaxBlendShape  : public RwCommBlendShape
{
public:
	RwMaxBlendShape(MorphR3 *pModifier);
	virtual ~RwMaxBlendShape();

	virtual RwCommError GetNumTargets(int &nTargets) const;
	virtual RwCommError SetTargetWeight(int iTarget, RwCommReal weight);
	virtual RwCommError GetTargetWeight(int iTarget, RwCommReal &weight) const;
	virtual RwCommError GetTargetName(int iTarget, RwWString & name) const;

    /** Get the MorphR3 object.*/
    MorphR3 *GetMorph() const { return m_pModifier; }

protected:

	MorphR3                         *m_pModifier;
	RwVector<morphChannel *>    m_vChannels;

};


/**
*  \ingroup maxbuilder 
*
*   This class is responsible for creating an RwMaxBlendShape object.
*   In addition, in its TraverseControllers() method it traverses and builds the morph targets animation
*   controllers.
*
*   \see RwMaxBlendShape
*   \see RwMaxTraverseControllers
*/
class RWMAX_API RwMaxBlendShapeCreator : public RwCommObjCreator, public RwMaxTraverseControllers
{
public:
	RwMaxBlendShapeCreator() : RwCommObjCreator() {}
	virtual ~RwMaxBlendShapeCreator() {}

	virtual RwCommObject *Create(const RwCommCreationData *pCreationData,
								 const RwCommExportOptions *pExportOptions) const;

	virtual void Init(	RwCommObject *pObject,
						const RwCommCreationData *pCreationData,
						const RwCommExportOptions *pExportOptions) const;

	virtual void Attach(RwCommObject *pObject, RwCommObject *pParent, RwCommScene *pScene,
                        const RwCommCreationData *pCreationData) const;

	virtual void TraverseControllers(RwMaxBuilder &rBuilder, RwCommCreationData *pCreationData, RwCommObject *pParent);
};

#endif // !defined(AFX_RWMAXMORPH_H__848C0C85_36D9_449D_AB47_35CECCC8C53B__INCLUDED_)
