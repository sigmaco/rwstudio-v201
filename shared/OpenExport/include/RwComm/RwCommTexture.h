// RwExpTexture.h: interface for the RwExpTexture class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPTEXTURE_H__211C8182_7B3F_441D_8F36_CF946EC591BC__INCLUDED_)
#define AFX_RWEXPTEXTURE_H__211C8182_7B3F_441D_8F36_CF946EC591BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwCommAnimatable.h"
#include "RwCommPrimitives.h"

class RwCommImage;
struct RwCommImageInfo;

const RwCommType RwCommTextureType("RwCommTexture");

/**
*  \ingroup commscenegraph  
*
*   This is the class representing a texture map. The class derives from
*   RwCommAnimatable and can be animated over time. You can extract the actual 2D
*   image from the texture using GetImage() method. The method
*   returns an RwCommImage object, encapsulating all of the image's pixel
*   data.
*
*   \see RwCommImage
*/
class RWCOMM_API RwCommTexture : public RwCommAnimatable 
{
public:

    /**
    *   This enum represents addressing modes texture flags available when mapping
    *   textures to polygons using UV texture coordinates. Flags can be ORed together.
    */
    enum TileState
    {
        /** no tiling */
        NO_TILE = 0x0,
        /** tile in U direction */
        TILE_U = 0x1,
        /** tile in V direction */
        TILE_V = 0x2,
        /** mirror in U direction */
        MIRROR_U = 0x4,
        /** mirror in V direction */
        MIRROR_V = 0x8
    };

    /** Texture filtering modes. */
    enum FilterType
    {
        NONE = 0,
        /** Point sampled */
        FILTERNEAREST,                
        /** Bilinear */        
        FILTERLINEAR,                 
        /** Point sampled per pixel mip map */        
        FILTERMIPNEAREST,             
        /** Bilinear per pixel mipmap */        
        FILTERMIPLINEAR,              
        /** MipMap interp point sampled */        
        FILTERLINEARMIPNEAREST,       
        /** Trilinear */        
        FILTERLINEARMIPLINEAR
    };

    // Animated channels
    static RwCommAnimatedChannel FileNameChannel;
    static RwCommAnimatedChannel UVChannel;

    // UV PRS animation channels
    static RwCommAnimatedChannel UVPRSChannel;
    static RwCommAnimatedChannel UVPChannel;
    static RwCommAnimatedChannel UVPxChannel;
    static RwCommAnimatedChannel UVPyChannel;
    static RwCommAnimatedChannel UVRChannel;
    static RwCommAnimatedChannel UVRxChannel;
    static RwCommAnimatedChannel UVRyChannel;
    static RwCommAnimatedChannel UVRzChannel;
    static RwCommAnimatedChannel UVSChannel;
    static RwCommAnimatedChannel UVSxChannel;
    static RwCommAnimatedChannel UVSyChannel;

    // UV frame animation channels
    static RwCommAnimatedChannel UVFramePRSChannel;
    static RwCommAnimatedChannel UVFramePChannel;
    static RwCommAnimatedChannel UVFramePxChannel;
    static RwCommAnimatedChannel UVFramePyChannel;
    static RwCommAnimatedChannel UVFrameRChannel;
    static RwCommAnimatedChannel UVFrameRxChannel;
    static RwCommAnimatedChannel UVFrameRyChannel;
    static RwCommAnimatedChannel UVFrameRzChannel;
    static RwCommAnimatedChannel UVFrameSChannel;
    static RwCommAnimatedChannel UVFrameSxChannel;
    static RwCommAnimatedChannel UVFrameSyChannel;


    RwCommTexture();
    virtual ~RwCommTexture();

    // Type methods
    virtual RwCommType  GetType() const { return RwCommTextureType; }
    virtual void        GetTypeHierarchy(RwVector<RwCommType> &vTypeVec) const;

    // Texture methods
    
    /**
     *  Get texture file name including full path.
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetFileName(RwWString &) const = 0;
    
    /**
     *  Get texture RwCommTexture::TileState.
     *  \param tile the int to be filled in with the tile state
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetTiling(int &tile) const = 0;
    
    /**
      *  Get texture cropping values.
      *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
      */
    virtual RwCommError GetCroppingValues(RwCommReal &u, RwCommReal &v,
                                          RwCommReal &w, RwCommReal &h) const = 0;
    /**
     *  Get the RwCommMatrix transformation matrix of the associated UV coordinate set.
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetUVTransform(RwCommMatrix &) const = 0;
    
    /**
     *  Get texture filter type.
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetFilter(FilterType &) const = 0;

    // Image methods
    /**
    *   Extract the actual 2D image from the texture in RwCommImage object which is
    *   encapsulating all of the image's pixel information.
    *
    *   \param image returns the image with all pixel data.
    *   \param pAlphaTexture points to a texture to be burned in the image as an
    *          alpha channel.
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError GetImage(RwCommImage &image,
                                 RwCommTexture *pAlphaTexture= NULL) = 0;
    /**
    *   Extract the actual 2D image from the texture in RwCommImage object which is
    *   encapsulating all of the image's pixel information. This method will force
    *   extracted image to have specified size
    *
    *   \param image returns the image with all pixel data.
    *   \param width of the image to be extracted.
    *   \param height of the image to be extracted.
    *   \param pAlphaTexture points to a texture to be burned in the image as an
    *          alpha channel.
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError GetImage(RwCommImage &image, int width, int height,
                                 RwCommTexture *pAlphaTexture = NULL) = 0;
    /**
    *   \param info the RwCommImageInfo to be filled in.
    *   \return Image information. This is very useful if you need some information
    *   about the image but don't want to extract the whole image using GetImage()
    */
    virtual RwCommError GetImageInfo(RwCommImageInfo &info) = 0;
};

#endif // !defined(AFX_RWEXPTEXTURE_H__211C8182_7B3F_441D_8F36_CF946EC591BC__INCLUDED_)
