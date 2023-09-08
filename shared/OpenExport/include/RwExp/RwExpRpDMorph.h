// RwExpRpDMorph.h: interface for the RwExpRpDMorph class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPRPDMORPH_H__134603ED_3C46_4D1E_9449_E6ECA78B696B__INCLUDED_)
#define AFX_RWEXPRPDMORPH_H__134603ED_3C46_4D1E_9449_E6ECA78B696B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwExp.h"
#include "RwCommExportObject.h"

class RwCommMesh;
class RwCommBlendShape;
class RwExpExportOptions;
class RwExpRpGeometry;

const RwCommType RwExpRpDMorphType("RwExpRpDMorph");

/**
*  \ingroup exportobject
*   This is the class responsible for creating RpDMorphs and RpDMorphAnimations. The class is mapped to the
*   RwCommBlendShape modifier in the Dff exporter. The class has two main phases: building the morph targets and
*   setting the RpDMorph animation.
* 
*   In its first phase the class builds the morph targets querying the RwCommBlendShape object. It does so by
*   setting the desired target weight to 1.0 and the other targets to 0.0 and then calling ReEvaluate() on the 
*   RwCommMesh which is morphed. It then re-samples the morphed mesh by extracting its vertex Ids from its exporter
*   object, and getting the morphed vertices from the mesh, using the Ids. It then pushes the morphed vertices into
*   the vertex pipeline, applying all vertex optimizations and then setting the optimized vertices in the
*   appropriate morphed target. Users wishing to override this phase can override the BuildTargets() method.
* 
*   The next phase builds the RpDMorph animation. It does so by getting all the key frame times from the animation
*   object attached to the RwCommBlendShape object. (Remember that the blend shape modifier is also an animatable
*   object and can have animations attached to it.) Then it uses the key frames to query the RwCommBlendShape object
*   for all of its targets weights at these times. It then sets these weight in the RpDMorphAnimation object that it
*   has created. Users wishing to customize this stage can override the BuildAnimation() method.
*   The RpDMorph animation is added as a separate asset to the asset list after it is created.
*/
class RWEXP_API RwExpRpDMorph  : public RwCommExportObject
{
public:
    RwExpRpDMorph();
    virtual ~RwExpRpDMorph();

    virtual RwCommType          GetType() const { return RwExpRpDMorphType; }

    virtual RwCommError         Export(RwCommObject *pObj, RwCommObject *pParent,
                                    RwCommScene *pScene, const RwCommExportOptions *pExportOptions);

    /**
    *   Not implemented.
    *
    *   \param userDataColl the user data to be added
    *
    *   \returns RWCOMM_NOT_IMPLEMENTED
    */
    virtual RwCommError         AddUserData(RwCommUserDataCollection &userDataColl);

protected:
    virtual RwCommError         BuildTargets(RpGeometry *pGeometry, RwCommMesh *pMesh,
                                    RwCommBlendShape *pBlendShape, RwExpRpGeometry *pExpGeom,
                                    const RwCommExportOptions *pExportOptions);

    virtual RpDMorphAnimation   *BuildAnimation(RpGeometry *pGeometry, RwCommMesh *pMesh,
                                    RwCommBlendShape *pBlendShape, RwExpRpGeometry *pExpGeom,
                                    const RwCommExportOptions *pExportOptions);
};

#endif // !defined(AFX_RWEXPRPDMORPH_H__134603ED_3C46_4D1E_9449_E6ECA78B696B__INCLUDED_)
