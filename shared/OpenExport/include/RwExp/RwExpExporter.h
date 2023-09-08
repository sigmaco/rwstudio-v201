
#ifndef RWEXPEXPORTER_H
#define RWEXPEXPORTER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwExp.h"
#include "RwExpTraverseAction.h"

#include "RwCommExporter.h"
#include "RwCommGeometry.h"

class RwExpExportOptions;
class RwExpStreamAssetList;

// =============================================================================
// -------------------------  RwExpExporter CLASS  --------------------------
// =============================================================================

/**
*
*  \ingroup exporters
*
*   This is the base class for all the specific RwExp exporters. The class is derived from the RwCommExporter class.
*   The class manages the traverse list and introduces the notion of traverse actions. A traverse action is a simple
*   modular action that performs some specific action on a traverse list. 
*
*   In its main Export() method, the RwExpExporter builds the traverse list and then passes it to a collection of
*   traverse actions. Since the RwExpExporter does not know how to exactly construct a traverse list, the process of
*   building the list is left to the deriving classes. Classes wishing to build their own traverse list override the
*   BuildTraverseList() method.
*
*   The RwExpExporter class manages a queue of traverse actions. After creating the traverse list, the RwExpExporter
*   cycles through all the traverse actions, passing the traverse list to them. Among the traverse actions, is
*   the RwExpExportAction, which is the action that performs the actual exporting, using the
*   RwCommExporter::ExportObject() method.
*
*   Each action in the queue can be enabled or disabled using the EnableAction() method. You can always
*   insert your own custom action using the InsertAction() method, or alternatively replace an existing
*   action with your custom action using the ReplaceAction() method.
*   Users wishing to access a specific action by type can use the GetAction() method.
*   You can get a list of all the action types in the action queue using the GetActionTypes() method and insert your
*   custom action after or before a specific action type.
*   Users wishing to execute their own custom export code, before or after the traverse actions are executed,
*   can override the PreExport() and PostExport() methods.
* 
*/

class RWEXP_API RwExpExporter : public RwCommExporter
{
public:
    RwExpExporter();
    virtual ~RwExpExporter();

    virtual RwCommError Export(const RwCommExportInfo &exportInfo);

    // Action methods

    // Get all action types
    void GetActionTypes(RwVector<RwCommType> &vActionTypes) const;

    // Get action by type
    RwExpTraverseAction *GetAction(const RwCommType &type) const;

    // Enable/disable action by type
    RwCommError EnableAction(const RwCommType &type, bool bEnable);

    // Add action at end/start of the pipeline
    RwCommError AddAction(RwExpTraverseAction *pAction, bool bAtEnd = true);

    // Insert action before/after given filter type
    RwCommError InsertAction(const RwCommType &type, RwExpTraverseAction *pAction, bool bBefore = true);

    // Insert action before given index
    RwCommError InsertAction(int index, RwExpTraverseAction *pAction);

    // Replaces an action specified by type with a new action
    RwCommError ReplaceAction(const RwCommType &type, RwExpTraverseAction *pAction);

    // Remove an action by type
    RwCommError RemoveAction(const RwCommType &type);
    
    // RwExpExporter methods
    RwCommReal  GetGlobalScaling();

    // The name of the asset being expored
    RwWString    &GetAssetName() { return m_sAssetName; }

    // The file being exported to
    RwWString    &GetFileName() { return m_sFileName; }

    // The GUID being exported
    RwWString    &GetGUID() { return m_sGUID; }

    // The GUID of the parent of the asset being exported
    RwWString    &GetParentGUID() { return m_sParentGUID; }

    // Get RwExpStreamAssetList
    RwExpStreamAssetList *GetAssetList();
    
    // Set RwExpStreamAssetList
    RwCommError SetAssetList(RwExpStreamAssetList *pAssetList);

protected:

    /** 
    * Called from RwExpExporter::Export() before the export proccess begins. Users can override this method
    * and add their own custom pre export code here.
    *
    *   \param pScene a pointer to the RwCommScene object
    *   \param vSelection a vector of the selected RwCommTransform objects
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise
    */
    virtual RwCommError PreExport(RwCommScene *pScene, const RwVector<RwCommObject *> &vSelection)
                            { return RWCOMM_SUCCESS; }

    /** Called from RwExpExporter::Export() after the export proccess ends. Users can override this method
    * and add their own custom post export code here.
    *
    *   \param pScene a pointer to the RwCommScene object
    *   \param vSelection a vector of the selected RwCommTransform objects
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise
    */
    virtual RwCommError PostExport(RwCommScene *pScene, const RwVector<RwCommObject *> &vSelection)
                            { return RWCOMM_SUCCESS; }
   
    /**
    *   Builds the traverse list. Each deriving class should build its traverse list in this method. Called
    *   from RwExpExporter::Export().
    *
    *   \param vTraverseList a vector of RwExpTraverseObject objects, which makes up the traverse list
    *   \param pScene a pointer to the RwCommScene object
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise
    **/
    virtual RwCommError BuildTraverseList(RwVector<RwExpTraverseObject> &vTraverseList, RwCommScene *pScene)
                            { return RWCOMM_NOT_IMPLEMENTED; }

    // Apply all the actions
    RwCommError ApplyActions(RwVector<RwExpTraverseObject> &vTraverseList,
                                RwCommScene *pScene, const RwCommExportOptions *pExportOptions);

    // Clean up all the actions
    RwCommError CleanUpActions();

    /** 
    * Called once before each traverse action is applied. Users can override this method and add their own action
    * initialization code here. 
    * \param pAction a pointer to the traverse action, which is going to be applied.
    */
    virtual void PreAction(RwExpTraverseAction *pAction) {}

    /**
    * Called once after each traverse action is applied. Users can override this method and add their own post action
    * code here.
    * \param pAction a pointer to the traverse action, which was just applied.
    */
    virtual void PostAction(RwExpTraverseAction *pAction) {}

protected:

    RwList<RwExpTraverseAction *>   m_lActions;
    RwWString                        m_sAssetName;
    RwWString                        m_sFileName;
    RwWString                        m_sGUID;
    RwWString                        m_sParentGUID;
    RwExpStreamAssetList                *m_pAssetList;
};

#endif // RWEXPEXPORTER_H
