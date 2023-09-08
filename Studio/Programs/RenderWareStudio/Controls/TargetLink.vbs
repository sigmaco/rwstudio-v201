'
' Enterprise Layout Management Script
'
' Layout object 'Target Link'
'
'------------------------------------------------------------------------------
' Global COM objects
'------------------------------------------------------------------------------

Dim ConnectionDlg
Set ConnectionDlg = CreateObject ("CSL.RWSTarget.RWSConnectionDlg")


'------------------------------------------------------------------------------ 
Sub TargetLink_OnContextMenu (X, Y, strRowUID, strColUID)
    Dim hWnd
    hWnd = TargetLink.GetHandle ()
    If strColUID = "" Then
        TargetContextMenu.strSelectedTarget = ""
    Else
        TargetContextMenu.strSelectedTarget = strRowUID
    End If

    TargetContextMenu.DisplayContextMenu X, Y, hWnd
End Sub

'------------------------------------------------------------------------------
' This event is fired when a user double clicks on a column for a connection.
'    Name   - Shows the properties for the connection.
'    Status - Either connects to, or disconnects from, the connection.
Sub TargetLink_OnDoubleClick (X, Y, strRowUID, strColUID)
    TargetContextMenu.strSelectedTarget = strRowUID

    If strColUID = "Name" Then
        TargetContextMenu.OnProperties
    ElseIf strColUID = "Status" Then
        ' We should only connect or disconnect if we have an active folder.
        If TargetContextMenu.ActiveFolderID > 0 Then
            TargetContextMenu.OnConnect
        End If
    End If
End Sub

'------------------------------------------------------------------------------
' Helpers

'------------------------------------------------------------------------------
Sub DisplayTableData ()

   TargetLink.ShowColumn "Name"
   TargetLink.ShowColumn "Status"
   TargetLink.ShowColumn "Address"
   TargetLink.ShowColumn "Platform"
   TargetLink.ShowColumn "Target"
   TargetLink.ShowColumn "EmbedAssets"
   TargetLink.ShowColumn "Resources"
   TargetLink.ShowColumn "Timeout"
   TargetLink.ShowColumn "Port"
   TargetLink.ShowColumn "TargetRF3Template"

   ' If the row begins with "Connection", then make it visible
   strConnection = "Connection"
   For Each RowUID In TargetLink.GetRowUIDs
      If strConnection = Left (RowUID, Len (strConnection)) Then

         TargetLink.ShowRow RowUID

         ' Don't forget to set the icon depending on the platform
         TargetLink.SetRowIcon RowUID, TargetContextMenu.GetIconFile (TargetLink.GetField (RowUID, "Platform"))

      End If
   Next

End Sub

'------------------------------------------------------------------------------
Sub CreateColumnData ()

   ' --- Populate table view ---
   ' Load the connection table contents
   TargetLink.AddColumn "Name",        "Name"
   TargetLink.AddColumn "Address",     "Address"
   TargetLink.AddColumn "Port",        "Port"
   TargetLink.AddColumn "Status",      "Status"
   TargetLink.AddColumn "Platform",    "Platform"
   TargetLink.AddColumn "Target",      "Target"
   TargetLink.AddColumn "Resources",   "Resources"
   TargetLink.AddColumn "Timeout",     "Timeout (ms)"
   TargetLink.AddColumn "ID",          ""
   TargetLink.AddColumn "EmbedAssets", "Embed Assets"
   TargetLink.AddColumn "TargetRF3Template", "Target RF3 Template"

   Dim NumCustomVals, CustomColUID, CustomColName
   NumCustomVals = ConnectionDlg.NumberOfUniqueCustomValues

   For Index = 1 To NumCustomVals
    CustomColUID = ConnectionDlg.CustomColUIDByIndex (Index - 1)
    CustomColName = TargetContextMenu.RemoveWhiteSpace (CustomColUID)
    TargetLink.AddColumn CustomColName, CustomColUID
   Next

End Sub

' ------------------------------------------------------------------------------
' Catch broadcast events
' ------------------------------------------------------------------------------

' ------------------------------------------------------------------------------
' Handle broadcast message to convert settings.
Sub Broadcast_UpdateApplicationSettingsVersion (XMLDoc, XMLRootNode, sngFileVersion)
    ' Targetlink.xml was merged into the settings file at ver. 2.0.
    ' Copy the <TargetLink> section from TargetLink.xml.
    If sngFileVersion < 2.0 Then
      ' Open the old targetlink.xml
      Dim XMLDocTL
      Set XMLDocTL = CreateObject ("Msxml2.DOMDocument.3.0")
      XMLDocTL.Async = False
      If XMLDocTL.Load (AppSettings.InstallDirectory & "\TargetLink.xml") Then
         ' Find the root node in the old targetlink.xml
         Dim RootNodeTL
         Set RootNodeTL = XMLDocTL.SelectSingleNode ("/Settings/TargetLink")
         If Not RootNodeTL Is Nothing Then
            ' Create root node for all scripted settings, if not already created.
            Dim RWSRootNode
            Set RWSRootNode = XMLRootNode.SelectSingleNode ("./RWSScriptedSettings")
            If RWSRootNode Is Nothing Then
               Set RWSRootNode = XMLDoc.CreateElement ("RWSScriptedSettings")
               XMLRootNode.AppendChild RWSRootNode
            End If

            If Not RWSRootNode Is Nothing Then
               RWSRootNode.AppendChild RootNodeTL
            End If
         End If
      End If
   End If
End Sub

'------------------------------------------------------------------------------
' Handle broadcast message to load settings.
Sub Broadcast_LoadApplicationSettings (XMLRootNode)
   ' Load table
   Set XMLElement = XMLRootNode.SelectSingleNode ("./TargetLink")
   If Not XMLElement Is nothing Then
      TargetLink.LoadData XMLElement

      ' Display the desired columns
      DisplayTableData

      Dim DefaultTargetElement
      Set DefaultTargetElement = XMLElement.SelectSingleNode ("./ActiveTarget")
      If Not DefaultTargetElement Is Nothing Then
         BuildScriptManager.SetActiveConnection (DefaultTargetElement.text)
      End If
   Else
      CreateColumnData
   End If
End Sub

'------------------------------------------------------------------------------
' Handle broadcast message to save settings.)
Sub Broadcast_SaveApplicationSettings (XMLDoc, XMLRootNode)
   ' Set row status to disconnected
   Dim RowUID
   For Each RowUID In TargetLink.GetRowUIDs
      TargetLink.SetField RowUID, "ID", "0"
      TargetLink.SetField RowUID, "Status", "Disconnected"
   Next

   ' Save XML
   Dim TableElement
   Set TableElement = XMLRootNode.SelectSingleNode ("./TargetLink")
   If TableElement Is nothing Then
      Set TableElement = XMLDoc.CreateElement ("TargetLink")
      XMLRootNode.AppendChild TableElement
   End If
   If Not TableElement Is nothing Then
      ' Now empty child nodes
      Dim ChildElement
      For Each ChildElement In TableElement.ChildNodes
         TableElement.RemoveChild ChildElement
      Next
   End If

   TargetLink.SaveData TableElement

   If Not TableElement Is nothing Then
      ' Add Default Target Child Node
      Dim DefaultTargetElement
      Set DefaultTargetElement = TableElement.SelectSingleNode ("./ActiveTarget")
      If DefaultTargetElement Is Nothing Then
         Set DefaultTargetElement = XMLDoc.CreateElement ("ActiveTarget")
         TableElement.AppendChild DefaultTargetElement
      End If
      If Not DefaultTargetElement Is Nothing Then
         DefaultTargetElement.Text = BuildScriptManager.strActiveTarget
      End If
   End If
End Sub
