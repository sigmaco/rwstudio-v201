// RWSAssetListCtrl.cpp : Implementation of CRWSAssetListCtrl

#include "stdafx.h"
#include "RWSAssetListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif // _DEBUG

/* -------------------------------------------------------------------------- */
namespace
{
   const int g_nHeightContracted = 24;

   void PopulateCombo (CComboBox &WndCombo)
   {
      WndCombo.ResetContent ();

      // Query all assets in database
      // (Attribute editor controls are already attached to the database)
      RWSID AssetID = RWSGetFirst (RWSAssetID);
      while (AssetID)
      {
         RWSAsset AssetData = {0};
         RWSAssetGet (AssetID, &AssetData);

         // Add asset's name to combo box
         if (AssetData.Name)
         {
            int nSel = WndCombo.AddString (AssetData.Name);
            if (nSel != CB_ERR)
               WndCombo.SetItemData (nSel, AssetID);
         }
         RWSAssetFreeData (&AssetData);
         AssetID = RWSGetNext (AssetID);
      }
   }
}

/* -------------------------------------------------------------------------- */
HRESULT CRWSAssetListCtrl::OnDraw (ATL_DRAWINFO& di)
{
	const RECT& rc = *reinterpret_cast<const RECT *>(di.prcBounds);
   HDC hDC = di.hdcDraw;

   // update attribute text
   if (m_szAttribName)
   {
      RECT rcText = rc;
      rcText.left += 2;
      rcText.right /= 2;
      rcText.bottom = rcText.top + g_nHeightContracted;
      DrawAttribText (hDC, rcText);
   }

   return S_OK;
}


/* -------------------------------------------------------------------------- */
RWSUInt32 CRWSAssetListCtrl::GetNumDefaultTypeChars (const RWSChar * const,
                                                     RWSUInt32)
{
   return wcslen (L"RwChar");
}

/* -------------------------------------------------------------------------- */
void CRWSAssetListCtrl::GetDefaultType (RWSChar * const szType,
                                        const RWSChar * const,
                                        RWSUInt32)
{
   wcscpy (szType, L"RwChar");
}

/* -------------------------------------------------------------------------- */
RWSUInt32 CRWSAssetListCtrl::GetDefaultDataSize (const RWSChar * const,
                                                 RWSUInt32)
{
   // 2 unsigned int header
   RWSUInt32 nDataSize = 2 * sizeof (RWSUInt32);

   // + 1 Char for an empty string
   nDataSize += sizeof (RWSChar);

   return nDataSize;
}

/* -------------------------------------------------------------------------- */
void CRWSAssetListCtrl::GetDefaultData (RWSByte * const pData,
                                        const RWSChar * const,
                                        RWSUInt32)
{
   // fill in first data tag header here
   RWSUInt32 *pDataHeader = reinterpret_cast<RWSUInt32 *>(pData);
   // specify data type
   pDataHeader[0] = RWSTypeChar;
   // specify the number of data items
   pDataHeader[1] = 1; // just 1 item

   // fill in the default data size - just an empty string
   RWSChar *szDefaultType = reinterpret_cast<RWSChar *>(pDataHeader + 2);
   *szDefaultType = 0; // null terminator
}

/* -------------------------------------------------------------------------- */
void CRWSAssetListCtrl::CreateUI ()
{
   ATLASSERT (IsWindow ());

   if (!m_WndCombo.IsWindow ())
   {
      // create the combo box control
      RECT rc;
      GetClientRect (&rc);
      rc.top += 2;
      rc.bottom = 200; // specify combo's initial drop-height
      rc.left = rc.right / 2;
	   m_WndCombo.Create (m_hWnd, rc, 0, WS_VISIBLE | WS_CHILD |
                                        CBS_DROPDOWNLIST | CBS_SORT |
                                        CBS_HASSTRINGS | WS_VSCROLL);
      ATLASSERT (m_WndCombo.IsWindow ());

      // set a nicer font
      m_WndCombo.SetFont (HFONT (GetStockObject (ANSI_VAR_FONT)));

      PopulateCombo (m_WndCombo);
   }
}

/* -------------------------------------------------------------------------- */
void CRWSAssetListCtrl::UpdateControls (const RWSByte * const pData)
{
   // update controls from the given data
   int nSelection = -1; // default to nothing selected

   // null pData represents multiple attributes edited simultanously with no like-data.
   if (const RWSUInt32 *pDataHeader = reinterpret_cast<const RWSUInt32 *>(pData))
   {
      // validate data header
      ATLASSERT (RWSTypeChar == pDataHeader[0]);

      // locate the UID in the database
      if (pDataHeader[1] > 1) // string has some characters
      {
         const RWSChar *szAssetID = reinterpret_cast<const RWSChar *>(pDataHeader + 2);

         if (RWSID AssetID = RWSGetID (RWSAssetID, szAssetID))
         {
            RWSAsset AssetData = {0};
            RWSAssetGet (AssetID, &AssetData);

            // find the string in the combo box
            if (AssetData.Name)
               nSelection = m_WndCombo.FindStringExact (-1, AssetData.Name);

            RWSAssetFreeData (&AssetData);
         }
      }
   }

   // update the UI
   m_WndCombo.SetCurSel (nSelection);
}

/* -------------------------------------------------------------------------- */
LRESULT CRWSAssetListCtrl::OnMouseMove (UINT, WPARAM, LPARAM, BOOL &bHandled)
{
   bHandled = FALSE;
   HandleTooltip (RWSTrue); // bShowTooltip = true
   return 0;
}

/* -------------------------------------------------------------------------- */
LRESULT CRWSAssetListCtrl::OnMouseLeave (UINT, WPARAM, LPARAM, BOOL &bHandled)
{
   bHandled = FALSE;
   HandleTooltip (RWSFalse); // bShowTooltip = false
   return 0;
}

/* -------------------------------------------------------------------------- */
LRESULT CRWSAssetListCtrl::OnSize (UINT, WPARAM, LPARAM lParam, BOOL &)
{
   // set position of combo box
   if (m_WndCombo.IsWindow ())
   {
      RECT rcCombo;
      m_WndCombo.GetClientRect (&rcCombo);

      int x = LOWORD (lParam) / 2;
      m_WndCombo.SetWindowPos (0, x, 2, x, rcCombo.bottom, SWP_NOZORDER);
   }
   return 0;
}

/* -------------------------------------------------------------------------- */
LRESULT CRWSAssetListCtrl::OnSelEndOK (WORD wNotifyCode, WORD wID,
                                       HWND hWndCtl, BOOL& bHandled)
{
   // called when combo box selection has changed
   // get current selection
   int nSel = m_WndCombo.GetCurSel ();
   if (nSel != CB_ERR)
   {
      // get the item data (RWSID of asset)
      RWSID AssetID = m_WndCombo.GetItemData (nSel);
      ATLASSERT (AssetID);

      // inquire the UID of the asset object
      if (RWSUInt32 nChars = RWSGetUID (AssetID, RWSAssetID, 0, 0))
      {
         // allocate some stack memory large enough for header, and UID
         int nDataSize = 2 * sizeof (RWSUInt32);
         nDataSize += nChars * sizeof (RWSChar);
         RWSByte *pData = reinterpret_cast<RWSByte *>(_alloca (nDataSize));

         // set up the header
         RWSUInt32 *DataHeader = reinterpret_cast<RWSUInt32 *>(pData);
         DataHeader[0] = RWSTypeChar; // Data type
         DataHeader[1] = nChars;  // Number of data items

         // copy the UID
         RWSChar *szAssetUID = reinterpret_cast<RWSChar *>(DataHeader + 2);
         *szAssetUID = 0;
         RWSGetUID (AssetID, RWSAssetID, szAssetUID, nChars);

         // now save the data
         if (!SaveDataToAttributes (pData, nDataSize)) // save with undo
         {
            // This call can fail if the AlienBrain check-out for the entity was
            // denied.
            //
            // To fix this, the last selection should be saved as a member variable
            // and set if successful, or restored if failed.
            // Code elided for clarity ;)
         }
      }
   }
   return 0;
}

/* -------------------------------------------------------------------------- */