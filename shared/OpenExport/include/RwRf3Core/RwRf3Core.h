
#ifndef RWRF3_CORE_H
#define RWRF3_CORE_H

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the RWRF3CORE_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// RWRF3CORE_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef RWRF3CORE_EXPORTS
#define RWRF3CORE_API __declspec(dllexport)
#else
#define RWRF3CORE_API __declspec(dllimport)
#endif

// This class is exported from the RwRf3Core.dll
class RWRF3CORE_API CRwRf3Core {
public:
	CRwRf3Core(void);
	// TODO: add your methods here.
};

extern RWRF3CORE_API int nRwRf3Core;

RWRF3CORE_API int fnRwRf3Core(void);

// RF3 generic tags
#define RWRF3_VERSION_NUM    L"1.02"
#define RWRF3_VERSION               L"version"
#define RWRF3_CREATION_DATE         L"date"
#define RWRF3_CREATION_TIME         L"time"
#define RWRF3_AUTHOR                L"author"

#define RWRF3_NODE_NAME     L"name"
#define RWRF3_NODE_REF      L"ref"
#define RWRF3_SOURCE_NAME   L"sourcename"

// RF3 asset tags
#define RWRF3_ASSET_TAG         L"asset"
#define RWRF3_ASSET_SELECTION   L"selection"
#define RWRF3_ASSET_TEMPLATE    L"template"
#define RWRF3_ASSET_GUID        L"guid"
#define RWRF3_ASSET_NAME        L"assetname"
#define RWRF3_ASSET_EXPORT_FILE L"customexportfile"
#define RWRF3_ASSET_TRANSLATOR  L"translator"
#define RWRF3_ASSET_ENABLE      L"enabled"

#define RWRF3_DEFAULT_TRANSLATOR    L"RwExpMgrExportManager"

#endif
