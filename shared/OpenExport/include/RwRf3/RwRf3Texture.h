// RwRf3Texture.h: interface for the RwRf3Texture class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWRF3TEXTURE_H__4DC5400E_A3E8_4DBE_B147_67301B490B82__INCLUDED_)
#define AFX_RWRF3TEXTURE_H__4DC5400E_A3E8_4DBE_B147_67301B490B82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwRf3.h"
#include "RwRf3Object.h"
#include "RwRf3ObjCreator.h"
#include "RwRf3Param.h"
#include "RwRf3Animatable.h"
#include "RwRf3DynKeyGen.h"

#include "RwCommTexture.h"

#define RWRF3_TEXTURE_TAG               L"texture"
#define RWRF3_TEXTURE_IMAGE             L"image"
#define RWRF3_TEXTURE_UVTRANS           L"uvtransform"

#define RWRF3_TEXTURE_TILE_U            L"tileu"
#define RWRF3_TEXTURE_TILE_V            L"tilev"
#define RWRF3_TEXTURE_TILE_NONE         L"CLAMP"
#define RWRF3_TEXTURE_TILE_WRAP         L"WRAP"
#define RWRF3_TEXTURE_TILE_MIRROR       L"MIRROR"

#define RWRF3_TEXTURE_FILTER            L"filter"
#define RWRF3_TEXTURE_FILTER_NONE       L"NONE"
#define RWRF3_TEXTURE_FILTER_NEAREST    L"NEAREST"
#define RWRF3_TEXTURE_FILTER_LINEAR     L"LINEAR"
#define RWRF3_TEXTURE_FILTER_MIPNEAREST L"MIPNEAREST"
#define RWRF3_TEXTURE_FILTER_MIPLINEAR  L"MIPLINEAR"
#define RWRF3_TEXTURE_FILTER_LINEARMIPNEAREST  L"LINEARMIPNEAREST"
#define RWRF3_TEXTURE_FILTER_LINEARMIPLINEAR  L"LINEARMIPLINEAR"

#define RWRF3_TEXTURE_CROP               L"crop"

class RwMITexture;

/**
*   \ingroup rf3write
*
*   This class is responsible for exporting RwCommTexture objects. In its Export() method the class
*   exports the texture's image name (using ExportImage()), uv transform, tile state (using ExportTiling()),
*   filter mode (using ExportFilter()), and its cropping values (using ExportCropping()). The root m_pNode
*   is then attached to its parent XML shader node.
*   
*   \see RwCommTexture
*/
class RF3_API RwRf3ExpTexture  : public RwRf3ExpObject, public RwRf3ExpAnimatable
{
public:
	RwRf3ExpTexture();
	virtual ~RwRf3ExpTexture();

    virtual RwCommError     Export( RwCommObject* pObject,
                                RwCommObject* pParent,
                                RwCommScene* pScene,
                                const RwCommExportOptions* pExportOptions );

protected:

    virtual RwCommError    ExportImage(RwCommTexture *pTexture, RwWString &filename,
                                const RwCommExportOptions* pExportOptions);
    virtual RwCommError    ExportTiling(RwCommTexture *pTexture);
    virtual RwCommError    ExportFilter(RwCommTexture *pTexture);
    virtual RwCommError    ExportCropping(RwCommTexture *pTexture);

    // RwRf3ExpAnimatable methods
    virtual RwCommError SampleKey(RwCommAnimatable* pAnimatable,
        RwCommReal frame, RwCommAnimatedChannel channel, bool bOrigKey);

    virtual RwCommError CreateAnimation(bool bGenDynamicKeys, RwCommReal tolerance);

protected:

    RwRf3Param          *m_pFileName;
    RwRf3Param          *m_pUVTransform;
    RwRf3MatAnimation   m_uvXformAnimation;


};

/**
*   \ingroup rf3read
*
*   This class builds an RwMITexture from an XML node. 
*
*   \see RwMIShader
*/
class RF3_API RwRf3TextureCreator : public RwRf3ObjCreator
{
public:
    RwRf3TextureCreator() {}
    virtual ~RwRf3TextureCreator() {}

	virtual RwCommObject* Create(const RwCommCreationData* pCreationData,
								const RwCommExportOptions* pExportOptions) const;

	virtual void Attach(RwCommObject* pObject, RwCommObject* pParent, RwCommScene* pScene,
                        const RwCommCreationData* pCreationData) const;

	virtual void Init(	RwCommObject* pObject,
						const RwCommCreationData* pCreationData,
						const RwCommExportOptions* pExportOptions) const;

protected:

    virtual void BuildImage(RwMITexture *pTexture, RwRf3Param *pParam, RwRf3Builder* pBuilder) const;
    virtual void BuildUVTransform(RwMITexture *pTexture, RwRf3Param *pParam) const;
    virtual void BuildFilter(RwMITexture *pTexture, RwRf3Param *pParam) const;
    virtual void BuildTileState(RwMITexture *pTexture, RwRf3Param *pTileU, RwRf3Param *pTileV) const;
    virtual void BuildCropping(RwMITexture *pTexture, RwRf3Param *pParam) const;

    RwWString  GetAbsolutePath(const RwWString &relative, const RwWString &ref);
    bool    FileExists(const RwWString &filename) const;

};

#endif // !defined(AFX_RWRF3TEXTURE_H__4DC5400E_A3E8_4DBE_B147_67301B490B82__INCLUDED_)
