// RwRf3Param.h: interface for the RwRf3Param class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RwRf3PARAM_H__77812B03_F864_4ECF_8034_70FE3708F871__INCLUDED_)
#define AFX_RwRf3PARAM_H__77812B03_F864_4ECF_8034_70FE3708F871__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwCommPlatform.h"

#include "RwXMLDOM.h"

#include "RwRf3Core.h"

#include <../RwMI/RwMIAnimProp.h>

class RwRf3IStream;

#define RWRF3_PARAM_TAG         L"param"
#define RWRF3_PARAM_NAME        L"name"
#define RWRF3_PARAM_TYPE        L"type"
#define RWRF3_PARAM_DATA        L"data"

// Data types
#define RWRF3_TYPE_REAL         L"real"
#define RWRF3_TYPE_BOOL         L"bool"
#define RWRF3_TYPE_INT          L"int"
#define RWRF3_TYPE_UINT         L"uint"
#define RWRF3_TYPE_STRING       L"string"
#define RWRF3_PARAM_REPTYPE		L"reptype"

#define RWRF3_DATA_REAL         L"real"
#define RWRF3_DATA_INT          L"int"
#define RWRF3_DATA_UINT         L"uint"
#define RWRF3_DATA_BOOL         L"bool"
#define RWRF3_DATA_STRING       L"string"

#define RWRF3_ANIMATION_TYPE    L"animation"
#define RWRF3_KEY_TYPE          L"key"
#define RWRF3_ANIMATION_CHANNEL L"channels"

enum RwRf3ParamDataType
{
    RwRf3NONTYPE = 0,
    RwRf3REAL32,            // 32 bit floating point real
    RwRf3UINT32,            // 32 bit unsigned integer
    RwRf3INT32,             // 32 bit signed integer
    RwRf3STRING,            // a string
    RwRf3BOOL               // a booean value
};

typedef RwMIAnimPropKey<RwCommReal> RwRf3RealKey;
typedef RwMIAnimPropKey<RwCommVec2> RwRf3Vec2Key;
typedef RwMIAnimPropKey<RwCommVec3> RwRf3Vec3Key;
typedef RwMIAnimPropKey<RwCommVec4> RwRf3Vec4Key;
typedef RwMIAnimPropKey< RwVector<RwCommReal> > RwRf3VecKey;
typedef RwMIAnimPropKey<RwCommMatrix> RwRf3MatrixKey;
typedef RwMIAnimPropKey<RwWString> RwRf3StringKey;

/**
*   \ingroup rf3helper 
*
*   This class is an rf3 param node. It contains helper methods for querying the param's name, type and value.
*   Since it derives from the RwXMLNode class, it can be attached to the XML tree like any XML node.
*   Users can set the param's value using the SetData() methods. In addition, the value can be animated using
*   the SetKeys() methods. All data can be extracted using the GetData() and GetKeys() methods.
*/
class RWRF3CORE_API RwRf3Param : public RwXMLNode
{
public:
	RwRf3Param();
    RwRf3Param( RwXMLNode* parentNode );
    RwRf3Param( const RwWString& name, RwXMLNode* parentNode = NULL );
	virtual ~RwRf3Param();

    RwCommError SetDataType( RwRf3ParamDataType type, int dimX = 1, int dimY = 1 );
    RwCommError GetDataType( RwRf3ParamDataType& type, int& dimX, int& dimY );

    /** Returns the X dimension of the array*/
    int GetDimX( void ) { return mDimX; }

    /** Returns the Y dimension of the array*/
    int GetDimY( void ) { return mDimY; }

    void SetParamName( const RwWString& name );
    RwCommError GetParamName( RwWString& name );

    RwCommError SetData( RwVector<RwCommVec4>& vValues1, RwVector<RwCommVec4>& vValues2 );
    RwCommError SetData( RwVector<RwCommVec3>& vValues1, RwVector<RwCommVec3>& vValues2 );
    RwCommError SetData( RwVector<RwCommVec2>& vValues1, RwVector<RwCommVec2>& vValues2 );
    RwCommError SetData( RwVector<RwCommVec4>& vValues );
    RwCommError SetData( RwVector<RwCommVec3>& vValues );
    RwCommError SetData( RwVector<RwCommVec2>& vValues );
    RwCommError SetData( RwVector<RwCommReal>& vValues );
    RwCommError SetData( RwVector<RwWString>& vValues );
    RwCommError SetData( RwVector<int>& vValues );
    RwCommError SetData( RwVector<unsigned int>& vValues );
    RwCommError SetData( RwVector<bool>& vValues );
    RwCommError SetData( const RwCommMatrix& M );
    RwCommError SetData( const RwCommVec2& v );
    RwCommError SetData( const RwCommVec3& v );
    RwCommError SetData( const RwCommVec4& v );
    RwCommError SetData( const RwCommReal& v );
    RwCommError SetData( const RwWString& s );
    RwCommError SetData( const bool v );
    RwCommError SetData( const unsigned int v );
    RwCommError SetData( const int v );

    RwCommError GetData( RwVector<RwCommVec4>& vValues1, RwVector<RwCommVec4>& vValues2 );
    RwCommError GetData( RwVector<RwCommVec3>& vValues1, RwVector<RwCommVec3>& vValues2 );
    RwCommError GetData( RwVector<RwCommVec2>& vValues1, RwVector<RwCommVec2>& vValues2 );
    RwCommError GetData( RwVector<RwCommVec4>& vValues );
    RwCommError GetData( RwVector<RwCommVec3>& vValues );
    RwCommError GetData( RwVector<RwCommVec2>& vValues );
    RwCommError GetData( RwVector<RwCommReal>& vValues );
    RwCommError GetData( RwVector<RwWString>& vValues );
    RwCommError GetData( RwVector<int>& vValues );
    RwCommError GetData( RwVector<bool>& vValues );
    RwCommError GetData( RwCommMatrix& M );
    RwCommError GetData( RwCommVec2& v );
    RwCommError GetData( RwCommVec3& v );
    RwCommError GetData( RwCommVec4& v );
    RwCommError GetData( RwCommReal& v );
    RwCommError GetData( RwWString& s );
    RwCommError GetData( int& i );
    RwCommError GetData( unsigned int &i );
    RwCommError GetData( bool& b );

    RwCommError GetKeys( RwVector<RwRf3MatrixKey>& vKeys );
    RwCommError GetKeys( RwVector<RwRf3RealKey>& vKeys );
    RwCommError GetKeys( RwVector<RwRf3Vec2Key>& vKeys );
    RwCommError GetKeys( RwVector<RwRf3Vec3Key>& vKeys );
    RwCommError GetKeys( RwVector<RwRf3Vec4Key>& vKeys );
    RwCommError GetKeys( RwVector<RwRf3VecKey>& vKeys );
    RwCommError GetKeys( RwVector<RwRf3StringKey>& vKeys );

    RwCommError SetKeys( RwVector<RwRf3MatrixKey>& vKeys );
    RwCommError SetKeys( RwVector<RwRf3RealKey>& vKeys );
    RwCommError SetKeys( RwVector<RwRf3Vec2Key>& vKeys );
    RwCommError SetKeys( RwVector<RwRf3Vec3Key>& vKeys );
    RwCommError SetKeys( RwVector<RwRf3Vec4Key>& vKeys );
    RwCommError SetKeys( RwVector<RwRf3VecKey>& vKeys );
    RwCommError SetKeys( RwVector<RwRf3StringKey>& vKeys );

    RwCommError Parse( RwRf3IStream& stream );

    bool IsAnimated( void );
    void RemoveAnimation( void );
    RwCommError GetAnimatedChannels( RwWString& sChannels );
    RwCommError SetAnimatedChannels( const RwWString& sChannels );

    void SetTargetPlatform(const RwCommPlatform &platform, bool usePlatform);
	bool GetTargetPlatform(const RwCommPlatform &platform);

    RwCommError ParseDataType( void );

protected:
     
    RwWString GenerateDataType( const RwWString& type, int dimX = 1, int dimY = 1);
    RwCommError CheckDirty( void );

private:

    int mDimX, mDimY;
    RwRf3ParamDataType mDataType;
    // do we need to generate type info
    bool mDirtyData;

    bool m_targetPlatforms[RwCommPlatformCount];
};

template<class ParamType>
class RwRf3PlatformParam
{
public:

	RwRf3PlatformParam(const RwWString &name)
	{
		m_sName = name;
		for (int i = 0; i < RwCommPlatformCount; i++)
		{				
			m_platformBin[i] = false;
		}
		m_pParamList = NULL;
	}

	~RwRf3PlatformParam()
	{
		// clean up
		if (m_pParamList)
		{
			delete m_pParamList;
		}
	}

	void SetData(const RwCommPlatform platform, const ParamType data, bool refresh = true)
	{
		m_platformBin[platform] = true;
		m_platformData[platform] = data;
		if (refresh)
		{
			RefreshParamList();
		}
	}

	void AttachToNode(RwXMLNode *pParent)
	{
		if (pParent)
		{
			RwXMLNodeListItor itor;

			for (itor = m_pParamList->begin(); itor != m_pParamList->end(); itor++)
			{
				pParent->AppendChild((*itor).GetPtr());
			}
		}
	}

	void GetParamList(RwXMLNodeList &paramList)
	{
		//copy the param list
		RwXMLNodeListItor itor;

		for (itor = m_pParamList->begin(); itor != m_pParamList->end(); itor++)
		{
			(*itor)->AddRef();
			paramList.push_back(*itor);
		}
	}
	
	void RefreshParamList( void )
	{
		int i;
		bool usedPlatformBin[RwCommPlatformCount];

		// clean up
		if (m_pParamList)
		{
			delete m_pParamList;
			m_pParamList = NULL;
		}

		// create empty param list
		m_pParamList = new RwXMLNodeList;

		// reset the bin so we know which ones have been generated
		for (i = 0; i < RwCommPlatformCount; i++)
		{
			usedPlatformBin[i] = false;
		}

		// create the params
		for (i = 0; i < RwCommPlatformCount; i++)
		{
			// if this is a valid platform and hasn't yet been made a param
			if (m_platformBin[i] && !usedPlatformBin[i])
			{
				// create the new param
				RwRf3Param *pParam = new RwRf3Param(m_sName);
				pParam->SetData(m_platformData[i]);
				pParam->SetTargetPlatform(static_cast<RwCommPlatform>(i), true);

				// look for similar platforms
				for (int j = i+1; j < RwCommPlatformCount; j++)
				{
					if (!usedPlatformBin[j] && m_platformBin[j] && 
						(m_platformData[i] == m_platformData[j])) 
					{
						// add this platform to the existing param
						pParam->SetTargetPlatform(static_cast<RwCommPlatform>(j), true);
						usedPlatformBin[j] = true;
					}
				}

                m_pParamList->push_back(pParam);
			}
		}
	}

protected:

	ParamType m_platformData[RwCommPlatformCount];
	bool m_platformBin[RwCommPlatformCount];
	RwWString m_sName;
	RwXMLNodeList* m_pParamList;
};


class RWRF3CORE_API RwRf3ParamList
{
public:
    RwRf3ParamList( RwXMLNode* pNode );
    virtual ~RwRf3ParamList();

    RwRf3Param* GetParam( const RwWString& name );
    RwRf3Param* GetParam( const RwWString& name, const RwCommPlatform platform);

private:
    RwXMLNodeList* paramList;
};

#endif // !defined(AFX_RwRf3PARAM_H__77812B03_F864_4ECF_8034_70FE3708F871__INCLUDED_)
