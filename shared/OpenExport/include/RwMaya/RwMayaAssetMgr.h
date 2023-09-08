// RwMayaAssetMgr.h: interface for the RwMayaAssetMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWMAYAASSETMGR_H__4A490F96_3EA3_43FF_B5DC_D0543CC6C1D2__INCLUDED_)
#define AFX_RWMAYAASSETMGR_H__4A490F96_3EA3_43FF_B5DC_D0543CC6C1D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
*  \ingroup mayascenegraph 
*
*  This is an RwMaya implementation of the RwCommAssetMgr singleton. It operates
*  on RwMayaAssets in the scene.
*
*   \see RwCommAssetMgr
*/
class RWMAYA_API RwMayaAssetMgr : public RwCommAssetMgr
{
public:
	RwMayaAssetMgr();
	virtual ~RwMayaAssetMgr();

    virtual RwCommError GetAssets(RwVector<RwCommAsset *> &vAssets) const;
    virtual RwCommError CreateAsset(RwCommAsset *&pAsset) const;
    virtual RwCommError DeleteAsset(RwCommAsset *pAsset) const;

    virtual RwCommAsset *GetProjectAsset() const;
    virtual RwCommError CreateProjectAsset(RwCommAsset *&pAsset) const;

    virtual RwCommError CreatePartitionObject( );
    virtual RwCommError RemovePartitionObject( );

private:
    void                GetAssetsOfType(RwVector<RwCommAsset *> &vAssets,
                        RwCommAsset::AssetScope scope) const;
};

#endif // !defined(AFX_RWMAYAASSETMGR_H__4A490F96_3EA3_43FF_B5DC_D0543CC6C1D2__INCLUDED_)
