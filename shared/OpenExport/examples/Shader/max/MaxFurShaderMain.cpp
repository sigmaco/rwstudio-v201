// MaxFurShader.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "MaxFurShaderMain.h"
#include "RwMaxBuilder.h"
#include "MaxMaterial.h"
#include "MaxFurShader.h"

#include "RwCommVersion.h"

HINSTANCE hGInstance;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    static bool bInit = false;

    hGInstance = (HINSTANCE) hModule;

    if (!bInit)
    {
        bInit = true;

        // Jaguar controls (MAX's custom controls)
        InitCustomControls(hGInstance);

        // Initialize Chicago controls (WIN95 controls)
        InitCommonControls();
    }
     
    return TRUE;
}


// This is an example of an exported variable
MAXFURSHADER_API int nMaxFurShader=0;

// This is an example of an exported function.
MAXFURSHADER_API int fnMaxFurShader(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see MaxFurShader.h for the class definition
CMaxFurShaderMain::CMaxFurShaderMain()
{ 
	return; 
}

__declspec(dllexport) const TCHAR *
LibDescription() 
{
    return (_T(" Test Shader Plug-In"));
}

__declspec(dllexport) int
LibNumberClasses() 
{
    return 1;
}

__declspec(dllexport) ClassDesc *
LibClassDesc(int i) 
{
    ClassDesc* pDesc = NULL;
    pDesc = GetMaxMaterialDesc();
    return pDesc;
}

__declspec(dllexport) ULONG
LibVersion() 
{
    return VERSION_3DSMAX;
}

bool InitRwPlugin()
{
    return TRUE;
}
void RegisterCustomCreatorObjects(RwCommBuilder *pBuilder)
{
    RwMaxBuilder *pMaxBuilder = dynamic_cast<RwMaxBuilder *>(pBuilder);
    if (pMaxBuilder)
    {
        // register  shader creator for  material (MYMATERIAL_CLASS_ID is defined in MaxMaterial.h)
        pMaxBuilder->RegisterCreator(FURMATERIAL_CLASS_ID, new MaxFurShaderCreator);
    }
}



