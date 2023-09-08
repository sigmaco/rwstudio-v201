// RwRf3BlendShape.h: interface for the RwRf3BlendShape class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWRF3BLENDSHAPE_H__8F47D2E2_C1C2_442F_8E0D_7AE346E18C3C__INCLUDED_)
#define AFX_RWRF3BLENDSHAPE_H__8F47D2E2_C1C2_442F_8E0D_7AE346E18C3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwRf3.h"
#include "RwRf3Object.h"
#include "RwRf3ObjCreator.h"
#include "RwRf3Animatable.h"
#include "RwRf3DynKeyGen.h"

#include "RwCommBlendShape.h"

class RwMIBlendShape;

#define RWRF3_BLENDSHAPE_TAG            L"blendshape"
#define RWRF3_BLENDSHAPE_TARGET         L"target"
#define RWRF3_BLENDSHAPE_TARGET_NAME    L"name"
#define RWRF3_BLENDSHAPE_WEIGHTS        L"weights"

/**
*   \ingroup rf3write
*
*   This class is responsible for exporting RwCommBlendShape modifiers. In its Export() method the class
*   exports the blend shape's targets, using the ExportTargets() method. At the end of the method the
*   root m_pNode is attached to its parent XML geometry node.
*   
*   In addition, the class implements the RwRf3ExpAnimatable base class for supporting animated weight
*   values.
*
*   \see RwCommBlendShape
*   \see RwRf3ExpAnimatable
*/
class RwRf3ExpBlendShape  : public RwRf3ExpObject, public RwRf3ExpAnimatable
{
public:
	RwRf3ExpBlendShape();
	virtual ~RwRf3ExpBlendShape();

    virtual RwCommError     Export( RwCommObject* pObj,
                                RwCommObject* pParent,
                                RwCommScene* pScene,
                                const RwCommExportOptions* pExportOptions );

protected:

    virtual RwCommError     ExportTargets(RwCommBlendShape *pBlendShape, const RwCommExportOptions* pExportOptions);

    // RwRf3ExpAnimatable methods
    virtual RwCommError SampleKey(RwCommAnimatable* pAnimatable,
        RwCommReal frame, RwCommAnimatedChannel channel, bool bOrigKey);

    virtual RwCommError CreateAnimation(bool bGenDynamicKeys, RwCommReal tolerance);

protected:
    
    RwRf3Param              *m_pWeights;
    RwRf3VecAnimation       m_weightsAnimation;
};

/**
*   \ingroup rf3read
*
*   This class builds an RwMIBlendShape from an XML node. 
*
*   \see RwMIBlendShape
*/
class RF3_API RwRf3BlendShapeCreator : public RwRf3ObjCreator
{
public:
    RwRf3BlendShapeCreator() {}
    virtual ~RwRf3BlendShapeCreator() {}

	virtual RwCommObject* Create(const RwCommCreationData* pCreationData,
								const RwCommExportOptions* pExportOptions) const;

	virtual void Attach(RwCommObject* pObject, RwCommObject* pParent, RwCommScene* pScene,
                        const RwCommCreationData* pCreationData) const;

	virtual void Init(	RwCommObject* pObject,
						const RwCommCreationData* pCreationData,
						const RwCommExportOptions* pExportOptions) const;

protected:

    virtual void BuildTarget(int nTarget, RwXMLNode *pNode, RwMIBlendShape *pBlendShape) const;
    virtual void BuildWeights(RwRf3Param *pNode, RwMIBlendShape *pBlendShape) const;
};

#endif // !defined(AFX_RWRF3BLENDSHAPE_H__8F47D2E2_C1C2_442F_8E0D_7AE346E18C3C__INCLUDED_)
