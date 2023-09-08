// RwExpCreationData.h: interface for the RwExpCreationData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPCREATIONDATA_H__10A3103A_3146_4F03_B93F_2441395C28EC__INCLUDED_)
#define AFX_RWEXPCREATIONDATA_H__10A3103A_3146_4F03_B93F_2441395C28EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwComm.h"

/**
*  \ingroup commbuilder  
*
*   This is simply a container of creation specific data that is needed to create
*   an instance of the desired RwCommObject. Each modeling tool, such as 3ds max
*   or Maya, will provide its own native implementations of that class, containing
*   the relevant creation information. For example in 3ds max it will contain an
*   INode or an Mtl etc, and in Maya MObject, MDagPath etc.
*
*   \see RwCommObjCreator
*/
class RWCOMM_API RwCommCreationData
{
public:
    RwCommCreationData();
    virtual ~RwCommCreationData();
};

#endif // !defined(AFX_RWEXPCREATIONDATA_H__10A3103A_3146_4F03_B93F_2441395C28EC__INCLUDED_)
