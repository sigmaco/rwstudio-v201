
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
 * Copyright (c) 2001 Criterion Software Ltd.
 * All Rights Reserved.
 *
 */

/****************************************************************************
 *                                                                         
 * events.c
 *
 * Copyright (C) 2001 Criterion Software Ltd
 *
 * Author: RenderWare Team
 *
 * Purpose: To illustrate how a clump's bounding-box and bounding-sphere 
 *          can be calculated.
 ****************************************************************************/

#include "rwcore.h"

#include "skeleton.h"
#include "menu.h"
#include "events.h"

#include "main.h"

static RwBool RotateClump = FALSE;
static RwBool TranslateClump = FALSE;



/*
 *****************************************************************************
 */
static RsEventStatus
HandleLeftButtonDown(RsMouseStatus *mouseStatus __RWUNUSED__)
{
    RotateClump = TRUE;

    return rsEVENTPROCESSED;
}


static RsEventStatus
HandleLeftButtonUp(RsMouseStatus *mouseStatus __RWUNUSED__)
{
    RotateClump = FALSE;

    return rsEVENTPROCESSED;
}


/*
 *****************************************************************************
 */
static RsEventStatus
HandleRightButtonDown(RsMouseStatus *mouseStatus __RWUNUSED__)
{
    TranslateClump = TRUE;

    return rsEVENTPROCESSED;
}


static RsEventStatus
HandleRightButtonUp(RsMouseStatus *mouseStatus __RWUNUSED__)
{
    TranslateClump = FALSE;

    return rsEVENTPROCESSED;
}


/*
 *****************************************************************************
 */
static RsEventStatus 
HandleMouseMove(RsMouseStatus *mouseStatus)
{
    if( RotateClump )
    {
        RwReal xAngle, yAngle;

        xAngle = mouseStatus->delta.x;
        yAngle = -mouseStatus->delta.y;

        ClumpRotate(Clump, Camera, xAngle, yAngle);
    }
    
    if( TranslateClump )
    {
        RwReal delta;

        delta = -mouseStatus->delta.y * 0.1f;
        
        ClumpTranslateZ(Clump, Camera, delta);
    }

    return rsEVENTPROCESSED;
}


/*
 *****************************************************************************
 */
static RsEventStatus 
MouseHandler(RsEvent event, void *param)
{
    /*
     * Let the menu system have a look-in first...
     */
    if( MenuMouseHandler(event, param) == rsEVENTPROCESSED )
    {
        return rsEVENTPROCESSED;
    }

    /*
     * ...then the application events, if necessary...
     */
    switch( event )
    {
        case rsLEFTBUTTONDOWN:
        {
            return HandleLeftButtonDown((RsMouseStatus *)param);
        }

        case rsLEFTBUTTONUP:
        {
            return HandleLeftButtonUp((RsMouseStatus *)param);
        }

        case rsRIGHTBUTTONDOWN:
        {
            return HandleRightButtonDown((RsMouseStatus *)param);
        }

        case rsRIGHTBUTTONUP:
        {
            return HandleRightButtonUp((RsMouseStatus *)param);
        }

        case rsMOUSEMOVE:
        {
            return HandleMouseMove((RsMouseStatus *)param);
        }

        default:
        {
            return rsEVENTNOTPROCESSED;
        }
    }
}


/*
 *****************************************************************************
 */
static RsEventStatus 
HandleKeyDown(RsKeyStatus *keyStatus __RWUNUSED__)
{
    return rsEVENTNOTPROCESSED;
}


/*
 *****************************************************************************
 */
static RsEventStatus 
HandleKeyUp(RsKeyStatus *keyStatus __RWUNUSED__)
{
    return rsEVENTNOTPROCESSED;
}


/*
 *****************************************************************************
 */
static RsEventStatus 
KeyboardHandler(RsEvent event, void *param)
{
    /*
     * Let the menu system have a look-in first...
     */
    if( MenuKeyboardHandler(event, param) == rsEVENTPROCESSED )
    {
        return rsEVENTPROCESSED;
    }

    /*
     * ...then the application events, if necessary...
     */
    switch( event )
    {
        case rsKEYDOWN:
        {
            return HandleKeyDown((RsKeyStatus *)param);
        }

        case rsKEYUP:
        {
            return HandleKeyUp((RsKeyStatus *)param);
        }

        default:
        {
            return rsEVENTNOTPROCESSED;
        }
    }
}


/*
 *****************************************************************************
 */
RwBool
AttachInputDevices(void)
{
    RsInputDeviceAttach(rsKEYBOARD, KeyboardHandler);

    RsInputDeviceAttach(rsMOUSE, MouseHandler);

    return TRUE;
}

/*
 *****************************************************************************
 */
