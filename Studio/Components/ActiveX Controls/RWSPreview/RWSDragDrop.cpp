#include "stdafx.h"
#include <string>
#include "RWSDragDrop.h"
#include "Common/RWSHints.h"
#include "API/Impl/Filepath.h"

using namespace RWS;

////////////////////////////////////////////////////////////////////////////////
// CRWSDataObject class
////////////////////////////////////////////////////////////////////////////////

const UINT CRWSDataObject::m_szClipboardFormat = 
   ::RegisterClipboardFormat (RWS::szClipboardFormat);

////////////////////////////////////////////////////////////////////////////////
//
void CRWSDataObject::Init (RWSDatabaseID DatabaseID, RWSID ParentID)
{
   m_DatabaseID = DatabaseID;
   m_ParentID = ParentID;
}

////////////////////////////////////////////////////////////////////////////////
//
void CRWSDataObject::SetNotify (HWND hWnd, UINT uMsg)
{
   m_hNotifyWnd = hWnd;
   m_uNotifyMsg = uMsg;
}

////////////////////////////////////////////////////////////////////////////////
//
void CRWSDataObject::FinalRelease ()
{
   if (m_uNotifyMsg && ::IsWindow (m_hNotifyWnd))
      ::SendMessage (m_hNotifyWnd, m_uNotifyMsg, 0, 0);
}

////////////////////////////////////////////////////////////////////////////////
//
void CRWSDataObject::AddID (RWSID ID)
{
   m_IDVector.push_back (ID);
}

////////////////////////////////////////////////////////////////////////////////
//
RWSBool CRWSDataObject::SetData ()
{
   using namespace RWS;

   // Early out if there is no data in the id vector
   if (m_IDVector.empty ())
      return RWSFalse;

   // Calculate the size in bytes of the structure
   UINT nDataSize = sizeof (RWSClipData) + (m_IDVector.size () * sizeof (RWSID));

   // Allocate global memory to hold the data
   HGLOBAL hGlobal = ::GlobalAlloc (GMEM_MOVEABLE, nDataSize);
   ATLASSERT (hGlobal);

   // Get a pointer to the allocated memory
   RWSClipData * pClipData =
      reinterpret_cast<RWSClipData*> (::GlobalLock (hGlobal));
   ATLASSERT (pClipData);

   // Set up the header
   pClipData->m_dwProcessID = ::GetCurrentProcessId ();
   pClipData->m_DatabaseID  = m_DatabaseID;
   pClipData->m_ParentID    = m_ParentID;
   pClipData->m_nCount      = 0;

   // Add the list of items to the vector
   std::vector <RWSID>::iterator iter = m_IDVector.begin ();
   while (iter != m_IDVector.end ())
   {
      pClipData->m_IDList [pClipData->m_nCount++] = *iter;
      ++iter;
   }

   // We've finished with the data
   ::GlobalUnlock (hGlobal);

   // Add the data to the clipboard
   return CSimpleDataObject::SetData (m_szClipboardFormat, hGlobal, TRUE);
}

////////////////////////////////////////////////////////////////////////////////
// CRWSDropObject class
////////////////////////////////////////////////////////////////////////////////

const UINT CRWSDropObject::m_szClipboardFormat = 
   ::RegisterClipboardFormat (RWS::szClipboardFormat);

////////////////////////////////////////////////////////////////////////////////
//
CRWSDropObject::CRWSDropObject (IDataObject * pDataObject,
                                RWSDatabaseID DatabaseID) : m_hGlobal (0),
                                                            m_pClipData (0)
{
   ATLASSERT (pDataObject);

   // Attempt to get the RWS clipboard data from 
   FORMATETC Format = {m_szClipboardFormat, 0, DVASPECT_CONTENT,
                       -1, TYMED_HGLOBAL};
   STGMEDIUM Storage = {0};

   if (SUCCEEDED (pDataObject->GetData (&Format, &Storage)))
   {
      ATLASSERT (Storage.hGlobal);
      m_hGlobal   = Storage.hGlobal;
      m_pClipData = reinterpret_cast<RWSClipData*>(::GlobalLock (m_hGlobal));
      DWORD Error = ::GetLastError ();

      ATLASSERT (m_pClipData);

      // If the database id's are not equal, then clean up
      if (!m_pClipData || (DatabaseID != m_pClipData->m_DatabaseID))
      {
         ::GlobalUnlock (m_hGlobal);
         m_hGlobal = 0;
         m_pClipData = 0;
      }
   }
}

////////////////////////////////////////////////////////////////////////////////
//
CRWSDropObject::CRWSDropObject (IDataObject * pDataObject) : m_hGlobal (0),
                                                             m_pClipData (0)
{
   ATLASSERT (pDataObject);

   // Attempt to get the RWS clipboard data from 
   FORMATETC Format = {m_szClipboardFormat, 0, DVASPECT_CONTENT,
                       -1, TYMED_HGLOBAL};
   STGMEDIUM Storage = {0};

   if (SUCCEEDED (pDataObject->GetData (&Format, &Storage)))
   {
      ATLASSERT (Storage.hGlobal);
      m_hGlobal   = Storage.hGlobal;
      m_pClipData = reinterpret_cast<RWSClipData*>(::GlobalLock (m_hGlobal));
      DWORD Error = ::GetLastError ();

      ATLASSERT (m_pClipData);

      // If the database id's are not equal, then clean up
      if (!m_pClipData)
      {
         ::GlobalUnlock (m_hGlobal);
         m_hGlobal = 0;
         m_pClipData = 0;
      }
   }
}


////////////////////////////////////////////////////////////////////////////////
//
CRWSDropObject::~CRWSDropObject ()
{
   if (m_hGlobal)
      ::GlobalUnlock (m_hGlobal);
}

////////////////////////////////////////////////////////////////////////////////
//
RWSClipData * CRWSDropObject::GetData ()
{
   return m_pClipData;
}

////////////////////////////////////////////////////////////////////////////////
//
CRWSDropObject::operator RWSClipData* () const
{
   return m_pClipData;
}

////////////////////////////////////////////////////////////////////////////////
//
RWSClipData* CRWSDropObject::operator->() const
{
   return m_pClipData;
}

////////////////////////////////////////////////////////////////////////////////
// CRWSFileDropObject class
////////////////////////////////////////////////////////////////////////////////

CRWSFileDropObject::CRWSFileDropObject (IDataObject * pDataObject) : m_hDrop (0)
{
   FORMATETC Format = {CF_HDROP, 0, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};
   STGMEDIUM Medium = {0};

   if (SUCCEEDED (pDataObject->GetData (&Format, &Medium)))
      m_hDrop = reinterpret_cast<HDROP>(Medium.hGlobal);
}

////////////////////////////////////////////////////////////////////////////////
//
UINT CRWSFileDropObject::GetFileCount ()
{
   if (m_hDrop) return ::DragQueryFile (m_hDrop, 0xFFFFFFFF, 0, 0);
   else         return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
RWSChar * CRWSFileDropObject::GetFileName (UINT nIndex)
{
   if (::DragQueryFile (m_hDrop, nIndex, m_szFileName, MAX_PATH))
      return m_szFileName;
   else
      return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
RWSChar * CRWSFileDropObject::GetFileNameRelative (UINT nIndex,
                                               const RWSChar * const szPathName)
{
   if (::DragQueryFile (m_hDrop, nIndex, m_szFileName, MAX_PATH))
   {
      // Convert the path into one relative to szPathName
      std::basic_string<RWSChar> strRelativePath;
      RWS::FilePath::MakeRelativePath (szPathName, m_szFileName,
                                       strRelativePath);
      lstrcpyW (m_szFileName, strRelativePath.c_str ());

      return m_szFileName;
   }
   else
      return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
CRWSFileDropObject::operator BOOL () const
{
   return (m_hDrop != 0);
}
