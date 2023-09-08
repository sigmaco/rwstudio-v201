// RwMIShader.h: interface for the RwMIShader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWMISHADER_H__443CCC34_C442_47D6_BDBE_B98E758DEE22__INCLUDED_)
#define AFX_RWMISHADER_H__443CCC34_C442_47D6_BDBE_B98E758DEE22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwMI.h"
#include "RwCommShader.h"
#include "RwMIAttributes.h"
#include "RwMIBaseShader.h"

/**
*   \ingroup miscenegraph
*
*   This is the MI implementation of RwCommShader.
*
*   \see RwCommShader
*/
class RWMI_API RwMIShader : public RwCommShader, public RwMIBaseShader, public RwMIAttributes 
{
public:
    RwMIShader();
    virtual ~RwMIShader();

    virtual RwCommError GetUserData(RwCommUserDataCollection &userDataColl);

    // Get attribute methods.
    virtual RwCommError GetAttribute(const RwWString &name, int &value, int index = 0);
    virtual RwCommError GetAttribute(const RwWString &name, float &value, int index = 0);
    virtual RwCommError GetAttribute(const RwWString &name, RwWString &value, int index = 0);
    virtual RwCommError GetAttributeNames(RwVector<RwWString> &vAttributeNames,
                                          RwVector<AttributeType> &vAttributeTypes);

    // Texture methods
    virtual RwCommError MapUVs(RwCommGeometry *pGeometry,
                               RwVector<RwCommTexture *> &vActiveTextures)
                              { return RWCOMM_NOT_IMPLEMENTED; }
    virtual RwCommError GetTextureUVSet(RwCommTexture *pTexture, int &uvSet) const;
    virtual RwCommError GetTexturesByType(TextureType type,
                                          RwVector<RwCommTexture *> &vTextures) const;
    virtual RwCommError GetTextureFactor(TextureType type, RwCommReal &factor) const;
    virtual RwCommError GetBlendFunctions(TextureType type,
                                          RwVector<RwCommBlendFunction> &vBlendFuncs) const;

    //Material attrributes
    virtual RwCommError GetAmbientValue(RwCommVec3 &) const;
    virtual RwCommError GetSpecularValue(RwCommVec3 &) const;
    virtual RwCommError GetDiffuseValue(RwCommVec3 &) const;
    virtual RwCommError GetShininess(RwCommReal &) const;
    virtual RwCommError GetShineStrength(RwCommReal &) const;
    virtual RwCommError GetTransparency(RwCommReal &) const;
    virtual RwCommError GetSelfIllumination(RwCommVec3 &) const;
    virtual RwCommError IsDoubleSided(bool &bStat) const;
};

#endif // !defined(AFX_RWMISHADER_H__443CCC34_C442_47D6_BDBE_B98E758DEE22__INCLUDED_)
