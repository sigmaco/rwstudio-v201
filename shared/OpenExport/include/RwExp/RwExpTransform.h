// RwExpTransform.h: interface for the RwExpTransform class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPTRANSFORM_H__1E180456_6F69_4DE0_B28D_5491889D1D7D__INCLUDED_)
#define AFX_RWEXPTRANSFORM_H__1E180456_6F69_4DE0_B28D_5491889D1D7D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwExp.h"
#include "RwCommExportObject.h"

class RwCommTransform;
class RwExpGlobalData;

/**
*  \ingroup exportobject
*
*   This class is a base class for RwCommTransform exporters. It is responsible for calculating the object space
*   transformation matrix from the RwCommTransform. Part of these calculations include removing the scaling from
*   the matrix and flipping matrices for cameras and lights.
*
*
*   \see RwCommTransform
*   \see RwFrame
*/
class RWEXP_API RwExpTransform  : public RwCommExportObject
{
public:
    RwExpTransform();
    virtual ~RwExpTransform();

    /**
    *   Retrieve the unique index of this transform.
    *
    *   \param nodeIndex a reference to the returned node index.
    *
    *   \returns RWCOMM_SUCCESS.
    *
    *   \see RpHAnimHierarchy
    *   \see RwExpHAnimHierarchy
    */
    virtual RwCommError GetNodeIndex(int &nodeIndex);

    /**
    *   Set the unique index of this transform.
    *
    *   \param nodeIndex the new node index.
    *
    *   \returns RWCOMM_SUCCESS.
    *
    *   \see RpHAnimHierarchy
    *   \see RwExpHAnimHierarchy
    */
    virtual RwCommError SetNodeIndex(int nodeIndex);

    /**
    *   Get the propagated scale value at this transform. Since scaling is removed
    *   from all exported matrices the removed scale value is propagated down the
    *   hierarchy so that it can be burnt into attached geometry.
    *
    *   \returns a vector representing the propagated scale at this transform.
    */
    virtual RwV3d       GetPropogatedScale() const { return m_vScale; }

    /**
    *   The function returns whether or not this transform has been flipped. Transforms
    *   with attached lights or cameras are flipped as RenderWare lights and camera have
    *   an opposite orientation to those from 3ds max and Maya.
    *
    *   \returns a bool indicating whether this transform has been flipped.
    */
    virtual bool        GetFlipAxis() const { return m_bFlipAxis; }

	/**
    *   This function flips or unflips the orientation of a matrix.
    *
	*	\param orientation the flipping matrix
    *   \param matrix the matrix to be flipped
    *   \param bReFlip indicates whether to apply or to remove a matrix flip
    *
    */
	virtual void		FlipMatrixAxis(const RwCommMatrix &orientation, RwMatrix *matrix,
							bool bReFlip);
    
    /**
    *   This function calculates the modelling matrix for pTransform (i.e. the transform
    *   matrix relative to it's parent). Any scaling is removed from the matrix and returned
    *   If pTransform has no parent and bSetRootIdent is true then the matrix is set to identity.
    *
    *   \param pTransform the transform to calculate the modelling matrix for
    *   \param pMatrix the matrix to be stored
    *   \param scale a scale to be applied to the matrix
    *   \param bSetRootIdent indicates whether the matrix on root transforms should be cleared
    *
    *   \returns the scale removed from pMatrix
    */

    virtual RwV3d       CalcTransformMatrix(RwCommTransform *pTransform, RwMatrix *matrix,
                            RwCommReal scale, bool bSetRootIdent);
    
    /**
    *   This function applies a custom axis system to a matrix. The matrix is simply
    *   multiplied by the axis matrix
    *
    *   \param pMatrix the matrix to be transformed
    *   \param pAxisMatrix the axis transform
    *
    */
    virtual void        ApplyAxisSystem(RwMatrix *pMatrix, RwCommMatrix *pAxisMatrix);

    /**
    *   This function takes a matrix and transforms it by the Axis system specified.
    *   This differs from the preceding function as it post multiplies by the inverse
    *   of the axis system to restore the original worldspace co-ordinate system.
    *
    *   \param pMatrix the matrix to be transformed
    *   \param AxisMatrix the axis transform
    *
    */
    static void        TransformByAxisSystem(RwMatrix *pMatrix, const RwCommMatrix &AxisMatrix);
    static void        TransformByAxisSystem(RwCommMatrix *pMatrix, const RwCommMatrix &AxisMatrix);
    
    /**
    *   This function calculates the matrix of a transform relative to a passed parent. It
    *   first calls CalcTransformMatrix to calculate the matrix relative to the transforms
    *   scene graph parent. If the transform has a custom axis system it is then applied using
    *   ApplyAxisSystem. If the passed pParent transform is not the scene graph parent it then
    *   works back up the scene graph applying the matrices from parents until pParent is found.
    *   This is done to handle hierarchy optimization node removal. Finally, if the transform
    *   has an attached light or camera the matrix is flipped.
    *
    *   \param pTransform the transform to calculate the matrix for
    *   \param pParent the parent the matrix is to be relative to. Not necessarily the scene parent
    *   \param pMatrix the matrix to be stored
    *   \param scale a scale to be applied to the matrix
    *   \param bSetRootIdent indicates whether the matrix on root transforms should be cleared
    *   \param bApplyFlips indicates whether the matrix of light and camera transforms should be flipped
    *
    *   \returns true on success, false if pParent is NULL
    */
    virtual bool        CalcHierarchyTransformMatrix(RwCommTransform *pTransform,
                            RwCommTransform *pParent, RwMatrix *pMatrix, RwCommReal scale,
                            bool bSetRootIdent = false, bool bApplyFlips = true, bool bApplyAxisConversion = false);

protected:
    int         m_nodeIndex;
    RwV3d       m_vScale;
    bool        m_bFlipAxis;
};

#endif // !defined(AFX_RWEXPTRANSFORM_H__1E180456_6F69_4DE0_B28D_5491889D1D7D__INCLUDED_)
