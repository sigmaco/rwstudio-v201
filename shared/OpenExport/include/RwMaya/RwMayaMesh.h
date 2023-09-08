#if !defined(RW_MAYA_MESH_H)
#define RW_MAYA_MESH_H

#pragma warning(disable : 4786)
#pragma warning(disable : 4251)

#include "RwMayaBlindData.h"

// =============================================================================
// ---------------------------  RwMayaMesh CLASS  ------------------------------
// =============================================================================

/**
*  \ingroup mayascenegraph 
*
*  This is the RwMaya implementation of the RwCommMesh.
*
*   \see RwCommMesh
*   \see RwMayaGeometry
*   \see RwMayaBlindData
*/
class RWMAYA_API RwMayaMesh : public RwCommMesh, public RwMayaGeometry, public RwMayaBlindData
{
public:
    RwMayaMesh(MDagPath dagPath);
    virtual ~RwMayaMesh();

    /**
     *  Maya implementation of RwCommObject::GetUserData. Simply calls RwMayaBlindData::GetUserData.
     *  \param userDataColl the RwCommUserDataCollection in which data is to be filled.
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetUserData(RwCommUserDataCollection &userDataColl);
    virtual RwCommError GetVertexUserData(RwCommUserDataCollection &userDataColl,
                                          const RwVector<RwCommVertexId>& vertIds);
    virtual RwCommError GetPrimitiveUserData(RwCommUserDataCollection &userDataColl);

    // Get attribute methods.
    virtual RwCommError GetAttribute(const RwWString &name, int &value, int index = 0);
    virtual RwCommError GetAttribute(const RwWString &name, float &value, int index = 0);
    virtual RwCommError GetAttribute(const RwWString &name, RwWString &value, int index = 0);

    /**
     *  Maya implementation of RwCommAnimatable::GetAttributeNames. Simply calls RwMayaBlindData::GetObjectAttributeNames.
     *  \param vAttributeNames the names of all attributes found.
     *  \param vAttributeTypes the types of all attributes found.
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetAttributeNames(RwVector<RwWString>& vAttributeNames, RwVector<AttributeType> &vAttributeTypes);

    /**
     *  Maya implementation of RwCommGeometry::operator==. Compares two meshes by checking
     *  they refer to the same MObject and that they have identical shading group assignments.
     *  \param geom the other RwMayaMesh to be compared.
     *  \return true if identical, false otherwise.
     */
    virtual bool operator==(const RwCommGeometry &geom) const;

    // Animatable methods
    RwCommError OnSetFrame(RwCommReal frame);

   // Bounding box
    virtual RwCommError GetWorldSpaceBoundingBox(RwCommBoundingBox&) const;

    // Evaluate geometry
    virtual RwCommError ReEvaluate(RwCommReal frame);

    // Visibility
    virtual RwCommError GetVisibility(bool &bIsVisible) const;

    // Primitive methods
    virtual RwCommError GetNumPrimitives(int &nPrims) const;
    virtual RwCommError ResetPrimIterator();
    
    
    /**
     *  Maya implementation of RwCommGeometry::AdvancePrimIterator. The function increments m_pMayaPrimIterator
     *  and m_iMayaCompIterator until a valid polygon is found or the current set is exhausted. In the latter
     *  case it moves onto the next member of m_usedComps and reinitializes m_pMayaPrimIterator and 
     *  m_iMayaCompIterator. If m_usedComps is exhausted RWCOMM_ITERATOR_END is returned.
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_ITERATOR_END if the entire mesh has been iterated.
     */
    virtual RwCommError AdvancePrimIterator();

    virtual RwCommError GetPrimVertexCount(int &nVertices);
    virtual RwCommError GetPrimVertex(int index, RwCommVertex &vertex, RwCommVertexId &id, bool bWorldSpace = false);
    virtual RwCommError GetPrimIndex(int &nIndex);
    virtual RwCommError GetPrimShader(RwCommShader * &pShader);
    virtual RwCommError GetPrimNormal(RwCommVec3 &normal, bool bWorldSpace = false);
    virtual RwCommError GetPrimTriangleCount(int &nTriangles);
    virtual RwCommError GetPrimTriangulation(RwVector< int > &triangleIndices);
    virtual RwCommError GetPrimEdges(RwVector<RwCommEdge> &edges);
    /**
     *  This function is not implemented under RwMaya.
     *  \param color a reference to a color to be filled in.
     *  \return RWCOMM_NOT_IMPLEMENTED.
     */
    virtual RwCommError GetDefaultColor(RwCommVec3 &color) const;
    virtual RwCommError GetToonInks(RwVector<RwCommToonInk> &toonInks);

    // Get vertex by id
    virtual RwCommError GetVertex(const RwCommVertexId& id, RwCommVertex& vertex, bool bWorldSpace = false);

protected:

    virtual void OnAddShader(RwCommShader* pShader);

    /**
     *  This function is called by AdvancePrimIterator and ResetPrimIterator to advance to the first valid
     *  polygon from (and including) the current position.
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_ITERATOR_END if the entire mesh has been iterated.
     */
    RwCommError AdvanceToValidPrim();

    //Get vertex data
    RwCommError GetMayaVertex(MItMeshPolygon *pFaceIter, int iVertex, RwCommVertex& vertex, bool bWorldSpace) const;
    RwCommError GetMayaVertexPosition(MItMeshPolygon *pFaceIter, int iVertex, RwCommVec3 &vertex, bool bWorldSpace) const;
    RwCommError GetMayaVertexPositionIndex(MItMeshPolygon *pFaceIter, int iVertex, int &iIndex) const;
    RwCommError GetMayaVertexNormal(MItMeshPolygon *pFaceIter, int iVertex, RwCommVec3 &normal, bool bWorldSpace) const;
    RwCommError GetMayaVertexColor(MItMeshPolygon *pFaceIter, int iVertex, RwCommVec3 &color) const;
    RwCommError	GetMayaVertexAlpha(MItMeshPolygon *pFaceIter, int iVertex, RwCommReal &alpha) const;
    /**
     *  This function is not implemented under RwMaya.
     *  \param pFaceIter the polygon iterator
     *  \param iVertex the vertex index within the polygon
     *  \param vIllum the illumination color to be filled in
     *  \return RWCOMM_NOT_IMPLEMENTED.
     */
    RwCommError	GetMayaVertexIllumination(MItMeshPolygon *pFaceIter, int iVertex, RwCommVec3 &vIllum) const;
    RwCommError	GetMayaVertexUVs(MItMeshPolygon *pFaceIter, int iVertex, RwCommShader *pShader,
                             RwVector< RwCommVec2 >& uvs) const;
    RwCommError GetFaceShader(MItMeshPolygon *pFaceIter, RwCommShader * & pShader) const;

    /**
     *  This function is used to intialise information about how many polygons this mesh contains
     *  and what shaders they are connected to. It is called on the first call to RwMayaMesh::ReEvaluate.
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_ITERATOR_END if the entire mesh has been iterated.
     */
    RwCommError InitialiseMesh();

protected:
    bool                    m_firstEvaluate;

    /**
     *  After the first call to RwMayaMesh::ReEvaluate this member holds an array of all the Maya sets
     *  connected to this mesh.
     */
    MObjectArray            m_sets;

    /**
     *  After the first call to RwMayaMesh::ReEvaluate this member holds an array of all the Maya components
     *  connected to this mesh.
     */
    MObjectArray            m_comps;

    /**
     *  After the first call to RwMayaMesh::ReEvaluate this member holds an array of all the Maya components
     *  used by this mesh. This may not match m_comps if the some connected sets are null.
     */
    MObjectArray            m_usedComps;

    /**
     *  After the first call to RwMayaMesh::ReEvaluate this member holds vector of vectors of
     *  bools representing which polygons in each set are valid. Polygons may be marked
     *  as invalid if Maya gave an error when extracting them, if they contain no vertices or if
     *  they reference vertices that are outside the range of the mesh. These are usually signs
     *  of corrupt Maya mesh data.
     */
    RwVector<RwVector<bool> >   m_validPolys;

    /**
     *  After the first call to RwMayaMesh::ReEvaluate this member holds the total number of valid
     *  polygons in the mesh.
     */
    int                     m_iPrimCount;

    /**
     *  Internal Maya polygon iterator used to iterate through the polygons of the currently
     *  accessed member of m_usedComps.
     */
    MItMeshPolygon          *m_pMayaPrimIterator;

    /**
     *  Internal index used to iterate through the members of m_usedComps.
     */
    int                     m_iMayaSetIterator;

    /**
     *  Internal index used to iterate through the members of m_usedComps[m_iMayaSetIterator].
     */
    int                     m_iMayaCompIterator;

    RwMap<int, int>     m_mUVSet;

    /**
     *  After the first call to RwMayaMesh::ReEvaluate this member holds names of all UV sets
     *  on the mesh.
     */
    MStringArray            m_UVSetNames;

    /**
     *  After the first call to RwMayaMesh::ReEvaluate this member holds a mapping from each valid
     *  polygon to the RwCommShader it is connected to.
     */
    RwMap<int, RwCommShader *> m_mPolyToShader;
};


// =============================================================================
// -----------------------  RwMayaMeshCreator CLASS  ---------------------------
// =============================================================================

/**
*  \ingroup mayabuilder 
*
*  This class is responsible for creating an RwMayaMesh object.
*
*   \see RwMayaMesh
*   \see RwMayaObjCreator
*/
class RWMAYA_API RwMayaMeshCreator  : public RwMayaGeometryCreator
{
public:
    RwMayaMeshCreator();
    virtual ~RwMayaMeshCreator();

    virtual RwCommObject *Create(   const RwCommCreationData *pCreationData,
                                    const RwCommExportOptions *pExportOptions) const;

    virtual void Traverse(RwCommBuilder &rBuilder,
                            RwCommCreationData *pCreationData,
                            RwCommObject *pParent) const;
};

#endif // !defined(RW_MAYA_MESH_H)
