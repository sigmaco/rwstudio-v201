// CustPrtProp.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "CustPrtPropMaya.h"

#include "RwMayaBuilder.h"
#include "RwMayaObjCreator.h"

#include "MyPrtMayaPrtSys.h"
//#include "RwPrtPlugin.h"

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

// This is an example of an exported variable
/*CUSTPRTPROP_MAYA_API int nCustPrtProp=0;

// This is an example of an exported function.
CUSTPRTPROP_MAYA_API int fnCustPrtProp(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see CustPrtProp.h for the class definition
CCustPrtProp::CCustPrtProp()
{ 
	return; 
}*/

bool InitRwPlugin()
{
    return true;
}

void RegisterCustomCreatorObjects(RwCommBuilder* pBuilder)
{
    RwMayaBuilder* pMayaBuilder = dynamic_cast<RwMayaBuilder*>(pBuilder);
    if (pMayaBuilder)
    {
        pMayaBuilder->RegisterExactCreator(L"RenderWareEmitter", new MyPrtMayaPrtSysCreator());
        
    }
}

