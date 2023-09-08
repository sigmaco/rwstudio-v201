// RwCommAssetMgr.h: interface for the RwCommAssetMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWCOMMASSETMGR_H__E7D3212B_DA21_4E2C_AE09_9D4E975A2F1E__INCLUDED_)
#define AFX_RWCOMMASSETMGR_H__E7D3212B_DA21_4E2C_AE09_9D4E975A2F1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwCommAsset.h"

/**
*  \ingroup commhelpers
*
*   This is a singleton class responsible for managing the assets. It is responsible for creating new assets,
*   deleting assets and querying all assets in the secene. It also manages the project's name, path and template
*   name.
*   
*   Applications can register their own custom asset manager, using the RegisterInstance() method, supplying
*   it with an instance of the custom derived asset manager.
*
*   \see RwCommAsset
*/
class RWCOMM_API RwCommAssetMgr  
{
public:
    virtual ~RwCommAssetMgr();
    
    /**
    *   \param pInstance of the derived class to register.
    *          RwCommAssetMgr is a singleton.
    */
    static void             RegisterInstance(RwCommAssetMgr *pInstance);
    /**
    *   \return static instance of the class. RwCommAssetMgr is a singleton.
    */
    static RwCommAssetMgr  *TheInstance();

    /**
     *  \param vAssets returns all assets in the scene.
     *
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError     GetAssets(RwVector<RwCommAsset *> &vAssets) const = 0;
    /**
     *  \param vTemplates returns all asset templates in the scene.
     *
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError     ListAssetTemplates(RwVector<RwWString> &vTemplates) const;
    /**
     *  \param pAsset returns a new asset.
     *
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError     CreateAsset(RwCommAsset *&pAsset) const = 0;
    /**
     *  \param pAsset specifies an asset to delete.
     *
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError     DeleteAsset(RwCommAsset *pAsset) const = 0;

    /**
     *  \param sProjectName returns a project name for the scene.
     *
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError     GetProjectName(RwWString &sProjectName) const;
    /**
     *  \param sProjectPath returns a project path for the scene.
     *
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError     GetProjectPath(RwWString &sProjectPath) const;
    /**
     *  \param sProjectTemplate returns a project template for the scene
     *
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError     GetProjectTemplate(RwWString &sProjectTemplate) const;

    /**
     *  \param sProjectName a project name for the scene to set.
     *
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError     SetProjectName(const RwWString &sProjectName) const;
    /**
     *  \param sProjectPath a project path for the scene to set.
     *
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError     SetProjectPath(const RwWString &sProjectPath) const;
    /**
     *  \param sProjectTemplate a project template for the scene to set.
     *
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError     SetProjectTemplate(const RwWString &sProjectTemplate) const;

    /**
     *  It will merge all asset selections into the first asset and then
     *  delete all other assets.
     *  
     *  \param vAssets a vector of assets to merge.
     *
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError     MergeAssets(RwVector<RwCommAsset *> &vAssets) const;

    /**
     *  \return a project asset for the scene. There should be only one
     *          project asset.
     *
     *  \see RwCommAsset::AssetScope
     */
    virtual RwCommAsset    *GetProjectAsset() const = 0;

    /**
     *  It will delete all the current assets and then call
     *  RwCommAssetAutoCreator::AutoCreateAssets()
     *  
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    RwCommError             AutoRecreateAssets() const;

    /**
     *  Assets can be customized by an user or auto-managed by the
     *  application.
     *  
     *  \param bCustomizedAssets returns true if assets are customized,
     *         false otherwise.
     *
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    RwCommError             GetCustomizedAssets(bool &bCustomizedAssets) const;
    /**
     *  Assets can be customized by an user or auto-managed by the
     *  application.
     *  
     *  \param bCustomizedAssets specifies if assets should be customized
     *         or not.
     *
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    RwCommError             SetCustomizedAssets(const bool bCustomizedAssets) const;

    /**
    *   Creates a partition visualization object for the scene.
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError     CreatePartitionObject() { return RWCOMM_NOT_IMPLEMENTED; }

    /**
    *   Removes a partition visualization object from the scene.
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError     RemovePartitionObject() { return RWCOMM_NOT_IMPLEMENTED; }


protected:
    RwCommAssetMgr();

private:
    static RwCommAssetMgr *m_pTheInstance;
};

#endif // !defined(AFX_RWCOMMASSETMGR_H__E7D3212B_DA21_4E2C_AE09_9D4E975A2F1E__INCLUDED_)
