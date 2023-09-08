// MyPrtMaxPrtSys.cpp: implementation of the MyPrtMaxPrtSys class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RwMaya.h"

#include "RwCommBuilder.h"
#include "RwCommObjCreator.h"

#include "RwMayaObjCreator.h"
#include "RwMayaCreationData.h"
#include "RwMayaTraverseAnimations.h"
#include "MyPrtMayaPrtSys.h"
#include "RwMayaTransform.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MyPrtMayaPrtSys::MyPrtMayaPrtSys(MDagPath dagPath)
    :RwPrtMayaPrtSys(dagPath), PlaneDeflectorComm()
{

}

MyPrtMayaPrtSys::~MyPrtMayaPrtSys()
{

}

RwCommError
MyPrtMayaPrtSys::GetPlane(RwCommVec3 &normal, RwCommVec3 &position) const
{
    RwCommError eResult = RWCOMM_FAIL;

    MStatus status;
    MStringArray    result;
    MSelectionList  selList;
    MFn::Type                       particleFilter = MFn::kParticle;
    MItDependencyGraph::Direction   direction = MItDependencyGraph::kDownstream;
    MItDependencyGraph::Traversal   traversalType = MItDependencyGraph::kBreadthFirst;
    MItDependencyGraph::Level       level = MItDependencyGraph::kNodeLevel;

    MItDependencyGraph dgParticleIter(m_dagPath.node(), particleFilter,
                                      direction, traversalType, level, &status);

    // process all particles nodes 
    for (; !dgParticleIter.isDone(); dgParticleIter.next())
    {  
        MObject particleObject = dgParticleIter.thisNode(&status);

        MGlobal::executeCommand(MString("collision -q particleShape1"), result);
        
        if(result.length() > 0 )
        {
            MSelectionList  selList;
            MDagPath collisionObjectDagPath;
            
            // removed the : from the end of the collision object name
            MStringArray splitBuffer;
            result[0].split( ':', splitBuffer);

            // get the dag path
            status = selList.add(splitBuffer[0].asChar());
            status = selList.getDagPath(0, collisionObjectDagPath);

            collisionObjectDagPath.pop(1);

            MMatrix planeMatrix = collisionObjectDagPath.inclusiveMatrix(&status);

            // use the up vector as the normal
            normal = RwCommVec3(static_cast<RwCommReal>(planeMatrix.matrix[1][0]),
                                static_cast<RwCommReal>(planeMatrix.matrix[1][1]),
                                static_cast<RwCommReal>(planeMatrix.matrix[1][2]));

            // get the translation for the position
            position = RwCommVec3(static_cast<RwCommReal>(planeMatrix.matrix[3][0]),
                                  static_cast<RwCommReal>(planeMatrix.matrix[3][1]),
                                  static_cast<RwCommReal>(planeMatrix.matrix[3][2]));

            eResult = RWCOMM_SUCCESS;
        }
    }
    
    return eResult;
}

MyPrtMayaPrtSysCreator::MyPrtMayaPrtSysCreator() : RwPrtMayaPrtSysCreator()
{

}

MyPrtMayaPrtSysCreator::~MyPrtMayaPrtSysCreator()
{

}

RwCommObject*
MyPrtMayaPrtSysCreator::Create(const RwCommCreationData* pCreationData,
						   const RwCommExportOptions* pExportOptions) const
{
	RwCommObject* pObject = NULL;

	const RwMayaCreationData* pData = dynamic_cast<const RwMayaCreationData*>(pCreationData);

    RwMayaTransform* pTransform = new RwMayaTransform(pData->GetDagPath());

	if(pData)
    {
        pObject = new MyPrtMayaPrtSys(pData->GetDagPath());

        if(pObject)
        {
		    pTransform->SetShape((RwCommShape*)pObject);
	    }
	}

	return (RwCommObject*)pTransform;
}
