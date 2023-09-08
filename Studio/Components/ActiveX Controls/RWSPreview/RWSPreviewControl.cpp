// RWSPreviewControl.cpp : Implementation of CRWSPreviewControl

#include "stdafx.h"
#include "RWSPreview.h"
#include "RWSPreviewControl.h"
#include "RWSDragDrop.h"

///////////////////////////////////////////////////////////////////////////////
LRESULT CRWSPreviewControl::OnCreate (UINT, WPARAM, LPARAM, BOOL& bHandled)
{
   if (m_Engine.Initialise (m_hWnd))
   {
      if (RpLight* pAmbientLight = RpLightCreate (rpLIGHTAMBIENT))
         RpWorldAddLight(m_Engine.World (), pAmbientLight);

      if (RpLight* pDirectionalLight = RpLightCreate(rpLIGHTDIRECTIONAL ))
      {
         RwFrame* pFrame = RwFrameCreate ();
         RpLightSetFrame (pDirectionalLight, pFrame);
         RpWorldAddLight (m_Engine.World (), pDirectionalLight);
         RpLightSetRadius (pDirectionalLight, 1000.0f);
         RwFrameRotate(RpLightGetFrame (pDirectionalLight), &YAxis, 120.0f,rwCOMBINEPRECONCAT);
      }
   }

   // Register this window as a drop target
   ::RegisterDragDrop (m_hWnd, this);
   return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
LRESULT CRWSPreviewControl::OnSetFocus (UINT, WPARAM, LPARAM, BOOL& bHandled)
{
   Invalidate ();
   return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
LRESULT CRWSPreviewControl::OnDestroy (UINT, WPARAM, LPARAM, BOOL& bHandled)
{
   if (m_Engine.IsInitialised ())
   {
      // Shutdown renderware
      m_Engine.Shutdown ();
   }

   ::RevokeDragDrop (m_hWnd);
   bHandled = FALSE;
   return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
LRESULT CRWSPreviewControl::OnNcDestroy (UINT, WPARAM, LPARAM, BOOL& bHandled)
{
   // This is a little harsh, but if the parent window has died, then kill
   // this process.
   if (m_pPreviewNotify && FAILED (m_pPreviewNotify->Ping ()))
   {
      m_pPreviewNotify = 0;

      // Remove the idle handler
      _Module.SetIdleHandler (0);

      // Shut down the graphics
      if (m_Engine.IsInitialised ())
         m_Engine.Shutdown ();

      // Kill this window
      DestroyWindow ();

      // Kill this process
      _exit (0);
   }

   bHandled = FALSE;
   return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
LRESULT CRWSPreviewControl::OnSize (UINT, WPARAM, LPARAM lParam, BOOL& bHandled)
{
   if (m_Engine.World ().Camera ().IsInitialised ())
      m_Engine.World ().Camera ().Resize (LOWORD (lParam), HIWORD (lParam));

   return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
HRESULT CRWSPreviewControl::OnDraw(ATL_DRAWINFO& di)
{
   if (m_Engine.World ().IsInitialised ())
      m_Engine.World ().Render (m_hWnd, m_DrawSkeleton, m_DrawSkelaxis, m_DrawText, m_DrawWorld, 
                                m_DrawSectors, m_DrawWireFrame, m_DrawNormals, m_CurrentSector, m_ZTest);

   return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//
HRESULT CRWSPreviewControl::IDropTarget_DragEnter (IDataObject *pDataObject,
                                 DWORD dwKeyState, POINTL pt, DWORD *pdwEffect)
{
   FORMATETC Format = {CF_HDROP, 0, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};
   STGMEDIUM Medium = {0};
   m_DragEffect= DROPEFFECT_NONE;

   if (SUCCEEDED (pDataObject->GetData (&Format, &Medium)))
   {
      HDROP hDrop = reinterpret_cast<HDROP>(Medium.hGlobal);
      UINT nFileCount = ::DragQueryFile (hDrop, 0xFFFFFFFF, 0, 0);
      _TCHAR szBuffer [_MAX_PATH];

      for (UINT nIndex = 0; nIndex < nFileCount; nIndex++)
      {
         if (::DragQueryFile (hDrop, nIndex, szBuffer, _MAX_PATH) != -1)
         {
            RWSString strFile (szBuffer), strType, strTextures;
            if (SUCCEEDED (GetAssetDataFromFile (strFile, strType, strTextures)))
            {
               USES_CONVERSION;
               switch (GetAssetPlugin (W2A (strType.c_str ())))
               {
               case rwID_CLUMP:
                  m_DragEffect = DROPEFFECT_COPY;
                  break;

               case rwID_ATOMIC:
                  m_DragEffect = DROPEFFECT_COPY;
                  break;

               case rwID_HANIMANIMATION:
                  if (m_Engine.World ().GetClumpCount ())
                     m_DragEffect = DROPEFFECT_COPY;
                  break;

               case rwID_WORLD:
                  if (m_Engine.World ().IsInitialised ())
                  {
                     m_DragEffect = DROPEFFECT_COPY;
                  }
                  break;
               }
            }
         }
      }
   }
   else
   {
      CRWSDropObject DropObject (pDataObject);
      if ( DropObject.GetData() )
      {
         for (UINT nIndex = 0; nIndex < DropObject->m_nCount; nIndex++)
         {
            RWSDatabaseID DatabaseID = DropObject->m_DatabaseID;
            RWSID AssetID = DropObject->m_IDList [nIndex];

            RWSString strFile, strTextures, strType;
            if (SUCCEEDED (GetAssetDataFromAssetID (DatabaseID, AssetID, strFile, strType, strTextures)))
            {
               USES_CONVERSION;
               switch (GetAssetPlugin (W2A (strType.c_str ())))
               {
               case rwID_CLUMP:
                  m_DragEffect = DROPEFFECT_COPY;
                  break;

               case rwID_ATOMIC:
                  m_DragEffect = DROPEFFECT_COPY;
                  break;

               case rwID_HANIMANIMATION:
                  if (m_Engine.World ().GetClumpCount ())
                     m_DragEffect = DROPEFFECT_COPY;
                  break;

               case rwID_WORLD:
                  if (m_Engine.World ().IsInitialised ())
                  {
                     m_DragEffect = DROPEFFECT_COPY;
                  }
                  break;
               }
            }
         }
      }
   }
   *pdwEffect = m_DragEffect;
   return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//
HRESULT CRWSPreviewControl::IDropTarget_DragOver (DWORD dwKeyState, POINTL pt,
                                                  DWORD *pdwEffect)
{
   *pdwEffect = m_DragEffect;
   return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//
HRESULT CRWSPreviewControl::IDropTarget_DragLeave (void)
{
   return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//
HRESULT CRWSPreviewControl::IDropTarget_Drop (IDataObject *pDataObject,
                                 DWORD dwKeyState, POINTL pt, DWORD *pdwEffect)
{
   FORMATETC Format = {CF_HDROP, 0, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};
   STGMEDIUM Medium = {0};

   if (SUCCEEDED (pDataObject->GetData (&Format, &Medium)))
   {
      HDROP hDrop = reinterpret_cast<HDROP>(Medium.hGlobal);
      UINT nFileCount = ::DragQueryFile (hDrop, 0xFFFFFFFF, 0, 0);
      _TCHAR szBuffer [_MAX_PATH];

      for (UINT nIndex = 0; nIndex < nFileCount; nIndex++)
      {
         if (::DragQueryFile (hDrop, nIndex, szBuffer, _MAX_PATH) != -1)
         {
            RWSString strFile (szBuffer), strType, strTextures;
            if (SUCCEEDED (GetAssetDataFromFile (strFile, strType, strTextures)))
            {
               USES_CONVERSION;
               switch (GetAssetPlugin (W2A (strType.c_str ())))
               {
               case rwID_CLUMP:
                  LoadClump (strFile, strTextures);
                  break;

               case rwID_ATOMIC:
                  LoadClump (strFile, strTextures);
                  break;

               case rwID_HANIMANIMATION:
                  LoadAnimation (strFile);
                  break;

               case rwID_WORLD:
                  LoadWorld(strFile, strTextures);
                  break;
               }
            }
         }
      }
   }
   else
   {
      CRWSDropObject DropObject (pDataObject);
      if ( DropObject.GetData() )
      {
         for (UINT nIndex = 0; nIndex < DropObject->m_nCount; nIndex++)
         {
            RWSDatabaseID DatabaseID = DropObject->m_DatabaseID;
            RWSID AssetID = DropObject->m_IDList [nIndex];

            RWSString strFile, strTextures, strType;
            if (SUCCEEDED (GetAssetDataFromAssetID (DatabaseID, AssetID, strFile,
                                                   strType, strTextures)))
            {
               USES_CONVERSION;
               switch (GetAssetPlugin (W2A (strType.c_str ())))
               {
               case rwID_CLUMP:
                  LoadClump (strFile, strTextures);
                  break;

               case rwID_ATOMIC:
                  LoadClump (strFile, strTextures);
                  break;
               
               case rwID_HANIMANIMATION:
                  LoadAnimation (strFile);
                  break;

               case rwID_WORLD:
                  LoadWorld(strFile, strTextures);
                  break;
               }
            }
         }
      }
   }

   return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// Returns the RenderWare plugin id of the given string (if supported).
RwCorePluginID CRWSPreviewControl::GetAssetPlugin (RwChar * szAssetType)
{
   if (!strcmpi (szAssetType, "rwID_CLUMP"))
      return rwID_CLUMP;
   else if (!strcmpi (szAssetType, "rwID_HANIMANIMATION"))
      return rwID_HANIMANIMATION;
   else if (!strcmpi (szAssetType, "rwID_ATOMIC"))
      return rwID_ATOMIC;
   else if (!strcmpi (szAssetType, "rwID_WORLD"))
      return rwID_WORLD;
   else
      return rwID_NAOBJECT;
}

///////////////////////////////////////////////////////////////////////////////
//
void CRWSPreviewControl::ControlClump (LPARAM lParam)
{
   CPoint ptMouseCurrent (lParam);
   CPoint ptMouseDelta (m_MousePos.x - ptMouseCurrent.x, m_MousePos.y - ptMouseCurrent.y);
   RwReal XMove = 0.0f;
   RwReal YMove = 0.0f;
   RwReal ZMove = 0.0f;

   if (RW::RenderWareClump* pClump = m_Engine.World ().GetClumpAtIndex (0))
   {    
      RwReal ObjectRadius  = pClump->BoundingSphere ().radius;

      switch(m_Mode)
      {
      case ORBIT:
         // Rotate the clump
         pClump->Rotate (ORBIT_SPEED * ptMouseDelta.x, ORBIT_SPEED * -ptMouseDelta.y, m_Engine.World ().Camera ());
         break;

      case PAN:
         XMove = ObjectRadius * ptMouseDelta.x * PAN_SPEED;
         YMove = ObjectRadius * -ptMouseDelta.y * PAN_SPEED;

         // Translate clump on X, Y axis
         pClump->TranslateBy (XMove, YMove, 0.0f);
         break;

      case ZOOM:
         ZMove = ObjectRadius * ptMouseDelta.y * ZOOM_SPEED;

         // Translate clump on Z axis
         pClump->TranslateBy (0.0f, 0.0f, -ZMove);
         break;
      }

      Invalidate ();
      m_MousePos = ptMouseCurrent;
   }
}


///////////////////////////////////////////////////////////////////////////////
//
void CRWSPreviewControl::ControlWorld (LPARAM lParam)
{
   CPoint ptMouseCurrent (lParam);
   CPoint ptMouseDelta (m_MousePos.x - ptMouseCurrent.x, m_MousePos.y - ptMouseCurrent.y);
   RwReal XMove = 0.0f;
   RwReal YMove = 0.0f;
   RwReal ZMove = 0.0f;
   RwReal TranslateScaleFactor = m_Engine.World ().m_WorldSphere.radius * ORBIT_SPEED;

   if (m_Engine.World ().IsInitialised ())
   {    
      switch(m_Mode)
      {
      case ORBIT:
         XMove = m_Engine.World ().m_WorldSphere.radius * -ptMouseDelta.x * (ORBIT_SPEED / TranslateScaleFactor);
         YMove = m_Engine.World ().m_WorldSphere.radius * -ptMouseDelta.y * (ORBIT_SPEED / TranslateScaleFactor);

         m_Engine.World ().CameraPoint(YMove, XMove);
         break;

      case PAN:
         XMove = m_Engine.World ().m_WorldSphere.radius * -ptMouseDelta.x * PAN_SPEED;
         YMove = m_Engine.World ().m_WorldSphere.radius * -ptMouseDelta.y * PAN_SPEED;
         m_Engine.World ().Camera ().TranslateBy(XMove, YMove, 0.0f);
         break;

      case ZOOM:
         ZMove = m_Engine.World ().m_WorldSphere.radius * ptMouseDelta.y * ZOOM_SPEED;
         m_Engine.World ().Camera ().TranslateBy(0.0f, 0.0f, ZMove);
         break;
      }

      Invalidate ();
      m_MousePos = ptMouseCurrent;
   }
}

///////////////////////////////////////////////////////////////////////////////
//
LRESULT CRWSPreviewControl::OnMouseButton (UINT uMsg, WPARAM, LPARAM lParam, BOOL& bHandled)
{
   switch (uMsg)
   {
      case WM_LBUTTONDOWN:
         {
            if (!m_MButton)
            {
               m_LButton = TRUE;  
               SetCapture ();
            }
         }
         break;

      case WM_LBUTTONUP:
         {
            if (!m_MButton)
            {
               m_LButton = FALSE; 
               ReleaseCapture (); 
            }
         }
         break;

      case WM_MBUTTONDOWN: 
         {
            if (!m_LButton)
            {
               m_MButton = TRUE;  
               m_OldMode = m_Mode; 
               SetPan(); 
               SetCapture (); 
            }
         }
         break;

      case WM_MBUTTONUP: 
         {
            if (!m_LButton)
            {
               m_MButton = FALSE; 
               m_Mode = m_OldMode; 
               ReleaseCapture (); 
            }
         }
         break;
   }
   
   m_MousePos.SetPoint (GET_X_LPARAM (lParam), GET_Y_LPARAM (lParam));
   return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
LRESULT CRWSPreviewControl::OnMouseMove (UINT, WPARAM, LPARAM lParam, BOOL& bHandled)
{
   if (m_MButton)
   {
      // Orbit mode
      SHORT altKey = GetAsyncKeyState(VK_MENU);
      SHORT controlKey = GetAsyncKeyState(VK_CONTROL);

      // If ALT key has been pressed.
      if ( (altKey&0x00008000) != 0) 
      {
         // If CONTROL key has been pressed
         if ((controlKey&0x00008000) != 0)
         {
            SetZoom();  
         }
         else
         {
            SetOrbit(); 
         }
      } 
      else
      {
         // If Right mouse button is held down by its self,  then go into pan mode.
         SetPan();
      }    
   }

   if ((m_Engine.World ().IsBspLoaded()) && (m_Engine.IsInitialised ()))
   {
      if (m_LButton || m_MButton)
      {
         ControlWorld(lParam);
      }
   }
   else
   {
      if ((m_Engine.World ().GetClumpCount ()) && (m_LButton || m_MButton))
      {
         ControlClump(lParam);
      }
   }

   return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
void CRWSPreviewControl::OnIdle ()
{
   if (RW::RenderWareClump* pClump = m_Engine.World ().GetClumpAtIndex (0))
   {
      if (!m_LastTime)
         m_LastTime = timeGetTime ();
      DWORD CurrentTime = timeGetTime ();

      RwBool bComplete = FALSE;
      RwReal AnimTime = RwReal (CurrentTime - m_LastTime) * 0.001f;
      pClump->Anim ().SetAnimationTimeIncrement (AnimTime, &bComplete);

      // Render the world
      if (m_Engine.World ().IsInitialised ())
         m_Engine.World ().Render (m_hWnd, m_DrawSkeleton, m_DrawSkelaxis, m_DrawText, m_DrawWorld, 
                                   m_DrawSectors,m_DrawWireFrame, m_DrawNormals, m_CurrentSector, m_ZTest);

      if (bComplete && !m_Repeat)
      {
         StopAnimation ();
      }
      else
      {
         m_LastTime = CurrentTime;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
//
HRESULT CRWSPreviewControl::GetAssetDataFromAssetID (RWSDatabaseID DatabaseID,
                                    RWSID AssetID, RWSString& strFileName,
                                    RWSString& strType, RWSString& strTextures)
{
   if (m_pPreviewNotify)
   {
      BSTR strBstr (0);
      if (SUCCEEDED (m_pPreviewNotify->GetAssetFile (DatabaseID, AssetID, &strBstr)))
      {
         strFileName = strBstr;
         ::SysFreeString (strBstr);
      }

      if (SUCCEEDED (m_pPreviewNotify->GetAssetType (DatabaseID, AssetID, &strBstr)))
      {
         strType = strBstr;
         ::SysFreeString (strBstr);
      }

      if (SUCCEEDED (m_pPreviewNotify->GetAssetTextures (DatabaseID, AssetID, &strBstr)))
      {
         strTextures = strBstr;
         strTextures.append (RWSSTRING ("\\")); // RenderWare requires a backslash

         ::SysFreeString (strBstr);
      }

      return strFileName.size () ? S_OK : E_FAIL;
   }
   else
      return E_FAIL;
}

///////////////////////////////////////////////////////////////////////////////
//
HRESULT CRWSPreviewControl::GetAssetDataFromFile (RWSString strFile,
                                    RWSString& strType, RWSString& strTextures)
{
   if (strFile.size ())
   {
      // Determine the object type from the file extension
      RWSChar * szExtension = PathFindExtensionW (strFile.c_str ());
      if (!wcsicmp (szExtension, L".anm"))
      {
         strType = L"rwID_HANIMANIMATION";
      }
      else if (!wcsicmp (szExtension, L".dff"))
      {
         strType = L"rwID_CLUMP";
      }
      else if (!wcsicmp (szExtension, L".bsp"))
      {
         strType = L"rwID_WORLD";
      }

      // Determine the texture path by appending "\textures"
      strTextures = strFile;
      RWSString::size_type LastBackslash = strTextures.find_last_of (L"\\");
      if (LastBackslash != RWSString::npos)
      {
         strTextures.erase (LastBackslash);
      }

      strTextures.append (L"\\textures\\");

      return S_OK;
   }

   return E_FAIL;
}

///////////////////////////////////////////////////////////////////////////////
//
RwBool CRWSPreviewControl::LoadClump (RWSString strFile, RWSString strTextures)
{
   StopAnimation ();

   USES_CONVERSION;
   m_Engine.World ().DestroyAllClumps ();

   RW::RenderWareClump * pClump = 
      m_Engine.World ().CreateClump (T2A (strFile.c_str ()), T2A (strTextures.c_str ()));

   if (pClump)
   {
      m_Engine.World ().SetBspFlag(FALSE);
      pClump->FitClumpInView(m_Engine.World ().Camera (), &pClump->BoundingSphere());

      m_Engine.World ().m_params.HasHierarchy = FALSE;
      m_Engine.World ().m_params.NumBones = 0;

      RpClumpForAllAtomics(pClump->Anim ().GetClump(), HasHierarchy, &m_Engine.World ().m_params);
      m_pPreviewNotify->SetHasWorld(FALSE);
      m_pPreviewNotify->SetHasHierarchy(m_Engine.World ().m_params.HasHierarchy);

      if (pClump->Anim ().HasAnimation ())
      {
         if (pClump->Anim().HasMorph())
         {
            m_pPreviewNotify->SetHasHierarchy(FALSE);
            m_pPreviewNotify->SetAnimDuration( pClump->Anim ().GetAnimationDuration() );
            m_pPreviewNotify->SetCurrentAnimTime(0.0f);    
            m_pPreviewNotify->SetMorph();
         }
         
         SetPlayState (_psPlaying);
      }
      else
      {
         SetPlayState (_psNoAnim);
      }
      
      pClump->GetClumpStats();

   }
   else
   {
      SetPlayState (_psNoAnim);

      if (m_pPreviewNotify)
      {
         // Output an error message as we couldn't load the clump.
         RWSString strMessage (L"Failed to load clump \"");
         strMessage.append (PathFindFileName (strFile.c_str ()));
         strMessage.append (L"\"");

         BSTR bstrMessage = ::SysAllocString (strMessage.c_str ());
         m_pPreviewNotify->Error (bstrMessage);
         ::SysFreeString (bstrMessage);
      }
   }

   Invalidate ();

   return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//
RwBool CRWSPreviewControl::LoadAnimation (RWSString strFile)
{
   BOOL bSuccess = FALSE;

   RW::RenderWareClump* pClump = m_Engine.World ().GetClumpAtIndex (0);
   if (!m_Engine.World ().IsBspLoaded())
   {
      if (pClump)
      {
         try
         {
            USES_CONVERSION;
            if (pClump->Anim ().SetAnimation (T2A (strFile.c_str ())))
            {

               bSuccess = TRUE;
               PlayAnimation ();
               m_pPreviewNotify->SetAnimDuration( pClump->Anim ().GetAnimationDuration() );
               m_pPreviewNotify->SetCurrentAnimTime(0.0f);
               m_pPreviewNotify->SetHAnim();         
            }
         }
         catch (...)
         {
            if (m_pPreviewNotify)
               m_pPreviewNotify->Recreate ();
         }
      }

      if (!bSuccess && m_pPreviewNotify)
      {
         // An error occurred. Output a message.
         RWSString strMessage (L"Failed to load animation \"");
         strMessage.append (PathFindFileName (strFile.c_str ()));
         strMessage.append (L"\"");

         BSTR bstrMessage = ::SysAllocString (strMessage.c_str ());
         m_pPreviewNotify->Error (bstrMessage);
         ::SysFreeString (bstrMessage);
      }
   }

   return bSuccess;
}

///////////////////////////////////////////////////////////////////////////////
//
RwBool CRWSPreviewControl::LoadWorld (RWSString strFile, RWSString strTextures)
{
   StopAnimation ();
   m_DrawWorld = TRUE;
   m_DrawSectors = FALSE;
   m_DrawWireFrame = FALSE;
   m_DrawNormals = FALSE;
   DrawWorld(m_DrawWorld);
   DrawSectors(m_DrawSectors);
   DrawWireFrame(m_DrawWireFrame);
   DrawNormals(m_DrawNormals);
   m_CurrentSector = 0;

   RwBool RetVal = m_Engine.World ().LoadWorld(strFile, strTextures);
   SetPlayState (_psNoAnim);
   m_pPreviewNotify->SetHasHierarchy(FALSE);
   m_pPreviewNotify->SetHasWorld(TRUE);
   SectorTraverse();

   Invalidate ();

   return RetVal;
}


///////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CRWSPreviewControl::SetPlayState (__PlayState State)
{
   RwBool bHasAnim = FALSE;

   // Check to see if we have a valid animation
   if (RW::RenderWareClump * pClump = m_Engine.World ().GetClumpAtIndex (0))
      bHasAnim = pClump->Anim ().HasAnimation ();

   switch (m_PlayState)
   {
   case _psPlaying:
      switch (State)
      {
      case _psPaused:
         PauseAnimation ();
         break;
      case _psStopped:
         StopAnimation ();
         break;
      }
      break;
   case _psStopped:
      switch (State)
      {
      case _psPlaying:
         if (bHasAnim) PlayAnimation ();
         break;
      }
      break;
   case _psPaused:
      switch (State)
      {
      case _psPlaying:
         PlayAnimation ();
         break;
      case _psStopped:
         StopAnimation ();
         break;
      }
      break;
   }

   if (_psNoAnim == State)
   {
      StopAnimation ();
      m_PlayState = _psNoAnim;
      if (m_pPreviewNotify)
         m_pPreviewNotify->StateChanged (m_PlayState);
   }

   return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CRWSPreviewControl::get_Repeat (VARIANT_BOOL* pbRepeat)
{
   *pbRepeat = m_Repeat ? VARIANT_TRUE : VARIANT_FALSE;
   return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CRWSPreviewControl::put_Repeat (VARIANT_BOOL bRepeat)
{
   m_Repeat = bRepeat == VARIANT_FALSE ? FALSE : TRUE;
   return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CRWSPreviewControl::_Init (IUnknown* pNotify)
{
   if (m_pPreviewNotify)
      m_pPreviewNotify->Release ();
   m_pPreviewNotify = 0;

   if (pNotify)
   {
      if (SUCCEEDED (pNotify->QueryInterface (&m_pPreviewNotify)))
         m_pPreviewNotify->StateChanged (m_PlayState);
   }

   return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CRWSPreviewControl::Reset ()
{
   if (RW::RenderWareClump * pClump = m_Engine.World ().GetClumpAtIndex (0))
      m_Engine.World ().DestroyClump (pClump);

   // Stop any animation so as not to use too much CPU time.
   SetPlayState (_psNoAnim);

   return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CRWSPreviewControl::SetOrbit ()
{
   m_Mode      = ORBIT;

   return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CRWSPreviewControl::SetPan ()
{
   m_Mode      = PAN;

   return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CRWSPreviewControl::SetZoom ()
{
   m_Mode      = ZOOM;

   return S_OK;
}

STDMETHODIMP CRWSPreviewControl::DrawSkeleton(VARIANT_BOOL draw)
{
   m_DrawSkeleton = draw == VARIANT_FALSE ? FALSE : TRUE;
   Invalidate ();
   
   return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CRWSPreviewControl::get_DrawSkeleton (VARIANT_BOOL* pbRepeat)
{
   *pbRepeat = m_DrawSkeleton ? VARIANT_TRUE : VARIANT_FALSE;
   return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CRWSPreviewControl::DrawSkeletonAxis(VARIANT_BOOL draw)
{
   m_DrawSkelaxis = draw == VARIANT_FALSE ? FALSE : TRUE;
   Invalidate ();
   
   return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CRWSPreviewControl::get_DrawSkeletonAxis (VARIANT_BOOL* pbRepeat)
{
   *pbRepeat = m_DrawSkelaxis ? VARIANT_TRUE : VARIANT_FALSE;
   return S_OK;
}


///////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CRWSPreviewControl::DrawClump(VARIANT_BOOL draw)
{
   m_DrawClump = draw == VARIANT_FALSE ? FALSE : TRUE;
   m_Engine.World ().SetIsVisible(m_DrawClump);
   Invalidate ();
   
   return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CRWSPreviewControl::get_DrawClump (VARIANT_BOOL* pbDrawClump)
{
   *pbDrawClump = m_DrawClump ? VARIANT_TRUE : VARIANT_FALSE;
   return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CRWSPreviewControl::DrawText(VARIANT_BOOL drawText)
{
   m_DrawText = drawText == VARIANT_FALSE ? FALSE : TRUE;   
   Invalidate ();
   
   return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CRWSPreviewControl::get_DrawText (VARIANT_BOOL* pbDrawText)
{
   *pbDrawText = m_DrawText ? VARIANT_TRUE : VARIANT_FALSE;
   return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//
void CRWSPreviewControl::SectorTraverse(void)
{
   if ((m_DrawSectors) || (m_DrawWireFrame) || (m_DrawNormals))
   {
      if (m_pPreviewNotify)
      {
         m_pPreviewNotify->SetSectorTraverse(TRUE);
      }     
   }
   else
   {
      if (m_pPreviewNotify)
      {
         m_pPreviewNotify->SetSectorTraverse(FALSE);
      }
   }
}
///////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CRWSPreviewControl::DrawWorld(VARIANT_BOOL drawWorld)
{
   m_DrawWorld = drawWorld == VARIANT_FALSE ? FALSE : TRUE;   
   SectorTraverse();

   Invalidate ();

   return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CRWSPreviewControl::get_DrawWorld (VARIANT_BOOL* pbDrawWorld)
{
   *pbDrawWorld = m_DrawWorld ? VARIANT_TRUE : VARIANT_FALSE;
   return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CRWSPreviewControl::DrawSectors(VARIANT_BOOL drawSectors)
{
   m_DrawSectors = drawSectors == VARIANT_FALSE ? FALSE : TRUE;
   SectorTraverse();

   Invalidate ();

   return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CRWSPreviewControl::get_DrawSectors (VARIANT_BOOL* pbDrawSectors)
{
   *pbDrawSectors = m_DrawSectors ? VARIANT_TRUE : VARIANT_FALSE;
   return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CRWSPreviewControl::DrawWireFrame(VARIANT_BOOL drawWireFrame)
{
   m_DrawWireFrame = drawWireFrame == VARIANT_FALSE ? FALSE : TRUE;   
   SectorTraverse();

   Invalidate ();

   return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CRWSPreviewControl::get_DrawWireFrame (VARIANT_BOOL* pbDrawWireFrame)
{
   *pbDrawWireFrame = m_DrawWireFrame ? VARIANT_TRUE : VARIANT_FALSE;
   return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CRWSPreviewControl::DrawNormals(VARIANT_BOOL drawNormals)
{
   m_DrawNormals = drawNormals == VARIANT_FALSE ? FALSE : TRUE;   
   SectorTraverse();

   Invalidate ();

   return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CRWSPreviewControl::get_DrawNormals (VARIANT_BOOL* pbDrawNormals)
{
   *pbDrawNormals = m_DrawNormals ? VARIANT_TRUE : VARIANT_FALSE;
   return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CRWSPreviewControl::ZTest(VARIANT_BOOL bZTest)
{
   m_ZTest = bZTest == VARIANT_FALSE ? FALSE : TRUE;   
   Invalidate ();

   return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CRWSPreviewControl::get_ZTest (VARIANT_BOOL* pbZTest)
{
   *pbZTest = m_ZTest ? VARIANT_TRUE : VARIANT_FALSE;
   return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//
void CRWSPreviewControl::PlayAnimation ()
{
   if (m_PlayState = _psPaused)
      m_LastTime = timeGetTime ();

   m_PlayState = _psPlaying;
   _Module.SetIdleHandler (this);

   if (m_pPreviewNotify)
      m_pPreviewNotify->StateChanged (m_PlayState);
}

///////////////////////////////////////////////////////////////////////////////
//
void CRWSPreviewControl::PauseAnimation ()
{
   m_PlayState = _psPaused;
   _Module.SetIdleHandler (0);

   if (m_pPreviewNotify)
      m_pPreviewNotify->StateChanged (m_PlayState);
}

///////////////////////////////////////////////////////////////////////////////
//
void CRWSPreviewControl::StopAnimation ()
{
   if (RW::RenderWareClump* pClump = m_Engine.World ().GetClumpAtIndex (0))
   {
      pClump->Anim ().SetAnimationTime (0.0f);
      Invalidate ();
   }

   m_PlayState = _psStopped;
   m_LastTime = 0;
   _Module.SetIdleHandler (0);

   if (m_pPreviewNotify)
      m_pPreviewNotify->StateChanged (m_PlayState);
}

///////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CRWSPreviewControl::SetBackgroundColor(unsigned int Red, unsigned int Green, unsigned int Blue)
{
   if (m_Engine.World ().Camera ().IsInitialised ())
   {
      m_Engine.World().Camera().SetBackGroundColor(Red, Green, Blue);
      Invalidate ();
   }

   return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CRWSPreviewControl::GetAnimationTime(float *time)
{
   if (RW::RenderWareClump* pClump = m_Engine.World ().GetClumpAtIndex (0))
   {
      *time = pClump->Anim ().GetAnimationTime(); 
   }

   return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CRWSPreviewControl::SetAnimationPosition(float Time)
{
   if (RW::RenderWareClump* pClump = m_Engine.World ().GetClumpAtIndex (0))
   {
      pClump->Anim ().SetAnimationTime(Time); 
      pClump->Anim ().m_MorphParameters.position = Time;

      Invalidate ();
   }

   return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CRWSPreviewControl::LocateInView()
{
   if (m_Engine.World ().Camera ().IsInitialised ())
   {
         if((m_Engine.World ().IsBspLoaded()))
         {
            m_Engine.World	().FitWorldInView(m_Engine.World	().Camera (), &m_Engine.World	().m_WorldSphere);
         }
         else
         {
            if (RW::RenderWareClump* pClump = m_Engine.World ().GetClumpAtIndex (0))
            {
               pClump->TranslateTo(0.0f, 0.0f, 0.0f);
               pClump->FitClumpInView(m_Engine.World ().Camera	(), &pClump->BoundingSphere());								
            }
         }

         Invalidate ();
   }

   return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CRWSPreviewControl::PrevSector()
{
   if (m_Engine.World ().Camera ().IsInitialised ())
   {
      if((m_Engine.World ().IsBspLoaded()))
      {
         m_CurrentSector--;

         if (m_CurrentSector < 0)
         {
            m_CurrentSector = m_Engine.World().m_LastFramesNumberOfSectors;
         }

         Invalidate ();
      }
   }

   return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CRWSPreviewControl::NextSector()
{
   if (m_Engine.World ().Camera ().IsInitialised ())
   {
      if((m_Engine.World ().IsBspLoaded()))
      {
         m_CurrentSector++;

         if (m_CurrentSector > m_Engine.World().m_LastFramesNumberOfSectors)
         {
            m_CurrentSector = 0;
         }

         Invalidate ();
      }
   }

   return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CRWSPreviewControl::ShowAllSectors()
{
   if (m_Engine.World ().Camera ().IsInitialised ())
   {
      if((m_Engine.World ().IsBspLoaded()))
      {
         m_CurrentSector = 0;

         Invalidate ();
      }
   }

   return S_OK;
}

