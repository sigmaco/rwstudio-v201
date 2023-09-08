/*****************************************************************************
*
* File :     RWSAttribEditIF.h
*
* Abstract : COM Interface to be inherited by RWS attribute editor controls.
*
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

#ifndef __IRWSATTRIB_EDIT_INTERFACE_H__
#define __IRWSATTRIB_EDIT_INTERFACE_H__

interface __declspec(uuid("3E73B80D-D779-486d-889C-13F2CCFA6D33"))
IRWSAttribEditEvents : public IUnknown
{
public:
   STDMETHOD (CreateUndo) (RWSID AttributeID, RWSID ParentID, 
                           RWSChar *szAttribName, RWSBool bUndoCreate) = 0;
};

const IID IID_IRWSAttribEditEvents = __uuidof (IRWSAttribEditEvents);

interface __declspec(uuid("60CDE305-09A3-4966-84DB-255085359B75"))
IRWSAttribEdit : public IUnknown
{
public:
   STDMETHOD (Advise)(IRWSAttribEditEvents __RPC_FAR *pSource) = 0;
   STDMETHOD (UnAdvise)() = 0;
   STDMETHOD (CreateData)(RWSID AttributeID) = 0;
   STDMETHOD (SetCommandID)(RWSID CommandID) = 0;
   STDMETHOD (AddEntity)(RWSID EntityID, RWSID AttributeID) = 0;
   STDMETHOD (RemoveEntity)(RWSID EntityID) = 0;
   STDMETHOD (OnAttribChanged)(RWSID AttributeID) = 0;
   STDMETHOD (OnAttribAdded)(RWSID EntityID, RWSID AttributeID) = 0;
   STDMETHOD (OnAttribRemoved)(RWSID EntityID, RWSID AttributeID) = 0;
   STDMETHOD (OnAttribDeleted)(RWSID AttributeID) = 0;
   STDMETHOD (GetHeightExtra)(/*[out]*/RWSUInt32 __RPC_FAR *Height) = 0;
   STDMETHOD (ExpandUI)(RWSBool bExpand) = 0;
};

const IID IID_IRWSAttribEdit = __uuidof (IRWSAttribEdit);


#endif // __IRWSATTRIB_EDIT_INTERFACE_H__