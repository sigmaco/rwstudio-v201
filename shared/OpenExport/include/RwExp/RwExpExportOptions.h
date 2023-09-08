
#ifndef RWEXPEXPORTOPTIONS_H
#define RWEXPEXPORTOPTIONS_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwCommExportOptions.h"
#include "RwCommError.h"

#include "RwExp.h"

typedef void (*RwExpSetCustomExportOptionsFunc) (RwCommExportOptions *pExportOptions);
#define SET_EXPORTOPTIONS_FUNC  "SetCustomExportOptions"

// BSP settings

/** \ingroup expopts
\def BSP_WORLDSECTORMAXPOLYS The Bsp Exporter max polygons per sector number.*/
#define             BSP_WORLDSECTORMAXPOLYS             L"Max Polys Per World Sector"

/** \ingroup expopts
\def BSP_WORLDSECTORSIZE The maximum per sector size.*/
#define             BSP_WORLDSECTORSIZE                 L"Max Sector Size"

/** \ingroup expopts
\def BSP_MAXPLANECHECK The Bsp Exporter maximum number of plane checks.*/
#define             BSP_MAXPLANECHECK                   L"Max Plane Checks"

/** \ingroup expopts
\def BSP_PARTITIONSELECTOR The Bsp Exporter partition scheme
\li 1 = Quick
\li 2 = Closed Occluder
\li 3 = Maximum Extent
\li 4 = Open occluder
\li 5 = General occluder
\li 6 = Material boundary
\li 7 = Material count
\li 8 = Partition hints
\li 0/9 = Fuzzy balanced).*/
#define             BSP_PARTITIONSELECTOR               L"Partition Scheme"


/** \ingroup expopts
\def BSP_MAXMATERIALS When the BSP partition scheme is set to Material Count this option
controls the maximum number of materials allowed per world sector.*/
#define             BSP_MAXMATERIALS                    L"Max Materials Per Sector"

/** \ingroup expopts
\def BSP_STEPUP When the BSP partition scheme is set to Open occluder or General occluder
this option controls the stetup value used.*/
#define             BSP_STEPUP                          L"Stepup"

/** \ingroup expopts
\def BSP_MAXSECTOROVERLAP The Bsp Exporter maximum sector overlap.*/
#define             BSP_MAXSECTOROVERLAP                L"Max Sector Overlap"

/** \ingroup expopts
\def BSP_WELDPOLYGONS The Bsp Exporter polygon weld option.*/
#define             BSP_WELDPOLYGONS                    L"Weld Polygons"

/** \ingroup expopts
\def BSP_DECIMATIONMODE The Bsp Exporter polygon weld approach to edge decimation.*/
#define             BSP_DECIMATIONMODE                  L"Decimation Mode"

/** \ingroup expopts
\def BSP_DECIMATIONPASSES The Bsp Exporter polygon weld number of decimation passes.*/
#define             BSP_DECIMATIONPASSES                L"Decimation Passes"

/** \ingroup expopts
\def BSP_CONVEXPARTITIONMODE The Bsp Exporter polygon weld approach to retriangulation.*/
#define             BSP_CONVEXPARTITIONMODE             L"Convex Partitioning Mode"

/** \ingroup expopts
\def BSP_POLYGONAREATHRESHOLD The Bsp Exporter polygon weld area theshold for small polygon removal.*/
#define             BSP_POLYGONAREATHRESHOLD            L"Polygon Area Threshold"

/** \ingroup expopts
\def BSP_POLYGONNORMALTHRESHOLD The Bsp Exporter normal theshold for polygon welding.*/
#define             BSP_POLYGONNORMALTHRESHOLD          L"Polygon Normal Threshold"

/** \ingroup expopts
\def BSP_POLYGONUVTHRESHOLD The Bsp Exporter uv theshold for polygon welding.*/
#define             BSP_POLYGONUVTHRESHOLD              L"Polygon UV Threshold"

/** \ingroup expopts
\def BSP_POLYGONPRELITCOLTHRESHOLD The Bsp Exporter prelight color theshold for polygon welding.*/
#define             BSP_POLYGONPRELITCOLTHRESHOLD       L"Polygon PreLitColor Threshold"

/** \ingroup expopts
\def BSP_TEXTUREWRAP The Bsp Exporter condition geometry's texture wrap option for UV realignment.*/
#define             BSP_TEXTUREWRAP                     L"Texture Wrap Mode"

/** \ingroup expopts
\def BSP_TEXTUREWRAP0 The Bsp Exporter condition geometry's texture wrap option for UV realignment on UV[0].*/
#define             BSP_TEXTUREWRAP0                    L"Texture Wrap Mode UV0"

/** \ingroup expopts
\def BSP_TEXTUREWRAP1 The Bsp Exporter condition geometry's texture wrap option for UV realignment on UV[1].*/
#define             BSP_TEXTUREWRAP1                    L"Texture Wrap Mode UV1"

/** \ingroup expopts
\def BSP_TEXTUREWRAP2 The Bsp Exporter condition geometry's texture wrap option for UV realignment on UV[2].*/
#define             BSP_TEXTUREWRAP2                    L"Texture Wrap Mode UV2"

/** \ingroup expopts
\def BSP_TEXTUREWRAP3 The Bsp Exporter condition geometry's texture wrap option for UV realignment on UV[3].*/
#define             BSP_TEXTUREWRAP3                    L"Texture Wrap Mode UV3"

/** \ingroup expopts
\def BSP_TEXTUREWRAP4 The Bsp Exporter condition geometry's texture wrap option for UV realignment on UV[4].*/
#define             BSP_TEXTUREWRAP4                    L"Texture Wrap Mode UV4"

/** \ingroup expopts
\def BSP_TEXTUREWRAP5 The Bsp Exporter condition geometry's texture wrap option for UV realignment on UV[5].*/
#define             BSP_TEXTUREWRAP5                    L"Texture Wrap Mode UV5"

/** \ingroup expopts
\def BSP_TEXTUREWRAP6 The Bsp Exporter condition geometry's texture wrap option for UV realignment on UV[6].*/
#define             BSP_TEXTUREWRAP6                    L"Texture Wrap Mode UV6"

/** \ingroup expopts
\def BSP_TEXTUREWRAP7 The Bsp Exporter condition geometry's texture wrap option for UV realignment on UV[7].*/
#define             BSP_TEXTUREWRAP7                    L"Texture Wrap Mode UV7"

/** \ingroup expopts
\def BSP_WORLDOFFSETX The Bsp Exporter X world offset.*/
#define             BSP_WORLDOFFSETX                    L"Offset X"

/** \ingroup expopts
\def BSP_WORLDOFFSETY The Bsp Exporter Y world offset.*/
#define             BSP_WORLDOFFSETY                    L"Offset Y"

/** \ingroup expopts
\def BSP_WORLDOFFSETZ The Bsp Exporter Z world offset.*/
#define             BSP_WORLDOFFSETZ                    L"Offset Z"

// Internal option
#define             BSP_EXPORTSELECTED                  L"BSP Export Selected"

// DFF settings

#define             DFF_PRELIGHTCLUMP                   L"Legacy Allocate Prelight Space"

/** \ingroup expopts
\def DFF_HANIMHIERARCHY Controls whether this clump will have an an HAnim hierarchy attached.*/
#define             DFF_HANIMHIERARCHY               L"Create RpHAnimHierarchy"

/** \ingroup expopts
\def ANM_HANIMNOMATRICES Controls whether the RpHAnimHierarchy created will have the rpHANIMHIERARCHYNOMATRICES flag set.*/
#define             ANM_HANIMNOMATRICES                 L"RpHAnimHierarchy No Matrices"

/** \ingroup expopts
\def ANM_HANIMLOCALSPACEMATRICES Controls whether the RpHAnimHierarchy created will have the rpHANIMHIERARCHYLOCALSPACEMATRICES flag set.*/
#define             ANM_HANIMLOCALSPACEMATRICES         L"RpHAnimHierarchy Local Matrices"


/** \ingroup expopts
\def DFF_MORPHTARGETS Controls whether the DFF exporter creates RpMorph targets for any
geometry animation found.*/
#define             DFF_MORPHTARGETS                    L"Export RpMorph Targets"

/** \ingroup expopts
\def DFF_MORPHSAMPLE Controls whether the DFF exporter should analyze the scene to decide
where to create keyframes or whether to use a specificed frame interval.*/
#define             DFF_MORPHSAMPLE                     L"RpMorph Sample"

/** \ingroup expopts
\def DFF_ANIMFRAMEINTERVAL The Dff Exporter morph animation interval.*/
#define             DFF_ANIMFRAMEINTERVAL               L"RpMorph Sample Interval"


/** \ingroup expopts
\def DFF_RETAINOBJINWORLDSPACE When enabled this option leaves animated hierarchies in world space rather
than transforming them to a local space.*/
#define             DFF_RETAINOBJINWORLDSPACE           L"World Space"

/** \ingroup expopts
\def DFF_BURNAXIS When enabled this option converts an animated hierarchy and its animation data in to 
the RenderWare axis system.  When disabled the exporter will apply a "fix up" rotation to the root nodes 
of hierarchies as appropriate.*/
#define             DFF_BURNAXIS                        L"Convert Axis System"

/** \ingroup expopts
\def DFF_EXPORTSKINS Controls whether the DFF exporter should export RpSkin animation for any skin modifiers
found in the scene.*/
#define             DFF_EXPORTSKINS                     L"Export Skinning"

/** \ingroup expopts
\def DFF_NUMWEIGHTS The maximum number of skin weights to export per vertex. Any additional weights
will be ignored.*/
#define             DFF_NUMWEIGHTS                      L"Max Skin Weights Per Vertex"

/** \ingroup expopts
\def DFF_EXPORTLABELS When enabled the exporter will export user defined object properties as RpUserData
on frames.*/
#define             DFF_EXPORTLABELS                    L"3ds max User Defined Properties"

/** \ingroup expopts
\def DFF_DMORPHENABLED Controls whether the DFF exporter creates RpDMorph targets for any
blend shape type geomety animation found.*/
#define             DFF_DMORPHENABLED                   L"Export RpDMorph"

/** \ingroup expopts
\def DFF_OPTIMIZEHIERARCHY The Dff Exporter optimize hierarchy option.*/
#define             DFF_OPTIMIZEHIERARCHY               L"Optimize Hierarchy"


/** \ingroup expopts
\def DFF_BEZIERENABLED The Dff Exporter export patches option.*/
#define             DFF_BEZIERENABLED                   L"Export Patches"

/** \ingroup expopts
\def DFF_ENABLE_SKIN_SPLITTING The Dff Exporter enable skin splitting.*/
#define             DFF_ENABLE_SKIN_SPLITTING           L"Enable Skin Splitting"

/** \ingroup expopts
\def DFF_BONE_LIMIT The Dff Exporter export bone limit option.*/
#define             DFF_BONE_LIMIT                      L"Bone Limit"

/** \ingroup expopts
\def DFF_TRAVERSE_ORDER Controls the order in which transforms are traversed.
\li 0 = Unsorted
\li 1 = By Name
\li 2 = By Node Tag).*/
#define             DFF_TRAVERSE_ORDER                  L"Traverse Order"

enum TransformTraverseOrder
{
    UNSORTED = 0,
    BY_NAME,
    BY_NODE_TAG
};


// ANM settings

/** \ingroup expopts
\def ANM_FILTERTOPLEVELBIPEDKEYS The Anm Exporter filter top level biped keys option.*/
#define             ANM_FILTERTOPLEVELBIPEDKEYS         L"3ds max Filter Biped Keys"

/** \ingroup expopts
\def ANM_HANIMSUBHIERARCHYANIM Controls whether the animation generated will be for the whole hierarchy or for a sub hierarchy.*/
#define             ANM_HANIMSUBHIERARCHYANIM           L"Sub Hierarchy Animation"

// Internal option
#define             ANM_DYNAMICKEYFRAMEGENERATION       L"Dynamic Keyframe Generation"

// Internal option
#define             ANM_KEYFRAMEGENERATIONTOLERANCE     L"Keyframe Generation Tolerance"

/** \ingroup expopts
\def ANM_COMPRESSION Controls whether the animation generated will be compressed.*/
#define             ANM_COMPRESSION                     L"Keyframe Compression"

enum KeyframeCompressionType
{
    NO_COMPRESSION = 0,
    FLOAT_COMPRESSION
};

// SPL settings.
/** \ingroup expopts
\def SPL_VERTEXTYPE Controls the spline exported, Edit points or Control points.*/
#define             SPL_VERTEXTYPE                      L"Spline Vertex Type"

// Common settings

/** \ingroup expopts
\def SCALEFACTOR The global scale factor.*/
#define             SCALEFACTOR                         L"Global Scale Factor"

/** \ingroup expopts
\def SCALETYPE The scale type
\li 0 = Scale By
\li 1 = Scale To).*/
#define             SCALETYPE                           L"Global Scale Type"

/** \ingroup expopts
\def TRISTRIPMESH The tri strip mesh option.*/
#define             TRISTRIPMESH                        L"Tri-strip Meshes"

/** \ingroup expopts
\def TRISTRIPMETHOD The tri strip method.
\li 0 = PreProccess
\li 1 = PreProccess ignore Winding
\li 2 = Tunnel
\li 3 = Tunnel igonre winding
\li 4 = Cache aware) */
#define             TRISTRIPMETHOD                      L"Tri-strip Method"

/** \ingroup expopts
\def TRISTRIPFACTOR The Tunnel tri strip quality (0-100).*/
#define             TRISTRIPFACTOR                      L"Tunnel Tri-strip Factor"

/** \ingroup expopts
\def LIMITUVS The Limit UVs option.*/
#define             LIMITUVS                            L"Limit UVs"

/** \ingroup expopts
\def LIMITUVS enum.*/
enum LimitUVEnum
{
    LIMITUVS_NOLIMIT = 0,
    LIMITUVS_UVLIMIT,
    LIMITUVS_TEXELLIMIT
};

/** \ingroup expopts
\def LIMITUVMIN The Limit UVs minimum range value.*/
#define             LIMITUVMIN                          L"Min Value"

/** \ingroup expopts
\def LIMITUVMAX The Limit UVs maximum range value.*/
#define             LIMITUVMAX                          L"Max Value"

/** \ingroup expopts
\def WELDVERTICES The weld vertex positions option.*/
#define             WELDVERTICES                        L"Weld Vertices"

/** \ingroup expopts
\def WELDTHRESHOLD The weld vertex threshold.*/
#define             WELDTHRESHOLD                       L"Position Weld Threshold"

/** \ingroup expopts
\def WELDTHRESHOLDUV The weld vertex UVs option.*/
#define             WELDTHRESHOLDUV                     L"UV Weld Threshold"

/** \ingroup expopts
\def WELDTHRESHOLDANGULAR The weld vertex angular option.*/
#define             WELDTHRESHOLDANGULAR                L"Angular Weld Threshold"

/** \ingroup expopts
\def WELDTHRESHOLDCOLOR The weld vertex prelit color option.*/
#define             WELDTHRESHOLDCOLOR                  L"PreLitColor Weld Threshold"

/** \ingroup expopts
\def GENERATECOLLISION The generate collision option.*/
#define             GENERATECOLLISION               L"Generate Collision"


/** \ingroup expopts
\def TWOSIDEDMATERIALS Duplicate polygons to maintain 2-sided material rendering.*/
#define             TWOSIDEDMATERIALS                   L"3ds max Export Two Sided Materials"

/** \ingroup expopts
\def EXPORTUVANIMATION Export texture uv animation.*/
#define             EXPORTUVANIMATION                   L"Export UV Animation"

/** \ingroup expopts
\def COLORVERTEXPRELIGHT The vertex pre lights option.*/
#define             COLORVERTEXPRELIGHT                 L"Vertex Prelights"

#define             TEXTURENAMECASE                     L"Texture Name Case"

/** \ingroup expopts
\def EXPORTNORMALS The vertex normals option.*/
#define             EXPORTNORMALS                       L"Vertex Normals"

/** \ingroup expopts
\def EXPORTLIT The lighting flag option.*/
#define             EXPORTLIT                           L"Lighting Flag"

// Internal option
#define             DISPLAYREPORT                       L"DISPLAYREPORT"

/** \ingroup expopts
\def PROCESSFORLIGHTMAPS This option controls the type of lightmap support. No Lightmaps: No lightmap information
**   is exported. Generate RtLtMap UVs: Export RtLtMap major axis information using an extra UV set. Lightmaps must
**   be generated after export using an external tool. Export Native Lightmaps: Export 3ds max viewport
**   extension lightmaps or Maya layered textures as RenderWare lightmaps.">
*/
#define             PROCESSFORLIGHTMAPS                 L"Process Lightmaps"

enum ProcessLightmapsEnum
{
    PROCESSLIGHTMAPS_OFF = 0,
    PROCESSLIGHTMAPS_EXPORTAXIS,
    PROCESSLIGHTMAPS_EXPORTNATIVE
};

/** \ingroup expopts
\def DEFAULTLIGHTMAPSIZE This option specifies the size of the generated lightmaps.*/
#define             DEFAULTLIGHTMAPSIZE                 L"Default Lightmap Size"

/** \ingroup expopts
\def DEFAULTLIGHTMAPBACKGROUND This option specifies the default background pattern of
**   the generated lightmaps.
*/
#define             DEFAULTLIGHTMAPBACKGROUND           L"Default Lightmap Background"

enum LightmapBackgroundEnum
{
    LIGHTMAP_BACKGROUND_CHECKERED = 0,
    LIGHTMAP_BACKGROUND_SOLIDCOLOR
};

/** \ingroup expopts
\def DEFAULTLIGHTMAPRED The default red component of the solid color lightmap background pattern.*/
#define             DEFAULTLIGHTMAPRED                  L"Default Lightmap Red"

/** \ingroup expopts
\def DEFAULTLIGHTMAPGREEN The default green component of the solid color lightmap background pattern.*/
#define             DEFAULTLIGHTMAPGREEN                L"Default Lightmap Green"

/** \ingroup expopts
\def DEFAULTLIGHTMAPBLUE The default blue component of the solid color lightmap background pattern.*/
#define             DEFAULTLIGHTMAPBLUE                 L"Default Lightmap Blue"

/** \ingroup expopts
\def LTMAPSAMPLINGFACTOR This option specifies the amount the converted lightmaps are scaled up by
**   during the conversion process before being finally sampled back down to the correct size.
*/
#define             LTMAPSAMPLINGFACTOR                 L"Sampling Factor"

/** \ingroup expopts
\def RESAMPLEIMPORTEDLTMAP If this option is set to true the native lightmaps will be resampled to the same
*    size as the conversion lightmaps before they are converted to the exported lightmaps.
*/
#define             RESAMPLEIMPORTEDLTMAP               L"Resample Imported"

/** \ingroup expopts
\def USEGREEDYLTMAP This option controls whether the edges of the lightmaps will be utilized.*/
#define             USEGREEDYLTMAP                      L"Use Greedy Rasterization"

/** \ingroup expopts
\def LTMAPREJECTIONFACTOR This option determines the minimum accepted area for a lightmaped region; any
**   area smaller will be rejected.
*/
#define             LTMAPREJECTIONFACTOR                L"Rejection Factor"

/** \ingroup expopts
\def LTMAPDENSITYFACTOR The density factor controls how texels in the source lightmaps are remapped to
**  texels in the generated lightmaps. The exporter assigns a default value that attempts to ensure
**  optimal usage. Depending on the lightmapped area of your model you may need to alter this value
**  to achieve optimal texture usage. This value can also be overidden on a per material basis using
**  the density control in the RwMaterial*/
#define             LTMAPDENSITYFACTOR                  L"Density Factor"

/** \ingroup expopts
\def DUMPLIGHMAPS This option controls whether a png version of the generated lightmap(s) should be
*    exported in the same directory as the export file.
*/
#define             DUMPLIGHMAPS                        L"Dump Lightmaps"

/** \ingroup expopts
\def LTMAPPREFIX This option controls the prefix used to generate the name of lightmap textures created.
     Maximum string length is 4. Longer strings are truncated.
*/
#define             LTMAPPREFIX                         L"Lightmap Prefix String"

/** \ingroup expopts
\def GENERATEDARKMAPS Lightmaps on the Playstation 2 are implemented differently than the other platforms
*    and are required to be inverted to Darkmaps. This option should always be set to true for Playstation
*    2 exports. */
#define             GENERATEDARKMAPS                    L"Generate Darkmaps"

/** \ingroup expopts
\def PROCESSBASETEXTURES Darkmaps for the Playstation 2 require the ‘luminance’ value to be stored inside
     the alpha channel of the base textures. This option should always be set to true for Playstation 2 exports.
*/
#define             PROCESSBASETEXTURES                 L"Process Base DarkMap Texture"

/** \ingroup expopts
\def LTMAPMIPMAP This option controls whether the lightmaps generated during the export process will be
     mipmapped or not.
*/
#define             LTMAPMIPMAP                         L"Lightmap Mipmapping"

enum LightmapColorDepthEnum
{
    LIGHTMAP_DEPTH_8888,
    LIGHTMAP_DEPTH_888,
    LIGHTMAP_DEPTH_PAL8,
    LIGHTMAP_DEPTH_PAL4
};

/** \ingroup expopts
\def LTCOLORDEPTH This option controls the color depth of lightmaps generated during the export process.
*/
#define             LTCOLORDEPTH                        L"Lightmap Color Depth"

// Internal option
#define             LTMAPDISABLECONVERSION              L"Disable Lightmap Conversion"

// Internal option
#define             EXPORTLIGHTMAPS                     L"EXPORTLIGHTMAPS"

// Internal option
#define             EXPORTSTARTTIME                     L"EXPORTSTARTTIME"

// Internal option
#define             EXPORTENDTIME                       L"EXPORTENDTIME"

// Internal option
#define             EMBEDRWSTUDIOID                     L"Embed RWStudio Identifiers"

/** \ingroup expopts
\def EXPORTUSERDATA Export user data option.*/
#define             EXPORTUSERDATA                      L"Export User Data"

/** \ingroup expopts
\def EXPORTOBJNAME Export object name as user data option.*/
#define             EXPORTOBJNAME                       L"Export Object Name"

/** \ingroup expopts
\def EXPOBJNAMEENTRY The name of the user data entry, to use with the EXPORTOBJNAME option.*/
#define             EXPOBJNAMEENTRY                     L"User Data Entry Name"

/** \ingroup expopts
\def EXPRPGEOMNAME Export object name as user data for RpGeometrys.*/
#define             EXPRPGEOMNAME                       L"Export RpGeometry Name"

/** \ingroup expopts
\def EXPRWFRAMENAME Export object name as user data for RwFrames.*/
#define             EXPRWFRAMENAME                      L"Export RwFrame Name"

/** \ingroup expopts
\def EXPRWCAMERANAME Export object name as user data for RwCameras.*/
#define             EXPRWCAMERANAME                     L"Export RwCamera Name"

/** \ingroup expopts
\def EXPRPLIGHTNAME Export object name as user data for RpLights.*/
#define             EXPRPLIGHTNAME                      L"Export RpLight Name"

/** \ingroup expopts
\def EXPRPMATERIALNAME Export object name as user data for RpMaterials.*/
#define             EXPRPMATERIALNAME                   L"Export RpMaterial Name"

/** \ingroup expopts
\def EXPRWTEXTURENAME Export object name as user data for RwTextures.*/
#define             EXPRWTEXTURENAME                    L"Export RwTexture Name"

/** \ingroup expopts
\def TOONEXPORT This option tells the exporter to process exported geometry for toon rendering*/
#define             TOONEXPORT                          L"Export Toon Data"

/** \ingroup expopts
\def TOONCREASEFROMSGS This option tells the exporter to generate crease edges on smoothing group
boundaries in the exported data.*/
#define             TOONCREASEFROMSGS                   L"Generate Crease Edges from Smoothing Groups"

/** \ingroup expopts
\def TOONDEFAULTCREASEINKNAME The default ink name used for crease edges in exported data.*/
#define             TOONDEFAULTCREASEINKNAME            L"Default Crease Ink Name"

/** \ingroup expopts
\def TOONDEFAULTSILHOUETTEINKNAME The default ink name used for silhouette edges in exported data.*/
#define             TOONDEFAULTSILHOUETTEINKNAME        L"Default Silhouette Ink Name"

/** \ingroup expopts
\def TOONDEFAULTPAINTNAME The default paint name used in exported toon data.*/
#define             TOONDEFAULTPAINTNAME                L"Default Paint Name"

enum ADCProcessingEnum
{
    ADC_PROCESSING_OFF,
    ADC_PROCESSING_ON,
    ADC_PROCESSING_ON_IGNORE_WINDING
};

/** \ingroup expopts
\def ADCPROCESSING This option controls whether RpGeometry and RpWorldSectors exported are processed
to support ADC flags. This should be set when a pipe override has been used that enables an ADC render
pipeline. Whether or not the the ADC processing should ignore winding order should be set to match the
tristripper in use.
*/
#define             ADCPROCESSING                       L"ADC Processing"

// Output Settings

/** \ingroup expopts
\def EXPORTTOLEGACYFILES Export to DFF/BSP/ANM/SPL files option.*/
#define             EXPORTTOLEGACYFILES                 L"Export Legacy Files"

/** \ingroup expopts
\def EXPORTTORWS Export to RWS file option.*/
#define             EXPORTTORWS                         L"Export RWS File"

/** \ingroup expopts
\def EXPORTTORF3 Export to RF3 file option.*/
#define             EXPORTTORF3                         L"Export RF3 File"

/** \ingroup expopts
\def RF3_SPLITBYASSET Whether to export a single RF3 file or an RF3 per asset.*/
#define             RF3_SPLITBYASSET                    L"Split By Asset"

/** \ingroup expopts
\def RF3_TEXPATH_METHOD Determines how to stream out the texture path.*/
#define             RF3_TEXPATH_METHOD                  L"Texture Path Type"

/** \ingroup expopts
\def RF3_RELATIVE_TEXPATH Specifies the texture relavtive path. (Only relevant if RF3_TEXPATH_METHOD is 1)*/
#define             RF3_RELATIVE_TEXPATH                L"Relative Texture Path"

/** \ingroup expopts
\def RF3_WAVEPATH_METHOD Determines how to stream out the wave path.*/
#define             RF3_WAVEPATH_METHOD                  L"Wave Path Type"

/** \ingroup expopts
\def RF3_RELATIVE_WAVEPATH Specifies the wave relavtive path. (Only relevant if RF3_WAVEPATH_METHOD is 1)*/
#define             RF3_RELATIVE_WAVEPATH                L"Relative Wave Path"

/** \ingroup expopts
\def RF3_REAL_AS_BINARY Streams out the RF3 real numbers in their binary representation.*/
#define             RF3_REAL_AS_BINARY                  L"Real As Binary"


/** \ingroup expopts
\def RWS_EMBEDTOC Embed TOC in RWS file option.*/
#define             RWS_EMBEDTOC                        L"Embed TOC"

/** \ingroup expopts
\def RWS_EMBEDGROUPCHUNKS Embed group chunks in the RWS file option.*/
#define             RWS_EMBEDGROUPCHUNKS                L"Embed Group Chunks"

/** \ingroup expopts
\def RWS_EMBEDTEXTUREDICT Embed texture dictionary in RWS file option.*/
#define             RWS_EMBEDTEXTUREDICT                L"Embed Texture Dictionary"

/** \ingroup expopts
\def RWS_EMBEDEFFECTDICT Embed effect dictionary in RWS file option.*/
#define             RWS_EMBEDEFFECTDICT                 L"Embed Effect Dictionary"

/** \ingroup expopts
\def RWS_EMBEDAMBIENTLIGHT Embed ambient light in RWS file option.*/
#define             RWS_EMBEDAMBIENTLIGHT               L"3ds max Embed Ambient Light"

/** \ingroup expopts
\def EXPORTABORTONERROR Determines if the export process will halt when an error occurs.*/
#define             EXPORTABORTONERROR                  L"Abort On Error"

enum EmbedAmbientEnum
{
    EMBED_AMBIENT_SMART = 0,
    EMBED_AMBIENT_TRUE,
    EMBED_AMBIENT_FALSE
};

/** \ingroup expopts
\def RWS_EXPORTBCKGCOLOR Embed background color in RWS file option.*/
#define             RWS_EXPORTBCKGCOLOR                 L"Embed Background Color"

/** \ingroup expopts
\def RWS_PREINSTANCE Whether exported data should be preinstanced.*/
#define             RWS_PREINSTANCE                     L"Pre Instance"

/** \ingroup expopts
\def RWS_PREINSTANCE Whether preinstancing should be performed locally only or also on remote console targets.*/
#define             RWS_LOCAL_PREINSTANCE               L"Pre Instance Locally Only"

// Internal option
#define             GLB_VERBOSITY                       L"VERBOSITY"

/** \ingroup expopts
\def GLB_VISPARTITIONS The visualize partitions option.*/
#define             GLB_VISPARTITIONS                   L"Visualize Partitions"

/** \ingroup expopts
\def ASSETTYPE The type of asset being exported.*/
#define             ASSETTYPE                           L"Asset Type"

/** \ingroup expopts
\def CANPREINSTANCE Whether the asset can be pre instanced or not. */
#define             ASSETCANPREINSTANCE                 L"Can Pre Instance"

#define             RF3_TEST                            L"Test RF3"
#define             EXPORTTOVIEW                        L"Export for visualization"

// Internal option
#define				CURRENTPLATFORM						L"Current Platform"

enum AssetType
{
    HANIMATION_ASSET = 0,
    WORLD_ASSET,
    CLUMP_ASSET,
    SPLINE_ASSET,
	AUDIOWAVEDICT_ASSET,
    AUDIOEVENTDICT_ASSET,
	AUDIOSTREAM_ASSET,
    USERDEFINED_ASSET_RANGE
};

#define             SHOW_EXPORT_WIZARD                  L"Show Export Wizard"

// Per object settings
#define             RWEXP_LIGHT_FLAG                    _L("RwLightFlag")
#define             RWEXP_EXPORT_NORMAL_STRING          _L("RwNormalsFlag")
#define             RWEXP_EXPORT_PRELIGHTS_STRING       _L("RwPrelightsFlag")
#define             RWEXP_MERGE_GROUP_STRING            _L("RwMergeGroup")
#define             RWEXP_TOON_EXPORT_STRING            _L("RwToonFlag")
#define             RWEXP_TOON_CREASES_STRING           _L("RwToonCreasesFlag")

// Audio export option
#define				RWAWAVDICT_INCLUDENAMES				L"Embed Wave Dictionary Names"
#define				RWAWAVDICT_INCLUDEUUIDS				L"Embed Wave Dictionary UUIDs"

#define				RWASTREAM_INCLUDENAMES				L"Embed Stream Names"
#define				RWASTREAM_INCLUDEUUIDS				L"Embed Stream UUIDs"
#define				RWASTREAM_MEDIASECTORSIZE			L"Media Sector Size"
#define             RWASTREAM_DEFAULTPACKETSIZE         L"Default Stream Packet Size"

#define             RWAEVENTDICT_MUTE                   L"Mute"
#define             RWAEVENTDICT_GAIN                   L"Gain"
#define             RWAEVENTDICT_PITCH                  L"Pitch"
#define             RWAEVENTDICT_GAINENABLED            L"Gain Enabled"
#define             RWAEVENTDICT_PITCHENABLED           L"Pitch Enabled"

#define             RWAEVENT_DEFAULTMUTE                L"Default Mute"
#define             RWAEVENT_DEFAULTGAINRANGE           L"Default Gain Range"
#define             RWAEVENT_DEFAULTGAINOFFSET          L"Default Gain Offset"
#define             RWAEVENT_DEFAULTGAINVARENABLED      L"Default Gain Variation Enabled"
#define             RWAEVENT_DEFAULTPITCHRANGE          L"Default Pitch Range"
#define             RWAEVENT_DEFAULTPITCHOFFSET         L"Default Pitch Offset"
#define             RWAEVENT_DEFAULTPITCHVARENABLED     L"Default Pitch Variation Enabled"
	
#define             RWAWAVE_DEFAULT3D                   L"Default Wave 3D"
#define             RWAWAVE_DEFAULTDEFNAME              L"Default Wave Definition Name"

#define             RWASRCFORMAT_OVERRIDE               L"Override Source Format"
#define             RWASRCFORMAT_DEFAULTDATATYPE        L"Default Source Data Type"
#define             RWASRCFORMAT_DEFAULTBITDEPTH        L"Default Source Bit Depth"
#define             RWASRCFORMAT_DEFAULTSAMPLERATE      L"Default Source Sample Rate"
#define             RWASRCFORMAT_DEFAULTNUMCHANNELS     L"Default Source Num Channels"
#define             RWASRCFORMAT_DEFAULTFRAMESIZE       L"Default Source Frame Size"
#define             RWASRCFORMAT_DEFAULTSAMPLEPERFRAME  L"Default Source Samples Per Frame"
#define             RWASRCFORMAT_DEFAULTSIGNED          L"Default Source Signed"

#define             RWATARFORMAT_OVERRIDE               L"Override Target Format"
#define             RWATARFORMAT_DEFAULTDATATYPE        L"Default Target Data Type"
#define             RWATARFORMAT_DEFAULTBITDEPTH        L"Default Target Bit Depth"
#define             RWATARFORMAT_DEFAULTSAMPLERATE      L"Default Target Sample Rate"
#define             RWATARFORMAT_DEFAULTNUMCHANNELS     L"Default Target Num Channels"
#define             RWATARFORMAT_DEFAULTFRAMESIZE       L"Default Target Frame Size"
#define             RWATARFORMAT_DEFAULTSAMPLEPERFRAME  L"Default Target Samples Per Frame"
#define             RWATARFORMAT_DEFAULTSIGNED          L"Default Target Signed"

RWEXP_API RwCommError RwExpInitArtistOptions(RwCommExportOptions &options, int type);

#endif // RWEXPEXPORTOPTIONS_H
