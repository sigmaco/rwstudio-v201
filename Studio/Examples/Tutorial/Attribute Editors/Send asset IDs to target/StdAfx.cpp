// stdafx.cpp : source file that includes just the standard includes
//  stdafx.pch will be the pre-compiled header
//  stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif // _DEBUG

#ifdef _ATL_STATIC_REGISTRY
#include <statreg.h>
#include <statreg.cpp>
#endif

#include <atlimpl.cpp>

const CLSID CLSID_RWSAssetListCtrl = {0X42A5BEC2,0X9867,0X492C,{0XBA,0XD3,0X5C,0X93,0X8D,0XB5,0XF5,0XF1}};

/* -------------------------------------------------------------------------- */
// display any memory leaks
#ifdef _DEBUG
namespace
{
   class CCrtMemoryCheck
   {
   public:
      CCrtMemoryCheck ()
      {
         int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG); // Get current flag
         flag |= _CRTDBG_LEAK_CHECK_DF; // Turn on leak-checking bit
         _CrtSetDbgFlag(flag); // Set flag to the new value
      }
      ~CCrtMemoryCheck ()
      {
         OutputDebugString (
               _T ("\n----------------------------------------------------\n"));
         OutputDebugString (
               _T ("RWSAssetList - Checking for memory leaks\n"));

         _CrtDumpMemoryLeaks (); // Dump memory leaks
         int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
         flag &= ~(_CRTDBG_LEAK_CHECK_DF);
         _CrtSetDbgFlag (flag); // Turn off checking

         OutputDebugString (
               _T ("\n----------------------------------------------------\n"));
         OutputDebugString (
               _T ("RWSAssetList - Finished checking for memory leaks\n\n"));
      }
   } g_CCrtMemoryCheck;
}

/* -------------------------------------------------------------------------- */

#endif // defined(_DEBUG)
