// MaxMaterial.cpp
#include "stdafx.h"

#include "Resource.h"
#include "MaxMaterial.h"

static MaxMaterialClassDesc MaxMaterialDesc;
ClassDesc2 *GetMaxMaterialDesc()
{
    return &MaxMaterialDesc;
}

// =============================================================================
// ----------------------------  HELPER CLASSES  -------------------------------
// =============================================================================

class DefaultMtlParamMap2DlgProc : public ParamMap2UserDlgProc
{
public:
        BOOL    DlgProc(TimeValue t, IParamMap2 *pmap, HWND hWnd,
                        UINT msg, WPARAM wParam, LPARAM lParam);
        void    DeleteThis()                                            {}
        void    SetThing(ReferenceTarget *m)                            {}
        void    Update(TimeValue t, Interval &valid, IParamMap2 *pmap)  {}
};

static DefaultMtlParamMap2DlgProc   MtlDialogProc;

// =============================================================================
// -------------------------  ParamBlock DESCRIPTORS  --------------------------
// =============================================================================

static ParamBlockDesc2 Pblock
(
    block_defmtl, _T("block_defmtl"), 0, 
        &MaxMaterialDesc, P_AUTO_CONSTRUCT + P_AUTO_UI, PBLOCK_REF_DEFMTL,
        IDD_MATERIAL, IDS_MAXTITLE, 0, 0, &MtlDialogProc,

        defmtl_color, _T("defmtl_color"), TYPE_RGBA, 0, IDS_DEFAULTMTLCOLOR,
            p_default, Color(1.0f,1.0f,1.0f),
            p_ui, TYPE_COLORSWATCH, IDC_DEFAULTMTLCOLOR,
        end,

        defmtl_texmap_texture, _T("defmtl_texmap_texture"), TYPE_TEXMAP,
            P_OWNERS_REF, IDS_DEFAULTMTLTEXTURE,
            p_refno, defmtl_basetex,
            p_subtexno, defmtl_basetex,
            p_ui, TYPE_TEXMAPBUTTON, IDC_DEFAULTMTLTEXTURE,
        end,
    end
);
// =============================================================================
// ----------------------------  HELPER CLASSES  -------------------------------
// =============================================================================

BOOL
DefaultMtlParamMap2DlgProc::DlgProc(TimeValue t, IParamMap2 *pmap, HWND hWnd,
                                    UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_INITDIALOG:
            return TRUE;
    }

    return FALSE;
}

// =============================================================================
// ----------------------------  RwMaterial CLASS  -----------------------------
// =============================================================================

MaxMaterial::MaxMaterial(BOOL loading)
{
    texmap[0]    = NULL;
    texhandle[0] = NULL;
    masterDlg    = NULL;
    ipblock      = NULL;

    Reset();
}

MaxMaterial::~MaxMaterial()
{

}

// =============================================================================
// -------------------------  From RwCommShader CLASS  -------------------------
// =============================================================================
RwCommError     
MaxMaterial::GetColor(RwCommVec4 &color) const
{
    Color col = ipblock->GetColor(defmtl_color);
    
    color = RwCommVec4(col.r, col.g, col.b, 1.0f);
    return RWCOMM_SUCCESS;
}

// =============================================================================
// -------------------------  From Animatable CLASS  ---------------------------
// =============================================================================

TSTR
MaxMaterial::SubAnimName(int i)
{
    if (IsReferenceTexture(i))
    {
        return GetSubTexmapTVName(ExtraTexturesReferenceToTexNum(i));
    }

    return TSTR(_T(""));
}

Animatable *
MaxMaterial::SubAnim(int i)
{
    if (texmap[0])
    {
        return texmap[0];

    }
    else
    {
        return NULL;
    }
}

int
MaxMaterial::NumParamBlocks()
{
    return (1);
}

IParamBlock2 *
MaxMaterial::GetParamBlock(int i)
{
    if (i == 0)
    {
        return ipblock;
    }
    
    return NULL;
}
 
IParamBlock2 *
MaxMaterial::GetParamBlockByID(BlockID id)
{ 
    if (ipblock->ID() == id)
    {
        return ipblock;
    }

    return NULL; 
}

// =============================================================================
// ---------------  From ReferenceMaker ReferenceTarget CLASSES  ---------------
// =============================================================================

#define MTL_HDR_CHUNK 0x4000

IOResult
MaxMaterial::Load(ILoad *iload)
{
    IOResult res;
    int id;

    while (IO_OK == (res = iload->OpenChunk()))
    {
        switch (id = iload->CurChunkID())
        {
            case MTL_HDR_CHUNK:
                res = MtlBase::Load(iload);
                break;
        }

        iload->CloseChunk();

        if (res != IO_OK)
        {
            return res;
        }
    }

    return IO_OK;
}

IOResult
MaxMaterial::Save(ISave *isave)
{
    IOResult res;
    isave->BeginChunk(MTL_HDR_CHUNK);
    res = MtlBase::Save(isave);

    if (res != IO_OK)
    {
        return res;
    }

    isave->EndChunk();

    return IO_OK;
}

void
MaxMaterial::SetReference(int i, RefTargetHandle rtarg)
{
    if (IsReferenceTexture(i))
    {
        texmap[ExtraTexturesReferenceToTexNum(i)] = static_cast<Texmap *>(rtarg);
    }
    else if (i == PBLOCK_REF_DEFMTL)
    {
        ipblock = static_cast<IParamBlock2 *>(rtarg);
    }
}

RefTargetHandle
MaxMaterial::GetReference(int i)
{
    if (IsReferenceTexture(i))
    {
        return texmap[ExtraTexturesReferenceToTexNum(i)];
    }
    else if (i == PBLOCK_REF_DEFMTL)
    {
        return ipblock;
    }

    return NULL;
}

RefTargetHandle
MaxMaterial::Clone(RemapDir &remap)
{
    int             i;
    MaxMaterial   *mnew = new MaxMaterial(FALSE);
    
    *((MtlBase *)mnew) = *((MtlBase *)this);
    mnew->ReplaceReference(PBLOCK_REF_DEFMTL, remap.CloneRef(ipblock));
    mnew->ivalid.SetEmpty();

    for (i = 0; i < NUM_TEX_ALL; i++)
    {
        mnew->texmap[i] = NULL;

        if (texmap[i])
        {
            mnew->ReplaceReference(ExtraTexturesTexNumToReference(i), remap.CloneRef(texmap[i]));
        }
    }

    BaseClone(this, mnew, remap);
    
    return (RefTargetHandle) mnew;
}

RefResult
MaxMaterial::NotifyRefChanged(Interval changeInt, RefTargetHandle hTarget,
                                PartID &partID, RefMessage message)
{
    int         i;
    ParamID     changing_param;

    switch (message)
    {
        case REFMSG_CHANGE:
            ivalid.SetEmpty();
            
            if (hTarget == ipblock)
            {
                changing_param = ipblock->LastNotifyParamID();
                Pblock.InvalidateUI(changing_param);
                Pblock.InvalidateUI(1);
            }
            else 
            {
                Pblock.InvalidateUI();
            }

            for (i = 0; i < NUM_TEX_ALL; i++)
            {
                if (hTarget == texmap[i])
                {
                    // If the texture has changed delete it's texhandle.
                    if (texhandle[i])
                    {
                        texhandle[i]->DeleteThis();
                        texhandle[i] = NULL;
                    }
                }
            }

            break;

    }
    return REF_SUCCEED;
}

// =============================================================================
// --------------------------  From ISubMap CLASS  -----------------------------
// =============================================================================

Texmap *
MaxMaterial::GetSubTexmap(int i)
{
    if (i < NUM_TEX_ALL)
    {
        return texmap[i];
    }

    return NULL;
}

void
MaxMaterial::SetSubTexmap(int i, Texmap *m)
{
    if (texhandle[i] != NULL && i == 0)
    {
        texhandle[i]->DeleteThis();
        texhandle[i] = NULL;
    }

    ReplaceReference(ExtraTexturesTexNumToReference(i), m);
    
    if (i == 0)
    {
        Pblock.InvalidateUI(1); 
    }
}

TSTR
MaxMaterial::GetSubTexmapSlotName(int i)
{
    if (i == 0)
    {
        return _T("Base Texture");
    }

    return _T("");
}

TSTR
MaxMaterial::GetSubTexmapTVName(int i)
{
    // Return i'th sub-texture name
    return GetSubTexmapSlotName(i);
}

BOOL
MaxMaterial::SetDlgThing(ParamDlg *dlg)
{
    return FALSE;
}

// =============================================================================
// ---------------------------  From MtlBase CLASS  ----------------------------
// =============================================================================

void
MaxMaterial::Reset()
{
    ivalid.SetEmpty();
    if (texmap[0])
    {
        DeleteReference(0);
        texmap[0] = NULL;
    }
    
    if (texhandle[0])
    {
        texhandle[0]->DeleteThis();
        texhandle[0] = NULL;
    }

    masterDlg = NULL;

    MaxMaterialDesc.MakeAutoParamBlocks(this);
}

void
MaxMaterial::Update(TimeValue t, Interval &valid)
{
    if (!ivalid.InInterval(t))
    {
        ivalid.SetInfinite();

        if (texmap[0])
        {
            texmap[0]->Update(t,ivalid);
        }
    }

    valid &= ivalid;
}

ULONG
MaxMaterial::LocalRequirements(int subMtlNum)
{
    ULONG req = 0;

    req |= MTLREQ_TRANSP;
    
    return req;
}

Interval
MaxMaterial::Validity(TimeValue t)
{
    Interval valid = FOREVER;

    if (texmap[0])
    {
        valid &= texmap[0]->Validity(t);
    }
    
    return valid;
}

ParamDlg*
MaxMaterial::CreateParamDlg(HWND hwMtlEdit, IMtlParams *imp)
{
    masterDlg = MaxMaterialDesc.CreateParamDlgs(hwMtlEdit, imp, this);

    return masterDlg;
}

void
MaxMaterial::SetupGfxMultiMaps(TimeValue t, Material *mtl, MtlMakerCallback &cb)
{

    int         i, j, numTexmapsUsed, numHWTexmapsSupported;
    Interval    valid = FOREVER;
    TextureInfo *texInfo;

    // Count the number of tex we have in the mtl, having in mind the
    // number of textures supported by hardware, usually two or four.
    numTexmapsUsed = 0;
    numHWTexmapsSupported = cb.NumberTexturesSupported();
    for (i = 0; ((i < NUM_TEX_ALL) && (numTexmapsUsed < numHWTexmapsSupported)); i++)
    {
        if (texmap[i])
        {
            numTexmapsUsed++;
        }
    }

    // Now set up that number and initialize texHandles to NULL.
    mtl->texture.SetCount(numTexmapsUsed);
    for (j = 0; j < numTexmapsUsed; j++)
    {
        mtl->texture[j].textHandle = NULL;
    }
    
    // Set up the texmaps.
    for (i = 0, j = 0; ((i < NUM_TEX_ALL) && (j < numTexmapsUsed)); i++)
    {
        if (texmap[i])
        {
            if (!texhandle[i])
            {
                texhandle[i] = cb.MakeHandle(texmap[i]->GetVPDisplayDIB(t, cb, valid));
            }

            // Get and initialize TextureInfo object.
            texInfo = &mtl->texture[j];
            cb.GetGfxTexInfoFromTexmap(t, mtl->texture[j], texmap[i]);

            texInfo->textHandle = texhandle[i]->GetHandle();
            texInfo->tiling[0]  = GW_TEX_REPEAT;
            texInfo->tiling[1]  = GW_TEX_REPEAT;
            texInfo->tiling[2]  = GW_TEX_REPEAT;
            texInfo->colorOp    = GW_TEX_MODULATE;
            texInfo->alphaOp    = GW_TEX_MODULATE;
            texInfo->alphaAlphaSource = GW_TEX_TEXTURE;
            
            j++;
        }
    }
}

// =============================================================================
// ----------------------------  From Mtl CLASS  -------------------------------
// =============================================================================

void
MaxMaterial::Shade(ShadeContext &sc)
{    
    int         i;
    bool        applied_texture = false;
    Color       total_light = Color (0.0, 0.0, 0.0);
    AColor      src, dest, color, texture_blend = AColor(0.0, 0.0, 0.0, 1.0);
    LightDesc   *l;

    // Render as default material.
    if (gbufID)
    {
        sc.SetGBufferID(gbufID);
    }

    // First add the ambient light and then all other lights in the scene.
    total_light += sc.ambientLight;
    
    for (i = 0; i < sc.nLights; i++)
    {
        Color           light_col;
        Point3          dir;
        register float  NL, kL;
        
        l = sc.Light(i);
        
        if (l->Illuminate(sc, sc.Normal(), light_col, dir, NL, kL))
        {
            if (l->ambientOnly)
            {
                total_light += light_col;
                continue;
            }
            
            if (NL <= 0.0f)
            {
                continue;
            }

            // Diffuse.
            if (l->affectDiffuse)
            {
                total_light += kL * light_col;
            }
        }
    }

    // Finished with calculating light, let's get the surface color.
    sc.out.c = ipblock->GetColor(defmtl_color);

    sc.out.t = Color(1.0f - ipblock->GetFloat(defmtl_color),
                     1.0f - ipblock->GetFloat(defmtl_color),
                     1.0f - ipblock->GetFloat(defmtl_color));
    
    if (sc.doMaps)
    {
        // Now the textures, we'll put everything in the texture_blend
        // and later multiply the sc.out.c with the texture_blend value.
        if (texmap[0])
        {
            applied_texture = true;
            texture_blend   = texmap[0]->EvalColor(sc);
        }

        // Before this we could iterate trough the xbox and gcn texmaps and just use
        // additive blending like this: texture_blend.r += texmap[i]->EvalColor(sc).r;
        // but that would probably ruin the look of standard matfx textures, and we
        // still have the ability to see just that texture in the preview window.
        if (applied_texture)
        {
            sc.out.c *= texture_blend;
            
            // texture_blend.a is zero based meanning zero is transparent. We return
            // temporarily to be zero based so we can multiple them, then do 1.0f
            sc.out.t = 1.0f - (1.0f - sc.out.t)*texture_blend.a;
        }
        
    }

    sc.out.c *= total_light;
}

Color
MaxMaterial::GetAmbient(int mtlNum, BOOL backFace)
{
    RwCommVec4  col;
    GetColor(col);

    return Color(col[0], col[1], col[2]);
}

Color
MaxMaterial::GetDiffuse(int mtlNum, BOOL backFace)
{
    RwCommVec4  col;
    GetColor(col);
    
    return Color(col[0], col[1], col[2]);
}

Color
MaxMaterial::GetSpecular(int mtlNum, BOOL backFace)
{
    RwCommVec4 col;
    GetColor(col);
    
    return Color(col[0], col[1], col[2]);
}

float
MaxMaterial::GetXParency(int mtlNum, BOOL backFace)
{
    RwCommVec4 col;
    GetColor(col);
    
    return 1.0f - (col[3]);
}

void
MaxMaterial::SetAmbient(Color c, TimeValue t)
{
}

void
MaxMaterial::SetDiffuse(Color c, TimeValue t)
{
}

void
MaxMaterial::SetSpecular(Color c, TimeValue t)
{
}

void
MaxMaterial::SetShininess(float v, TimeValue t)              
{
}

float 
MaxMaterial::GetShininess(int mtlNum, BOOL backFace)
{
    return 0.0f;
}

float 
MaxMaterial::GetShinStr(int mtlNum, BOOL backFace)   
{
    return 1.0f;
}

BOOL 
MaxMaterial::SupportsMultiMapsInViewport() 
{ 
    return TRUE;
}

BOOL
MaxMaterial::SupportTexDisplay()
{
    return TRUE;
}

int
MaxMaterial::NumRefs()
{
    return NUM_REFERENCES;
}

int
MaxMaterial::NumSubs()
{
    return NUM_REFERENCES;
}

void
MaxMaterial::DeleteThis()
{
    delete this;
}

Class_ID
MaxMaterial::ClassID()
{
    return FURMATERIAL_CLASS_ID;
}

SClass_ID
MaxMaterial::SuperClassID()
{
    return MATERIAL_CLASS_ID;
}

void
MaxMaterial::GetClassName(TSTR &s)
{
    s = _T("MaxFurMaterial");
}

bool
MaxMaterial::IsReferenceTexture(int refNum)
{
    if ((refNum < NUM_TEX_BASE) || 
        ((refNum >= NUM_TEX_EXTRA_OFFSET) && (refNum < NUM_REFERENCES)))
    {
        return true;
    }

    return false;
}

int
MaxMaterial::ExtraTexturesReferenceToTexNum(int refNum)
{
    if (refNum < NUM_TEX_BASE)
    {
        return refNum;
    }
    else if (refNum >= NUM_TEX_EXTRA_OFFSET)
    {
        return (refNum - NUM_TEX_EXTRA_OFFSET + NUM_TEX_BASE);
    }
    else
    {
        assert(false);
        return -1;
    }
}

int
MaxMaterial::ExtraTexturesTexNumToReference(int texNum)
{
    if (texNum < NUM_TEX_BASE)
    {
        return texNum;
    }
    else
    {
        return (texNum - NUM_TEX_BASE + NUM_TEX_EXTRA_OFFSET);
    }
}

void
MaxMaterial::DestroyTexHandles(int first, int last)
{
    if (texhandle[0])
    {
        texhandle[0]->DeleteThis();
        texhandle[0] = NULL;
    }
}

int
MaxMaterial::NumSubTexmaps()
{
    return NUM_TEX_ALL;
}
