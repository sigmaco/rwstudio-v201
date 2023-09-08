// RwMaxTraverseControllers.h: interface for the RwMaxTraverseControllers class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWMAXTRAVERSECONTROLLERS_H__9FA58510_4753_446B_AB32_A79E7B0DDAE0__INCLUDED_)
#define AFX_RWMAXTRAVERSECONTROLLERS_H__9FA58510_4753_446B_AB32_A79E7B0DDAE0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwMax.h"
#include "RwCommAnimatable.h"
#include "RwMaxCreationData.h"

class RwMaxBuilder;

/**
*  \ingroup maxbuilder 
*
*   Base class for all RwCommObjCreator objects, that require the RwMaxBuilder to build their assigned
*   animations (RwCommAnimation). The RwMaxBuilder calls the TraverseControllers() method, (at
*   the end of the RwMaxBuilder::BuildObject() method), for each RwCommObjCreator object implementing this
*   method. The default implementation extracts the main TMController using the 
*   INode::GetTMController() method and tries to build it. It then traverses its sub controllers and attempts
*   to build them as well. Users can override this method and introduce their own custom animation
*   builder functionality.
*
*/
class RWMAX_API RwMaxTraverseControllers  
{
public:
	RwMaxTraverseControllers();
	virtual ~RwMaxTraverseControllers();

	virtual void TraverseControllers(RwMaxBuilder &rBuilder, RwCommCreationData *pCreationData, RwCommObject *pParent);

    // Animated channels lookup registration functions
    void RegisterChannelByParamBlockName(const RwString &name, RwCommAnimatedChannel channel);
    void RegisterChannelByParamBlockIndex(int iParamBlockIndex, int iParamBlockEntry, RwCommAnimatedChannel channel);
    void RegisterChannelBySubAnimIndex(int iSubAnimIndex, RwCommAnimatedChannel channel);

    // Registration helper functions
    void RegisterPositionEntries();
    void RegisterRotationEntries();
    void RegisterScaleEntries();

protected:

	RwCommObject    *BuildControl(	RwMaxBuilder &rBuilder, RwMaxControlData *pCtrlData, RwCommObject *pParent);

    // Build control helper functions, for looking up the appropriate animated channel
    RwCommObject    *BuildControl(	RwMaxBuilder &rBuilder, RwMaxControlData *pCtrlData, RwCommObject *pParent,
                                    const RwString &channelName);
    RwCommObject    *BuildControl(	RwMaxBuilder &rBuilder, RwMaxControlData *pCtrlData, RwCommObject *pParent,
                                    int iParamBlockIndex, int iParamBlockEntry );
    RwCommObject    *BuildControl(	RwMaxBuilder &rBuilder, RwMaxControlData *pCtrlData, RwCommObject *pParent,
                                    int iSubAnimIndex );
private:

    RwString MakeChannelEntry(int i1, int i2);
    RwString MakeChannelEntry(int i);

private:

    RwMap<RwString, RwCommAnimatedChannel>  m_mChannels; 
};



#endif // !defined(AFX_RWMAXTRAVERSECONTROLLERS_H__9FA58510_4753_446B_AB32_A79E7B0DDAE0__INCLUDED_)
