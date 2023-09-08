// RwCommPluginManager.h: interface for the RwCommPluginManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWCOMMPLUGINMANAGER_H__ABB64C10_D29E_4FEE_99D7_8784FEDE1C12__INCLUDED_)
#define AFX_RWCOMMPLUGINMANAGER_H__ABB64C10_D29E_4FEE_99D7_8784FEDE1C12__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable : 4251)

#include "RwComm.h"
#include "RwCommError.h"
#include "RwCommPrimitives.h"

#if (!defined(DOXYGEN))

class RWCOMM_API RwCommPluginManager  
{
public:

    struct ModuleInfo
    {
        RwWString Name;
        RwWString Version;
        RwWString FullPath;
    };

    virtual ~RwCommPluginManager();

    static RwCommPluginManager  &TheInstance();

    const RwVector<HMODULE>       &GetAllModules() const { return m_vModules; }
    const RwVector<ModuleInfo>    &GetAllUnloadedModules() const { return m_vUnloadedModules; }

    RwCommError         CheckLibVersion(const RwWString& module);

    static void         SetPluginFolderPath(const RwWString &path);

    static RwWString    GetDllFolderPath();
    static void         SetDllFolderPath(const RwWString &path);
    
private:
    RwCommPluginManager();
    void                LoadModules(const RwWString &filePath);
    void                AddInvalidModule(const RwWString &name, unsigned int version, const RwWString &path, bool bDisplayVersion);

private:
    RwVector<HMODULE>       m_vModules;
    RwVector<ModuleInfo>    m_vUnloadedModules;
    static RwWString        m_pluginFolderPath;
    static RwWString        m_dllFolderPath;

private:
// LoadModules support
    bool    CheckLoadLibrary(const wchar_t *fullFilePathName, HMODULE &hModule, RwWString &strFailureMsg);
    bool    CheckIsPlugin(HMODULE hModule);
    bool    CheckInitPlugin(HMODULE hModule, RwWString &strFailureMsg);
    bool    CheckPluginVersion(HMODULE hModule, RwWString &strFailureMsg,
                               bool &bHaveVersion, unsigned int &version);
    bool    CheckPluginBuild(HMODULE hModule, RwWString &strFailureMsg);
};

#endif // (!defined(DOXYGEN))

#endif // !defined(AFX_RWCOMMPLUGINMANAGER_H__ABB64C10_D29E_4FEE_99D7_8784FEDE1C12__INCLUDED_)
