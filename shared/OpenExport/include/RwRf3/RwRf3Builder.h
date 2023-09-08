// RwRf3Builder.h: interface for the RwRf3Builder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RwRf3BUILDER_H__CD32C193_8981_45B1_AD29_B8AB53F800F8__INCLUDED_)
#define AFX_RwRf3BUILDER_H__CD32C193_8981_45B1_AD29_B8AB53F800F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwXMLDOM.h"

#include "RwRf3.h"
#include "RwCommBuilder.h"
#include "RwCommObjCreator.h"

class RwMIScene;
class RwRf3XMLParser;

/**
*   \ingroup rf3read 
*
*   This is the main class, responsible for reading rf3 files. The class is an RwCommBuilder class, responsible
*   for building an RwComm scene graph from an rf3 file. The scene graph that the RwRf3Builder builds is a
*   modeler-independent (MI) scene graph, which simply implements the RwComm abstract classes, caching all
*   the scene graph data internally.
*   
*   In order to build the MI scene graph from an rf3 file, the RwRf3Builder class parses the rf3 file using
*   the RwRf3XMLParser, which builds an XML DOM tree from the file. Then, the builder traverses the XML tree, and for
*   each XML node that it encounters it attempts to build a scene graph object from it. To achieve this, the builder
*   uses a lookup table, mapping XML node names to RwCommObjCreator objects. Each time the builder encounters
*   an XML node, whose name is registered in the lookup table, it launches its mapped RwCommObjCreator, which
*   creates the appropriate RwCommObject from it. Users can register their own RwCommObjCreator, using the
*   RwRf3Builder::RegisterCreator() method, in order to customize this process.
*
*   
*   By default, the RwRf3Builder is constructed with the following RwCommObjCreator objects:
*
<TABLE BORDER CELLSPACING=2 BORDERCOLOR="#000000" CELLPADDING=7 WIDTH=576>
<TR>
    <TD WIDTH="33%" VALIGN="MIDDLE" BGCOLOR="#ffffff">
    <B><P ALIGN="CENTER">XML Node Name</B></TD>
    <TD WIDTH="38%" VALIGN="TOP" BGCOLOR="#ffffff">
    <B><P ALIGN="CENTER">RwCommobjcreator</B></TD>
    <TD WIDTH="30%" VALIGN="MIDDLE" BGCOLOR="#ffffff">
    <B><P ALIGN="CENTER">RwCommobject</B></TD>
</TR>
<TR>
    <TD WIDTH="33%" VALIGN="TOP" HEIGHT=11>
    <B><P>RWRF3_TRANSFORM_TAG</B></TD>
    <TD WIDTH="38%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwRf3TransformCreator</B></TD>
    <TD WIDTH="30%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMITransform</B></TD>
</TR>
<TR>
    <TD WIDTH="33%" VALIGN="TOP" HEIGHT=11>
    <B><P>RWRF3_MESH_TAG</B></TD>
    <TD WIDTH="38%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwRf3MeshCreator</B></TD>
    <TD WIDTH="30%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMIMesh</B></TD>
</TR>
<TR>
    <TD WIDTH="33%" VALIGN="TOP" HEIGHT=11>
    <B><P>RWRF3_PATCH_TAG</B></TD>
    <TD WIDTH="38%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwRf3PatchMeshCreator</B></TD>
    <TD WIDTH="30%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMIPatchMesh</B></TD>
</TR>
<TR>
    <TD WIDTH="33%" VALIGN="TOP" HEIGHT=11>
    <B><P>RWRF3_SPLINE_TAG</B></TD>
    <TD WIDTH="38%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwRf3SplineCreator</B></TD>
    <TD WIDTH="30%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMISpline</B></TD>
</TR>
<TR>
    <TD WIDTH="33%" VALIGN="TOP" HEIGHT=11>
    <B><P>RWRF3_SHADER_FUNCTYPE</B></TD>
    <TD WIDTH="38%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwRf3ShaderCreator</B></TD>
    <TD WIDTH="30%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMIShader</B></TD>
</TR>
<TR>
    <TD WIDTH="33%" VALIGN="TOP" HEIGHT=11>
    <B><P>RWRF3_RWMATERIAL_SHADERFUNC</B></TD>
    <TD WIDTH="38%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwRf3RwMaterialCreator</B></TD>
    <TD WIDTH="30%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMIRwMaterial</B></TD>
</TR>
<TR>
    <TD WIDTH="33%" VALIGN="TOP" HEIGHT=11>
    <B><P>RWRF3_TEXTURE_TAG</B></TD>
    <TD WIDTH="38%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwRf3TextureCreator</B></TD>
    <TD WIDTH="30%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMITexture</B></TD>
</TR>
<TR>
    <TD WIDTH="33%" VALIGN="TOP" HEIGHT=11>
    <B><P>RWRF3_ASSET_TAG</B></TD>
    <TD WIDTH="38%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwRf3AssetCreator</B></TD>
    <TD WIDTH="30%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMIAsset</B></TD>
</TR>
<TR>
    <TD WIDTH="33%" VALIGN="TOP" HEIGHT=11>
    <B><P>RWRF3_BLENDSHAPE_TAG</B></TD>
    <TD WIDTH="38%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwRf3BlendShapeCreator</B></TD>
    <TD WIDTH="30%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMIBlendShape</B></TD>
</TR>
<TR>
    <TD WIDTH="33%" VALIGN="TOP" HEIGHT=11>
    <B><P>RWRF3_SKIN_TAG</B></TD>
    <TD WIDTH="38%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwRf3SkinCreator</B></TD>
    <TD WIDTH="30%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMISkin</B></TD>
</TR>
<TR>
    <TD WIDTH="33%" VALIGN="TOP" HEIGHT=11>
    <B><P>RF3_LIGHT_TAG</B></TD>
    <TD WIDTH="38%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwRf3LightCreator</B></TD>
    <TD WIDTH="30%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMILight</B></TD>
</TR>
<TR>
    <TD WIDTH="33%" VALIGN="TOP" HEIGHT=11>
    <B><P>RWRF3_CAMERA_TAG</B></TD>
    <TD WIDTH="38%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwRf3CameraCreator</B></TD>
    <TD WIDTH="30%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMICamera</B></TD>
</TR>
<TR>
    <TD WIDTH="33%" VALIGN="TOP" HEIGHT=11>
    <B><P>RWRF3_MORPH_TAG</B></TD>
    <TD WIDTH="38%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwRf3MorphCreator</B></TD>
    <TD WIDTH="30%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMIMorph</B></TD>
</TR>
</TABLE>
*   
*   \see RwCommBuilder
*   \see RwCommObjCreator
*   \see RwRf3XMLParser
*/
class RF3_API RwRf3Builder  : public RwCommBuilder 
{
public:
	RwRf3Builder();
	virtual ~RwRf3Builder();

	//Build the scene
	virtual RwCommError Build(RwCommScene *&pScene);

	// Register a creator object
	RwCommError     RegisterCreator(const RwWString &tag, RwCommObjCreator *pCreator);

	RwCommObject    *BuildObject(RwXMLNode *pNode, RwCommObject *pParent, RwCommScene *pScene,
                                    bool bAttachParent = true, bool bForceRebuild = false);
    RwCommObject    *BuildObject(const RwWString &name, RwCommScene *pScene, bool bForceRebuild = false);


    /**
    *   Sets the input rf3 file to build
    *
    *   \param filename the full filename of the .rf3 file to build.
    */
    void            SetFileName(const RwWString& filename) { m_RwRf3FileName = filename; }

    /**
    *   Gets the input rf3 file name
    *
    *   \returns a string storing the current filename.
    */
    RwWString     GetFileName() const { return m_RwRf3FileName; }

    /**
    *   Sets the input rf3 XML root node. Users can either set the Rf3 XML root
    *   explicitly, or set the rf3 file name using the SetFileName() method.
    *
    *   \param pRf3RootNode the root of the rf3 DOM tree
    *   \param the Rf3XML parser used to build the DOM
    */
    void            SetRf3RootNode(RwXMLNode *pRf3RootNode, RwRf3XMLParser *pParser);


    /**
    *   Sets the texture path, used for locating textures
    *
    *   \param path a the new texture path.
    */
    void            SetTexturePath(const RwWString& path) { m_texPath = path; }

    /**
    *  Gets the texture path
    *
    *   \returns a string storing the texture path
    */
    RwWString          GetTexturePath() const { return m_texPath; }

    /**
    *   Sets the wave path, used for locating waves
    *
    *   \param path a the new wave path.
    */
    void            SetWavePath(const RwWString& path) { m_sWavPath = path; }

    /**
    *  Gets the wave path
    *
    *   \returns a string storing the wave path
    */
    RwWString          GetWavePath() const { return m_sWavPath; }

    // Registers the custom rf3 XML nodes to the given parser
    static void     RegisterCustomXMLNodes(RwRf3XMLParser *pParser);

protected:

    RwCommObject*   BuildNode(RwXMLNode *pNode, RwXMLNode *pParent, RwCommObject *pCommParent, RwCommScene *pScene);

    virtual void    BuildSceneInfo(RwXMLNode *pNode, RwMIScene *pScene);

    RwWString    GetShaderFuncName(RwXMLNode *pNode);

protected:

    RwWString                            m_RwRf3FileName;
    RwMap<RwWString, RwCommObjCreator*>  m_mCreators;

    RwRf3XMLParser                      *m_pParser;
    RwXMLNode                           *m_pRootRf3;

    RwMap<RwCommObject*, RwWString>      m_mRenameList;

    RwWString                            m_texPath;
    RwWString                            m_sWavPath;
};


/**
*   \ingroup rf3read 
*
*   This is the build data container class, passed to the RwCommObjCreator objects upon creation. It simply
*   contains the RwXMLNode object to build, and an instance of the RwRf3Builder object.
*/   
class RF3_API RwRf3CreationData : public RwCommCreationData
{
public:
    RwRf3CreationData(RwRf3Builder *pBuilder, RwXMLNode *pNode) : m_pBuilder(pBuilder), m_pNode(pNode) {}
    virtual ~RwRf3CreationData() {}

    /**
    *   Gets the XML node to build
    *
    *   \returns the stored RwXMLNode object.
    */
	RwXMLNode* GetNode() const { return m_pNode; }

    /**
    *   Gets the RwRf3Builder object
    *
    *   \returns the stored RwRf3Builder object.
    */
    RwRf3Builder* GetBuilder() const { return m_pBuilder; }

protected:

    RwRf3Builder  *m_pBuilder;
	RwXMLNode    *m_pNode;

};

#endif // !defined(AFX_RwRf3BUILDER_H__CD32C193_8981_45B1_AD29_B8AB53F800F8__INCLUDED_)
