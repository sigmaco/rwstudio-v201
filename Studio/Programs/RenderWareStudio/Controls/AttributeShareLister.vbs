'
' Enterprise Layout Management Script
'
' Layout object 'Attribute Share Lister'
'

Option Explicit ' All variables must be explicitly declared

Const g_PersistElement = "AttributeShareLister"

AttributeShareLister.SelectionIdentifier = "AttributeShareSelections"

Dim g_bIsVisible, g_bNeedToPopulate ' Flags so we know when window is visible and can be populated
g_bIsVisible = False
g_bNeedToPopulate = False

' --------------------------------------------------------------------
' Displays a context menu on the attribute share lister
Sub AttributeShareLister_OnContextMenu ( Context, nX, nY, hWnd )
    ContextMenu.DisplayListerMenu Context, nX, nY, hWnd
End Sub

' ------------------------------------------------------------------------------
' This event is fired when the user presses the Del key
Sub AttributeShareLister_OnDeleteRemove (Context)
   'Check that an item is selected
   If Not Context.IsNode (GlobalScript.AttributeShareListerNode) Then
      ContextMenu.DeleteOrRemove Context
   End If
End Sub

' ------------------------------------------------------------------------------
' Calls into utils to Request / Set status of an object
Sub AttributeShareLister_OnRequestStatus (ObjectID)
   VersionControl.GetPersistStatusInformation ObjectID
End Sub

'------------------------------------------------------------------------------
Sub NewDatabase ()
   ' If lister is visible, populate it immediately,
   ' otherwise it will be populated when it becomes visible
   If g_bIsVisible Then
      g_bNeedToPopulate = False
      AttributeShareLister.Populate GlobalScript.AttributeShareListerNode
   Else
      g_bNeedToPopulate = True
   End If
End Sub

'------------------------------------------------------------------------------
Sub AttributeShareLister_OnVisibleChanged (bVisible)
   If bVisible And Not g_bIsVisible Then
      ' Only refresh if we need to (source changed, or new root-folder is active
      If g_bNeedToPopulate Then
         g_bNeedToPopulate = False
         AttributeShareLister.Populate GlobalScript.AttributeShareListerNode
      End If
   End If
   g_bIsVisible = bVisible
End Sub

' ------------------------------------------------------------------------------
' Catch broadcast events
' ------------------------------------------------------------------------------

' ------------------------------------------------------------------------------
' Handle broadcast message to convert settings.
'Sub Broadcast_UpdateApplicationSettingsVersion (XMLDoc, XMLRootNode, nFileVersion)
'End Sub

' ------------------------------------------------------------------------------
' Handle broadcast message to load settings.)
Sub Broadcast_LoadApplicationSettings (XMLRootNode)
   ' Load the state of the lister on startup
   ListerTools.LoadLister AttributeShareLister, g_PersistElement, XMLRootNode
End Sub

' ------------------------------------------------------------------------------
' Handle broadcast message to save settings.
Sub Broadcast_SaveApplicationSettings (XMLDoc, XMLRootNode)
   ' Save state of lister into XML file
   ListerTools.SaveLister AttributeShareLister, g_PersistElement, XMLDoc, XMLRootNode
End Sub
