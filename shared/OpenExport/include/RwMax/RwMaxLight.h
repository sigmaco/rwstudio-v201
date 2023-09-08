// RwMaxLight.h: interface for the RwMaxLight class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWMAXLIGHT_H__7CA258EB_CCD2_47EA_8240_C400A3E89FDF__INCLUDED_)
#define AFX_RWMAXLIGHT_H__7CA258EB_CCD2_47EA_8240_C400A3E89FDF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "RwCommLight.h"
#include "RwCommObjCreator.h"

#include "RwMax.h"
#include "RwMaxCustomAttributes.h"
#include "RwMaxTraverseModifiers.h"
#include "RwMaxTraverseControllers.h"

/**
*  \ingroup maxscenegraph 
*
*  This is the RwMax implementation of the RwCommLight.
*
*   \see RwCommLight
*/
class RWMAX_API RwMaxLight : public RwCommLight, public RwMaxCustomAttributes
{
public:
	RwMaxLight(GenLight *pLight);
	virtual ~RwMaxLight();

    virtual RwCommError GetUserData(RwCommUserDataCollection &userDataColl);

    // Get attribute methods.
    virtual RwCommError GetAttribute(const RwWString &name, int &value, int index = 0);
    virtual RwCommError GetAttribute(const RwWString &name, float &value, int index = 0);
    virtual RwCommError GetAttribute(const RwWString &name, RwWString &value, int index = 0);
    virtual RwCommError GetAttributeNames(RwVector<RwWString> &vAttributeNames, RwVector<AttributeType> &vAttributeTypes);

    // Shape methods
    virtual RwCommError GetVisibility(bool &bIsVisible) const;

	//Light Methods
    virtual RwCommError GetLightType(LightType &type) const;

    virtual RwCommError GetColor(RwCommVec3 &color) const;
    virtual RwCommError GetIntensity(RwCommReal &intensity) const;

    virtual RwCommError GetAttenuationValues(RwCommReal &start, RwCommReal &end) const;
    virtual RwCommError GetConeAngle(RwCommReal &angle) const;
    virtual RwCommError GetHotSpotAngle(RwCommReal &angle) const;

    void SetLightType(LightType lt) { m_eLightType = lt; }

    /** Get the max GenLight object.*/
    GenLight *GetLight() const { return m_pLight; }

protected:

	GenLight    *m_pLight;
    LightType   m_eLightType;

};

/**
*  \ingroup maxbuilder 
*
*  This class is responsible for creating an RwMaxLight object.
*
*   \see RwCommLight
*/
class RWMAX_API RwMaxLightCreator : public RwCommObjCreator,
 public RwMaxTraverseModifiers,
 public RwMaxTraverseControllers 
{
public:
	RwMaxLightCreator();
	virtual ~RwMaxLightCreator();

	virtual RwCommObject *Create(const RwCommCreationData *pCreationData,
								 const RwCommExportOptions *pExportOptions) const;

	virtual void Init(	RwCommObject *pObject,
						const RwCommCreationData *pCreationData,
						const RwCommExportOptions *pExportOptions) const;

	virtual void Attach(RwCommObject *pObject, RwCommObject *pParent, RwCommScene *pScene,
                        const RwCommCreationData *pCreationData) const;

    virtual void TraverseControllers(RwMaxBuilder &rBuilder, RwCommCreationData *pCreationData,
                                                                         RwCommObject *pParent);
};

#endif // !defined(AFX_RWMAXLIGHT_H__7CA258EB_CCD2_47EA_8240_C400A3E89FDF__INCLUDED_)
