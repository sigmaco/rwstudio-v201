// RwCommBlendShape.h: interface for the RwExpMorph class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWCOMMBLENDSHAPE_H__2C379865_A226_4A87_8A5A_5C7565708790__INCLUDED_)
#define AFX_RWCOMMBLENDSHAPE_H__2C379865_A226_4A87_8A5A_5C7565708790__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwCommMorph.h"

const RwCommType RwCommBlendShapeType("RwCommBlendShape");

/**
*  \ingroup commscenegraph
*
*   This is the class representing a multiple morph targets modifier. It is derived
*   from RwCommModifier and it morphs a geometry, using multiple morph targets and
*   an associated weight for each target.
*
*   To query each specific morph target's vertices, you can set that morph target's
*   weight to 1.0, while setting the other targets to 0.0, and then calling
*   RwCommGeometry::ReEvaluate() on the geometry.
*/

class RWCOMM_API RwCommBlendShape : public RwCommMorph
{
public:
    // Animated channels
    static RwCommAnimatedChannel WeightChannel;

    RwCommBlendShape();
    virtual ~RwCommBlendShape();

   // Type methods
    virtual RwCommType  GetType() const { return RwCommBlendShapeType; }
    virtual void        GetTypeHierarchy(RwVector<RwCommType> &vTypeVec) const;

    /**
     *  \param nTargets is filled with the number of morph targets.
     *
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetNumTargets(int &nTargets) const = 0;

    /**
     *  Sets the target's weight value. This will modify the morphed geometry, but it
     *  will not be affected however, until you call the RwCommGeometry::ReEvaluate()
     *  method for that geometry.   
     *
     *  \param iTarget the index of the target which weight is set.
     *  \param weight to be assigned to specified target.
     *
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError SetTargetWeight(int iTarget, RwCommReal weight) = 0;

    /**
     *  \param iTarget is the index of the target which weight is queried.
     *  \param weight is filled with the target's weight.
     *
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetTargetWeight(int iTarget, RwCommReal &weight) const = 0;

    /**
     *  \param iTarget is the index of the target which named is quiered.
     *  \param name is filled with the name of the target.
     *
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetTargetName(int iTarget, RwWString &name) const = 0;
};

#endif // !defined(AFX_RWEXPMORPH_H__2C379865_A226_4A87_8A5A_5C7565708790__INCLUDED_)
