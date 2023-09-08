// RwExpRpSpline.h: interface for the RwExpRpSpline class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPRPSPLINE_H__4C4B704B_D82D_4DD4_93A9_CE8AC9318006__INCLUDED_)
#define AFX_RWEXPRPSPLINE_H__4C4B704B_D82D_4DD4_93A9_CE8AC9318006__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwExp.h"
#include "RwCommExportObject.h"
#include "RwExpVertexCollection.h"
#include "RwExpVertexPipeline.h"

class RwCommSpline;

class RWEXP_API RwExpRpSplinePipeline: public RwExpVertexPipeline
{
public:
    RwExpRpSplinePipeline();
    virtual ~RwExpRpSplinePipeline() {}
};

const RwCommType RwExpRpSplineType("RwExpRpSpline");

/**
*  \ingroup exportobject
*
*   This is the class responsible for exporting the various spline curves in the scene. It is mapped to the
*   RwCommSpline object in the RwExpSplExporter class. The RwExpRpSpline class, like other geometry exporter classes,
*   uses a vertex pipeline and a vertex collection to store all of its extracted vertices. To access the
*   RwExpRpSpline vertex collection use the GetVertexCollection() method. The vertex pipeline is a static member of
*   the RwExpRpSpline class, shared by all instances of the class. To add your own custom vertex filter to it, you
*   do not need to refer to a specific instance of the RwExpRpSpline class, but just use the GetVertexPipeline()
*   method, which is also a static method. The RwExpRpSpline's vertex pipeline is constructed with the
*   RwExpTransformWorldSpaceFilter vertex filter only.

*   Unlike other exporter objects, the RwExpRpSpline exporter extracts all the spline's vertices in world space
*   instead of object space. 
*
*   \see RwExpTransformWorldSpaceFilter
*  
*/
class RWEXP_API RwExpRpSpline : public RwCommExportObject   
{
public:
    RwExpRpSpline();
    virtual ~RwExpRpSpline();

    virtual RwCommType      GetType() const { return RwExpRpSplineType; }

    virtual RwCommError     Export(RwCommObject *pObj,
                                RwCommObject *pParent,
                                RwCommScene *pScene,
                                const RwCommExportOptions *pExportOptions );

    /** Get the generated RpSpline object*/
    RpSpline *GetRpSpline() const { return m_pRpSpline; }

    /** Get the RwExpRpSpline's vertex collection.*/
    const RwExpVertexCollection &GetVertexCollection() const { return m_collection; }

    /** Get the vertex pipeline.*/
    static RwExpVertexPipeline &GetVertexPipeline() { return m_vertexPipeline; }

protected:

    RwCommError BuildRpSpline(RwCommSpline *pSpline, const RwCommExportOptions *pExportOptions);

protected:

    RpSpline                        *m_pRpSpline;
    RwExpVertexCollection           m_collection;
    static  RwExpRpSplinePipeline   m_vertexPipeline;

};

#endif // !defined(AFX_RWEXPRPSPLINE_H__4C4B704B_D82D_4DD4_93A9_CE8AC9318006__INCLUDED_)
