#ifndef RWMAYA_H
#define RWMAYA_H

#pragma warning(disable : 4786)
#pragma warning(disable : 4251)

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the RWMAYA_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// RWMAYA_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef RWMAYA_EXPORTS
#define RWMAYA_API __declspec(dllexport)
#else
#define RWMAYA_API __declspec(dllimport)
#endif

#endif //RWMAYA_H
