// RwMaxControl.h: interface for the RwMaxControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWMAXCONTROL_H__47276443_AD95_44D2_9977_A9AC22F77331__INCLUDED_)
#define AFX_RWMAXCONTROL_H__47276443_AD95_44D2_9977_A9AC22F77331__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "RwMax.h"
#include "RwCommAnimation.h"
 

/**
*  \ingroup maxcontrols 
*
*  This is a base class, implementing the RwCommAnimation inetrface and encapsulating a 3ds max Control object. 
*  In its GetKeyTimes() method, the class returns all of the key frame times of the Control object.
*
*   \see RwCommAnimation
*/
class RWMAX_API RwMaxDefaultControl : public RwCommAnimation 
{
public:
	RwMaxDefaultControl(Control *pControl, INode *pNode);
	virtual ~RwMaxDefaultControl();

    virtual RwCommError GetKeyTimes(RwCommReal iStartFrame, RwCommReal iEndFrame, RwVector<RwCommReal> &sKeyFrames) const;

    /** Gets the Control ojbect*/
    Control     *GetControl() const { return m_pControl; }

    /** Gets the INode object*/
    INode       *GetINode() const { return m_pNode; }

protected:

    void    ApplyORTypes(RwCommReal iStartFrame, RwCommReal iEndFrame, RwVector<RwCommReal> &sKeyFrames) const;

protected:

	Control     *m_pControl;
	INode       *m_pNode;

};

/**
*  \ingroup maxcontrols 
*
*  This is an RwCommAnimation implementation class, for all objects that need continuous animation sampling. 
*  In its GetKeyTimes() method, the class returns all continuous key frame values in the animation [start,end] interval,
*  regardless of the original Control's key frame times.
*
*   \see RwMaxDefaultControl
*/
class RWMAX_API RwMaxRapidSampleControl : public RwMaxDefaultControl 
{
public:
	RwMaxRapidSampleControl(Control *pControl, INode *pNode) :
	  RwMaxDefaultControl(pControl, pNode) {}
	virtual ~RwMaxRapidSampleControl() {}

    virtual RwCommError GetKeyTimes(RwCommReal iStartFrame, RwCommReal iEndFrame, RwVector<RwCommReal> &sKeyFrames) const;
};

/**
*  \ingroup maxcontrols 
*
*  This is an RwCommAnimation implementation class, for all animation curves that need smoothing. 
*  In its GetKeyTimes() method, the class returns all of the Control's original key frame time values, plus additional key frames
*  for creating a smoother curve representation of the sampled RwCommAnimatable object.
*
*   \see RwMaxDefaultControl
*/
class RWMAX_API RwMaxSmoothCurveControl : public RwMaxDefaultControl 
{
public:
	RwMaxSmoothCurveControl(Control *pControl, INode *pNode) :
	  RwMaxDefaultControl(pControl, pNode) {}
	virtual ~RwMaxSmoothCurveControl() {}

    virtual RwCommError GetKeyTimes(RwCommReal iStartFrame, RwCommReal iEndFrame, RwVector<RwCommReal> &sKeyFrames) const;
};

/**
*  \ingroup maxcontrols 
*
*  This is an RwCommAnimation implementation class, for linear rotation controls. 
*  In its GetKeyTimes() method, the class returns all of the Control's original key frame time values, plus additional key frames
*  for making sure that rotation is interpolated using the right angle, and not the reversed angle.
*
*   \see RwMaxDefaultControl
*/
class RWMAX_API RwMaxLinearRotationControl : public RwMaxDefaultControl 
{
public:
	RwMaxLinearRotationControl(Control *pControl, INode *pNode) :
	  RwMaxDefaultControl(pControl, pNode) {}
	virtual ~RwMaxLinearRotationControl() {}

    virtual RwCommError GetKeyTimes(RwCommReal iStartFrame, RwCommReal iEndFrame, RwVector<RwCommReal> &sKeyFrames) const;
};

/**
*  \ingroup maxcontrols 
*
*  This is an RwCommAnimation implementation class, for Bezier rotation controls. 
*  In its GetKeyTimes() method, the class returns all of the Control's original key frame time values, plus additional key frames
*  for making sure that rotation is interpolated using the right angle, and not the reversed angle.
*
*   \see RwMaxDefaultControl
*/
class RWMAX_API RwMaxHybridRotationControl : public RwMaxSmoothCurveControl 
{
public:
	RwMaxHybridRotationControl(Control *pControl, INode *pNode) :
	  RwMaxSmoothCurveControl(pControl, pNode) {}
	virtual ~RwMaxHybridRotationControl() {}

    virtual RwCommError GetKeyTimes(RwCommReal iStartFrame, RwCommReal iEndFrame, RwVector<RwCommReal> &sKeyFrames) const;
};

/**
*  \ingroup maxcontrols 
*
*  This is an RwCommAnimation implementation class, for TCB rotation controls. 
*  In its GetKeyTimes() method, the class returns all of the Control's original key frame time values, plus additional key frames
*  for making sure that rotation is interpolated using the right angle, and not the reversed angle.
*
*   \see RwMaxDefaultControl
*/
class RWMAX_API RwMaxTCBRotationControl : public RwMaxSmoothCurveControl 
{
public:
	RwMaxTCBRotationControl(Control *pControl, INode *pNode) :
	  RwMaxSmoothCurveControl(pControl, pNode) {}
	virtual ~RwMaxTCBRotationControl() {}

    virtual RwCommError GetKeyTimes(RwCommReal iStartFrame, RwCommReal iEndFrame, RwVector<RwCommReal> &sKeyFrames) const;
};

/**
*  \ingroup maxcontrols 
*
*  This is an RwCommAnimation implementation class, for IK slave controls. 
*  In its GetKeyTimes() method, the class traverses its entire IK chain and extracts all the key frames in the
*  chain.
*
*   \see RwMaxDefaultControl
*/
class RWMAX_API RwMaxIKSlaveControl : public RwMaxDefaultControl
{
public:
    RwMaxIKSlaveControl(Control *pControl, INode *pNode) : RwMaxDefaultControl(pControl, pNode) {}
    virtual ~RwMaxIKSlaveControl() {}

    virtual RwCommError GetKeyTimes(RwCommReal iStartFrame, RwCommReal iEndFrame, RwVector<RwCommReal> &sKeyFrames) const;
};

/**
*  \ingroup maxcontrols 
*
*  This is an RwCommAnimation implementation class, for IIKChainControl objects. 
*  In its GetKeyTimes() method, the class traverses its entire IK chain and extracts all the key frames in the
*  chain. 
*
*   \see RwMaxDefaultControl
*/
class RWMAX_API RwMaxIKControl : public RwMaxDefaultControl
{
public:
    RwMaxIKControl(Control *pControl, INode *pNode) : RwMaxDefaultControl(pControl, pNode) {}
    virtual ~RwMaxIKControl() {}

    virtual RwCommError GetKeyTimes(RwCommReal iStartFrame, RwCommReal iEndFrame, RwVector<RwCommReal> &sKeyFrames) const;
};

#endif // !defined(AFX_RWMAXCONTROL_H__47276443_AD95_44D2_9977_A9AC22F77331__INCLUDED_)
