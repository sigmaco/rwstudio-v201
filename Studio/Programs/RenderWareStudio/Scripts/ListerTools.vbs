'
' Enterprise Layout Management Script
'
' Layout object 'Lister Tools'
'
' Helper functions common to all listers

Option Explicit ' All variables must be explicitly declared

Const g_XMLDOMProgID = "Msxml2.DOMDocument.3.0"

' --------------------------------------------------------------------
' Loads the state of the lister from an XML file
Sub LoadLister (Lister, ListerName, XMLRootNode)

   Lister.Load XMLRootNode.SelectSingleNode ("./ListerTools/" & ListerName)

   ' Load the plugins for the assets
   Dim AssetPlugin, AssetIcon, AssetBitmap
   For Each AssetPlugin In XMLRootNode.SelectNodes ("./ListerTools/ListerAssetPlugins/Plugin")
      Lister.AddAssetPlugin AssetPlugin.GetAttribute ("type"), _
                            AssetPlugin.GetAttribute ("progid"), _
                            AssetPlugin.GetAttribute ("backgroundthread")
   Next

   ' Load the icons for the assets
   For Each AssetIcon In XMLRootNode.SelectNodes ("./ListerTools/ListerAssetPlugins/Icon")
      Lister.AddAssetIcon AssetIcon.GetAttribute ("type"), _
                          AssetIcon.GetAttribute ("filename")
   Next

   ' Load the bitmaps for the assets
   For Each AssetBitmap In XMLRootNode.SelectNodes ("./ListerTools/ListerAssetPlugins/Bitmap")
      Lister.AddAssetBitmap AssetBitmap.GetAttribute ("type"), _
                            AssetBitmap.GetAttribute ("filename")
   Next
End Sub

' --------------------------------------------------------------------
' Save state of lister into XML file
Sub SaveLister (Lister, ListerName, XMLDoc, XMLRootNode)
   Dim RootListerNode
   Set RootListerNode = XMLRootNode.SelectSingleNode ("./ListerTools")

   If RootListerNode Is Nothing Then
      Set RootListerNode = XMLDoc.CreateElement ("ListerTools")
      XMLRootNode.AppendChild RootListerNode
   End If

   If Not RootListerNode Is Nothing Then
      Dim ListerElement
      Set ListerElement = RootListerNode.SelectSingleNode ("./" & ListerName)

      If ListerElement Is Nothing Then
         Set ListerElement = XMLDoc.CreateElement (ListerName)
         RootListerNode.AppendChild ListerElement
      End If

      ' Now empty child nodes
      Dim ChildElement
      For Each ChildElement In ListerElement.ChildNodes
         ListerElement.RemoveChild ChildElement
      Next

      Lister.Save XMLDoc, ListerElement
   End If
End Sub

' ------------------------------------------------------------------------------
' Catch broadcast events
' ------------------------------------------------------------------------------

' ------------------------------------------------------------------------------
' Handle broadcast message to convert settings.
Sub Broadcast_UpdateApplicationSettingsVersion (XMLDoc, XMLRootNode, sngFileVersion)
    ' Targetlink.xml was merged into the settings file at ver. 2.0.
    ' Copy all settings from TargetLink.xml except the <TargetLink> section.
    If sngFileVersion < 2.0 Then
      ' Open the old targetlink.xml
      Dim XMLDocTL
      Set XMLDocTL = CreateObject ("Msxml2.DOMDocument.3.0")
      XMLDocTL.Async = False
      If XMLDocTL.Load (AppSettings.InstallDirectory & "\TargetLink.xml") Then
         ' Find the root node in the old targetlink.xml
         Dim RootNodeTL
         Set RootNodeTL = XMLDocTL.SelectSingleNode ("/Settings")
         If Not RootNodeTL Is Nothing Then
            ' Create root node for all scripted settings, if not already created.
            Dim RWSRootNode
            Set RWSRootNode = XMLRootNode.SelectSingleNode ("./RWSScriptedSettings")
            If RWSRootNode Is Nothing Then
               Set RWSRootNode = XMLDoc.CreateElement ("RWSScriptedSettings")
               XMLRootNode.AppendChild RWSRootNode
            End If

            If Not RWSRootNode Is Nothing Then
               Dim RootListerNode
               Set RootListerNode = RWSRootNode.SelectSingleNode ("./ListerTools")
               If RootListerNode Is Nothing Then
                  Set RootListerNode = XMLDoc.CreateElement ("ListerTools")
                  RWSRootNode.AppendChild RootListerNode
               End If

               ' Copy all xml nodes under the root into the settings file
               If Not RootListerNode Is Nothing Then
                  Dim Node
                  For Each Node In RootNodeTL.ChildNodes
                     If Not Node.NodeName = "TargetLink" And _
                        Not Node.NodeName = "#comment" Then
                        RootListerNode.AppendChild Node
                     End If
                  Next
               End If
            End If
         End If
      End If
   End If
End Sub
