#ifndef RWEXPMGRTEMPLATEMANAGER_H
#define RWEXPMGRTEMPLATEMANAGER_H

#define RWEXP_TEMPLATE_VERSION 3,7,0,3

/**
*  \ingroup helperclasses 
*
*   Template types. Beside asset and project templates, there is  default template type.
*   It defines only one template file which lives in \Graphics\export\bin and is called
*   DefaultTemplate.rwt All other templates are generated from this one if not present
*   and it has all the options from all templates. You can't set the folder for this
*   template.
*/
enum RwExpMgrTemplateType
{
    /** asset template */
    assetTemplate,
    /** project template */
    projectTemplate,
    /** default template */
    defaultTemplate,
};

/**
*  \ingroup helperclasses 
*
*   Property structure is like an RwCommOption but much richer. It holds hierarchy,
*   descriptions, enums etc.
*/
struct RWEXPMGR_API RwExpMgrTemplateProperty
{
    /** Name of the property. */
    RwWString                        m_name;
    /** Type of the property. It can be bool, int, enum, float, string, enumstring. */
    RwWString                        m_type;
    /** Value of the property, stored as the string. */
    RwWString                        m_value;
    /** Description of the property. */
    RwWString                        m_description;
    /**
    *   Location of the property. It is a vector of string: gamecube, generic, ps2,
    *   xbox, anm, bsp, dff, spl. They define in which project/asset template this
    *   option belongs.
    */
    RwVector<RwWString>          m_location;
    /** Template version in which this property was added. */
    RwWString                        m_born;
    /** Whether this property has been retired from use in the current template version. */
    RwWString                        m_retired;
    /** Vector of enum definitions for the property. */
    RwVector<RwWString>          m_enums;
    /** Vector of all child properties. */
    RwVector<RwExpMgrTemplateProperty*> m_children;

public:

    ~RwExpMgrTemplateProperty( void );
    RwExpMgrTemplateProperty( void );
    RwExpMgrTemplateProperty(const RwExpMgrTemplateProperty &CopyMe);
    RwExpMgrTemplateProperty &operator =(const RwExpMgrTemplateProperty &CopyMe);

private:
    
    void DeleteAllChildren( void );
    void Clone(const RwExpMgrTemplateProperty &CopyMe);

};


// =============================================================================
// ----------------------  RwExpMgrTemplateManager CLASS  ----------------------
// =============================================================================
class RwExpMgrTemplateManager;
enum ExpMgrTemplateScopeType;

//disable warnings on extern before template instantiation

#include "patterns\RwObserver.h"
#include "RwUtilConfig.h"

RW_IMPLEMENT_OBSERVER_LINKING(RwExpMgrTemplateManager, void*);

/**
*
*  \ingroup helperclasses 
*
*   RwExpMgrTemplateManager class is managing the template files. Template files
*   have .rwt extension and they are xml files that can be edited from any text
*   or xml editor. 
*
*   They hold all the export options for projects and assets that are divided into
*   sections. Top level sections are Output (only for project templates), Common
*   and Platforms sections. Platform section has a subsection for each platform
*   that can be enabled or disabled by an Enable option in the section. The
*   template structure can be mirrored in hierarchy of RwExpMgrTemplateProperty.
*/
class RWEXPMGR_API RwExpMgrTemplateManager : public RwObserverSubject<RwExpMgrTemplateManager>
{
public:

    enum ObserverUpdateMessages
    {
        AssetTemplatesDirty = 0,
        ProjectTemplatesDirty, 
    };

public:
    RwExpMgrTemplateManager();
    ~RwExpMgrTemplateManager();

    /**
    *   \return static instance of the class. RwExpMgrExportManager is a singleton.
    */
    static          RwExpMgrTemplateManager  &TheInstance();
    
    RwCommError GetCurrentConfiguration(RwUtilComponentConfiguration &config, ExpMgrTemplateScopeType &scope) const;
    RwCommError SetCurrentConfiguration(const RwUtilComponentConfiguration &config, const ExpMgrTemplateScopeType &scope);

    /**
    *   Get the template folder path from the registry.
    *
    *   \param type of the templates to get the folder from
    *   \param templateFolder returns the path where the templates of this type live
    *   
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    RwCommError     GetTemplateFolder(RwExpMgrTemplateType type, RwWString &templateFolder);
    /**
    *   Set the template folder path. Path is saved in the registry.
    *
    *   \param type of the templates to set the folder for
    *   \param templateFolder path where the templates of this type live
    *   
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    RwCommError     SetTemplateFolder(RwExpMgrTemplateType type, const RwWString &templateFolder);

    /**
    *   \param type of the templates to get
    *   \param templates returns all the templates' names of the specified type
    *   
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    RwCommError     GetTemplates(RwExpMgrTemplateType type, RwVector<RwWString> &templates);
    
    // Per template methods.
    /**
    *   This will return all the common options plus platform specific ones if the platform
    *   is not an empty string.
    *
    *   \param type of the template to get the options from
    *   \param templateName name of the template to query
    *   \param platform specifies which section to query in the given template. If empty
    *                   string no platform specific options are queried.
    *   \param options returns the export options of the template for the specified platform
    *   
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    RwCommError     GetTemplateOptions(RwExpMgrTemplateType type, const RwWString &templateName,
                                       const RwWString &platform, RwCommExportOptions &options);
    /**
    *   Create a template file from the specified options. RwCommOption doesn't hold
    *   a hierarchy, but first properties, which have a hierarchy, are loaded from the
    *   defaultTemplate and then options set to the specified values.
    *
    *   \param type of the template
    *   \param templateName name of the template to be saved
    *   \param options to save
    *   
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    RwCommError     SetTemplateOptions(RwExpMgrTemplateType type, const RwWString &templateName,
                                       const RwCommExportOptions &options);
    /**
    *   Iterate over templates of specified type and check if there is one with given options.
    *
    *   \param templateName returns the template name if successful
    *   \param type of the templates to check
    *   \param options to check
    *   
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    RwCommError     FindTemplateFromOptions(RwWString &templateName, RwExpMgrTemplateType type, 
                                            const RwCommExportOptions &options);

    /**
    *   Get enabled platforms from the specified template.
    *
    *   \param type of the template to query
    *   \param templateName name of the template file
    *   \param platforms returns all the string of the enabled platforms in the specified
    *                    template
    *   
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    RwCommError     GetTemplatePlatforms(RwExpMgrTemplateType type, const RwWString &templateName,
                                         RwVector<RwWString> &platforms);

    /**
    *   Load template properties from the specified template file
    *
    *   \param type of the template to load
    *   \param templateName name of the template file
    *   \param property returns top level property
    *   \param version of the template read
    *   
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    RwCommError     LoadTemplateProperties(RwExpMgrTemplateType type, const RwWString &templateName,
                                           RwExpMgrTemplateProperty *property, int &version);

    /**
    *   Rename the specified template.
    *
    *   \param type of the template
    *   \param templateName name of the template file to rename
    *   \param NewName for the template
    *   
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    RwCommError     RenameTemplateProperties(RwExpMgrTemplateType type, const RwWString &templateName,
                                             RwWString &NewName);

    /**
    *   Save properties to the template file.
    *
    *   \param type of the template to save
    *   \param templateName name of the template file
    *   \param version number to give to the template
    *   \param property to save
    *   
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    RwCommError     SaveTemplateProperties(RwExpMgrTemplateType type, const RwWString &templateName,
                                           int version, RwExpMgrTemplateProperty *property);
    /**
    *   Create a basic hierarchy of top level properties. It will leave Output, Common, and
    *   all Platform, but without any options in them.
    *
    *   \param type of the template properties to create
    *   \param property returns the top level property
    *   
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    RwCommError     CreateTemplateProperties(RwExpMgrTemplateType type,
                                             RwExpMgrTemplateProperty *property);

    /**
    *   Check if the template file is writable or read-only.
    *
    *   \param type of the template
    *   \param templateName name of the template to check
    *   \param bWritable returns true if the file is writable, false otherwise
    *   
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    RwCommError     GetTemplateWritable(RwExpMgrTemplateType type, const RwWString &templateName,
                                        bool &bWritable);
    /**
    *   Set if the template file is writable or read-only.
    *
    *   \param type of the template
    *   \param templateName name of the template to set
    *   \param bWritable true for writable, false for read-only
    *   
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    RwCommError     SetTemplateWritable(RwExpMgrTemplateType type, const RwWString &templateName,
                                        bool bWritable);

    // Versioning.
    /**
    *   Get template version from the version string that is extracted from the template
    *   files. Version string also contains RenderWare version number.
    *
    *   \param strVersion version string
    *
    *   \return Template version number.
    */
    int             GetTemplateVersionFromVersionString(const RwWString &strVersion);
    /**
    *   Create a version string we save in the template files, from the template version
    *   number. Version string also contains RenderWare version number.
    *
    *   \param templateVersion template version number
    *
    *   \return Version string that we stream out in the template files.
    */
    RwWString    GetVersionStringFromTemplateVersion(int templateVersion);

    // Default template files.
    /**
    *   Create all default project and asset templates. This is executed on each start of
    *   the 3D application. If the files exist nothing is done. Missing files are recreated.
    */
    void            CreateDefaultTemplateFiles();

    /**
    *   \param type of the template
    *   \param templateName name of the template to get the path for.
    *   
    *   \return The full path of the specified template.
    */
    RwWString    CreateTemplatePath(RwExpMgrTemplateType type, const RwWString &templateName);


    RwCommError     UpdateTemplateFile(const RwWString &templatePath);

private:
    RwCommError     BuildProperties(RwXMLNodePtr rootNode, RwExpMgrTemplateProperty *rootProperty);
    RwCommError     BuildRwXMLNodes(RwExpMgrTemplateProperty *rootProperty, RwXMLNode *rootNode);
    RwCommError     GetPropertiesOptions(RwExpMgrTemplateProperty *rootProperty,
                                         RwCommExportOptions &options);
    RwCommError     UpdateLegacyProperties(RwExpMgrTemplateProperty *property, int templateVersion, bool bReport);
    RwCommError     GetPropertyLocations(RwExpMgrTemplateProperty *property, RwVector<RwWString> &locations);
    RwCommError     GetPropertyPlatforms(RwExpMgrTemplateProperty *property, RwVector<RwWString> &platforms);
    RwWString    GetPropertyAssetType(RwExpMgrTemplateProperty *properties);
    RwCommError     ConvertProperties(RwExpMgrTemplateProperty *property, int templateVersion, bool bReport);
    RwCommError     RemoveRetiredProperties(RwExpMgrTemplateProperty *property,
                        RwExpMgrTemplateProperty *defaultsProperty, bool bReport);
    RwCommError     GetAllRetiredProperties(RwExpMgrTemplateProperty *property, RwSet<RwWString> &sNames);
    RwCommError     AddNewProperties(RwExpMgrTemplateProperty *property,
                        RwExpMgrTemplateProperty *defaultsProperty, int templateVersion, bool bReport);
    RwCommError     GetAllNewerProperties(RwExpMgrTemplateProperty *property, int templateVersion,
                        RwVector< RwPair<RwExpMgrTemplateProperty *, RwExpMgrTemplateProperty *> > &vProperties);
    void            DeletePropertiesByName(RwExpMgrTemplateProperty *rootProperty, const RwSet<RwWString> &sNames,
                                           bool bReport);

    void            FilterProperties(RwExpMgrTemplateProperty *rootProperty, const RwVector<RwWString> &sLocations);
    void            FilterPropertiesFromOptions(RwExpMgrTemplateProperty *rootProperty, const RwWString &sLocation,
                        RwExpMgrTemplateProperty *pTargetProp, const RwCommExportOptions &options, RwSet<RwWString> &sOptions); 
    RwExpMgrTemplateProperty*   FindPropertyByName(RwExpMgrTemplateProperty *rootProperty, const RwWString &name);
    bool            IsGroup(const RwExpMgrTemplateProperty *property);

    RwCommError     CreatePropertiesFromOptions(RwExpMgrTemplateProperty *props, RwExpMgrTemplateType type,
                            const RwCommExportOptions &options);
    void            SetPropertyFromOptions(RwExpMgrTemplateProperty *props, const RwCommExportOptions &options);
    bool            CompareProperties(RwExpMgrTemplateProperty *props1, RwExpMgrTemplateProperty *props2);
    void            SetPropertyTemplateType(RwExpMgrTemplateType type, RwExpMgrTemplateProperty *prop);
    void            RemoveOutputSection(RwExpMgrTemplateProperty *props);
    RwWString    GetTemplateDescription(const RwWString &sLocation);

    RwUtilComponentConfiguration m_currentConfiguration;
    ExpMgrTemplateScopeType m_scope;
};

#endif // RWEXPMGRTEMPLATEMANAGER_H
