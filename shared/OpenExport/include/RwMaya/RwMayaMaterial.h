#if !defined(RW_MAYA_MATERIAL_H)
#define RW_MAYA_MATERIAL_H

#include "RwMayaTraverseAnimations.h"

/**
*  \ingroup mayascenegraph 
*
*  This is the RwMaya implementation of the RwCommShader.
*
*   \see RwCommShader
*/
class RWMAYA_API RwMayaMaterial : public RwMayaBaseMaterial,  public RwCommShader,
                                  public RwMayaBlindData
{
public:
	RwMayaMaterial(MObject shader, MObject set);
	virtual ~RwMayaMaterial();

    /**
     *  Maya implementation of RwCommObject::GetUserData. Simply calls RwMayaBlindData::GetUserData.
     *  \param userDataColl the RwCommUserDataCollection in which data is to be filled.
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetUserData(RwCommUserDataCollection &userDataColl);
    virtual void OnAddTexture(RwCommTexture *pTexture);

    // Get attribute methods.
    virtual RwCommError GetAttribute(const RwWString& name, int &value, int index = 0);
    virtual RwCommError GetAttribute(const RwWString& name, float &value, int index = 0);
    virtual RwCommError GetAttribute(const RwWString& name, RwWString &value, int index = 0);
    /**
     *  Maya implementation of RwCommAnimatable::GetAttributeNames. Simply calls RwMayaBlindData::GetObjectAttributeNames.
     *  \param vAttributeNames the names of all attributes found.
     *  \param vAttributeTypes the types of all attributes found.
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetAttributeNames(RwVector<RwWString>& vAttributeNames, RwVector<AttributeType> &vAttributeTypes);

    /**
     *  Maya implementation of RwCommShader::operator==. Compares the MObject returned by
     *  RwMayaBaseMaterial::GetShaderObject and then compares that the RwMayaBaseMaterial::m_vUniqueMayaUvSets
     *  members are identical. This allows Maya shaders to be differentiated by their UV set mapping as
     *  well as their underlying Maya dependency nodes.
     *  \param shader the other RwMayaMaterial to be compared.
     *  \return true if the two shaders are identical, false otherwise.
     */
    virtual bool operator==(const RwCommShader& shader) const;

    /**
     *  Maya implementation of RwCommShader::GetTextureUVSet. Iterates through m_vUvTextures until
     *  this texture is found and then extracts the matching entry from m_vOutputUvSets.
     *  \param pTexture the RwMayaTexture for which to find the UV set.
     *  \param uvSet the output UV set used by this texture.
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetTextureUVSet(RwCommTexture *pTexture, int &uvSet) const;

    /**
     *  Implementation of RwCommShader::MapUVs. Records the active textures in
     *  RwMayaBaseMaterial::m_vUvTextures and then calls RwMayaBaseMaterial::MapUVSets.
     *  \param pGeometry the RwMayaGeometry with UV sets to map to.
     *  \param vActiveTextures the textures that are are active and need mapping to UV sets.
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError MapUVs(RwCommGeometry* pGeometry, RwVector<RwCommTexture *> & vActiveTextures);
    
    /**
    *   Retrieve all textures of a given type. If this is a layered texture, the method returns
    *   a list of all the textures in the layered group. It will get texture MObject, of the
    *   specified type, connected to the shader, and then call GetShaderTexture()
    *
    *   \param type of textures to get.
    *   \param vTextures vector of textures with specified type.
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError GetTexturesByType(TextureType type, RwVector<RwCommTexture *> & vTextures) const;
    virtual RwCommError GetTextureFactor(TextureType type, RwCommReal & factor) const;
    virtual RwCommError GetBlendFunctions(TextureType type, RwVector<RwCommBlendFunction>& vBlendFuncs) const;

	// Material methods
    /**
     *  This function is not implemented under RwMaya.
     *  \return RWCOMM_NOT_IMPLEMENTED.
     */
	virtual RwCommError GetAmbientValue(RwCommVec3 &) const;
	virtual RwCommError GetDiffuseValue(RwCommVec3 &) const;
	virtual RwCommError GetTransparency(RwCommReal &) const;
    /**
     *  This function is not implemented under RwMaya.
     *  \return RWCOMM_NOT_IMPLEMENTED.
     */
	virtual RwCommError GetSpecularValue(RwCommVec3 &) const;
    /**
     *  This function is not implemented under RwMaya.
     *  \return RWCOMM_NOT_IMPLEMENTED.
     */
	virtual RwCommError GetShininess(RwCommReal &) const;
    /**
     *  This function is not implemented under RwMaya.
     *  \return RWCOMM_NOT_IMPLEMENTED.
     */
	virtual RwCommError GetShineStrength(RwCommReal &) const;
    /**
     *  This function is not implemented under RwMaya.
     *  \return RWCOMM_NOT_IMPLEMENTED.
     */
    virtual RwCommError GetSelfIllumination(RwCommVec3 &) const;
    /**
     *  This function is not implemented under RwMaya.
     *  \return RWCOMM_NOT_IMPLEMENTED.
     */
    virtual RwCommError IsDoubleSided(bool& bStat) const;

    // Internal methods
    RwCommError         GetTexColorGain(MObject texNode, RwCommVec3 & color) const;
    RwCommError         GetTexAlphaGain(MObject texNode, RwCommReal & a) const;

    /**
     *  Find the scene graph texture from a Maya texture object. Called from GetTexturesByType()
     *  \return The texture in the scene graph.
     */
    RwCommTexture       *GetShaderTexture(MObject texture) const;
    /**
     *  Get all the textures that belong to the Maya layered texture. 
     *
     *  \param colorTexObject Maya layered texture
     *  \param textureObjects returns Maya textures used by the layered texture
     */
    void                GetLayeredTextureObjects(MObject colorTexObject, MObjectArray &textureObjects) const;

protected:
};



/**
*  \ingroup mayabuilder 
*
*  This class is responsible for creating an RwMayaMaterial object.
*
*   \see RwMayaMaterial
*   \see RwMayaObjCreator
*/
class RWMAYA_API RwMayaMaterialCreator : public RwMayaObjCreator
{
public:
	RwMayaMaterialCreator();
	virtual ~RwMayaMaterialCreator();

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

#endif // !defined(RW_MAYA_MATERIAL_H)
