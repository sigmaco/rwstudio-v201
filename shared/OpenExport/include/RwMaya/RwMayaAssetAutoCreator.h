// RwMayaAssetCreator.h: interface for the RwMayaAssetCreator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWMAYAASSETCREATOR_H__6EB4FC37_F079_48FE_8956_0842AA88DD1B__INCLUDED_)
#define AFX_RWMAYAASSETCREATOR_H__6EB4FC37_F079_48FE_8956_0842AA88DD1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
*  \ingroup mayascenegraph  
*
*   This is the RwMaya implementation of the RwCommAssetAutoCreator. It will
*   analyze the scene and create an asset for each logical entity in the scene.
*   Users can override the main AutoCreateAssets() for changing the entire
*   auto creation process, or override the AutoCreateAsset() method for tweaking
*   the creation process of a single RwCommAsset. In addition, a set of overridable
*   helper methods are provided for analyzing each MDagPath.
*   Users deriving their own auto creation class can register it by using the
*   RwCommAssetAutoCreator::RegisterInstance() method.
*
*   \see RwCommAssetAutoCreator
*   \see RwCommAsset
*/
class RWMAYA_API RwMayaAssetAutoCreator : public RwCommAssetAutoCreator  
{
public:
	RwMayaAssetAutoCreator();
	virtual ~RwMayaAssetAutoCreator();

    virtual RwCommError AutoCreateAssets(bool bSelectionOnly = false);
    virtual RwCommAsset *AutoCreateAsset();

    RwVector<RwMayaSkinCluster> &GetSkinClusters() { return m_SkinClusters; }
    RwVector<MObject>     &GetBlendShapeObjects() { return m_BlendShapeObjects; }
    RwVector<MObject>     &GetGeometryFilterObjects() { return m_GeometryFilterObjects; }

protected:

    // Overridables
    /** Returns true if the object is animated */
    virtual bool        IsAnimated(MDagPath dagPath);

    /** Returns true if the object is a blend shape */
    virtual bool        IsBlendShaped(MDagPath dagPath);

    /** Returns true if the object is a camera or a light */
    virtual bool        IsLightOrCamera(MDagPath dagPath);

    /** Returns true if the object is a patricle */
    virtual bool        IsParticle(MDagPath dagPath);

    /** Returns true if the object has morphing */
    virtual bool        IsMorphed(MDagPath dagPath);

    /** Returns true if the object is a skeleton */
    virtual bool        IsSkeleton(MDagPath dagPath);

    /** Returns true if the object is skinned */
    virtual bool        IsSkinned(MDagPath dagPath);


    void                GetHierarchyTypes(MDagPath dagPath, RwVector<int> &vTypes);
    static bool         GetHierarchyFlagsCallBack(MDagPath dagPath, void *pData);
    static bool         SelectBspNodeCallBack(MDagPath dagPath, void *pData);

private:
    void                CreateCache();
    void                ClearCache();

    // This is a cache value used to prevent flicker when auto creating assets.
    // AutoCreateAsset checks this cache value first to find a node to create an asset from. 
    MDagPath                        *m_pSelectedDag;

    int                             m_cacheRefCount;
    RwVector<RwMayaSkinCluster> m_SkinClusters;
    RwVector<MObject>           m_BlendShapeObjects;
    RwVector<MObject>           m_GeometryFilterObjects;
};


#endif // !defined(AFX_RWMAYAASSETCREATOR_H__6EB4FC37_F079_48FE_8956_0842AA88DD1B__INCLUDED_)
