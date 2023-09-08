// RwMaxGroup.cpp: implementation of the RwMaxGroup class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MaxSimpleTransform.h"
#include "MaxSimpleBuilder.h"

#include "RwUtilString.h"


/** Construct an RwMaxTransform object from an INode.*/
MaxSimpleTransform::MaxSimpleTransform(INode* pNode) : RwCommTransform(), m_pNode(pNode)
{

}

MaxSimpleTransform::~MaxSimpleTransform()
{

}

/** Get the object matrix using the INode::GetNodeTM() method. (Does not include the offset transformation).*/
RwCommError	    
MaxSimpleTransform::GetObjMatrix(RwCommMatrix& matrix) const
{
	Matrix3 nodeTM = m_pNode->GetNodeTM((TimeValue)(m_nFrame * GetTicksPerFrame()));

    if (m_pNode->GetParentNode() && !m_pNode->GetParentNode()->IsRootNode())   //i.e. this node isn't the top of a heirarchy
    {
        INode* parentNode = m_pNode->GetParentNode();
        Matrix3 parentNodeTM = parentNode->GetNodeTM((TimeValue)(m_nFrame * GetTicksPerFrame()));

        nodeTM = nodeTM * Inverse(parentNodeTM);
    }
 
    matrix = Mat3ToRwCommMatrix( nodeTM );

    return RWCOMM_SUCCESS;
}

/** Get the world space matrix using the INode::GetNodeTM() method. (Does not include the offset transformation).*/
RwCommError 	
MaxSimpleTransform::GetWorldMatrix(RwCommMatrix& matrix) const
{
    Matrix3 worldSpaceMatrix;

    worldSpaceMatrix = m_pNode->GetNodeTM((TimeValue)(m_nFrame * GetTicksPerFrame()));

    matrix = Mat3ToRwCommMatrix(worldSpaceMatrix);
    return RWCOMM_SUCCESS;
}

RwCommError     
MaxSimpleTransform::GetVisibility(bool& bIsVisible) const
{
    bIsVisible = true;

    return RWCOMM_SUCCESS;
}


RwCommMatrix 
MaxSimpleTransform::Mat3ToRwCommMatrix(const Matrix3& m) const
{
    RwCommMatrix M;
    for (int i = 0; i < 4; i++)
    {
        Point3 p = m.GetRow(i);
        M.SetRow(i, RwCommVec4(p.x, p.y, p.z, 0.0));
    }
    
    M[3][3] = 1.0f;
    return M;
}

// =============================================================================
// -----------------------  RwMaxTransformCreator CLASS  -----------------------
// =============================================================================

MaxSimpleTransformCreator::MaxSimpleTransformCreator()
{

}

MaxSimpleTransformCreator::~MaxSimpleTransformCreator()
{

}

/** Create an MaxSimpleTransform from an INode.*/
RwCommObject* 
MaxSimpleTransformCreator::Create(  const RwCommCreationData* pCreationData,
							    const RwCommExportOptions* pExportOptions) const
{
	RwCommObject* pObject = NULL;

	const MaxSimpleCreationData* pData = dynamic_cast<const MaxSimpleCreationData*>(pCreationData);
	if (pData)
    {
        ObjectState os = pData->m_pNode->EvalWorldState(0); 
        if (os.obj)
        {
            pObject = new MaxSimpleTransform(pData->m_pNode);
        }
	}

	return pObject;
}

/** Set the RwMaxTransform's name.*/
void MaxSimpleTransformCreator::Init(RwCommObject* pObject,
							   const RwCommCreationData* pCreationData,
							   const RwCommExportOptions* pExportOptions) const
{
	const MaxSimpleCreationData* pData = dynamic_cast<const MaxSimpleCreationData*>(pCreationData);
	if (pData)
    {
		pObject->SetName( RwUtilA2W(pData->m_pNode->GetName()) );
	}
}

/** Attach the MaxSimpleTransform to its parent MaxSimpleTransform. If this is a root transform, add it
*   to the scene as a root object, using the RwCommScene::AddRootObject() method.
*/
void MaxSimpleTransformCreator::Attach(RwCommObject* pObject, RwCommObject* pParent, RwCommScene* pScene,
                                       const RwCommCreationData* pCreationData) const
{
	RwCommTransform* pGroup = dynamic_cast<RwCommTransform*>(pParent);
	if (pGroup) 
    {
		pGroup->AddChild((RwCommTransform*)pObject);
	}
    else
    {
        pScene->AddRootObject(pObject);
    }
}
