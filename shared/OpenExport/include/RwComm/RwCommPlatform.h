// RwCommPlatform.h: valid platforms
//
////////////////////////////////////

#pragma once

#include "RwCommError.h"

/**
*  \ingroup commlowlevelobjects
*   This enumeration is used to list the valid plaforms.
*/
enum RwCommPlatform
{
	RwCommPlatformDefault,
	RwCommPlatformGeneric,
	RwCommPlatformGameCube,
	RwCommPlatformPS2,
	RwCommPlatformXBox,
	// add new platforms here
	RwCommPlatformCount
};


/** 
*   
*
*   \ingroup commhelpers
*
*   This class describes a multi platform data item which can have a different values per platform. 
*   It will maintain  a list of which platforms are active and and which are not.
*/
template<class DataType>
class RwCommPlatformDataItem
{
public:

	RwCommPlatformDataItem()
	{
        for (int i = 0; i < RwCommPlatformCount; i++)
        {
            // deactivate all platforms
            m_platformBin[i] = false;
        }
    }

    /**
    *   Set the data item for a given platform.
    *   \param platform the platform to active and set the data for
    *   \param data the data to set for the given platform        
    */
    void SetData(const RwCommPlatform &platform, const DataType data)
	{
		m_platformBin[platform] = true;
		m_platformData[platform] = data;
	}

    /**
    *   Get the data for a given platform if the platform is active.
    *   \param platform the platform to query
    *   \param data the returned data for the given platform. Only set it the platform is active
    *   \param useDefault if this flag is set to true and the default platform is active and the 
    *          given platform is not active the default platform date will be returned instead.  
               This flag is set to true by default.
    *
    *   \return RWCOMM_SUCCESS if the data is found, RWCOMM_OBJECT_NOT_FOUND if the data cannot be found
    */
    RwCommError GetData(const RwCommPlatform &platform, DataType &data, bool useDefault = true) const
    {
        if (m_platformBin[platform])
        {
            data = m_platformData[platform];
            return RWCOMM_SUCCESS;
        }
        else
        {
            if (useDefault && m_platformBin[RwCommPlatformDefault])
            {
                data = m_platformData[RwCommPlatformDefault];
                return RWCOMM_SUCCESS;
            }
        }
        
        return RWCOMM_OBJECT_NOT_FOUND;
    }

    /**
    *   Set the status of a platform.
    *   \param platform the platform to activate or deactivate
    *   \param isActive the new status for the platform. This is true by default
    */
    void ActivatePlatform(const RwCommPlatform &platform, bool isActive = true)
    {
        m_platformBin[platform] = isActive;
    }

private:
    DataType    m_platformData[RwCommPlatformCount];
    bool        m_platformBin[RwCommPlatformCount];
};