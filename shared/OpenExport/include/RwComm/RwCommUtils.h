
#ifndef RWCOMMUTILS_H
#define RWCOMMUTILS_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwComm.h"
#include "RwCommUserData.h"

struct RwCommVertexId;

/**
*   Remaps the user data in the collection using the map provided. If the size of the map
*   is bigger then the user data arrays, they will be expanded in the process of remapping.
*
*   \ingroup commhelpers
*
*   \param userDataColl collection of user data to remap.
*   \param dataType type of user data to remap
*   \param dataMap is the map to use for remapping
*
*   \return the status. RWCOMM_SUCCESS for success, else RWCOMM_FAIL.
*
*   \see RwExpPrimToTriMap
*/
RWCOMM_API RwCommError  RwCommRemapUserData(RwCommUserDataCollection &userDataColl,
                                            RwCommUserDataCollection::Type dataType,
                                            RwVector<int> &dataMap);
/**
*   Rounds all the elements of the vector.
*
*   \ingroup commhelpers
*
*   \param vec2 vector to round
*   \param precision to round by
*
*   \return the status. RWCOMM_SUCCESS for success, else RWCOMM_FAIL.
*/
RWCOMM_API RwCommError  RwCommRoundVec2(RwCommVec2 &vec2, RwCommReal precision);

/**
*   Rounds all the elements of the vector.
*
*   \ingroup commhelpers
*
*   \param vec3 vector to round
*   \param precision to round by
*
*   \return the status. RWCOMM_SUCCESS for success, else RWCOMM_FAIL.
*/
RWCOMM_API RwCommError  RwCommRoundVec3(RwCommVec3 &vec3, RwCommReal precision);

#endif // RWCOMMUTILS_H
