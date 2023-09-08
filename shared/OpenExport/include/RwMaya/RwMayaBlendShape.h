#if !defined(RW_MAYA_BLEND_SHAPE_H)
#define RW_MAYA_BLEND_SHAPE_H

/**
*  \ingroup mayascenegraph 
*
*  This is the RwMaya implementation of RwCommBlendShape.
*
*   \see RwCommCamera
*/
class RWMAYA_API RwMayaBlendShape  : public RwCommBlendShape
{
public:
	RwMayaBlendShape(MObject object);
	virtual ~RwMayaBlendShape();

    virtual RwCommError OnSetFrame(RwCommReal frame);

	virtual RwCommError GetNumTargets(int &nTargets) const;
	virtual RwCommError SetTargetWeight(int iTarget, RwCommReal weight);
	virtual RwCommError GetTargetWeight(int iTarget, RwCommReal &weight) const;
	virtual RwCommError GetTargetName(int iTarget, RwWString &name) const;

protected:
    MObject                 m_object;
    MFnBlendShapeDeformer   m_deformer;
};


/**
*  \ingroup mayabuilder 
*
*  This class is responsible for creating an RwMayaBlendShape object.
*
*   \see RwMayaBlendShape
*   \see RwMayaObjCreator
*/
class RWMAYA_API RwMayaBlendShapeCreator : public RwMayaObjCreator
{
public:
	RwMayaBlendShapeCreator();
	virtual ~RwMayaBlendShapeCreator();

	virtual RwCommObject *Create(const RwCommCreationData *pCreationData,
								const RwCommExportOptions *pExportOptions) const;

	virtual void Init(	RwCommObject *pObject,
						const RwCommCreationData *pCreationData,
						const RwCommExportOptions *pExportOptions) const;

	virtual void Attach(RwCommObject* pObject, RwCommObject* pParent, RwCommScene* pScene,
                        const RwCommCreationData* pCreationData) const;

    virtual void Traverse(RwCommBuilder &rBuilder,
                            RwCommCreationData *pCreationData,
                            RwCommObject *pParent) const;
protected: 
    RwMayaTraverseBlendShapeAnimations m_AnimationTraverse;
};

#endif // !defined(RW_MAYA_BLEND_SHAPE_H)
