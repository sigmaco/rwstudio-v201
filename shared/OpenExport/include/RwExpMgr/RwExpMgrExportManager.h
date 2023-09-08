// RwExpMgrExportManager.h: interface for the RwExpMgrExportManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPMGREXPORTMANAGER_H__85D44548_3589_4D0A_B80E_9E3B5EE60A6F__INCLUDED_)
#define AFX_RWEXPMGREXPORTMANAGER_H__85D44548_3589_4D0A_B80E_9E3B5EE60A6F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwRf3Translator.h"

class RwExpMgrExportManager;

#define INIT_EXPORT_MANAGER_FUNC    "InitExportManager"
#define SET_EXPORT_MANAGER_FUNC     "SetCustomExportManager"

typedef void (*RwInitExportManagerFunc)  (RwExpMgrExportManager *pExportManager);

typedef void (*RwSetExportManagerFunc)(RwExpMgrExportManager *&pExportManager);

/**
*
*  \ingroup exportmanager
*
*   The RwExpMgrExportManager class is repsonsible for managing the entire export process including
*   building the scene graph, exporting and launching a viewer. You can export to or from rf3
*   files, define your own custom builders and exporters, customize options, saving of
*   the files, viewing etc.
*
*   \see RwCommBuilder, RwExpExporter RwRf3Builder, RwRf3Exporter
*/
class RWEXPMGR_API RwExpMgrExportManager : public RwRf3Translator 
{
public:
	virtual ~RwExpMgrExportManager();
    /**
    *   \return static instance of the class. RwExpMgrExportManager is a singleton.
    */
    static RwExpMgrExportManager *TheInstance();

    /**
    *   \return the registered RwExpAnmExporter.
    */
    RwCommExporter          *GetAnmExporter();
    /**
    *   \return the registered RwExpBspExporter.
    */
    RwCommExporter          *GetBspExporter();
    /**
    *   \return the registered RwExpDffExporter.
    */
    RwCommExporter          *GetDffExporter();
    /**
    *   \return the registered RwExpSplExporter.
    */
    RwCommExporter          *GetSplExporter();
    /**
    *   This method returns the exporter, mapped to the given asset type.
    *   \param assetType the asset type of the exporter
    *   \returns the exporter if successful. NULL otherwise.
    */
    RwCommExporter          *GetExporter(int assetType);

    /**
    *   \return the registered builder.
    */
    RwCommBuilder           *GetBuilder()     { return m_pBuilder; };
    /**
    *   \return the registered RwRf3Exporter.
    */
    RwRf3Exporter           *GetRf3Exporter() { return m_pRf3Exporter; };
    /**
    *   \return the registered RwRf3Builder.
    */
    RwRf3Builder            *GetRf3Builder()  { return m_pRf3Builder; };

    /**
    *   This function registers an anm exporter with the export manager.
    *   \param anmExporter RwExpAnmExporter to register
    */
    void                    SetAnmExporter(RwCommExporter *anmExporter);
    /**
    *   This function registers a bsp exporter with the export manager.
    *   \param bspExporter RwExpBspExporter to register
    */
    void                    SetBspExporter(RwCommExporter *bspExporter);
    /**
    *   This function registers a dff exporter with the export manager.
    *   \param dffExporter RwExpDffExporter to register
    */
    void                    SetDffExporter(RwCommExporter *dffExporter);
    /**
    *   This function registers an spl exporter with the export manager.
    *   \param splExporter RwExpSplExporter to register
    */
    void                    SetSplExporter(RwCommExporter *splExporter);
    /**
    *   This function registers a custom exporter with the export manager. Users can assign their own
    *   custom exporters to an asset type.
    *   \param assetType the type of the asset to map the exporter to
    *   \param pExporter a pointer to the exporter
    */
    void                    SetExporter(int assetType, RwCommExporter *pExporter);
    /**
    *   This function registers a builder with the export manager.
    *   \param pBuilder builder to register
    */
    void                    SetBuilder(RwCommBuilder *pBuilder);
    /**
    *   This function registers an rf3 exporter with the export manager.
    *   \param pRf3Exporter rf3 exporter to register
    */
    void                    SetRf3Exporter(RwRf3Exporter *pRf3Exporter);
    /**
    *   This function registers an rf3 builder with the export manager.
    *   \param pRf3Builder rf3 builder to register
    */
    void                    SetRf3Builder(RwRf3Builder *pRf3Builder);

    /**
    *   \return True if export is in progress. This is based on m_bExportInProgress which is set to true
    *           while export process is on.
    */
    bool                    GetExportInProgress() { return m_bExportInProgress; };

    /**
    *   \param bInProgress value to set the internal variable. When set to true analysis of the 3d
    *          application scene is disabled.
    */
    void                    SetExportInProgress(bool bInProgress) { m_bExportInProgress = bInProgress; };

    /**
    *   This functions exports a set of assets. It first calls ProcessProjectTemplate() to setup the export
    *   manager state for the export. It then calls BuildScene() to create an RwCommScene. It then calls
    *   PreExport(), ExportScene() and finally PostExport() to perform the actual export.
    *
    *   \param vAssets a vector of assets to export
    *   \param sProjectTemplate name of the project template
    *   \param sSceneName name of the file to save
    *   \param sExportPath path where to save the file
    *   \param pOptionOverrides an optional set of option overrides. If non NULL these options are added to the final set of export options.
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError     Export(const RwVector<RwCommAsset *> &vAssets,
                                   const RwWString &sProjectTemplate,
                                   const RwWString &sSceneName,
                                   const RwWString &sExportPath = L"",
                                   RwCommExportOptions *pOptionOverrides = NULL);

    /**
    *   This functions exports a set of Rf3 files. It first calls ProcessProjectTemplate() to setup the export
    *   manager state for the export. It then calls PreExport() to start the RenderWare engine and allocate
    *   asset lists. It then iterates through all the rf3 files and builds an RwCommScene for each using
    *   ReadRF3File() and exports it by by calling ExportScene(). Finally it calls PostExport() to stream
    *   assets and close the RenderWare engine.
    *
    *   \param vFiles a vector of rf3 files to export
    *   \param sProjectTemplate name of the project template
    *   \param sSceneName name of the file to save
    *   \param sExportPath path where to save the file
    *   \param pOptionOverrides an optional set of option overrides. If non NULL these options are added to the final set of export options.
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError     Export(const RwVector<RwWString> &vFiles,
                                   const RwWString &sProjectTemplate,
                                   const RwWString &sSceneName,
                                   const RwWString &sExportPath = L"",
                                   RwCommExportOptions *pOptionOverrides = NULL);



    /**
    *   This function performs an export and view of a set of assets.
    *   It first ensures that previous temporary view files are removed by calling
    *   DeleteTempViewFiles(). It then sets up  m_viewProjectOptions by calling SetViewProjectOptions().
    *   It then performs the export to a temporary location using the Export() function and views it
    *   using the ViewFile() function. It finally clears m_viewProjectOptions by calling ClearViewProjectOptions(). 
    *
    *   \param vAssets a vector of asset to view
    *   \param sProjectTemplate name of the project template
    *   \param pOptionOverrides an optional set of option overrides. If non NULL these options are added to the final set of export options.
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError     View(const RwVector<RwCommAsset *> &vAssets,
                                 const RwWString &sProjectTemplate,
                                 RwCommExportOptions *pOptionOverrides = NULL);

    /**
    *   This function performs an export and view of a set of rf3 files.
    *   The function first ensures that previous temporary view files are removed by calling
    *   DeleteTempViewFiles(). It then sets up the m_viewProjectOptions by calling SetViewProjectOptions().
    *   It then performs the export to a temporary location using the Export() function and views it
    *   using the ViewFile function. It finally clears m_viewProjectOptions by calling ClearViewProjectOptions(). 
    *
    *   \param vFiles a vector rf3 files to view
    *   \param sProjectTemplate name of the project template
    *   \param pOptionOverrides an optional set of option overrides. If non NULL these options are added to the final set of export options.
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError     View(const RwVector<RwWString> &vFiles, const RwWString &sProjectTemplate,
                            RwCommExportOptions *pOptionOverrides = NULL);

    /**
    *   Initialize all per asset options such as Sub-Hierarchy Animation, Dynamic Key Frame Generation,
    *   Start Frame, End Frame, etc to their default values. This function is called from ProcessProjectTemplate().
    *   
    *   \param options to initialize
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError     InitializePerAssetOptions(RwCommExportOptions &options);

    /**
    *   This function is called on each export to verify any custom export file paths being
    *   used. It converts backslashes to forward slashes, makes relative paths absolute
    *   to sProjectPath, strips any extension and replaces empty filenames with "default".
    *   
    *   \param sCustomExportFile the filename to verify
    *   \param sProjectPath the default export path to prepend to relative paths
    *
    *   \return RWCOMM_SUCCESS
    */
    virtual RwCommError     VerifyCustomExportFile(RwWString &sCustomExportFile, const RwWString &sProjectPath);
    
    /**
    *   Build a legacy file name for an asset. If the asset has a custom filename setup then the path
    *   and filename used are taken from this value. Otherwise they default to the path passed to the
    *   function and the asset name. The extension used is based on the asset type passed.
    *   The postfixes "_PS2", "_Xbox" and "_GameCube" are used on the relevant platforms to prevent name
    *   clashes when exporting to multiple platforms. The path and filename used are validated
    *   and set to sensible default in the event of errors.
    *
    *   \param sPath location where to save the asset
    *   \param sAssetName name of the asset.
    *   \param assetType type of the asset, animated hierarchy, static world etc.
    *   \param sCustomAssetFile a custom asset file. Ingored if an empty string.
    *   \param Platform to which we export
    *
    *   \return The full path for the legacy file.
    */
    virtual RwWString     BuildLegacyFileName(const RwWString &sPath,
                                                 const RwWString &sAssetName, 
                                                 const int assetType,
                                                 const RwWString &sCustomAssetFile,
                                                 const RwWString &Platform);

    /**
    *   Build an rws file name for an asset. If the asset has a custom filename setup then the path
    *   and filename used are taken from this value. Otherwise they default to the path and project names
    *   passed to the function. The extension used is is based on the platform being exporting to and
    *   is chosen from ".rws", ".rp2", ".rg1" and ".rx1". The path and filename used are validated
    *   and set to sensible default in the event of errors.
    *
    *   \param sPath the default export path
    *   \param sProject the current project name, by default used for the rws file name
    *   \param sPlatform the name of the platform to export to
    *   \param sCustomAssetFile a custom asset file. Ingored if an empty string.
    *
    *   \return The full path for the rws file.
    */
    virtual RwWString    BuildRwsName(const RwWString &sPath, const RwWString &sProject,
                            const RwWString &sPlatform, const RwWString &sCustomAssetFile);

    /**
    *   Build an rf3 file name for an asset. If the asset has a custom filename setup then the path
    *   and filename used are taken from this value. Otherwise they default to the path and project names
    *   passed to the function. The extension used is is based on the platform being exporting to and
    *   is chosen from ".rws", ".rp2", ".rg1" and ".rx1". The path and filename used are validated
    *   and set to sensible default in the event of errors.
    *
    *   \param sPath the default export path
    *   \param sProject the current project name, by default used for the rws file name
    *   \param sAssetName name of the asset.
    *   \param sCustomAssetFile a custom asset file. Ingored if an empty string.
    *   \param bSplitRf3 whether this rf3 file will be split by asset
    *
    *   \return The full path for the rws file.
    */
    virtual RwWString    BuildRf3Name(const RwWString &sPath, const RwWString &sProject,
                            const RwWString &sAssetName, const RwWString &sCustomAssetFile, bool bSplitRf3);


    // From RwRf3Translator

    virtual RwCommError   OnPreTranslate(const RwWString &sProjectTemplate = L"",
                                         const RwWString &sProjectName = L"",
                                         const RwWString &sExportPath = L"",
                                         RwCommExportOptions *pOptionOverrides = NULL);

    virtual RwCommError   Translate(RwXMLNode *pRf3RootNode, RwRf3XMLParser *pParser,
                                    const RwWString &sProjectTemplate = L"",
                                    const RwWString &sProjectName = L"",
                                    const RwWString &sExportPath = L"",
                                    RwCommExportOptions *pOptionOverrides = NULL);

    virtual RwCommError   OnPostTranslate(const RwWString &sProjectTemplate = L"",
                                         const RwWString &sProjectName = L"",
                                         const RwWString &sExportPath = L"",
                                         RwCommExportOptions *pOptionOverrides = NULL);

    /**
    *   This function returns calculates the output files that will be generated when a given
    *   asset is exported. The passed project template and the asset template referenced by the
    *   asset will be parsed. The output file information will be returned in vFileInfo.
    *
    *   \param asset the asset to be queried
    *   \param sProjectTemplate the
    *   \param sExportPath the default export path
    *   \param sProjectName the current project name, by default used for the rws file name
    *   \param assetData a structure containing the asset data, which will also be filled in with
    *                    the output file data
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError     GetOutputFileNames(RwRf3AssetData &assetData,
                                               const RwWString &sProjectTemplate,
                                               const RwWString &sExportPath,
                                               const RwWString &sProjectName,
                                               RwCommExportOptions *pOptionOverrides = NULL);

    /**
    *   This function returns calculates the output files that will be generated when a given
    *   asset is exported. No template files are parsed during this function call. The output
    *   file information will be returned in vFileInfo.
    *
    *   
    *   \param options the current export options
    *   \param vPlatforms the current export platforms
    *   \param sExportPath the default export path
    *   \param sProjectName the current project name, by default used for the rws file name
    *   \param assetData a structure containing the asset data, which will also be filled in with
    *                    the output file data
    *   \param eType the type of the asset
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError     GetOutputFileNames(RwRf3AssetData &assetData,
                                               const RwCommExportOptions &options,
                                               const RwVector<RwWString> &vPlatforms,
                                               const RwWString &sExportPath,
                                               const RwWString &sProjectName,
                                               AssetType eType);



protected:
    RwExpMgrExportManager();

    // Overridables
    /**
    *   This function is called in the main export functions before ExportScene is called. It starts the
    *   RenderWare Engine.
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError     PreExport();
    /**
    *   This function is called at the end of the main export functions and is responsible for streaming
    *   the asset data out, pre-instancing platform specific data and closing the RenderWare Engine.
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError     PostExport();

    /**
    *   This function simply uses the RwExpStreamAssetList::StreamToFile. The TOC paramater is setup
    *   based on the RWS_EMBEDTOC option from the current project template.
    *   \param pAssetList an asset list to stream out
    *   \param filename destination file name
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError     StreamAssets(RwExpStreamAssetList *pAssetList, const RwWString &filename);
    
    /**
    *   This function ensures that the size of m_vAssetLists equals that of m_vExportFileNames.
    *   If the size of m_vAssetLists is increased then an asset lists per member of m_vPlatforms
    *   is created.
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError     AllocateAssetLists();
    
    /**
    *   Delete the RwExpStreamAssetLists we created in AllocateAssetLists().
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError     FreeAssetLists();

    /**
    *   Do the actual export. Called from Export(). Loops all export files, platforms and assets
    *   and calls ExportAsset().
    *
    *   \param pScene a pointer to the scene graph to export
    *   \param pOptionOverrides an optional set of option overrides. If non NULL these options are added to the final set of export options.
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError     ExportScene(RwCommScene *pScene, RwCommExportOptions *pOptionOverrides = NULL);

    /**
    *   Export an asset. Calls RwCommExporter::Export() for the appropriate registered exporter.
    *
    *   \param pScene a pointer to the scene graph to export
    *   \param pAsset a pointer to an assets to export from the scene graph
    *   \param options for the export process
    *   \param pAssetList where to store the exported data
    *   \param Platform to which we export
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError     ExportAsset(RwCommScene *pScene,
                                        RwCommAsset *pAsset,
                                        RwCommExportOptions &options,
                                        RwExpStreamAssetList *pAssetList,
                                        const RwWString &Platform);
    /**
    *   Export the ambient light from a scene.
    *
    *   \param pScene a pointer to the scene graph to export
    *   \param pAssetList where to store the exported data
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError     ExportAmbientLight(const RwCommScene *pScene, 
                                RwExpStreamAssetList *pAssetList);
                                
    /**
    *   Exports the rendering environment from a scene.  
    *   This data is currently only the background colour.
    *
    *   \param pScene a pointer to the scene graph to export
    *   \param pAssetList where to store the exported data
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError     ExportEnvironment(const RwCommScene *pScene, 
                                RwExpStreamAssetList *pAssetList);

    /**
    *   This function attempts to pre-instance the given file for the given platform. This is done
    *   using the Instance function from RwInstance.dll. On the Xbox platform DDS format textures are
    *   extracted from the asset list using GetTypeTextureNames and passed explicitly to the instancer.
    *
    *   \param sFileName full path of the file to instance.
    *   \param sPlatform the platform to instance on.
    *   \param pAssetList the current platform asset list.
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError     PreInstanceFile(const RwWString &sFileName,
                                            const RwWString &sPlatform,
                                            RwExpStreamAssetList *pAssetList);
    /**
    *   View an rws file. This is called from View() with a file that is saved in a
    *   temporary location.
    *
    *   \param sFileName full path of the rws file to view
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError     ViewFile(const RwWString &sFileName);

    /**
    *   Create a partition object if any Static World asset requires
    *   partition visualization
    *
    *   \param vAsset a vector of assets to process for partition visualization
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError     CreateAssetPartitionObject( const RwVector<RwCommAsset *> &vAssets );

    /**
    *   Build the scene graph from an rf3 file using the registered RwRf3Builder.
    *   
    *   \param sceneFile full path of the rf3 file
    *   \param pScene returns a scene graph
    *   \param vAssets returns a vector of assets
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    RwCommError     ReadRF3File(const RwWString &sceneFile, RwCommScene *&pScene,
                            RwVector<RwCommAsset *> &vAssets);

    /**
    *   Build the scene graph from an rf3 root node using the registered RwRf3Builder.
    *   
    *   \param pRootRf3Node the root of an XML rf3 tree
    *   \param pScene returns a scene graph
    *   \param vAssets returns a vector of assets
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    RwCommError     ReadRF3File(RwXMLNode *pRootRf3Node, RwRf3XMLParser *pParser, RwCommScene *&pScene,
                                RwVector<RwCommAsset *> &vAssets);
    /**
    *   Export an rf3 file from the scene graph using the registered RwRf3Exporter. It first
    *   constructs a vector of RwRf3AssetInfo objects from the passed assets using the 
    *   ProcessAssetOptions function. Depending on the RF3_SPLITBYASSET option it then
    *   makes a single or multiple calls to RwRf3Exporter::ExportFromAssets.
    *   
    *   \param pScene pointer to the scene graph
    *   \param vAssets vector of assets to export
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    RwCommError     SaveRF3File(RwCommScene *pScene, const RwVector<RwCommAsset *> &vAssets);

    /**
    *   This function builds a vector of RwRf3AssetInfo objects which represent the assets
    *   passed in vAssets.
    *   
    *   \param pScene pointer to the scene graph
    *   \param vAssets vector of assets to export
    *   \param vAssetInfo the vector of asset info built
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    RwCommError     BuildAssetInfo(RwCommScene *pScene, const RwVector<RwCommAsset *> &vAssets,
                                      RwVector<RwRf3AssetInfo> &vAssetInfo);

    /**
    *   This function ensures that for all assets in a vector all parents of each asset are also
    *   members of the vector. Any assets that are not present are disabled and added to the vector.
    *   
    *   \param vAssets vector of assets to be checked
    *   \param vUpdatedAssets the new list of assets including parents
    *   \param vDisabledAssets any assets that have been disabled
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    RwCommError     AddRF3AssetParents(const RwVector<RwCommAsset *> &vAssets,
                                       RwVector<RwCommAsset *> &vUpdatedAssets,
                                       RwVector<RwCommAsset *> &vDisabledAssets);

    /**
    *   Retrieve the selection for this asset. If the asset has parents the selection should include
    *   the parent selections.
    *
    *   \param pAsset to retrieve the selection from
    *   \param pScene pointer to the scene graph
    *   \param vSelection vector of selected objects in the scene to be filled in
    *
    */
    RwCommError GetAssetSelection(RwCommAsset *pAsset,
                                  RwCommScene *pScene,
                                  RwVector<RwCommObject *> &vSelection);

    /**
    *   This function sets up export data that relies on the contents of the project template.
    *   It first extracts the platforms to be targetted using RwExpMgrTemplateManager::GetTemplatePlatforms
    *   and stores them in the m_vPlatforms member. It then sets up the default template options using
    *   RwExpMgrTemplateManager::GetTemplateOptions. It then adds the common options from the specified
    *   project template using the same function. It finally extracts the platform specific options from the
    *   project template and stores the concatenated options in the m_vPlatformProjectOptions member.
    *
    *   \param sProjectTemplate the project template file to be processed.
    *   \param pOptionOverrides an optional set of option overrides. If non NULL these options are added to the final set of export options.
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    RwCommError     ProcessProjectTemplate(const RwWString &sProjectTemplate,
                                           RwCommExportOptions *pOptionOverrides = NULL);
    /**
    *   Get all the options for this asset. The functions first recurses up the hierarchy if the
    *   asset has parents. It then extracts common followed by platform specifc options from the asset
    *   template linked to by the asset (RwCommAsset::GetAssetTemplateName). The template manager,
    *   RwExpMgrTemplateManager, is used to extract options from the template file. The functions finally
    *   adds in any per-asset artists options returned from RwCommAsset::GetOptions. The order in which
    *   options are extracted is important as the RwCommExportOptions implementation of operator+ overides
    *   options that already exist.
    *
    *   \param pAsset asset to get the options for
    *   \param sPlatform platform that we export to
    *   \param pScene pointer to the scene graph
    *   \param options returns all merged options
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    RwCommError     ProcessAssetOptions(const RwCommAsset *pAsset, const RwWString &sPlatform,
                            RwCommScene *pScene, RwCommExportOptions &options);
    
    /**
    *   This function checks the per-asset options for any problems. Currently it checks that the
    *   animation end frame is later than the start frame. If this is not the case the function
    *   sets the end frame to the start frame + 1. The function also replaces the "Start" and "End"
    *   animation frame special cases with the current scene start and end frames.
    *
    *   \param options the options to be checked
    *   \param pScene pointer to the scene graph
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    RwCommError     CheckAssetOptions(RwCommExportOptions &options, RwCommScene *pScene);

    /** This function takes a flat list of assets and groups them by the file name they
    *   will create on export. It fills in the m_vExportFileNames and m_vExportFileAssets member
    *   variables. If no assets have custom export paths they will all be grouped by the export
    *   file name indicated by sExportPath + sSceneName. The calcuated export file names do
    *   not include extensions as these are based on the output file options set in the project
    *   template
    *
    *   \param vAssets the assets to be grouped.
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    RwCommError     GroupAssetsByExportFileName(const RwVector<RwCommAsset *> &vAssets);
    
    /** This function adds a single asset to the m_vExportFileNames and m_vExportFileAssets members.
    *   If the given export file name is not already in m_vExportFileNames a new entry is added to
    *   both vectors.
    *
    *   \param pAsset the asset to be added
    *   \param sExportFile the file this asset is to be exported to (without extension).
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    RwCommError     AddAssetToExportFileVectors(RwCommAsset *pAsset, const RwWString &sExportFile);

    /** Set the project option overrides used when viewing. The options should be stored in the
    *   m_viewProjectOptions member and will used to override the normal options when performing views.
    *   By default these options will disable all file exports other than .rws. It also enables
    *   texture and effect dictionaries and disables pre-instancing.
    */
    void            SetViewProjectOptions();
    
    /** Clear all viewing options */
    void            ClearViewProjectOptions();

    /**
    *   Outputs statistics on the texture dictionary using RwExpOutputTextureDictionaryStats and
    *   then embeds the Platform Indpendent dictionary returned by RwExpStreamAssetList ::GetTextureDictionary.
    *   \param pAssetList an asset list where to embed the texture dictionary.
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    RwCommError     EmbedTextureDictionary(RwExpStreamAssetList *pAssetList);
    /**
    *   Creates an effect dictionary using RpMTEffectDictCreate and then add the
    *   unique effect file names from RwExpStreamAssetList ::GetEffectFiles. This
    *   effect dictionary is then added to the asset list.
    *   \param pAssetList an asset list in which to embed the effect dictionary.
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    RwCommError     EmbedEffectDictionary(RwExpStreamAssetList *pAssetList);
    /**
    *   This function creates an ambient RenderWare light clump and adds a new clump entry to the asset list.
    *   \param pAssetList an asset list in which to embed the ambient light
    *   \param ambientColor ambient light color
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    RwCommError     EmbedAmbientLight(RwExpStreamAssetList *pAssetList, const RwCommVec3 &ambientColor);

    /**Build the scene graph using the registered builder */
    RwCommScene     *BuildScene();

    /**
    *   Set up parameters for the progress bar. The progress bar iteration count is setup based on
    *   m_vPlatforms * vAssets.size().
    *   \param vAssets the assets being exported.
    */
    void            SetupProgressBar(const RwVector<RwCommAsset *> &vAssets);
    
    /** Start timing of the export process. Prints an output message and initialises the m_tStart member.*/
    void            StartTimer();
    
    /** End timing of the export process. Stores the end time in m_tEnd and outputs the export duration.*/
    void            EndTimer();

    /** Fills in a vector of enabled assets.
    *   \param vAssets an vector of assets which may be enabled or disabled.
    *   \param vEnabledAssets a vector of the enabled assets.
    */
    void            GetEnabledAssets(const RwVector<RwCommAsset *> &vAssets,
                                     RwVector<RwCommAsset *> &vEnabledAssets);
    /**
    *   Some assets are organized in a hierarchy like Animated Hierarchy and Animations.
    *   This function flattens this hierarchy so that subsequent code does not need to
    *   travserse the asset hierarchy.
    *   \param vAssets a vector of source assets
    *   \param vFlatAssets a flattened output vector of assets.
    */
    void            FlattenAssetVector(const RwVector<RwCommAsset *> &vAssets,
                                       RwVector<RwCommAsset *> &vFlatAssets);
    /**
    *   Add an asset and its children to a vector. If the asset is already a member of the vector
    *   it is not added.
    *   \param pAsset the asset to be added.
    *   \param vAssets an output vector of assets.
    */
    void            AddAssetToVector(RwCommAsset *pAsset, RwVector<RwCommAsset *> &vAssets);
    /**
    *   Run through all the textures in the asset list and extract the ones with the
    *   specified extension.
    *   \param pAssetList the asset list to search.
    *   \param extension the file extension to be matched.
    *   \param matchingNames an output vector of matching names.
    */
    const void      GetTypeTextureNames(RwExpStreamAssetList *pAssetList, RwWString &extension,
                                           RwVector<RwWString> &matchingNames);

    /**
    *   View an rws file. This is called from View() with a file that is saved in a
    *   temporary location.
    *
    *   \param sTmpFiles files to delete. Wildcards  a allowed. For example c:\temp.*
    *   \param sPath files to delete. Wildcards  a allowed. For example c:\temp.*
    */
    void            DeleteTempViewFiles(const RwWString &sTmpFiles, const RwWString &sPath);
    /**
    *   This function checks whether the passed file exists or not. This is used to check
    *   the export completed sucessfully before starting the viewer.
    *   \param sTmpFiles location of the file to check
    *
    *   \return True if the file exists, false otherwise.
    */
    bool            TempViewFilesExist(const RwWString &sTmpFiles) const;
    
    /**
    *   This function verifies a project name. Currently the function simply checks that
    *   the project name is not an empty string and sets it to "default" if it is.
    *   \param sDefaultProjectName the project name to be verified
    *
    *   \return the verified project name
    */
    RwWString    VerifyProjectName(const RwWString &sDefaultProjectName) const;

    /**
    *   This function verifies an export path. Currently it checks that the path is
    *   not an empty string, that all slashes are back slashes and that it contains
    *   a trailing slash. Empty paths are set to c:\
    *   \param sDefaultExportPath the path to be verified
    *
    *   \return the verified project path
    */
    RwWString    VerifyExportPath(const RwWString &sDefaultExportPath) const;

    /**
    *   Internal function used for testing only.
    *   \param spScene
    *   \param vAssets
    *
    *   \return An RwCommScene
    */
    RwCommScene     *WriteAndReadRf3(RwCommScene *pScene, RwVector<RwCommAsset *> &vAssets);

protected:

    RwWString                    m_sDefaultSceneName;
    RwWString                    m_sDefaultExportPath;
    time_t                          m_tStart, m_tEnd;

    // The file name per export file
    RwVector<RwWString>      m_vExportFileNames;

    // The list of assets contained in each export file
    RwVector< RwVector<RwCommAsset *> > m_vExportFileAssets;

    // The per platform assets lists for each export file
    RwVector< RwVector<RwExpStreamAssetList *> >  m_vAssetLists;

    // The global project options
    RwCommExportOptions             m_projectOptions;

    // The view specific project option overrides
    RwCommExportOptions             m_viewProjectOptions;
    
    // The platforms we're exporting to
    RwVector<RwWString>      m_vPlatforms;
    
    // The project options for each platform
    RwVector<RwCommExportOptions> m_vPlatformProjectOptions;

    bool                            m_bExportInProgress;
    bool                            m_bExportAborted;

private:

    static RwExpMgrExportManager    *m_pInstance;

    RwCommBuilder                   *m_pBuilder;
    
    RwRf3Builder                    *m_pRf3Builder;
    RwRf3Exporter                   *m_pRf3Exporter;

    RwMap<int, RwCommExporter*> m_mExporters;
};

#endif // !defined(AFX_RWEXPMGREXPORTMANAGER_H__85D44548_3589_4D0A_B80E_9E3B5EE60A6F__INCLUDED_)
