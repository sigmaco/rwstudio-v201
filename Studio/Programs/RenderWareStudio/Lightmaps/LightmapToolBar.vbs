'
' LightmapToolbar.vbs
'
' This script contains all functions that are called when a toolbar button is pressed.

Option Explicit ' All variables must be explicitly declared (with Dim)

' ------------------------------------------------------------------------------
Sub OnLtMapGenerate
    LightmapEditor.oLightmapEditor.GenerateFinal (LightmapEditor.oLightmapEditor.ActiveFolderID)
End Sub

' ------------------------------------------------------------------------------
Sub OnLtMapPreview
    LightmapEditor.oLightmapEditor.GeneratePreview (LightmapEditor.oLightmapEditor.ActiveFolderID)
End Sub

' ------------------------------------------------------------------------------
Sub OnLtMapSettings
    LightmapEditor.oLightmapEditor.ChangeOptions
End Sub
