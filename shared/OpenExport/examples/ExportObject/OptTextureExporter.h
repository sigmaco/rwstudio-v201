// OptTextureExporter.h: interface for the OptTextureExporter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPTTEXTUREEXPORTER_H__9CCCB8E8_97B5_4ED8_B6EC_41462802FD77__INCLUDED_)
#define AFX_OPTTEXTUREEXPORTER_H__9CCCB8E8_97B5_4ED8_B6EC_41462802FD77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwExpRwTexture.h"

class OptTextureExporter  : public RwExpRwTexture
{
public:
	OptTextureExporter();
	virtual ~OptTextureExporter();

    // Destroy the object - called by RwCommObject::Release()
    virtual void Destroy();

    // Main export method
    virtual RwCommError     Export( RwCommObject *pObj,
                                    RwCommObject *pParent,
                                    RwCommScene *pScene,
                                    const RwCommExportOptions *pExportOptions );

protected:

    virtual RwCommError ExtractImage(RwCommTexture *pTexture, RwCommTexture *pAlphaTexture, RwCommImage &image);

};

#endif // !defined(AFX_OPTTEXTUREEXPORTER_H__9CCCB8E8_97B5_4ED8_B6EC_41462802FD77__INCLUDED_)
