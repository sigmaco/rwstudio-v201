// RwExpPositionOps.h: interface for the RwExpPositionOps class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPPOSITIONOPS_H__67312917_D4D2_4423_9065_5763ED920711__INCLUDED_)
#define AFX_RWEXPPOSITIONOPS_H__67312917_D4D2_4423_9065_5763ED920711__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwExp.h"
#include "RwExpVertexFilter.h"

class RwCommTransform;

const RwCommType RwExpScaleFilterType("RwExpScaleFilter");

/**
*
*  \ingroup vertexpipeline
*
*   This vertex filter is responsible for scaling vertex positions by a specified factor. The factor can be set
*   using the RwExpScaleFilter::SetScale() method. The filter uses the RwCommTransform::GetScalePivots() method 
*   to calculate the final scaling matrix, which is given by:
*   \verbatim Sfinal = preScale * Sfactor * postScale \endverbatim
    where: preScale & postScale 
*   are the scale pivots,
*
*   Sfactor is a scaling matrix, scaled by the given factor.
*/
class RWEXP_API RwExpScaleFilter : public RwExpVertexFilter  
{
public:
    RwExpScaleFilter();
    virtual ~RwExpScaleFilter() {}

    virtual RwCommType GetType() const { return RwExpScaleFilterType; }

    virtual unsigned GetVertexDataMask() const { return RwCommVertex::POSITION; }

    // Initialise the filter. Called at the beginning of the pipeline.
    virtual bool Init(RwCommGeometry *pGeometry, const RwCommExportOptions *pGlobalData);

    // Apply the filter 
     virtual void Do(RwVector<RwCommVertex *> &vertices, RwVector<RwCommPrimitive> &primitives,
        RwCommGeometry *pGeometry, const RwCommExportOptions *pExportOptions);

     /** Set the filter's scale factor.*/
     virtual void SetScale(const RwCommVec3 &scale) { m_vScale = scale; }

private:

    RwCommVec3 m_vScale;

};

const RwCommType RwExpOffsetFilterType("RwExpOffsetFilter");

/**
*
*  \ingroup vertexpipeline
*
*   This vertex filter is responsible for transforming a geometry by a calculated offset transformation. The offest
*   transformation is calculated according to the following equation:
*   
*   \code Tsource * Inv(Tdest) \endcode
*   
*   Where Tsource is the geometry's world space transformation, and Tdest is a target world space transformation.
*   
*   The target transformation is set using the RwExpOffsetFilter::SetTarget() method.
*/
class RWEXP_API RwExpOffsetFilter : public RwExpVertexFilter  
{
public:
    RwExpOffsetFilter() : m_pTarget(NULL), m_vScale(1.0) {}
    virtual ~RwExpOffsetFilter() {}

    virtual RwCommType GetType() const { return RwExpOffsetFilterType; }

    virtual unsigned GetVertexDataMask() const { return RwCommVertex::POSITION; }

    // Initialise the filter. Called at the beginning of the pipeline.
    virtual bool Init(RwCommGeometry *pGeometry, const RwCommExportOptions *pGlobalData);

    // Apply the filter 
    virtual void Do(RwVector<RwCommVertex *> &vertices, RwVector<RwCommPrimitive> &primitives,
        RwCommGeometry *pGeometry, const RwCommExportOptions *pExportOptions);

    // Remove the scale from a world space matrix
    virtual void RemoveOffsetMatrixScale(RwCommMatrix &matrix) { matrix.RemoveScale(); }

    /** Set the target transformation.*/
    virtual void SetTarget(RwCommTransform *pTarget) { m_pTarget = pTarget; }
    virtual void SetScale(RwCommReal scale) { m_vScale = scale; }

private:

    RwCommTransform     *m_pTarget;
    RwCommReal          m_vScale;
};

const RwCommType RwExpTransformWorldSpaceFilterType("RwExpTransformWorldSpaceFilter");

/**
*
*  \ingroup vertexpipeline
*
*   This vertex filter is responsible for transforming a geometry by the global scene axis system, and by
*   the given global scaling factor.
*
*   \see RwCommScene::GetAxisSystem()
*/
class RWEXP_API RwExpTransformWorldSpaceFilter : public RwExpVertexFilter  
{
public:
    RwExpTransformWorldSpaceFilter() {}
    virtual ~RwExpTransformWorldSpaceFilter() {}

    virtual RwCommType GetType() const { return RwExpTransformWorldSpaceFilterType; }

    virtual unsigned GetVertexDataMask() const { return RwCommVertex::POSITION | RwCommVertex::NORMAL; }

    // Initialize the filter. Called at the beginning of the pipeline.
    virtual bool Init(RwCommGeometry *pGeometry, const RwCommExportOptions *pExportOptions);

    // Apply the filter 
     virtual void Do(RwVector<RwCommVertex *> &vertices, RwVector<RwCommPrimitive> &primitives,
        RwCommGeometry *pGeometry, const RwCommExportOptions *pExportOptions);

     virtual void SetGlobalScaling(RwCommReal scale) { m_vScale = scale; }

protected:
    RwCommMatrix    m_posTransformMatrix;
    RwCommMatrix    m_normTransformMatrix;
    RwCommReal      m_vScale;
};

#endif // !defined(AFX_RWEXPPOSITIONOPS_H__67312917_D4D2_4423_9065_5763ED920711__INCLUDED_)
