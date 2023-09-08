// OptTextureExporter.cpp: implementation of the OptTextureExporter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OptTextureExporter.h"

#include "RwCommImage.h"
#include "RwCommtexture.h"

#define OPTIMIZED_TEXTURE_SIZE 64

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

OptTextureExporter::OptTextureExporter()
{

}

OptTextureExporter::~OptTextureExporter()
{

}

void
OptTextureExporter::Destroy()
{
    delete this;
}

RwCommError     
OptTextureExporter::Export(RwCommObject *pObj, RwCommObject *pParent,RwCommScene *pScene,
                           const RwCommExportOptions *pExportOptions )
{
    // Call the base class implementation
    return RwExpRwTexture::Export(pObj, pParent, pScene, pExportOptions);
}

// This method should create a valid RwCommImage object. 
// The RwExpRwTexture class will create the RwTexture from this image.
RwCommError 
OptTextureExporter::ExtractImage(RwCommTexture *pTexture, RwCommTexture *pAlphaTexture, RwCommImage &image)
{
    RwCommError eResult = RWCOMM_FAIL;
    RwCommImageInfo info;
    int w = OPTIMIZED_TEXTURE_SIZE, h = OPTIMIZED_TEXTURE_SIZE;

    // Calculate new optimized texture size
    if (pTexture->GetImageInfo(info) == RWCOMM_SUCCESS)
    {
        if (info.width < OPTIMIZED_TEXTURE_SIZE)
        {
            w = info.width;
        }

        if (info.height < OPTIMIZED_TEXTURE_SIZE)
        {
            h = info.height;
        }
    }

    // Get the image, using the new size
    eResult = pTexture->GetImage(image, w, h, pAlphaTexture);

    if (eResult != RWCOMM_SUCCESS)
    {
        // The modeller failed to resize our texture, so we'll use RenderWare's RwImageCreateResample() 

        // Get the original image
        pTexture->GetImage(image, pAlphaTexture);

        // We'll convert to true color to simplify things
        image.ConvertToTrueColor();

        RwImage *pRwImage = RwCommImageToRwImage(image);
        if (pRwImage)
        {
            RwCommImage::ImageType type = image.GetType();
            int depth = image.GetDepth();

            // Convert to RwImage
            RwImage *pNewImage = RwImageCreateResample(pRwImage, w, h);

            // Get the pixels
            RwUInt8 *pixels = RwImageGetPixels(pNewImage);

            // Calc the byte size
            int nBytes = w * h * image.GetRealDepth() ;

            // Destroy the image
            image.Destroy();

            // Init the image with the new size
            image.Init(w, h, depth, type);

            // Copy image data to our image
            memcpy(image.GetPixels(), pixels, nBytes);

            // Desrtoy the RW images
            if (pRwImage)
            {
                RwImageDestroy(pRwImage);
            }

            if (pNewImage)
            {
                RwImageDestroy(pNewImage);
            }

            eResult = RWCOMM_SUCCESS;
        }

    }

    return eResult;
}
