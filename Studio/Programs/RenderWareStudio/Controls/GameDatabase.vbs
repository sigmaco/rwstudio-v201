'
' Enterprise Layout Management Script
'
' Layout object 'GameDatabase'
'

Option Explicit ' All variables must be explicitly declared

Const g_PersistElement = "GameDatabase"

Dim g_bIsVisible, g_bNeedToPopulate ' Flags so we know when window is visible and can be populated
g_bIsVisible = False
g_bNeedToPopulate = False

' ------------------------------------------------------------------------------
' Initialization dependant on other objects being loaded.
Sub OnLoad ()
   GameDatabase.SelectionIdentifier = GlobalScript.g_strGlobalSelection
End Sub

' ------------------------------------------------------------------------------
' Displays a context menu on the game database
Sub GameDatabase_OnContextMenu (Context, nX, nY, hWnd )
    ContextMenu.DisplayListerMenu Context, nX, nY, hWnd
End Sub

' ------------------------------------------------------------------------------
' This event is fired when the user presses the Del key
Sub GameDatabase_OnDeleteRemove (Context)
   'Check that an item is selected
   If Not Context.IsNode (GlobalScript.GameDatabaseNode) Then
      ContextMenu.DeleteOrRemove Context
   End If
End Sub

' ------------------------------------------------------------------------------
' Calls into utils to Request / Set status of an object
Sub GameDatabase_OnRequestStatus (ObjectID)
   VersionControl.GetPersistStatusInformation ObjectID
End Sub

'------------------------------------------------------------------------------
Sub NewDatabase ()
   ' If lister is visible, populate it immediately,
   ' otherwise it will be populated when it becomes visible
   If g_bIsVisible Then
      g_bNeedToPopulate = False
      GameDatabase.Populate GlobalScript.GameDatabaseNode
   Else
      g_bNeedToPopulate = True
   End If
End Sub

'------------------------------------------------------------------------------
Sub GameDatabase_OnVisibleChanged (bVisible)
   If bVisible And Not g_bIsVisible Then
      ' Only refresh if we need to (source changed, or new root-folder is active
      If g_bNeedToPopulate Then
         g_bNeedToPopulate = False
         GameDatabase.Populate GlobalScript.GameDatabaseNode
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
' Handle broadcast message to load settings.
Sub Broadcast_LoadApplicationSettings (XMLRootNode)
   ' Load the state of the lister on startup
   ListerTools.LoadLister GameDatabase, g_PersistElement, XMLRootNode
End Sub

' ------------------------------------------------------------------------------
' Handle broadcast message to save settings.
Sub Broadcast_SaveApplicationSettings (XMLDoc, XMLRootNode)
   ListerTools.SaveLister GameDatabase, g_PersistElement, XMLDoc, XMLRootNode
End Sub
