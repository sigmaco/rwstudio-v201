// RwMIAnimation.h: interface for the RwMIAnimation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWMIANIMATION_H__C85506C7_3BCC_4E55_AB61_AF0E750DA738__INCLUDED_)
#define AFX_RWMIANIMATION_H__C85506C7_3BCC_4E55_AB61_AF0E750DA738__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwMI.h"
#include "RwCommAnimation.h"

/**
*   \ingroup miscenegraph
*
*   This is the MI implementation of RwCommAnimation.
*
*   \see RwCommAnimation
*/
class RWMI_API RwMIAnimation : public RwCommAnimation
{
public:
    RwMIAnimation();
    virtual ~RwMIAnimation();

    virtual RwCommError GetKeyTimes(RwCommReal iStartFrame, RwCommReal iEndFrame,
                                    RwVector<RwCommReal> &sKeyFrames) const;

    RwCommError SetKeyTimes(RwVector<RwCommReal> &vKeyFrames);
    RwCommError AddKeyTime(RwCommReal keyTime);

protected:
    RwVector<RwCommReal> m_vKeyFrames;
};

#endif // !defined(AFX_RWMIANIMATION_H__C85506C7_3BCC_4E55_AB61_AF0E750DA738__INCLUDED_)
