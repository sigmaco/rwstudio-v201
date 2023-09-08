// RwMIMorph.h: interface for the RwMIMorph class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWMIMORPH_H__95927F13_351D_49F7_93B6_FBB5EDC3C736__INCLUDED_)
#define AFX_RWMIMORPH_H__95927F13_351D_49F7_93B6_FBB5EDC3C736__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwMI.h"
#include "RwMIModifier.h"

#include "RwCommMorph.h"

struct RwMIMorphTarget
{
    RwMIMorphTarget() : m_time(0.0) {}
    RwMIMorphTarget(RwCommReal time) : m_time(time) {}

    bool operator() (const RwMIMorphTarget &t1, const RwMIMorphTarget &t2) const
                    { return t1.m_time < t2.m_time; }

    RwCommReal                  m_time;
    RwVector<RwCommVec3>    m_vPositions;
    RwVector<RwCommVec3>    m_vNormals;
};

/**
*   \ingroup miscenegraph
*
*   This is the MI implementation of RwCommMorph.
*
*   \see RwCommMorph
*/
class RWMI_API RwMIMorph : public RwCommMorph, public RwMIModifier
{
public:
    RwMIMorph();
    virtual ~RwMIMorph();

    // RwMIMorph methods
    RwCommError AddTargetVertex(RwCommReal frame, const RwCommVec3 &pos,
                                const RwCommVec3 &normal);

    // RwMIModifier methods
    virtual RwCommError Evaluate(RwCommReal frame, RwCommVertex &vertex, int index);

protected:
    RwSet<RwMIMorphTarget, RwMIMorphTarget> m_sTargets;
};

#endif // !defined(AFX_RWMIMORPH_H__95927F13_351D_49F7_93B6_FBB5EDC3C736__INCLUDED_)
