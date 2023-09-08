// RwExpVertex.h: interface for the RwExpVertex class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEXPVERTEX_H__F19C7541_3AB1_424F_8C08_D0179548FE5F__INCLUDED_)
#define AFX_RWEXPVERTEX_H__F19C7541_3AB1_424F_8C08_D0179548FE5F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RwComm.h"
#include "RwCommPrimitives.h"
#include "RwCommError.h"

class RwCommShader;
class RwCommUserData;
class RwCommUserDataCollection;
class RwCommGeometry;

/**
*  \ingroup commlowlevelobjects
*
*   This is the class representing a geometry's vertex. The vertex holds all of
*   the relevant coordinate information, such as position, position index, normal,
*   UV coordinates, color, illumination, alpha and associated shader.
*
*   To query if a vertex has a valid specified field use the HasDataType() method
*   specifying the type of vertex information you are interested in. Vertices that
*   do not have a specific data field, such as color information for example, will
*   return RwCommError::RWCOMM_INVALID_FIELD when trying to query the color.
*/
class RWCOMM_API RwCommVertex 
{
public:

    /** Bitfields specifying vertex information availabe for the vertex.*/
    enum FieldType
    {   
        /** position */
        POSITION = 0x1,
        /** position index */
        POS_INDEX = 0x2,
        /** normal */
        NORMAL = 0x4,
        /** pre-light color */
        COLOR = 0x8,
        /** pre-light alpha */
        ALPHA = 0x10,
        /** illumination */
        ILLUM = 0x20,
        /** UV coordinates */
        UVS = 0x40,
        /** shader of the primitive that vertex belongs to */
        SHADER = 0x80,
        /** unique id of the vertex */
        ID = 0x100
    };

public:
    /** Default constructor.*/
    RwCommVertex();
    /** Copy constructor.*/
    RwCommVertex(const RwCommVertex &v);
    virtual ~RwCommVertex();

    /**
    *   \param dataType data field to check.
    *   \return True if a vertex has a valid specified field, false otherwise.
    */
    bool        HasDataType(unsigned int dataType) const; 
    /** \return the bitfield specifying vertex information availabe for the vertex.*/
    unsigned int GetDataType() const { return m_iValidFields; }
    /** \param dataType bitfield indicating vertex information to set.*/
    void        SetDataType(unsigned int dataType) { m_iValidFields = dataType; }

    // Vertex order 
    /** Less than operator, used to sort vertices.*/
    virtual bool operator<(const RwCommVertex &v) const;

    // Vertex data fields
    /** \param v position to set to the vertex.*/
    void        SetPosition(const RwCommVec3 &v);
    /**
    *   \param v returns position for the vertex.
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_INVALID_FIELD if not present.
    */
    RwCommError GetPosition(RwCommVec3 &v) const;

    /** \param iIndex position index to set to the vertex.*/
    void SetPositionIndex(int iIndex);
    /**
    *   \param iIndex returns position index for the vertex.
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_INVALID_FIELD if not present.
    */
    RwCommError GetPositionIndex(int &iIndex) const;

    /** \param v normal to set to the vertex.*/
    void        SetNormal(const RwCommVec3 &v);
    /**
    *   \param v returns normal for the vertex.
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_INVALID_FIELD if not present.
    */
    RwCommError GetNormal(RwCommVec3 &v) const;

    /** \param v pre-light color to set to the vertex.*/
    void        SetColor(const RwCommVec3 &v);
    /**
    *   \param v returns pre-light color for the vertex.
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_INVALID_FIELD if not present.
    */
    RwCommError GetColor(RwCommVec3 &v) const;

    /** \param alpha pre-light alpha to set to the vertex.*/
    void        SetAlpha(const RwCommReal alpha);
    /**
    *   \param alpha returns pre-light alpha for the vertex.
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_INVALID_FIELD if not present.
    */
    RwCommError GetAlpha(RwCommReal &alpha) const;

    /** \param uv values to add as another new UV sets values.*/
    void        AddUV(const RwCommVec2 &uv);
    /**
    *   \param index of the UV set to query the UVs from.
    *   \param uv values to set to the specified UV set.
    */
    void        SetUV(int index, const RwCommVec2 &uv);
    /**
    *   \param uvs returns UV values from all UV sets.
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_INVALID_FIELD
    *           if no UV sets are present.
    */
    RwCommError GetUVs(RwVector<RwCommVec2> &uvs) const;
    /**
    *   \param index of the UV set to query the UVs from.
    *   \param uv returns UV values from the specified UV set.
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
    */
    RwCommError GetUV(int index, RwCommVec2 &uv) const;
    /** \param nUVs returns the number of UV sets.*/
    RwCommError GetNumUvs(int &nUVs) const;

    /** \param nUVs set the number of UV sets for this vertex */
    RwCommError SetNumUVs(const int nUVs);

    /** \param v illumination to set to the vertex.*/
    void        SetIllumination(const RwCommVec3 &v);
    /**
    *   \param v returns illumination for the vertex.
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_INVALID_FIELD if not present.
    */
    RwCommError GetIllumination(RwCommVec3 &v) const;

    /** \param pShader shader to set to the vertex.*/
    void        SetShader(RwCommShader *pShader);
    /**
    *   \param pShader returns shader for the vertex.
    *   \return RWCOMM_SUCCESS if successful, RWCOMM_INVALID_FIELD if not present.
    */
    RwCommError GetShader(RwCommShader *&pShader) const;

    void        SetUniqueID(int id) { m_uniqueID = id; m_iValidFields |= ID; }
    int         GetUniqueID() const { return m_uniqueID; }

protected:
    unsigned int                m_iValidFields;
    RwCommVec3                  m_vPos;
    unsigned int                m_iPosIndex;
    RwCommVec3                  m_vNormal;
    RwCommVec3                  m_vColor;
    RwCommReal                  m_rAplha;
    RwCommVec3                  m_vIlluim;
    RwVector<RwCommVec2>    m_vUVs;
    RwCommShader                *m_pShader;
    int                         m_uniqueID;
};

/**
*  \ingroup commlowlevelobjects
*
*   This structure is returned when querying a geometry for a vertex using
*   RwCommGeometry::GetVertex(). This structure is the unique identifier
*   for that vertex. The Id is used in various API methods, that deal with
*   vertices. You can extract the following information from the Id: the
*   source geometry where the vertex came from; the associated primitive
*   index in the geometry; the vertex index within the primitive and the
*   global vertex index. All indices refer to the original 3D modeling tool
*   geometry.
*/
struct RWCOMM_API RwCommVertexId
{
    /** Default constructor.*/
    RwCommVertexId() : pGeometry(NULL), face(-1), vertex(-1), index(-1), backFace(false) {}
    /** Copy constructor.*/
    RwCommVertexId(RwCommGeometry *pGeom, int f, int v, int i, bool bf = false):
    pGeometry(pGeom),face(f), vertex(v), index(i), backFace(bf) {}

    /**
    *   \return the geometry the vertex came from.
    */
    RwCommGeometry     *SourceGeometry() const { return pGeometry; }

    RwCommGeometry     *pGeometry;
    /** Primitive index in the geometry.*/
    int                 face;
    /** Local vertex index in the primitive.*/
    int                 vertex;
    /** Global position vertex index in the geometry.*/
    int                 index;
    /**
    *   Indicates if this is a back face vertex duplicated from
    *   a vertex belonging to a be double-sided primitive.
    */
    bool                backFace;
};

#endif // !defined(AFX_RWEXPVERTEX_H__F19C7541_3AB1_424F_8C08_D0179548FE5F__INCLUDED_)
