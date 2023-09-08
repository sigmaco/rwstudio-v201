// RwMaxMaterial.h: interface for the RwMaxMaterial class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWMAXMATERIAL_H__AB336BB0_B1A7_4117_B9E2_BD9D24A26680__INCLUDED_)
#define AFX_RWMAXMATERIAL_H__AB336BB0_B1A7_4117_B9E2_BD9D24A26680__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwCommShader.h"
#include "RwCommObjCreator.h"

#include "RwMax.h"
#include "RwMaxCustomAttributes.h"
#include "RwMaxTraverseControllers.h"

// This is an enum for the MetalBump viewport shader param block, defined in:
// C:\3dsmax5\maxsdk\samples\hardwareshaders\MetalBump\MetalBump.h
// We can't include this file since it is an example file and inlcude many other
// file from the project like resource.h etc. Anyway this should change it will
// probably have more values, but the current ones should stay.
#define		MAX_SHADER_CLASS_ID	Class_ID(0x20542fe5, 0x8f74721f)
#define     LIGHTMAP_CLASS_ID	Class_ID(0x727d33be, 0x3255c000)

enum 
{ 
	DIFFUSE_FILE,
	NORMAL_FILE,
	DETAIL_FILE,
	MASK_FILE,
	REFLECTION_FILE,
	BUMP_FILE,

	DIFFUSE_ON,
	NORMAL_ON,
	SPECULAR_ON,
	DETAIL_ON,
	MASK_ON,
	REFLECTION_ON,
	BUMP_ON,

	SPIN_BUMPSCALE,
	SPIN_MIXSCALE,
	SPIN_ALPHA,

	COLOR_AMBIENT,
	COLOR_DIFFUSE,
	COLOR_SPECULAR,

	MAPPING_DIFFUSE1,
	MAPPING_DIFFUSE2,
	MAPPING_SPECULAR,
	MAPPING_BUMP,

	SYNC_MATERIAL,
	SPIN_REFLECTSCALE,
	ALPHA_ON
};

enum { 
	pb_light_texture,
	pb_diffuse_texture,
	pb_diffuse_mapping,
	pb_lightmap_filename,
	pb_diffuse_filename,
	pb_lightmap_on,
	pb_diffuse_on,
	pb_lightmap_mapping,
};

#define     DXMATERIAL_CLASS_ID Class_ID(0xed995e4, 0x6133daf2)


/**
*  \ingroup maxscenegraph 
*
*   This is the RwMax implementation of the RwCommShader class. The class encapsulates an Mtl object.
*
*   \see RwCommShader
*/
class RWMAX_API RwMaxShader  : public RwCommShader, public RwMaxCustomAttributes
{
public:
    RwMaxShader(Mtl *pMtl);
    RwMaxShader(const RwMaxShader &that);
    virtual ~RwMaxShader();

    virtual RwCommError GetUserData(RwCommUserDataCollection &userDataColl);

    // Get attribute methods.
    virtual RwCommError GetAttribute(const RwWString &name, int &value, int index = 0);
    virtual RwCommError GetAttribute(const RwWString &name, float &value, int index = 0);
    virtual RwCommError GetAttribute(const RwWString &name, RwWString &value, int index = 0);
    virtual RwCommError GetAttributeNames(RwVector<RwWString> &vAttributeNames,
                                            RwVector<AttributeType> &vAttributeTypes);

    // Animatable methods
    RwCommError OnSetFrame(RwCommReal frame);

    // Tetxure methods
    virtual RwCommError MapUVs(RwCommGeometry *pGeometry, RwVector<RwCommTexture*> &vActiveTextures) { return RWCOMM_NOT_IMPLEMENTED; }
    virtual RwCommError GetTextureUVSet(RwCommTexture *pTexture, int& uvSet) const;
    virtual RwCommError GetTexturesByType(TextureType type, RwVector<RwCommTexture*> &vTextures) const;
    virtual RwCommError GetTextureFactor(TextureType type, RwCommReal &factor) const;
    virtual RwCommError GetBlendFunctions(TextureType type, RwVector<RwCommBlendFunction> &vBlendFuncs) const;

    //Material attrributes
    virtual RwCommError GetAmbientValue(RwCommVec3 &) const;
    virtual RwCommError GetSpecularValue(RwCommVec3 &) const;
    virtual RwCommError GetDiffuseValue(RwCommVec3 &) const;
    virtual RwCommError GetShininess(RwCommReal &) const;
    virtual RwCommError GetShineStrength(RwCommReal &) const;
    virtual RwCommError GetTransparency(RwCommReal &) const;
    virtual RwCommError GetSelfIllumination(RwCommVec3 &) const;
    virtual RwCommError IsDoubleSided(bool &bStat) const;

    /** Get the Mtl object.*/
    Mtl                 *GetMaterial() const                    {return m_pMtl;}
    
    BitmapTex           *GetViewPortNMap() const                {return m_viewPortNMap;}
    void                SetViewPortNMap(BitmapTex *normMap)     {m_viewPortNMap = normMap;}

    BitmapTex           *GetViewPortDiffuseLMap() const                {return m_viewPortDiffuseLMap;}
    void                SetViewPortDiffuseLMap(BitmapTex *diffuseLMap) {m_viewPortDiffuseLMap = diffuseLMap;}    

    BitmapTex           *GetViewPortLMap() const                {return m_viewPortLMap;}
    void                SetViewPortLMap(BitmapTex *lMap)        {m_viewPortLMap = lMap;}    

protected:
    virtual void    OnAddTexture(RwCommTexture* pTexture);
    /**
    *   Get the specified texture. Called from GetTexturesByType().
    *   \param index of the texture to retrieve
    *   \return The RwCommTexture specified by the index.
    */
    RwCommTexture   *FindTexture(int index) const;
    RwCommTexture   *FindTexture(BitmapTex *pTexmap) const;

protected:
    Mtl         *m_pMtl;
    BitmapTex   *m_viewPortNMap;

    BitmapTex   *m_viewPortDiffuseLMap;
    BitmapTex   *m_viewPortLMap;
};


/**
*  \ingroup maxbuilder 
*
*  This class is responsible for creating an RwMaxShader object.
*
*   \see RwCommShader
*/
class RWMAX_API RwMaxShaderCreator  : public RwCommObjCreator, public RwMaxTraverseControllers
{
public:
    RwMaxShaderCreator();
    virtual ~RwMaxShaderCreator();

    virtual RwCommObject *Create(const RwCommCreationData *pCreationData,
                                 const RwCommExportOptions *pExportOptions) const;
    virtual void Init(  RwCommObject *pObject,
                        const RwCommCreationData *pCreationData,
                        const RwCommExportOptions *pExportOptions) const;

	virtual void Attach(RwCommObject *pObject, RwCommObject *pParent, RwCommScene *pScene,
                        const RwCommCreationData *pCreationData) const;

	virtual void TraverseControllers(RwMaxBuilder &rBuilder, RwCommCreationData *pCreationData,
                                                                        RwCommObject *pParent);
};

#endif // !defined(AFX_RWMAXMATERIAL_H__AB336BB0_B1A7_4117_B9E2_BD9D24A26680__INCLUDED_)
