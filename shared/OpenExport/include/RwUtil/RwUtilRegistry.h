#ifndef RWUTIL_REGISTRY_H
#define RWUTIL_REGISTRY_H

#include "RwUtil.h"
#include "RwUtilTypes.h"

/**
*   Returns the top level registry path for the installed version of
*   RenderWare. This is always relative to HKEY_CURRENT_USER\Software.
*
*   \ingroup rwutilregistry
*
*   \param ComponentName the component whose registry key path is desired
*
*   \returns the top level registry path
*/
RWUTIL_API RwWString RwUtilRegGetCurrentKeyPath(const RwWString &componentName);

/**
*   Set a registry value relative to path returned by RwUtilRegGetCurrentKeyPath().
*
*   If entry is an empty string std::invalid_argument exception is thrown.
*
*   It is preferable to use the \ref utilconfigoverview classes rather than direct registry access 
*   if generalised configuration access is required.
*
*   \ingroup rwutilregistry
*
*   \param application an optional application name. If not empty the function will create a key of this name.
*   \param section and optional section name. If not empty the function will create a key of this name.
*   \param entry the name of the string value to be set
*   \param value the value to be set
*   \param ComponentName the component whose whose registry key is to be edited
*
*/
RWUTIL_API void RwUtilRegSetString (const RwWString &application,
                                    const RwWString &section, 
                                    const RwWString &entry,
                                    const RwWString &value,
                                    const RwWString &componentName);

/**
*   Retrieve a registry value relative to path returned by RwUtilRegGetCurrentKeyPath().
*   If the key requested does not exist it is also checked for in the default locations
*   of previous RenderWare installations. If it does not exist in any, the key is created
*   in the current location and set to defaultVal.
*
*   If entry is an empty string std::invalid_argument exception is thrown.
*
*   It is preferable to use the  \ref utilconfigoverview classes rather than direct registry access 
*   if generalised configuration access is required.
*
*   \ingroup rwutilregistry
*
*   \param application an optional application name. If not empty the function will look for a key of this name.
*   \param section and optional section name. If not empty the function will look for a key of this name.
*   \param entry the name of the string value to be retrieved
*   \param defaultVal the default value to be returned if the value does not exist
*   \param ComponentName the component whose whose registry key is to be retrieved
*
*   \returns the registry value found, defaultVal if the value was not present
*/

RWUTIL_API const RwWString RwUtilRegGetString (const RwWString &application,
                                              const RwWString &section, 
                                              const RwWString &entry,
                                              const RwWString &defaultVal,
                                              const RwWString &componentName);

#endif // RWUTIL_REGISTRY_H
