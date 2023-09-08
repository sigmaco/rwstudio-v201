// CustPrtProp.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "CustPrtPropComm.h"

//#include "RwMaxBuilder.h"

#include "RwExpDffExporter.h"
#include "RwRf3Exporter.h"

#include "MyPrtExpRpPrtSys.h"
#include "RwPrtCommPrtSys.h"

#include "MyRf3ExpRpPrtSys.h"

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
/*CUSTPRTPROP_API int nCustPrtProp=0;

// This is an example of an exported function.
CUSTPRTPROP_API int fnCustPrtProp(void)
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

void RegisterCustomExporterObjects( RwCommExporter* pExporter )
{
    RwExpDffExporter* pDffExporter = dynamic_cast<RwExpDffExporter*>( pExporter );
    if( pDffExporter )
    {       
        pDffExporter->RegisterExportObject( RwPrtCommPrtSysType, MyPrtExpRpPrtSys );       
    }

    RwRf3Exporter* pRf3Exporter = dynamic_cast<RwRf3Exporter*>( pExporter );
    if( pRf3Exporter )
    {
        pRf3Exporter->RegisterExportObject( RwPrtCommPrtSysType, MyRf3ExpRpPrtSys ); 
    }
}

/*void RegisterCustomCreatorObjects(RwCommBuilder* pBuilder)
{
    RwMaxBuilder* pMaxBuilder = dynamic_cast<RwMaxBuilder*>(pBuilder);
    if (pMaxBuilder)
    {
        if (!pMaxBuilder->GetObjCreator(RENDERWAREPARTICLE_CLASS_ID))
        {            
            pMaxBuilder->RegisterCreator(RENDERWAREPARTICLE_CLASS_ID, new MyPrtMaxPrtSysCreator);
        }        
    }
}*/

