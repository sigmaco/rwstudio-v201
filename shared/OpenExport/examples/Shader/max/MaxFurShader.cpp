//MaxFurShader.cpp

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "RwUtilString.h"
#include "MaxFurShader.h"
#include "MaxMaterial.h"
#include "RwMaxTexture.h"
#include "RwCommGeometry.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MaxFurShader::MaxFurShader(Mtl *pMtl) : RwMaxShader(pMtl)
{

}

MaxFurShader::~MaxFurShader()
{

}

void
MaxFurShader::GetTypeHierarchy(RwVector<RwCommType> &vTypeVec) const
{
    vTypeVec.push_back(FurShaderType);
    RwCommAnimatable::GetTypeHierarchy(vTypeVec);
}


RwCommError 
MaxFurShader::GetTexturesByType(TextureType type, RwVector<RwCommTexture *> &vTextures) const
{
    RwCommError eResult = RWCOMM_FAIL;

    if ((int)type == FurTextureType)
    {
        // Look for the fur texture. Since it's the only texture in the MaxMaterial
        // it should be the first one in the list.
        RwCommTexture *pTexture = FindTexture(0);

        if (pTexture)
        {
            vTextures.push_back(pTexture);
            eResult = RWCOMM_SUCCESS;
        }

    }

    return eResult;
}

RwCommError 
MaxFurShader::GetFurColor(RwCommVec3 &color) const
{
    RwCommVec4 col;

    // Cast to the MaxMaterial & get the color
    MaxMaterial *pMaxMaterial = (MaxMaterial*)m_pMtl;
    pMaxMaterial->GetColor(col);

    color = RwCommVec3(col[0], col[1], col[2]);
    return RWCOMM_SUCCESS;
}



/*-------------------------------------------------------------
 * Class MaxShaderCreator
 *-----------------------------------------------------------*/

MaxFurShaderCreator::MaxFurShaderCreator() : RwCommObjCreator()
{

}

MaxFurShaderCreator::~MaxFurShaderCreator()
{

}

// Creates a MaxFurShader object from an Mtl object. The Mtl object is extracted from the
//   RwMaxMaterialData object, passed to this method.
RwCommObject *
MaxFurShaderCreator::Create(const RwCommCreationData *pCreationData,
                          const RwCommExportOptions *pExportOptions) const
{
    RwCommObject *pObject = NULL;

    const RwMaxMaterialData *pData = dynamic_cast<const RwMaxMaterialData *>(pCreationData);
    if (pData)
    {
        pObject = new MaxFurShader(pData->GetMaterial());
    }
    
    return pObject;
}

// Sets the name of the shader
void 
MaxFurShaderCreator::Init(RwCommObject *pObject, const RwCommCreationData *pCreationData,
                         const RwCommExportOptions *pExportOptions) const
{
    MaxFurShader *pShader = dynamic_cast<MaxFurShader *>(pObject);
    if (pShader)
    {
        const RwMaxMaterialData *pData = dynamic_cast<const RwMaxMaterialData *>(pCreationData);
        if (pData)
        {
            CStr str = pData->GetMaterial()->GetName();
            pObject->SetName(RwUtilA2W(str.data()));
        } 
    }
}

// We do not need to attach our MaxFurShader to its RwCommGeometry, since the RwCommGeometryCreator
// class will do that for us.
void 
MaxFurShaderCreator::Attach(RwCommObject *pObject, RwCommObject *pParent, RwCommScene *pScene,
                               const RwCommCreationData *pCreationData) const
{

}
