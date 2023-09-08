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
 * startup.c
 *
 ****************************************************************************/

#include "windows.h"

// Renderware Includes
#include "rwcore.h"
#include "rpworld.h"
#include "rtpng.h"
#include "rtbmp.h"
#include "rt2d.h"
#include "rt2danim.h"

#include "startup.h"
#include "camera.h"

RwCamera *camera = 0;
RpWorld *world = 0;
RpLight *light = 0;

// ******************************************************************************
int Startup(HWND hWnd)
{
    RwEngineOpenParams openParams;
    RwBBox bb;    
    RwRGBAReal color = {0.0f, 0.0f, 0.0f, 1.0f};

    /* Start up RenderWare */
    if(!RwEngineInit(0, 0, 0))
    {
        return -1;
    }

    if (!RpWorldPluginAttach())
    {
        return -1;
    }

    openParams.displayID = hWnd;
    
    if (!RwEngineOpen(&openParams))
    {
       RwEngineTerm();
       return -1;
    }

#if defined (RWSPLATFORM_OPENGL) || \
    defined (RWSPLATFORM_DIRECTX)
    if (!RwEngineSetSubSystem(0))
    {
       RwEngineTerm();
       return -1;
    }
#endif

    if (!RwEngineStart())
    {
        RwEngineClose();
        RwEngineTerm();
        return -1;
    }

    RwImageRegisterImageFormat(RWSTRING("png"), RtPNGImageRead, RtPNGImageWrite);
    RwImageRegisterImageFormat(RWSTRING("bmp"), RtBMPImageRead, RtBMPImageWrite);

    bb.inf.x = bb.inf.y = bb.inf.z = -100.0f;
    bb.sup.x = bb.sup.y = bb.sup.z = 100.0f;

    world = RpWorldCreate(&bb);

    if (!world)
    {
        return -1;
    }

    light = RpLightCreate(rpLIGHTAMBIENT);
    
    if(!light)
    {
        return -1;
    }
    
    RpLightSetColor(light, &color);

    RpWorldAddLight(world, light);

    camera = CameraCreate(320, 240, TRUE);
    if( camera )
    {
        RpWorldAddCamera(world, camera);

        RwCameraSetNearClipPlane(camera, 0.1f);
        RwCameraSetFarClipPlane(camera, 30.0f);
    }
    
    /* Open the Tool kit */
    Rt2dOpen(camera);

    /* Open the animation tool kit */
    Rt2dAnimOpen();    

    return 0;
}

void Close(void)
{
    Rt2dAnimClose();

    Rt2dClose();

    RpWorldRemoveCamera(world, camera);

    CameraDestroy(camera);

    RpWorldRemoveLight(world, light);
    RpWorldDestroy(world);

    /* Shut down RenderWare */
    RwEngineStop();
    RwEngineClose();
    RwEngineTerm();
}

