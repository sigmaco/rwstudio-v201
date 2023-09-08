//MaxMaterial.h

#include "stdafx.h"

#include "RwCommRwMaterial.h"

#include "RwMax.h"
#include "RwMaxCustomAttributes.h"
#include "RwMaxTraverseControllers.h"

extern HINSTANCE hGInstance;

enum
{
    block_defmtl
};

enum
{
    defmtl_color,
    defmtl_texmap_texture
};

enum
{
    defmtl_basetex
};

// class id for the material.
#define FURMATERIAL_CLASS_ID Class_ID(0x239b5816, 0x2c6a4ad2)

#define NUM_TEX_ALL             1
#define NUM_REFERENCES          2
#define NUM_TEX_BASE            NUM_TEX_ALL 
#define NUM_TEX_DEFMTL          NUM_TEX_ALL
#define NUM_TEX_EXTRA_OFFSET    32
#define PBLOCK_REF_DEFMTL       NUM_TEX_BASE

// =============================================================================
// ----------------------------  RwMaterial CLASS  -----------------------------
// =============================================================================

class MaxMaterial : public Mtl
{
public:
    MaxMaterial(BOOL loading);
    virtual ~MaxMaterial();

    // RwMaxRwMaterial methods.
    void                    DeleteThis();
    void                    DestroyTexHandles(int first, int last);
    Texmap                  *GetTexmap(int texture_no);

    // From Animatable Class.
    Class_ID                ClassID();
    SClass_ID               SuperClassID();
    int                     NumSubs();
    void                    GetClassName(TSTR &s);
    TSTR                    SubAnimName(int i);
    Animatable              *SubAnim(int i);
    int                     NumParamBlocks();
    IParamBlock2            *GetParamBlock(int i);
    IParamBlock2            *GetParamBlockByID(BlockID id);

    // From ReferenceMaker, ReferenceTarget Class.
    IOResult                Load(ILoad *iload);
    IOResult                Save(ISave *isave);
    int                     NumRefs();
    void                    SetReference(int i, RefTargetHandle rtarg);
    RefTargetHandle         GetReference(int i);
    RefTargetHandle         Clone(RemapDir &remap);
    RefResult               NotifyRefChanged(Interval changeInt,
                                RefTargetHandle hTarget, PartID &partID,
                                RefMessage message);

    // From ISubMap Class.
    int                     NumSubTexmaps();
    Texmap                  *GetSubTexmap(int i);
    void                    SetSubTexmap(int i, Texmap *m);
    TSTR                    GetSubTexmapSlotName(int i);
    TSTR                    GetSubTexmapTVName(int i);
    BOOL                    SetDlgThing(ParamDlg *dlg);

    // From MtlBase Class.
    void                    Reset();
    void                    Update(TimeValue t, Interval &valid);
    BOOL                    SupportTexDisplay();
    ULONG                   LocalRequirements(int subMtlNum);
    Interval                Validity(TimeValue t);
    ParamDlg                *CreateParamDlg(HWND hwMtlEdit,
                                        IMtlParams *imp);
    BOOL                    SupportsMultiMapsInViewport();
    void                    SetupGfxMultiMaps(TimeValue t, Material *mtl,
                                                MtlMakerCallback &cb);
    // From Mtl Class.
    void                    SetAmbient(Color c, TimeValue t);
    void                    SetDiffuse(Color c, TimeValue t);
    void                    SetSpecular(Color c, TimeValue t);
    void                    SetShininess(float v, TimeValue t);
    void                    Shade(ShadeContext &sc);
    Color                   GetAmbient(int mtlNum=0, BOOL backFace=FALSE);
    Color                   GetDiffuse(int mtlNum=0, BOOL backFace=FALSE);
    Color                   GetSpecular(int mtlNum=0, BOOL backFace=FALSE);
    float                   GetXParency(int mtlNum=0, BOOL backFace=FALSE);
    float                   GetShininess(int mtlNum, BOOL backFace);
    float                   GetShinStr(int mtlNum, BOOL backFace);

    Texmap                  **GetTexmap() {return texmap;}
    RwCommError             GetColor(RwCommVec4 &color) const;

protected:
    /**
    *   Get the specified texture. Called from GetTexturesByType().
    *   \param index of the texture to retrieve
    *   \return The RwCommTexture specified by the index.
    */
    RwCommTexture           *FindTexture(int index) const;
    bool                    IsReferenceTexture(int refNum);
    int                     ExtraTexturesReferenceToTexNum(int refNum);
    int                     ExtraTexturesTexNumToReference(int texNum);

private:
    TSTR                    effFileXBox;

    IParamBlock2            *ipblock;

    Texmap                  *texmap[NUM_TEX_ALL];

    TexHandle               *texhandle[NUM_TEX_ALL];
                            
    Interval                ivalid;
    IAutoMParamDlg          *masterDlg;
};

// =============================================================================
// ------------------------  RwMaterialClassDesc CLASS  ------------------------
// =============================================================================
class MaxMaterialClassDesc : public ClassDesc2
{
public:
    int                     IsPublic()              { return TRUE; }
    void                    *Create(BOOL loading)   { return new MaxMaterial(loading); }
                                                        
    const TCHAR             *Category()             { return _T("RenderWare");}
    const TCHAR             *ClassName()            { return _T("Max Fur Material"); }
    const TCHAR             *InternalName()         { return _T("Max Fur Material"); }
                                                        
    Class_ID                ClassID()               { return FURMATERIAL_CLASS_ID; }
    SClass_ID               SuperClassID()          { return MATERIAL_CLASS_ID; }
                                                        
    HINSTANCE               HInstance()             { return hGInstance; }
};

ClassDesc2 *GetMaxMaterialDesc();
