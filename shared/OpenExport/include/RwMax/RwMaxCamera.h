// RwMaxCamera.h: interface for the RwMaxCamera class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWMAXCAMERA_H__4CAFB08C_B7F3_4641_8143_947A7660E4DC__INCLUDED_)
#define AFX_RWMAXCAMERA_H__4CAFB08C_B7F3_4641_8143_947A7660E4DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "RwCommCamera.h"
#include "RwCommObjCreator.h"

#include "RwMax.h"
#include "RwMaxCustomAttributes.h"
#include "RwMaxTraverseModifiers.h"
#include "RwMaxTraverseControllers.h"

/**
*  \ingroup maxscenegraph 
*
*  This is the RwMax implementation of the RwCommCamera.
*
*   \see RwCommCamera
*/
class RWMAX_API RwMaxCamera  : public RwCommCamera, public RwMaxCustomAttributes
{
public:
	RwMaxCamera(CameraObject *pCam);
	virtual ~RwMaxCamera();

    virtual RwCommError GetUserData(RwCommUserDataCollection &userDataColl);

    // Get attribute methods.
    virtual RwCommError GetAttribute(const RwWString &name, int &value, int index = 0);
    virtual RwCommError GetAttribute(const RwWString &name, float &value, int index = 0);
    virtual RwCommError GetAttribute(const RwWString &name, RwWString &value, int index = 0);
    virtual RwCommError GetAttributeNames(RwVector<RwWString> &vAttributeNames, RwVector<AttributeType> &vAttributeTypes);

    // Shape methods
    virtual RwCommError GetVisibility(bool &bIsVisible) const;

   // Projection
    virtual RwCommError GetProjectionType(ProjectionType &type) const;

    //Camera attributes
    virtual RwCommError GetNear(RwCommReal &fNear) const;
    virtual RwCommError GetFar(RwCommReal &fFar) const;
    virtual RwCommError GetFovX(RwCommReal &fov) const;
    virtual RwCommError GetFovY(RwCommReal &fov) const;
    virtual RwCommError GetOrthoWidth(RwCommReal &width) const;
    virtual RwCommError GetOrthoHeight(RwCommReal &height) const;
    virtual RwCommError GetTargetDistance(RwCommReal &distance) const;
    virtual RwCommError GetFogDistance(RwCommReal &distance) const;

    /** Get the max camera object.*/
    CameraObject *GetCamera() const { return m_pCamera; }

protected:

	CameraObject    *m_pCamera;

};


/**
*  \ingroup maxbuilder 
*
*  This class is responsible for creating an RwMaxCamera object.
*
*   \see RwCommCamera
*/
class RWMAX_API RwMaxCameraCreator  : public RwCommObjCreator,
 public RwMaxTraverseModifiers,
 public RwMaxTraverseControllers
{
public:
	RwMaxCameraCreator();
	virtual ~RwMaxCameraCreator();

	virtual RwCommObject *Create(const RwCommCreationData *pCreationData,
								 const RwCommExportOptions *pExportOptions) const;

	virtual void Init(	RwCommObject *pObject,
						const RwCommCreationData *pCreationData,
						const RwCommExportOptions *pExportOptions) const;

	virtual void Attach(RwCommObject *pObject, RwCommObject *pParent, RwCommScene *pScene,
                        const RwCommCreationData* pCreationData) const;

	virtual void TraverseControllers(RwMaxBuilder &rBuilder, RwCommCreationData *pCreationData,
                                                                        RwCommObject *pParent);
};

#endif // !defined(AFX_RWMAXCAMERA_H__4CAFB08C_B7F3_4641_8143_947A7660E4DC__INCLUDED_)
