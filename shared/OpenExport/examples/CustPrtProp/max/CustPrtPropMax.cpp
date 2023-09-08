// CustPrtPropMax.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "CustPrtPropMax.h"

#include "RwMaxBuilder.h"

#include "MyPrtMaxPrtSys.h"

#define RENDERWAREPARTICLE_CLASS_ID	Class_ID(0x64957c11, 0x49a01df9)

#include "RwCommVersion.h"

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

bool InitRwPlugin()
{
    return true;
}

void RegisterCustomCreatorObjects(RwCommBuilder* pBuilder)
{
    RwMaxBuilder* pMaxBuilder = dynamic_cast<RwMaxBuilder*>(pBuilder);
    if (pMaxBuilder)
    {
        pMaxBuilder->RegisterCreator(RENDERWAREPARTICLE_CLASS_ID, new MyPrtMaxPrtSysCreator);
    }
}

