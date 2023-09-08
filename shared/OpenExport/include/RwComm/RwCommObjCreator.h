// RwExpObjCreator.h: interface for the RwExpObjCreator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPOBJCREATOR_H__F2F1EDEF_2A58_4B2B_8BFB_E8702E4C161B__INCLUDED_)
#define AFX_RWEXPOBJCREATOR_H__F2F1EDEF_2A58_4B2B_8BFB_E8702E4C161B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwCommObject.h"
#include "RwCommCreationData.h"
#include "RwCommScene.h"
#include "RwCommExportOptions.h"

/**
*  \ingroup commbuilder  
*   This is an abstract base class for creating an RwCommObject element.
*   A builder maintains a mapping table from its own native object type
*   to the appropriate RwCommObjCreator instance. The object creator's
*   role is to create an RwCommObject, initialize it and attach it to
*   the scene graph.
*
*   After the builder object finds an RwCommObjCreator instance in the
*   table, it executes these three methods in the following order: First
*   it calls RwCommObjCreator::Create(), then it calls
*   RwCommObjCreator::Init() with the created object and finally it
*   calls RwCommObjCreator::Attach() with the created object, the parent
*   and the scene object.
*
*   \see RwCommBuilder, RwCommCreationData
*/
class RWCOMM_API RwCommObjCreator : public RwCommObject
{
public:
    RwCommObjCreator();
    virtual ~RwCommObjCreator();

    /** 
    *   Create the specific RwCommObject.
    *
    *   \param pCreationData container of creation specific data that is needed
    *                        to create an instance of the desired RwCommObject.
    *   \param pExportOptions export options pointer.
    *
    *   \return RwCommObject object that is created
    */
    virtual RwCommObject *Create(const RwCommCreationData *pCreationData,
                                 const RwCommExportOptions *pExportOptions) const = 0;

    /** 
    *   Initialize the created RwCommObject. Any specific object initialization
    *   should be executed in this method.
    *
    *   \param pObject RwCommObject that was created by RwCommObjCreator::Create()
    *   \param pCreationData container of creation specific data that is needed
    *                        to create an instance of the desired RwCommObject.
    *   \param pExportOptions export options pointer.
    */
    virtual void Init(RwCommObject *pObject,
                      const RwCommCreationData *pCreationData,
                      const RwCommExportOptions *pExportOptions) const = 0;

    /** 
    *   Attach the created RwCommObject to the scene graph.
    *
    *   \param pObject RwCommObject that was created by RwCommObjCreator::Create()
    *   \param pParent the logical parent in the process of traversing the scene graph.
    *                  For example, if we are creating an RwCommTransform object, the
    *                  parent will usually be another RwCommTransform. If we are creating
    *                  an RwCommTexture object, the parent will usually be an
    *                  RwCommShader object.
    *   \param pScene a pointer to the scene node.
    *   \param pCreationData container of creation specific data that is needed to create an 
    *                   instance of the desired RwCommObject.
    */
    virtual void Attach(RwCommObject *pObject, RwCommObject *pParent, RwCommScene *pScene,
                        const RwCommCreationData *pCreationData) const = 0;
};

#endif // !defined(AFX_RWEXPOBJCREATOR_H__F2F1EDEF_2A58_4B2B_8BFB_E8702E4C161B__INCLUDED_)
