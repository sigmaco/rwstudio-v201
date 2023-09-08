// RwExpAnimation.h: interface for the RwExpAnimation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPANIMATION_H__7AE6012C_95F7_4685_801F_B39B740B27F8__INCLUDED_)
#define AFX_RWEXPANIMATION_H__7AE6012C_95F7_4685_801F_B39B740B27F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma warning(disable : 4786)
#pragma warning(disable : 4251)

#include <set>

#include "RwCommObject.h"
#include "RwCommPrimitives.h"

class RwCommAnimatable;

const RwCommType RwCommAnimationType("RwCommAnimation");

/**
*  \ingroup commscenegraph
*
*   This is the base class for all animation objects. This class is a generic key frame container class,
*   not dealing with specific types of animations. Users can derive their own animation classes for extracting
*   additional information about their animation systems. (i.e. ease in, ease out, tangent information etc.) 
*/
class RWCOMM_API RwCommAnimation : public RwCommObject
{
public:
    RwCommAnimation();
    virtual ~RwCommAnimation();

    // Type methods
     /**
     *  \return RwCommAnimationType
     *
     */
    virtual RwCommType  GetType() const { return RwCommAnimationType; }

    /**
     *  \return the RwCommAnimation type hierarchy.
     *
     */
    virtual void        GetTypeHierarchy(RwVector<RwCommType> &vTypeVec) const;

    /**
     *  Attaches the RwCommAnimation object to a RwCommAnimatable object.
     *
     *  \param pAnimatable pointer to an RwCommAnimatable object
     *
     *  \note Do not call this method explicitly. It will be called by RwCommAnimatable::AddAnimation().
     *  \see RwCommAnimatable::AddAnimation
     */
    void                SetAnimatable(RwCommAnimatable *pAnimatable) { m_pAnimatable = pAnimatable; }

    /**
     *  \return the RwCommAnimatable object, the RwCommAnimation object is attached to.
     *
     *  \see RwCommAnimatable
     */
    RwCommAnimatable   *GetAnimatable() const { return m_pAnimatable; }


    /**
     *  Gets the animation's key frame values within a given interval.
     *
     *  \param iStartFrame the start frame of the interval.
     *  \param iEndFrame the end frame of the interval.
     *  \param sKeyFrames a vector, which is filled with of all the animation's key frame values.
     *
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetKeyTimes(RwCommReal iStartFrame, RwCommReal iEndFrame,
                                    RwVector<RwCommReal> &sKeyFrames) const = 0;

protected:
    RwCommAnimatable   *m_pAnimatable;
};

#endif // !defined(AFX_RWEXPANIMATION_H__7AE6012C_95F7_4685_801F_B39B740B27F8__INCLUDED_)
