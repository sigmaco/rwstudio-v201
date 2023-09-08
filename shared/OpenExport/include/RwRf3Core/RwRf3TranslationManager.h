// RwRf3TranslationManager.h: interface for the RwRf3TranslationManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWRF3TRANSLATIONMANAGER_H__E6264724_3E08_4C99_BC97_19AA737A7E0C__INCLUDED_)
#define AFX_RWRF3TRANSLATIONMANAGER_H__E6264724_3E08_4C99_BC97_19AA737A7E0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwRf3Translator.h"
#include "RwRf3XMLParser.h"

/**
*   \ingroup rf3translator 
*
*  The RwRf3TranslationManager parses the given rf3 file, extracts its asset information and launches the
*  appropriate translator, according to the asset data. Each specific translator can then perform the actual
*  translation, according to the rf3 XML data passed to it. Each asset in the rf3 file can contain an 
*  optional translator name. If no translator is found, the RwRf3TranslationManager launches the default
*  export manager (RwExpMgrExportManager), which uses the entire scene graph architechture to perform the
*  translation. Users, developing their own custom game assets (which might not be scene graph related),
*  can use this mechanism to apply their own custom rf3 translation.
*
*  \see RwRf3Translator
*/
class RWRF3CORE_API RwRf3TranslationManager  
{
public:
	
	virtual ~RwRf3TranslationManager();

    /** Get the translator manager instance */
    static RwRf3TranslationManager &TheInstance();

    /**
    *   Register a custom translator with the translator manager.
    *
    *   \param translatorName the name of the translator. (This is the name that should also appear inside the
    *           rf3 file).
    *   \param pTranslator an instance of the custom translator to register.
    */
    void    Register(const RwWString &translatorName, RwRf3Translator *pTranslator);

    /** Get the RwRf3XMLParser used to do the initial XML parsing, for extracting the asset data */
    RwRf3XMLParser  &GetParser() { return m_Parser; }

    /**
    *   Do the translation.
    *
    *   \param vFiles a vector of input rf3 file names, on which to perform the tranlation. 
    *   \param sProjectTemplate the name of the project template to use in the translation.
    *   \param sProjectName a reference name, that will determine the actual output file name.
    *   \param sExportPath the output path name.
    *   \param pOptionOverrides a set of export options, which override the project template options.
    */
    RwCommError   Translate(const RwVector<RwWString> &vFiles,
                            const RwWString &sProjectTemplate = L"",
                            const RwWString &sProjectName = L"",
                            const RwWString &sExportPath = L"",
                            RwCommExportOptions *pOptionOverrides = NULL);


    /**
    *   Helper method to extract the asset information from the given rf3 file.
    *
    *   \param vAssetData a vector of RwRf3AssetData, describing each asset. The vector
    *               describes the top level assets. The child asset data is stored in the
    *               appropriate m_vChildren member, inside each RwRf3AssetData.
    *   \param sRf3FileName the input rf3 file name
    *   \param sProjectTemplate the name of project template to use in the translation.
    *   \param sProjectName a reference name, that will determine the actual output file name.
    *   \param sExportPath the output path name.
    *   \param pOptionOverrides a set of export options, which override the project template options.
    */
    RwCommError   GetAssetData(RwVector<RwRf3AssetData> &vAssetData,
                               const RwWString &sRf3FileName,
                               const RwWString &sProjectTemplate = L"",
                               const RwWString &sExportPath = L"",
                               const RwWString &sProjectName = L"",
                               RwCommExportOptions *pOptionOverrides = NULL);
                               

private:

    RwRf3TranslationManager();

    
    RwCommError     GetAssetInfo(const RwVector<RwXMLNodePtr> &vAssets, RwVector<RwRf3AssetData> &vAssetInfo);
    RwRf3AssetData  *ParseAsset(RwXMLNode *pNode, RwRf3AssetData *pParentData, RwVector<RwRf3AssetData> &vAssetInfo);
    RwCommError     GetAssetOutput(RwRf3AssetData &AssetData, RwRf3Translator *pTranslator,
                                      const RwWString &sProjectTemplate,
                                      const RwWString &sExportPath,
                                      const RwWString &sProjectName,
                                      RwCommExportOptions *pOptionOverrides);
private:

    RwMap<RwWString, RwRf3Translator*>   m_TransMap;

    RwRf3XMLParser                      m_Parser;

};

#endif // !defined(AFX_RWRF3TRANSLATIONMANAGER_H__E6264724_3E08_4C99_BC97_19AA737A7E0C__INCLUDED_)
