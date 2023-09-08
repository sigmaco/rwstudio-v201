' Script to check for when an asset object is modified and so offer to import the 
' resource file to version control.
' ImportAsset.vbs
Option Explicit

Dim fso
Set fso = CreateObject ("Scripting.FileSystemObject")

Dim ImportAssetScript
Set ImportAssetScript = CreateObject ("CSL.RWSScript.RWSScript")
RENHost.ConnectObject ImportAssetScript, "ImportAssetScript"

Dim g_strResourceRoot
g_strResourceRoot = ""

' --------------------------------------------------------------------
Sub AttachToDatabase (DatabaseID)
    ImportAssetScript.AttachToDatabase (DatabaseID)
    If DatabaseID > 0 Then
       ImportAssetScript.SetEventMask Database.otAsset
       ImportAssetScript.QueuedEvents = True
    End If
End Sub

' --------------------------------------------------------------------
Sub ImportAssetScript_OnChange (APIObject)
   If (APIObject.IDType = Database.otAsset) Then
      Dim strFilename
      strFilename = fso.BuildPath (g_strResourceRoot, APIObject.File)
      strFilename = fso.GetAbsolutePathName (strFilename)

      ' See if ID already has a version control status property, if so ignore
      Dim PropertiesContainer, Property, bHasProperty
      bHasProperty = False
      Set PropertiesContainer = GlobalScript.RWSScript.Properties (APIObject)
      For Each Property In PropertiesContainer
         If Property.UID = Properties.conResourceStatus Then
            bHasProperty = True
            Exit For
         End If
      Next

      If Not bHasProperty Then
         If fso.FileExists (strFilename) Then
            ' Check if the project is managed
            If VersionControl.IsProjectManaged () Then
               ' Check if file is already managed
               If Not VersionControl.IsPathManaged (strFilename) Then
                  ' Check if the file is in a managed location
                  Dim strFolder
                  strFolder = Persist.GetParentFolderName (strFilename)
                  If VersionControl.IsPathManaged (strFolder) Then
                     ' Attempt to import file to version control. User will be able
                     ' to cancel any import using the provided dialogs.
                     VersionControl.ImportFile strFilename
                  End If
               End If
            End If
         End If
         ' Update the property describing the status of the resource
         VersionControl.GetResourceStatusInformation (APIObject.ID)
      End If
   End If
End Sub

' ---------------------------------------------------------------------
Sub SetResourceRoot (strResourceRoot)
    g_strResourceRoot = strResourceRoot
End Sub
