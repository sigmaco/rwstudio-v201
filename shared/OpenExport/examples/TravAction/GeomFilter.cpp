// GeomFilter.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

#include "RwExpBspExporter.h"
#include "RwExpSelectionFilter.h"

#include "GeomFilterAction.h"

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

void RegisterCustomExporterObjects(RwCommExporter* pExporter)
{
    // Register the action with the Bsp exporter after the selction filter action
    RwExpBspExporter* pBspExporter = dynamic_cast<RwExpBspExporter*>(pExporter);
    if (pBspExporter)
    {
        GeomFilterAction* pGeomAction = new GeomFilterAction;
        // Use "_filter" for our filtering pattern
        pGeomAction->SetFilterPattern(L"_filter");

        pBspExporter->InsertAction(RwExpSelectionFilterType , pGeomAction, false);
    }
}
