// PreLightVertexFilter.cpp: implementation of the PreLightVertexFilter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PreLightVertexFilter.h"

#include "RwCommGeometry.h"
#include "RwCommShader.h"
#include "RwCommMessage.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PreLightVertexFilter::PreLightVertexFilter() : m_lightDirection(0.0, 0.0, 1.0),
m_ambientIntensity(0.2f), m_diffuseIntensity(0.8f)
{

}

PreLightVertexFilter::~PreLightVertexFilter()
{

}

void
PreLightVertexFilter::Destroy()
{
    delete this;
}

// Initialize the filter. Called at the beginning of the pipeline.
bool 
PreLightVertexFilter::Init(RwCommGeometry *pGeometry, const RwCommExportOptions *pExportOptions)
{
    // Apply the filter only if the pre-light option is checked
    return pExportOptions->GetOption(COLORVERTEXPRELIGHT).GetBool();
}


// Apply the filter 
void 
PreLightVertexFilter::Do(RwVector<RwCommVertex*>& vertices, RwVector<RwCommPrimitive>& primitives,
                         RwCommGeometry* pGeometry, const RwCommExportOptions* pExportOptions)
{

    PRINT_MESSAGE(L"---------------------------------------------------------", L"", 2);
    PRINT_MESSAGE(L"Applying PreLightVertexFilter!!!", pGeometry->GetName(), 2);
    PRINT_MESSAGE(L"---------------------------------------------------------", L"", 2);

    for (unsigned int i = 0; i < vertices.size(); i++)
    {
        RwCommVec3 normal;
        RwCommVec3 ambient(1.0, 1.0, 1.0), diffuse(1.0 ,1.0 ,1.0);

        RwCommVertex &vertex = *vertices[i];

        RwCommShader *pShader = NULL;

        // Extract the vertex's shader
        if (vertex.GetShader(pShader) == RWCOMM_SUCCESS)
        {
            // Extract material diffuse and ambient values
            pShader->GetAmbientValue(ambient);
            pShader->GetDiffuseValue(diffuse);
        }
        else
        {
            // No shader exists, then use the geometry's default color
            pGeometry->GetDefaultColor(diffuse);
        }
            
        // Get the vertex normal
        if (vertex.GetNormal(normal) == RWCOMM_SUCCESS)
        {
            RwCommVec3 color;

            RwCommReal dotProd = -normal[0]*m_lightDirection[0] +
                                    -normal[1]*m_lightDirection[1] + 
                                    -normal[2]*m_lightDirection[2];

            // Apply some very basic lighting equation
            color[0] = m_ambientIntensity*ambient[0] + m_diffuseIntensity*diffuse[0]*dotProd;
            color[1] = m_ambientIntensity*ambient[1] + m_diffuseIntensity*diffuse[1]*dotProd;
            color[2] = m_ambientIntensity*ambient[2] + m_diffuseIntensity*diffuse[2]*dotProd;

            if (color[0] < 0.0)
            {
                color[0] = 0.0;
            }
            if (color[1] < 0.0)
            {
                color[1] = 0.0;
            }
            if (color[2] < 0.0)
            {
                color[2] = 0.0;
            }

            // Set the vertex color
            vertex.SetColor(color);
        }
        
     }    
}
