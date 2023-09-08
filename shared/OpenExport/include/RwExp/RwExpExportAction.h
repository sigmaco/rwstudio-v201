// RwExpExportAction.h: interface for the RwExpExportAction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPEXPORTACTION_H__783FA3FC_ECE9_4915_BCE2_8711F1006FD2__INCLUDED_)
#define AFX_RWEXPEXPORTACTION_H__783FA3FC_ECE9_4915_BCE2_8711F1006FD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwExpTraverseAction.h"

const RwCommType RwExpExportActionType("RwExpExportAction");

/**
*
*  \ingroup traverseactions
*
*   This is the main export traverse action. The action traverses all RwCommObjects in the traverse list
*   and calls RwCommExporter::ExportObject() for each one.
*/
class RWEXP_API RwExpExportAction  : public RwExpTraverseAction
{
public:
    RwExpExportAction();
    virtual ~RwExpExportAction();

    //RwCommObject method
    virtual RwCommType	GetType() const { return RwExpExportActionType; }

    // Apply the action
    virtual RwCommError Apply(RwVector<RwExpTraverseObject> &vTraverseList,
                                const RwCommExportOptions *pExportOptions);

};

#endif // !defined(AFX_RWEXPEXPORTACTION_H__783FA3FC_ECE9_4915_BCE2_8711F1006FD2__INCLUDED_)
