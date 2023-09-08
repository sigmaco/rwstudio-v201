// MyPrtMaxPrtSys.h: interface for the MyPrtMaxPrtSys class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYPRTMAXPRTSYS_H__A9C718EA_ED7F_4EC5_964E_50B85B1E42DF__INCLUDED_)
#define AFX_MYPRTMAXPRTSYS_H__A9C718EA_ED7F_4EC5_964E_50B85B1E42DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CustPrtPropMax.h"

#include "RwPrtMaxPrtSys.h"
#include "PlaneDeflectorComm.h"

class CUSTPRTPROP_MAX_API MyPrtMaxPrtSys : public RwPrtMaxPrtSys, public PlaneDeflectorComm
{
public:	
    MyPrtMaxPrtSys(Object* pObject, INode* m_pEmitterNode);
	virtual ~MyPrtMaxPrtSys();

    // the max inplementation of the common interface
    virtual RwCommError GetPlane(RwCommVec3 &normal, RwCommVec3 &position) const;
};

class CUSTPRTPROP_MAX_API MyPrtMaxPrtSysCreator : public RwPrtMaxPrtSysCreator                                   
{
public:
	MyPrtMaxPrtSysCreator();
	virtual ~MyPrtMaxPrtSysCreator();

    virtual RwCommObject* Create(const RwCommCreationData* pCreationData,
								const RwCommExportOptions* pExportOptions) const;
};


#endif // !defined(AFX_MYPRTMAXPRTSYS_H__A9C718EA_ED7F_4EC5_964E_50B85B1E42DF__INCLUDED_)
