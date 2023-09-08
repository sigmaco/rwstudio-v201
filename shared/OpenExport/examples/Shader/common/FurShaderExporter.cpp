// FurShaderExporter.cpp: implementation of the FurShaderExporter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FurShaderExporter.h"
#include "CommFurShader.h"

#include "RwCommGeometry.h"
#include "RwCommExporter.h"
#include "RwExpUtils.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

FurShaderExporter::FurShaderExporter() : m_pFurMaterial(NULL)
{

}

FurShaderExporter::~FurShaderExporter()
{

}


// In this example we'll extract the fur texture & color and apply it to a standard RpMaterial
RwCommError
FurShaderExporter::Export(RwCommObject *pObj,
                          RwCommObject *pParent,
                          RwCommScene *pScene,
                          const RwCommExportOptions *pExportOptions)
{
    RwCommError         eResult = RWCOMM_INVALID_OBJECT;
    
    RwSurfaceProperties rwSurfProps = {1.0f, 1.0f, 1.0f};

    CCommFurShader *pFurShader = dynamic_cast<CCommFurShader*>(pObj);
    if (pFurShader)
    {
        RwCommVec3                  furColour;
        RwVector<RwCommTexture *>   vTextures;

        // Cast to RwCommShader
        RwCommShader *pShader = dynamic_cast<RwCommShader*>(pFurShader);

        // Create the RpMaterial
        m_pFurMaterial = RpMaterialCreate();

        // Set it's surface propertis
        RpMaterialSetSurfaceProperties(m_pFurMaterial, &rwSurfProps);

        // Now get the fur color, and use it as the material's color
        if (pFurShader->GetFurColor(furColour) == RWCOMM_SUCCESS)
        {
            RwRGBA              iColour;
            RwRGBAReal          fColour = {1.0f, 1.0f, 1.0f};

            // Set the color 
            RwExpCommVec3ToRwRGBAReal(fColour, furColour);
            RwRGBAFromRwRGBAReal(&iColour, &fColour);
            RpMaterialSetColor(m_pFurMaterial, &iColour);            
        }

        if (pShader->GetTexturesByType((RwCommShader::TextureType)CCommFurShader::FurTextureType, vTextures) == RWCOMM_SUCCESS)
        {
            // Map the uvs according to the active textures, on the given geometry
            pShader->MapUVs(dynamic_cast<RwCommGeometry *>(pParent), vTextures);
        }

        
        // Export the fur texture as a standard color texture
        if (vTextures.size() > 0)
        {
            // Get the texture's export object
            GetExporter()->ExportObject(vTextures[0], pShader, pShader->GetScene());
            RwTexture* pRwTexture = GetRwTexture(vTextures[0]);
            if (pRwTexture)
            {
                RpMaterialSetTexture(m_pFurMaterial, pRwTexture);
            }
        }

        eResult = RWCOMM_SUCCESS;
    }

    return eResult;
}
