#ifndef RWEXPUSERDATA_H
#define RWEXPUSERDATA_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwComm.h"
#include "RwCommObject.h"

class RwCommUserData;

typedef RwList<RwCommUserData *>             RwCommUserDataList;
typedef RwList<RwCommUserData *>::iterator   RwCommUserDataListIt;


// =============================================================================
// ----------------------  RwCommUserDataCollection CLASS  ---------------------
// =============================================================================

/**
*  \ingroup commlowlevelobjects  
*
*   This is a container of RwCommUserData objects. Each RwCommObject can return
*   a collection of user data elements, attached to that object.
*
*  \note To extract vertex and primitive user data collections use the
*   RwCommGeometry::GetVertexUserData() and
*   RwCommGeometry::GetPrimitiveUserData() methods. For all other user
*   data collection queries, use the RwCommGeometry::GetUserData() method.
*
*   \see RwCommUserData
*/
class RWCOMM_API RwCommUserDataCollection
{
public:
    /** 
    *   Format is specifying what an RwCommUserData element consists of.
    */
    enum Format
    {
        /** not used */
        rpNAUSERDATAFORMAT = 0,
        /** int */
        rpINTUSERDATA,
        /** float */
        rpREALUSERDATA,
        /** string */
        rpSTRINGUSERDATA,
    };

    /** 
    *   Type is specifying what an RwCommUserData element applies to.
    */
    enum Type
    {
        /** not used */
        invalidUserDataType = 0,
        /** vertices */
        vertexUserDataType,
        /** primitives */
        primitiveUserDataType,
        /** shape */
        shapeUserDataType,
        /** transform */
        transformUserDataType,
        /** light */
        lightUserDataType,
        /** camera */
        cameraUserDataType,
        /** texture */
        textureUserDataType,
        /** material */
        materialUserDataType,
    };

    /** Default constructor.*/
    RwCommUserDataCollection();
    /** Copy constructor.*/
    RwCommUserDataCollection(const RwCommUserDataCollection& collection);
    /**
    *   Construct an object and initializes the m_object this collection
    *   refers to.
    */
    RwCommUserDataCollection(RwCommObject *object);
    virtual ~RwCommUserDataCollection();

    /**
    *   This will merge these two collections. It will add m_numVerts and
    *   m_numPolys on both collections together and all vertex and primitive
    *   RwCommUserData elements will be resized to feet the new size. Equal
    *   RwCommUserData elements will be merged together and the new ones added
    *   to the current collection. 
    *
    *   For elements other then vertex and primitive
    *   user data, elements with the same name will be prefixed with an object
    *   name these elements refer to.
    */
    RwCommUserDataCollection   &operator+=(RwCommUserDataCollection &b);

    /**
    *   Remove all RwCommUserData elements other the ones with the specified
    *   type.
    */
    void                        FilterUserDataList(Type dataType);
    /**
    *   Dump all the entire collection in the debugger output window
    *   using OutputDebugString().
    */
    void                        Dump();

public:
    /** RwCommObject this collection refers to.*/
    RwCommObject       *m_object;
    /** Number of vertices for RwCommUserData elements of vertexUserDataType.*/
    int                 m_numVerts;
    /**
    *   Number of primitives for RwCommUserData elements of
    *   primitiveUserDataType.
    */
    int                 m_numPolys;
    /** List of RwCommUserData that belong to this collection .*/
    RwCommUserDataList  m_list;
};

// =============================================================================
// ---------------------------  RwCommUserData CLASS  --------------------------
// =============================================================================

/**
*  \ingroup commlowlevelobjects  
*
*   This class represents an user data element.
*
*   \see RwCommUserDataCollection
*/
class RWCOMM_API RwCommUserData
{
public:
    RwCommUserData();
    virtual ~RwCommUserData();
    
    /**
    *   Equality operator.
    *
    *   \return True if the name, type and format are the same. Actual data is
    *   not compared.
    */
    bool    operator==(const RwCommUserData &other) const;

    /** Return number of user data elements*/
    int     GetNumElements() const;
    /**
    *   This will dump all the elements in the debugger output window using
    *   OutputDebugString().
    */
    void    Dump() const;

public:
    /** User data name.*/
    RwWString                           m_name;
    /** User data type.*/
    RwCommUserDataCollection::Type      m_type;
    /** User data format.*/
    RwCommUserDataCollection::Format    m_format;

    /** 
    *   Vector of int user data. This is used only if the format is
    *   rpINTUSERDATA
    */
    RwVector<int>                   m_intData;
    /**
    *   Vector of float user data. This is used only if the format is
    *   rpREALUSERDATA
    */
    RwVector<float>                 m_floatData;
    /**
    *   Vector of string user data. This is used only if the format is
    *   rpSTRINGUSERDATA
    */
    RwVector<RwWString>          m_stringData;
};

#endif // RWEXPUSERDATA_H
