// RwExpSceneTraverse.h: interface for the RwExpSceneTraverse class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPSCENETRAVERSE_H__4B7107BF_F25F_4113_8C9E_4456A6617D27__INCLUDED_)
#define AFX_RWEXPSCENETRAVERSE_H__4B7107BF_F25F_4113_8C9E_4456A6617D27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwExp.h"
#include "RwExpTraverseAction.h"

#include "RwCommTransform.h"

class RWEXP_API RwExpGraphicsSceneTraverse  
{
public:
	RwExpGraphicsSceneTraverse();
	virtual ~RwExpGraphicsSceneTraverse();

    // Traverse methods
    virtual RwCommError TraverseTransforms(RwCommTransform *pTransform, RwCommTransform *pParent, RwVector<RwExpTraverseObject>& vTraverseList, const RwCommExportOptions *pOptions);
    virtual RwCommError SortTransforms(const RwCommTransform *pParent, RwVector<RwCommTransform *> &vChildren, const RwCommExportOptions *pOptions);
    virtual RwCommError TraverseAnimations(RwCommAnimatable *pAnimatable, RwVector<RwExpTraverseObject> &vTraverseList, const RwCommExportOptions *pOptions);
    virtual RwCommError TraverseModifiers(RwCommModifiable *pModifiable, RwVector<RwExpTraverseObject> &vTraverseList, const RwCommExportOptions *pOptions);
    virtual RwCommError TraverseShape(RwCommShape *pShape, RwVector<RwExpTraverseObject>  &vTraverseList, const RwCommExportOptions *pOptions);
    virtual RwCommError TraverseShaders(RwCommShape *pShape, RwVector<RwExpTraverseObject>  &vTraverseList, const RwCommExportOptions *pOptions);
    virtual RwCommError TraverseExternalSkinnedGeometries(RwCommTransform *pRoot, RwVector<RwExpTraverseObject> &vTraverseList, const RwCommExportOptions *pOptions);
    virtual RwCommError RemoveExternallySkinnedGeometries(RwCommTransform *pRoot, RwVector<RwExpTraverseObject> &vTraverseList, const RwCommExportOptions *pOptions);
};

#endif // !defined(AFX_RWEXPSCENETRAVERSE_H__4B7107BF_F25F_4113_8C9E_4456A6617D27__INCLUDED_)
