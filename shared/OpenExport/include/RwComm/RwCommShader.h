// RwExpMaterial.h: interface for the RwExpMaterial class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPMATERIAL_H__C62F57E1_2BC7_4017_BF34_AC2EDC697498__INCLUDED_)
#define AFX_RWEXPMATERIAL_H__C62F57E1_2BC7_4017_BF34_AC2EDC697498__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwCommPrimitives.h"
#include "RwCommAnimatable.h"
#include "RwCommTexture.h"

const RwCommType RwCommShaderType("RwCommShader");

class RwCommGeometry;

/**
*  \ingroup commscenegraph  
*
*   This is the class representing a shader object. This object determines
*   how a shape is rendered. The class derives from RwCommAnimatable and can
*   be animated over time. 
*
*   The shader holds a list of textures associated with that shader. 
*   A shader can have an arbitrary number of textures attached to it. All textures are
*   arranged according to their texture type. The type can be:
*   \li COLOR - for the main diffuse channel
*   \li ENV - for environnment map channel
*   \li BUMP - for the bump channel
*   \li TRANSPARENCY - for the alpha channel
*   \li NORMAL - for the normal map channel
*
*   The shader can hold multiple textures of the
*   same type - this imposes a multi pass operation on that channel.
*   Each texture can be mapped to a specific UV channel on the associated geometry.
*   Before querying the geometry for UV information via the primitive iteration
*   methods and RwCommGeometry::GetPrimVertex(), the geometry needs to be
*   explicitly bound to a list of relevant textures.
*   This list can be a subset of all the textures attached to the shader.
*   To bind the geometry with the list of textures use the shader's MapUVs()
*   method. Failing to call this function may result in invalid UV channel
*   mapping on the geometry.
*   
*   Each texture type can have an assigned factor. For ENV textures this is the environment factor,
*   for BUMP textures - the bump amount, for TRANSPARENCY textures - the alpha value and for
*   NORMAL textures - the normal map amount. In addition, if the channel contains multiple textures then
*   a blend operation can be specified on all the channel members. This is given by a blend function 
*   for each member of the channel. 
*   
*/
class RWCOMM_API RwCommShader : public RwCommAnimatable
{
public:
    /** Texture can have a specific role or type, relating to that shader. */
    enum TextureType
    {
        NONE = 0,
        /** color texture */
        COLOR,          
        /** enviroment texture */
        ENV,
        /** bump texture */
        BUMP,
        /** transparency texture */
        TRANSPARENCY,
        /** normal map texture, holding vertex normal information */
        NORMAL,
        /** lightmap texture */
        LIGHTMAP
    };

    /**
    *   Blend modes are factors used to modulate either the source or destination
    *   pixel color when blending to the frame buffer.
    */
    enum RwCommBlendFunction
    {
        BLENDNABLEND = 0,     
        /** (0, 0, 0, 0 ) */         
        BLENDZERO,          
        /** (1, 1, 1, 1 ) */   
        BLENDONE,           
        /** (Rs, Gs, Bs, As ) */   
        BLENDSRCCOLOR,      
        /** (1-Rs, 1-Gs, 1-Bs, 1-As) */ 
        BLENDINVSRCCOLOR,   
        /** (As, As, As, As ) */ 
        BLENDSRCALPHA,      
        /** (1-As, 1-As, 1-As, 1-As) */ 
        BLENDINVSRCALPHA,   
        /** (Ad, Ad, Ad, Ad ) */ 
        BLENDDESTALPHA,     
        /** (1-Ad, 1-Ad, 1-Ad, 1-Ad) */ 
        BLENDINVDESTALPHA,  
        /** (Rd, Gd, Bd, Ad ) */ 
        BLENDDESTCOLOR,     
        /** (1-Rd, 1-Gd, 1-Bd, 1-Ad) */ 
        BLENDINVDESTCOLOR,  
        /** (f, f, f, 1 ) f = min (As, 1-Ad) */ 
        BLENDSRCALPHASAT    
    };

    // Animated channels
    static RwCommAnimatedChannel AmbientChannel;
    static RwCommAnimatedChannel AmbientRChannel;
    static RwCommAnimatedChannel AmbientGChannel;
    static RwCommAnimatedChannel AmbientBChannel;
    static RwCommAnimatedChannel DiffuseChannel;
    static RwCommAnimatedChannel DiffuseRChannel;
    static RwCommAnimatedChannel DiffuseGChannel;
    static RwCommAnimatedChannel DiffuseBChannel;
    static RwCommAnimatedChannel SpecularChannel;
    static RwCommAnimatedChannel SpecularRChannel;
    static RwCommAnimatedChannel SpecularGChannel;
    static RwCommAnimatedChannel SpecularBChannel;
    static RwCommAnimatedChannel ShininessChannel;
    static RwCommAnimatedChannel ShineStrengthChannel;
    static RwCommAnimatedChannel TransparencyChannel;
    static RwCommAnimatedChannel SelfIlluminationChannel;
    static RwCommAnimatedChannel SelfIlluminationRChannel;
    static RwCommAnimatedChannel SelfIlluminationGChannel;
    static RwCommAnimatedChannel SelfIlluminationBChannel;
    static RwCommAnimatedChannel UVChannel;


    RwCommShader();
    virtual ~RwCommShader();

    // Type methods
    virtual RwCommType  GetType() const { return RwCommShaderType; }
    virtual void        GetTypeHierarchy(RwVector<RwCommType> &vTypeVec) const;

    /** 
    *   Comparison operator. Determines if two shaders are identical. This method is used by the
    *   exporters to test if multiple geometries can share the same shader instance.
    */
    virtual bool        operator==(const RwCommShader &shader) const { return false; }

    // Texture methods
    /** \param pTexture to add to the shader */
    void                AddTexture(RwCommTexture *pTexture);
    /** Remove all the textures from the shader */
    void                RemoveAllTextures();
    /** \return the number of the textures */
    int                 GetNumTextures() const;
    /**
    *   Get specified texture.
    *   
    *   \param i specifies texture to get
    *
    *   \return texture with specified index.
    */
    RwCommTexture       *GetTexture(int i);
    /** \return all textures associated with the shader. */
    const RwVector<RwCommTexture *> &GetTextures() const { return m_vTextures; }

    /**
    *   Bind the geometry with the list of textures relevant textures. Each
    *   texture can be mapped to a specific UV channel on the associated geometry.
    *   Before querying the geometry for UV information via the primitive iteration
    *   methods and RwCommGeometry::GetPrimVertex(), the geometry needs to be
    *   explicitly bound to a list of relevant textures that we want to export.
    *   This list can be a subset of all the textures attached to the shader.
    *   
    *   \param pGeometry this shader is applied to
    *   \param vActiveTextures list of textures we'll export
    *   
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    *
    *   \see RwExpRpMaterial::GetActiveTextures()
    */
    virtual RwCommError MapUVs(RwCommGeometry *pGeometry,
                               RwVector<RwCommTexture *> &vActiveTextures) = 0;
    /**
    *   Get the UV set, assigned to the specified texture.
    *   
    *   \param pTexture specifies texture for which to get UV set number
    *   \param uvSet returns UV set number
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError GetTextureUVSet(RwCommTexture *pTexture, int &uvSet) const = 0;
    /**
    *   Retrieve all textures of a given type. If this is a layered texture, the method returns
    *   a list of all the textures in the layered group. Otherwise, it just returns the one texture
    *   belonging to that texture type.
    *   
    *   \param type of textures to get.
    *   \param vTextures vector of textures with specified type.
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError GetTexturesByType(TextureType type,
                                          RwVector<RwCommTexture *> &vTextures) const = 0;
    /**
    *   Retrieve the factor of the given texture type. For ENV textures this is the environment factor,
    *   for BUMP textures - the bump amount, for TRANSPARENCY textures - the alpha value and for
    *   NORMAL textures - the normal map amount.
    *   
    *   \param type of textures to get.
    *   \param factor of texture with specified type. For a bump texture it's
    *          a bump value, for a transparency texture it's an alpha value, etc.
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError GetTextureFactor(TextureType type, RwCommReal& factor) const = 0;

    /**
    *   Retrieve all the blend functions for the given texture type. If this is a layered texture, the
    *   method returns a function for each texture, corresponsing to the textures returned from the
    *   GetTexturesByType() method.
    *   
    *   \param type of textures to get.
    *   \param vBlendFuncs vector of blend functions for textures of specified type.
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError GetBlendFunctions(TextureType type,
                                          RwVector<RwCommBlendFunction> &vBlendFuncs) const = 0;

    // Material attributes.
    
    /**
     *  Get ambient value.
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetAmbientValue(RwCommVec3 &) const = 0;
    
    /**
     *  Get specular value.
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetSpecularValue(RwCommVec3 &) const = 0;
    
    /**
     *  Get diffuse value.
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetDiffuseValue(RwCommVec3 &) const = 0;
    
    /**
     *  Get Shininess.
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetShininess(RwCommReal &) const = 0;
    
    /**
     *  Get shine strength.
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetShineStrength(RwCommReal &) const = 0;
    
    /**
     * Get transparency value.
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetTransparency(RwCommReal &) const = 0;
    
    /**
     *  Get self illumination value.
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetSelfIllumination(RwCommVec3 &) const = 0;
    
    /**
     *  Get whether this shader is double sided.
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError IsDoubleSided(bool &bStat) const = 0;

protected:
    /**
    *   Executed after each texture is added to the shader with AddTexture()
    *   
    *   \param pTexture that is being added
    */
    virtual void        OnAddTexture(RwCommTexture *pTexture) {}

protected:
    RwVector<RwCommTexture *> m_vTextures;
};

#endif // !defined(AFX_RWEXPMATERIAL_H__C62F57E1_2BC7_4017_BF34_AC2EDC697498__INCLUDED_)
