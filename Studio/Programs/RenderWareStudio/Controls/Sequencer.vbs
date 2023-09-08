'
' Enterprise Layout Management Script
'
' Layout object 'Sequencer'
'

Option Explicit ' All variables must be explicitly declared 

Dim oSequencerComms
Set oSequencerComms = CreateObject ("CSL.RWSTarget.RWSComms")
RENHost.ConnectObject oSequencerComms, "SequencerComms"

' ------------------------------------------------------------------------------
' Event fired when the sequencer user selects "Locate in Game Explorer"
Sub Sequencer_OnLocateInGameExplorer (EntityID)
   GameDatabase.LocateEntity EntityID
   CSLRenderwareStudioWorkspace.ShowObject GameDatabase
End Sub

' ------------------------------------------------------------------------------
' Event fired when the sequencer user selected "Locate in Templates"
Sub Sequencer_OnLocateInTemplates (TemplateID)
   TemplateLister.LocateEntity TemplateID
   CSLRenderwareStudioWorkspace.ShowObject TemplateLister
End Sub

' ------------------------------------------------------------------------------
' Event fired when the sequencer user selects "Locate in View"
Sub Sequencer_OnLocateInView (EntityID)
   DesignView.RWSUtility.LocateEntity EntityID
   CSLRenderwareStudioWorkspace.ShowObject DesignView
End Sub

' ------------------------------------------------------------------------------
' Event fired when the sequencer user selects "Aim at"
Sub Sequencer_OnAimAt (EntityID)
   DesignView.RWSUtility.AimAtEntity EntityID
   CSLRenderwareStudioWorkspace.ShowObject DesignView
End Sub

' ------------------------------------------------------------------------------
' Event fired when the sequencer user selected the "New" toolbar item
Sub Sequencer_OnNew ()
   ' If we're currently editting a sequence, then save it.
   If SequencerTools.EnsureSequenceAssetSaved (Sequencer) Then
      Sequencer.NewFile
   End If
End Sub

' ------------------------------------------------------------------------------
' Event fired when the sequencer user selected the "New" toolbar item
Sub Sequencer_OnOpen ()
   ' If we're currently editting a sequence, then save it.
   If SequencerTools.EnsureSequenceAssetSaved (Sequencer) Then
      ' Select the file to open
      Dim oCommCtrl 
      Set oCommCtrl = CreateObject ("CSL.RENCommonDialog") 
      With oCommCtrl
         .Filter = "RF3 Sequence (*.rf3)|*.rf3" 
         .FilterIndex = 2 
         .MaxFileSize = 260 
         .hwndOwner = RENManager.GetObjectHWND("CSLRenderwareStudioWorkspace") 
         .flags = .OFN_HIDEREADONLY + .OFN_FILEMUSTEXIST
         .DialogTitle = "Open Sequence" 
         .InitDir = GlobalScript.g_strResourceRoot 

         If .ShowOpen () And Len (.Filename) Then
            ' Open the file
            If Sequencer.OpenFile (.Filename) Then
               MessageLog.Log "[Sequencer] Sequence """ & .Filename & """ successfully loaded"

               ' Ask the user if they want to import into studio
               SequencerTools.ImportAsset Sequencer
            Else
               MessageLog.Error "[Sequencer] Unable to load sequence """ & .Filename & """"
            End If
         End If
      End With
   End If
End Sub

' ------------------------------------------------------------------------------
' Event fired when the sequencer user selected the "New" toolbar item
Sub Sequencer_OnSave ()
   If Len (Sequencer.CurrentFile) Then
      If Not Sequencer.SaveFile () Then
         MessageLog.Error "[Sequencer] Sequence failed to save to file """ & Sequencer.CurrentFile & """"

         ' Attempt a save as
         Sequencer_OnSaveAs
      End If
   Else
      Sequencer_OnSaveAs
   End If
End Sub

' ------------------------------------------------------------------------------
' Event fired when the sequencer user selected the "Save As" toolbar item
Sub Sequencer_OnSaveAs ()
   Dim oCommCtrl 
   Set oCommCtrl = CreateObject ("CSL.RENCommonDialog") 
   With oCommCtrl
      .Filter = "RF3 Sequence (*.rf3)|*.rf3" 
      .FilterIndex = 2 
      .MaxFileSize = 260 
      .hwndOwner = RENManager.GetObjectHWND("CSLRenderwareStudioWorkspace") 
      .flags = .OFN_HIDEREADONLY + .OFN_OVERWRITEPROMPT + .OFN_PATHMUSTEXIST
      .DialogTitle = "Save Sequence As" 
      .InitDir = GlobalScript.g_strResourceRoot 

      If .ShowSave () And Len (.Filename) Then
         If Sequencer.SaveFileAs (.Filename) Then
            MessageLog.Log "[Sequencer] Sequence saved as """ & .Filename & """"

            ' Give the user the chance to create an asset with the new file
            SequencerTools.ImportAsset Sequencer
         Else
            MessageLog.Error "[Sequencer] Sequence failed to save to file """ & .Filename & """"
         End If
      End If
   End With
End Sub

' ------------------------------------------------------------------------------
' Fired when edits are made to a sequence.
Sub Sequencer_OnSequenceDirty ()
   ' Update the "Save Project" menu item and the save toolbar button
   Persist.Modified

   ' Check out the sequence if it is under source control
   If Len (Sequencer.CurrentFile) Then
      If VersionControl.IsPathManaged (Sequencer.CurrentFile) Then
         ' Check out the file if latest version without a dialog.
         VersionControl.CheckOutFile Sequencer.CurrentFile, True, False
      End If
   End If
End Sub

' -----------------------------------------------------------------------------
' Enable/disable the processing of console updates depending upon whether we
' are connected to any consoles. This can be determined by listening to the
' events fired by RWSComms.

' -----------------------------------------------------------------------------
' Called when a connection is made to a console
Sub SequencerComms_OnConnectionConnected (oConnection)
   Sequencer.EnableConsoleUpdates (oSequencerComms.Connections.Count)
End Sub

' -----------------------------------------------------------------------------
' Called when a connection is disconnected from a console
Sub SequencerComms_OnConnectionDisconnected (oConnection)
   Sequencer.EnableConsoleUpdates (oSequencerComms.Connections.Count)
End Sub

' -----------------------------------------------------------------------------
' Called when a connection to a console is lost
Sub SequencerComms_OnConnectionLost (oConnection)
   Sequencer.EnableConsoleUpdates (oSequencerComms.Connections.Count)
End Sub

' ------------------------------------------------------------------------------
' Fired when a dynamic change on the console is required. The instruction
' container contains a list of all operations that need updating.
Sub Sequencer_OnAttributeUpdate (oInstructionContainer)
   Dim oConnection, oMake, oPlatformFilter, oMemoryBuffer 
   For Each oConnection In GlobalScript.RWSComms.Connections  
      Set oMake = oConnection.GetParam (BuildScriptManager.strMakeAssociation)
      Set oPlatformFilter = GlobalScript.RWSComms.CreatePlatformFilter ( _
                   GlobalScript.RWSComms.GetPlatformFlags (oConnection.Platform))
      Set oMemoryBuffer = GlobalScript.RWSComms.CreateMemoryBuffer ()  
      oMemoryBuffer.BigEndian = oConnection.BigEndian  
      oMemoryBuffer.Unicode = oConnection.Unicode

      oMake.AddTask "SequenceConsoleUpdate", oInstructionContainer, oPlatformFilter, oMemoryBuffer
      oMake.Build

      oConnection.SendBuffer oMemoryBuffer
    Next  
End Sub

' ------------------------------------------------------------------------------
' Fired when an attribute is no longer being edited, so the attribute needs to 
' be restored to the value in the database
Sub Sequencer_OnAttributeRestore (AttributeID)
   Dim oAttribute
   Set oAttribute = BuildScriptManager.BSMScript.Attribute (AttributeID)
   If Not oAttribute Is Nothing Then
      BuildScriptManager.BSMScriptEvents_OnChange oAttribute
   End If
End Sub

' ------------------------------------------------------------------------------
' Fired when the user clicks on the preview button. strTempFilename is a
' temporary file containing an upto date RF3 sequence asset. This event compiles
' this file and sends it to the console to display a preview.
'
' strTempFilename - The filename of a temporary file that represents the
'                   current sequence being edited.
' nOffset         - The offset in milliseconds of where the sequence should start
'                   playing from.
Sub Sequencer_OnPreview (strTempFilename, nOffset)
   Dim oConnection, oMake, strPlatformFlags, oMemoryBuffer, oSequenceTranslator
   Set oSequenceTranslator = BuildScriptManager.SequenceTranslator
   For Each oConnection In GlobalScript.RWSComms.Connections
      Set oMake = oConnection.GetParam (BuildScriptManager.strMakeAssociation)
      strPlatformFlags = GlobalScript.RWSComms.GetPlatformFlags (oConnection.Platform)

      ' Create a memory buffer to hold the console stream
      Set oMemoryBuffer = GlobalScript.RWSComms.CreateMemoryBuffer ()  
      oMemoryBuffer.BigEndian = oConnection.BigEndian  
      oMemoryBuffer.Unicode = oConnection.Unicode

      ' Allow the GPM to translate the sequence
      oMake.AddTask "SequencePreview", oMemoryBuffer, oSequenceTranslator, _
                    strTempFilename, strPlatformFlags, nOffset
      oMake.Build

      ' Send the buffer to the console
      oConnection.SendBuffer oMemoryBuffer
   Next
End Sub

' ------------------------------------------------------------------------------
' Determines whether a template exists in the design views. This is used to
' enable/disable the 'Locate in view' and 'Aim At' template menu items.
Sub Sequencer_IsInView (EntityID, ByRef bRetVal)
   bRetVal = DesignView.RWSUtility.IsInView (EntityID)
End Sub

' ------------------------------------------------------------------------------
' Catch broadcast events
' ------------------------------------------------------------------------------

' ------------------------------------------------------------------------------
' Handle broadcast message to convert settings.
'Sub Broadcast_UpdateApplicationSettingsVersion (XMLDoc, XMLRootNode, nFileVersion)
'End Sub
       
' ------------------------------------------------------------------------------
' Handle broadcast message to load settings.
Sub Broadcast_LoadApplicationSettings (XMLRootNode)
   ' Load the sequencer plugins
   SequencerTools.LoadSequencer Sequencer, XMLRootNode
End Sub

' ------------------------------------------------------------------------------
' Handle broadcast message to save settings.
Sub Broadcast_SaveApplicationSettings (XMLDoc, XMLRootNode)
   ' Save the sequencer plugins
   SequencerTools.SaveSequencer Sequencer, XMLDoc, XMLRootNode
End Sub

' ------------------------------------------------------------------------------
' Handle broadcast event to say source parse has completed
Sub Broadcast_PostParseSource ()
   Sequencer.RefreshErrors
End Sub
