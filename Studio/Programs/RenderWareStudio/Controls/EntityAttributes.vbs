'
' Enterprise Layout Management Script
'
' Layout object 'EntityAttributes'
'

' ------------------------------------------------------------------------------
' Initialization dependant on other objects being loaded.
Sub OnLoad ()
    ' Initialize the entity attributes selection to be the global selection
    ' of entities
    EntityAttributes.CurrentSelection = GlobalScript.g_strGlobalSelection
    
    ' Entity attribute updates are disabled by default. They will be enabled
    ' only when shown to the user.
    EntityAttributes.Enabled = False
End Sub

' ------------------------------------------------------------------------------
Sub EntityAttributes_OnGotoSource ( ObjectID )
    GlobalScript.ViewObjectSource ObjectID
End Sub

' ------------------------------------------------------------------------------
Sub EntityAttributes_OnVisibleChanged (bVisible)
   ' Only enable attribute editor updates whilst the attribute editor is visible.
   EntityAttributes.Enabled = bVisible
End Sub

' ------------------------------------------------------------------------------
Sub EntityAttributes_OnViewHelp ( CommandID )
    ' Get the help string
    Dim Command, Params

    Set Command = GlobalScript.RWSScript.Command (CommandID)
    If Not Command Is Nothing Then
        Params = Command.ParamList
        If UBound (Params) > 1 Then
            ' Get the help string from command parameters
            Dim strHelp, fso, PathArray, strSourceRoot
            strHelp = Params (2)
            strHelp = UnquoteString (strHelp)

            ' Help string might be a URL (prefixed with "RWSHELP:")
            If Len (strHelp) > 7 Then
                If Left (strHelp, 8) = "RWSHELP:" Then
                    Dim strPath
                    Set fso = CreateObject ("Scripting.FileSystemObject")

                    strHelp = Right (strHelp, Len (strHelp) - 8)

                    ' Relative to source root?
                    PathArray = Split (GlobalScript.g_strSourceRoot, ";")
                    For Each strSourceRoot In PathArray
                        strPath = fso.BuildPath (strSourceRoot, "Help")
                        strPath = fso.BuildPath (strPath, strHelp)
                        If fso.FileExists (strPath) Then
                            strHelp = strPath
                            Exit For
                        End If
                    Next

                    CSLRenderwareStudioWorkspace.ShowObject HelpWindow
                    HelpWindow.ShowHelpURL strHelp
                    Exit Sub
                End If
            End If

            If Len (strHelp) = 0 Then
                ' If default help exists for this behaviour, display it
                Set fso = CreateObject ("Scripting.FileSystemObject")

                ' Split the line up into semi-colon separated strings
                PathArray = Split (GlobalScript.g_strSourceRoot, ";")
                For Each strSourceRoot In PathArray
                    strHelp = fso.BuildPath (strSourceRoot, "Help")
                    strHelp = fso.BuildPath (strHelp, Command.ParentBehaviors (1).UID & ".htm")

                    If fso.FileExists (strHelp) Then
                        CSLRenderwareStudioWorkspace.ShowObject HelpWindow
                        HelpWindow.ShowHelpURL strHelp
                        Exit Sub
                    End If
                Next
            Else
               ' Get the title, and display the help
               Dim strTitle
               strTitle = Params (1)
               strTitle = UnquoteString (strTitle)

               ' Show help text
               CSLRenderwareStudioWorkspace.ShowObject HelpWindow
               HelpWindow.ShowHelp strTitle, strHelp
            End If

        End If
    End If
End Sub

Function UnquoteString (ByRef str)
    If Len (str) > 1 Then
        If Left (str, 1) = """" And Right (str, 1) = """" Then
            str = Mid (str, 2, Len (str) - 2)
        End If
    End If
    UnquoteString = str
End Function

' ------------------------------------------------------------------------------
' Catch broadcast events
' ------------------------------------------------------------------------------

' ------------------------------------------------------------------------------
' Handle broadcast event to say source parse is about to happen
Sub Broadcast_PreParseSource ()
    EntityAttributes.ParseSourceStart
End Sub

' ------------------------------------------------------------------------------
' Handle broadcast event to say source parse has completed
Sub Broadcast_PostParseSource ()
    EntityAttributes.ParseSourceCompleted
End Sub
