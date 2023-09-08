// RwExpSampleAnimation.h: interface for the RwExpSampleAnimation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPSAMPLEANIMATION_H__D55C6416_F6CC_4504_AB53_FF352077419C__INCLUDED_)
#define AFX_RWEXPSAMPLEANIMATION_H__D55C6416_F6CC_4504_AB53_FF352077419C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwExp.h"
#include "RwExpTraverseAction.h"
#include "RwExpAnimation.h"

class RwExpAnmExporter;

const RwCommType RwExpSampleAnimationType("RwExpSampleAnimation");

/**
*
*  \ingroup traverseactions
*
*   This class traverses all RwCommTransform objects in the traverse list. For each one it extracts its
*   RwExpMatrixAnimation object. Using the RwExpMatrixAnimation's key frame values, it samples the
*   RwCommTransform object for its matrix values at those key frames. It then adds the sampled matrices 
*   to the RwExpMatrixAnimation object.
*/

class RWEXP_API RwExpSampleAnimation  : public RwExpTraverseAction 
{
public:
    RwExpSampleAnimation();
    virtual ~RwExpSampleAnimation();

    //RwCommObject method
    virtual RwCommType GetType() const { return RwExpSampleAnimationType; }

    // RwExpTraverseAction methods
    virtual bool Init(const RwCommExportOptions *pExportOptions);
    virtual RwCommError Apply(RwVector<RwExpTraverseObject> &vTraverseList, const RwCommExportOptions *pExportOptions);
};

#endif // !defined(AFX_RWEXPSAMPLEANIMATION_H__D55C6416_F6CC_4504_AB53_FF352077419C__INCLUDED_)
