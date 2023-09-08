'
' BuildLog.vbs
'
' This script wraps up helpful functions for adding messages to the build
' log. It is preferable to use these functions instead of the more direct
' AddMessage() function. It allows the adding of messages to be centralized and
' the calling code to be easier to read.

Option Explicit ' All variables must be explicitly declared (With Dim)

Const conInformation = 0
Const conWarning     = 1
Const conError       = 2

' ------------------------------------------------------------------------------
' Calls the appropriate local function depending on the severity of the message.
Sub Message (nSeverity, strMessage)
   Select Case nSeverity
      Case conInformation
         Log strMessage
      Case conWarning
         Warning strMessage
      Case conError
         Error strMessage
   End Select
End Sub

' ------------------------------------------------------------------------------
' Writes out messages to the Build log window with a severity of information.
Sub Log (strMessage)
   BuildLog.AddMessage conInformation, strMessage
End Sub

' ------------------------------------------------------------------------------
' Writes out messages to the Build log window with a severity of warning.
Sub Warning (strMessage)
   BuildLog.AddMessage conWarning, strMessage
End Sub

' ------------------------------------------------------------------------------
' Writes out messages to the Build log window with a severity of error and
' brings the message log window to the front.
' Note, DO NOT call this from an OnLoad() function or globally from a script as
' the BuildLog object may not have been created or its script loaded yet.
Sub Error (strMessage)
   BuildLog.AddMessage conError, strMessage
   ' All errors bring the log to the front, but only if a layout has been loaded.
   If CSLRenderwareStudioWorkspace.bLayoutLoaded Then
      CSLRenderwareStudioWorkspace.ShowObject BuildLog
   End If
End Sub
