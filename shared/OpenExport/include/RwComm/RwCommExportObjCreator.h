#ifndef RWEXPEXPORTOBJCREATOR_H
#define RWEXPEXPORTOBJCREATOR_H

#include "RwCommObject.h"

class RwExpExportObject;

/**
*  \ingroup commexporter  
*
*   This is an abstract base class used for creating RwCommExportObject objects.
*   
*   An RwCommExporter object holds a mapping table, containing a map from a
*   RwCommType to an RwCommExportObjCreator instance. For each RwComm object
*   the traverse process in the export encounters, it creates an instance of an
*   RwCommExportObject using RwCommExportObjCreator::Create()
*
*   \note You should not use this class directly, use the RegisterExportObject
*         instead.
*
*   \see RwCommExporter, RwCommExportObject, RegisterExportObject
*/
class RWCOMM_API RwCommExportObjCreator : public RwCommObject
{
public:
    RwCommExportObjCreator() : RwCommObject() {}
    virtual ~RwCommExportObjCreator() {}

    /** \return RwCommExportObject created by this creator */
    virtual RwCommExportObject *Create() = 0;
};

/**
*  \ingroup commexporter  
*
*   This is a template class used for creating RwCommExportObject objects.
*   
*   \note You should not use this template directly, use the
*         RegisterExportObject instead.
*
*   \see RwCommExporter, RwCommExportObject
*/
template< class T >
class RwCommExportObjCreatorImpl : public RwCommExportObjCreator
{
public:
    RwCommExportObjCreatorImpl() : RwCommExportObjCreator() {}
    virtual ~RwCommExportObjCreatorImpl() {}

    virtual RwCommExportObject *Create() { return new T; }
};

#endif