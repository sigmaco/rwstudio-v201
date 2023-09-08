#ifndef RWMAYABLINDDATA_H
#define RWMAYABLINDDATA_H

#include "RwCommError.h"
#include "RwCommUserData.h"

#include "RwMaya.h"

class RwMayaScene;

// =============================================================================
// --------------------------  RwMayaBlindData CLASS  --------------------------
// =============================================================================

/**
*  \ingroup mayascenegraph 
*
*  This class contains all Maya blind data extraction function. It is used by the various
*  RwMaya scene graph classes that implement RwCommObject::GetUserData, RwCommGeometry::GetVertexUserData
*  or RwCommGeometry::GetPrimitiveUserData.
*
*   \see RwCommObject
*   \see RwCommGeometry
*/
class RWMAYA_API RwMayaBlindData
{
public:
    RwMayaBlindData();
    virtual ~RwMayaBlindData();

protected:
    /**
     *  Extracts user data from a Maya object. Object, face and vertex blind data are supported.
     *  For mesh vertices and faces AddMayaCompBlindData is used to extract the blind data. For
     *  all other types AddMayaNodeBlindData is used.
     * 
     *  \param mObject the Maya MObject to extract blind data from.
     *  \param userDataColl the RwCommUserDataCollection in which the data is to be stored.
     *
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     *
     */
    virtual RwCommError     GetUserData(MObject mObject, RwCommUserDataCollection &userDataColl);

    /**
     *  Extracts face or vertex blind data from a Maya object. Used by GetUserData()
     * 
     *  \param mObject the Maya MObject to extract blind data from
     *  \param userDataColl the RwCommUserDataCollection in which the data is to be stored.
     *  \param dataComponent the type of blind data to be extracted.
     *
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     *
     */
    virtual RwCommError     AddMayaCompBlindData(MObject mObject,
                                RwCommUserDataCollection &userDataColl, MFn::Type dataComponent);

    /**
     *  Extracts object blind data from a Maya object. Used by GetUserData()
     * 
     *  \param mObject the Maya MObject to extract blind data from
     *  \param userDataColl the RwCommUserDataCollection in which the data is to be stored.
     *  \param type the type of data to store in the RwCommUserDataCollection.
     *
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     *
     */
    virtual RwCommError     AddMayaNodeBlindData(MObject mObject,
                                RwCommUserDataCollection &userDataColl,
                                RwCommUserDataCollection::Type type);

    RwMayaScene             *GetMayaScene();

    /**
     *  This function is used to extract the names and types of all attributes on a Maya object.
     *  Maya types are mapped to RwComm types as follows:
     * 
     * <TABLE BORDER CELLSPACING=2 BORDERCOLOR="#000000" CELLPADDING=7 WIDTH=562>
     * <TR>
     *    <TD WIDTH="43%" VALIGN="MIDDLE" BGCOLOR="#ffffff">
     *     <B><P ALIGN="CENTER">Maya Type;</B></TD>
     *    <TD WIDTH="57%" VALIGN="MIDDLE" BGCOLOR="#ffffff">
     *    <B><P ALIGN="CENTER">RwComm Type</B></TD>
     * </TR>
     * <TR>
     *    <TD WIDTH="43%" VALIGN="TOP" HEIGHT=11>
     *    <P>MFnNumericData::kBoolean</TD>
     *    <TD WIDTH="57%" VALIGN="TOP" HEIGHT=11>
     *    <P>RwCommAnimatable::INT</TD>
     * </TR>
     * <TR>
     *    <TD WIDTH="43%" VALIGN="TOP" HEIGHT=11>
     *    <P>MFnNumericData::kByte</TD>
     *    <TD WIDTH="57%" VALIGN="TOP" HEIGHT=11>
     *    <P>RwCommAnimatable::INT</TD>
     * </TR>
     * <TR>
     *    <TD WIDTH="43%" VALIGN="TOP" HEIGHT=11>
     *    <P>MFnNumericData::kShort</TD>
     *    <TD WIDTH="57%" VALIGN="TOP" HEIGHT=11>
     *    <P>RwCommAnimatable::INT</TD>
     * </TR>
     * <TR>
     *    <TD WIDTH="43%" VALIGN="TOP" HEIGHT=11>
     *    <P>MFnNumericData::kFloat</TD>
     *    <TD WIDTH="57%" VALIGN="TOP" HEIGHT=11>
     *    <P>RwCommAnimatable::FLOAT</TD>
     * </TR>
     * <TR>
     *    <TD WIDTH="43%" VALIGN="TOP" HEIGHT=11>
     *    <P>MFnNumericData::kDouble</TD>
     *    <TD WIDTH="57%" VALIGN="TOP" HEIGHT=11>
     *    <P>RwCommAnimatable::FLOAT</TD>
     * </TR>
     * <TR>
     *    <TD WIDTH="43%" VALIGN="TOP" HEIGHT=11>
     *    <P>MFnData::kIntArray</TD>
     *    <TD WIDTH="57%" VALIGN="TOP" HEIGHT=11>
     *    <P>RwCommAnimatable::INT</TD>
     * </TR>
     * <TR>
     *    <TD WIDTH="43%" VALIGN="TOP" HEIGHT=11>
     *    <P>MFnData::kDoubleArray</TD>
     *    <TD WIDTH="57%" VALIGN="TOP" HEIGHT=11>
     *    <P>RwCommAnimatable::FLOAT</TD>
     * </TR>
     * <TR>
     *    <TD WIDTH="43%" VALIGN="TOP" HEIGHT=11>
     *    <P>MFnData::kString</TD>
     *    <TD WIDTH="57%" VALIGN="TOP" HEIGHT=11>
     *    <P>RwCommAnimatable::STRING</TD>
     * </TR>
     * <TR>
     *    <TD WIDTH="43%" VALIGN="TOP" HEIGHT=11>
     *    <P>MFnData::kStringArray</TD>
     *    <TD WIDTH="57%" VALIGN="TOP" HEIGHT=11>
     *    <P>RwCommAnimatable::STRING</TD>
     * </TR>
     * </TABLE>
     * 
     *  \param object the Maya MObject to extract attributes from.
     *  \param vNames the names of all attributes found.
     *  \param vAttributeTypes the types of all attributes found.
     *
     *  \return true if successful, false if an error occurs.
     *
     */
    bool    GetObjectAttributeNames(MObject object, RwVector<RwWString> &vNames,
                RwVector<RwCommAnimatable::AttributeType> &vAttributeTypes);
};

#endif // RWMAYABLINDDATA_H
