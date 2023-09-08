/*****************************************************************************
*
* File :     RWSAttribEdit.h
*
* Abstract : Helper class for RWS attribute editor controls.
*
*            Handles editing of multiple attributes per control, saving
*            attribute data, relevant incoming API events, undo information,
*            and drawing of the attribute name text in the UI.
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
* Copyright © 1993 - 2002 Criterion Software Ltd. All rights reserved.
* All Rights Reserved.
*
*****************************************************************************/

#ifndef __RWS_ATTRIB_EDIT_H__
#define __RWS_ATTRIB_EDIT_H__

/* -------------------------------------------------------------------------- */
template<typename Deriving>
class __declspec(novtable) IRWSAttribEditImpl : public IRWSAttribEdit
{
protected: // ctor (must derive from this class)
   /* ----------------------------------------------------------------------- */
   IRWSAttribEditImpl () : m_pUndoEvent (0), m_CommandID (0), m_ClassID (0),
                           m_ClassUID(0), m_szAttribName (0),
                           m_szAttribHelp (0), m_pDefaultAttribData (0),
                           m_nAttribDataSize (0),
                           m_bSettingAttribData (RWSFalse)
   {
      memset (&m_CommandData, 0, sizeof (RWSCommand));
   }

public: // dtor
   /* ----------------------------------------------------------------------- */
   virtual ~IRWSAttribEditImpl ()
   {
      // clean up data
      RWSCommandFreeData (&m_CommandData);
      m_EntityToAttribMap.clear ();
      delete [] m_ClassUID;
      delete [] m_szAttribName;
      delete [] m_szAttribHelp;
      delete [] m_pDefaultAttribData;
   }

public:
   // IRWSAttribEdit methods
   /* ----------------------------------------------------------------------- */
   STDMETHOD(Advise)(/*[in]*/ IRWSAttribEditEvents *pSource)
   {
      if (pSource)
      {
         m_pUndoEvent = pSource;
         m_pUndoEvent->AddRef ();
         return S_OK;
      }
      else
      {
         return E_FAIL;
      }
   }

   /* ----------------------------------------------------------------------- */
   STDMETHOD(UnAdvise)()
   {
      ATLASSERT (m_pUndoEvent);
      m_pUndoEvent->Release ();

      return S_OK;
   }

   /* ----------------------------------------------------------------------- */
   STDMETHOD(CreateData)(/*[in]*/ RWSID AttributeID)
   {
      CreateAndPopulateData (AttributeID, m_pDefaultAttribData,
                             m_nAttribDataSize);

      return S_OK;
   }

   /* ----------------------------------------------------------------------- */
   STDMETHOD(SetCommandID)(/*([in]*/ RWSID CommandID)
   {
      ATLASSERT (CommandID);

      m_CommandID = CommandID;

      // get class ID and class data
      m_ClassID = RWSParentGetFirst (m_CommandID, RWSClassID);
      ATLASSERT (m_ClassID);
      // update the class UID
      delete [] m_ClassUID;
      RWSUInt32 UIDLen = RWSGetUID(m_ClassID, RWSClassID, 0, 0);
      ATLASSERT(UIDLen>0);
      m_ClassUID = new RWSChar[UIDLen];
      RWSGetUID(m_ClassID, RWSClassID, m_ClassUID, UIDLen);

      Deriving *This = static_cast<Deriving *>(this);

      // update previous command data
      RWSCommandFreeData (&m_CommandData);
      RWSCommandGet (CommandID, &m_CommandData);
      if (RWSENoError == RWSErrorGetLast () && m_CommandData.ParamList)
      {
         // get the default attribute data associated with this command
         if (m_nAttribDataSize = This->GetDefaultDataSize (
                            m_CommandData.ParamList, m_CommandData.ParamLength))
         {
            if (!m_pDefaultAttribData)
               m_pDefaultAttribData = new RWSByte[m_nAttribDataSize];

            This->GetDefaultData (m_pDefaultAttribData, m_CommandData.ParamList,
                                  m_CommandData.ParamLength);
         }
      }

      // update attribute name
      // get command name from param list
      delete [] m_szAttribName;
      m_szAttribName = 0;
      if (const RWSChar *szQuotedName = RWS::GetNthParam (
                            m_CommandData.ParamList, m_CommandData.ParamLength,
                            RWS::g_nDisplayNameParam))
      {
         // allocate enough space for length + null terminator - quotes
         if (_tcslen (szQuotedName) - 1 > 0)
         {
            // get attribute name
            m_szAttribName = new RWSChar [_tcslen (szQuotedName) - 1];
            if (!RWS::UnquoteParamString (szQuotedName, m_szAttribName))
            {
               delete m_szAttribName;
               m_szAttribName = 0;
            }
         }
      }

      // get help string from param list
      delete [] m_szAttribHelp;
      m_szAttribHelp = 0;
      if (const RWSChar *cszHelp = RWS::GetNthParam (m_CommandData.ParamList,
                                                     m_CommandData.ParamLength,
                                                     RWS::g_nHelpParam))
      {
         // remove quotes from help
         m_szAttribHelp = new RWSChar[_tcslen (cszHelp) + 1];
         RWS::UnquoteParamString (cszHelp, m_szAttribHelp);
      }

      if (This->IsWindow ())
      {
         This->CreateUI ();

         // create tooltip window
         m_WndTooltip.Create (This->m_hWnd);
      }

      return S_OK;
   }

   /* ----------------------------------------------------------------------- */
   STDMETHOD(AddEntity)(/*[in]*/ RWSID EntityID, /*[in]*/ RWSID AttribID)
   {
      ATLASSERT (EntityID);

      // if item isn't in the map, add it and update control state
      if (m_EntityToAttribMap.end () == m_EntityToAttribMap.find (EntityID))
      {
         m_EntityToAttribMap.insert (std::make_pair (EntityID, AttribID));

         Deriving *This = static_cast<Deriving *>(this);
         This->UpdateControlState ();
      }

      return S_OK;
   }

   /* ----------------------------------------------------------------------- */
   STDMETHOD(RemoveEntity)(/*[in]*/ RWSID EntityID)
   {
      ATLASSERT (EntityID);

      // remove entity from map, and update control state
      if (m_EntityToAttribMap.erase (EntityID))
      {
         Deriving *This = static_cast<Deriving *>(this);
         This->UpdateControlState ();
      }

      return S_OK;
   }

   /* ----------------------------------------------------------------------- */
   STDMETHOD(OnAttribChanged)(/*[in]*/ RWSID AttributeID)
   {
      if (m_bSettingAttribData) // we set the data, so don't respond to callback
         return S_OK;

      ATLASSERT (AttributeID);
      Deriving *This = static_cast<Deriving *>(this);
      This->UpdateControlState ();
      return S_OK;
   }

   /* ----------------------------------------------------------------------- */
   STDMETHOD(OnAttribAdded)(/*[in]*/ RWSID EntityID,
                            /*[in]*/ RWSID AttributeID)
   {
      ATLASSERT (EntityID);
      ATLASSERT (AttributeID);
      ATLASSERT (m_EntityToAttribMap.find (EntityID) !=
                 m_EntityToAttribMap.end ());

      // update entry in map
      EntityToAttribMap::iterator it = m_EntityToAttribMap.find (EntityID);
      if (it != m_EntityToAttribMap.end ())
         it->second = AttributeID;

      // update controls if we're not adding attribute in SaveDataToAttributes
      if (!m_bSettingAttribData)
      {
         Deriving *This = static_cast<Deriving *>(this);
         This->UpdateControlState ();
      }

      return S_OK;
   }

   /* ----------------------------------------------------------------------- */
   STDMETHOD(OnAttribRemoved)(/*[in]*/ RWSID EntityID,
                              /*[in]*/ RWSID AttributeID)
   {
      ATLASSERT (EntityID);
      ATLASSERT (AttributeID);
      ATLASSERT (m_EntityToAttribMap.find (EntityID) !=
                 m_EntityToAttribMap.end ());

      // update map entry to say it hasn't got an attribute anymore
      EntityToAttribMap::iterator it = m_EntityToAttribMap.find (EntityID);
      if (it != m_EntityToAttribMap.end ())
         it->second = 0;

      Deriving *This = static_cast<Deriving *>(this);
      This->UpdateControlState ();

      return S_OK;
   }

   /* ----------------------------------------------------------------------- */
   STDMETHOD(OnAttribDeleted)(/*[in]*/ RWSID AttributeID)
   {
      // find map entry with this attribute and update it
      EntityToAttribMap::iterator it = m_EntityToAttribMap.begin ();
      while (it != m_EntityToAttribMap.end ())
      {
         ATLASSERT (it->first);
         if (it->second == AttributeID)
         {
            it->second = 0;
            break;
         }
         ++it;
      }

      Deriving *This = static_cast<Deriving *>(this);
      This->UpdateControlState ();

      return S_OK;
   }

   /* ----------------------------------------------------------------------- */
   STDMETHOD(GetHeightExtra)(/*[out]*/ RWSUInt32 *Height)
   {
      *Height = 0;
      return S_OK;
   }

   /* ----------------------------------------------------------------------- */
   STDMETHOD(ExpandUI)(/*[in]*/ RWSBool bExpand)
   {
      ATLASSERT (!"No UI To Expand - provide your own override for ExpandUI.");
      return S_OK;
   }

   /* ----------------------------------------------------------------------- */
protected:
   // helper functions
   /* ----------------------------------------------------------------------- */
   void UpdateControlState ()
   {
      // check there's some UI to update
      Deriving *This = static_cast<Deriving *>(this);
      if (!This->IsWindow ())
         return;

      if (!m_pDefaultAttribData)
         return;

      ATLASSERT (m_nAttribDataSize);

      // get first entity's data
      EntityToAttribMap::const_iterator cit = m_EntityToAttribMap.begin ();
      if (cit == m_EntityToAttribMap.end ())
         return;

      RWSAttribute LastAttributeData;

      ATLASSERT (cit->first);
      if (RWSID AttributeID = cit->second)
      {
         RWSAttributeGet (AttributeID, &LastAttributeData);
         if (RWSErrorGetLast () != RWSENoError) // Failed!
         {
            memset (&LastAttributeData, 0, sizeof (RWSAttribute));
         }
         else if (!LastAttributeData.pData)
         {
            // if no attribute data, use default
            ATLASSERT (!LastAttributeData.DataSize);

            RWSAttributeFreeData (&LastAttributeData);
            memset (&LastAttributeData, 0, sizeof (RWSAttribute));

            LastAttributeData.pData    = m_pDefaultAttribData;
            LastAttributeData.DataSize = m_nAttribDataSize;
         }
      }
      else
      {
         // No attribute, so use default data
         memset (&LastAttributeData, 0, sizeof (RWSAttribute));
         LastAttributeData.pData    = m_pDefaultAttribData;
         LastAttributeData.DataSize = m_nAttribDataSize;
      }

      // next selected item
      ++cit;

      // Now loop over all entities in selection set, comparing the data with
      // the last item's data.  If data is different, break out as state is 
      // indeterminate.
      //
      // If no attribute data for an entry, compare last data against default.
      while (cit != m_EntityToAttribMap.end ())
      {
         ATLASSERT (cit->first);

         if (RWSID AttributeID = cit->second)
         {
            RWSAttribute AttributeData;
            RWSAttributeGet (AttributeID, &AttributeData);
            if (RWSENoError == RWSErrorGetLast ())
            {
               if (!AttributeData.pData)
               {
                  // if no attribute data, use default
                  ATLASSERT (!AttributeData.DataSize);

                  RWSAttributeFreeData (&AttributeData);
                  memset (&AttributeData, 0, sizeof (RWSAttribute));

                  AttributeData.pData    = m_pDefaultAttribData;
                  AttributeData.DataSize = m_nAttribDataSize;
               }

               if (AttributeData.DataSize != LastAttributeData.DataSize ||
                   memcmp (AttributeData.pData, LastAttributeData.pData,
                           AttributeData.DataSize))
               {
                  // data is different from last data, so clean up and break out
                  if (AttributeData.pData != m_pDefaultAttribData)
                     RWSAttributeFreeData (&AttributeData);

                  if (LastAttributeData.pData != m_pDefaultAttribData)
                     RWSAttributeFreeData (&LastAttributeData);

                  memset (&LastAttributeData, 0, sizeof (RWSAttribute));
                  break;
               }

               if (AttributeData.pData != m_pDefaultAttribData)
                  RWSAttributeFreeData (&AttributeData);
            }
         }
         else
         {
            // no attribute data, so compare default value with last
            if (m_nAttribDataSize != LastAttributeData.DataSize ||
                memcmp (m_pDefaultAttribData, LastAttributeData.pData,
                        m_nAttribDataSize))
            {
               // data is different, so state is mixed
               if (LastAttributeData.pData != m_pDefaultAttribData)
                  RWSAttributeFreeData (&LastAttributeData);

               memset (&LastAttributeData, 0, sizeof (RWSAttribute));
               break;
            }
         }

         // next selected item
         ++cit;
      }

      // update Deriving class's UI with current selection state
      This->UpdateControls (LastAttributeData.pData);

      if (LastAttributeData.pData != m_pDefaultAttribData)
         RWSAttributeFreeData (&LastAttributeData);
   }

   /* ----------------------------------------------------------------------- */
   // Returns bool - True:  Data is saved, so the UI must be changed to
   //                       reflect this.
   //                False: Data cannot be saved due to denied permission. UI
   //                       MUST be restored to previous state.
   //
   /* ----------------------------------------------------------------------- */
   bool SaveDataToAttributes (const RWSByte * const pData,
                              const RWSUInt32 nDatasize,
                              RWSBool bCreateUndo = RWSTrue)
   {
      // now update attribute data accordingly, creating undo data first
      RWSAttribute AttributeData;
      EntityToAttribMap::iterator it = m_EntityToAttribMap.begin ();
      while (it != m_EntityToAttribMap.end ())
      {
         ATLASSERT (it->first);

         // if no attribute, create one and add to entity
         RWSID AttributeID = it->second;
         if (!AttributeID)
         {
            if (AttributeID = RWSCreate (RWSAttributeID, 0))
            {
               // Create undo object for attribute create/add
               if (bCreateUndo)
               {
                  m_pUndoEvent->CreateUndo (AttributeID, it->first,
                                            m_szAttribName,
                                            RWSTrue); // bUndoCreate = RWSTrue
               }
               
               CreateAndPopulateData (AttributeID, pData, nDatasize);

               // Perform add child first to ensure entity is checked out.
               m_bSettingAttribData = RWSTrue;
               RWSChildAdd (it->first, AttributeID);
               m_bSettingAttribData = RWSFalse;

               if (RWSErrorGetLast () == RWSENotPermitted)
               {
                  RWSDelete (AttributeID);
                  return false;
               }

               it->second = AttributeID;
            }
         }
         else
         {
            // create undo object for attribute change
            if (bCreateUndo)
            {
               m_pUndoEvent->CreateUndo (AttributeID, it->first, m_szAttribName,
                                         RWSFalse); // bUndoCreate = RWSFalse
            }

            // set data
            RWSAttributeGet (AttributeID, &AttributeData);
            if (RWSENoError == RWSErrorGetLast ())
            {
               if (AttributeData.pData)
                  RWSFreeMemory (AttributeData.pData);

               AttributeData.pData = const_cast<RWSByte *>(pData);
               AttributeData.DataSize = nDatasize;
               
               m_bSettingAttribData = RWSTrue;
               RWSAttributeSet (AttributeID, &AttributeData);
               m_bSettingAttribData = RWSFalse;
               if (RWSENotPermitted == RWSErrorGetLast ())
                  return false;
               
               // clean up
               AttributeData.pData = 0; // set to null before data is freed
               RWSAttributeFreeData (&AttributeData);
            }
         }
         ++it;
      } // while (it != m_EntityToAttribMap.end ())
      return true;
   }

   /* ----------------------------------------------------------------------- */
   // Creates the attribute data and populates it with the relevant data.
   /* ----------------------------------------------------------------------- */
   void CreateAndPopulateData (RWSID AttributeID, const RWSByte * const pData,
                               const RWSUInt32 nDatasize)
   {
      ATLASSERT (AttributeID);
      
      // create default settings for attribute
      RWSAttribute AttributeData = {0};
      
      // UID
      AttributeData.ClassUID = m_ClassUID;
      
      // set name of class member variable
      if (m_CommandData.ParamList)
      {
         if (const RWSChar *szName = RWS::GetNthParam (
                           m_CommandData.ParamList, m_CommandData.ParamLength,
                           RWS::g_nMemberParam))
         {
            // allocate enough space for length + null terminator
            ATLASSERT (_tcslen (szName) - 1 > 0);
            AttributeData.Member = reinterpret_cast<RWSChar *>(
               _alloca (sizeof (RWSChar) * (_tcslen (szName) + 1)));
            
            _tcscpy (AttributeData.Member, szName);
         }
      }
      
      Deriving *This = static_cast<Deriving *>(this);
      
      // type
      if (RWSUInt32 nLen =
         This->GetNumDefaultTypeChars (m_CommandData.ParamList,
         m_CommandData.ParamLength))
      {
         AttributeData.Type = reinterpret_cast<RWSChar *>(
            _alloca (sizeof (RWSChar) * (nLen + 1)));
         This->GetDefaultType (AttributeData.Type, m_CommandData.ParamList,
            m_CommandData.ParamLength);
      }
      
      // platform
      AttributeData.Platform = 0;

      // size and data
      AttributeData.DataSize = nDatasize;
      AttributeData.pData = const_cast<RWSByte *>(pData);
      
      m_bSettingAttribData = RWSTrue;
      RWSAttributeSet (AttributeID, &AttributeData);
      m_bSettingAttribData = RWSFalse;
   }

   /* ----------------------------------------------------------------------- */
   void DrawAttribText (HDC hDC, const RECT &Rect) const
   {
      ATLASSERT (m_szAttribName);

      if (int nLen = _tcslen (m_szAttribName))
      {
         RECT r = Rect;

         // set background colour and font
         COLORREF OldBKCol = SetBkColor (hDC, GetSysColor (COLOR_BTNFACE));
         HGDIOBJ Obj = SelectObject (hDC, GetStockObject (ANSI_VAR_FONT));

         DrawText (hDC, m_szAttribName, nLen, &r,
                   DT_WORD_ELLIPSIS | DT_SINGLELINE | DT_NOPREFIX |
                   DT_LEFT | DT_VCENTER);

         // restore previous font and colour
         SelectObject (hDC, Obj);
         SetBkColor (hDC, OldBKCol);
      }
      return;
   }

   /* ----------------------------------------------------------------------- */
   void HandleTooltip (RWSBool bShowTip)
   {
      if (!m_WndTooltip.IsWindow () || !m_szAttribHelp)
         return;

      ATLASSERT (m_szAttribName);

      bool bDisplayTip = false;
      if (bShowTip)
      {
         // Set capture so we can hide the tooltip when mouse is outside window
         TRACKMOUSEEVENT tme = {0};
         tme.cbSize    = sizeof (TRACKMOUSEEVENT);
         tme.dwFlags   = TME_LEAVE;
         tme.hwndTrack = static_cast<Deriving *>(this)->m_hWnd;
         TrackMouseEvent (&tme);

         // Finally got the text, now get its extents
         HDC dc = GetDC (tme.hwndTrack);
         HFONT OldFont = reinterpret_cast<HFONT>(
            SelectObject (dc, reinterpret_cast<HFONT>(
                                              GetStockObject (ANSI_VAR_FONT))));
         SIZE TextSize;
         if (GetTextExtentPoint32 (dc, m_szAttribName, _tcslen (m_szAttribName),
                                   &TextSize))
         {
            RECT rcTooltip;
            rcTooltip.left   = 2;
            rcTooltip.top    = 2;
            rcTooltip.right  = rcTooltip.left + TextSize.cx;
            rcTooltip.bottom = rcTooltip.top + TextSize.cy;

            POINT pt;
            // Show tooltip if necessary
            if (GetCursorPos (&pt) && ScreenToClient (tme.hwndTrack, &pt) &&
                PtInRect (&rcTooltip, pt))
            {
               m_WndTooltip.Show (m_szAttribHelp, &rcTooltip);
               bDisplayTip = true;
            }
         }
         ReleaseDC (tme.hwndTrack, dc);
      }

      // Hide tooltip if it doesn't need displaying
      if (!bDisplayTip)
         m_WndTooltip.Hide ();
   }

   /* ----------------------------------------------------------------------- */
protected:
   // Data
   IRWSAttribEditEvents *m_pUndoEvent;
   RWSID m_CommandID;
   RWSID m_ClassID;
   RWSChar *m_ClassUID;
   RWSCommand m_CommandData;
   RWSChar *m_szAttribName;
   RWSChar *m_szAttribHelp;
   RWSByte *m_pDefaultAttribData;
   RWSUInt32 m_nAttribDataSize;
   // Flag to say we're editing data.  This is used so we don't respond to the
   // RWS API callback fired when we call RWSAttributeSet.
   RWSBool m_bSettingAttribData;

   // Map entity ID to associated attribute
   typedef std::map<RWSID, RWSID> EntityToAttribMap;
   EntityToAttribMap m_EntityToAttribMap;
   RWS::CSimpleToolTip m_WndTooltip;
   /* ----------------------------------------------------------------------- */
};

#endif //__RWS_ATTRIB_EDIT_H__
