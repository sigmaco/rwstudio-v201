// MIFurShader.h: interface for the MIFurShader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MIFURSHADER_H__93226639_6400_4782_A55D_5D1F8F5CD04D__INCLUDED_)
#define AFX_MIFURSHADER_H__93226639_6400_4782_A55D_5D1F8F5CD04D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CommFurShader.h"
#include "RwMIShader.h"

class MIFurShader  : public RwMIShader, public CCommFurShader
{
public:
	MIFurShader();
	virtual ~MIFurShader();

    // RwCommObject methods
    virtual RwCommType GetType() { return FurShaderType; }
    void    GetTypeHierarchy(RwVector<RwCommType> &vTypeVec) const;

    // CCommFurShader methods
    virtual RwCommError GetFurColor(RwCommVec3 &color) const;

    // MIFurShader methods
    void    SetFurColor(const RwCommVec3 &color) { m_furColor = color; }

private:

    RwCommVec3  m_furColor;

};

#endif // !defined(AFX_MIFURSHADER_H__93226639_6400_4782_A55D_5D1F8F5CD04D__INCLUDED_)
