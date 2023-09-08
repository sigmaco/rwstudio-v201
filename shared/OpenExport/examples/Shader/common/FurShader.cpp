// FurShader.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

#include "RwCommVersion.h"

#include "RwCommBuilder.h"

#include "FurShader.h"
#include "CommFurShader.h"
#include "FurShaderExporter.h"
#include "Rf3FurShader.h"

#include "RwExpBspExporter.h"
#include "RwExpDffExporter.h"
#include "RwRf3Exporter.h"
#include "RwRf3Builder.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}


// This is an example of an exported variable
FURSHADER_API int nFurShader=0;

// This is an example of an exported function.
FURSHADER_API int fnFurShader(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see FurShader.h for the class definition
CFurShader::CFurShader()
{ 
	return; 
}

bool InitRwPlugin()
{
    return true;
}

void RegisterCustomExporterObjects(RwCommExporter *pExporter)
{
    // Register the fur shader exporter in the Bsp exporter
    RwExpBspExporter *pBspExporter = dynamic_cast<RwExpBspExporter*>(pExporter);
    if (pBspExporter)
    {
        pBspExporter->RegisterExportObject(FurShaderType, FurShaderExporter);
    }

    // Register the fur shader exporter in the Dff exporter
    RwExpDffExporter *pDffExporter = dynamic_cast<RwExpDffExporter*>(pExporter);
    if (pDffExporter)
    {
        pDffExporter->RegisterExportObject(FurShaderType, FurShaderExporter);
    }

    // Register the rf3 fur shader exporter in the Rf3 exporter
    RwRf3Exporter *pRf3Exporter = dynamic_cast<RwRf3Exporter*>(pExporter);
    if (pRf3Exporter)
    {
        pRf3Exporter->RegisterExportObject(FurShaderType, Rf3ExpFurShader);
    }

}

void RegisterCustomCreatorObjects(RwCommBuilder *pBuilder)
{
    RwRf3Builder *pRf3Builder = dynamic_cast<RwRf3Builder *>(pBuilder);

    // Register the rf3 fur shader creator in the Rf3 builder
    if (pRf3Builder)
    {
        pRf3Builder->RegisterCreator(L"fur", new RwRf3FurShaderCreator);
    }
}

