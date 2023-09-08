'
' Enterprise Layout Management Script
'
' Layout object 'AppSettings'
'
' This script contains all of the RenderWareStudio settings that
' a system administrator should want to change.


' ---------------------------------------------------------------------
' Directory and file settings
Dim InstallDirectory    ' The install directory for RenderWare Studio.
Dim BuildRulesDirectory ' The directory where the default build rules are taken from.
Dim SettingsFilename    ' The filename of the file that the main app settings are stored in.
Dim ToolDirectory       ' The directory where the general tools reside.
Dim HelpFilename        ' The applications help filename.
Dim NetTestCommand      ' The command to run Net Test. Requires: "<NetTestCommand>" <port No.>

InstallDirectory    = GetAppPath ()
SettingsFilename    = InstallDirectory & "\RwStudio.settings"
BuildRulesDirectory = InstallDirectory & "\Workspace\Build Rules"
ToolDirectory       = InstallDirectory & "\tools"
HelpFilename        = InstallDirectory & "\..\Help\RWStudio.chm"
NetTestCommand      = ToolDirectory & "\NetTest.hta"


' ---------------------------------------------------------------------
' Tooltip Settings
' Tooltip_WaitTime    ' The time a mouse must remain stationary before a tooltip is shown in the Game View (in ms - min: 10)
' Tooltip_DisplayTime ' The time a tooltip remains displayed on screen in the Game View (in ms = Min: 10)
Const Tooltip_WaitTime    = 750  ' 3/4 second
Const Tooltip_DisplayTime = 6000 ' 6 seconds


' ---------------------------------------------------------------------
' Recent Files List Settings
' RecentFiles_MaxFiles ' Maximum number of files shown in the recent files list (Min: 1)
Const RecentFiles_MaxFiles = 12


' ---------------------------------------------------------------------
' Undo Settings
' Undo_StackSize ' The maximum number of items in the undo and redo stacks
Const Undo_StackSize = 64


' ---------------------------------------------------------------------
' Helper functions
' ---------------------------------------------------------------------

' ---------------------------------------------------------------------
Function GetAppPath ()
    GetAppPath = ""

    Dim WshShell
    Set WshShell = CreateObject("WScript.Shell.1")
    ' Disable error trapping - otherwise we get a nasty script type message if
    ' the app path doesn't exist
    On Error Resume Next ' Disables error trapping

    regKey = "HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths\RWStudio.exe\path"
    AppDir = WshShell.RegRead(regKey)

    If Err.Number <> 0 Then
        bKeyExist = False

       ' Inform us nicely
       strOutputText = "[Workspace] Unable to access registry key: '" + regKey + "' &lt; " + Err.Description + " &gt;"
       CSLRenderwareStudioWorkspace.ShowObject MessageLog
       MessageLog.Error strOutputText
    Else
        bKeyExist = True
    End if

    On Error Goto 0  ' That's a zero - re-enables error trapping

    If bKeyExist Then
        GetAppPath = StripEnvironment (AppDir)
    End if
End Function

'------------------------------------------------------------------------
' Replaces any environment variables found in a string
Function StripEnvironment (EnvString)
   Dim WshShell, EnvRegExp, Matches, WshEnvironmentUser, WshEnvironmentSystem
   Set WshShell = CreateObject("WScript.Shell")

   ' Create a regular expression to find an environment variable
   Set EnvRegExp = New RegExp
   EnvRegExp.Pattern = "\%\w*\%"

   Set WshEnvironmentUser = WshShell.Environment ("User")
   Set WshEnvironmentSystem = WshShell.Environment ("System")

   Set Matches = EnvRegExp.Execute (EnvString)
   StripEnvironment = EnvString
   For Each Match In Matches

      EnvValue = WshEnvironmentUser (Mid (Match.Value, 2, Match.Length - 2))

      if EnvValue = "" then
         EnvValue = WshEnvironmentSystem (Mid (Match.Value, 2, Match.Length - 2))
      End if

      If EnvValue = "" Then
         MsgBox "Unable to find environment variable " + Match.Value
      Else
         StripEnvironment = Replace (StripEnvironment, Match.Value, EnvValue)
      End if

   Next
End Function