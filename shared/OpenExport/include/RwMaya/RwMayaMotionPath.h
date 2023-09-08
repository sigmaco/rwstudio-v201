#if !defined(RW_MAYA_MOTION_PATH_H)
#define RW_MAYA_MOTION_PATH_H

/**
*  \ingroup maxcontrols 
*
*  This is an RwCommAnimation implementation class for the Maya motionPath dependency node.
*
*   \see RwCommAnimation
*/
class RWMAYA_API RwMayaMotionPath : public RwCommAnimation 
{
public:

    RwMayaMotionPath(MObject object);
	virtual ~RwMayaMotionPath();

    virtual RwCommError GetKeyTimes(RwCommReal iStartFrame, RwCommReal iEndFrame, RwVector<RwCommReal> &sKeyFrames) const;
    
    MObject             GetMObject() { return m_object; }

protected:

    MObject             m_object;
    MFnMotionPath       m_MotionPath;

};

/**
*  \ingroup mayabuilder 
*
*  This class is responsible for creating an RwMayaMotionPath object.
*
*   \see RwMayaMotionPath
*   \see RwMayaObjCreator
*/
class RWMAYA_API RwMayaMotionPathCreator : public RwMayaObjCreator
{
public:
	RwMayaMotionPathCreator();
	virtual ~RwMayaMotionPathCreator();

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


#endif // !defined(RW_MAYA_MOTION_PATH_H)
