
#ifndef RWEXPTOON_H
#define RWEXPTOON_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwCommMesh.h"

#include "RwExp.h"

class RwExpExportOptions;
class RwCommUserDataCollection;
class RwCommExportOptions;

struct RWEXP_API RwExpMeshInks
{
    RwCommMesh                  *m_mesh;
    RwVector<RwCommToonInk> m_inks;
};

// =============================================================================
// -----------------------------  RwExpToon CLASS  -----------------------------
// =============================================================================

class RWEXP_API RwExpToon
{
public:
    RwExpToon();
    ~RwExpToon();

    // Common functions.
    static void             ApplyInkAndPaintDefaults(RpToonGeo *toonGeo,
                                                     const RwCommExportOptions *pGlobalData);
    static void             ProcessVertexThicknesses(RwCommUserDataCollection &userDataColl, int count,
                                                     RpToonGeo *toonGeo, int *vertexMap);
    
    // Bsp functions.
    static void             ProcessWorldSectorEdges(RpToonGeo *toonGeo, RpWorldSector *worldSector,
                                                    int *polygonMap);
    static bool             PointsAreCollinear(const RwV3d *a1minusa0, const RwV3d *a0, const RwV3d *b);
    static bool             LineSegmentsAreCollinear(const RwV3d *a0, const RwV3d *a1,
                                                     const RwV3d *b0, const RwV3d *b1);

private:

};

#endif // RWEXPTOON_H
