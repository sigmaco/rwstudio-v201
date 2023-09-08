
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
 * keyframe.h
 *
 * Copyright (C) 2001 Criterion Software Ltd
 *
 * Author: RenderWare Team
 *
 * Purpose: To demonstrate the usage of custom RpHAnimAnimation 
 *          keyframe interpolation schemes.
 * *****************************************************************************/

#ifndef KEYFRAME2_H
#define KEYFRAME2_H

#include "rwcore.h"
#include "rpworld.h"
#include "rtquat.h"
#include "rphanim.h"

#define HAnimFullKeyFrameID  0x44a07195

/*
 *****************************************************************************
 */
typedef struct HAnimFullKeyFrame  HAnimFullKeyFrame;
struct HAnimFullKeyFrame
{
    HAnimFullKeyFrame   *prevFrame;
    RwReal              time;
    RtQuat              q;
    RwV3d               pos;
    RwV3d               scale;
};

typedef struct HAnimFullInterpFrame  HAnimFullInterpFrame;
struct HAnimFullInterpFrame
{
    HAnimFullKeyFrame   *keyFrame1;
    HAnimFullKeyFrame   *keyFrame2;
    RtQuat              q;
    RwV3d               pos;
    RwV3d               scale;
};

/*
 ******************************************************************************
 */
#ifdef __cplusplus
extern "C"
{
#endif  /* __cplusplus */

extern RwBool
HAnimFullKeyFrameRegister(void);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* KEYFRAME2_H */
