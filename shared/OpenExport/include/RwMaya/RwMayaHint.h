#include "RwCommHint.h"
#include "RwCommObjCreator.h"

#if (!defined(DOXYGEN))

/////////////////////////////////////////////////////////////////////
//
// Geometry class
//
class shapePrimitive 
{
public:
    short shapeTypeID;
    bool shieldHintID;
    bool partitionHintID;
    float shieldHintStrengthID;
    float partitionHintStrengthID;
    
    MVector centre;
    int condition;
    float unitScale;
};

/////////////////////////////////////////////////////////////////////
//
// Shape class - defines the non-UI part of a shape node
//
class RWMAYA_API RwMayaHint : public MPxSurfaceShape, public RwCommHint
{
public:
	RwMayaHint(/*MDagPath dagPath*/);
	virtual ~RwMayaHint();

    virtual RwCommError GetHintType(int& type) const;
    virtual RwCommError GetHintStrengths(float& shieldHintStrength,
        float& partitionHintStrength) const;
    virtual RwCommError GetHintDimensions(float& width, float& length, float& height) const;

    virtual void			postConstructor();
	virtual bool            isBounded() const;
	virtual MBoundingBox    boundingBox() const; 

	static  void *		creator();
	static  MStatus		initialize();
	shapePrimitive*		getGeometry();
    RwCommError GetVisibility(bool &bIsVisible) const;

private:
	shapePrimitive*		fGeometry;

	// Attributes
	//
    static  MObject     shapeType;
    static  MObject     shieldHint;
    static  MObject     partitionHint;
    static  MObject     shieldHintStrength;
    static  MObject     partitionHintStrength;

public:
	// Shape type id
	//
	static	MTypeId		id;

};

/////////////////////////////////////////////////////////////////////
//
// UI class	- defines the UI part of a shape node
//
class RWMAYA_API RwMayaHintUI : public MPxSurfaceShapeUI
{
public:
	RwMayaHintUI();
	virtual ~RwMayaHintUI(); 

	virtual void	getDrawRequests( const MDrawInfo & info,
									 bool objectAndActiveOnly,
									 MDrawRequestQueue & requests );
	virtual void	draw( const MDrawRequest & request,
						  M3dView & view ) const;
	virtual bool	select( MSelectInfo &selectInfo,
							MSelectionList &selectionList,
							MPointArray &worldSpaceSelectPts ) const;

    void            drawHint(float shieldHintStrength, float partitionHintStrength, float unitScale) const;
    void            drawPlane(int type, float shieldHintStrength, float partitionHintStrength, float unitScale) const;
	static void *  creator();

private:
    int                renderRequest;

};

class RWMAYA_API RwMayaHintCreator : public RwMayaObjCreator
{
public:
	RwMayaHintCreator();
	virtual ~RwMayaHintCreator();

	virtual RwCommObject *Create(const RwCommCreationData *pCreationData,
        const RwCommExportOptions *pExportOptions) const;

	virtual void Init(  RwCommObject *pObject,
                        const RwCommCreationData *pCreationData,
                        const RwCommExportOptions *pExportOptions) const;

	virtual void Attach(RwCommObject* pObject, RwCommObject* pParent, RwCommScene* pScene,
                        const RwCommCreationData* pCreationData) const;

    virtual void Traverse(RwCommBuilder & rBuilder,
                            RwCommCreationData *pCreationData,
                            RwCommObject *pParent) const;
};

#endif // (!defined(DOXYGEN))