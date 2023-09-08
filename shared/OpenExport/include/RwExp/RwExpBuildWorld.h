// RwExpBuildWorld.h: interface for the RwExpBuildWorld class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPBUILDWORLD_H__642300AA_5B4F_421B_99E3_0D1CBC6634D8__INCLUDED_)
#define AFX_RWEXPBUILDWORLD_H__642300AA_5B4F_421B_99E3_0D1CBC6634D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwExp.h"
#include "RwExpTraverseAction.h"

class RwExpExporter;
class RwExpExportOptions;
class RwExpBspGeometry;
class RwCommTransform;
class RwCommShape;
class RwExpVertexCollection;

// =============================================================================
// -------------------------  RwExpBuildWorld CLASS  ---------------------------
// =============================================================================

const RwCommType RwExpBuildWorldType("RwExpBuildWorld");


struct RwExpUserDataRef
{
    // this is the index value of the original vertex/polygon
    // if the index is a negative value the item is an interpolated
    // vertex userdata item
    int index;
    int indexStart;
    int indexEnd;
    float delta;
};

/**
*
*  \ingroup traverseactions
*
*   This class builds an RpWorld object from a traverse list. It traverses the list, and for
*   each geometry object it encounters, it finds its associated RwExpBspGeometry object exporter. It then
*   extracts the RwExpBspGeometry's vertex and face information and adds them to the RtWorldImport structure.
*
*   \see RwExpBspGeometry
*   \see RtWorldImport
*   \see RpWorld
*/
class RWEXP_API RwExpBuildWorld  : public RwExpTraverseAction 
{
public:
    RwExpBuildWorld();
    virtual ~RwExpBuildWorld();

    //RwCommObject method
    virtual RwCommType      GetType() const     {return RwExpBuildWorldType;}

    // Apply the action
    virtual RwCommError     Apply(RwVector<RwExpTraverseObject> &vTraverseList,
                                const RwCommExportOptions *pExportOptions);
    virtual RwCommError     CleanUp();

    /** Get the generated RpWorld object.*/
    RpWorld                 *GetWorld() { return m_pWorld; }

    /** Set the bounding box of the world.*/
    void                    SetWorldBoundingBox(const RwCommBoundingBox &bbox) {m_bbox = bbox;}

protected:

    /** Sets the RtWorldImport import parameters. Users deriving from this class can set their
    *   own custom RtWorldImport parameters in this method.
    *
    *   \param pImportParams a pointer to an RtWorldImportParameters object
    *   \param pExportOptions a pointer to an export options object
    *   \param WorldHasPreLighting flag indicating whether the world has pre-light information
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise
    */
    virtual RwCommError     SetRtWorldImportParameters(RtWorldImportParameters *pImportParams,
                                const RwCommExportOptions *pExportOptions, bool WorldHasPreLighting);

    virtual RwCommError     AddShapeVerticesToRtWorldImport(RwExpBspGeometry *pBspGeometry,
                                RwCommScene *pScene, const RwCommExportOptions *globalData, bool &bSceneHasPreLighting);
    virtual RwCommError     AddShapeFacesToRtWorldImport(RwExpBspGeometry *pBspGeometry,
                                RwCommTransform *pTransform);
    virtual RwCommError     AddShapeUserData(RwExpBspGeometry *pBspGeometry,
                                RwCommTransform *pTransform);
    virtual RwCommError     AddShapeToonData(RwCommShape *pShape, RwExpVertexCollection &collection);

    static RwBool           WorldProgressCallBack(RwInt32 msg, RwReal value);

protected:    
    RwExpUserDataRef            *m_userDataRemap;
    int                         m_numAddedVertices;
    int                         m_numAddedPrimitives;
    int                         m_numUvSets;
    RtWorldImport               *m_pImportWorld;
    RpWorld                     *m_pWorld;
    RwCommBoundingBox           m_bbox;
};

#endif // !defined(AFX_RWEXPBUILDWORLD_H__642300AA_5B4F_421B_99E3_0D1CBC6634D8__INCLUDED_)
