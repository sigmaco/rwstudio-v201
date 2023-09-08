'
' Enterprise Layout Management Script
'
' Layout object 'GetLatest'
'

Option Explicit ' All variables must be explicitly declared (With Dim)

' Constants defining the function of this dialog
Const conGetLatest     = 1
Const conImportProject = 2

' Variables defining the state of the dialog
Dim bProjectChecked, bResourceRootChecked
bProjectChecked      = True
bResourceRootChecked = True

' ---------------------------------------------------------------------
' Sets up the dialog and shows it.
' Returns: True if the user pressed OK
'          False if the user pressed Cancel.
Function DoModal (nDialogType)
    ' Set the function-dependant captions.
    Select Case nDialogType
    Case conGetLatest
        GetLatest.Caption          = "Get Latest"
        GetLatestProject.Caption   = "Get latest of current project:"
        GetLatestResources.Caption = "Get latest resources:"
        OKButton.Caption           = "Get Latest"
    Case conImportProject
        GetLatest.Caption          = "Import Project to alienbrain"
        GetLatestProject.Caption   = "Import current project:"
        GetLatestResources.Caption = "Import resource root:"
        OKButton.Caption           = "Import"
    Case Else
        DoModal = False
        Exit Function
    End Select

    ' Set the project name and the check mark.
    ProjectName.Text = Persist.strCurrentFilename
    GetLatestProject.Value = bProjectChecked
    ' If the project location is managed, enable the check.
    Dim bProjectPathManaged
    bProjectPathManaged      = VersionControl.IsPathOrParentPathManaged (Persist.strCurrentFilename)
    GetLatestProject.Enabled = bProjectPathManaged
    ProjectName.Enabled      = bProjectPathManaged

    ' Set the resource root and if the root is managed, enable the check mark.
    If GlobalScript.g_strResourceRoot = "" Then
         Dim SettingsFile
         SettingsFile = Persist.BuildSettingsFilePath (Persist.strCurrentFilename)
         GlobalScript.g_strResourceRoot = GlobalScript.GetResourceRoot (SettingsFile)
    End If
    ResourceRoot.Text = GlobalScript.g_strResourceRoot
    GetLatestResources.Value = bResourceRootChecked
    ' If the resource root is managed, enable and set the resource root controls.
    Dim bResourceRootPathManaged, bResourceRootParentManaged
    bResourceRootPathManaged   = VersionControl.IsPathManaged (ResourceRoot.Text)
    bResourceRootParentManaged = VersionControl.IsPathOrParentPathManaged (ResourceRoot.Text)
    Select Case nDialogType
    Case conGetLatest
      ' If the root is managed, allow the checkbox and path.
      GetLatestResources.Enabled  = bResourceRootPathManaged
      ResourceRoot.Enabled        = bResourceRootPathManaged
    Case conImportProject
      If Not bResourceRootPathManaged And bResourceRootParentManaged Then
         ' If the root is not managed, but one of it's parents can be, allow the checkbox and path.
         GetLatestResources.Enabled  = True
         ResourceRoot.Enabled        = True
      Else
         GetLatestResources.Enabled  = False
         ResourceRoot.Enabled        = False
      End If
    End Select

    ' Show the modal dialog and return if the user pressed OK or Cancel.
    RENManager.ShowModal "GetLatest", "CSLRenderwareStudioWorkspace"
    DoModal = CBool (GetLatest.ModalResult)
End Function

' ---------------------------------------------------------------------
' Returns true if the project path if it was selected (checked) and enabled
'         false otherwise.
Function ProjectSelected ()
   If bProjectChecked Then
      If GetLatestProject.Enabled Then
         If Len (ProjectName) > 0 Then
            ProjectSelected = true
            Exit Function
         End If
      End If
   End If

   ProjectSelected = false
End Function

' ---------------------------------------------------------------------
' Returns true if the resource root if it was selected (checked) and enabled
'         false otherwise.
Function ResourceRootSelected ()
   If bResourceRootChecked Then
      If GetLatestResources.Enabled Then
         If Len (ResourceRoot) > 0 Then
            ResourceRootSelected = true
            Exit Function
         End If
      End If
   End If

   ResourceRootSelected = false
End Function

' ---------------------------------------------------------------------
Sub OkButton_Click (  )
    GetLatest.StopModal True
    bProjectChecked      = GetLatestProject.Value
    bResourceRootChecked = GetLatestResources.Value
End Sub

' ---------------------------------------------------------------------
Sub CancelButton_Click (  )
    GetLatest.StopModal False
End Sub
