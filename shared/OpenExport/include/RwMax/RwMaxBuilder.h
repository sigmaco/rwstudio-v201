// RwMaxBuilder.h: interface for the RwMaxBuilder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWMAXBUILDER_H__C4007D85_C05A_4522_815D_A38A3830EFCD__INCLUDED_)
#define AFX_RWMAXBUILDER_H__C4007D85_C05A_4522_815D_A38A3830EFCD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#pragma warning(disable : 4786)
#pragma warning(disable : 4251)

#include <map>

#include "RwCommBuilder.h"
#include "RwCommObjCreator.h"
#include "RwMaxScene.h"
#include "RwMax.h"

/**
*  \ingroup maxbuilder 
*
*   This is the RwMax implementation of the RwCommBuilder.
*   The class's main Build() method traverses the 3ds max scene graph, and for each node it encounters, 
*   it looks up if that 3ds max object has an RwCommObjCreator object mapped to it. The class does this by maintaining
*   two lookup tables. One for mapping Class_ID structures to RwCommObjCreator objects, and the other mapping 
*   SClass_ID objects to RwCommObjCreator objects. For each node the builder checks if that object has a Class_ID
*   creator, and if not it looks for an SClass_ID creator. This type checking is performed inside the 
*   class's BuildObject() method, which is called for each 3ds max object and returns a valid RwCommObjCreator if it
*   finds one.
*   
*   The RwMaxBuilder class is constructed with a default set of creator objects. Users wishing to register their own
*   custom creator objects can do so by using the RegisterCreator() method, supplying a Class_ID or a SClass_ID with
*   an instance of an RwCommObjCreator object. Users can also unregister a specific Class_ID type by supplying a
*   NULL creator.
*   
*   For each INode the builder traverses, it attempts to build the RwComm objects in the following order:
*   \li First it attempts to build the actual INode, which by default is mapped to an RwMaxTransformCreator object. (RwMaxCreationData class is used.)
*   \li Then, it traverses the materials of that INode, attempting to build each one. (RwMaxMaterialData class is used.)
*   \li For each material, it traverses the texture list of that material, attempting to build each one. (RwMaxTextureData class is used.)
*   \li Then, it tries to build the INode's actual object, by calling EvalWorldState() on that INode. (RwMaxCreationData class is used.)
*   \li It then proceeds with attempting to build the INode's modifiers. (RwMaxModifierData class is used.)
*   \li And finally, it tries to build the INode's controls. (RwMaxControlData class is used.)
*
*   Here's a list of all the different default SClass_IDs mapped to the different RwCommObjCreator instances
*   (with their created RwCommObjects):
* 
*   <TABLE BORDER CELLSPACING=2 BORDERCOLOR="#000000" CELLPADDING=7 WIDTH=576>
<TR>
    <TD WIDTH="33%" VALIGN="MIDDLE" BGCOLOR="#ffffff">
    <B><P ALIGN="CENTER">SCLASS_ID</B></TD>
    <TD WIDTH="38%" VALIGN="TOP" BGCOLOR="#ffffff">
    <B><P ALIGN="CENTER">RwCommobjcreator</B></TD>
    <TD WIDTH="30%" VALIGN="MIDDLE" BGCOLOR="#ffffff">
    <B><P ALIGN="CENTER">RwCommobject</B></TD>
</TR>
<TR>
    <TD WIDTH="33%" VALIGN="TOP" HEIGHT=11>
    <B><P>BASENODE_CLASS_ID</B></TD>
    <TD WIDTH="38%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMaxTransformCreator</B></TD>
    <TD WIDTH="30%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMaxTransform</B></TD>
</TR>
<TR>
    <TD WIDTH="33%" VALIGN="TOP" HEIGHT=11>
    <B><P>GEOMOBJECT_CLASS_ID</B></TD>
    <TD WIDTH="38%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMaxMeshCreator</B></TD>
    <TD WIDTH="30%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMaxMesh</B></TD>
</TR>
<TR>
    <TD WIDTH="33%" VALIGN="TOP" HEIGHT=11>
    <B><P>CAMERA_CLASS_ID</B></TD>
    <TD WIDTH="38%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMaxCameraCreator</B></TD>
    <TD WIDTH="30%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMaxCamera</B></TD>
</TR>
<TR>
    <TD WIDTH="33%" VALIGN="TOP" HEIGHT=11>
    <B><P>MATERIAL_CLASS_ID</B></TD>
    <TD WIDTH="38%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMaxShaderCreator</B></TD>
    <TD WIDTH="30%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMaxShader</B></TD>
</TR>
<TR>
    <TD WIDTH="33%" VALIGN="TOP" HEIGHT=11>
    <B><P>TEXMAP_CLASS_ID</B></TD>
    <TD WIDTH="38%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMaxTextureCreator</B></TD>
    <TD WIDTH="30%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMaxTexture</B></TD>
</TR>
<TR>
    <TD WIDTH="33%" VALIGN="TOP" HEIGHT=11>
    <B><P>LIGHT_CLASS_ID</B></TD>
    <TD WIDTH="38%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMaxLightCreator</B></TD>
    <TD WIDTH="30%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMaxLight</B></TD>
</TR>
<TR>
    <TD WIDTH="33%" VALIGN="TOP" HEIGHT=11>
    <B><P>CTRL_FLOAT_CLASS_ID</B></TD>
    <TD WIDTH="38%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMaxDefaultControlCreator</B></TD>
    <TD WIDTH="30%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMaxDefaultControl</B></TD>
</TR>
<TR>
    <TD WIDTH="33%" VALIGN="TOP" HEIGHT=11>
    <B><P>CTRL_POINT3_CLASS_ID</B></TD>
    <TD WIDTH="38%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMaxDefaultControlCreator</B></TD>
    <TD WIDTH="30%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMaxDefaultControl</B></TD>
</TR>
<TR>
    <TD WIDTH="33%" VALIGN="TOP" HEIGHT=11>
    <B><P>CTRL_MATRIX3_CLASS_ID</B></TD>
    <TD WIDTH="38%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMaxDefaultControlCreator</B></TD>
    <TD WIDTH="30%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMaxDefaultControl</B></TD>
</TR>
<TR>
    <TD WIDTH="33%" VALIGN="TOP" HEIGHT=11>
    <B><P>CTRL_SCALE_CLASS_ID</B></TD>
    <TD WIDTH="38%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMaxDefaultControlCreator</B></TD>
    <TD WIDTH="30%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMaxDefaultControl</B></TD>
</TR>
<TR>
    <TD WIDTH="33%" VALIGN="TOP" HEIGHT=11>
    <B><P>CTRL_ROTATION_CLASS_ID</B></TD>
    <TD WIDTH="38%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMaxDefaultControlCreator</B></TD>
    <TD WIDTH="30%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMaxDefaultControl</B></TD>
</TR>
<TR>
    <TD WIDTH="33%" VALIGN="TOP" HEIGHT=11>
    <B><P>CTRL_POSITION_CLASS_ID</B></TD>
    <TD WIDTH="38%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMaxDefaultControlCreator</B></TD>
    <TD WIDTH="30%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMaxDefaultControl</B></TD>
</TR>
</TABLE>
*
*
*   Here's a partial list of all the different default Class_IDs mapped to the different RwCommObjCreator
*   instances:
*
<TABLE BORDER CELLSPACING=2 BORDERCOLOR="#000000" CELLPADDING=7 WIDTH=595>
<TR>
    <TD WIDTH="32%" VALIGN="MIDDLE" BGCOLOR="#ffffff">
    <B><P ALIGN="CENTER">CLASS_ID</B></TD>
    <TD WIDTH="41%" VALIGN="TOP" BGCOLOR="#ffffff">
    <B><P ALIGN="CENTER">RwCommobjcreator</B></TD>
    <TD WIDTH="27%" VALIGN="MIDDLE" BGCOLOR="#ffffff">
    <B><P ALIGN="CENTER">RwCommobject</B></TD>
</TR>
<TR>
    <TD WIDTH="32%" VALIGN="TOP" HEIGHT=11>
    <B><P>patchObjectClassID</B></TD>
    <TD WIDTH="41%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMaxPatchMeshCreator</B></TD>
    <TD WIDTH="27%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMaxPatchMesh</B></TD>
</TR>
<TR>
    <TD WIDTH="32%" VALIGN="TOP" HEIGHT=11>
    <B><P>TRIOBJ_CLASS_ID</B></TD>
    <TD WIDTH="41%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMaxMeshCreator</B></TD>
    <TD WIDTH="27%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMaxMesh</B></TD>
</TR>
<TR>
    <TD WIDTH="32%" VALIGN="TOP" HEIGHT=11>
    <B><P>EDITTRIOBJ_CLASS_ID</B></TD>
    <TD WIDTH="41%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMaxMeshCreator</B></TD>
    <TD WIDTH="27%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMaxMesh</B></TD>
</TR>
<TR>
    <TD WIDTH="32%" VALIGN="TOP" HEIGHT=11>
    <B><P>LOOKAT_CAM_CLASS_ID</B></TD>
    <TD WIDTH="41%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMaxCameraCreator</B></TD>
    <TD WIDTH="27%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMaxCamera</B></TD>
</TR>
<TR>
    <TD WIDTH="32%" VALIGN="TOP" HEIGHT=11>
    <B><P>BP_CLASS_ID_WSM</B></TD>
    <TD WIDTH="41%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMaxBonesProCreator</B></TD>
    <TD WIDTH="27%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMaxBonesPro</B></TD>
</TR>
<TR>
    <TD WIDTH="32%" VALIGN="TOP" HEIGHT=11>
    <B><P>BP_CLASS_ID_OSM</B></TD>
    <TD WIDTH="41%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMaxBonesProCreator</B></TD>
    <TD WIDTH="27%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMaxBonesPro</B></TD>
</TR>
<TR>
    <TD WIDTH="32%" VALIGN="TOP" HEIGHT=11>
    <B><P>PHYSIQUE_CLASS_ID</B></TD>
    <TD WIDTH="41%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMaxPhysiqueCreator</B></TD>
    <TD WIDTH="27%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMaxPhysique</B></TD>
</TR>
<TR>
    <TD WIDTH="32%" VALIGN="TOP" HEIGHT=11>
    <B><P>MR3_CLASS_ID</B></TD>
    <TD WIDTH="41%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMaxBlendShapeCreator</B></TD>
    <TD WIDTH="27%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMaxBlendShape</B></TD>
</TR>
</TABLE>
*
*   \see RwCommBuilder
*   \see RwMaxCreationData
*
*/
class RWMAX_API RwMaxBuilder : public RwCommBuilder 
{
public:
	RwMaxBuilder();
	virtual ~RwMaxBuilder();

	// Scene build function
	RwCommError         Build(RwCommScene *&pScene);

	// Creator Methods
	RwCommError RegisterCreator(const Class_ID &cid, RwCommObjCreator *pCreator);
	RwCommError RegisterCreator(const SClass_ID &cid, RwCommObjCreator *pCreator);

    const RwCommObjCreator *GetObjCreator(const Class_ID &cid) const;
    const RwCommObjCreator *GetObjCreator(const SClass_ID &cid) const;

	BOOL Traverse(INode *node, RwCommObject *pParent);
	BOOL TraverseMaterials(Mtl *pMtl, INode *node, RwCommObject *pParent);

	virtual BOOL	OnBuildObject(ReferenceTarget *pObj) const;

	RwCommObject    *BuildObject(Class_ID &cid, SClass_ID &sid,
								RwCommCreationData *pCreationData,
								RwCommObject *pParent,
								ReferenceTarget *pObj);

protected:

    BitmapTex       *BuildBitmapTex(RwCommObject *pMaterial, PBBitmap *pBBitmap, int channel);
    RwCommError     ExtractViewportExtensionLightmapData(Mtl *pMtl,
                                               RwCommObject *pExpMaterial,
                                               PBBitmap *&pLightmapBitmap,
                                               PBBitmap *&pDiffuseBitmap,
                                               int &lightmapChannel,
                                               int &diffuseChannel);

    RwCommError     ExtractDX9ShaderLightmapData(Mtl *pMtl,
                                               RwCommObject *pExpMaterial,
                                               PBBitmap *&pLightmapBitmap,
                                               PBBitmap *&pDiffuseBitmap,
                                               int &lightmapChannel,
                                               int &diffuseChannel);
protected:

    RwMaxScene      *m_pScene;

private:

	RwMap<Class_ID, RwCommObjCreator *>   m_mCreators;
	RwMap<SClass_ID, RwCommObjCreator *>  m_mBaseCreators;

};

#endif // !defined(AFX_RWMAXBUILDER_H__C4007D85_C05A_4522_815D_A38A3830EFCD__INCLUDED_)
