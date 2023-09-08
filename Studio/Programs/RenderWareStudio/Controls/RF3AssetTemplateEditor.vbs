'
' RF3AssetTemplateEditor.vbs 
' 
' The ActiveX control for editing the values in an RF3 asset templates.

Option Explicit ' All variables must be explicitly declared (with Dim)

' ------------------------------------------------------------------------------
' Initialization function
Sub OnLoad () 
    RF3AssetTemplateEditor.SetType 0 ' 0 = asset templates
End Sub