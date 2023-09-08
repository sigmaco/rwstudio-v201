// RwMaxHint.h: interface for the RwMaxHint class.
//
//////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "RwCommHint.h"
#include "RwCommObjCreator.h"

#include "RwMax.h"
#include "RwMaxCustomAttributes.h"
#include "RwMaxTraverseModifiers.h"
#include "RwMaxTraverseControllers.h"

extern HINSTANCE hGInstance;


class RWMAX_API RwMaxHint : public SimpleObject, public IParamArray, public RwCommHint
{
   public:
        // Pure virtuals from RwCommHint
        virtual RwCommError GetHintType(int& type) const;
        virtual RwCommError GetHintStrengths(float& shieldHintStrength, float& partitionHintStrength) const;
        virtual RwCommError GetHintDimensions(float& width, float& length, float& height) const;

        // Shape methods
        virtual RwCommError GetVisibility(bool& bIsVisible) const;

       // Class vars
        static IParamMap *pmapCreate;
        static IParamMap *pmapTypeIn;
        static IParamMap *pmapParam;
        static IObjParam *ip;

        static int createMeth;
        int createType;
        int partitionHint;
        int shieldHint;
        float partitionHintStrength;
        float shieldHintStrength;
        
        static Point3 crtPos;
        static float crtWidth;
        static float crtHeight;
        static float crtLength;

        RwMaxHint();
        virtual ~RwMaxHint() {}
        

        //  inherited virtual methods:
        Class_ID ClassID() { return Class_ID(0x6ff97d65, 0x35763c37); }

		// Animatable methods
		void DeleteThis() { delete this; }

        // From Object
        int CanConvertToType(Class_ID obtype);
        void GetCollapseTypes(Tab<Class_ID> &clist,Tab<TSTR*> &nlist);
        
        // From BaseObject
        CreateMouseCallBack *GetCreateMouseCallBack();
        void BeginEditParams( IObjParam *ip, ULONG flags,Animatable *prev);
        void EndEditParams( IObjParam *ip, ULONG flags,Animatable *next);
        TCHAR *GetObjectName() { return _T("RenderWare Hint"); }
        int Display(TimeValue t, INode *inode, ViewExp *vpt, int flagst);

        // From ref
        RefTargetHandle Clone(RemapDir& remap = NoRemap());
		int NumRefs() {return 1;}
		RefTargetHandle GetReference(int i);
		void SetReference(int i, RefTargetHandle rtarg);		

        // From IParamArray
        BOOL SetValue(int i, TimeValue t, int v);
        BOOL SetValue(int i, TimeValue t, float v);
        BOOL SetValue(int i, TimeValue t, Point3 &v);
        BOOL GetValue(int i, TimeValue t, int &v, Interval &ivalid);
        BOOL GetValue(int i, TimeValue t, float &v, Interval &ivalid);
        BOOL GetValue(int i, TimeValue t, Point3 &v, Interval &ivalid);

        // From SimpleObject
        void BuildMesh(TimeValue t);
        

        int GetParamBlockIndex(int id);


        //BOOL OKtoDisplay(TimeValue t);
        void InvalidateUI();
        ParamDimension *GetParameterDim(int pbIndex);
        TSTR GetParameterName(int pbIndex);        
};          
      
//Class descriptor for plugin
class RwMaxHintClassDesc : public ClassDesc2
{
    public:
        int             IsPublic() { return 1; }
        void *          Create(BOOL loading = FALSE) {return new RwMaxHint;}
        const TCHAR *   ClassName() { return _T("Add Hint"); }
        SClass_ID       SuperClassID() { return GEOMOBJECT_CLASS_ID; }
        Class_ID        ClassID() { return Class_ID(0x6ff97d65, 0x35763c37); }
        const TCHAR*    Category() { return _T("RW Sectorization Hints");}   
        void            ResetClassParams(BOOL fileReset);

        HINSTANCE                   HInstance()             {return hGInstance;}
};

RWMAX_API ClassDesc* GetRwMaxHintDesc();



class RWMAX_API RwMaxHintCreator : public RwCommObjCreator,
 public RwMaxTraverseModifiers,
 public RwMaxTraverseControllers 
{
public:
	RwMaxHintCreator();
	virtual ~RwMaxHintCreator();

	virtual RwCommObject *Create(const RwCommCreationData *pCreationData,
								 const RwCommExportOptions *pExportOptions) const;

	virtual void Init(	RwCommObject *pObject,
						const RwCommCreationData *pCreationData,
						const RwCommExportOptions *pExportOptions) const;

	virtual void Attach(RwCommObject *pObject, RwCommObject *pParent, RwCommScene *pScene,
                        const RwCommCreationData *pCreationData) const;
};
