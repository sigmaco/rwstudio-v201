
#ifndef RWEXPANMEXPORTER_H
#define RWEXPANMEXPORTER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwExp.h"
#include "RwExpExporter.h"
#include "RwExpAnimation.h"
#include "RwExpTraverseAction.h"
#include "RwExpSceneTraverse.h"

// =============================================================================
// -------------------------  RwExpAnmExporter CLASS  --------------------------
// =============================================================================

class RwCommAnimatable;
class RwCommTransform;

const RwCommType RwExpAnmExporterType("RwExpAnmExporter");

/**

   \ingroup exporters

    This is the class responsible for exporting the RpHAnimAnimation structure.
    The class is derived from RwExpExporter.
    It is constructed with the following default exporter objects (RwCommExportObject),
    assigned to the following RwCommObjects :


<TABLE BORDER CELLSPACING=2 BORDERCOLOR="#000000" CELLPADDING=7 WIDTH=406>
<TR>
    <TD WIDTH="47%" VALIGN="MIDDLE" BGCOLOR="#ffffff">
    <B><P>RWCOMMOBJECT </B></TD>
    <TD WIDTH="53%" VALIGN="MIDDLE" BGCOLOR="#ffffff">
    <B><P>RWCOMMEXPORTOBJECT</B></TD>
</TR>
<TR>
    <TD WIDTH="47%" VALIGN="TOP" HEIGHT=11>
    <P>RwCommTransform</TD>
    <TD WIDTH="53%" VALIGN="TOP" HEIGHT=11>
    <P>RwExpMatrixAnimation</TD>
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
<TR>
    <TD WIDTH="45%" VALIGN="TOP" HEIGHT=11>
    <P>RwExpInitAnimatables</TD>
    <TD WIDTH="55%" VALIGN="TOP" HEIGHT=11>
    <P>Goes over all animatables in the traverse list and initializes their animation frame to the user supplied frame value. </TD>
</TR>
<TR>
    <TD WIDTH="45%" VALIGN="TOP" HEIGHT=11>
    <P>RwExpOptimizeHierarchy</TD>
    <TD WIDTH="55%" VALIGN="TOP" HEIGHT=11>
    <P>Optimizes the hierarchy. </TD>
</TR>
<TR>
    <TD WIDTH="45%" VALIGN="TOP" HEIGHT=11>
    <P>RwExpCalcBBoxAction</TD>
    <TD WIDTH="55%" VALIGN="TOP" HEIGHT=11>
    <P>Calculates the traverse list’s bounding box.</TD>
</TR>
<TR>
    <TD WIDTH="45%" VALIGN="TOP" HEIGHT=11>
    <P>RwExpExportAction</TD>
    <TD WIDTH="55%" VALIGN="TOP" HEIGHT=11>
    <P>Performs the actual export action.</TD>
</TR>
<TR>
    <TD WIDTH="45%" VALIGN="TOP" HEIGHT=11>
    <P>RwExpExtractKeyFrames</TD>
    <TD WIDTH="55%" VALIGN="TOP" HEIGHT=11>
    <P>Extracts the key frames from the RwCommAnimation</B> objects and sets them to the RwExpAnimation</B> objects.</TD>
</TR>
<TR>
    <TD WIDTH="45%" VALIGN="TOP" HEIGHT=11>
    <P>RwExpSampleAnimation</TD>
    <TD WIDTH="55%" VALIGN="TOP" HEIGHT=11>
    <P>Samples the RwCommTransform</B> objects for their matrices and sets them to the RwExpMatrixAnimation</B> objects.</TD>
</TR>
<TR>
    <TD WIDTH="45%" VALIGN="TOP" HEIGHT=11>
    <P>RwExpDynamicKeyFrameGen</TD>
    <TD WIDTH="55%" VALIGN="TOP" HEIGHT=11>
    <P>Creates extra key frames if the sampled key frame times are greater than the given threshold.</TD>
</TR>
<TR>
    <TD WIDTH="45%" VALIGN="TOP" HEIGHT=11>
    <P>RwExpBuildHAnimAnimation</TD>
    <TD WIDTH="55%" VALIGN="TOP" HEIGHT=11>
    <P>Builds the actual RpHAnimAnimation </B>object.</TD>
</TR>
<TR>
    <TD WIDTH="45%" VALIGN="TOP" HEIGHT=11>
    <P>RwExpCheckNodesAction</TD>
    <TD WIDTH="55%" VALIGN="TOP" HEIGHT=11>
    <P>Performs per platform object checks.</TD>
</TR>
</TABLE>
*/



class RWEXP_API RwExpAnmExporter  : public RwExpExporter, public RwExpGraphicsSceneTraverse
{
public:
    RwExpAnmExporter();
    virtual ~RwExpAnmExporter();

    //RwCommObject method
    virtual RwCommType	GetType() const { return RwExpAnmExporterType; }

    // RwExpAnmExporter methods
    bool                        IsInTheSubHierarchy(RwCommTransform *pTransform); 

    /** Gets the root transformation of the hierarchy, the RwExpAnmExporter is exporting.*/
    RwCommTransform             *GetRootTransform() const { return m_pRootTransform; }

protected:

    // RwExpExporter methods.
    virtual RwCommError         PreExport(RwCommScene *pScene, const RwVector<RwCommObject *> &vSelection); 
    virtual RwCommError         PostExport(RwCommScene *pScene, const RwVector<RwCommObject *> &vSelection);

    virtual RwCommError         BuildTraverseList(RwVector<RwExpTraverseObject> &vTraverseList, RwCommScene *pScene);

    /**
     *  Perform any requested compression on the animation. Can be overriden to perform custom compression.
     * 
     *  \param hAnim the animation to be compressed.
     *
     */
    virtual RwCommError         CompressAnimation(RtAnimAnimation *&hAnim);

protected:
    RwCommTransform             *m_selectedTransform;
    RwCommTransform             *m_pRootTransform;
};

#endif // RWEXPANMEXPORTER_H
