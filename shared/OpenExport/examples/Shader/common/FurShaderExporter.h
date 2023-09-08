// FurShaderExporter.h: interface for the FurShaderExporter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FURSHADEREXPORTER_H__1B5A2961_7C99_4886_8FCC_3BDB4790082C__INCLUDED_)
#define AFX_FURSHADEREXPORTER_H__1B5A2961_7C99_4886_8FCC_3BDB4790082C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwExpRpMaterial.h"

class FurShaderExporter  : public RwExpRpMaterial
{
public:
	FurShaderExporter();
	virtual ~FurShaderExporter();

    // RwCommExportObject methods
    virtual RwCommError     Export(RwCommObject *pObj,
                                RwCommObject *pParent,
                                RwCommScene *pScene,
                                const RwCommExportOptions *pExportOptions );

    // RwExpRpMaterial methods
    virtual RpMaterial      *GetRpMaterial() const { return m_pFurMaterial; }

private:
    RpMaterial  *m_pFurMaterial;

};

#endif // !defined(AFX_FURSHADEREXPORTER_H__1B5A2961_7C99_4886_8FCC_3BDB4790082C__INCLUDED_)
