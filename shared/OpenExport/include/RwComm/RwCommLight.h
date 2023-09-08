// RwExpLight.h: interface for the RwExpLight class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPLIGHT_H__13666682_75E0_4F16_BC19_C00C4DD530E3__INCLUDED_)
#define AFX_RWEXPLIGHT_H__13666682_75E0_4F16_BC19_C00C4DD530E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwCommShape.h"

const RwCommType RwCommLightType("RwCommLight");

/**
*  \ingroup commscenegraph  
*
*  This is the base class for camera objects.
*/
class RWCOMM_API RwCommLight : public RwCommShape
{
public:
    enum LightType {POINT = 0, DIRECTIONAL, AMBIENT, SPOT};

    // Animated channels
    static RwCommAnimatedChannel ColorChannel;
    static RwCommAnimatedChannel ColorRChannel;
    static RwCommAnimatedChannel ColorGChannel;
    static RwCommAnimatedChannel ColorBChannel;
    static RwCommAnimatedChannel IntensityChannel;
    static RwCommAnimatedChannel AttenuationStartChannel;
    static RwCommAnimatedChannel AttenuationEndChannel;
    static RwCommAnimatedChannel ConeAngleChannel;
    static RwCommAnimatedChannel HotSpotChannel;

    RwCommLight();
    virtual ~RwCommLight();

    // Type methods
    /** \return RwCommLightType */
    virtual RwCommType  GetType() const { return RwCommLightType; }
    virtual void        GetTypeHierarchy(RwVector<RwCommType> &vTypeVec) const;

	virtual RwCommError ShapeOrientation(RwCommMatrix &flip) const;

    /**
     *  The supported light types are: POINT, for a point light, DIRECTIONAL, for a
     *  directional light, AMBIENT, for an ambient light and SPOT, for a spotlight.
     *
     *  \param type is filled with the light type.
     *
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetLightType(LightType &type) const = 0;

    /**
     *  \param color is filled with the light color.
     *
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetColor(RwCommVec3 &color) const = 0;

    /**
     *  \param intensity is filled with the light intensity.
     *
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetIntensity(RwCommReal &intensity) const = 0;

    /**
     *  \param start is filled with the distance where attenuation starts.
     *  \param end is filled with the distance where attenuation ends.
     *
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     *
     *  \note Should be used only for spot and point lights.
     */
    virtual RwCommError GetAttenuationValues(RwCommReal &start, RwCommReal &end) const = 0;
    
    /**
     *  \param angle is filled with the cone angle value in degrees.
     *
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     *
     *  \note Should be used only for spot lights.
     */
    virtual RwCommError GetConeAngle(RwCommReal &angle) const = 0;

    /**
     *  \param angle is filled with the hot spot value.
     *
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     *
     *  \note Should be used only for spot lights.
     */
    virtual RwCommError GetHotSpotAngle(RwCommReal &angle) const = 0;
};

#endif // !defined(AFX_RWEXPLIGHT_H__13666682_75E0_4F16_BC19_C00C4DD530E3__INCLUDED_)
