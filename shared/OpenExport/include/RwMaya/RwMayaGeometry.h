#ifndef RW_MAYA_GEOMETRY_H
#define RW_MAYA_GEOMETRY_H


// =============================================================================
// ---------------------------  RwMayaGeometry CLASS  ---------------------------
// =============================================================================

/**
*  \ingroup mayahelper 
*
*  This is the base RwMaya geometry class which contains functionality required by
*  all Maya geometry types. It is derived from by RwMayaMesh, RwMayaNurbsSurface and
*  RwMayaSpline.
*
*/
class RWMAYA_API RwMayaGeometry
{
public:
    RwMayaGeometry(MDagPath dagPath);
    virtual ~RwMayaGeometry();

    /**
     *  Get the Maya dag path for this geometry.
     * 
     *  \returns the MDagPath referencing this geometry.
     *
     */
    MDagPath GetDagPath() const { return m_dagPath; }

protected:
	MDagPath m_dagPath;
};

// =============================================================================
// -----------------------  RwMayaGeometryCreator CLASS  ------------------------
// =============================================================================

/**
*  \ingroup mayabuilder 
*
*  This class is responsible for creating an RwMayaGeometry object.
*
*   \see RwMayaObjCreator
*   \see RwMayaGeometry
*/
class RWMAYA_API RwMayaGeometryCreator : public RwMayaObjCreator
{
public:
    RwMayaGeometryCreator();
    virtual ~RwMayaGeometryCreator();

    virtual void Init(RwCommObject* pObject,
                      const RwCommCreationData* pCreationData,
                      const RwCommExportOptions* pExportOptions) const;

	virtual void Attach(RwCommObject* pObject, RwCommObject* pParent, RwCommScene* pScene,
                        const RwCommCreationData* pCreationData) const;
};

#endif // RW_MAYA_GEOMETRY_H