'
' LightMapEditor.vbs
'
' Holds the lightMapEditor object that performs the preview and generation
' of light maps in RenderWare Studio.

Option Explicit ' All variables must be explicitly declared

' --------------------------------------------------------------------
' Create objects required by this script.
Dim oLightmapEditor
Set oLightmapEditor = CreateObject ("CSL.RWSLightmapEditor.RWSLightmapEditorCtrl")
RENHost.ConnectObjectEx oLightmapEditor, LightmapEditor, "LightmapEditorEvents"

' --------------------------------------------------------------------
' LtMapEditor version control status values
Const vcfs_Unknown      = &H00000000
Const vcfs_Managed      = &H00000001
Const vcfs_CheckedOut   = &H00000002
Const vcfs_Import       = &H00000004
Const vcfs_Error        = &H01000000

' =============================================================================
Dim oLMSelection
Set oLMSelection = CreateObject("CSL.RWSSelection.Selection")
RENHost.ConnectObject oLMSelection, "LMSelection"

' ------------------------------------------------------------------------------
' Catch broadcast events
' ------------------------------------------------------------------------------

' ------------------------------------------------------------------------------
' Handle broadcast message to load settings.
Sub Broadcast_LoadApplicationSettings (XMLRootNode)
    oLightmapEditor.Load XMLRootNode.SelectSingleNode ("./LightMapEditor/RWSLightmapEditor")
End Sub

' ------------------------------------------------------------------------------
' Handle broadcast message to save settings.
Sub Broadcast_SaveApplicationSettings (XMLDoc, XMLRootNode)
    Dim RootScriptElement
    Set RootScriptElement = XMLRootNode.SelectSingleNode ("./LightMapEditor")
    If RootScriptElement Is Nothing Then
        Set RootScriptElement = XMLDoc.CreateElement ("LightMapEditor")
        XMLRootNode.AppendChild RootScriptElement
    End If
    
    If Not RootScriptElement Is Nothing Then
      ' Now empty child nodes
      Dim ChildElement
      For Each ChildElement In RootScriptElement.ChildNodes
            RootScriptElement.RemoveChild ChildElement
      Next

      Dim LightmapElement
      Set LightmapElement = XMLDoc.CreateElement ("RWSLightmapEditor")
      RootScriptElement.AppendChild LightmapElement

      If Not LightmapElement Is Nothing Then
         ' Save data
         oLightmapEditor.Save XMLDoc, LightmapElement
      End If
   End If
End Sub

' ------------------------------------------------------------------------------
' Handle broadcast event after source has been parsed to create a fake light 
' behavior. 
Sub Broadcast_PostParseSource
    CreateLightMapperToolBehavior ()
End Sub

' ------------------------------------------------------------------------------
' Convert VersionControl status constant tp LtMapeditor status constant.
Function convertStatucVCtoLtMapVC( status )
	convertStatucVCtoLtMapVC = vcfs_Error
	Select Case status
		Case VersionControl.conStateUnknown 
			convertStatucVCtoLtMapVC = vcfs_Unknown

		Case VersionControl.conStateNotSaved 
			convertStatucVCtoLtMapVC = vcfs_Unknown

		Case VersionControl.conStateLocal 
			convertStatucVCtoLtMapVC = vcfs_Unknown

		Case VersionControl.conStatePendingImport 
			convertStatucVCtoLtMapVC = vcfs_Import

		Case VersionControl.conStateManaged
			convertStatucVCtoLtMapVC = vcfs_Managed

		Case VersionControl.conStateCheckedOutToUser 
			convertStatucVCtoLtMapVC = vcfs_CheckedOut

		Case VersionControl.conStateCheckedOutToOtherUser
			convertStatucVCtoLtMapVC = vcfs_Error

	End Select
End Function


' ------------------------------------------------------------------------------
' Lightmap events : 
' ------------------------------------------------------------------------------
Sub LightmapEditorEvents_OnOutputMessage ( Severity, Message ) 
    MessageLog.Message Severity, "[Lightmap Editor] " & Message 
End Sub

' --------------------------------------------------------------------
' LtMapEditor needs version control file status
Sub LightmapEditorEvents_OnVCFileGetStatus( filename )
	' get the status through VersionControl
	oLightmapEditor.VersionControlAssetStatus = convertStatucVCtoLtMapVC( VersionControl.GetFileStatus( filename ) )
	' if the file is not managed, let's have a pick at it's folder, as if the folder is managed but not the file
	' we will want to import it.
	If( oLightmapEditor.VersionControlAssetStatus = vcfs_Unknown ) Then 
		Dim fso
		Set fso = CreateObject ("Scripting.FileSystemObject")
		If( TRUE = VersionControl.IsPathManaged(fso.GetParentFolderName(Filename)) ) Then
			oLightmapEditor.VersionControlAssetStatus = vcfs_Managed
		End If
	End If
End Sub

' --------------------------------------------------------------------
' LtMapEditor needs version control asset status
Sub LightmapEditorEvents_OnVCAssetGetStatus( rwsid )
	oLightmapEditor.VersionControlAssetStatus = vcfs_Error
	If GlobalScript.RWSScript.IDType(rwsid) = Database.otAsset Then
      oLightmapEditor.VersionControlAssetStatus = convertStatucVCtoLtMapVC( VersionControl.GetResourceStatusInformation(rwsid) )
   End If
End Sub

' --------------------------------------------------------------------
' LtMapEditor needs to import a file
Sub LightmapEditorEvents_OnVCFileImport( filename )
	oLightmapEditor.VersionControlResult = FALSE
	
	If( TRUE = VersionControl.IsInstalled() ) Then
		If( TRUE = VersionControl.IsProjectManaged() ) Then
			Dim fso
			Set fso = CreateObject ("Scripting.FileSystemObject")
			If( TRUE = VersionControl.IsPathManaged(fso.GetParentFolderName(filename)) ) Then
				oLightmapEditor.VersionControlResult = VersionControl.ImportFile (filename)
			End If
		End If
	End If
End Sub

' --------------------------------------------------------------------
' LtMapEditor needs to import an asset
Sub LightmapEditorEvents_OnVCAssetImport( rwsid )
	oLightmapEditor.VersionControlResult = FALSE
	' No Go : we shale not create any asset : just modify them !!
End Sub

' --------------------------------------------------------------------
' LtMapEditor needs to check out a file
Sub LightmapEditorEvents_OnVCFileCheckOut( filename )
	oLightmapEditor.VersionControlResult = VersionControl.CheckOutFile(filename,TRUE,FALSE)
End Sub

' --------------------------------------------------------------------
' LtMapEditor needs to check out an asset
Sub LightmapEditorEvents_OnVCAssetCheckOut( rwsid )
	oLightmapEditor.VersionControlResult = VersionControl.OnPermit (rwsid, 0, VersionControl.conActionChange)
End Sub


' ------------------------------------------------------------------------------
' Handle global events.
' ------------------------------------------------------------------------------

' ------------------------------------------------------------------------------
' Handle resource root being set
' Note: This method may be called prior to the 
' RWSUtils_OnDatabaseAttach event.
Sub RWSUtils_OnResourceRootChanged (strResourceRoot)
    LightMapEditor.oLightmapEditor.SetResourceRoot (strResourceRoot)
End Sub   

' ---------------------------------------------------------------------
Sub RWSUtils_OnSetActiveFolder (FolderID)
    LightMapEditor.oLightmapEditor.ActiveFolderID = FolderID
End Sub

' --------------------------------------------------------------------
Sub RWSUtils_OnDatabaseAttached (DatabaseID)
    If Not LightMapEditor.oLightmapEditor.AttachToDatabase (DatabaseID) Then
        GlobalScript.DatabaseAttachedError "LightmapEditor"
    End If
    If Not LightmapAssetEditor.AttachToDatabase (DatabaseID) Then 
        GlobalScript.DatabaseAttachedError "LightmapAssetEditor" 
    End If 
    If Not LightmapSceneEditor.AttachToDatabase (DatabaseID) Then  
        GlobalScript.DatabaseAttachedError "LightmapSceneEditor"  
    End If  
End Sub

' --------------------------------------------------------------------
Sub RWSUtils_OnDatabaseDetached ()
   LightMapEditor.oLightmapEditor.AttachToDatabase (0)
   LightmapAssetEditor.AttachToDatabase (0)
   LightmapSceneEditor.AttachToDatabase (0)
End Sub

' ------------------------------------------------------------------------------
' Light map processuing
' ------------------------------------------------------------------------------

' ------------------------------------------------------------------------------   
'  Behavior for light mapping tool
Sub CreateLightMapperToolBehavior () 

   Dim lightMapClass, paramListArray
   Dim lightColourCommand, lightRadiusCommand, lightTypeCommand
   Dim lightFlagsCommand, coneAngleCommand, lightJitterCommand 

   Set lightMapClass = RWSScript.CreateBehavior ("LightMap Light")

   lightMapClass.Creatable = True

   ' Set up some commands
   ReDim paramListArray(5)

   ' Light colour
   Set lightColourCommand = RWSScript.CreateCommand ()
   paramListArray(0) = "CMD_Set_t_color" 
   paramListArray(1) = Chr(34) & "Color" & Chr(34)
   paramListArray(2) = Chr(34) & "Set light color" & Chr(34)
   paramListArray(3) = "COLOR"
   paramListArray(4) = "RwUInt32"
   paramListArray(5) = "DEFAULT(0xffffffff)"

   lightColourCommand.Name = "RWS_ATTRIBUTE"
   lightColourCommand.ParamList = paramListArray

   lightMapClass.AddChildCommand lightColourCommand

    ' Light radius
   Set lightRadiusCommand = RWSScript.CreateCommand ()
   paramListArray(0) = "CMD_Set_t_radius" 
   paramListArray(1) = Chr(34) & "Radius" & Chr(34)
   paramListArray(2) = Chr(34) & "Set light radius" & Chr(34)
   paramListArray(3) = "SLIDER"
   paramListArray(4) = "RwReal"

   ' This range is OK for the GP1 levels - alter to suit, as required
   paramListArray(5) = "RANGE(0, 10, 250)"

   lightRadiusCommand.Name = "RWS_ATTRIBUTE"
   lightRadiusCommand.ParamList = paramListArray

   lightMapClass.AddChildCommand lightRadiusCommand

    ' Light type - default is point
   Set lightTypeCommand = RWSScript.CreateCommand ()
   paramListArray(0) = "CMD_Set_t_Light_Type" 
   paramListArray(1) = Chr(34) & "Type" & Chr(34)
   paramListArray(2) = Chr(34) & "Set light type" & Chr(34)
   paramListArray(3) = "RADIO_BUTTON"
   paramListArray(4) = "RwUInt32"
   paramListArray(5) = "LIST(" & Chr(34) & "rpLIGHTDIRECTIONAL|@rpLIGHTPOINT|rpLIGHTSPOT|rpLIGHTSPOTSOFT" & Chr(34) & ")"

   lightTypeCommand.Name = "RWS_ATTRIBUTE"
   lightTypeCommand.ParamList = paramListArray

   lightMapClass.AddChildCommand lightTypeCommand   

   ' Light flags - default is light atomics & worlds
   Set lightFlagsCommand = RWSScript.CreateCommand ()
   paramListArray(0) = "CMD_Set_t_flag" 
   paramListArray(1) = Chr(34) & "Apply Light to" & Chr(34)
   paramListArray(2) = Chr(34) & "Apply Light to atomics, worlds, or both" & Chr(34)
   paramListArray(3) = "RADIO_BUTTON"
   paramListArray(4) = "RwUInt32"
   paramListArray(5) = "LIST(" & Chr(34) & "@Atomics + Worlds|Atomics Only|Worlds Only" & Chr(34) & ")"

   lightFlagsCommand.Name = "RWS_ATTRIBUTE"
   lightFlagsCommand.ParamList = paramListArray

   lightMapClass.AddChildCommand lightFlagsCommand   

   ' Jitter flag - 
   Set lightJitterCommand = RWSScript.CreateCommand ()
   paramListArray(0) = "CMD_Set_t_lightjitter" 
   paramListArray(1) = Chr(34) & "Enable jitter" & Chr(34)
   paramListArray(2) = Chr(34) & "Enable jitter on this light" & Chr(34)
   paramListArray(3) = "BOOLEAN"
   paramListArray(4) = "RwUInt32"
   paramListArray(5) = "DEFAULT(0)"

   lightJitterCommand.Name = "RWS_ATTRIBUTE"
   lightJitterCommand.ParamList = paramListArray

   lightMapClass.AddChildCommand lightJitterCommand   

   ' Cone angle - 
   Set coneAngleCommand = RWSScript.CreateCommand ()
   paramListArray(0) = "CMD_Set_t_coneangle" 
   paramListArray(1) = Chr(34) & "Cone angle" & Chr(34)
   paramListArray(2) = Chr(34) & "Cone angle (for spot lights)" & Chr(34)
   paramListArray(3) = "SLIDER"
   paramListArray(4) = "RwReal"
   paramListArray(5) = "RANGE(0,30,90)"

   coneAngleCommand.Name = "RWS_ATTRIBUTE"
   coneAngleCommand.ParamList = paramListArray

   lightMapClass.AddChildCommand coneAngleCommand   

End Sub

' ------------------------------------------------------------------------------   
'  Light map 'aliases' - place geometry in the 3D view to show parameters

Sub OnLoad ()
    ' Without this, the add/Remove selection events don't work
    LMSelection.SelectionIdentifier = GlobalScript.g_strGlobalSelection
End Sub

' ------------------------------------------------------------------------------
'   Notify edit control of selection event  

Sub LMSelection_OnAddSelection (id)
   oLightmapEditor.AddSelection id  
	LightmapSceneEditor.AddSelection id  
	LightmapAssetEditor.AddSelection id  
End Sub

' ------------------------------------------------------------------------------
'   Notify edit control of deselection event  

Sub LMSelection_OnRemoveSelection (id)
   oLightmapEditor.RemoveSelection id
	LightmapSceneEditor.RemoveSelection id
	LightmapAssetEditor.RemoveSelection id
End Sub

' ------------------------------------------------------------------------------
'   Notify edit control of clear event  

Sub LMSelection_OnClearSelection ()
   oLightmapEditor.ClearSelection
	LightmapSceneEditor.ClearSelection
	LightmapAssetEditor.ClearSelection
End Sub
