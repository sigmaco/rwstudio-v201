// MyRf3ExpRpPrtSys.h: interface for the MyRf3ExpRpPrtSys class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYRF3EXPRPPRTSYS_H__F4E7A332_6794_4F71_B395_27BAF5348E65__INCLUDED_)
#define AFX_MYRF3EXPRPPRTSYS_H__F4E7A332_6794_4F71_B395_27BAF5348E65__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwRf3RpPrtSys.h"

class MyRf3ExpRpPrtSys : public RwRf3ExpRpPrtSys  
{
public:
	MyRf3ExpRpPrtSys();
	virtual ~MyRf3ExpRpPrtSys();

    virtual RwCommError     Export( RwCommObject* pObj,
                                RwCommObject* pParent,
                                RwCommScene* pScene,
                                const RwCommExportOptions* pExportOptions );   
};

#endif // !defined(AFX_MYRF3EXPRPPRTSYS_H__F4E7A332_6794_4F71_B395_27BAF5348E65__INCLUDED_)
