// RwRf3Translator.h: interface for the RwRf3Translator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWRF3TRANSLATOR_H__FCD3FE2A_C32D_4B43_A1E1_104AC6030910__INCLUDED_)
#define AFX_RWRF3TRANSLATOR_H__FCD3FE2A_C32D_4B43_A1E1_104AC6030910__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwRf3Core.h"
#include "RwCommObject.h"
#include "RwXMLDOM.h"
#include "RwRf3XMLParser.h"

#include "RwCommExportOptions.h"

/**
*   \ingroup rf3translator 
*
*   The rf3 asset output path data structure. This structure describes the translator output path for each
*   asset.
*
*  \see RwRf3AssetData::m_vExportFileInfo
*/
struct RWRF3CORE_API RwRf3ExportFileInfo
{
    /** The abolsute filename of the translated file. */
    RwWString m_sFileName;

    /** The translated file platform. */
    RwWString m_sPlatform;
};

/**
*   \ingroup rf3translator 
*
*   The rf3 asset data structure. This structure describes the rf3 asset information.
*
*  \see RwRf3Translator::GetOutputFileNames()
*/
struct RWRF3CORE_API RwRf3AssetData
{
    /** The name of the asset. */
    RwWString                       m_sName;

    /** The asset's template name. */
    RwWString                       m_sTemplateName;

    /** The asset's GUID*/
    RwWString                       m_Guid;

    /** The asset's translator name */
    RwWString                       m_Translator;

    /** Enable state*/
    bool                            m_bEnabled;

    /** Vector of child assets */
    RwVector<RwRf3AssetData>        m_vChildren;

    /** Custom export file name*/
    RwWString                       m_sCustomExportFile;

    /** Vector of file export info*/
    RwVector<RwRf3ExportFileInfo>   m_vExportFileInfo;
};


/**
*   \ingroup rf3translator 
*
*   The rf3 translator abstract base class. Derive off this class to develop your own custom rf3 translator.
*   To register your translator with the translation manager use the RwRf3TranslationManager::Register()
*   method.
*   It is the translator's responsibility to stream out the final output file(s). The input rf3 file is
*   given as an XML DOM hierarchy.
*
*   \see RwRf3Translator::Translate()
*   \see RwRf3TranslationManager
*/
class RWRF3CORE_API RwRf3Translator  : public RwCommObject
{
public:
	RwRf3Translator();
	virtual ~RwRf3Translator();

    /**
    *   Called once at the start of the translation.
    *
    *   \param sProjectTemplate the name of the project template to use in the translation.
    *   \param sProjectName a reference project name, that will determine the actual output file name.
    *   \param sExportPath the output path name.
    *   \param pOptionOverrides a set of export options, which override the project template options.
    *
    */
    virtual RwCommError   OnPreTranslate(const RwWString &sProjectTemplate = L"",
                                         const RwWString &sProjectName = L"",
                                         const RwWString &sExportPath = L"",
                                         RwCommExportOptions *pOptionOverrides = NULL) { return RWCOMM_SUCCESS; }

    /**
    *   Implement the actual translation. Called for each rf3 file in the translation process.
    *
    *   \param pRf3RootNode the rf3 XML root node, on which to perform the translation.
    *   \param pParser the rf3 XML parser, used to parse the rf3 file
    *   \param sProjectTemplate the name of the project template to use in the translation.
    *   \param sProjectName a reference project name, that will determine the actual output file name.
    *   \param sExportPath the output path name.
    *   \param pOptionOverrides a set of export options, which override the project template options.
    *
    */
    virtual RwCommError   Translate(RwXMLNode *pRf3RootNode, RwRf3XMLParser *pParser,
                                    const RwWString &sProjectTemplate = L"",
                                    const RwWString &sProjectName = L"",
                                    const RwWString &sExportPath = L"",
                                    RwCommExportOptions *pOptionOverrides = NULL) = 0;

    /**
    *   Called once at the end of the translation.
    *
    *   \param sProjectTemplate the name of the project template to use in the translation.
    *   \param sProjectName a reference project name, that will determine the actual output file name.
    *   \param sExportPath the output path name.
    *   \param pOptionOverrides a set of export options, which override the project template options.
    *
    */
    virtual RwCommError   OnPostTranslate(const RwWString &sProjectTemplate = L"",
                                          const RwWString &sProjectName = L"",
                                          const RwWString &sExportPath = L"",
                                          RwCommExportOptions *pOptionOverrides = NULL) { return RWCOMM_SUCCESS; }


    /**
    *   Derive this method to fill in the RwRf3AssetData::m_vExportFileInfo with the translator ouput file name(s).
    *   Called once for each asset. You can specify a different output path for each asset, or a single output
    *   path if all of the given assets are translated to the same output file.
    *
    *   \param sProjectTemplate the name of the project template to use in the translation.
    *   \param sExportPath the output path name.
    *   \param sProjectName a reference project name, that will determine the actual output file name.
    *
    */
    virtual RwCommError   GetOutputFileNames(RwRf3AssetData &assetData,
                                             const RwWString &sProjectTemplate,
                                             const RwWString &sExportPath,
                                             const RwWString &sProjectName,
                                             RwCommExportOptions *pOptionOverrides = NULL) = 0;

};

#endif // !defined(AFX_RWRF3TRANSLATOR_H__FCD3FE2A_C32D_4B43_A1E1_104AC6030910__INCLUDED_)
