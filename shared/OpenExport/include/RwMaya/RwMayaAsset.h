#if !defined(RW_ASSET_H)
#define RW_ASSET_H

/**
*  \ingroup mayascenegraph 
*
*  This is an RwCommAsset implementation class.
*  It is also a Maya dependecy node which is used to store asset related information
*  within the Maya scene (dependency node name is "RenderWareAsset", id is 0x00104D84).
*
*  
*   The node has the following attributes:
*  
<TABLE BORDER CELLSPACING=2 BORDERCOLOR="#000000" CELLPADDING=7 WIDTH=704>
<TR>
    <TD WIDTH="20%" VALIGN="MIDDLE" BGCOLOR="#ffffff">
    <B><P ALIGN="CENTER">Long Name</B></TD>
    <TD WIDTH="16%" VALIGN="MIDDLE" BGCOLOR="#ffffff">
    <B><P ALIGN="CENTER">Short Name</B></TD>
    <TD WIDTH="12%" VALIGN="TOP" BGCOLOR="#ffffff">
    <B><P>Type</B></TD>
    <TD WIDTH="52%" VALIGN="TOP" BGCOLOR="#ffffff">
    <B><P>Description</B></TD>
</TR>
<TR>
    <TD WIDTH="20%" VALIGN="TOP" HEIGHT=11>
    <P>Scope</TD>
    <TD WIDTH="16%" VALIGN="TOP" HEIGHT=11>
    <P>sco</TD>
    <TD WIDTH="12%" VALIGN="TOP" HEIGHT=11>
    <P>kShort</TD>
    <TD WIDTH="52%" VALIGN="TOP" HEIGHT=11>
    <P>Type of node (0 = asset, 1 = scene).</TD>
</TR>
<TR>
    <TD WIDTH="20%" VALIGN="TOP" HEIGHT=11>
    <P>Name</TD>
    <TD WIDTH="16%" VALIGN="TOP" HEIGHT=11>
    <P>nam</TD>
    <TD WIDTH="12%" VALIGN="TOP" HEIGHT=11>
    <P>kString</TD>
    <TD WIDTH="52%" VALIGN="TOP" HEIGHT=11>
    <P>Name of asset.</TD>
</TR>
<TR>
    <TD WIDTH="20%" VALIGN="TOP" HEIGHT=11>
    <P>Enable</TD>
    <TD WIDTH="16%" VALIGN="TOP" HEIGHT=11>
    <P>ena</TD>
    <TD WIDTH="12%" VALIGN="TOP" HEIGHT=11>
    <P>kShort</TD>
    <TD WIDTH="52%" VALIGN="TOP" HEIGHT=11>
    <P>Whether this asset is enabled.</TD>
</TR>
<TR>
    <TD WIDTH="20%" VALIGN="TOP" HEIGHT=11>
    <P>TemplateName</TD>
    <TD WIDTH="16%" VALIGN="TOP" HEIGHT=11>
    <P>tem</TD>
    <TD WIDTH="12%" VALIGN="TOP" HEIGHT=11>
    <P>kString</TD>
    <TD WIDTH="52%" VALIGN="TOP" HEIGHT=11>
    <P>The template being used by this asset.</TD>
</TR>
<TR>
    <TD WIDTH="20%" VALIGN="TOP" HEIGHT=11>
    <P>ExportPath</TD>
    <TD WIDTH="16%" VALIGN="TOP" HEIGHT=11>
    <P>epa</TD>
    <TD WIDTH="12%" VALIGN="TOP" HEIGHT=11>
    <P>kString</TD>
    <TD WIDTH="52%" VALIGN="TOP" HEIGHT=11>
    <P>The export path for this asset.</TD>
</TR>
<TR>
    <TD WIDTH="20%" VALIGN="TOP" HEIGHT=11>
    <P>Selection</TD>
    <TD WIDTH="16%" VALIGN="TOP" HEIGHT=11>
    <P>sel</TD>
    <TD WIDTH="12%" VALIGN="TOP" HEIGHT=11>
    <P>kString</TD>
    <TD WIDTH="52%" VALIGN="TOP" HEIGHT=11>
    <P>The names of the scenes nodes included in this asset.</TD>
</TR>
<TR>
    <TD WIDTH="20%" VALIGN="TOP" HEIGHT=11>
    <P>Parent</TD>
    <TD WIDTH="16%" VALIGN="TOP" HEIGHT=11>
    <P>par</TD>
    <TD WIDTH="12%" VALIGN="TOP" HEIGHT=11>
    <P>kString</TD>
    <TD WIDTH="52%" VALIGN="TOP" HEIGHT=11>
    <P>The names of the parent of this asset.</TD>
</TR>
<TR>
    <TD WIDTH="20%" VALIGN="TOP" HEIGHT=11>
    <P>Children</TD>
    <TD WIDTH="16%" VALIGN="TOP" HEIGHT=11>
    <P>chi</TD>
    <TD WIDTH="12%" VALIGN="TOP" HEIGHT=11>
    <P>kString</TD>
    <TD WIDTH="52%" VALIGN="TOP" HEIGHT=11>
    <P>The names of the children of this asset.</TD>
</TR>
<TR>
    <TD WIDTH="20%" VALIGN="TOP" HEIGHT=11>
    <P>Options</TD>
    <TD WIDTH="16%" VALIGN="TOP" HEIGHT=11>
    <P>opt</TD>
    <TD WIDTH="12%" VALIGN="TOP" HEIGHT=11>
    <P>kString</TD>
    <TD WIDTH="52%" VALIGN="TOP" HEIGHT=11>
    <P>A semicolon delimited list of options.</TD>
</TR>
<TR>
    <TD WIDTH="20%" VALIGN="TOP" HEIGHT=11>
    <P>GUID</TD>
    <TD WIDTH="16%" VALIGN="TOP" HEIGHT=11>
    <P>gui</TD>
    <TD WIDTH="12%" VALIGN="TOP" HEIGHT=11>
    <P>kString</TD>
    <TD WIDTH="52%" VALIGN="TOP" HEIGHT=11>
    <P>A string representing the GUID assigned to this asset.</TD>
</TR>
</TABLE>
*
*   \see RwCommAsset
*/
class RWMAYA_API RwMayaAsset : public MPxNode, public RwCommAsset
{
public:

    RwMayaAsset() {};
    virtual ~RwMayaAsset() {};
    
    static  void        *creator();
    static  MStatus     initialize();

    virtual RwCommError SetAssetScope(const AssetScope aScope);
    virtual RwCommError GetAssetScope(AssetScope &aScope) const;

    virtual RwCommError SetName(const RwWString &name);
    virtual RwCommError GetName(RwWString &name) const;

    virtual RwCommError SetEnable(const bool bEnable);
    virtual RwCommError GetEnable(bool &bEnable) const;

    virtual RwCommError SetAssetTemplateName(const RwWString &templateName);
    virtual RwCommError GetAssetTemplateName(RwWString &templateName) const;

    virtual RwCommError SetGlobalExportPath(const RwWString &exportPath);
    virtual RwCommError GetGlobalExportPath(RwWString &exportPath) const;

    virtual RwCommError SetCustomExportFile(const RwWString &exportFile);
    virtual RwCommError GetCustomExportFile(RwWString &exportFile) const;

    virtual RwCommError SetGUID(const RwWString &guid);
    virtual RwCommError GetGUID(RwWString &guid) const;

    virtual RwCommError SetSelection(const RwCommScene *pScene, const RwVector<RwCommObject *> &vSelection);
    virtual RwCommError GetSelection(RwCommScene *pScene, RwVector<RwCommObject *> &vSelection) const;

    virtual RwCommError GetSelectionNames(RwVector<RwPair<RwWString, int> > &sNames) const;

    virtual RwCommError SetOptions(const RwCommExportOptions &options);
    virtual RwCommError GetOptions(RwCommExportOptions &options) const;

    virtual RwCommError SaveSelection();
    virtual RwCommError LoadSelection() const;
    virtual RwCommError LoadSelection(const RwVector<int> &SelectedAssest) const;

    virtual RwCommError AddToSelection();
    virtual RwCommError RemoveFromSelection(int Index);
    virtual RwCommError RemoveFromSelection(const RwVector<int> &Indices);
    virtual RwCommError ClearSelection();
    
    virtual RwCommError SetParent(RwCommAsset *pParent);
	virtual RwCommAsset *GetParent() const;

   	virtual RwCommError AddChild(RwCommAsset *pChild);
   	virtual RwCommError RemoveChild(int i);
    virtual int         GetNumChildren() const;
	virtual RwCommAsset *GetChild(int i) const;

    virtual RwCommError AddSelection(const RwCommAsset &other);

	virtual RwCommError SetDescription(const RwWString& description) { return RWCOMM_NOT_IMPLEMENTED; }
	virtual RwCommError GetDescription(RwWString &description) const { return RWCOMM_NOT_IMPLEMENTED; }

    const RwCommError   GetSelectionList(const RwWString &sAttribute, MSelectionList &selList) const;
    RwCommError         SetSelectionList(const RwWString &sAttribute, MSelectionList &selList);

    static  MTypeId     id;

private:
    static  MObject     iScope; // 0 = asset, 1 = scene
    static  MObject     sName;
    static  MObject     iEnable;
    static  MObject     sTemplateName;
    static  MObject     sExportPath;
    static  MObject     sCustomExportFile;
    static  MObject     sSelection;
    static  MObject     sParent;
    static  MObject     sChildren;
    static  MObject     sOptions;
    static  MObject     sGUID;
};

/**
*  \ingroup mayabuilder 
*
*  This class is responsible for creating an RwMayaAsset object.
*
*   \see RwMayaAsset
*   \see RwMayaObjCreator
*/
class RWMAYA_API RwMayaAssetCreator : public RwMayaObjCreator
{
public:
	RwMayaAssetCreator();
	virtual ~RwMayaAssetCreator();

	virtual RwCommObject    *Create(const RwCommCreationData *pCreationData,
								const RwCommExportOptions *pExportOptions) const;

	virtual void            Init(RwCommObject *pObject,
						        const RwCommCreationData *pCreationData,
						        const RwCommExportOptions *pExportOptions) const;

	virtual void Attach(RwCommObject* pObject, RwCommObject* pParent, RwCommScene* pScene,
                        const RwCommCreationData* pCreationData) const;
	virtual void            Traverse(RwCommBuilder &rBuilder, RwCommCreationData *pCreationData,
                                RwCommObject *pParent) const;
};

#endif // !defined(RW_ASSET_H)
