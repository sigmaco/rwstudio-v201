#if !defined(RW_MAYA_CAMERA_H)
#define RW_MAYA_CAMERA_H

#include "RwMayaBlindData.h"

/**
*  \ingroup mayascenegraph 
*
*  This is the RwMaya implementation of RwCommCamera.
*
*   \see RwCommCamera
*/
class RWMAYA_API RwMayaCamera  : public RwCommCamera, public RwMayaBlindData
{
public:
	RwMayaCamera(MDagPath dagPath);
	virtual ~RwMayaCamera();

    /**
     *  Maya implementation of RwCommObject::GetUserData. Simply calls RwMayaBlindData::GetUserData.
     *  \param userDataColl the RwCommUserDataCollection in which data is to be filled.
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetUserData(RwCommUserDataCollection &userDataColl);

    // Get attribute methods.
    virtual RwCommError GetAttribute(const RwWString& name, int &value, int index = 0);
    virtual RwCommError GetAttribute(const RwWString& name, float &value, int index = 0);
    virtual RwCommError GetAttribute(const RwWString& name, RwWString &value, int index = 0);
    /**
     *  Maya implementation of RwCommAnimatable::GetAttributeNames. Simply calls RwMayaBlindData::GetObjectAttributeNames.
     *  \param vAttributeNames the names of all attributes found.
     *  \param vAttributeTypes the types of all attributes found.
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetAttributeNames(RwVector<RwWString>& vAttributeNames, RwVector<AttributeType> &vAttributeTypes);

    // Shape methods
    virtual RwCommError GetVisibility(bool &bIsVisible) const;

   // Projection
    virtual RwCommError GetProjectionType(ProjectionType &type) const;

    // Camera attributes
    virtual RwCommError GetNear(RwCommReal &fNear) const;
    virtual RwCommError GetFar(RwCommReal &fFar) const;
    virtual RwCommError GetFovX(RwCommReal &fov) const;
    virtual RwCommError GetFovY(RwCommReal &fov) const;
    virtual RwCommError GetOrthoWidth(RwCommReal &width) const;
    virtual RwCommError GetOrthoHeight(RwCommReal &height) const;
    /**
     *  This function is not implemented under RwMaya.
     *  \return RWCOMM_NOT_IMPLEMENTED.
     */
    virtual RwCommError GetTargetDistance(RwCommReal &distance) const;
    /**
     *  This function is not implemented under RwMaya.
     *  \return RWCOMM_NOT_IMPLEMENTED.
     */
    virtual RwCommError GetFogDistance(RwCommReal &distance) const;

protected:

    MDagPath    m_dagPath;
    MFnCamera   m_fnCamera;

};


/**
*  \ingroup mayabuilder 
*
*  This class is responsible for creating an RwMayaCamera object.
*
*   \see RwMayaCamera
*   \see RwMayaObjCreator
*/
class RWMAYA_API RwMayaCameraCreator : public RwMayaObjCreator
{
public:
	RwMayaCameraCreator();
	virtual ~RwMayaCameraCreator();

	virtual RwCommObject* Create(const RwCommCreationData* pCreationData,
								const RwCommExportOptions* pExportOptions) const;

	virtual void Init(	RwCommObject* pObject,
						const RwCommCreationData* pCreationData,
						const RwCommExportOptions* pExportOptions) const;

	virtual void Attach(RwCommObject* pObject, RwCommObject* pParent, RwCommScene* pScene,
                        const RwCommCreationData* pCreationData) const;
	virtual void Traverse(RwCommBuilder& rBuilder, RwCommCreationData* pCreationData, RwCommObject* pParent) const;
protected:
    RwMayaTraverseAnimations m_AnimationTraverse;
};

#endif // !defined(RW_MAYA_CAMERA_H)
