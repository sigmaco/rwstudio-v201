'
' SplineOptions.vbs
'
' Spline Options Script
'
'

Dim NodeSize, NewNodeYOffset

' Initial values outside of Game.settings file
NodeSize       = 0.01 ' Size of Nodes in the game 
NewNodeYOffset = 0.1  ' Y Offset for new nodes created in the game

' ---------------------------------------------------------------------

Sub DoModal ()
    RENManager.ShowModal "SplineOptions", "CSLRenderwareStudioWorkspace"    
End Sub 

' ---------------------------------------------------------------------

Sub OnLoad ()
    EditBoxNodeSize.Value = NodeSize
    EditBoxNewNodeOffset.Value = NewNodeYOffset
    
    RWSSpline.SetNodeSize NodeSize
    RWSSpline.SetNodesOffset NewNodeYOffset
End Sub

' ---------------------------------------------------------------------

Sub Apply ()
    NodeSize = EditBoxNodeSize.Value 
    NewNodeYOffset = EditBoxNewNodeOffset.Value
    
    RWSSpline.SetNodeSize NodeSize
    RWSSpline.SetNodesOffset NewNodeYOffset
End Sub

' ---------------------------------------------------------------------

Sub OKButton_Click ()
    Apply
    SplineOptions.StopModal True
End Sub

' ---------------------------------------------------------------------

Sub CancelButton_Click () 
    SplineOptions.StopModal False 
End Sub

' --------------------------------------------------------------------- 
 
Sub ApplyButton_Click () 
    Apply 
End Sub 



' ------------------------------------------------------------------------------ 
' Catch broadcast events 
' ------------------------------------------------------------------------------ 
 
' ------------------------------------------------------------------------------ 
' Handle broadcast message to load settings.
Sub Broadcast_LoadGameSettings (XMLRootNode)
   Dim NodesElement
   Set NodesElement = XMLRootNode.SelectSingleNode ("./SplineOptions/Nodes")
   If Not NodesElement Is Nothing Then
        NodeSize = NodesElement.GetAttribute("NodeSize")
        NewNodeYOffset = NodesElement.GetAttribute("NewNodeYOffset")
   End If
   
   If IsNumeric (NodeSize) Then
       EditBoxNodeSize.Value = NodeSize
       RWSSpline.SetNodeSize NodeSize
   End If

   If IsNumeric (NewNodeYOffset) Then
       EditBoxNewNodeOffset.Value = NewNodeYOffset
       RWSSpline.SetNodesOffset NewNodeYOffset
   End If
End Sub
 
' ------------------------------------------------------------------------------ 
' Handle broadcast message to save settings. 
Sub Broadcast_SaveGameSettings (XMLDoc, XMLRootNode) 
   Dim SplinesElement, NodesElement, ChildElement 
   Set SplinesElement = XMLRootNode.SelectSingleNode ("./SplineOptions")
 
   ' Create the root Spline element if it isn't already in the settings file. 
   If SplinesElement Is nothing Then 
         Set SplinesElement = XMLDoc.CreateElement ("SplineOptions") 
         XMLRootNode.AppendChild SplinesElement 
   End If 
 
   ' Check we have a Splines element to add our settings to. 
   If Not SplinesElement Is nothing Then 
      ' Remove all child nodes. 
      For Each ChildElement In SplinesElement.ChildNodes 
         SplinesElement.RemoveChild ChildElement 
      Next 
      ' Add in new child nodes. 
      Set NodesElement = XMLDoc.CreateElement ("Nodes")  
      NodesElement.SetAttribute "NodeSize", NodeSize
      NodesElement.SetAttribute "NewNodeYOffset", NewNodeYOffset
      SplinesElement.AppendChild NodesElement 
   End If 
End Sub