' Enterprise Layout Management Script
'
' Layout object 'DlgImport'
'

Dim GUIDArray ()
Dim NameArray ()
Dim numLevels
Dim g_strProjectFilename

Function GetRootFolderName (fso, strFolderDir, strGUID)
    Dim strName, strXMLFile

    strName = ""
    strXMLFile = fso.BuildPath (strFolderDir, strGUID & ".xml")

    Dim XMLDoc
    Set XMLDoc = CreateObject ("Msxml2.DOMDocument.3.0")
    XMLDoc.Async = False

    ' Get the name from the folder's XML file
    If XMLDoc.Load (strXMLFile) Then
        Dim NameNode
        Set NameNode = XMLDoc.SelectSingleNode ("/RenderWareStudio/Folder/Name")
        If Not NameNode Is Nothing Then
            strName = NameNode.Text
        End If
    End If

    GetRootFolderName = strName
End Function

Sub GetRootFolders (fso, strGameDir, strFolderDir)
    numLevels = 0

    ' Clear the arrays
    ReDim GUIDArray (-1)
    ReDim NameArray (-1)

    ' Find the first valid XML file in the game folder
    If fso.FolderExists (strGameDir) Then

        Dim Folder, File
        Set Folder = fso.GetFolder (strGameDir)
        For Each File In Folder.Files
            Dim XMLDoc
            Set XMLDoc = CreateObject ("Msxml2.DOMDocument.3.0")
            XMLDoc.Async = False

            ' Loop over all child folders in the folder XML file
            If XMLDoc.Load (File.Path) Then

                Dim ChildNodes, ChildNode
                Set ChildNodes = XMLDoc.SelectNodes ("/RenderWareStudio/Game/ChildReference")
                If ChildNodes.Length > 0 Then

                    For Each ChildNode In ChildNodes
                        If ChildNode.GetAttribute ("type") = "RWSFolderID" Then
                            ' Get the folder object's guid and name
                            Dim strName, strGUID
                            strGUID = ChildNode.GetAttribute ("id")

                            ' Add name and guid to arrays
                            If Len (strGUID) > 0 Then
                                numLevels = numLevels + 1

                                ' Allocate another array element
                                ReDim Preserve GUIDArray (UBound (GUIDArray) + 1)
                                ReDim Preserve NameArray (UBound (NameArray) + 1)

                                ' Set array elements
                                GUIDArray (UBound (GUIDArray)) = strGUID
                                strName = GetRootFolderName (fso, strFolderDir, strGUID)
                                NameArray (UBound (NameArray)) = strName
                            End If
                        End If
                    Next

                    Exit For
                End If
            End If
        Next
    End If
End Sub

Sub Initialize (strProjectFilename)

    g_strProjectFilename = strProjectFilename

    ' Open the project file to find the game location
    Dim XMLDoc
    Set XMLDoc = CreateObject ("Msxml2.DOMDocument.3.0")
    XMLDoc.Async = False

    If XMLDoc.Load (strProjectFilename) Then

        ' Remove file from path and append game folder
        Dim fso, strGameDir, strFolderDir, strDir, XMLNode
        Set fso = CreateObject ("Scripting.FileSystemObject")

        strGameDir = ""
        Set XMLNode = XMLDoc.SelectSingleNode ("/RenderWareStudio/GamesLocation")
        If Not XMLNode Is Nothing Then
            strGameDir = XMLNode.GetAttribute ("path")
        End If

        strFolderDir = ""
        Set XMLNode = XMLDoc.SelectSingleNode ("/RenderWareStudio/FoldersLocation")
        If Not XMLNode Is Nothing Then
            strFolderDir = XMLNode.GetAttribute ("path")
        End If

        strDir       = fso.GetParentFolderName (fso.GetAbsolutePathName (strProjectFilename))
        strGameDir   = fso.BuildPath (strDir, strGameDir)
        strFolderDir = fso.BuildPath (strDir, strFolderDir)

        ' Populate the arrays of folder names and guids
        GetRootFolders fso, strGameDir, strFolderDir
    End If

    LevelsList.clear()

    Dim n
    If numLevels > 0 Then
        For n = 0 To numLevels-1
            LevelsList.AddItem NameArray(n) '& " (UID: " GUIDArray(n) & ")"
        Next
    End If

    ImportButton.Enabled = False

    CheckBox_ImportTemplates.Value = False

End Sub

Sub ImportButton_Click (  )
    ' Import the levels
    For i = 0 To LevelsList.ListCount-1
        If (LevelsList.Selected (i) = True) Then
            If RWSUtils.ImportFolder (g_strProjectFilename, GUIDArray (i), "folder, '" + NameArray (i) + "'") Then
                MessageLog.Log "[Import] Imported: " & NameArray(i)
            Else
                MessageLog.Warning "[Import] Failed to import: " & NameArray(i)
            End If
        End If
    Next

    ' Import the templates if required
    If CheckBox_ImportTemplates.Value = True Then
        If RWSUtils.ImportFolder (g_strProjectFilename, "RWS_TemplateHierarchyFolder", "templates") Then
            MessageLog.Log "[Import] Imported: Templates"
        Else
            MessageLog.Warning "[Import] Failed to import: Templates"
        End If
    End If

    DlgImport.StopModal True
End Sub


Sub CancelButton_Click (  )
    DlgImport.StopModal False
End Sub


Sub LevelsList_OnChange (  )
    CheckIfCanImport
End Sub


Sub CheckBox_ImportTemplates_Change (  )
    CheckIfCanImport
End Sub


' Enable/disable the import button based on whether there are
' any levels selected for import or if the templates need importing.
Sub CheckIfCanImport ()
    If LevelsList.SelectedCount > 0 OR CheckBox_ImportTemplates.Value = True Then
        ImportButton.Enabled = True
    Else
        ImportButton.Enabled = False
    End If
End Sub
