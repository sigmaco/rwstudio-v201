// CommFurShader.h: interface for the CCommFurShader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMFURSHADER_H__E82CEDD9_670C_4723_9A16_CFF7A545AE47__INCLUDED_)
#define AFX_COMMFURSHADER_H__E82CEDD9_670C_4723_9A16_CFF7A545AE47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwCommTexture.h"


const RwCommType FurShaderType("FurShader");

class CCommFurShader  
{
public:

    // Create a new fur texture type, so that we can extract the fur texture using 
    // RwCommShader::GetTexturesByType()
    enum { FurTextureType = 10 };

    CCommFurShader() {}
    virtual ~CCommFurShader() {}

    // Get the fur color
    virtual RwCommError GetFurColor(RwCommVec3 &color) const = 0;

};

#endif // !defined(AFX_COMMFURSHADER_H__E82CEDD9_670C_4723_9A16_CFF7A545AE47__INCLUDED_)
