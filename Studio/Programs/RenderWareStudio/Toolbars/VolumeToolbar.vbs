' 
' Enterprise Layout Management Script
'
' Layout object 'VolumeToolbar'
'

' Shape mode globals
Dim smBox, smAABox
smBox = 0
smAABox = 1

' Corner mode globals
Dim pmCornerToCorner, pmCenterToCorner
pmCornerToCorner = 0
pmCenterToCorner = 1

' Current modes
Dim g_CurrentShape, g_CurrentMode, g_LastMouseFocus
g_CurrentShape = -1
g_CurrentMode = pmCornerToCorner
Set g_LastMouseFocus = Nothing

' Texture selection menu
Dim g_TextureMenuIndex
g_TextureMenuIndex   = 100

' Current texture
Dim g_strCurrentTexture
g_strCurrentTexture  = ""

' Dummy menu for when no textures are available
Dim g_strNoTextures
g_strNoTextures = "No textures available (see game properties)"

' The Location of the textures
Dim g_strTexturePath
g_strTexturePath     = ""

Dim dtChar ' From RWSScript's DataType enumeration
dtChar = 6

' ------------------------------------------------------------------------------
' Initialization dependant on other objects being loaded.
Sub OnLoad ()
   ' Initialize menu options
   SetSelectionMenuChecks

   PopulateTextureMenu ()

End Sub

' ------------------------------------------------------------------------------
Sub SetSelectionMenuChecks

   ' Shape mode
   If g_CurrentShape = smBox Then

      VolumeToolbar.CheckMenuItem 1, False ' Switch off smAABox mode
      VolumeToolbar.CheckMenuItem 2, True  ' Switch on smBox mode

   ElseIf g_CurrentShape = smAABox Then

      VolumeToolbar.CheckMenuItem 1, True  ' Switch on smAABox mode
      VolumeToolbar.CheckMenuItem 2, False ' Switch off smBox mode

   Else

      VolumeToolbar.CheckMenuItem 1, False ' Switch off smAABox mode
      VolumeToolbar.CheckMenuItem 2, False ' Switch off smBox mode

   End If

   ' Corner mode
   If g_CurrentMode = pmCornerToCorner Then
      VolumeToolbar.CheckMenuItem 4, True  ' Switch on pmCornerToCorner mode
      VolumeToolbar.CheckMenuItem 3, False ' Switch off pmCenterToCorner mode
   ElseIf g_CurrentMode = pmCenterToCorner Then
      VolumeToolbar.CheckMenuItem 4, False ' Switch off pmCornerToCorner mode
      VolumeToolbar.CheckMenuItem 3, True  ' Switch on pmCenterToCorner mode
   End If

End Sub

' ------------------------------------------------------------------------------
Sub EnableMenus (bEnable)

   ClearMenuChecks

   VolumeToolbar.EnableMenuItem 1, bEnable
   VolumeToolbar.EnableMenuItem 2, bEnable
   VolumeToolbar.EnableMenuItem 3, bEnable
   VolumeToolbar.EnableMenuItem 4, bEnable
   VolumeToolbar.EnableMenuItem 5, bEnable

End Sub

' ------------------------------------------------------------------------------
Sub ClearMenuChecks ()
   g_CurrentShape = -1
   SetSelectionMenuChecks
End Sub

' ------------------------------------------------------------------------------
Sub OnAABoxMode ()
   If g_CurrentShape = smBox Then
      g_CurrentShape = smAABox
   ElseIf g_CurrentShape = smAABox Then
      g_CurrentShape = -1
   Else
      Set g_LastMouseFocus = DesignView.ModuleMouseFocus
      g_CurrentShape = smAABox
   End If

   If Not g_CurrentShape = -1 Then

      DesignView.VolumeEdit.VolumeType = g_CurrentShape

      ' Set the mouse input to be handled by the volume editor
      DesignView.ModuleMouseFocus = "Volume Editing Tool"

      ' Initialize the volume editor with the pick dispatch interface
      DesignView.VolumeEdit.SetPick DesignView.Pick3D
   Else
      ' Restore last mode
      DesignView.ModuleMouseFocus = g_LastMouseFocus
   End If

   ' Update toolbars
   MainMenu.SetSelectionMenuChecks
   SetSelectionMenuChecks
End Sub

' ------------------------------------------------------------------------------
Sub OnBoxMode ()
   If g_CurrentShape = smAABox Then
      g_CurrentShape = smBox
   ElseIf g_CurrentShape = smBox Then
      g_CurrentShape = -1
   Else
      Set g_LastMouseFocus = DesignView.ModuleMouseFocus
      g_CurrentShape = smBox
   End If

   If Not g_CurrentShape = -1 Then

      DesignView.VolumeEdit.VolumeType = g_CurrentShape

      ' Set the mouse input to be handled by the volume editor
      DesignView.ModuleMouseFocus = "Volume Editing Tool"

      ' Initialize the volume editor with the pick dispatch interface
      DesignView.VolumeEdit.SetPick DesignView.Pick3D
   Else
      ' Restore last mode
      DesignView.ModuleMouseFocus = g_LastMouseFocus
   End If

   ' Update toolbars
   MainMenu.SetSelectionMenuChecks
   SetSelectionMenuChecks

End Sub

' ------------------------------------------------------------------------------
Sub OnCentreMode ()
   g_CurrentMode = pmCenterToCorner
   DesignView.VolumeEdit.CenterMode = g_CurrentMode
   SetSelectionMenuChecks
End Sub

' ------------------------------------------------------------------------------
Sub OnCornerMode ()
   g_CurrentMode = pmCornerToCorner
   DesignView.VolumeEdit.CenterMode = g_CurrentMode
   SetSelectionMenuChecks
End Sub

' ------------------------------------------------------------------------------
Sub VolumeToolbar_Click (nMenuIndex)

   ' Handler for texture menu clicks
   If nMenuIndex > g_TextureMenuIndex Then

      Dim TexMenuItems
      Set TexMenuItems = VolumeToolbar.MenuItems ("Texture selection").MenuItems

      g_strCurrentTexture = ""

      ' Clear all texture menu checks, and find current index
      Dim MenuItem
      For Each MenuItem In TexMenuItems
         If MenuItem.Index = nMenuIndex Then
            
            If Not MenuItem.MenuName = g_strNoTextures Then
               MenuItem.Checked = True
               g_strCurrentTexture = MenuItem.MenuName
            End If
         Else
            MenuItem.Checked = False
         End If
      Next

      ' Select the current texture
      If Len (g_strCurrentTexture) > 0 Then

         ' Get the currently selected entities
         Dim GlobalSelection
         Set GlobalSelection = CreateObject ("CSL.RWSSelection.Selection")
         GlobalSelection.SelectionIdentifier = GlobalScript.g_strGlobalSelection

         ' Set each entity's texture property
         If GlobalSelection.Count > 0 Then
            For Each SelectedItem In GlobalSelection
               If Not SelectedItem = 0 Then 

                  Dim APIObject
                  Set APIObject = GlobalScript.ObjectFromId (SelectedItem)

                  If Not APIObject is nothing Then
                     If APIObject.IDType = Database.otEntity Then

                        ' Add a string property for the hull texture
                        Dim Property, PropertyData, DataSection
                        Set Property = RWSScript.Property (Properties.conHullTexture)
                        Set PropertyData = CreateObject ("CSL.RWSScript.RWSData")

                        Set DataSection = PropertyData.Add (dtChar)
                        DataSection.Add g_strCurrentTexture

                        On Error Resume Next
                           Property.Set APIObject, PropertyData
   
                           ' Check for checkout-denied errors
                           If Not Err.Number = 0 Then

                              ' Display error message
                              Dim strOutputText
                              strOutputText = "[Volume Editor] Unable to change the texture for the current volume(s).  Please verify the checkout status of the selected volume(s)."
                              CSLRenderwareStudioWorkspace.ShowObject MessageLog
                              MessageLog.Error strOutputText

                              ' Exit if an error occured
                              Exit Sub

                           End If
                        On Error Goto 0
                     End If 
                  End If
               End If
            Next

         End If

         ' Set the texture
         DesignView.VolumeEdit.Texture = g_strCurrentTexture

      End If
   End If
End Sub

' ------------------------------------------------------------------------------
Sub PopulateTextureMenu ()

   ' Remove any menu items under the texture selection menubar
   Dim TexMenu
   Set TexMenu = VolumeToolbar.MenuItems ("Texture selection")

   Dim SubMenu
   For Each SubMenu In TexMenu.MenuItems
      VolumeToolbar.DeleteItem SubMenu.Index
   Next

   ' Find all textures and add to the dropdown list
   Dim fso, strFolder, nTextures
   nTextures = 0
   Set fso = CreateObject ("Scripting.FileSystemObject")
   strFolder = fso.GetAbsolutePathName (g_strTexturePath)

   If fso.FolderExists (strFolder) Then
      Dim Folder
      Set Folder = fso.GetFolder (strFolder)

      ' Add in the new item
      Dim File
      For Each File In Folder.Files

         ' Look for recognised texture files
         If IsValidTextureFile (fso, File.Path) Then
            nTextures = nTextures + 1
            VolumeToolbar.AddSubMenuItem File.Name, 5, g_TextureMenuIndex + nTextures, 0, True, "", 0, False, False, False

            ' If menu is current texture name, check it
            If File.Name = g_strCurrentTexture Then
               VolumeToolbar.MenuItems ("Texture selection").MenuItems (nTextures - 1).Checked = True
            End If
         End If
      Next
   End If

   ' Check the first texture menu item if we have no current texture
   If nTextures = 0 Then
      ' Add a dummy sub-menu under texture selection toolbar
      ' This allows us to populate the sub-menu when the button is clicked
      VolumeToolbar.AddSubMenuItem g_strNoTextures, 5, g_TextureMenuIndex + 1, 0, True, "", 0, False, False, False
   End If
End Sub

' ------------------------------------------------------------------------------
Function IsValidTextureFile (fso, strPath)
   Dim FileExt
   FileExt = fso.GetExtensionName (strPath)

   Select Case FileExt
   Case "jpg", "jpeg", "jpe", "jif", "png", "tga", "bmp", _
        "pcx", "rgb", "rgba", "int", "inta"
      IsValidTextureFile = True
   Case Else
      IsValidTextureFile = False
   End Select

End Function

' ------------------------------------------------------------------------------
Sub SetTextureRoot (strTextureRoot)
   g_strTexturePath = strTextureRoot
   PopulateTextureMenu

   DesignView.VolumeEdit.TexturePath = g_strTexturePath
End Sub

' ------------------------------------------------------------------------------