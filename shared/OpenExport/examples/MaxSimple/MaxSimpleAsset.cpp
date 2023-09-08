// MaxSimpleAsset.cpp: implementation of the MaxSimpleAsset class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MaxSimpleAsset.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MaxSimpleAsset::MaxSimpleAsset()
{

}

MaxSimpleAsset::~MaxSimpleAsset()
{

}

RwCommError 
MaxSimpleAsset::SetName(const RwWString &name)
{
    m_name = name;
    return RWCOMM_SUCCESS;
}

RwCommError 
MaxSimpleAsset::GetName(RwWString &name) const
{
    name = m_name;
    return RWCOMM_SUCCESS;
}

RwCommError 
MaxSimpleAsset::GetEnable(bool &bEnable) const
{
    bEnable = true;
    return RWCOMM_SUCCESS;
}

RwCommError 
MaxSimpleAsset::GetAssetScope(AssetScope &scope) const
{
    scope = ASSET;
    return RWCOMM_SUCCESS;
}

RwCommError 
MaxSimpleAsset::SetAssetTemplateName(const RwWString &templateName)
{
    m_templateName = templateName;
    return RWCOMM_SUCCESS;
}

RwCommError 
MaxSimpleAsset::GetAssetTemplateName(RwWString &templateName) const
{
    templateName = m_templateName;
    return RWCOMM_SUCCESS;
}
