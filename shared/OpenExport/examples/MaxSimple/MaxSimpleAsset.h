// MaxSimpleAsset.h: interface for the MaxSimpleAsset class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAXSIMPLEASSET_H__6901B7B7_729E_4159_9C2B_C41E5A5967B1__INCLUDED_)
#define AFX_MAXSIMPLEASSET_H__6901B7B7_729E_4159_9C2B_C41E5A5967B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwCommAsset.h"
#include "RwCommExportOptions.h"

class MaxSimpleAsset  : public RwCommAsset  
{
public:
	MaxSimpleAsset();
	virtual ~MaxSimpleAsset();

    virtual RwCommError SetAssetScope(const AssetScope scope) { return RWCOMM_NOT_IMPLEMENTED; }
    virtual RwCommError GetAssetScope(AssetScope &scope) const;

    virtual RwCommError SetName(const RwWString &name);
    virtual RwCommError GetName(RwWString &name) const;

    virtual RwCommError SetEnable(const bool bEnable) { return RWCOMM_NOT_IMPLEMENTED; }
    virtual RwCommError GetEnable(bool &bEnable) const;

    virtual RwCommError SetAssetTemplateName(const RwWString &templateName);
    virtual RwCommError GetAssetTemplateName(RwWString &templateName) const;

    virtual RwCommError SetGlobalExportPath(const RwWString &exportPath) { return RWCOMM_NOT_IMPLEMENTED; }
    virtual RwCommError GetGlobalExportPath(RwWString &exportPath) const { return RWCOMM_NOT_IMPLEMENTED; }

    virtual RwCommError GetCustomExportFile(RwWString &exportFile) const { return RWCOMM_SUCCESS; }
    virtual RwCommError SetCustomExportFile(const RwWString &exportFile) { return RWCOMM_SUCCESS; }

    virtual RwCommError SetGUID(const RwWString &guid) { return RWCOMM_NOT_IMPLEMENTED; }
    virtual RwCommError GetGUID(RwWString &guid) const { return RWCOMM_NOT_IMPLEMENTED; }

    virtual RwCommError SetSelection(const RwCommScene *pScene, const RwVector<RwCommObject *> &vSelection) { return RWCOMM_NOT_IMPLEMENTED; }
    virtual RwCommError GetSelection(RwCommScene *pScene, RwVector<RwCommObject *> &vSelection) const { return RWCOMM_SUCCESS; }

    virtual RwCommError GetSelectionNames(RwVector<RwPair<RwWString, int> > &sNames) const { return RWCOMM_NOT_IMPLEMENTED; }

    virtual RwCommError SetOptions(const RwCommExportOptions &options) { return RWCOMM_NOT_IMPLEMENTED; }
    virtual RwCommError GetOptions(RwCommExportOptions &options) const { return RWCOMM_SUCCESS; }

    virtual RwCommError SaveSelection() { return RWCOMM_NOT_IMPLEMENTED; }
    virtual RwCommError LoadSelection() const { return RWCOMM_NOT_IMPLEMENTED; }
    virtual RwCommError LoadSelection(const RwVector<int> &SelectedAssest) const { return RWCOMM_NOT_IMPLEMENTED; }
    virtual RwCommError AddToSelection() { return RWCOMM_NOT_IMPLEMENTED; }
    virtual RwCommError RemoveFromSelection(const RwVector<int> &Indices) { return RWCOMM_NOT_IMPLEMENTED; }
    virtual RwCommError RemoveFromSelection(int Index) { return RWCOMM_NOT_IMPLEMENTED; }
    virtual RwCommError ClearSelection() { return RWCOMM_NOT_IMPLEMENTED; }

    virtual RwCommError RemoveAsset(const RwVector<int> &Indicies) { return RWCOMM_NOT_IMPLEMENTED; }
    virtual RwCommError RemoveAsset(int Index) { return RWCOMM_NOT_IMPLEMENTED; }

    virtual RwCommError SetParent(RwCommAsset *pParent) { return RWCOMM_NOT_IMPLEMENTED; }
	virtual RwCommAsset *GetParent() const { return NULL; }

   	virtual RwCommError AddChild(RwCommAsset *pChild) { return RWCOMM_NOT_IMPLEMENTED; }
   	virtual RwCommError RemoveChild(int i) { return RWCOMM_NOT_IMPLEMENTED; }
    virtual int         GetNumChildren() const { return 0; }
	virtual RwCommAsset *GetChild(int i) const { return NULL; }

    virtual RwCommError AddSelection(const RwCommAsset &other) { return RWCOMM_NOT_IMPLEMENTED; }

	virtual RwCommError SetDescription(const string& description) { return RWCOMM_NOT_IMPLEMENTED; }
	virtual RwCommError GetDescription(string &description) const { return RWCOMM_NOT_IMPLEMENTED; }
	virtual RwCommError SetDescription(const RwWString &description) { return RWCOMM_NOT_IMPLEMENTED; }
	virtual RwCommError GetDescription(RwWString &description) const { return RWCOMM_NOT_IMPLEMENTED; }

protected:

    RwWString    m_name;
    RwWString    m_templateName;
};

#endif // !defined(AFX_MAXSIMPLEASSET_H__6901B7B7_729E_4159_9C2B_C41E5A5967B1__INCLUDED_)
