#if !defined(RW_MAYA_SCENE_H)
#define RW_MAYA_SCENE_H

#include "RwCommScene.h"

#include "RwMayaBlindData.h"

// =============================================================================
// ----------------------  RwMayaBlindDataTemplate STRUCT  ---------------------
// =============================================================================

/**
*  \ingroup mayascenegraph 
*
*   This is a structure holding information about Maya blind data templates.
*   This is used by the RwMayaBlindData::AddMayaNodeBlindData()
*
*   \see RwMayaScene::ProcessBlineDataTemplates()
*/
struct RWMAYA_API RwMayaBlindDataTemplate
{
    /** ID of the blind data template */
    int     templateID;
    /** name of the blind data template */
    RwWString  templateName;
    /** template node */
    MObject templateNode;
    /** template node name */
    RwWString  templateNodeName;
};

typedef RwList<RwMayaBlindDataTemplate>             RwMayaBlindDataTemplateList;
typedef RwList<RwMayaBlindDataTemplate>::iterator   RwMayaBlindDataTemplateListIt;

// =============================================================================
// ----------------------------  RwMayaScene CLASS  ----------------------------
// =============================================================================

/**
*  \ingroup mayascenegraph 
*
*   This is the RwMaya implementation of RwCommScene.
*
*   \see RwCommScene
*/
class RWMAYA_API RwMayaScene  : public RwCommScene
{
public:
	RwMayaScene();
	virtual ~RwMayaScene();

    // Scene settings
    virtual RwCommReal      FramesToSec(RwCommReal frame) const;

    /**
     *  Maya implementation of RwCommScene::GetAxisSystem. Returns the identity matrix
     * 
     *  \param matrix the matrix to be filled in.
     *  \returns a pointer to the creator if found, NULL otherwise.
     */
    virtual RwCommError     GetAxisSystem(RwCommMatrix &matrix) const;

    virtual RwCommReal      GetStartFrame() const;
    virtual RwCommReal      GetEndFrame() const;
    virtual RwCommError     GetDescription(RwWString &description) const { return RWCOMM_NOT_IMPLEMENTED; }
    virtual RwCommError     GetGUID(RwWString &guid) const { return RWCOMM_NOT_IMPLEMENTED; }
    virtual RwCommError     GetBackgroundColor(RwCommVec3 &color) const;

    // Object mapping functions
    void                    MapObject(MDagPath dagPath, RwCommObject *pObject);

    // Object get functions
    RwCommObject            *GetCommObject(MDagPath dagPath);

    // Time set
    void                    SetFrame(RwCommReal frame);

    /**
     *  This function sets up a cache of Maya blind data templates used by the RwMayaBlindData class.
     *  It is called from the RwMayaScene constructor.
     */
    void                    ProcessBlineDataTemplates();

    /**
     *  Gets the user defined name associated with a blind data template ID.
     *  \param templateID the template ID to be looked up.
     *  \returns the template name if successful, empty string otherwise.
     */
    RwWString             GetBlindDataTemplateName(int templateID);

    /**
     *  Gets the dependency node name matching a blind data template ID.
     *  \param templateID the template ID to be looked up.
     *  \returns the template node name if successful, empty string otherwise.
     */
    RwWString             GetBlindDataTemplateNodeName(int templateID);

    /**
     *  Gets the dependency node matching a blind data template ID.
     *  \param templateID the template ID to be looked up.
     *  \returns the template node if successful, null MObject otherwise.
     */
    MObject                 GetBlindDataTemplateNode(int templateID);

    RwMayaBlindDataTemplateList             m_blindDataTemplates;

protected:
    RwList< RwPair<MDagPath, RwCommObject *> >  m_mObjects;
    RwCommReal                              m_frameConvert;
    RwCommReal                              m_currentFrame;
};

#endif // !defined(RW_MAYA_SCENE_H)
