// RwRf3Transform.h: interface for the RwRf3Transform class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RwRf3TRANSFORM_H__E4C3EDCC_3F71_4898_9D27_89F8E7953E33__INCLUDED_)
#define AFX_RwRf3TRANSFORM_H__E4C3EDCC_3F71_4898_9D27_89F8E7953E33__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwRf3.h"
#include "RwRf3Object.h"

#include "RwRf3ObjCreator.h"
#include "RwRf3Animatable.h"
#include "RwRf3DynKeyGen.h"


class RwMITransform;

// Transform 
#define RWRF3_TRANSFORM_TAG           L"group"
#define RWRF3_TRANSFORM_LOCXFORM      L"localxform"
#define RWRF3_TRANSFORM_WORLDXFORM    L"worldxform"
#define RWRF3_TRANSFORM_SPACE         L"space"
#define RWRF3_TRANSFORM_OBJSPACE      L"object"
#define RWRF3_TRANSFORM_SHAPE         L"shape"
#define RWRF3_TRANSFORM_USERTAG       L"tag"
#define RWRF3_TRANSFORM_USERTAG_AUTO_GEN  L"autogen"
#define RWRF3_TRANSFORM_SPIVOT_IN     L"spivotin"
#define RWRF3_TRANSFORM_SPIVOT_OUT    L"spivotout"

/**
*   \ingroup rf3write
*
*   This class is responsible for exporting RwCommTransform objects. In its Export() method the class
*   exports the transform's matrix information (using ExportObjMatrix()), tagging information (using
*   ExportTag()), and scale pivot information, (using the ExportPivots() method).
*   
*   In addition, the class implements the RwRf3ExpAnimatable base class for animating matrix values.
*
*   \see RwCommTransform
*   \see RwRf3ExpAnimatable
*/
class RF3_API RwRf3ExpTransform  : public RwRf3ExpObject, public RwRf3ExpAnimatable
{
public:
	RwRf3ExpTransform();
	virtual ~RwRf3ExpTransform();

    virtual RwCommError     Export( RwCommObject* pObject,
                                RwCommObject* pParent,
                                RwCommScene* pScene,
                                const RwCommExportOptions* pExportOptions );

protected:

    virtual RwCommError ExportObjMatrix(RwCommTransform *pTransform);
    virtual RwCommError ExportTag(RwCommTransform *pTransform);
    virtual RwCommError ExportPivots(RwCommTransform *pTransform);


    // RwRf3ExpAnimatable methods
    virtual RwCommError SampleKey(RwCommAnimatable* pAnimatable,
        RwCommReal frame, RwCommAnimatedChannel channel, bool bOrigKey);

    virtual RwCommError CreateAnimation(bool bGenDynamicKeys, RwCommReal tolerance);

protected:

    RwWString  MakeAnimType(RwCommTransform *pTransform);

protected:
    
    RwRf3Param          *m_pXformNode;
    RwRf3MatAnimation   m_xformAnimation;
    RwCommMatrix        m_axisSystem;
    RwWString        m_sAnimType;
};

/**
*   \ingroup rf3read
*
*   This class builds an RwMITransform from an XML node. 
*
*   \see RwMITransform
*/
class RF3_API RwRf3TransformCreator : public RwRf3ObjCreator
{
public:
    RwRf3TransformCreator() {}
    virtual ~RwRf3TransformCreator() {}

	virtual RwCommObject* Create(const RwCommCreationData* pCreationData,
								const RwCommExportOptions* pExportOptions) const;

    virtual void Init(RwCommObject* pObject, const RwCommCreationData* pCreationData,
                            const RwCommExportOptions* pExportOptions) const;

	virtual void Attach(RwCommObject* pObject, RwCommObject* pParent, RwCommScene* pScene,
                        const RwCommCreationData* pCreationData) const;

protected:

    virtual void BuildObjMatrix(RwRf3Param *pParam, RwMITransform *pTransform) const;
    virtual void BuildTag(RwRf3Param *pParam, RwMITransform *pTransform) const;
    virtual void BuildScalePivots(RwRf3Param *pScaleIn, RwRf3Param *pScaleOut, RwMITransform *pTransform) const;


};

#endif // !defined(AFX_RwRf3TRANSFORM_H__E4C3EDCC_3F71_4898_9D27_89F8E7953E33__INCLUDED_)
