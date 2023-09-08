// MyPrtMayaPrtSys.h: interface for the MyPrtMayaPrtSys class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYPRTMAYAPRTSYS_H__A9C718EA_ED7F_4EC5_964E_50B85B1E42DF__INCLUDED_)
#define AFX_MYPRTMAYAPRTSYS_H__A9C718EA_ED7F_4EC5_964E_50B85B1E42DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CustPrtPropMaya.h"

//#include "RwMayaObjCreator.h"

#include "RwPrtMayaPrtSys.h"
#include "PlaneDeflectorComm.h"


class CUSTPRTPROP_MAYA_API MyPrtMayaPrtSys : public RwPrtMayaPrtSys, public PlaneDeflectorComm
{
public:
    MyPrtMayaPrtSys(MDagPath dagPath);
	virtual ~MyPrtMayaPrtSys();

    // the max inplementation of the common interface
    virtual RwCommError GetPlane(RwCommVec3 &normal, RwCommVec3 &position) const;
};

class CUSTPRTPROP_MAYA_API MyPrtMayaPrtSysCreator : public RwPrtMayaPrtSysCreator
{
public:
	MyPrtMayaPrtSysCreator();
	virtual ~MyPrtMayaPrtSysCreator();

    virtual RwCommObject* Create(const RwCommCreationData* pCreationData,
								const RwCommExportOptions* pExportOptions) const;
};

#endif // !defined(AFX_MYPRTMAYAPRTSYS_H__A9C718EA_ED7F_4EC5_964E_50B85B1E42DF__INCLUDED_)
