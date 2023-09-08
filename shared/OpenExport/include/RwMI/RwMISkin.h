// RwMISkin.h: interface for the RwMISkin class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWMISKIN_H__AD8B528C_994C_47AF_8CD8_545ECC29F0D3__INCLUDED_)
#define AFX_RWMISKIN_H__AD8B528C_994C_47AF_8CD8_545ECC29F0D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwMI.h"
#include "RwCommSkin.h"

/**
*   \ingroup miscenegraph
*
*   This is the MI implementation of RwCommSkin.
*
*   \see RwCommSkin
*/
class RWMI_API RwMISkin : public RwCommSkin
{
public:
    RwMISkin();
    virtual ~RwMISkin();

    virtual RwCommError GetSkinWeights(const RwCommVertexId &id,
                                       RwVector<RwCommSkinWeight> &vWeights) const;
    virtual RwCommError GetInfluenceBones(RwVector<RwCommTransform *> &vBones) const;

    // Set the number of vertices
    RwCommError SetNumVertices(int nVertices);

    // Add a weight to the given bertex index
    RwCommError AddVertexWeight(int index, const RwCommSkinWeight &weight);

    // Add an influence bone
    RwCommError AddBone(RwCommTransform *pBone);

    // Get a bone
    RwCommTransform *GetBone(int index);

protected:

    RwVector< RwVector<RwCommSkinWeight> >  m_vWeights;
    RwVector<RwCommTransform *>                 m_vBones;
};

#endif // !defined(AFX_RWMISKIN_H__AD8B528C_994C_47AF_8CD8_545ECC29F0D3__INCLUDED_)
