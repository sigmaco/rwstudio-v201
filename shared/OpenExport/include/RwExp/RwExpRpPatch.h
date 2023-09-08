// RwExpRpPatch.h: interface for the RwExpRpPatch class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPRPPATCH_H__DB3C0CF4_3A63_4D8A_910E_D9501DDE9090__INCLUDED_)
#define AFX_RWEXPRPPATCH_H__DB3C0CF4_3A63_4D8A_910E_D9501DDE9090__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "rtbezpat.h"

#include "RwExpRpAtomic.h"
#include "RwCommVertex.h"
#include "RwExpVertexCollection.h"

class RwCommPatchMesh;

class RWEXP_API RwExpRpPatchPipeline: public RwExpVertexPipeline
{
public:
    RwExpRpPatchPipeline();
    virtual ~RwExpRpPatchPipeline() {}
};

class RwExpExportOptions;

class RwCommPatchMesh;
class RwCommTransform;

const RwCommType RwExpRpPatchType("RwExpRpPatch");

/**
*  \ingroup exportobject
*
*   The RwExpRpPatch class is the class responsible for exporting RpPatchMeshes from RwCommPatchMeshes.
*   The export process of the patch mesh can be divided into the following phases:
*   \li First the vertex collection is initialized and filled with all of the patch's vertices.
*   \li Next, the RpPatch is created using all of the vertex collection statistics, including the number of quad and tri patches.
*   \li The vertices are pulled out of the vertex collection and filled in the RpPatch.
*   \li The primitives are pulled out of the vertex collection and inserted as tri or quad patches with their appropriate RpMaterials.
*   \li An RpAtomic is created and attached to the RpPatch.
*   \li The atomic is attached to its appropriate frame.
*   \li The atomic is added to the clump.
*
*   The RwExpRpPatch has a static RwExpVertexPipeline member. Users can create their own vertex filters
*   and add them to the vertex pipeline.
*   Here’s a list of all the default vertex filters constructed with the RwExpRpPatch pipeline (in their original
*   order within the vertex pipeline), along with a short
*   description of their functionality:

<TABLE BORDER CELLSPACING=2 BORDERCOLOR="#000000" CELLPADDING=7 WIDTH=548>
<TR>
    <TD WIDTH="40%" VALIGN="MIDDLE" BGCOLOR="#ffffff">
    <B><P>VERTEX FILTER</B></TD>
    <TD WIDTH="60%" VALIGN="MIDDLE" BGCOLOR="#ffffff">
    <B><P>DESCRIPTION</B></TD>
</TR>
<TR>
    <TD WIDTH="40%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwExpFitPatchFilter</B></TD>
    <TD WIDTH="60%" VALIGN="TOP" HEIGHT=11>
    <P>Tries to fit tri patches of a higher order, and patches that have no normals.</TD>
</TR>
<TR>
    <TD WIDTH="40%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwExpVertexDataFilter</B></TD>
    <TD WIDTH="60%" VALIGN="TOP" HEIGHT=11>
    <P>Filters out vertex data fields, according to the export options.</TD>
</TR>
<TR>
    <TD WIDTH="40%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwExpTransformUVFilter</B></TD>
    <TD WIDTH="60%" VALIGN="TOP" HEIGHT=11>
    <P>Transforms the UV coordinates by their UV texture transformation and flips their v values.</TD>
</TR>
<TR>
    <TD WIDTH="40%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwExpWeldFilter</B></TD>
    <TD WIDTH="60%" VALIGN="TOP" HEIGHT=11>
    <P>Welds positions and UVs to the supplied vertex threshold.</TD>
</TR>
<TR>
    <TD WIDTH="40%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwExpCropFilter</B></TD>
    <TD WIDTH="60%" VALIGN="TOP" HEIGHT=11>
    <P>Applies cropping to UVs.</TD>
</TR>
<TR>
    <TD WIDTH="40%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwExpLimitUVFilter</B></TD>
    <TD WIDTH="60%" VALIGN="TOP" HEIGHT=11>
    <P>Applies UV limiting.</TD>
</TR>
<TR>
    <TD WIDTH="40%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwExpLightMapFilter</B></TD>
    <TD WIDTH="60%" VALIGN="TOP" HEIGHT=11>
    <P>Applies UV light map calculations.</TD>
</TR>
<TR>
    <TD WIDTH="40%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwExpSelfIllumFilter</B></TD>
    <TD WIDTH="60%" VALIGN="TOP" HEIGHT=11>
    <P>Bakes pre light self illumination information into vertex colors.</TD>
</TR>
<TR>
    <TD WIDTH="40%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwExpMarkPatchFilter</B></TD>
    <TD WIDTH="60%" VALIGN="TOP" HEIGHT=11>
    <P>Flags the tri patches (for specific PS2 implementations). </TD>
</TR>
<TR>
    <TD WIDTH="40%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwExpRemapFilter</B></TD>
    <TD WIDTH="60%" VALIGN="TOP" HEIGHT=11>
    <P>Re-maps the vertices, to share a maximum amount of boundary vertices within primitives. Also sorts the vertices by shader information. </TD>
</TR>
<TR>   
    <TD WIDTH="40%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwExpScaleFilter</B></TD>
    <TD WIDTH="60%" VALIGN="TOP" HEIGHT=11>
    <P>Scales the vertices by the transformation’s propagated scale value and the user defined global scaling value.</TD>
</TR>
<TR>
    <TD WIDTH="40%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwExpOffsetFilter</B></TD>
    <TD WIDTH="60%" VALIGN="TOP" HEIGHT=11>
    <P>Geometries that are not attached to their default <B>RwFrames</B>, like skinned geometries that reside in different hierarchies, are transformed to match their target <B>RwFrame</B>.</TD>
</TR>
</TABLE>

*   \see RwCommPatchMesh
*   \see RpPatch
*   \see RwExpVertexPipeline
*/

class RWEXP_API RwExpRpPatch : public RwExpRpAtomic 
{
public:
    RwExpRpPatch();
    virtual ~RwExpRpPatch();

    virtual RwCommType          GetType() const { return RwExpRpPatchType; }

    virtual RwCommError         Export(RwCommObject *pObj, RwCommObject *pParent,
                                    RwCommScene *pScene, const RwCommExportOptions *pExportOptions);
    /**
    *   Not implemented.
    *
    *   \param userDataColl the user data to be added
    *
    *   \returns RWCOMM_NOT_IMPLEMENTED
    */
    virtual RwCommError         AddUserData(RwCommUserDataCollection &userDataColl);

    // Get RwExpRpGeometry's vertex pipeline
    static RwExpVertexPipeline  &GetVertexPipeline() { return m_vertexPipeline; }

    RpPatchMesh                 *GetRpPatchMesh() const { return m_pRpPatchMesh; }

protected:
    RwCommError                 BuildRpPatchMesh(RwCommPatchMesh *pPatchMesh,
                                    const RwCommExportOptions *pExportOptions, bool bMirrored);

    // Overridables
    virtual RpPatchMesh         *CreateRpPatchMesh(RwCommPatchMesh *pPatchMesh,
                                    const RwCommExportOptions *pExportOptions, int nQuads, int nTris);

    virtual RwCommError         SetPatchVertices(RpPatchMesh *pRpPatchMesh, RwExpVertexCollection &collection, 
                                    RwCommPatchMesh *pPatchMesh, const RwCommExportOptions *pExportOptions); 

    virtual RwCommError         SetPatches(RpPatchMesh *pRpPatchMesh, RwExpVertexCollection &collection, 
                                    RwCommPatchMesh *pPatchMesh, const RwCommExportOptions *pExportOptions,
                                    bool bMirrored); 

    virtual RwCommError         Attach(RpAtomic *pAtomic, RwCommTransform *pTransform);


protected:
    void                        GetNumPatches(int &nQuads, int &nTris);
    void                        FlipPatch(RwVector<int> &vPatchIndices);
    bool                        PatchHasColoredMaterials();
    bool                        PatchHasMaterialFX();
    bool                        PatchHasNormalMap();

protected:

    RpPatchMesh                 *m_pRpPatchMesh;

    static RwExpRpPatchPipeline m_vertexPipeline;
};

#endif // !defined(AFX_RWEXPRPPATCH_H__DB3C0CF4_3A63_4D8A_910E_D9501DDE9090__INCLUDED_)
