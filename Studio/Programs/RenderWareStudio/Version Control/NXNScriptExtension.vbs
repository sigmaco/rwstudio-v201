'------------------------------------------------------------------------------
' NXNScriptExtension.vbs
'------------------------------------------------------------------------------

Option Explicit ' All variables must be explicitly declared

'------------------------------------------------------------------------------
' Global COM objects
'------------------------------------------------------------------------------
Dim oNXNScriptExtension
On Error Resume Next
Set oNXNScriptExtension = CreateObject ("CSL.RWSNxNExtensions.ScriptExtensions")
RENHost.ConnectObject oNXNScriptExtension, "oNXNScriptExtensionEvents"
On Error Goto 0

Dim oVCRWSScript
Set oVCRWSScript = CreateObject ("CSL.RWSScript.RWSScript")
RENHost.ConnectObject oVCRWSScript, "oVCRWSScriptEvents"

' Define bool which flags if project database requires reloading
Dim bReloadProject
bReloadProject = false

' Define array object to hold filenames of objects that require reload.
Dim ReloadList()
ReDim ReloadList(-1)

'------------------------------------------------------------------------------
Function AttachToDatabase (DatabaseID)
    If (DatabaseID = 0) Then
        AttachToDatabase = oVCRWSScript.AttachToDatabase (0)
    Else
        AttachToDatabase = oVCRWSScript.AttachToDatabase (DatabaseID)
        oVCRWSScript.SetEventMask Database.otAll
        oVCRWSScript.QueuedEvents = True
    End If
End Function 

' ---------------------------------------------------------------------
Sub Broadcast_PostLoadProject (strFilename)
    If VersionControl.IsProjectManaged () Then

        ' Tell user name of current (default) change set.
        Dim DefaultChangeSetName
        DefaultChangeSetName = VersionControl.GetDefaultChangeSetName()
    
        VersionControlLog.Log "Default change set is '" & DefaultChangeSetName & "'."
 
        ' To ask user if they want to change default change sets, uncomment this code.
'       Dim bManageChangeSets
'       bManageChangeSets = MsgBox ("Default change set is '" & _
'                                   DefaultChangeSetName & _
'                                   "'. Show alienbrain explorer to manage change sets?", _
'                                   vbYesNo + vbQuestion, "Version Control")
'       If bManageChangeSets = vbYes Then
'           VersionControl.ShowProjectInExplorer
'       End If

   End If
End Sub

' ---------------------------------------------------------------------
' Handle OnPermit event generated by RWSUtils.
' This will be called whenever an API objects is about to be changed
' for the first time. If bIsPermitted is set to False on the exit of this
' method then the edit of the API object will not be allowed.
Sub RWSUtils_OnPermit (ID1, ID2, Action, bIsPermitted)
   If BuildScriptManager.bImporting Then
      ' During the importing of assets (even while loading a game) premission may
      ' be needed to change data, e.g. RF3s adding children.
      bIsPermitted = VersionControl.OnPermit (ID1, ID2, Action)
   ElseIf GlobalScript.g_bLoading Then
      ' We're loading a game, always give permission.
      bIsPermitted = True
   Else
      ' Check version control for permission to edit.
   	bIsPermitted = VersionControl.OnPermit (ID1, ID2, Action)
   End If
End Sub

' ---------------------------------------------------------------------
Sub RWSUtils_OnCheckOutFile (strFilename)
   If VersionControl.IsPathManaged (strFilename) Then
      ' Only try to check out the file if it's managed, but not already checked out to user.
      If VersionControl.GetFileStatus (strFilename) > VersionControl.conStateLocal And _
         (Not VersionControl.GetFileStatus (strFilename) = VersionControl.conStateCheckedOutToUser) Then
         If VersionControl.CheckOutFile (strFilename, false, false) Then
            ' The file has been checked out, update the version control menu.
            GlobalScript.UpdateVersionControlMenu ()
         End If
      End If
   End If
End Sub

' ---------------------------------------------------------------------
Sub RWSUtils_OnImportToVersionControl (strFilename)
   If Persist.fso.FileExists (strFilename) Then
      If Not VersionControl.IsPathManaged (strFilename) Then
         If VersionControl.GetFileStatus (strFilename) = VersionControl.conStateLocal Then
            If VersionControl.ImportFile (strFilename) Then
               ' The file has been added to the changeset, update the version control menu.
               GlobalScript.UpdateVersionControlMenu ()
            End If
         End If
      End If
   End If
End Sub

'------------------------------------------------------------------------------
' Handle the event fired by the creation of a new API object
Sub oVCRWSScriptEvents_OnCreate (APIObject, bIsNewObject)
   If bIsNewObject Then
      If VersionControl.IsProjectManaged () Then
         ' Forward the event to VersionControl script
         VersionControl.OnCreateAPIObject (APIObject)
      End If
   End If
End Sub

' -----------------------------------------------------------------------------
' Indicates end of an operation. If in required state then must reload project/files
Sub PostNxNDatabaseChanged ()

   If UBound (ReloadList) > -1 Then
      ReloadModifiedFiles
   ElseIf bReloadProject Then
      Dim bReload
      bReload = vbYes
      If Not VersionControl.bImportingToVersionControl Then
         ' We're not importing so offer to reload database.
         bReload = MsgBox ("The local project database has changed and affected objects could not be loaded. Project must be reloaded. Do you wish to reload the project?", vbYesNo + vbQuestion, GlobalScript.g_strTitle)
      End If
      If bReload = vbYes Then
         Persist.ReloadProjectDatabase
      End If
   End If

   ' Clear the reload list and flags
   ReDim ReloadList (-1)
   bReloadProject = false
   VersionControl.bImportingToVersionControl = False
End Sub

'------------------------------------------------------------------------------
' Main NXN alienbrain event handler
' This will recieve namespace events for operations performed in Workspace or the NXN
' dialogs supplied as part of the NXN Windows Integration Framework
Sub oNXNScriptExtensionEvents_OnDatabaseChange (strEvent, oEventContainer)
' FOR DEBUG ONLY
'   DisplayVerboseEventInfo strEvent, oEventContainer
' FOR DEBUG ONLY
   Dim strLocalPath, strUID, oAPIObject

   Select Case strEvent
      ' Add all NXN messages to the Version Control Log for user information
      Case "NxNMessage" 
         VersionControlLog.Log GetProperty ("Message", oEventContainer)

      ' Pre/Post recursion events indicate when an operation is about to begin/end
      Case "Pre_Recursion" 
         ' About to start a recursive operation so clear reload list and flags
         bReloadProject = false
         ReDim ReloadList (-1)

      Case "Post_Recursion" 
         If Not GetProperty ("OpCode", oEventContainer) = "SubmitPendingChange" Then
            PostNxNDatabaseChanged
         End If

      Case "Post_CheckIn"
         PostNxNDatabaseChanged

      ' Look for events which will result in local PROJECT files possibly being modified
      Case "Post_CheckOut"
         strLocalPath = GetProperty ("CheckOutPath", oEventContainer)
         VersionControl.OutputToMessageLogs "Check out successful", strLocalPath, VersionControlLog.conInformation
         ' If during call to OnPermit then reload is not required as check out
         ' is only permitted there if user has head revision.
         If Not VersionControl.bInOnPermit Then
            ' Need to reload as manual check out may have synced us to head revision
            AddToReloadList strLocalPath
         End If

      Case "Post_GetLatest"
         strLocalPath = GetProperty ("LocalPath", oEventContainer)
         VersionControl.OutputToMessageLogs "Get latest version successful", strLocalPath, VersionControlLog.conInformation
         ' We only get this event if a local file has been touched
         AddToReloadList strLocalPath
 
      Case "Post_GetVersion"
         strLocalPath = GetProperty ("LocalPath", oEventContainer)
         VersionControl.OutputToMessageLogs "Get version successful", strLocalPath, VersionControlLog.conInformation
         ' We only get this event if a local file has been touched
         AddToReloadList strLocalPath
 
      Case "Post_SubmitPendingChange"
         strLocalPath = GetProperty ("LocalPath", oEventContainer)
         VersionControl.OutputToMessageLogs "Submit pending changes successful", strLocalPath, VersionControlLog.conInformation
         If Not VersionControl.bImportingToVersionControl Then
            ' We're not importing so offer to reload file as user may have resolved multiple edits
            AddToReloadList strLocalPath
         Else
            ' We are importing, set the status of the object from the imported arrow to managed.
            strUID = Persist.GetUIDFromFilename (strLocalPath)
            If Len (strUID) > 0 Then
               Set oAPIObject = Database.GetAPIObjectFromUID (strUID)
               ' The object may be Nothing if the object was deleted, but still had a file.
               If Not oAPIObject Is Nothing Then
                  VersionControl.GetPersistStatusInformation (oAPIObject.ID)
               End If
            End If
         End If
         ' Update NXN properties
         SetNXNProperties strLocalPath

      Case "Post_UndoPendingChange"
         strLocalPath = GetProperty ("LocalPath", oEventContainer)
         VersionControl.OutputToMessageLogs "Undo pending changes successful", strLocalPath, VersionControlLog.conInformation
         ' Reload file as local file may have been modified
         AddToReloadList strLocalPath
         
      Case "Post_DeleteLocalCopy"
         ' If file may be related to the loaded project's XML then reload
         strLocalPath = GetProperty ("LocalPath", oEventContainer)
         strUID = Persist.GetUIDFromFilename (strLocalPath)
         If Len (strUID) > 0 Then
            bReloadProject = True
         End If
         
      Case "Post_Delete"
         ' If file may be related to the loaded project's XML then reload
         strLocalPath = GetProperty ("LocalPath", oEventContainer)
         strUID = Persist.GetUIDFromFilename (strLocalPath)
         If Len (strUID) > 0 Then
            bReloadProject = True
         End If

      Case "Post_Import"
         ' Do nothing
   End Select
End Sub

'------------------------------------------------------------------------------
' Set the NXN properties on a file
' This will set the properties to be whatever the user 
' currently has in memory. If the name, type or description has 
' been changed as a result of a merge then this users data will override.
Sub SetNXNProperties (strFilename)
   Dim oAPIObject
   Set oAPIObject = Persist.GetAPIObjectFromFilename (strFilename)
   If Not oAPIObject Is Nothing Then
      VersionControl.SetNXNProperties (oAPIObject)
   End If
End Sub

'------------------------------------------------------------------------------
' Display verbose event information in the version control log
Sub DisplayVerboseEventInfo (strEvent, oEventContainer)
   ' For debug, list all messages in the build log for now.
   Dim oEventProp, strEventProperties

   For Each oEventProp In oEventContainer
      strEventProperties = strEventProperties & Chr(13) & "                      " & _
                                                oEventProp.Name & " = " & oEventProp.Value
   Next

   VersionControlLog.Log "[NXN Event] " & strEvent & _
                         ", has " & oEventContainer.Count & " properties :" & _
                         strEventProperties
End Sub

'------------------------------------------------------------------------------
' Add the given filename to the array of files to reload
Sub AddToReloadList (strFilename)
   Dim strUID
   strUID = Persist.GetUIDFromFilename (strFilename)
   If Len (strUID) > 0 Then
      ReDim Preserve ReloadList (UBound (ReloadList) + 1)
      ReloadList (UBound (ReloadList)) = strUID
   End If
End Sub

'------------------------------------------------------------------------------
' Reload each of the files listed in the array
Sub ReloadModifiedFiles
   Dim bReloadWholeProject
   bReloadWholeProject = false
   Dim i
   For i = 0 to UBound (ReloadList)
      If Not ReloadList (i) = "" Then
         Dim oAPIObject
         ' If the object did not exist before, the next call will load
         ' it due to the "on demand" loading code.
         Set oAPIObject = Database.GetAPIObjectFromUID (ReloadList (i))
         If Not oAPIObject Is Nothing Then
            If oAPIObject.IDType = Database.otGame Then
               MessageLog.Warning "[Persistence] Game object file has been modified, must reload project"
               bReloadWholeProject = true
               Exit For
            Else
               MessageLog.Log "[Persistence] Reloading '" & oAPIObject.PersistentFilename & "'"

               ' Clear the undo stack, since undo operations will no longer be valid
               GlobalScript.Undo.Clear

               If oVCRWSScript.Load (ReloadList (i), oAPIObject.IDType) = 0 Then
                 ' Load failed so reload the whole database
                  MessageLog.Error "[Persistence] Failed to load '" & oAPIObject.PersistentFilename & "'"
                  bReloadWholeProject = true
                  Exit For
               End If 
            End If
            ' Re-get the API object as may have been invalidated by reload
            Set oAPIObject = Database.GetAPIObjectFromUID (ReloadList (i))
            If Not oAPIObject Is Nothing Then
               VersionControl.GetPersistStatusInformation (oAPIObject.ID)
            End If
         End If
      End If
   Next
   
   ' If something failed above then we reload the project just in case
   If bReloadWholeProject Then
      Dim bReload
      bReload = MsgBox ("The local project database has changed and affected objects could not be loaded. Project must be reloaded. Do you wish to reload the project?", vbYesNo + vbQuestion, GlobalScript.g_strTitle)
      If bReload = vbYes Then
         Persist.ReloadProjectDatabase
      End If
   End If
End Sub

'------------------------------------------------------------------------------
' Get the named property from an event
Function GetProperty (strPropertyName, oEventContainer)
   Dim oEventProp
   For Each oEventProp In oEventContainer
      If oEventProp.Name = strPropertyName Then
         GetProperty = oEventProp.Value
         Exit Function
      End If
   Next     
   
   GetProperty = ""
End Function