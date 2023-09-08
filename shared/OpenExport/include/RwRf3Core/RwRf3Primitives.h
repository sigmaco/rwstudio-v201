// RwRf3Primitives.h: interface for the RwRf3Primitives class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWRF3PRIMITIVES_H__35F2009A_747A_4A55_9A38_DF1959A31C44__INCLUDED_)
#define AFX_RWRF3PRIMITIVES_H__35F2009A_747A_4A55_9A38_DF1959A31C44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwRf3Core.h"
#include "RwXMLDOM.h"

#include "RwCommPrimitives.h"
#include "RwCommObject.h"

class RwRf3Matrix
{
public:
    RwRf3Matrix() {}
	RwRf3Matrix(const RwCommMatrix &M);
	virtual ~RwRf3Matrix();

    void Write(RwXMLNode* pNode);
    void Read(RwXMLNode* pNode);
    const RwCommMatrix& Get() const { return m_matrix; }

protected:

    RwCommMatrix    m_matrix;

};

class RwRf3Real
{
public:
    RwRf3Real();
    ~RwRf3Real();

    RwWString Write( const RwCommReal& value, bool bin = true );
    RwCommError Read( const RwWString& data, RwCommReal& value );
    RwCommError Create(RwXMLNode* pNode, const RwCommReal value, bool bin = true );
    RwCommError Build(RwXMLNode* pNode, RwCommReal& value );
};

class RwRf3String
{
public:
    RwRf3String();
    ~RwRf3String();

    RwWString Write( const RwWString& result );
    RwCommError Read( const RwWString& buffer, RwWString& result );
    RwCommError Create(RwXMLNode* pNode, const RwWString& value );
    RwCommError Build(RwXMLNode* pNode, RwWString& value );
};


#endif // !defined(AFX_RWRF3PRIMITIVES_H__35F2009A_747A_4A55_9A38_DF1959A31C44__INCLUDED_)
