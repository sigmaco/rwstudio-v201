#ifndef __RWS_AE_UTILITY_H__
#define __RWS_AE_UTILITY_H__

#include <commctrl.h>

namespace RWS
{
   const _TCHAR g_szAttributeCmd[]        = _T("RWS_ATTRIBUTE");
   const _TCHAR g_szPrivateAttributeCmd[] = _T("RWS_PRIVATE_ATTRIBUTE");
   const _TCHAR g_szMessageCmd  []        = _T("RWS_MESSAGE");
   const _TCHAR g_szSeparatorCmd[]        = _T("RWS_SEPARATOR");

   static const RWSChar *g_szDefaultValue = _T("DEFAULT(");
   static const RWSChar *g_szList         = _T("LIST(");
   static const RWSChar *g_szRange        = _T("RANGE(");
   static const RWSChar *g_szRanges       = _T("RANGES(");
   static const RWSChar *g_szReceive      = _T("RECEIVE");
   static const RWSChar *g_szUser         = _T("USER");

   static const int g_nMemberParam         = 0; // index of attribute name
   static const int g_nDisplayNameParam    = 1; // index of displayed name
   static const int g_nHelpParam           = 2; // index of help string
   static const int g_nDataTypeParam       = 4; // index of data type param
   static const int g_nMessageDefaultParam = 5; // default message param index

   /* ----------------------------------------------------------------------- */
   // stringified known data types
   struct DataType
   {
      DataType (const RWSChar *_szDataType, const RWSUInt32 _nDataType,
                const RWSUInt32 _nDataSize)
         : szDataType (_szDataType), nDataType (_nDataType),
           nDataSize (_nDataSize) {}

      const RWSChar *szDataType;
      const RWSUInt32 nDataType;
      const RWSUInt32 nDataSize;
   };

   /* ----------------------------------------------------------------------- */
   const DataType *FindDataType (const RWSChar * const szTypeName);

   const RWSChar *GetNthParam (const RWSChar * const szParamList,
                               RWSUInt32 szParamLength, RWSUInt32 nParam);

   const RWSChar *FindParamString (const RWSChar * const szParamList,
                                   RWSUInt32 ParamLength,
                                   const RWSChar * const szSearch);

   bool UnquoteParamString (const RWSChar * const szParam,
                            RWSChar * const szBuffer);

   // ----------------------------------------------------------------------- //
   // A basic tooltip implementation which places tooltips at given rectangle
   class CSimpleToolTip : public CWindow
   {
   public:
      // -------------------------------------------------------------------- //
      HWND Create (HWND hParentWnd)
      {
         m_hParentWnd = hParentWnd;

         // Create the tool tip control.
         CWindow::Create (TOOLTIPS_CLASS, m_hParentWnd, &CWindow::rcDefault);
         SendMessage (TTM_ACTIVATE, TRUE, 0L);

         // Add a tool
         TOOLINFO ti = {0};
         ti.cbSize   = sizeof (TOOLINFO);
         ti.hwnd     = m_hParentWnd;
         ti.uFlags   = TTF_IDISHWND;
         ti.uId      = reinterpret_cast<UINT>(ti.hwnd);
         ti.lpszText = _T("");
         SendMessage (TTM_ADDTOOL, 0, reinterpret_cast<LPARAM>(&ti));

         return m_hWnd;
      }

      // -------------------------------------------------------------------- //
      void Show (LPCTSTR cszText, LPRECT lpRect)
      {
         TOOLINFO ti = {0};
         ti.cbSize   = sizeof (TOOLINFO);
         ti.hwnd     = m_hParentWnd;
         ti.uFlags   = TTF_IDISHWND;
         ti.uId      = reinterpret_cast<UINT>(m_hParentWnd);
         ti.lpszText = const_cast<LPTSTR>(cszText);
         SendMessage (TTM_TRACKACTIVATE, TRUE, reinterpret_cast<LPARAM>(&ti));

         RECT rc = *lpRect;
         CWindow (m_hParentWnd).ClientToScreen (&rc);
         SendMessage (TTM_TRACKPOSITION, 0, MAKELPARAM (rc.left, rc.top));

         ti.uFlags |= TTF_TRACK | TTF_ABSOLUTE | TTF_TRANSPARENT;
         SendMessage (TTM_SETTOOLINFO, 0, reinterpret_cast<LPARAM>(&ti));
      }

      // -------------------------------------------------------------------- //
      void Hide ()
      {
         ShowWindow (SW_HIDE);
      }

   private:
      HWND m_hParentWnd;
   };

}

#endif __RWS_AE_UTILITY_H__
