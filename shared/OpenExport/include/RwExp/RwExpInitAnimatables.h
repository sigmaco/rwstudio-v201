// RwExpInitAnimatables.h: interface for the RwExpInitAnimatables class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPINITANIMATABLES_H__AAACB7AC_E71A_425A_A2BE_93E8B8F42627__INCLUDED_)
#define AFX_RWEXPINITANIMATABLES_H__AAACB7AC_E71A_425A_A2BE_93E8B8F42627__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwExp.h"
#include "RwExpTraverseAction.h"

const RwCommType RwExpInitAnimatablesType("RwExpInitAnimatables");

/**
*
*  \ingroup traverseactions
*
*   This class traverses all RwCommAnimatable objects in the traverse list, and sets each one's animation
*   frame value to the user specified initial frame.
*
*   \see RwCommAnimatable
*/

class RWEXP_API RwExpInitAnimatables  : public RwExpTraverseAction 
{
public:
    RwExpInitAnimatables();
    virtual ~RwExpInitAnimatables();

    //RwCommObject method
    virtual RwCommType GetType() const { return RwExpInitAnimatablesType; }

    // Apply the action
    virtual RwCommError Apply(RwVector<RwExpTraverseObject> &vTraverseList,
                                const RwCommExportOptions *pExportOptions);

};

#endif // !defined(AFX_RWEXPINITANIMATABLES_H__AAACB7AC_E71A_425A_A2BE_93E8B8F42627__INCLUDED_)
