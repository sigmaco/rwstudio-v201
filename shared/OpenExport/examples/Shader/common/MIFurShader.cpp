// MIFurShader.cpp: implementation of the MIFurShader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MIFurShader.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MIFurShader::MIFurShader()
{

}

MIFurShader::~MIFurShader()
{

}

void
MIFurShader::GetTypeHierarchy(RwVector<RwCommType> &vTypeVec) const
{
    vTypeVec.push_back(FurShaderType);
    RwCommAnimatable::GetTypeHierarchy(vTypeVec);
}

RwCommError 
MIFurShader::GetFurColor(RwCommVec3 &color) const
{
    color = m_furColor;
    return RWCOMM_SUCCESS;
}
