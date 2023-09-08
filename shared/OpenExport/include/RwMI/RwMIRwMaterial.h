// RwMIRwMaterial.h: interface for the RwMIRwMaterial class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWMIRWMATERIAL_H__680B337C_5AEA_4988_9ACB_26B643DC5BD5__INCLUDED_)
#define AFX_RWMIRWMATERIAL_H__680B337C_5AEA_4988_9ACB_26B643DC5BD5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwMI.h"
#include "RwMIAttributes.h"
#include "RwMIBaseShader.h"

#include "RwCommRwMaterial.h"
/**
*   \ingroup miscenegraph
*
*   This is the MI implementation of RwCommRwMaterial.
*
*   \see RwCommRwMaterial
*/
class RWMI_API RwMIRwMaterial : public RwCommRwMaterial, public RwMIBaseShader, public RwMIAttributes
{
public:
    RwMIRwMaterial();
    virtual ~RwMIRwMaterial();

    virtual RwCommError GetUserData(RwCommUserDataCollection &userDataColl);

    // Get attribute methods.
    virtual RwCommError GetAttribute(const RwWString &name, int &value, int index = 0);
    virtual RwCommError GetAttribute(const RwWString &name, float &value, int index = 0);
    virtual RwCommError GetAttribute(const RwWString &name, RwWString &value, int index = 0);
    virtual RwCommError GetAttributeNames(RwVector<RwWString> &vAttributeNames,
                                          RwVector<AttributeType> &vAttributeTypes);

    // Texture methods
    virtual RwCommError MapUVs(RwCommGeometry *pGeometry, RwVector<RwCommTexture *> &vActiveTextures)
                              { return RWCOMM_NOT_IMPLEMENTED; }
    virtual RwCommError GetTextureUVSet(RwCommTexture *pTexture, int &uvSet) const;
    virtual RwCommError GetTexturesByType(TextureType type, RwVector<RwCommTexture *> &vTextures) const;
    virtual RwCommError GetTextureFactor(TextureType type, RwCommReal &factor) const;
    virtual RwCommError GetBlendFunctions(TextureType type,
                                          RwVector<RwCommBlendFunction> &vBlendFuncs) const;

    //Material attrributes
    virtual RwCommError GetAmbientValue(RwCommVec3 &) const;
    virtual RwCommError GetSpecularValue(RwCommVec3 &) const;
    virtual RwCommError GetDiffuseValue(RwCommVec3 &) const;
    virtual RwCommError GetShininess(RwCommReal &) const;
    virtual RwCommError GetShineStrength(RwCommReal &) const;
    virtual RwCommError GetTransparency(RwCommReal &) const;
    virtual RwCommError GetSelfIllumination(RwCommVec3 &) const;
    virtual RwCommError IsDoubleSided(bool &bStat) const;

    // RwCommRwMaterial methods

    // Default material
    virtual RwCommError GetBaseColor(RwCommVec4 &color) const;
    virtual RwCommError GetSurfaceProperties(RwCommSurfaceProps &sProps) const;

    // Basic matfx extension.
    virtual RwCommError GetMatFXEffect(RwCommMatFXMaterialFlags &effect) const;

    // Xbox and GameCube extension
    virtual RwCommError IsExtensionEnabled(ExtensionType type, bool &bEnabled) const;
    virtual RwCommError GetXBoxEffectFile(RwWString &filename) const;
    virtual RwCommError GetGameCubeEffectFile(RwWString &filename) const;
    virtual RwCommError GetXboxTexture(RwVector<RwCommTexture *> &vTextures);
    virtual RwCommError GetGameCubeTexture(RwVector<RwCommTexture *> &vTextures);
    
    // Toon extension
    virtual RwCommError GetToonPaintName(RwWString &paintname) const;

    // Light map extension
    virtual RwCommError GetLightMapFlags(RwCommLtMapMaterialFlags &flags) const;
    virtual RwCommError GetLightMapDensity(RwCommReal &density) const;
    virtual RwCommError GetLightMapAreaLightColor(RwCommVec4 &color) const;
    virtual RwCommError GetLightMapAreaLightDensity(RwCommReal &density) const;
    virtual RwCommError GetLightMapAreaLightRadius(RwCommReal &radius) const;

    // Normal map extension
    virtual RwCommError GetNormalMapTexture(RwVector<RwCommTexture *> &vTextures) const;
    virtual RwCommError GetNormalMapEnvFactor(RwCommReal &envFactor) const;
    virtual RwCommError GetNormalMapEnvModulate(bool &bModulate) const;
    virtual RwCommError GetNormalMapConvertAlpha(bool &bConvert) const;
    virtual RwCommError GetNormalMapBumpFactor(RwCommReal &bumpFactor) const;

    // RwMIRwMaterila methods
    RwCommError SetBaseColor(const RwCommVec4 &color);
    RwCommError SetSurfaceProperties(const RwCommSurfaceProps &sProps);

    RwCommError SetMatFXEffect(const RwCommMatFXMaterialFlags effect);

    RwCommError EnableExtension(ExtensionType type);
    RwCommError SetXBoxEffectFile(const RwWString &filename);
    RwCommError SetGameCubeEffectFile(const RwWString &filename);
    RwCommError SetXboxTextures(const RwVector<RwCommTexture *> &vTextures);
    RwCommError AddXboxTexture(RwCommTexture *pTexture);
    RwCommError SetGameCubeTextures(RwVector<RwCommTexture *> &vTextures);
    RwCommError AddGameCubeTexture(RwCommTexture *pTexture);


    RwCommError SetToonPaintName(RwWString &paintname);

    // Light map extension
    RwCommError SetLightMapFlags(RwCommLtMapMaterialFlags flags);
    RwCommError SetLightMapDensity(RwCommReal density);
    RwCommError SetLightMapAreaLightColor(const RwCommVec4 &color);
    RwCommError SetLightMapAreaLightDensity(RwCommReal density);
    RwCommError SetLightMapAreaLightRadius(RwCommReal radius);

    // Normal map extension
    RwCommError SetNormalMapTexture(RwVector<RwCommTexture *> &vTextures);
    RwCommError SetNormalMapEnvFactor(RwCommReal envFactor);
    RwCommError SetNormalMapEnvModulate(bool bModulate);
    RwCommError SetNormalMapConvertAlpha(bool bConvert);
    RwCommError SetNormalMapBumpFactor(RwCommReal bumpFactor);

protected:
    RwCommVec4                  m_baseColor;
    RwCommSurfaceProps          m_surfaceProps;
    RwCommMatFXMaterialFlags    m_matFxFlags;
    RwVector<bool>          m_vExtensions;
    RwWString                m_XBoxEffectFile;
    RwWString                m_GCEffectFile;

    RwVector<RwCommTexture *>     m_vXBoxTextures;
    RwVector<RwCommTexture *>     m_vGCTextures;
    RwVector<RwCommTexture *>     m_vNormalMapTextures;

    RwWString                m_toonPaint;

    RwCommLtMapMaterialFlags    m_LMFlags;
    RwCommReal                  m_LMDensity;
    RwCommVec4                  m_LMAreaColor;
    RwCommReal                  m_LMAreaLightDensity;
    RwCommReal                  m_LMAreaLightRadius;

    RwCommReal                  m_NMEnvFactor;
    bool                        m_NMEnvModulate;
    bool                        m_NMConvertAlpha;
    RwCommReal                  m_NMBumpFactor;
};

#endif // !defined(AFX_RWMIRWMATERIAL_H__680B337C_5AEA_4988_9ACB_26B643DC5BD5__INCLUDED_)
