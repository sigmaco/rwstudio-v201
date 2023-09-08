#if !defined(RW_MAYA_TRAVERSE_ANIMATIONS_H)
#define RW_MAYA_TRAVERSE_ANIMATIONS_H

#pragma warning(disable : 4786)
#pragma warning(disable : 4251)


class RwMayaBuilder;
class RwCommCreationData;
#include "RwMayaCreationData.h"


/**
*  \ingroup mayabuilder 
*
*
*/
class RWMAYA_API RwMayaAnimatedChannelMap
{
public:
    RwMayaAnimatedChannelMap() {};
    virtual ~RwMayaAnimatedChannelMap() {};

    void RegisterChannelByAttributeName(const RwWString &name, RwCommAnimatedChannel channel);
    
    void CopyRegisteredChannels(RwMap<RwWString, RwCommAnimatedChannel> &src);

protected:

    void SetCreationData(MObject animObject, RwMayaCreationData &creationData) const;      

    RwMap<RwWString, RwCommAnimatedChannel>  m_mChannels;      
};

/**
*  \ingroup mayabuilder 
*/
class RWMAYA_API RwMayaTraverseBlendShapeAnimations : public RwMayaAnimatedChannelMap
{
public:
    RwMayaTraverseBlendShapeAnimations() : RwMayaAnimatedChannelMap() {}
    virtual ~RwMayaTraverseBlendShapeAnimations() {}

	virtual void Traverse(RwCommBuilder &rBuilder, RwCommCreationData *pCreationData, RwCommObject *pParent) const;
};

/**
*  \ingroup mayabuilder 
*
*  This class iterates over all upstream motionPath and animCurve dependendency nodes
*  and and builds RwComm nodes for them. 
*
*/
class RWMAYA_API RwMayaTraverseAnimations : public RwMayaAnimatedChannelMap
{
public:
    RwMayaTraverseAnimations() : RwMayaAnimatedChannelMap() {}
    virtual ~RwMayaTraverseAnimations() {}

	virtual void Traverse(RwCommBuilder &rBuilder, RwCommCreationData *pCreationData, RwCommObject *pParent) const;
};

/**
*  \ingroup mayabuilder 
*
*  This class iterates over all parents of the supplied dag path and traverses
*  animations from each (using RwMayaTraverseAnimations).
*
*   \see RwMayaTraverseAnimations
*
*/
class RWMAYA_API RwMayaTraverseParentAnimations : public RwMayaAnimatedChannelMap
{
public:
    RwMayaTraverseParentAnimations() : RwMayaAnimatedChannelMap() {}
	virtual ~RwMayaTraverseParentAnimations() {}

	virtual void Traverse(RwCommBuilder &rBuilder, RwCommCreationData *pCreationData, RwCommObject *pParent);
};

/**
*  \ingroup mayabuilder 
*
*  This class traverses all animation curves related to the given dag
*  path through IK.
*
*/
class RWMAYA_API RwMayaTraverseIKAnimations : public RwMayaAnimatedChannelMap
{
public:
	RwMayaTraverseIKAnimations() : RwMayaAnimatedChannelMap() {};
	virtual ~RwMayaTraverseIKAnimations() {};

	virtual void Traverse(RwCommBuilder &rBuilder, RwCommCreationData *pCreationData, RwCommObject *pParent);
};

/**
*  \ingroup mayabuilder 
*
*  This class traverses all animation curves related to the given dag
*  path by Maya constraint nodes.
*
*/
class RWMAYA_API RwMayaTraverseConstraintAnimations : public RwMayaAnimatedChannelMap
{
public:
	RwMayaTraverseConstraintAnimations() : RwMayaAnimatedChannelMap() {};
	virtual ~RwMayaTraverseConstraintAnimations() {};

	virtual void Traverse(RwCommBuilder &rBuilder, RwCommCreationData *pCreationData, RwCommObject *pParent);

private:
    RwSet<RwWString> visitedObjects;
};

#endif // !defined(RW_MAYA_TRAVERSE_ANIMATIONS_H)
