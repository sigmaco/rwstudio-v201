// RwExpSkin.h: interface for the RwExpSkin class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPSKIN_H__EFDA41E2_7B96_4ADF_A613_7EAAE6D10200__INCLUDED_)
#define AFX_RWEXPSKIN_H__EFDA41E2_7B96_4ADF_A613_7EAAE6D10200__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwCommModifier.h"

class   RwCommTransform;
class   RwCommScene;
struct  RwCommVertexId;

/**
*  \ingroup commlowlevelobjects
*   This struct is responsible for holding a skin weight.
*
*   \see RwCommSkin
*/
struct RWCOMM_API RwCommSkinWeight
{
    RwCommSkinWeight() : w(0.0), bone(NULL) {}

    /** The vertex weight value.*/
    RwCommReal w;

    /** The associated bone which is an RwCommTransform object.*/
    RwCommTransform *bone;
};

const RwCommType RwCommSkinType("RwCommSkin");

/**
*  \ingroup commscenegraph  
*
*  This is the class representing a skin modifier. 
*/
class RWCOMM_API RwCommSkin : public RwCommModifier  
{
public:
    RwCommSkin();
    virtual ~RwCommSkin();

    // Type methods
    /** \return RwCommSkinType*/
    virtual RwCommType  GetType() const { return RwCommSkinType; }
    virtual void        GetTypeHierarchy(RwVector<RwCommType> &vTypeVec) const;

    // Skin methods
    /**
     *  This method returns a list of RwCommSkinWeight skin weights for the specified vertex ID. Each skin
     *  weight determines how much influence a bone is going to have on a vertex. 
     *
     *  \param id of the vertex whose weights we are querying.
     *  \param vWeights is filled with the list of RwCommSkinWeight for the specified vertex.
     *
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetSkinWeights(const RwCommVertexId &id, RwVector<RwCommSkinWeight> &vWeights) const = 0;

    /**
     *  Returns a list of all the bones that affect the skinned geometry.
     *  \param vBones is filled with the list of all the bones that affect the skinned geometry.
     *
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetInfluenceBones(RwVector<RwCommTransform *> &vBones) const = 0;
};

#endif // !defined(AFX_RWEXPSKIN_H__EFDA41E2_7B96_4ADF_A613_7EAAE6D10200__INCLUDED_)
