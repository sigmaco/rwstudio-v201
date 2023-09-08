// RwExpHint.h: interface for the RwExpHint class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_RWEXPCAMERA_H__02B87964_D1B6_483E_AE08_C2BB071C7B7B__INCLUDED_)
//#define AFX_RWEXPCAMERA_H__02B87964_D1B6_483E_AE08_C2BB071C7B7B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwCommShape.h"

const RwCommType RwCommPartitionType("RwCommPartition");

#if (!defined(DOXYGEN))

class RWCOMM_API RwCommPartition : public RwCommShape
{
public:
    RwCommPartition();
    virtual ~RwCommPartition();

    // RwCommObject methods
    virtual RwCommType  GetType() const { return RwCommPartitionType; }
    virtual void        GetTypeHierarchy(RwVector<RwCommType> &vTypeVec) const;

    // RwCommShape methods
    virtual RwCommError GetVisibility(bool &bIsVisible) const = 0;
    
    //  RwCommPartition methods
    virtual RwCommError SetXMLString(const RwString &s) = 0;
    virtual RwCommError GetXMLString(RwString &s) = 0;

    virtual RwCommError Redraw() = 0;
};

#endif // (!defined(DOXYGEN))