#ifndef __RWSASSETLISTCTRL_H__
#define __RWSASSETLISTCTRL_H__

#include "resource.h"       // main symbols
#include <atlctl.h>

// Assuming that WTL 3.1 is installed (see tutorial), and exists where the compiler can find it
#include <atlapp.h>
#include <atlctrls.h>


// If you wish to host ActiveX controls in your dialog, change
// CDialogImpl to CAxDialogImpl in the typedef below.
typedef CComControl<class CRWSAssetListCtrl, CDialogImpl<class CRWSAssetListCtrl> >
                                                                 CComDlgControl;

class ATL_NO_VTABLE CRWSAssetListCtrl : 
   public CComObjectRootEx<CComSingleThreadModel>,
   public CComDlgControl,
   public IPersistStreamInitImpl<CRWSAssetListCtrl>,
   public IOleControlImpl<CRWSAssetListCtrl>,
   public IOleObjectImpl<CRWSAssetListCtrl>,
   public IOleInPlaceActiveObjectImpl<CRWSAssetListCtrl>,
   public IViewObjectExImpl<CRWSAssetListCtrl>,
   public IOleInPlaceObjectWindowlessImpl<CRWSAssetListCtrl>,
   public IPersistStorageImpl<CRWSAssetListCtrl>,
   public ISpecifyPropertyPagesImpl<CRWSAssetListCtrl>,
   public IQuickActivateImpl<CRWSAssetListCtrl>,
   public IDataObjectImpl<CRWSAssetListCtrl>,
   public CComCoClass<CRWSAssetListCtrl, &CLSID_RWSAssetListCtrl>,
   public IRWSAttribEditImpl<CRWSAssetListCtrl>
{
public:
   enum { IDD = IDD_RWSASSETLISTCTRLDLG };
public:
DECLARE_REGISTRY_RESOURCEID(IDR_RWSASSETLISTCTRL)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CRWSAssetListCtrl)
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
   COM_INTERFACE_ENTRY(IRWSAttribEdit)
END_COM_MAP()

BEGIN_PROP_MAP(CRWSAssetListCtrl)
   PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
   PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
   // Example entries
   // PROP_ENTRY("Property Description", dispid, clsid)
   // PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()

BEGIN_MSG_MAP(CRWSAssetListCtrl)
   MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
   MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
   MESSAGE_HANDLER(WM_SIZE, OnSize)
   COMMAND_CODE_HANDLER(CBN_SELENDOK, OnSelEndOK)

   CHAIN_MSG_MAP(CComDlgControl)
   DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

// IViewObjectEx
   DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// IRWSAssetListCtrl
public:
   CRWSAssetListCtrl () { m_bWindowOnly = TRUE; }

public:
   HRESULT OnDraw (ATL_DRAWINFO& di);

protected:
   // overrides
   virtual HWND CreateControlWindow (HWND hWndParent, RECT& rcPos)
   {
      Create (hWndParent, rcPos);
      ShowWindow (SW_SHOW);

      return m_hWnd;
   }

protected:
   // windows message handlers
   LRESULT OnMouseMove (UINT, WPARAM, LPARAM, BOOL &);
   LRESULT OnMouseLeave (UINT, WPARAM, LPARAM, BOOL &);
   LRESULT OnSize (UINT, WPARAM, LPARAM, BOOL &);
   LRESULT OnSelEndOK (WORD wNotifyCode, WORD wID,
                       HWND hWndCtl, BOOL& bHandled);

public:
   // IRWSAttribEdit interface

public:
   // IRWSAttribEditImpl helpers
   RWSUInt32 GetNumDefaultTypeChars (const RWSChar * const szParamList,
                                     RWSUInt32 nParamLength);
   void GetDefaultType (RWSChar * const szType,
                        const RWSChar * const szParamList,
                        RWSUInt32 nParamLength);
   RWSUInt32 GetDefaultDataSize (const RWSChar * const szParamList,
                                 RWSUInt32 nParamLength);
   void GetDefaultData (RWSByte * const pData,
                        const RWSChar * const szParamList,
                        RWSUInt32 nParamLength);
   void CreateUI ();
   void UpdateControls (const RWSByte * const pData);

protected:
   // data
   CComboBox m_WndCombo;
};

#endif //__RWSASSETLISTCTRL_H__
