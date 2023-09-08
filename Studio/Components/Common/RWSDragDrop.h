#ifndef RWSDRAGDROP_H
#define RWSDRAGDROP_H

#include <vector>
#include "RWSHints.h"
#include "DragDropImpl.h"

////////////////////////////////////////////////////////////////////////////////
// Creates the data for a drag operation
class CRWSDataObject : public CSimpleDataObject
{
public:
   CRWSDataObject () : m_DatabaseID (0), m_ParentID (0),
                       m_hNotifyWnd (0), m_uNotifyMsg (0) {}

   void Init (RWSDatabaseID DatabaseID, RWSID ParentID = 0);
   void SetNotify (HWND hWnd, UINT uMsg);
   void AddID (RWSID ID);
   RWSBool SetData ();
   void FinalRelease ();

private:
   static const UINT   m_szClipboardFormat;
   RWSDatabaseID       m_DatabaseID;
   RWSID               m_ParentID;
   HWND                m_hNotifyWnd;
   UINT                m_uNotifyMsg;
   std::vector <RWSID> m_IDVector;
};
typedef CComObject <CRWSDataObject> CRWSDataComObject;


////////////////////////////////////////////////////////////////////////////////
// Decodes the data available on a drop operation
class CRWSDropObject
{
public:
   typedef RWS::RWSClipData RWSClipData;

   CRWSDropObject (IDataObject * pDataObject, RWSDatabaseID DatabaseID);
   CRWSDropObject (HGLOBAL hData, CLIPFORMAT ClipFormat, RWSDatabaseID DatabaseID);
   ~CRWSDropObject ();

   RWSClipData * GetData ();
   operator RWSClipData* () const;
   RWSClipData* operator->() const;

   static const UINT   m_szClipboardFormat;

protected:
   HGLOBAL             m_hGlobal;
   RWS::RWSClipData *  m_pClipData;
};

////////////////////////////////////////////////////////////////////////////////
// Helper class for decoding dropped files
class CRWSFileDropObject
{
public:
   CRWSFileDropObject (IDataObject * pDataObject);
   UINT GetFileCount ();
   RWSChar * GetFileName (UINT nIndex);
   RWSChar * GetFileNameRelative (UINT nIndex, const RWSChar * const szPathName);
   operator BOOL () const;

protected:
   HDROP   m_hDrop;
   RWSChar m_szFileName [MAX_PATH];
};

#endif // RWSDRAGDROP_H