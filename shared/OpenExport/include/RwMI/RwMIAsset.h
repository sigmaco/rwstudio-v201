// RwMIAsset.h: interface for the RwMIAsset class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWMIASSET_H__4986B501_415E_4DEC_9E34_F03667159F83__INCLUDED_)
#define AFX_RWMIASSET_H__4986B501_415E_4DEC_9E34_F03667159F83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwMI.h"
#include "RwCommAsset.h"
#include "RwCommExportOptions.h"

/**
*   \ingroup miscenegraph
*
*   This is the MI implementation of RwCommAsset.
*
*   \see RwCommAsset
*/
class RWMI_API RwMIAsset : public RwCommAsset  
{
public:
    RwMIAsset();
    virtual ~RwMIAsset();

    virtual RwCommError SetAssetScope(const AssetScope aScope);
    virtual RwCommError GetAssetScope(AssetScope &aScope) const;

    virtual RwCommError SetName(const RwWString &name);
    virtual RwCommError GetName(RwWString &name) const;

    virtual RwCommError SetEnable(const bool bEnable);
    virtual RwCommError GetEnable(bool &bEnable) const;

    virtual RwCommError SetAssetTemplateName(const RwWString &templateName);
    virtual RwCommError GetAssetTemplateName(RwWString &templateName) const;

    virtual RwCommError SetGlobalExportPath(const RwWString &exportPath);
    virtual RwCommError GetGlobalExportPath(RwWString &exportPath) const;

    virtual RwCommError SetCustomExportFile(const RwWString &exportFile);
    virtual RwCommError GetCustomExportFile(RwWString &exportFile) const;

    virtual RwCommError SetGUID(const RwWString &guid);
    virtual RwCommError GetGUID(RwWString &guid) const;


    virtual RwCommError SetSelection(const RwCommScene *pScene, const RwVector<RwCommObject *> &vSelection);
    virtual RwCommError GetSelection(RwCommScene *pScene, RwVector<RwCommObject *> &vSelection) const;

    virtual RwCommError GetSelectionNames(RwVector<RwPair<RwWString, int> > &sNames) const;

    virtual RwCommError SetOptions(const RwCommExportOptions &options);
    virtual RwCommError GetOptions(RwCommExportOptions &options) const;

    virtual RwCommError SaveSelection() { return RWCOMM_NOT_IMPLEMENTED; }
    virtual RwCommError LoadSelection() const { return RWCOMM_NOT_IMPLEMENTED; }
    virtual RwCommError LoadSelection(const RwVector<int> &SelectedAssest) const { return RWCOMM_NOT_IMPLEMENTED; }
    virtual RwCommError ClearSelection() { return RWCOMM_NOT_IMPLEMENTED; }

    virtual RwCommError AddToSelection() { return RWCOMM_NOT_IMPLEMENTED; }
    virtual RwCommError RemoveFromSelection( int Index) { return RWCOMM_NOT_IMPLEMENTED; }
    virtual RwCommError RemoveFromSelection( const RwVector<int> &Indices) { return RWCOMM_NOT_IMPLEMENTED; }

    virtual RwCommError SetParent(RwCommAsset *pParent);
    virtual RwCommAsset *GetParent() const;

    virtual RwCommError AddChild(RwCommAsset *pChild);
    virtual RwCommError RemoveChild(int i);
    virtual int         GetNumChildren() const;
    virtual RwCommAsset *GetChild(int i) const;

    virtual RwCommError AddSelection(const RwCommAsset &other);

	virtual RwCommError SetDescription(const RwWString& description);
	virtual RwCommError GetDescription(RwWString &description) const;

protected:
    AssetScope                  m_scope;
    bool                        m_bEnable;

	RwWString				    m_description;
    RwWString                    m_assetName;
    RwWString                    m_templateName;
    RwWString                    m_exportPath;
    RwWString                    m_exportFile;
    RwWString                    m_guid;

    RwCommExportOptions         m_exportOptions;
    RwCommAsset                 *m_pParent;
    RwVector<RwCommObject *>    m_vSelection;
    RwVector<RwCommAsset*>      m_vChildren;
};

#endif // !defined(AFX_RWMIASSET_H__4986B501_415E_4DEC_9E34_F03667159F83__INCLUDED_)
