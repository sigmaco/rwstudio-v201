'
' Enterprise Layout Management Script
'
' Layout object 'WorkspaceOptions'
'


Dim DirtyFlag
DirtyFlag = false

' ---------------------------------------------------------------------

Sub Initialize
    if IsObject(Globalscript.WorkspaceSettingsObj) then
        CheckBoxLoadLastProject.Value         =  Globalscript.WorkspaceSettingsObj.LoadLastProject
        CheckBoxSaveSelectionSettings.Value   =  Globalscript.WorkspaceSettingsObj.SaveSelectionSettings
        CheckBoxSaveCameraPosition.Value      =  Globalscript.WorkspaceSettingsObj.SaveCameraPosition
        CheckBoxRestoreHidden.Value           =  Globalscript.WorkspaceSettingsObj.RestoreHiddenObjects

        ' Setup the values in the source viewers' combo box
        ComboBoxSourceViewer.Clear
        ComboBoxSourceViewer.AddItem "(Associated Viewer)" 
        AddViewerToCombo "CodeWarrior",        "CodeWarrior.CodeWarriorApp"
        AddViewerToCombo "Visual C++ 6",       "msdev.application" 
        AddViewerToCombo "Visual Studio .NET", "VisualStudio.DTE"
        On Error Resume Next 
        ComboBoxSourceViewer.Value = Globalscript.WorkspaceSettingsObj.SourceViewer
        On Error Goto 0    
        If ComboBoxSourceViewer.Value = "" Then
            ComboBoxSourceViewer.Value = "(Associated Viewer)" 'Default value
        End If
    Else
        MsgBox "Error: Globalscript.WorkspaceSettingsObj does not appear to exist", vbOKOnly + vbExclamation, "Error"
    End If
    DirtyFlag = False
End Sub

' ---------------------------------------------------------------------
' Only add the item (as strName) to the viewer's list if the viewing
' object (named using strObjectCreate) has been registered in the
' registry.
Sub AddViewerToCombo (strName, strClassName)
    Dim WshShell, RegKey, strClassID
    Set WshShell = CreateObject("WScript.Shell.1")
    RegKey       = "HKCR\" & strClassName & "\CLSID\"

    ' Disable error trapping - otherwise we get a nasty script type message if
    ' the key doesn't exist
    On Error Resume Next ' Disables error trapping
    strClassID = WshShell.RegRead(regKey)
    On Error Goto 0    

    If Len (strClassID) > 0 Then
        ComboBoxSourceViewer.AddItem strName   
    End if
End Sub

' ---------------------------------------------------------------------

Sub CheckBoxLoadLastProject_Change (  )
    DirtyFlag = True
End Sub

' ---------------------------------------------------------------------

Sub CheckBoxSaveCameraPosition_Change (  )
    DirtyFlag = True
End Sub

' ---------------------------------------------------------------------

Sub CheckBoxSaveSelectionSettings_Change (  )
    DirtyFlag = True
End Sub

' ---------------------------------------------------------------------

Sub CheckBoxRestoreHidden_Change (  )
    DirtyFlag = True
End Sub

' ---------------------------------------------------------------------

Sub Apply
    if IsObject(Globalscript.WorkspaceSettingsObj) then
        Globalscript.WorkspaceSettingsObj.LoadLastProject        =  CheckBoxLoadLastProject.Value
        Globalscript.WorkspaceSettingsObj.SaveSelectionSettings  =  CheckBoxSaveSelectionSettings.Value
        Globalscript.WorkspaceSettingsObj.SaveCameraPosition     =  CheckBoxSaveCameraPosition.Value
        Globalscript.WorkspaceSettingsObj.RestoreHiddenObjects   =  CheckBoxRestoreHidden.Value  
        Globalscript.WorkspaceSettingsObj.SourceViewer           =  ComboBoxSourceViewer.Value   
    Else
        MsgBox "Error: Globalscript.WorkspaceSettingsObj does not appear to exist", vbOKOnly + vbExclamation, "Error"
    End If
    DirtyFlag = False
End Sub

' ---------------------------------------------------------------------

Sub CancelButton_Click (  )
    WorkspaceOptions.StopModal False
End Sub

Sub OKButton_Click (  )
    Apply
    WorkspaceOptions.StopModal True
End Sub