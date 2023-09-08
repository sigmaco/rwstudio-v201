#if !defined(RW_MAYA_SPLINE_H)
#define RW_MAYA_SPLINE_H

#include "RwMayaBlindData.h"

// =============================================================================
// ---------------------------  RwMayaSpline CLASS  ------------------------------
// =============================================================================

/**
*  \ingroup mayascenegraph 
*
*  This is the RwMaya implementation of RwCommSpline.
*
*   \see RwCommSpline
*   \see RwMayaGeometry
*   \see RwMayaBlindData
*/

class RWMAYA_API RwMayaSpline  : public RwCommSpline, public RwMayaGeometry,
                                 public RwMayaBlindData
{
public:
	RwMayaSpline(MDagPath dagPath);
	virtual ~RwMayaSpline();

    virtual RwCommError     GetUserData(RwCommUserDataCollection &userDataColl);

    // Get attribute methods.
    virtual RwCommError GetAttribute(const RwWString& name, int &value, int index = 0);
    virtual RwCommError GetAttribute(const RwWString& name, float &value, int index = 0);
    virtual RwCommError GetAttribute(const RwWString& name, RwWString &value, int index = 0);
    /**
     *  Maya implementation of RwCommAnimatable::GetAttributeNames. Simply calls RwMayaBlindData::GetObjectAttributeNames.
     *  \param vAttributeNames the names of all attributes found.
     *  \param vAttributeTypes the types of all attributes found.
     *  \return RWCOMM_SUCCESS if successful, RWCOMM_FAIL otherwise.
     */
    virtual RwCommError GetAttributeNames(RwVector<RwWString>& vAttributeNames, RwVector<AttributeType> &vAttributeTypes);

	// Animatable methods
	RwCommError             OnSetFrame(RwCommReal frame);

   // Shape methods
    virtual RwCommError     GetVisibility(bool& bIsVisible) const;

   // Bounding box
	virtual RwCommError     GetWorldSpaceBoundingBox(RwCommBoundingBox&) const;

    // Reevaluate geometry
	virtual RwCommError     ReEvaluate(RwCommReal frame);

    // Get vertex by id
    virtual RwCommError     GetVertex(const RwCommVertexId& id, RwCommVertex& vertex, bool bWorldSpace = false) { return RWCOMM_NOT_IMPLEMENTED; }

    // Primitive methods
    virtual RwCommError     GetNumPrimitives(int &nPrims) const;
    virtual RwCommError     ResetPrimIterator();
    virtual RwCommError     AdvancePrimIterator();
    virtual RwCommError     GetPrimVertexCount(int &nVertices);
    virtual RwCommError     GetPrimVertex(int index, RwCommVertex &vertex, RwCommVertexId &id, bool bWorldSpace = false);
    virtual RwCommError     GetPrimIndex(int& nIndex);
    /**
     *  This function is not implemented under RwMaya.
     *  \return RWCOMM_NOT_IMPLEMENTED.
     */
    virtual RwCommError     GetPrimShader(RwCommShader *&pShader) ;
    virtual RwCommError     GetPrimNormal(RwCommVec3 &normal, bool bWorldSpace = false) { return RWCOMM_NOT_IMPLEMENTED; }
    /**
     *  This function is not implemented under RwMaya.
     *  \return RWCOMM_NOT_IMPLEMENTED.
     */
    virtual RwCommError     GetDefaultColor(RwCommVec3 &color) const;

    // Spline methods
    virtual RwCommError     PrimIsClosed(bool &bClosed) const;
    virtual RwCommError     SetVertexType(VertexType type);

    RwCommError             SetPointType(const int type);

protected:

	MDagPath                m_dagPath;
    int                     m_iFirstPoint;
    int                     m_iLastPoint;
    MFnNurbsCurve           m_fnCurve;
    MDoubleArray            m_knotValues;
    VertexType              m_type;
    int                     m_curveForm;
    int                     m_curveDegree;


};



// =============================================================================
// -----------------------  RwMayaSplineCreator CLASS  ---------------------------
// =============================================================================

/**
*  \ingroup mayabuilder 
*
*  This class is responsible for creating an RwMayaSpline object.
*
*   \see RwMayaSpline
*   \see RwMayaObjCreator
*/

class RWMAYA_API RwMayaSplineCreator : public RwMayaGeometryCreator
{
public:
	RwMayaSplineCreator();
	virtual ~RwMayaSplineCreator();

	virtual RwCommObject *Create(const RwCommCreationData *pCreationData,
								const RwCommExportOptions *pExportOptions) const;

    virtual void Traverse(RwCommBuilder &rBuilder,
                            RwCommCreationData *pCreationData,
                            RwCommObject *pParent) const;
};

#endif // !defined(RW_MAYA_SPLINE_H)
