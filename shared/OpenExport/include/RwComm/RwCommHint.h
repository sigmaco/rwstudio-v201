// RwExpHint.h: interface for the RwExpHint class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_RWEXPCAMERA_H__02B87964_D1B6_483E_AE08_C2BB071C7B7B__INCLUDED_)
//#define AFX_RWEXPCAMERA_H__02B87964_D1B6_483E_AE08_C2BB071C7B7B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwCommShape.h"

const RwCommType RwCommHintType("RwCommHint");

#if (!defined(DOXYGEN))

class RWCOMM_API RwCommHint : public RwCommShape
{

public:
    RwCommHint();
    virtual ~RwCommHint();

    // RwCommObject methods
    /** \return RwCommHintType */
    virtual RwCommType  GetType() const { return RwCommHintType; }
    virtual void        GetTypeHierarchy(RwVector<RwCommType> &vTypeVec) const;

    // RwCommHint methods
    virtual RwCommError GetHintType(int &type) const = 0;
    virtual RwCommError GetHintStrengths(float &shieldHintStrength, float &partitionHintStrength) const = 0;
    virtual RwCommError GetHintDimensions(float &width, float &length, float &height) const = 0;
};

#endif // (!defined(DOXYGEN))