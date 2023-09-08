
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
 * keyframe.c
 *
 * Copyright (C) 2001 Criterion Software Ltd
 *
 * Author: RenderWare Team
 *
 * Purpose: Example of implementing an animation keyframe scheme supporting
 *          animation of a matrix's scale, position and rotation
 *
 * *****************************************************************************/

#include "rwcore.h"
#include "rpworld.h"
#include "rtquat.h"
#include "rphanim.h"

#include "scalekey.h"

#define EPSILON (0.001f)

#define RwV3dInterpolate(o, a, s, b)                            \
MACRO_START                                                     \
{                                                               \
    (o)->x = (((a)->x) + ((s)) * (((b)->x) - ((a)->x)));        \
    (o)->y = (((a)->y) + ((s)) * (((b)->y) - ((a)->y)));        \
    (o)->z = (((a)->z) + ((s)) * (((b)->z) - ((a)->z)));        \
}                                                               \
MACRO_STOP

/*
 ******************************************************************************
 */
static void
HAnimFullKeyFrameToMatrix(void *mtx, void *voidIFrame)
{
    RwMatrix scaleMatrix;
    RwMatrix rotMatrix;
    RwMatrix *matrix = (RwMatrix *)mtx;
    HAnimFullInterpFrame *iFrame = (HAnimFullInterpFrame *)(voidIFrame);

    /*
     *  Convert the interpolated keyframe to a modeling matrix for the
     *  bone hierarchy.
     */
    RtQuatUnitConvertToMatrix(&iFrame->q, &rotMatrix);
    RwMatrixScale(&scaleMatrix, &iFrame->scale, rwCOMBINEREPLACE);

    RwMatrixMultiply(mtx, &scaleMatrix, &rotMatrix);
    RwV3dAssign(&matrix->pos, &iFrame->pos);
    rwMatrixSetFlags(matrix, rwMatrixGetFlags(matrix) & ~rwMATRIXINTERNALIDENTITY);

    return;
}

/*
 ******************************************************************************
 */
static void
HAnimFullKeyFrameBlend(void *voidOut, void *voidIn1, void *voidIn2, RwReal alpha)
{
    HAnimFullInterpFrame   *out = (HAnimFullInterpFrame *) voidOut;
    HAnimFullInterpFrame   *in1 = (HAnimFullInterpFrame *) voidIn1;
    HAnimFullInterpFrame   *in2 = (HAnimFullInterpFrame *) voidIn2;
    RwReal              beta, cosTheta;

    /*
     *  Perform a spherical linear interpolation of quaternions (or SLERP).
     *  More information can be found in "Advanced Animation and Rendering
     *  Techniques" by Alan Watt and Mark Watt.
     */

    /*
     *  Compute 4D dot product. This is equal to the cosine of the angle
     *  between quaternions on a hypersphere.
     */
    cosTheta = RwV3dDotProduct(&in1->q.imag, &in2->q.imag) 
                + in1->q.real * in2->q.real;

    /*
     *  If quaternions are in opposite hemispheres, then flip one.
     */
    if (cosTheta < 0.0f)
    {
        cosTheta = - cosTheta;
        RwV3dNegate(&in2->q.imag, &in2->q.imag);
        in2->q.real = - in2->q.real;
    }

    /*
     *  Set factors to do linear interpolation, as a special case where the
     *  quaternions are close together. 
     */
    beta = 1.0f - alpha;

    /*
     *  If the quaternions aren't close, proceed with spherical interpolation.
     */
    if (cosTheta < (1.0f - EPSILON))
    {
        RwReal  theta, cosecTheta;

        theta = (RwReal) RwACos(cosTheta);
        cosecTheta = 1.0f / (RwReal) RwSin(theta);
        beta  = cosecTheta * (RwReal) RwSin(beta * theta);
        alpha = cosecTheta * (RwReal) RwSin(alpha * theta);
    }

    /*
     *  Do the interpolation
     */
    out->q.imag.x = beta * in1->q.imag.x + alpha * in2->q.imag.x;
    out->q.imag.y = beta * in1->q.imag.y + alpha * in2->q.imag.y;
    out->q.imag.z = beta * in1->q.imag.z + alpha * in2->q.imag.z;
    out->q.real   = beta * in1->q.real   + alpha * in2->q.real;

    RwV3dInterpolate(&out->pos, &in1->pos, alpha, &in2->pos);
    RwV3dInterpolate(&out->scale, &in1->scale, alpha, &in2->scale);

    return;
}

/*
 ******************************************************************************
 */
static void
HAnimFullKeyFrameInterpolate(void *voidOut, void *voidIn1, void *voidIn2, 
                             RwReal time, void *customData)
{
    /* HAnimFullKeyFrame *out = (HAnimFullKeyFrame *) voidOut; */
    HAnimFullKeyFrame *in1 = (HAnimFullKeyFrame *) voidIn1;
    HAnimFullKeyFrame *in2 = (HAnimFullKeyFrame *) voidIn2;
    const RwReal fraction = ( (time - in1->time) / 
                              (in2->time - in1->time) );

    /* Treat keyframes as interp frames, since they have the same structure
     * apart from the header data, which is not involved here. 
     */
    HAnimFullKeyFrameBlend(voidOut, voidIn1, voidIn2, fraction);

    return;
}

/*
 ******************************************************************************
 */
static RwBool
HAnimFullKeyFrameStreamWrite(const RtAnimAnimation *animation, RwStream *stream)
{
    RwInt32             i;
    HAnimFullKeyFrame   *frames = (HAnimFullKeyFrame *)animation->pFrames;

    /*
     *  Write the keyframes of the given animation to the binary stream.
     */
    for (i=0; i<animation->numFrames; i++)
    {
        RwInt32     temp;

        /*
         *  Calculate memory offset for previous frame
         */
        temp = (RwInt32)frames[i].prevFrame - (RwInt32)frames;

        /*
         *  Write out time, quaternion, and offset.
         */
        if (   !RwStreamWriteReal(
                    stream, (RwReal *)&frames[i].time, 11 * sizeof(RwReal))
            || !RwStreamWriteInt(
                    stream, (RwInt32 *)&temp, sizeof(RwInt32)) )
        {
            return(FALSE);
        }
    }

    return (TRUE);
}

/*
 ******************************************************************************
 */
static RtAnimAnimation *
HAnimFullKeyFrameStreamRead(RwStream *stream, RtAnimAnimation *animation)
{
    RwInt32             i;
    HAnimFullKeyFrame  *frames;

    /*
     *  Read keyframes from a binary stream. 
     */
    frames = (HAnimFullKeyFrame *) animation->pFrames;

    for (i = 0; i < animation->numFrames; i++)
    {
        RwInt32     temp;

        /*
         *  Read the keyframe time and quaternion, then the previous frame
         *  memory offset.
         */
        if (    !RwStreamReadReal(
                    stream, (RwReal *) &frames[i].time, 11 * sizeof(RwReal))
            ||  !RwStreamReadInt(
                    stream, (RwInt32 *) &temp, sizeof(RwInt32)))
        {
            return((RtAnimAnimation *)NULL);
        }

        /*
         *  Fix up the previous frame pointer.
         */
        frames[i].prevFrame = (HAnimFullKeyFrame *) ((RwInt32)frames + temp);
    }

    return (animation);
}

/*
 ******************************************************************************
 */
static RwInt32
HAnimFullKeyFrameStreamGetSize(const RtAnimAnimation *animation)
{
    /*
     *  Stream size for keyframes of the given animation.
     */
    return (animation->numFrames * (11 * sizeof(RwReal) + sizeof(RwInt32)));
}

/*
 ******************************************************************************
 */
static void
HAnimFullKeyFrameMulRecip(void *voidAnimFrame, void *voidStartFrame)
{
    HAnimFullKeyFrame   *animFrame = (HAnimFullKeyFrame *)voidAnimFrame;
    HAnimFullKeyFrame   *startFrame = (HAnimFullKeyFrame *)voidStartFrame;
    RtQuat              recip, temp;

    /*  
     *  Multiply the first keyframe (in-place) by the reciprocal of the second.
     *  This callback will be used when generating a delta animation.
     */
    temp = animFrame->q;
    RtQuatReciprocal(&recip, &startFrame->q);
    RtQuatMultiply(&animFrame->q, &recip, &temp);

    RwV3dSub(&animFrame->pos, &animFrame->pos, &startFrame->pos);
    RwV3dSub(&animFrame->scale, &animFrame->scale, &startFrame->scale);

    return;
}

/*
 ******************************************************************************
 */
static void
HAnimFullKeyFrameAdd(void *voidOut, void *voidIn1, void *voidIn2)
{
    HAnimFullInterpFrame *out = (HAnimFullInterpFrame *)voidOut;
    HAnimFullInterpFrame *in1 = (HAnimFullInterpFrame *)voidIn1;
    HAnimFullInterpFrame *in2 = (HAnimFullInterpFrame *)voidIn2;

    /*
     *  Add two keyframes together (this is equivalent to multiplicative
     *  combination of quaternions). This callback is used for blending
     *  between hierarchies.
     */
    RtQuatMultiply(&out->q, &in1->q, &in2->q);
    RwV3dAdd(&out->pos, &in1->pos, &in2->pos);
    RwV3dAdd(&out->scale, &in1->scale, &in2->scale);

    return;
}

/*
 ******************************************************************************
 */
RwBool
HAnimFullKeyFrameRegister(void)
{
    RtAnimInterpolatorInfo     info;

    /*
     *  Register the custom keyframe interpolation scheme.
     */
    info.typeID = HAnimFullKeyFrameID;
    info.animKeyFrameSize          = sizeof(HAnimFullKeyFrame);
    info.interpKeyFrameSize        = sizeof(HAnimFullKeyFrame);
    info.keyFrameApplyCB           = HAnimFullKeyFrameToMatrix;
    info.keyFrameBlendCB           = HAnimFullKeyFrameBlend;
    info.keyFrameInterpolateCB     = HAnimFullKeyFrameInterpolate;
    info.keyFrameAddCB             = HAnimFullKeyFrameAdd;
    info.keyFrameMulRecipCB        = HAnimFullKeyFrameMulRecip;
    info.keyFrameStreamReadCB      = HAnimFullKeyFrameStreamRead;
    info.keyFrameStreamWriteCB     = HAnimFullKeyFrameStreamWrite;
    info.keyFrameStreamGetSizeCB   = HAnimFullKeyFrameStreamGetSize;
    info.customDataSize            = 0;

    if (!RtAnimRegisterInterpolationScheme(&info))
    {
        return FALSE;
    }

    return TRUE;
}

