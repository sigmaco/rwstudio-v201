// RwExpModifiable.h: interface for the RwExpModifiable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPMODIFIABLE_H__A99298BC_7957_4595_83AA_E45429CE658B__INCLUDED_)
#define AFX_RWEXPMODIFIABLE_H__A99298BC_7957_4595_83AA_E45429CE658B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwCommObject.h"

class RwCommModifier;

/**
*  \ingroup commscenegraph  
*
*   This is the base class for all classes that can be modified. A modifiable class simply means that
*   this class can have an abstract modifier attached to it. A modifier is an object that modifies
*   the hosting RwCommModifiable object in some way. 
*/
class RWCOMM_API RwCommModifiable : public RwCommObject
{
public:
    RwCommModifiable();
    virtual ~RwCommModifiable();

    int             GetNumModifiers() const;
    void            AddModifier(RwCommModifier *pModifier);
    RwCommModifier *GetModifier(int i);

protected:
    RwVector<RwCommModifier *> m_vModifiers;
};

#endif // !defined(AFX_RWEXPMODIFIABLE_H__A99298BC_7957_4595_83AA_E45429CE658B__INCLUDED_)
