#include <rwcore.h>
#include <rpworld.h>

#include "skeleton.h"
#include "utils.h"

RpClump *
DffLoad(RwChar *filename)
{
    RwStream    *stream = NULL;
    RpClump     *clump = NULL;
    RwChar	    *pathName;

    /* Open stream */
    pathName = RsPathnameCreate(filename);
    stream = RwStreamOpen(rwSTREAMFILENAME, rwSTREAMREAD, pathName);
    if (stream)
    {
        /* Find clump chunk */
        if (RwStreamFindChunk(stream, rwID_CLUMP, NULL, NULL))
        {
            /* Load clump */
            clump = RpClumpStreamRead(stream);
        }

        /* Close stream */
        RwStreamClose(stream, NULL);
    }
    RsPathnameDestroy(pathName);

    return (clump);
}

/* AtomicGetBBox Function */
void
AtomicGetBBox(RpAtomic *atom, RwBBox *box)
{
    RpGeometry    *geom   = RpAtomicGetGeometry(atom);
    RwInt32        nVerts = RpGeometryGetNumVertices(geom);
    RpMorphTarget *morph  = RpGeometryGetMorphTarget(geom, 0);
    RwV3d         *verts  = RpMorphTargetGetVertices(morph);
    RwUInt16       i;

    box->inf = *verts;
    box->sup = *verts;

    for (verts++, i = 1; i < nVerts; i++, verts++)
    {
        if (verts->x < box->inf.x) box->inf.x = verts->x;
        if (verts->y < box->inf.y) box->inf.y = verts->y;
        if (verts->z < box->inf.z) box->inf.z = verts->z;
        if (verts->x > box->sup.x) box->sup.x = verts->x;
        if (verts->y > box->sup.y) box->sup.y = verts->y;
        if (verts->z > box->sup.z) box->sup.z = verts->z;
    }

    RwV3dScale(&box->sup, &box->sup, 1.09f);
    RwV3dScale(&box->inf, &box->inf, 1.09f);
}
