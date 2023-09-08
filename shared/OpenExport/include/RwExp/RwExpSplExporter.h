
#ifndef RWEXPSPLEXPORTER_H
#define RWEXPSPLEXPORTER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwExp.h"
#include "RwExpExporter.h"
#include "RwExpSceneTraverse.h"

// =============================================================================
// -------------------------  RwExpSplExporter CLASS  --------------------------
// =============================================================================

class RwCommAnimatable;
class RwCommTransform;

const RwCommType RwExpSplExporterType("RwExpSplExporter");

/**

   \ingroup exporters

    This is the class responsible for exporting spline curves.
    The class is derived from RwExpExporter class. It is constructed with the following default exporter objects 
    (RwCommExportObject), assigned to the following RwCommObjects :


<TABLE BORDER CELLSPACING=2 BORDERCOLOR="#000000" CELLPADDING=7 WIDTH=406>
<TR>
    <TD WIDTH="47%" VALIGN="MIDDLE" BGCOLOR="#ffffff">
    <B><P>RWCOMMOBJECT </B></TD>
    <TD WIDTH="53%" VALIGN="MIDDLE" BGCOLOR="#ffffff">
    <B><P>RWCOMMEXPORTOBJECT</B></TD>
</TR>
<TR>
    <TD WIDTH="47%" VALIGN="TOP" HEIGHT=11>
    <P>RwCommSpline</TD>
    <TD WIDTH="53%" VALIGN="TOP" HEIGHT=11>
    <P>RwExpRpSpline</TD>
</TR>
</TABLE>
*
*
*   In addition, the RwExpAnmExporter is constructed with the following default traverse actions:
*
<TABLE BORDER CELLSPACING=2 BORDERCOLOR="#000000" CELLPADDING=7 WIDTH=548>
<TR>
    <TD WIDTH="45%" VALIGN="MIDDLE" BGCOLOR="#ffffff">
    <B><P>Traverse action</B></TD>
    <TD WIDTH="55%" VALIGN="MIDDLE" BGCOLOR="#ffffff">
    <B><P>Description</B></TD>
</TR>
<TR><TD WIDTH="45%" VALIGN="TOP" HEIGHT=11>
    <P>RwExpInitAnimatable</TD>
    <TD WIDTH="55%" VALIGN="TOP" HEIGHT=11>
    <P>Goes over all animatables in the traverse list and initializes their animation frame to the user supplied frame value. </TD>
</TR>
<TR>
    <TD WIDTH="45%" VALIGN="TOP" HEIGHT=11>
    <P>RwExpExportAction</TD>
    <TD WIDTH="55%" VALIGN="TOP" HEIGHT=11>
    <P>Performs the actual export action.</TD>
</TR>
</TABLE>
*/


class RWEXP_API RwExpSplExporter  : public RwExpExporter, public RwExpGraphicsSceneTraverse
{
public:
    RwExpSplExporter();
    virtual ~RwExpSplExporter();

    //RwCommObject method
    virtual RwCommType	GetType() const { return RwExpSplExporterType; }

protected:

    // RwExpExporter methods.
    virtual RwCommError     PreExport(RwCommScene *pScene, const RwVector<RwCommObject *> &vSelection); 
    virtual RwCommError     PostExport(RwCommScene *pScene, const RwVector<RwCommObject *> &vSelection);
    virtual RwCommError     BuildTraverseList(RwVector<RwExpTraverseObject> &vTraverseList, RwCommScene *pScene);

    virtual RwCommError     TraverseTransforms(RwCommTransform *pTransform, RwCommTransform *pParent,
                                               RwVector<RwExpTraverseObject> &vTraverseList,
                                               const RwCommExportOptions *pOptions);
    virtual RwCommError     TraverseShape(RwCommShape *pShape,
                                          RwVector<RwExpTraverseObject> &vTraverseList,
                                          const RwCommExportOptions *pOptions);
protected:
    
    RwCommTransform         *m_pSelectedTransform;
    RwCommShape             *m_pSplineShape;
};

#endif // RWEXPSPLEXPORTER_H
