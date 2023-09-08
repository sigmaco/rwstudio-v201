// RwExpDffExporter.h: interface for the RwExpDffExporter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPDFFEXPORTER_H__A6846689_4ADB_407B_9011_EF6BEBB691A0__INCLUDED_)
#define AFX_RWEXPDFFEXPORTER_H__A6846689_4ADB_407B_9011_EF6BEBB691A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwExp.h"
#include "RwExpExporter.h"
#include "RwExpExportOptions.h"
#include "RwExpSceneTraverse.h"


#include "RwCommUserData.h"

class RwCommAnimatable;
class RwCommModifiable;
class RwCommGeometry;
class RwCommTransform;

const RwCommType RwExpDffExporterType("RwExpDffExporter");

/**

   \ingroup exporters

    This is the class responsible for exporting RpClumps. The class is derived from RwExpExporter.
    It is constructed with the following default exporter objects (RwCommExportObject),
     assigned to the following RwCommObjects :


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
    <P>RwExpFrame</TD>
</TR>
<TR>
    <TD WIDTH="47%" VALIGN="TOP" HEIGHT=11>
    <P>RwCommMesh</TD>
    <TD WIDTH="53%" VALIGN="TOP" HEIGHT=11>
    <P>RwExpRpGeometry</TD>
</TR>
<TR>
    <TD WIDTH="47%" VALIGN="TOP" HEIGHT=11>
    <P>RwCommPatchMesh</TD>
    <TD WIDTH="53%" VALIGN="TOP" HEIGHT=11>
    <P>RwExpRpPatch</TD>
</TR>
<TR>
    <TD WIDTH="47%" VALIGN="TOP" HEIGHT=11>
    <P>RwCommShader</TD>
    <TD WIDTH="53%" VALIGN="TOP" HEIGHT=11>
    <P>RwExpRpMaterial</TD>
</TR>
<TR>
    <TD WIDTH="47%" VALIGN="TOP" HEIGHT=11>
    <P>RwCommTexture</TD>
    <TD WIDTH="53%" VALIGN="TOP" HEIGHT=11>
    <P>RwExpRwTexture</TD>
</TR>
<TR>
    <TD WIDTH="47%" VALIGN="TOP" HEIGHT=11>
    <P>RwCommBlendShape</TD>
    <TD WIDTH="53%" VALIGN="TOP" HEIGHT=11>
    <P>RwExpRpDMorph</TD>
</TR>
<TR>
    <TD WIDTH="47%" VALIGN="TOP" HEIGHT=11>
    <P>RwCommSkin</TD>
    <TD WIDTH="53%" VALIGN="TOP" HEIGHT=11>
    <P>RwExpRpSkin</TD>
</TR>
<TR>
    <TD WIDTH="47%" VALIGN="TOP" HEIGHT=11>
    <P>RwCommCamera</TD>
    <TD WIDTH="53%" VALIGN="TOP" HEIGHT=11>
    <P>RwExpRwCamera</TD>
</TR>
<TR>
    <TD WIDTH="47%" VALIGN="TOP" HEIGHT=11>
    <P>RwCommLight</TD>
    <TD WIDTH="53%" VALIGN="TOP" HEIGHT=11>
    <P>RwExpRpLight</TD>
</TR>
</TABLE>
*
*
*   In addition, the RwExpDffExporter is constructed with the following default traverse actions:
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
    <P>RwExpOptimizeShaders</B></TD>
    <TD WIDTH="55%" VALIGN="TOP" HEIGHT=11>
    <P>Removes redundant shaders from the traverse list.</TD>
</TR>
<TR>
    <TD WIDTH="45%" VALIGN="TOP" HEIGHT=11>
    <P>RwExpOptimizeHierarchy</B></TD>
    <TD WIDTH="55%" VALIGN="TOP" HEIGHT=11>
    <P>Optimizes the hierarchy. </TD>
</TR>
<TR>
    <TD WIDTH="45%" VALIGN="TOP" HEIGHT=11>
    <P>RwExpSetPolygonLimitAction</B></TD>
    <TD WIDTH="55%" VALIGN="TOP" HEIGHT=11>
    <P>Counts number of primitives per geometry, for progress bar use.</TD>
</TR>
<TR>
    <TD WIDTH="45%" VALIGN="TOP" HEIGHT=11>
    <P>RwExpHAnimHierarchy</B></TD>
    <TD WIDTH="55%" VALIGN="TOP" HEIGHT=11>
    <P>Builds the RpHAnimHierarchy</B>.</TD>
</TR>
<TR>
    <TD WIDTH="45%" VALIGN="TOP" HEIGHT=11>
    <P>RwExpMergeGeomAction</B></TD>
    <TD WIDTH="55%" VALIGN="TOP" HEIGHT=11>
    <P>Checks if geometries can be merged within the traverse list.</TD>
</TR>
<TR>
    <TD WIDTH="45%" VALIGN="TOP" HEIGHT=11>
    <P>RwExpCalcBBoxAction</B></TD>
    <TD WIDTH="55%" VALIGN="TOP" HEIGHT=11>
    <P>Calculates the traverse list’s bounding box</TD>
</TR>
<TR>
    <TD WIDTH="45%" VALIGN="TOP" HEIGHT=11>
    <P>RwExpExportAction</B></TD>
    <TD WIDTH="55%" VALIGN="TOP" HEIGHT=11>
    <P>Performs the actual export action</TD>
</TR>
<TR>
    <TD WIDTH="45%" VALIGN="TOP" HEIGHT=11>
    <P>RwExpCheckNodesAction</TD>
    <TD WIDTH="55%" VALIGN="TOP" HEIGHT=11>
    <P>Performs per platform object checks.</TD>
</TR>
</TABLE>
*/


class RWEXP_API RwExpDffExporter : public RwExpExporter, public RwExpGraphicsSceneTraverse
{
public:
    RwExpDffExporter();
    virtual ~RwExpDffExporter();

    //RwCommObject method
    virtual RwCommType  GetType() const { return RwExpDffExporterType; }

    /** \returns the generated RpClump*/
    RpClump                     *GetRpClump() const { return m_pClump; }

    /** Gets the root transformation of the hierarchy, the RwExpDffExporter is exporting.*/
    RwCommTransform             *GetRootTransform() const { return m_pRootTransform; }
    RwCommUserDataCollection&   GetSkinUserDataCollection() { return m_skinUserDataColl; }

protected:

    virtual RwCommError PreExport(RwCommScene *pScene, const RwVector<RwCommObject *> &vSelection);
    virtual RwCommError PostExport(RwCommScene *pScene, const RwVector<RwCommObject *> &vSelection);
    
    // Build the traverse list
    virtual RwCommError BuildTraverseList(RwVector<RwExpTraverseObject> &vTraverseList, RwCommScene *pScene);

    virtual void PostAction(RwExpTraverseAction *pAction);

    virtual RwCommError InsertRootClumpFrame(RwCommScene *pScene);
    virtual RwCommError SetRootInWorldSpace(RwCommTransform *rootTransform, RwCommScene *pScene);

    virtual RwCommError SplitSkins();
    static RpAtomic     *SplitAtomicSkin(RpAtomic *atomic, void *pData);
    static RpAtomic     *MoveSkinsToRoot(RpAtomic *atomic, void *pData);

    virtual RwCommError OverridePipes();
    
    /** This function is responsible for performing any ADC conversion requested. Based on the
    *   value of the ADCPROCESSING export option it uses the RpADC RenderWare plugin to perform
    *   ADC processing on all RpGeometrys in the export clump.
    *   \param world the world to be converted
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise
    */
    virtual RwCommError     AdcConvert();
    static RpAtomic         *AtomicAdcConvert(RpAtomic *atomic, void *data);

protected:

    RpClump                     *m_pClump;
    RwCommTransform             *m_pRootTransform;
    int                         m_iGeometryCount;
    RwCommUserDataCollection    m_skinUserDataColl;
};

#endif // !defined(AFX_RWEXPDFFEXPORTER_H__A6846689_4ADB_407B_9011_EF6BEBB691A0__INCLUDED_)
