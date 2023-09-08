#include "stdafx.h"
#include "MayaFurShader.h"

#include "RwUtilString.h"
#include "RwCommShader.h"
#include "RwCommShape.h"
#include "RwMayaMaterialCreationData.h"
#include "RwMayaTraverseTextures.h"
#include "RwMayaUtils.h"

MayaFurShader::MayaFurShader(MObject shader, MObject set) : RwMayaMaterial(shader, set)
{

}

MayaFurShader::~MayaFurShader()
{

}

void
MayaFurShader::GetTypeHierarchy(RwVector<RwCommType> &vTypeVec) const
{
    vTypeVec.push_back(FurShaderType);
    RwCommAnimatable::GetTypeHierarchy(vTypeVec);
}


RwCommError 
MayaFurShader::GetTexturesByType(TextureType type, RwVector<RwCommTexture *> & vTextures) const
{
    RwCommError     eResult = RWCOMM_FAIL;

    // Check if this is a fur type
    if ((int)type == FurTextureType)
    {
        MObject textureObject  = RwMayaGetObjectConnectionAsObject(m_ShaderObject, L"color", 0);
        
        // If we didn't find any just use the base color texture
        if (!textureObject.isNull())
        {
            RwCommTexture *pTexture = GetShaderTexture(textureObject);

            if (pTexture != NULL)
            {
                vTextures.push_back(pTexture);
                eResult = RWCOMM_SUCCESS;
            }
        }
    }

    return eResult;
}

RwCommError 
MayaFurShader::GetFurColor(RwCommVec3 &color) const
{
    MObject colorTextureObject = RwMayaGetObjectConnectionAsObject(m_ShaderObject, L"color", 0);

    // If we have a texture then the diffuse color comes from the texture color gain.
    if (!colorTextureObject.isNull())
    {
        // If we have a layered texture the color comes from the first layer.
        if (colorTextureObject.apiType() == MFn::kLayeredTexture)
        {
            MObjectArray layerTextureObjects;
            GetLayeredTextureObjects(colorTextureObject, layerTextureObjects);
            if (layerTextureObjects.length() > 0)
            {
                colorTextureObject = layerTextureObjects[0];
            }
        }

        GetTexColorGain(colorTextureObject, color);
    }
    // Otherwise the color comes from the material
    else
    {
        RwMayaGetObjectAttributeAsFloat(m_ShaderObject, L"colorR", 0, color[0]);
        RwMayaGetObjectAttributeAsFloat(m_ShaderObject, L"colorG", 0, color[1]);
        RwMayaGetObjectAttributeAsFloat(m_ShaderObject, L"colorB", 0, color[2]);
    }
    return RWCOMM_SUCCESS;
}

MayaFurShaderCreator::MayaFurShaderCreator() : RwMayaObjCreator()
{

}

MayaFurShaderCreator::~MayaFurShaderCreator()
{

}

void
MayaFurShaderCreator::Init(RwCommObject *pObject,
                        const RwCommCreationData *pCreationData,
                        const RwCommExportOptions *pExportOptions) const
{
    const RwMayaMaterialCreationData *pData = 
        dynamic_cast<const RwMayaMaterialCreationData *>(pCreationData);

    if (pData)
    {
        MObject             mObject = pData->GetMObject();
        MFnDependencyNode   depNode(mObject);

        pObject->SetName(RwUtilA2W(depNode.name().asChar()));
    }
}

RwCommObject *
MayaFurShaderCreator::Create(const RwCommCreationData *pCreationData,
                          const RwCommExportOptions *pExportOptions) const
{
    RwCommObject *pObject = NULL;

    const RwMayaMaterialCreationData *pData = 
        dynamic_cast<const RwMayaMaterialCreationData *>(pCreationData);
    
    if (pData)
    {
        pObject = new MayaFurShader(pData->GetMObject(), pData->GetSet());
    }

    return pObject;
}

void
MayaFurShaderCreator::Attach(RwCommObject *pObject, RwCommObject *pParent,
                          RwCommScene *pScene,
                          const RwCommCreationData *pCreationData) const
{
    RwCommShape     *pShape = dynamic_cast<RwCommShape *>(pParent);
    RwCommShader    *pShader = dynamic_cast<RwCommShader *>(pObject);

    if (pShape && pShader)
    {
        pShape->AddShader(pShader);
    }
}

void
MayaFurShaderCreator::Traverse(RwCommBuilder &rBuilder,
                            RwCommCreationData *pCreationData,
                            RwCommObject *pParent) const
{
    // Traverse textures attached to this shader
    RwMayaTraverseTextures textureTraverse;
    textureTraverse.Traverse(rBuilder, pCreationData, pParent);
}
