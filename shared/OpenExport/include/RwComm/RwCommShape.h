// RwCommShape.h: interface for the RwCommShape class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWCOMMSHAPE_H__9EB284D2_52E1_4956_92E9_9199C4E64671__INCLUDED_)
#define AFX_RWCOMMSHAPE_H__9EB284D2_52E1_4956_92E9_9199C4E64671__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwCommAnimatable.h"
#include "RwCommShader.h"

class RwCommTransform;

const RwCommType RwCommShapeType("RwCommShape");

/**
*  \ingroup commscenegraph  
*
*   This is the base class for all scene graph elements that are shapes. A shape is an 
*   object that can be attached to a transformation, thus having a position in 3D space.
*   A shape also manages a list of associated shaders.
*/
class RWCOMM_API RwCommShape : public RwCommAnimatable
{
public:
    RwCommShape();
    virtual ~RwCommShape();

    // Type methods
    virtual RwCommType      GetType() const                             { return RwCommShapeType; }
    virtual void            GetTypeHierarchy(RwVector<RwCommType> &vTypeVec) const;

    // Visibility
    /**
     *  In addition, like the RwCommTransform class, the shape has a notion of visibility,
     *  stating if this object is visible/invisible. Unlike the transformation class,
     *  the visibility state of the shape is not hierarchical and affects the shape only.
     *
     *  \param bIsVisible will be filled in with the visibility state.
     *
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError     GetVisibility(bool &bIsVisible) const = 0;

    // Transform methods
    /**
     *  \param pTranform Transform this shape will be attached to.
     */
    void                    SetTransform(RwCommTransform *pTranform)    { m_pTransform = pTranform; }

    /**
     *  \return The transform this shape is attached to.
     */
    RwCommTransform        *GetTransform() const                        { return m_pTransform; }

    // Shader methods
    /** Add a shader to the shape*/
    void                    AddShader(RwCommShader *pShader);

    /** Get the number of shaders, associated with the shape
     *  \return the number of shaders attached to the shape.
     */
    int                     GetNumShaders() const;

    /** Get shader by index*/
    RwCommShader           *GetShader(int i) const;

	/** The transformation to apply to the shape to orientate it correctly.
	 *  \param flip The matrix containing the transformation.
	 *  \return RWCOMM_SUCCESS if orientation is required.
	 */

	virtual RwCommError ShapeOrientation(RwCommMatrix &flip) const { return RWCOMM_NOT_IMPLEMENTED; }

protected:

    // Called on adding a shader
    /**
    *   Called each time a shader is attached to the shape. 
    */
    virtual void            OnAddShader(RwCommShader *pShader) {}

protected:

    RwCommTransform                 *m_pTransform;
    RwVector<RwCommShader *>    m_vShaders;

};

#endif // !defined(AFX_RWCOMMSHAPE_H__9EB284D2_52E1_4956_92E9_9199C4E64671__INCLUDED_)
