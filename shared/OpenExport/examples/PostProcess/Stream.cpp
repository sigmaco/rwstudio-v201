// Stream.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "ExportManager.h"

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

void SetCustomExportManager(RwExpMgrExportManager *&pExportManager)
{
    pExportManager = new CExportManager;
}
