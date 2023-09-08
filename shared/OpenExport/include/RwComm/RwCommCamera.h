// RwExpCamera.h: interface for the RwExpCamera class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPCAMERA_H__02B87964_D1B6_483E_AE08_C2BB071C7B7B__INCLUDED_)
#define AFX_RWEXPCAMERA_H__02B87964_D1B6_483E_AE08_C2BB071C7B7B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwCommShape.h"

const RwCommType RwCommCameraType("RwCommCamera");

/**
*  \ingroup commscenegraph  
*
*   This is the base class for camera objects. RwCommCamera objects are located in the origin, looking 
*   towards the +Z direction.
*/
class RWCOMM_API RwCommCamera : public RwCommShape  
{

public:
    enum ProjectionType {PERSPECTIVE = 0, ORTHOGONAL = 1};

    // Animated channels
    static RwCommAnimatedChannel NearChannel;
    static RwCommAnimatedChannel FarChannel;
    static RwCommAnimatedChannel FovChannel;
    static RwCommAnimatedChannel FovXChannel;
    static RwCommAnimatedChannel FovYChannel;
    static RwCommAnimatedChannel OrthoWidthChannel;
    static RwCommAnimatedChannel OrthoHeightChannel;
    static RwCommAnimatedChannel TargetDistanceChannel;
    static RwCommAnimatedChannel FogDistanceChannel;

    RwCommCamera();
    virtual ~RwCommCamera();

    // Type methods
    /** \return RwCommCameraType */
    virtual RwCommType  GetType() const { return RwCommCameraType; }
    virtual void        GetTypeHierarchy(RwVector<RwCommType> &vTypeVec) const;

	virtual RwCommError ShapeOrientation(RwCommMatrix &flip) const;

    // Projection
    /**
     *  The camera class has two basic projection types; PERSPECTIVE, representing a
     *  perspective camera and ORTHOGONAL representing an orthographic camera.
     *
     *  \param type This will be filled with the projection type.
     *
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetProjectionType(ProjectionType &type) const = 0;

    // Camera attributes
    /**
     *  \param fNear is filled with the near clipping plane.
     *
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetNear(RwCommReal &fNear) const = 0;

    /**
     *  \param fFar is filled with the far clipping plane.
     *
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetFar(RwCommReal &fFar) const = 0;

    /**
     *  \param fov is filled with the horizontal field of view angle in radians.
     *
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     *
     *  \note Should be used only for perspective cameras.
     */
    virtual RwCommError GetFovX(RwCommReal &fov) const = 0;

    /**
     *  \param fov is filled with the vertical field of view angle in radians.
     *
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     *
     *  \note Should be used only for perspective cameras.
     */
    virtual RwCommError GetFovY(RwCommReal &fov) const = 0;

    /**
     *  \param width is filled with the width of the orthographic camera's frustum.
     *
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetOrthoWidth(RwCommReal &width) const = 0;

    /**
     *  \param height is filled with the height of the orthographic camera's frustum.
     *
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetOrthoHeight(RwCommReal &height) const = 0;

    /**
     *  \param distance is filled with the camera target distance.
     *
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetTargetDistance(RwCommReal &distance) const = 0;

    /**
     *  \param distance is filled with the fog distance.
     *
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetFogDistance(RwCommReal &distance) const = 0;
};

#endif // !defined(AFX_RWEXPCAMERA_H__02B87964_D1B6_483E_AE08_C2BB071C7B7B__INCLUDED_)
