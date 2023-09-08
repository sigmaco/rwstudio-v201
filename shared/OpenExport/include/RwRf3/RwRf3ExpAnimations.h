// RwRf3ExpAnimations.h: interface for the RwRf3ExpAnimations class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWRF3EXPANIMATIONS_H__40EBE96C_C39D_420F_AD57_A893D5209657__INCLUDED_)
#define AFX_RWRF3EXPANIMATIONS_H__40EBE96C_C39D_420F_AD57_A893D5209657__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwRf3.h"
#include "RwExpTraverseAction.h"
#include "RwExpExporter.h"

const RwCommType RwRf3ExportAnimationsType("RwRf3ExpAnimations");

/**
*   \ingroup rf3write
*
*   This class is a traverse action, responsible for exporting animation data. The class works with conjunction
*   with the RwRf3ExpAnimatable class. The class loops through all frame values in the scene - for each one calling
*   SampleKey() on each exporter object, deriving from the RwRf3ExpAnimatable base class. It is the responsibility
*   of the class, deriving from the RwRf3ExpAnimatable base class, to sample the animated data at that frame and generate the appropriate
*   XML data, in the XML tree. At the end of the loop, the RwRf3ExpAnimations class calls the RwRf3ExpAnimatable::CreateAnimation()
*   method, allowing the implementing classes to finalize the key generation process.
*   Inside the frame loop, the class calls RwCommAnimatable::SetFrame() with the iterated frame, so there is no need
*   for the classes, implementing RwRf3ExpAnimatable, to call this method explicitly in their SampleKey() method.
*
*   \see RwRf3ExpAnimatable
*   \see RwCommAnimatable
*/
class RwRf3ExpAnimations  : public RwExpTraverseAction 
{
public:
	RwRf3ExpAnimations();
	virtual ~RwRf3ExpAnimations();

    //RwCommObject method
    virtual RwCommType	GetType() const { return RwRf3ExportAnimationsType; }

    // Apply the action
    virtual RwCommError Apply(RwVector<RwExpTraverseObject>& vTraverseList, const RwCommExportOptions* pExportOptions);

};

#endif // !defined(AFX_RWRF3EXPANIMATIONS_H__40EBE96C_C39D_420F_AD57_A893D5209657__INCLUDED_)
