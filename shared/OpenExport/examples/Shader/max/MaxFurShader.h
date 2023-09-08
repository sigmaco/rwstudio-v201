//MaxShader.h

#ifndef _MAX_FUR_SHADER_H
#define _MAX_FUR_SHADER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"

#include "RwCommShader.h"
#include "RwMaxShader.h"
#include "RwMaxCreationData.h"

#include "CommFurShader.h"


class MaxFurShader : public RwMaxShader, public CCommFurShader
{
public:
    MaxFurShader(Mtl *pMtl);
    virtual ~MaxFurShader();

    // RwCommObject methods
    virtual RwCommType  GetType() { return FurShaderType; }
    void    GetTypeHierarchy(RwVector<RwCommType> &vTypeVec) const;

    // RwCommShader methods
    virtual RwCommError GetTexturesByType(TextureType type, RwVector<RwCommTexture *> & vTextures) const;

    virtual RwCommError GetSelfIllumination(RwCommVec3 &illum)  const { return RWCOMM_NOT_IMPLEMENTED; };
    virtual RwCommError GetAmbientValue(RwCommVec3 &)           const { return RWCOMM_NOT_IMPLEMENTED; };
    virtual RwCommError GetSpecularValue(RwCommVec3 &)          const { return RWCOMM_NOT_IMPLEMENTED; };
    virtual RwCommError GetShininess(RwCommReal &)              const { return RWCOMM_NOT_IMPLEMENTED; };
    virtual RwCommError GetShineStrength(RwCommReal &)          const { return RWCOMM_NOT_IMPLEMENTED; };
    virtual RwCommError IsDoubleSided(bool &bStat)              const { return RWCOMM_NOT_IMPLEMENTED; };
    virtual RwCommError GetDiffuseValue(RwCommVec3 &color)      const { return RWCOMM_NOT_IMPLEMENTED; };
    virtual RwCommError GetTransparency(RwCommReal &trans)      const { return RWCOMM_NOT_IMPLEMENTED; };

    // CCommFurShader methods
    virtual RwCommError GetFurColor(RwCommVec3 &color) const;

};

class MaxFurShaderCreator  : public RwCommObjCreator
{
public:
    MaxFurShaderCreator();
    virtual ~MaxFurShaderCreator();

    virtual RwCommObject *Create(const RwCommCreationData *pCreationData,
                                 const RwCommExportOptions *pExportOptions) const;
    virtual void Init(  RwCommObject *pObject,
                        const RwCommCreationData *pCreationData,
                        const RwCommExportOptions *pExportOptions) const;

    virtual void Attach(RwCommObject *pObject, RwCommObject *pParent, RwCommScene *pScene,
                        const RwCommCreationData *pCreationData) const;
};



#endif // !defined(AFX_RWMAXMATERIAL_H__AB336BB0_B1A7_4117_B9E2_BD9D24A26680__INCLUDED_)
