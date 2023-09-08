
#ifndef RWEXPANIMATION_H
#define RWEXPANIMATION_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwExp.h"
#include "RwExpExportOptions.h"
#include "RwExpTransform.h"

// =============================================================================
// ---------------------------  RwExpAnimKey CLASS  ----------------------------
// =============================================================================

class RwExpAnimation;

/**
*  \ingroup structures
*   This class is responsible for holding generic key frame data.
*
*   \see RwExpAnimation
*/
class RWEXP_API RwExpAnimKey
{
public:
    RwExpAnimKey(float time, bool isKeyFrame);
    virtual ~RwExpAnimKey();

    bool operator==(const RwExpAnimKey &other) const;
    bool operator<(const RwExpAnimKey &other) const;

    void* SetHAnimFrame(RwCommReal startTime, RpHAnimAnimation *pHAnim, int frame, void *pPrevFrame);
    void SetExpAnimation(RwExpAnimation *pAnimation) { m_pExpAnimation = pAnimation; }

    /** Flag indicating if this was an original key frame, or a generated one.*/
    bool        m_isKeyFrame;

    /** The time of the key frame.*/
    float       m_time;

    /** The node index of the RwCommTransform, to which the key frame belongs.*/
    RwInt32     m_nodeIndex;
private:

    RwExpAnimation *m_pExpAnimation;
};

typedef RwVector<RwExpAnimKey>::const_iterator RwExpAnimIt;

const RwCommType RwExpAnimationType("RwExpAnimation");

// =============================================================================
// --------------------------  RwExpAnimation CLASS  --------------------------
// =============================================================================

/**
*  \ingroup exportobject
*   This is an abstract base class responsible for exporting generic animation information. The class does not deal 
*   with specific animation data, such as matrix data, quaternion data or other types of animation data, but rather
*   only manages the list of the animated objects' key frame times.
*   The class has a set of methods for inserting, extracting and finding key frames. Use the InsertAnimKey(),
*   FindAnimKey() and GetAnimKeys() methods for this. 
*
*   In addition the action exposes the following pure virtual method:
*
*   \verbatim
    virtual void* SetHAnimFrame(const RwExpAnimKey &animKey,
                            RwCommReal startTime,
                            RpHAnimAnimation *pHAnim,
                            int frameNumber,
                            void *pPrevFrame) = 0;
    \endverbatim
*   The method allows the deriving classes to set their specific animation data to the RpHAnimAnimation structure.
*   Users, exporting their own custom animation data, should derive from this base class, store their custom
*   animation data in their derived class and implement the SetHAnimFrame() method. By default, the RenderWare
*   exports export quaternion and position animation, using the RwExpMatrixAnimation class. Refer to the RwExpMatrixAnimation
*   section for the default implementation of this method.
*
*/
class RWEXP_API RwExpAnimation : public RwExpTransform
{
public:
    RwExpAnimation();
    virtual ~RwExpAnimation();

    virtual RwCommType  GetType() const { return RwExpAnimationType; }

    virtual RwCommError Export(RwCommObject *pObj,
                                   RwCommObject *pParent,
                                   RwCommScene *pScene,
                                   const RwCommExportOptions *pExportOptions);

    /**
    *   Not implemented.
    *
    *   \param userDataColl the user data to be added
    *
    *   \returns RWCOMM_NOT_IMPLEMENTED
    */
    virtual RwCommError     AddUserData(RwCommUserDataCollection &userDataColl);

    // HAnim methods

    /** Virtual method for setting the HAnimFrame data.
    *
    *   \param animKey a reference to the animation key frame to add.
    *   \param startTime the start time of the entire animation (in frames).
    *   \param pHAnim a pointer to the RpHAnimAnimation structure.
    *   \param frameNumber the frame number of the key frame.
    *   \param pPrevFrame a pointer to the previous frame (RpHAnimStdKeyFrame by default).
    *
    *   \returns a pointer to the  generated key frame (RpHAnimStdKeyFrame by default).
    */
    virtual void *SetHAnimFrame(const RwExpAnimKey &animKey,
                                RwCommReal startTime,
                                RpHAnimAnimation *pHAnim,
                                int frameNumber,
                                void *pPrevFrame) = 0;

    // Dynamic key frame generation methods
    virtual RwCommError   CheckKeyFrameErrors(RwCommReal tolerance) { return RWCOMM_NOT_IMPLEMENTED; }

    // Animation key methods

    /** Get the list of all the key frames in the animation object.*/
    const RwVector<RwExpAnimKey > &GetAnimKeys() const { return m_vAnimKeys; }
    void                InsertAnimKey(const RwExpAnimKey &animKey);
    RwExpAnimKey*       FindAnimKey(const RwExpAnimKey &animKey);

    void                SortAndRemoveDuplicateKeys();
    void                AppendKeyFrames(const RwExpAnimation &other);

protected:

    RwExpAnimKey* FindKey(const RwExpAnimKey &animKey, int start, int end, int &index);


protected:

    RwCommTransform             *m_pTransform;
    RwCommTransform             *m_pParentTransform;

    RwVector<RwExpAnimKey>  m_vAnimKeys;
};



// =============================================================================
// --------------------------  RwExpMatrixAnimation CLASS  --------------------------
// =============================================================================

/**
*  \ingroup structures
*   This struct is responsible for holding matrix animation key frame data.
*
*   \see RwExpAnimKey
*   \see RwExpAnimation
*/
struct RwExpMatKey
{
    RwExpMatKey();
    RwExpMatKey & operator*=(const RwExpMatKey &b);

    /** The matrix value.*/
    RwMatrix        m_matrix;

    /** The quaternion extracted from the matrix.*/
    RtQuat          m_quat;

    /** A pointer to the RwExpAnimKey object, corresponding to this matrix key frame.*/
    RwExpAnimKey    *m_pAnimKey;
};

typedef RwVector<RwExpMatKey>::iterator RwExpMatIt;

/**
*  \ingroup exportobject
*   This is the class, implementing the RwExpAnimation class, responsible for exporting matrix animation data. 
*   It is mapped to the RwCommTransform object in the Anm exporter.
*   The class builds a list of matrix values corresponding to its base class's  key frame time values.
*   In addition, it implements the SetHanimFrame() method by setting its rotation and position values:
*
*   \verbatim
void* 
RwExpMatrixAnimation::SetHAnimFrame(const RwExpAnimKey &animKey, RwCommReal startTime, 
                                    RpHAnimAnimation *pHAnim, int frame, void *pPrevFrame)
{
    void* pKeyFrame = NULL;
    int index = -1;
    if (m_pTransform && m_vAnimKeys.size() > 0)
    {
        if (FindKey(animKey, 0, m_vAnimKeys.size() - 1, index) != NULL)
        {
            RpHAnimStdKeyFrame *hAnimFrames = (RpHAnimStdKeyFrame *)(pHAnim->pFrames);
            RwCommScene *pScene = m_pTransform->GetScene();

            hAnimFrames[frame].time        = pScene->FramesToSec(animKey.m_time - startTime);
            hAnimFrames[frame].q.imag      = m_vMatKeys[index].m_quat.imag;
            hAnimFrames[frame].q.real      = m_vMatKeys[index].m_quat.real;
            hAnimFrames[frame].t           = m_vMatKeys[index].m_matrix.pos;
            hAnimFrames[frame].prevFrame   = (RpHAnimStdKeyFrame *)pPrevFrame; 
            pKeyFrame = &(hAnimFrames[frame]);
        }
    } 

    return pKeyFrame;
}
    \endverbatim
*
*   \see RwExpAnimation
*/
class RWEXP_API RwExpMatrixAnimation : public RwExpAnimation
{
public:
    RwExpMatrixAnimation() {}
    virtual ~RwExpMatrixAnimation() {}

    // HAnim methods
    virtual void                *SetHAnimFrame(const RwExpAnimKey &animKey,
                                    RwCommReal startTime,
                                    RpHAnimAnimation *pHAnim,
                                    int frameNumber,
                                    void *pPrevFrame);

    // Dynamic key frame generation methods
    virtual RwCommError         CheckKeyFrameErrors(RwCommReal tolerance);

    void                        AddMatKey(const RwExpMatKey &matKey);
    RwVector<RwExpMatKey>   &GetMatKeys() { return m_vMatKeys; }
 
protected:

    // Dynamic key frame generation methods
    void                        CheckKeyFrameRangeError(RwExpMatIt startKey, RwExpMatIt endKey,
                                    int rangeKeyCount,  RwCommReal tolerance);
    bool                        CheckKeyValueErrors(RwExpMatIt startKey, RwExpMatIt middleKey,
                                    RwExpMatIt endKey, RwCommReal tolerance);
protected:

    RwVector<RwExpMatKey>   m_vMatKeys;
};

#endif // RWEXPANIMATION_H
