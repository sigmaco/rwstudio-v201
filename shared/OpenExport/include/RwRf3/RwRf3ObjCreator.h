// RwRf3ObjCreator.h: interface for the RwRf3ObjCreator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RwRf3OBJCREATOR_H__E9B0908B_EACA_4A44_A229_5A3AC8B30D89__INCLUDED_)
#define AFX_RwRf3OBJCREATOR_H__E9B0908B_EACA_4A44_A229_5A3AC8B30D89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwRf3.h"
#include "RwCommObjCreator.h"
#include "RwCommUserData.h"

class RwXMLNode;
class RwRf3Builder;
class RwRf3Param;
class RwMIAttributes;

/**
*   \ingroup rf3read
*
*   This class is the base class for all RwRf3 object creator classes. The class contains two methods
*   for building user attributes and building user data information.
*/
class RF3_API RwRf3ObjCreator : public RwCommObjCreator  
{
public:
	RwRf3ObjCreator();
	virtual ~RwRf3ObjCreator();

    void BuildUserAttribute(RwRf3Param *pParam, RwMIAttributes* pAttributes) const;

    virtual RwCommError BuildUserData( RwXMLNode* pNode, RwCommUserDataCollection& userDataColl,
        RwCommUserDataCollection::Type userDataType , RwCommObject *pObject) const;


};

#endif // !defined(AFX_RwRf3OBJCREATOR_H__E9B0908B_EACA_4A44_A229_5A3AC8B30D89__INCLUDED_)
