// RwExpRpAtomic.h: interface for the RwExpRpAtomic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPRPATOMIC_H__13B69722_87CB_4E65_864B_A7EF231B5649__INCLUDED_)
#define AFX_RWEXPRPATOMIC_H__13B69722_87CB_4E65_864B_A7EF231B5649__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwExp.h"
#include "RwCommExportObject.h"
#include "RwExpVertexCollection.h"


class RwCommGeometry;

/**
*  \ingroup exportobject
*   This is an intermediate utility base class for geometry exporters.
*   It holds the geometry exporters' vertex collection and RpAtomic object.
*
*/
class RWEXP_API RwExpRpAtomic  : public RwCommExportObject
{
public:
    RwExpRpAtomic(RwExpVertexPipeline &rVertexPipeline);
    virtual ~RwExpRpAtomic();

    /** Get the geometry's atomic. */
    RpAtomic                *GetRpAtomic() { return m_pAtomic; }

    /** Get the geometry's vertex collection.*/
    const RwExpVertexCollection &GetVertexCollection() const { return m_collection; }

    RwCommGeometry          *GetInstance(RwCommGeometry* pGeometry);

    RwCommTransform         *GetSkinAttachedTransform(RwCommGeometry *pGeometry, const RwCommExportOptions *pOptions);

protected:

    void                    GetScaleAndMirroring(RwCommGeometry *pGeometry, int frame, RwCommVec3 &scale, bool &bMirror);
    static RpMaterial       *CheckMaterialFX(RpMaterial *material, void *pData);
    bool                    HasSkin(RwCommGeometry *pGeometry);
    bool                    GetLightFlag(RwCommGeometry *pGeometry, bool &bFlag);

    RwCommTransform         *GetSkeletonRoot(RwCommGeometry *pGeometry, const RwCommExportOptions *pOptions);

protected:

    RpAtomic                *m_pAtomic;
    RwExpVertexCollection   m_collection;
    RwCommTransform         *m_pAttachTransform;
};

#endif // !defined(AFX_RWEXPRPATOMIC_H__13B69722_87CB_4E65_864B_A7EF231B5649__INCLUDED_)
