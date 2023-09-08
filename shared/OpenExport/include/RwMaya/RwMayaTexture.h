#if !defined(RW_MAYA_TEXTURE_H)
#define RW_MAYA_TEXTURE_H

#include "RwMayaBlindData.h"

/**
*  \ingroup mayascenegraph 
*
*  This is the RwMaya implementation of RwCommTexture.
*
*   \see RwCommTexture
*   \see RwMayaBlindData
*/
class RWMAYA_API RwMayaTexture : public RwCommTexture, public RwMayaBlindData
{
public:
	RwMayaTexture(MObject object);
	virtual ~RwMayaTexture();

    virtual RwCommError OnSetFrame(RwCommReal frame);

	// Texture methods
	virtual RwCommError GetFileName(RwWString &) const;
	virtual RwCommError GetTiling(int & tile) const;
    /**
     *  This function is not implemented under RwMaya.
     *  \return RWCOMM_NOT_IMPLEMENTED.
     */
	virtual RwCommError GetCroppingValues(	RwCommReal & u, RwCommReal & v,
					    					RwCommReal & w, RwCommReal & h ) const;
	virtual RwCommError GetUVTransform(RwCommMatrix &) const;
    virtual RwCommError GetFilter(FilterType &) const;
    
    // Image methods
    /**
     *  This function is not implemented under RwMaya.
     *  \return RWCOMM_NOT_IMPLEMENTED.
     */
    virtual RwCommError GetImage(RwCommImage & image, RwCommTexture *pAlphaTexture = NULL);
    virtual RwCommError GetImage(RwCommImage & image, int width, int height, RwCommTexture *pAlphaTexture = NULL);
    virtual RwCommError GetImageInfo(RwCommImageInfo& info);

    /**
     *  Maya implementation of RwCommObject::GetUserData. Simply calls RwMayaBlindData::GetUserData.
     *  \param userDataColl the RwCommUserDataCollection in which data is to be filled.
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetUserData(RwCommUserDataCollection &userDataColl);

    // Get attribute methods.
    virtual RwCommError GetAttribute(const RwWString& name, int &value, int index = 0);
    virtual RwCommError GetAttribute(const RwWString& name, float &value, int index = 0);
    virtual RwCommError GetAttribute(const RwWString& name, RwWString &value, int index = 0);
    /**
     *  Maya implementation of RwCommAnimatable::GetAttributeNames. Simply calls RwMayaBlindData::GetObjectAttributeNames.
     *  \param vAttributeNames the names of all attributes found.
     *  \param vAttributeTypes the types of all attributes found.
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetAttributeNames(RwVector<RwWString>& vAttributeNames, RwVector<AttributeType> &vAttributeTypes);

    MObject     GetTextureObject() { return m_TexObject; }

    /**
     *  If m_pImage is NULL this function attempts to open the texture file and updates m_pImage.
     *  \return a valid ILimage pointer if successful, NULL otherwise.
     */
    ILimage     *GetILImage();
    
    /**
     *  If m_pImage is non-NULL it is closed using ILclose.
     */
    void        ReleaseILImage();
    RwCommError GetHeader();

protected:
    /**
     *  Extracts a texture file name from the "fileTextureName" attribute of m_TexObject and then attempts
     *  open an IL file handle to it (m_pImage).
     */
    bool        OpenFile();

    /**
     *  This function constructs a list of possible Maya texture locations based on a texture file name.
     *  If the file name passed contains a path it is added. The texture is also looked for in the current
     *  scene directory and the \textures and \sourceimages sub-directories.
     *  \param fileName the file name to base the possible file names on.
     *  \param possibleNames the list of possible file names.
     */
    void        ConstructPossibleFileNames(RwWString fileName, RwVector<RwWString> &possibleNames) const;

    /**
     *  This function takes a truecolor 32bpp RwCommImage and attempts to create a 4 or 6 but palettise
     *  image from it. It does not perform quantization so images with more than 256 colors will not
     *  be palettised.
     *  \param image the 32bpp RwCommImage image to be palettised..
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    RwCommError PalettiseImage(RwCommImage & image);
    bool        FileExists(const RwWString &filename) const;

	MObject     m_TexObject;
    ILimage     *m_pImage;
};


/**
*  \ingroup mayabuilder 
*
*  This class is responsible for creating an RwMayaTexture object.
*
*   \see RwMayaTexture
*   \see RwMayaObjCreator
*/
class RWMAYA_API RwMayaTextureCreator  : public RwMayaObjCreator
{
public:
	RwMayaTextureCreator();
	virtual ~RwMayaTextureCreator();

	virtual RwCommObject *Create(const RwCommCreationData *pCreationData,
								const RwCommExportOptions *pExportOptions) const;

	virtual void Init(	RwCommObject* pObject,
						const RwCommCreationData *pCreationData,
						const RwCommExportOptions *pExportOptions) const;

	virtual void Attach(RwCommObject* pObject, RwCommObject* pParent, RwCommScene* pScene,
                        const RwCommCreationData* pCreationData) const;

    virtual void Traverse(RwCommBuilder& rBuilder,
                            RwCommCreationData* pCreationData,
                            RwCommObject* pParent) const;
protected:
    RwMayaTraverseAnimations m_AnimationTraverse;
};

#endif // !defined(RW_MAYA_TEXTURE_H)
