// MyPrtMaxPrtSys.cpp: implementation of the MyPrtMaxPrtSys class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyPrtMaxPrtSys.h"
#include "icollision.h"
#include "simpmod.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MyPrtMaxPrtSys::MyPrtMaxPrtSys(Object* pObject, INode* m_pEmitterNode)
    :RwPrtMaxPrtSys(pObject, m_pEmitterNode), PlaneDeflectorComm()
{

}

MyPrtMaxPrtSys::~MyPrtMaxPrtSys()
{

}

RwCommError
MyPrtMaxPrtSys::GetPlane(RwCommVec3 &normal, RwCommVec3 &position) const
{
    RwCommError eResult = RWCOMM_FAIL;

    IDerivedObject *derivedObj = m_pEmitterNode->GetWSMDerivedObject();

	if (!derivedObj)
	{
		return RWCOMM_FAIL;
	}

	// get the first modifier
	Modifier *mod = derivedObj->GetModifier(0);

	if (!mod)
	{
		return RWCOMM_FAIL;
	}

	// get the inode
	INode* childNode = (INode*)mod->GetReference(SIMPWSMMOD_NODEREF);

	if (!childNode)
	{
		return RWCOMM_FAIL;
	}

	// get the class id type
	ObjectState os = childNode->EvalWorldState(0);
	Class_ID cid = os.obj->ClassID();

	// if it is a "deflector" extract the plane normal and position
	if(cid == Class_ID(938562, 0))
	{   
		Matrix3 tm = childNode->GetNodeTM(0);

		Point3 planeVector;
		Point3 planePoint;

		planeVector = tm.VectorTransform( Point3(0.0f, 1.0f, 0.0f) );
		planePoint = tm.PointTransform( Point3(0.0f, 0.0f, 0.0f) );

		normal = RwCommVec3(planeVector.x, planeVector.y, planeVector.z);
		position = RwCommVec3(planePoint.x, planePoint.y, planePoint.z);

		eResult = RWCOMM_SUCCESS;
	}	
    
    return eResult;
}

MyPrtMaxPrtSysCreator::MyPrtMaxPrtSysCreator() : RwPrtMaxPrtSysCreator()
{

}

MyPrtMaxPrtSysCreator::~MyPrtMaxPrtSysCreator()
{

}

RwCommObject*
MyPrtMaxPrtSysCreator::Create(const RwCommCreationData* pCreationData,
						   const RwCommExportOptions* pExportOptions) const
{
	RwCommObject* pObject = NULL;

	const RwMaxCreationData* pData = dynamic_cast<const RwMaxCreationData*>(pCreationData);

	if(pData)
    {
		ObjectState os = pData->GetNode()->EvalWorldState(0);

		if(os.obj)
        {
			pObject = new MyPrtMaxPrtSys(os.obj, pData->GetNode());
		}
	}

	return pObject;
}

