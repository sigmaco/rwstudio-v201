// RwExpExtractKeyFrames.h: interface for the RwExpExtractKeyFrames class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPEXTRACTKEYFRAMES_H__A67AD522_525C_48B8_BBE6_3505B21E8676__INCLUDED_)
#define AFX_RWEXPEXTRACTKEYFRAMES_H__A67AD522_525C_48B8_BBE6_3505B21E8676__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwExp.h"
#include "RwExpTraverseAction.h"

class RwExpAnmExporter;
class RwCommTransform;

const RwCommType RwExpExtractKeyFramesType("RwExpExtractKeyFrames");

/**
*
*  \ingroup traverseactions
*
*   This class extracts the key frames from all RwCommAnimation objects, attached to RwCommTransform objects,
*   in the traverse list. It then adds those key frames to the appropriate RwExpAnimation objects, assigned to
*   the RwCommTransforms.
*
*   \see RwCommAnimation
*   \see RwExpAnimation
*   \see RwCommTransform
*/

class RWEXP_API RwExpExtractKeyFrames  : public RwExpTraverseAction 
{
public:
    RwExpExtractKeyFrames();
    virtual ~RwExpExtractKeyFrames();

    //RwCommObject method
    virtual RwCommType GetType() const { return RwExpExtractKeyFramesType; }

    // RwExpTraverseAction methods
    virtual bool Init(const RwCommExportOptions *pExportOptions);
    virtual RwCommError Apply(RwVector<RwExpTraverseObject> &vTraverseList, const RwCommExportOptions *pExportOptions);
};

#endif // !defined(AFX_RWEXPEXTRACTKEYFRAMES_H__A67AD522_525C_48B8_BBE6_3505B21E8676__INCLUDED_)
