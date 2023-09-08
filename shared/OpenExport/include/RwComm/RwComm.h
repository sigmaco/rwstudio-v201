#ifndef _RW_COMM
#define _RW_COMM
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the RWCOMM_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// RWCOMM_API functions as being imported from a DLL, wheras this DLL sees symbols

// defined with this macro as being exported.
#ifdef RWCOMM_EXPORTS
#define RWCOMM_API __declspec(dllexport)
#else
#define RWCOMM_API __declspec(dllimport)
#endif

/**
*   \ingroup rwcomm  
*
*  \page RwComm Overview
*
*   The RwComm library contains a set of abstract classes, which together form an abstract 3D scene graph
*   architecture. This scene graph encapsulates common 3D functionality, shared by most modeling applications,
*   such as 3ds max, Maya and SoftImage. The scene graph contains a neutral set of interfaces, independent of
*   a specific modeling application, but rich enough to encapsulate most of the relevant functionality.
*   The scene graph acts as a database holding all of the 3D world we are exporting. It does not contain any
*   rendering, modeling or geometrical functionality and shouldn't be confused with other 3D scene 
*   graph architectures. Its sole purpose is to encapsulate 3D data for exporting.
*   
*   The RwComm library is an abstract low level library and does not contain any specific implementation.
*   It does not know anything about RenderWare Graphics, 3ds max, Maya or any other specific modeling tool.
*   It creates a generic framework for exporting 3D data from any modeling tool to another specific 3D format.
*   The basic workflow of the export process begins by building the scene graph, which is a snapshot of the
*   3D world we are about to export. The next phase in the process involves traversing this scene graph and
*   exporting a set of 3D elements of interest. This section consists of the various scene graph
*   elements, the scene graph builder mechanism and the scene graph exporter mechanism.
*/

#define RWCOMM_MAKE_VERSION(v1,v2,v3,v4) ((v1 << 24) | (v2 << 20) | (v3 << 16) | v4)

#if (!defined(DOXYGEN))

// This class is exported from the RwComm.dll
class RWCOMM_API CRwComm
{
public:
    CRwComm(void);
    // TODO: add your methods here.
};

#endif // (!defined(DOXYGEN))

extern RWCOMM_API int nRwComm;

RWCOMM_API int fnRwComm(void);

#endif
