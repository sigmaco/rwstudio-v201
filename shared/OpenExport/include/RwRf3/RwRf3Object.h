// RwRf3Object.h: interface for the RwRf3Object class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(RF3EXPOBJECT_H)
#define RwRf3EXPOBJECT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwRf3.h"

#include "RwXMLDOM.h"

#include "RwCommObject.h"
#include "RwCommExportObject.h"

#define RWRF3_OBJECT_TAG    L"object"

/**
*   \ingroup rf3write
*
*   This class is the base class for all RwRf3 exporter classes. In its Export() method the class
*   creates the local XML root node for the exported rf3 object and stores it in m_pNode. In addition,
*   it generates the rf3 object's unique name and sets it as an XML attribute in the root m_pNode.
*/
class RF3_API RwRf3ExpObject  : public RwCommExportObject
{
public:
	RwRf3ExpObject();
	virtual ~RwRf3ExpObject();

    virtual RwCommError     Export( RwCommObject* pObject,
                                RwCommObject* pParent,
                                RwCommScene* pScene,
                                const RwCommExportOptions* pExportOptions );

    virtual RwCommError     AddUserData(RwCommUserDataCollection &userDataColl);

    RwXMLNode*   GetNode() { return m_pNode; }

protected:

    virtual RwCommError ExportAttribues(RwCommObject* pObject);

protected:

    RwXMLNode*   m_pNode;
};

#endif // !defined(AFX_RwRf3OBJECT_H__1622B036_5D28_48D8_ACDC_D1F37F433DD2__INCLUDED_)
