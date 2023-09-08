#if !defined(RW_MAYA_ANIM_CURVE_H)
#define RW_MAYA_ANIM_CURVE_H

/**
*  \ingroup mayascenegraph 
*
*  This is an RwCommAnimation implementation class for the Maya animCurve dependency node.
*
*   \see RwCommAnimation
*/
class RWMAYA_API RwMayaAnimCurve : public RwCommAnimation 
{
public:

    enum AnimCurveType
    {
        translationAnimCurve = 0,
        rotationAnimCurve,
        scaleAnimCurve,
        unknownAnimCurve
    };

	RwMayaAnimCurve(MObject object);
	virtual ~RwMayaAnimCurve();

    virtual RwCommError GetKeyTimes(RwCommReal iStartFrame, RwCommReal iEndFrame, RwVector<RwCommReal> &sKeyFrames) const;

    MObject             GetMObject() { return m_object; }

protected:

    void                CorrectLargeRotations(RwSet<RwCommReal> &keyFrames) const;
    void                BakeCycles(RwSet<RwCommReal> &keyFrames, RwCommReal iStartFrame, RwCommReal iEndFrame) const;
    float               GetKeyframeRange(RwSet<RwCommReal> &keyFrames) const;
    void                CopyKeyFrames(RwSet<RwCommReal> &src, RwSet<RwCommReal> &dst, RwCommReal offset, bool reverse) const;
    AnimCurveType       GetAnimCurveType(MObject animCurve) const;

    MObject             m_object;
    MFnAnimCurve        m_fnAnimCurve;
};

/**
*  \ingroup mayabuilder 
*
*  This class is responsible for creating an RwMayaAnimCurve object.
*
*   \see RwMayaAnimCurve
*   \see RwMayaObjCreator
*/
class RWMAYA_API RwMayaAnimCurveCreator : public RwMayaObjCreator
{
public:
	RwMayaAnimCurveCreator();
	virtual ~RwMayaAnimCurveCreator();

	virtual RwCommObject *Create(const RwCommCreationData *pCreationData,
        const RwCommExportOptions *pExportOptions) const;

	virtual void Init(  RwCommObject *pObject,
                        const RwCommCreationData *pCreationData,
                        const RwCommExportOptions *pExportOptions) const;

	virtual void Attach(RwCommObject* pObject, RwCommObject* pParent, RwCommScene* pScene,
                        const RwCommCreationData* pCreationData) const;

    virtual void Traverse(RwCommBuilder & rBuilder,
                            RwCommCreationData *pCreationData,
                            RwCommObject *pParent) const;
};


#endif // !defined(RW_MAYA_ANIM_CURVE_H)
