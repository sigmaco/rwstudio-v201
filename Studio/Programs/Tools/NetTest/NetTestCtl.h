/*****************************************************************************
 *
 * File :     NetTestCtl.h
 *
 * Abstract:  Header file
 *
 *****************************************************************************
 *
 * This file is a product of Criterion Software Ltd.
 *
 * This file is provided as is with no warranties of any kind and is
 * provided without any obligation on Criterion Software Ltd. or
 * Canon Inc. to assist in its use or modification.
 *
 * Criterion Software Ltd. will not, under any
 * circumstances, be liable for any lost revenue or other damages arising
 * from the use of this file.
 *
 * Copyright (c) 2004 Criterion Software Ltd.
 * All Rights Reserved.
 *
 * RenderWare is a trademark of Canon Inc.
 *
 *****************************************************************************/

// NetTestCtl.h

#ifndef __NETTESTCTL_H_
#define __NETTESTCTL_H_

#include "resource.h"       // main symbols
#include "NetTestCP.h"
#include <string>

using namespace std;

// -------------------------------------------------------------------------- //
class ATL_NO_VTABLE CNetTestCtl :
   public CComObjectRootEx<CComSingleThreadModel>,
   public CComCoClass<CNetTestCtl, &CLSID_NetTestCtl>,
   public IConnectionPointContainerImpl<CNetTestCtl>,
   public IDispatchImpl<INetTestCtl, &IID_INetTestCtl, &LIBID_NETTESTLib>,
   public IProvideClassInfo2Impl<&CLSID_NetTestCtl, 
                                 &DIID__INetTestCtlEvents,
                                 &LIBID_NETTESTLib>,
   public CProxy_INetTestCtlEvents< CNetTestCtl >,
   public INetTestCtlUtils
{
public:
DECLARE_REGISTRY_RESOURCEID(IDR_NETTESTCTL)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CNetTestCtl)
   COM_INTERFACE_ENTRY(INetTestCtl)
   COM_INTERFACE_ENTRY(INetTestCtlUtils)
   COM_INTERFACE_ENTRY(IDispatch)
   COM_INTERFACE_ENTRY(IConnectionPointContainer)
   COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
   COM_INTERFACE_ENTRY(IProvideClassInfo2)
   COM_INTERFACE_ENTRY(IProvideClassInfo)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CNetTestCtl)
   CONNECTION_POINT_ENTRY(DIID__INetTestCtlEvents)
END_CONNECTION_POINT_MAP()

public:
   // Ctor
   CNetTestCtl () : m_hThread (0), m_nPort (0),
                    m_bUnicode (false), m_bBigEndian (false), m_dwCookie (0) {}

public:
   // Init/Shutdown
   HRESULT FinalConstruct ();
   void FinalRelease ();

public:
   static void SendDisconnectEvent (DWORD dwCookie, UINT nErrorCode);
   static void SendMessageEvent (DWORD dwCookie, BSTR strMessage);
   static long GetPort (DWORD dwCookie);
   static bool IsBigEndian (DWORD dwCookie);
   static bool IsUnicode (DWORD dwCookie);
   static void Revoke (DWORD dwCookie);

public:
   // INetTestCtl
   STDMETHOD (Start) (long nPort, VARIANT_BOOL bUnicode,
                      VARIANT_BOOL bBigEndian);
   STDMETHOD (Stop) ();
   STDMETHOD (ShowStreamContents) (/*[in]*/BSTR strStreamFile);

   // INetTestCtlUtils - Allows access from worker thread through static fns
   STDMETHOD (FireDisconnect) (long nErrorCode)
   {
      // Reset these variables so clients can stop/start from message handler
      m_hThread = 0;
      m_dwCookie = 0;

      // A zero error code indicates stopped by user, so don't fire
      return nErrorCode ? Fire_OnDisconnect (nErrorCode) : S_OK;
   }
   STDMETHOD (FireMessage) (BSTR strMessage)
   {
      return Fire_OnMessage (strMessage);
   }
   STDMETHOD (GetPort) (long *pPort)
   {
      *pPort = m_nPort;
      return S_OK;
   }
   STDMETHOD (IsUnicode) (VARIANT_BOOL *pbUnicode)
   {
      *pbUnicode = m_bUnicode;
      return S_OK;
   }
   STDMETHOD (IsBigEndian) (VARIANT_BOOL *pbBigEndian)
   {
      *pbBigEndian = m_bBigEndian;
      return S_OK;
   }

private:
   // Data
   HANDLE m_hThread;
   long m_nPort;
   bool m_bUnicode;
   bool m_bBigEndian;
   DWORD m_dwCookie;
};

// -------------------------------------------------------------------------- //
#endif //__NETTESTCTL_H_
