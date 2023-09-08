// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__42A5BEC2_9867_492C_BAD3_5C938DB5F5F1__INCLUDED_)
#define AFX_STDAFX_H__42A5BEC2_9867_492C_BAD3_5C938DB5F5F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#define _ATL_APARTMENT_THREADED

#include <atlbase.h>

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif // _DEBUG

//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>
#include <atlctl.h>
#include <atlhost.h>

#include <map> // for map of entity to attribute

// RWS API includes
#include "RWSCore.h"

#include "Utility.h"

// interface file
#include "RWSAttribEditIF.h"
#include "RWSAttribEdit.h"

extern const CLSID CLSID_RWSAssetListCtrl;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__42A5BEC2_9867_492C_BAD3_5C938DB5F5F1__INCLUDED)
