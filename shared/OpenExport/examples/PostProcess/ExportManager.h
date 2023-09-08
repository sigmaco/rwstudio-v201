// ExportManager.h: interface for the CExportManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXPORTMANAGER_H__F3ADA813_28F9_401D_9467_6EE5098F0CDA__INCLUDED_)
#define AFX_EXPORTMANAGER_H__F3ADA813_28F9_401D_9467_6EE5098F0CDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RwExpBspExporter;
class RwExpAnmExporter;
class RwExpDffExporter;
class RwExpSplExporter;
class RwRf3Builder;
class RwRf3Exporter;
class RwExpStreamAssetList;
struct RwRf3AssetInfo;

#include "RwCommBuilder.h"
#include "RwCommExporter.h"

#include "RwExpExportOptions.h"
#include "RwExpMgr.h"
#include "RwExpMgrExportManager.h"

class CExportManager  : public RwExpMgrExportManager
{
public:
	CExportManager();
	virtual ~CExportManager();

protected:

    virtual RwCommError     StreamAssets(RwExpStreamAssetList *pAssetList, const RwWString &filename);

    void    ModifyClump(RpClump *pClump, int rep);
    static  RpAtomic    *AtomicCallback(RpAtomic * pAtomic, void *data);
    static  RpMaterial  *MaterialCallback(RpMaterial *material, void *data);

};

#endif // !defined(AFX_EXPORTMANAGER_H__F3ADA813_28F9_401D_9467_6EE5098F0CDA__INCLUDED_)
