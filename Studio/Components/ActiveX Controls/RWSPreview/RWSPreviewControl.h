// RWSPreviewControl.h : Declaration of the CRWSPreviewControl

#ifndef __RWSPREVIEW_H_
#define __RWSPREVIEW_H_

#include "resource.h"       // main symbols
#include "Components\Common\DragDropImpl.h"

#include "RenderWareEngine.h"

using namespace RW;

typedef CWinTraitsOR <0, WS_EX_CLIENTEDGE> CRWSPreviewWindowTraits;

const float ORBIT_SPEED = 0.5f;
const float WORLD_ORBIT_SPEED = 1.5f;
const float PAN_SPEED   = 0.002f;
const float ZOOM_SPEED  = 0.002f;

enum {REFRESH_TIMER_ID = 0};

const RwV3d XAxis = {(RwReal)(1.0),  (RwReal)(0.0), (RwReal)(0.0)};
const RwV3d YAxis = {(RwReal)(0.0), (RwReal)(1.0), (RwReal)(0.0)};
const RwV3d ZAxis = {(RwReal)(0.0), (RwReal)(0.0), (RwReal)(1.0)};


/////////////////////////////////////////////////////////////////////////////
// CRWSPreviewControl
class ATL_NO_VTABLE CRWSPreviewControl : 
   public CComObjectRootEx<CComSingleThreadModel>,
   public IDispatchImpl<IRWSPreview, &IID_IRWSPreview, &LIBID_RWSPREVIEWLib>,
   public CComControl<CRWSPreviewControl>,
   public IPersistStreamInitImpl<CRWSPreviewControl>,
   public IOleControlImpl<CRWSPreviewControl>,
   public IOleObjectImpl<CRWSPreviewControl>,
   public IOleInPlaceActiveObjectImpl<CRWSPreviewControl>,
   public IViewObjectExImpl<CRWSPreviewControl>,
   public IOleInPlaceObjectWindowlessImpl<CRWSPreviewControl>,
   public IPersistStorageImpl<CRWSPreviewControl>,
   public ISpecifyPropertyPagesImpl<CRWSPreviewControl>,
   public IQuickActivateImpl<CRWSPreviewControl>,
   public IDataObjectImpl<CRWSPreviewControl>,
   public IProvideClassInfo2Impl<&CLSID_RWSPreview, NULL, &LIBID_RWSPREVIEWLib>,
   public CComCoClass<CRWSPreviewControl, &CLSID_RWSPreview>,
   public IDropTargetImpl <CRWSPreviewControl>
{
public:
DECLARE_WND_CLASS_EX (_T ("RenderWare Preview Window"), 0, -1); // -1 is null brush
DECLARE_REGISTRY_RESOURCEID(IDR_RWSPREVIEW1)
DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CRWSPreviewControl)
   COM_INTERFACE_ENTRY(IDropTarget)
   COM_INTERFACE_ENTRY(IRWSPreview)
   COM_INTERFACE_ENTRY(IDispatch)
   COM_INTERFACE_ENTRY(IViewObjectEx)
   COM_INTERFACE_ENTRY(IViewObject2)
   COM_INTERFACE_ENTRY(IViewObject)
   COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
   COM_INTERFACE_ENTRY(IOleInPlaceObject)
   COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceObjectWindowless)
   COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
   COM_INTERFACE_ENTRY(IOleControl)
   COM_INTERFACE_ENTRY(IOleObject)
   COM_INTERFACE_ENTRY(IPersistStreamInit)
   COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)
   COM_INTERFACE_ENTRY(ISpecifyPropertyPages)
   COM_INTERFACE_ENTRY(IQuickActivate)
   COM_INTERFACE_ENTRY(IPersistStorage)
   COM_INTERFACE_ENTRY(IDataObject)
   COM_INTERFACE_ENTRY(IProvideClassInfo)
   COM_INTERFACE_ENTRY(IProvideClassInfo2)
END_COM_MAP()

BEGIN_PROP_MAP(CRWSPreviewControl)
END_PROP_MAP()

BEGIN_MSG_MAP(CRWSPreviewControl)
   MESSAGE_HANDLER (WM_CREATE, OnCreate)
   MESSAGE_HANDLER (WM_DESTROY, OnDestroy)
   MESSAGE_HANDLER (WM_NCDESTROY, OnNcDestroy)
   MESSAGE_HANDLER (WM_SIZE, OnSize)
   MESSAGE_HANDLER (WM_LBUTTONDOWN, OnMouseButton)
   MESSAGE_HANDLER (WM_MBUTTONDOWN, OnMouseButton)
   MESSAGE_HANDLER (WM_MBUTTONUP, OnMouseButton)
   MESSAGE_HANDLER (WM_LBUTTONUP, OnMouseButton)
   MESSAGE_HANDLER (WM_MOUSEMOVE, OnMouseMove)

   MESSAGE_HANDLER (WM_RBUTTONDOWN, OnMouseButton)
   MESSAGE_HANDLER (WM_RBUTTONUP, OnMouseButton)
   MESSAGE_HANDLER (WM_SETFOCUS , OnSetFocus)

   CHAIN_MSG_MAP(CComControl<CRWSPreviewControl>)
   DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()

// IViewObjectEx
   DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)
public:
   CRWSPreviewControl () : m_DatabaseID (0), m_MousePos (0, 0), m_pPreviewNotify (0),
      m_LastTime (0), m_PlayState (_psNoAnim), m_Repeat (FALSE), m_LButton(FALSE), 
      m_MButton(FALSE),m_Mode(ORBIT), m_OldMode(NONE), m_DrawSkeleton(TRUE), 
      m_DrawSkelaxis(TRUE), m_DrawClump(TRUE), m_DrawText(FALSE),
      m_DrawWorld(TRUE), m_DrawSectors(FALSE),m_DrawWireFrame(FALSE), m_DrawNormals(FALSE),
      m_CurrentSector(0),m_ZTest(FALSE){}

   LRESULT OnCreate (UINT, WPARAM, LPARAM, BOOL& bHandled);
   LRESULT OnDestroy (UINT, WPARAM, LPARAM, BOOL& bHandled);
   LRESULT OnNcDestroy (UINT, WPARAM, LPARAM, BOOL& bHandled);
   LRESULT OnSize (UINT, WPARAM, LPARAM lParam, BOOL& bHandled);
   LRESULT OnMouseButton (UINT uMsg, WPARAM, LPARAM lParam, BOOL& bHandled);
   LRESULT OnMouseMove (UINT uMsg, WPARAM, LPARAM lParam, BOOL& bHandled);
   LRESULT OnTimer (UINT, WPARAM, LPARAM, BOOL&);
   HRESULT OnDraw(ATL_DRAWINFO& di);
   LRESULT OnSetFocus (UINT, WPARAM, LPARAM, BOOL& bHandled);

   void OnIdle ();
   void PlayAnimation ();
   void PauseAnimation ();
   void StopAnimation ();

   typedef std::basic_string <RWSChar> RWSString;
   RwCorePluginID GetAssetPlugin (RwChar * szAssetType);
   RwBool LoadClump (RWSString strFile, RWSString strTextures);
   RwBool LoadAnimation (RWSString strFile);
   RwBool LoadWorld (RWSString strFile, RWSString strTextures);
   HRESULT GetAssetDataFromAssetID (RWSDatabaseID DatabaseID, RWSID AssetID, RWSString& strFileName, RWSString& strType, RWSString& strTextures);
   HRESULT GetAssetDataFromFile (RWSString strFile, RWSString& strType,RWSString& strTextures);   

private:
   void ControlClump (LPARAM lParam);
   void ControlWorld (LPARAM lParam);
   void SectorTraverse(void);

   __PlayState      m_PlayState;          // Playstate for an animation.
   RWSDatabaseID    m_DatabaseID;         // RwStudio Database Id.
   RenderWareEngine m_Engine;             // RenderWare Engine class.
   CPoint           m_MousePos;           // Mouse pointer.
   IRWSPreviewNotify* m_pPreviewNotify;   // Interface to the preview proxy class.
   DWORD              m_DragEffect;       // Used for drag and drop.
   DWORD            m_LastTime;           // Used for animation timing.
   RwBool           m_Repeat;             // Does animation repeat?
   RwBool           m_LButton;            // Left mouse button pressed?
   RwBool           m_MButton;            // Middle mouse button pressed?    
   RwBool           m_DrawSkeleton;       // Draw skeleton for clump?
   RwBool           m_DrawSkelaxis;       // Drax axis handles for clumps skeleton?
   RwBool           m_DrawClump;          // Draw clump in window?
   RwBool           m_DrawText;           // Draw text stats in window?
   RwBool           m_DrawWorld;          // Draw world in window?
   RwBool           m_DrawSectors;        // Draw sector visualization in window?
   RwBool           m_DrawWireFrame;      // Draw wireframe world in window?
   RwBool           m_DrawNormals;        // Draw world vertex normals in window?
   RwBool           m_ZTest;              // Person Z test for world rendering?
   RwInt32          m_CurrentSector;      // Number of current sector when cycling though sectors?


   enum _Mode {NONE, PAN, ORBIT, ZOOM};   // Navigation mode enum
   _Mode            m_Mode;               // Current navigation mode.
   _Mode            m_OldMode;            // Old navigation mode.

// IRWSPreview
public:
   STDMETHOD(DrawSkeleton)(/*[in]*/ VARIANT_BOOL draw);
   STDMETHOD(get_DrawSkeleton) (VARIANT_BOOL* pbRepeat);
   STDMETHOD(DrawSkeletonAxis)(/*[in]*/ VARIANT_BOOL draw);
   STDMETHOD(get_DrawSkeletonAxis) (VARIANT_BOOL* pbRepeat);
   STDMETHOD(DrawClump)(/*[in]*/ VARIANT_BOOL draw);
   STDMETHOD(get_DrawClump) (VARIANT_BOOL* pbDrawClump);
   STDMETHOD(DrawText)(/*[in]*/ VARIANT_BOOL drawText);
   STDMETHOD(get_DrawText) (VARIANT_BOOL* pbDrawText);
   STDMETHOD(DrawWorld)(/*[in]*/ VARIANT_BOOL drawWorld);
   STDMETHOD(get_DrawWorld) (VARIANT_BOOL* pbDrawWorld);
   STDMETHOD(DrawSectors)(/*[in]*/ VARIANT_BOOL drawSectors);
   STDMETHOD(get_DrawSectors) (VARIANT_BOOL* pbDrawSectors);
   STDMETHOD(DrawWireFrame)(/*[in]*/ VARIANT_BOOL drawWireFrame);
   STDMETHOD(get_DrawWireFrame) (VARIANT_BOOL* pbDrawWireFrame);
   STDMETHOD(DrawNormals)(/*[in]*/ VARIANT_BOOL drawNormals);
   STDMETHOD(get_DrawNormals) (VARIANT_BOOL* pbDrawNormals);
   STDMETHOD(ZTest)(/*[in]*/ VARIANT_BOOL bZTest);
   STDMETHOD(get_ZTest) (VARIANT_BOOL* pbZTest);
   STDMETHOD(LocateInView)();
   STDMETHOD(SetAnimationPosition)(/*[in]*/ float Time);
   STDMETHOD(GetAnimationTime)(/*[out]*/ float* time);
   STDMETHOD(SetBackgroundColor)(/*[in]*/ unsigned int Red, /*[in]*/ unsigned int Green, /*[in]*/ unsigned int Blue);
   STDMETHOD (SetPlayState) (__PlayState State);
   STDMETHOD (get_Repeat) (VARIANT_BOOL* pbRepeat);
   STDMETHOD (put_Repeat) (VARIANT_BOOL bRepeat);
   STDMETHOD (Reset) ();
   STDMETHOD (_Ping) () { return S_OK; }
   STDMETHOD (_Init) (IUnknown* pNotify);
   STDMETHOD(SetZoom)();
   STDMETHOD(SetPan)();
   STDMETHOD(SetOrbit)();
   STDMETHOD(PrevSector)();
   STDMETHOD(NextSector)();
   STDMETHOD(ShowAllSectors)();

   // IDropTarget
   HRESULT IDropTarget_DragEnter (IDataObject *pDataObj, DWORD dwKeyState, POINTL pt, DWORD *pdwEffect);
   HRESULT IDropTarget_DragOver (DWORD dwKeyState, POINTL pt, DWORD *pdwEffect);
   HRESULT IDropTarget_DragLeave (void);
   HRESULT IDropTarget_Drop (IDataObject *pDataObj, DWORD dwKeyState, POINTL pt, DWORD *pdwEffect);
};

#endif //__RWSPREVIEW_H_
