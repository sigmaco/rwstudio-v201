'
' RF3ProjectTemplateEditor.vbs
'
' The ActiveX control for editing the values in an RF3 project templates.

Option Explicit ' All variables must be explicitly declared (with Dim)

' ------------------------------------------------------------------------------
' Initialization function
Sub OnLoad ()
    RF3ProjectTemplateEditor.SetType 1 ' 1 = project templates
End Sub