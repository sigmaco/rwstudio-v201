// RwMaxScene.h: interface for the RwMaxScene class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWMAXSCENE_H__BADD6167_AA46_48A1_9DCA_478E79AA203E__INCLUDED_)
#define AFX_RWMAXSCENE_H__BADD6167_AA46_48A1_9DCA_478E79AA203E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma warning(disable : 4786)
#pragma warning(disable : 4251)

#include "RwMax.h"
#include "RwCommScene.h"
#include "RwCommShader.h"

/**
*  \ingroup maxscenegraph 
*
*   This is the RwMax extension class to the RwCommScene. The class maps 3ds max objects to RwCommObjects.
*   Each time a new RwCommObject is created in the RwMaxBuilder, it is mapped to its original max object using
*   the class's MapObject() method. 
*
*   \see RwCommScene
*/
class RWMAX_API RwMaxScene  : public RwCommScene
{
public:
	RwMaxScene();
	virtual ~RwMaxScene();

    // Scene settings
    virtual RwCommReal  FramesToSec(RwCommReal frame) const;
    virtual RwCommReal  GetStartFrame() const;
    virtual RwCommReal  GetEndFrame() const;


    void    MapObject(Animatable *pAnimatable, RwCommObject *pObject);
    RwCommObject *GetCommObject(Animatable *pAnimatable);

    virtual RwCommError GetAxisSystem(RwCommMatrix &matrix) const;

    virtual RwCommError GetAmbientColor(RwCommVec3 &color) const;
    virtual RwCommError GetBackgroundColor(RwCommVec3 &color) const;

    virtual RwCommError GetDescription(RwWString &description) const { return RWCOMM_NOT_IMPLEMENTED; }
    virtual RwCommError GetGUID(RwWString &guid) const { return RWCOMM_NOT_IMPLEMENTED; }

private:

    RwMap<Animatable *, RwCommObject *> m_mObjects;
};

#endif // !defined(AFX_RWMAXSCENE_H__BADD6167_AA46_48A1_9DCA_478E79AA203E__INCLUDED_)
