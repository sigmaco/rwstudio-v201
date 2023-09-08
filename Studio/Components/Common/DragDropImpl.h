#ifndef __DRAGDROP_H__
#define __DRAGDROP_H__

////////////////////////////////////////////////////////////////////////////////
namespace ATL
{
   template<>
   class _Copy<FORMATETC>
   {
   public:
      static HRESULT copy(FORMATETC* pDest, FORMATETC* pSrc)
      {
         ATLASSERT(pDest != NULL);
         ATLASSERT(pSrc != NULL);
         CopyMemory (pDest, pSrc, sizeof(FORMATETC));
         return S_OK;
      }

      static void init(FORMATETC* p)
      {
         p->cfFormat = CF_TEXT;
         p->ptd = NULL;
         p->dwAspect = DVASPECT_CONTENT;
         p->lindex = -1;
         p->tymed = TYMED_HGLOBAL;
      }

      static void destroy(FORMATETC* p)
      {
      }
   };
}

////////////////////////////////////////////////////////////////////////////////
// Currently only supports a single clipboard format
class CSimpleDataObject : public CComObjectRoot, public IDataObject
{
   // CSimpleDataObject
public:
   CSimpleDataObject () : m_bRelease (FALSE) {}

BEGIN_COM_MAP (CSimpleDataObject)
   COM_INTERFACE_ENTRY (IDataObject)
END_COM_MAP ()

   /////////////////////////////////////////////////////////////////////////////
   // Helper method for setting a single hGlobal data object
   BOOL SetData (UINT nClipboardFormat, HGLOBAL hGlobal, BOOL bRelease)
   {
      ATLASSERT (nClipboardFormat);
      ATLASSERT (hGlobal);

      IStream * pStream = 0;
      ::CreateStreamOnHGlobal (hGlobal, TRUE, &pStream);

      FORMATETC Format = {0};
      Format.cfFormat  = nClipboardFormat;
      Format.ptd       = 0;
      Format.dwAspect  = DVASPECT_CONTENT;
      Format.lindex    = -1;
      Format.tymed     = TYMED_HGLOBAL;

      STGMEDIUM Medium = {0};
      Medium.tymed     = TYMED_HGLOBAL;
      Medium.hGlobal   = hGlobal;
      Medium.pUnkForRelease = pStream;

      return SUCCEEDED (SetData (&Format, &Medium, bRelease));
   }

   /////////////////////////////////////////////////////////////////////////////
   // Clears any data
   void ClearData (void)
   {
      // Free any old data
      if (m_bRelease && m_Format.tymed == TYMED_HGLOBAL)
      {
         ATLASSERT (m_Medium.hGlobal);
         ::GlobalFree (m_Medium.hGlobal);
      }

      ZeroMemory (&m_Format, sizeof (FORMATETC));
      ZeroMemory (&m_Medium, sizeof (STGMEDIUM));
      m_bRelease = FALSE;
   }

   // IDataObjectImpl
public:
   /////////////////////////////////////////////////////////////////////////////
   //
   STDMETHOD (GetData) (FORMATETC *pFormatEtcIn,
                        STGMEDIUM *pMedium)
   {
      if (!pFormatEtcIn || (pFormatEtcIn->cfFormat != m_Format.cfFormat))
         return DV_E_FORMATETC;

      if (!(pFormatEtcIn->tymed & TYMED_HGLOBAL))
         return DV_E_TYMED;

      if (pFormatEtcIn->dwAspect != DVASPECT_CONTENT)
         return DV_E_DVASPECT;

      if (pFormatEtcIn->lindex != -1)
         return DV_E_LINDEX;

      CopyMemory (pMedium, &m_Medium, sizeof (STGMEDIUM));
      pMedium->pUnkForRelease->AddRef ();
      return S_OK;
   }

   /////////////////////////////////////////////////////////////////////////////
   //
   STDMETHOD (GetDataHere) (FORMATETC *pFormatEtcIn,
                            STGMEDIUM *pmedium)
   {
      ATLASSERT (!"Not yet implemented");
      return E_FAIL;
   }

   /////////////////////////////////////////////////////////////////////////////
   //
   STDMETHOD (QueryGetData) (FORMATETC *pFormatEtcIn)
   {
      if (!pFormatEtcIn)
         return DV_E_FORMATETC;

      if (!(pFormatEtcIn->tymed & TYMED_HGLOBAL))
         return DV_E_TYMED;

      if (pFormatEtcIn->dwAspect != DVASPECT_CONTENT)
         return DV_E_DVASPECT;

      if (pFormatEtcIn->lindex != -1)
         return DV_E_LINDEX;

      return S_OK;
   }

   /////////////////////////////////////////////////////////////////////////////
   //
   STDMETHOD (GetCanonicalFormatEtc) (FORMATETC *pformatectIn,
                                      FORMATETC *pformatetcOut)
   {
      return E_UNEXPECTED;
   }

   /////////////////////////////////////////////////////////////////////////////
   //
   STDMETHOD (SetData) (FORMATETC *pformatetc,
                        STGMEDIUM *pmedium,
                        BOOL fRelease)
   {
      ClearData ();
      CopyMemory (&m_Format, pformatetc, sizeof (FORMATETC));
      CopyMemory (&m_Medium, pmedium, sizeof (STGMEDIUM));
      m_bRelease = fRelease;

      return S_OK;
   }

   /////////////////////////////////////////////////////////////////////////////
   //
   STDMETHOD (EnumFormatEtc) (DWORD dwDirection,
                              IEnumFORMATETC ** ppenumFormatEtc)
   {
      typedef CComObject <CComEnum <IEnumFORMATETC, &IID_IEnumFORMATETC,
                              FORMATETC, _Copy<FORMATETC> > > CComFormatEtcEnum;

      CComFormatEtcEnum * pFee = 0;
      CComFormatEtcEnum::CreateInstance (&pFee);
      ATLASSERT (pFee);

      pFee->Init (&m_Format, &m_Format + 1,
                  static_cast <IUnknown*> (this), AtlFlagNoCopy);
      
      return pFee->QueryInterface (IID_IEnumFORMATETC,
                                   reinterpret_cast<void**>(ppenumFormatEtc));
   }

   /////////////////////////////////////////////////////////////////////////////
   //
   STDMETHOD (DAdvise) (FORMATETC *pformatetc,
                        DWORD advf,
                        IAdviseSink *pAdvSink,
                        DWORD *pdwConnection)
   {
      return OLE_E_ADVISENOTSUPPORTED;
   }

   /////////////////////////////////////////////////////////////////////////////
   //
   STDMETHOD (DUnadvise) (DWORD dwConnection)
   {
      return OLE_E_ADVISENOTSUPPORTED;
   }

   /////////////////////////////////////////////////////////////////////////////
   //
   STDMETHOD (EnumDAdvise) (IEnumSTATDATA **ppenumAdvise)
   {
      return OLE_E_ADVISENOTSUPPORTED;
   }

private:
   FORMATETC m_Format;
   STGMEDIUM m_Medium;
   BOOL      m_bRelease;
};

////////////////////////////////////////////////////////////////////////////////
//
template <class T>
class IDropSourceImpl : public IDropSource
{
   // IDropSourceImpl
public:
   /////////////////////////////////////////////////////////////////////////////
   // Use this function to begin a drag grop
   HRESULT DoDrag (IDataObject * pDataObject,
                   DWORD dwOKEffect,
                   DWORD * pdwEffect)
   {
      return ::DoDragDrop (pDataObject, static_cast <IDropSource*>(this),
                           dwOKEffect, pdwEffect);
   }

   /////////////////////////////////////////////////////////////////////////////
   // Default implementation (Simulated dynamic binding)
   HRESULT IDropSource_QueryContinueDrag (BOOL fEscapePressed, DWORD dwKeyState)
   {
      if (fEscapePressed) return DRAGDROP_S_CANCEL;
      else if (!(dwKeyState & MK_LBUTTON)) return DRAGDROP_S_DROP;
      else return S_OK;
   }

   /////////////////////////////////////////////////////////////////////////////
   // Default implementation (Simulated dynamic binding)
   HRESULT IDropSource_GiveFeedback (DWORD dwEffect)
   {
      return DRAGDROP_S_USEDEFAULTCURSORS;
   }

   // IDropSource
public:
   /////////////////////////////////////////////////////////////////////////////
   //
   STDMETHOD (QueryContinueDrag) (BOOL fEscapePressed, DWORD dwKeyState)
   {
      return static_cast<T*>(this)->
         IDropSource_QueryContinueDrag (fEscapePressed, dwKeyState);
   }

   /////////////////////////////////////////////////////////////////////////////
   //
   STDMETHOD (GiveFeedback) (DWORD dwEffect)
   {
      return static_cast<T*>(this)->
         IDropSource_GiveFeedback (dwEffect);
   }
};
typedef CComObject <CSimpleDataObject> CDataObject;

////////////////////////////////////////////////////////////////////////////////
//
template <class T>
class IDropTargetImpl : public IDropTarget
{
   // IDropTargetImpl
public:
   /////////////////////////////////////////////////////////////////////////////
   // Registers the current window as a drag drop target
   BOOL RegisterDragDrop (void)
   {
      HRESULT hr = ::RegisterDragDrop (static_cast <T*>(this)->m_hWnd,
                                       static_cast <IDropTarget*>(this));
      return (hr == S_OK) ? TRUE : FALSE;
   }

   /////////////////////////////////////////////////////////////////////////////
   // Unregisters the current window as a drag drop target
   BOOL RevokeDragDrop (void)
   {
      HRESULT hr = ::RevokeDragDrop (static_cast<T*>(this)->m_hWnd);
      return (hr == S_OK) ? TRUE : FALSE;
   }

   /////////////////////////////////////////////////////////////////////////////
   // Default implementation (Simulated dynamic binding)
   HRESULT IDropTarget_DragEnter (IDataObject *pDataObj, DWORD dwKeyState,
                                  POINTL pt, DWORD *pdwEffect)
   {
      *pdwEffect = DROPEFFECT_NONE;
      return S_OK;
   }

   /////////////////////////////////////////////////////////////////////////////
   // Default implementation (Simulated dynamic binding)
   HRESULT IDropTarget_DragOver (DWORD dwKeyState, POINTL pt, DWORD *pdwEffect)
   {
      *pdwEffect = DROPEFFECT_NONE;
      return S_OK;
   }

   /////////////////////////////////////////////////////////////////////////////
   // Default implementation (Simulated dynamic binding)
   HRESULT IDropTarget_DragLeave (void)
   {
      return S_OK;
   }

   /////////////////////////////////////////////////////////////////////////////
   // Default implementation (Simulated dynamic binding)
   HRESULT IDropTarget_Drop (IDataObject *pDataObj, DWORD dwKeyState,
                             POINTL pt, DWORD *pdwEffect)
   {
      *pdwEffect = DROPEFFECT_NONE;
      return S_OK;
   }

   // IDropTarget
public:
   /////////////////////////////////////////////////////////////////////////////
   //
   STDMETHOD (DragEnter) (IDataObject *pDataObj, DWORD grfKeyState,
                          POINTL pt, DWORD *pdwEffect)
   {
      return static_cast<T*>(this)->
         IDropTarget_DragEnter (pDataObj, grfKeyState, pt, pdwEffect);
   }

   /////////////////////////////////////////////////////////////////////////////
   //
   STDMETHOD (DragOver) (DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
   {
      return static_cast<T*>(this)->
         IDropTarget_DragOver (grfKeyState, pt, pdwEffect);
   }

   /////////////////////////////////////////////////////////////////////////////
   //
   STDMETHOD (DragLeave) (void)
   {
      return static_cast<T*>(this)->IDropTarget_DragLeave ();
   }

   /////////////////////////////////////////////////////////////////////////////
   //
   STDMETHOD (Drop) (IDataObject *pDataObj, DWORD grfKeyState,
                     POINTL pt, DWORD *pdwEffect)
   {
      return static_cast<T*>(this)->
         IDropTarget_Drop (pDataObj, grfKeyState, pt, pdwEffect);
   }
};

#endif // #ifndef __DRAGDROP_H__
