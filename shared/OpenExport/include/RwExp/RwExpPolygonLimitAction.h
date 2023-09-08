// RwExpPolygonLimitAction.h: interface for the RwExpPolygonLimitAction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPPROGRESSBAR_H__2B4A7350_DB5E_4D47_B667_55F8972530CF__INCLUDED_)
#define AFX_RWEXPPROGRESSBAR_H__2B4A7350_DB5E_4D47_B667_55F8972530CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwExp.h"
#include "RwExpTraverseAction.h"

const RwCommType RwExpSetPolygonLimitActionType("RwExpSetPolygonLimitAction");
/**
*
*  \ingroup traverseactions
*
*   This class counts the number of geometry primitives in the traverse list. It feeds this information to
*   the progress bar.
*
*   \see RwCommProgressBar
*/

class RWEXP_API RwExpSetPolygonLimitAction : public RwExpTraverseAction 
{
public:
    RwExpSetPolygonLimitAction();
    virtual ~RwExpSetPolygonLimitAction();

    //RwCommObject method
    virtual RwCommType GetType() const { return RwExpSetPolygonLimitActionType; }

    virtual RwCommError Apply(RwVector<RwExpTraverseObject> &vTraverseList,
                                const RwCommExportOptions *pExportOptions);
    virtual RwCommError CleanUp();

    void    SetTessellationFlag(bool mode) { m_bAlwaysTessellate = mode; }
        
protected:
    int     m_primitiveCount;
    bool    m_bAlwaysTessellate;
};

#endif // !defined(AFX_RWEXPPROGRESSBAR_H__2B4A7350_DB5E_4D47_B667_55F8972530CF__INCLUDED_)
