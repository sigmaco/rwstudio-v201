// RwExpTessellateAction.h: interface for the RwExpTessellateAction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPTESSELLATEACTION_H__843663FE_61F0_4DCB_AB06_EE5C08C6A14D__INCLUDED_)
#define AFX_RWEXPTESSELLATEACTION_H__843663FE_61F0_4DCB_AB06_EE5C08C6A14D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwExp.h"
#include "RwExpTraverseAction.h"

const RwCommType RwExpTessellateActionType("RwExpTessellateAction");

/**
*
*  \ingroup traverseactions
*
*   This class tessellates all patches in the traverse list. If the patch is successfully tessellated, then
*   it is replaced with its tessellation (in the teraverse list).
*
*/

class RWEXP_API RwExpTessellateAction  : public RwExpTraverseAction 
{
public:
    RwExpTessellateAction();
    virtual ~RwExpTessellateAction();

    //RwCommObject method
    virtual RwCommType GetType() const { return RwExpTessellateActionType; }

    virtual bool Init(const RwCommExportOptions *pExportOptions);

    // Apply the action
    virtual RwCommError Apply(RwVector<RwExpTraverseObject> &vTraverseList,
                                const RwCommExportOptions *pExportOptions);

};

#endif // !defined(AFX_RWEXPTESSELLATEACTION_H__843663FE_61F0_4DCB_AB06_EE5C08C6A14D__INCLUDED_)
