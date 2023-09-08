'
' MessageLog.vbs
'
' This script wraps up helpful functions for adding messages to the message
' log. It is preferable to use these functions instead of the more direct
' AddMessage() function. It allows the adding of messages to be centralized and
' the calling code to be easier to read.

Option Explicit ' All variables must be explicitly declared (With Dim)

Const conInformation = 0
Const conWarning     = 1
Const conError       = 2

' Write out the message log's first message welcoming users to RenderWare Studio
MessageLog.AddMessage conInformation, "Welcome to RenderWare Studio"

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
' Writes out messages to the Message log window with a severity of information.
Sub Log (strMessage)
   MessageLog.AddMessage conInformation, strMessage
End Sub

' ------------------------------------------------------------------------------
' Writes out messages to the Message log window with a severity of warning.
Sub Warning (strMessage)
   MessageLog.AddMessage conWarning, strMessage
End Sub

' ------------------------------------------------------------------------------
' Writes out messages to the Message log window with a severity of error and
' brings the message log window to the front.
' Note, DO NOT call this from an OnLoad() function or globally from a script as
' the MessageLog object may not have been created or its script loaded yet.
Sub Error (strMessage)
   MessageLog.AddMessage conError, strMessage
   ' All errors bring the log to the front, but only if a layout has been loaded.
   If CSLRenderwareStudioWorkspace.bLayoutLoaded Then
      CSLRenderwareStudioWorkspace.ShowObject MessageLog
   End If
End Sub
