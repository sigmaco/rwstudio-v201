// RwRf3Camera.h: interface for the RwRf3Camera class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWRF3CAMERA_H__C096F937_1BAA_43E0_BF79_9BD65E3B9055__INCLUDED_)
#define AFX_RWRF3CAMERA_H__C096F937_1BAA_43E0_BF79_9BD65E3B9055__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwRf3.h"
#include "RwRf3Object.h"

#include "RwRf3ObjCreator.h"

#define RWRF3_CAMERA_TAG            L"camera"
#define RWRF3_CAMERA_NEAR           L"nearclip"
#define RWRF3_CAMERA_FAR            L"farclip"
#define RWRF3_CAMERA_FOG            L"fogdist"
#define RWRF3_CAMERA_PROJECTION     L"projection"
#define RWRF3_CAMERA_PERSPECTIVE    L"PERSPECTIVE"
#define RWRF3_CAMERA_PARALLEL       L"PARALLEL"
#define RWRF3_CAMERA_FOVX           L"fovx"
#define RWRF3_CAMERA_FOVY           L"fovy"
#define RWRF3_CAMERA_ORTHOH         L"orthoheight"
#define RWRF3_CAMERA_ORTHOW         L"orthowidth"
#define RWRF3_CAMERA_TARGDIST       L"targetdist"
#define RWRF3_CAMERA_DEFNEARCLP     1.0f
#define RWRF3_CAMERA_DEFFARCLP      500.0f

/**
*   \ingroup rf3write
*
*   This class is responsible for exporting RwCommCamera objects. In its Export() method the class
*   exports all the camera properties as RwRf3Param objects and attaches them to the local root m_pNode object. At
*   the end of the method the m_pNode is attached to its XML parent transform.
*
*   \see RwCommCamera
*   \see RwRf3Param
*/
class RF3_API RwRf3ExpCamera : public RwRf3ExpObject
{
public:
	RwRf3ExpCamera();
	virtual ~RwRf3ExpCamera();

    virtual RwCommError     Export( RwCommObject* pObject,
                                RwCommObject* pParent,
                                RwCommScene* pScene,
                                const RwCommExportOptions* pExportOptions );
};

/**
*   \ingroup rf3read
*
*   This class builds an RwMICamera from an XML node. 
*
*   \see RwMICamera
*/
class RF3_API RwRf3CameraCreator : public RwRf3ObjCreator
{
public:
    RwRf3CameraCreator() {}
    virtual ~RwRf3CameraCreator() {}

	virtual RwCommObject* Create(const RwCommCreationData* pCreationData,
								const RwCommExportOptions* pExportOptions) const;

	virtual void Attach(RwCommObject* pObject, RwCommObject* pParent, RwCommScene* pScene,
                        const RwCommCreationData* pCreationData) const;

    virtual void Init(  RwCommObject* pObject,
					    const RwCommCreationData* pCreationData,
					    const RwCommExportOptions* pExportOptions) const;
};

#endif // !defined(AFX_RWRF3CAMERA_H__C096F937_1BAA_43E0_BF79_9BD65E3B9055__INCLUDED_)