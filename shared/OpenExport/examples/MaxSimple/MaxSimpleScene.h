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

#include "stdafx.h"

#include "RwCommScene.h"
#include "RwCommShader.h"


class MaxSimpleScene  : public RwCommScene
{
public:
	MaxSimpleScene();
	virtual ~MaxSimpleScene();

    // Destroy the object - called by RwCommObject::Release()
    virtual void Destroy() { delete this; }

    // Scene settings
    virtual RwCommReal  FramesToSec(RwCommReal frame) const;
    virtual RwCommReal  GetStartFrame() const;
    virtual RwCommReal  GetEndFrame() const;

    virtual RwCommError GetAxisSystem(RwCommMatrix &matrix) const;

    virtual RwCommError GetDescription(RwWString &description) const { return RWCOMM_NOT_IMPLEMENTED; }
    virtual RwCommError GetGUID(RwWString &guid) const { return RWCOMM_NOT_IMPLEMENTED; }

};

#endif // !defined(AFX_RWMAXSCENE_H__BADD6167_AA46_48A1_9DCA_478E79AA203E__INCLUDED_)
