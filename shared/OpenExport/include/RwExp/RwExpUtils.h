// RwExpUtils.h: interface for the RwExpUtils class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPUTILS_H__17AFE7FF_F707_450A_9C27_A7D9E561C4A5__INCLUDED_)
#define AFX_RWEXPUTILS_H__17AFE7FF_F707_450A_9C27_A7D9E561C4A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <shlobj.h>

#include "RwExp.h"
#include "RwCommError.h"
#include "RwCommPrimitives.h"
#include "RwCommExporter.h"
#include "RwCommExportOptions.h"

class RwCommTransform;
class RwCommUserData;
class RwCommMesh;
class RwCommTexture;
class RwExpStreamAssetList;
class RwExpVertexCollection;
class RwCommUserDataCollection;

/**
*  \ingroup structures
*   This struct is responsible for holding clump statistics
*
*   \see RwExpGetClumpStatistics
*/
struct clumpStatistics
{
    /** The number of frame object */
    int                 numFrames;
    /** The number of light objects */
    int                 numLights;
    /** The number of camera objects */
    int                 numCameras;
    /** The number of geometry objects */
    int                 numGeometries;
    /** The number of atomic objects */
    int                 numAtomics;
    /** The number of skinned atomic objects */
    int                 numSkinnedAtomics;
    /** The number of patch objects */
    int                 numPatches;
    /** The number of triangles */
    int                 numTriangles;
    /** The number of vertex object */
    int                 numVertices;
    /** The geometry objects */
    RwSet<RpGeometry *> geometrySet;
};

/** 
*   Starts the RenderWare export engine and attaches any
*   required RW plugins.
*
*   \ingroup helpclasses
*
*   \param window is a Windows device handle to be used by RwEngineOpen. Usualy NULL
*          for the exporters.
*
*   \returns the result true for success else false.
*/
RWEXP_API bool          RwExpStartEngine(HWND window);

/** 
*   Closes down the RenderWare export engine
*
*   \ingroup helpclasses
*
*   \returns the result true for success else false.
*/
RWEXP_API bool          RwExpStopEngine();

/** 
*   Sets up a default RenderWare filesystem
*
*   \ingroup helpclasses
*
*   \returns the result true for success else false.
*/
RWEXP_API bool          RwExpSetupFileSystem();

/** 
*   Close the RenderWare filesystem
*
*   \ingroup helpclasses
*
*   \returns the result true for success else false.
*/
RWEXP_API bool          RwExpCloseFileSystem();


/** 
*   Creates a RpMaterial with the given default color.
*
*   \ingroup helpclasses
*
*   \param color the color to set the RpMaterial as default. 
*
*   \returns a pointer to the new RpMaterial.
*/
RWEXP_API RpMaterial    *RwExpTheDefaultMaterial(RwCommVec3 color);

/** 
*   Removes any duplicates from the vector and sorts it.
*
*   \ingroup helpclasses
*
*   \param vKeyFrames the vector to be processed.
*/
RWEXP_API void          RwExpSortAndRemoveDuplicates(RwVector<RwCommReal> &vKeyFrames);

/** 
*   Sets the tri-stripping method used to export RpMesh objects. The supported
*   methods are: 0 - PreProcess, 1 - PreProcess ignore winding, 2 - Tunnel,
*   3 - Tunnel ignore winding, 4 - Cache Aware. This function is called from the
*   RwExpBspExporter::SetTriStripMethod() and RwExpRpGeometry::SetTriStripMethod()
*
*   \ingroup helpclasses
*
*   \param method the tri-stripping method to use.
*   \param factor the quality value use by the tunnelling methods.
*
*   \see RwExpBspExporter::SetTriStripMethod() RwExpRpGeometry::SetTriStripMethod()
*/
RWEXP_API void          RwExpSetTriStripMethod(int method, int factor);

// Vector & matrix utils

/** 
*   Copies the contents of a RwCommVec3 object into the RGB fields of
*   a RwRGBAReal object.
*
*   \ingroup helpclasses
*
*   \param rwColor the RwRGBAReal destination object
*   \param vec3 the RwCommVec3 source object
*/
RWEXP_API void          RwExpCommVec3ToRwRGBAReal(RwRGBAReal &rwColor, RwCommVec3 &vec3);

/** 
*   Copies the contents of a RwCommVec4 object into a RwRGBAReal object. 
*
*   \ingroup helpclasses
*
*   \param rwColor the RwRGBAReal destination object 
*   \param vec4 the RwCommVec4 source object
*/
RWEXP_API void          RwExpCommVec4ToRwRGBAReal(RwRGBAReal &rwColor, RwCommVec4 &vec4);


/** 
*   Copies the contents of a RwCommMatrix object into a RwMatrix object.
*
*   \ingroup helpclasses
*
*   \param rwMatrix the RwMatrix destination object
*   \param commMatrix the RwCommMatrix source object
*   \param removeScale scaling from the RwCommMatrix will be removed while being
*    copied into the RwMatix if this flag is set.
*/
RWEXP_API void          RwExpCommMatrixToRwMatrix(RwMatrix *rwMatrix,
                           const RwCommMatrix &commMatrix, bool removeScale = true);

/** 
*   Copies the contents of a RwMatrix object into a RwCommMatrix object.
*
*   \ingroup helpclasses
*
*   \param rwMatrix the RwMatrix source object
*   \param commMatrix the RwCommMatrix destination object
*/
RWEXP_API void          RwExpRwMatrixToCommMatrix(RwCommMatrix &commMatrix, RwMatrix *rwMatrix);

/** 
*   Calculates the scaling contained within a RwMatrix object.
*
*   \ingroup helpclasses
*
*   \param rwMatrix the RwMatrix object containing the scaling info.
*
*   \returns a RwV3d containing the scaling info.
*/
RWEXP_API RwV3d         RwExpMatrixGetScale(RwMatrix *pMatrix);


/** 
*   Normalizes a RwMatrix object.
*
*   \ingroup helpclasses
*
*   \param pMatrix the RwMatrix object to be Normalized.
*
*   \returns a RwV3d containing the length of the matrix.
*/
RWEXP_API RwV3d         RwExpMatrixNormalise(RwMatrix *pMatrix);

/** 
*   Normalizes a RwCommVec3 object, removing its scale.
*
*   \ingroup helpclasses
*
*   \param vec the RwCommVec3 object to be Normalized.
*
*/
RWEXP_API void          RwExpVectorNormalise(RwCommVec3 &vec);

/** 
*   Tests if a RwCommMatrix is mirrored. It returns true if the triple product (combination of
*   the cross product and dot product that represents the volume) is negative, which indicates
*   that matrix has an odd negative scaling and the vertex order in the faces need to be inverted.
*
*   \ingroup helpclasses
*
*   \param M the RwCommMatrix to be tested
*
*   \returns true if it is mirrored else false.
*/
RWEXP_API bool          RwExpIsMirrored(const RwCommMatrix &M);

/** 
*   Perform a slerp operation between two RwCommMatrix objects
*
*   \ingroup helpclasses
*
*   \param factor the slerp factor between the two matrix objects
*
*   \returns the interpolated matrix result.
*/
RWEXP_API RwCommMatrix  RwExpSlerp(RwCommReal factor, const RwCommMatrix &M1, const RwCommMatrix &M2);

/** 
*   Extract a Quaternion from a Matrix object
*
*   \ingroup helpclasses
*
*   \param M the RwCommMatrix object to convert
*
*   \returns the quaternion vector
*/
RWEXP_API RwCommVec4    RwExpQuatFromMatrix(const RwCommMatrix &M);

/** 
*   Extract a RtQuat object from a RwMatrix object
*
*   \ingroup helpclasses
*
*   \param M the RpMatrix object to convert
*
*   \returns the RtQuat vector
*/
RWEXP_API RtQuat        RwExpRpQuatFromRwMatrix(const RwMatrix &M);

/** 
*   Tests if a RwCommMatrix is positive ortho normal. if it is not
*   it is normalized and made orthogonal. 
*
*   \ingroup helpclasses
*
*   \param M the RpCommMatrix object to test and fixup.
*
*   \returns true
*/
RWEXP_API bool          RwExpOrthoNormalize(RwCommMatrix &M);

/** 
*   Converts user data stored in an RwCommUserData object into 
*   a RenderWare RpUserDataArray object.
*
*   \ingroup helpclasses
*
*   \param commUserData the RwCommUserData source object
*   \param rwUserDataArray the RpUserDataArray destination object
*
*   \returns true
*/
RWEXP_API bool          RwExpCommUserDataToRpUserDataArray(RwCommUserData *commUserData,
                                                      RpUserDataArray *rwUserDataArray);

/** 
*   Extracts statistics from a RpClump and stores them in a
*   clumpStatistics structure
*
*   \ingroup helpclasses
*
*   \param pClump the RpClump to extract the stats from.
*   \param stats the RpClump's extracted stats.
*
*   \returns true
*/
RWEXP_API bool          RwExpGetClumpStatistics(RpClump *pClump, struct clumpStatistics &stats);

/** 
*   Output statistics from a texture dictionary through the RwCommMessage interface.
*
*   \ingroup helpclasses
*
*   \param pAssetList the RwExpStreamAssetList containing the texture dictionary
*/
RWEXP_API void          RwExpOutputTextureDictionaryStats(RwExpStreamAssetList *pAssetList);

/** 
*   Returns the given texture filter mode name as a string.
*
*   \ingroup helpclasses
*
*   \param filterMode the RwTextureFilterMode to convert to a string
*
*   \returns the texture filter mode as a string.
*/
RWEXP_API RwWString  RwExpGetNameForFilterMode(RwTextureFilterMode filterMode);

/** 
*   Returns the given user data format name as a string.
*
*   \ingroup helpclasses
*
*   \param format the RpUserDataFormat to convert to a string
*
*   \returns the user data format as a string.
*/
RWEXP_API RwWString  RwExpGetNameForUserDataFormat(enum RpUserDataFormat format);

/** 
*   Calculates the number of indices the an RpMesh contains
*
*   \ingroup helpclasses
*
*   \param mesh the RpMesh containing the indices to count.
*   \param meshHeader reserved, not used.
*   \param pData the initial value to start the count from.
*    This value is cast from a (int*).
*
*   \returns the mesh passed in.
*/
RWEXP_API RpMesh        *RwExpGetNumMeshIndices(RpMesh *mesh, RpMeshHeader *meshHeader, void *pData);

/** 
*   Overrides the default pipelines for an RpWorld object.
*
*   \ingroup helpclasses
*
*   \param world the RpWorld object containing the Static World objects
*   \param pOptions the RwCommExportOptions object containing the pipeline
*    options to override.
*/
RWEXP_API void          RwExpOverrideWorldPipes(RpWorld *world, const RwCommExportOptions *pOptions);

/** 
*   Overrides the default pipelines for an RpClump object. It will create a vector of RpExpPipeTransform
*   objects from user specified options and call RwExpOverrideAtomicPipes() for each atomic in the clump.
*
*   \ingroup helpclasses
*
*   \param clump the RpClump object containing the Animated Hierarchy objects
*   \param pOptions the RwCommExportOptions object containing the pipeline
*    options to override.
*
*   \see RwExpOverrideAtomicPipes
*/
RWEXP_API void          RwExpOverrideClumpPipes(RpClump *clump, const RwCommExportOptions *pOptions);

/** 
*   Overrides the default pipelines for an RpAtomic object. It is called from RwExpOverrideClumpPipes()
*   for each atomic and is passed the vector of pipe-transfroms defined by user user export options.
*
*   \ingroup helpclasses
*
*   \param atomic the RpAtomic object.
*   \param pData a vector of RpExpPipeTransform objects containing the pipeline
*    tranforms to override. The value is cast from a (RwVector<RpExpPipeTransform> *).
*   \returns the atomic
*
*   \see RwExpOverrideClumpPipes
*/
RWEXP_API RpAtomic      *RwExpOverrideAtomicPipes(RpAtomic *atomic, void *pData);

/** 
*   Converts a string GUID to a _rtTOCGUID object
*
*   \ingroup helpclasses
*
*   \param guid the string guid.
*   \param rtTOCGUID the _rtTOCGUID object.
*   \returns the status. RWCOMM_SUCCESS for success, else RWCOMM_FAIL.
*/
RWEXP_API RwCommError   RwExpGuidRwGuid(const RwWString &guid, _rtTOCGUID &rtTOCGUID);

/** 
*   Converts a _rtTOCGUID object to a string GUID
*
*   \ingroup helpclasses
*
*   \param rtTOCGUID the _rtTOCGUID object.
*   \param guid the string guid.
*   \returns the status. RWCOMM_SUCCESS for success, else RWCOMM_FAIL.
*/
RWEXP_API RwCommError   RwExpRwGuidToGuid(const _rtTOCGUID &rtTOCGUID, RwWString &guid);

/** 
*   Ensure that a filename is valid. The path is tested and if it does
*   not exist it is created. If the path cannot be created it will changed to
*   a temp directory.
*
*   \ingroup helpclasses
*
*   \param sFileName the files path.
*/
RWEXP_API void          RwExpEnsureValidFileName(RwWString &sFileName);

/** 
*   This function returns a triangle map which maps triangles to the original primitive
*   application index. You should use this function when you need to remap per primitive
*   data like the RwCommUserData which is ordered by primitive application index. For
*   example:
* 
*   \code
*       RwCommMesh              *pMesh;
*       RwExpVertexCollection   m_collection;
*       RwVector<int>       vPrimIndices, vTriIndices;
*
*       // ...
*
*       // Get the per primitive user data.
*       pMesh->GetPrimitiveUserData(m_UserDataColl);
*
*       // Get the initial primitive map from the collection.
*       m_collection.GetPrimitiveMap(pMesh, vPrimIndices);
*       // Change the map to trianlge map with original application indices.
*       RwExpPrimToTriMap(vPrimIndices, vTriIndices, pMesh);
*
*       // Remap the data.
*       RwCommRemapUserData(m_UserDataColl, RwCommUserDataCollection::primitiveUserDataType, vTriIndices);
*   \endcode
*
*   \ingroup helpclasses
*
*   \param primMap the initial primitive to primitive map.
*   \param triMap returns the result map
*   \param pMesh points to the mesh primitives belong to.
*/
RWEXP_API void          RwExpPrimToTriMap(const RwVector<int> &primMap, RwVector<int> &triMap,
                                          RwCommMesh *pMesh);
/**
*   This function takes a vertex collection and generates a collection of userdata
*   as required by the rtltmapcnv toolkit.
*
*   \ingroup helpclasses
*
*   \param vCollection the vertex collection to process for userdata
*   \param lightMapUserDataCol the userdata collection
*/

RWEXP_API RwCommError  RwExpCreateLightMapUserData(RwExpVertexCollection &vCollection,
                                               RwCommUserDataCollection &lightMapUserDataCol,
                                               const RwCommExporter *TheExporter,
                                               const RwCommExportOptions *pOptions);

/** 
*   This function gets the RenderWare texture name from an RwCommTexture. The
*   RenderWare texture name is the name that will be used if the texture is
*   added to a texture dictionary.
*
*   \ingroup helpclasses
*
*   \param pTexture the RwCommTexture whose name is to be returned.
*   \returns the texture name
*/
RWEXP_API RwWString  RwExpGetTextureName(RwCommTexture *pTexture);

/** 
*   This function dumps a list of lightmap images to png files for testing purposes.
*
*   \ingroup helpclasses
*
*   \param list the list of lightmap images to dump
*   \param sPath the path in which to place the texture.
*/
RWEXP_API void          RwExpDumpLightMapTextures(rtLtMapCnvImageList *imageList, const RwWString sPath);

/** 
*   This function removes a vector of textures from the current texture dictionary. RwTextureDestroy is
*   called on each texture after removal from the dictionary.
*
*   \ingroup helpclasses
*
*   \param vTextures the vector of textures to remove.
*/
RWEXP_API void          RwExpRemoveTexturesFromCurrentDictionary(const RwVector<RwTexture *> &vTextures);

/**
*   This function translates the LTCOLORDEPTH and LTMAPMIPMAP export options into a set of
*   raster flags for use with the lightmap conversion toolkit.
*
*   \ingroup helpclasses
*
*   \param pOptions the current export options
*/
RWEXP_API RwUInt32       RwExpGetLightmapRasterFlags(const RwCommExportOptions *pOptions);

#endif // !defined(AFX_RWEXPUTILS_H__17AFE7FF_F707_450A_9C27_A7D9E561C4A5__INCLUDED_)
