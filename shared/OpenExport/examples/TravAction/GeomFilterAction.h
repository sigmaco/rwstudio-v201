// GeomFilterAction.h: interface for the GeomFilterAction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GEOMFILTERACTION_H__63BDBCAA_E944_4160_967C_64F9F3146525__INCLUDED_)
#define AFX_GEOMFILTERACTION_H__63BDBCAA_E944_4160_967C_64F9F3146525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwExp.h"
#include "RwExpTraverseAction.h"

const RwCommType GeomFilterActionType("GeomFilterAction");

class GeomFilterAction : public RwExpTraverseAction 
{
public:
	GeomFilterAction();
	virtual ~GeomFilterAction();

    //RwCommObject method
    virtual RwCommType	GetType() const { return GeomFilterActionType; }

    // Destroy the object - called by RwCommObject::Release()
    virtual void Destroy(); 

    // Init the action
    virtual bool Init(const RwCommExportOptions *pGlobalData);

    // Apply the action
    virtual RwCommError Apply(RwVector<RwExpTraverseObject> &vTraverseList, const RwCommExportOptions *pGlobalData);

    // Set the filtering pattern
    void SetFilterPattern(const RwWString &pattern) { m_filterPattern = pattern; }

private:

    RwWString  m_filterPattern;
};

#endif // !defined(AFX_GEOMFILTERACTION_H__63BDBCAA_E944_4160_967C_64F9F3146525__INCLUDED_)
