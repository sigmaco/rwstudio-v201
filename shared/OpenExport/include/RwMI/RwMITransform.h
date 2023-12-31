// RwMITransform.h: interface for the RwMITransform class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWMITRANSFORM_H__4CD0103E_3A40_4120_96E4_134CE4B02FDE__INCLUDED_)
#define AFX_RWMITRANSFORM_H__4CD0103E_3A40_4120_96E4_134CE4B02FDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwMI.h"
#include "RwCommTransform.h"
#include "RwMIAttributes.h"
#include "RwMIAnimProp.h"

/**
*   \ingroup miscenegraph
*
*   This is the MI implementation of RwCommTransform.
*
*   \see RwCommTransform
*/
class RWMI_API RwMITransform : public RwCommTransform, public RwMIAttributes
{
public:
    RwMITransform();
    virtual ~RwMITransform();

    // User Data
    virtual RwCommError GetUserData(RwCommUserDataCollection &userDataColl);

    // Get attribute methods.
    virtual RwCommError GetAttribute(const RwWString &name, int &value, int index = 0);
    virtual RwCommError GetAttribute(const RwWString &name, float &value, int index = 0);
    virtual RwCommError GetAttribute(const RwWString &name, RwWString &value, int index = 0);
    virtual RwCommError GetAttributeNames(RwVector<RwWString> &vAttributeNames,
                                          RwVector<AttributeType> &vAttributeTypes);

    RwCommAnimType  GetAnimationType(int i);

    // Matrix querys
    virtual RwCommError GetObjMatrix(RwCommMatrix &matrix) const;
    virtual RwCommError GetWorldMatrix(RwCommMatrix &matrix) const;

    // Visibility
    virtual RwCommError GetVisibility(bool &bIsVisible) const;

    // Axis system
    virtual RwCommError GetAxisSystem(RwCommMatrix &matrix) const;

    // Vertex scaling
    virtual RwCommError GetScalePivots(RwCommMatrix &preScale,
                                       RwCommMatrix &postScale) const;
    
    virtual RwCommError GetNodeTagValue(int &tagValue, bool &bAutoGenerated) const;

    // RwMITransform methods
    RwCommError SetObjMatrix(RwCommReal frame, const RwCommMatrix &M);
    RwCommError SetObjMatrix(RwVector< RwMIAnimPropKey<RwCommMatrix> > &vKeys);

    RwCommError SetWorldMatrix(RwCommReal frame, const RwCommMatrix &M);
    RwCommError SetWorldMatrix(RwVector< RwMIAnimPropKey<RwCommMatrix> > &vKeys);

    RwCommError SetVisibility(RwCommReal frame, bool visibility);
    RwCommError SetVisibility(RwVector< RwMIAnimPropKey<bool> > &vKeys);

    RwCommError SetAxisSystem(const RwCommMatrix &M);

    RwCommError SetScalePivots(RwCommReal frame, const RwCommMatrix &pre,
                               const RwCommMatrix &post);

    RwCommError SetNodeTagValue(int tag, bool bAutoGenerated);

    RwCommError SetTransformAnimType(const RwCommAnimType &type); 

protected:
    RwCommMatrix                m_axisSysytem;

    RwMIAnimProp<bool>          m_visibility;
    RwMIAnimProp<RwCommMatrix>  m_localXForm;
    RwMIAnimProp<RwCommMatrix>  m_globalXForm;
    RwMIAnimProp<RwCommMatrix>  m_preScale;
    RwMIAnimProp<RwCommMatrix>  m_postScale;

    int                         m_tag;
    bool                        m_tagAutoGenerated;
    RwCommAnimType              m_animType;
};

#endif // !defined(AFX_RWMITRANSFORM_H__4CD0103E_3A40_4120_96E4_134CE4B02FDE__INCLUDED_)
