#ifndef RW_MAYA_SHADER_H
#define RW_MAYA_SHADER_H

#include "RwCommPrimitives.h"

//  Our Node Class.
class RWMAYA_API RwShaderNode : public MPxNode
{
public:
                        RwShaderNode();
    virtual             ~RwShaderNode();

    virtual void        postConstructor();
    virtual bool        setInternalValue(const MPlug&, const MDataHandle&);
    virtual MStatus     connectionMade(const MPlug & plug, const MPlug & otherPlug, bool asSrc);
    virtual MStatus     connectionBroken(const MPlug & plug, const MPlug & otherPlug, bool asSrc);

    virtual MStatus     compute(const MPlug&, MDataBlock&);


    static  void*       creator();
    static  MStatus     initialize();

    static  MTypeId     id;

protected:   

    static MObject version;
    
    static MObject defaultFrameCollapse;
    static MObject colorR;
    static MObject colorG;
    static MObject colorB;
    static MObject color;
    static MObject transR;
    static MObject transG;
    static MObject transB;
    static MObject transparency;
    static MObject defaultAmbientCoefficient;
    static MObject defaultDiffuseCoefficient;
    static MObject defaultSpecularCoefficient;
    static MObject defaultBaseTextureR;
    static MObject defaultBaseTextureG;
    static MObject defaultBaseTextureB;
    static MObject defaultBaseTexture;
    static MObject defaultBaseTextureConnection;
    static MObject defaultMatFXEffect;
    static MObject defaultBumpTextureR;
    static MObject defaultBumpTextureG;
    static MObject defaultBumpTextureB;
    static MObject defaultBumpTexture;
    static MObject defaultBumpValue;
    static MObject defaultEnvTextureR;
    static MObject defaultEnvTextureG;
    static MObject defaultEnvTextureB;
    static MObject defaultEnvTexture;
    static MObject defaultEnvValue;
    static MObject defaultSrcBlend;
    static MObject defaultDstBlend;
    static MObject defaultDualTextureR;
    static MObject defaultDualTextureG;
    static MObject defaultDualTextureB;
    static MObject defaultDualTexture;

    static MObject XBoxFrameCollapse;
    static MObject XBoxEnable;
    static MObject XBoxNumberPasses;
    static MObject XBoxTextureR;
    static MObject XBoxTextureG;
    static MObject XBoxTextureB;
    static MObject XBoxTexture;
    static MObject XBoxEffectFile;

    static MObject GameCubeFrameCollapse;
    static MObject GameCubeEnable;
    static MObject GameCubeNumberPasses;
    static MObject GameCubeTextureR;
    static MObject GameCubeTextureG;
    static MObject GameCubeTextureB;
    static MObject GameCubeTexture;
    static MObject GameCubeEffectFile;

    static MObject ToonFrameCollapse;
    static MObject ToonEnable;
    static MObject ToonPaintName;

    static MObject LtMapFrameCollapse;
    static MObject LtMapEnable;
    static MObject LtMapAreaLight;
    static MObject LtMapSky;
    static MObject LtMapNoShadow;
    static MObject LtMapFlatShaded;
    static MObject LtMapDensity;
    static MObject LtMapAreaLightColorR;
    static MObject LtMapAreaLightColorG;
    static MObject LtMapAreaLightColorB;
    static MObject LtMapAreaLightColor;
    static MObject LtMapAreaLightDensity;
    static MObject LtMapAreaLightRadius;

    static MObject NMapEnable;
    static MObject NMapTextureR;
    static MObject NMapTextureG;
    static MObject NMapTextureB;
    static MObject NMapTexture;
    static MObject NMapBumpValue;
    static MObject NMapEnvTextureR;
    static MObject NMapEnvTextureG;
    static MObject NMapEnvTextureB;
    static MObject NMapEnvTexture;
    static MObject NMapEnvValue;
    static MObject NMapEnvModulate;
    static MObject NMapConvAlpha;

    static MObject PreviewTextureFrameCollapse;
    static MObject PreviewTexture;

    static MObject normalCameraX;
    static MObject normalCameraY;
    static MObject normalCameraZ;
    static MObject normalCamera;

    static MObject lightDirectionX;
    static MObject lightDirectionY;
    static MObject lightDirectionZ;
    static MObject lightDirection;

    static MObject lightIntensityR;
    static MObject lightIntensityG;
    static MObject lightIntensityB;
    static MObject lightIntensity;

    static MObject lightAmbient;
    static MObject lightDiffuse;
    static MObject lightSpecular;
    static MObject lightShadowFraction;
    static MObject preShadowIntensity;
    static MObject lightBlindData;
    static MObject lightData;

    static MObject outColorR;
    static MObject outColorG;
    static MObject outColorB;
    static MObject outColor;
    
    static MObject outTransR;
    static MObject outTransG;
    static MObject outTransB;
    static MObject outTransparency;

private:
    MStatus             ConnectionChange(const MPlug & plug, const MPlug & otherPlug,
                                            bool asSrc, bool connectionMade);
    void                UpdateConnectedTextures();
    void                UpdatePreviewTexture(int previewTex);
    MPlug               GetFileTextureMessagePlug(RwWString attributeName);
    MPlug               GetArrayFileTextureMessagePlug(RwWString attributeName, int index);
    MObject             FindMaterialInfoObject();
    void                BreakPlugConnections(MPlug plug);
    void                MakePlugConnection(MPlug srcPlug, MPlug dstPlug);

    template <class T> bool setAttributeValue(MFnDependencyNode &node,
                                RwWString attributeName, T value);
    template <class T> bool setArrayAttributeValue(MFnDependencyNode &node,
                        RwWString attributeName, int index, T value);
};

enum PreviewTextureMenuValues
{
    PREVIEW_TEXTURE_INVALID = 0,
    PREVIEW_TEXTURE_NONE,
    PREVIEW_TEXTURE_SW,
    PREVIEW_TEXTURE_BASE,
    PREVIEW_TEXTURE_BUMP,
    PREVIEW_TEXTURE_ENV,
    PREVIEW_TEXTURE_DUAL,
    PREVIEW_TEXTURE_XBOX_0,
    PREVIEW_TEXTURE_XBOX_1,
    PREVIEW_TEXTURE_XBOX_2,
    PREVIEW_TEXTURE_XBOX_3,
    PREVIEW_TEXTURE_GAMECUBE_0,
    PREVIEW_TEXTURE_GAMECUBE_1,
    PREVIEW_TEXTURE_GAMECUBE_2,
    PREVIEW_TEXTURE_GAMECUBE_3,
    PREVIEW_TEXTURE_GAMECUBE_4,
    PREVIEW_TEXTURE_GAMECUBE_5,
    PREVIEW_TEXTURE_GAMECUBE_6,
    PREVIEW_TEXTURE_GAMECUBE_7,
    PREVIEW_TEXTURE_LTMAP,
    PREVIEW_TEXTURE_NMAP,
    PREVIEW_TEXTURE_NMAP_ENV,
};

#endif // RW_MAYA_SHADER_H