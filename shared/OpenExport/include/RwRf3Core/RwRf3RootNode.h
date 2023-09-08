// RwRf3RootNode.h: interface for the RwRf3RootNode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWRF3ROOTNODE_H__BB6A2982_3EED_4292_9119_FD57397F575F__INCLUDED_)
#define AFX_RWRF3ROOTNODE_H__BB6A2982_3EED_4292_9119_FD57397F575F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwXMLDOM.h"
#include "RwRf3Core.h"
/**
*   \ingroup rf3helper 
*
*   This class is a helper class for creating the rf3 root node. It will create the rf3 root node,
*   containing the rf3 version, author, creation date and time.
*/
class RWRF3CORE_API RwRf3RootNode  : public RwXMLNode
{
public:
	RwRf3RootNode();
    RwRf3RootNode( RwXMLNode* parentNode );
	virtual ~RwRf3RootNode();

protected:

    void SetRootNodeData();

};

#endif // !defined(AFX_RWRF3ROOTNODE_H__BB6A2982_3EED_4292_9119_FD57397F575F__INCLUDED_)
