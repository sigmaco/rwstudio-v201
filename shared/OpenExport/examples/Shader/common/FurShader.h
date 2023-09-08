
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the FURSHADER_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// FURSHADER_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef FURSHADER_EXPORTS
#define FURSHADER_API __declspec(dllexport)
#else
#define FURSHADER_API __declspec(dllimport)
#endif

// This class is exported from the FurShader.dll
class FURSHADER_API CFurShader {
public:
	CFurShader(void);
	// TODO: add your methods here.
};

extern FURSHADER_API int nFurShader;

FURSHADER_API int fnFurShader(void);

