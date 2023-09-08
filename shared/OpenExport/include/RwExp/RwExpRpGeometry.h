// RwExpRpGeometry.h: interface for the RwExpRpGeometry class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPRPGEOMETRY_H__06A58F98_318B_4E4B_85F1_77A115D63544__INCLUDED_)
#define AFX_RWEXPRPGEOMETRY_H__06A58F98_318B_4E4B_85F1_77A115D63544__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwCommExportObject.h"
#include "RwCommUserData.h"
#include "RwCommVertex.h"

#include "RwExpVertexPipeline.h"
#include "RwExpRpAtomic.h"

class RwCommMesh;
class RwCommTransform;
class RwExpExportOptions;

class RWEXP_API RwExpRpGeometryPipeline: public RwExpVertexPipeline
{
public:
    RwExpRpGeometryPipeline();
    virtual ~RwExpRpGeometryPipeline() {}
};

const RwCommType RwExpRpGeometryType("RwExpRpGeometry");

/**
*  \ingroup exportobject
*
*   This is the class responsible for exporting RpGeometry objects. It is mapped to the RwCommMesh object.
*   The entire creation process of the RpGeometry can roughly be divided into a couple of phases:
*   \li First the vertex collection is initialized and filled with all of the mesh's vertices.
*   \li Next the RpGeometry is created using all of the vertex collection statistics.
*   \li The primitives are pulled out of the vertex collection and inserted as triangles with their appropriate RpMaterials.
*   \li The vertices are pulled out of the vertex collection and filled in the RpGeometry.
*   \li If the morph option is checked, morph targets are built for the RpGeometry.
*   \li An RpAtomic is created and attached to the RpGeometry.
*   \li The atomic is attached to its appropriate frame.
*   \li The atomic is added to the clump.
*
*   The RwExpRpGeometry has a static RwExpVertexPipeline member. Users can create their own vertex filters
*   and add them to the vertex pipeline.
*   Here’s a list of all the default vertex filters constructed with the RwExpRpGeometry pipeline (in their original
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
    <P>Bakes prelight self illumination information into vertex colors.</TD>
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
<TR>
    <TD WIDTH="40%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwExpSortPrimFilter</B></TD>
    <TD WIDTH="60%" VALIGN="TOP" HEIGHT=11>
    <P> Sorts the primitives by shader and in 3d space.</TD>
</TR>
</TABLE>

*
*   \see RwCommMesh
*   \see RpGeometry
*   \see RwExpVertexPipeline
*/

class RWEXP_API RwExpRpGeometry : public RwExpRpAtomic
{
public:
    RwExpRpGeometry();
    virtual ~RwExpRpGeometry();

    virtual RwCommType          GetType() const { return RwExpRpGeometryType; }

    virtual RwCommError         Export(RwCommObject *pObj,
                                    RwCommObject *pParent,
                                    RwCommScene *pScene,
                                    const RwCommExportOptions *pExportOptions );

    /** Get the RwExpRpGeometry's vertex pipeline.*/
    static RwExpVertexPipeline &GetVertexPipeline() { return m_vertexPipeline; }

    /** Return the generated RpGeometry object.*/
    virtual RpGeometry          *GetRpGeometry() const { return m_pRpGeometry; }

    bool                        HasTooManyVerts() const { return m_tooManyVerts; }

protected:

    RwCommError                 BuildRpGeometry(RwCommMesh *pMesh,
                                    const RwCommExportOptions *pExportOptions, bool bMirrored);
    // Overridables
    virtual RpGeometry          *CreateGeometry(RwCommMesh *pMesh, const RwCommExportOptions *pExportOptions);

    virtual RwCommError         SetGeometryTriangles(RpGeometry *pRpGeometry,
                                    RwExpVertexCollection &collection, RwCommMesh *pMesh,
                                    const RwCommExportOptions *pExportOptions, bool bMirrored);

    virtual RwCommError         SetGeometryVertices(RpGeometry *geometry,
                                    RwExpVertexCollection &collection, RwCommMesh *pMesh,
                                    const RwCommExportOptions *pExportOptions);

    virtual RwCommError         BuildMorphTargets(RpGeometry *geometry, RwCommMesh *pMesh,
                                    RwExpVertexCollection &collection,
                                    const RwCommExportOptions *pExportOptions);

    virtual RwCommError         Attach(RpAtomic *pAtomic, RwCommTransform *pTransform);

    virtual void                SetTriStripMethod(const RwCommExportOptions *pExportOptions);

    /** This function is responsible for converting native lightmap data to RenderWare lightmap
    *   data using the RtLtMapConvert toolkit.
    *
    *   \param pObj the RwCommObject pointer for the mesh being exported
    *   \param pExpData the export options in use
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise
    */
    virtual RwCommError         ConvertNativeLightmaps(RwCommObject *pObj, const RwCommExportOptions *pExpData);

    virtual void                AddCollisionData(RwCommMesh *pMesh, RpGeometry *geometry,
                                    const RwCommExportOptions *pExportOptions);

    virtual RwCommError         AddObjectNameAsUserData(const RwWString &userDataEntry, RwCommMesh *pMesh);
    
    /**
    *   This function adds the passed user data onto the RpGeometry as RpUserData arrays.
    *   It iterates through the members of userDataColl checking their types. For members of
    *   type shapeUserDataType it extracts the data size using RwCommUserData::GetNumElements.
    *   For members of type primitiveUserDataType it uses RwCommUserDataCollection::m_numPolys and
    *   for vertexUserDataType it uses RwCommUserDataCollection::m_numVerts. Members of other
    *   types are ignored. For valid members it creates an RpUserData array on the geometry
    *   using RpGeometryAddUserDataArray and then calls RwExpCommUserDataToRpUserDataArray to
    *   fill in the data.
    *
    *   \param userDataColl the user data to be added
    *
    *   \returns RWCOMM_SUCCESS
    */
    virtual RwCommError         AddUserData(RwCommUserDataCollection &userDataColl);

    virtual RwCommError         AddToonData(RwCommMesh *pMesh, const RwCommExportOptions *pExportOptions); 

protected:
    void                        PrintStats(const RwString &objName);
    bool                        GeometryHasColoredMaterials();
    bool                        GeometryHasMaterialFX();
    bool                        GeometryHasNormalMap();
    bool                        EnableAttachedBlendShape(bool bEnable, RwCommMesh *pMesh, 
                                    RwVector<RwCommReal> &vWeights);

private:
    RpGeometry                      *m_pRpGeometry;
    static RwExpRpGeometryPipeline  m_vertexPipeline;
    RwCommUserDataCollection        m_mergedUserDataColl;
    RwVector<RwCommMesh*>             m_allMergedMeshes;
    bool                            m_tooManyVerts;
};


#endif // !defined(AFX_RWEXPRPGEOMETRY_H__06A58F98_318B_4E4B_85F1_77A115D63544__INCLUDED_)
