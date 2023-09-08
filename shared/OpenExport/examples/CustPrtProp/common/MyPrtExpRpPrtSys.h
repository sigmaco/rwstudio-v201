// MyPrtExpRpPrtSys.h: interface for the MyPrtExpRpPrtSys class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYPRTEXPRPPRTSYS_H__8B48B3F9_C18F_4AC6_8C2C_289505F84C52__INCLUDED_)
#define AFX_MYPRTEXPRPPRTSYS_H__8B48B3F9_C18F_4AC6_8C2C_289505F84C52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CustPrtPropComm.h"

#include "RwPrtExpRpPrtSys.h"

#include "PlaneDeflectorComm.h"

#define PRIVATEPROPERTYCODE(_code)                      ((0x02 << 24) | (_code))

#define EMITTERPRTPROPPLANE                             PRIVATEPROPERTYCODE(0)

struct MySimpleDeflectorPlane
{
    RwCommVec3 position;
    RwCommVec3 normal;
};

class CUSTPRTPROP_COMM_API MyPrtExpRpPrtSys : public RwPrtExpRpPrtSys
{
public:
	MyPrtExpRpPrtSys();
	virtual ~MyPrtExpRpPrtSys();

    virtual RwCommError     Export( RwCommObject* pObj,
                                RwCommObject* pParent,
                                RwCommScene* pScene,
                                const RwCommExportOptions* pExportOptions );

    virtual RwCommError AddCustomProperties( RpPrtStdPropertyTable *propTab );

    virtual RwCommError SetCustomProperties();

private:
    PlaneDeflectorComm *m_pPlaneDeflector;
};

#endif // !defined(AFX_MYPRTEXPRPPRTSYS_H__8B48B3F9_C18F_4AC6_8C2C_289505F84C52__INCLUDED_)
