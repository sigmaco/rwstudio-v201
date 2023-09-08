
' ------------------------------------------------------------------------------
' RF3Tools.vbs
'
' This script contains functions used for generating resources from an RF3 file.

Option Explicit ' All variables must be explicitly declared (with Dim)

' The RF3 task-specific parameter names. Use: Task.GetParam (<conName>)
const conRootAssetParam  = "RF3ROOTASSET"
const conAssetInfosParam = "RF3ASSETINFOS"
const conAssetCount      = "AssetCount-" ' Append the child asset exported filename to this

' The RF3 export details XML file node names
const conNumAssets       = "NumberOfGeneratedAssets"

'-----------------------------------------------------------------------------------------
' Returns the filename that should be used for the asset.
' If the asset resource was contained within the exported file, then a mangled filename
' will be returned (RootAsset indexed with nAssetCount).
' Returns an empty string if there isn't a filename to return.
Function GetResourceFilename (Task, strExportedFilename, AssetInfo)
   Dim nNumGeneratedAssets, nAssetCount, ExportFilenames
   nNumGeneratedAssets         = GetNumGeneratedAssets (Task, strExportedFilename)
   ExportFilenames             = AssetInfo.ExportFileNames

   ' Get the number of assets that are contained in the exported file.
   If (UBound (ExportFilenames) - LBound (ExportFilenames)) + 1 = 1 Then
      On Error Resume Next
      nAssetCount = Task.GetParam (conAssetCount & ExportFilenames (0))
      If Err.Number <> 0 Then
         nAssetCount = -1
      End If
      On Error Goto 0
   Else
      nAssetCount = -1
   End If

   ' Check if there was a chunk file created for this asset.
   If nNumGeneratedAssets = -1 Then
      ' There hasn't a file generated on disk yet that stores the number of assets.
      GetResourceFilename = ""
   ElseIf nNumGeneratedAssets = 0 Then
      ' No files have been generated from the split process, use the exported file.
      GetResourceFilename = strExportedFilename
   ElseIf nAssetCount > nNumGeneratedAssets Then
      ' Not enough files have been generated from the split process, warn the
      ' user. There must be something wrong with the exporter.
      BuildWarning "Asset number " & nAssetCount & ", " & AssetInfo.Name & ", won't have its own " & _
                   "file, only " & nNumGeneratedAssets & " files were generated."
   Else
      ' Enough files have been generated from the split process, assign the
      ' appropriate file.
      GetResourceFilename = GetConcatFilename (strExportedFilename, nAssetCount)
   End If
End Function

'-----------------------------------------------------------------------------------------
' Reads the local file that stores the number of files RWSComms generates from the
' original exported file.
' Returns the number in the file or -1 if the file exists or contains nothing.
Function GetNumGeneratedAssets (Task, strExportedFilename)
   Dim XMLDoc, RootNode, FileNode, strNumAssets, strFindFilename
   Set RootNode = GetLocalFileRootNode (Task, XMLDoc)

   GetNumGeneratedAssets = -1
   If Not RootNode Is Nothing Then
      ' Find the File node. Convert all slashes to double slashes, required for XPath.
      strFindFilename = Replace (strExportedFilename, "\" , "\\")
      Set FileNode = RootNode.SelectSingleNode ("./File[.='" & strFindFilename & "']")
      If Not FileNode Is nothing Then
         ' Read the attribute from the file and convert to an int, this is the number of
         ' assets that will have been split from the main rws file.
         strNumAssets = FileNode.GetAttribute (conNumAssets)
         If Len (strNumAssets) > 0 Then
            If IsNumeric (strNumAssets) Then
               GetNumGeneratedAssets = CInt (strNumAssets)
            End If
         End If
      End If
   End If
End Function

'------------------------------------------------------------------------------
' Will return the root node of the local file that contains details about the
' export. The return of this function should be checked against "Nothing". If
' Nothing is returned, you cannot read from or write to the XML doc.
Function GetLocalFileRootNode (Task, XMLDoc)
   Dim RootNode
   Set XMLDoc = CreateObject ("Msxml2.DOMDocument.3.0")
   XMLDoc.Async     = False
   Set RootNode     = Nothing ' Initialize to an empty object
   If XMLDoc.Load (GetRF3ExportDetailsFilename (Task)) Then
      Set RootNode = XMLDoc.SelectSingleNode ("/RF3Export")
   End If
   If RootNode Is Nothing Then
         Set RootNode = XMLDoc.CreateElement ("RF3Export")
         XMLDoc.AppendChild RootNode
   End If
   Set GetLocalFileRootNode = RootNode
End Function

'-----------------------------------------------------------------------------------------
' Returns the location where assets should be exported to from an RF3 file.
Function GetExportsLocation (Task)
   Dim RootAsset, strExportsLocation, strAssetRelativeDir, strRootDir
   Set RootAsset = Task.GetParam (conRootAssetParam)

   ' If there isn't a resource root, use the drive name from the asset.
   strExportsLocation = GetParam ("WKSRESOURCEROOT")
   If Len (strExportsLocation) = 0 Then
      strExportsLocation = FileSystem.GetDriveName (RootAsset.file) & "\"
      If len (strExportsLocation) = 0 Then
         BuildWarning "There is no resource root and the asset isn't reference from " & _
                        "a drive letter. The export location will be incorrect."
      End If
   End If

   ' Add on the hard coded directory name and the template name.
   strExportsLocation = BuildPath (strExportsLocation, "RWStudio RF3 Exports")
   strExportsLocation = BuildPath (strExportsLocation, _
                              FileSystem.GetBaseName (GetParam ("RF3PROJECTTEMPLATENAME")))

   ' Add on the (adjusted) relative path to the root asset followed by the asset's name.
   strAssetRelativeDir = FileSystem.GetParentFolderName (RootAsset.file)
   strAssetRelativeDir = Replace (strAssetRelativeDir, "..", "dotdot")
   strAssetRelativeDir = Replace (strAssetRelativeDir, ":", "Drive")
   strExportsLocation  = BuildPath (strExportsLocation, strAssetRelativeDir)
   strExportsLocation  = BuildPath (strExportsLocation, FileSystem.GetBaseName (RootAsset.file))

   ' Return with an ending slash.
   GetExportsLocation = strExportsLocation & "\"
End Function

'------------------------------------------------------------------------------
' Returns where on disk to find the file that contains details that will force
' a re-export of the RF3 file.
' ALWAYS check if the details are in the file before writing to it. Writing to
' this file will force a re-export of the RF3 file.
Function GetRF3ExportDetailsFilename (Task)
   GetRF3ExportDetailsFilename = BuildPath (GetExportsLocation (Task), _
                                             "RF3ExportDetails.xml")
End Function

'------------------------------------------------------------------------------
' Increments the count of the number of assets that are exported to each
' exported file.
Sub IncrementAssetCount (Task, AssetInfo)
   ' Increment the count of the number of assets using it.
   Dim nAssetCount, ExportFilenames
   ExportFilenames = AssetInfo.ExportFileNames
   If ((UBound (ExportFilenames) - LBound (ExportFilenames)) + 1) = 1 Then
      On Error Resume Next
      nAssetCount = Task.GetParam (conAssetCount & ExportFilenames (0))
      If Err.Number <> 0 Then
         nAssetCount = 0
      End If
      On Error Goto 0
      Task.SetParam conAssetCount & ExportFilenames (0), nAssetCount + 1
   End If
End Sub
