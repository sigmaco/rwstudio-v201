// MaxSimple.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "MaxSimpleExp.h"

HINSTANCE hInstance;


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    hInstance = (HINSTANCE)hModule;
    return TRUE;
}

__declspec( dllexport ) const TCHAR* LibDescription() 
{
	return "SimpleMaxExporter";
}

__declspec( dllexport ) int LibNumberClasses() 
{
	return 1;
}

static MaxSimpleExpDesc simpleExpDesc;

__declspec( dllexport ) ClassDesc* LibClassDesc(int i) 
{
	switch(i) {
	case 0: return &simpleExpDesc;
	default: return 0;
	}
}

__declspec( dllexport ) ULONG LibVersion() 
{
	return VERSION_3DSMAX;
}

