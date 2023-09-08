// RwExpCheckNodesAction.h: interface for the RwExpCheckNodesAction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(RW_EXP_CHECK_NODES_ACTION_H)
#define RW_EXP_CHECK_NODES_ACTION_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwExp.h"
#include "RwExpExporter.h"
#include "RwExpTraverseAction.h"

const RwCommType RwExpCheckNodesActionType("RwExpCheckNodesAction");
/**
*
*  \ingroup traverseactions
*
*   This class performs different tests on the traverse list, for each target platform, (using the
*   RwExpNodeChecker mechanism), and displays warnings and errors accordingly.
*
*   \see RwExpNodeChecker
*   \see RwExpNodeCheckerFactory    
*/

class RWEXP_API RwExpCheckNodesAction : public RwExpTraverseAction  
{
public:
    RwExpCheckNodesAction();
    virtual ~RwExpCheckNodesAction();
    
    // RwCommObject method
    virtual RwCommType  GetType() const { return RwExpCheckNodesActionType; }

    // RwExpTraverseAction methods
    virtual bool        Init(const RwCommExportOptions *pExportOptions);
    virtual RwCommError Apply(RwVector<RwExpTraverseObject> &vTraverseList, const RwCommExportOptions *pExportOptions);

};

#endif // !defined(RW_EXP_CHECK_NODES_ACTION_H)
