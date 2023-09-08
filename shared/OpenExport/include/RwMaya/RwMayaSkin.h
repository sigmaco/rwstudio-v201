#if !defined(RW_MAYA_SKIN_H)
#define RW_MAYA_SKIN_H

const RwCommType RwMayaRigidSkinType("RwMayaRigidSkin");

// =============================================================================
// ----------------------------  RwMayaSkin CLASS  -----------------------------
// =============================================================================

/**
*  \ingroup mayascenegraph 
*
*  This is the RwMaya implementation of RwCommSkin which handles the smooth skin
*  deformer (skinCluster dependency node).
*
*   \see RwCommSkin
*/

class RWMAYA_API RwMayaSkin : public RwCommSkin
{
public:
    RwMayaSkin(MObject object);
    virtual ~RwMayaSkin();

    virtual RwCommError             OnSetFrame(RwCommReal frame);

    virtual RwCommError             GetSkinWeights(const RwCommVertexId &id, RwVector<RwCommSkinWeight> &vWeights) const;
    virtual RwCommError             GetInfluenceBones(RwVector<RwCommTransform*>& vBones) const;

protected:
    bool                            StoreSkinCache();

    MObject                         m_object;
    MFnSkinCluster                  m_deformer;
    MDagPath                        m_deformedDagPath;
    bool                            m_bHaveCache;
    RwVector<MObject>           m_vertexComponents;
    RwVector<RwCommTransform *> m_vBoneCommTransforms;

};

// =============================================================================
// -------------------------  RwMayaSkinCreator CLASS  -------------------------
// =============================================================================

/**
*  \ingroup mayabuilder 
*
*  This class is responsible for creating an RwMayaSkin object.
*
*   \see RwMayaSkin
*   \see RwMayaObjCreator
*/

class RWMAYA_API RwMayaSkinCreator : public RwMayaObjCreator
{
public:
    RwMayaSkinCreator();
    virtual ~RwMayaSkinCreator();

    virtual RwCommObject *Create(const RwCommCreationData *pCreationData,
                                const RwCommExportOptions *pExportOptions) const;

    virtual void Init(  RwCommObject *pObject,
                        const RwCommCreationData *pCreationData,
                        const RwCommExportOptions *pExportOptions) const;

	virtual void Attach(RwCommObject* pObject, RwCommObject* pParent, RwCommScene* pScene,
                        const RwCommCreationData* pCreationData) const;

    virtual void Traverse(RwCommBuilder &rBuilder,
                            RwCommCreationData *pCreationData,
                            RwCommObject *pParent) const;
};

// =============================================================================
// --------------------------  RwMayaRigidSkin CLASS  --------------------------
// =============================================================================

/**
*  \ingroup mayascenegraph 
*
*  This is the RwMaya implementation of RwCommSkin which handles the rigid skin
*  deformer (jointCluster dependency node). Rigid skinning is not supported and
*  this class is used merely to detect rigid skinning in the scene and give a
*  warning.
*
*   \see RwCommSkin
*/

class RWMAYA_API RwMayaRigidSkin : public RwCommSkin
{
public:
    RwMayaRigidSkin(MObject object) : m_object(object) {};
    virtual ~RwMayaRigidSkin() {};

    virtual RwCommType	        GetType() const { return RwMayaRigidSkinType; }

    /**
     *  This function is not implemented under RwMaya.
     *  \return RWCOMM_NOT_IMPLEMENTED.
     */
    virtual RwCommError         GetSkinWeights(const RwCommVertexId &id,
                                    RwVector<RwCommSkinWeight> &vWeights) const;
    virtual RwCommError         GetInfluenceBones(RwVector<RwCommTransform*>& vBones) const;

protected:
    MObject                     m_object;
};

// =============================================================================
// ----------------------  RwMayaRigidSkinCreator CLASS  -----------------------
// =============================================================================

/**
*  \ingroup mayabuilder 
*
*  This class is responsible for creating an RwMayaRigidSkin object.
*
*   \see RwMayaRigidSkin
*   \see RwMayaObjCreator
*/

class RWMAYA_API RwMayaRigidSkinCreator : public RwMayaObjCreator
{
public:
    RwMayaRigidSkinCreator();
    virtual ~RwMayaRigidSkinCreator();

    virtual RwCommObject    *Create(const RwCommCreationData *pCreationData,
                                    const RwCommExportOptions *pExportOptions) const;

    virtual void            Init(RwCommObject *pObject, const RwCommCreationData *pCreationData,
                                               const RwCommExportOptions *pExportOptions) const;

	virtual void            Attach(RwCommObject* pObject, RwCommObject* pParent, RwCommScene* pScene,
                                    const RwCommCreationData* pCreationData) const;

    virtual void            Traverse(RwCommBuilder &rBuilder, RwCommCreationData *pCreationData,
                                                                   RwCommObject *pParent) const;
};

#endif // !defined(RW_MAYA_SKIN_H)
