
#ifndef RWMAXASSETMGR_H
#define RWMAXASSETMGR_H

struct RwMaxAddAssetNodeToVector
{
    RwMaxAddAssetNodeToVector(RwVector<INode *> &vec, RwCommAsset::AssetScope scope) : m_vec(vec), m_scope(scope) {}

    void operator ()(INode *iNode)
    {
        RwMaxAsset   *assetNode = (RwMaxAsset *) iNode->GetObjectRef();
        
        if (assetNode && assetNode->ClassID() == RwMaxAsset_CLASS_ID)
        {
            if (assetNode->pblock->GetInt(RwMaxAsset::scope, 0) == m_scope)
            {
                m_vec.push_back(iNode);
            }
        }
    }

    RwCommAsset::AssetScope m_scope;
    RwVector<INode *>   &m_vec;
};

// =============================================================================
// ---------------------------  RwMaxAssetMgr CLASS  ---------------------------
// =============================================================================

/**
*  \ingroup maxscenegraph 
*
*  This is an RwMax implementation of the RwCommAssetMgr singleton. It operates
*  on RwMaxAsset objects in the scene.
*
*   \see RwCommAssetMgr
*/
class RWMAX_API RwMaxAssetMgr : public RwCommAssetMgr  
{
public:
	RwMaxAssetMgr();
	virtual ~RwMaxAssetMgr();
    
    virtual RwCommError GetAssets(RwVector<RwCommAsset *> &vAssets) const;
    virtual RwCommError CreateAsset(RwCommAsset *&pAsset) const;
    virtual RwCommError DeleteAsset(RwCommAsset *pAsset) const;

    virtual RwCommError CreatePartitionObject( );
    virtual RwCommError RemovePartitionObject( );

    virtual RwCommAsset *GetProjectAsset() const;
};

#endif // RWMAXASSETMGR_H
