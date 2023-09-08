
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the RWEXPMGR_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// RWEXPMGR_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef RWEXPMGR_EXPORTS
#define RWEXPMGR_API __declspec(dllexport)
#else
#define RWEXPMGR_API __declspec(dllimport)
#endif

/**
*   \ingroup rwexpmgr   
*
*  \page RwExpMgr Overview
*
*   The RwExpMgr library is responsible for managing the entire export process, building
*   the scene graph, exporting, managing options in template files, launching the viewer,
*   etc.
*
*   \see RwCommBuilder 
*   \see RwExpExporter
*/
