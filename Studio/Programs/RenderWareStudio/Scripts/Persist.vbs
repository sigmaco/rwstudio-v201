' 
' Enterprise Layout Management Script
'
' Layout object 'Persist'
'
' This script is the place for all global variables and functions that
' access the disk - loading/saving.

Option Explicit ' All variables must be explicitly declared

' --------------------------------------------------------------------
' Create objects required by this script.
Dim fso
Set fso = CreateObject ("Scripting.FileSystemObject")   

' --------------------------------------------------------------------
' Script Globals
Dim strCurrentFilename, strCurrentFolder, sngCurrentSettingsFileVersion
Dim bModified, bSettingGameProperties
strCurrentFilename            = ""
sngCurrentSettingsFileVersion = 2.0
strCurrentFolder       = "" 
bModified              = False
bSettingGameProperties = False

' --------------------------------------------------------------------
' Initialize constants
Const LoadDialog  = True
Const SaveDialog  = False
Const SaveChanges = 0
Const SaveAll     = 1
' Special Windows folders to be used with: fso.GetSpecialFolder
Const WindowsFolder   = 0
Const SystemFolder    = 1
Const TemporaryFolder = 2

' --------------------------------------------------------------------
Sub SetCurrentProjectGlobals (strFilename)
    strCurrentFilename = strFilename
    
    If Instr (1, Right (strCurrentFilename, 9), ".rwstudio", 1) Then ' Instr: Case insensitive search
       strCurrentFolder = Left (strCurrentFilename, Len (strCurrentFilename) - 9)
    Else
       strCurrentFolder = ""
    End If
End Sub

' --------------------------------------------------------------------
' Load a Project.
' strFilename - The name of the project to load.
'               If this is empty, show a file open dialog.
Sub LoadProject (strFilename)
    Dim bSaveFirst, bOK, ResourceRoot, SettingsFile, bProjectIsManaged

    ' Save any settings for the current project 
    If Len(strCurrentFilename) > 0 Then
        SaveGameSettings
    End If

    ' Ask if user wants to save first
    bSaveFirst = vbNo
    If bModified Then
        bSaveFirst = MsgBox ("Do you wish to save your current changes?", vbYesNoCancel + vbQuestion, GlobalScript.g_strTitle)
        If bSaveFirst = vbCancel Then
            Exit Sub
        End If       
        If bSaveFirst = vbYes Then 
            SaveProject SaveChanges
            If Len (strCurrentFilename) = 0 Then ' User hit cancel 
                Exit Sub 
            End If 
        End If 
    End If

    ' If we didn't pass a filename in, ask for one
    If Len (strFilename) = 0 Then
        strFilename = ShowGameFileDialog (LoadDialog)
        If Len (strFilename) = 0 Then
            Exit Sub
        End If
    End If

    ' May need to convert to a newer DB file version
    Dim bHasConverted
    bHasConverted = False
    strFilename = CheckAndConvertDBVersion (strFilename, bHasConverted)
    If Len (strFilename) = 0 Then ' "Save as" operation cancelled
       Exit Sub
    End If
    SetCurrentProjectGlobals (strFilename)

    LoadGameSettings 
    GlobalScript.g_bLoading = True 

    bProjectIsManaged = VersionControl.IsPathManaged (strFilename)

    If bProjectIsManaged Then         
        MessageLog.Log "[Version Control] Project is managed by alienbrain."
    Else
        MessageLog.Log "[Version Control] Project is not managed by alienbrain."
    End If

    ' Inform other scripts that we're about to load a project.
    Broadcast.PreLoadProject strFilename
    bOK = GlobalScript.RWSUtils.LoadGame (strFilename, bProjectIsManaged)
    GlobalScript.g_bLoading = False 
 
    If bOK = False Then 
        SetCurrentProjectGlobals ("")
        MsgBox "Load Failed", 1 + vbExclamation, GlobalScript.g_strTitle

        ' update recent files list, possibly removing this, failed load, project.
        GlobalScript.UpdateRecentFilesList 
        GlobalScript.ClearDocs
        Exit Sub 
    End If 

    ' The load has succeeded. If the database had to be converted, reload all assets.
    ' This forces an export of all RF3 assets and their file locations to be updated.
    ' A clean (from PreReloadAsset) is needed to ensure that the build rule will be
    ' called for this asset and the asset's properties will be updated.
    If bHasConverted Then
        Dim oAsset
        For Each oAsset In RWSScript.Assets
            If Not oAsset Is Nothing Then
                If left (oAsset.Type, 3) = "RF3" Then
                    BuildScriptManager.PreReloadAsset oAsset.ID
                    BuildScriptManager.PreImportAsset oAsset
                End If
            End If
        Next
    End If

    ' Inform other scripts that a project has just loaded successfully.
    Broadcast.PostLoadProject strFilename

    GlobalScript.UpdateAppCaption
    GlobalScript.UpdateRecentFilesList 
    
    ' Update the menus 
    GlobalScript.SetMenuOptionsOnStartGame False
    GlobalScript.UpdateVersionControlMenu 
End Sub

'---------------------------------------------------------------------
' Save a project
' SaveChangesOrAll - Flag to decide whether to save all files in a
'                    project or just the changed files. Use one of the
'                    constants, SaveChanges or SaveAll.
Sub SaveProject (SaveChangesOrAll)
    ' If there's no database created/loaded yet, then there's nothing to save.
    If GlobalScript.g_DatabaseID = 0 Then
        Exit Sub
    End If

    Dim strFilename ' The filename to save to 

    If SaveChangesOrAll = SaveAll or Len (strCurrentFilename) = 0 Then 
        strFilename = ShowGameFileDialog (SaveDialog) 
        SaveChangesOrAll = SaveAll
    Else ' Save changes only
        strFilename = strCurrentFilename
    End If 

    ' If we still haven't got a filename, the user pressed cancel, so cancel the save.
    If Len (strFilename) = 0 Then
       Exit Sub 
    End If
    
    ' Don't allow a SaveAll over the top of an existing project
    If SaveChangesOrAll = SaveAll And fso.FileExists (strFilename) Then
       If MsgBox ("Save failed. You are not permitted to save over an existing project.", _
                   vbExclamation + vbRetryCancel, GlobalScript.g_strTitle) = vbRetry Then 
          SaveProject (SaveAll)
       End If
       Exit Sub 
    End If

    ' Now we have the project filename, inform other scripts that we're about to save a project.
    Broadcast.PreSaveProject strFilename

    ' Save the spline assets.
    RWSSpline.Save

    ' Make sure the stream editor updates the custom property
    StreamEditControl.StoreInAsset  

    ' Don't save API objects if we're trying to save changes and there aren't any.
    Dim bShouldSaveAPIObjects
    bShouldSaveAPIObjects = True
    If SaveChangesOrAll = SaveChanges Then
        If Not GlobalScript.RWSUtils.IsDirty() Then
            bShouldSaveAPIObjects = False
        End If
    End If

    If bShouldSaveAPIObjects Then
        If GlobalScript.RWSUtils.SaveGame(strFilename, SaveChangesOrAll) <> True Then 
           ' The save failed so warn user and produce a SaveAs dialog 
           Dim TryAgain
           TryAgain = MsgBox ("Save failed.", vbExclamation + vbRetryCancel, GlobalScript.g_strTitle) 
           If TryAgain = vbRetry Then 
              SaveProject SaveAll
           End If 
        Else
           ' Save worked OK 
           SetCurrentProjectGlobals (strFilename)
           GlobalScript.UpdateAppCaption 
           
           VersionControl.ImportNewAPIObjects
           
           GlobalScript.UpdateRecentFilesList 
           GlobalScript.UpdateVersionControlMenu 
           GlobalScript.UpdateUndoUserInterface 
           If SaveChangesOrAll = SaveAll Then 
               ' Update version control status 
               VersionControl.RefreshAllObjectsManagedState ()
           End If
        End If 
    End If 

    ' Now try and save the game's settings.
    SaveGameSettings

    ' Inform other scripts that we've just saved the project.
    Broadcast.PostSaveProject strFilename
End Sub

' --------------------------------------------------------------------
' Loads the settings appropriate to the current game.
Sub LoadGameSettings ()
    If Len (strCurrentFilename) = 0 Then
        Exit Sub
    End If

    Dim Filepath, AppProperties, GameSettings, HiddenObjects, DisplayOptions, StreamViewerSettings
    Dim EditOptions, EditOptionsSettings

    Filepath = BuildSettingsFilePath (strCurrentFilename)
    If Len (Filepath) > 0 Then
        
        ' create the app properties object to load the settings
        Set AppProperties = CreateObject("CSL.AppProps.Settings")
        Set GameSettings  = GlobalScript.RWSUtils.GetSettings ("Game")
        Set HiddenObjects = GlobalScript.RWSUtils.GetSettings ("HiddenObjects") '  - the hidden entities UIDs
        Set DisplayOptions = GlobalScript.RWSUtils.GetSettings ("DisplayOptions") ' - the entity display options
        Set StreamViewerSettings = StreamViewer.GetSettings ("View Settings")
        
        AppProperties.AddObject "Game Settings", GameSettings
        AppProperties.AddObject "HiddenObjects", HiddenObjects
        AppProperties.AddObject "DisplayOptions", DisplayOptions
        AppProperties.AddObject "StreamViewer Settings", StreamViewerSettings
        
        ' Now load the file
        AppProperties.Load Filepath
        AppProperties.RemoveObjects

        'If the user has the setting turned on for
        'reselecting the pick/fly selection then do so....
        If IsObject(GlobalScript.WorkspaceSettingsObj) = True then
            If GlobalScript.WorkspaceSettingsObj.SaveCameraPosition = True then
                'Restore Camera Settings
                Set EditOptions         = CreateObject("CSL.AppProps.Settings")
                Set EditOptionsSettings = GlobalScript.RWSUtils.GetSettings( "EditOptions")
                EditOptions.AddObject "EditOptions Settings", EditOptionsSettings
                EditOptions.Load Filepath
                EditOptions.RemoveObjects
            End If
        End If
    End If

    ' Only use the edit settings if the edit settings have been requested to be used.
    GlobalScript.RWSUtils.RestoreCameraSettingsOnLoad Globalscript.WorkspaceSettingsObj.SaveCameraPosition

    ' Open xml settings file
    If Len (Filepath) > 0 Then
        Dim XMLDoc
        Set XMLDoc = CreateObject ("Msxml2.DOMDocument.3.0")
        XMLDoc.Async = False
        XMLDoc.Load Filepath

        Dim RootNode
        Set RootNode = XMLDoc.SelectSingleNode ("/OBJECT")
        If RootNode Is Nothing Then
            Set RootNode = XMLDoc.CreateElement ("OBJECT")
            XMLDoc.AppendChild RootNode
        End If

        If Not RootNode Is Nothing Then
            ' Broadcast an event to any controls that wish to load game specific settings
            Broadcast.LoadGameSettings RootNode
        End If
    End If
End Sub

' --------------------------------------------------------------------
' Saves the settings appropriate to the current game. 
Sub SaveGameSettings ()
    If Len (strCurrentFilename) = 0 Then
        Exit Sub
    End If

    Dim Filepath, AppProperties, GameSettings, EditOptionsSettings, StreamViewerSettings
    Dim HiddenObjects, DisplayOptions, file, strRetryMessage

    Filepath = BuildSettingsFilePath (strCurrentFilename)
    If Len (Filepath) > 0 Then
        strRetryMessage = "The game settings file is read only: " & vbCR & Filepath & vbCR & _
                          "The file should not be under source control or set as read only." & vbCR & _
                          "Make it write enabled to allow saving, then click Yes." & vbCR & _
                          "Do you wish to try saving the file again?"
        If CanTryToSaveFile (Filepath, strRetryMessage) Then
            ' Open xml settings file
            Dim XMLDoc 
            Set XMLDoc = CreateObject ("Msxml2.DOMDocument.3.0")
            XMLDoc.Async = False
            XMLDoc.Load Filepath

            Dim RootNode
            Set RootNode = XMLDoc.SelectSingleNode ("/OBJECT")
            If RootNode Is Nothing Then
                  Set RootNode = XMLDoc.CreateElement ("OBJECT")
                  XMLDoc.AppendChild RootNode
            End If

            ' Ask modules to save data to xml
            If Not RootNode Is Nothing Then
                Broadcast.SaveGameSettings XMLDoc, RootNode
                XMLDoc.Save Filepath
            End If

            ' create the app properties object to load the settings
            Set AppProperties        = CreateObject("CSL.AppProps.Settings")
            Set GameSettings         = GlobalScript.RWSUtils.GetSettings ("Game")
            Set EditOptionsSettings  = GlobalScript.RWSUtils.GetSettings ("EditOptions") ' - the camera posn, etc.
            Set HiddenObjects        = GlobalScript.RWSUtils.GetSettings ("HiddenObjects")' - the hidden entities UIDs
            Set DisplayOptions       = GlobalScript.RWSUtils.GetSettings ("DisplayOptions")' - the entity display options
            Set StreamViewerSettings = StreamViewer.GetSettings ("View Settings")
              
            AppProperties.AddObject "Game Settings",         GameSettings
            AppProperties.AddObject "EditOptions Settings",  EditOptionsSettings
            AppProperties.AddObject "HiddenObjects",         HiddenObjects
            AppProperties.AddObject "DisplayOptions",        DisplayOptions
            AppProperties.AddObject "StreamViewer Settings", StreamViewerSettings        

            ' Now save the file
            AppProperties.Save Filepath
            AppProperties.RemoveObjects
        Else
            ' Settings file is read only, warn the user of the consequences and keep the game modified.
            MessageLog.Warning "[Workspace] Game settings file not saved. Any changes made to it will be lost if it is not saved."
            Modified
        End If
    End If
End Sub

' --------------------------------------------------------------------
' Returns the full filename for the settings file for a project saved
' at location defined in 'strProjectFilename'
Function BuildSettingsFilePath (strProjectFilename)
    Dim Filepath, Filename, ExtPos, Result

    ' If the last character is a dot, then remove it
    Filepath = strProjectFilename
    If Mid(Filepath, Len(Filepath), 1) = "." Then
        Filepath = Mid(Filepath, 1, Len(Filepath)-1)
    End If

    ExtPos = Instr(1, GetExtensionName (Filepath), "rwstudio", 1) ' case insensitive search
    If ExtPos > 0 Then ' don't use the rwstudio extension
        Filename = GetBaseNameLessExtension (Filepath)
    Else ' use the extension.
        Filename = GetBaseName (Filepath)
    End If

    Result = GetParentFolderName (Filepath)
    Result = Result & "/" & Filename
    Result = Result & "/" & Filename & ".settings"

    BuildSettingsFilePath = Result
End Function

' --------------------------------------------------------------------  
' Show a file open/save dialog for RWStudio games. 
' bLoadOrSaveDialog - Use one of the constants, LoadDialog or
'                     SaveDialog to specify which dialog to show.
' Returns the name of the file selected.
Function ShowGameFileDialog (bLoadOrSaveDialog)
    Dim oCommCtrl 
    Set oCommCtrl = CreateObject ("CSL.RENCommonDialog") 
 
    With oCommCtrl  
        If  GlobalScript.WorkspaceSettingsObj.LastDirectory <> "" then 
            .InitDir = GlobalScript.WorkspaceSettingsObj.LastDirectory 
        Else 
            .InitDir = AppSettings.InstallDirectory 
        End If 
 
        .Filter = "RWS Project File (*.rwstudio)|*.rwstudio" 
        .FilterIndex = 2 
        .MaxFileSize = 260 
        .hwndOwner = RENManager.GetObjectHWND("CSLRenderwareStudioWorkspace") 
        If bLoadOrSaveDialog= LoadDialog Then 
            .flags = .OFN_HIDEREADONLY + .OFN_FILEMUSTEXIST 
            .DialogTitle = "Open Project"
            .showopen() 
        Else ' Saving ... 
            .flags = .OFN_HIDEREADONLY + .OFN_PATHMUSTEXIST 
            .DialogTitle = "Save Project As" 
            ' Set a default name if we don't currently have one. 
            If Len (strCurrentFilename) = 0 Then 
                .Filename = "game.rwstudio" 
            Else 
                .Filename = strCurrentFilename 
            End if 
            ' only check for existing paths if the Save button is pressed 
            If Not .showsave() Then 
                ' Cancel was pressed, reset the dialog's filename. 
                .Filename = "" 
            End if 
        End If 
 
        ShowGameFileDialog = oCommCtrl.Filename 
 
        If oCommCtrl.Filename  <> "" then 
            GlobalScript.WorkspaceSettingsObj.LastDirectory  = .Filename 
        End If 
    End With 
End Function 
 
' --------------------------------------------------------------------   
' Show a file open dialog for finding an asset. 
' Returns the name of the file selected. 
Function ShowAssetLocateFileDialog () 
    Dim oCommCtrl  
    Set oCommCtrl = CreateObject ("CSL.RENCommonDialog")  
  
    With oCommCtrl 
        .Filter = "All Files (*.*)|*.*|" &_ 
                  "Animation Files (*.anm)|*.anm|" &_ 
                  "World Files (*.bsp)|*.bsp|" &_ 
                  "Clump Files (*.dff)|*.dff|" &_ 
                  "RF3 Files (*.rf3)|*.rf3|" &_ 
                  "RenderWare Stream Files (*.rws)|*.rws|" &_  
                  "Spline Files (*.spl)|*.spl" 
        .FilterIndex = 7 ' Filter on All Files initially 
        .MaxFileSize = 260  
        .hwndOwner = RENManager.GetObjectHWND("CSLRenderwareStudioWorkspace")  
        .flags = .OFN_HIDEREADONLY + .OFN_FILEMUSTEXIST  
        .DialogTitle = "Select asset file" 
        .showopen() 
 
        ShowAssetLocateFileDialog = oCommCtrl.Filename 
    End With  
End Function 

' ------------------------------------------------------------------------------ 
' Function GetExtensionName (path) 
' 
' Returns the file extension for a given file. 
' The file or directories do not have to exists to successfully call this 
' function. This is just a string parsing function. 
' 
' Example: path = "C:\first\second\third\test.filename.txt" 
' Returns:        "txt" 
' 
Function GetExtensionName (path) 
   Dim pos, i
   GetExtensionName = "" 
 
   pos = GetFinalDirPosition(path) 
   If pos > 0 Then 
      For i = Len (path) To pos Step -1 
         If Mid (path, i, 1) = "." Then 
            GetExtensionName = Mid (path, i+1, Len(path)-i) 
            exit Function 
         End If 
      Next 
   End If 
End Function 
 
' ------------------------------------------------------------------------------ 
' Function GetBaseNameLessExtension (path) 
' 
' Returns the file name of the given file and path name. 
' The file or directories do not have to exists to successfully call this 
' function. This is just a string parsing function. 
' 
' Example: path = "C:\first\second\third\test.filename.txt" 
' Returns:        "test.filename.txt" 
' 
Function GetBaseNameLessExtension (path) 
   Dim name, ext
   GetBaseNameLessExtension = "" 
 
   name = GetBaseName (path) 
   ext  = GetExtensionName (path) 
   If Len(name) > 0 And Len (ext) > 0 Then 
      GetBaseNameLessExtension = Mid(name, 1, Len(name)-Len(ext)-1) 
   End If 
End Function 
 
' ------------------------------------------------------------------------------ 
' Function GetBaseName (path) 
' 
' Returns the file name of the given file and path name, including the 
' extension. 
' The file or directories do not have to exists to successfully call this 
' function. This is just a string parsing function. 
' 
' Example: path = "C:\first\second\third\test.filename.txt" 
' Returns:        "test.filename.txt" 
' 
Function GetBaseName (path) 
   Dim pos
   GetBaseName = "" 
 
   pos = GetFinalDirPosition(path) 
   If pos > 0 Then 
      GetBaseName = Mid (path, pos+1, Len(path)-pos) 
   End If 
End Function 
 
' ------------------------------------------------------------------------------ 
' Function GetParentFolderName (path) 
' 
' Removes the filename, including extension from a path. 
' The file or directories do not have to exists to successfully call this 
' function. This is just a string parsing function. 
' 
' Example: path = "C:\first\second\third\test.filename.txt" 
' Returns:        "C:\first\second\third" 
' 
Function GetParentFolderName (path)
   Dim pos
   GetParentFolderName = "" 
 
   pos = GetFinalDirPosition(path) 
   If pos > 0 Then 
      GetParentFolderName = Mid (path, 1, pos-1) 
   End If 
End Function 
 
' ------------------------------------------------------------------------------ 
' Function GetFinalDirPosition (path) 
' 
' Returns the position of the final \ in a path. 
' Example: path = "C:\first\second\third\test.filename.txt" 
' Returns: 22 
' 
Function GetFinalDirPosition (path) 
   Dim size, i
   GetFinalDirPosition = 0 
 
   path = NormalisePath (path) 
   size = Len (path) 
   If size < 1 Then 
      exit Function 
   End If 
 
   For i = size To 1 Step -1 
      If Mid (path, i, 1) = "\" Then '"
         GetFinalDirPosition = i 
         exit Function 
      End If 
   Next 
End Function 
 
' ------------------------------------------------------------------------------ 
' Function NormalisePath (path) 
' 
' Normalises the string passed in to ensure that all path separators 
' are back slashes, not forward slashes. 
' 
Function NormalisePath (path) 
   Dim i
   NormalisePath = "" 
   For i = 1 to Len (path) 
      If Mid (path, i, 1) = "/" Then 
         NormalisePath = NormalisePath & "\" 
      Else 
         NormalisePath = NormalisePath & Mid (path, i, 1) 
      End If 
   Next 
End Function

'------------------------------------------------------------------------------
' This function returns the directory where all intermediary output files are
' created when performing a build. The platform name is used as the directory
' name underneath the "Build Output" directory. Therefore, all platform names
' should be unique - Note: this is not checked here!
Function GetPlatformBuildDirectory (strPlatformName)
   Dim strPlatformFolder, InvalidCharacters, Index, Character

   ' Replace invalid filespec characters from the platform name
   InvalidCharacters = "/\:*?<>|" & Chr (34)  ' Chr (34) is double-quote
   For Index = 1 To Len (strPlatformName)
      Character = Mid (strPlatformName, Index, 1)
      If InStr (InvalidCharacters, Character) Then
         Character = "_"
      End If
      strPlatformFolder = strPlatformFolder + Character
   Next

   GetPlatformBuildDirectory = GetBuildDirectory ("Build Output\" & strPlatformFolder)
End Function

'------------------------------------------------------------------------------
' This function returns the directory where the current project is saved (the
' temp directory if not saved yet) and appends the directory name passed in.
Function GetBuildDirectory (strFolderName)
   Dim strBuildDir, tfolder

   strBuildDir = strCurrentFilename
   If Instr (1, Right (strBuildDir, 9), ".rwstudio", 1) Then ' Instr: Case insensitive search
      strBuildDir = Left (strBuildDir, Len (strBuildDir) - 9)
   End If

   ' If the game is not saved yet, use a temporary location
   If Len (strBuildDir) < 1 Then
      Set tfolder = fso.GetSpecialFolder (TemporaryFolder)
      strBuildDir = tfolder.Path & "\RenderWare Studio"
   End If

   strBuildDir = strBuildDir & "\" & strFolderName & "\" '"

   GetBuildDirectory = strBuildDir
End Function

'------------------------------------------------------------------------------
' This function returns the filename of the converted database if a conversion
' to update to latest version is necessary.
' bHasConverted will be modified in this function to return if a database has
' been converted.
Function CheckAndConvertDBVersion (strFilename, bHasConverted)
   bHasConverted = False

   ' Open the xml file and check the version
   Dim XMLDoc
   Set XMLDoc = CreateObject ("Msxml2.DOMDocument.3.0")
   XMLDoc.Async = False
   XMLDoc.Load strFilename

   Dim RootNode
   Set RootNode = XMLDoc.SelectSingleNode ("/RenderWareStudio")
   If RootNode Is Nothing Then ' No root node - this isn't likely to load...
      CheckAndConvertDBVersion = strFilename
      Exit Function
   End If

   ' If version is 2.0 or above, no action necessary
   Dim strVersion
   strVersion = RootNode.GetAttribute ("version")
   If Len (strVersion) > 0 Then
      Dim OriginalLocale
      OriginalLocale = SetLocale ("en-gb")
      If IsNumeric (strVersion) Then
          If CSng (strVersion) > 1.5 Then
             CheckAndConvertDBVersion = strFilename
             SetLocale OriginalLocale
             Exit Function
          End If
      End If
      SetLocale OriginalLocale
   End If

   ' If we got here, we need to ask for a file to output to
   If vbNo = MsgBox ("This database must be converted to the RWS 2.0 file format.  " & _
                     "If you wish to proceed with the conversion, select ""yes"", " & _
                     "and then enter a new path to save the project to on the resulting dialog.", _
                     vbYesNo + vbQuestion, "Proceed with database conversion?") _
   Then
      CheckAndConvertDBVersion = ""
      Exit Function
   End If

   ' Go for conversion
   Dim bOK
   Dim strOldFilename, strSafeCurrentFilename
   strSafeCurrentFilename = strCurrentFilename
   strCurrentFilename = "" ' set to "" so dialog presents a default project path
   strOldFilename = strFilename
   Do
      strFilename = ShowGameFileDialog (SaveDialog)
      bOK = True
      If Len (strFilename) > 0 Then
         If strOldFilename = strFilename Then
            MsgBox "Please provide a new filename for the project.",, "Invalid project filename"
            bOK = False
         ElseIf fso.FileExists (strFilename) Then
            MsgBox "Please provide a project filename that doesn't exist.",, "Invalid project filename"
            bOK = False
         End If
      End If
   Loop While bOK = False

   If Len (strFilename) > 0 Then
      ' Restore global current file name
      strCurrentFilename = strSafeCurrentFilename

      CSLRenderwareStudioWorkspace.ShowObject MessageLog

      ' Convert project
      MessageLog.Log "[Workspace] Converting project, please wait..."
      If ConvertProjectToVersion2 (strOldFilename, strFilename) Then
         MessageLog.Log "[Workspace] Database conversion Complete."
         bHasConverted = True
      Else
         MessageLog.Error "[Workspace] Database conversion Failed."
         strFilename = ""
      End If
   End If

   if bHasConverted = true then
      VerifyConversion.DoVerify strOldFilename, strFilename
   End If

   CheckAndConvertDBVersion = strFilename
End Function

'------------------------------------------------------------------------------
Function ConvertProjectToVersion2 (strOldFilename, strFilename)
   ' Create a GPM to build perform the conversion
'   Dim oMake
'   Set oMake = CreateObject ("CSL.RWSBuild.Make")
'   GlobalScript.RWSMakeProxy.ConnectMake oMake

   ' Add the conversion task
'   oMake.LoadRule AppSettings.BuildRulesDirectory & "\Workspace.rule", True
'   oMake.AddTask "ConvertProjectToVersion2", strOldFilename, strFilename
'   oMake.SetParam "TOOLDIR", AppSettings.ToolDirectory

   ' Build the task
'   ConvertProjectToVersion2 = oMake.Build ()
'   GlobalScript.RWSMakeProxy.DisconnectMake oMake

   ' This code can be replaced with the above code when Alienbrain 7 code goes in...
   Dim strConverterToolExe, strConverterCommand
   strConverterToolExe = AppSettings.ToolDirectory & "\" & "DBConverter.exe"
   strConverterCommand = """" & strConverterToolExe & """ """ & strOldFilename & """ """ & strFilename & """"

   Dim oShell, oExec, strOutput, cChar
   Set oShell = CreateObject ("WScript.Shell")
   Set oExec = oShell.Exec (strConverterCommand)

   ' Print the output from the command
   Do While oExec.Status = 0
      If Not oExec.StdOut.AtEndOfStream Then
         cChar = oExec.StdOut.Read (1)
         If cChar = Chr (10) OR cChar = Chr (13) Then
            If Len (strOutput) > 0 Then
               MessageLog.Log "[Database converter] " & strOutput
            End If
            strOutput = ""
         Else
            strOutput = strOutput + cChar
         End If
      End If
   Loop

   MessageLog.Log strOutput

   ConvertProjectToVersion2 = oExec.ExitCode = 0

End Function

'------------------------------------------------------------------------------
' Return the UID from the filename of a possible API object
' If the file is not below the currently loaded project location
' then an empty string is returned
Function GetUIDFromFilename (strFilename)
   GetUIDFromFilename = ""
   If Len (strFilename) > 0 Then
      If Right (LCase(strFilename), 4) = ".xml" Then
         ' The file is an possibly a Studio object
         If Left (LCase(strFilename), Len (strCurrentFolder)) = LCase(strCurrentFolder) Then
            ' The file is under the top level project folder so may relate to current project
            ' Extract UID from file name
            GetUIDFromFilename = fso.GetBaseName (strFilename)
         End If
      End If
   End If
End Function

'------------------------------------------------------------------------------
' Return the API object that relates to the given local filename
Function GetAPIObjectFromFilename (strFilename)
   Dim oAPIObject
   Set oAPIObject = Nothing
   
   Dim strUID
   strUID = GetUIDFromFilename (strFilename)
   Set oAPIObject = Database.GetAPIObjectFromUID (strUID)
   Set GetAPIObjectFromFilename = oAPIObject
End Function

' ------------------------------------------------------------------------------
' Similar to FileSystem.BuildPath except if the FileName has an absolute path then
' the Path and Filename aren't concatenated.
Function BuildPath (Path, Name)
   Dim Drive
   Drive = fso.GetDriveName ( Name )

   ' Don't append file to path if absolute file name
   If Len (Drive) = 0 Then
      BuildPath = fso.BuildPath (Path, Name)
   Else
      BuildPath = Name
   End If
End Function

'------------------------------------------------------------------------------
' Reload the project database, retaining current user settings
Sub ReloadProjectDatabase
   ' Make sure we have a current game object
   Dim oGame
   Set oGame = Nothing
   On Error Resume Next
   Set oGame = RWSScript.Game ()
   On Error Goto 0
   
   If Not oGame Is Nothing Then
      ' Take copy of current camera matrix
      Dim CameraAttributeData
      Set CameraAttributeData = RWSScript.Game.CameraAttribute.Data 

      ' Take copy of current working and active folder UIDs
      Dim oWorkingFolder, oActiveFolder, strWorkingFolderUID, strActiveFolderUID
      
      Set oWorkingFolder = RWSScript.Game.WorkingFolder
      If Not oWorkingFolder Is Nothing Then
         strWorkingFolderUID = oWorkingFolder.UID
      End If
      
      Set oActiveFolder = RWSScript.Game.ActiveFolder
      If Not oActiveFolder Is Nothing Then
         strActiveFolderUID = oActiveFolder.UID
      End If
   End If
   
   ' Reload the project
   LoadProject strCurrentFilename
   ' Note that after a reload all oAPIObject objects and IDs will be invalid
   
   ' Try and get new game object (load may have failed)
   Set oGame = Nothing
   On Error Resume Next
   Set oGame = RWSScript.Game ()
   On Error Goto 0
   
   If Not oGame Is Nothing Then
      ' Reset working and active folders
      Set oWorkingFolder = Database.GetAPIObjectFromUID (strWorkingFolderUID)
      If Not oWorkingFolder Is Nothing Then
         RWSScript.Game.WorkingFolder = oWorkingFolder
      End If
      
      Set oActiveFolder = Database.GetAPIObjectFromUID (strActiveFolderUID)
      If Not oActiveFolder Is Nothing Then
         RWSScript.Game.ActiveFolder = oActiveFolder
      End If
      
      ' Reset camera matrix
      RWSScript.Game.CameraAttribute.Data = CameraAttributeData
   End If
End Sub

'------------------------------------------------------------------------------
' Something in RWSUtils has changed, set the modify flag.
Sub RWSUtils_OnModified (bUtilsModified)
   If bUtilsModified Then
      Modified
   Else
      bModified = False
      MainMenu.EnableMenuItem     23, False ' Save Project 
      MainToolbar.EnableMenuItem  12, False ' Save Project
   End If
End Sub

'------------------------------------------------------------------------------
' Function for all scripts to call when they've modified some data that will
' need saving. This function enables the save menu item and button.
Sub Modified ()
   If (GlobalScript.g_DatabaseID > 0) Or bSettingGameProperties Then
      bModified = True
      MainMenu.EnableMenuItem     23, True ' Save Project 
      MainToolbar.EnableMenuItem  12, True ' Save Project
   End If
End Sub

'------------------------------------------------------------------------------
' Function to check if a file can be save at a specified location. If there is
' already a file created, check if it is read only and offer the user chance to
' make it writable.
Function CanTryToSaveFile (strFilename, strReadOnlyRetryMessage)
   Dim retryEntry, file
   If fso.FileExists(strFilename) Then
      Set file = fso.GetFile (strFilename)
      Do
         If file.attributes and 1 Then '(1 = read only)
            retryEntry = MsgBox (strReadOnlyRetryMessage, _
                                 vbYesNo + vbQuestion, _
                                 GlobalScript.g_strTitle)
            If retryEntry <> vbYes then
               CanTryToSaveFile = False
               Exit Function
            End If
         Else
            ' File is not read only, so we can try to save the file here.
            CanTryToSaveFile = True
            Exit Function
         End If
      Loop While retryEntry = vbYes
   Else
      ' File doesn't exist, so we can try to save the file here.
      CanTryToSaveFile = True
      Exit Function
   End If
End Function

'------------------------------------------------------------------------------
' This event is fired out during a load after the database has been created,
' but prior to loading the XML files.
Sub RWSUtils_OnDatabaseCreated ()
    ' If there isn't a settings file, show the game's properties
    Dim strSettingsFilepath
    strSettingsFilepath = BuildSettingsFilePath (strCurrentFilename)
    If Not fso.FileExists (strSettingsFilepath) Then
        Dim Container, Selection 
        Set Container = CreateObject ("CSL.RWSScript.RWSContainer") 
        Container.Add GlobalScript.RWSScript.Game.ID
        bSettingGameProperties = True
        GlobalScript.RWSUtils.ShowProperties Container, True
        bSettingGameProperties = False
    End If
End Sub
