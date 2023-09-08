// RwExpBuildHAnimAnimation.h: interface for the RwExpBuildHAnimAnimation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPBUILDHANIMANIMATION_H__5EEBE4EF_F25D_4433_A6A7_7DD7F7DA5A7D__INCLUDED_)
#define AFX_RWEXPBUILDHANIMANIMATION_H__5EEBE4EF_F25D_4433_A6A7_7DD7F7DA5A7D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwExp.h"
#include "RwExpTraverseAction.h"
#include "RwExpAnimation.h"

class RwExpAnmExporter;
class RwCommScene;

const RwCommType RwExpBuildHAnimAnimationType("RwExpBuildHAnimAnimation");

/**
*
*  \ingroup traverseactions
*
*   This class builds an RpHAnimAnimation object from a traverse list. It traverses the list and for
*   each RwCommTransform it encounters, it finds its associated RwExpAnimation object exporter. It then
*   extracts the RwExpAnimation's key frames and builds the RpHAnimAnimation from those key frames.
*
*   \see RwExpAnimation
*   \see RwCommTransform
*   \see RpHAnimAnimation
*/
class RWEXP_API RwExpBuildHAnimAnimation  : public RwExpTraverseAction 
{
public:
    RwExpBuildHAnimAnimation();
    virtual ~RwExpBuildHAnimAnimation();

    //RwCommObject method
    virtual RwCommType  GetType() const { return RwExpBuildHAnimAnimationType; }

    // RwExpTraverseAction methods
    virtual RwCommError     Apply(RwVector<RwExpTraverseObject> &vTraverseList,
                                    const RwCommExportOptions *pExportOptions);
    virtual RwCommError     CleanUp();

    /**
    *  Get the RpHAnimAnimation structure generated during Apply().
    *  \return the RpHAnimAnimation created if successful, null otherwise.
    */
    RpHAnimAnimation* GetHAnimation() const { return m_pHAnim; }

protected:

    // Overridables

    /** Create a RpHAnimAnimation structure. Classes deriving from the RwExpBuildHAnimAnimation class, can implement
    *   their own custom CreateHAnimAnimation() method.
    *
    *   \param nFrames the number of frames in the animation.
    *   \param animTimeInterval the duration of the animation.
    *   \return the RpHAnimAnimation created if successful, null otherwise.
    */
    virtual RpHAnimAnimation* CreateHAnimAnimation(int nFrames, RwCommReal animTimeInterval);

protected:

    RwCommError GenerateAnimFromHierarchyKeyFrames(RwCommScene *pScene, RpHAnimAnimation *&hAnim);

protected:

    RpHAnimAnimation            *m_pHAnim;

private:
    RwSet<RwExpAnimKey>     m_unifiedAnimKeys;

};

#endif // !defined(AFX_RWEXPBUILDHANIMANIMATION_H__5EEBE4EF_F25D_4433_A6A7_7DD7F7DA5A7D__INCLUDED_)
