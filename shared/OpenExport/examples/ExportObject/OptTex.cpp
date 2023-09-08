// OptTex.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

#include "RwExpBspExporter.h"
#include "RwExpDffExporter.h"

#include "RwCommTexture.h"

#include "OptTextureExporter.h"

//Must include this header for version compatability
#include "RwCommVersion.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

bool InitRwPlugin()
{
    return true;
}

void RegisterCustomExporterObjects(RwCommExporter *pExporter)
{
    // Register the new texture exporter in the Bsp exporter
    RwExpBspExporter *pBspExporter = dynamic_cast<RwExpBspExporter*>(pExporter);
    if (pBspExporter)
    {
        pBspExporter->RegisterExportObject(RwCommTextureType, OptTextureExporter);
    }

    // Register the new texture exporter in the Dff exporter
    RwExpDffExporter *pDffExporter = dynamic_cast<RwExpDffExporter*>(pExporter);
    if (pDffExporter)
    {
        pDffExporter->RegisterExportObject(RwCommTextureType, OptTextureExporter);
    }
}
