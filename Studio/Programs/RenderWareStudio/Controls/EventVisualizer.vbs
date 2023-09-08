'
' Enterprise Layout Management Script
'
' Layout object 'EventVisualizer'
'

' --------------------------------------------------------------------
' Initialization dependant on other objects being loaded.
Sub OnLoad ()
    ' Initialize event viz's selection to be the global selection of entities.
    EventVisualizer.CurrentSelection = GlobalScript.g_strGlobalSelection
End Sub

' --------------------------------------------------------------------
Sub EventVisualizer_SetIsDirty(IsDirty)
    MainMenu.EnableMenuItem  42, IsDirty ' Refresh Event Vizualiser
    MainToolbar.EnableMenuItem  15, IsDirty ' Refresh Event Vizualiser
End Sub

' --------------------------------------------------------------------
Sub EventVisualizer_InformRequiresReBuild
    MessageLog.Warning "Select 'Refresh Event View' to generate event graph."
End Sub

' ------------------------------------------------------------------------------
' Catch broadcast events
' ------------------------------------------------------------------------------

' ------------------------------------------------------------------------------
' Handle broadcast event to say source parse has completed
Sub Broadcast_PostParseSource ()
    EventVisualizer.SetIsDirty true
End Sub

' ------------------------------------------------------------------------------
' Handle broadcast event to say active folder has changed
Sub Broadcast_ActiveFolderChanged (NewRootFolder)
    EventVisualizer.SetActiveFolder NewRootFolder
End Sub
