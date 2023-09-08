// RwMILight.h: interface for the RwMILight class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWMILIGHT_H__85D30533_77BC_4167_BB1B_B695C984CB7A__INCLUDED_)
#define AFX_RWMILIGHT_H__85D30533_77BC_4167_BB1B_B695C984CB7A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwMI.h"
#include "RwCommLight.h"
#include "RwMIAttributes.h"
#include "RwMIAnimProp.h"

/**
*   \ingroup miscenegraph
*
*   This is the MI implementation of RwCommLight.
*
*   \see RwCommLight
*/
class RWMI_API RwMILight : public RwCommLight, public RwMIAttributes
{
public:
    RwMILight();
    virtual ~RwMILight();

    virtual RwCommError GetUserData(RwCommUserDataCollection &userDataColl);

    // Get attribute methods.
    virtual RwCommError GetAttribute(const RwWString &name, int &value, int index = 0);
    virtual RwCommError GetAttribute(const RwWString &name, float &value, int index = 0);
    virtual RwCommError GetAttribute(const RwWString &name, RwWString &value, int index = 0);
    virtual RwCommError GetAttributeNames(RwVector<RwWString> &vAttributeNames,
                                          RwVector<AttributeType> &vAttributeTypes);

    // Shape methods
    virtual RwCommError GetVisibility(bool &bIsVisible) const;

    //Light Methods
    virtual RwCommError GetLightType(LightType &type) const;

    virtual RwCommError GetColor(RwCommVec3 &color) const;
    virtual RwCommError GetIntensity(RwCommReal &intensity) const;

    virtual RwCommError GetAttenuationValues(RwCommReal &start, RwCommReal &end) const;
    virtual RwCommError GetConeAngle(RwCommReal &angle) const;
    virtual RwCommError GetHotSpotAngle(RwCommReal &angle) const;

    // RwMILight methods
    RwCommError SetLightType(LightType lt);

    RwCommError SetVisibility(RwCommReal frame, bool visibility);
    RwCommError SetVisibility(RwVector< RwMIAnimPropKey<bool> > &vKeys);

    RwCommError SetColor(RwCommReal frame, const RwCommVec3 &color);
    RwCommError SetColor(RwVector< RwMIAnimPropKey<RwCommVec3> > &vKeys);

    RwCommError SetIntensity(RwCommReal frame, RwCommReal intensity);
    RwCommError SetIntensity(RwVector< RwMIAnimPropKey<RwCommReal> > &vKeys);

    RwCommError SetAttenuationValues(RwCommReal frame, RwCommReal start, RwCommReal end);
    RwCommError SetAttenuationStart(RwVector< RwMIAnimPropKey<RwCommReal> > &vKeys);
    RwCommError SetAttenuationEnd(RwVector< RwMIAnimPropKey<RwCommReal> > &vKeys); 

    RwCommError SetConeAngle(RwCommReal frame, RwCommReal angle);
    RwCommError SetConeAngle(RwVector< RwMIAnimPropKey<RwCommReal> > &vKeys);

    RwCommError SetHotSpotAngle(RwCommReal frame, RwCommReal angle);
    RwCommError SetHotSpotAngle(RwVector< RwMIAnimPropKey<RwCommReal> > &vKeys);
    
protected:
    LightType                   m_eLightType;
    RwMIAnimProp<bool>          m_visibility;
    RwMIAnimProp<RwCommVec3>    m_color;
    RwMIAnimProp<RwCommReal>    m_intensity;
    RwMIAnimProp<RwCommReal>    m_attenStart;
    RwMIAnimProp<RwCommReal>    m_attenEnd;
    RwMIAnimProp<RwCommReal>    m_coneAngle;
    RwMIAnimProp<RwCommReal>    m_hotSpotAngle;
};

#endif // !defined(AFX_RWMILIGHT_H__85D30533_77BC_4167_BB1B_B695C984CB7A__INCLUDED_)
