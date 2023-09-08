// ScaleAnim.h
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCALEANIM_H__63BDBCAA_E944_4160_967C_64F9F3146525__INCLUDED_)
#define AFX_SCALEANIM_H__63BDBCAA_E944_4160_967C_64F9F3146525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwExp.h"
#include "RwExpTransform.h"
#include "RwExpFrame.h"
#include "RwExpAnimation.h"
#include "RwExpBuildHAnimAnimation.h"
#include "RwExpHAnimHierarchy.h"

class RwScaleAnimFrame : public RwExpFrame 
{
public:
    RwScaleAnimFrame() {};
    virtual ~RwScaleAnimFrame() {};

    // Destroy the object - called by RwCommObject::Release()
    virtual void Destroy() { delete this; }

    virtual RwV3d CalcTransformMatrix(RwCommTransform *pTransform,
                RwMatrix *matrix, RwCommReal scale, bool bSetRootIdent);
};

class RwScaleMatrixAnimation : public RwExpMatrixAnimation 
{
public:
    RwScaleMatrixAnimation() {};
    virtual ~RwScaleMatrixAnimation() {};

    virtual RwV3d CalcTransformMatrix(RwCommTransform *pTransform,
                RwMatrix *matrix, RwCommReal scale, bool bSetRootIdent);

    virtual void *SetHAnimFrame(const RwExpAnimKey &animKey, RwCommReal startTime, 
                        RpHAnimAnimation *pHAnim, int frame, void *pPrevFrame);
};

class RwExpScaleHAnimHierarchy : public RwExpHAnimHierarchy
{
public:
    RwExpScaleHAnimHierarchy() {};
    virtual ~RwExpScaleHAnimHierarchy() {};

    // Destroy the object - called by RwCommObject::Release()
    virtual void Destroy() { delete this; }

protected:
    virtual RpHAnimHierarchy* CreateHAnimHierarchy(RwInt32 numNodes, RwUInt32 *nodeFlags,
                       RwInt32 *nodeIDs, RpHAnimHierarchyFlag flags, RwInt32 maxKeyFrameSize);
};

class RwExpBuildScaleHAnimAnimation : public RwExpBuildHAnimAnimation
{
public:
	RwExpBuildScaleHAnimAnimation();
    virtual ~RwExpBuildScaleHAnimAnimation() {};

    // Destroy the object - called by RwCommObject::Release()
    virtual void Destroy() { delete this; }

protected:
    virtual RpHAnimAnimation* CreateHAnimAnimation(int nFrames, RwCommReal animTimeInterval);
};

class RwExpScaleAnimScaleFilter : public RwExpScaleFilter
{
public:
    RwExpScaleAnimScaleFilter() {};
    virtual ~RwExpScaleAnimScaleFilter() {};

    // Destroy the object - called by RwCommObject::Release()
    virtual void Destroy() { delete this; }

protected:
     virtual void SetScale(const RwCommVec3& scale);
};

class RwExpScaleOffsetFilter : public RwExpOffsetFilter
{
public:
    RwExpScaleOffsetFilter() {};
    virtual ~RwExpScaleOffsetFilter() {};

    // Destroy the object - called by RwCommObject::Release()
    virtual void Destroy() { delete this; }

protected:
    virtual void RemoveOffsetMatrixScale(RwCommMatrix &matrix);
    virtual void SetScale(RwCommReal scale);
};
#endif // !defined(AFX_SCALEANIM_H__63BDBCAA_E944_4160_967C_64F9F3146525__INCLUDED_)
