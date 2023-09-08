// RwMaxMaterial.h: interface for the RwMaxMaterial class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWMAXMATERIAL_H__AB336BB0_B1A7_4117_B9E2_BD9D24A26680__INCLUDED_)
#define AFX_RWMAXMATERIAL_H__AB336BB0_B1A7_4117_B9E2_BD9D24A26680__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"

#include "RwCommShader.h"
#include "RwCommObjCreator.h"

class MaxSimpleShader  : public RwCommShader
{
public:
    MaxSimpleShader(Mtl* pMtl);
    virtual ~MaxSimpleShader();

    // Destroy the object - called by RwCommObject::Release()
    virtual void Destroy() { delete this; }

    virtual RwCommError GetUserData(RwCommUserDataCollection &userDataColl) { return RWCOMM_NOT_IMPLEMENTED; }

    // Get attribute methods.
    virtual RwCommError GetAttribute(const RwString& name, int &value, int index = 0) { return RWCOMM_NOT_IMPLEMENTED; }
    virtual RwCommError GetAttribute(const RwString& name, float &value, int index = 0) { return RWCOMM_NOT_IMPLEMENTED; }
    virtual RwCommError GetAttribute(const RwString& name, RwString &value, int index = 0) { return RWCOMM_NOT_IMPLEMENTED; }
    virtual RwCommError GetAttributeNames(RwVector<RwString>& vAttributeNames, RwVector<AttributeType> &vAttributeTypes) { return RWCOMM_NOT_IMPLEMENTED; }

    // Tetxure methods
    virtual RwCommError MapUVs(RwCommGeometry* pGeometry, RwVector<RwCommTexture*>& vActiveTextures) { return RWCOMM_NOT_IMPLEMENTED; }
    virtual RwCommError GetTextureUVSet(RwCommTexture *pTextures, int& uvSet) const { return RWCOMM_NOT_IMPLEMENTED; }
    virtual RwCommError GetTexturesByType(TextureType type, RwVector<RwCommTexture*>& vTextures) const { return RWCOMM_NOT_IMPLEMENTED; }
    virtual RwCommError GetTextureFactor(TextureType type, RwCommReal& factor) const { return RWCOMM_NOT_IMPLEMENTED; }
    virtual RwCommError GetBlendFunctions(TextureType type, RwVector<RwCommBlendFunction>& vBlendFuncs) const { return RWCOMM_NOT_IMPLEMENTED; }

    //Material attrributes
    virtual RwCommError GetAmbientValue(RwCommVec3&) const;
    virtual RwCommError GetSpecularValue(RwCommVec3&) const;
    virtual RwCommError GetDiffuseValue(RwCommVec3&) const;
    virtual RwCommError GetShininess(RwCommReal&) const;
    virtual RwCommError GetShineStrength(RwCommReal&) const;
    virtual RwCommError GetTransparency(RwCommReal&) const;
    virtual RwCommError GetSelfIllumination(RwCommVec3&) const;
    virtual RwCommError IsDoubleSided(bool& bStat) const;

    /** Get the Mtl object.*/
    Mtl                 *GetMaterial() const                    {return m_pMtl;}

protected:

    RwCommVec3          ColorToRwCommVec3(const Color& c) const;

    
protected:
    Mtl         *m_pMtl;
};


class MaxSimpleShaderCreator  : public RwCommObjCreator
{
public:
    MaxSimpleShaderCreator();
    virtual ~MaxSimpleShaderCreator();

    virtual RwCommObject* Create(const RwCommCreationData* pCreationData,
                                 const RwCommExportOptions* pExportOptions) const;
    virtual void Init(  RwCommObject* pObject,
                        const RwCommCreationData* pCreationData,
                        const RwCommExportOptions* pExportOptions) const;

	virtual void Attach(RwCommObject* pObject, RwCommObject* pParent, RwCommScene* pScene,
                        const RwCommCreationData* pCreationData) const;
};

#endif // !defined(AFX_RWMAXMATERIAL_H__AB336BB0_B1A7_4117_B9E2_BD9D24A26680__INCLUDED_)
