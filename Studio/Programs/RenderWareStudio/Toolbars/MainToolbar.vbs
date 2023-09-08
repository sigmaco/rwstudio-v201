' Criterion Software ©2001
' Enterprise Layout Management Script
'
' MainToolbar

'NOTE: As the menu toolbar items are essentially shortcuts to items on the
'MainMenu, it is recommended that code in this script simply calls the
'appropriate mainmenu item

' ---------------------------------------------------------------------
Sub OnMenuToolbarNewProject
    MainMenu.OnMenuNewProject
End Sub

' ---------------------------------------------------------------------
Sub OnMenuToolbarOpenProject
    MainMenu.OnMenuOpenProject
End Sub

' ---------------------------------------------------------------------
Sub OnMenuToolbarSaveProject
    MainMenu.OnMenuSaveProject
End Sub

' ---------------------------------------------------------------------
Sub OnMenuToolbarUndo
    MainMenu.OnMenuUndo
End Sub

' ---------------------------------------------------------------------
Sub OnMenuToolbarRedo
    MainMenu.OnMenuRedo
End Sub

' ---------------------------------------------------------------------
Sub OnMenuShowUndoStack
    MainMenu.OnMenuShowUndoStack
End Sub

' --------------------------------------------------------------------- 
Sub OnMenuParseSource 
    MainMenu.OnMenuToolsParseSource 
End Sub 
 
' --------------------------------------------------------------------- 
Sub OnMenuRefreshEventView 
    MainMenu.OnMenuRefreshEventView 
End Sub