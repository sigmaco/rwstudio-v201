// RwMaxTexture.h: interface for the RwMaxTexture class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWMAXTEXTURE_H__6417A8DB_041D_4715_B771_078501478379__INCLUDED_)
#define AFX_RWMAXTEXTURE_H__6417A8DB_041D_4715_B771_078501478379__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwCommTexture.h"
#include "RwCommObjCreator.h"

#include "RwMax.h"
#include "RwMaxCustomAttributes.h"
#include "RwMaxTraverseControllers.h"

/**
*  \ingroup maxscenegraph 
*
*   This is the RwMax implementation of the RwCommTexture class. The class encapsulates a Texmap object.
*
*   see RwCommTexture
*/
class RWMAX_API RwMaxTexture : public RwCommTexture, public RwMaxCustomAttributes
{
public:
	RwMaxTexture(Texmap *pTexmap);
	virtual ~RwMaxTexture();

    virtual RwCommError GetUserData(RwCommUserDataCollection &userDataColl);

    // Get attribute methods.
    virtual RwCommError GetAttribute(const RwWString &name, int &value, int index = 0);
    virtual RwCommError GetAttribute(const RwWString &name, float &value, int index = 0);
    virtual RwCommError GetAttribute(const RwWString &name, RwWString &value, int index = 0);
    virtual RwCommError GetAttributeNames(RwVector<RwWString> &vAttributeNames, RwVector<AttributeType> &vAttributeTypes);

    // Animatable methods
    RwCommError OnSetFrame(RwCommReal frame);

	// Texture methods
	virtual RwCommError GetFileName(RwWString &) const;
	virtual RwCommError GetTiling(int &tile) const;
	virtual RwCommError GetCroppingValues(	RwCommReal &u, RwCommReal &v,
					    					RwCommReal &w, RwCommReal &h ) const;
	virtual RwCommError GetUVTransform(RwCommMatrix&) const;
    /**
    *   Returns the filter type for the texture. If this is a Bitmap texture, we map
    *   the types in the following way: 
    *   \li Pyramidal -> FILTERLINEARMIPLINEAR,
    *   \li Summed Area -> FILTERLINEAR
    * 
    *   If this is not the Bitmap texture FILTERLINEAR is returned. 
    */
    virtual RwCommError GetFilter(FilterType &) const;
    virtual RwCommError GetImage(RwCommImage &image, RwCommTexture *pAlphaTexture= NULL);
    virtual RwCommError GetImage(RwCommImage &image, int width, int height, RwCommTexture *pAlphaTexture= NULL);
    virtual RwCommError GetImageInfo(RwCommImageInfo &info);

    /**
     *  Gets the Texmap object.
    *   \return the 3ds max Texmap
     */
	Texmap             *GetTexmap() { return m_pTexmap; }

    /**
     *  Adds this texture's alpha map to the given image
    *   \param image the alpha image to be added
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise
     */
    RwCommError         AddAlphaMap(RwCommImage &image) const;

protected:
    /**
    *   Convert 3ds max Bitmap to RwCommImage. Called from GetImage()
    *   \param pBitmap to convert
    *   \param image returns the result of conversion.
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise
    */
    RwCommError         BitmapToImage(Bitmap *pBitmap, RwCommImage &image) const;
    
    /**
    *   Render 3ds max procedural texture to Bitmap. Called from GetImage().
    *   \param the width to render at.
    *   \param the height to render at.
    *   \return the 3ds max Bitmap rendered.
    */
    Bitmap              *RenderBitmap(int width, int height) const;

protected:
	Texmap              *m_pTexmap;
};

/**
*  \ingroup maxbuilder 
*
*  This class is responsible for creating an RwMaxTexture object.
*
*   \see RwMaxTexture
*/
class RWMAX_API RwMaxTextureCreator  : public RwCommObjCreator, public RwMaxTraverseControllers
{
public:
	RwMaxTextureCreator();
	virtual ~RwMaxTextureCreator();

	virtual RwCommObject *Create(const RwCommCreationData *pCreationData,
								 const RwCommExportOptions *pExportOptions) const;

	virtual void Init(	RwCommObject *pObject,
						const RwCommCreationData *pCreationData,
						const RwCommExportOptions *pExportOptions) const;

	virtual void Attach(RwCommObject *pObject, RwCommObject *pParent, RwCommScene *pScene,
                        const RwCommCreationData* pCreationData) const;

	virtual void TraverseControllers(RwMaxBuilder &rBuilder, RwCommCreationData *pCreationData,
                                                                        RwCommObject *pParent);
};

#endif // !defined(AFX_RWMAXTEXTURE_H__6417A8DB_041D_4715_B771_078501478379__INCLUDED_)
