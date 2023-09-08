' ------------------------------------------------------------------------------
'
' Enterprise Layout Management Script
'
' Layout object 'Sequencer Tools'
'
' Helper functions related to the RWSSequencer control


Option Explicit ' All variables must be explicitly declared 

Const g_XMLDOMProgID = "Msxml2.DOMDocument.3.0"

' ------------------------------------------------------------------------------
' Loads the sequencer's settings
Sub LoadSequencer (TheSequencer, XMLRootNode)
   Dim SequencerNode
   Set SequencerNode = XMLRootNode.SelectSingleNode ("./SequencerTools")
   If SequencerNode Is Nothing Then
      ' Load defaults
      LoadDefaultSettings XMLRootNode

      ' This should now work
      Set SequencerNode = XMLRootNode.SelectSingleNode ("./SequencerTools")
   End If

   ' Read the plugins from the XML document
   ' Key Frame Plugins:
   Dim KfPlugin, KfIndex
   For Each KfPlugin In SequencerNode.SelectNodes ("./keyframe/plugin")
      KfIndex = TheSequencer.AddKeyFramePlugin (KfPlugin.GetAttribute ("name"), _
                                                KfPlugin.GetAttribute ("progid"))

      ' Register any types with this key frame plugin                                                 
      If Not KfIndex = -1 Then
            ' Load each of the comma delimited key frame types
            Dim KfType
            For Each KfType In Tokenize ("[^,]+", KfPlugin.GetAttribute ("type"))
               TheSequencer.AddKeyFramePluginType KfIndex, Trim (KfType)
            Next
      End If
   Next

   ' Interpolator plugins
   Dim Interpolator, InterpIndex
   For Each Interpolator In SequencerNode.SelectNodes ("./interpolators/interpolator")
      InterpIndex = TheSequencer.AddInterpolator (Interpolator.GetAttribute ("name"), _
                                                  Interpolator.GetAttribute ("consoleid"))
      If Not InterpIndex = -1 Then
         ' Load each of the interpolator types
         Dim InterpPlugin
         For Each InterpPlugin In Interpolator.SelectNodes ("./plugin")
            ' The type attribute consists of a comma delimited set of strings.
            Dim ProgId, InterpType
            ProgId = InterpPlugin.GetAttribute ("progid")
            For Each InterpType In Tokenize ("[^,]+", InterpPlugin.GetAttribute ("type"))
               TheSequencer.AddInterpolatorPlugin InterpIndex, ProgId, Trim (InterpType)
            Next
         Next
      End If
   Next

   TheSequencer.SelectionIdentifier = GlobalScript.g_strGlobalSelection
End Sub

' ------------------------------------------------------------------------------
' Saves the sequencer's settings
Sub SaveSequencer (Sequencer, XMLDoc, XMLRootNode)
   Dim SequencerNode
   Set SequencerNode = XMLRootNode.SelectSingleNode ("./SequencerTools")

   If SequencerNode Is Nothing Then
      ' Load defaults and append to root node
      LoadDefaultSettings XMLRootNode
   End If
End Sub

' ------------------------------------------------------------------------------ 
' Simple regular expression tokenizing of a string 
Function Tokenize (strPattern, strString) 
   Dim oRegExp 
   Set oRegExp = New RegExp 
   oRegExp.Pattern = strPattern 
   oRegExp.Global = True 
   Set Tokenize = oRegExp.Execute (strString) 
End Function

' ------------------------------------------------------------------------------
' Loads the default settings for the sequencer.
Sub LoadDefaultSettings (XMLRootNode)
    ' Load some default XML into the settings node
    Dim NewXmlDoc
    Set NewXmlDoc = CreateObject (g_XMLDOMProgID)
    If NewXmlDoc.LoadXML (GetDefaultSettings ()) Then
       XMLRootNode.AppendChild NewXmlDoc.FirstChild
    End If
End Sub

' ------------------------------------------------------------------------------
' The default settings for the sequencer.
Function GetDefaultSettings ()
   GetDefaultSettings = _
"<SequencerTools>" +_
   "<keyframe>" +_ 
      "<plugin name=""Attribute control editor"" progid=""CSL.RWSSequencer.DefaultKeyframePlugin"" type=""*""/>" +_ 
      "<plugin name=""Design view matrix editor"" progid=""CSL.RWSUtility.MatrixKeyframePlugin"" type=""RwMatrix""/>" +_ 
      "<plugin name=""Design view camera editor"" progid=""CSL.RWSUtility.CameraKeyframePlugin"" type=""RwMatrix, RwMatrixFov""/>" +_
   "</keyframe>" +_
   "<interpolators>" +_ 
      "<interpolator name=""Linear Interpolator"" consoleid=""CLinearInterpolator"">" +_  
         "<plugin progid=""CSL.RWSSequencer.MatrixInterpolatorPlugin"" type=""RwMatrix""/>" +_ 
         "<plugin progid=""CSL.RWSSequencer.ColorInterpolatorPlugin"" type=""RwRGBA""/>" +_ 
         "<plugin progid=""CSL.RWSSequencer.LinearInterpolatorPlugin"" type=""*""/>" +_  
      "</interpolator>" +_
   "</interpolators>" +_
"</SequencerTools>"
End Function

' ------------------------------------------------------------------------------
' Ensures the supplied sequence has been saved
' Returns a boolean indicating whether this method succeeded.
Function EnsureSequenceAssetSaved (TheSequencer)
   Dim bShouldSave
   EnsureSequenceAssetSaved = True
   bShouldSave = False

   ' Determine if we should save the current sequence
   If TheSequencer.IsDirty () Then
      If MsgBox ("The current sequence has been modified. Would you like to save?", _
                 vbYesNo + vbQuestion, "Sequencer") = vbYes Then
         bShouldSave = True
      Else
         Persist.Modified
      End If
   End If

   ' Allow the sequencer to save its loaded sequence
   If bShouldSave Then
      If Not Len (TheSequencer.CurrentFile) = 0 Then
         If Not TheSequencer.SaveFile () Then
            MsgBox "Failed to save file " & TheSequencer.CurrentFile, vbExclamation, "Sequencer"
            EnsureSequenceAssetSaved = False
         End If
      End If
   End If

   ' If the sequencer is still dirty, then the save failed. Pop up a dialog
   ' to ask the user for a new file.
   If bShouldSave And TheSequencer.IsDirty () Then
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

         Do
            EnsureSequenceAssetSaved = True
            If .ShowSave () And Len (.Filename) Then 
               If Not TheSequencer.SaveFileAs (.Filename) Then
                  ' Save failed. Give the user the option to retry.
                  EnsureSequenceAssetSaved = False
                  If MsgBox ("Failed to save file " & .Filename, vbExclamation + vbRetryCancel, "Sequencer") = vbCancel Then
                     ' User didn't want to retry, so exit.
                     Persist.Modified
                     Exit Do
                  End If
               Else
                  ' Save successfull.
                  ImportAsset TheSequencer
                  Exit Do
               End If
            Else
               ' User clicked cancel to save.
               Persist.Modified
               EnsureSequenceAssetSaved = False
               Exit Do
            End If
         Loop
      End With
   End If
End Function

' ------------------------------------------------------------------------------
' Determines if an asset is a sequence.
Function IsSequence (oAsset)
   If oAsset.BuildCommand = "SequenceAsset" Then
      IsSequence = True
   Else
      IsSequence = False
   End If
End Function

' ------------------------------------------------------------------------------
' If the sequencer is editting the asset then it makes sure that it is saved.
Sub SaveIfDirty (oSequencer, oAsset)
   If IsSequence (oAsset) Then
      If oSequencer.CurrentFile = GlobalScript.g_strResourceRoot & "\" & oAsset.File Then
         EnsureSequenceAssetSaved oSequencer
      End If
   End If
End Sub

' ------------------------------------------------------------------------------
' Asks the user if they would like to create a sequence asset that represents
' the currently loaded sequence
Sub ImportAsset (oSequencer)
   If MsgBox ("Would you like to import the sequence into the database?", vbQuestion + vbYesNo, "Sequencer") = vbYes Then
      Dim NewAsset, FileSystemObject, RWSScript
      Set FileSystemObject = CreateObject ("Scripting.FileSystemObject")
      Set RWSScript = CreateObject ("CSL.RWSScript.RWSScript")
      Set NewAsset = RWSScript.CreateAsset ()

      ' Ask the sequencer to create the sequence
      NewAsset.Name = FileSystemObject.GetBaseName (oSequencer.CurrentFile)
      NewAsset.File = GlobalScript.RWSUtils.GetRelativePath (oSequencer.CurrentFile, GlobalScript.g_strResourceRoot)
      NewAsset.Type = "RF3"
      NewAsset.BuildCommand = "SequenceAsset"

      ' Add the new asset to the parent
      RWSScript.AssetHierarchy.AddChildAsset NewAsset
   End If
End Sub
