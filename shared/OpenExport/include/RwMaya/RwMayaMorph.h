#if !defined(RW_MAYA_MORPH_H)
#define RW_MAYA_MORPH_H


/**
*  \ingroup mayabuilder 
*
*  This class is responsible for creating an RwCommMorph object from generic (non-blend shape)
*  geometry animation.
*
*   \see RwCommMorph
*   \see RwMayaObjCreator
*/

class RWMAYA_API RwMayaMorphCreator : public RwMayaObjCreator
{
public:
	RwMayaMorphCreator();
	virtual ~RwMayaMorphCreator();

	virtual RwCommObject *Create(   const RwCommCreationData* pCreationData,
								    const RwCommExportOptions* pExportOptions) const;

	virtual void Init(	RwCommObject* pObject,
						const RwCommCreationData* pCreationData,
						const RwCommExportOptions* pExportOptions) const;

	virtual void Attach(RwCommObject* pObject, RwCommObject* pParent, RwCommScene* pScene,
                        const RwCommCreationData* pCreationData) const;

    virtual void Traverse(RwCommBuilder & rBuilder,
                            RwCommCreationData *pCreationData,
                            RwCommObject *pParent) const;
protected:
    RwMayaTraverseAnimations m_AnimationTraverse;
};

#endif // !defined(RW_MAYA_MORPH_H)
