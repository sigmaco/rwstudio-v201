
#ifndef RWEXPBSPGEOMETRY_H
#define RWEXPBSPGEOMETRY_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwCommExportObject.h"
#include "RwCommVertex.h"
#include "RwExpVertexCollection.h"
#include "RwExpVertexFilter.h"

// =============================================================================
// ----------------------  RwExpBspGeometryPipeline CLASS  ---------------------
// =============================================================================

class RwExpBspGeometryPipeline: public RwExpVertexPipeline
{
public:
    RwExpBspGeometryPipeline();
    virtual ~RwExpBspGeometryPipeline();
};


// =============================================================================
// --------------------------  RwExpBspGeometry CLASS  -------------------------
// =============================================================================

/**
*  \ingroup exportobject
*
*   This is the class responsible for collecting all the vertices and primitives from an RwCommMesh, which will later on
*   be added to the RtWorldImport. The class is mapped to a RwCommMesh in the Bsp exporter.
*   
*   The RwExpBspGeometry class, like other geometry exporter classes, uses a vertex collection to store all of its extracted vertices.
*   Within the main Export() method, all of the RwCommMesh's vertices are extracted and inserted into the vertex
*   collection. Since the geometry exporters need to apply different optimizations on the vertices, the vertices are
*   pushed into a vertex pipeline, where they are modified, remapped and then, later on, stored in the vertex
*   collection. To access the RwExpBspGeometry vertex collection use the GetVertexCollection() method.
*   
*   The vertex pipeline is a static member of the RwExpBspGeometry class, shared by all instances of the
*   RwExpBspGeometry class. To add your own custom vertex filter to it, you do not need to refer to a specific
*   instance of the RwExpBspGeometry class, but just use the GetVertexPipeline() method, which is also a static
*   method. 
*
*   Unlike the RwExpRpGeometry exporter, the RwExpBspGeometry exporter extracts all the mesh's vertices in 
*   world space instead of object space. This is because the RtWorldImport requires all the vertices to be in 
*   world space. 
*
*   Here's a list of all the default vertex filters constructed with the RwExpBspGeometry pipeline,
*   along with a short description of their functionality:
*
<TABLE BORDER CELLSPACING=2 BORDERCOLOR="#000000" CELLPADDING=7 WIDTH=548>
<TR>
    <TD WIDTH="41%" VALIGN="MIDDLE" BGCOLOR="#ffffff">
    <B><P>VERTEX FILTER</B></TD>
    <TD WIDTH="59%" VALIGN="MIDDLE" BGCOLOR="#ffffff">
    <B><P>DESCRIPTION</B></TD>
</TR>
<TR>
    <TD WIDTH="40%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwExpVertexDataFilter</B></TD>
    <TD WIDTH="60%" VALIGN="TOP" HEIGHT=11>
    <P>Filters out vertex data fields, according to the export options.</TD>
</TR>
<TR>
    <TD WIDTH="41%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwExpTransformUVFilter</B></TD>
    <TD WIDTH="59%" VALIGN="TOP" HEIGHT=11>
    <P>Transforms the UV coordinates by their UV texture transformation and flips their v values.</TD>
</TR>
<TR>
    <TD WIDTH="41%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwExpWeldFilter</B></TD>
    <TD WIDTH="59%" VALIGN="TOP" HEIGHT=11>
    <P>Welds positions and UVs to the supplied vertex threshold.</TD>
</TR>
<TR>
    <TD WIDTH="41%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwExpCropFilter</B></TD>
    <TD WIDTH="59%" VALIGN="TOP" HEIGHT=11>
    <P>Applies cropping to UVs.</TD>
</TR>
<TR>
    <TD WIDTH="41%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwExpLimitUVFilter</B></TD>
    <TD WIDTH="59%" VALIGN="TOP" HEIGHT=11>
    <P>Applies UV limiting.</TD>
</TR>
<TR>
    <TD WIDTH="41%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwExpLightMapFilter</B></TD>
    <TD WIDTH="59%" VALIGN="TOP" HEIGHT=11>
    <P>Applies UV light map calculations.</TD>
</TR>
<TR>
    <TD WIDTH="41%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwExpSelfIllumFilter</B></TD>
    <TD WIDTH="59%" VALIGN="TOP" HEIGHT=11>
    <P>Bakes prelight self illumination information into vertex colors.</TD>
</TR>
<TR>
    <TD WIDTH="41%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwExpRemapFilter</B></TD>
    <TD WIDTH="59%" VALIGN="TOP" HEIGHT=11>
    <P>Re-maps the vertices, to share a maximum amount of boundary vertices within primitives. Also sorts the vertices by shader information. </TD>
</TR>
<TR>
    <TD WIDTH="41%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwExpTransformWorldSpaceFilter</B></TD>
    <TD WIDTH="59%" VALIGN="TOP" HEIGHT=11>
    <P>Transforms the final vertices by user specified scaling and adjusts the world space axis system.</TD>
</TR>
<TR>
    <TD WIDTH="40%" VALIGN="TOP" HEIGHT=11>
    <B><P>RwExpSortPrimFilter</B></TD>
    <TD WIDTH="60%" VALIGN="TOP" HEIGHT=11>
    <P> Sorts the primitives by shader and in 3d space.</TD>
</TR>
</TABLE>

*   \see RwCommMesh
*   \see RwExpVertexPipeline
*
*/

class RWEXP_API RwExpBspGeometry  : public RwCommExportObject
{
public:
    RwExpBspGeometry();
    virtual ~RwExpBspGeometry();

    virtual RwCommError             Export(RwCommObject *pObj,
                                        RwCommObject *pParent,
                                        RwCommScene *pScene,
                                        const RwCommExportOptions *pExportOptions);

    /**
    *   Not implemented.
    *
    *   \param userDataColl the user data to be added
    *
    *   \returns RWCOMM_NOT_IMPLEMENTED
    */
    virtual RwCommError             AddUserData(RwCommUserDataCollection &userDataColl);

    /** Get the RwExpBespGeometry vertex pipeline.*/
    static RwExpVertexPipeline      &GetVertexPipeline()        {return m_vertexPipeline;}

    /** Get the RwExpBespGeometry vertex collection.*/
    RwExpVertexCollection           &GetVertexCollection()      {return m_collection;}

    /** Get the number of triangles in the collection.*/
    int                             GetNumTriangles()           {return m_numTriangles;}

private:
    int                             m_numTriangles;
    RwExpVertexCollection           m_collection;
    static RwExpBspGeometryPipeline m_vertexPipeline;
};


#endif // RWEXPBSPGEOMETRY_H
