#if !defined RW_EXP_NODE_CHECKER_H
#define RW_EXP_NODE_CHECKER_H

#include <typeinfo>

#include "RwExp.h"
#include "rwcore.h"
#include "RwCommObject.h"
#include "RwExpExporter.h"

class RwExpNodeCheckerFactory;


// =============================================================================
// -------------------------  RwExpExporter CLASS  --------------------------
// =============================================================================

/**
*  \ingroup helpclasses
*
*   This enumaration is used as return values from Node Checking operations. 
*
*    \see RwExpNodeCheckerFactory
*    \see RwExpNodeChecker
*    \see RwExpNodeCheck
*    \see RwExpNodeCheckerBaseImp
*/
typedef enum
{
    /** The check reported no problems with the node*/ 
    RwNodeCheckOk,

    /** The node generated a warning */
    RwNodeCheckWarning,

    /** The node generated an error */
    RwNodeCheckError,

    /** The node passed in is of incorrect type for this check */
    RwNodeCheckInvalidOperand,

    /** Could not check this node for some reason */
    RwNodeCheckFailed 

} RwNodeCheckResult;

// =============================================================================
// ------------------------  RwExpNodeCheck CLASS  -----------------------------
// =============================================================================

/**
*  \ingroup helpclasses
*
*    This class defines the interface for an atomic check acting on a RwCommObject. 
*
*    Before they are exported, scene graph nodes are checked for validity. This checking 
*    is  performed by a series of atomic check objects derived from RwExpNodeCheck.
*    Each object implements a specific check (Examples may be "Check if a texture
*    is power-of-two along each axis" or "Check that a hierarchy has less than 
*    255 bones"). These individual checks are then grouped together into platform
*    specific RwExpNodeChecker objects. Each Supported platform has an associated RwExpNodeChecker 
*    object. 
*
*    Adding a new check to a given platform is straight-forward. 
*
*       A new class is derived from RwExpNodeCheck and all relevant methods are implemented. 
*       Special attention should be paid to the CheckNode method that actually implements the 
*       check. 
* 
*       The checker for the relevant platform can be retrived via the RwExpNodeCheckerFactory
*       class using the SetPlatform and GetChecker Methods. If the check is required on all 
*       platforms, then it should be added to each platform checker in turn. 
*
*       Once the relevant checker is obtained, it is a simple matter of calling its AddCheck
*       method with an instance of the new check class. This check will then be called on 
*       all  relevant nodes being exported. 
*
*
*       GetOperandTypeName    --  The derived class should override this method to return 
*                                 the type name of the nodes that the check is valid for. 
*                                 An example would be a check that deals with textures would
*                                 return "RwCommShader", and a check that deals with geometry
*                                 would return "RwCommMesh"
*
*       IsCheckValidForExporter-- Some checks only apply when a node is being exported 
*                                 as a specific asset type. If the check only applies when
*                                 the node is being exported as an animation then the derived
*                                 class should override IsCheckValidForExporter to check that 
*                                 the given exporter is a dff exporter (RwExpDffExporterType).
*   
*
*
*    \see RwExpNodeCheckerFactory
*    \see RwExpNodeChecker
*    \see RwExpNodeCheckerBaseImp
*/
class RWEXP_API RwExpNodeCheck
{
    friend class RwExpNodeChecker;

protected:
    virtual ~RwExpNodeCheck() {};

public:

    /** The main check method. This is called for each node of the correct type 
    *   in the scene.  The implementation of this class should perform a single 
    *   check to assertain the validity of the given node. 
    *
    *   \param TheNode a pointer to the RwCommObject to be checked
    *   \param RwExpExporter a pointer to the exporter object for the given node. 
    *   \return RwNodeCheckOk If the node passed the check. 
    */
    virtual RwNodeCheckResult CheckNode(RwCommObject *TheNode, RwExpExporter *Exporter) = 0;

    /** Each Checker is assigned a meaningful human-readable name. Implementations should
    *   overload this method to provide the name of the check performed.
    *
    *
    *   \return A pointer to a null terminated string containing the name of the checker. 
    */
    virtual const wchar_t *GetCheckerName(void) = 0;

    /** Most checks act on one type of node only. This method is overloaded to provide the 
    *   object type that the check acts on. 
    *
    *   \return A pointer to a null terminated string containing the type that this checker acts on. 
    */
    virtual const wchar_t *GetOperandTypeName(void) = 0;

    /** If a given node fails its check, it should return a suitable warning message through this 
    *   method. This method will be called by the checker if CheckNode returns a value of 
    *   RwNodeCheckWarning.
    *
    *   \return A string containing a warning message relevant to the last check failure. 
    */
    virtual RwWString GetWarningMessage(void) = 0;

    /** To ensure only suitable checks are performed on nodes, this method 
    *   allows the check to filter nodes based on their exporter type. If the 
    *   check does not recognise, or cannot use the given Exporter, it should return 
    *   false. If this method returns false, the check will not be called on nodes 
    *   associated with the given RwExpExporter.
    *
    *   \param RwExpExporter A pointer to the RwExpExporter object that will be used during checking.
    *   \return true if the RwExpExporter is suitable, false otherwise. 
    */
    virtual bool   IsCheckValidForExporter(RwExpExporter *Exporter) = 0;

    /** Called before each node is checked to ensure the state of the check is correct. 
    *
    */
    virtual void   Reset(void) = 0;

    /** Some checks may wish to concatenate the result of several checks into a single 
    *   report to the user. If this is the case they may use CheckSummary. CheckSummary 
    *   is called after all nodes in the current traverse list are checked. 
    *
    *   \param summray A reference to a string object. This will contain the summary report. 
    *   \return RwNodeCheckOk if no summary is available, another value if there is a summary. 
    */
    virtual RwNodeCheckResult CheckSummary(RwWString &summary) { return RwNodeCheckOk; };
};

// =============================================================================
// ------------------------  RwExpNodeChecker CLASS  ---------------------------
// =============================================================================

/**
*  \ingroup helpclasses
*   
*    The RwExpNodeChecker represents a collection of RwExpNodeChecks specialised for a given
*    platform. Each supported platform will have an assoicated checker. See RwExpNodeCheck
*    for a explination of the node checking architecture and its use. Seperate RwExpNodeCheckers
*    are required for each platform, as each platform has specific limitations that must be tested 
*    for.  
*
*    \see RwExpNodeCheckerFactory
*    \see RwExpNodeCheck
*    \see RwExpNodeCheckerBaseImp
*/
class RWEXP_API RwExpNodeChecker
{
    friend class RwExpNodeCheckerFactory;

protected:

    RwExpNodeChecker(void) {};

    virtual ~RwExpNodeChecker(void){};

    /** Delete the given node check.
    *
    *   \param Check A pointer to a RwExpNodeCheck object to be deleted. 
    */
    void DeleteCheck(RwExpNodeCheck *Check)
    {
        delete Check;
    }

public:

    /** Different platforms have different checks. This method returns the name of the platform 
    *   for which this RwExpNodeChecker is valid. 
    *
    *   \return A Null terminated string indicating the platform for which this checker is valid.
    */
    virtual const wchar_t *GetPlatform(void) = 0;

    /** This is a generic node checking method. It takes an object of type RwCommObject and 
    *   applies all of the relevant checks on this node. 
    *
    *   \param TheNode A pointer to the node to be checked. 
    *   \param Exporter A Pointer to the exporter associated with the given node. 
    *   \return RwNodeCheckOk If the node passed the checks 
    */
    virtual RwNodeCheckResult CheckNode(RwCommObject *TheNode, RwExpExporter *Exporter) = 0;

    /** Add a new check to this collection of checks. 
    *
    *   \param TheCheck A pointer to the new check to add. 
    */
    virtual void AddCheck(RwExpNodeCheck *TheCheck) = 0;

    /** This method allows individual checks to be removed from this checker. 
    *
    *   \param Name The name of the check to remove. 
    */
    virtual void RemoveCheck(const RwWString &Name) = 0;

    /** This method resets all of the checks associated with this checker, making 
    *   them ready for use again. 
    */
    virtual void Reset(void) = 0;

    /** This method is called after all of the nodes have been checked.
    *   it is during this method that all of the individual RwExpNodeCheck summaries are reported 
    *   to the user.
    */
    virtual void PostCheck(void) = 0;
};

// =============================================================================
// ---------------------  RwExpNodeCheckerFactory CLASS  -----------------------
// =============================================================================

/**
*  \ingroup helpclasses
*
*   This class provides a partial implementation of a RwExpNodeChecker. It implements the 
*   RwExpNodeChecker interface. It does not contain any checkers itself.
*   New checkers may derive from this object to provide a basic imeplementation. 
*
*    \see RwExpNodeChecker
*    \see RwExpNodeCheck
*    \see RwExpNodeCheckerBaseImp
*
*/
class RWEXP_API RwExpNodeCheckerFactory 
{

protected:

    RwExpNodeCheckerFactory(void);
   
public:

    /** The RwExpNodeCheckerFactory class is implemented as a singleton. This 
    *   method grants access to the sole instance of this class. 
    *
    *   \return A reference to the single instance of this class.  
    */
    static RwExpNodeCheckerFactory &GetInstance(void);

    virtual ~RwExpNodeCheckerFactory(void);

    /** This method returns the currently selected checker. 
    *
    *   \return A pointer to the currently selected checker.  
    */
    RwExpNodeChecker *GetNodeChecker(void);

    /** Checkers are implemented for each supported platform. This method 
    *   allows the current platform to be set. All following 
    *   calls to GetNodeChecker will return a checker for the selected platform. 
    *
    *   \param Plat The name of the platform to select. 
    */
    void SetCurrentPlatform(RwWString Plat);
   
    /** Before a checker can be used it must be registered with the factory. 
    *   This method associates the given checker with the given platform.
    *   If a checker is already associated with the given platform, it will be 
    *   Replaced. 
    *
    *   \param PlatformID The platform with which the given checker should be associated. 
    *   \param Checker The new checker object 
    */
    void RegisterNodeChecker(RwWString PlatformID, RwExpNodeChecker *Checker);

    
private:

    RwWString                                m_CurrentPlatform;
    RwMap<RwWString, RwExpNodeChecker *> m_NodeCheckers;
};


// =============================================================================
// ---------------------  RwExpNodeCheckerBaseImp CLASS  -----------------------
// =============================================================================

/**
*  \ingroup helpclasses
*
*   This call provides a partial implementation of a RwExpNodeChecker. It implements the 
*   RwExpNodeChecker interface. It does not contain any checkers itself.
*   New checkers may derive from this object to provide a basic imeplementation. 
*
*    \see RwExpNodeCheckerFactory
*    \see RwExpNodeChecker
*    \see RwExpNodeCheck
*/
class RwExpNodeCheckerBaseImp : public RwExpNodeChecker
{
protected:

    RwExpNodeCheckerBaseImp( void ){};
   
public:

    ~RwExpNodeCheckerBaseImp( void );

    // check the given node
    virtual RwNodeCheckResult CheckNode(RwCommObject *TheNode, RwExpExporter *Exporter);

    // add a check for the given type
    virtual void AddCheck(RwExpNodeCheck *TheCheck);
    // remove the named check for the given type
    virtual void RemoveCheck(const RwWString &Name) ;

    // this resets all of the checks, ready to start afresh
    virtual void Reset(void);

    void PostCheck();
    
private:
    void DumpMessage(RwNodeCheckResult Result, RwExpNodeCheck *TheCheck);
    void DumpSummary(RwExpNodeCheck *TheCheck);

    RwMap<RwWString, RwList<RwExpNodeCheck *> > m_Checks;
};

// =============================================================================
// ---------------------  RwNodeCheckerRegistration CLASS  --------------------
// =============================================================================

template <class T> class RwNodeCheckerRegistration
{
public:

    RwNodeCheckerRegistration()
    {
        T *NewInstance  = new T;
        RwWString Platform = NewInstance->GetPlatform();

        RwExpNodeCheckerFactory::GetInstance().RegisterNodeChecker(Platform, NewInstance);
    }
};

RW_IMPLEMENT_SINGLETON_IMPORT(RwExpNodeCheckerFactory);


// =============================================================================
// -------------------------  RwBaseTextureUVSetChecker CLASS  ----------------------
// =============================================================================

class RwBaseTextureUVSetChecker  : public RwExpNodeCheck
{
private:

    RwWString    m_WarningMessage;
    RwWString    m_ShaderName;

public:

    RwBaseTextureUVSetChecker(void);
    virtual RwNodeCheckResult CheckNode(RwCommObject *TheNode, RwExpExporter *TheExporter);
    virtual const wchar_t *GetOperandTypeName(void);
    virtual RwWString GetWarningMessage(void);
    virtual bool   IsCheckValidForExporter(RwExpExporter *Exporter);
    virtual void Reset(void);
    virtual RwNodeCheckResult CheckSummary(RwWString &summary);

    const wchar_t *GetCheckerName(void)
    {
        return L"Check base UV set";
    }
};

// =============================================================================
// -------------------------  RwDualPassUVSetChecker CLASS  ----------------------
// =============================================================================

class RwDualPassUVSetChecker  : public RwExpNodeCheck
{
private:

    RwWString    m_WarningMessage;
    RwWString    m_ShaderName;

public:

    RwDualPassUVSetChecker(void);
    virtual RwNodeCheckResult CheckNode(RwCommObject *TheNode, RwExpExporter *TheExporter);
    virtual const wchar_t *GetOperandTypeName(void);
    virtual RwWString GetWarningMessage(void);
    virtual bool   IsCheckValidForExporter(RwExpExporter *Exporter);
    virtual void Reset(void);
    virtual RwNodeCheckResult CheckSummary(RwWString &summary);

    const wchar_t *GetCheckerName(void)
    {
        return L"Check dual pass UV set";
    }    
};

// =============================================================================
// -------------------------  RwTextureSizeChecker CLASS  ----------------------
// =============================================================================

class RwTextureSizeChecker  : public RwExpNodeCheck
{
private:

    int             m_MaxHeight;
    int             m_MaxWidth;
    RwWString        m_WarningMessage;
    RwSet<RwWString> m_processedTextures;

public:

    RwTextureSizeChecker(int MaxWidth, int MaxHeight);
    virtual RwNodeCheckResult CheckNode(RwCommObject *TheNode, RwExpExporter *TheExporter);
    virtual const wchar_t *GetOperandTypeName(void);
    virtual RwWString GetWarningMessage(void);
    virtual bool   IsCheckValidForExporter(RwExpExporter *Exporter);
    virtual void Reset(void);
    virtual RwNodeCheckResult CheckSummary(RwWString &summary);

    const wchar_t *GetCheckerName(void)
    {
        return L"Check texture size";
    }
    
};

// =============================================================================
// -------------------------  RwCheckTexturePow2 CLASS  ------------------------
// =============================================================================

class RwCheckTexturePow2  : public RwExpNodeCheck
{
    RwWString        m_WarningMessage;
    RwSet<RwWString> m_processedTextures;

public:

    RwCheckTexturePow2( void );
    virtual RwNodeCheckResult CheckNode(RwCommObject *TheNode, RwExpExporter *TheExporter);
    virtual const wchar_t *GetOperandTypeName(void);
    virtual RwWString GetWarningMessage(void);
    virtual bool   IsCheckValidForExporter(RwExpExporter *Exporter);
    virtual void Reset(void);
    virtual RwNodeCheckResult CheckSummary(RwWString &summary);

    const wchar_t *GetCheckerName(void)
    {
        return L"Check texture dimensions";
    }
};

// =============================================================================
// -------------------------  RwMeshSizeChecker CLASS  -------------------------
// =============================================================================

class RwMeshSizeChecker : public RwExpNodeCheck
{
    RwWString m_WarningMessage;
public:
    RwMeshSizeChecker(void);
    virtual RwNodeCheckResult CheckNode(RwCommObject *TheNode, RwExpExporter *TheExporter);
    virtual const wchar_t *GetOperandTypeName(void);
    virtual RwWString GetWarningMessage(void);
    virtual bool   IsCheckValidForExporter(RwExpExporter *Exporter);
    virtual void Reset(void);

    const wchar_t *GetCheckerName(void)
    {
        return L"Check mesh size";
    }
};

// =============================================================================
// -------------------------  RwMeshUVSetChecker CLASS  -------------------------
// =============================================================================

class RwMeshUVSetChecker : public RwExpNodeCheck
{
    RwWString m_WarningMessage;
public:
    RwMeshUVSetChecker(void);
    virtual RwNodeCheckResult CheckNode(RwCommObject *TheNode, RwExpExporter *TheExporter);
    virtual const wchar_t *GetOperandTypeName(void);
    virtual RwWString GetWarningMessage(void);
    virtual bool   IsCheckValidForExporter(RwExpExporter *Exporter);
    virtual void Reset(void);

    const wchar_t *GetCheckerName(void)
    {
        return L"Check mesh UV sets";
    }
};

// =============================================================================
// -------------------------  RwMeshSkinChecker CLASS  -------------------------
// =============================================================================

class RwMeshSkinChecker : public RwExpNodeCheck
{
public:
    RwMeshSkinChecker(void) {};

    virtual RwNodeCheckResult   CheckNode(RwCommObject *TheNode, RwExpExporter *TheExporter);
    virtual const wchar_t          *GetOperandTypeName(void);
    virtual RwWString            GetWarningMessage(void);
    virtual bool                IsCheckValidForExporter(RwExpExporter *Exporter);
    virtual void                Reset(void)             { m_WarningMessage = L""; }

    const wchar_t                  *GetCheckerName(void) { return L"Check rigid skinning"; }

public:
    RwWString                    m_WarningMessage;
};


// =============================================================================
// -------------------------  RwMeshSkinChecker CLASS  -------------------------
// =============================================================================

class RwCheckNumWeights : public RwExpNodeCheck
{
public:
    RwCheckNumWeights(void) {};

    virtual RwNodeCheckResult   CheckNode(RwCommObject *TheNode, RwExpExporter *TheExporter);
    virtual const wchar_t          *GetOperandTypeName(void);
    virtual RwWString            GetWarningMessage(void);
    virtual bool                IsCheckValidForExporter(RwExpExporter *Exporter);
    virtual void                Reset(void)             { m_WarningMessage = L""; }
    virtual RwNodeCheckResult   CheckSummary(RwWString &summary);

    const wchar_t                  *GetCheckerName(void) { return L"Check number of weights"; }

public:
    RwWString                    m_WarningMessage;
};

// =============================================================================
// ---------------------  RwUnweightedVertsChecker CLASS  ----------------------
// =============================================================================

class RwUnweightedVertsChecker : public RwExpNodeCheck
{
    RwWString m_WarningMessage;
public:

    RwUnweightedVertsChecker(void);
    virtual RwNodeCheckResult   CheckNode(RwCommObject *TheNode, RwExpExporter *TheExporter);
    virtual const wchar_t          *GetOperandTypeName(void);
    virtual RwWString        GetWarningMessage(void);
    virtual bool                IsCheckValidForExporter(RwExpExporter *Exporter);
    virtual void                Reset(void);

    const wchar_t *GetCheckerName(void)
    {
        return L"Check unweighted verts";
    }
};

// =============================================================================
// -----------------  RwUnsupportedMatAnimationChecker CLASS  ------------------
// =============================================================================

class RwUnsupportedMatAnimationChecker : public RwExpNodeCheck
{
    RwWString        m_WarningMessage;
    RwSet<RwWString> m_sShaders;

public:

    RwUnsupportedMatAnimationChecker(void);
    virtual RwNodeCheckResult CheckNode(RwCommObject *TheNode, RwExpExporter *TheExporter);
    virtual const wchar_t *GetOperandTypeName(void);
    virtual RwWString GetWarningMessage(void);
    virtual bool   IsCheckValidForExporter(RwExpExporter *Exporter);
    virtual void Reset(void);
    virtual RwNodeCheckResult CheckSummary(RwWString &summary);

    const wchar_t *GetCheckerName(void)
    {
        return L"Check unsupported material animations";
    }
};

// =============================================================================
// -----------------  RwUnsupportedTexAnimationChecker CLASS  ------------------
// =============================================================================

class RwUnsupportedTexAnimationChecker : public RwExpNodeCheck
{
    RwWString        m_WarningMessage;
    RwSet<RwWString> m_processedTextures;

public:
    RwUnsupportedTexAnimationChecker(void);
    virtual RwNodeCheckResult CheckNode(RwCommObject *TheNode, RwExpExporter *TheExporter);
    virtual const wchar_t *GetOperandTypeName(void);
    virtual RwWString GetWarningMessage(void);
    virtual bool   IsCheckValidForExporter(RwExpExporter *Exporter);
    virtual void Reset(void);
    virtual RwNodeCheckResult CheckSummary(RwWString &summary);

    const wchar_t *GetCheckerName(void)
    {
        return L"Check unsupported texture animations";
    }
};

// =============================================================================
// -------------------  RwDoubleSidedMaterialChecker CLASS  --------------------
// =============================================================================

class RwTwoSidedMaterialChecker : public RwExpNodeCheck
{
    RwWString        m_WarningMessage;
    RwSet<RwWString> m_sShaders;

public:
    RwTwoSidedMaterialChecker();
    virtual RwNodeCheckResult CheckNode(RwCommObject *TheNode, RwExpExporter *TheExporter);
    virtual const wchar_t *GetOperandTypeName();
    virtual RwWString GetWarningMessage();
    virtual bool IsCheckValidForExporter(RwExpExporter *pExporter);
    virtual void Reset();
    virtual RwNodeCheckResult CheckSummary(RwWString &summary);

    const wchar_t *GetCheckerName()
    {
        return L"Check 2-sided materials";
    }
};

// =============================================================================
// ----------------  RwUnsupportedLightAnimationChecker CLASS  -----------------
// =============================================================================

class RwUnsupportedLightAnimationChecker : public RwExpNodeCheck
{
    RwWString m_WarningMessage;

public:
    RwUnsupportedLightAnimationChecker(void);
    virtual RwNodeCheckResult CheckNode(RwCommObject *TheNode, RwExpExporter *TheExporter);
    virtual const wchar_t *GetOperandTypeName(void);
    virtual RwWString GetWarningMessage(void);
    virtual bool   IsCheckValidForExporter(RwExpExporter *Exporter);
    virtual void Reset(void);
    virtual RwNodeCheckResult CheckSummary(RwWString &summary);

    const wchar_t *GetCheckerName(void)
    {
        return L"Check unsupported light animations";
    }
};

// =============================================================================
// ----------------  RwUnsupportedCameraAnimationChecker CLASS  ----------------
// =============================================================================

class RwUnsupportedCameraAnimationChecker : public RwExpNodeCheck
{
    RwWString m_WarningMessage;

public:
    RwUnsupportedCameraAnimationChecker(void);
    virtual RwNodeCheckResult CheckNode(RwCommObject *TheNode, RwExpExporter *TheExporter);
    virtual const wchar_t *GetOperandTypeName(void);
    virtual RwWString GetWarningMessage(void);
    virtual bool   IsCheckValidForExporter(RwExpExporter *Exporter);
    virtual void Reset(void);
    virtual RwNodeCheckResult CheckSummary(RwWString &summary);

    const wchar_t *GetCheckerName(void)
    {
        return L"Check unsupported camera animations";
    }
};

// =============================================================================
// ----------------  RwUnsupportedVisibilityAnimationChecker CLASS  ------------
// =============================================================================

class RwUnsupportedVisibilityAnimationChecker : public RwExpNodeCheck
{
    RwWString m_WarningMessage;

public:
    RwUnsupportedVisibilityAnimationChecker(void);
    virtual RwNodeCheckResult CheckNode(RwCommObject *TheNode, RwExpExporter *TheExporter); 
    virtual const wchar_t *GetOperandTypeName(void);
    virtual RwWString GetWarningMessage(void);
    virtual bool   IsCheckValidForExporter(RwExpExporter *Exporter);
    virtual void Reset(void);
    virtual RwNodeCheckResult CheckSummary(RwWString &summary);

    const wchar_t *GetCheckerName(void)
    {
        return L"Check unsupported visibility animation checker";
    }
};

// =============================================================================
// ------------------------------  RwCheckNumLights CLASS  ---------------------
// =============================================================================

class RwCheckNumLights  : public RwExpNodeCheck
{
private:

    int         m_MaxLights;
    int         m_CurrentLightCounts;
    RwWString    m_WarningMessage;

public:

    RwCheckNumLights(int NumLights);
    virtual RwNodeCheckResult CheckNode(RwCommObject *TheNode, RwExpExporter *TheExporter);
    virtual const wchar_t *GetOperandTypeName(void);
    virtual RwWString GetWarningMessage(void);
    virtual bool   IsCheckValidForExporter(RwExpExporter *Exporter);
    virtual void Reset(void);
    virtual RwNodeCheckResult CheckSummary(RwWString &Message);

    const wchar_t *GetCheckerName(void)
    {
        return L"Check number of lights";
    }
};

// =============================================================================
// ------------------------  RwCheckNumBonesPerHeirarchy CLASS  ----------------
// =============================================================================

class RwCheckNumBonesPerHierarchy  : public RwExpNodeCheck
{
private:

    int m_MaxNumBones;
    
public:

   RwCheckNumBonesPerHierarchy(int MaxBones);
    virtual RwNodeCheckResult CheckNode(RwCommObject *TheNode, RwExpExporter *TheExporter);
    virtual const wchar_t *GetOperandTypeName(void);
    virtual RwWString GetWarningMessage(void);
    virtual bool   IsCheckValidForExporter(RwExpExporter *Exporter);
    virtual void Reset(void);

    const wchar_t *GetCheckerName(void)
    {
        return L"Check number of bones per hierarchy";
    }
};

// =============================================================================
// ------------------------  RwCheckNumBonesPerMesh CLASS  ---------------------
// =============================================================================

class RwCheckNumBonesPerMesh  : public RwExpNodeCheck
{
private:
    RwWString    m_CheckMessage;
    int         m_MaxNumBones;
    
public:

    RwCheckNumBonesPerMesh(int MaxBones);
    virtual RwNodeCheckResult CheckNode(RwCommObject *TheNode, RwExpExporter *TheExporter);
    virtual const wchar_t *GetOperandTypeName(void);
    virtual RwWString GetWarningMessage(void);
    virtual bool   IsCheckValidForExporter(RwExpExporter *Exporter);
    virtual void Reset(void);

    const wchar_t *RwCheckNumBonesPerMesh::GetCheckerName(void)
    {
        return L"Check number of bones per mesh";
    }

};

// =============================================================================
// -----------------------  RwCheckNumBonesSingleWeight CLASS ------------------
// =============================================================================

class RwCheckNumBonesSingleWeight  : public RwExpNodeCheck
{
private:
    RwWString    m_CheckMessage;
    int         m_MaxNumBonesSingleWeight;    

public:

    RwCheckNumBonesSingleWeight(int MaxBonesSingleWeight);
    virtual RwNodeCheckResult CheckNode(RwCommObject *TheNode, RwExpExporter *TheExporter);
    virtual const wchar_t *GetOperandTypeName(void);
    virtual RwWString GetWarningMessage(void);
    virtual bool   IsCheckValidForExporter(RwExpExporter *Exporter);
    virtual void Reset(void);

    const wchar_t *GetCheckerName(void)
    {
        return L"Check weight specific bones";
    }
};

// =============================================================================
// -----------------------  RwPatchCheckNumCtrlPts CLASS -----------------------
// =============================================================================

class RwPatchCheckNumCtrlPts  : public RwExpNodeCheck
{
private:
    RwWString  m_NodeName;
    int     m_MaxNumCtrlPoints;
    
public:

   RwPatchCheckNumCtrlPts(int Max);
    virtual RwNodeCheckResult CheckNode(RwCommObject *TheNode, RwExpExporter *TheExporter);
    virtual const wchar_t *GetOperandTypeName(void);
    virtual RwWString GetWarningMessage(void);
    virtual bool   IsCheckValidForExporter(RwExpExporter *Exporter);
    virtual void Reset(void);

    const wchar_t *GetCheckerName(void)
    {
        return L"Check number of patch mesh control points";
    }
};

// =============================================================================
// ------------------  RwPatchCheckNumGeneratedVerts CLASS ---------------------
// =============================================================================

class RwPatchCheckNumGeneratedVerts  : public RwExpNodeCheck
{
private:

    int             m_MaxNumGenVerts;
    RwWString    m_NodeName;
    int             m_MaxSafeLOD;
    
public:

   RwPatchCheckNumGeneratedVerts(int Max);
    virtual RwNodeCheckResult CheckNode(RwCommObject *TheNode, RwExpExporter *TheExporter);
    virtual const wchar_t *GetOperandTypeName(void);
    virtual RwWString GetWarningMessage(void);
    virtual bool   IsCheckValidForExporter(RwExpExporter *Exporter);
    virtual void Reset(void);

    const wchar_t *GetCheckerName(void)
    {
        return L"Check number of patch generated verts";
    }

};
// =============================================================================
// -------------------------  RwExpUVAnimMatFXCheck CLASS  ---------------------
// =============================================================================

class RwExpUVAnimMatFXCheck : public RwExpNodeCheck
{
public:
    RwExpUVAnimMatFXCheck(void) {};

    virtual RwNodeCheckResult   CheckNode(RwCommObject *TheNode, RwExpExporter *TheExporter);
    virtual const wchar_t          *GetOperandTypeName(void);
    virtual RwWString        GetWarningMessage(void);
    virtual bool                IsCheckValidForExporter(RwExpExporter *Exporter);
    virtual void                Reset(void);

    const wchar_t                  *GetCheckerName(void) { return L"Check UVAnim with MatFX"; }
    virtual RwNodeCheckResult CheckSummary(RwWString &Message);

private:
    RwWString        m_WarningMessage;
    RwSet<RwWString> m_sShaders;
    bool            m_failed;
};
// =============================================================================
// -------------------------  RwExpMatAlphaTextureCheck CLASS  ---------------------
// =============================================================================

class RwExpAlphaTextureCheck : public RwExpNodeCheck
{
public:
    RwExpAlphaTextureCheck(void) {};

    virtual RwNodeCheckResult CheckNode(RwCommObject *TheNode, RwExpExporter *Exporter) {return RwNodeCheckOk;}
    virtual const wchar_t        *GetOperandTypeName(void) {return L"";}
    virtual RwWString      GetWarningMessage(void) {return L"";}
    virtual void              Reset(void) {}

    virtual bool              IsCheckValidForExporter(RwExpExporter *Exporter);
    const wchar_t                *GetCheckerName(void) { return L"Check alpha textures"; }
    virtual RwNodeCheckResult CheckSummary(RwWString &Message);
};

// =============================================================================
// ------------------  RwExpCheckUVFilterErrors CLASS -------------------------
// =============================================================================

class RwExpLimitUVFilter;

class RwExpCheckUVFilterErrors  : public RwExpNodeCheck
{
private:
    //some helpful typedefs
    typedef RwSet<RwExpLimitUVFilter *>             UVLimitFilterSet;
    typedef RwSet<RwExpLimitUVFilter *>::iterator   UVLimitFilterSetIt;
    typedef RwMap<RwWString, RwWString >              TextureData;
    typedef RwMap<RwWString, RwWString >::iterator    TextureDataIt;


    //The filter we are extracting data from
    UVLimitFilterSet  m_UVFilterSet;

public:

    RwExpCheckUVFilterErrors();
    virtual RwNodeCheckResult CheckNode(RwCommObject *node, RwExpExporter *exporter);
    virtual const wchar_t *GetOperandTypeName(void);
    virtual RwWString GetWarningMessage(void);
    virtual bool   IsCheckValidForExporter(RwExpExporter *exporter);
    virtual void Reset(void);
    virtual RwNodeCheckResult CheckSummary(RwWString &message);

    const wchar_t *GetCheckerName(void)
    {
        return L"Check required textures are present when texel UV limiting";
    }

};

// =============================================================================
// -----------------------  RwExpXBoxChecker CLASS -----------------------------
// =============================================================================

class RwExpXBoxChecker : public RwExpNodeCheckerBaseImp
{
public:
    virtual const wchar_t *GetPlatform(void)
    {
        return L"Xbox";
    }

    RwExpXBoxChecker(void);
};

// =============================================================================
// -----------------------  RwExpGCNChecker CLASS ------------------------------
// =============================================================================

class RwExpGCNChecker : public RwExpNodeCheckerBaseImp
{
public:
    virtual const wchar_t *GetPlatform(void)
    {
        return L"GameCube";
    }

    RwExpGCNChecker(void);
    
};

// =============================================================================
// ------------------------  RwExpGenericChecker CLASS  ------------------------
// =============================================================================

class RwExpGenericChecker : public RwExpNodeCheckerBaseImp
{
public:
    virtual const wchar_t *GetPlatform(void)
    {
        return L"Generic";
    }

    RwExpGenericChecker(void);
    
};

// =============================================================================
// -----------------------  RwExpPS2Checker CLASS ------------------------------
// =============================================================================

class RwExpPS2Checker : public RwExpNodeCheckerBaseImp
{
public:
    virtual const wchar_t *GetPlatform(void)
    {
        return L"PS2";
    }

    RwExpPS2Checker(void);
  
};

#endif
