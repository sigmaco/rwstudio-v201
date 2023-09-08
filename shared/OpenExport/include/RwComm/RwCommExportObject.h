// RwExpExportObject.h: interface for the RwExpExportObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPEXPORTOBJECT_H__F6F104C0_0D52_43C7_B0F2_C6B05FA4B110__INCLUDED_)
#define AFX_RWEXPEXPORTOBJECT_H__F6F104C0_0D52_43C7_B0F2_C6B05FA4B110__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "RwComm.h"
#include "RwCommError.h"
#include "RwCommObject.h"

class RwCommScene;
class RwCommExportOptions;
class RwCommExporter;

/**
*  \ingroup commexporter  
*
*   This class is an abstract base class for creating export handlers for specific RwComm
*   objects. They are created by the RwCommExporter for each RwCommObject instance.
*/
class RWCOMM_API RwCommExportObject : public RwCommObject
{
public:
    RwCommExportObject() : RwCommObject(), m_pExporter(NULL) {}
    virtual ~RwCommExportObject() {}

    // Transform methods
    /**
     *  Classes, implementing this method, should implement their main export functionality here.
     *
     *  \param pObj the actual RwCommObject we want to export.
     *  \param pParent the object's scene graph parent.
     *  \param pScene the scene object.
     *  \param pExportOptions the export options.
     *
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError     Export(RwCommObject *pObj,
                                   RwCommObject *pParent,
                                   RwCommScene  *pScene,
                                   const RwCommExportOptions *pExportOptions ) = 0;
    
    /**
     *  Sets the main Exporter to this export object. This is called from the
     *  RwCommExporter::ExportObject() just before the RwCommExportObject::Export()
     *
     *  \param pExporter Exporter to be set.
     */
    void                    SetExporter(RwCommExporter *pExporter) { m_pExporter = pExporter; }

    /**
     *  Sets the main Exporter to this export object. This is called from the
     *  RwCommExporter::ExportObject() just before the RwCommExportObject::Export()
     *
     *  \return the exporter set to this object.
     */
    RwCommExporter          *GetExporter() const { return m_pExporter; }

private:
    RwCommExporter *m_pExporter;
};




#endif // !defined(AFX_RWEXPEXPORTOBJECT_H__F6F104C0_0D52_43C7_B0F2_C6B05FA4B110__INCLUDED_)
