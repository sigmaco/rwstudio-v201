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
 * Copyright (c) 2000 Criterion Software Ltd.
 * All Rights Reserved.
 *
 */

/****************************************************************************
 *                                                                         
 * main.h
 *
 * Copyright (C) 2000 Criterion Software Ltd
 *
 * Author: RenderWare Team
 *
 * Purpose: RenderWare Graphics camera example.
 *          can be calculated.
 ****************************************************************************/


#ifndef MAIN_H
#define MAIN_H

#include "rwcore.h"
#include "rpworld.h"

extern RwBool Rendering;

extern RwV3d XAxis;
extern RwV3d YAxis;
extern RwV3d ZAxis;

extern RpClump *Clump;

#ifdef    __cplusplus
extern "C"
{
#endif  /* __cplusplus */


extern RpClump *ClumpRotate(RpClump *clump, RwCamera *camera, 
                            RwReal xAngle, RwReal yAngle);

extern RpClump *ClumpTranslate(RpClump *clump, RwCamera *camera, 
                               RwReal xDelta, RwReal zDelta);

extern RpClump *ClumpSetPosition(RpClump *clump, RwV3d *position);


#ifdef    __cplusplus
}
#endif  /* __cplusplus */

#endif /* MAIN_H */
