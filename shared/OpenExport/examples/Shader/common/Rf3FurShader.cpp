// Rf3FurShader.cpp: implementation of the Rf3FurShader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Rf3FurShader.h"
#include "RwRf3Texture.h"
#include "RwRf3Exporter.h"
#include "RwRf3Builder.h"

#include "CommFurShader.h"
#include "RwCommGeometry.h"

#include "MIFurShader.h"
#include "RwRf3Core.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Rf3ExpFurShader::Rf3ExpFurShader()
{

}

Rf3ExpFurShader::~Rf3ExpFurShader()
{

}

RwCommError
Rf3ExpFurShader::Export(RwCommObject *pObj,
                        RwCommObject *pParent,
                        RwCommScene *pScene,
                        const RwCommExportOptions *pExportOptions)
{
    RwCommError         eResult = RWCOMM_INVALID_OBJECT;

    CCommFurShader *pFurShader = dynamic_cast<CCommFurShader*>(pObj);
    if (pFurShader)
    {
        // Call base class implementation
        RwRf3ExpShader::Export(pObj, pParent, pScene, pExportOptions);

        if (m_pShaderFunc)
        {
            RwVector<RwCommTexture *>   vTextures;
            RwCommVec3                  furColor;

            m_pShaderFunc->SetAttribute(L"type", L"fur");

            // Now get the fur color, and add it as a param
            if (pFurShader->GetFurColor(furColor) == RWCOMM_SUCCESS)
            {
                RwRf3Param *pParam = new RwRf3Param(L"furColor", m_pShaderFunc);
                pParam->SetData(furColor);
            }

            // Cast to RwCommShader
            RwCommShader *pShader = dynamic_cast<RwCommShader*>(pFurShader);

            if (pShader->GetTexturesByType((RwCommShader::TextureType)CCommFurShader::FurTextureType, vTextures) == RWCOMM_SUCCESS)
            {
                // Map the uvs according to the active textures, on the given geometry
                pShader->MapUVs(dynamic_cast<RwCommGeometry *>(pParent), vTextures);
            }

        
            // Export the fur texture 
            if (vTextures.size() > 0)
            {
                int uvSet = 0;
                pShader->GetTextureUVSet(vTextures[0], uvSet);

                // Get the rf3 exporter
                RwRf3Exporter *pExporter = dynamic_cast<RwRf3Exporter*>(GetExporter());

                // Create the channel node
                RwXMLNode *pChannel = new RwXMLNode(m_pShaderFunc);
                pChannel->SetName(RWRF3_SHADER_CHANNEL);

                // Set the channel uv set
                pChannel->SetAttribute(RWRF3_SHADER_CHANNELUVSET, uvSet);

                // Set the channel name
                pChannel->SetAttribute(RWRF3_SHADER_CHANNELNAME, L"fur");

                // Now attach the texture ref to channel
                RwXMLNode *pTexNode = new RwXMLNode(pChannel);
                pTexNode->SetName(RWRF3_TEXTURE_TAG);
                RwWString name = pExporter->GetObjectName(vTextures[0]);
                pTexNode->SetAttribute(RWRF3_NODE_REF, name);

            }
        }
    }

    return eResult;
}


// Class RwRf3FurShaderCreator
RwCommObject* 
RwRf3FurShaderCreator::Create(const RwCommCreationData* pCreationData,
                           const RwCommExportOptions* pExportOptions) const
{
    return new MIFurShader;
}


// We override the BuildShaderFunc method to get the fur color param
void 
RwRf3FurShaderCreator::BuildShaderFunc(RwXMLNode *pNode, RwCommShader *pShader, RwRf3Builder* pBuilder) const
{
    // Call base class implementation
    RwRf3ShaderCreator::BuildShaderFunc(pNode, pShader, pBuilder);

    // Now look for the fur color
    MIFurShader *pMIFurShader = dynamic_cast<MIFurShader*>(pShader);
    if (pMIFurShader)
    {
        RwRf3Param *pParam = NULL;
        RwXMLNode *pChild = NULL;

        // Loop thru all children and find the fur color param
        for (pChild = pNode->GetFirstChild() ; pChild != NULL ; pChild = pChild->GetNextNode())
        {
            if ((pParam = dynamic_cast<RwRf3Param*>(pChild)) != NULL)
            {
                RwWString name;
                if (pParam->GetParamName(name) == RWCOMM_SUCCESS)
                {
                    if (name == L"furColor")
                    {
                        RwCommVec3 color;
                        pParam->GetData(color);

                        // Set the fur color on the MIFurShader
                        pMIFurShader->SetFurColor(color);
                    }
                }
            }
        }

    }
}

// We override the BuildChannel() method to build our own custom fur channel 
void 
RwRf3FurShaderCreator::BuildChannel(RwXMLNode *pNode, RwCommShader *pShader, RwRf3Builder* pBuilder) const
{
    RwWString channelName;

    if (pNode->GetAttribute(RWRF3_SHADER_CHANNELNAME, channelName))
    {
        if (channelName == L"fur")
        {
            RwXMLNode *pChild = NULL;
            int uvSet = 0;

            // Get uv set
            pNode->GetAttribute(RWRF3_SHADER_CHANNELUVSET, uvSet);

            // Loop thru all textures in the channel (even though we should only have one)
            for (pChild = pNode->GetFirstChild() ; pChild != NULL ; pChild = pChild->GetNextNode())
            {
                RwWString texName;
                if (pChild->GetName() == RWRF3_TEXTURE_TAG && pChild->GetAttribute(RWRF3_NODE_REF, texName))
                {
                    // Build the texture
                    RwCommTexture *pTexture = 
                            dynamic_cast<RwCommTexture*>(pBuilder->BuildObject(texName, pShader->GetScene()));

                    if (pTexture)
                    {
                        MIFurShader *pMIFurShader = dynamic_cast<MIFurShader*>(pShader);

                        if (pMIFurShader)
                        {
                            RwCommShader::RwCommBlendFunction bf = RwCommShader::BLENDNABLEND;

                            // Set texture attributes
                            pMIFurShader->SetTextureUVSet(pTexture, uvSet);
                            pMIFurShader->SetTextureInfo(pTexture, (RwCommShader::TextureType) CCommFurShader::FurTextureType, bf);
                        }
                    }
                }
            }
        }
        else
        {
            // Call base class implementation
            RwRf3ShaderCreator::BuildChannel(pNode, pShader, pBuilder);
        }
    }
}


