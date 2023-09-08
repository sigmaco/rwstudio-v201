#if !defined(RW_MAYA_RW_MATERIAL_H)
#define RW_MAYA_RW_MATERIAL_H

#include "RwMayaBlindData.h"

class PlatformSettings
{
public:
    int                         Enable;
    int                         NumberPasses;
    int                         StagesPerPass;
    RwVector<MObject>       TexNodes;
    RwVector<RwWString>  EffectFile;
};

/**
*  \ingroup mayascenegraph 
*
*  This is the RwMaya implementation of the RwCommRwMaterial. It encapsulates a
*  built in Maya material such as lambert, phong or blinn.
*
*   \see RwCommRwMaterial
*   \see RwMayaBaseMaterial
*   \see RwMayaBlindData
*/

class RWMAYA_API RwMayaRwMaterial  : public RwMayaBaseMaterial, public RwCommRwMaterial,
                                     public RwMayaBlindData
{
public:
	RwMayaRwMaterial(MObject shader, MObject set);
	virtual ~RwMayaRwMaterial();

    /**
     *  Maya implementation of RwCommObject::GetUserData. Simply calls RwMayaBlindData::GetUserData.
     *  \param userDataColl the RwCommUserDataCollection in which data is to be filled.
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetUserData(RwCommUserDataCollection &userDataColl);

    // Get attribute methods.
    virtual RwCommError GetAttribute(const RwWString &name, int &value, int index = 0);
    virtual RwCommError GetAttribute(const RwWString &name, float &value, int index = 0);
    virtual RwCommError GetAttribute(const RwWString &name, RwWString &value, int index = 0);
    /**
     *  Maya implementation of RwCommAnimatable::GetAttributeNames. Simply calls RwMayaBlindData::GetObjectAttributeNames.
     *  \param vAttributeNames the names of all attributes found.
     *  \param vAttributeTypes the types of all attributes found.
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetAttributeNames(RwVector<RwWString> &vAttributeNames, RwVector<AttributeType> &vAttributeTypes);

    virtual void OnAddTexture(RwCommTexture *pTexture);

    // Comparison operator used for instancing
    virtual bool operator==(const RwCommShader &shader) const;

    /**
     *  Maya implementation of RwCommShader::GetTextureUVSet. Iterates through m_vUvTextures until
     *  this texture is found and then extracts the matching entry from m_vOutputUvSets.
     *  \param pTexture the RwMayaTexture for which to find the UV set.
     *  \param uvSet the output UV set used by this texture.
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError     GetTextureUVSet(RwCommTexture *pTexture, int &uvSet) const;

    /**
     *  Implementation of RwCommShader::MapUVs. Records the active textures in
     *  RwMayaBaseMaterial::m_vUvTextures and then calls RwMayaBaseMaterial::MapUVSets.
     *  \param pGeometry the RwMayaGeometry with UV sets to map to.
     *  \param vActiveTextures the textures that are are active and need mapping to UV sets.
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError     MapUVs(RwCommGeometry *pGeometry, RwVector<RwCommTexture *> & vActiveTextures);

    virtual RwCommError     GetTexturesByType(TextureType type, RwVector<RwCommTexture *> &vTextures) const;
    virtual RwCommError     GetTextureFactor(TextureType type, RwCommReal &factor) const;
    virtual RwCommError     GetBlendFunctions(TextureType type, RwVector<RwCommBlendFunction> &vBlendFuncs) const;

	// Material attrributes
    /**
     *  This function is not implemented under RwMaya.
     *  \return RWCOMM_NOT_IMPLEMENTED.
     */
	virtual RwCommError     GetAmbientValue(RwCommVec3 &) const;
    /**
     *  This function is not implemented under RwMaya.
     *  \return RWCOMM_NOT_IMPLEMENTED.
     */
	virtual RwCommError     GetDiffuseValue(RwCommVec3 &) const;
    /**
     *  This function is not implemented under RwMaya.
     *  \return RWCOMM_NOT_IMPLEMENTED.
     */
	virtual RwCommError     GetTransparency(RwCommReal &) const;
    /**
     *  This function is not implemented under RwMaya.
     *  \return RWCOMM_NOT_IMPLEMENTED.
     */
	virtual RwCommError     GetSpecularValue(RwCommVec3 &) const;
    /**
     *  This function is not implemented under RwMaya.
     *  \return RWCOMM_NOT_IMPLEMENTED.
     */
	virtual RwCommError     GetShininess(RwCommReal &) const;
    /**
     *  This function is not implemented under RwMaya.
     *  \return RWCOMM_NOT_IMPLEMENTED.
     */
	virtual RwCommError     GetShineStrength(RwCommReal &) const;
    /**
     *  This function is not implemented under RwMaya.
     *  \return RWCOMM_NOT_IMPLEMENTED.
     */
    virtual RwCommError     GetSelfIllumination(RwCommVec3 &) const;
    /**
     *  This function is not implemented under RwMaya.
     *  \return RWCOMM_NOT_IMPLEMENTED.
     */
    virtual RwCommError     IsDoubleSided(bool &bStat) const;

    // Default material.
    virtual RwCommError     GetBaseColor(RwCommVec4 &color) const;

    virtual RwCommError     GetSurfaceProperties(RwCommSurfaceProps &sProps) const;

    // Basic matfx extension.
    virtual RwCommError     GetMatFXEffect(RwCommMatFXMaterialFlags &effect) const;

    // Xbox and GameCube extension
    virtual RwCommError     IsExtensionEnabled(ExtensionType type, bool &bEnabled) const;
    virtual RwCommError     GetXBoxEffectFile(RwWString &filename) const;
    virtual RwCommError     GetGameCubeEffectFile(RwWString &filename) const;
    virtual RwCommError     GetXboxTexture(RwVector<RwCommTexture *> &vTextures);
    virtual RwCommError     GetGameCubeTexture(RwVector<RwCommTexture *> &vTextures);
    
    // Toon extension
    virtual RwCommError     GetToonPaintName(RwWString &paintname) const;

    // Light map extension
    virtual RwCommError     GetLightMapFlags(RwCommLtMapMaterialFlags &flags) const;
    virtual RwCommError     GetLightMapDensity(RwCommReal &density) const;
    virtual RwCommError     GetLightMapAreaLightColor(RwCommVec4 &color) const;
    virtual RwCommError     GetLightMapAreaLightDensity(RwCommReal &density) const;
    virtual RwCommError     GetLightMapAreaLightRadius(RwCommReal &radius) const;
    
    // Normal map extension
    virtual RwCommError     GetNormalMapEnvFactor(RwCommReal &envFactor) const;
    virtual RwCommError     GetNormalMapEnvModulate(bool &bModulate) const;
    virtual RwCommError     GetNormalMapConvertAlpha(bool &bConvert) const;
    virtual RwCommError     GetNormalMapBumpFactor(RwCommReal &bumpFactor) const;

    // Internal functions
    void                    GetPlatformSettings(const RwWString &platform, PlatformSettings &settings,
                                            int StagesPerPass);
    RwCommTexture           *GetShaderTexture(MObject texture) const;

protected:
    PlatformSettings    XBoxSettings;
    PlatformSettings    GameCubeSettings;
};


/**
*  \ingroup mayabuilder 
*
*  This class is responsible for creating an RwMayaRwMaterial object.
*
*   \see RwMayaRwMaterial
*   \see RwMayaObjCreator
*/
class RWMAYA_API RwMayaRwMaterialCreator : public RwMayaObjCreator
{
public:
	RwMayaRwMaterialCreator();
	virtual ~RwMayaRwMaterialCreator();

	virtual void Init(	RwCommObject* pObject,
						const RwCommCreationData *pCreationData,
						const RwCommExportOptions *pExportOptions) const;

	virtual RwCommObject *Create(const RwCommCreationData *pCreationData,
								 const RwCommExportOptions *pExportOptions) const;

	virtual void Attach(RwCommObject* pObject, RwCommObject* pParent, RwCommScene* pScene,
                        const RwCommCreationData* pCreationData) const;
	virtual void Traverse(RwCommBuilder& rBuilder, RwCommCreationData* pCreationData, RwCommObject* pParent) const;
protected:
    RwMayaTraverseAnimations m_AnimationTraverse;
};

#endif // !defined(RW_MAYA_RW_MATERIAL_H)
