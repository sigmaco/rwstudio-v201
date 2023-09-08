// RwCommBuilder.h: interface for the RwCommBuilder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWCOMMBUILDER_H__E19DCB75_778B_467A_AE33_9FEB6CD2F57D__INCLUDED_)
#define AFX_RWCOMMBUILDER_H__E19DCB75_778B_467A_AE33_9FEB6CD2F57D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwComm.h"
#include "RwCommScene.h"

class RwCommExportOptions;

/**
 *  \ingroup commbuilder  
 *
 *  This class is an abstract base class that is responsible for building the scene graph. 
 *  The process of building a scene graph involves translating native specific objects to
 *  RwComm scene graph objects. Each specific modeling tool builder traverses its native
 *  scene graph, and for each object it encounters, creates the appropriate RwComm scene
 *  graph element.
 *
 *  The mapping from the native element to the RwComm element is achieved via an extendable
 *  mapping table, implemented in the derived classes. Each entry in the table holds a mapping from
 *  the native object type to a RwCommObjCreator class instance. The RwCommObjCreator object,
 *  creates the appropriate RwComm element. Each specific modeling tool builder is constructed with a
 *  default set of RwCommObjCreator objects. The user can always override this default set, by
 *  registering his own set of RwCommObjCreator objects within the builder, and thus
 *  changing the way the scene graph is constructed.
 *
 *  \see RwCommObjCreator
 */
class RWCOMM_API RwCommBuilder : public RwCommObject
{
public:
    RwCommBuilder();
    virtual ~RwCommBuilder();

    // Set Global Export Data
    /**
     *  Use this function to set the builder's options.
     *
     *  \param pData options to set to the builder.
     */
    void    SetExportOptions(RwCommExportOptions *pData) { m_pExportOptions = pData; }

    /**
    *   This is the main method that triggers the entire build proccess of the scene graph.
    *   The method creates a new RwCommScene object and builds the scene graph under it.
    *
    *   \param pScene a pointer to an RwCommScene object, under which the scene graph will be built.
    *
    *   \return RWCOMM_SUCCESS if the scene graph was built successfully. RWCOMM_FAIL otherwise.
    */
    virtual RwCommError Build(RwCommScene *&pScene) = 0;

protected:
    void AttachPlugins();

protected:
    RwCommExportOptions *m_pExportOptions;
};

#endif // !defined(AFX_RWCOMMBUILDER_H__E19DCB75_778B_467A_AE33_9FEB6CD2F57D__INCLUDED_)
