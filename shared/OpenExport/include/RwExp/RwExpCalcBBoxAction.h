// RwExpCalcBBoxAction.h: interface for the RwExpCalcBBoxAction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPCALCBBOXACTION_H__7C749B93_09C3_4E78_8FB9_E394E9556DF5__INCLUDED_)
#define AFX_RWEXPCALCBBOXACTION_H__7C749B93_09C3_4E78_8FB9_E394E9556DF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwExp.h"
#include "RwExpTraverseAction.h"

#include "RwCommPrimitives.h"

const RwCommType RwExpCalcBBoxActionType("RwExpCalcBBoxAction");

/**
*
*  \ingroup traverseactions
*
*   This class calculates the bounding box of the traverse list. 
*/

class RWEXP_API RwExpCalcBBoxAction  : public RwExpTraverseAction 
{
public:
    RwExpCalcBBoxAction();
    virtual ~RwExpCalcBBoxAction();

    //RwCommObject method
    virtual RwCommType GetType() const { return RwExpCalcBBoxActionType; }

    // RwExpTraverseAction methods
    virtual bool Init(const RwCommExportOptions *pExportOptions);
    virtual RwCommError Apply(RwVector<RwExpTraverseObject> &vTraverseList,
                                const RwCommExportOptions *pExportOptions);

    /** Get the computed bounding box.*/
    const RwCommBoundingBox &GetBoundingBox() const { return m_bbox; }

protected:

    RwCommBoundingBox   m_bbox;

};

#endif // !defined(AFX_RWEXPCALCBBOXACTION_H__7C749B93_09C3_4E78_8FB9_E394E9556DF5__INCLUDED_)
