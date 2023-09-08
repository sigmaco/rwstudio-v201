// RwMIHint.h: interface for the RwMIHint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWMIHINT_H__5CF4A4BC_28DD_49C1_92A9_3157F731AC07__INCLUDED_)
#define AFX_RWMIHINT_H__5CF4A4BC_28DD_49C1_92A9_3157F731AC07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwMI.h"
#include "RwCommHint.h"
#include "RwMIAnimProp.h"

class RWMI_API RwMIHint : public RwCommHint
{
public:
    RwMIHint();
    virtual ~RwMIHint();

    // Shape methods
    virtual RwCommError GetVisibility(bool &bIsVisible) const;

    // Pure virtuals from RwCommHint
    virtual RwCommError GetHintType(int &type) const;
    virtual RwCommError GetHintStrengths(float &shieldHintStrength,
                                         float &partitionHintStrength) const;
    virtual RwCommError GetHintDimensions(float &width, float &length, float &height) const;

    RwCommError SetVisibility(RwCommReal frame, bool visibility);
    RwCommError SetHintType(int type);
    RwCommError SetHintStrengths(float shieldHintStrength, float partitionHintStrength);
    RwCommError SetHintDimensions(float width, float length, float height);

protected:
    RwMIAnimProp<bool>  m_visibility;
    int                 m_type;
    float               m_shieldStrength;
    float               m_partitionStrength;
    float               m_width;
    float               m_height;
    float               m_length;
};

#endif // !defined(AFX_RWMIHINT_H__5CF4A4BC_28DD_49C1_92A9_3157F731AC07__INCLUDED_)
