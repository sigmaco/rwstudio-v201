// RwExpRwMaterial.h: interface for the RwExpRwMaterial class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPRWMATERIAL_H__17EFF89E_728B_4441_9AFF_43983D0F1951__INCLUDED_)
#define AFX_RWEXPRWMATERIAL_H__17EFF89E_728B_4441_9AFF_43983D0F1951__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwCommShader.h"

/**
*  \ingroup commlowlevelobjects
*
*   This structure defines standard surface properties of RpMaterial
*
*   \note Refer to RenderWare Graphics API Reference for more information
*         about RpMaterial
*
*   \see RwCommRwMaterial
*/
struct RWCOMM_API RwCommSurfaceProps
{
    /** ambient reflection coefficient */
    RwCommReal  ambient;
    /** specular reflection coefficient */
    RwCommReal  specular;
    /** reflection coefficient */
    RwCommReal  diffuse;
};

const RwCommType RwCommRwMaterialType("RwCommRwMaterial");

/**
*  \ingroup commscenegraph  
*
*   This is the class representing a RenderWare material which provides direct
*   settings for the RenderWare Graphics RpMaterial object and no translation
*   of these settings is performed when exporting.
*
*   \note Refer to RenderWare Graphics API Reference for more information
*         about RpMaterial
*/
class RWCOMM_API RwCommRwMaterial : public RwCommShader
{
public:

    /** Extensions to the standard material settings available in RpMaterial */
    enum ExtensionType
    {
        NONE = 0,
        /** Xbox extension*/
        XBOX,           
        /** GameCube extension*/
        GAMECUBE,       
        /** Toon extension*/
        TOON,           
        /** LightMap extension*/
        LIGHTMAP,
        /** LightMap texture extension*/
        LIGHTMAPTEX,
        /** Normal map extension*/
        NORMALMAP,
    };

    /**
    *   These flags control the material effects (MatFX) settings for the
    *   RenderWare Graphics RpMaterial
    *
    *   \note Refer to RenderWare Graphics API Reference for more information
    *         about MatFX.
    */
    enum RwCommMatFXMaterialFlags
    {
        /** has no material effect. */
        rpMATFXEFFECTNULL            = 0, 
        /** enables bump mapping on the chosen material. */
        rpMATFXEFFECTBUMPMAP         = 1, 
        /** enables environment mapping on the chosen material. */
        rpMATFXEFFECTENVMAP          = 2, 
        /** enables both bump and environment mapping on the chosen material. */
        rpMATFXEFFECTBUMPENVMAP      = 3, 
        /** enables dual pass texturing. This option enables blending modes. */
        rpMATFXEFFECTDUAL            = 4, 
        /** single pass texture coordination animation.  */
        rpMATFXEFFECTUVTRANSFORM     = 5, 
        /** dual pass texture coordination animation. */
        rpMATFXEFFECTDUALUVTRANSFORM = 6, 
        rpMATFXEFFECTMAX,
    };

    enum RwCommLtMapMaterialFlags
    {
        rtLTMAPMATERIALNAFLAG      = 0,
        /** This material should be lightmapped. */
        rtLTMAPMATERIALLIGHTMAP    = 1,
        /** This material is an area light emitter. */
        rtLTMAPMATERIALAREALIGHT   = 2,
        /** This material does not block light. */
        rtLTMAPMATERIALNOSHADOW    = 4,
        /**
        *   This material blocks everything but directional lights, to allow sky
        *   polygons to occlude geometry and yet emit directional light.
        */
        rtLTMAPMATERIALSKY         = 8,
        /** This material will be lit as if flat-shaded. */
        rtLTMAPMATERIALFLATSHADE   = 16,

        rtLTMAPMATERIALFLAGFORCEENUMSIZEINT = 0x7FFFFFFF
    };

    RwCommRwMaterial();
    virtual ~RwCommRwMaterial();

   // Type methods
    virtual RwCommType      GetType() const { return RwCommRwMaterialType; }
    virtual void            GetTypeHierarchy(RwVector<RwCommType> &vTypeVec) const;

    /**
    *   \param color returns material RGBA color
    *   
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise
    */
    virtual RwCommError     GetBaseColor(RwCommVec4 &color) const = 0;
    /**
    *   \param sProps returns surface properties
    *   
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise
    */
    virtual RwCommError     GetSurfaceProperties(RwCommSurfaceProps &sProps) const = 0;

    // Basic matfx extension.
    /**
    *   \param effect returns MatFX type
    *   
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise
    */
    virtual RwCommError     GetMatFXEffect(RwCommMatFXMaterialFlags &effect) const = 0;

    // Xbox and GameCube extension
    /**
    *   \param type of the extension to query.
    *   \param bEnabled returns true if the specified extension is enabled.
    *   
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise
    */
    virtual RwCommError     IsExtensionEnabled(ExtensionType type, bool &bEnabled) const = 0;
    /**
    *   \param filename returns the name of the Xbox effect file name.
    *   
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise
    */
    virtual RwCommError     GetXBoxEffectFile(RwWString &filename) const = 0;
    /**
    *   \param filename returns the name of the GameCube effect file name.
    *   
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise
    */
    virtual RwCommError     GetGameCubeEffectFile(RwWString &filename) const = 0;
    /**
    *   \param vTextures returns a vector of all given Xbox textures
    *   
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise
    */
    virtual RwCommError     GetXboxTexture(RwVector<RwCommTexture *> &vTextures) = 0;
    /**
    *   \param vTextures returns a vector of all given GameCube textures
    *   
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise
    */
    virtual RwCommError     GetGameCubeTexture(RwVector<RwCommTexture *> &vTextures) = 0;
    
    // Toon extension
    /**
    *   \param paintname returns toon paint name
    *   
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise
    */
    virtual RwCommError     GetToonPaintName(RwWString &paintname) const = 0;

    // Light map extension
    /**
    *   \param flags returns light map flags
    *   
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise
    */
    virtual RwCommError     GetLightMapFlags(RwCommLtMapMaterialFlags &flags) const = 0;
    /**
    *   \param density returns light map density
    *   
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise
    */
    virtual RwCommError     GetLightMapDensity(RwCommReal &density) const = 0;
    /**
    *   \param color returns the color of the area light emitted by polygons
    *          of this material
    *   
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise
    */
    virtual RwCommError     GetLightMapAreaLightColor(RwCommVec4 &color) const = 0;
    /**
    *   \param density returns light map area light density
    *   
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise
    */
    virtual RwCommError     GetLightMapAreaLightDensity(RwCommReal &density) const = 0;
    /**
    *   \param radius returns the radius of the area light emitted by polygons
    *          of this material
    *   
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise
    */
    virtual RwCommError     GetLightMapAreaLightRadius(RwCommReal &radius) const = 0;

    // Normal map extension
    /**
    *   \param envFactor returns normal map environment factor
    *   
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise
    */
    virtual RwCommError     GetNormalMapEnvFactor(RwCommReal &envFactor) const = 0;

    /**
    *   \param bModulate returns whether the env map should be modulated by the base texture
    *   
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise
    */
    virtual RwCommError     GetNormalMapEnvModulate(bool &bModulate) const = 0;

    /**
    *   \param bConvert returns true if the texture is an alpha map that should
    *          be converted into normal map
    *   
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise
    */
    virtual RwCommError     GetNormalMapConvertAlpha(bool &bConvert) const = 0;
    /**
    *   \param bumpFactor returns normal map bump value
    *   
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise
    */
    virtual RwCommError     GetNormalMapBumpFactor(RwCommReal &bumpFactor) const = 0;
};

#endif // !defined(AFX_RWEXPRWMATERIAL_H__17EFF89E_728B_4441_9AFF_43983D0F1951__INCLUDED_)
