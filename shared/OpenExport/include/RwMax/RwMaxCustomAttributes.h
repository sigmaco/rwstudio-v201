#ifndef RWMAXCUSTOMATTRIBUTES_H
#define RWMAXCUSTOMATTRIBUTES_H

#include "RwCommError.h"
#include "RwCommUserData.h"

#include "RwMax.h"

// =============================================================================
// -----------------------  RwMaxCustomAttributes CLASS  -----------------------
// =============================================================================

/**
*  \ingroup maxhelper 
*
*   This is an RwMax helper class for handling custom attributes and user data.
*
*   \see RwCommMesh
*/
class RWMAX_API RwMaxCustomAttributes
{
public:
    RwMaxCustomAttributes();
    virtual ~RwMaxCustomAttributes();

    template <class T> bool GetCustomAttributeByName(Animatable *animObj, RwWString name,
                                                  T &value, TimeValue time, int index);


    virtual RwCommError     GetCustomAttributeNames(Animatable *animObj, RwVector<RwWString> &vAttributeNames,
                                RwVector<RwCommAnimatable::AttributeType> &vAttributeTypes);

protected:
    /**
    *   Extracts custom attributes from an 3ds max object. For materials and textures we'll
    *   get custom attribute container from the animatable and call ProcessCustAttribContainer().
    *   For cameras, lights and other shapes, we'll call AddNodeUserData().
    *
    *   \param animObj animatable to extract the custom attributes from
    *   \param userDataColl the RwCommUserDataCollection in which the data is to be stored.
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError     GetUserData(Animatable *animObj, RwCommUserDataCollection &userDataColl);
    /**
    *   Extract the data from the parameter block. This is called from the
    *   ProcessCustAttribContainer(). It will loop through the parameters. Check if the type
    *   parameter matches the type from specified in the function call, create an RwCommUserData
    *   object and add it to the collection.
    *
    *   \param paramblock parameter block to extract the data from.
    *   \param userDataColl the RwCommUserDataCollection in which the data is to be stored.
    *   \param dataType user data type
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError     ProcessCustomAttribParamBlock(IParamBlock2 *paramblock,
                                          RwCommUserDataCollection &userDataColl,
                                          RwCommUserDataCollection::Type dataType);
    /**
    *   Extract custom attributes from an 3ds max ICustAttribContainer. It is called from
    *   AddNodeUserData() and GetUserData() directly for materials and textures.
    *
    *   It will get the attributes with 3ds max's GetNumCustAttribs(), loop through them, and
    *   then loop through all parameter block on each attribute. Only parameter blocks which 
    *   have a parameter of type TYPE_STRING, name "TargetApp" and value "RenderWare", will be
    *   be processed. Parameter block should also have a parameter of type TYPE_STRING, name
    *   "DataType" and value which matches one of the types: "Vertex", "Face", "Object",
    *   "Transform", "Light", "Camera", "Texture" or "Material". If this is an our parameter
    *   block then call ProcessCustomAttribParamBlock() for it.
    *
    *   You should use max scripts given in TechnicalArtistGuide.pdf for assigning custom
    *   attributes in appropriate form.
    *
    *   \param custAttribContainer to extract the data from
    *   \param userDataColl the RwCommUserDataCollection in which the data is to be stored
    *   \param dataType user data type
    *
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError     ProcessCustAttribContainer(ICustAttribContainer *custAttribContainer,
                                                       RwCommUserDataCollection &userDataColl,
                                                       RwCommUserDataCollection::Type dataType);
    /**
    *   Extract the user data from an INode. This function is called from GetUserData().
    *   It calls 3ds max's GetCustAttribContainer() for the base object and for all its 
    *   modifiers to get and pass the ICustAttribContainer to ProcessCustAttribContainer().
    *   In addition it calls AddNodeUserProperties() for transforms and AddToonUserData()
    *   for shapes.
    *
    *   \param animObj animatable to extract the custom attributes from
    *   \param userDataColl the RwCommUserDataCollection in which the data is to be stored
    *   \param dataType user data type
    *
    *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError     AddNodeUserData(Animatable *animObj,
                                RwCommUserDataCollection &userDataColl,
                                RwCommUserDataCollection::Type dataType);
    /**
    *   Get User Defined Properties from an INode and store them as RwComm user data. Data
    *   will be extracted as a string array and name of the user data will be "3dsmax User Properties"
    *
    *   \param node an INode to extract the User Defined Properties from
    *   \param userDataColl the RwCommUserDataCollection in which the data is to be stored
    *   \param dataType user data type
    *
    *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError     AddNodeUserProperties(INode *node, RwCommUserDataCollection &userDataColl,
                                RwCommUserDataCollection::Type dataType);
    /**
    *   This function will extract toon vertex thickness data assigned by RpToonVTM.dlm modifier.
    *   This data is passed as RwComm user data to the exporter. Name of the data array is 
    *   "RpToon.VertexThickness" and format is RwCommUserDataCollection::rpREALUSERDATA.
    *
    *   \param node an INode to extract the toon data from, if present
    *   \param userDataColl the RwCommUserDataCollection in which the data is to be stored
    *
    *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    virtual RwCommError     AddToonUserData(INode *node, RwCommUserDataCollection &userDataColl);

protected:
    int             m_numVerts;
    int             m_numPolys;
};


#endif // RWMAXCUSTOMATTRIBUTES_H
