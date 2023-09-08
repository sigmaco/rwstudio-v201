// RwMaxMaterial.cpp: implementation of the RwMaxMaterial class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "RwUtilString.h"
#include "RwCommGeometry.h"

#include "MaxSimpleShader.h"
#include "MaxSimpleBuilder.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/** Constructs an MaxSimpleShader from a Mtl object.*/
MaxSimpleShader::MaxSimpleShader(Mtl* pMtl) : RwCommShader(), m_pMtl(pMtl)
{

}

MaxSimpleShader::~MaxSimpleShader()
{
}


RwCommError 
MaxSimpleShader::GetAmbientValue(RwCommVec3& color) const
{
    Color c = m_pMtl->GetAmbient();
    color = ColorToRwCommVec3(c);
    return RWCOMM_SUCCESS;
}

RwCommError 
MaxSimpleShader::GetSpecularValue(RwCommVec3& color) const
{
    Color c = m_pMtl->GetSpecular();
    color = ColorToRwCommVec3(c);
    return RWCOMM_SUCCESS;
}

RwCommError 
MaxSimpleShader::GetDiffuseValue(RwCommVec3& color) const
{
    Color c = m_pMtl->GetDiffuse();
    color = ColorToRwCommVec3(c);
    return RWCOMM_SUCCESS;
}

RwCommError 
MaxSimpleShader::GetShininess(RwCommReal& v) const
{
    v = m_pMtl->GetShininess();
    return RWCOMM_SUCCESS;
}

RwCommError 
MaxSimpleShader::GetShineStrength(RwCommReal& v) const
{
    v = m_pMtl->GetShinStr();
    return RWCOMM_SUCCESS;
}

RwCommError 
MaxSimpleShader::GetTransparency(RwCommReal& v) const
{
    v = m_pMtl->GetXParency();
    return RWCOMM_SUCCESS;
}

RwCommError 
MaxSimpleShader::GetSelfIllumination(RwCommVec3& color) const
{
    Color c;
    if (m_pMtl->GetSelfIllumColorOn())
    {
       c = m_pMtl->GetSelfIllumColor();
    }
    else 
    {
        // Do mono self illumination.
        float illum = m_pMtl->GetSelfIllum();
        c = Color(illum, illum, illum);
    }
    color = ColorToRwCommVec3(c);

    return RWCOMM_SUCCESS;
}

RwCommError 
MaxSimpleShader::IsDoubleSided(bool& bStat) const
{
    if (m_pMtl->ClassID() == Class_ID(DMTL_CLASS_ID, 0))
    {
        bStat = ((StdMat *)m_pMtl)->GetTwoSided() ? true : false;
    }
    return RWCOMM_SUCCESS;
}

RwCommVec3 
MaxSimpleShader::ColorToRwCommVec3(const Color& c) const
{
    return RwCommVec3(c.r, c.g, c.b);
}

//Class RwMaxMaterialCreator

MaxSimpleShaderCreator::MaxSimpleShaderCreator() : RwCommObjCreator()
{

}

MaxSimpleShaderCreator::~MaxSimpleShaderCreator()
{

}


/** Creates a MaxSimpleShader object from an Mtl object. The Mtl object is extracted from the
*   RwMaxMaterialData object, passed to this method.
*/
RwCommObject* 
MaxSimpleShaderCreator::Create(const RwCommCreationData* pCreationData,
                          const RwCommExportOptions* pExportOptions) const
{
    RwCommObject* pObject = NULL;

    const MaxSimpleCreationData* pData = dynamic_cast<const MaxSimpleCreationData*>(pCreationData);
    if (pData)
    {
        pObject = new MaxSimpleShader(pData->m_pMtl);    
    }

    return pObject;
}

/** Sets the name of the shader.*/
void 
MaxSimpleShaderCreator::Init(RwCommObject* pObject,  const RwCommCreationData* pCreationData,
                         const RwCommExportOptions* pExportOptions) const
{
    const MaxSimpleCreationData* pData = dynamic_cast<const MaxSimpleCreationData*>(pCreationData);
    if (pData)
    {
        CStr str = pData->m_pMtl->GetName();
        pObject->SetName( RwUtilA2W(str.data()) );
    }   
}

/** Attaches the MaxSimpleShader to its RwCommShape.*/
void 
MaxSimpleShaderCreator::Attach(RwCommObject* pObject, RwCommObject* pParent, RwCommScene* pScene,
                               const RwCommCreationData* pCreationData) const
{
    RwCommShader *pShader = (RwCommShader*)pObject;
    RwCommShape *pShape = dynamic_cast<RwCommShape*>(pParent);
    if (pShape)
    {
        pShape->AddShader(pShader);
    }
}

