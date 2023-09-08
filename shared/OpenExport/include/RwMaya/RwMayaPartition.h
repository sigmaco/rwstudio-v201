#include "RwCommPartition.h"
#include "RwCommObjCreator.h"

//#include "RwCommPartition.h"
#include "RwCommObjCreator.h"

#include "RwMaya.h"
#include "guidekd.h"

#if (!defined(DOXYGEN))

/////////////////////////////////////////////////////////////////////
//
// Geometry class
//
class partitioningTree
{
public:
    short displayTypeComponent;
    short colorTypeComponent;
    short hueTypeComponent;
    float alphaComponent;
    float colorScaleComponent;
    GuideKDTree* KD; // contains the geometry of the geometry
    int maxDepth;
    int condition;
};

/////////////////////////////////////////////////////////////////////
//
// Shape class - defines the non-UI part of a shape node
//
class RWMAYA_API RwMayaPartition : public MPxSurfaceShape, public RwCommPartition
{
public:
	RwMayaPartition();
	~RwMayaPartition(); 

	virtual void			postConstructor();
	virtual bool            isBounded() const;
	virtual MBoundingBox    boundingBox() const; 

	static  void *		    creator();
	static  MStatus		    initialize();
	partitioningTree*		assignGeometryComponents();

private:
	partitioningTree*		geometry;
	// Attributes
	//
    static MObject          RwEncodedKD;
    static MObject          displayTypeAttr;
    static MObject          colorTypeAttr;
    static MObject          hueTypeAttr;
    static MObject          transpAttr;
    static MObject          cscaleAttr;
public:
	// Shape type id
	//
	static MTypeId		    id;

    RwCommError             GetVisibility(bool &bIsVisible) const;

    RwCommError             SetXMLString(const RwString &s);
    RwCommError             GetXMLString(RwString &s);

    virtual RwCommError     Redraw();

};

/////////////////////////////////////////////////////////////////////
//
// UI class	- defines the UI part of a shape node
//
class RWMAYA_API RwMayaPartitionUI : public MPxSurfaceShapeUI
{
public:

    RwMayaPartitionUI();
	virtual ~RwMayaPartitionUI(); 

	virtual void	getDrawRequests( const MDrawInfo & info,
									 bool objectAndActiveOnly,
									 MDrawRequestQueue & q );
	virtual void	draw( const MDrawRequest & request,
						  M3dView & view ) const;
    void            drawElements(MPoint loc, partitioningTree * geom) const;
    void            drawSelectRecurser(GuideKDTree* KD,
                        partitioningTree* geom,
                        MPoint loc, int depth) const;
    void            drawSelect(GuideKDTree* KD, partitioningTree* geom, int depth) const;
    void            drawSector(GuideKDTree* KD,
                        BoundingBox * halfplane, partitioningTree* geom,
                        int depth) const;
    void            drawPartition(GuideKDTree* KD,
                        BoundingBox * halfplane, partitioningTree* geom,
                        int depth) const;
    static void     setColor(GuideKDTree* KD, partitioningTree* geom, int type, int depth);

    virtual bool	select( MSelectInfo &selectInfo,
							MSelectionList &selectionList,
							MPointArray &worldSpaceSelectPts ) const;
    static void            defineHeatSignature(partitioningTree* geom, float fraction, int cc);
	static void     *creator();


private:
    int             renderRequest;

};

class RWMAYA_API RwMayaPartitionCreator : public RwMayaObjCreator
{
public:
	RwMayaPartitionCreator();
	virtual ~RwMayaPartitionCreator();

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