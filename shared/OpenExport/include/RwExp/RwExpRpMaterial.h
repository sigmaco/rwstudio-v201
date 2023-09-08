// RwExpRpMaterial.h: interface for the RwExpRpMaterial class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPRPMATERIAL_H__3D927612_1B58_43A0_AC15_9B5D18FA4A84__INCLUDED_)
#define AFX_RWEXPRPMATERIAL_H__3D927612_1B58_43A0_AC15_9B5D18FA4A84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwExp.h"
#include "RwCommExportObject.h"
#include "RwCommRwMaterial.h"

class RwCommTexture;
class RwCommGeometry;
struct RwExpUVAnimKey;

const RwCommType RwExpRpMaterialType("RwExpRpMaterial");

/**
*  \ingroup exportobject
*   This is the class responsible for exporting RpMaterials from RwCommShaders. In its main Export() method the
*   RwExpRpMaterial class creates an RpMaterial object, extracts all the material properties from its assigned 
*   RwCommShader and sets them to the RpMaterial.
*   
*   Before the RwExpRpMaterial class creates an RpMaterial, it checks if an identical RwCommShader has
*   already been processed in the scene. If it has, it reuses its RpMaterial instance. To check if two
*   shaders are identical it uses the RwCommShader's comparison operator (operator==). Users can override
*   this operator to introduce their own shader sharing criteria.
*   
*   In addition, the RwExpRpMatrial class is also responsible for triggering the export process of its
*   relevant attached textures. Since the RwCommShader object can have an arbitrary number of textures
*   attached and the exporter might want to export a subset of these textures, the RwCommTexture objects are
*   never added to the traverse list. Instead the RwExpRpMaterial class decides what subset of textures to
*   export and for those textures it calls the ExportObject() method. This task is performed in two methods.
*   The first one, GetActiveTextures(), decides which textures to export. The second one, AttachTextures(),
*   iterates through these textures, exporting each one by calling ExportObject(), and setting its 
*   appropriate texture and MatFX attributes. Users wishing to customize the texture export behavior, can
*   override these two methods.
*/
class RWEXP_API RwExpRpMaterial  : public RwCommExportObject
{
public:
    RwExpRpMaterial();
    virtual ~RwExpRpMaterial();

    virtual RwCommType      GetType() const { return RwExpRpMaterialType; }

    virtual RwCommError     Export(RwCommObject *pObj,
                                RwCommObject *pParent,
                                RwCommScene *pScene,
                                const RwCommExportOptions *pExportOptions );

    /**
    *   This function adds the passed user data onto the material as RpUserData arrays.
    *   It iterates through the members of userDataColl and skips any which have types not equal
    *   to RwCommUserDataCollection::materialUserDataType or which have zero elements. For valid
    *   members it creates an RpUserData array on the material using RpMaterialAddUserDataArray. It then
    *   calls RwExpCommUserDataToRpUserDataArray to fill in the data.
    *
    *   \param userDataColl the user data to be added
    *
    *   \returns RWCOMM_SUCCESS
    */
    virtual RwCommError     AddUserData(RwCommUserDataCollection &userDataColl);

    /** Get the generted RpMaterial object.*/
    virtual RpMaterial      *GetRpMaterial() const { return m_pMaterial; }

    static RpMaterial       *CheckMaterialColors(RpMaterial *material, void *pData);
    static RpMaterial       *CheckMaterialFX(RpMaterial *material, void *pData);
    static RpMaterial       *CheckNormalMap(RpMaterial *material, void *pData);

protected:
    RwCommShader            *GetInstance(RwCommShader *pShader);
    RwTexture               *GetRwTexture(const RwCommTexture *pTexture);
    virtual RwCommError     AttachTextures(RwCommShader *pShader);
    virtual RwCommError     GetActiveTextures(RwCommShader *pShader,
                                RwVector<RwCommTexture *> &vActiveTextures) const;
    virtual RwCommError     SetPlatformMultiTextureEffect(RwCommShader *pShader,
                                RwCommRwMaterial::ExtensionType extType);
    virtual RwCommError     SetToonExtension(RwCommShader *pShader);
    virtual RwCommError     SetLightMapExtension(RwCommShader *pShader);
    virtual RwCommError     SetNormalMapTextures(RwCommShader *pShader);

    bool                    HasUVAnimation(RwVector<RwCommTexture *> &vTextures);
    RwCommError             ApplyUVAnimation(RwVector<RwCommTexture *> &vTextures, RwCommShader *pShader);
    virtual RpUVAnim        *CreateUVAnimation(RwSet<RwExpUVAnimKey> &sKeys, int numNodes, RwUInt32 *pUVMap, RwCommReal startTime, RwCommShader *pShader);
    RpUVAnimKeyFrame        *SetUVKeyFrame(RwExpUVAnimKey &key, RpUVAnim *pUVAnim, RwCommReal startTime, int frame,
                                RpUVAnimKeyFrame *pPrev, RwCommShader *pShader);
    void                    AddUVAnimToDict(RpUVAnim *pUVAnim);

    virtual RwCommError     AddObjectNameAsUserData(const RwWString &userDataEntry, RwCommShader *pShader);

private:
    RpMaterial  *m_pMaterial;
};

#endif // !defined(AFX_RWEXPRPMATERIAL_H__3D927612_1B58_43A0_AC15_9B5D18FA4A84__INCLUDED_)
