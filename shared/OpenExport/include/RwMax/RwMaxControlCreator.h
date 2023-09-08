// RwMaxControlCreator.h: interface for the RwMaxControlCreator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWMAXCONTROLCREATOR_H__A9425AF3_22C9_4A85_B4CD_CA5632A25069__INCLUDED_)
#define AFX_RWMAXCONTROLCREATOR_H__A9425AF3_22C9_4A85_B4CD_CA5632A25069__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma warning(disable : 4786)
#pragma warning(disable : 4251)


#include "RwMax.h"
#include "RwCommObjCreator.h"
#include "RwMaxTraverseControllers.h"
#include "RwMaxCreationData.h"

/**
*  \ingroup maxbuilder 
*
*   This class is responsible for creating an RwMaxDefaultControl object from a standard Max TMController object.
*   The class uses the default implementation of the RwMaxTraverseControllers::TraverseControllers() method.
*   Only non-empty controllers are built (controllers containing at least one key frame).
*
*   \see RwMaxDefaultControl
*   \see RwMaxTraverseControllers
*/
class RWMAX_API RwMaxDefaultControlCreator : public RwCommObjCreator , public RwMaxTraverseControllers
{
public:
    RwMaxDefaultControlCreator();
    virtual ~RwMaxDefaultControlCreator();

    virtual RwCommObject *Create(const RwCommCreationData *pCreationData,
                                 const RwCommExportOptions *pExportOptions) const;

    virtual void Init(  RwCommObject *pObject,
                        const RwCommCreationData *pCreationData,
                        const RwCommExportOptions *pExportOptions) const;

	virtual void Attach(RwCommObject *pObject, RwCommObject *pParent, RwCommScene *pScene,
                        const RwCommCreationData *pCreationData) const;

protected:

    bool    IsControlEmpty(Control *pControl) const;
};

/**
*  \ingroup maxbuilder 
*
*   This class is responsible for creating an RwMaxRapidSampleControl object from a standard Max TMController object.
*   The class uses the default implementation of the RwMaxTraverseControllers::TraverseControllers() method.
*   Only non-empty controllers are built (controllers containing at least one key frame).
*
*   \see RwMaxRapidSampleControl
*   \see RwMaxTraverseControllers
*/
class RWMAX_API RwMaxRapidSampleControlCreator : public RwMaxDefaultControlCreator 
{
public:
    RwMaxRapidSampleControlCreator() : RwMaxDefaultControlCreator() {}
    virtual ~RwMaxRapidSampleControlCreator() {}

    virtual RwCommObject *Create(const RwCommCreationData *pCreationData,
                                const RwCommExportOptions *pExportOptions) const;

};

/**
*  \ingroup maxbuilder 
*
*   This class is responsible for creating an RwMaxSmoothCurveControl object from a standard Max TMController object.
*   The class uses the default implementation of the RwMaxTraverseControllers::TraverseControllers() method.
*   Only non-empty controllers are built (controllers containing at least one key frame).
*
*   \see RwMaxSmoothCurveControl
*   \see RwMaxTraverseControllers
*/
class RWMAX_API RwMaxSmoothCurveControlCreator : public RwMaxDefaultControlCreator 
{
public:
    RwMaxSmoothCurveControlCreator() : RwMaxDefaultControlCreator() {}
    virtual ~RwMaxSmoothCurveControlCreator() {}

    virtual RwCommObject *Create(const RwCommCreationData *pCreationData,
                                 const RwCommExportOptions *pExportOptions) const;

};

/**
*  \ingroup maxbuilder 
*
*   This is a generic class, responsible for creating RwCommAnimation objects from Max PRS controllers (Position/Rotation/Scale).
*   The top level controller is not built (RwMaxPRSControlCreator::Create() returns NULL), but the PRS
*   children are each built recursively using the RwMaxBuilder::BuildObject() method. This class overrides
*   the default implementation of the RwMaxTraverseControllers::TraverseControllers() method, and implements
*   its PRS children traversal functionality instead.
*   Only non-empty controllers are built (controllers containing at least one key frame).
*
*   \see RwMaxTraverseControllers
*/
class RWMAX_API RwMaxPRSControlCreator : public RwMaxDefaultControlCreator
{
public:
    RwMaxPRSControlCreator() : RwMaxDefaultControlCreator() {}
    virtual ~RwMaxPRSControlCreator() {}

    virtual RwCommObject *Create(const RwCommCreationData *pCreationData,
                                 const RwCommExportOptions *pExportOptions) const { return NULL; }

    virtual void TraverseControllers(RwMaxBuilder &rBuilder, RwCommCreationData *pCreationData, RwCommObject *pParent);
};

/**
*  \ingroup maxbuilder 
*
*   This class is identical to the RwMaxPRSControlCreator class, only that in its TraverseControllers() method
*   it also tries to build the target controller, using the INode::GetTarget() method.
*   
*
*   \see RwMaxPRSControlCreator
*/
class RWMAX_API RwMaxLookAtControlCreator : public RwMaxPRSControlCreator
{
public:
    RwMaxLookAtControlCreator() : RwMaxPRSControlCreator() {}
    virtual ~RwMaxLookAtControlCreator() {}

    virtual void TraverseControllers(RwMaxBuilder &rBuilder, RwCommCreationData *pCreationData, RwCommObject *pParent);
};

class RWMAX_API RwMaxBipedControlCreator : public RwMaxDefaultControlCreator
{
public:
    RwMaxBipedControlCreator() : RwMaxDefaultControlCreator() {}
    virtual ~RwMaxBipedControlCreator() {}

    virtual RwCommObject *Create(const RwCommCreationData *pCreationData,
                                 const RwCommExportOptions *pExportOptions) const { return NULL; }
};

class RWMAX_API RwMaxBlockControlCreator : public RwMaxDefaultControlCreator
{
public:
    RwMaxBlockControlCreator() : RwMaxDefaultControlCreator() {}
    virtual ~RwMaxBlockControlCreator() {}

    virtual RwCommObject *Create(const RwCommCreationData *pCreationData,
                                const RwCommExportOptions *pExportOptions) const { return NULL; }
};

class RWMAX_API RwMaxEulerControlCreator : public RwMaxDefaultControlCreator
{
public:
    RwMaxEulerControlCreator() : RwMaxDefaultControlCreator()
    {
        RegisterRotationEntries();
    }
    virtual ~RwMaxEulerControlCreator() {}

    virtual RwCommObject *Create(const RwCommCreationData *pCreationData,
                                 const RwCommExportOptions *pExportOptions) const { return NULL; }
};

/**
*  \ingroup maxbuilder 
*
*   This class is responsible for creating an RwMaxDefaultControl object from a wire controller object.
*   The class overrides the TraverseControllers() method, looks for all the slave controllers hooked up
*   to the master wire, and attempts to build them as well. This is done so that master and slave controllers
*   will share the same key frames on their animated objects.
*
*   \see RwMaxDefaultControl
*   \see RwMaxTraverseControllers
*/
class RWMAX_API RwMaxWireControlCreator : public RwMaxDefaultControlCreator
{
public:
    RwMaxWireControlCreator() : RwMaxDefaultControlCreator() {}
    virtual ~RwMaxWireControlCreator() {}

    virtual RwCommObject *Create(const RwCommCreationData *pCreationData,
                                 const RwCommExportOptions *pExportOptions) const;

    virtual void TraverseControllers(RwMaxBuilder &rBuilder, RwCommCreationData *pCreationData, RwCommObject *pParent);
};

/**
*  \ingroup maxbuilder 
*
*   This class is responsible for creating an RwMaxLinearRotationControl object from a standard Max TMController object.
*   The class uses the default implementation of the RwMaxTraverseControllers::TraverseControllers() method.
*   Only non-empty controllers are built (controllers containing at least one key frame).
*
*   \see RwMaxLinearRotationControl
*   \see RwMaxTraverseControllers
*/
class RWMAX_API RwMaxLinearRotationControlCreator : public RwMaxDefaultControlCreator 
{
public:
    RwMaxLinearRotationControlCreator() : RwMaxDefaultControlCreator()
    {
        RegisterRotationEntries();
    }
    virtual ~RwMaxLinearRotationControlCreator() {}

    virtual RwCommObject *Create(const RwCommCreationData *pCreationData,
                                 const RwCommExportOptions *pExportOptions) const;

};

/**
*  \ingroup maxbuilder 
*
*   This class is responsible for creating an RwMaxHybridRotationControl object from a standard Max TMController object.
*   The class uses the default implementation of the RwMaxTraverseControllers::TraverseControllers() method.
*   Only non-empty controllers are built (controllers containing at least one key frame).
*
*   \see RwMaxHybridRotationControl
*   \see RwMaxTraverseControllers
*/
class RWMAX_API RwMaxHybridRotationControlCreator : public RwMaxDefaultControlCreator 
{
public:
    RwMaxHybridRotationControlCreator() : RwMaxDefaultControlCreator()
    {
        RegisterRotationEntries();
    }
    virtual ~RwMaxHybridRotationControlCreator() {}

    virtual RwCommObject *Create(const RwCommCreationData *pCreationData,
                                 const RwCommExportOptions *pExportOptions) const;

};

/**
*  \ingroup maxbuilder 
*
*   This class is responsible for creating an RwMaxTCBRotationControl object from a standard Max TMController object.
*   The class uses the default implementation of the RwMaxTraverseControllers::TraverseControllers() method.
*   Only non-empty controllers are built (controllers containing at least one key frame).
*
*   \see RwMaxTCBRotationControl
*   \see RwMaxTraverseControllers
*/
class RWMAX_API RwMaxTCBRotationControlCreator : public RwMaxDefaultControlCreator 
{
public:
    RwMaxTCBRotationControlCreator() : RwMaxDefaultControlCreator()
    {
        RegisterRotationEntries();
    }
    virtual ~RwMaxTCBRotationControlCreator() {}

    virtual RwCommObject *Create(const RwCommCreationData *pCreationData,
                                 const RwCommExportOptions *pExportOptions) const;

};

/**
*  \ingroup maxbuilder 
*
*   This class is used for building RwMaxIKSlaveControl objects from IK slave controllers. It traverses
*   its attached PRS children using the RwMaxPRSControlCreator::TraverseControllers() method.
*   
*   \see RwMaxIKSlaveControl
*   \see RwMaxPRSControlCreator
*/
class RWMAX_API RwMaxIKSlaveControlCreator : public RwMaxPRSControlCreator
{
public:
    RwMaxIKSlaveControlCreator(): RwMaxPRSControlCreator() {}
    virtual ~RwMaxIKSlaveControlCreator() {}

    virtual RwCommObject *Create(const RwCommCreationData *pCreationData,
                                 const RwCommExportOptions *pExportOptions) const;

};

/**
*  \ingroup maxbuilder 
*
*   This class is used for building RwMaxIKControl objects from IIKChainControl objects. It traverses
*   its attached PRS children using the RwMaxPRSControlCreator::TraverseControllers() method.
*   
*   \see RwMaxIKControl
*   \see RwMaxPRSControlCreator
*/
class RWMAX_API RwMaxIKControlCreator : public RwMaxPRSControlCreator
{
public:
    RwMaxIKControlCreator(): RwMaxPRSControlCreator() {}
    virtual ~RwMaxIKControlCreator() {}

    virtual RwCommObject *Create(const RwCommCreationData *pCreationData,
                                 const RwCommExportOptions *pExportOptions) const;

};

#endif // !defined(AFX_RWMAXCONTROLCREATOR_H__A9425AF3_22C9_4A85_B4CD_CA5632A25069__INCLUDED_)
