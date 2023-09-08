// RwExpRwHint.h: interface for the RwExpRwHint class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_RwExpRwHint_H__8EB75E7E_3AD0_41BF_9129_0A797987472B__INCLUDED_)
//#define AFX_RwExpRwHint_H__8EB75E7E_3AD0_41BF_9129_0A797987472B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwExp.h"
#include "RwCommExportObject.h"
#include "RwCommHint.h"


class RWEXP_API RwExpRwHint : public  RwCommExportObject
{
public:
	RwExpRwHint();
	virtual ~RwExpRwHint();

    virtual RwCommError     Export( RwCommObject* pObj,
                                RwCommObject* pParent,
                                RwCommScene* pScene,
                                const RwCommExportOptions* pExportOptions );

    

protected:

    //Texture*     m_pTexture;

};

//#endif // !defined(AFX_RwExpRwHint_H__8EB75E7E_3AD0_41BF_9129_0A797987472B__INCLUDED_)
