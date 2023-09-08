#if !defined(RW_MAYA_BUILDER_H)
#define RW_MAYA_BUILDER_H

#pragma warning(disable : 4786)
#pragma warning(disable : 4251)

#include "RwMaya.h"
#include "RwMayaScene.h"

#include "RwCommBuilder.h"
#include "RwCommError.h"
#include "RwCommObjCreator.h"


class RwMayaObjCreator;

class RWMAYA_API RwMayaIkHandle
{
public:
    MDagPath                handle;
    RwVector<MDagPath>  affectedNodes;
};


/**
*  \ingroup mayabuilder 
*
*   This is the RwMaya implementation of the RwCommBuilder.
*   The main Build() method traverses all the top level dag nodes in the Maya scene. At each dag path it calls
*   the Traverse() method. Traverse() in turn calls the BuildObject() method before recursing on any child dag
*   nodes. BuildObject() attempts find a creator function that may be used to build an RwCommObject for that
*   particular type of dependency node. Assuming a creator class is found it's Create(), Init(), Attach() and
*   Traverse() functions are called. The RwMayaObjCreator::Traverse() method is reponsible for traversing any
*   relevant dependency nodes attached to the dag node.
*
*   The BuildObject() method maps dependency nodes onto RwMayaObjCreator classes using two internal tables. The
*   first (the exact creator table) maps Maya dependency node type names (as returned by MFnDependencyNode::typeName())
*   onto RwMayaObjCreator pointers. The second (the compatible creator table) maps MFn::Type onto RwMayaObjCreator
*   pointers. When trying to find a creator the function first looks for an exact match to the type name in the exact
*   creator table and if this fails it tries to find a compatible creator using the MObject::hasFn() method.
*
*   The RwMayaBuilder class is constructed with a default set of creator objects. Users wishing to register
*   their own custom creator objects can do so by using RegisterExactCreator() and RegisterCompatibleCreator()
*   functions. Users can also unregister a specific Class_ID type by supplying NULL creators.
*
*
*   The following table represents the contents of the exact creator table:
* <TABLE BORDER CELLSPACING=2 BORDERCOLOR="#000000" CELLPADDING=7 WIDTH=622>
<TR>
    <TD WIDTH="32%" VALIGN="MIDDLE" BGCOLOR="#ffffff"></TD>
    <TD WIDTH="38%" VALIGN="MIDDLE" BGCOLOR="#ffffff">
    <B><P>RwCommObjCreator</B></TD>
    <TD WIDTH="30%" VALIGN="MIDDLE" BGCOLOR="#ffffff"></TD>
    <B><P>RwCommObject</B></TD>
</TR>
<TR>
    <TD WIDTH="32%" VALIGN="TOP" HEIGHT=11>
    <B><P>&quot;transform&quot;</B></TD>
    <TD WIDTH="38%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMayaTransformCreator</B></TD>
    <TD WIDTH="30%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMayaTransform</B></TD>
</TR>
<TR>
    <TD WIDTH="32%" VALIGN="TOP" HEIGHT=11>
    <B><P>&quot;joint&quot;</B></TD>
    <TD WIDTH="38%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMayaTransformCreator</B></TD>
    <TD WIDTH="30%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMayaTransform</B></TD>
</TR>
<TR>
    <TD WIDTH="32%" VALIGN="TOP" HEIGHT=11>
    <B><P>&quot;skinCluster&quot;</B></TD>
    <TD WIDTH="38%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMayaSkinCreator</B></TD>
    <TD WIDTH="30%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMayaSkin</B></TD>
</TR>
<TR>
    <TD WIDTH="32%" VALIGN="TOP" HEIGHT=11>
    <B><P>&quot;blendShape&quot;</B></TD>
    <TD WIDTH="38%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMayaBlendShapeCreator</B></TD>
    <TD WIDTH="30%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMayaBlendShape</B></TD>
</TR>
<TR>
    <TD WIDTH="32%" VALIGN="TOP" HEIGHT=11>
    <B><P>&quot;mesh&quot;</B></TD>
    <TD WIDTH="38%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMayaMeshCreator</B></TD>
    <TD WIDTH="30%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMayaMesh</B></TD>
</TR>
<TR>
    <TD WIDTH="32%" VALIGN="TOP" HEIGHT=11>
    <B><P>&quot;nurbsSurface&quot;</B></TD>
    <TD WIDTH="38%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMayaNurbsSurfaceCreator</B></TD>
    <TD WIDTH="30%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMayaNurbsSurface</B></TD>
</TR>
<TR>
    <TD WIDTH="32%" VALIGN="TOP" HEIGHT=11>
    <B><P>&quot;RenderWareShader&quot;</B></TD>
    <TD WIDTH="38%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMayaRwMaterialCreator</B></TD>
    <TD WIDTH="30%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMayaMaterial</B></TD>
</TR>
<TR>
    <TD WIDTH="32%" VALIGN="TOP" HEIGHT=11>
    <P>&quot;<B>RenderWareAsset</B>&quot;</TD>
    <TD WIDTH="38%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMayaAssetCreator</B></TD>
    <TD WIDTH="30%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMayaAsset</B></TD>
</TR>
</TABLE>
*
*
*   The following table represents the contents of the compatible creator table:
<TABLE BORDER CELLSPACING=2 BORDERCOLOR="#000000" CELLPADDING=7 WIDTH=624>
<TR>
    <TD WIDTH="31%" VALIGN="MIDDLE" BGCOLOR="#ffffff">
    <B><P ALIGN="CENTER">mfn::type</B></TD>
    <TD WIDTH="39%" VALIGN="MIDDLE" BGCOLOR="#ffffff">
    <B><P ALIGN="CENTER">RwCommObjCreator</B></TD>
    <TD WIDTH="29%" VALIGN="MIDDLE" BGCOLOR="#ffffff">
    <B><P>RwCommObject</B></TD>
</TR>
<TR>
    <TD WIDTH="31%" VALIGN="TOP" HEIGHT=11>
    <B><P>MFn::kGeometryFilt</B></TD>
    <TD WIDTH="39%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMayaMorphCreator</B></TD>
    <TD WIDTH="29%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwCommMorph</B></TD>
</TR>
<TR>
    <TD WIDTH="31%" VALIGN="TOP" HEIGHT=11>
    <B><P>MFn::kLambert</B></TD>
    <TD WIDTH="39%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMayaMaterialCreator</B></TD>
    <TD WIDTH="29%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMayaMaterial</B></TD>
</TR>
<TR>
    <TD WIDTH="31%" VALIGN="TOP" HEIGHT=11>
    <B><P>MFn::kFileTexture</B></TD>
    <TD WIDTH="39%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMayaTextureCreator</B></TD>
    <TD WIDTH="29%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMayaTexture</B></TD>
</TR>
<TR>
    <TD WIDTH="31%" VALIGN="TOP" HEIGHT=11>
    <B><P>MFn::kAnimCurve</B></TD>
    <TD WIDTH="39%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMayaAnimCurveCreator</B></TD>
    <TD WIDTH="29%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMayaAnimCurve</B></TD>
</TR>
<TR>
    <TD WIDTH="31%" VALIGN="TOP" HEIGHT=11>
    <B><P>MFn::kMotionPath</B></TD>
    <TD WIDTH="39%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMayaMotionPathCreator</B></TD>
    <TD WIDTH="29%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMayaMotionPath</B></TD>
</TR>
<TR>
    <TD WIDTH="31%" VALIGN="TOP" HEIGHT=11>
    <B><P>MFn::kLight</B></TD>
    <TD WIDTH="39%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMayaLightCreator</B></TD>
    <TD WIDTH="29%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMayaLight</B></TD>
</TR>
<TR>
    <TD WIDTH="31%" VALIGN="TOP" HEIGHT=11>
    <B><P>MFn::kCamera</B></TD>
    <TD WIDTH="39%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMayaCameraCreator</B></TD>
    <TD WIDTH="29%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMayaCamera</B></TD>
</TR>
<TR>
    <TD WIDTH="31%" VALIGN="TOP" HEIGHT=11>
    <B><P>MFn::kNurbsCurve</B></TD>
    <TD WIDTH="39%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMayaSplineCreator</B></TD>
    <TD WIDTH="29%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMayaSpline</B></TD>
</TR>
<TR>
    <TD WIDTH="31%" VALIGN="TOP" HEIGHT=11>
    <B><P>MFn::kNurbsSurface</B></TD>
    <TD WIDTH="39%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMayaNurbsSurfaceCreator</B></TD>
    <TD WIDTH="29%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwMayaNurbsSurface</B></TD>
</TR>
</TABLE>
*
*
*
*   \see RwCommBuilder
*   \see RwMaxCreationData
*
*/
class RWMAYA_API RwMayaBuilder : public RwCommBuilder 
{
public:
	RwMayaBuilder();
	virtual ~RwMayaBuilder();

    /**
     *  This is the main method that triggers the entire build proccess of the scene graph.
     * 
     *  \param pScene reference to an RwCommScene pointer that will recieve the RwMayaScene scene graph built.
     *
     */
	RwCommError         Build(RwCommScene *&pScene);

    /**
     *  Register an RwMayaObjCreator in the exact creator table
     * 
     *  \param type the name of the type to match
     *  \param pCreator pointer to the creator to be used to build.
     *
     */
    RwCommError         RegisterExactCreator(const RwWString type, RwMayaObjCreator *pCreator);

    /**
     *  Get the RwMayaObjCreator registered against a type in the exact creator table.
     * 
     *  \param type the name of the type to look up
     *  \returns a pointer to the creator if found, NULL otherwise.
     *
     */
    RwMayaObjCreator    *GetExactCreator(const RwWString type);

    /**
     *  Register an RwMayaObjCreator in the compatible creator table
     * 
     *  \param type the MFn::Type of the type to match
     *  \param pCreator pointer to the creator to be used to build.
     *
     */
    RwCommError         RegisterCompatibleCreator(const MFn::Type type, RwMayaObjCreator *pCreator);

    /**
     *  Get the RwMayaObjCreator registered against a type in the compatible creator table.
     * 
     *  \param type the MFn::Type of the type to look up
     *  \returns a pointer to the creator if found, NULL otherwise.
     *
     */
    RwMayaObjCreator    *GetCompatibleCreator(const MFn::Type type);

	bool                Traverse(MDagPath & dagPath, RwCommObject* pParent);

    /**
     *  This function is called by BuildObject() before attempting to find a creator for an MObject.
     * 
     *  \param mObject the Maya object which is about to be built.
     *
     */
	virtual bool        OnBuildObject(MObject mObject) const;

    /**
     *  The BuildObject() method maps dependency nodes onto RwMayaObjCreator classes using two tables. The
     *  first (the exact creator table) maps Maya dependency node type names (as returned by MFnDependencyNode::typeName())
     *  onto RwMayaObjCreator pointers. The second (the compatible creator table) maps MFn::Type onto RwMayaObjCreator
     *  pointers. When trying to find a creator the function first looks for an exact match to the type name in the exact
     *  creator table and if this fails it tries to find a compatible creator using the MObject::hasFn() method.
     * 
     *  \param mObject the Maya object to be built from
     *  \param pCreationData the RwMayaCreationData to use during building
     *  \param pParent the parent of the object being built
     *  \returns the object built if successful, NULL otherwise.
     *
     */
	RwCommObject        *BuildObject(MObject mObject,
                                RwCommCreationData *pCreationData,
                                RwCommObject *pParent);

    void                InitialiseExportOptions();
    void                ClearExportOptions();

    RwVector<RwMayaIkHandle> m_ikHandles;

protected:

    RwMayaScene                                 *m_pScene;
	RwMap<RwWString, RwMayaObjCreator *> m_mExactCreators;
	RwMap<MFn::Type, RwMayaObjCreator *>    m_mCompatibleCreators;
};

#endif // !defined(RW_MAYA_BUILDER_H)
