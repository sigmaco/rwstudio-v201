// RwCommImage.h: interface for the RwCommImage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWCOMMIMAGE_H__EF3E1390_94A3_4245_B57C_012448481FA9__INCLUDED_)
#define AFX_RWCOMMIMAGE_H__EF3E1390_94A3_4245_B57C_012448481FA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwComm.h"
#include "RwCommError.h"

/**
*  \ingroup commlowlevelobjects  
*
*   This class represents a 2D image. The image can have any width or height.
*   Dimensions are limited only by the available memory. Each image has a
*   specified color depth, which can be four, eight or thirty-two
*   bits-per-pixel. An image is associated with a stride. A stride defines
*   the physical number of bytes needed to get from one pixel in the image
*   to the one directly below. Four bit and eight bit image depths require
*   palettes. Palettes consist of up to 16 entries for four bit formats and
*   up to 256 entries for eight bit formats. Each entry is an RGBA value
*   defining a color. You can create an image by using the Init() method,
*   supplying a witdh, height, depth and type parameters. You can read and
*   write into the pixel buffer after you extract it using the GetPixels()
*   method. You can also read and write into the palette after you extract
*   it using the GetPalette() method. 
*
*   \see RwCommImageInfo
*/
class RWCOMM_API RwCommImage  
{
public:
    /** Image type. */ 
    enum ImageType
    {
        /** not used */
        NONE = 0,
        /** true color image */
        TRUE_COLOR = 1,
        /** palletized image */
        PALETTED = 2
    };

    RwCommImage();
    virtual ~RwCommImage();

    /**
    *   Create an image.
    *
    *   \param width of the image to create
    *   \param height of the image to create
    *   \param depth of the image to create
    *   \param type of the image to create
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    RwCommError Init(int width, int height, int depth, ImageType type);

    /** Destroy the image. */
    RwCommError Destroy();

    /** Convert from palletized 8 bit to 32bit true color image. */
    RwCommError ConvertToTrueColor();

    // 32 bit pixel methods
    /**
    *   Set the specified pixel with given color.
    *
    *   \param x horizontal position of the pixel
    *   \param y vertical position of the pixel
    *   \param pixel RGBA 4-byte pixel color
    *
    *   \note This method should be used only for true color image.
    */
    void SetPixel32(int x, int y, UINT pixel);
    /**
    *   Set the specified pixel's red component.
    *
    *   \param x horizontal position of the pixel
    *   \param y vertical position of the pixel
    *   \param value of the red pixel component.
    *
    *   \note This method should be used only for true color image.
    */
    void SetRed(int x, int y, unsigned char value);
    /**
    *   Set the specified pixel's green component.
    *
    *   \param x horizontal position of the pixel
    *   \param y vertical position of the pixel
    *   \param value of the green pixel component.
    *
    *   \note This method should be used only for true color image.
    */
    void SetGreen(int x, int y, unsigned char value);
    /**
    *   Set the specified pixel's blue component.
    *
    *   \param x horizontal position of the pixel
    *   \param y vertical position of the pixel
    *   \param value of the blue pixel component.
    *
    *   \note This method should be used only for true color image.
    */
    void SetBlue(int x, int y, unsigned char value);
    /**
    *   Set the specified pixel's alpha component.
    *
    *   \param x horizontal position of the pixel
    *   \param y vertical position of the pixel
    *   \param value of the alpha pixel component.
    *
    *   \note This method should be used only for true color image.
    */
    void SetAlpha(int x, int y, unsigned char value);

    // 8 bit pixel methods
    /**
    *   Set the specified pixel with an index in the palette.
    *
    *   \param x horizontal position of the pixel
    *   \param y vertical position of the pixel
    *   \param index in the palette.
    *
    *   \note This method should be used only for palletized images.
    */
    void SetPixel8(int x, int y, unsigned char index);
    /**
    *   Set the specified palette entry.
    *
    *   \param i palette entry to be changed.
    *   \param entry pixel RGBA 4-byte pixel color
    *
    *   \note This method should be used only for palletized images.
    */
    void SetPaletteEntry(int i, UINT entry);

    // RwCommImage data methods
    /** \return the image width. */
    int GetWidth() const { return m_width; }
    /** \return the image height. */
    int GetHeight() const { return m_height; }
    /** \return the image depth in bits. */
    int GetDepth() const { return m_depth; }
    /**
    *   \return the image depth in bytes.
    *   \note When creating an image depth is specified in bits.
    */
    int GetRealDepth() const { return m_realDepth; }
    /**
    *   \return the image stride. That is a physical number of bytes needed
    *   to get from one pixel in the image to the one directly below.
    */
    int GetStride() const { return m_stride; }
    /** \return the image type. */
    ImageType GetType() const { return m_type; }
    /**
    *   \return the image buffer. You can read and write into the
    *   pixel buffer after you extract it using this function.
    */
    unsigned char *GetPixels() const { return m_pPixels; }
    /**
    *   \return the image palette. You can read and write into the
    *   palette after you extract it using this function.
    */
    UINT *GetPalette() const { return m_pPalette; }
    /** \return true if the image was initialized using Init(). */
    bool IsValid() const { return (m_pPixels != NULL); }

protected:
    int             m_width; 
    int             m_height; 

    int             m_depth;
    int             m_realDepth;
    int             m_stride;

    unsigned char   *m_pPixels;
    UINT            *m_pPalette;

    ImageType       m_type;
};

/**
*  \ingroup commlowlevelobjects  
*
*   This structure holds image information. It's returned by RwCommTexture::GetImageInfo()
*   which is useful if you just need image information and don't want to extract the image.
*
*   \see RwCommImage
*/
struct RWCOMM_API RwCommImageInfo
{
    /** Image width */
    int width;
    /** Image height */
    int height;
    /** True if image has alpha channel */
    bool hasAlpha;
};

#endif // !defined(AFX_RWCOMMIMAGE_H__EF3E1390_94A3_4245_B57C_012448481FA9__INCLUDED_)
