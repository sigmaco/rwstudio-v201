// RwExpSelectionFilter.h: interface for the RwExpSelectionFilter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPSELECTIONFILTER_H__FA6CB563_76FF_4E8B_94AB_AE1D32CC8E8B__INCLUDED_)
#define AFX_RWEXPSELECTIONFILTER_H__FA6CB563_76FF_4E8B_94AB_AE1D32CC8E8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwExp.h"
#include "RwExpTraverseAction.h"

class RwCommTransform;

const RwCommType RwExpSelectionFilterType("RwExpSelectionFilter");

/**
*
*  \ingroup traverseactions
*
*   This class removes all unselected objects from the traverse list. It is used by the BSP exporter to export
*   the selected objects only.
*
*   \see RwExpBspExporter
*/

class RWEXP_API RwExpSelectionFilter : public RwExpTraverseAction 
{
public:
    RwExpSelectionFilter();
    virtual ~RwExpSelectionFilter();

    //RwCommObject method
    virtual RwCommType GetType() const { return RwExpSelectionFilterType; }

    // RwExpTraverseAction methods
    virtual bool Init(const RwCommExportOptions *pExportOptions);
    virtual RwCommError Apply(RwVector<RwExpTraverseObject> &vTraverseList, const RwCommExportOptions *pExportOptions);

    /** Sets the selection.
    *
    *   \param vSelection a vector of selected RwCommTransform objects.
    */
    void SetSelection(RwVector<RwCommTransform *> &vSelection);

protected:

    RwVector<RwCommTransform *>   m_vSelection;
};

#endif // !defined(AFX_RWEXPSELECTIONFILTER_H__FA6CB563_76FF_4E8B_94AB_AE1D32CC8E8B__INCLUDED_)
