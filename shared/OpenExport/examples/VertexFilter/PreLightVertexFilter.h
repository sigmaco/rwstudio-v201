// PreLightVertexFilter.h: interface for the PreLightVertexFilter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRELIGHTVERTEXFILTER_H__D9A5C702_C7FD_40EB_86E3_91B95115C826__INCLUDED_)
#define AFX_PRELIGHTVERTEXFILTER_H__D9A5C702_C7FD_40EB_86E3_91B95115C826__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwExp.h"
#include "RwExpVertexFilter.h"

const RwCommType PreLightVertexFilterType("PreLightVertexFilter");

class PreLightVertexFilter  : public RwExpVertexFilter  
{
public:
	PreLightVertexFilter();
	virtual ~PreLightVertexFilter();

    virtual RwCommType GetType() const { return PreLightVertexFilterType; }

    // Destroy the object - called from RwCommObject::Release()
    virtual void Destroy();

    // Return the vertex data format we are modifying
    virtual unsigned GetVertexDataMask() const { return RwCommVertex::COLOR; }

    // Initialise the filter. Called at the beginning of the pipeline.
    virtual bool Init(RwCommGeometry *pGeometry, const RwCommExportOptions *pExportOptions);

    // Apply the filter 
     virtual void Do(RwVector<RwCommVertex*>& vertices, RwVector<RwCommPrimitive>& primitives,
                     RwCommGeometry* pGeometry, const RwCommExportOptions* pExportOptions);

     // Light attributes
     void SetLightDirection(const RwCommVec3 &direction) { m_lightDirection = direction; }
     void SetAmbientIntensity(RwCommReal i) { m_ambientIntensity = i; }
     void SetDiffuseIntensity(RwCommReal i) { m_diffuseIntensity = i; }

private:

    RwCommVec3  m_lightDirection;
    RwCommReal  m_ambientIntensity;
    RwCommReal  m_diffuseIntensity;
};

#endif // !defined(AFX_PRELIGHTVERTEXFILTER_H__D9A5C702_C7FD_40EB_86E3_91B95115C826__INCLUDED_)
