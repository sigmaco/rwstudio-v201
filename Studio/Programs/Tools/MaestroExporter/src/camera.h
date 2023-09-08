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
 * Copyright (c) 2003 Criterion Software Ltd.
 * All Rights Reserved.
 *
 */

/****************************************************************************
 *
 * camera.h
 *
 ****************************************************************************/

#ifndef CAMERA_H__
#define CAMERA_H__

void CameraDestroy(RwCamera *camera);
RwCamera *CameraCreate(RwInt32 width, RwInt32 height, RwBool zBuffer);

#endif // CAMERA_H__