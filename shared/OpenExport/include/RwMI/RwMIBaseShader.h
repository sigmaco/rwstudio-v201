// RwMIBaseShader.h: interface for the RwMIBaseShader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWMIBASESHADER_H__D3A94DFA_C3C4_4426_84CC_7BBD3ECCC15C__INCLUDED_)
#define AFX_RWMIBASESHADER_H__D3A94DFA_C3C4_4426_84CC_7BBD3ECCC15C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwMI.h"
#include "RwCommShader.h"
#include "RwMIAnimProp.h"


struct RwMITexGroup
{
    RwMITexGroup() : m_factor(0.0) {}

    RwVector<RwCommTexture*>                      m_vTextures;
    RwVector<RwCommShader::RwCommBlendFunction>   m_vBlendFuncs;
    RwCommReal                                        m_factor;
};

/**
*   \ingroup miscenegraph
*
*   This is the base class for all MI shaders. The class caches material and texture information.
*
*/
class RWMI_API RwMIBaseShader
{
public:
    RwMIBaseShader();
    virtual ~RwMIBaseShader();

    // Texture methods
    virtual RwCommError MapUVs(RwCommGeometry *pGeometry, RwVector<RwCommTexture *> &vActiveTextures)
                              { return RWCOMM_NOT_IMPLEMENTED; }
    virtual RwCommError GetTextureUVSet(RwCommTexture *pTexture, int &uvSet) const;
    virtual RwCommError GetTexturesByType(RwCommShader::TextureType type,
                                          RwVector<RwCommTexture *> &vTextures) const;
    virtual RwCommError GetTextureFactor(RwCommShader::TextureType type, RwCommReal &factor) const;
    virtual RwCommError GetBlendFunctions(RwCommShader::TextureType type,
                                          RwVector<RwCommShader::RwCommBlendFunction> &vBlendFuncs) const;

    //Material attrributes
    virtual RwCommError GetAmbientValue(RwCommReal frame, RwCommVec3&) const;
    virtual RwCommError GetSpecularValue(RwCommReal frame, RwCommVec3&) const;
    virtual RwCommError GetDiffuseValue(RwCommReal frame, RwCommVec3&) const;
    virtual RwCommError GetShininess(RwCommReal frame, RwCommReal&) const;
    virtual RwCommError GetShineStrength(RwCommReal frame, RwCommReal&) const;
    virtual RwCommError GetTransparency(RwCommReal frame, RwCommReal&) const;
    virtual RwCommError GetSelfIllumination(RwCommReal frame, RwCommVec3&) const;
    virtual RwCommError IsDoubleSided(bool &bStat) const;

    // RwMIBaseShader methods
    RwCommError SetTextureUVSet(RwCommTexture *pTexture, int uvSet);
    RwCommError SetTextureInfo(RwCommTexture *pTexture, RwCommShader::TextureType type,
                               RwCommShader::RwCommBlendFunction bf = RwCommShader::BLENDNABLEND);
    RwCommError SetTextureTypeFactor(RwCommShader::TextureType type, RwCommReal factor);

    RwCommError SetAmbientValue(RwCommReal frame, const RwCommVec3 &v);
    RwCommError SetAmbientValue(RwVector< RwMIAnimPropKey<RwCommVec3> > &vKeys);

    RwCommError SetSpecularValue(RwCommReal frame, const RwCommVec3 &v);
    RwCommError SetSpecularValue(RwVector< RwMIAnimPropKey<RwCommVec3> > &vKeys);

    RwCommError SetDiffuseValue(RwCommReal frame, const RwCommVec3 &v);
    RwCommError SetDiffuseValue(RwVector< RwMIAnimPropKey<RwCommVec3> > &vKeys);

    RwCommError SetShininess(RwCommReal frame, RwCommReal s);
    RwCommError SetShininess(RwVector< RwMIAnimPropKey<RwCommReal> > &vKeys);

    RwCommError SetShineStrength(RwCommReal frame, RwCommReal s);
    RwCommError SetShineStrength(RwVector< RwMIAnimPropKey<RwCommReal> > &vKeys);

    RwCommError SetTransparency(RwCommReal frame, RwCommReal t);
    RwCommError SetTransparency(RwVector< RwMIAnimPropKey<RwCommReal> > &vKeys);

    RwCommError SetSelfIllumination(RwCommReal frame, const RwCommVec3 &v);
    RwCommError SetSelfIllumination(RwVector< RwMIAnimPropKey<RwCommVec3> > &vKeys);

    RwCommError SetDoubleSided(bool b);

protected:
    RwMIAnimProp<RwCommVec3>    m_ambient;
    RwMIAnimProp<RwCommVec3>    m_specular;
    RwMIAnimProp<RwCommVec3>    m_diffuse;
    RwMIAnimProp<RwCommReal>    m_shininess;
    RwMIAnimProp<RwCommReal>    m_shineStrength;
    RwMIAnimProp<RwCommReal>    m_transparency;
    RwMIAnimProp<RwCommVec3>    m_selfIllum;
    bool                        m_bIsDoubleSided;

private:
    RwMap<RwCommTexture *, int>   m_mUvSets;
    RwMap<RwCommShader::TextureType, RwMITexGroup>  m_mTexTypes;

};

#endif // !defined(AFX_RWMIBASESHADER_H__D3A94DFA_C3C4_4426_84CC_7BBD3ECCC15C__INCLUDED_)
