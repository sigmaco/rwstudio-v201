// RwRf3Animatable.h: interface for the RwRf3Animatable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWRF3ANIMATABLE_H__5CA76B95_8BAC_44E2_A4F6_ED6DFB600581__INCLUDED_)
#define AFX_RWRF3ANIMATABLE_H__5CA76B95_8BAC_44E2_A4F6_ED6DFB600581__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwRf3.h"
#include "RwCommAnimatable.h"

/**
*   \ingroup rf3write
*
*   This class is the base class for all RwRf3 export objects, that need to export key frame animation
*   data. All deriving classes need to implement two pure virtual methods: CreateAnimation() and
*   SampleKey(). The class works with conjunction with the RwRf3ExpAnimations class. The RwRf3ExpAnimations
*   class is a traverse action, which loops through all frames in the scene, for each one calling
*   SampleKey() on each exporter object implementing the RwRf3ExpAnimatable base class. It is the responsibility
*   of the implementing class, to sample the animated data at that frame and generate the appropriate
*   XML data, in the XML tree. At the end of the loop, the RwRf3ExpAnimations class calls the RwRf3ExpAnimatable::CreateAnimation()
*   method, allowing the implementing classes to finalize the key generation process.
*   All classes supporting dynamic key frame generation, are expected to cache the sampled key frame data when
*   SampleKey() is called, and then calculate all dynamic key frames in CreateAnimation(). This can be 
*   achieved by using the RwRf3DynKeyGen helper class.
*
*   \see RwRf3ExpAnimations
*   \see RwRf3DynKeyGen
*   \see RwCommAnimatable
*/
class RF3_API RwRf3ExpAnimatable  
{
public:
	RwRf3ExpAnimatable();
	virtual ~RwRf3ExpAnimatable();

    void SetKeyTimes(RwCommAnimatedChannel channel, const RwVector<RwCommReal> &vKeys);
    void SampleKey(RwCommAnimatable* pAnimatable, RwCommReal frame, bool bDynKeyFrameGen);

    // Overridables
    /** 
    *   Called by the RwRf3ExpAnimations traverse action, at the end of the frame loop. Classes, implementing
    *   dynamic key frame generation (DKFG), should generate all their final key frames at this point. The method
    *   receives indication whether dynamic key frame generation is enabled, and if so what is the DKFG tolerance.
    *
    *   \param bGenDynamicKeys a flag indicating whether dynamic key frame generation is enabled
    *   \param tolerance the dynamic key frame generation tolerance
    */
    virtual RwCommError CreateAnimation(bool bGenDynamicKeys, RwCommReal tolerance) = 0;

protected:

    /**
    *   Called by the RwRf3ExpAnimations traverse action, for each frame in the scene. Implementing
    *   classes should sample the RwCommAnimatable object at the given frame, and generate the appropriate
    *   animation data.
    *
    *   \param pAnimatable a pointer to the exported RwCommAnimatable object
    *   \param frame the frame in which to sample the RwCommAnimatable object
    *   \param channel the channel driving the animation
    *   \param bOrigKey indicates whether this is an original key frame of the object (bOrigKey == true)
    *           or a psuedo key frame (bOrigKey == false). All classes supporting dynamic key frame generation
    *           can use this flag to decide whether to create a key frame at this frame or not.
    */
    virtual RwCommError SampleKey(RwCommAnimatable* pAnimatable,
        RwCommReal frame, RwCommAnimatedChannel channel, bool bOrigKey) = 0;
    

private:

    RwMap<RwCommAnimatedChannel, RwSet<RwCommReal> > m_mChannels;
};

#endif // !defined(AFX_RWRF3ANIMATABLE_H__5CA76B95_8BAC_44E2_A4F6_ED6DFB600581__INCLUDED_)
