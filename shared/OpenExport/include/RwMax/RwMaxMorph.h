// RwMaxMorph.h: interface for the RwMaxMorph class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWMAXMORPH_H__5160046E_1B56_44CD_99EA_4DD35D0B7956__INCLUDED_)
#define AFX_RWMAXMORPH_H__5160046E_1B56_44CD_99EA_4DD35D0B7956__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "RwMax.h"
#include "RwCommMorph.h"
#include "RwCommObjCreator.h"
#include "RwMaxTraverseControllers.h"

#define MORPH_CLASS_ID 0xff4b1001
/**
*  \ingroup maxbuilder 
*
*  This class is responsible for creating an RwCommMorph object. The RwCommMorph object in the 3ds max
*  architecture, resresents any generic mesh deformation operation. Each time the RwMaxBuilder encounters a mesh
*  that has a modifier assigned to it, it attempts to find its RwCommObjCreator object. If no creator object
*  is found, the builder creates a generic RwCommMorph object and attaches it to its RwMaxMesh. This is to identify
*  that the RwMaxMesh has some sort of deformation modifier assigned to it.
*  
*
*   \see RwCommMorph
*/

class RWMAX_API RwMaxMorphCreator : public RwCommObjCreator, public RwMaxTraverseControllers
{
public:
	RwMaxMorphCreator() : RwCommObjCreator() {}
	virtual ~RwMaxMorphCreator() {}

	virtual RwCommObject *Create(const RwCommCreationData *pCreationData,
								 const RwCommExportOptions *pExportOptions) const;

	virtual void Init(	RwCommObject *pObject,
						const RwCommCreationData *pCreationData,
						const RwCommExportOptions *pExportOptions) const;

	virtual void Attach(RwCommObject *pObject, RwCommObject *pParent, RwCommScene *pScene,
                        const RwCommCreationData *pCreationData) const;

    virtual void TraverseControllers(RwMaxBuilder &rBuilder, RwCommCreationData *pCreationData, RwCommObject *pParent);

};

#endif // !defined(AFX_RWMAXMORPH_H__5160046E_1B56_44CD_99EA_4DD35D0B7956__INCLUDED_)
