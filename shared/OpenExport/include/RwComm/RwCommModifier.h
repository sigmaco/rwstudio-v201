// RwExpModifier.h: interface for the RwExpModifier class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPMODIFIER_H__858FB514_DA2E_4672_8697_897991CD149B__INCLUDED_)
#define AFX_RWEXPMODIFIER_H__858FB514_DA2E_4672_8697_897991CD149B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwCommObject.h"
#include "RwCommAnimatable.h"

class RwCommModifiable;

const RwCommType RwCommModifierType("RwCommModifier");

/**
*  \ingroup commscenegraph  
*
*   This is the base class for all of the scene graph modifiers. An RwCommModifiable
*   object holds a list of RwCommModifier objects that modify it. 
*/
class RWCOMM_API RwCommModifier : public RwCommAnimatable 
{
public:
    RwCommModifier();
    virtual ~RwCommModifier();

    // Type methods
    virtual RwCommType  GetType() const { return RwCommModifierType; }
    virtual void        GetTypeHierarchy(RwVector<RwCommType> &vTypeVec) const;

    /**
     *  Sets the modifiable object. This method should not be called explicitly. It
     *  will be called from the RwCommModifiable::AddModifier() method.
     *
     *  \param pModifiable to which this modifier will be attached.
     */
    void SetModifiable(RwCommModifiable *pModifiable) { m_pModifiable = pModifiable; }

    /**
     *  Get the modifiable object.
     *  \return modifiable this modifier is attached to.
     */
    RwCommModifiable *GetModifiable() const { return m_pModifiable; }

protected:
    RwCommModifiable *m_pModifiable;
};

#endif // !defined(AFX_RWEXPMODIFIER_H__858FB514_DA2E_4672_8697_897991CD149B__INCLUDED_)
