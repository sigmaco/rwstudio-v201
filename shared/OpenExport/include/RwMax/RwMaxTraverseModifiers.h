// RwMaxTraverseModifiers.h: interface for the RwMaxTraverseModifiers class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWMAXTRAVERSEMODIFIERS_H__40E3D187_9AD2_430C_8C54_4794778E97E8__INCLUDED_)
#define AFX_RWMAXTRAVERSEMODIFIERS_H__40E3D187_9AD2_430C_8C54_4794778E97E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma warning(disable : 4786)
#pragma warning(disable : 4251)

#include "RwMax.h"
#include "RwCommObject.h"

class RwMaxBuilder;
class RwCommCreationData;

/**
*  \ingroup maxbuilder 
*
*   Base class for all RwCommObjCreator objects, that require the RwMaxBuilder to build their assigned
*   modifiers (RwCommModifier). The RwMaxBuilder calls the TraverseModifiers() method, (at
*   the end of the RwMaxBuilder::BuildObject() method), for each RwCommObjCreator object implementing this
*   method. The default implementation traverses the Max modifier
*   stack, and attempts to build each modifier that it encounters. Users can override this method and
*   introduce their own custom modifier builder functionality.
*
*/
class RWMAX_API RwMaxTraverseModifiers  
{
public:
	RwMaxTraverseModifiers();
	virtual ~RwMaxTraverseModifiers();

	virtual void TraverseModifiers(RwMaxBuilder &rBuilder, RwCommCreationData *pCreationData, RwCommObject *pParent);

};

#endif // !defined(AFX_RWMAXTRAVERSEMODIFIERS_H__40E3D187_9AD2_430C_8C54_4794778E97E8__INCLUDED_)
