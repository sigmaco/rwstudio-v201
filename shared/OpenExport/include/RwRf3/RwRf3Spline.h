// RwRf3Spline.h: interface for the RwRf3Spline class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWRF3SPLINE_H__CAA1C7C5_04A4_43DD_8424_BF21764F6541__INCLUDED_)
#define AFX_RWRF3SPLINE_H__CAA1C7C5_04A4_43DD_8424_BF21764F6541__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwRf3.h"
#include "RwRf3Geometry.h"

#include "RwCommSpline.h"

#define RWRF3_SPLINE_TAG                    L"spline"
#define RWRF3_SPLINE_ISCLOSED               L"closed"

/**
*   \ingroup rf3write
*
*   This class is responsible for exporting RwCommSpline objects. In its Export() method the class calls
*   the base class implementation of RwRf3ExpGeometry::Export() for generating all vertex and primitive
*   information. 
*   
*   In its constructor, the class registers an RwRf3RemapFilter for remapping vertices,
*   in its vertex pipeline.
*
*   \see RwRf3ExpGeometry
*   \see RwExpRemapFilter
*/
class RF3_API RwRf3ExpSpline : public RwRf3ExpGeometry 
{
public:
	RwRf3ExpSpline();
	virtual ~RwRf3ExpSpline();

    virtual RwCommError     Export( RwCommObject* pObj,
                                RwCommObject* pParent,
                                RwCommScene* pScene,
                                const RwCommExportOptions* pExportOptions );

};

/**
*   \ingroup rf3read
*
*   This class builds an RwMISpline from an XML node. 
*
*   \see RwMISpline
*   \see RwRf3GeometryCreator
*/
class RF3_API RwRf3SplineCreator : public RwRf3GeometryCreator
{
public:
    RwRf3SplineCreator() {}
    virtual ~RwRf3SplineCreator() {}

	virtual RwCommObject* Create(const RwCommCreationData* pCreationData,
								const RwCommExportOptions* pExportOptions) const;

protected:

    virtual void BuildPrimList(RwRf3PrimList *pPrimList, RwMIGeometry *pGeometry, RwCommScene *pScene) const;

};

#endif // !defined(AFX_RWRF3SPLINE_H__CAA1C7C5_04A4_43DD_8424_BF21764F6541__INCLUDED_)
