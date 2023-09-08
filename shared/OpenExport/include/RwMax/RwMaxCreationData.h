// RwMaxCreationData.h: interface for the RwMaxCreationData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWMAXCREATIONDATA_H__AB9B4A45_6A99_43FE_8662_5068B25595B4__INCLUDED_)
#define AFX_RWMAXCREATIONDATA_H__AB9B4A45_6A99_43FE_8662_5068B25595B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwCommAnimatable.h"
#include "RwCommCreationData.h"
#include "RwMax.h"

/**
*   \ingroup maxcreationdata 
*
*   This class is the generic creation data base class, containing the object's INode. It is passed to all
*   RwCommObjCreator objects that build RwCommTransform and RwCommShape (meshes, patches cameras and lights) 
*   objects.
*/
class RWMAX_API RwMaxCreationData : public RwCommCreationData
{
public:
	RwMaxCreationData(INode *pNode);
	virtual ~RwMaxCreationData();

    /** Get the INode object*/
	INode *GetNode() const { return m_pNode; }

protected:

	INode *m_pNode;
};


/**
*   \ingroup maxcreationdata 
*
*   This class is used for the creation of modifiers. It is passed to all RwCommObjCreator objects that
*   build RwCommModifier objects.
*/
class RWMAX_API RwMaxModifierData : public RwMaxCreationData
{
public:
	RwMaxModifierData(INode *pNode, Modifier *pModifier);
	virtual ~RwMaxModifierData();

    /** Get the Modifier */
	Modifier *GetModifier() const { return m_pModifier; }

protected:

	Modifier *m_pModifier;
};

/**
*   \ingroup maxcreationdata 
*
*   This class is used for the creation of animations. It is passed to all RwCommObjCreator objects that
*   build RwCommAnimation objects.
*/
class RWMAX_API RwMaxControlData : public RwMaxCreationData
{
public:
    RwMaxControlData(INode *pNode, Control *pControl, RwCommAnimatedChannel channel = RwCommAnimatable::UndefinedChannel);
	virtual ~RwMaxControlData();

    /** Get the Controller object */
	Control *GetControl() const { return m_pControl; }

    /** Get animated channel.*/
    RwCommAnimatedChannel GetAnimatedChannel() const { return m_channel; }

    void SetAnimatedChannel(const RwCommAnimatedChannel &channel) { m_channel = channel; }
protected:

	Control *m_pControl;
    RwCommAnimatedChannel   m_channel;
};

/**
*   \ingroup maxcreationdata 
*
*   This class is used for the creation of shaders. It is passed to all RwCommObjCreator objects that
*   build RwCommShader objects.
*/
class RWMAX_API RwMaxMaterialData : public RwCommCreationData
{
public:
	RwMaxMaterialData(Mtl *pMtl) : RwCommCreationData(), m_pMtl(pMtl) {}

	virtual ~RwMaxMaterialData() {}

    /** Get the Mtl object*/
	Mtl *GetMaterial() const { return m_pMtl; }

protected:

	Mtl *m_pMtl;
};

/**
*   \ingroup maxcreationdata 
*
*   This class is used for the creation of textures. It is passed to all RwCommObjCreator objects that
*   build RwCommTexture objects.
*/
class RWMAX_API RwMaxTextureData : public RwCommCreationData
{
public:
	RwMaxTextureData(Texmap* pTex) : RwCommCreationData(), m_pTexmap(pTex) {}
	virtual ~RwMaxTextureData() {}

    /** Get the Texmap object*/
	Texmap *GetTexmap() const { return m_pTexmap; }

protected:

	Texmap *m_pTexmap;

};

#endif // !defined(AFX_RWMAXCREATIONDATA_H__AB9B4A45_6A99_43FE_8662_5068B25595B4__INCLUDED_)
