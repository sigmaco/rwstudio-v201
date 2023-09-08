// Rf3FurShader.h: interface for the Rf3FurShader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RF3FURSHADER_H__ADD11CCE_ACC6_4961_B88B_6F74E6DF91DC__INCLUDED_)
#define AFX_RF3FURSHADER_H__ADD11CCE_ACC6_4961_B88B_6F74E6DF91DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwRf3Shader.h"

class Rf3ExpFurShader  : public RwRf3ExpShader 
{
public:
	Rf3ExpFurShader();
	virtual ~Rf3ExpFurShader();

    virtual RwCommError     Export( RwCommObject* pObject,
                                RwCommObject* pParent,
                                RwCommScene* pScene,
                                const RwCommExportOptions* pExportOptions );

};

class RwRf3FurShaderCreator : public RwRf3ShaderCreator
{
public:
    RwRf3FurShaderCreator() {}
    virtual ~RwRf3FurShaderCreator() {}

	virtual RwCommObject* Create(const RwCommCreationData* pCreationData,
								const RwCommExportOptions* pExportOptions) const;

protected:

    virtual void BuildShaderFunc(RwXMLNode *pNode, RwCommShader *pShader, RwRf3Builder* pBuilder) const;
    virtual void BuildChannel(RwXMLNode *pNode, RwCommShader *pShader, RwRf3Builder* pBuilder) const;
};

#endif // !defined(AFX_RF3FURSHADER_H__ADD11CCE_ACC6_4961_B88B_6F74E6DF91DC__INCLUDED_)
