#ifndef _RwRf3_H
#define _RwRf3_H
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the RwRf3_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// RwRf3_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef RF3_EXPORTS
#define RF3_API __declspec(dllexport)
#else
#define RF3_API __declspec(dllimport)
#endif

// This class is exported from the RwRf3.dll
class RF3_API CRwRf3 {
public:
	CRwRf3(void);
	// TODO: add your methods here.
};

extern RF3_API int nRf3;

RF3_API int fnRf3(void);

#endif
