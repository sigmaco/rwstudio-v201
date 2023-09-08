#ifndef RWMAX_H
#define RWMAX_H

#pragma warning(disable : 4786)
#pragma warning(disable : 4251)

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the RWMAX_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// RWMAX_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef RWMAX_EXPORTS
#define RWMAX_API __declspec(dllexport)
#else
#define RWMAX_API __declspec(dllimport)
#endif

// This class is exported from the RwMax.dll
class RWMAX_API CRwMax {
public:
	CRwMax(void);
	// TODO: add your methods here.
};

extern RWMAX_API int nRwMax;

RWMAX_API int fnRwMax(void);
#endif
