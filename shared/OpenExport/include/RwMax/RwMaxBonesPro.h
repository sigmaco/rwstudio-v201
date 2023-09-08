// RwMaxBonesPro.h: interface for the RwMaxBonesPro class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWMAXBONESPRO_H__38F04D74_6FF5_4E97_A4A8_AB47D7FF66CF__INCLUDED_)
#define AFX_RWMAXBONESPRO_H__38F04D74_6FF5_4E97_A4A8_AB47D7FF66CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "RwMax.h"
#include "RwCommSkin.h"
#include "RwCommObjCreator.h"


// PROPEPRTY ID ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#define BP_CLASS_ID_OSM Class_ID(0x37e26e45, 0x543b6b2a)
#define BP_CLASS_ID_WSM Class_ID(0x37e26e45, 0x543b6a2a)

#define BP_TIME_ATTACHED TimeValue(0x80000000) 
#define BP_PROPID        (PROPID_USER+0xFF)

#define BP_PROPID_GET_N_BONES        (BP_PROPID + 0)
#define BP_PROPID_GET_N_VERTS        (BP_PROPID + 1)
#define BP_PROPID_GET_WEIGHTS        (BP_PROPID + 2)
#define BP_PROPID_GET_BONE           (BP_PROPID + 3)
#define BP_PROPID_GET_BONE_STAT      (BP_PROPID + 4)
#define BP_PROPID_GET_BONE_BY_NAME   (BP_PROPID + 5)
#define BP_PROPID_SET_BONE_FALLOFF   (BP_PROPID + 6)
#define BP_PROPID_SET_BONE_STRENGTH  (BP_PROPID + 7)
#define BP_PROPID_REFRESH            (BP_PROPID + 8)
#define BP_PROPID_SET_BONE_MARK      (BP_PROPID + 9)
#define BP_PROPID_GET_VERT_SEL       (BP_PROPID + 10)
#define BP_PROPID_SET_VERT_SEL       (BP_PROPID + 11)
#define BP_PROPID_SET_VERT_SEL_ANI   (BP_PROPID + 12)
#define BP_PROPID_SET_BONE_SEL_ANI   (BP_PROPID + 13)
#define BP_PROPID_GET_BV             (BP_PROPID + 14)
#define BP_PROPID_SET_BV             (BP_PROPID + 15)

#define BP_PROPID_END                (BP_PROPID + 50)


#pragma pack( push, before_bonespro_h )
#pragma pack(1)

typedef struct 
{
  int   nb;
  int   nv;
  float w[1];
} BonesPro_WeightArray;

typedef struct
{
  TimeValue t; 
  int       index; 
  char      name [256]; 
  INode     *node; 
  float     matrix [4][3]; 
  float     scale [3]; 
  float     falloff; 
  float     strength; 
  int       marked; 
} BonesPro_Bone;

typedef struct
{
  int index;
  int selected;
} BonesPro_Vertex;

typedef struct
{
  int bindex;
  int vindex;
  int included;
  int forced_weight;
} BonesPro_BoneVertex;

#pragma pack( pop, before_bonespro_h )


/**
*  \ingroup maxscenegraph 
*
*  This is the Bones Pro specific implementation of RwCommSkin. The class encapsulates a Bones Pro modifier.
*
*   \see RwCommSkin
*/
class RWMAX_API RwMaxBonesPro  : public RwCommSkin
{
public:
	RwMaxBonesPro(Modifier *pModifier);
	virtual ~RwMaxBonesPro();

    /** Called on each RwCommAnimatable::SetFrame(). The first time this method is called the object
    caches all the Bones Pro bones and vertex weights internally.*/
    virtual RwCommError OnSetFrame(RwCommReal frame);

    // RwCommSkin methods
	virtual RwCommError GetSkinWeights(const RwCommVertexId &id, RwVector<RwCommSkinWeight> &vWeights) const;
    virtual RwCommError GetInfluenceBones(RwVector<RwCommTransform *> &vBones) const;

    //RwMaxBonesPro methods
    RwCommError GetInfluenceBones(RwVector<INode *> &vBones) const;

    /** Get the Bones Pro modifier*/
    Modifier *GetModifier() const { return m_pModifier; }

protected:

	Modifier                    *m_pModifier;
    BonesPro_WeightArray        *m_pWeightArray;
	RwVector<BonesPro_Bone>	m_vBones;
};

/**
*  \ingroup maxbuilder 
*
*  This class is responsible for creating an RwMaxBonesPro object.
*
*   \see RwCommSkin
*/
class RWMAX_API RwMaxBonesProCreator  : public RwCommObjCreator
{
public:
	RwMaxBonesProCreator();
	virtual ~RwMaxBonesProCreator();

	virtual RwCommObject *Create(const RwCommCreationData *pCreationData,
								const RwCommExportOptions *pExportOptions) const;

	virtual void Init(	RwCommObject *pObject,
						const RwCommCreationData *pCreationData,
						const RwCommExportOptions *pExportOptions) const;

	virtual void Attach(RwCommObject *pObject, RwCommObject *pParent, RwCommScene *pScene,
                        const RwCommCreationData *pCreationData) const;

};

#endif // !defined(AFX_RWMAXBONESPRO_H__38F04D74_6FF5_4E97_A4A8_AB47D7FF66CF__INCLUDED_)
