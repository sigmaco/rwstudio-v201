' Criterion Software ©2001
' Enterprise Layout Management Script
'
' MainMenu

' ---------------------------------------------------------------------
' Script objects
Dim Shell32Object, WshShell, Selection
Set Shell32Object   = CreateObject("Shell.Application.1")
Set WshShell        = CreateObject("WScript.Shell.1")
Set GlobalSelection = CreateObject("CSL.RWSSelection.Selection")

RENHost.ConnectObject RENManager, "RENManagertoMenu"

strPickModuleName    = "3D Object Pick Tool"
strFlightModuleName  = "Camera Controller"
strCamDragModuleName = "Drag Camera Controller"

dim strCurrentNavigationModule
strCurrentNavigationModule = ""

dim strLastNonFullViewLayoutSelected
strLastNonFullViewLayoutSelected = ""

' Orbiting types.

Const ORBIT_TYPE_ORBIT_ONLY = 0
Const ORBIT_TYPE_ORBIT_OBJECT = 1
Const ORBIT_TYPE_CAMERA_ARC = 2

' ---------------------------------------------------------------------
' Initialization dependant on other objects being loaded.
Sub OnLoad()
    GlobalSelection.SelectionIdentifier = GlobalScript.g_strGlobalSelection
    SetSelectionMenuChecks
End Sub

'**********************************************************************
'                           File Menu
'**********************************************************************

' ---------------------------------------------------------------------
Sub OnMenuNewProject()
    ' New Game
    Broadcast.PreNewProject

    ' Ensure that the current sequence is saved
    SequencerTools.EnsureSequenceAssetSaved Sequencer

    ' Save any settings for the current project
    If Len(Persist.strCurrentFilename) > 0 Then
       Persist.SaveGameSettings
    End If

    ' Ask if user wants to save first
    bSaveFirst = vbNo
    If Persist.bModified Then
       bSaveFirst = MsgBox ("Do you wish to save your current changes?", vbYesNoCancel + vbQuestion, GlobalScript.g_strTitle)
    End If

    If bSaveFirst = vbCancel Then
       Exit Sub
    End If

    If bSaveFirst = vbYes Then
        Persist.SaveProject Persist.SaveChanges
        If Len (Persist.strCurrentFilename) = 0 Then ' User hit cancel
            Exit Sub
        End If
    End If

    ' Clear any values that are required for a new game.
    Persist.strCurrentFilename               = ""
    GlobalScript.g_strResourceRoot           = ""
    GlobalScript.g_strSourceRoot             = ""
    GlobalScript.g_strRF3ProjectTemplateFile = ""

    GlobalScript.ClearDocs 

    ' Now do the new game call
    GlobalScript.RWSUtils.NewGame

    ' Update the app's caption
    GlobalScript.UpdateAppCaption

    ' Update the menus
    GlobalScript.SetMenuOptionsOnStartGame True
    GlobalScript.UpdateVersionControlMenu

    Dim Container, Selection 
    Set Container = CreateObject ("CSL.RWSScript.RWSContainer") 
    Container.Add GlobalScript.RWSScript.Game.ID
    GlobalScript.RWSUtils.ShowProperties Container, True
    Persist.Modified

    Broadcast.PostNewProject
End Sub

' ---------------------------------------------------------------------
Sub OnMenuOpenProject()
    SequencerTools.EnsureSequenceAssetSaved Sequencer
    Persist.LoadProject ""
End Sub

' ---------------------------------------------------------------------
Sub OnMenuSaveProject
    SequencerTools.EnsureSequenceAssetSaved Sequencer
    Persist.SaveProject Persist.SaveChanges
End Sub

' ---------------------------------------------------------------------
Sub OnMenuSaveProjectAs
    SequencerTools.EnsureSequenceAssetSaved Sequencer
    Persist.SaveProject Persist.SaveAll
End Sub

' ---------------------------------------------------------------------
Sub OnMenuImport
    GlobalScript.Import
End Sub

' ---------------------------------------------------------------------
Sub MainMenu_Click (index)
    '**Recommendation**:  do not use indexes to write menu option handler
    'Instead, define an *event* in the menu control property page

    If index > globalscript.RecentFileMarker Then ' One of the recent games
       Dim arrRecentFiles, strProjectToLoad

       arrRecentFiles   = GlobalScript.RWSUtils.GetRecentFileList
       strProjectToLoad = arrRecentFiles (index - GlobalScript.RecentFileMarker - 1)

       If Len (strProjectToLoad) > 0 Then
           Persist.LoadProject strProjectToLoad
       End If
    End If
End Sub

' ---------------------------------------------------------------------
Sub OnMenuImportToAlienBrain
   ' User must save any changes first
   If Persist.bModified Then
      If vbOK = MsgBox ("All changes must be saved before import. Press OK to save current changes.", _
                        vbOKCancel + vbQuestion, GlobalScript.g_strTitle) Then
         Persist.SaveProject Persist.SaveChanges
      Else
         Exit Sub
      End If
   End If

   VersionControl.ImportProjectToVersionControl ()
End Sub

' ---------------------------------------------------------------------
' Submits the changes in the currently-selected default change set to
' version control.
Sub OnMenuAlienBCheckin
   ' Ask if user wants to save first
   bSaveFirst = vbNo
   If Persist.bModified Then
      bSaveFirst = MsgBox ("Do you wish to save your current changes?", vbYesNoCancel + vbQuestion, GlobalScript.g_strTitle)
   End If

   If Not bSaveFirst = vbCancel Then ' If user canceled then we don't do anything
      If bSaveFirst = vbYes Then
         Persist.SaveProject Persist.SaveChanges

         'Save any asset files before checkin
         RWSSpline.Save
      End If

      ' Check in the default change set.
      VersionControl.SubmitDefaultChangeSet
      GlobalScript.UpdateUndoUserInterface
   End If
End Sub

' ---------------------------------------------------------------------
' Undo's the changes in the currently-selected default change set in
' version control.
Sub OnMenuAlienBUndoCheckOut
   ' Ask if user wants to save first
   bSaveFirst = vbNo
   If Persist.bModified Then
      bSaveFirst = MsgBox ("Do you wish to save your current changes?", vbYesNoCancel + vbQuestion, GlobalScript.g_strTitle)
   End If

   If Not bSaveFirst = vbCancel Then ' If user canceled then we don't do anything
      If bSaveFirst = vbYes Then
         Persist.SaveProject Persist.SaveChanges
      End If

      ' Undo the default change set.
      VersionControl.UndoDefaultChangeSet
      GlobalScript.UpdateUndoUserInterface
   End If
End Sub

' ---------------------------------------------------------------------
Sub OnMenuAlienBGetLatest
   ' Ask if user wants to save first
   bSaveFirst = vbNo
   If Persist.bModified Then
       bSaveFirst = MsgBox ("Do you wish to save your current changes?", vbYesNoCancel + vbQuestion, GlobalScript.g_strTitle)
   End If

   If Not bSaveFirst = vbCancel Then ' If user canceled then we don't do anything
       If bSaveFirst = vbYes Then
           Persist.SaveProject Persist.SaveChanges
       End If

       VersionControl.GetLatestVersion ()
   End If
End Sub

' ---------------------------------------------------------------------
Sub OnMenuAlienBShowProjectInExplorer ()
   VersionControl.ShowProjectInExplorer ()
End Sub

' ---------------------------------------------------------------------
Sub OnMenuAlienBRefresh
   VersionControl.RefreshAllObjectsManagedState
End Sub

' ---------------------------------------------------------------------
Sub OnMenuExit
    ' Exit
    RENManager.KillLoop
End Sub

'**********************************************************************
'                           Edit Menu
'**********************************************************************

' ---------------------------------------------------------------------
Sub OnMenuUndo
   ' Undo
   if GlobalScript.Undo.GetItemCount (GlobalScript.UndoFlag) then
       GlobalScript.Undo.Undo
   else
       GlobalScript.UpdateUndoUserInterface
       MessageLog.Log "[Undo] No items to undo"
   end if
End Sub

' ---------------------------------------------------------------------
Sub OnMenuRedo
   ' Redo
   if GlobalScript.Undo.GetItemCount (GlobalScript.RedoFlag) then
    GlobalScript.Undo.Redo
   else
       GlobalScript.UpdateUndoUserInterface
       MessageLog.Log "[Workspace] No items to redo"
   end if
End Sub

' ---------------------------------------------------------------------
Sub OnMenuShowUndoStack
    UndoStack.Initialize
    RENManager.ShowModal "UndoStack", "CSLRenderwareStudioWorkspace"
End Sub

' ---------------------------------------------------------------------
Sub OnMenuSearchGame
    RENManager.ShowModal "Search", "CSLRenderwareStudioWorkspace"
End Sub

'**********************************************************************
'                           View Menu
'**********************************************************************

' ---------------------------------------------------------------------
Sub OnCloneCurrentLayout ()
   Layouts.CloneCurrentLayout ()
End Sub

' ---------------------------------------------------------------------
Sub OnRenameCurrentLayout ()
   Layouts.RenameCurrentLayout ()
End Sub

' ---------------------------------------------------------------------
Sub OnDeleteCurrentLayout ()
   Layouts.DeleteCurrentLayout ()
End Sub

' --------------------------------------------------------------------- 
Sub OnRestoreCurrentLayout ()
   Layouts.RestoreCurrentLayout ()
End Sub

' --------------------------------------------------------------------- 
Sub OnRestoreToDefaultLayouts ()
   Layouts.RestoreToDefaultLayouts ()
End Sub

' ---------------------------------------------------------------------
Sub OnMenuRefreshEventView
    ' Refresh event view
    EventVisualizer.Refresh
End Sub

Sub OnMenuShowAllEntities
    GlobalScript.ShowAllEntities True
End Sub

Sub OnMenuHideAllEntitites
    GlobalScript.ShowAllEntities False
End Sub

'**********************************************************************
'                           Selection Menu
'**********************************************************************

'
' Separate version of the camera control function have been included (NoPreview versions) because of a
' bug which is caused for unknown reasons. Because the Preview window is an out of process server you
' get an exception when setting the flight modes from the design views if they make calls to the RwsPreview object.
'
' Therefore 2 versions of the functions have been created,  one which gets called from the design view's middle mouse button
' functionality, and one which gets called from the toolbar.
'
' ---------------------------------------------------------------------
Sub OnMenuFlyCamera
   ' Flight Mode
   DesignView.ModuleMouseFocus = strFlightModuleName
   strCurrentNavigationModule = strFlightModuleName
   SetSelectionMenuChecks
   VolumeToolbar.ClearMenuChecks
End Sub

' ---------------------------------------------------------------------
Sub OnMenuOrbitCamera

    OnMenuOrbitCameraNoPreview
    RwsPreview.SetOrbit
    VolumeToolbar.ClearMenuChecks
End Sub

' ---------------------------------------------------------------------
Sub OnMenuOrbitCameraNoPreview
    ' Max navigation
    DesignView.ModuleMouseFocus = strCamDragModuleName
    DesignView.DragCamera.NavigationMode = 2
    SetSelectionMenuChecks
    VolumeToolbar.ClearMenuChecks
End Sub

' ---------------------------------------------------------------------
Sub OnMenuPanCamera
    OnMenuPanCameraNoPreview
    RwsPreview.SetPan
    VolumeToolbar.ClearMenuChecks
End Sub

' ---------------------------------------------------------------------
Sub OnMenuPanCameraNoPreview
    ' Max navigation
    DesignView.ModuleMouseFocus = strCamDragModuleName
    DesignView.DragCamera.NavigationMode = 0
    SetSelectionMenuChecks
    VolumeToolbar.ClearMenuChecks
End Sub

' ---------------------------------------------------------------------
Sub OnMenuZoomCamera
    OnMenuZoomCameraNoPreview
    RwsPreview.SetZoom
    VolumeToolbar.ClearMenuChecks
End Sub

' ---------------------------------------------------------------------
Sub OnMenuZoomCameraNoPreview
    ' Max navigation
    DesignView.ModuleMouseFocus = strCamDragModuleName
    DesignView.DragCamera.NavigationMode = 1
    SetSelectionMenuChecks
    VolumeToolbar.ClearMenuChecks
End Sub

' ---------------------------------------------------------------------
Sub OnMenuPickMode

    ' Pick only mode
    if DesignView.RWSUtility.HighlightMaterials then
       DesignView.RWSUtility.HighlightMaterials = False
       DesignView.Object3D.SelectDisplayMode = 0
    end if

    DesignView.ModuleMouseFocus = strPickModuleName
    DesignView.Object3D.AxisMode = 3
    SetSelectionMenuChecks
    ' In pick only mode, use object information as if in translate mode.
    ObjectInformation.SetCurrentMode 0
    VolumeToolbar.ClearMenuChecks
End Sub

' ---------------------------------------------------------------------
Sub OnMenuPickTranslateMode

    ' Pick and translate mode
    if DesignView.RWSUtility.HighlightMaterials then
       DesignView.RWSUtility.HighlightMaterials = False
       DesignView.Object3D.SelectDisplayMode = 0
    end if

    DesignView.ModuleMouseFocus = strPickModuleName
    DesignView.Object3D.AxisMode = 0
    SetSelectionMenuChecks
    ObjectInformation.SetCurrentMode 0
    VolumeToolbar.ClearMenuChecks
End Sub

' ---------------------------------------------------------------------
Sub OnMenuPickRotateMode

    ' Pick and translate mode
    if DesignView.RWSUtility.HighlightMaterials then
       DesignView.RWSUtility.HighlightMaterials = False
       DesignView.Object3D.SelectDisplayMode = 0
    end if
    
    DesignView.ModuleMouseFocus = strPickModuleName
    DesignView.Object3D.AxisMode = 1
    SetSelectionMenuChecks
    ObjectInformation.SetCurrentMode 1
    VolumeToolbar.ClearMenuChecks
End Sub

' ---------------------------------------------------------------------
Sub OnMenuPickScaleMode

    ' Pick and translate mode
    if DesignView.RWSUtility.HighlightMaterials then
       DesignView.RWSUtility.HighlightMaterials = False
       DesignView.Object3D.SelectDisplayMode = 0
    end if

    DesignView.ModuleMouseFocus = strPickModuleName
    DesignView.Object3D.AxisMode = 2
    SetSelectionMenuChecks
    ObjectInformation.SetCurrentMode 2
    VolumeToolbar.ClearMenuChecks
End Sub

' ---------------------------------------------------------------------
Sub OnMenuPickAndDrag

    ' Pick and drag mode
    if DesignView.RWSUtility.HighlightMaterials then 
       DesignView.RWSUtility.HighlightMaterials = False 
       DesignView.Object3D.SelectDisplayMode = 0 
    end if 

    DesignView.ModuleMouseFocus = strPickModuleName
    DesignView.Object3D.AxisMode = 4
    SetSelectionMenuChecks
    ObjectInformation.SetCurrentMode 0
    VolumeToolbar.ClearMenuChecks
End Sub

' ---------------------------------------------------------------------
Sub OnMenuPickMaterials

    DesignView.ModuleMouseFocus = strPickModuleName

    if DesignView.RWSUtility.HighlightMaterials then

       DesignView.RWSUtility.HighlightMaterials = False
       DesignView.Object3D.SelectDisplayMode = 0
    
    else

       DesignView.RWSUtility.HighlightMaterials = True 
       DesignView.Object3D.AxisMode = 3
       DesignView.Object3D.SelectDisplayMode = 1 
   
    end if

    SetSelectionMenuChecks
    ObjectInformation.SetCurrentMode 0
    VolumeToolbar.ClearMenuChecks

End Sub

' ---------------------------------------------------------------------
Sub OnMenuLockSelection
    ' Lock / unlock selection - toggle the value.

    If (DesignView.Object3D.SelectionLockEnabled = False) Then
        DesignView.Object3D.SelectionLockEnabled = True
    Else
        DesignView.Object3D.SelectionLockEnabled = False
    End If
    SetSelectionMenuChecks
End Sub

' ---------------------------------------------------------------------
Sub OnMenuOrbitOnly
    ' Change orbit type and reflect change in menus.
    DesignView.DragCamera.OrbitType = ORBIT_TYPE_ORBIT_ONLY
    SetSelectionMenuChecks
    VolumeToolbar.ClearMenuChecks
End Sub

' ---------------------------------------------------------------------
Sub OnMenuOrbitObj
    ' Change orbit type and reflect change in menus.
    DesignView.DragCamera.OrbitType = ORBIT_TYPE_ORBIT_OBJECT
    SetSelectionMenuChecks
    VolumeToolbar.ClearMenuChecks
End Sub

' ---------------------------------------------------------------------
Sub OnMenuCamArc
    ' Change orbit type and reflect change in menus.
    DesignView.DragCamera.OrbitType = ORBIT_TYPE_CAMERA_ARC
    SetSelectionMenuChecks
    VolumeToolbar.ClearMenuChecks
End Sub

' ---------------------------------------------------------------------
' OnMenuNextCameraController
'
' Move the camera or pick mode on to the next one
'
Sub OnMenuNextMode

    If DesignView.ModuleMouseFocus is DesignView.Pick3D Then

        'in pick mode, move onto the next
        Select Case DesignView.Object3D.AxisMode

            case 3 ' Pick only - change to translate - first icon on list
                OnMenuPickTranslateMode
                Exit Sub

            case 0 ' Translate - change to rotate
                OnMenuPickRotateMode
                Exit Sub

            case 1 ' Rotate - change to scale
                OnMenuPickScaleMode
                Exit Sub

            case 2 ' Scale - change to drag
                OnMenuPickAndDrag
                Exit Sub

            case 4 ' Drag - change to pick only.
                OnMenuPickMode
                Exit Sub

        End Select
        Exit Sub
    End If

    If DesignView.ModuleMouseFocus is DesignView.Camera Then
        'camera to orbit
        OnMenuOrbitCamera
        Exit Sub
    End If

    If DesignView.ModuleMouseFocus is DesignView.DragCamera Then
        Select Case DesignView.DragCamera.NavigationMode
            Case 0  'Pan  - change to zoom
                OnMenuZoomCamera
                Exit Sub
            Case 1  'Zoom - change to fly
                OnMenuFlyCamera
                Exit Sub
            Case 2  'Orbit  - change to pan
                OnMenuPanCamera
                Exit Sub
        End Select
        Exit Sub
    End If

End Sub

' ---------------------------------------------------------------------
' OnMenuPrevMode
'
' Move the camera or pick mode on to the previous one
'
Sub OnMenuPrevMode

    If DesignView.ModuleMouseFocus is DesignView.Pick3D Then

        'in pick mode, move onto the next
        Select Case DesignView.Object3D.AxisMode

            case 3 ' Pick only - change to scale. First button on toolbar.
                OnMenuPickAndDrag
                Exit Sub

            case 0 ' Translate - change to pick only.
                OnMenuPickMode
                Exit Sub

            case 1 ' Rotate - change to translate
                OnMenuPickTranslateMode
                Exit Sub

            case 2 ' Scale - change to rotate
                OnMenuPickRotateMode
                Exit Sub

            case 4 ' Drag - change to scale
                OnMenuPickScaleMode
                Exit Sub

        End Select
        Exit Sub
    End If

    If DesignView.ModuleMouseFocus is DesignView.Camera Then
        'camera to zoomt
        OnMenuZoomCamera
        Exit Sub
    End If

    If DesignView.ModuleMouseFocus is DesignView.DragCamera Then
        Select Case DesignView.DragCamera.NavigationMode
            Case 0  'Pan  - change to zoom
                OnMenuOrbitCamera
                Exit Sub
            Case 1  'Zoom - change to pan
                OnMenuPanCamera
                Exit Sub
            Case 2  'Orbit  - change to fly
                OnMenuFlyCamera
                Exit Sub
        End Select
        Exit Sub
    End If

End Sub


' ---------------------------------------------------------------------
Sub OnMenuToggle
   ' Pick mode on/off
   TogglePickCamera
   SetSelectionMenuChecks
End Sub

' ---------------------------------------------------------------------

Sub OnMenuLocateEntity
   ' Locate
   If GlobalSelection.Count > 0 Then
      DesignView.RWSUtility.LocateEntity GlobalSelection.Item (GlobalSelection.Count)
   End If
End Sub

' ---------------------------------------------------------------------

Sub OnMenuAimAtEntity
   ' Locate
   If GlobalSelection.Count > 0 Then
      DesignView.RWSUtility.AimAtEntity GlobalSelection.Item (GlobalSelection.Count)
   End If
End Sub

' ---------------------------------------------------------------------
Sub TogglePickCamera()
    if IsObject(DesignView.ModuleMouseFocus) = False Then
        DesignView.ModuleMouseFocus = strPickModuleName
        Exit Sub
    end if

    If DesignView.ModuleMouseFocus is DesignView.Pick3D Then

        ' If the current navigation mode is not sent, then set up
        ' a default value
        If strCurrentNavigationModule = "" then
          strCurrentNavigationModule = strCamDragModuleName
        end if

        DesignView.ModuleMouseFocus = strCurrentNavigationModule
    Else

        if nCurrentNavigationModule = "" then

            ' Need to set up the name of the current navigation mode
            if DesignView.ModuleMouseFocus is DesignView.Camera then
                strCurrentNavigationModule = strFlightModuleName
            else
                strCurrentNavigationModule = strCamDragModuleName
            end if

        End if

        DesignView.ModuleMouseFocus = strPickModuleName
    End If
End Sub

' ---------------------------------------------------------------------
Sub SetSelectionMenuChecks()

    'uncheck everything on MainMenu
    MainMenu.CheckMenuItem 359, False 'orbit only
    MainMenu.CheckMenuItem 360, False 'orbit object
    MainMenu.CheckMenuItem 361, False 'camera arc
    MainMenu.CheckMenuItem 358, False 'lock selection
    MainMenu.CheckMenuItem 328, False 'pick / translate
    MainMenu.CheckMenuItem 349, False 'pick / rotate
    MainMenu.CheckMenuItem 350, False 'pick / scale
    MainMenu.CheckMenuItem 363, False 'pick / drag
    MainMenu.CheckMenuItem 367, False 'pick / materials    
    MainMenu.CheckMenuItem 357, False 'pick only
    MainMenu.CheckMenuItem 330, False 'orbit
    MainMenu.CheckMenuItem 331, False 'pan
    MainMenu.CheckMenuItem 332, False 'zoom
    MainMenu.CheckMenuItem 329, False 'fly

    'Uncheck everything on the Toolbars
    SelectionToolbar.CheckMenuItem 1, False 'Fly
    SelectionToolbar.CheckMenuItem 2, False 'Orbit
    SelectionToolbar.CheckMenuItem 3, False 'Pan
    SelectionToolbar.CheckMenuItem 4, False 'Zoom
    SelectionToolbar.CheckMenuItem 9, False 'Pick only
    SelectionToolbar.CheckMenuItem 5, False 'Pick/translate
    SelectionToolbar.CheckMenuItem 6, False 'Pick/rotate
    SelectionToolbar.CheckMenuItem 7, False 'Pick/scale
    SelectionToolbar.CheckMenuItem 13, False 'Pick/drag
    SelectionToolbar.CheckMenuItem 8, False 'Lock selection
    SelectionToolbar.CheckMenuItem 10, False 'Orbit only
    SelectionToolbar.CheckMenuItem 11, False 'Orbit objects
    SelectionToolbar.CheckMenuItem 12, False 'Camera arc
    SelectionToolbar.CheckMenuItem 14, False 'Pick materials

    ' Lock selection ticking...

    If DesignView.Object3D.SelectionLockEnabled = True Then
        MainMenu.CheckMenuItem 358, True
        SelectionToolbar.CheckMenuItem 8, True
    End If

    ' Orbiting type.

    Select Case DesignView.DragCamera.OrbitType
        Case ORBIT_TYPE_ORBIT_ONLY
            MainMenu.CheckMenuItem 359, True
            SelectionToolbar.CheckMenuItem 10, True
        Case ORBIT_TYPE_ORBIT_OBJECT
            MainMenu.CheckMenuItem 360, True
            SelectionToolbar.CheckMenuItem 11, True
        Case ORBIT_TYPE_CAMERA_ARC
            MainMenu.CheckMenuItem 361, True
            SelectionToolbar.CheckMenuItem 12, True
    End Select


    'Check According to the module mousefocus
    ' Pick Mode

   If DesignView.ModuleMouseFocus is DesignView.Pick3D Then

      If DesignView.RWSUtility.HighlightMaterials Then

         MainMenu.CheckMenuItem 367, True
         SelectionToolbar.CheckMenuItem 14, True

      else

         '   When the method exists to update the menu text ...
         '   MainMenu.SetMenuItemText 333, "Next pick mode"
         '   MainMenu.SetMenuItemText, 352, "Previous pick mode"
         Select Case DesignView.Object3D.AxisMode
            case 0 ' Translate
               MainMenu.CheckMenuItem         328, True
               SelectionToolbar.CheckMenuItem 5,   True
            case 1 ' Rotate
               MainMenu.CheckMenuItem         349, True
               SelectionToolbar.CheckMenuItem 6,   True
            case 2 ' Scale
               MainMenu.CheckMenuItem         350, True
               SelectionToolbar.CheckMenuItem 7,   True
            case 3 ' Pick ONLY
               MainMenu.CheckMenuItem         357, True
               SelectionToolbar.CheckMenuItem 9,   True
            case 4 ' Drag
               MainMenu.CheckMenuItem         363, True
               SelectionToolbar.CheckMenuItem 13,  True
         End Select

      End If

      Exit Sub

   End If

 '  When the method exists to update the menu text ...
 '    MainMenu.SetMenuItemText 333, "Next camera mode"
 '    MainMenu.SetMenuItemText, 352, "Previous camera mode"

    ' Fly Mode
    If DesignView.ModuleMouseFocus is DesignView.Camera Then
        MainMenu.CheckMenuItem         329, True
        SelectionToolbar.CheckMenuItem 1,   True
        Exit Sub
    End If

    If DesignView.ModuleMouseFocus is DesignView.DragCamera Then

        Select Case DesignView.DragCamera.NavigationMode

            Case 0  'Pan
                MainMenu.CheckMenuItem         331, True
                SelectionToolbar.CheckMenuItem 3,   True
            Case 1  'Zoom
                MainMenu.CheckMenuItem         332, True
                SelectionToolbar.CheckMenuItem 4,   True
            Case 2  'Orbit
                MainMenu.CheckMenuItem         330, True
                SelectionToolbar.CheckMenuItem 2,   True
        End Select
        Exit Sub
    End If
End Sub

'********************************************************************** 
'                           Target Menu 
'**********************************************************************
 
' --------------------------------------------------------------------- 
Sub OnMenuReloadGame 
    GlobalScript.ResetGame (True) ' True: Resend all assets 
End Sub 
 
' --------------------------------------------------------------------- 
Sub OnMenuResetAllEntities 
    Dim GlobalFolder 
    If Not GlobalScript.RWSScript.Game Is Nothing Then 
        Set GlobalFolder = GlobalScript.RWSScript.Game.GlobalFolder 
        If Not GlobalFolder Is Nothing Then 
            GlobalScript.ResetEntities GlobalFolder.ID 
        End If 
 
        If Not GlobalScript.RWSScript.Game.ActiveFolder Is Nothing Then 
            GlobalScript.ResetEntities GlobalScript.RWSScript.Game.ActiveFolder.ID 
        End If 
    End If 
End Sub 
 
' --------------------------------------------------------------------- 
Sub OnMenuDirectorsCamera 
    GlobalScript.ToggleDirectorsCamera 
    GlobalScript.SetDirectorsCameraChecks 
End Sub 
 
' --------------------------------------------------------------------- 
Sub OnMenuPauseMode 
    GlobalScript.TogglePauseMode 
    GlobalScript.SetPauseModeChecks 
End Sub

' ---------------------------------------------------------------------
Sub OnMenuClean
    If Not BuildScriptManager.strActiveTarget = "" Then
        '                            Clean, Build, Connect, AllFolders, Target
        BuildScriptManager.BuildGame True, False, False, False, BuildScriptManager.strActiveTarget
    End If
End Sub

' ---------------------------------------------------------------------
Sub OnMenuBuild
    If Not BuildScriptManager.strActiveTarget = "" Then
        '                            Clean, Build, Connect, AllFolders, Target
        BuildScriptManager.BuildGame False, True, False, False, BuildScriptManager.strActiveTarget
    End If
End Sub

' ---------------------------------------------------------------------
Sub OnMenuRebuild 
    If Not BuildScriptManager.strActiveTarget = "" Then
        '                            Clean, Build, Connect, AllFolders, Target
        BuildScriptManager.BuildGame True, True, False, False, BuildScriptManager.strActiveTarget
    End If
End Sub

' ---------------------------------------------------------------------
Sub OnMenuCleanAll
    If Not BuildScriptManager.strActiveTarget = "" Then
        '                            Clean, Build, Connect, AllFolders, Target
        BuildScriptManager.BuildGame True, False, False, True, BuildScriptManager.strActiveTarget
    End If
End Sub

' ---------------------------------------------------------------------
Sub OnMenuBuildAll 
    If Not BuildScriptManager.strActiveTarget = "" Then
       '                            Clean, Build, Connect, AllFolders, Target
       BuildScriptManager.BuildGame False, True, False, True, BuildScriptManager.strActiveTarget
    End If
End Sub

' ---------------------------------------------------------------------
Sub OnMenuRebuildAll 
    If Not BuildScriptManager.strActiveTarget = "" Then
        '                            Clean, Build, Connect, AllFolders, Target
        BuildScriptManager.BuildGame True, True, False, True, BuildScriptManager.strActiveTarget
    End If
End Sub

' ---------------------------------------------------------------------
Sub OnMenuConnect  
    If Not BuildScriptManager.strActiveTarget = "" Then
      '                               Clean, Build, Connect, AllFolders, Target
      BuildScriptManager.BuildConnect False, False, True, False, BuildScriptManager.strActiveTarget
    End If
End Sub 
 
' ---------------------------------------------------------------------
Sub OnMenuBuildAndConnect  
    If Not BuildScriptManager.strActiveTarget = "" Then
      '                               Clean, Build, Connect, AllFolders, Target
      BuildScriptManager.BuildConnect False, True, True, False, BuildScriptManager.strActiveTarget
    End If
End Sub

' ---------------------------------------------------------------------
Sub OnMenuReloadGameTarget
    BuildScriptManager.ReloadGame (BuildScriptManager.strActiveTarget)
End Sub

' --------------------------------------------------------------------- 
Sub OnMenuResetAllEntitiesTarget
    BuildScriptManager.ResetAllEntities (BuildScriptManager.strActiveTarget)
End Sub

' --------------------------------------------------------------------- 
Sub OnSyncCamera
    BuildScriptManager.SendSyncCamera (BuildScriptManager.strActiveTarget)
End Sub

' ---------------------------------------------------------------------
Sub OnMenuLaunch
    If Not BuildScriptManager.strActiveTarget = "" Then     
         Dim CurrentPlatform, CurrentTarget, cmdVal, cmdExe, RWSMakeLauncher  
         CurrentPlatform  = TargetLink.GetField (BuildScriptManager.strActiveTarget, "Platform")   
         CurrentTarget    = TargetLink.GetField (BuildScriptManager.strActiveTarget, "Target")    
         cmdVal           = TargetContextMenu.ConnectionDlg.CommandValue (CurrentPlatform, CurrentTarget, 0) 
         cmdExe           = BuildScriptManager.ExpandString (BuildScriptManager.strActiveTarget, cmdVal)   
 
         If Not cmdExe = "" Then 
            ' Create the Make object and hook up its events   
            Set RWSMakeLauncher = CreateObject ("CSL.RWSBuild.Make")   
            GlobalScript.RWSMakeProxy.ConnectMake RWSMakeLauncher   
 
            ' Load the rule and run the build process to launch the command   
            RWSMakeLauncher.LoadRule AppSettings.BuildRulesDirectory & "\TargetLaunch.rule", True 
            RWSMakeLauncher.AddTask  "RWSLaunch", cmdExe, CurrentPlatform   
            RWSMakeLauncher.Build   
 
            ' Disconnect the events   
            GlobalScript.RWSMakeProxy.DisconnectMake RWSMakeLauncher
         End If
    End If
End Sub

' -------------------------------------------------------------------- 
Function OnProperties ()
    TargetContextMenu.SetupConnectionDlg (BuildScriptManager.strActiveTarget)

    ' Display the dialog
    If TargetContextMenu.ConnectionDlg.Show () Then
        TargetContextMenu.StoreConnectionDlgValues (BuildScriptManager.strActiveTarget)
        OnProperties = True
    Else
        OnProperties = False
    End If
End Function


'**********************************************************************
'                           Options Menu
'**********************************************************************

' ---------------------------------------------------------------------
Sub OnMenuFlightOptions
    ' Flight preferences
    FlightOptions.Initialize 
    RENManager.ShowModal "FlightOptions", "CSLRenderwareStudioWorkspace"    
End Sub

' ---------------------------------------------------------------------
Sub OnMenuFlightKeys
    ' Flight keys
    DesignView.Camera.ChangeFlightKeys
End Sub

' ---------------------------------------------------------------------
Sub OnMenuDragOptions
    ' Snap preferences
    DesignView.Object3D.ConfigureSnapOptions
End Sub

' ---------------------------------------------------------------------
Sub OnMenuDisplayOptions
    DisplayOptions.Initialize
    RENManager.ShowModal "DisplayOptions", "CSLRenderwareStudioWorkspace"
End Sub

' ---------------------------------------------------------------------
Sub OnMenuWorkspaceOptions
    WorkspaceOptions.Initialize
    RENManager.ShowModal "WorkspaceOptions", "CSLRenderwareStudioWorkspace"
End Sub

' --------------------------------------------------------------------- 
Sub OnMenuSplineOption
    SplineOptions.DoModal
End Sub

' ---------------------------------------------------------------------
Sub OnMenuLightmapOption
    LightmapEditor.oLightmapEditor.ChangeOptions
End Sub


'**********************************************************************
'                           Tools Menu
'**********************************************************************

' ---------------------------------------------------------------------
Sub OnMenuToolsParseSource
    GlobalScript.ParseSource
End Sub

Sub OnMenuToolsValidateDB
    DBValidate.Validate
End Sub

'**********************************************************************
'                           Help Menu
'**********************************************************************

' ---------------------------------------------------------------------
Sub OnMenuWorkSpaceHelp
    'Help - ui guide
    Shell32Object.ShellExecute AppSettings.HelpFilename
End Sub

' ---------------------------------------------------------------------
Sub OnMenuAbout
   About.Initialize
   RENManager.ShowModal "About", "CSLRenderwareStudioWorkspace"
End Sub

'**********************************************************************
'                           Helpers
'**********************************************************************

' ---------------------------------------------------------------------
Sub DesignViewtoMenu_Object3DAxisModeChanged ( new_mode )
   ' Something has changed the axis mode - need to update the
   ' toolbar buttons / menu checks
   SetSelectionMenuChecks
   ObjectInformation.SetCurrentMode new_mode
End Sub
