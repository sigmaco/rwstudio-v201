// RwUtilConfig.h
// This file containers helper classes for retrieving an application's 
// configuration information

#pragma once
#include "RwUtil.h"
#include "RwUtilTypes.h"
#include "RwUtilManagedObj.h"
#include "RwUtilPtr.h"

/**
*  \ingroup rwutilconfig
*  \page utilconfigoverview RwUtilConfig
*
*  These classes wrap up configuration setting access and modification.
*
*  The classes should not be created directly, rather an RwUtilComponentConfiguration should be obtained
*  from the relevant function ( RwOpenExportConfig(), RwGraphicsConfig() or RwAudioConfig() ) and
*  then the relevant section retrieved and edited:
*
\code
RwWString MyData = RwOpenExportConfig().GetSection("My Section").GetEntry("MyData");
RwOpenExportConfig().GetSection("My Section").SetEntry("MyData", "Hello");
\endcode
*
*  Sub sections can be accessed:
*
\code
RwWString MyData = RwOpenExportConfig().GetSection("My Section").GetSubSection("My Sub Section").GetEntry("MyData");
RwOpenExportConfig().GetSection("My Section").GetSubSection("My Sub Section").SetEntry("MyData", "Hello");
\endcode
*
*  And so can the root section of a component's configuration:
*
\code
RwWString MyData = RwOpenExportConfig().GetRootSection().GetEntry("MyData");
RwOpenExportConfig().GetRootSection().SetEntry("MyData", "Hello");
\endcode
*
*  If editing lots of data on a particular section it may be helpful to store the relevant section in a local variable:
*
\code
RwUtilConfigurationSection MySection = RwOpenExportConfig().GetSection("My Section").GetSubSection("My Sub Section");
RwWString MyData1 = MySection.GetEntry("MyData1");
RwWString MyData2 = MySection.GetEntry("MyData2");
RwWString MyData3 = MySection.GetEntry("MyData3");
RwWString MyData4 = MySection.GetEntry("MyData4");
MySection.SetEntry("MyData5", "Hello1");
MySection.SetEntry("MyData6", "Hello2");
\endcode
*
*  Sub sections may contain further sub sections up to the limits of the underlying implementation of the configuration 
*  data storage.
*
*/

#if (! defined(DOXYGEN))
/**
*   This interface hides the implementation details of any configuration system from the users of that
*   configuration system.
*/
class RwUtilConfigurationSectionImpl : public RwUtilManagedObj
{
    public:

    virtual RwUtilConfigurationSectionImpl *GetSubSection(const RwWString &SubSectionName) = 0;
    virtual const RwWString GetEntry(const RwWString &EntryName, const RwWString &DefaultValue = L"") = 0;
    virtual void SetEntry(const RwWString &EntryName, const RwWString &Value) = 0;
};

/**
*   This interface hides the implementation details of any configuration system from the users of that
*   configuration system
*/

class RwUtilComponentConfigurationImpl : public RwUtilManagedObj
{
    public:

    virtual RwUtilConfigurationSectionImpl *GetSection(const RwWString &SectionName) = 0;
    virtual RwUtilConfigurationSectionImpl *GetRootSection() = 0;

};
#endif //(! defined(DOXYGEN))
/**
*  \ingroup rwutilconfig
*   This is a configuration section object that allows you to access values and sub sections within
*   this configuration section
*/
class RwUtilConfigurationSection
{
#if (! defined(DOXYGEN))
    RwUtilRCAutoPtr<RwUtilConfigurationSectionImpl> m_pImpl;
#endif //(! defined(DOXYGEN))

    public:

    RwUtilConfigurationSection(RwUtilConfigurationSectionImpl *Implementation)
        : m_pImpl(Implementation)
    {}


    /**
    *   This function gets a sub section within this configuration section
    *   \param SubSectionName the sub section to be retrieved
    *   \returns the relevant section
    */
    const RwUtilConfigurationSection GetSubSection(const RwWString &SubSectionName) const
    {
        return RwUtilConfigurationSection(m_pImpl->GetSubSection(SubSectionName));
    }

    /**
    *   This function gets an entry in this configuration section
    *   \param EntryName the entry to be retrieved
    *   \param DefaultValue a default value to return if the entry isn't there
    *   \returns the configuration entry
    */
    const RwWString GetEntry(const RwWString &EntryName, const RwWString &DefaultValue = L"") const
    {
        return m_pImpl->GetEntry(EntryName, DefaultValue);
    }

    /**
    *   This function sets an entry in this configuration section
    *   \param EntryName the entry to be retrieved
    *   \param Value the value to set the entry to
    */
    void  SetEntry(const RwWString &EntryName, const RwWString &Value) const
    {
        m_pImpl->SetEntry(EntryName, Value);
    }
};

/**
*  \ingroup rwutilconfig
*   This is a component configuration object that allows you to access sections of the component's
*   configuration data
*/
class RwUtilComponentConfiguration
{
#if (! defined(DOXYGEN))
    RwUtilRCAutoPtr<RwUtilComponentConfigurationImpl> m_pImpl;
#endif //(! defined(DOXYGEN))

    public:

    RwUtilComponentConfiguration(RwUtilComponentConfigurationImpl *Implementation)
        : m_pImpl(Implementation)
    {}

    /**
    *   This function gets a section of this component's configuration data
    *   \param SectionName the section to be retrieved
    *   \returns the relevant section
    */
    const RwUtilConfigurationSection GetSection(const RwWString &SectionName) const
    {
        return RwUtilConfigurationSection(m_pImpl->GetSection(SectionName));
    };

    /**
    *   This function gets the root section of this component's configuration data
    *   \returns the relevant section
    */
    const RwUtilConfigurationSection GetRootSection() const
    {
        return RwUtilConfigurationSection(m_pImpl->GetRootSection());
    };

};

// These function will get the relevant configuration object for a component.
// These functions are the _only_ way to get a component's configuration
/**
*  \ingroup rwutilconfig
*   This function returns the configuration object for the RenderWare Open Export component
*/
RWUTIL_API const RwUtilComponentConfiguration RwOpenExportConfig();
/**
*  \ingroup rwutilconfig
*   This function returns the configuration object for the RenderWare Graphics component
*/
RWUTIL_API const RwUtilComponentConfiguration RwGraphicsConfig();
/**
*  \ingroup rwutilconfig
*   This function returns the configuration object for the RenderWare audio component
*/
RWUTIL_API const RwUtilComponentConfiguration RwAudioConfig();

/**
*  \ingroup rwutilconfig
*   This function returns the installation state for the RenderWare Open Export component
*/
RWUTIL_API const bool RwIsOpenExportInstalled();
/**
*  \ingroup rwutilconfig
*   This function returns the installation state for the RenderWare Graphics component
*/
RWUTIL_API const bool RwIsGraphicsInstalled();
/**
*  \ingroup rwutilconfig
*   This function returns the installation state for the RenderWare audio component
*/
RWUTIL_API const bool RwIsAudioInstalled();
