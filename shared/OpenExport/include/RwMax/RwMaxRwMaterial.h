
#ifndef RWMAXRWMATERIAL_H
#define RWMAXRWMATERIAL_H


#include "RwCommRwMaterial.h"
#include "RwCommObjCreator.h"

#include "RwMax.h"
#include "RwMaxCustomAttributes.h"
#include "RwMaxTraverseControllers.h"

extern HINSTANCE hGInstance;

enum
{
    block_defmtl,
    block_xbox_mtl,
    block_gcn_mtl,
    block_toon_mtl,
    block_ltmap_mtl,
    block_nmap_mtl
};

enum
{
    defmtl_color,
    defmtl_coloralpha,
    defmtl_sp_ambient,
    defmtl_sp_diffuse,
    defmtl_sp_specular,
    defmtl_texmap_texture,
    defmtl_texmap_envmap,
    defmtl_texmap_bumpmap,
    defmtl_texmap_pass2,
    defmtl_matfxeffect,
    defmtl_envmap_amount,
    defmtl_bumpmap_amount,
    defmtl_pass2_srcblend,
    defmtl_pass2_destblend,
    defmtl_version,
};

enum
{
    defmtl_basetex,
    defmtl_envmap, 
    defmtl_bumpmap,
    defmtl_pass2,  
};

enum
{
    xbox_enable,
    xbox_effect_file,
    xbox_texmap_texture0,
    xbox_texmap_texture1,
    xbox_texmap_texture2,
    xbox_texmap_texture3,
    xbox_effect_file_eb,
    xbox_normalmap_enable,
    xbox_texmap_texture4,
    xbox_texmap_texture5,
    xbox_envmap_amount,
    xbox_convert_alpha,
    xbox_bump_amount,
};

enum
{
    gcn_enable,
    gcn_effect_file,
    gcn_texmap_texture0,
    gcn_texmap_texture1,
    gcn_texmap_texture2,
    gcn_texmap_texture3,
    gcn_texmap_texture4,
    gcn_texmap_texture5,
    gcn_texmap_texture6,
    gcn_texmap_texture7,
    gcn_effect_file_eb,
};

enum
{
    toon_enable,
    toon_paint_name
};

enum
{
    ltmap_enable,
    ltmap_arealight,
    ltmap_sky,
    ltmap_noshadow,
    ltmap_flatshaded,
    ltmap_density,
    ltmap_arealightcolor,
    ltmap_arealightdensity,
    ltmap_arealightradius,
};

enum
{
    normalmap_enable,
    normalmap_texture,
    normalmap_envmap_texture,
    normalmap_envmap_amount,
    normalmap_envmap_modulate,
    normalmap_convert_alpha,
    normalmap_bump_amount,
};

// class id for the material.
#define RWMATERIAL_CLASS_ID Class_ID(0x64843c66, 0x2a8b2eab)

// Version number for material
#define RWMATERIAL_CLASS_VER 2

// Reference numbers and number of texmaps managed by this material.
#define NUM_TEX_DEFMTL              4
#define NUM_TEX_XBOX                4
#define NUM_TEX_GCN                 8
#define NUM_TEX_BASE                (NUM_TEX_DEFMTL + NUM_TEX_XBOX + NUM_TEX_GCN)   // 16
#define PBLOCK_REF_DEFMTL           NUM_TEX_BASE                                    // 16
#define PBLOCK_REF_XBOX             (PBLOCK_REF_DEFMTL + 1)                         // 17
#define PBLOCK_REF_GCN              (PBLOCK_REF_XBOX + 1)                           // 18
#define PBLOCK_REF_TOON             (PBLOCK_REF_GCN + 1)                            // 19
#define PBLOCK_REF_LTMAP            (PBLOCK_REF_TOON + 1)                           // 20
#define PBLOCK_REF_NMAP             (PBLOCK_REF_LTMAP + 1)                          // 21
#define NUM_TEX_LEGACY_NORMALMAP    2
#define NUM_TEX_NORMALMAP           2
#define NUM_TEX_EXTRA               (NUM_TEX_LEGACY_NORMALMAP + NUM_TEX_NORMALMAP)
#define NUM_TEX_ALL                 (NUM_TEX_BASE + NUM_TEX_EXTRA)
#define NUM_TEX_EXTRA_OFFSET        32
#define NUM_REFERENCES              (NUM_TEX_EXTRA_OFFSET + NUM_TEX_EXTRA)

// Looks like we didn't leave place for for more then 16 textures for reference ids
// and the problem is we can't shift param block ParamIDs since they are permanent
// and unique. We'll start adding more textures from number 32. So when we add tex
// we should change NUM_TEX_EXTRA, not NUM_TEX_BASE which has to be hardcoded to 16.
// There is a place for aditional param block, currently around 12 slots. Important
// thing to know whenever working with textures and references, is to use functions:
//      IsReferenceTexture()
//      ExtraTexturesReferenceToTexNum()
//      ExtraTexturesTexNumToReference()

// =============================================================================
// ----------------------------  RwMaterial CLASS  -----------------------------
// =============================================================================

/**
*  \ingroup maxscenegraph 
*
*   This is the RwMax implementation of the RwCommRwMaterial and represents RwMaterial
*   in 3ds max.
*
*   \see RwCommRwMaterial, RwMaxCustomAttributes
*/
class RWMAX_API RwMaxRwMaterial : public Mtl, public RwCommRwMaterial,
                                  public RwMaxCustomAttributes
{
public:
    RwMaxRwMaterial(BOOL loading);
    RwMaxRwMaterial(const RwMaxRwMaterial &that);
    virtual ~RwMaxRwMaterial();

    // RwCommObject methods.
    /**
     *  RwMax implementation of RwCommObject::GetUserData. Simply calls
     *  RwMaxCustomAttributes::GetUserData().
     *  \param userDataColl the RwCommUserDataCollection in which data is to be filled.
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError     GetUserData(RwCommUserDataCollection &userDataColl);

    // Get attribute methods.
    virtual RwCommError GetAttribute(const RwWString &name, int &value, int index = 0);
    virtual RwCommError GetAttribute(const RwWString &name, float &value, int index = 0);
    virtual RwCommError GetAttribute(const RwWString &name, RwWString &value, int index = 0);
    /**
     *  RwMax implementation of RwCommAnimatable::GetAttributeNames. Simply calls
     *  RwMaxCustomAttributes::GetCustomAttributeNames().
     *  \param vAttributeNames the names of all attributes found.
     *  \param vAttributeTypes the types of all attributes found.
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetAttributeNames(RwVector<RwWString> &vAttributeNames,
                                            RwVector<AttributeType> &vAttributeTypes);

    // RwCommShader methods.
    /**
     *  This function is not implemented under RwMax.
     *  \return RWCOMM_NOT_IMPLEMENTED.
     */
    virtual RwCommError     MapUVs(RwCommGeometry *pGeometry,
                                RwVector<RwCommTexture*> &vActiveTextures)    {return RWCOMM_NOT_IMPLEMENTED;}
    /**
     *  3dsmax implementation of RwCommShader::GetTextureUVSet. Returns the 
     *  UV set Map Channel value from the Bitmap in 3ds max. If the texture
     *  is the Env Map in the Normal Map Extension, the Map Channel from the 
     *  Normal Map will be returned.
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError     GetTextureUVSet(RwCommTexture *pTexture,
                                int& uvSet) const;
    virtual RwCommError     GetTexturesByType(TextureType type,
                                RwVector<RwCommTexture*> &vTextures) const;
    virtual RwCommError     GetTextureFactor(TextureType type,
                                RwCommReal &factor) const;
    virtual RwCommError     GetBlendFunctions(TextureType type,
                                RwVector<RwCommShader::RwCommBlendFunction>
                                &vBlendFuncs) const;
    virtual RwCommError     GetAmbientValue(RwCommVec3 &) const;
    virtual RwCommError     GetSpecularValue(RwCommVec3 &) const;
    virtual RwCommError     GetDiffuseValue(RwCommVec3 &) const;
    virtual RwCommError     GetShininess(RwCommReal &) const;
    virtual RwCommError     GetShineStrength(RwCommReal &) const;
    virtual RwCommError     GetTransparency(RwCommReal &) const;
    /**
     *  This function is not implemented under RwMax.
     *  \return RWCOMM_NOT_IMPLEMENTED.
     */
    virtual RwCommError     GetSelfIllumination(RwCommVec3 &) const          {return RWCOMM_NOT_IMPLEMENTED;}
    /**
     *  This function is not implemented under RwMax.
     *  \return RWCOMM_NOT_IMPLEMENTED.
     */
    virtual RwCommError     IsDoubleSided(bool &bStat) const                {return RWCOMM_NOT_IMPLEMENTED;}

    // RwCommRwMaterial methods.
    virtual RwCommError     GetBaseColor(RwCommVec4 &color) const;
    virtual RwCommError     GetSurfaceProperties(RwCommSurfaceProps &sProps) const;

    virtual RwCommError     GetDualPassSrcBlend(RwCommBlendFunction &func) const;
    virtual RwCommError     GetDualPassDestBlend(RwCommBlendFunction &func) const;
    virtual RwCommError     GetMatFXEffect(RwCommMatFXMaterialFlags &effect) const;

    virtual RwCommError     IsExtensionEnabled(ExtensionType type, bool &bEnabled) const;
    virtual RwCommError     GetXBoxEffectFile(RwWString &filename) const;
    virtual RwCommError     GetGameCubeEffectFile(RwWString &filename) const;
    virtual RwCommError     GetXboxTexture(RwVector<RwCommTexture *> &vTextures);
    virtual RwCommError     GetGameCubeTexture(RwVector<RwCommTexture*> &vTextures);

    virtual RwCommError     GetToonPaintName(RwWString& paintname) const;

    virtual RwCommError     GetLightMapFlags(RwCommLtMapMaterialFlags &flags) const;
    virtual RwCommError     GetLightMapDensity(RwCommReal &density) const;
    virtual RwCommError     GetLightMapAreaLightColor(RwCommVec4 &color) const;
    virtual RwCommError     GetLightMapAreaLightDensity(RwCommReal &density) const;
    virtual RwCommError     GetLightMapAreaLightRadius(RwCommReal &radius) const;

    virtual RwCommError     GetNormalMapEnvFactor(RwCommReal &envFactor) const;
    virtual RwCommError     GetNormalMapConvertAlpha(bool &bConvert) const;
    virtual RwCommError     GetNormalMapBumpFactor(RwCommReal &bumpFactor) const;
    virtual RwCommError     GetNormalMapEnvModulate(bool &bModulate) const;

    // RwMaxRwMaterial methods.
    void                    DeleteThis()                                    {delete this;}
    void                    DestroyTexHandles(int first, int last);
    Texmap                  *GetTexmap(int texture_no);
    void                    EqualizeEffFiles();
    void                    UpdateInitFileNames();
    void                    UpdateEffFiles(int blockID, ParamID paramID,
                                        TCHAR *newVale);

    BitmapTex               *GetViewPortNMap() const                        {return m_viewPortNMap;}
    void                    SetViewPortNMap(BitmapTex *normMap)             {m_viewPortNMap = normMap;}
    BitmapTex               *GetViewPortDiffuseLMap() const                {return m_viewPortDiffuseLMap;}
    void                    SetViewPortDiffuseLMap(BitmapTex *diffuseLMap) {m_viewPortDiffuseLMap = diffuseLMap;}    

    BitmapTex               *GetViewPortLMap() const                        {return m_viewPortLMap;}
    void                    SetViewPortLMap(BitmapTex *lMap)                {m_viewPortLMap = lMap;}    
		
	Mtl						*GetClonedMaterial()							{return m_pClonedFrom;}

    // From Animatable Class.
    Class_ID                ClassID()                                       {return RWMATERIAL_CLASS_ID;}
    SClass_ID               SuperClassID()                                  {return MATERIAL_CLASS_ID;}
    int                     NumSubs()                                       {return NUM_REFERENCES;}
    void                    GetClassName(TSTR &s)                           {s = _T("RwMaterial");}
    TSTR                    SubAnimName(int i);
    Animatable              *SubAnim(int i);
    int                     NumParamBlocks();
    IParamBlock2            *GetParamBlock(int i);
    IParamBlock2            *GetParamBlockByID(BlockID id);

    // From ReferenceMaker, ReferenceTarget Class.
    IOResult                Load(ILoad *iload);
    IOResult                Save(ISave *isave);
    int                     NumRefs()                                       {return NUM_REFERENCES;}
    void                    SetReference(int i, RefTargetHandle rtarg);
    RefTargetHandle         GetReference(int i);
    RefTargetHandle         Clone(RemapDir &remap);
    RefResult               NotifyRefChanged(Interval changeInt,
                                RefTargetHandle hTarget, PartID &partID,
                                RefMessage message);

    // From ISubMap Class.
    int                     NumSubTexmaps()                                 {return NUM_TEX_ALL;}
    Texmap*                 GetSubTexmap(int i);
    void                    SetSubTexmap(int i, Texmap *m);
    TSTR                    GetSubTexmapSlotName(int i);
    TSTR                    GetSubTexmapTVName(int i);
    BOOL                    SetDlgThing(ParamDlg* dlg);

    // From MtlBase Class.
    void                    Reset();
    void                    Update(TimeValue t, Interval& valid);
    BOOL                    SupportTexDisplay()                             {return TRUE;}
    ULONG                   LocalRequirements(int subMtlNum);
    Interval                Validity(TimeValue t);
    ParamDlg                *CreateParamDlg(HWND hwMtlEdit,
                                        IMtlParams *imp);
    // Disable 3ds max 5 multi texture display until D3D support is fixed
    BOOL                    SupportsMultiMapsInViewport()                   {return TRUE;}
    void                    SetupGfxMultiMaps(TimeValue t, Material *mtl,
                                                MtlMakerCallback &cb);

    // From Mtl Class.
    void                    SetAmbient(Color c, TimeValue t)                {}
    void                    SetDiffuse(Color c, TimeValue t)                {}
    void                    SetSpecular(Color c, TimeValue t)               {}
    void                    SetShininess(float v, TimeValue t)              {}
    void                    Shade(ShadeContext& sc);
    Color                   GetAmbient(int mtlNum=0, BOOL backFace=FALSE);
    Color                   GetDiffuse(int mtlNum=0, BOOL backFace=FALSE);
    Color                   GetSpecular(int mtlNum=0, BOOL backFace=FALSE);
    float                   GetXParency(int mtlNum=0, BOOL backFace=FALSE);
    float                   GetShininess(int mtlNum=0, BOOL backFace=FALSE) {return 0.0f;}
    float                   GetShinStr(int mtlNum=0, BOOL backFace=FALSE)   {return 1.0f;}

    IParamBlock2            *ipblock_defmtl;
    IParamBlock2            *ipblock_xbox;
    IParamBlock2            *ipblock_gcn;
    IParamBlock2            *ipblock_toon;
    IParamBlock2            *ipblock_ltmap;
    IParamBlock2            *ipblock_nmap;

protected:
    /**
    *   Get the specified texture. Called from GetTexturesByType().
    *   \param index of the texture to retrieve
    *   \return The RwCommTexture specified by the index.
    */
    RwCommTexture           *FindTexture(int index) const;
    RwCommTexture           *FindTexture(BitmapTex *pTexmap) const;
    bool                    IsReferenceTexture(int refNum);
    int                     ExtraTexturesReferenceToTexNum(int refNum);
    int                     ExtraTexturesTexNumToReference(int texNum);
    BitmapTex               *m_viewPortNMap;
    BitmapTex               *m_viewPortDiffuseLMap;
    BitmapTex               *m_viewPortLMap;
	
private:
    TSTR                    effFileXBox;
    TSTR                    effFileGCN;
                            
    Texmap                  *texmap[NUM_TEX_ALL];
    TexHandle               *texhandle[NUM_TEX_ALL];
                            
    float                   envmap_factor;
    float                   bumpmap_factor;
    Interval                ivalid;
    IAutoMParamDlg          *masterDlg;
	Mtl						*m_pClonedFrom;
};

// =============================================================================
// ------------------------  RwMaterialClassDesc CLASS  ------------------------
// =============================================================================

class RwMaxRwMaterialClassDesc : public ClassDesc2
{
public:
    int                         IsPublic()              {return TRUE;}
    void                        *Create(BOOL loading)   {return new RwMaxRwMaterial(loading);}
                                                        
    const TCHAR                 *Category()             {return _T("RenderWare");}
    const TCHAR                 *ClassName()            {return _T("RwMaterial");}
    const TCHAR                 *InternalName()         {return _T("RwMaterial");}
                                                        
    Class_ID                    ClassID()               {return RWMATERIAL_CLASS_ID;}
    SClass_ID                   SuperClassID()          {return MATERIAL_CLASS_ID;}
                                                        
    HINSTANCE                   HInstance()             {return hGInstance;}
};

RWMAX_API ClassDesc2            *GetRwMaterialDesc();


// =============================================================================
// ------------------------  RwMaxRwMaterialPostLoadCallback CLASS  ------------------------
// =============================================================================

class RwMaxRwMaterialPostLoadCallback : public PostLoadCallback
{
public:
        RwMaxRwMaterial *mat;

        RwMaxRwMaterialPostLoadCallback(RwMaxRwMaterial *m);
		void proc(ILoad *iload);
};


/**
*  \ingroup maxbuilder 
*
*  This class is responsible for creating an RwMaxRwMaterial object.
*
*   \see RwCommRwMaterial
*/

class RWMAX_API RwMaxRwMaterialCreator : public RwCommObjCreator, public RwMaxTraverseControllers
{
public:
    RwMaxRwMaterialCreator() : RwCommObjCreator()       {}
    virtual ~RwMaxRwMaterialCreator()                   {}

    /**
    *   This function is responsible for creating an RwMaxRwMaterial from a 3ds max Mtl.
    *   Because RwMaxRwMaterial derives from the Mtl class this function simply casts
    *   RwCommCreationData::GetMaterial to an RwCommObject.
    *
    *   \param pCreationData the creation data containing the Mtl to be created from.
    *   \param pExportOptions the export options in use
    *   \return the created RwCommObject
    */
    virtual RwCommObject        *Create(const RwCommCreationData *pCreationData,
                                    const RwCommExportOptions *pExportOptions) const;

    /**
    *   This function initialised an RwMaxRwMaterial once it has been created. It simply
    *   sets the RwCommObject name based on the 3ds max Mtl name.
    *
    *   \param pObject the RwCommObject to be initialised
    *   \param pCreationData the creation data containing the Mtl to be intialised from.
    *   \param pExportOptions the export options in use
    */
    virtual void                Init(RwCommObject *pObject,
                                    const RwCommCreationData *pCreationData,
                                    const RwCommExportOptions *pExportOptions) const;

    /**
    *   This function does nothing as the materials are attached to the parent geometry in
    *   RwMaxGeometryCreator::Attach.
    *
    *   \param pObject the RwCommObject to be attached to the scene
    *   \param pParent the parent geometry of the RwCommObject
    *   \param pScene the current RwCommScene
    *   \param pCreationData the creation data containing the Mtl.
    */
    virtual void                Attach(RwCommObject *pObject, RwCommObject *pParent, RwCommScene *pScene,
                                        const RwCommCreationData* pCreationData) const;

    /**
    *   Extracts all the controllers from the material's paramater block and tries to build them using
    *   the RwMaxTraverseControllers::BuildControl() method.
    *
    *   \param rBuilder the current RwMaxBuilder
    *   \param pCreationData the creation data containing the Mtl.
    *   \param pParent the parent geometry of the RwCommObject
    */
    virtual void                TraverseControllers(RwMaxBuilder &rBuilder,
                                    RwCommCreationData *pCreationData, RwCommObject *pParent);
};

#endif // RWMAXRWMATERIAL_H