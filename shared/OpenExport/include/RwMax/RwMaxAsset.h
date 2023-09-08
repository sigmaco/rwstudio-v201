
#ifndef RWMAXASSET_H
#define RWMAXASSET_H

#include "resource.h"

#include "RwCommAsset.h"
#include "RwCommObjCreator.h"

#include "RwMaxScene.h"

#define RwMaxAsset_CLASS_ID     Class_ID(0x3b713d2c, 0x35484e3f)
#define RwMaxAsset_PBLOCK_REF   0

typedef RwVector<RefTargetHandle> REFERENCELIST;

extern HINSTANCE hGInstance;

RWMAX_API ClassDesc2    *GetRwMaxAssetDesc();

// =============================================================================
// ----------------------------  RwMaxAsset CLASS  -----------------------------
// =============================================================================

/**
*  \ingroup maxscenegraph  
*
*   This is an RwMax implementation of the RwCommAsset, and it represents an asset
*   An asset is a node in the scene representing a sub set of the scene.
*
*   \see RwCommAsset
*/
class RWMAX_API RwMaxAsset : public HelperObject, public RwCommAsset
{
public:
    enum
    {
        assetnode_params = 1,
    };

    enum
    {
        name                = 1,
        scope               = 2,
        num_refs            = 3,
        export_path         = 4,
        template_name       = 5,
        options             = 6,
        enable              = 7,
        guid                = 8,
        custom_export_file  = 9
    };

    RwMaxAsset();
    ~RwMaxAsset();

    // RwCommAsset methods.
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

    virtual RwCommError SaveSelection();
    virtual RwCommError LoadSelection() const;
    virtual RwCommError LoadSelection(const RwVector<int> &SelectedAssest) const;

    virtual RwCommError AddToSelection();
    virtual RwCommError RemoveFromSelection(int Index);
    virtual RwCommError RemoveFromSelection(const RwVector<int> &Indices);
    virtual RwCommError ClearSelection();

    virtual RwCommError SetParent(RwCommAsset *pParent);
    virtual RwCommAsset *GetParent() const;

    virtual RwCommError AddChild(RwCommAsset *pChild);
    virtual RwCommError RemoveChild(int i);
    virtual int         GetNumChildren() const;
    virtual RwCommAsset *GetChild(int i) const;

    virtual RwCommError AddSelection(const RwCommAsset &other);

	virtual RwCommError SetDescription(const RwWString& description) { return RWCOMM_NOT_IMPLEMENTED; }
	virtual RwCommError GetDescription(RwWString &description) const { return RWCOMM_NOT_IMPLEMENTED; }

    // Loading/Saving
    // IOResult         Load(ILoad *iload);
    // IOResult         Save(ISave *isave);
    IParamBlock2        *pblock; //ref 0

    // From Animatable
    Class_ID            ClassID()                       {return RwMaxAsset_CLASS_ID;}
    SClass_ID           SuperClassID()                  {return HELPER_CLASS_ID;}
    void                GetClassName(TSTR& s)           {s = TSTR("RenderWareAsset");}

    RefTargetHandle     Clone( RemapDir &remap );
    RefResult           NotifyRefChanged(Interval changeInt, RefTargetHandle hTarget, 
                                                PartID& partID,  RefMessage message);

    int                 NumSubs() { return 1; }
    TSTR                SubAnimName(int i)              {return "Parameters";}
    Animatable          *SubAnim(int i)                 {return pblock;}

    // Maintain the number of references.
    int                 NumRefs();
    RefTargetHandle     GetReference(int i);
    void                SetReference(int i, RefTargetHandle rtarg);

    int                 NumParamBlocks()                {return 1;}
    IParamBlock2        *GetParamBlock(int i)           {return pblock;}
    IParamBlock2        *GetParamBlockByID(BlockID id)  {return (pblock->ID() == id) ? pblock : NULL;}

    void                DeleteThis()                    {delete this; }
    ObjectState         Eval(TimeValue t)               {return ObjectState(this);}
    CreateMouseCallBack *GetCreateMouseCallBack();
    
    int                 IsRenderable()                  {return 0;}
    void                InitNodeName(TSTR& s)           {s = TSTR("RenderWareAsset");};

    // Selection list functions for this RwMaxAsset.
    /** \param Node 3ds max INode to add in the asset selection list. */
    void                AddNode(INode *Node);
    /** \param node 3ds max INode to remove from the asset selection list. */
    void                DeleteSelectionNode(INode *node);
    /** Clear the selection on this asset. */
    void                DeleteAllSelectionNodes();
    /** \return The number of nodes in the asset selection list. */
    int                 NumNodes() const;
    /** \return The i-th selected node in the current asset. */
    INode               *GetSelectionNode(int i) const;

    // INode functions
    INode               *GetINode() const;
    void                SetINode(INode *iNode);

private:
    // The mapping between the references and this vector is 121,
    // except when you try to SetReference, GetReference or
    // MakeRefByID the indices are one based, not zero based.
    REFERENCELIST       NodeReferenceList;
    INode               *m_iNode;
	RwString		    m_sDescription;
};

// =============================================================================
// ------------------------  RwMaxAssetClassDesc CLASS  ------------------------
// =============================================================================

class RwMaxAssetClassDesc : public ClassDesc2 {
public:
    int                     IsPublic()              {return FALSE;}
    void                    *Create(BOOL loading)   {return new RwMaxAsset;}

    const TCHAR             *Category()             {return _T("RenderWare");}
    const TCHAR             *ClassName()            {return _T("RenderWareAsset");}
    const TCHAR             *InternalName()         {return _T("RenderWareAsset");}

    Class_ID                ClassID()               {return RwMaxAsset_CLASS_ID;}
    SClass_ID               SuperClassID()          {return HELPER_CLASS_ID;}

    HINSTANCE               HInstance()             {return hGInstance;}
};

// =============================================================================
// -------------------------  RwMaxAssetCreator CLASS  -------------------------
// =============================================================================

class RWMAX_API RwMaxAssetCreator  : public RwCommObjCreator
{
public:
    RwMaxAssetCreator() : RwCommObjCreator()       {}
    virtual ~RwMaxAssetCreator()                   {}

    virtual RwCommObject        *Create(const RwCommCreationData *pCreationData,
                                    const RwCommExportOptions *pExportOptions) const;
    virtual void                Init(RwCommObject *pObject,
                                    const RwCommCreationData *pCreationData,
                                    const RwCommExportOptions *pExportOptions) const;
    virtual void                Attach(RwCommObject *pObject, RwCommObject *pParent,
                                    RwCommScene *pScene, const RwCommCreationData *pCreationData) const;
};

#endif // RWMAXASSET_H
