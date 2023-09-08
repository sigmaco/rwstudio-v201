/*****************************************************************************
*
* File :     rphull.cpp
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

#include <rwcore.h>
#include "rphull.h"

 /****************************************************************************
 Local variables
 */
static RwInt32 rpHullSectorOffset=-1;
static RwInt32 rpHullWorldOffset=-1;
static RwFreeList *flpHulls;
static RwFreeList *flpHullTies;

#define rwID_HULLPLUGIN MAKECHUNKID(rwVENDORID_CSLRD, 0x83)

/***********************************************************************/
RpHull *
RpHullTransform(RpHull *hull, const RwMatrix *matrix)
{
	RwV3d pinplane,out,tnormal;
	RwInt32 i;

	for (i=0; i<hull->numPlanes; i++)
	{
		/* not the fastest... */
		RwV3dScale(&pinplane, &hull->planes[i].normal, hull->planes[i].d);

		/* do the normal */
		RwV3dTransformVectors(&tnormal, &hull->planes[i].normal, 1, matrix);
		RwV3dNormalize(&hull->planes[i].normal, &tnormal);

		RwV3dTransformPoints(&out, &pinplane, 1, matrix);
		hull->planes[i].d = RwV3dDotProduct(&hull->planes[i].normal, &out);
	}

    return(hull);
}

/***********************************************************************/ 
void WriteHullChunkHeader(RwStream *pStream, const RpHull *pHull)
{   
   RwInt32 chunkDataSize = 0;
 
   chunkDataSize =  sizeof(pHull->center);
   chunkDataSize += sizeof(pHull->numPlanes);
   chunkDataSize += sizeof(pHull->planes);
 
   RwStreamWriteChunkHeader (pStream, rwID_HULLPLUGIN, chunkDataSize);
}
 
/***********************************************************************/
void RpHullStreamWrite(RwStream *pStream, const RpHull *pHull)
{
   RwStreamWriteReal(pStream, (RwReal *)&pHull->center, sizeof(pHull->center));
   RwStreamWriteInt(pStream, &pHull->numPlanes, sizeof(pHull->numPlanes));
   RwStreamWriteReal(pStream, (RwReal *)pHull->planes, sizeof(pHull->planes));
}

/***********************************************************************/