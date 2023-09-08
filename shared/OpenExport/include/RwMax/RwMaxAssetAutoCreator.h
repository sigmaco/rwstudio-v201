
#ifndef RWMAXASSETAUTOCREATOR_H
#define RWMAXASSETAUTOCREATOR_H

// =============================================================================
// -----------------------  RwMaxAssetAutoCreator CLASS  -----------------------
// =============================================================================
/**
*  \ingroup maxscenegraph  
*
*   This is the RwMax implementation of the RwCommAssetAutoCreator. It will
*   analyze the scene and create an asset for each logical entity in the scene.
*   Users can override the main AutoCreateAssets() for changing the entire
*   auto creation process, or override the AutoCreateAsset() method for tweaking
*   the creation process of a single RwCommAsset. In addition, a set of overridable
*   helper methods are provided for analyzing each INode.
*   Users deriving their own auto creation class can register it by using the
*   RwCommAssetAutoCreator::RegisterInstance() method.
*
*   \see RwCommAssetAutoCreator
*   \see RwCommAsset
*/
class RWMAX_API RwMaxAssetAutoCreator : public RwCommAssetAutoCreator  
{
protected:
    struct  MaxSkinPair;
    struct  AddToVectorIfStaticGeometry;
    struct  AddToVectorIfSpline;
    struct  AddSkinNodeToVector;

public:
	RwMaxAssetAutoCreator();
	virtual ~RwMaxAssetAutoCreator();

    virtual RwCommError AutoCreateAssets(bool bSelectionOnly = false);
    virtual RwCommAsset *AutoCreateAsset();

protected:

    // Overridables

    /** Returns true if the object is animated */
    virtual bool    NodeIsAnimated(INode *node);

    /** Returns true if the hierarchy is animated */
    virtual bool    HierarchyIsAnimated(INode *root);

    /** Returns true if the hierarchy should be created as an animated hierarchy. Calls NodeIsAnimated()
        for every INode in the hierarchy.*/
    virtual bool    HierarchyIsDff(INode *root);

    /** Returns true if the object has a morph modifier */
    virtual bool    HasMaxMorphModifier(INode *node);

    /** Returns true if the object is a camera */
    virtual bool    IsNodeCamera(INode *node);

    /** Returns true if the object is a light */
    virtual bool    IsNodeLight(INode *node);

    void    FindSkinNodes(RwVector<MaxSkinPair> &skins);
    void    UpdateTimeSlider();
    void    GetHierarchyTypes(INode *selectedNode, RwVector<INode *> &bspNodes,
                                                  RwVector<int> &vTypes);

protected:
    RwVector<MaxSkinPair>   m_skinsInScene;
    bool                        m_bInitialised;
    bool                        m_bSelectionOnly;
    
    // this is a cache value used
    // to prevent flicker when auto creating assets
    // AutoCreateAsset looks into this cache value first
    // to find a node to create an asset for. 
    // The Value of m_SelectedNode is destoryed after a call to AutoCreateAsset
    INode               *m_SelectedNode; 

};

#endif // RWMAXASSETAUTOCREATOR_H
