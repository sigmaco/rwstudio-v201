#define xml_string 1

#include "RwCommPartition.h"
#include "RwCommObjCreator.h"

#include "RwMax.h"
#include "RwMaxCustomAttributes.h"
#include "RwMaxTraverseModifiers.h"
#include "RwMaxTraverseControllers.h"
#include "guidekd.h"

extern HINSTANCE hGInstance;

class RWMAX_API RwMaxPartition : public SimpleObject, public IParamArray, public RwCommPartition
{
   public:
        // Shape methods
        virtual RwCommError GetVisibility(bool& bIsVisible) const;

        // RwCommPartition methods
        virtual RwCommError SetXMLString(const RwString &s);
        virtual RwCommError GetXMLString(RwString &s);

        virtual RwCommError Redraw();       

        // Class vars
        static IParamMap *pmapCreate;
        static IParamMap *pmapShow;
        static IParamMap *pmapHue;
        static IParamMap *pmapTransp;
        static IObjParam *ip;

        RwString xmlString;
        //void S-etXMLString(RwString input);

        IParamBlock2	    *pblock2; //ref 0
        //int testsave;
        //IOResult Save(ISave* isave);
        //IOResult Load(ILoad* iload);

        static int displayType; //Sectors or partitions
        static int hueType; //Col or b&w
        static int codeType; //Col or b&w
        static float transp;
        static float crange;
        GuideKDTree *KD;
        void top(INode *node);


        RwMaxPartition();
        ~RwMaxPartition();
        
        Box3 GetHierarchicalBbox(INode *npNode);

        void drawSelectRecurser(GuideKDTree *KD, Point3 loc, int depth);
        void drawSelect(GuideKDTree *KD, int depth) const;
        void drawPartition(GuideKDTree *KD, BoundingBox *halfplane, int depth) const;
        void drawSector(GuideKDTree *KD, BoundingBox *bbox, int depth) const;
        void setColor(GuideKDTree *KD, Material &m, int depth, int wireframe) const;
        void defineHeatSignature(float fraction, Material &m, int wireframe) const;

        //  inherited virtual methods:
        Class_ID ClassID() { return Class_ID(0x54474f93, 0x296669b7); }

		// Animatable methods
		void DeleteThis() { delete this; }

	    int                 NumSubs()                       {return 1;}
	    TSTR                SubAnimName(int i)              {return "Parameters";}
        Animatable          *SubAnim(int i)                 {return pblock2;}

        // From Object
        int CanConvertToType(Class_ID obtype);
        void GetCollapseTypes(Tab<Class_ID> &clist,Tab<TSTR*> &nlist);
        
        // From BaseObject
        CreateMouseCallBack* GetCreateMouseCallBack();
        void BeginEditParams( IObjParam *ip, ULONG flags,Animatable *prev);
        void EndEditParams( IObjParam *ip, ULONG flags,Animatable *next);
        TCHAR *GetObjectName() { return _T("RenderWare Partition"); }
        int Display(TimeValue t, INode* inode, ViewExp *vpt, int flagst);

        RefTargetHandle Clone(RemapDir& remap = NoRemap());
        int	                NumParamBlocks()                {return 1;}
	    IParamBlock2        *GetParamBlock(int i)           {return pblock2;}
	    IParamBlock2        *GetParamBlockByID(BlockID id)  {return (pblock2->ID() == id) ? pblock2 : NULL;}
        int NumRefs() {return 1;}

        RefTargetHandle GetReference(int i)
        {
            return pblock2;
        }
        void SetReference(int i, RefTargetHandle rtarg)
        {
            pblock2 = (IParamBlock2 *)rtarg;
        }


        // From IParamArray
        BOOL SetValue(int i, TimeValue t, int v);
        BOOL GetValue(int i, TimeValue t, int &v, Interval &ivalid);
        BOOL SetValue(int i, TimeValue t, float v);
        BOOL GetValue(int i, TimeValue t, float &v, Interval &ivalid);

        // From SimpleObject
        void BuildMesh(TimeValue t);
        //BOOL OKtoDisplay(TimeValue t);
        void InvalidateUI();
        ParamDimension *GetParameterDim(int pbIndex);
        TSTR GetParameterName(int pbIndex);        
};                


//Class descriptor for plugin
class RwMaxPartitionClassDesc : public ClassDesc2
{
    public:
        //The partition object is not public (i.e. not in the object menu)
        int             IsPublic() { return 0; }
        void            *Create(BOOL loading = FALSE) {return new RwMaxPartition;}
        const TCHAR     *ClassName() { return _T("Partitions"); }
        SClass_ID       SuperClassID() { return GEOMOBJECT_CLASS_ID; }
        Class_ID        ClassID() { return Class_ID(0x54474f93, 0x296669b7); }
        const TCHAR     *Category() { return _T("Partition");}   
        void            ResetClassParams(BOOL fileReset);

        HINSTANCE                   HInstance()             {return hGInstance;}
};

RWMAX_API ClassDesc *GetRwMaxPartitionDesc();



class RWMAX_API RwMaxPartitionCreator : public RwCommObjCreator,
 public RwMaxTraverseModifiers,
 public RwMaxTraverseControllers 
{
public:
	RwMaxPartitionCreator();
	virtual ~RwMaxPartitionCreator();

	virtual RwCommObject *Create(const RwCommCreationData *pCreationData,
								 const RwCommExportOptions *pExportOptions) const;

	virtual void Init(	RwCommObject *pObject,
						const RwCommCreationData *pCreationData,
						const RwCommExportOptions *pExportOptions) const;

	virtual void Attach(RwCommObject *pObject, RwCommObject *pParent, RwCommScene *pScene,
                        const RwCommCreationData *pCreationData) const;
};
