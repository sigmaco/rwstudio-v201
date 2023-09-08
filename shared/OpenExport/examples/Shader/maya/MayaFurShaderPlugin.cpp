// MayaShader.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"


class RwExpDffExporter;
class RwRf3Builder;
class RwRf3Exporter;

#include "RwCommPluginManager.h"

#include "RwMayaBuilder.h"
#include "RwCommBuilder.h"
#include "RwCommExporter.h"

#include "MayaFurShader.h"
#include "RwCommVersion.h"

#include "RwCommMesh.h"
#include "RwMayaMaterial.h"


bool InitRwPlugin()
{
    return TRUE;
}

void RegisterCustomCreatorObjects(RwCommBuilder *pBuilder)
{
    RwMayaBuilder *pMayaBuilder = dynamic_cast<RwMayaBuilder *>(pBuilder);

    if (pMayaBuilder)
    {
        pMayaBuilder->RegisterExactCreator(L"MayaShader", new MayaFurShaderCreator);
    }
}


