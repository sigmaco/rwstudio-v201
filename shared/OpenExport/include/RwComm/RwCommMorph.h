// RwCommMorph.h: interface for the RwCommMorph class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWCOMMMORPH_H__89BF8F7F_F428_4066_94D7_509392CAC1DE__INCLUDED_)
#define AFX_RWCOMMMORPH_H__89BF8F7F_F428_4066_94D7_509392CAC1DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwCommModifier.h"

const RwCommType RwCommMorphType("RwCommMorph");

/**
*  \ingroup commscenegraph  
*
*  This is the base class for all modifiers that perform any kind of vertex
*  modification operation on a geometry.
*/
class RWCOMM_API RwCommMorph : public RwCommModifier
{
public:

    // Animated channels
    static RwCommAnimatedChannel MorphChannel;

    RwCommMorph();
    virtual ~RwCommMorph();

    // Type methods
    virtual RwCommType  GetType() const { return RwCommMorphType; }
    virtual void        GetTypeHierarchy(RwVector<RwCommType> &vTypeVec) const;
};

#endif // !defined(AFX_RWCOMMMORPH_H__89BF8F7F_F428_4066_94D7_509392CAC1DE__INCLUDED_)
