#if !defined(MAYA_SHADER_EXAMPLE_H)
#define MAYA_SHADER_EXAMPLE_H

#include "RwCommBuilder.h"
#include "RwCommObjCreator.h"

#include "CommFurShader.h"

#include "RwMaya.h"
#include "RwMayaObjCreator.h"
#include "RwMayaCreationData.h"
#include "RwMayaBaseMaterial.h"
#include "RwMayaBlindData.h"
#include "RwMayaMaterial.h"

class MayaFurShader : public RwMayaMaterial, public CCommFurShader
{
public:

    MayaFurShader(MObject shader, MObject set);
    virtual ~MayaFurShader();

    // RwCommObject methods
    virtual RwCommType GetType() { return FurShaderType; }
    void    GetTypeHierarchy(RwVector<RwCommType> &vTypeVec) const;

    // RwCommShader methods
    virtual RwCommError GetTexturesByType(TextureType type, RwVector<RwCommTexture *> & vTextures) const;

    virtual RwCommError GetSelfIllumination(RwCommVec3 &illum)  const { return RWCOMM_NOT_IMPLEMENTED; };
    virtual RwCommError GetAmbientValue(RwCommVec3 &)           const { return RWCOMM_NOT_IMPLEMENTED; };
    virtual RwCommError GetSpecularValue(RwCommVec3 &)          const { return RWCOMM_NOT_IMPLEMENTED; };
    virtual RwCommError GetShininess(RwCommReal &)              const { return RWCOMM_NOT_IMPLEMENTED; };
    virtual RwCommError GetShineStrength(RwCommReal &)          const { return RWCOMM_NOT_IMPLEMENTED; };
    virtual RwCommError IsDoubleSided(bool &bStat)              const { return RWCOMM_NOT_IMPLEMENTED; };
    virtual RwCommError GetDiffuseValue(RwCommVec3 &color)      const { return RWCOMM_NOT_IMPLEMENTED; };
    virtual RwCommError GetTransparency(RwCommReal &trans)      const { return RWCOMM_NOT_IMPLEMENTED; };

    // CCommFurShader methods
    virtual RwCommError GetFurColor(RwCommVec3 &color) const;
};

class MayaFurShaderCreator : public RwMayaObjCreator
{
public:
    MayaFurShaderCreator();
    virtual ~MayaFurShaderCreator();

    virtual void Init(RwCommObject* pObject,
                      const RwCommCreationData *pCreationData,
                      const RwCommExportOptions *pExportOptions) const;

    virtual RwCommObject *Create(const RwCommCreationData *pCreationData,
                                 const RwCommExportOptions *pExportOptions) const;

    virtual void Attach(RwCommObject *pObject, RwCommObject *pParent, RwCommScene *pScene,
                        const RwCommCreationData *pCreationData) const;
    virtual void Traverse(RwCommBuilder &rBuilder, RwCommCreationData *pCreationData,
                          RwCommObject *pParent) const;
};

#endif // MAYA_SHADER_EXAMPLE_H