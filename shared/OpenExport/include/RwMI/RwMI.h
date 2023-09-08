#ifndef _RWMI_H
#define _RWMI_H

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the RWMI_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// RWMI_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef RWMI_EXPORTS
#define RWMI_API __declspec(dllexport)
#else
#define RWMI_API __declspec(dllimport)
#endif

// This class is exported from the RwMI.dll
class RWMI_API CRwMI
{
public:
    CRwMI(void);
    // TODO: add your methods here.
};

extern RWMI_API int nRwMI;

RWMI_API int fnRwMI(void);

#endif
