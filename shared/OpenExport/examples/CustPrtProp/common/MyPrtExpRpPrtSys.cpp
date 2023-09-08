// MyPrtExpRpPrtSys.cpp: implementation of the MyPrtExpRpPrtSys class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyPrtExpRpPrtSys.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


MyPrtExpRpPrtSys::MyPrtExpRpPrtSys()
{
    m_pPlaneDeflector = NULL;
}

MyPrtExpRpPrtSys::~MyPrtExpRpPrtSys()
{

}

RwCommError
MyPrtExpRpPrtSys::SetCustomProperties( void )
{
    if( m_pPlaneDeflector != NULL )
    {    
        MySimpleDeflectorPlane theDeflectorPlane;

        if( m_pPlaneDeflector->GetPlane( theDeflectorPlane.normal,
            theDeflectorPlane.position ) == RWCOMM_SUCCESS )
        {
            /// get the property offset
            RwInt32 offset = RpPrtStdPropTabGetPropOffset(m_EmtClass->propTab, EMITTERPRTPROPPLANE);
    
            MySimpleDeflectorPlane *deflectorPlane = 
                (MySimpleDeflectorPlane *) (((RwChar *)m_Emt) + offset);

            deflectorPlane->normal = theDeflectorPlane.normal;
            deflectorPlane->position = theDeflectorPlane.position;
        
            return RWCOMM_SUCCESS;
        }
    }

    return RWCOMM_FAIL;
}

RwCommError
MyPrtExpRpPrtSys::AddCustomProperties( RpPrtStdPropertyTable *propTab )
{
    if( m_pPlaneDeflector != NULL )
    {    
        // append a deflector plane property to the end of the emitter proprty table
        RwInt32 prop[1], propSize[1];
        RpPrtStdPropertyTable *newPropTable;

        prop[0] = EMITTERPRTPROPPLANE;
        propSize[0] = sizeof(MySimpleDeflectorPlane);

        MySimpleDeflectorPlane theDeflectorPlane;

        if( m_pPlaneDeflector->GetPlane( theDeflectorPlane.normal,
            theDeflectorPlane.position ) == RWCOMM_SUCCESS )
        {
            newPropTable = RpPrtStdPropTabAppend(m_EmtClass->propTab, 1, prop, propSize);

            RpPrtStdEClassSetPropTab(m_EmtClass, newPropTable);
           
            return RWCOMM_SUCCESS;
        }
    }

    return RWCOMM_FAIL;
}

RwCommError 
MyPrtExpRpPrtSys::Export( RwCommObject* pObj, RwCommObject* pParent, RwCommScene* pScene,
                      const RwCommExportOptions* pExportOptions )
{    
    m_pPlaneDeflector = dynamic_cast<PlaneDeflectorComm *>(pObj);
    
    return RwPrtExpRpPrtSys::Export(pObj, pParent, pScene, pExportOptions);        
}




