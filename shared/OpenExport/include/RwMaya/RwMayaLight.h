#if !defined(RW_MAYA_LIGHT_H)
#define RW_MAYA_LIGHT_H

#include "RwMayaBlindData.h"

/**
*  \ingroup mayascenegraph 
*
*  This is the RwMaya implementation of the RwCommLight.
*
*   \see RwCommLight
*/
class RWMAYA_API RwMayaLight : public RwCommLight, public RwMayaBlindData
{
public:
	RwMayaLight(MDagPath dagPath);
	virtual ~RwMayaLight();

    /**
     *  Maya implementation of RwCommObject::GetUserData. Simply calls RwMayaBlindData::GetUserData.
     *  \param userDataColl the RwCommUserDataCollection in which data is to be filled.
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetUserData(RwCommUserDataCollection &userDataColl);

    // Get attribute methods.
    virtual RwCommError GetAttribute(const RwWString& name, int &value, int index = 0);
    virtual RwCommError GetAttribute(const RwWString& name, float &value, int index = 0);
    virtual RwCommError GetAttribute(const RwWString& name, RwWString &value, int index = 0);
    /**
     *  Maya implementation of RwCommAnimatable::GetAttributeNames. Simply calls RwMayaBlindData::GetObjectAttributeNames.
     *  \param vAttributeNames the names of all attributes found.
     *  \param vAttributeTypes the types of all attributes found.
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetAttributeNames(RwVector<RwWString>& vAttributeNames, RwVector<AttributeType> &vAttributeTypes);

    // Shape methods
    virtual RwCommError GetVisibility(bool &bIsVisible) const;

    /**
     *  Maya implementation of RwCommLight::GetLightType.
     *  
     *  Maya light types are converted as follows.
     *  
     *<TABLE BORDER CELLSPACING=2 BORDERCOLOR="#000000" CELLPADDING=7 WIDTH=562>
     *<TR>
     *  <TD WIDTH="43%" VALIGN="MIDDLE" BGCOLOR="#ffffff">
     *  <B><P ALIGN="CENTER">Maya Type;</B></TD>
     *  <TD WIDTH="57%" VALIGN="MIDDLE" BGCOLOR="#ffffff">
     *  <B><P ALIGN="CENTER">RwComm Type</B></TD>
     *</TR>
     *<TR>
     *  <TD WIDTH="43%" VALIGN="TOP" HEIGHT=11>
     *  <P>MFn::kAmbientLight</TD>
     *  <TD WIDTH="57%" VALIGN="TOP" HEIGHT=11>
     *  <P>RwCommLight::AMBIENT</TD>
     *</TR>
     *<TR>
     *  <TD WIDTH="43%" VALIGN="TOP" HEIGHT=11>
     *  <P>MFn::kDirectionalLight</TD>
     *  <TD WIDTH="57%" VALIGN="TOP" HEIGHT=11>
     *  <P>RwCommLight::DIRECTIONAL</TD>
     *</TR>
     *<TR>
     *  <TD WIDTH="43%" VALIGN="TOP" HEIGHT=11>
     *  <P>MFn::kPointLight</TD>
     *  <TD WIDTH="57%" VALIGN="TOP" HEIGHT=11>
     *  <P>RwCommLight::POINT</TD>
     *</TR>
     *<TR>
     *  <TD WIDTH="43%" VALIGN="TOP" HEIGHT=11>
     *  <P>MFn::kSpotLight</TD>
     *  <TD WIDTH="57%" VALIGN="TOP" HEIGHT=11>
     *  <P>RwCommLight::SPOT</TD>
     *</TR>
     *</TABLE>
     * 
     * Other Maya light types are not supported.
     *
     *  \param type is filled with the light type
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetLightType(LightType &type) const;

    virtual RwCommError GetColor(RwCommVec3 &color) const;
    /**
     *  This function is not implemented under RwMaya.
     *  \return RWCOMM_NOT_IMPLEMENTED.
     */
    virtual RwCommError GetIntensity(RwCommReal &intensity) const;

    virtual RwCommError GetAttenuationValues(RwCommReal &start, RwCommReal &end) const;
    virtual RwCommError GetConeAngle(RwCommReal &angle) const;
    /**
     *  This function is not implemented under RwMaya.
     *  \return RWCOMM_NOT_IMPLEMENTED.
     */
    virtual RwCommError GetHotSpotAngle(RwCommReal& angle) const;
    
    MDagPath             GetDagPath() { return m_dagPath; }

protected:
    MDagPath m_dagPath;
    MFnLight m_fnLight;

};


/**
*  \ingroup mayabuilder 
*
*  This class is responsible for creating an RwMayaLight object.
*
*   \see RwMayaLight
*   \see RwMayaObjCreator
*/
class RWMAYA_API RwMayaLightCreator : public RwMayaObjCreator
{
public:
	RwMayaLightCreator();
	virtual ~RwMayaLightCreator();

	virtual RwCommObject *Create(const RwCommCreationData *pCreationData,
        const RwCommExportOptions *pExportOptions) const;

	virtual void Init(  RwCommObject *pObject,
                        const RwCommCreationData *pCreationData,
                        const RwCommExportOptions *pExportOptions) const;

	virtual void Attach(RwCommObject* pObject, RwCommObject* pParent, RwCommScene* pScene,
                        const RwCommCreationData* pCreationData) const;

    virtual void Traverse(RwCommBuilder & rBuilder,
                            RwCommCreationData *pCreationData,
                            RwCommObject *pParent) const;
protected:
    RwMayaTraverseAnimations m_AnimationTraverse;
};

#endif // !defined(RW_MAYA_LIGHT_H)
