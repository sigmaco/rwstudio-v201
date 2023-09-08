/*****************************************************************************
*
* File :     rphull.h
*
*****************************************************************************
*
* This file is a product of Criterion Software Ltd.
*
* This file is provided as is with no warranties of any kind and is
* provided without any obligation on Criterion Software Ltd. or
* Canon Inc. to assist in its use or modification.
*
* Criterion Software Ltd. will not, under any
* circumstances, be liable for any lost revenue or other damages arising
* from the use of this file.
*
* Copyright © 1993 - 2003 Criterion Software Ltd. All rights reserved.
* All Rights Reserved.
*
* RenderWare is a trademark of Canon Inc.
*
*****************************************************************************/

#ifndef RPHULL_H
#define RPHULL_H

#include "rtintsec.h"
#include "rpcollis.h"

/****************************************************************************
 Function prototypes
 */
#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

/**********************************************/
typedef struct _plane
{
   RwV3d normal;
   RwReal d;
}
RpPlane;

#define RP_HULL_MAX_PLANES 16

typedef struct _hull
{
   void *attachedData;

   RwUInt32 timestamp;
   RwLLLink llHulls;     /* RpWorld list of hulls */

   RwV3d center;
   RwInt32 numPlanes;
   RpPlane planes[RP_HULL_MAX_PLANES];
}
RpHull;

extern RpHull *
RpHullTransform(RpHull *hull, const RwMatrix *matrix);

extern void
RpHullStreamWrite(RwStream *pStream, const RpHull *pHull);

extern void
WriteHullChunkHeader(RwStream *pStream, const RpHull *pHull);

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

#endif /* RPHULL_H */

