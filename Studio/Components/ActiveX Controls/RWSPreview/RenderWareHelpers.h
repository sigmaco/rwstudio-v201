#ifndef __RENDERWAREHELPERS_H__
#define __RENDERWAREHELPERS_H__

const RwUInt32 MAX_BONES = 128;
const RwReal   AXIS_SCALE = 0.25f;
extern RwIm3DVertex *WorldIm3DVertices;
extern RwReal NormalsScaleFactor;
extern RwUInt32 CurrentSector;

RpClump * DestroyClumpCallback( RpClump *c, void *d);
RpAtomic * AtomicAddBSphereCentre (RpAtomic *pAtomic, void *data);
RpAtomic * AtomicCompareBSphere (RpAtomic *atomic, void *data);
RpAtomic * AtomicMorphAddTime (RpAtomic * atomic, void *data);
RpAtomic * AtomicMorphSetTime (RpAtomic * atomic, void *data);
RpAtomic * AtomicMorphGetDuration(RpAtomic * atomic, void * data);
RpAtomic * AtomicMorphGetPosition(RpAtomic * atomic, void * data);
RpAtomic * AtomicMorphGetTime(RpAtomic * atomic, void * data);
RpAtomic * SkeletonRender(RpAtomic *atom, void *data);
RpAtomic * HasHierarchy(RpAtomic *atom, void *data);
RpAtomic * SetIsVisibleCallback(RpAtomic* pAtomic, void* pData);
RpAtomic * GeometryProperties(RpAtomic* pAtomic, void* pData);
RpAtomic * AtomicMorphGetNumMorph(RpAtomic * atomic, void * data);

void CameraSize(RwCamera *camera, RwRect *rect, RwReal viewWindow, RwReal aspectRatio);
void SetIsVisible( RpAtomic& pAtomic, bool isVisible );
void ClumpGetBoundingSphere (RpClump *clump, RwSphere *clumpSphere);
RwBool ClumpSetupInterpolators(RpClump *clump);

void InitTextPrint( void );
void DestroyTextPrint( void);
void PrintfXY (const RwInt32 x, const RwInt32 y, const char *fmt, ...);

RpWorldSector *WorldSectorCompareSize(RpWorldSector *worldSector, void *data);
void RenderWorldSectorBoundingBox(RpWorldSector *worldSector);
void RenderWorldSectorWireMesh(RpWorldSector *worldSector);
void RenderWorldSectorVertexNormals(RpWorldSector *worldSector);
RpWorldSector *SectorsRenderCallBack(RpWorldSector *worldSector, void *pData); 

// Max size of debug buffer per frame
const unsigned int MAX_DEBUG_TOOLS_BUFFER_SIZE = 2048;

// Used for setting morph position
struct MorphIncTime
{
   RwReal m_Time;
   RwBool m_bComplete;
};

// Structure used to calculate position and duration of a morph target.
struct MorphParams
{
   RwUInt32  numMorph;   // temp variable used for calculating duration and position.
   RwUInt32  numInterp;  // temp variable used for calculating duration and position.
   RwReal    duration;   // Duration of a morph target.
   RwReal    position;   // Position of a morph target.
};

// Skeletal parameters
struct SkeletonParams
{
   RwBool HasHierarchy; // Does this clump have a skeleton
   RwBool DrawSkeleton; // Is skeleton to be rendered.
   RwUInt32   NumBones; // Number of bones in skeleton.
};

// Stats pulled from a clump
struct ClumpStats
{
   RwUInt32 numAtomics;    // No of atomics in a clump.
   RwUInt32 numVertices;   // Number of vertices in clump.
   RwUInt32 numTriangles;  // Number of triangles in clump.
};

// Stats pulled from a world
struct WorldStats
{
   RwUInt32 NumSectors;                // Total number of sectors in the world.
   RwUInt32 NumVertices;               // Total number of vertices in the world.
   RwUInt32 NumTriangles;              // Total number of triangles in the world.
   RwBool   DrawSectors;               // Flag to enable drawing of world sector bounding boxes.
   RwUInt32 Currentsector;             // Current sector to render.  Used to cycling through the sectors.
   RwBool   DrawWireFrame;             // Flag to enable drawing of bounding boxes.
   RwBool   DrawNormals;               // Flag to enable drawing normals.
   RwUInt32 CurrentSectorNumVertices;  // Stores number of vertices for the current sector.
   RwUInt32 CurrentSectorNumTriangles; // Stores number of triangles for the current sector.
};

#endif // __RENDERWAREHELPERS_H__