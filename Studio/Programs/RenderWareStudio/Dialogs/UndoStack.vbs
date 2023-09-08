'
' Enterprise Layout Management Script
'
' Layout object 'Undo Stack'
'

' Note: 2 text labels have been used in the layout to display as groups.
'       This is because using MS Forms groups does not allow the ren file to save.

Dim UndoFlag
Dim RedoFlag

Dim ignoreChange
ignoreChange = False
Dim UndoSelectedItem ' The index of the selected item in the undo lister.
Dim RedoSelectedItem ' The index of the selected item in the redo lister.
UndoSelectedItem = -1
RedoSelectedItem = -1

'---------------------------------------------------------------------
' Initialization dependant on other objects being loaded.
Sub OnLoad ()
    UndoFlag = GlobalScript.UndoFlag
    RedoFlag = GlobalScript.RedoFlag
End Sub

Sub Initialize
    PopulateLists
End Sub

Sub PopulateLists
    ignoreChange = True
    ' Clear the form, so that we don't see previous values
    UndoList.Clear ()
    RedoList.Clear ()
    RedoSelectedItem = -1
    UndoSelectedItem = -1

    ' Add the Redo elements
    RedoCount = GlobalScript.Undo.GetItemCount (RedoFlag)
    If RedoCount > 0 Then
        For i = 0 To RedoCount-1 Step 1
            RedoItem = GlobalScript.Undo.GetItemDescription (i, RedoFlag)
            RedoList.AddItem RedoItem
        Next
        RedoSelectedItem = 0
        RedoList.SetCurSelection 0
        RedoButton.Enabled = True
    Else
        RedoButton.Enabled = False
    End If

    ' Add the Undo elements
    UndoCount = GlobalScript.Undo.GetItemCount (UndoFlag)
    If UndoCount > 0 Then
        For i = 0 to UndoCount-1 Step 1
            UndoItem = GlobalScript.Undo.GetItemDescription (i, UndoFlag)
            UndoList.AddItem UndoItem
        Next
        UndoSelectedItem = 0
        UndoList.SetCurSelection 0
        UndoButton.Enabled = True
    Else
        UndoButton.Enabled = False
    End If
    ignoreChange = False

End Sub

Sub CloseButton_Click (  )
    UndoStack.StopModal False
End Sub

Sub UndoButton_Click (  )
    numUndos = UndoSelectedItem + 1
    For i = 1 To numUndos Step 1
        MainMenu.OnMenuUndo
    Next
    PopulateLists
End Sub

Sub RedoButton_Click (  )
    numRedos = RedoSelectedItem   + 1
    For i = 1 To numRedos Step 1
        MainMenu.OnMenuRedo
    Next
    PopulateLists
End Sub

Sub UndoList_OnChange (  )
    If ignoreChange = True Then
        Exit Sub
    End If

    UndoSelectedItem = UndoList.GetCurSelection

    If UndoSelectedItem < 0 Then
        Exit Sub
    End If

    ignoreChange = True
    For i = 0 To UndoSelectedItem Step 1
        UndoList.SetCurSelection (i)
    Next

    ' If there are more items in the list, then clear them.
    If UndoSelectedItem + 1 < UndoList.ListCount Then
        For i = UndoSelectedItem + 1 To UndoList.ListCount Step 1
            UndoList.Clear i
        Next
    End If

    ignoreChange = False
End Sub

Sub RedoList_OnChange (  )
    If ignoreChange = True Then
        Exit Sub
    End If

    RedoSelectedItem = RedoList.GetCurSelection
    If RedoSelectedItem < 0 Then
        Exit Sub
    End If

    ignoreChange = True
    For i = 0 To RedoSelectedItem Step 1
        RedoList.SetCurSelection (i)
    Next

    ' If there are more items in the list, then clear them.
    If UndoSelectedItem + 1 > RedoList.ListCount Then
        For i = UndoSelectedItem + 1 To RedoList.ListCount Step 1
            RedoList.Clear i
        Next
    End If

   ignoreChange = False
End Sub