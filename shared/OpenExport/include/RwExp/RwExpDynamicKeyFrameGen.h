// RwExpDynamicKeyFrameGen.h: interface for the RwExpDynamicKeyFrameGen class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPDYNAMICKEYFRAMEGEN_H__D1DE4A1F_D64E_40BB_ACAB_C097B2ECE4C8__INCLUDED_)
#define AFX_RWEXPDYNAMICKEYFRAMEGEN_H__D1DE4A1F_D64E_40BB_ACAB_C097B2ECE4C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwExp.h"
#include "RwExpTraverseAction.h"

class RwExpAnmExporter;

const RwCommType RwExpDynamicKeyFrameGenType("RwExpDynamicKeyFrameGen");

/**
*
*  \ingroup traverseactions
*
*   This class generates intermediate animation key frames, for creating smoother animation curves.
*   This method is enabled only if the ANM_DYNAMICKEYFRAMEGENERATION export option is on.
*/

class RWEXP_API RwExpDynamicKeyFrameGen  : public RwExpTraverseAction
{
public:
    RwExpDynamicKeyFrameGen();
    virtual ~RwExpDynamicKeyFrameGen();

    //RwCommObject method
    virtual RwCommType GetType() const { return RwExpDynamicKeyFrameGenType; }

    // RwExpTraverseAction methods
    virtual bool Init(const RwCommExportOptions *pExportOptions);
    virtual RwCommError Apply(RwVector<RwExpTraverseObject> &vTraverseList,
                                const RwCommExportOptions *pExportOptions);
};

#endif // !defined(AFX_RWEXPDYNAMICKEYFRAMEGEN_H__D1DE4A1F_D64E_40BB_ACAB_C097B2ECE4C8__INCLUDED_)
