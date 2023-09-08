'
' Enterprise Layout Management Script
'
' Layout object 'BuildScriptManager'
'
' This script contains all global variables and functions for using the
' Game Production Manager.

Option Explicit ' All variables must be explicitly declared (with Dim)

'------------------------------------------------------------------------------
' Script Variables
'------------------------------------------------------------------------------
Dim strActiveTarget, strTargetResourceRoot, strMakeAssociation
Dim TestFireEventTag, ActiveFolderTag, ReloadAssetMsg
Dim PreImportAssetMessage, PostImportAssetMessage, PostImportAssetsMessage
Dim ConnectedMask, UnconnectedMask, bImporting, ActiveFolderID
Dim g_intNumBuildWarnings, g_intNumBuildErrors
Dim bCameraChanged, nCameraTimerElapse, bIsActiveTarget
g_intNumBuildWarnings = 0
g_intNumBuildErrors   = 0
ActiveFolderID        = 0
strActiveTarget       = ""
strTargetResourceRoot = ""
strMakeAssociation    = "RWSMAKE"
bImporting            = False
bCameraChanged        = False
nCameraTimerElapse    = 33    ' Approximately 30fps update of directors camera
bIsActiveTarget       = True

'------------------------------------------------------------------------------
' COM objects
'------------------------------------------------------------------------------
Dim SequenceTranslator, BSMScript, BSMTimer, RF3Translator, TgMenu
Set TgMenu             = MainMenu.MenuItems ("&Target")
Set SequenceTranslator = CreateObject ("CSL.RWSSequencerXlator.RWSSequenceTranslator")
Set BSMScript          = CreateObject ("CSL.RWSScript.RWSScript")
Set BSMTimer           = CreateObject ("CSL.RWSUtils.RWSTimer")
Set RF3Translator      = CreateObject ("RwRf3Translator.Rf3Translator")
RENHost.ConnectObject SequenceTranslator, "SequenceTranslator"
RENHost.ConnectObject BSMScript,          "BSMScriptEvents"
RENHost.ConnectObject BSMTimer,           "BSMTimerEvents"
RENHost.ConnectObject RF3Translator,      "RF3TranslatorEvents"

'------------------------------------------------------------------------------
' Script constants
'------------------------------------------------------------------------------
' Consts from RWSTarget's DataType enumeration 
Const dtInt32 = 1
Const dtReal  = 3


'------------------------------------------------------------------------------
' Initialization dependant on other objects being loaded.
Sub OnLoad ()
   ' Masks to set against the BSMScript object while connected and unconnected.
   ConnectedMask   = Database.otAll - Database.otBehavior - Database.otCommand 
   UnconnectedMask = Database.otAsset
End Sub

'------------------------------------------------------------------------------
Sub SequenceTranslator_OnSequenceTranslate (Sequence, FileBuffer, strPlatformFlags)
    Dim oMake
    Set oMake = CreateObject ("CSL.RWSBuild.Make")
    GlobalScript.RWSMakeProxy.ConnectMake oMake 
    SetupMake oMake, "Name", "ResourceRoot", "", True, True

    oMake.AddTask "SequenceTransform", Sequence, FileBuffer, strPlatformFlags
    oMake.Build

    GlobalScript.RWSMakeProxy.DisconnectMake oMake
End Sub

' --------------------------------------------------------------------
Sub ReloadGame (strTarget) 
    Dim Connection 
    Set Connection = CurrentlySelectedConnection (strTarget)
    If Not Connection Is Nothing Then
       GlobalScript.ResetGameForConnection Connection, True ' True: Resend all assets
    End If
End Sub 

' --------------------------------------------------------------------
Sub ResetAllEntities (strTarget) 
    Dim Connection, GlobalFolder 
    Set Connection = CurrentlySelectedConnection (strTarget) 
 
    If Not Connection Is Nothing Then
        Set GlobalFolder = GlobalScript.RWSScript.Game.GlobalFolder 
        If Not GlobalFolder Is Nothing Then 
           GlobalScript.ResetEntities GlobalFolder.ID 
        End If 

        If Not ActiveFolderID = 0 Then 
           GlobalScript.ResetEntitiesForConnection ActiveFolderID, Connection 
        End If
    End If
End Sub 

' --------------------------------------------------------------------
Sub SetActiveConnection (strNewActive)
    ' Call SetDefaultTargetText with current active target to clear it.
    TargetLink.SetDefaultTargetText strActiveTarget, False
    
    ' Set new active target.
    strActiveTarget = strNewActive 
    
    ' Call SetDefaultTargetText with new active target to set it as bold.
    TargetLink.SetDefaultTargetText strActiveTarget
    
    GlobalScript.EnableBuildOptions
End Sub

'------------------------------------------------------------------------------ 
Sub BuildConnect (bClean, bBuild, bConnect, bBuildAllFolders, strTarget)
   ' If we're already connected, disconnect 
    Dim ConnectionID
    ConnectionID = CurrentlySelectedConnectionID (strTarget)
    If ConnectionID = 0 Then
        'Connect
        BuildGame bClean, bBuild, bConnect, bBuildAllFolders, strTarget ' Clean, Build, Connect, AllFolders, DefaultTarget
    Else 
       OnDisconnect ConnectionID
    End If 
End Sub

'------------------------------------------------------------------------------ 
Sub OnDisconnect (ConnectionID)
    Dim Connection
    Set Connection = RWSComms.Connection (ConnectionID) 
    If Not Connection Is Nothing Then 
        Disconnect Connection 
    End If
End Sub

'------------------------------------------------------------------------------ 
Sub BuildGame (bClean, bBuild, bConnect, bBuildAllFolders, strTarget)
    Dim strName, strPlatform, strAddress, strResourceRoot, strPort, strTimeout
    Dim strRF3Template, bEmbedAssets, ConnectionID, nTimeoutMs, nPort
    Dim Make, Connection 

    ConnectionID = 0 

      strName         = TargetLink.GetField (strTarget, "Name") 
      strPlatform     = TargetLink.GetField (strTarget, "Platform") 
      strAddress      = TargetLink.GetField (strTarget, "Address") 
      strResourceRoot = TargetLink.GetField (strTarget, "Resources") 
      strPort         = TargetLink.GetField (strTarget, "Port") 
      strTimeout      = TargetLink.GetField (strTarget, "Timeout") 
      strRF3Template  = TargetLink.GetField (strTarget, "TargetRF3Template") 
      bEmbedAssets    = True 
      If "no" = TargetLink.GetField (strTarget, "EmbedAssets") Then 
         bEmbedAssets = False 
      End If 
      If Len (strPort) < 1 Then 
         strPort = "0" 
      End If

   ' Now create an RWSMake object and set it up 
    Set Make = CreateObject ("CSL.RWSBuild.Make") 
    GlobalScript.RWSMakeProxy.ConnectMake Make 
    SetupMake Make, strName, strResourceRoot, strRF3Template, bEmbedAssets, True

    ' Build the current data to ensure it's up-to-date 
    TargetLink.SetField strTarget, "Status", "Building..." 

    nTimeoutMs = 0 
    If Len (strTimeout) > 0 Then 
       If Not strTimeout = "Infinite" Then 
          nTimeoutMs = CInt (strTimeout) 
       End If 
    End If 

    ' Allow the sequencer to save its loaded sequence
    SequencerTools.EnsureSequenceAssetSaved Sequencer

    If LaunchBuildRule (Make, strTarget, strName, strPlatform, strResourceRoot,_
                        bEmbedAssets, bClean, bBuild, bBuildAllFolders) _
    Then

       ' Build was successful, so connect if necessary
       If bConnect Then
           If Not bBuild Then
               MessageLog.Warning "[Target] Connecting without building can cause unpredictable results with an out-of-date stream."
           End If
           nPort = CInt (strPort)  
           ConnectionID = LaunchConnectRule (Make, strPlatform, strAddress, strResourceRoot, _  
                                             bEmbedAssets, nPort, nTimeoutMs, 4000) ' 4 second connect timeout
       End If
    End If

    ' Disconnect the Make object if we didn't connect 
    ' Otherwise this is disconnected when the connection is lost/disconnected 
    If ConnectionID = 0 Then 
        On Error Resume Next 
        GlobalScript.RWSMakeProxy.DisconnectMake Make 
        On Error Goto 0 
    End If 

    ' update the status 
    If Len (strTarget) > 0 Then ' strTarget can be set to "" if connection lost 
        TargetLink.SetField strTarget, "ID", CStr (ConnectionID) 
        If ConnectionID = 0 Then 
            TargetLink.SetField strTarget, "Status", "Disconnected" 
        Else 
            TargetLink.SetField strTarget, "Status", "Connected" 
            ' The director's camera starts disabled, so only send it if it's enabled.
            If GlobalScript.gbDirectorsCameraEnabled Then
               GlobalScript.UpdateDirectorsCamera 
            End If
            ' The pause state starts paused, so only send it if we're un-paused.
            If Not GlobalScript.gbPauseGameEnabled Then
               GlobalScript.UpdatePauseMode
            End If
        End If 
    End If 
End Sub 

'------------------------------------------------------------------------------
' Set up the required params that the build scripts require.
Sub SetupMake (oMake, strTargetName, strTargetResourceRoot, strRF3Template, _
               bEmbedAssets, bConsoleBuild)
    ' Set the RF3 template params.
    If Persist.fso.FileExists (strRF3Template) Then
        oMake.SetParam "RF3PROJECTTEMPLATENAME", Persist.fso.GetFilename         (strRF3Template)
        oMake.SetParam "RF3PROJECTTEMPLATEPATH", Persist.fso.GetParentFolderName (strRF3Template)
    Else
        ' Use the default template, don't set the path.
        oMake.SetParam "RF3PROJECTTEMPLATENAME", "Generic.rwt" 
        oMake.SetParam "RF3PROJECTTEMPLATEPATH", RF3Translator.ProjectTemplateFolder
        If len (strRF3Template) > 0 Then
            BuildLog.Warning "[Target] The RF3 template, " & _
                             strRF3Template & ", does not exist"
        End If
    End If

    ' Set the RF3 template directories if they need to change.
    Dim strProjectTemplateDirectory, strAssetTemplateDirectory
    strProjectTemplateDirectory = oMake.GetParam ("RF3PROJECTTEMPLATEPATH")
    strAssetTemplateDirectory   = Persist.fso.GetParentFolderName (strProjectTemplateDirectory) & "\Asset"
    If Not RF3Translator.ProjectTemplateFolder = strProjectTemplateDirectory Then
       RF3Translator.ProjectTemplateFolder = strProjectTemplateDirectory
    End If
    If Not RF3Translator.AssetTemplateFolder = strAssetTemplateDirectory Then
       RF3Translator.AssetTemplateFolder   = strAssetTemplateDirectory
    End If

    oMake.SetParam "DESTDIR",         Persist.GetPlatformBuildDirectory (strTargetName)
    oMake.SetParam "RESOURCEROOT",    strTargetResourceRoot ' Deprecated, use "TGTRESOURCEROOT"
    oMake.SetParam "TGTRESOURCEROOT", strTargetResourceRoot
    oMake.SetParam "WKSRESOURCEROOT", GlobalScript.g_strResourceRoot
    oMake.SetParam "EMBEDASSETS",     bEmbedAssets 
    oMake.SetParam "SENTOBJECTS",     CreateObject ("CSL.RWSScript.RWSContainer") 
    oMake.SetParam "TOOLDIR",         AppSettings.ToolDirectory 
    oMake.SetParam "CONSOLEBUILD",    bConsoleBuild
    oMake.SetParam "RF3TRANSLATOR",   RF3Translator
    oMake.SetParam "RWSUTILS",        RWSUtils
    oMake.SetParam "RESOURCEROOTMANAGED", VersionControl.IsPathManaged (GlobalScript.g_strResourceRoot)
    ' Load the game-specific rules 
    LoadRules oMake, Persist.GetBuildDirectory ("Build Rules")

    ' Load the Global app rules 
    LoadRules oMake, AppSettings.BuildRulesDirectory 
End Sub 

'------------------------------------------------------------------------------ 
Function LaunchConnectRule (oMake, strPlatform, strAddress, strResourceRoot, _ 
                            bEmbedAssets, nPort, nSendTimeoutMs, nConnectTimeoutMs)
    Dim Connection, strPlatformFlags, PlatformFilter, strMessage, strPath, fso

    ' Create a connection to the console 
    Set Connection = GlobalScript.RWSComms.CreateConnection (strAddress, nPort, nSendTimeoutMs, nConnectTimeoutMs, strPlatform, strResourceRoot, bEmbedAssets) 
    If Connection Is nothing Then 
       CSLRenderwareStudioWorkspace.ShowObject MessageLog
       MessageLog.Error "[Target] Failed to connect to " & strAddress 
       LaunchConnectRule = 0 
       Exit Function 
    End If

    ' Make sure the event mask is set so that all appropriate events are 
    ' captured while connected to a console. 
    BSMScript.SetEventMask ConnectedMask
    BSMScript.QueuedEvents = True 

    ' Set up the platform flags 
    strPlatformFlags   = GlobalScript.RWSComms.GetPlatformFlags (Connection.Platform) 
    Set PlatformFilter = GlobalScript.RWSComms.CreatePlatformFilter (strPlatformFlags) 

    ' Associate the make object with this connection 
    ' This link is broken when the connection is lost/disconnected 
    Connection.SetParam strMakeAssociation, oMake 

    ' Inform other controls we've connected 
    OnConnect Connection 

    oMake.SetParam "Cleaning", False 
    oMake.AddTask "Connect", BSMScript.Game, PlatformFilter, Connection, True 
    If Not oMake.Build () Then
        ' Failed to build, but we're now connected, so disconnect 
        On Error Resume Next 
        Connection.Disconnect 
        On Error Goto 0
        LaunchConnectRule = 0 
        Exit Function 
    End If

    LaunchConnectRule = Connection.ID 
End Function
 
'------------------------------------------------------------------------------ 
Function LaunchBuildRule (oMake, strConnectionID, strConnectionName, _
                          strPlatform, strResourceRoot, _
                          bEmbedAssets, bClean, bBuild, bBuildAllFolders) 
    Dim StartTime, Connection, strPlatformFlags, PlatformFilter, strMessage
    Dim strBuildRule, strStartMessage
    StartTime = Time 
    LaunchBuildRule = True 

    ' Create a null connection to perform the build 
    Set Connection = GlobalScript.RWSComms.CreateNullConnection (strPlatform, strResourceRoot, bEmbedAssets) 
    oMake.SetParam "BIGENDIAN", Connection.BigEndian 
    oMake.SetParam "UNICODE",   Connection.Unicode 
    oMake.SetParam "PLATFORM",  Connection.Platform
    oMake.SetParam "PLATFORMFLAGS",  GlobalScript.RWSComms.GetPlatformFlags (Connection.Platform)

    ' Get the connection's platform filter 
    strPlatformFlags   = GlobalScript.RWSComms.GetPlatformFlags (Connection.Platform) 
    Set PlatformFilter = GlobalScript.RWSComms.CreatePlatformFilter (strPlatformFlags) 

    Persist.SaveProject Persist.SaveChanges
    If Len (Persist.strCurrentFilename) = 0 Then ' User hit cancel
        strMessage = "[Target] Save abandoned - aborting " 
        If bClean Then 
            strMessage = strMessage & "the clean operation" 
        ElseIf bBuild Then 
            strMessage = strMessage & "the build operation" 
        End If 

        BuildLog.Log strMessage 
        LaunchBuildRule = False 
        Exit Function 
    End If 

    ' Reset the number of warnings and errors to report from the build.
    g_intNumBuildWarnings = 0
    g_intNumBuildErrors   = 0

    ' Set the appropriate build rule, depending if there's an overloaded game build rule or
    ' whether you're building all folders.
    If Len (BSMScript.Game.BuildCommand) > 0 Then
        ' There's a custom game build rule, so use it.
        strBuildRule    = BSMScript.Game.BuildCommand
        strStartMessage = "using the custom game rule '" & strBuildRule & "'"
    Else
        ' There isn't a custom game build rule, use one of the defaults.
        If bBuildAllFolders Then
            strBuildRule    = "BuildAllFolders"
            strStartMessage = "all folders"
        Else
            strBuildRule    = "BuildGame"
            strStartMessage = "the active and global folders"
        End If
    End If
    strStartMessage = strStartMessage & " for the " & strConnectionName & " target..." 

    If bClean Then 
        CSLRenderwareStudioWorkspace.ShowObject BuildLog
        BuildLog.Log "[Target] Cleaning " & strStartMessage
        oMake.SetParam "Cleaning", True 
        oMake.AddTask strBuildRule, BSMScript.Game, PlatformFilter 
        Broadcast.PreCleanProject strConnectionID, strConnectionName
        If Not oMake.Clean () Then 
            LaunchBuildRule = False 
            oMake.SetParam "Cleaning", False 
        End If 
        oMake.SetParam "Cleaning", False 
        Broadcast.PostCleanProject strConnectionID, strConnectionName, LaunchBuildRule
    End If 

    If bBuild And LaunchBuildRule Then 
        Dim fso, strPath 
        Set fso = CreateObject ("Scripting.FileSystemObject") 
        CSLRenderwareStudioWorkspace.ShowObject BuildLog
        BuildLog.Log "[Target] Building " & strStartMessage
        strPath = oMake.GetParam ("DESTDIR") 
        strPath = fso.BuildPath (strPath, "Misc") 

        ' Create misc folder if it doesn't exist 
        If Not fso.FolderExists (strPath) Then 
            GlobalScript.CreateDirectory strPath, fso 
        End If 

       ' Write out the globals files (if necessary) which all assets/entities depend on 
       ' This determines whether the workspace resource root, connection resource root 
       ' or "embed assets" flags etc. have been changed since the last build 
       UpdateEntitySettingsFile fso.BuildPath (strPath, "entityglobals.txt"), GlobalScript.g_strSourceRoot 
       UpdateAssetSettingsFile fso.BuildPath (strPath, "assetglobals.txt"),_ 
           oMake.GetParam ("TGTRESOURCEROOT"), oMake.GetParam ("WKSRESOURCEROOT"), bEmbedAssets 

       oMake.SetParam "Cleaning", False 
       oMake.AddTask "ParseSource", GlobalScript
       oMake.AddTask strBuildRule,  BSMScript.Game, PlatformFilter 

       Dim XMLLog
       Set XMLLog = Nothing
       ' If you do not wish to create a log of build activity, comment out the line below.
       Set XMLLog = CreateObject ("Msxml2.DOMDocument.3.0")

       Broadcast.PreBuildProject strConnectionID, strConnectionName
       If Not oMake.Build (XMLLog) Then 
           LaunchBuildRule = False 
       End If
       Broadcast.PostBuildProject strConnectionID, strConnectionName, LaunchBuildRule

       If Not XMLLog Is Nothing Then
           SaveLog XMLLog, Persist.GetParentFolderName (oMake.GetParam ("DESTDIR")) &_
                           "\BuildActivity.xml"
       End If
    End If 

    ReportTime StartTime, Time 
End Function 

'------------------------------------------------------------------------------
Sub UpdateEntitySettingsFile (strFilename, strSourceRoot) 
    Dim fso, bUpdateContent, TextFile 
    Set fso = CreateObject ("Scripting.FileSystemObject") 

    bUpdateContent = True 

    ' If file exists, check its contents are up to date 
    If fso.FileExists (strFilename) Then 
        Set TextFile = fso.OpenTextFile (strFilename) 
        If Not TextFile Is Nothing Then
            ' Check file contents 
            Dim strSR 

            ' Read the data 
            On Error Resume Next ' Trap "Input past end of file" error 
                strSR = TextFile.ReadLine () 
            On Error Goto 0 

            ' Compare source root 
            If strSR = strSourceRoot Then 
                bUpdateContent = false 
            End If 
 
            TextFile.Close 
        End If 
    End If 

    ' Now write out new content if necessary 
    If bUpdateContent Then
        Dim strRetryMessage
        strRetryMessage = "The entity settings file is read only: " & CHR(13) & strFilename & CHR(13) & _
                          "The file should not be under source control or set as read only." & CHR(13) & _
                          "Make it write enabled to allow saving, then click Yes." & CHR(13) & _
                          "Do you wish to try saving the file again?"
        If Persist.CanTryToSaveFile (strFilename, strRetryMessage) Then
            ' Write out the entity settings file.
            Set TextFile = fso.CreateTextFile (strFilename, True, False) 

            ' Write the values to the file 
            TextFile.WriteLine strSourceRoot 
            TextFile.Close
        Else
            BuildLog.Warning "[Workspace] Entity settings file not saved. It may contain the incorrect source root, so the entities may be built incorrectly."
            g_intNumBuildWarnings = g_intNumBuildWarnings + 1
        End If
    End If
End Sub 

'------------------------------------------------------------------------------ 
' 
Sub UpdateAssetSettingsFile (strFilename, strResourceRoot, strWorkspaceResourceRoot, bEmbedAssets) 
    Dim fso, bUpdateContent, TextFile 
    Set fso = CreateObject ("Scripting.FileSystemObject") 

    bUpdateContent = True 

    ' If file exists, check its contents are up to date 
    If fso.FileExists (strFilename) Then 
        Set TextFile = fso.OpenTextFile (strFilename) 
        If Not TextFile Is Nothing Then 
            ' Check file contents 
            Dim strRR, strWRR, strEmbed 

            ' Read the data 
            On Error Resume Next ' Trap "Input past end of file" error 
                strRR    = TextFile.ReadLine () 
                strWRR   = TextFile.ReadLine () 
                strEmbed = TextFile.ReadLine () 
            On Error Goto 0 

            ' Compare strings and "embedded assets" flag 
            If strRR = strResourceRoot And strWRR = strWorkspaceResourceRoot Then
                If bEmbedAssets Then 
                    If strEmbed = "true" Then ' File content matches 
                        bUpdateContent = false 
                    End If 
                 Else 
                     If strEmbed = "false" Then ' File content matches 
                         bUpdateContent = false 
                     End If 
                 End If 
            End If 
 
        TextFile.Close 
        End If 
    End If 

    ' Now write out new content if necessary 
    If bUpdateContent Then 
        Dim strRetryMessage
        strRetryMessage = "The asset settings file is read only: " & CHR(13) & strFilename & CHR(13) & _
                          "The file should not be under source control or set as read only." & CHR(13) & _
                          "Make it write enabled to allow saving, then click Yes." & CHR(13) & _
                          "Do you wish to try saving the file again?"
        If Persist.CanTryToSaveFile (strFilename, strRetryMessage) Then
            ' Write out the entity settings file.
            Set TextFile = fso.CreateTextFile (strFilename, True, False)
            ' Write the values to the file 
            TextFile.WriteLine strResourceRoot 
            TextFile.WriteLine strWorkspaceResourceRoot 
            If bEmbedAssets Then 
                  TextFile.WriteLine "true" 
            Else 
                  TextFile.WriteLine "false" 
            End If 

            TextFile.Close 
        Else
            BuildLog.Warning "[Workspace] Asset settings file not saved. It may contain the incorrect resource root, so the assets may be built incorrectly."
            g_intNumBuildWarnings = g_intNumBuildWarnings + 1
        End If
    End If
End Sub 
 
'------------------------------------------------------------------------------ 
' 
Function BuildRule (APIObject, DefaultRule) 
    If Len (APIObject.BuildCommand) > 0 Then 
        BuildRule = APIObject.BuildCommand 
    Else 
        BuildRule = DefaultRule 
    End If
End Function 
 
'------------------------------------------------------------------------------ 
' Loads all of the rules from a given directory into the RWSMake object. 
Sub LoadRules (RWSMake, Directory) 
    Dim fso, Folder, File, Extension 
    Set fso = CreateObject ("Scripting.FileSystemObject") 
    If fso.FolderExists (Directory) Then 
        Set Folder = fso.GetFolder (Directory) 
        For Each File in Folder.Files 
            Extension = fso.GetExtensionName (File) 
            If Right (Extension, 4) = "rule" Then 
                RWSMake.LoadRule File, True ' True allows duplicate rules to exist
            End If 
        Next 
    End If 
End Sub 
 
'------------------------------------------------------------------------------ 
Sub ReportTime (StartTime, EndTime) 
    Dim DeltaMinute, DeltaSecond
    
    DeltaMinute = Minute (EndTime - StartTime) 
    DeltaSecond = Second (EndTime - StartTime) 
    BuildLog.Log "[Target] Build process took " & DeltaMinute & " minutes and " & DeltaSecond & " seconds." 
    BuildLog.Log "[Target] Build process - " & g_intNumBuildWarnings & " warning(s) " & _
                                               g_intNumBuildErrors   & " error(s)"
End Sub 

'------------------------------------------------------------------------------ 
' Called when a connection is successfully accepted 
Sub OnConnect (Connection) 
   ' Inform any controls that the connection has been accepted 
   MetricsCtrl.OnConnect          Connection.ID 
   MemoryCtrl.OnConnect           Connection.ID 
   RWSFileSaver.OnConnect         Connection.ID, "RWS::FileSave" 
   RWSProfileViewerCtrl.OnConnect Connection.ID 
   StreamEditControl.OnConnect    Connection.ID
   Broadcast.ConnectToTarget Connection.ID
   
   ' Enable the timer used to send camera messages to the console
   BSMTimer.EnableTimer nCameraTimerElapse
End Sub 
 
'------------------------------------------------------------------------------ 
Sub Disconnect (Connection) 
    Dim Make 
    Set Make = Connection.GetParam (strMakeAssociation) 

    Make.AddTask "Disconnect", Connection 
    If Not Make.Build () Then 
        On Error Resume Next 
        Connection.Disconnect 
        On Error Goto 0 
    End If 

   ' If we have no more connections to the console, then disable the timer.
   ' We use less than two since the disconnecting target is yet to be removed
   ' from the container at this stage.
   If GlobalScript.RWSComms.Connections.Count < 2 Then
      BSMTimer.DisableTimer
   End If
End Sub  

'------------------------------------------------------------------------------  
Sub ConnectionLost (Connection)  
    Dim RowUID, strID, strAddress 
 
    ' Find row in the table  
    For Each RowUID In TargetLink.GetRowUIDs  
        strID = TargetLink.GetField (RowUID, "ID")  
        If Len (strID) > 0 Then  
            If CInt (strID) = Connection.ID Then 
                ' Update status  
                TargetLink.SetField RowUID, "ID", "0"  
                TargetLink.SetField RowUID, "Status", "Disconnected"  
 
                strAddress = TargetLink.GetField (RowUID, "Address")  
                MessageLog.Log "[Target] No longer connected to " & strAddress 
                Exit For  
            End If  
        End If  
    Next  
 
    ' If there are no more connections, turn off the events  
    If GlobalScript.RWSComms.Connections.Count < 1 Then  
        On Error Resume Next ' Might not be connected to database  
        BSMScript.SetEventMask UnconnectedMask
        On Error Goto 0  
    End If  
 
    ' Inform any controls that the connection has been lost  
    MetricsCtrl.OnDisconnect          Connection.ID  
    MemoryCtrl.OnDisconnect           Connection.ID  
    RWSFileSaver.OnDisconnect         Connection.ID  
    RWSProfileViewerCtrl.OnDisconnect Connection.ID  
    StreamEditControl.OnDisconnect    Connection.ID  
 
    ' Destroy the associated make object  
    On Error Resume Next  
    GlobalScript.RWSMakeProxy.DisconnectMake Connection.GetParam (strMakeAssociation)  
    On Error Goto 0  
 
    Connection.SetParam strMakeAssociation, Nothing 
    Broadcast.DisconnectFromTarget Connection.ID
End Sub

'------------------------------------------------------------------------------   
Sub SendSyncCamera (strTarget)
    Dim Connection, Make
    Set Connection = CurrentlySelectedConnection (strTarget)

    If Not Connection Is Nothing Then
        Set Make = Connection.GetParam (strMakeAssociation)
        Make.AddTask "SyncCamera", Connection   
        Make.Build
    End If
End Sub

' --------------------------------------------------------------------  
Sub TestFireReceiveEvent (Entity, AttributeOrCommand)  
    Dim Make, Connection
    For Each Connection In GlobalScript.RWSComms.Connections  
        Set Make = Connection.GetParam (strMakeAssociation)    
        Make.AddTask "TestFireReceiveEvent", Connection, Entity, AttributeOrCommand  
        Make.Build  
   Next  
End Sub  
  
' --------------------------------------------------------------------  
Sub TestFireSendEvent (theEvent)  
    Dim Make, Connection
    For Each Connection In GlobalScript.RWSComms.Connections  
        Set Make = Connection.GetParam (strMakeAssociation)
        Make.AddTask "TestFireSendEvent", Connection, theEvent.Name  
        Make.Build  
    Next  
End Sub 

' --------------------------------------------------------------------   
Sub RunExtendedCommand (nCustomMenuIndex, strTarget) 
    Dim CurrentPlatform, CurrentTarget, cmdVal, cmdExe, RWSMakeLauncher 
    CurrentPlatform  = TargetLink.GetField (strTarget, "Platform")  
    CurrentTarget    = TargetLink.GetField (strTarget, "Target")   
    cmdVal           = TargetContextMenu.ConnectionDlg.CommandValue (CurrentPlatform, CurrentTarget, nCustomMenuIndex)
    cmdExe           = ExpandString (strTarget, cmdVal)  

    ' Create the Make object and hook up its events  
    Set RWSMakeLauncher = CreateObject ("CSL.RWSBuild.Make")  
    GlobalScript.RWSMakeProxy.ConnectMake RWSMakeLauncher  

    ' Load the rule and run the build process to launch the command  
    RWSMakeLauncher.LoadRule AppSettings.BuildRulesDirectory & "\TargetLaunch.rule", True
    RWSMakeLauncher.AddTask  "RWSLaunch", cmdExe, CurrentPlatform  
    RWSMakeLauncher.Build  

    ' Disconnect the events  
    GlobalScript.RWSMakeProxy.DisconnectMake RWSMakeLauncher  
End Sub 

'------------------------------------------------------------------------------
Sub PreImportAsset (Asset)

    ' Set the importing flag to prevent recursive calls.
    If bImporting Then
        Exit Sub
    End If
    bImporting = True

    Dim strImportAssetFilename
    strImportAssetFilename = Persist.BuildPath (GlobalScript.g_strResourceRoot, Asset.File)

    ' Get the asset info
    Set Asset = GetRootRF3Asset (Asset)
    If Len (Asset.File) > 0 Then
        ' Only interested in creating RF3 contents
        Dim fso, strAssetFile
        Set fso = CreateObject ("Scripting.FileSystemObject")
        strAssetFile = Asset.File

        If Not fso.FileExists (strAssetFile) Then
            strAssetFile = Persist.BuildPath (GlobalScript.g_strResourceRoot, Asset.File)
            If Not fso.FileExists (strAssetFile) Then
                bImporting = False
                Exit Sub
            End If
        End If

        If Not Asset Is Nothing Then
            If Left (Asset.Type, 3) = "RF3" Then
                  Dim RWSMakeRF3

                  ' Create the Make object and hook up its events
                  Set RWSMakeRF3 = CreateObject ("CSL.RWSBuild.Make")
                  GlobalScript.RWSMakeProxy.ConnectMake RWSMakeRF3

                  ' Set the build parameters so they are available from RF3Asset.rule
                  SetupMake RWSMakeRF3, "", "", GlobalScript.g_strRF3ProjectTemplateFile, False, False
                  RWSMakeRF3.SetParam "Cleaning", False
                  Dim XMLLog
                  Set XMLLog = Nothing
                  ' If you do not wish to create a log of build activity, comment out the line below.
                  Set XMLLog = CreateObject ("Msxml2.DOMDocument.3.0")

                  ' Load the rule and build the asset
                  RWSMakeRF3.AddTask  "RF3AssetExporter", Asset.ID
                  RWSMakeRF3.Build XMLLog

                  If Not XMLLog Is Nothing Then
                     Dim strLogName
                     strLogName = "BuildActivity"
                     If Persist.Fso.FileExists (strAssetFile) Then
                        strLogName = strLogName & "_" & Persist.Fso.GetBaseName (strImportAssetFilename)
                     End If
                     strLogName = strLogName & ".xml"
                     SaveLog XMLLog, GetRF3ExportsLocation (Asset, RWSMakeRF3) & strLogName
                  End If

                  ' Disconnect the events
                  GlobalScript.RWSMakeProxy.DisconnectMake RWSMakeRF3
            End If
        End If
    End If

    bImporting = False ' Release the importing flag.
End Sub

'------------------------------------------------------------------------------
Sub PostImportAsset (Asset)
    ' Get the asset info
    ' If this is a spline asset, inform the spline editor that it's been imported
    If Asset.Type = "rwID_SPLINE" Then
        RWSSpline.AddSpline Asset
    End If
End Sub

'------------------------------------------------------------------------------
' This function is called before we reload an asset.
' If the asset is an RF3, it deletes any files that would be generated from it.
' This is is preparation for the reload so that it will generate ALL of its
' files.
Sub PreReloadAsset (AssetID)
    Dim Asset

    ' Get the asset info
    Set Asset = BSMScript.Asset (AssetID)
    Set Asset = GetRootRF3Asset (Asset)
    If Not Asset Is Nothing Then
        If Left (Asset.Type, 3) = "RF3" Then
            Dim RWSMakeRF3

            ' Create the Make object and hook up its events
            Set RWSMakeRF3 = CreateObject ("CSL.RWSBuild.Make")
            GlobalScript.RWSMakeProxy.ConnectMake RWSMakeRF3

            ' Clean the contents list for this asset, so we will re-read
            ' the rf3 file and rebuild the generated files when we reload.
            SetupMake RWSMakeRF3, "", "", GlobalScript.g_strRF3ProjectTemplateFile, False, False

            ' Load the rule and clean the asset
            RWSMakeRF3.SetParam "Cleaning", True
            RWSMakeRF3.AddTask "RF3AssetExporter", Asset.ID
            RWSMakeRF3.Clean
            RWSMakeRF3.SetParam "Cleaning", False

            ' Disconnect the events
            GlobalScript.RWSMakeProxy.DisconnectMake RWSMakeRF3
        End If
    End If
End Sub

'------------------------------------------------------------------------------
' This function is called before we reload all assets. This function deletes
' all files generated from an rf3.
' This deletion is required because if the template had changed, the reload
' rule may not have automatically generated the files because the dependencies
' would appear to be up to date.
Sub PreReloadAssets ()
    Dim Asset, RWSMakeRF3

    ' Create the Make object and hook up its events
    Set RWSMakeRF3 = CreateObject ("CSL.RWSBuild.Make")
    GlobalScript.RWSMakeProxy.ConnectMake RWSMakeRF3

    SetupMake RWSMakeRF3, "", "", GlobalScript.g_strRF3ProjectTemplateFile, False, False
    RWSMakeRF3.SetParam "Cleaning", True

    ' Load the rule and clean each root rf3 asset.
    For Each Asset in BSMScript.Assets ()
        If Asset.ParentAssets.Count = 0 Then
            If Left (Asset.Type, 3) = "RF3" Then
                ' Clean all generated files from an rf3 build.
                RWSMakeRF3.AddTask "RF3AssetExporter", Asset.ID
                RWSMakeRF3.Clean
            End If
        End If
    Next
    RWSMakeRF3.SetParam "Cleaning", False

    ' Disconnect the events
    GlobalScript.RWSMakeProxy.DisconnectMake RWSMakeRF3
End Sub

'------------------------------------------------------------------------------ 
' Events from the objects informing us of changes that need reflecting on a
' connected console.
'------------------------------------------------------------------------------ 

' --------------------------------------------------------------------  
Sub BSMScriptEvents_OnChange (APIObject)  
    Dim Connection, Make, PlatformFilter, MemoryBuffer 
    For Each Connection In GlobalScript.RWSComms.Connections  
        Set Make = Connection.GetParam (strMakeAssociation)  
 
        Select Case APIObject.IDType  
            Case Database.otGame, Database.otFolder, Database.otAssetFolder, Database.otAsset, Database.otEntity, Database.otAttributeShare, Database.otAttribute 
                Set PlatformFilter     = GlobalScript.RWSComms.CreatePlatformFilter (GlobalScript.RWSComms.GetPlatformFlags (Connection.Platform))  
                Set MemoryBuffer       = GlobalScript.RWSComms.CreateMemoryBuffer ()  
                MemoryBuffer.BigEndian = Connection.BigEndian  
                Make.AddTask Database.GetTypeString (APIObject) & "_OnChange", APIObject, PlatformFilter, MemoryBuffer  
                Make.Build  
                Connection.SendBuffer MemoryBuffer  
        End Select  
   Next  
End Sub  
  
' --------------------------------------------------------------------  
Sub BSMScriptEvents_OnDelete (APIObject)  
    Dim Connection, Make, PlatformFilter, MemoryBuffer 
    For Each Connection In GlobalScript.RWSComms.Connections  
    Set Make = Connection.GetParam (strMakeAssociation)  
 
    Select Case APIObject.IDType  
        Case Database.otGame, Database.otFolder, Database.otAssetFolder, Database.otAsset, Database.otEntity, Database.otAttributeShare, Database.otAttribute
            Set PlatformFilter     = GlobalScript.RWSComms.CreatePlatformFilter (GlobalScript.RWSComms.GetPlatformFlags (Connection.Platform))  
            Set MemoryBuffer       = GlobalScript.RWSComms.CreateMemoryBuffer ()  
            MemoryBuffer.BigEndian = Connection.BigEndian  
            Make.AddTask Database.GetTypeString (APIObject) & "_OnDelete", APIObject, PlatformFilter, MemoryBuffer  
            Make.Build  
            Connection.SendBuffer MemoryBuffer  
    End Select  
    Next  
End Sub  
  
' --------------------------------------------------------------------  
Sub BSMScriptEvents_OnAdd (Parent, Child)  
    Dim Connection, Make, PlatformFilter, MemoryBuffer 
    For Each Connection In GlobalScript.RWSComms.Connections  
    Set Make = Connection.GetParam (strMakeAssociation)  
 
    Select Case Parent.IDType  
        Case Database.otGame, Database.otFolder, Database.otAssetFolder, Database.otAsset, Database.otEntity, Database.otAttributeShare, Database.otAttribute
            Set PlatformFilter     = GlobalScript.RWSComms.CreatePlatformFilter (GlobalScript.RWSComms.GetPlatformFlags (Connection.Platform))  
            Set MemoryBuffer       = GlobalScript.RWSComms.CreateMemoryBuffer ()  
            MemoryBuffer.BigEndian = Connection.BigEndian  
            Make.AddTask Database.GetTypeString (Parent) & "_OnAdd", Parent, Child, PlatformFilter, MemoryBuffer  
            Make.Build  
            Connection.SendBuffer MemoryBuffer  
    End Select  
    Next  
End Sub  
  
' --------------------------------------------------------------------  
Sub BSMScriptEvents_OnRemove (Parent, Child)  
    Dim Connection, Make, PlatformFilter, MemoryBuffer 
    For Each Connection In GlobalScript.RWSComms.Connections  
        Set Make = Connection.GetParam (strMakeAssociation)  
 
        Select Case Parent.IDType  
            Case Database.otGame, Database.otFolder, Database.otAssetFolder, Database.otAsset, Database.otEntity, Database.otAttributeShare, Database.otAttribute
                Set PlatformFilter     = GlobalScript.RWSComms.CreatePlatformFilter (GlobalScript.RWSComms.GetPlatformFlags (Connection.Platform))  
                Set MemoryBuffer       = GlobalScript.RWSComms.CreateMemoryBuffer ()  
                MemoryBuffer.BigEndian = Connection.BigEndian  
                Make.AddTask Database.GetTypeString (Parent) & "_OnRemove", Parent, Child, PlatformFilter, MemoryBuffer  
                Make.Build  
                Connection.SendBuffer MemoryBuffer  
        End Select  
    Next  
End Sub  
  
' --------------------------------------------------------------------  
Sub BSMScriptEvents_OnCameraChange (CameraAttribute)
   ' Camera changes are sent on a timer. Set a flag so that we know if
   ' the camera has changed. The camera can then be sent to the console
   ' on the BSMTimerEvents_OnTimer event.
   bCameraChanged = True
End Sub

' -------------------------------------------------------------------- 
Sub BSMScriptEvents_OnTagChange (Tag, Object, Data) 
    Select Case Tag 
        Case TestFireEventTag 
            If Object.IDType = Database.otEntity Then 
                ' Event is a 'RECEIVE' event 
                ' We need to determine if we have an attribute or command id 
                Select Case BSMScript.IDType (Data) 
                    Case Database.otAttribute 
                        TestFireReceiveEvent Object, BSMScript.Attribute (Data) 
                    Case Database.otCommand 
                        TestFireReceiveEvent Object, BSMScript.Command (Data) 
                End Select 
 
            ElseIf Object.IDType = Database.otEvent Then 
                ' Event is a 'SEND' event 
                ' Send the event to the console 
                TestFireSendEvent Object 
            End If 
    End Select 
End Sub 

' -----------------------------------------------------------------------------
Sub BSMScriptEvents_OnMessage (Message, Data)
   Dim Asset
   Select Case Message 
      Case ReloadAssetMsg 
         ' An asset has been reloaded. Fire an OnChange event for that asset 
         If BSMScript.IDType (Data) = Database.otAsset Then 
            BSMScriptEvents_OnChange BSMScript.Asset (Data) 
         End If 

      Case PreImportAssetMessage
         Set Asset = BSMScript.Asset (Data)
         If Not Asset Is Nothing Then
            PreImportAsset Asset
         End If

      Case PostImportAssetMessage
         Set Asset = BSMScript.Asset (Data)
         If Not Asset Is Nothing Then
            PostImportAsset Asset
         End If

      Case PostImportAssetsMessage
         Broadcast.PostImportAllAssets
   End Select
End Sub

' -----------------------------------------------------------------------------
Sub BSMTimerEvents_OnTimer ()
    ' Stop build being re-entrant whilst we're switching levels 
    Dim Connection, Make 
    If GlobalScript.g_bSwitchingFolders = False And _
       GlobalScript.gbDirectorsCameraEnabled And _
       bCameraChanged _
    Then
        For Each Connection In GlobalScript.RWSComms.Connections
            Set Make = Connection.GetParam (strMakeAssociation)
            Make.AddTask "CameraChange", Connection, BSMScript.Game.CameraAttribute
            Make.Build
        Next
    End If
    bCameraChanged = False
End Sub

' ----------------------------------------------------------------------------- 
Sub RWSUtils_OnDatabaseAttached (DatabaseID)
   ' Register the "test fire event" tag. 
   Set TestFireEventTag        = BSMScript.RegisterTag (Tags.conTestFireEvent)
   Set ActiveFolderTag         = BSMScript.RegisterTag (Tags.conActiveFolder)
   Set ReloadAssetMsg          = BSMScript.RegisterMessage (Messages.conReloadAsset)
   Set PreImportAssetMessage   = BSMScript.RegisterMessage (Messages.conPreImportAsset)
   Set PostImportAssetMessage  = BSMScript.RegisterMessage (Messages.conPostImportAsset)
   Set PostImportAssetsMessage = BSMScript.RegisterMessage (Messages.conPostImportAssets)

   ' Set up the BSMScript object
   If Not BSMScript.AttachToDatabase (DatabaseID) Then
       GlobalScript.DatabaseAttachedError "BSMScript"
   End If
   BSMScript.SetEventMask UnconnectedMask
End Sub 
 
' ----------------------------------------------------------------------------- 
Sub RWSUtils_OnDatabaseDetached () 
   Set TestFireEventTag        = Nothing 
   Set ActiveFolderTag         = Nothing 
   Set ReloadAssetMsg          = Nothing
   Set PreImportAssetMessage   = Nothing
   Set PostImportAssetMessage  = Nothing
   Set PostImportAssetsMessage = Nothing

   BSMScript.AttachToDatabase (0)
 
   ' Disconnect any connections 
   Dim Connection
   For Each Connection In GlobalScript.RWSComms.Connections 
      Disconnect Connection 
   Next 
End Sub

'------------------------------------------------------------------------------ 
Sub RWSComms_OnConnectionDisconnected (Connection) 
   ConnectionLost (Connection) 
End Sub 
 
'------------------------------------------------------------------------------ 
Sub RWSComms_OnConnectionLost (Connection)
   bIsActiveTarget = True
   ConnectionLost (Connection) 
End Sub

'------------------------------------------------------------------------------ 
Sub RWSComms_OnConnectionBufferReceived (Connection, Buffer) 
    Dim Command, Version, BuildNumber, DataSize, strMessage, strText, Index, msgText
    const cWarningText = "WARNING - "   ' MUST match string used in RWS_DEBUGSTREAM_WARN
    const cErrorText = "ERROR - "       ' MUST match string used in RWS_DEBUGSTREAM_ERR
    Command     = CLng (0) 
    Version     = CLng (0) 
    BuildNumber = CLng (0) 
    
    On Error Resume Next
    
      DataSize    = Buffer.ReadDataHeader (Command, Version, BuildNumber) 

      If Command = 0 Then ' 0 is strfunc_SendNamedData from connected target 
         strMessage = Buffer.ReadString () 
         If strMessage = "RWS::DebugMessage" Then
               strText = Buffer.ReadString () 
               If InStr(strText, cWarningText) <> 0 Then
                  msgText = Mid(strText, Len(cWarningText))
                  MessageLog.Warning strText 
               ElseIf InStr(strText, cErrorText) <> 0 Then
                  msgText = Mid(strText, Len(cErrorText))
                  MessageLog.Error strText 
               Else
                  MessageLog.Log strText 
               End If
         ElseIf strMessage = "RWS::SendTransform" Then 
               ' Camera or entity transform changed 
               Dim strGUID, Entity, strMatrix, MatrixElement

               strGUID = GetGuidFromBuffer (Buffer) 
               If Len (strGUID) > 0 Then
                  Set Entity = BSMScript.Entity (strGUID) 
                  If Not Entity Is Nothing Then
                     ' Handle entity frame update
                     For Index = 1 To 3 
                           MatrixElement = Buffer.Read (dtReal) 
                           MatrixElement = Buffer.Read (dtReal) 
                           MatrixElement = Buffer.Read (dtReal) 
                     Next 

                     strMatrix = "[GPM] Received Entity matrix update, UID: " & Entity.Name & " Pos = " 
                     strMatrix = strMatrix & CStr (Buffer.Read (dtReal)) & " " 
                     strMatrix = strMatrix & CStr (Buffer.Read (dtReal)) & " " 
                     strMatrix = strMatrix & CStr (Buffer.Read (dtReal)) 

                     MessageLog.Log strMatrix 
                  End If 
            Else 
                  ' Camera frame update 
                  Dim CameraData, CameraDataSection 
                  Set CameraData = BSMScript.Game.CameraAttribute.Data 

                  ' Camera data consists of 16 floats (RwMatrix) 
                  ' and a field of view 
                  CameraData.Clear 
                  Set CameraDataSection = CameraData.Add (dtReal) 
                  For Index = 1 To 4 
                     CameraDataSection.Add Buffer.Read (dtReal) 
                     CameraDataSection.Add Buffer.Read (dtReal) 
                     CameraDataSection.Add Buffer.Read (dtReal) 
                     CameraDataSection.Add 0.0 
                  Next 
                  CameraDataSection.Add 42.5 * 3.142 / 180.0 

                  ' Set the camera attribute data
                  BSMScript.Game.CameraAttribute.Data = CameraData
            End If 

         ' 
         ' Add any extra named network command handlers here 
         ' 
         End If 
      Else 
         ' 
         ' Add any custom network command handlers here - see strfunc.h in game framework code 
         ' 
      End If 
    
   On Error Goto 0    
   
End Sub 

' ---------------------------------------------------------------------
Sub RWSUtils_OnSetCameraID (CameraID)
    BSMScript.SetCameraID CameraID
End Sub

'------------------------------------------------------------------------------
' Events reporting the progress of the export.
' strMessage: The feedback message.
' strObject:  The name of the node being operated on (from the rf3).
' nSeverity:  The status level of the message. The higher the number, the less
'             important it is.
'             0: error, 1, 2, 3 & 4, information
' nVerbosity: The requested verbosity of messages
Sub RF3TranslatorEvents_Message (strObject, strMessage, nSeverity, nVerbosity)
   ' Ignore empty strings and strings only containing a space - we do get some!
   If Len (strMessage) = 0 Then
   ElseIf strMessage = " " Then
   Else
      Dim strOutput
      If Len (strObject) > 0 Then
         strOutput = "'" & strObject & "' "
      End If

      If nSeverity = 0 Then
         g_intNumBuildErrors = g_intNumBuildErrors + 1
         BuildLog.Error "[RF3 Translator] " & strOutput & strMessage
      Else
         BuildLog.Log "[RF3 Translator] " & strOutput & strMessage
      End If
   End If
End Sub

'------------------------------------------------------------------------------
' Helper functions
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
' Returns the asset (that references the rf3 file) that generated the Asset.
Function GetRootRF3Asset (Asset)
    If Asset.ParentAssets.Count = 0 Then
        Set GetRootRF3Asset = Asset
    Else
        Set GetRootRF3Asset = GetRootRF3Asset (Asset.ParentAssets (1))
    End If
End Function

'------------------------------------------------------------------------------ 
Function GetGUIDFromBuffer (Buffer) 
   Dim strGuid, GUIDArray (3) 
 
   GUIDArray (0) = Buffer.Read (dtInt32) 
   GUIDArray (1) = Buffer.Read (dtInt32) 
   GUIDArray (2) = Buffer.Read (dtInt32) 
   GUIDArray (3) = Buffer.Read (dtInt32) 
 
   If GUIDArray (0) = 0 And GUIDArray (1) = 0 And GUIDArray (2) = 0 And GUIDArray (3) = 0 Then 
      strGuid = "" 
   Else 
      ' Format into a guid 
      strGuid = BSMScript.ArrayToUID (GUIDArray) 
   End If 
 
   GetGUIDFromBuffer = strGuid 
End Function

'------------------------------------------------------------------------------  
Function CurrentlySelectedConnection (strTarget) 
    Set CurrentlySelectedConnection = Nothing  
    Dim ConnectionID 
    ConnectionID = CurrentlySelectedConnectionID (strTarget) 
    If ConnectionID > 0 Then
        Set CurrentlySelectedConnection = GlobalScript.RWSComms.Connection (ConnectionID) 
    End If  
End Function 
 
'------------------------------------------------------------------------------   
Function CurrentlySelectedConnectionID (strTarget) 
    Dim strID, ConnectionID
    strID = TargetLink.GetField (strTarget, "ID")   
   
    ConnectionID = 0 
    CurrentlySelectedConnectionID = 0 
    If Len (strID) > 0 Then   
        On Error Resume Next 
        ConnectionID = CLng (strID) 
        If Not Err.Number = 0 Then
            ConnectionID = 0
        End If
        On Error Goto 0 
        CurrentlySelectedConnectionID = ConnectionID 
    End If   
End Function

'------------------------------------------------------------------------------  
Function ExpandString (strTarget, LaunchString)  
    Dim Reg, Match 
    ExpandString = LaunchString  
    Set Reg = New RegExp  
    Reg.Pattern = "\$\([A-Za-z0-9_ ]*\)"  
  
    For Each Match In Reg.Execute (LaunchString)  
        ExpandString = ExpandString (strTarget, Reg.Replace (LaunchString, GetCustomValue (strTarget, Mid (Match.Value, 3, Match.Length - 3))))  
    Next  
End Function 

'------------------------------------------------------------------------------  
Function GetCustomValue (strTarget, CustomVariable)  
    GetCustomValue = TargetLink.GetField (strTarget, TargetContextMenu.RemoveWhiteSpace (CustomVariable)) 
End Function

' ------------------------------------------------------------------------------
' Saves the build activity log (XMLLog) to the location specified in
' strUniqueLogName, creating the directory if necessary.
Sub SaveLog (XMLLog, strUniqueLogName)
   Dim strBuildDir
   strBuildDir = Persist.GetParentFolderName (strUniqueLogName)
   If Persist.fso.FolderExists (strBuildDir) Then
      ' The directory already exists, use it.
   ElseIf GlobalScript.CreateDirectory (strBuildDir, Persist.fso) Then
      ' Directory is now created, use it
   Else
      strBuildDir = "C:\RWStudio Logs"
      If GlobalScript.CreateDirectory (strBuildDir, Persist.fso) Then
         ' Use the logs directory
      Else
         BuildLog.Log "Couldn't create the log for " & strUniqueLogName
         Exit Sub
      End If
   End If

   Dim file
   If Persist.fso.FileExists (strUniqueLogName) Then
      Set file = Persist.fso.GetFile (strUniqueLogName)
      If file.attributes and 1 Then '(1 = read only)
         BuildLog.Warning "The build activity log, " & strUniqueLogName & ", is read only. " & vbCR &_
                          "It can not be written to, so will contain an incorrect log of build activity. " &_
                          "Make it writable to re-enable logging."
         Exit Sub
      End If
   End If

   BuildLog.Log "[Target] Saving build activity Log to " & strUniqueLogName
   XMLLog.Save strUniqueLogName
End Sub

'-----------------------------------------------------------------------------------------
' Returns the location where assets should be exported to from an RF3 file.
Function GetRF3ExportsLocation (RootAsset, oMake)
   Dim strExportsLocation, strAssetRelativeDir, strRootDir

   ' If there isn't a resource root, use the drive name from the asset.
   strExportsLocation = oMake.GetParam ("WKSRESOURCEROOT")
   If Len (strExportsLocation) = 0 Then
      strExportsLocation = Persist.fso.GetDriveName (RootAsset.file) & "\"
      If len (strExportsLocation) = 0 Then
         BuildLog.Warning "There is no resource root and the asset isn't reference from " & _
                          "a drive letter. The export location will be incorrect."
      End If
   End If

   ' Add on the hard coded directory name and the template name.
   strExportsLocation = Persist.fso.BuildPath (strExportsLocation, "RWStudio RF3 Exports")
   strExportsLocation = Persist.fso.BuildPath (strExportsLocation, _
                              Persist.fso.GetBaseName (oMake.GetParam ("RF3PROJECTTEMPLATENAME")))

   ' Add on the (adjusted) relative path to the root asset followed by the asset's name.
   strAssetRelativeDir = Persist.fso.GetParentFolderName (RootAsset.file)
   strAssetRelativeDir = Replace (strAssetRelativeDir, "..", "dotdot")
   strAssetRelativeDir = Replace (strAssetRelativeDir, ":", "Drive")
   strExportsLocation  = Persist.fso.BuildPath (strExportsLocation, strAssetRelativeDir)
   strExportsLocation  = Persist.fso.BuildPath (strExportsLocation, Persist.fso.GetBaseName (RootAsset.file))

   ' Return with an ending slash.
   GetRF3ExportsLocation = strExportsLocation & "\"
End Function

'------------------------------------------------------------------------------
' Catch Broadcast Events
'
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------ 
Sub Broadcast_ConnectToTarget (nConnectionID)
   MainMenu.EnableMenuItem  416, True ' Reload Game
   MainMenu.EnableMenuItem  417, True ' Reset All Entities
   MainMenu.EnableMenuItem  418, True ' Directors Camera
   MainMenu.EnableMenuItem  419, True ' Pause Mode
   CurrentTargetToolbar.EnableMenuItem 9,  True ' Reset All Entities
   CurrentTargetToolbar.EnableMenuItem 10, True ' Pause Mode
   CurrentTargetToolbar.EnableMenuItem 11, True ' Directors Camera
   
   ' If connecting via context menu, only proceed if the selected connection is
   ' the Active target
   If Not bIsActiveTarget Then
        bIsActiveTarget = True
        Exit Sub
   End If
    
   Dim ConnectItem
   CurrentTargetToolbar.CheckMenuItem 2, True
   
   Set ConnectItem   = TgMenu.MenuItems ("Connect")
   ConnectItem.MenuName = "Disconnect"
   
   GlobalScript.EnableBuildOptions

   If Not strActiveTarget = "" Then
      If nConnectionID > 0 Then
         CurrentTargetToolbar.EnableMenuItem 8, True ' Sync Camera
         MainMenu.EnableMenuItem  420, True ' Reload Game
         MainMenu.EnableMenuItem  421, True ' Reset All Entities
         MainMenu.EnableMenuItem  422, True ' Sync Camera
         
         MainMenu.EnableMenuItem  406, False ' Clean
         MainMenu.EnableMenuItem  407, False ' Build
         MainMenu.EnableMenuItem  408, False ' Rebuild
         MainMenu.EnableMenuItem  409, False ' Clean All Folders
         MainMenu.EnableMenuItem  410, False ' Build All Folders
         MainMenu.EnableMenuItem  411, False ' Rebuild All Folders
         MainMenu.EnableMenuItem  413, False ' Build and Connect
         
         CurrentTargetToolbar.EnableMenuItem 3, False ' Build & Connect
         CurrentTargetToolbar.EnableMenuItem 4, False ' Clean
         CurrentTargetToolbar.EnableMenuItem 5, False ' Build
         CurrentTargetToolbar.EnableMenuItem 6, False ' Rebuild
      End If
   End If
   
   bIsActiveTarget = True
End Sub

' ------------------------------------------------------------------------------
Sub Broadcast_DisconnectFromTarget (nConnectionID)
   If GlobalScript.RWSComms.Connections.Count < 1 Then
      MainMenu.EnableMenuItem  416, False ' Reload Game
      MainMenu.EnableMenuItem  417, False ' Reset All Entities
      MainMenu.EnableMenuItem  418, False ' Directors Camera
      MainMenu.EnableMenuItem  419, False ' Pause Mode
      CurrentTargetToolbar.EnableMenuItem 9,  False ' Reset All Entities
      CurrentTargetToolbar.EnableMenuItem 10, False ' Pause Mode
      CurrentTargetToolbar.EnableMenuItem 11, False ' Directors Camera
   End If
   
   ' If Disconnecting via context menu, only proceed if the selected connection is
   ' the Active target
   If Not bIsActiveTarget Then
        bIsActiveTarget = True
        Exit Sub
   End If
   
   Dim ConnectItem
   CurrentTargetToolbar.CheckMenuItem 2, False
   
   Set ConnectItem   = TgMenu.MenuItems ("Connect")
   ConnectItem.MenuName = "Connect"
   
   GlobalScript.EnableBuildOptions
   
   CurrentTargetToolbar.EnableMenuItem 8, False ' Sync Camera
   MainMenu.EnableMenuItem  420, False ' Reload Game
   MainMenu.EnableMenuItem  421, False ' Reset All Entities
   MainMenu.EnableMenuItem  422, False ' Sync Camera
   
   bIsActiveTarget = True
End Sub

' ------------------------------------------------------------------------------
' Handle broadcast event to say active folder has changed
Sub Broadcast_ActiveFolderChanged (NewRootFolder)
   ActiveFolderID = NewRootFolder
End Sub
