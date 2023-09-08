#ifndef RWEXP_H
#define RWEXP_H

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the RWEXP_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// RWEXP_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef RWEXP_EXPORTS
#define RWEXP_API __declspec(dllexport)
#else
#define RWEXP_API __declspec(dllimport)
#endif

// This class is exported from the RwExp.dll
class RWEXP_API CRwExp {
public:
	CRwExp(void);
};

extern RWEXP_API int nRwExp;

RWEXP_API int fnRwExp(void);
#endif

