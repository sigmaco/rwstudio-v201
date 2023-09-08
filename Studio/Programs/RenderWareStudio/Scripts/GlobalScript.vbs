'
' Enterprise Layout Management Script
'
' Layout object 'GlobalScript'
'

'------------------------------------------------------------------------------
' Global Variables
'------------------------------------------------------------------------------

Dim g_strTitle, g_strGlobalSelection, g_strResourceRoot, g_strSourceRoot
Dim g_strRF3ProjectTemplateFile, g_strCommandLineClean, g_strCommandLineBuild
Dim g_bLoading, g_DatabaseID, g_strCommandLineGame
g_DatabaseID                = 0
g_strTitle                  = "RenderWare Studio"
g_strGlobalSelection        = "GlobalSelection" ' Selection identifier used by modules who access the global selection
g_strResourceRoot           = ""
g_strSourceRoot             = ""
g_strRF3ProjectTemplateFile = ""
g_strCommandLineClean       = ""
g_strCommandLineBuild       = ""
g_strCommandLineGame        = ""

Dim g_CommandLineClose, UndoFlag, RedoFlag
g_CommandLineClose = False
UndoFlag           = True
RedoFlag           = False
g_bLoading         = False

Dim gbDirectorsCameraEnabled  
gbDirectorsCameraEnabled = False  

Dim gbPauseGameEnabled
gbPauseGameEnabled = False

Dim g_bSwitchingFolders
g_bSwitchingFolders = False

RENManager.ModalParentHWND = RENManager.GetObjectHWND("CSLRenderwareStudioWorkspace")

'------------------------------------------------------------------------------
' Global COM objects
'------------------------------------------------------------------------------
Dim RWSUtils, Undo, StreamEditControl
Dim WorkspaceSettingsObj, RWSScript, RWSComms, RWSMakeProxy

Set RWSUtils             = CreateObject ("CSL.RWSUtils.Utility")
Set Undo                 = CreateObject ("CSL.Undo.CareTaker")
Set StreamEditControl    = CreateObject ("CSL.RWSStreamEditControl.RWSStreamEdit")  
Set WorkspaceSettingsObj = CreateObject ("CSL.RWSUtils.RWSWorkspaceSettings")
Set RWSScript            = CreateObject ("CSL.RWSScript.RWSScript")
Set RWSComms             = CreateObject ("CSL.RWSTarget.RWSComms")
Set RWSMakeProxy         = CreateObject ("CSL.RWSBuild.MakeEventProxy")

' Allow the other scripts to receive the COM objects' events:
RENHost.ConnectObject RWSUtils,               "RWSUtils"
RENHost.ConnectObject Undo,                   "Undo"
RENHost.ConnectObject StreamEditControl,      "StreamEditControl" 
RENHost.ConnectObject RWSScript,              "RWSScript"
RENHost.ConnectObject RWSComms,               "RWSComms"
RENHost.ConnectObject RWSMakeProxy,           "RWSMakeProxy"

Dim RecentFileMarker, RecentFileParent, NumRecentFiles
RecentFileMarker = 1000
RecentFileParent = 310
NumRecentFiles   = 0

Dim g_CloseImmediately
g_CloseImmediately = False

' Visual Studio .NET shuts down when the DTE (application) object  
' is no longer referenced  
Dim g_VSDTE


'------------------------------------------------------------------------------  
' Global Constants    
'------------------------------------------------------------------------------  

const strMakeAssociation = "RWSMAKE"

' Object types as specified within RWSScript type library  
Const otGame                 = 1  
Const otEntity               = 2  
Const otFolder               = 4  
Const otAsset                = 8  
Const otAssetFolder          = 16  
Const otTemplate             = 32  
Const otTemplateFolder       = 64  
Const otBehavior             = 128  
Const otBehaviorFolder       = 256  
Const otCommand              = 512  
Const otAttribute            = 1024  
Const otAttributeShare       = 2048  
Const otAttributeShareFolder = 4096  
Const otEvent                = 8192  

' Visual C++ 6 automation constants
Const dsExtend = 1

' ---------------------------------------------------------------------
Sub RWSUtils_OnSetActiveFolder (FolderID)
    Dim OldFolderID
    OldFolderID = TargetContextMenu.ActiveFolderID 

    If Not OldFolderID = FolderID Then 
        SwitchFolders OldFolderID 'pass previous folder 
    End If

    ' Broadcast the event to say active folder changed
    Broadcast.ActiveFolderChanged FolderID
End Sub

' ---------------------------------------------------------------------
Sub RWSUtils_OnGetFileType (AssetID, strFilename, strType)
    strType = GetAssetFileType (AssetID, strfilename)
End Sub

' --------------------------------------------------------------------
' GPM events
' --------------------------------------------------------------------
Sub RWSMakeProxy_BuildLog (Make, strRule, strLog)
   BuildLog.Log "[" & strRule & "] " & strLog
End Sub

Sub RWSMakeProxy_BuildWarning (Make, strRule, strWarning)
   BuildScriptManager.g_intNumBuildWarnings = BuildScriptManager.g_intNumBuildWarnings + 1
   BuildLog.Warning "[" & strRule & "] " & strWarning
End Sub

Sub RWSMakeProxy_BuildError (Make, strRule, strError)
   BuildScriptManager.g_intNumBuildErrors = BuildScriptManager.g_intNumBuildErrors + 1
   CSLRenderwareStudioWorkspace.ShowObject BuildLog
   BuildLog.Error "[" & strRule & "] " & strError
End Sub

Sub RWSMakeProxy_SystemError (Make, BuildError)
   Dim Message
   Message = "["
   If Len (BuildError.Rule) > 0 Then
      Message = Message & BuildError.Rule & " Rule"
   Else
      Message = Message & "Game Production Manager"
   End If
   Message = Message & "]"

   If Not BuildError.LineNumber = 0 Then
      Message = Message & " (Line " & BuildError.LineNumber & ")"
   End If

   If Len (BuildError.Message) > 0 Then
      Message = Message & " " & BuildError.Message
   End If

   If Len (BuildError.Description) > 0 Then
      Message = Message & " - " & BuildError.Description
   End If

   CSLRenderwareStudioWorkspace.ShowObject BuildLog
   BuildLog.Message BuildError.ErrorLevel, Message
   If BuildError.ErrorLevel = MessageLog.conWarning Then
      BuildScriptManager.g_intNumWarnings = BuildScriptManager.g_intNumWarnings + 1
   ElseIf BuildError.ErrorLevel = MessageLog.conError Then
      BuildScriptManager.g_intNumBuildErrors = BuildScriptManager.g_intNumBuildErrors + 1
   End If
End Sub

' ---------------------------------------------------------------------
Function GetAssetFileType (AssetID, strFilename)
    ' Broadcast the event to get the asset's type
    Dim bHandled, strAssetsType
    bHandled      = False
    strAssetsType = ""

    Broadcast.GetAssetResourceType AssetID, strFilename, strAssetsType, bHandled
    If bHandled And Len (strAssetsType) > 0 Then
        GetAssetFileType = strAssetsType
    Else
        GetDefaultFileResourceType = "Unknown"
        If persist.fso.FileExists (strFilename) Then
            Dim FileExt
            FileExt = persist.fso.GetExtensionName (strFilename)
            If Len (FileExt) > 0 Then
                GetAssetFileType = UCase (FileExt)
            End If
        End If
    End If
End Function

' ---------------------------------------------------------------------
' Catch the broadcast event to get the type of an asset's resource.
Sub Broadcast_GetAssetResourceType (AssetID, strResourceFilename, strAssetType, bHandled)
    If Not bHandled Then
        Dim nGroupIndex, ChunkGroupIndexTag

        ' Register the tag used for the run-time assets
        Set ChunkGroupIndexTag = RWSScript.RegisterTag (Tags.conChunkGroupIndex)
        ' Look for the named-group index tag
        nGroupIndex = ChunkGroupIndexTag.Get (AssetID)

        ' Try to determine the RenderWare stream type
        strType = DesignView.RWSUtility.GetStreamType (strResourceFilename, nGroupIndex)

        ' If we failed, then attempt to determine the file type from its extension
        If strType = "Unknown" Then
            Dim fso
            Set fso = CreateObject ("Scripting.FileSystemObject")
            If fso.FileExists (strResourceFilename) Then
                FileExt = fso.GetExtensionName (strResourceFilename)
                If LCase (FileExt) = "ka" Then
                    strType = "kID_KASSET"
                ElseIf LCase (FileExt) = "txt" Then
                    strType = "TEXT"
                End If
            End If
        End If

        ' If we set a type, use it.
        If strType <> "Unknown" Then
            strAssetType = strType
            bHandled     = True
        End If
    End If
End Sub

' ---------------------------------------------------------------------
Sub UpdateRecentFilesList ()

    'Remove recent files menus
    nItem = RecentFileMarker + 1
    For n = 1 To NumRecentFiles
        MainMenu.DeleteItem nItem
        nItem = nItem + 1
    Next
    MainMenu.MenuItems.Item("&File").MenuItems.Item("&Recent Projects").MenuItems.RemoveAll

    MainMenu.EnableMenuItem RecentFileParent, FALSE
    NumRecentFiles = 0

    'Add recent files to main menu
    arrRecentFiles = RWSUtils.GetRecentFileList ()
    If UBound (arrRecentFiles) >= 0 Then
        MainMenu.EnableMenuItem RecentFileParent , TRUE
        dim maxRecentFiles
        maxRecentFiles = UBound (arrRecentFiles)
        If maxRecentFiles > AppSettings.RecentFiles_MaxFiles Then
            maxRecentFiles = AppSettings.RecentFiles_MaxFiles-1 ' 0-indexed
        End If
        For n = LBound (arrRecentFiles) To maxRecentFiles
            dim shortcut
            If n < 9 Then ' < 9 will catch the first 9.
                shortcut = "&" & CSTR (n+1) & " "
            Else
                If n = 9 Then
                    shortcut = "1&0 "
                Else ' For files 11 upwards, don't have a shortcut.
                    shortcut = CSTR (n+1) & " "
                End If
            End If
            MainMenu.AddSubMenuItem shortcut & arrRecentFiles(n), RecentFileParent, RecentFileMarker + 1 + n, -1, TRUE, "", 0, FALSE, FALSE, FALSE
            NumRecentFiles = NumRecentFiles + 1
        Next
    End If
End Sub

' --------------------------------------------------------------------   
Sub ClearDocs ()
    HelpWindow.ShowHelp "", ""
End Sub

' --------------------------------------------------------------------
' Update the menu options and toolbar buttons that need turning on
' when a game is created or loaded.
Sub SetMenuOptionsOnStartGame (bNewGame)
    ' File Menu
    MainMenu.EnableMenuItem    24, True ' Save Project As...
    MainMenu.EnableMenuItem   353, True ' Import...

    If (RWSUtils.IsOldFileVersion) Then
        ' Disable the save buttons, only Save As should be available.
        MainMenu.EnableMenuItem     23, False ' Save Project 
        MainToolbar.EnableMenuItem  12, False ' Save Project
    End If
    ' ---------
    ' Edit Menu
    MainMenu.EnableMenuItem    30, False ' Undo
    MainMenu.EnableMenuItem    31, False ' Redo
    MainMenu.EnableMenuItem   348, False ' Show undo stack
    MainToolbar.EnableMenuItem  3, False ' Undo
    MainToolbar.EnableMenuItem  4, False ' Redo
    MainToolbar.EnableMenuItem 13, False ' Show undo stack
    ' ---------
    ' View Menu
    MainMenu.EnableMenuItem  42, True ' Refresh Event Vizualiser
    MainMenu.EnableMenuItem 345, True ' Show all entities in active folder
    MainMenu.EnableMenuItem 346, True ' Hide all entities in active folder
    MainToolbar.EnableMenuItem  15, True ' Refresh Event Vizualiser
    ' --------------
    ' Selection Menu
    MainMenu.EnableMenuItem  329, True ' Fly
    MainMenu.EnableMenuItem  330, True ' Orbit
    MainMenu.EnableMenuItem  331, True ' Pan
    MainMenu.EnableMenuItem  332, True ' Zoom
    MainMenu.EnableMenuItem  333, True ' Next
    MainMenu.EnableMenuItem  352, True ' Prev
    MainMenu.EnableMenuItem  207, True ' Locate
    MainMenu.EnableMenuItem  362, True ' Aim At
    MainMenu.EnableMenuItem  357, True ' Pick only
    MainMenu.EnableMenuItem  328, True ' Pick/translate
    MainMenu.EnableMenuItem  349, True ' Pick/rotate
    MainMenu.EnableMenuItem  350, True ' Pick/scale
    MainMenu.EnableMenuItem  363, True ' Pick/drag
    MainMenu.EnableMenuItem  367, True ' Pick materials
    MainMenu.EnableMenuItem  358, True ' Lock selection
    MainMenu.EnableMenuItem  359, True ' Orbit only
    MainMenu.EnableMenuItem  360, True ' Orbit objects
    MainMenu.EnableMenuItem  361, True ' Camera arc
    MainMenu.EnableMenuItem  351, True ' Toggle Pick/Camera
    SelectionToolbar.EnableMenuItem 1, True ' Fly
    SelectionToolbar.EnableMenuItem 2, True ' Orbit
    SelectionToolbar.EnableMenuItem 3, True ' Pan
    SelectionToolbar.EnableMenuItem 4, True ' Zoom
    SelectionToolbar.EnableMenuItem 8, True ' Pick only
    SelectionToolbar.EnableMenuItem 5, True ' Pick/translate
    SelectionToolbar.EnableMenuItem 6, True ' Pick/rotate
    SelectionToolbar.EnableMenuItem 7, True ' Pick/scale
    SelectionToolbar.EnableMenuItem 13, True ' Pick/drag
    SelectionToolbar.EnableMenuItem 14, True ' Pick/materials
    SelectionToolbar.EnableMenuItem 9, True ' Lock selection
    SelectionToolbar.EnableMenuItem 10, True ' Orbit only
    SelectionToolbar.EnableMenuItem 11, True ' Orbit objects
    SelectionToolbar.EnableMenuItem 12, True ' Camera arc

    ' --------------
    ' Target Menu
    EnableBuildOptions

    ' ----------
    ' Tools Menu
    MainMenu.EnableMenuItem   342, True ' Parse Source
    MainToolbar.EnableMenuItem 14, True ' Parse Source
    MainMenu.EnableMenuItem   354, True ' Validate Database

    ' --------------
    ' Spline Toolbar
    SplineToolbar.EnableMenuItem 1, False 'New Nodes
    SplineToolbar.EnableMenuItem 2, False 'Insert Node
    SplineToolbar.EnableMenuItem 3, False 'Delete Nodes
    SplineToolbar.EnableMenuItem 4, False 'Open Spline
    SplineToolbar.EnableMenuItem 5, False 'Close Spline

    ' ---------------------
    ' Volume Editor Toolbar
   VolumeToolbar.SetSelectionMenuChecks
   VolumeToolbar.EnableMenus True

End Sub

' --------------------------------------------------------------------
Sub EnableBuildOptions
   If Not BuildScriptManager.strActiveTarget = "" Then
      MainMenu.EnableMenuItem  414, True ' Launch
      CurrentTargetToolbar.EnableMenuItem 1, True ' Launch
      MainMenu.EnableMenuItem  423, True ' Properties

      If g_DatabaseID > 0 Then
         MainMenu.EnableMenuItem  406, True ' Clean
         MainMenu.EnableMenuItem  407, True ' Build
         MainMenu.EnableMenuItem  408, True ' Rebuild
         MainMenu.EnableMenuItem  409, True ' Clean All Folders
         MainMenu.EnableMenuItem  410, True ' Build All Folders
         MainMenu.EnableMenuItem  411, True ' Rebuild All Folders
         MainMenu.EnableMenuItem  412, True ' Connect
         MainMenu.EnableMenuItem  413, True ' Build and Connect
         
         CurrentTargetToolbar.EnableMenuItem 2, True ' Connect
         CurrentTargetToolbar.EnableMenuItem 3, True ' Build & Connect
         CurrentTargetToolbar.EnableMenuItem 4, True ' Clean
         CurrentTargetToolbar.EnableMenuItem 5, True ' Build
         CurrentTargetToolbar.EnableMenuItem 6, True ' Rebuild
      End If
    End If
End Sub

' --------------------------------------------------------------------
Sub UpdateVersionControlMenu
    bHasVersionControlInstalled = VersionControl.IsInstalled ()
    If bHasVersionControlInstalled Then
        bIsManaged = VersionControl.IsProjectManaged ()
        If bIsManaged Then
            MainMenu.EnableMenuItem 339, True  'Get Latest
            MainMenu.EnableMenuItem 340, True  'Refresh
            MainMenu.EnableMenuItem 336, False 'Import Project To alienbrain
            If VersionControl.NumFilesInDefaultChangeSet () > 0 Then
               MainMenu.EnableMenuItem 337, True 'Check In
               MainMenu.EnableMenuItem 338, True 'Undo Check Outs
            Else
               MainMenu.EnableMenuItem 337, False 'Check In
               MainMenu.EnableMenuItem 338, False 'Undo Check Outs
            End If
        Else
            MainMenu.EnableMenuItem 339, False 'Get Latest
            MainMenu.EnableMenuItem 340, False 'Refresh
            MainMenu.EnableMenuItem 336, True  'Import Project To alienbrain
        End If
    Else
        MainMenu.EnableMenuItem 336, False 'Import Project To alienbrain
        MainMenu.EnableMenuItem 337, False 'Check In
        MainMenu.EnableMenuItem 338, False 'Undo Check Outs
        MainMenu.EnableMenuItem 339, False 'Get Latest
        MainMenu.EnableMenuItem 340, False 'Refresh
        MainMenu.EnableMenuItem 365, False 'Show NXN explorer
    End If
End Sub

'---------------------------------------------------------------------
Function CreateDirectory (strFolder, fso) ' fso = FileSystem object
   CreateDirectory = False
   If Not fso.FolderExists (strFolder) Then

      Dim Folder
      Set Folder = Nothing

      ' Try to create folder
      On Error Resume Next
      Set Folder = fso.CreateFolder (strFolder)
      On Error Goto 0

      If Folder Is Nothing Then
         ' Try to recursively create parent folder
         Dim strParentFolder
         strParentFolder = fso.GetParentFolderName (strFolder)

         If CreateDirectory (strParentFolder, fso) Then
            ' Try to create folder
            On Error Resume Next
            Set Folder = fso.CreateFolder (strFolder)
            On Error Goto 0

            If Not Folder Is Nothing Then
               CreateDirectory = True
            End If
         End If
      Else
         CreateDirectory = True
      End If
   End If
End Function

'---------------------------------------------------------------------
' Note: This method may be called prior to the 
' RWSUtils_OnDatabaseAttach event.
Sub RWSUtils_OnSourceRootChanged (strSourceRoot)
   If Persist.bSettingGameProperties Then
      Persist.Modified
   Else
      ParseSource
      g_strSourceRoot = strSourceRoot
      If Not GlobalScript.g_bLoading Then
         Persist.Modified
      End If
   End If
End Sub

' --------------------------------------------------------------------
' Note: This method may be called prior to the 
' RWSUtils_OnDatabaseAttach event.
Sub RWSUtils_OnResourceRootChanged (strResourceRoot)
   If Persist.bSettingGameProperties Then
      Persist.Modified
   Else
      DesignView.RWSUtility.SetResourceRoot strResourceRoot
      GameDatabase.ResourceRoot = strResourceRoot
      AssetLister.ResourceRoot = strResourceRoot
      TemplateLister.ResourceRoot = strResourceRoot
      RWSPreview.SetResourceRoot strResourceRoot
      ImportAsset.SetResourceRoot strResourceRoot
      RWSSpline.SetResourceRoot strResourceRoot
      ContextMenu.SetResourceRoot strResourceRoot 
      StreamViewer.SetResourceRoot strResourceRoot
      StreamEditControl.SetResourceRoot strResourceRoot 
      g_strResourceRoot = strResourceRoot
      If Not GlobalScript.g_bLoading Then
         Persist.Modified
      End If
   End If
End Sub

'---------------------------------------------------------------------
Sub RWSUtils_OnRF3ProjectTemplateChanged (strFullTemplatePath)
   If Persist.bSettingGameProperties Then
      Persist.Modified
   Else
      If Not g_strRF3ProjectTemplateFile = strFullTemplatePath Then
         g_strRF3ProjectTemplateFile = strFullTemplatePath
         If Not g_bLoading Then
            BuildScriptManager.PreReloadAssets
            DesignView.RWSUtility.ReloadAssets
         End If
      End If
      If Not GlobalScript.g_bLoading Then
         Persist.Modified
      End If
   End If
End Sub

'---------------------------------------------------------------------
Sub RWSUtils_OnVolumeTextureRootChanged (strTextureRoot)
   If Persist.bSettingGameProperties Then
      Persist.Modified
   Else
      VolumeToolbar.SetTextureRoot strTextureRoot
      If Not GlobalScript.g_bLoading Then
         Persist.Modified
      End If
   End If
End Sub

' --------------------------------------------------------------------
Sub RWSUtils_OnOutputText (Severity, strMessage)
    MessageLog.Message Severity, strMessage
End Sub

Const GameDatabaseNode = 0
Const BehaviorListerNode = 1
Const AssetListerNode = 2
Const TemplateListerNode = 3
Const AttributeShareListerNode = 4
Const SearchResultsListerNode = 5

' --------------------------------------------------------------------
Sub RWSUtils_OnDatabaseAttached (DatabaseID)
    g_DatabaseID = DatabaseID
    
    ' Attach RWS modules to database
    If Not EntityAttributes.AttachToDatabase (DatabaseID) Then
        DatabaseAttachedError "Entity Attributes"
    End If
    If Not EventVisualizer.AttachToDatabase (DatabaseID) Then
        DatabaseAttachedError "Event Visualizer"
    End If
    If Not GameDatabase.AttachToDatabase (DatabaseID) Then
        DatabaseAttachedError "Game Database"
    End If
    If Not BehaviorLister.AttachToDatabase (DatabaseID) Then
        DatabaseAttachedError "Behavior Lister"
    End If

    If Not AssetLister.AttachToDatabase (DatabaseID) Then
        DatabaseAttachedError "Asset Lister"
    End If

    If Not TemplateLister.AttachToDatabase (DatabaseID) Then
        DatabaseAttachedError = "Template Lister"
    End If
    If Not AttributeShareLister.AttachToDatabase (DatabaseID) Then
        DatabaseAttachedError = "Attribute Share Lister"
    End If
    If Not RWSPreview.AttachToDatabase (DatabaseID) Then
        DatabaseAttachedError "Animation Preview"
    End If
    If Not RWSScript.AttachToDatabase (DatabaseID) Then
        DatabaseAttachedError "RWSScript"
    End If
    If Not RWSEventList.AttachToDatabase (DatabaseID) Then
        DatabaseAttachedError "RWSEventList"
    End If
    If Not SearchResults.AttachToDatabase (DatabaseID) Then
        DatabaseAttachedError "Search Results"
    End If
    If Not RWSSpline.AttachToDatabase (DatabaseID) Then
        DatabaseAttachedError "Spline"
    End If
    If Not Sequencer.AttachToDatabase (DatabaseID) Then
        DatabaseAttachedError "Sequencer"
    End If

    If Not NXNScriptExtension.AttachToDatabase (DatabaseID) Then
        DatabaseAttachedError "NXN Script Extensions"
    End If

    ' Initialize volume editor and enable all volume toolbar controls
    DesignView.VolumeEdit.Initialize
    VolumeToolbar.EnableMenus True

    ' The success of attaching to the database cannot be found because the following results in a posted message.
    DesignView.RWSUtility.AttachToDatabase (DatabaseID)

    ' Populate the listers with data
    GameDatabase.NewDatabase
    BehaviorLister.NewDatabase
    AssetLister.NewDatabase
    TemplateLister.NewDatabase
    AttributeShareLister.NewDatabase
    SearchResults.PopulateContainer CreateObject("CSL.RWSScript.RWSContainer")

    ' Inform other forms that database has been attached to
    ImportAsset.AttachToDatabase (DatabaseID)
    
    ' Stream viewer / editor
    StreamEditControl.AttachToDatabase (DatabaseID)
    StreamEditControl.SetStackControl StreamPropertyList
    StreamViewer.AttachToDatabase (DatabaseID)

    TargetContextMenu.ActiveFolderID = 0
End Sub

' --------------------------------------------------------------------
' Add the error message to the output window and show an Error dialog
Sub DatabaseAttachedError (msg)
    CSLRenderwareStudioWorkspace.ShowObject MessageLog
    MessageLog.Error "[" + msg + "] Cannot attach to database. Contact Criterion developer relations."
    MsgBox msg + " cannot attach to database. Contact Criterion developer relations.", vbExclamation + vbOKCancel - vbOK, "Fatal Error"
End Sub

' --------------------------------------------------------------------
Sub RWSUtils_OnDatabaseDetached ()
   g_DatabaseID = 0
   
   ' Detach RWS modules from database
   EntityAttributes.AttachToDatabase (0)
   EventVisualizer.AttachToDatabase (0)
   GameDatabase.AttachToDatabase (0)
   BehaviorLister.AttachToDatabase (0)
   AssetLister.AttachToDatabase (0)
   TemplateLister.AttachToDatabase (0)
   AttributeShareLister.AttachToDatabase (0)
   DesignView.RWSUtility.AttachToDatabase (0)
   RWSScript.AttachToDatabase (0)
   RWSPreview.AttachToDatabase (0)
   RWSEventList.AttachToDatabase (0)
   ImportAsset.AttachToDatabase (0)
   SearchResults.AttachToDatabase (0)
   RWSSpline.AttachToDatabase (0)
   StreamViewer.AttachToDatabase (0)
   StreamEditControl.AttachToDatabase (0)
   Sequencer.AttachToDatabase (0)

   DesignView.VolumeEdit.Shutdown
   NXNScriptExtension.AttachToDatabase (0)
End Sub

' --------------------------------------------------------------------
' Called when an error occurs while parsing the script
Sub RWSUtils_OnParseError (strFileName, nLineNumber, strErrorDescription)
    strOutputText = "[Source Parser] " +  strFileName + "(" + _
                     CStr (nLineNumber) + ") - " + strErrorDescription

    ' Output the error message in the output window
    CSLRenderwareStudioWorkspace.ShowObject MessageLog
    MessageLog.Error strOutputText
End Sub

'---------------------------------------------------------------------
' Initialization dependant on other objects being loaded.
Sub OnLoad()
    Dim AppProperties

    RENManager.Show "CSLRenderwareStudioWorkspace"

    Set AppProperties  = GetAppProperties

    If len (AppSettings.SettingsFilename) > 0 Then
        AppProperties.Load AppSettings.SettingsFilename

        ' Open xml settings file
        Dim XMLDoc 
        Set XMLDoc = CreateObject ("Msxml2.DOMDocument.3.0")
        XMLDoc.Async = False
        XMLDoc.Load AppSettings.SettingsFilename

        Dim RootNode
        Set RootNode = XMLDoc.SelectSingleNode ("/OBJECT")
        If RootNode Is Nothing Then
            Set RootNode = XMLDoc.CreateElement ("OBJECT")
            XMLDoc.AppendChild RootNode
        End If

        If Not RootNode Is Nothing Then
            Dim RWSRootNode
            Set RWSRootNode = RootNode.SelectSingleNode ("./RWSScriptedSettings")
            If RWSRootNode Is Nothing Then
               Set RWSRootNode = XMLDoc.CreateElement ("RWSScriptedSettings")
               RootNode.AppendChild RWSRootNode
            End If

            If Not RWSRootNode Is Nothing Then
               ' Convert to latest version of settings file if necessary
               Dim strVersion
               strVersion = RootNode.GetAttribute ("VERSION")
               If Len (strVersion) > 0 Then
               Else
                  strVersion = "1.0"
               End If
               ' Only broadcast the update event if we're trying to load an older version of a settings file.
               Dim OriginalLocale, sngVersion
               OriginalLocale = SetLocale ("en-gb")
               sngVersion = CSng (strVersion)
               SetLocale OriginalLocale
               If sngVersion < Persist.sngCurrentSettingsFileVersion Then
                  Broadcast.UpdateApplicationSettingsVersion XMLDoc, RootNode, sngVersion
                  Dim strRetryMessage
                  strRetryMessage = "The application settings file is read only: " & vbCR & AppSettings.SettingsFilename & vbCR & _
                                    "The file should not be under source control or set as read only." & vbCR & _
                                    "Make it write enabled to allow saving, then click Yes." & vbCR & _
                                    "Do you wish to try saving the file again?"
                  If Persist.CanTryToSaveFile (AppSettings.SettingsFilename, strRetryMessage) Then
                      ' Save the updated XML doc.
                      XMLDoc.Save AppSettings.SettingsFilename
                  End If
               End If

               ' Ask modules to load data from xml
               Broadcast.LoadApplicationSettings RWSRootNode
            End If
        End If
    Else
       strOutputText = "[Workspace] Unable to load RWStudio.settings file  < " + Err.Description + " >"
       CSLRenderwareStudioWorkspace.ShowObject MessageLog
       MessageLog.Error strOutputText
    End if

    AppProperties.RemoveObjects

    ' update recent files list
    UpdateRecentFilesList

    ' Set up the Rf3Translator so RWSUtils can access the project template.
    RWSUtils.SetRf3Translator BuildScriptManager.RF3Translator

    'if the user has the setting turned on for
    'reloading the last *project* then do so....
    If IsObject(WorkspaceSettingsObj) = True then
        if WorkspaceSettingsObj.LoadLastProject = True then
            if WorkspaceSettingsObj.LastProject <> "" then
                g_strCommandLineGame = WorkspaceSettingsObj.LastProject
            End If
        End If
    End If


    'if the user has the setting turned on for
    'reselecting the pick/fly selection then do so....
    If IsObject(WorkspaceSettingsObj) = True then
        if WorkspaceSettingsObj.SaveSelectionSettings = True then
            RestoreSelectionSettings
        End If
    End If

    'Set the tooltip options.
    DesignView.Tooltips.TipWaitTime    = AppSettings.Tooltip_WaitTime
    DesignView.Tooltips.TipDisplayTime = AppSettings.Tooltip_DisplayTime

    ' Set the Undo options
    Undo.SetStackSize (AppSettings.Undo_StackSize)

    RENManager.RemoveSplash
    VerifyAppSettings

    If Not RWSUtils.IsRWSAPIInitialized() Then
        MsgBox "Fatal error, API not initialized. Contact Criterion developer relations.", vbExclamation + vbOKCancel - vbOK, "Fatal Error"
        g_CloseImmediately = True
        RENManager.KillLoop ' This calls through to RENManager_OnTerminate
    Else
        ' Set the project filename to load from command line if there is one
        If Len (RenHost.GetCommandArgument ("game")) Then
            g_strCommandLineGame = RenHost.GetCommandArgument ("game")
        End If

        ' Set the option to close the app from the command line if there is one
        ' Useful after an automated build.
        If Len (RenHost.GetCommandArgument ("close")) Then
            If LCase((RenHost.GetCommandArgument ("close"))) = "true" Then
                g_CommandLineClose = True
            End If
        End If

        ' Set the option to clean this game - using the target name.
        Dim strConnectionName, strRowUID
        If Len (RenHost.GetCommandArgument ("clean")) Then
              strConnectionName = RenHost.GetCommandArgument ("clean")
              For Each strRowUID In TargetLink.GetRowUIDs
              If strConnectionName = TargetLink.GetField (strRowUID, "Name") Then
                 g_strCommandLineClean = strRowUID
                 Exit For
              End If
              Next
              If len (strConnectionName) > 0 And Len (g_strCommandLineClean) = 0 Then
                 MessageLog.Warning "[Workspace] Unknown " & _
                                    "target name '" & strConnectionName & _
                                    "' used on command line option /clean - " & _
                                    "Tip: surround name with quotes for space-separated names"
                 g_CommandLineClose = false ' so the user can see the error message
              End If
        End If

        ' Set the option to build this game - using the target name.
        If Len (RenHost.GetCommandArgument ("build")) Then
              strConnectionName = RenHost.GetCommandArgument ("build")
              For Each strRowUID In TargetLink.GetRowUIDs
              If strConnectionName = TargetLink.GetField (strRowUID, "Name") Then
                 g_strCommandLineBuild = strRowUID
                 Exit For
              End If
              Next
              If len (strConnectionName) > 0 And Len (g_strCommandLineBuild) = 0 Then
                 MessageLog.Warning "[Workspace] Unknown " & _
                                    "target name '" & strConnectionName & _
                                    "' used on command line option /build - " & _
                                    "Tip: surround name with quotes for space-separated names"
                 g_CommandLineClose = false ' so the user can see the error message
              End If
        End If
    End If

    ' Clear the docs window (Show a default doc).
    ClearDocs

    ' Setup the appropriate checks on menu items and button selection.
    SetDirectorsCameraChecks
End Sub

'---------------------------------------------------------------------
' Retrieves the Resource Root Value from the game settings file
Function GetResourceRoot (strFilename)
   GetResourceRoot = ""

   Set XmlDom = CreateObject ("MSXML2.DomDocument")
 
   Dim resFile

   If XmlDom.Load (strFilename) Then
      Set resFile = XmlDom.selectSingleNode ("/OBJECT/OBJECT[@NAME='Game Settings']/PARAM[@NAME='ResourceRoot']/@VALUE")
   
      If Not resFile Is Nothing Then
        g_strResourceRoot = resFile.nodeValue
	GetResourceRoot = g_strResourceRoot
      End If
   End If
End Function

'---------------------------------------------------------------------
' Verify any values in the AppSettings
Sub VerifyAppSettings
    If Tooltip_WaitTime < 10 Then
        Tooltip_WaitTime = 10
    End If

    If Tooltip_DisplayTime < 10 Then
        Tooltip_DisplayTime = 10
    End If

    If AppSettings.RecentFiles_MaxFiles < 1 Then
        AppSettings.RecentFiles_MaxFiles = 1
    End If
End Sub

'---------------------------------------------------------------------
Sub RENManager_OnTerminate (cancel, pause)
    If Not g_CloseImmediately Then
        dim SaveFirst

        If Persist.bModified Then
            SaveFirst = MsgBox ("Do you wish to save your current changes?", vbYesNoCancel + vbQuestion, g_strTitle)
            if SaveFirst = vbCancel then
                cancel = True
                Exit Sub
            End If
        End If

        'if here, then the user does not wish to cancel the termination
        DoExitWorkspaceSettings
        If SaveFirst = vbYes Then
            Persist.SaveProject Persist.SaveChanges
            If Len (Persist.strCurrentFilename) = 0 Then ' User hit cancel
                cancel = True
                Exit Sub
            End If
            Persist.strCurrentFilename = "" 'this prevents SaveGameSettings from happening twice, see below
        End If

        ' Save the application settings file.
        Dim strRetryMessage
        strRetryMessage = "The application settings file is read only: " & vbCR & AppSettings.SettingsFilename & vbCR & _
                          "The file should not be under source control or set as read only." & vbCR & _
                          "Make it write enabled to allow saving, then click Yes." & vbCR & _
                          "Do you wish to try saving the file again?"
        If Persist.CanTryToSaveFile (AppSettings.SettingsFilename, strRetryMessage) Then
            Dim AppProperties
            Set AppProperties  = GetAppProperties
            ' Open xml settings file
            Dim XMLDoc 
            Set XMLDoc = CreateObject ("Msxml2.DOMDocument.3.0")
            XMLDoc.Async = False
            XMLDoc.Load AppSettings.SettingsFilename

            ' All script settings should appear under Object/RWSScriptSettings/ScriptName/Setting,
            ' so we must ensure that there are Object and RWSScriptSettings nodes.
            Dim RootNode
            Set RootNode = XMLDoc.SelectSingleNode ("/OBJECT")
            If RootNode Is Nothing Then
                  Set RootNode = XMLDoc.CreateElement ("OBJECT")
                  XMLDoc.AppendChild RootNode
            End If

            ' Ask modules to save data to xml
            If Not RootNode Is Nothing Then
                  Dim RWSRootNode
                  Set RWSRootNode = RootNode.SelectSingleNode ("./RWSScriptedSettings")
                  If RWSRootNode Is Nothing Then
                     Set RWSRootNode = XMLDoc.CreateElement ("RWSScriptedSettings")
                     RootNode.AppendChild RWSRootNode
                  End If

                  If Not RWSRootNode Is Nothing Then
                     Broadcast.SaveApplicationSettings XMLDoc, RWSRootNode
                  End If
                  
                  ' Save application settings file
                  XMLDoc.Save AppSettings.SettingsFilename
            End If

            ' save application settings file
            AppProperties.Save AppSettings.SettingsFilename
            AppProperties.RemoveObjects
        End If

        ' Always persist out the game settings
        If Len (Persist.strCurrentFilename) > 0 Then
            Persist.SaveGameSettings
        End If

        ' Save Layouts
        RENManager.SaveAllLayouts

    End If

    ' Make sure the sequence asset has been saved.
    SequencerTools.EnsureSequenceAssetSaved Sequencer

    ' Inform other scripts that we're about to close, so save any unsaved data now.
    Broadcast.WorkspaceEnd

    ' Detach from the database
    RWSUtils.CloseGame

    ' The listers needs to shut down at this stage. This allows it to release any plugins.
    ' This is especially important for the asset previews.
    GameDatabase.Shutdown
    BehaviorLister.Shutdown
    AssetLister.Shutdown
    TemplateLister.Shutdown
    AttributeShareLister.Shutdown
    SearchResults.Shutdown

    ' Give RWSComms and RWSUtils chance to clean up
    RWSComms.ShutDown
    RWSUtils.Shutdown

End Sub

'---------------------------------------------------------------------
Sub RENManager_OnUnhandledCall (Object, Name)
   Dim strOutputText
   strOutputText = "The function or variable: " + Name + " is not accessible for " + _
                    "the: " +  Object + ". Ensure that the " + _
                    "latest control exists, is registered and has a corresponding RENObj." 
   MessageLog.Error strOutputText
End Sub

'---------------------------------------------------------------------

Sub DoExitWorkspaceSettings
    WorkspaceSettingsObj.LastProject = Persist.strCurrentFilename
    'N.B. WorkspaceSettingsObj.LastDirectory is set in the Persist.ShowGameFileDialog subroutine

    'save the selection settings
     WorkspaceSettingsObj.SelectionSettingsIndex = GetSelectionSettingsIndex
End Sub

'---------------------------------------------------------------------

Function GetSelectionSettingsIndex

    If DesignView.ModuleMouseFocus is DesignView.Pick3D Then
        GetSelectionSettingsIndex = 1
        Exit Function
    End If

    If DesignView.ModuleMouseFocus is DesignView.Camera Then
        GetSelectionSettingsIndex = 0
        Exit Function
    End If

    If DesignView.ModuleMouseFocus is DesignView.DragCamera Then
        Select Case DesignView.DragCamera.NavigationMode
            Case 0  'Pan
                GetSelectionSettingsIndex = 5
            Case 1  'Zoom
                GetSelectionSettingsIndex = 7
            Case 2  'Orbit
                GetSelectionSettingsIndex = 6
        End Select
        Exit Function
    End If

End Function

'---------------------------------------------------------------------

Sub RestoreSelectionSettings

    Select Case WorkspaceSettingsObj.SelectionSettingsIndex
        Case 0 'fly
            MainMenu.OnMenuFlyCamera
        Case 1 'pick
            DesignView.ModuleMouseFocus = MainMenu.strPickModuleName
            MainMenu.SetSelectionMenuChecks
        Case 5 'Pan
            MainMenu.OnMenuPanCamera

        Case 6 'orbit
            MainMenu.OnMenuOrbitCamera

        Case 7 'zoom
            MainMenu.OnMenuZoomCamera
    End Select
End Sub

'---------------------------------------------------------------------

Function GetAppProperties

   ' create the app properties object to save the settings
   Dim AppProperties
   Set AppProperties = CreateObject("CSL.AppProps.Settings")

   ' add platform settings
   Dim PlatformSettings
   Set PlatformSettings = RWSComms.GetSettings ("Application")
   AppProperties.AddObject "TargetLink", PlatformSettings

   ' add Recent Files list
   Dim RecentFileList
   Set RecentFileList = RWSUtils.GetSettings("RecentFileList")
   AppProperties.AddObject "RecentFileList", RecentFileList

   ' add Asset Types list
   Dim AssetTypeList
   Set AssetTypeList = RWSUtils.GetSettings("AssetTypeList")
   AppProperties.AddObject "AssetTypeList", AssetTypeList

   ' add PlatformFlags list
   Dim PlatformFlagsList
   Set PlatformFlagsList = RWSUtils.GetSettings ("PlatformFlags")
   AppProperties.AddObject "Platform Flags", PlatformFlagsList

   ' add Workspace settings
   Dim WorkspaceSettings
   Set WorkspaceSettings = WorkspaceSettingsObj.GetSettings("RWSWorkspaceSettings")
   AppProperties.AddObject "RWSWorkspaceSettings", WorkspaceSettings

   ' Add the flight settings
   Dim FlightSettings
   Set FlightSettings = DesignView.Camera.GetSettings
   AppProperties.AddObject "Camera", FlightSettings

   ' Add the DesignViewer settings
   Dim DesignViewerSettings
   Set DesignViewerSettings = DesignView.Ortho.GetSettings ("SplitterSettings")
   AppProperties.AddObject "DesignViewSplitter", DesignViewerSettings

   Dim DesignViewDisplaySettings
   Set DesignViewDisplaySettings = DesignView.Ortho.GetSettings ("Display")
   AppProperties.AddObject "DesignViewDisplay", DesignViewDisplaySettings

   Dim DesignViewViewLinkSettings
   Set DesignViewViewLinkSettings = DesignView.Ortho.GetSettings ("ViewLinking")
   AppProperties.AddObject "DesignViewLinking", DesignViewViewLinkSettings

   ' Add the recent source root paths
   Dim MRUSourceRootSettings
   Set MRUSourceRootSettings = RWSUtils.GetSettings ("RecentSourceRoots")
   AppProperties.AddObject "MRUSourceRoots", MRUSourceRootSettings

   ' Add the recent resource root paths
   Dim MRUResourceRootSettings
   Set MRUResourceRootSettings = RWSUtils.GetSettings ("RecentResourceRoots")
   AppProperties.AddObject "MRUResourceRoots", MRUResourceRootSettings

   ' Add the recent RF3 Project Template files
   Dim MRURecentRF3ProjectTemplatesSettings
   Set MRURecentRF3ProjectTemplatesSettings = RWSUtils.GetSettings ("RecentRF3ProjectTemplates")
   AppProperties.AddObject "MRURF3ProjectTemplates", MRURecentRF3ProjectTemplatesSettings

   ' create an attribute editor control name map
   Dim AEControlMap
   Set AEControlMap = CreateObject("CSL.RWSUtils.AEControlMap")
   Dim AESettings
   Set AESettings = AEControlMap.GetSettings("Application")
   AppProperties.AddObject "AttributeEditorControls", AESettings

   Set GetAppProperties = AppProperties
End Function

'---------------------------------------------------------------------
Sub UpdateAppCaption ()
    Dim NewCaption

    If Len (Persist.strCurrentFilename) > 0 Then
        NewCaption = Persist.strCurrentFilename
    Else
        NewCaption = "Untitled"
    End If

    NewCaption = "RenderWare Studio Workspace - [" + NewCaption + "]"
    CSLRenderWareStudioWorkspace.Caption = NewCaption
End Sub

'---------------------------------------------------------------------
Sub ParseSource
    Broadcast.PreParseSource
    RWSUtils.ParseSource

    ' Touch a target file so we know when the source was last parsed
    Dim fso
    Set fso = CreateObject ("Scripting.FileSystemObject") 
    Dim strFolder
    strFolder = Persist.GetPlatformBuildDirectory ("")

    CreateDirectory strFolder, fso ' Create folder if it doesn't exist
    If fso.FolderExists (strFolder) Then 
        Set oTextFile = fso.CreateTextFile (strFolder & "SourceParsed", True, False)
        If Not oTextFile Is Nothing Then
            oTextFile.WriteLine "Source last parsed at " & Now
            oTextFile.Close
        End If
    End If

    Broadcast.PostParseSource
End Sub

'---------------------------------------------------------------------
Sub UpdateUndoUserInterface
    UndoEnabled = Undo.GetItemCount (UndoFlag) > 0
    RedoEnabled = Undo.GetItemCount (RedoFlag) > 0

    MainMenu.EnableMenuItem   30, UndoEnabled ' Undo
    MainMenu.EnableMenuItem   31, RedoEnabled ' Redo
    MainToolbar.EnableMenuItem 3, UndoEnabled ' Undo
    MainToolbar.EnableMenuItem 4, RedoEnabled ' Redo

    If UndoEnabled OR RedoEnabled Then 
        ' Enable the options 
        MainMenu.EnableMenuItem    348, True ' Undo stack 
        MainToolbar.EnableMenuItem  13, True ' Undo stack 
    Else 
        ' Disable the options 
        MainMenu.EnableMenuItem    348, False ' Undo stack 
        MainToolbar.EnableMenuItem  13, False ' Undo stack 
    End If
End Sub

'---------------------------------------------------------------------
Sub Undo_OnUndo
    UpdateUndoUserInterface

    ' An undone operation will now be item zero on the redo stack
    MessageLog.Log "[Undone] " + Undo.GetItemDescription (0, RedoFlag)
End Sub

'---------------------------------------------------------------------
Sub Undo_OnRedo
    UpdateUndoUserInterface

    ' An redone operation will now be item zero on the undo stack
    MessageLog.Log "[Redone] " + Undo.GetItemDescription (0, UndoFlag)
End Sub

'---------------------------------------------------------------------
Sub Undo_OnAdd
    UpdateUndoUserInterface
End Sub

'---------------------------------------------------------------------
Sub Undo_OnClear
    UpdateUndoUserInterface
End Sub

'---------------------------------------------------------------------
Sub ShowAllEntities (bShowEntities)
    RWSUtils.ShowEntities bShowEntities
End Sub

'---------------------------------------------------------------------
Sub Import
    ' Get filename
    strProjectFilename = Persist.ShowGameFileDialog (Persist.LoadDialog)

    If Len (strProjectFilename) > 0 Then

        ' Allow the user to select which items they want to import.
        DlgImport.Initialize strProjectFilename
        RENManager.ShowModal "DlgImport", "CSLRenderwareStudioWorkspace"

        UpdateUndoUserInterface
        Persist.Modified
    End If
End Sub

' ---------------------------------------------------------------------  
Sub SetDirectorsCameraChecks
    MainMenu.CheckMenuItem     418, gbDirectorsCameraEnabled          ' Director's Camera Menu item  
    CurrentTargetToolbar.CheckMenuItem 11,   gbDirectorsCameraEnabled ' Director's Camera Button  
End Sub  
  
' --------------------------------------------------------------------- 
Sub UpdateDirectorsCamera
    Dim Make  
    For Each Connection In RWSComms.Connections  
        Set Make = Connection.GetParam (strMakeAssociation)  
        Make.AddTask "DirectorsCamera", Connection, gbDirectorsCameraEnabled  
        Make.Build  
    Next  
End Sub 
 
'------------------------------------------------------------------------------
Sub ToggleDirectorsCamera
    gbDirectorsCameraEnabled = Not gbDirectorsCameraEnabled
    UpdateDirectorsCamera
End Sub

'------------------------------------------------------------------------------
Sub TogglePauseMode ()
    gbPauseGameEnabled = Not gbPauseGameEnabled
    UpdatePauseMode
End Sub

'------------------------------------------------------------------------------
Sub UpdatePauseMode ()
    Dim Make
    For Each Connection In RWSComms.Connections
        Set Make = Connection.GetParam (strMakeAssociation)
        Make.AddTask "PauseGame", Connection, gbPauseGameEnabled
        Make.Build
    Next
End Sub

'----------------------------------------------------------------------
Sub SetPauseModeChecks ()
    MainMenu.CheckMenuItem     419, gbPauseGameEnabled  ' Pause Mode Menu item
    CurrentTargetToolbar.CheckMenuItem 10,   gbPauseGameEnabled  ' Pause Mode button
End Sub

'------------------------------------------------------------------------------
Sub SwitchFolders (PreviousFolderID)
    ' Boolean flag used so we don't send camera updates causing re-entrant build errors
    g_bSwitchingFolders = True

    For Each Connection In RWSComms.Connections
        strPlatformFlags   = RWSComms.GetPlatformFlags (Connection.Platform)
        Set PlatformFilter = RWSComms.CreatePlatformFilter (strPlatformFlags)

        Set Make = Connection.GetParam (strMakeAssociation)
        Make.AddTask "ChangeActiveFolder", Connection, RWSScript.Game, PlatformFilter, PreviousFolderID
        Make.Build
    Next

    g_bSwitchingFolders = False
End Sub

'------------------------------------------------------------------------------
Sub ResetGame (bSendAssets)
    For Each Connection In RWSComms.Connections
        ResetGameForConnection Connection, bSendAssets
    Next
End Sub

'------------------------------------------------------------------------------
Sub ResetGameForConnection (Connection, bSendAssets)
    Dim Make
    If Not Connection Is Nothing Then
        strPlatformFlags   = RWSComms.GetPlatformFlags (Connection.Platform)
        Set PlatformFilter = RWSComms.CreatePlatformFilter (strPlatformFlags)

        Set Make = Connection.GetParam (strMakeAssociation)
        Make.AddTask "ResetGame", Connection, RWSScript.Game, PlatformFilter, bSendAssets
        Make.Build
    End If
End Sub

'------------------------------------------------------------------------------
' Reset all of the entities in the given object. Only Entity and Folder
' types are valid, others will be ignored.

Sub ResetEntitiesForConnection (ObjectID, Connection)
    Dim Make
    Dim PlatformFilter
    Dim Buffer
    Dim RWSObject
   Set Make = Connection.GetParam (strMakeAssociation)
   Set PlatformFilter = RWSComms.CreatePlatformFilter (RWSComms.GetPlatformFlags (Connection.Platform))
   Set Buffer = RWSComms.CreateMemoryBuffer()
   Buffer.BigEndian = Connection.BigEndian
   Buffer.Unicode = Connection.Unicode
   Select Case RWSScript.IDType (ObjectID)
       Case otEntity
           Set RWSObject = RWSScript.Entity(ObjectID)
           Make.AddTask "EntityReset", RWSObject, PlatformFilter, Buffer
           Make.Build

           MessageLog.Log "[" & Connection.Platform & "] Resetting entity : " & RWSObject.Name

       Case otFolder
           Set RWSObject = RWSScript.Folder (ObjectID)
           Make.AddTask "FolderResetEntities", RWSObject, PlatformFilter, Buffer
           Make.Build

           MessageLog.Log "[" & Connection.Platform & "] Resetting entities in folder : " & RWSObject.Name

   End Select

   Connection.SendBuffer Buffer
End Sub

'------------------------------------------------------------------------------

Sub ResetEntities (ObjectID)
    Dim Make
    Dim PlatformFilter
    Dim Buffer
    Dim RWSObject

    For Each Connection In RWSComms.Connections
        If Not Connection Is Nothing Then
            ResetEntitiesForConnection ObjectID, Connection
        End If
    Next
End Sub

'------------------------------------------------------------------------------
'
Function ObjectFromId (APIObjectID)

   Select Case RWSScript.IDType(APIObjectID) 
      case otGame
         Set ObjectFromId = RWSScript.Game

      case otEntity         
         Set ObjectFromId = RWSScript.Entity(APIObjectID)

      case otFolder
         Set ObjectFromId = RWSScript.Folder(APIObjectID)

      case otAsset
         Set ObjectFromId = RWSScript.Asset(APIObjectID)

      case otAssetFolder
         Set ObjectFromId = RWSScript.AssetFolder(APIObjectID)

      case otTemplate
         Set ObjectFromId = RWSScript.Template(APIObjectID)

      case otTemplateFolder
         Set ObjectFromId = RWSScript.TemplateFolder(APIObjectID)

      case otBehavior
         Set ObjectFromId = RWSScript.Behavior(APIObjectID)

      case otBehaviorFolder
         Set ObjectFromId = RWSScript.BehaviorFolder(APIObjectID)

      case otCommand
         Set ObjectFromId = RWSScript.Command(APIObjectID)

      case otAttribute
         Set ObjectFromId = RWSScript.Attribute(APIObjectID)

      case otAttributeShare
         Set ObjectFromId = RWSScript.AttributeShare(APIObjectID)

      case otAttributeShareFolder
         Set ObjectFromId = RWSScript.AttributeShareFolder(APIObjectID)

      case otEvent
         Set ObjectFromId = RWSScript.Event(APIObjectID)

      case else
         Set ObjectFromId = nothing

   End Select

End Function

'------------------------------------------------------------------------------
'
Function QueryDeleteObject (APIObject)
   Dim ParentDependencies, ChildDependencies
 
   Set ParentDependencies = RWSScript.CreateContainer ()
   Set ChildDependencies = RWSScript.CreateContainer ()
 
   RWSScript.GetDependencies APIObject, ParentDependencies, ChildDependencies 
 
   If (ParentDependencies.Count) > 1 Then
      Dim Response
      
      Response = MsgBox ("Warning " & ContextMenu.GetObjectName(APIObject, True) & " is referenced by " & (ParentDependencies.Count-1) & " object(s), are you sure you want to delete it?", vbYesNo, g_strTitle)

      If Response = vbYes Then
         QueryDeleteObject = True
      Else
         QueryDeleteObject = False
      End If
   Else
      QueryDeleteObject = True 
   End If
 
End Function
 
' ------------------------------------------------------------------------------  
'   
Sub ViewFileCodeWarrior (FileName, LineBegin, LineEnd)  
   Dim CodeWarrior, CWTextDocument     
   Set CodeWarrior = Nothing
   On Error Resume Next
   Set CodeWarrior = CreateObject ("CodeWarrior.CodeWarriorApp")    
   On Error Goto 0
   If Not CodeWarrior Is Nothing Then
      Set CWTextDocument = CodeWarrior.OpenTextDocument (FileName, False)  
      If Not CWTextDocument Is Nothing Then  
         CWTextDocument.TextEngine.SelectionLineStart = LineBegin - 1 
         CWTextDocument.TextEngine.SelectionLineEnd = LineEnd  
         CWTextDocument.ScrollToSelection  

         ' Attempt to bring the window into the foreground  
         Dim WshShell  
         Set WshShell = CreateObject ("WScript.Shell")  
         WshShell.AppActivate "Metrowerks CodeWarrior"  
      End If  
   Else
      MessageLog.Error "Unable to open CodeWarrior" 
   End If
End Sub

' ------------------------------------------------------------------------------  
'   
Sub ViewFileVC6 (FileName, LineBegin, LineEnd)  
   Dim MSDev, VCTextDocument 
   Set MSDev = Nothing
   On Error Resume Next 
   Set MSDev = GetObject (, "msdev.application") 
   If Err = 0 Then 
      MSDev.Documents.Open FileName    
   Else 
      Set MSDev = CreateObject ("msdev.application") 
      If Not MSDev Is Nothing Then
         MSDev.Documents.Open FileName
      End If
   End If 
   On Error Goto 0 
   If Not MSDev Is Nothing Then 
      Set VCTextDocument = MSDev.Documents(FileName) 
      If Not VCTextDocument Is Nothing Then 
         VCTextDocument.Selection.MoveTo LineBegin, 1 
         VCTextDocument.Selection.MoveTo LineEnd + 1, 1, dsExtend 
      End If 
      MSDev.Visible = True 
      MSDev.Active = True 
   Else 
      MessageLog.Error "Unable to open Visual C++ 6" 
   End If 
End Sub

' ------------------------------------------------------------------------------  
'   
Sub ViewFileVisualStudioNet (FileName, LineBegin, LineEnd)  
   Set g_VSDTE = Nothing
   On Error Resume Next  
   Set g_VSDTE = GetObject (,"VisualStudio.DTE")  
   If Err > 0 Then  
      Set g_VSDTE = CreateObject ("VisualStudio.DTE")  
   End If  
   On Error Goto 0 
   If Not g_VSDTE Is Nothing Then  
      g_VSDTE.ExecuteCommand "File.OpenFile", """" & FileName & """"
      g_VSDTE.MainWindow.Visible = True  

      Dim DocNum, Found
      DocNum = 1
      Found = False
      While DocNum <= g_VSDTE.Documents.Count And Not Found
         If g_VSDTE.Documents.Item(DocNum).FullName = FileName Then
            Found = True
            g_VSDTE.Documents.Item(DocNum).Selection.GoToLine LineBegin
            g_VSDTE.Documents.Item(DocNum).Selection.MoveToLineAndOffset LineEnd + 1, 1, True
         End If
         DocNum = DocNum + 1
      Wend
   Else  
      MessageLog.Error "Unable to open Visual Studio .NET"  
   End If  
End Sub

' ------------------------------------------------------------------------------  
' Views the given file and selects the given block
Sub ViewFile (FileName, LineBegin, LineEnd)  
   If Len (FileName) Then  
      Select Case WorkspaceSettingsObj.SourceViewer
         Case "CodeWarrior"
            ViewFileCodeWarrior FileName, LineBegin, LineEnd
         Case "Visual C++ 6"
            ViewFileVC6 FileName, LineBegin, LineEnd
         Case "Visual Studio .NET"
            ViewFileVisualStudioNet FileName, LineBegin, LineEnd
         Case Else
            'No application set - use associated viewer 
            Dim Shell   
            Set Shell = CreateObject ("Shell.Application")   
            Shell.ShellExecute FileName
      End Select
   End If  
End Sub  
  
' ------------------------------------------------------------------------------  
' Views the source code for the given object  
Sub ViewSource (Object)  
   If Object.IDType = otBehavior Then  
      ViewFile Object.FileName, Object.LineNumber, Object.LineNumber 
   ElseIf Object.IDType = otCommand Then  
      ViewFile Object.FileName, Object.LineBegin, Object.LineEnd
   ElseIf Object.IDType = otAttribute Then  
      Dim Command 
      Set Command = Object.Command 
      If Not Command Is Nothing Then 
         ViewFile Command.FileName, Command.LineBegin, Command.LineEnd
      End If 
   End If  
End Sub  
 
' ------------------------------------------------------------------------------   
' Views the source code for the object with the given ID
Sub ViewObjectSource (ObjectID) 
    Dim IDType 
    IDType = RWSScript.IDType (ObjectID) 
    
    Dim Object
    If IDType = otCommand Then 
       Set Object = RWSScript.Command (ObjectID)  
       ViewSource Object
    End If 
End Sub

' ------------------------------------------------------------------------------
