// RwExpOptimizeShaders.h: interface for the RwExpOptimizeShaders class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPOPTIMIZESHADERS_H__97599DDB_0BA6_4CE8_A94D_A16767E73B29__INCLUDED_)
#define AFX_RWEXPOPTIMIZESHADERS_H__97599DDB_0BA6_4CE8_A94D_A16767E73B29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwExp.h"
#include "RwExpTraverseAction.h"

class RwCommGeometry;

const RwCommType RwExpOptimizeShadersType("RwExpOptimizeShaders");

/**
*
*  \ingroup traverseactions
*
*   This class removes all redundant shaders from the traverse list. Redundant shaders are shaders that are not
*   assigned to any geometry in the traverse list.
*/
class RWEXP_API RwExpOptimizeShaders  : public RwExpTraverseAction  
{
public:
    RwExpOptimizeShaders();
    virtual ~RwExpOptimizeShaders();

    // RwCommObject method
    virtual RwCommType  GetType() const { return RwExpOptimizeShadersType; }

    virtual RwCommError Apply(RwVector<RwExpTraverseObject> &vTraverseList, const RwCommExportOptions *pExportOptions);

protected:

    void GetActiveShaders(RwCommGeometry *pGeometry, RwSet<RwCommShader *> &sShaders);

};

#endif // !defined(AFX_RWEXPOPTIMIZESHADERS_H__97599DDB_0BA6_4CE8_A94D_A16767E73B29__INCLUDED_)
