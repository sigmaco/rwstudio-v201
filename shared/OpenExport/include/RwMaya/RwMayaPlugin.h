#ifndef RW_MAYA_PLUGIN_H
#define RW_MAYA_PLUGIN_H

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the RWMAYAPLUGIN_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// RWMAYAPLUGIN_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef RWMAYAPLUGIN_EXPORTS
#define RWMAYAPLUGIN_API __declspec(dllexport)
#else
#define RWMAYAPLUGIN_API __declspec(dllimport)
#endif

#define PARSE_OPTIONS_FUNC      "ParseOptions"

enum exporterType
{
    rwNoneExporter = 0,
    rwAnmExporter,
    rwBspExporter,
    rwDffExporter,
    rwSplExporter
};

typedef void (*RwMayaPluginParseOptionsFunc)    (const exporterType type,
                                                const MString &optionString,
                                                RwCommExportOptions *exportOptions);

typedef MStatus (*initializePluginFunc)(MObject obj);
typedef MStatus (*uninitializePluginFunc)(MObject obj);

static void SceneChangeEventHandler(void *data);
static void DeleteAllEventHandler(void *data);

#endif // RW_MAYA_PLUGIN_H
