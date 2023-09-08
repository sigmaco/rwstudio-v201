// MyRf3ExpRpPrtSys.cpp: implementation of the MyRf3ExpRpPrtSys class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyRf3ExpRpPrtSys.h"
#include "PlaneDeflectorComm.h"
#include "RwRf3Param.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MyRf3ExpRpPrtSys::MyRf3ExpRpPrtSys()
{

}

MyRf3ExpRpPrtSys::~MyRf3ExpRpPrtSys()
{

}



RwCommError
MyRf3ExpRpPrtSys::Export( RwCommObject* pObj,
                          RwCommObject* pParent,
                          RwCommScene* pScene,
                          const RwCommExportOptions* pExportOptions )
{    
    RwCommError eResult = RwRf3ExpRpPrtSys::Export( pObj, pParent, pScene, pExportOptions );
           
    RwCommVec3 normal;
    RwCommVec3 position;

    RwRf3Param* paramNode;

    PlaneDeflectorComm* pPrtSys = dynamic_cast<PlaneDeflectorComm*>( pObj );

    if (pPrtSys->GetPlane( normal, position ) == RWCOMM_SUCCESS)
    {
        paramNode = new RwRf3Param( L"MyDeflectorPlaneNormal" );
        paramNode->SetData( normal );
        m_pNode->AppendChild( (RwXMLNode*)paramNode );

        paramNode = new RwRf3Param( L"MyDeflectorPlanePosition" );
        paramNode->SetData( position );
        m_pNode->AppendChild( (RwXMLNode*)paramNode );
    }   

    return eResult;
}
