' Enterprise Layout Management Script
'
' Layout object 'SplineToolbar'
'

Sub OnNewNodes ()
   If RWSSpline.NewNodesEnabled Then
      RWSSpline.NewNodes 0
      SplineToolbar.CheckMenuItem 1, False ' Uncheck New Nodes button
   Else
      RWSSpline.NewNodes 1
      SplineToolbar.CheckMenuItem 1, True ' Check New Nodes button
   End If
End Sub

Sub OnInsertNode ()
   RWSSpline.InsertNode
End Sub

Sub OnDeleteNodes ()
   RWSSpline.DeleteNodes
End Sub

Sub OnOpen ()
   RWSSpline.Open
End Sub

Sub OnClose ()
   RWSSpline.Close
End Sub