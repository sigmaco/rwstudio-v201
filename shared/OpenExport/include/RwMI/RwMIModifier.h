// RwMIModifier.h: interface for the RwMIModifier class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWMIMODIFIER_H__1B9C0821_62CA_46B9_941B_FE6E8945B47D__INCLUDED_)
#define AFX_RWMIMODIFIER_H__1B9C0821_62CA_46B9_941B_FE6E8945B47D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwMI.h"
#include "RwCommVertex.h"

/**
*   \ingroup miscenegraph
*
*   This is the base class for all MI modfiers, that need to apply vertex modification on
*   RwMIMesh objects. The class's Evaluate() method will be called from the RwMIMesh class
*   each time it is queried for vertex information.
*   
*/
class RWMI_API RwMIModifier
{
public:
    RwMIModifier();
    virtual ~RwMIModifier();

    /**
    *   Implementing classes should implement their vertex modification code here.
    *
    *   \param frame the frame of the modifiable RwMIMesh object
    *   \param vertex the vertex to modify
    *   \param index an index of the vertex in the RwMIMesh's vertex array
    *
    **/
    virtual RwCommError Evaluate(RwCommReal frame, RwCommVertex &vertex, int index) = 0;
};

#endif // !defined(AFX_RWMIMODIFIER_H__1B9C0821_62CA_46B9_941B_FE6E8945B47D__INCLUDED_)
