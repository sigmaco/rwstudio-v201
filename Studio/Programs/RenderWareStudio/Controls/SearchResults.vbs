'
' Enterprise Layout Management Script
'
' Layout object 'SearchResults'
'

Option Explicit ' All variables must be explicitly declared

Const g_PersistElement = "SearchResults"


' -----------------------------------------------------------------------------
' Initialization dependant on other objects being loaded.
Sub OnLoad ()
   SearchResults.SelectionIdentifier = GlobalScript.g_strGlobalSelection
End Sub

' ------------------------------------------------------------------------------
' Displays a context menu on the search results lister
Sub SearchResults_OnContextMenu (Context, nX, nY, hWnd)
   ContextMenu.DisplayListerMenu Context, nX, nY, hWnd
End Sub

' ------------------------------------------------------------------------------
' This event is fired when the user presses the Del key
Sub SearchResults_OnDeleteRemove (Context)
   'Check that an item is selected
   If Not Context.IsNode (GlobalScript.SearchResultsListerNode) Then
      ContextMenu.DeleteOrRemove Context
   End If
End Sub

' ------------------------------------------------------------------------------
' Calls into utils to Request / Set status of an object
Sub SearchResults_OnRequestStatus (ObjectID)
   VersionControl.GetPersistStatusInformation ObjectID
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
   ListerTools.LoadLister SearchResults, g_PersistElement, XMLRootNode
End Sub

' ------------------------------------------------------------------------------
' Handle broadcast message to save settings.
Sub Broadcast_SaveApplicationSettings (XMLDoc, XMLRootNode)
   ListerTools.SaveLister SearchResults, g_PersistElement, XMLDoc, XMLRootNode
End Sub
