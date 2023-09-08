// RwRf3Stream.h: interface for the RwRf3Stream class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWRF3STREAM_H__2B76496B_9FE0_42E6_9FC1_5EBA9A4E0B04__INCLUDED_)
#define AFX_RWRF3STREAM_H__2B76496B_9FE0_42E6_9FC1_5EBA9A4E0B04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwRf3Core.h"

#include "RwCommPrimitives.h"
#include <strstream>

/**
*   \ingroup rf3helper 
*
*   This class is a low level class for streaming out basic data types. It derives from the ostringstream
*   class and adds special operators for streaming out RwComm data types.
*/
class RWRF3CORE_API RwRf3OStream  
{
public:
	RwRf3OStream();
	virtual ~RwRf3OStream();

    RwRf3OStream& operator<<(const wchar_t* c);
    RwRf3OStream& operator<<(RwCommReal r);
    RwRf3OStream& operator<<(const RwWString& s);
    RwRf3OStream& operator<<(const RwCommVec2 &v);
    RwRf3OStream& operator<<(const RwCommVec3 &v);
    RwRf3OStream& operator<<(const RwCommVec4 &v);
    RwRf3OStream& operator<<(const RwVector<RwCommReal> &v);
    RwRf3OStream& operator<<(const RwCommMatrix &M);
    RwRf3OStream& operator<<(wchar_t c);
    RwRf3OStream& operator<<(unsigned int n);
    RwRf3OStream& operator<<(int n);
    RwRf3OStream& operator<<(bool n);

    RwWString  str();

    static void binary(bool binOn) { mBinaryReal = binOn; }
private:

    std::basic_ostringstream<wchar_t>   *m_pStream;
    static bool mBinaryReal;
};

/**
*   \ingroup rf3helper 
*
*   This class is a low level class for streaming in basic data types. It derives from the istringstream
*   class and adds special operators for streaming in RwComm data types.
*/
class RWRF3CORE_API RwRf3IStream  
{
public:
	RwRf3IStream();
    RwRf3IStream(const RwWString& str);
	virtual ~RwRf3IStream();

    RwRf3IStream& operator>>(RwCommReal& r);
    RwRf3IStream& operator>>(unsigned int& r);
    RwRf3IStream& operator>>(int& r);
    RwRf3IStream& operator>>(bool& b);
    RwRf3IStream& operator>>(RwWString& s);
    RwRf3IStream& operator>>(RwCommVec2& v);
    RwRf3IStream& operator>>(RwCommVec3& v);
    RwRf3IStream& operator>>(RwCommVec4& v);
    RwRf3IStream& operator>>(RwVector<RwCommReal> &v);
    RwRf3IStream& operator>>(RwCommMatrix& M);

    inline bool skipdelim(void);
    bool fail() const;
    bool eof() const;

private:

   std::basic_istringstream<wchar_t>   *m_pStream;

};
#endif // !defined(AFX_RWRF3STREAM_H__2B76496B_9FE0_42E6_9FC1_5EBA9A4E0B04__INCLUDED_)
