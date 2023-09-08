// RwMaxPhysique.h: interface for the RwMaxPhysique class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWMAXPHYSIQUE_H__F943742D_600B_44E3_8EC0_C38677762076__INCLUDED_)
#define AFX_RWMAXPHYSIQUE_H__F943742D_600B_44E3_8EC0_C38677762076__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma warning(disable : 4786)
#pragma warning(disable : 4251)

#define PHYSIQUE_CLASS_ID_A 0x00100
#define PHYSIQUE_CLASS_ID_B	0x00000


#include "RwMax.h"
#include "RwCommSkin.h"
#include "RwCommObjCreator.h"

class PhysiqueHandle;

struct RwMaxSkinWeight
{
	RwMaxSkinWeight() : w(0.0), node(NULL) {}
	RwCommReal	w;
	INode   *node;
};

struct PhysiqueData {
	RwVector< RwVector<RwMaxSkinWeight> > weights;
    RwSet<INode *> bones;
};

// =============================================================================
// ------------------------  ExtractPhysiqueData CLASS  ------------------------
// =============================================================================

class PhysiqueHandle
{
public:
    virtual void ExtractPhysiqueData(Modifier *pModifier,
                        INode *pNode, PhysiqueData &) = 0;
};

// =============================================================================
// --------------------------  Physique2Handle CLASS  --------------------------
// =============================================================================

class Physique2Handle : public PhysiqueHandle
{
public:
    void ExtractPhysiqueData(Modifier *pModifier, INode *pNode, PhysiqueData &);
};

// =============================================================================
// --------------------------  Physique3Handle CLASS  --------------------------
// =============================================================================

class Physique3Handle : public PhysiqueHandle
{
public:
    void ExtractPhysiqueData(Modifier *pModifier, INode *pNode, PhysiqueData &);
};

// =============================================================================
// -------------------------  Physique32Handle CLASS  --------------------------
// =============================================================================

class Physique32Handle : public PhysiqueHandle
{
public:
    void ExtractPhysiqueData(Modifier *pModifier, INode *pNode, PhysiqueData &);
};

// =============================================================================
// ---------------------------  RwMaxPhysique CLASS  ---------------------------
// =============================================================================

/**
*  \ingroup maxscenegraph 
*
*   This is RwMax's Physique implementation of the RwCommSkin class. The class 
*   encapsulates a Physique modifier.
*
*   \see RwCommSkin
*/
class RWMAX_API RwMaxPhysique : public RwCommSkin 
{
public:
	RwMaxPhysique(Modifier *pModifier, INode *pModifiedNode);
	virtual ~RwMaxPhysique();

    // Animatable methos
    virtual RwCommError OnSetFrame(RwCommReal frame);

    // RwCommSkin methods
	virtual RwCommError GetSkinWeights(const RwCommVertexId &id,
                            RwVector<RwCommSkinWeight> &vWeights) const;
    virtual RwCommError GetInfluenceBones(RwVector<RwCommTransform *> &vBones) const;

    //RwMaxPhysique methods
    RwCommError GetInfluenceBones(RwVector<INode *> &vBones) const;

    /** Get the Physique modifier*/
    Modifier *GetModifier() const { return m_pModifier; }

    /** Get the Physique modifier INode*/
    INode   *GetINode() const { return m_pModifiedNode; }

protected:

	static PhysiqueHandle* CreatePhysiqueHandle();


protected:

	static PhysiqueHandle   *m_pPhysiqueHandle;
	PhysiqueData	        m_sPhysiqueData;

    INode                   *m_pModifiedNode;
    Modifier                *m_pModifier;

};

// =============================================================================
// -----------------------  RwMaxPhysiqueCreator CLASS  ------------------------
// =============================================================================

/**
*  \ingroup maxbuilder 
*
*   This class is responsible for creating an RwMaxPhysiqueCreator object.
*
*   \see RwMaxPhysiqueCreator
*/
class RWMAX_API RwMaxPhysiqueCreator  : public RwCommObjCreator
{
public:
	RwMaxPhysiqueCreator();
	virtual ~RwMaxPhysiqueCreator();

	virtual RwCommObject *Create(const RwCommCreationData *pCreationData,
								 const RwCommExportOptions *pExportOptions) const;

	virtual void Init(	RwCommObject *pObject,
						const RwCommCreationData *pCreationData,
						const RwCommExportOptions *pExportOptions) const;

	virtual void Attach(RwCommObject *pObject, RwCommObject *pParent, RwCommScene *pScene,
                        const RwCommCreationData *pCreationData) const;

};
#endif // !defined(AFX_RWMAXPHYSIQUE_H__F943742D_600B_44E3_8EC0_C38677762076__INCLUDED_)
