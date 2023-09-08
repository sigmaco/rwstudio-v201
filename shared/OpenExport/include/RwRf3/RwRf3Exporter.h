// RwRf3Exporter.h: interface for the RwRf3Exporter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RwRf3EXPORTER_H__58A4C312_D591_45BD_82A9_7C7543455408__INCLUDED_)
#define AFX_RwRf3EXPORTER_H__58A4C312_D591_45BD_82A9_7C7543455408__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwRf3.h"
#include "RwExpExporter.h"
#include "RwExpSceneTraverse.h"

#include "RwCommExportOptions.h"

class RwCommAsset;
class RwXMLNode;

#define RWRF3_SCENE_TYPE            L"scene"
#define RWRF3_SCENE_START_FRAME     L"startframe"
#define RWRF3_SCENE_END_FRAME       L"endframe"
#define RWRF3_SCENE_FPS             L"fps"
#define RWRF3_SCENE_AMBIENT_COLOR   L"ambient color"
#define RWRF3_SCENE_BACKGROUND_COLOR L"background color"
#define RWRF3_SCENE_DESCRIPTION     L"description"
#define RWRF3_SCENE_UUID            L"uuid"
#define RWRF3_SCENE_AXIS_SYSTEM     L"axis system"

#define RWRF3_SHADERLIST_TYPE       L"shaderlist"

struct RF3_API RwRf3AssetInfo
{
    RwCommAsset         *m_pAsset;
    RwCommExportOptions m_options;
};

/**
*
*   \ingroup rf3write
*
*   This is the main class responsible for writing rf3 files. The class acts like as an RwComm scene 
*   graph exporter, traversing the scene graph and exporting each RwCommObject it encounters.
*   Each RwCommObject has an RwCommExportObject mapped to it, each one responsible for building an XML
*   sub tree, representing that object's rf3 representation. At the end of the export process, after traversing
*   the entire scene graph, an entire XML tree is built under the class's m_pRootNode member.
*   
*   The class has two main export methods: Export() for writing an rf3 file without any RwCommAsset 
*   information, and ExportFromAssets() for writing an rf3 with the asset information.
*   
*   Just like other RwExpExporter objects the class is constructed with the following default exporter
*   objects (RwCommExportObject), assigned to the following RwCommObjects :

<TABLE BORDER CELLSPACING=2 BORDERCOLOR="#000000" CELLPADDING=7 WIDTH=406>
<TR>
    <TD WIDTH="47%" VALIGN="MIDDLE">
    <B><P>RWCOMMOBJECT </B></TD>
    <TD WIDTH="53%" VALIGN="MIDDLE">
    <P><B>RWCOMMEXPORTOBJECT</B></TD>
</TR>
<TR>
    <TD WIDTH="47%" VALIGN="TOP" HEIGHT=11>
    <P>RwCommTransform</TD>
    <TD WIDTH="53%" VALIGN="TOP" HEIGHT=11>
    <P>RwRf3ExpTransform</TD>
</TR>
<TR>
    <TD WIDTH="47%" VALIGN="TOP" HEIGHT=11>
    <P>RwCommMesh</TD>
    <TD WIDTH="53%" VALIGN="TOP" HEIGHT=11>
    <P>RwRf3ExpMesh</TD>
</TR>
<TR>
    <TD WIDTH="47%" VALIGN="TOP" HEIGHT=11>
    <P>RwCommPatchMesh</TD>
    <TD WIDTH="53%" VALIGN="TOP" HEIGHT=11>
    <P>RwRf3ExpPatchMesh</TD>
</TR>
<TR>
    <TD WIDTH="47%" VALIGN="TOP" HEIGHT=11>
    <P>RwCommShader</TD>
    <TD WIDTH="53%" VALIGN="TOP" HEIGHT=11>
    <P>RwRf3ExpShader</TD>
</TR>
<TR>
    <TD WIDTH="47%" VALIGN="TOP" HEIGHT=11>
    <P>RwCommTexture</TD>
    <TD WIDTH="53%" VALIGN="TOP" HEIGHT=11>
    <P>RwRf3ExpTexture</TD>
</TR>
<TR>
    <TD WIDTH="47%" VALIGN="TOP" HEIGHT=11>
    <P>RwCommBlendShape</TD>
    <TD WIDTH="53%" VALIGN="TOP" HEIGHT=11>
    <P>RwRf3ExpBlendShape</TD>
</TR>
<TR>
    <TD WIDTH="47%" VALIGN="TOP" HEIGHT=11>
    <P>RwCommSkin</TD>
    <TD WIDTH="53%" VALIGN="TOP" HEIGHT=11>
    <P>RwRf3ExpSkin</TD>
</TR>
<TR>
    <TD WIDTH="47%" VALIGN="TOP" HEIGHT=11>
    <P>RwCommCamera</TD>
    <TD WIDTH="53%" VALIGN="TOP" HEIGHT=11>
    <P>RwRf3ExpCamera</TD>
</TR>
<TR>
    <TD WIDTH="47%" VALIGN="TOP" HEIGHT=11>
    <P>RwCommLight</TD>
    <TD WIDTH="53%" VALIGN="TOP" HEIGHT=11>
    <P>RwRf3ExpLight</TD>
</TR>
<TR>
    <TD WIDTH="47%" VALIGN="TOP" HEIGHT=11>
    <P>RwCommAsset</TD>
    <TD WIDTH="53%" VALIGN="TOP" HEIGHT=11>
    <P>RwRf3ExpAsset</TD>
</TR>
</TABLE>
*
*
*   In addition, the class is constructed with the following default traverse actions:
*
<TABLE BORDER CELLSPACING=2 BORDERCOLOR="#000000" CELLPADDING=7 WIDTH=548>
<TR>
    <TD WIDTH="45%" VALIGN="MIDDLE" BGCOLOR="#ffffff">
    <B><P>Traverse Action</B></TD>
    <TD WIDTH="55%" VALIGN="MIDDLE" BGCOLOR="#ffffff">
    <B><P>Description</B></TD>
</TR>
<TR>
    <TD WIDTH="45%" VALIGN="TOP" HEIGHT=11>
    <P>RwExpInitAnimatables</B></TD>
    <TD WIDTH="55%" VALIGN="TOP" HEIGHT=11>
    <P>Goes over all animatables in the traverse list and initializes their animation frame to the user supplied frame value. </TD>
</TR>
<TR>
    <TD WIDTH="45%" VALIGN="TOP" HEIGHT=11>
    <P>RwExpTessellateAction</TD>
    <TD WIDTH="55%" VALIGN="TOP" HEIGHT=11>
    <P>Tessellates all patches in the traverse list. </TD>
</TR>
<TR>
    <TD WIDTH="45%" VALIGN="TOP" HEIGHT=11>
    <P>RwExpExporter</B></TD>
    <TD WIDTH="55%" VALIGN="TOP" HEIGHT=11>
    <P>Performs the actual export action</TD>
</TR>
<TR>
    <TD WIDTH="45%" VALIGN="TOP" HEIGHT=11>
    <P>RwRf3ExpAnimations</TD>
    <TD WIDTH="55%" VALIGN="TOP" HEIGHT=11>
    <P>Exports animation information for all animated objects.</TD>
</TR>
</TABLE>
*
*
*   \see RwExpExporter
*/
class RF3_API RwRf3Exporter : public RwExpExporter, public RwExpGraphicsSceneTraverse  
{
public:
	RwRf3Exporter();
	virtual ~RwRf3Exporter();

    RwCommError     ExportFromAssets(RwCommScene *pScene, const RwVector<RwRf3AssetInfo> &vAssetInfo,
                                const RwWString &filename);

    /** Gets the XML root scene node*/
    RwXMLNode*      GetSceneNode() { return m_pSceneNode; }

    /** Gets the XML root shader list node*/
    RwXMLNode       *GetShaderList();

    // Naming methods
    RwWString    GetObjectName(RwCommObject *pObject);
    void            SetObjectName(RwCommObject *pObject, const RwWString& name);

    /** Streams out all reals in binary.*/
    void            StreamAsBinary(bool mode);

protected:

    virtual RwCommError PreExport(RwCommScene *pScene, const RwVector<RwCommObject*>& vSelection);
    virtual RwCommError PostExport(RwCommScene* pScene, const RwVector<RwCommObject*>& vSelection);
    virtual RwCommError BuildTraverseList(RwVector<RwExpTraverseObject>& vTraverseList, RwCommScene* pScene);
    virtual RwCommError TraverseShaders(RwCommShape* pShape, RwVector<RwExpTraverseObject>  &vTraverseList, const RwCommExportOptions *pOptions);
    virtual RwCommError TraverseAudioObject(RwVector<RwExpTraverseObject> &vTraverseList,
                            RwCommObject *pObject, RwCommScene *pScene);
    virtual RwCommError ExportAssets(const RwVector<RwRf3AssetInfo> &vAssetInfo, RwCommScene *pScene);
    virtual RwCommError ExportFileInfo();
    virtual RwCommError ExportSceneInfo(RwCommScene *pScene);

    void    FetchExternalSkins(RwCommTransform* pRoot, RwVector<RwCommObject*> &vTopLevelObjects);

    void    MergeExportOptions(RwCommExportOptions &dest, const RwCommExportOptions &src);

protected:

    RwXMLNode                       *m_pSceneNode;
    RwXMLNode                       *m_pRootNode;
    RwXMLNode                       *m_pShaderList;
    RwVector<RwCommObject *>        m_vSelection;

    RwMap<RwWString, int>            m_mNameInstances;
    RwMap<RwCommObject *, RwWString> m_mNameTable;

    bool                            m_bWriteFile;

};

#endif // !defined(AFX_RwRf3EXPORTER_H__58A4C312_D591_45BD_82A9_7C7543455408__INCLUDED_)
