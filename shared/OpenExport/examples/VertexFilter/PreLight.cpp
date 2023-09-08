// PreLight.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

#include "RwExpBspExporter.h"
#include "RwExpSelectionFilter.h"

#include "RwExpBspGeometry.h"

#include "PreLightVertexFilter.h"

//Must include this header for version compatability
#include "RwCommVersion.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    // When the plugin is unloading, remove the filter from the pipeline
    if (ul_reason_for_call == DLL_PROCESS_DETACH)
    {
        RwExpBspGeometry::GetVertexPipeline().RemoveFilter(PreLightVertexFilterType);
    }
    return TRUE;
}

bool InitRwPlugin()
{
    PreLightVertexFilter* pFilter = new PreLightVertexFilter;

    // Register the pre-light vertex filter in the RwExpBspGeometry class at the end
    // of the pipeline 
    RwExpBspGeometry::GetVertexPipeline().AddFilter( pFilter );

    return true;
}


