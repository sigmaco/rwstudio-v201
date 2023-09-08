// RwMIBlendShape.h: interface for the RwMIBlendShape class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWMIBLENDSHAPE_H__F73EF3B7_EFFD_4E04_B774_60CE3F5BE2A9__INCLUDED_)
#define AFX_RWMIBLENDSHAPE_H__F73EF3B7_EFFD_4E04_B774_60CE3F5BE2A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwMI.h"
#include "RwMIModifier.h"
#include "RwMIAnimProp.h"

#include "RwCommBlendShape.h"


struct RwMIBlendShapeTarget
{
    RwWString                m_name;
    RwVector<RwCommVec3>    m_vPositions;
    RwVector<RwCommVec3>    m_vNormals;
    RwMIAnimProp<RwCommReal>    m_weights;
};

/**
*   \ingroup miscenegraph
*
*   This is the MI implementation of RwCommBlendShape.
*
*   \see RwCommBlendShape
*/
class RWMI_API RwMIBlendShape : public RwCommBlendShape, public RwMIModifier
{
public:
    RwMIBlendShape();
    virtual ~RwMIBlendShape();

    // RwCommBlendShape methods
    virtual RwCommError GetNumTargets(int &nTargets) const;
    virtual RwCommError SetTargetWeight(int iTarget, RwCommReal weight);
    virtual RwCommError GetTargetWeight(int iTarget, RwCommReal &weight) const;
    virtual RwCommError GetTargetName(int iTarget, RwWString &name) const;

    // RwMIBlendShape methods
    RwCommError SetNumTargets(int nTargets);
    RwCommError SetTargetName(int nTarget, const RwWString &name);
    RwCommError AddTargetVertex(int nTarget, const RwCommVec3 &pos, const RwCommVec3 &normal);
    RwCommError AddTargetWeight(int nTarget, RwCommReal frame, RwCommReal weight);
    RwCommError GetTargetWeight(RwCommReal frame, int iTarget, RwCommReal &weight) const;

    // RwMIModifier methods
    virtual RwCommError Evaluate(RwCommReal frame, RwCommVertex &vertex, int index);

protected:
    RwVector<RwMIBlendShapeTarget> m_vTargets;
};

#endif // !defined(AFX_RWMIBLENDSHAPE_H__F73EF3B7_EFFD_4E04_B774_60CE3F5BE2A9__INCLUDED_)
