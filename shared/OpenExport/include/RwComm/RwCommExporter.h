// RwExpExporter.h: interface for the RwExpExporter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPEXPORTER_H__98CCDE62_5B87_4ED9_B355_D1A92DCBA97A__INCLUDED_)
#define AFX_RWEXPEXPORTER_H__98CCDE62_5B87_4ED9_B355_D1A92DCBA97A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma warning(disable : 4786)
#pragma warning(disable : 4251)
#include <map>

#include "RwCommExportObject.h"
#include "RwCommExportObjCreator.h"

class RwCommTransform;

/**
*  \ingroup structures
*   This structure is responsible for holding data used to drive a single export process.
*   It is passed to the RwCommExporter::Export function.
*
*   \see RwCommExporter
*/
struct RWCOMM_API RwCommExportInfo
{
    /** The scene being exported from. */
    RwCommScene                     *pScene;

    /** The selection for this export. */
    RwVector<RwCommObject *> vSelection;
    
    /** The name of the asset being exported. */
    RwWString                    sAssetName;
    
    /** The file name this export should generate when the Export Legacy Files option is used. */
    RwWString                    sFileName;
    
    /** The GUID of the asset being exported. */
    RwWString                    sGUID;
    
    /** If the asset being exported has a parent this stores its GUID. Empty string otherwise. */
    RwWString                    sParentGUID;
};

/**
*  \ingroup commexporter  
*
*   This is an abstract base class responsible for exporting the scene graph. Since
*   traversing the scene graph can be performed in many ways, the RwCommExporter
*   class leaves this task to its specific classes derived from it. The main entry point for
*   the export process is the Export() method. It is responsible for traversing 
*   and exporting the various elements. To assist with this task, the RwCommExporter
*   object holds a mapping table, containing a map from a RwCommType to an 
*   RwCommExportObjCreator instance. For each RwComm object the traverse process
*   encounters, it creates an instance of a RwCommExportObject. This object
*   is an export handler object of that specific RwCommObjectType, and is
*   responsible for executing the actual export action on that specific RwComm
*   object. The ExportObject() method is the one that creates an RwCommExportObject
*   instance, and executes its RwCommExportObject::Export() operation.
*
*   Classes, deriving from RwCommExporter, perform their own specific traverse and
*   call this method for each node they encounter. After the RwCommExportObject has
*   been created, you can always re-extract it from the exporter using the
*   GetExportObject() method. Classes, deriving from RwCommExporter, are usually
*   constructed with their own default set of RwCommExportObjects mapped to specific
*   RwComm objects. You can always override this behavior, by registering your own
*   set of custom RwCommExportObjects. For this, in your RwComm plugin you can
*   include the following function:
* 
*   \code void RegisterCustomExporterObjects(RwCommExporter* pExporter); \endcode
* 
*   Within this function you can register your own custom exporter objects.It
*   will be picked up by AttachPlugins() method every time an RwCommExporter object
*   is instanced.
*
*   To register your own exporter object use the RegisterExportObject() macro,
*   supplying the RwCommObjectType and the name of your RwCommExportObject class.
*/
class RWCOMM_API RwCommExporter : public RwCommObject
{
public:
    RwCommExporter();
    virtual ~RwCommExporter();

    /**
    *   Export the scene. This is the main entry point for the exporter. The method
    *   should traverse the scene graph, calling ExportObject() on each RwCommObject
    *   that it encounters.
    *   
    */
    virtual RwCommError         Export(const RwCommExportInfo &exportInfo) = 0;
    /**
    *   The ExportObject() method creates an RwCommExportObject instance, for a specific
    *   RwComm object and executes its RwCommExportObject::Export() operation. When given
    *   an RwCommObject, this method creates the appropriate RwCommExportObject by
    *   searching the table, executes its RwCommExportObject::Export() method and returns
    *   the instance of the RwCommExportObject it created.
    *   
    *   \param pObject being exported
    *   \param pParent of the object being exported in the scene graph.
    *   \param pScene RwCommScene object to be passed to the
    *                 RwCommExportObject::Export() method.
    *
    *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError         ExportObject(RwCommObject *pObject, RwCommObject *pParent,
                                                                    RwCommScene *pScene);

    /**
    *   Get the number of export objects, assigned to the given RwCommObject.
    *
    *   \param pObject for which to get the number of RwCommExportObject
    *
    *   \return the number of export objects. 
    */
    int                         GetNumExportObjects(const RwCommObject *pObject) const;

    /**
    *   Retrieve the RwCommExportObject for the specified RwCommObject.
    *
    *   \param pObject for which to find an export object
    *   \param index the index of the export object within the export object list. Use GetNumExportObjects()
    *           to get the number of export objects in the list.
    *
    *   \return RwCommExportObject* or NULL if not found. 
    */
    RwCommExportObject          *GetExportObject(const RwCommObject *pObject, int index = 0) const;

    /**
    *   Retrieve the RwCommExportObject with the specified type for the specified RwCommObject.
    *
    *   \param pObject for which to find an export object
    *   \param type the type of the RwCommExportObject to retrieve
    *
    *   \return RwCommExportObject* or NULL if not found. 
    */
    RwCommExportObject          *GetExportObject(const RwCommObject *pObject, RwCommType type) const;

    /**
    *   \return An RwCommExportOptions used in the export process.
    */
    RwCommExportOptions         *GetExportOptions() const { return m_pExportOptions; }
    /** \param  pExportOptions RwCommExportOptions object to be set for the exporter. */
    void                        SetExportOptions(RwCommExportOptions *pExportOptions)
                                                { m_pExportOptions = pExportOptions; }

    /**
    *   Register an RwCommExportObjCreator for a specified object type. Users can use
    *   the RegisterExportObject() macro instead.
    */
    void                        Register(const RwCommType sType, RwCommExportObjCreator *pExportObject,
                                            bool bAppend = false);

    /** Check if there is an export object registered for the specified type. */
    bool                        IsRegistered(const RwCommType sType) const;

    /** Clear the list of all export object the exporter is holding. */
    void                        ClearAllExportObjects();

protected:
    /** Iterate Open Export plugins and call:
    *   
    *   \code void RegisterCustomExporterObjects(RwCommExporter* pExporter); \endcode
    *   
    *   if present.
    */
    void                        AttachPlugins();

    /** Called before export of each object The return value should be true for export
    *   process to proceed.
    *
    *   \param pObject that is being exported.
    *
    *   \return true if export process should proceed. Default is true.
    */
    virtual bool OnExportObject(RwCommObject *pObject) { return true; }

protected:
    RwCommExportOptions *m_pExportOptions;

private:
    RwMap<RwCommType, RwVector<RwCommExportObjCreator *> >         m_mExporters;
    RwMap<const RwCommObject *, RwVector<RwCommExportObject *> >   m_mExpObjects;
};

/**
*  \ingroup commexporter  
*
*   This macro is used to register an RwCommExportObjCreator for a specified object type.
*   The registered creator will replace the first RwCommExportObjCreator on the list, if one exists.
*   To append your RwCommExportObjCreator to the RwCommExportObjCreator list, use the RegisterAdditionalExportObject() macro.
*   For example, for registering your own Mesh exporter:
*   \code
*   pDffExporter->RegisterExportObject(RwCommMesh, MyMeshExporter);
*   \endcode
*/
#define RegisterExportObject(TYPE, CREATOR) Register(TYPE, new RwCommExportObjCreatorImpl<CREATOR>) 


/**
*  \ingroup commexporter  
*
*   This macro is used to register an RwCommExportObjCreator for a specified object type.
*   The registered creator will be appended at the end of the RwCommExportObjCreator list, and will NOT replace
*   an existing RwCommExportObjCreator.
*   For example, for registering your own Mesh exporter:
*   \code
*   pDffExporter->RegisterAdditionalExportObject(RwCommMesh, MyMeshExporter);
*   \endcode
*/
#define RegisterAdditionalExportObject(TYPE, CREATOR) Register(TYPE, new RwCommExportObjCreatorImpl<CREATOR>, true) 

#endif // !defined(AFX_RWEXPEXPORTER_H__98CCDE62_5B87_4ED9_B355_D1A92DCBA97A__INCLUDED_)
