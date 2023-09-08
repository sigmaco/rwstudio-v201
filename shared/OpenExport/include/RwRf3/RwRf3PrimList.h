// RwRf3PrimList.h: interface for the RwRf3PrimList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RwRf3PRIMLIST_H__2418CB84_C6DA_48C4_99E9_051BAEECDD80__INCLUDED_)
#define AFX_RwRf3PRIMLIST_H__2418CB84_C6DA_48C4_99E9_051BAEECDD80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwXMLDOM.h"

#include "RwRf3.h"
#include "RwCommMesh.h"

// Prim lists
#define RWRF3_PRIMLIST_TYPE     L"primlist"
#define RWRF3_PRIMLIST_PRIM     L"p"
#define RWRF3_PRIMLIST_SHADER   L"shader"
#define RWRF3_PRIMLIST_COUNT    L"count"
#define RWRF3_PRIMLIST_PRIMLEN  L"length"

#define RWRF3_TRIANGULATION_TYPE    L"triangulation"
#define RWRF3_TRIANGULATION_TRILIST L"t"

#define RWRF3_EDGELIST_TYPE         L"edges"
#define RWRF3_EDGELIST_EDGES        L"e"

class RwCommScene;

/**
*   \ingroup rf3helpers 
*
*   This class is an rf3 primitive list node. It contains helper methods for setting and getting the primitive data.
*   Since it derives from the RwXMLNode class, it can be attached to the XML tree like any XML node.
*/
class RF3_API RwRf3PrimList  : public RwXMLNode
{
public:
	RwRf3PrimList();
    RwRf3PrimList( RwXMLNode* parentNode );
    RwRf3PrimList( const RwWString& name, RwXMLNode* parentNode = NULL );
	virtual ~RwRf3PrimList();

    RwCommError SetData(const RwVector<RwCommPrimitive> &vPrimList, const RwWString &shaderName);
    RwCommError GetData(RwVector<RwCommPrimitive> &vPrimList, RwCommScene* pScene);

    virtual RwCommError AddUserData( RwCommUserDataCollection& userDataColl );
    virtual RwCommError AddUserData( RwCommUserDataCollection& userDataColl, RwVector<int> &vPrimMap );
    virtual RwCommError GetUserData( RwCommUserDataCollection& userDataColl );
};

/**
*   \ingroup rf3helpers
*
*   This class is an rf3 triangulation node. It contains helper methods for setting and getting triangulation data.
*   Since it derives from the RwXMLNode class, it can be attached to the XML tree like any XML node.
*/
class RF3_API RwRf3Triangulation : public RwXMLNode
{
public:
    RwRf3Triangulation();
    RwRf3Triangulation( RwXMLNode* parentNode );
    RwRf3Triangulation( const RwWString& name, RwXMLNode* parentNode = NULL );
    virtual ~RwRf3Triangulation();

    RwCommError        AddData(const RwVector<int> &vTriangulation);

    // Build from node
    RwCommError GetData(RwVector< RwVector<int> > &vTriangulation);


};

class RF3_API RwRf3EdgeList : public RwXMLNode
{
public:
    RwRf3EdgeList();
    RwRf3EdgeList( RwXMLNode* parentNode );
    RwRf3EdgeList( const RwWString& name, RwXMLNode* parentNode = NULL );
    virtual ~RwRf3EdgeList();

    RwCommError        AddData(const RwVector<RwCommEdge> &vEdges);

    // Build from node
    RwCommError GetData(RwVector< RwVector<RwCommEdge> > &vEdges);


};

#endif // !defined(AFX_RwRf3PRIMLIST_H__2418CB84_C6DA_48C4_99E9_051BAEECDD80__INCLUDED_)
