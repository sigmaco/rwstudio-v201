// RWSPreview.cpp : Implementation of WinMain

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "RWSPreview.h"

#include "RWSPreview_i.c"
#include "RWSPreviewControl.h"

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_RWSPreview, CRWSPreviewControl)
END_OBJECT_MAP()

/////////////////////////////////////////////////////////////////////////////
//
LPCTSTR FindOneOf(LPCTSTR p1, LPCTSTR p2)
{
    while (p1 != NULL && *p1 != NULL)
    {
        LPCTSTR p = p2;
        while (p != NULL && *p != NULL)
        {
            if (*p1 == *p)
                return CharNext(p1);
            p = CharNext(p);
        }
        p1 = CharNext(p1);
    }
    return NULL;
}

/////////////////////////////////////////////////////////////////////////////
//
extern "C" int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR lpCmdLine, int)
{
    OleInitialize (0);
    HRESULT hRes = CoInitialize(NULL);
    _ASSERTE(SUCCEEDED(hRes));

    _Module.Init(ObjectMap, hInstance, &LIBID_RWSPREVIEWLib);
    _Module.dwThreadID = GetCurrentThreadId();
    TCHAR szTokens[] = _T("-/");

    int nRet = 0;
    BOOL bRun = TRUE;
    LPCTSTR lpszToken = FindOneOf(lpCmdLine, szTokens);
    while (lpszToken != NULL)
    {
        if (lstrcmpi(lpszToken, _T("UnregServer"))==0)
        {
            _Module.UpdateRegistryFromResource(IDR_RWSPreview, FALSE);
            nRet = _Module.UnregisterServer(TRUE);
            bRun = FALSE;
            break;
        }
        if (lstrcmpi(lpszToken, _T("RegServer"))==0)
        {
            _Module.UpdateRegistryFromResource(IDR_RWSPreview, TRUE);
            nRet = _Module.RegisterServer(TRUE);
            bRun = FALSE;
            break;
        }
        lpszToken = FindOneOf(lpszToken, szTokens);
    }

    if (bRun)
    {
        _Module.StartMonitor();
        hRes = _Module.RegisterClassObjects(CLSCTX_LOCAL_SERVER, REGCLS_SINGLEUSE);
        _ASSERTE(SUCCEEDED(hRes));

        _Module.ProcessMessageLoop ();

        _Module.RevokeClassObjects();
        Sleep (CRWSPreviewModule::TIMEOUT); //wait for any threads to finish
    }

    _Module.Term();
    CoUninitialize();
    OleUninitialize ();
    return nRet;
}
