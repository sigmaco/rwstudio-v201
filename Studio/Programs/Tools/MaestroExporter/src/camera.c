/****************************************************************************
 *
 * This file is a product of Criterion Software Ltd.
 *
 * This file is provided as is with no warranties of any kind and is
 * provided without any obligation on Criterion Software Ltd.
 * or Canon Inc. to assist in its use or modification.
 *
 * Criterion Software Ltd. and Canon Inc. will not, under any
 * circumstances, be liable for any lost revenue or other damages
 * arising from the use of this file.
 *
 * Copyright (c) 2002 Criterion Software Ltd.
 * All Rights Reserved.
 *
 */

/****************************************************************************
 *
 * camera.c
 *
 ****************************************************************************/

// Renderware Includes
#include "rwcore.h"
#include "rpworld.h"

#include "camera.h"

// ******************************************************************************
void CameraDestroy(RwCamera *camera)
{
    if( camera )
    {
        RwRaster *raster;
        RwFrame *frame;

        frame = RwCameraGetFrame(camera);
        if( frame )
        {
            RwCameraSetFrame(camera, NULL);

            RwFrameDestroy(frame);
        }

        raster = RwCameraGetRaster(camera);
        if( raster )
        {
            RwRasterDestroy(raster);

            RwCameraSetRaster(camera, NULL);
        }

        raster = RwCameraGetZRaster(camera);
        if( raster )
        {
            RwRasterDestroy(raster);

            RwCameraSetZRaster(camera, NULL);
        }

        RwCameraDestroy(camera);
    }

    return;
}

// ******************************************************************************
RwCamera *CameraCreate(RwInt32 width, RwInt32 height, RwBool zBuffer)
{
    RwCamera *camera;
    RwRaster *raster;

    camera = RwCameraCreate();
    if( camera )
    {
        RwCameraSetFrame(camera, RwFrameCreate());

        raster = RwRasterCreate(width, height, 0, rwRASTERTYPECAMERA);

        RwCameraSetRaster(camera, raster);

        raster = RwCameraGetRaster(camera);

        if( zBuffer )
        {
           raster = RwRasterCreate(width, height, 0, rwRASTERTYPEZBUFFER);

           RwCameraSetZRaster(camera, raster);

           raster = RwCameraGetRaster(camera);
        }

        /*
         * Check everything is valid...
         */
        if (RwCameraGetFrame(camera))
        {
            if (RwCameraGetRaster(camera))
            {
                if (!zBuffer || RwCameraGetZRaster(camera))
                {
                   /*
                    * Everything OK...
                    */
                   return camera;
                }
            }
        }
    }

    /*
     * Error - so clean up...
     */
    CameraDestroy(camera);

    return NULL;
}
