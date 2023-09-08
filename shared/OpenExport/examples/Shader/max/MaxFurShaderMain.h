
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MAXFURSHADER_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MAXFURSHADER_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef MAXFURSHADER_EXPORTS
#define MAXFURSHADER_API __declspec(dllexport)
#else
#define MAXFURSHADER_API __declspec(dllimport)
#endif

// This class is exported from the MaxFurShader.dll
class MAXFURSHADER_API CMaxFurShaderMain {
public:
	CMaxFurShaderMain(void);
	// TODO: add your methods here.
};

extern MAXFURSHADER_API int nMaxFurShader;

MAXFURSHADER_API int fnMaxFurShader(void);

