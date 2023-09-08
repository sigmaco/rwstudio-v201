// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__23FA13F3_430D_45DA_9043_AFEA9375BABA__INCLUDED_)
#define AFX_STDAFX_H__23FA13F3_430D_45DA_9043_AFEA9375BABA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#define _ATL_APARTMENT_THREADED

//#define _ATL_DEBUG_QI
//#define _ATL_DEBUG_INTERFACES

#include <atlbase.h>
#include "RWSPreviewModule.h"
extern CRWSPreviewModule _Module;

#include <atlcom.h>
#include <atlctl.h>
#include "atlapp.h"
#include "atlmisc.h"

// RenderWare Includes
#include "rwcore.h"
#include "rpltmap.h"
#include "rtfsyst.h"
#include "rpworld.h"
#include "rphanim.h"
#include "rtcmpkey.h"
#include "rpskin.h"

// RenderWare Studio Includes
#include "api/inc/rwscore.h"

// Minimum distance between the near and far clip planes...
#define MINNEARTOFAR (0.01f)

// Initial camera horizontal field-of-view in degrees...    
#define INITHFOV (60.0f)

// Initial camera tilt and turn in degrees...    
#define INITTILTWORLD (-30.0f)
#define INITTILTCLUMP (0.0f)
#define INITTURNWORLD (0.0f)
#define INITTURNCLUMP (180.0f)
#define DEGTORAD(x) ((x) * rwPI / 180.0f)
#define CLIPFACTOR (5.0f)
#define NORMALSFACTOR (0.01f)
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__23FA13F3_430D_45DA_9043_AFEA9375BABA__INCLUDED)
