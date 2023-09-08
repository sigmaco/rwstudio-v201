'
' Enterprise Layout Management Script
'
' Layout object 'SelectionToolbar'
'

'NOTE: As the menu toolbar items are essentially shortcuts to items on the
'MainMenu, it is recommended that code in this script simply calls the
'appropriate mainmenu item

' ---------------------------------------------------------------------
Sub OnMenuFlyCamera
    MainMenu.OnMenuFlyCamera
End Sub

' ---------------------------------------------------------------------
Sub OnMenuOrbitCamera
    MainMenu.OnMenuOrbitCamera
End Sub

' ---------------------------------------------------------------------
Sub OnMenuPanCamera
    MainMenu.OnMenuPanCamera
End Sub

' ---------------------------------------------------------------------
Sub OnMenuZoomCamera
    MainMenu.OnMenuZoomCamera
End Sub

' ---------------------------------------------------------------------
Sub OnMenuPickMode
    MainMenu.OnMenuPickMode
End Sub

' ---------------------------------------------------------------------
Sub OnMenuPickTranslate
    MainMenu.OnMenuPickTranslateMode
End Sub

' ---------------------------------------------------------------------
Sub OnMenuPickRotate
    MainMenu.OnMenuPickRotateMode
End Sub

' ---------------------------------------------------------------------
Sub OnMenuPickScale
    MainMenu.OnMenuPickScaleMode
End Sub

' ---------------------------------------------------------------------
Sub OnMenuPickAndDrag
    MainMenu.OnMenuPickAndDrag
End Sub

' ---------------------------------------------------------------------
Sub OnMenuLockSelection
    MainMenu.OnMenuLockSelection
End Sub

' ---------------------------------------------------------------------
Sub OnMenuOrbitOnly
    MainMenu.OnMenuOrbitOnly
End Sub

' ---------------------------------------------------------------------
Sub OnMenuOrbitObj
    MainMenu.OnMenuOrbitObj
End Sub

' ---------------------------------------------------------------------
Sub OnMenuCamArc
    MainMenu.OnMenuCamArc
End Sub

' ---------------------------------------------------------------------
Sub OnMenuPickMaterials
    MainMenu.OnMenuPickMaterials
End Sub
