// RwMaxBuilder.cpp: implementation of the RwMaxBuilder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "RwUtilString.h"
#include "MaxSimpleBuilder.h"
#include "MaxSimpleScene.h"
#include "MaxSimpleTransform.h"
#include "MaxSimpleMesh.h"
#include "MaxSimpleShader.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MaxSimpleBuilder::MaxSimpleBuilder() : RwCommBuilder()
{
    // Register creators
    RegisterCreator(BASENODE_CLASS_ID, new MaxSimpleTransformCreator);
    RegisterCreator(SClass_ID(GEOMOBJECT_CLASS_ID), new MaxSimpleMeshCreator);
    RegisterCreator(SClass_ID(MATERIAL_CLASS_ID), new MaxSimpleShaderCreator);
}

MaxSimpleBuilder::~MaxSimpleBuilder()
{
    RwMap<SClass_ID, RwCommObjCreator* >::iterator i;
    for (i = m_mCreators.begin(); i != m_mCreators.end(); i++)
    {
        if (i->second != NULL)
        {
            i->second->Release();
        }
    }
}

/**
*   Register a RwCommObjCreator in the SClass_ID lookup table.
*
*   \param cid the SClass_ID lookup key, which will be mapped to the given RwCommObjCreator.
*   \param pCreator a pointer to the RwCommObjCreator to register.
*
*   \returns RWCOMM_SUCCESS.
*/
RwCommError 
MaxSimpleBuilder::RegisterCreator(const SClass_ID& cid, RwCommObjCreator* pCreator)
{
    RwCommObjCreator* pPrevCreator = NULL;

    // Add the RwCommObjCreator object to the lookup table
    RwMap<SClass_ID, RwCommObjCreator* >::iterator i;
    if ((i = m_mCreators.find(cid)) != m_mCreators.end())
    {
        pPrevCreator= i->second;
    }
    m_mCreators[cid] = pCreator;

    // Handle reference counting
    if (pCreator)
    {
        pCreator->AddRef();
    }

    if (pPrevCreator)
    {
        pPrevCreator->Release();
    }
    return RWCOMM_SUCCESS;
}




/**
*   This is the main method that triggers the entire build proccess of the scene graph.
*
*   \param pScene a pointer to a RwCommScene object, under which the scene graph will be built.
*
*   \returns RWCOMM_SUCCESS if the scene graph was built successfully. RWCOMM_FAIL otherwise.
*/
RwCommError 
MaxSimpleBuilder::Build(RwCommScene *&pScene)
{
    int     i;
    INode   *rootNode = GetCOREInterface()->GetRootNode();

    // Create the scene
    pScene = new MaxSimpleScene;
    pScene->SetName(RwUtilA2W(RwString(GetCOREInterface()->GetCurFileName())));

    for (i = 0; i < rootNode->NumberOfChildren(); i++)
    {
        Traverse(rootNode->GetChildNode(i), NULL, pScene);
    }
    
    return RWCOMM_SUCCESS;
}

BOOL 
MaxSimpleBuilder::Traverse(INode* pNode, RwCommObject* pParent, RwCommScene* pScene) 
{
    MaxSimpleCreationData data;
    RwCommObject* pObject = pParent;

    // Build the Transformation object
    SClass_ID cid = pNode->SuperClassID();
    data.m_pNode = pNode;
    pObject = BuildObject(cid, &data, pParent, pNode, pScene);

    // Now build the shape
    ObjectState os = pNode->EvalWorldState(0); 
    if (os.obj)
    {
        // Build Shape
        SClass_ID cid = os.obj->SuperClassID();
        RwCommObject* pShape = BuildObject(cid, &data, pObject, os.obj, pScene);

        // Now build the shape's materials
        Mtl* pMtl = pNode->GetMtl();
        if (pMtl)
        {
            TraverseMaterials(pMtl, pNode, pShape, pScene);
        }

    }
    
    // Recurse on child nodes
    for (int c = 0; c < pNode->NumberOfChildren(); c++) {
        if (!Traverse(pNode->GetChildNode(c), pObject, pScene))
            return FALSE;
    }
    

    return TRUE;
}


BOOL MaxSimpleBuilder::TraverseMaterials(Mtl* pMtl, INode* pNode, RwCommObject* pParent, RwCommScene* pScene)
{
    MaxSimpleCreationData data;
    SClass_ID cid = pMtl->SuperClassID();

    if (pMtl)
    {
        // If this is a composite material build its sub materials only.
        if (pMtl->ClassID() == Class_ID(MULTI_CLASS_ID, 0))
        {
            int nSubs = pMtl->NumSubMtls();
            for (int i = 0; i < nSubs; i++)
            {
                Mtl* pSubMtl = pMtl->GetSubMtl(i);
                data.m_pMtl = pSubMtl;
                RwCommObject* pExpMaterial = BuildObject(cid, &data, pParent,  pMtl, pScene);
            }
        }
        // Attach the main material.
        else
        {
            data.m_pMtl = pMtl;
            RwCommObject* pExpMaterial = BuildObject(cid, &data, pParent,  pMtl, pScene);
        }
    }

    return TRUE;
}

RwCommObject* 
MaxSimpleBuilder::BuildObject(  SClass_ID& cid, 
                                RwCommCreationData* pCreationData,
                                RwCommObject* pParent,
                                ReferenceTarget* pRefObj,
                                RwCommScene* pScene)
{
    RwCommObject* pObject = NULL;
    RwCommObjCreator* pCreator = NULL;

    //Check if we have a creator for this object
    RwMap<SClass_ID, RwCommObjCreator* >::iterator cit;
    if ((cit = m_mCreators.find(cid)) != m_mCreators.end())
    {
        pCreator = cit->second;
        if (pCreator)
        {
            // Create the RwCommObject
            pObject = pCreator->Create(pCreationData, m_pExportOptions);
            if (pObject) {
                // Call Init() & Attach() on the created object
                pCreator->Init(pObject, pCreationData, m_pExportOptions);
                pCreator->Attach(pObject, pParent, pScene, pCreationData);
            }
        }
    }

    // Add the created object to the scene
    if (pObject)
    {
        pScene->AddObject(pObject);
    }


    return( pObject );
}

