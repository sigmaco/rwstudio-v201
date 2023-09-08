// RwExpTraverseAction.h: interface for the RwExpTraverseAction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPTRAVERSEACTION_H__914A0CE0_9234_4BB6_BE63_5CDCF73897F5__INCLUDED_)
#define AFX_RWEXPTRAVERSEACTION_H__914A0CE0_9234_4BB6_BE63_5CDCF73897F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwExp.h"
#include "RwCommError.h"
#include "RwCommObject.h"

class RwCommObject;
class RwCommExportOptions;
class RwExpExporter;

/**
*
*  \ingroup traverseactions
*
*   The traverse list contains a collection of RwExpTraverseObject objects. Each RwExpTraverseObject is a pair,
*   consisting of an RwCommObject and its traverse list parent. (Also an RwCommObject). The object is the one that
*   is eventually exported. The parent doesn't have to necessarily be the scene graph parent. It is the parent that
*   the exported object will be attached to.
*/

class RWEXP_API RwExpTraverseObject
{
public:
    RwExpTraverseObject() : m_pObject(NULL), m_pParent(NULL) {}

    /** Constructs a new RwExpTraverseObject from a given object and a parent.*/
    RwExpTraverseObject(RwCommObject *pObject, RwCommObject *pParent) : m_pObject(pObject), m_pParent(pParent) {}

    /** Get the traverse list object.*/
    RwCommObject    *GetCommObject() { return m_pObject; }
    void            SetObject(RwCommObject *pObject) { m_pObject = pObject; }

    /** Get the parent of a traverse list object.*/
    RwCommObject    *GetParent() { return m_pParent; }
    void            SetParent(RwCommObject *pParent) { m_pParent = pParent; }

private:

    RwCommObject *m_pObject;
    RwCommObject *m_pParent;
};

/**
*
*  \ingroup traverseactions
*
*   This is the base class for all traverse actions. A traverse action can apply any action on a traverse list.
*   This can range from any sort of internal action, to modifying the traverse list itself.
*   Actions can add or remove new RwCommObjects to or from the list.
*   Actions can behave as filters, removing unnecessary objects from the list. 
*/

class RWEXP_API RwExpTraverseAction  : public RwCommObject
{
public:
    RwExpTraverseAction();
    virtual ~RwExpTraverseAction();

    /** Initialize the traverse action. Classes deriving from RwExpTraverseAction implement
    *   their action initialization here. This method is called once before each export proccess, for each
    *   traverse action.
    *
    *   \param pExportOptions a pointer to an export options object.
    *
    *   \return true if the action is successfully initialized. Only actions that return true are applied in the
    *   export proccess.
    */
    virtual bool Init(const RwCommExportOptions* pExportOptions) { return true; }

    /** Apply the action. Classes deriving from RwExpTraverseAction implement their main traverse action code here.
    *
    *   \param vTraverseList a vector of RwExpTraverseObject objects. (The traverse list).
    *   \param pExportOptions a pointer to an export options object.
    *
    *   \return RWCOMM_SUCCESS if the action succeeds, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError Apply(RwVector<RwExpTraverseObject> &vTraverseList,
                                const RwCommExportOptions *pExportOptions) = 0;

    /** Classes deriving from RwExpTraverseAction implement their action clean up here. This method is called
    *   once after each export process, for each traverse action, regardless if the action is enabled or not.
    *
    *   \return RWCOMM_SUCCESS if the action succeeds, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError CleanUp() { return RWCOMM_SUCCESS; }

    /** Enable/Disable the action. Actions that are disabled are not applied in the export proccess.
    *
    *   \param bEnable flag indicating whether to enable or disable the action.
    */
    void Enable(bool bEnable) { m_bEnabled = bEnable; }

    /**
    *  Is the action enabled/disabled
    *
    *  \return a bool indicating whether the action is enabled or not.
    */
    bool IsEnabled() const { return m_bEnabled; }

    void SetExporter(RwExpExporter *pExporter) { m_pExporter = pExporter; }

    /**
    *  Get the RwExpExporter, hosting the traverse action
    *
    *  \return the RwExpExporter
    */
    RwExpExporter *GetExporter() { return m_pExporter; }

    bool DeleteTransform(RwVector<RwExpTraverseObject> &vTraverseList, RwExpTraverseObject object);


protected:

    bool    m_bEnabled;
    RwExpExporter   *m_pExporter;
};

#endif // !defined(AFX_RWEXPTRAVERSEACTION_H__914A0CE0_9234_4BB6_BE63_5CDCF73897F5__INCLUDED_)
