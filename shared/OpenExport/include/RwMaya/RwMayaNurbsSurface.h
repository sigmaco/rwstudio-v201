
#ifndef RWMAYANURBSSURFACE_H
#define RWMAYANURBSSURFACE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// =============================================================================
// -------------------------  RwMayaNurbsSurface CLASS  ------------------------
// =============================================================================

/**
*  \ingroup mayascenegraph 
*
*  This is the RwMaya implementation of the RwCommPatchMesh.
*
*   \see RwCommPatchMesh
*   \see RwMayaGeometry
*   \see RwMayaBlindData
*/
class RWMAYA_API RwMayaNurbsSurface : public RwCommPatchMesh, public RwMayaGeometry,
                                      public RwMayaBlindData
{
public:
    RwMayaNurbsSurface(MDagPath dagPath);
    virtual ~RwMayaNurbsSurface();

    virtual RwCommError     GetUserData(RwCommUserDataCollection &userDataColl);
    /**
     *  This function is not implemented under RwMaya.
     *  \return RWCOMM_NOT_IMPLEMENTED.
     */
    virtual RwCommError     GetVertexUserData(RwCommUserDataCollection &userDataColl,
                                              const RwVector<RwCommVertexId>& vertIds);
    /**
     *  This function is not implemented under RwMaya.
     *  \return RWCOMM_NOT_IMPLEMENTED.
     */
    virtual RwCommError     GetPrimitiveUserData(RwCommUserDataCollection &userDataColl);

    // Get attribute methods.
    virtual RwCommError     GetAttribute(const RwWString &name, int &value, int index = 0);
    virtual RwCommError     GetAttribute(const RwWString &name, float &value, int index = 0);
    virtual RwCommError     GetAttribute(const RwWString &name, RwWString &value, int index = 0);
    virtual RwCommError     GetAttributeNames(RwVector<RwWString>& vAttributeNames, RwVector<AttributeType> &vAttributeTypes);

    /**
     *  Maya implementation of RwCommGeometry::operator==. Compares two nurbs surfaces by checking
     *  they refer to the same MObject and that they have identical shading group assignments.
     *  \param geom the other RwMayaMesh to be compared.
     *  \return true if identical, false otherwise.
     */
    virtual bool            operator==(const RwCommGeometry& geom) const;

   // Bounding box
    virtual RwCommError     GetWorldSpaceBoundingBox(RwCommBoundingBox&) const;

    /**
    *   Clear any temporary object by calling ClearIntermediates() and rebuild
    *   the surface to Bezier nurbs surface if necessary.
    */
    virtual RwCommError     ReEvaluate(RwCommReal frame);

    // Visibility
    virtual RwCommError     GetVisibility(bool &bIsVisible) const;

    // Animatable methods
    RwCommError             OnSetFrame(RwCommReal frame);

    // Get vertex by id
    virtual RwCommError     GetVertex(const RwCommVertexId& id, RwCommVertex& vertex,
                                                           bool bWorldSpace = false);

    // Geometry methods
    virtual RwCommError     GetNumPrimitives(int& nPrims) const;
    virtual RwCommError     ResetPrimIterator();
    virtual RwCommError     AdvancePrimIterator();
    virtual RwCommError     GetPrimVertexCount(int& nVertices);
    virtual RwCommError     GetPrimVertex(int index, RwCommVertex& vertex, RwCommVertexId& id,
                                                                    bool bWorldSpace = false);
    virtual RwCommError     GetPrimIndex(int& nIndex);
    virtual RwCommError     GetPrimShader(RwCommShader*& pShader);
    /**
     *  This function is not implemented under RwMaya.
     *  \return RWCOMM_NOT_IMPLEMENTED.
     */
    virtual RwCommError     GetPrimNormal(RwCommVec3& normal, bool bWorldSpace = false);
    /**
     *  This function is not implemented under RwMaya.
     *  \return RWCOMM_NOT_IMPLEMENTED.
     */
    virtual RwCommError     GetDefaultColor(RwCommVec3 &color) const;

    // Patch methods
    /**
    *   All patches are of type QUAD
    *   \param type returns the type of the current patch primitive.
    */
    virtual RwCommError     GetPatchType(PatchType& type) const;
    /**
    *   All patches have degree of four.
    *   \param degree returns the degree of the current patch primitive.
    */
    virtual RwCommError     GetPatchDegree(int& degree) const;
    /**
     *  This function is not implemented under RwMaya.
     *  \return RWCOMM_NOT_IMPLEMENTED.
     */
    virtual RwCommError     InterpolatePatch(RwCommReal u, RwCommReal v,
                                     RwCommReal w, RwCommVec3& p) const;
    /**
    *   Create the RwMayaMesh, add it to the scene and set the transform. The actual
    *   tessellation is done in TessellateNurbsSurface()
    *
    *   \param pMesh returns the RwCommMesh representing the tessellated patch mesh.
    *       The mesh that is returned is attached to the RwCommScene.
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise
    */
    virtual RwCommError     Tessellate(RwCommMesh*& pMesh);

    /** \return The number of control points in the nurbs surface */
    int                     GetNumControlPoints() const;
    int                     GetNumControlPointsInU() const;
    int                     GetNumControlPointsInV() const;
    virtual RwCommError     GetComponent(int index, MObject &comp);

protected:
    /**
    *   Nurbs surface can be rebuilt to a bezier nurbs surface, or even tessellated.
    *   this will clear both temporary object if they were created.
    */
    void                    ClearIntermediates();

    /** Get the specified control point.
    *
    *   \param patchIndex to which control point belongs
    *   \param cpLocIndex of the control point. They are in range 0..15. For example if
    *          local index is 6, (U, V) = (1, 2), where both U and V are in range 0..3
    *   \param vertex to which control point belongs
    *   \param bWorldSpace to which control point belongs
    *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise
    Called from the GetVertex() and GetPrimVertex() */
    virtual RwCommError     GetCtrlPnt(int patchIndex, int cpLocIndex,
                                RwCommVertex& vertex, bool bWorldSpace = false);
    
    /**
     *  Tessellate the surface and store the result in m_tessMeshDagPath
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise
     */
    virtual RwCommError     TessellateNurbsSurface();
    virtual RwCommError     RemapCtrlPointToBezierSurf(int &u, int &v);

protected:
    int                     m_spensInUIt;
    int                     m_spensInVIt;

    /** True if the surface is rebuilt to the Bezier surface */
    bool                    m_rebuilt;
    MObject                 m_rebuildNode;
    /** Dag path to the rebuilt Bezier surface */
    MDagPath                m_rebuiltDagPath;

    MObjectArray            m_sets;
    MObjectArray            m_comps;
    
    MFnNurbsSurface         m_nurbsSurface;
    
    /** True if there is a tessalated mesh */
    bool                    m_tessellated;
    /** Dag path for the tessalated mesh */
    MDagPath                m_tessMeshDagPath;

    RwCommShader            *m_pPrimShaderCache;
    /** Dag path to the tessalated mesh */
    RwCommMesh              *m_pTessellatedMesh;
};

// =============================================================================
// ---------------------  RwMayaNurbsSurfaceCreator CLASS  ---------------------
// =============================================================================

/**
*  \ingroup mayabuilder 
*
*  This class is responsible for creating an RwMayaNurbsSurface object.
*
*   \see RwMayaNurbsSurface
*   \see RwMayaObjCreator
*/
class RWMAYA_API RwMayaNurbsSurfaceCreator : public RwMayaGeometryCreator
{
public:
    RwMayaNurbsSurfaceCreator() : RwMayaGeometryCreator() {}
    virtual ~RwMayaNurbsSurfaceCreator() {}

    virtual RwCommObject    *Create(const RwCommCreationData *pCreationData,
                                    const RwCommExportOptions *pExportOptions) const;

    virtual void            Traverse(RwCommBuilder &rBuilder,
                                     RwCommCreationData *pCreationData,
                                     RwCommObject *pParent) const;
};

#endif // RWMAYANURBSSURFACE_H
