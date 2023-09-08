'
' Enterprise Layout Management Script
'
' Layout object 'ContextMenu'
'
' Used to display a context menu in the listers.
'

Option Explicit

Dim CurrentContext, MenuScript, ResourceRoot
Set MenuScript = CreateObject ("CSL.RWSScript.RWSScript")

' Lister view modes
Const SmallIcon = 0
Const SmallThumbnail = 1
Const LargeThumbnail = 2
Const Detail = 3
Const List = 4
Const Tree = 5

' Lister node types
Const GameDatabaseNode = 0
Const BehaviorListerNode = 1
Const AssetListerNode  = 2
Const TemplateListerNode = 3
Const AttributeShareListerNode = 4
Const SearchResultsListerNode = 5

' Object types as specified within RWSScript type library
Const otGame                 = 1
Const otEntity               = 2
Const otFolder               = 4
Const otAsset                = 8
Const otAssetFolder          = 16
Const otTemplate             = 32
Const otTemplateFolder       = 64
Const otBehavior             = 128
Const otBehaviorFolder       = 256
Const otCommand              = 512
Const otAttribute            = 1024
Const otAttributeShare       = 2048
Const otAttributeShareFolder = 4096
Const otEvent                = 8192

' Attribute Data Section Types
Const dtBool                  = 0
Const dtInt32                 = 1
Const dtUInt32                = 2
Const dtReal                  = 3
Const dtDouble                = 4
Const dtByte                  = 5
Const dtChar                  = 6


' Messages and tags used below
Const msgReloadAsset         = "RWSMSG_RELOADASSET"
Const tagActiveFolder        = "RWSTAG_ACTIVE_FOLDER"

' ------------------------------------------------------------------------------ 
' Initialization
ResourceRoot = ""

' ------------------------------------------------------------------------------ 
Sub SetResourceRoot (NewResourceRoot)
    ResourceRoot = NewResourceRoot
End Sub

' ------------------------------------------------------------------------------
' Checks the relavent view mode in the menu (assumes the menu is cloned)
Sub SetupViewMenu (ViewMenuItem, Context)
   Dim ViewMode
   ViewMode = Context.Lister.ViewMode

   If ViewMode = SmallIcon Then
      ViewMenuItem.MenuItems("Small Icons").Checked = True
   ElseIf ViewMode = SmallThumbnail Then
      ViewMenuItem.MenuItems("Small Thumbnails").Checked = True
   ElseIf ViewMode = LargeThumbnail Then
      ViewMenuItem.MenuItems("Large Thumbnails").Checked = True
   ElseIf ViewMode = Detail Then
      ViewMenuItem.MenuItems("Detail").Checked = True
   ElseIf ViewMode = List Then
      ViewMenuItem.MenuItems("List").Checked = True
   ElseIf ViewMode = Tree Then
      ViewMenuItem.MenuItems("Tree").Checked = True
   End If
End Sub

' ------------------------------------------------------------------------------
' Enables or disables the cut/copy/paste menus depending upon the current context
Sub SetupClipboardMenu (Menu, Context)
   Menu.MenuItems ("Cut").Enabled = Context.CanCut
   Menu.MenuItems ("Copy").Enabled = Context.CanCopy
   Menu.MenuItems ("Paste").Enabled = Context.CanPaste
End Sub

' ------------------------------------------------------------------------------
' Enabled or disables the move up/move down menus depending upon the context
Sub SetupMoveMenu (Menu, Context)
   Menu.MenuItems ("Move Up").Enabled = Context.CanMoveUp
   Menu.MenuItems ("Move Down").Enabled = Context.CanMoveDown
End Sub

' ------------------------------------------------------------------------------
' Sets up the menu for the focused game. Returns the index of the menu item to
' display.
Function SetupGameMenu (Menu, Context)
   Dim GameMenu
   Set GameMenu = Menu.MenuItems ("Game")

   ' Setup the view menu
   SetupViewMenu GameMenu.MenuItems ("View"), Context

   ' Determine whether the item has any XML that can be viewed 
   If CanViewXML (Context) Then 
      ' Do nothing, item is already enabled
   Else 
      GameMenu.MenuItems ("View XML").Enabled = False 
   End If

   ' Setup NXN alienbrain menu
   SetupAlienbrainMenu GameMenu.MenuItems ("NXN alienbrain"), Context
   
   ' Return the index of the item we want to display
   SetupGameMenu = GameMenu.Index
End Function

' ------------------------------------------------------------------------------
' Sets up the menu for the focused folder. Returns the index of the menu item to
' display
Function SetupFolderMenu (Menu, Context)
   Dim FolderMenu, DeleteRemoveItem, Folder
   Set FolderMenu = Menu.MenuItems ("Folder")
   Set DeleteRemoveItem = FolderMenu.MenuItems.Item("<DeleteRemove>")

   Dim GlobalFolder, GlobalFolderID
   Set GlobalFolder = GlobalScript.RWSScript.Game.GlobalFolder
   If Not GlobalFolder Is Nothing Then
      GlobalFolderID = GlobalFolder.ID
   Else
      GlobalFolderID = 0
   End If

   ' Determine whether we can delete or remove this item
   Set Folder = GetFocusObject (Context)
   If Folder.ID = GlobalFolderID Then
      DeleteRemoveItem.MenuName = "Delete"
      DeleteRemoveItem.Enabled  = False
   ElseIf Folder.ParentFolders.Count > 1 Then
      DeleteRemoveItem.MenuName = "Remove"
      DeleteRemoveItem.Event    = "OnRemove"
   Else
      DeleteRemoveItem.MenuName = "Delete"
      DeleteRemoveItem.Event    = "OnDelete"
   End If

   ' Determine whether this item can be set as active.
   If Context (1).Count > 1 Then
      Dim ActiveFolder, SetAsActiveMenuItem
      Set ActiveFolder = MenuScript.Game.ActiveFolder
      Set SetAsActiveMenuItem = FolderMenu.MenuItems ("Active Folder")

      If Context (1).Item (2).IDType = otGame Then
         ' Folder is a child of the game - can be set as active (default)

         ' If the folder isn't the current active folder then enable the menu item
         If Not ActiveFolder = Context (1).Item (1) Then
            SetAsActiveMenuItem.Checked = False
            SetAsActiveMenuItem.Enabled = True
         Else
            SetAsActiveMenuItem.Checked = True
            SetAsActiveMenuItem.Enabled = False
         End If
      Else
         Dim WorkingFolder
         Set WorkingFolder = MenuScript.Game.WorkingFolder

         ' Folder isn't below the game. Allow the menu item to set working folder
         SetAsActiveMenuItem.MenuName = "Working Folder"

         ' If this folder isn't currently the working folder, then enable it
         If Context (1).Exists (ActiveFolder) Then
            SetAsActiveMenuItem.Enabled = True
            If WorkingFolder Is Nothing Then
               SetAsActiveMenuItem.Event = "OnFolderSetWorking"
            ElseIf WorkingFolder = Context (1).Item (1) Then
               SetAsActiveMenuItem.Event = "OnFolderClearWorking"
               SetAsActiveMenuItem.Checked = True
            Else
               SetAsActiveMenuItem.Event = "OnFolderSetWorking"
            End If
         End If
      End If
   End If

   ' Determine whether the item has any XML that can be viewed
   If CanViewXML (Context) Then
      ' Do nothing, item is already enabled
   Else
      FolderMenu.MenuItems ("View XML").Enabled = False
   End If
    
   ' Setup the view menu
   SetupViewMenu FolderMenu.MenuItems ("View"), Context

   ' Setup the cut/copy/paste menus
   SetupClipboardMenu FolderMenu, Context

   ' Setup the move up/move down menus
   SetupMoveMenu FolderMenu, Context

   ' Setup the reset item.
   SetupResetEntityMenu FolderMenu, Context

   ' Setup NXN alienbrain menu
   SetupAlienbrainMenu FolderMenu.MenuItems ("NXN alienbrain"), Context
   
   ' Return the index of the item we want to display
   SetupFolderMenu = FolderMenu.Index
End Function

' -----------------------------------------------------------------------------
' Sets up the context menu for a behavior folder. Returns the index of the
' behavior folder menu
Function SetupBehaviorFolderMenu (Menu, Context)
   Dim BFMenu
   Set BFMenu = Menu.MenuItems ("BehaviorFolder")

   ' Setup the view menu
   SetupViewMenu BFMenu.MenuItems ("View"), Context

   ' Return the index of the item we want to display
   SetupBehaviorFolderMenu = BFMenu.Index
End Function

' ------------------------------------------------------------------------------
' Sets up the context menu for a behavior. Returns the index of the behavior menu
Function SetupBehaviorMenu (Menu, Context)
   Dim BehaviorMenu
   Set BehaviorMenu = Menu.MenuItems ("Behavior")

   ' Setup the view menu
   SetupViewMenu BehaviorMenu.MenuItems ("View"), Context

   ' Setup the cut/copy/paste menus
   SetupClipboardMenu BehaviorMenu, Context

   ' Return the index of the behavior menu
   SetupBehaviorMenu = BehaviorMenu.Index

   ' Determine whether the item has any source that can be viewed
   If CanViewSource (Context) Then
      BehaviorMenu.MenuItems ("View Source").Enabled = True
   Else
      BehaviorMenu.MenuItems ("View Source").Enabled = False
   End If
   
   ' Determine whether the item has any help that can be viewed
   If Len (GetHelpURL (Context)) > 0 Then
      BehaviorMenu.MenuItems ("View Help").Enabled = True
   Else
      BehaviorMenu.MenuItems ("View Help").Enabled = False
   End If
End Function

' ------------------------------------------------------------------------------
' Sets up the context menu for a command. Returns the index of the command menu
Function SetupCommandMenu (Menu, Context)
   Dim CommandMenu
   Set CommandMenu = Menu.MenuItems ("Command")

   ' Setup the view menu
   SetupViewMenu CommandMenu.MenuItems ("View"), Context

   ' Determine whether the item has any source that can be viewed 
   If CanViewSource (Context) Then
      CommandMenu.MenuItems ("View Source").Enabled = True 
   Else 
      CommandMenu.MenuItems ("View Source").Enabled = False 
   End If

   ' Return the index of the command menu
   SetupCommandMenu = CommandMenu.Index
End Function

' ------------------------------------------------------------------------------
' Sets up the context menu for an attribute. Returns the index of the attribute
' menu
Function SetupAttributeMenu (Menu, Context)
   Dim AttributeMenu
   Set AttributeMenu = Menu.MenuItems ("Attribute")

   ' Setup the view menu
   SetupViewMenu AttributeMenu.MenuItems ("View"), Context

   ' Setup the cut/copy/paste menus
   SetupClipboardMenu AttributeMenu, Context

   ' Determine whether the item has any XML that can be viewed
   If CanViewXML (Context) Then  
      ' Do nothing, item is already enabled
   Else  
      AttributeMenu.MenuItems ("View XML").Enabled = False  
   End If

   ' Setup NXN alienbrain menu
   SetupAlienbrainMenu AttributeMenu.MenuItems ("NXN alienbrain"), Context
   
   ' Determine whether the item has any source that can be viewed  
   If CanViewSource (Context) Then
      AttributeMenu.MenuItems ("View Source").Enabled = True  
   Else  
      AttributeMenu.MenuItems ("View Source").Enabled = False  
   End If

   ' Return the index of the attribute menu
   SetupAttributeMenu = AttributeMenu.Index
End Function

' ------------------------------------------------------------------------------
' Sets up the context menu for an asset. This can either return the asset menu
' or the referenced asset menu depending upon whether the asset is parented by
' an asset folder or an entity/folder.
Function SetupAssetMenu (Menu, Context)
   Dim AssetMenu, DeleteRemoveItem, AssetParent
   Set AssetMenu = Menu.MenuItems ("Asset")
   Set DeleteRemoveItem = AssetMenu.MenuItems ("<DeleteRemove>")

   ' Determine which menu to display
   If Context (1).Count > 1 Then
      Set AssetParent = Context (1).Item (2)
      Select Case AssetParent.IDType
      Case otAssetFolder
         DeleteRemoveItem.MenuName = "Delete"
         DeleteRemoveItem.Event    = "OnDelete"
      Case otEntity, otFolder, otTemplate, otTemplateFolder
         DeleteRemoveItem.MenuName = "Remove"
         DeleteRemoveItem.Event    = "OnRemove"
      Case otAsset
         DeleteRemoveItem.MenuName = "Delete"
         DeleteRemoveItem.Enabled  = False
      Case Else
         Stop ' Fire up the debugger
      End Select
   Else
      ' No parent - disable delete
      DeleteRemoveItem.MenuName = "Delete"
      DeleteRemoveItem.Enabled = False
   End If

   ' Setup the activate menu
   If Not Context.CanActivate Then
      AssetMenu.MenuItems ("Activate Preview").Enabled = False
   End If

   ' Setup the view menu
   SetupViewMenu AssetMenu.MenuItems ("View"), Context

   ' Setup the cut/copy/paste menus
   SetupClipboardMenu AssetMenu, Context

   ' Setup the move up/move down menus
   SetupMoveMenu AssetMenu, Context

   ' Setup the rename menu - can't rename sub-assets.
   AssetMenu.MenuItems ("Rename").Enabled = Context.CanRename

   ' Determine whether the item has any XML that can be viewed   
   If CanViewXML (Context) Then   
      ' Do nothing, item is already enabled
   Else   
      AssetMenu.MenuItems ("View XML").Enabled = False   
   End If
   
   ' Setup NXN alienbrain menus
   Dim AssetNxNMenu
   Set AssetNxNMenu = AssetMenu.MenuItems ("NXN alienbrain")
   SetupAlienbrainMenu AssetNxNMenu.MenuItems ("Studio Asset"), Context
   SetupResourceAlienbrainMenu AssetNxNMenu.MenuItems ("Asset's Resource"), Context

   ' Setup the Rebuild and Reload menu item
   SetupRebuildAndReloadMenu AssetMenu, Context
   
   ' Return the index of the asset menu
   SetupAssetMenu = AssetMenu.Index
End Function

' ------------------------------------------------------------------------------
' Sets up the context menu for an asset folder. The menu must be modified to
' determine whether the focus item can be removed or deleted
Function SetupAssetFolderMenu (Menu, Context)
   Dim AFMenu, DeleteRemoveItem, AFParent
   Set AFMenu = Menu.MenuItems ("AssetFolder")
   Set DeleteRemoveItem = AFMenu.MenuItems ("<DeleteRemove>")

   ' Determine whether we can delete or remove this item
   If Context (1).Count > 1 Then
      Set AFParent = Context (1).Item (2)
      Select Case AFParent.IDType
      Case otAssetFolder
         DeleteRemoveItem.MenuName = "Delete"
         DeleteRemoveItem.Event    = "OnDelete"
      Case otEntity, otFolder, otTemplate, otTemplateFolder
         DeleteRemoveItem.MenuName = "Remove"
         DeleteRemoveItem.Event    = "OnRemove"
      Case Else
         Stop ' Start up debugger. This shouldn't happen.
      End Select
   Else
      ' No parent - disable delete
      DeleteRemoveItem.MenuName = "Delete"
      DeleteRemoveItem.Enabled = False
   End If

   ' Setup the view menu
   SetupViewMenu AFMenu.MenuItems ("View"), Context

   ' Setup the cut/copy/paste menus
   SetupClipboardMenu AFMenu, Context

   ' Setup the move up/move down menus
   SetupMoveMenu AFMenu, Context

   ' Determine whether the item has any XML that can be viewed    
   If CanViewXML (Context) Then    
      ' Do nothing, item is already enabled
   Else    
      AFMenu.MenuItems ("View XML").Enabled = False    
   End If

   ' Setup NXN alienbrain menu
   SetupAlienbrainMenu AFMenu.MenuItems ("NXN alienbrain"), Context
   
   ' Return the index of the asset folder menu
   SetupAssetFolderMenu = AFMenu.Index
End Function

' ------------------------------------------------------------------------------
' Sets up the context menu for an entity. The menu must be modified to
' determine whether the focus item can be removed or deleted
Function SetupEntityMenu (Menu, Context)
   Dim EntityMenu, Entity, DeleteRemoveItem
   Set EntityMenu = Menu.MenuItems ("Entity")
   Set DeleteRemoveItem = EntityMenu.MenuItems.Item("<DeleteRemove>")

   ' Determine whether we can delete or remove this item
   Set Entity = GetFocusObject (Context)
   If Entity.ParentFolders.Count > 1 Then
      DeleteRemoveItem.MenuName = "Remove"
      DeleteRemoveItem.Event    = "OnRemove"
   Else
      DeleteRemoveItem.MenuName = "Delete"
      DeleteRemoveItem.Event    = "OnDelete"
   End If

   ' Setup the view menu
   SetupViewMenu EntityMenu.MenuItems ("View"), Context

   ' Setup the cut/copy/paste menus
   SetupClipboardMenu EntityMenu, Context

   ' Setup the move up/move down menus
   SetupMoveMenu EntityMenu, Context

   ' Determine whether the item has any XML that can be viewed     
   If CanViewXML (Context) Then     
      ' Do nothing, item is already enabled
   Else     
      EntityMenu.MenuItems ("View XML").Enabled = False     
   End If

   ' Setup whether to gray out the "Share Attributes" item.
   SetupShareAttributesMenu EntityMenu, Context

   ' We always enable the "Use Attributes" item,
   '       as determining whether we're allowed requires too much Database interrogation

   ' Setup the reset item.
   SetupResetEntityMenu EntityMenu, Context

   ' Setup NXN alienbrain menu
   SetupAlienbrainMenu EntityMenu.MenuItems ("NXN alienbrain"), Context
   
   ' Return the index of the entity menu
   SetupEntityMenu = EntityMenu.Index
End Function

' ------------------------------------------------------------------------------
' Sets up the context menu for an attribute share.
Function SetupAttributeShareMenu (Menu, Context)
   Dim ASMenu, ASParent, DeleteRemoveItem, Temp
   Set ASMenu = Menu.MenuItems ("AttributeShare")
   Set DeleteRemoveItem = ASMenu.MenuItems ("<DeleteRemove>")
   Set Temp = DeleteRemoveItem

   If Context (1).Count > 1 Then
      Set ASParent = Context (1).Item (2)
      Select Case ASParent.IDType
      Case otAttributeShareFolder
         DeleteRemoveItem.MenuName = "Delete"
         DeleteRemoveItem.Event    = "OnDelete"
      Case otEntity, otTemplate
         DeleteRemoveItem.MenuName = "Remove"
         DeleteRemoveItem.Event    = "OnRemove"
      Case Else
         Stop ' Fire up the debugger. Unhandled parent type.
      End Select
   Else
      ' No parent - disable delete
      DeleteRemoveItem.MenuName = "Delete"
      DeleteRemoveItem.Enabled = False
   End If

   ' Setup the view menu
   SetupViewMenu ASMenu.MenuItems ("View"), Context

   ' Setup the move up/move down menus
   SetupMoveMenu ASMenu, Context

   ' Determine whether the item has any XML that can be viewed      
   If CanViewXML (Context) Then      
      ' Do nothing, item is already enabled
   Else      
      ASMenu.MenuItems ("View XML").Enabled = False      
   End If
   
   ' Setup NXN alienbrain menu
   SetupAlienbrainMenu ASMenu.MenuItems ("NXN alienbrain"), Context
   
   ' Return the index of the attribute share menu
   SetupAttributeShareMenu = ASMenu.Index
End Function

' ------------------------------------------------------------------------------
' Sets up the context menu for an attribute share folder.
Function SetupAttributeShareFolderMenu (Menu, Context)
   Dim ASFMenu
   Set ASFMenu = Menu.MenuItems ("AttributeShareFolder")

   ' Setup the view menu
   SetupViewMenu ASFMenu.MenuItems ("View"), Context

   ' Setup the move up/move down menus
   SetupMoveMenu ASFMenu, Context

   ' Setup the cut/copy/paste menus
   SetupClipboardMenu ASFMenu, Context

   ' Setup NXN alienbrain menu
   SetupAlienbrainMenu ASFMenu.MenuItems ("NXN alienbrain"), Context
   
   ' Return the index of the attribute share folder menu
   SetupAttributeShareFolderMenu = ASFMenu.Index
End Function

' ------------------------------------------------------------------------------
' Sets up the context menu for a template.
Function SetupTemplateMenu (Menu, Context)
   Dim TemplateMenu
   Set TemplateMenu = Menu.MenuItems ("Template")

   ' Setup the view menu
   SetupViewMenu TemplateMenu.MenuItems ("View"), Context

   ' Setup the cut/copy/paste menus
   SetupClipboardMenu TemplateMenu, Context

   ' Setup the move up/move down menus
   SetupMoveMenu TemplateMenu, Context

   ' Determine whether the item has any XML that can be viewed       
   If CanViewXML (Context) Then       
      ' Do nothing, item is already enabled
   Else       
      TemplateMenu.MenuItems ("View XML").Enabled = False       
   End If
   
   ' Setup NXN alienbrain menu
   SetupAlienbrainMenu TemplateMenu.MenuItems ("NXN alienbrain"), Context
   
   ' Return the index of the template menu
   SetupTemplateMenu = TemplateMenu.Index
End Function

' ------------------------------------------------------------------------------
' Sets up the context menu for a template folder.
Function SetupTemplateFolderMenu (Menu, Context)
   Dim TFMenu
   Set TFMenu = Menu.MenuItems ("TemplateFolder")

   ' Setup the view menu
   SetupViewMenu TFMenu.MenuItems ("View"), Context

   ' Setup the cut/copy/paste menus
   SetupClipboardMenu TFMenu, Context

   ' Setup the move up/move down menus
   SetupMoveMenu TFMenu, Context

   ' Determine whether the item has any XML that can be viewed        
   If CanViewXML (Context) Then        
      ' Do nothing, item is already enabled
   Else        
      TFMenu.MenuItems ("View XML").Enabled = False        
   End If
   
   ' Setup NXN alienbrain menu
   SetupAlienbrainMenu TFMenu.MenuItems ("NXN alienbrain"), Context
   
   ' Return the index of the template folder menu
   SetupTemplateFolderMenu = TFMenu.Index
End Function

' ------------------------------------------------------------------------------
' Sets up the game database menu. This is simply the view menu
Function SetupGameDatabaseMenu (Menu, Context)
   Dim GDBMenu
   Set GDBMenu = Menu.MenuItems ("GameDatabase")

   ' Setup the view menu
   SetupViewMenu GDBMenu.MenuItems ("View"), Context

   ' Return the index of the game database menu
   SetupGameDatabaseMenu = GDBMenu.Index
End Function

' ------------------------------------------------------------------------------
' Sets up the behavior lister menu. The behavior folder menu will do fine.
Function SetupBehaviorListerMenu (Menu, Context)
   SetupBehaviorListerMenu = SetupBehaviorFolderMenu (Menu, Context)
End Function

' ------------------------------------------------------------------------------
' Sets up the default asset lister menu.
Function SetupAssetListerMenu (Menu, Context)
   Dim ALMenu
   Set ALMenu = Menu.MenuItems ("AssetLister")

   ' Setup the view menu
   SetupViewMenu ALMenu.MenuItems ("View"), Context

   ' Setup the cut/copy/paste menu items
   SetupClipboardMenu ALMenu, Context

   ' Setup NXN alienbrain menu
   SetupAlienbrainMenu ALMenu.MenuItems ("NXN alienbrain"), Context

   ' Return the index of the asset lister menu
   SetupAssetListerMenu = ALMenu.Index
End Function

' ------------------------------------------------------------------------------
' Sets up the default template lister menu.
Function SetupTemplateListerMenu (Menu, Context)
   Dim TLMenu
   Set TLMenu = Menu.MenuItems ("TemplateLister")

   ' Setup the view menu
   SetupViewMenu TLMenu.MenuItems ("View"), Context

   ' Setup the cut/copy/paste menu items
   SetupClipboardMenu TLMenu, Context

   ' Setup NXN alienbrain menu
   SetupAlienbrainMenu TLMenu.MenuItems ("NXN alienbrain"), Context

   ' Return the index of the template lister menu
   SetupTemplateListerMenu = TLMenu.Index
End Function

' ------------------------------------------------------------------------------
' Sets up the default attribute share lister menu.
Function SetupAttributeShareListerMenu (Menu, Context)
   Dim ASLMenu
   Set ASLMenu = Menu.MenuItems ("AttributeShareLister")

   ' Setup the view menu
   SetupViewMenu ASLMenu.MenuItems ("View"), Context

   ' Setup the cut/copy/paste menu items
   SetupClipboardMenu ASLMenu, Context

   ' Setup NXN alienbrain menu
   SetupAlienbrainMenu ASLMenu.MenuItems ("NXN alienbrain"), Context

   ' Return the index of the attribute share lister menu
   SetupAttributeShareListerMenu = ASLMenu.Index
End Function

' ------------------------------------------------------------------------------
' Sets up the search results lister menu. This is simply the view menu,
' so the game database menu can be re-used
Function SetupSearchResultsListerMenu (Menu, Context)
   Dim SRLMenu
   Set SRLMenu = Menu.MenuItems ("GameDatabase")

   ' Setup the view menu
   SetupViewMenu SRLMenu.MenuItems ("View"), Context

   ' Return the index of the search results lister menu
   SetupSearchResultsListerMenu = SRLMenu.Index
End Function

' ------------------------------------------------------------------------------
Sub DisableMenuItems (MenuItem)
   Dim SubItem

   For Each SubItem In MenuItem.MenuItems
      SubItem.Enabled = False
      DisableMenuItems SubItem
   Next
End Sub

' ------------------------------------------------------------------------------
' Returns the focus item in the given context
Function GetFocusObject (Context)
   Set GetFocusObject = Context(1).Item(1)
End Function

' ------------------------------------------------------------------------------
' Displays a menu correct for the given context at the given location.
'
' CloneMenu() is used to take a copy of the menu so that we can gray out, and
' optionally add/remove, items from the menu defined in Enterprise Author.
' This cloned menu will be discarded when you select an item, so the next time
' you show the context menu, you start from a clean menu.
Sub DisplayListerMenu (Context, nX, nY, hWnd)
   Dim Menu, MenuIndex
   Set Menu = ContextMenu.CloneMenu ()
   MenuIndex = -1

   Set CurrentContext = Context
   CurrentContext.SetScript MenuScript

   If Context.IsNode (GameDatabaseNode) Then
      MenuIndex = SetupGameDatabaseMenu (Menu, Context)
      Broadcast.PreShowListerContextMenu Menu, GameDatabaseNode
   ElseIf Context.IsNode (BehaviorListerNode) Then
      MenuIndex = SetupBehaviorListerMenu (Menu, Context)
      Broadcast.PreShowListerContextMenu Menu, BehaviorListerNode
   ElseIf Context.IsNode (AssetListerNode) Then
      MenuIndex = SetupAssetListerMenu (Menu, Context)
      Broadcast.PreShowListerContextMenu Menu, AssetListerNode
   ElseIf Context.IsNode (TemplateListerNode) Then
      MenuIndex = SetupTemplateListerMenu (Menu, Context)
      Broadcast.PreShowListerContextMenu Menu, TemplateListerNode
   ElseIf Context.IsNode (AttributeShareListerNode) Then
      MenuIndex = SetupAttributeShareListerMenu (Menu, Context)
      Broadcast.PreShowListerContextMenu Menu, AttributeShareListerNode
   ElseIf Context.IsNode (SearchResultsListerNode) Then
      MenuIndex = SetupSearchResultsListerMenu (Menu, Context)
      Broadcast.PreShowListerContextMenu Menu, SearchResultsListerNode
   Else
      Select Case GetFocusObject (Context).IDType
      Case otGame
         MenuIndex = SetupGameMenu (Menu, Context)
      Case otEntity
         MenuIndex = SetupEntityMenu (Menu, Context)
      Case otFolder
         MenuIndex = SetupFolderMenu (Menu, Context)
      Case otAsset
         MenuIndex = SetupAssetMenu (Menu, Context)
      Case otAssetFolder
         MenuIndex = SetupAssetFolderMenu (Menu, Context)
      Case otTemplate
         MenuIndex = SetupTemplateMenu (Menu, Context)
      Case otTemplateFolder
         MenuIndex = SetupTemplateFolderMenu (Menu, Context)
      Case otBehavior
         MenuIndex = SetupBehaviorMenu (Menu, Context)
      Case otBehaviorFolder
         MenuIndex = SetupBehaviorFolderMenu (Menu, Context)
      Case otCommand
         MenuIndex = SetupCommandMenu (Menu, Context)
      Case otAttribute
         MenuIndex = SetupAttributeMenu (Menu, Context)
      Case otAttributeShare
         MenuIndex = SetupAttributeShareMenu (Menu, Context)
      Case otAttributeShareFolder
         MenuIndex = SetupAttributeShareFolderMenu (Menu, Context)
      Case otEvent
      End Select
      Broadcast.PreShowObjectContextMenu Menu, GetFocusObject (Context).IDType
   End If

   If Not MenuIndex = -1 Then
      If GlobalScript.RWSUtils.IsOldFileVersion Then
         DisableMenuItems Menu
      End If

      Menu.ShowAsContextMenu MenuIndex, nX, nY, hWnd
   End If

End Sub

' ------------------------------------------------------------------------------
' Decide if the "Share Attributes" menu item should be grayed out.
Sub SetupShareAttributesMenu (EntityMenu, Context)
   ' Check if there is more than one entity selected.
   If Context.SelectionCount > 1 Then
      EntityMenu.MenuItems.Item ("Share Attributes").Enabled = False
      Exit Sub
   End If

   ' Check if the single entity has a parsed behavior
   Dim Entity
   Set Entity = GetFocusObject (CurrentContext)
   If Entity.IDType = otEntity Then
      Dim Behavior
      Set Behavior = MenuScript.Behavior (Entity.Behavior)
      If Behavior Is Nothing Then
         EntityMenu.MenuItems.Item ("Share Attributes").Enabled = False
      End If
   End If
End Sub

' ------------------------------------------------------------------------------
' Menu Event - Sets the listers view mode to small icon
Sub OnViewSmallIcon ()
   CurrentContext.SetViewMode SmallIcon
End Sub

' ------------------------------------------------------------------------------
' Menu Event - Sets the listers view mode to small thumbnail
Sub OnViewSmallThumbnail ()
   CurrentContext.SetViewMode SmallThumbnail
End Sub

' ------------------------------------------------------------------------------
' Menu Event - Sets the listers view mode to large thumbnail
Sub OnViewLargeThumbnail ()
   CurrentContext.SetViewMode LargeThumbnail
End Sub

' ------------------------------------------------------------------------------
' Menu Event - Sets the listers view mode to detail/report view
Sub OnViewDetail ()
   CurrentContext.SetViewMode Detail
End Sub

' ------------------------------------------------------------------------------
' Menu Event - Sets the listers view mode to list view
Sub OnViewList ()
   CurrentContext.SetViewMode List
End Sub

' ------------------------------------------------------------------------------
' Menu Event - Sets the listers view mode to tree view
Sub OnViewTree ()
   CurrentContext.Lister.ViewMode = Tree
End Sub

' ------------------------------------------------------------------------------
' Displays a property sheet for each selected object
Sub OnProperties ()
   Dim Container, Selection
   Set Container = CreateObject ("CSL.RWSScript.RWSContainer")

   For Each Selection In CurrentContext
      Container.Add Selection (1)
   Next

   GlobalScript.RWSUtils.ShowProperties Container, False
End Sub

' ------------------------------------------------------------------------------ 
' Shows the stream info for the currently selected asset 
Sub OnViewStream () 
   Dim Container, Selection 
   Set Container = CreateObject ("CSL.RWSScript.RWSContainer") 
 
   StreamEditControl.ClearEditControls
   
   CSLRenderwareStudioWorkspace.ShowObject StreamViewer
   
   For Each Selection In CurrentContext
      StreamViewer.ShowAsset Selection (1)  
   Next 
 
End Sub

' ------------------------------------------------------------------------------
' Renames the focus item (The first item in the selection)
Sub OnRename ()
   If CurrentContext.CanRename Then
      CurrentContext.Rename
   End If
End Sub

' ------------------------------------------------------------------------------
' 'Cuts' the current context (focus and selection) into the clipboard
Sub OnCut ()
   If CurrentContext.CanCut Then
      CurrentContext.Cut
   End If
End Sub

' ------------------------------------------------------------------------------
' 'Copies' the current context (focus and selection) into the clipboard
Sub OnCopy ()
   If CurrentContext.CanCopy Then
      CurrentContext.Copy
   End If
End Sub

' ------------------------------------------------------------------------------
' 'Pastes' the clipboard onto the current context (focus and selection)
Sub OnPaste ()
   If CurrentContext.CanPaste Then
      CurrentContext.Paste
   End If
End Sub

' ------------------------------------------------------------------------------
' Swaps the focused item with the item above it in the hierarchy
Sub OnMoveUp ()
   If CurrentContext.CanMoveUp Then
      CurrentContext.MoveUp
   End If
End Sub

' ------------------------------------------------------------------------------
' Swaps the focused item with the item below it in the hierarchy
Sub OnMoveDown ()
   If CurrentContext.CanMoveDown Then
      CurrentContext.MoveDown
   End If
End Sub

' ------------------------------------------------------------------------------
' Returns a user friendly type string for the given object
Function GetObjectType (APIObject)
   Select Case APIObject.IDType
   Case otGame                   GetObjectType = "game"
   Case otEntity                 GetObjectType = "entity"
   Case otFolder                 GetObjectType = "folder"
   Case otAsset                  GetObjectType = "asset"
   Case otAssetFolder            GetObjectType = "asset folder"
   Case otTemplate               GetObjectType = "template"
   Case otTemplateFolder         GetObjectType = "template folder"
   Case otBehavior               GetObjectType = "behavior"
   Case otBehaviorFolder         GetObjectType = "behavior folder"
   Case otCommand                GetObjectType = "command"
   Case otAttribute              GetObjectType = "attribute"
   Case otAttributeShare         GetObjectType = "attribute share"
   Case otAttributeShareFolder   GetObjectType = "attribute share folder"
   Case otEvent                  GetObjectType = "event"
   Case Else                     GetObjectType = "unknown type"
   End Select
End Function

' ------------------------------------------------------------------------------
'
Function GetObjectName (APIObject, IncludeType)
   ' Optionally prefix the object name with the object type
   If IncludeType Then
      GetObjectName = GetObjectType (APIObject) & " """
   Else
      GetObjectName = """"
   End If

   ' Add the descriptive name to the string
   Select Case APIObject.IDType
   Case otGame, otEntity, otFolder, otAsset, otAssetFolder, otTemplate, _
        otTemplateFolder, otBehaviorFolder, otAttributeShare, otAttributeShareFolder
        GetObjectName = GetObjectName & APIObject.Name
   Case otBehavior
        GetObjectName = GetObjectName & APIObject.UID
   Case otCommand
        GetObjectName = GetObjectName & APIObject.Name
   Case otAttribute
        GetObjectName = GetObjectName & APIObject.Member
   Case otEvent
        GetObjectName = GetObjectName & APIObject.Name
   Case Else
        GetObjectName = GetObjectName & "Unknown Name"
   End Select

   GetObjectName = GetObjectName & """"
End Function

' ------------------------------------------------------------------------------
' Deletes all selected items
Sub OnDelete ()
   Dim Undo, Selection
   If CurrentContext.SelectionCount = 1 Then
      ' Delete single object
      Set Selection = CurrentContext (1)
      Set Undo = MenuScript.CreateUndo ("Deleting " & GetObjectName (Selection(1), True))

      DeleteObject Selection (1), MenuScript, Undo
   Else
      ' Delete multiple objects
      Set Undo = MenuScript.CreateUndo ("Deleting multiple objects")
      For Each Selection In CurrentContext
         DeleteObject Selection (1), MenuScript, Undo
      Next
   End If

   ' Tell the selection object (for this lister) to clear whatever was selected (now deleted)
   Dim ContextMenuSelection
   Set ContextMenuSelection = CreateObject("CSL.RWSSelection.Selection")
   ContextMenuSelection.SelectionIdentifier = CurrentContext.Lister.SelectionIdentifier
   ContextMenuSelection.Clear
End Sub

' ------------------------------------------------------------------------------
' Deletes a single item, rolling back the undo stack if it fails.
' The most likely failure would be from failing to check out the object, or one
' of its dependencies from source control.
Sub DeleteObject (Object, objScript, objUndo)
   If GlobalScript.QueryDeleteObject (Object) Then
      If Not objScript.Delete (Object) Then
         objUndo.Rollback
         CSLRenderwareStudioWorkspace.ShowObject MessageLog
         MessageLog.Error "Unable to delete " & GetObjectName (Object, True) & ". " & _
                          "Ensure that the object and its dependencies can be modified."
      End If
   End If
End Sub

' ------------------------------------------------------------------------------
' Removes all selected items from their parent
Sub OnRemove ()
   Dim Selection, Undo
   If CurrentContext.SelectionCount = 1 Then

      Set Selection = CurrentContext (1)

      ' Removing a single object
      ' Create an undo to represent the operation
      Select Case Selection (1).IDType
      Case otAsset, otEntity, otFolder, otAssetFolder, otAttributeShare
         Set Undo = MenuScript.CreateUndo ("Removing " & GetObjectName (Selection (1), True)& _
                                           " from " & GetObjectName (Selection (2), True))
      End Select

      ' Remove the object from its parent
      On Error Resume Next
      Select Case Selection (1).IDType
      Case otAsset
         Selection (2).RemoveChildAsset Selection (1)
      Case otEntity
         Selection (2).RemoveChildEntity Selection (1)
      Case otFolder
         Selection (2).RemoveChildFolder Selection (1)
      Case otAssetFolder
         Selection (2).RemoveChildAssetFolder Selection (1)
      Case otAttributeShare
         Selection (2).RemoveChildAttributeShare Selection (1)
      End Select
      On Error Goto 0
   Else
      ' Removing multiple objects from their parents
      ' Create an undo
      Set Undo = MenuScript.CreateUndo ("Removing multiple objects")

      On Error Resume Next
      For Each Selection In CurrentContext
         ' Remove the object from its parent
         Select Case Selection (1).IDType
         Case otAsset
            Selection (2).RemoveChildAsset Selection (1)
         Case otEntity
            Selection (2).RemoveChildEntity Selection (1)
         Case otFolder
            Selection (2).RemoveChildFolder Selection (1)
         Case otAssetFolder
            Selection (2).RemoveChildAssetFolder Selection (1)
         Case otAttributeShare
            Selection (2).RemoveChildAttributeShare Selection (1)
         End Select
      Next
      On Error Goto 0
   End If
End Sub

' ------------------------------------------------------------------------------ 
' Returns whether there is an XML to be viewed in the current context.
Function CanViewXML (TheContext)
   Dim Selection, XMLFile
   Set Selection = TheContext (1) 

   If Selection (1).IDType = otAttribute Then
      ' For attributes, we need to view the appropriate parent's xml. 
      If Selection (1).ParentAttributeShares.Count > 0 Then 
         XMLFile = Selection (1).ParentAttributeShares(1).PersistentFilename
      ElseIf Selection (1).ParentEntities.Count > 0 Then  
         XMLFile = Selection (1).ParentEntities(1).PersistentFilename
      End If 
   Else
       XMLFile = Selection (1).PersistentFilename
   End If

   CanViewXML = False 
   If Len (XMLFile) > 0 Then
      Dim fso
      Set fso = CreateObject ("Scripting.FileSystemObject") 
      If fso.FileExists (XMLFile) Then
         CanViewXML = True
      End If
   End If

End Function

' ------------------------------------------------------------------------------
' Displays the XML file of the currently selected items
Sub OnViewXML ()
   Dim Selection, Shell
   Set Shell = CreateObject ("Shell.Application")

   For Each Selection In CurrentContext
      Dim XMLFile

      If Selection (1).IDType = otAttribute Then
         ' For attributes, we need to view the appropriate parent's xml.
         If Selection (1).ParentAttributeShares.Count > 0 Then 
            XMLFile = Selection (1).ParentAttributeShares(1).PersistentFilename 
         ElseIf Selection (1).ParentEntities.Count > 0 Then  
            XMLFile = Selection (1).ParentEntities(1).PersistentFilename 
         End If 
      Else
         XMLFile = Selection (1).PersistentFilename
      End If
      
      If Not Len (XMLFile) = 0 Then
         ' The shell will launch the XMLFile with the default browser
         Shell.ShellExecute XMLFile
      End If
   Next
End Sub

' ------------------------------------------------------------------------------
' Displays the NxN alienbrain history dialog
Sub OnShowNxNHistory () 
   Dim Selection
 
   For Each Selection In CurrentContext 
      Dim XMLFile 
 
      If Selection (1).IDType = otAttribute Then 
         ' For attributes, we need to view the appropriate parent's xml. 
         Dim ParentObject
         Set ParentObject = Database.GetAttributesParent (Selection (1))
         If Not ParentObject Is Nothing Then
            XMLFile = ParentObject.PersistentFilename  
         End If
      Else 
         XMLFile = Selection (1).PersistentFilename 
      End If 
       
      If Not Len (XMLFile) = 0 Then 
         ' Show history dialog
         VersionControlImpl.ShowFileHistoryDialog (XMLFile)
      End If 
   Next 
End Sub 

' ------------------------------------------------------------------------------ 
' Displays the NxN alienbrain properties dialog
Sub OnShowNxNProperties () 
   Dim Selection
 
   For Each Selection In CurrentContext 
      Dim XMLFile 
 
      If Selection (1).IDType = otAttribute Then 
         ' For attributes, we need to view the appropriate parent's xml. 
         Dim ParentObject
         Set ParentObject = Database.GetAttributesParent (Selection (1))
         If Not ParentObject Is Nothing Then
            XMLFile = ParentObject.PersistentFilename  
         End If
      Else 
         XMLFile = Selection (1).PersistentFilename 
      End If 
       
      If Not Len (XMLFile) = 0 Then 
         ' Show properties dialog
         VersionControlImpl.ShowFilePropertiesDialog (XMLFile)
      End If 
   Next 
End Sub 

' ------------------------------------------------------------------------------ 
' Displays the NxN alienbrain history dialog for a resource file
Sub OnResourceShowNxNHistory () 
   Dim Selection
   For Each Selection In CurrentContext 
      Dim FileName
      FileName = Persist.BuildPath (GlobalScript.g_strResourceRoot, Selection (1).File)
      
      If Not Len (FileName) = 0 Then 
         ' Show history dialog
         VersionControlImpl.ShowFileHistoryDialog (FileName)
      End If 
   Next 
End Sub 

' ------------------------------------------------------------------------------ 
' Displays the NxN alienbrain properties dialog for a resource file
Sub OnResourceShowNxNProperties () 
   Dim Selection
   For Each Selection In CurrentContext 
      Dim FileName
      FileName = Persist.BuildPath (GlobalScript.g_strResourceRoot, Selection (1).File)
      
      If Not Len (FileName) = 0 Then 
         ' Show properties dialog
         VersionControlImpl.ShowFilePropertiesDialog (FileName)
      End If 
   Next 
End Sub 

' ------------------------------------------------------------------------------
' Reparses the source code
Sub OnParseAllSource ()
   GlobalScript.ParseSource
End Sub

' ------------------------------------------------------------------------------  
' Display the help 
Function GetHelpURL (TheContext)
   Dim Selection, strHelpURL
   Set Selection = TheContext (1)

   strHelpURL = ""

   ' Help for a single object
   Select Case Selection (1).IDType
   Case otBehavior
      ' Look for the help for this behavior
      Dim strBehaviorHelp, PathArray, fso, strSourceRoot
      Set fso = CreateObject ("Scripting.FileSystemObject")

      ' Split the line up into semi-colon separated strings
      PathArray = Split (GlobalScript.g_strSourceRoot, ";")
      For Each strSourceRoot In PathArray
         strBehaviorHelp = fso.BuildPath (strSourceRoot, "Help")
         strBehaviorHelp = fso.BuildPath (strBehaviorHelp, Selection (1).UID & ".htm")
         If fso.FileExists (strBehaviorHelp) Then
            ' Display the help
            strHelpURL = strBehaviorHelp
            Exit For
         End If
      Next
   End Select
   On Error Goto 0

   GetHelpURL = strHelpURL
End Function

' ------------------------------------------------------------------------------ 
' Display the help
Sub OnShowHelp ()
   If CurrentContext.SelectionCount = 1 Then 
      CSLRenderwareStudioWorkspace.ShowObject HelpWindow
      HelpWindow.ShowHelpURL GetHelpURL (CurrentContext)
   End If
End Sub

' ------------------------------------------------------------------------------ 
' Returns whether there is a source file to be viewed in the current context. 
Function CanViewSource (TheContext)
   Dim Selection, Filename, Command, fso
   Set Selection = TheContext (1)  

   If Selection (1).IDType = otBehavior Then
      Filename = Selection (1).FileName
   ElseIf Selection (1).IDType = otCommand Then
      Filename = Selection (1).FileName
   ElseIf Selection (1).IDType = otAttribute Then
      Set Command = Selection (1).Command  
      If Not Command Is Nothing Then  
         Filename = Command.FileName
      End If  
   End If
   
   CanViewSource = False 
   If Len (Filename) > 0 Then
      Set fso = CreateObject ("Scripting.FileSystemObject") 
      If fso.FileExists (Filename) Then
         CanViewSource = True
      Else
         CSLRenderwareStudioWorkspace.ShowObject MessageLog
         MessageLog.Error "[Workspace] The source file doesn't exist: " & Filename
      End If
   End If
End Function

' ------------------------------------------------------------------------------
' Views the source code for the current behavior or command
Sub OnViewSource ()
   Dim Selection
   For Each Selection In CurrentContext
      Dim APIObject
      Set APIObject = Selection (1)
      GlobalScript.ViewSource APIObject  
   Next
End Sub

' ------------------------------------------------------------------------------
' Creates a new folder as a child of the focus item
Sub OnNewFolderAsChild ()
   Dim Parent, NewFolder, Undo
   Set Parent = GetFocusObject (CurrentContext)

   Set Undo = MenuScript.CreateUndo ()
   Set NewFolder = MenuScript.CreateFolder ()
   NewFolder.Name = "Folder <" & NewFolder.ID & ">"
   Undo.Description = "Created " & GetObjectName (NewFolder, True) & _
                      " as a child of " & GetObjectName (Parent, True)

   On Error Resume Next
   Parent.AddChildFolder NewFolder
   If Err.Number = 0 Then
      CurrentContext.ExpandAndFocus NewFolder ' Focus on this item
   End If
   On Error Goto 0
End Sub

' ------------------------------------------------------------------------------
' Create a template folder as a child of the focused template folder
Sub OnNewTemplateFolderAsChild ()
   Dim Parent, NewTemplateFolder, Undo
   Set Parent = GetFocusObject (CurrentContext)

   If Parent.IDType = otTemplateFolder Then
      Set Undo = MenuScript.CreateUndo ()
      Set NewTemplateFolder = MenuScript.CreateTemplateFolder ()
      NewTemplateFolder.Name = "Template Folder <" & NewTemplateFolder.ID & ">"
      Undo.Description = "Created " & GetObjectName (NewTemplateFolder, True) & _
                         " as child of " & GetObjectName (Parent, True)

      On Error Resume Next
      Parent.AddChildTemplateFolder NewTemplateFolder
      If Err.Number = 0 Then
         CurrentContext.ExpandAndFocus NewTemplateFolder
      End If
      On Error Goto 0
   End If
End Sub



' =============================================================================

Sub AddBoolProperty (Entity, boolPropertyUID, boolData)

   ' Add a string property for the given UID
   Dim Property, PropertyData, DataSection
   Set Property = GlobalScript.RWSScript.Property (boolPropertyUID)
   Set PropertyData = CreateObject ("CSL.RWSScript.RWSData")

   Set DataSection = PropertyData.Add (dtBool)
   DataSection.Add boolData


   ' Turn on the copy flag, so entites copied will also have this
   ' property  - 
   Property.Set Entity, PropertyData, Properties.sysflagCopyProperty

End Sub


' ------------------------------------------------------------------------------ 
Sub OnNewLightmapLightTemplate ()
  
   Dim fullPathname
   Dim Undo, Entity, newAsset, NewTemplate, paramListArray, Behaviour
   Dim assetFound

   Set Undo = MenuScript.CreateUndo () 
   
   ' Create an entity which references the light map behaviour
   Set Entity = MenuScript.CreateEntity ()
   Entity.Name = "LightMap light"
   Entity.Behavior = "LightMap Light"

   Entity.Platform = "None"   ' Set the entity up so that it doesn't get sent to the consoles

   AddBoolProperty Entity, Properties.conLightMapLight, True


   ' Create an asset for the entity to use 

   assetFound = False

   ' See if the light map asset already exists
   Dim Asset
   For Each Asset In MenuScript.Assets
      If Asset.Name = "LightMap Alias" Then
         assetFound = True
         Entity.AddChildAsset Asset

      End If
   Next

   if assetFound = False then

      Set newAsset = MenuScript.CreateAsset ()

      fullPathname          = AppSettings.StripEnvironment( "%RWSWORKSPACE%\Resources\LightSphere.dff" )
      newAsset.File         = GlobalScript.RWSUtils.GetRelativePath (fullPathname, ResourceRoot)

      fullPathname          = AppSettings.StripEnvironment( "%RWSWORKSPACE%\Resources\Textures\" )
      newAsset.Dependencies = GlobalScript.RWSUtils.GetRelativePath (fullPathname, ResourceRoot)
   
      newAsset.Name         = "LightMap Alias"
      newAsset.Type         = "rwID_CLUMP" 
      newAsset.Description  = "Asset representing light map light"
   
      Entity.AddChildAsset newAsset

   End if

   ' Create a template based on this entity
   Set NewTemplate  = Entity.CreateTemplate () 

   NewTemplate.Name = "LightMap Light"
   MenuScript.TemplateHierarchy.AddChildTemplate NewTemplate 
   
   Undo.Description = "Created template " & GetObjectName (NewTemplate, False) 
 
   ' Make sure we can see the new template
   CurrentContext.ExpandAndFocus NewTemplate 

   ' Can get rid of the entity, now we've created the template
   RWSScript.Delete Entity
  
End Sub

' ------------------------------------------------------------------------------
Sub OnNewAssetAsChild ()
   Dim Parent
   Set Parent = GetFocusObject (CurrentContext)

   If Parent.IDType    = otAssetFolder Then
      Dim NewAsset, Undo, fso, FullFilename, RelFilename, Name, DependencyPath 

      Set Undo     = MenuScript.CreateUndo ()
      Set fso      = CreateObject ("Scripting.FileSystemObject") 
      Set NewAsset = MenuScript.CreateAsset () 

      FullFilename = Persist.ShowAssetLocateFileDialog ()
      If Len (FullFilename) > 0 Then  
         Name           = fso.GetBaseName (FullFilename)
         RelFilename    = GlobalScript.RWSUtils.GetRelativePath (FullFilename, ResourceRoot)
         DependencyPath = fso.GetParentFolderName (RelFilename)
         If Len (DependencyPath) > 0 Then 
            DependencyPath = DependencyPath & "\" '" 
         End If
         NewAsset.File         = RelFilename
         NewAsset.Dependencies = DependencyPath & "Textures"
         NewAsset.Type         = GlobalScript.GetAssetFileType (NewAsset.ID, FullFilename) 

         ' Import the asset
         BuildScriptManager.PreImportAsset NewAsset

         ' Load it into RWStudio
         Dim ReloadMsg
         Set ReloadMsg = MenuScript.RegisterMessage (msgReloadAsset)
         ReloadMsg.Send NewAsset
      End If 

      If Len (Name) = 0 Then    
         Name = "Asset <" & NewAsset.ID & ">"  
      End If 
      NewAsset.Name    = Name
      Undo.Description = "Created " & GetObjectName (NewAsset, True) & _
                         " as child of " & GetObjectName (Parent, True)

      On Error Resume Next
      Parent.AddChildAsset NewAsset
      If Err.Number = 0 Then
         CurrentContext.ExpandAndFocus NewAsset
      End If
      On Error Goto 0
   End If
End Sub

' ------------------------------------------------------------------------------
Sub OnNewAssetFolderAsChild ()
   Dim Parent, NewAssetFolder, Undo
   Set Parent = GetFocusObject (CurrentContext)

   If Parent.IDType = otAssetFolder Then
      Set Undo = MenuScript.CreateUndo ()
      Set NewAssetFolder = MenuScript.CreateAssetFolder ()
      NewAssetFolder.Name = "Asset Folder <" & NewAssetFolder.ID & ">"
      Undo.Description = "Created " & GetObjectName (NewAssetFolder, True) & _
                         " as child of " & GetObjectName (Parent, True)

      On Error Resume Next
      Parent.AddChildAssetFolder NewAssetFolder
      If Err.Number = 0 Then
         CurrentContext.ExpandAndFocus NewAssetFolder
      End If
      On Error Goto 0
   End If
End Sub

' ------------------------------------------------------------------------------
Sub OnNewAttributeShareFolderAsChild ()
   Dim Parent, NewAttributeShareFolder, Undo
   Set Parent = GetFocusObject (CurrentContext)

   If Parent.IDType = otAttributeShareFolder Then
      Set Undo = MenuScript.CreateUndo ()
      Set NewAttributeShareFolder = MenuScript.CreateAttributeShareFolder ()
      NewAttributeShareFolder.Name = "Attribute Share Folder <" & NewAttributeShareFolder.ID & ">"
      Undo.Description = "Created " & GetObjectName (NewAttributeShareFolder, True) & _
                         " as child of " & GetObjectName (Parent, True)

      On Error Resume Next
      Parent.AddChildAttributeShareFolder NewAttributeShareFolder
      If Err.Number = 0 Then
         CurrentContext.ExpandAndFocus NewAttributeShareFolder
      End If
      On Error Goto 0
   End If
End Sub

' ------------------------------------------------------------------------------
' Activates the preview window associated with the current context
Sub OnAssetActivatePreview ()
   If CurrentContext.CanActivate Then
      CurrentContext.Activate
   End If
End Sub

' ------------------------------------------------------------------------------
' Reloads the graphics used to display the preview
Sub OnAssetReload ()
   Dim ReloadMsg, Selection
   Set ReloadMsg = MenuScript.RegisterMessage (msgReloadAsset)

   For Each Selection In CurrentContext
      Dim Asset, RootAsset
      Set Asset = Selection (1)
      If Asset.IDType = otAsset Then
         ReloadMsg.Send Asset
      End If
   Next
End Sub

' ------------------------------------------------------------------------------ 
Sub OnAssetRebuildAndReload
   Dim ReloadMsg, Selection 
   Set ReloadMsg = MenuScript.RegisterMessage (msgReloadAsset) 
 
   For Each Selection In CurrentContext 
      Dim Asset, RootAsset 
      Set Asset = Selection (1) 
      If Asset.IDType = otAsset Then 
 
         Set RootAsset = BuildScriptManager.GetRootRF3Asset (Asset) 
         If Not RootAsset Is Nothing Then 
            If Left (RootAsset.Type, 3) = "RF3" Then 
               Set Asset = RootAsset 
            End If 
         End If 
 
         ' If the root asset is a sequence asset, then trigger a save
         SequencerTools.SaveIfDirty Sequencer, RootAsset

         BuildScriptManager.PreReloadAsset Asset.ID 
         ' This message will be picked up by DesignView.RWSUtility 
         ReloadMsg.Send Asset 
      End If 
   Next
End Sub 

' ------------------------------------------------------------------------------  
Sub SetupAlienbrainMenu (AlienbrainMenu, Context) 
   Dim Selection, oAPIObject
   Set Selection = Context (1) 
   Set oAPIObject = Selection (1)
   If oAPIObject.IDType = otAttribute Then
      ' For attributes, we need to look at the appropriate parent's state. 
      Set oAPIObject = Database.GetAttributesParent (oAPIObject)
   End If

   If Not oAPIObject Is Nothing Then
      Dim PersistState
      PersistState = VersionControl.GetPersistStatusInformation (oAPIObject)
      EnableAlienbrainMenuItems AlienbrainMenu, PersistState
   End If
End Sub

' ------------------------------------------------------------------------------  
Sub SetupResourceAlienbrainMenu (AlienbrainMenu, Context) 
   Dim Selection, APIObject
   Set Selection = Context (1) 
   Set APIObject = Selection (1)

   Dim PersistState
   PersistState = VersionControl.GetStatusInformation (APIObject, VersionControl.conBIsResource)
   EnableAlienbrainMenuItems AlienbrainMenu, PersistState
End Sub

' ------------------------------------------------------------------------------  
Sub EnableAlienbrainMenuItems (AlienbrainMenu, PersistState)
   If PersistState < VersionControl.conStatePendingImport Then
      AlienbrainMenu.MenuItems ("Check Out...").Enabled = False
      AlienbrainMenu.MenuItems ("Submit Pending Changes...").Enabled = False
      AlienbrainMenu.MenuItems ("Undo Pending Changes...").Enabled = False
      AlienbrainMenu.MenuItems ("Get Latest Version").Enabled = False
      AlienbrainMenu.MenuItems ("Show History").Enabled = False
      AlienbrainMenu.MenuItems ("Properties").Enabled = False
   Else
      If PersistState = VersionControl.conStateCheckedOutToUser Then
         AlienbrainMenu.MenuItems ("Check Out...").Enabled = False
      ElseIf PersistState = VersionControl.conStatePendingImport Then
         ' All menu items are enabled
      Else
         AlienbrainMenu.MenuItems ("Submit Pending Changes...").Enabled = False
         AlienbrainMenu.MenuItems ("Undo Pending Changes...").Enabled = False
      End If
   End If
End Sub

' ------------------------------------------------------------------------------  
Sub SetupRebuildAndReloadMenu (AssetMenu, Context) 
   Dim bAtLeastOneRF3, Selection 
   bAtLeastOneRF3 = False  
    
   For Each Selection In Context    
      Dim Asset, RootAsset    
      Set Asset = Selection (1)    
      If Asset.IDType = otAsset Then    
    
         Set RootAsset = BuildScriptManager.GetRootRF3Asset (Asset)    
         If Not RootAsset Is Nothing Then    
            If Left (RootAsset.Type, 3) = "RF3" Then    
               bAtLeastOneRF3 = True
               Exit For
            End If    
         End If    
      End If    
   Next   
 
   ' If there is not at least 1 RF3 asset selected, then disable the menu item. 
   If Not bAtLeastOneRF3 Then 
      AssetMenu.MenuItems ("Rebuild and Reload").Enabled = False 
   End If 
End Sub 

' ------------------------------------------------------------------------------
' We can only reset the selected entity/entities/contained entities in a folder
' if there is a target console connected and the focussed item is in either the
' global or active folder.
Sub SetupResetEntityMenu (EntityMenu, Context)
   ' If we can't reset the entities, gray out the items.
   If Not CanResetEntity (Context) Then
      Dim oFocussedObject
      Set oFocussedObject = GetFocusObject (Context)
      If oFocussedObject.IDType = Database.otFolder Then
         EntityMenu.MenuItems ("Reset Contained Entities on Target(s)").Enabled = False
      ElseIf oFocussedObject.IDType = Database.otEntity Then
         EntityMenu.MenuItems ("Reset on Target(s)").Enabled = False
      End If
   End If
End Sub

' ------------------------------------------------------------------------------
' Creates a template of each selected entity
Sub OnEntityCreateTemplate ()
   Dim Selection, Entity, Template, Undo

   If CurrentContext.SelectionCount = 1 Then
      Set Entity = CurrentContext (1).Item (1)
      If Entity.IDType = otEntity Then
         Set Undo = MenuScript.CreateUndo ()

         Set Template  = Entity.CreateTemplate ()
         Template.Name = GetUniqueName ("Template", Entity, MenuScript.TemplateHierarchy)
         MenuScript.TemplateHierarchy.AddChildTemplate Template
         Undo.Description = "Created template " & GetObjectName (Template, False)
      End If
   Else
      Set Undo = MenuScript.CreateUndo ("Created multiple templates")
      For Each Selection In CurrentContext
         Set Entity = Selection (1)
         If Entity.IDType = otEntity Then
            MenuScript.TemplateHierarchy.AddChildTemplate Entity.CreateTemplate ()
         End If
      Next
   End If
End Sub

' ------------------------------------------------------------------------------
Function GetUniqueName (strPrefix, ObjBasedOn, ParentFolder)
   Dim testTemplate, strTemplateName, bFound, copyNumber

   strTemplateName = strPrefix & " of " & ObjBasedOn.Name
   If TemplateExists (MenuScript.TemplateHierarchy, strTemplateName) Then
      ' Template already exists with that name, choose another
      Dim bExists, nCount
      bExists = True
      nCount  = 2
      While bExists
         strTemplateName = strPrefix & " (" & nCount
         strTemplateName = strTemplateName & ") of " & ObjBasedOn.Name
         
         bExists = TemplateExists (ParentFolder, strTemplateName)
         nCount  = nCount + 1
      Wend
   End If

   GetUniqueName = strTemplateName
End Function

' ------------------------------------------------------------------------------
Function TemplateExists (TemplateFolder, strTemplateName)
   Dim Template
   For Each Template In TemplateFolder.ChildTemplates
      If Template.Name = strTemplateName Then
         TemplateExists = True
         Exit Function
      End If
   Next

   TemplateExists = False
End Function

' ------------------------------------------------------------------------------
' Creates a template folder for each folder currently selected
Sub OnCreateTemplateFolder ()
   Dim Selection, Folder, TemplateFolder, Undo
   If CurrentContext.SelectionCount = 1 Then
      Set Folder = CurrentContext (1).Item (1)
      If Folder.IDType = otFolder Then
         Set Undo = MenuScript.CreateUndo ()
         Set TemplateFolder = Folder.CreateTemplateFolder
         MenuScript.TemplateHierarchy.AddChildTemplateFolder TemplateFolder
         Undo.Description = "Created template folder " & GetObjectName (TemplateFolder, False)
      End If
   Else
      Set Undo = MenuScript.CreateUndo ("Created multiple template folders")
      For Each Selection In CurrentContext
         Set Folder = Selection (1)
         If Folder.IDType = otFolder Then
            Set TemplateFolder = Folder.CreateTemplateFolder
            MenuScript.TemplateHierarchy.AddChildTemplateFolder TemplateFolder
         End If
      Next
   End If
End Sub

' ------------------------------------------------------------------------------
' Hides entities and folders from begin visible in DesignView
Sub OnHideFromView ()
   Dim Selection
   For Each Selection In CurrentContext
      Dim APIObject
      Set APIObject = Selection (1)
      If APIObject.IDType = otEntity Or APIObject.IDType = otFolder Then
         GlobalScript.RWSUtils.ShowVisibility APIObject, False
      End If
   Next
End Sub

' ------------------------------------------------------------------------------
' Sets visible previously hidden entities and folders
Sub OnShowInView ()
   Dim Selection
   For Each Selection In CurrentContext
      Dim APIObject
      Set APIObject = Selection (1)
      If APIObject.IDType = otEntity Or APIObject.IDType = otFolder Then
         GlobalScript.RWSUtils.ShowVisibility APIObject, True
      End If
   Next
End Sub

' ------------------------------------------------------------------------------ 
' Unfreezes entites and folders 
Sub OnUnfreeze () 
   Dim Selection 
   For Each Selection In CurrentContext 
      Dim APIObject 
      Set APIObject = Selection (1) 
      If APIObject.IDType = otEntity Or APIObject.IDType = otFolder Then 
         GlobalScript.RWSUtils.FreezeObject APIObject, False 
      End If 
   Next 
End Sub 
 
' ------------------------------------------------------------------------------ 
' Freezes entities and folders from being pickable
Sub OnFreeze () 
   Dim Selection 
   For Each Selection In CurrentContext 
      Dim APIObject 
      Set APIObject = Selection (1) 
      If APIObject.IDType = otEntity Or APIObject.IDType = otFolder Then 
         GlobalScript.RWSUtils.FreezeObject APIObject, True 
      End If 
   Next 
End Sub

' ------------------------------------------------------------------------------
' Locates the focus entity in DesignView
Sub OnLocateInView ()
   Dim Entity
   Set Entity = GetFocusObject (CurrentContext)
   If Entity.IDType = otEntity Then
      DesignView.RWSUtility.LocateEntity Entity
      CSLRenderwareStudioWorkspace.ShowObject DesignView
   End If
End Sub

' ------------------------------------------------------------------------------
' Rotates the camera to be facing an entity
Sub OnAimAt ()
   Dim Entity
   Set Entity = GetFocusObject (CurrentContext)
   If Entity.IDType = otEntity Then
      DesignView.RWSUtility.AimAtEntity Entity
   End If
End Sub

' ------------------------------------------------------------------------------
' Displays the share attributes dialog for the currently focused entity
Sub OnEntityShareAttributes ()
   Dim Entity
   Set Entity = GetFocusObject (CurrentContext)
   If Entity.IDType = otEntity Then
      Dim Behavior
      Set Behavior = MenuScript.Behavior (Entity.Behavior)
      If Not Behavior Is Nothing Then
         Dim SharedAttributesDlg
         Set SharedAttributesDlg = CreateObject ("CSL.RWSUtils.SharedAttribute")
         SharedAttributesDlg.DisplayShareDialog Entity
      Else
         ' Can't display a dialog. Report an error to the output window.
         CSLRenderwareStudioWorkspace.ShowObject MessageLog
         MessageLog.Error "To ""Share Attributes"", the entity must have a parsed behavior"
      End If
   End If
End Sub

' ------------------------------------------------------------------------------
'
Function BuildEntityContainer (bDisplayWarning)
   Set BuildEntityContainer = Nothing
   ' Build a container of the validly-parsed, selected entities
   Dim EntityContainer, Selection, Entity, Behavior
   Set EntityContainer = CreateObject ("CSL.RWSScript.RWSContainer")

   For Each Selection In CurrentContext
      If Selection (1).IDType = otEntity Then
         Set Entity = Selection (1)
         Set Behavior = MenuScript.Behavior (Entity.Behavior)
         If Behavior Is Nothing Then
            If bDisplayWarning Then
               ' Can't display a dialog if no behavior exists for this entity
               CSLRenderwareStudioWorkspace.ShowObject MessageLog
               MessageLog.Error "To use attributes, the entity, " & Selection (1).Name & " must have a parsed behavior"
            End If
            Exit Function
         Else
            EntityContainer.Add Entity
         End If
      End If
   Next

   If EntityContainer.Count > 0 Then
      Set BuildEntityContainer = EntityContainer
   End If
End Function

' ------------------------------------------------------------------------------
'
Sub OnEntityUseAttributes ()

   ' First check if there are any attribute shares to use.
   ' This isn't done when the context menu is displayed as it can create an annoying progress bar.
   If RWSScript.AttributeShares.Count = 0 Then
      ' Can't display a dialog if no behavior exists for this entity
      CSLRenderwareStudioWorkspace.ShowObject MessageLog
      MessageLog.Warning "To use attributes, one or more attribute shares must exist."
      Exit Sub
   End If

   Dim EntityContainer
   Set EntityContainer = BuildEntityContainer (True)

   If Not EntityContainer Is Nothing Then
      Dim UseAttributes
      Set UseAttributes = CreateObject ("CSL.RWSUtils.SharedAttribute")
      If UseAttributes.CanDisplayUseDialog (EntityContainer) Then
         UseAttributes.DisplayUseDialog EntityContainer
      Else
         ' Can't display a dialog if no compatible attributes exist
         CSLRenderwareStudioWorkspace.ShowObject MessageLog
         MessageLog.Warning "To use attributes, at least one attribute share must contain " & _
                            "an attribute that belongs to the entity's behavior."
      End If
   End If
End Sub

' ------------------------------------------------------------------------------
' Sets the current context focus object as the active folder
Sub OnFolderSetActive ()
   MenuScript.Game.ActiveFolder = GetFocusObject (CurrentContext)
End Sub

' ------------------------------------------------------------------------------
' Sets the current context focus object as the working folder
Sub OnFolderSetWorking ()
   MenuScript.Game.WorkingFolder = GetFocusObject (CurrentContext)
End Sub

' ------------------------------------------------------------------------------  
'  
Sub OnFolderClearWorking ()
   MenuScript.Game.WorkingFolder = Nothing
End Sub

' ------------------------------------------------------------------------------
' Entities Using Behavior(s)/asset(s)
Sub OnSearchAll ()
   Searches.DoSearch false, false, "Find entities using "
End Sub

' ------------------------------------------------------------------------------
' Active Entities Using Behavior(s)/asset(s)
Sub OnSearchLevel ()
   Searches.DoSearch true, false, "Find active entities using "
End Sub

' ------------------------------------------------------------------------------
' Entities Using Asset(s) Type
Sub OnSearchAllType ()
   Searches.DoSearch false, true, "Find entities using "
End Sub

' ------------------------------------------------------------------------------
' Active Entities Using Asset(s) Type
Sub OnSearchLevelType ()
   Searches.DoSearch true, true, "Find active entities using "
End Sub

' ------------------------------------------------------------------------------
' Will check if the focussed entity or folder (from the current context) is
' within either the global or active folders.
Function CanResetEntity (Context)
   CanResetEntity = False

   ' If we are not conntected to at least one target, we can't reset.
   If GlobalScript.RWSComms.Connections.Count < 1 Then
      Exit Function
   End If

   ' Can only reset if object is a folder or entity.
   Dim oFocussedObject
   Set oFocussedObject = GetFocusObject (Context)
   If (oFocussedObject.IDType = Database.otFolder) Or _
      (oFocussedObject.IDType = Database.otEntity) Then
      Dim oGlobalFolder
      Set oGlobalFolder = GlobalScript.RWSScript.Game.GlobalFolder
      If Not oGlobalFolder Is Nothing Then
         If Database.IsInFolder (oFocussedObject, oGlobalFolder) Then
            CanResetEntity = True
            Exit Function
         End If
      End If

      ' The entity is not in the global folder, check the active folder.
      Dim oActiveFolder
      Set oActiveFolder = GlobalScript.RWSScript.Game.ActiveFolder
      If Not oActiveFolder Is Nothing Then
         If Database.IsInFolder (oFocussedObject, oActiveFolder) Then
            CanResetEntity = True
            Exit Function
         End If
      End If
   End If
End Function

' ------------------------------------------------------------------------------
'
Sub OnResetEntities ()
   If CanResetEntity (CurrentContext) Then
      Dim RWSObject, RWSObjectCtr
      For Each RWSObjectCtr In CurrentContext
         Set RWSObject = RWSObjectCtr.Item(1)
         Select Case RWSObject.IDType
         Case otEntity
            GlobalScript.ResetEntities (RWSObject.ID)
         Case otFolder
            GlobalScript.ResetEntities (RWSObject.ID)
         End Select
      Next
   End If
End Sub

' ------------------------------------------------------------------------------
'
Sub DeleteOrRemove (Context)
   Dim Delete, Remove
   Delete = False
   Remove = False

   Set CurrentContext = Context
   CurrentContext.SetScript MenuScript

   Select Case GetFocusObject (Context).IDType
   Case otEntity
      SetupEntityDeleteRemove Context, Delete, Remove
   Case otFolder
      SetupFolderDeleteRemove Context, Delete, Remove
   Case otAsset
      SetupAssetDeleteRemove Context, Delete, Remove
   Case otAssetFolder
      SetupAssetFolderDeleteRemove Context, Delete, Remove
   Case otTemplate, otTemplateFolder, otAttribute
      Delete = True
   Case otAttributeShare
      SetupAttributeShareDeleteRemove Context, Delete, Remove
   End Select

   If Delete Then
      OnDelete
   ElseIf Remove Then
      OnRemove
   End If
End Sub

' ------------------------------------------------------------------------------
' Sets up the delete/remove ability for the focused folder
Sub SetupFolderDeleteRemove (Context, Delete, Remove)
   ' Determine whether we can delete or remove this item
   Dim GlobalFolder, GlobalFolderID
   Set GlobalFolder = GlobalScript.RWSScript.Game.GlobalFolder
   If Not GlobalFolder Is Nothing Then
      GlobalFolderID = GlobalFolder.ID
   Else
      GlobalFolderID = 0
   End If

   Dim Folder
   Set Folder = GetFocusObject (Context)
   If Folder.ID = GlobalFolderID Then
      'Delete/remove disabled
   ElseIf Folder.ParentFolders.Count > 1 Then
      Remove = True
   Else
      Delete = True
   End If
End Sub

' ------------------------------------------------------------------------------
' Sets up the delete/remove ability for the focused asset
Sub SetupAssetDeleteRemove (Context, Delete, Remove)
   ' Determine whether we can delete or remove this item
   If Context (1).Count > 1 Then
      Dim AssetParent
      Set AssetParent = Context (1).Item (2)
      Select Case AssetParent.IDType
      Case otAssetFolder
         Delete = True
      Case otEntity, otFolder, otTemplate
         Remove = True
      Case otAsset
         'Delete/remove disabled
      Case Else
         Stop ' Fire up the debugger
      End Select
   End If
End Sub

' ------------------------------------------------------------------------------
' Sets up the delete/remove ability for the focused asset folder
Sub SetupAssetFolderDeleteRemove (Context, Delete, Remove)
   ' Determine whether we can delete or remove this item
   If Context (1).Count > 1 Then
      Dim AFParent
      Set AFParent = Context (1).Item (2)
      Select Case AFParent.IDType
      Case otAssetFolder
         Delete = True
      Case otEntity, otFolder
         Remove = True
      Case Else
         Stop ' Start up debugger. This shouldn't happen.
      End Select
   End If
End Sub

' ------------------------------------------------------------------------------
' Sets up the delete/remove ability for the focused entity
Sub SetupEntityDeleteRemove (Context, Delete, Remove)
   ' Determine whether we can delete or remove this item
   Dim Entity
   Set Entity = GetFocusObject (Context)
   If Entity.ParentFolders.Count > 1 Then
      Remove = True
   Else
      Delete = True
   End If
End Sub

' ------------------------------------------------------------------------------
' Sets up the delete/remove ability for the focused attribute share.
Sub SetupAttributeShareDeleteRemove (Context, Delete, Remove)
   ' Determine whether we can delete or remove this item
   If Context (1).Count > 1 Then
      Dim ASParent
      Set ASParent = Context (1).Item (2)
      Select Case ASParent.IDType
      Case otAttributeShareFolder
         Delete = True
      Case otEntity, otTemplate
         Remove = True
      Case Else
         Stop ' Fire up the debugger. Unhandled parent type.
      End Select
   End If
End Sub

' ------------------------------------------------------------------------------
'
Sub OnAssetEdit
   Dim Asset, strFileName
   Set Asset   = GetFocusObject (CurrentContext)
   strFileName = Persist.BuildPath (GlobalScript.g_strResourceRoot, Asset.File)

   Dim bCanEdit
   bCanEdit = True
   ' If this asset is a sequence, then we can always edit the asset since it
   ' will be checked out by the sequencer when an attempt is made to edit.
   ' Otherwise, ff strFilename (the asset's resource) is managed attempt to check out.
   If SequencerTools.IsSequence (Asset) Then
      bCanEdit = True
   ElseIf VersionControl.IsPathManaged (strFilename) Then
      If VersionControl.CheckOutFile (strFilename, true, true) = False Then
         bCanEdit = False
       Else
         MessageLog.Warning "Unable to check out the asset's resource for '" & Asset.name & "'"
      End If
   End If

   If bCanEdit Then
      If Asset.Type = "rwID_SPLINE" Then
         If RWSSpline.Edit (Asset) Then
            SplineToolbar.CheckMenuItem 1, False ' Uncheck New Nodes ready for Edit

            SplineToolbar.EnableMenuItem 1, True 'New Nodes
            SplineToolbar.EnableMenuItem 2, True 'Insert Node
            SplineToolbar.EnableMenuItem 3, True 'Delete Nodes
            SplineToolbar.EnableMenuItem 4, True 'Open Spline
            SplineToolbar.EnableMenuItem 5, True 'Close Spline
         End If

      ElseIf SequencerTools.IsSequence (Asset) Then
         ' If a sequence is loaded, then save it
         If SequencerTools.EnsureSequenceAssetSaved (Sequencer) Then
            ' Open the sequence specified by the asset
            Sequencer.OpenFile strFileName
            CSLRenderwareStudioWorkspace.ShowObject Sequencer
         End If

      ElseIf Asset.Type = "rwaiID_SCRIPT" Then
         If Persist.fso.FileExists(strFileName) Then 
            Dim execShell
            Set execShell = CreateObject ("WScript.Shell") 
            On Error Resume Next ' On Error GoTo does not work... 
            execShell.Run strFileName ' Use the regular editor associated with this type of files
            If Err.Number <> 0 Then 
                execShell.Exec "Notepad " & strFileName ' Use Notepad if no editor has been associated with this type of files
            End If
            On Error Goto 0
         Else 
            MsgBox "Invalid file path (" & strFileName & ")", vbCritical, "Error" 
         End If 
        
      Else
         ' Have not handled edit request above so broadcast to other scripts to see if they
         ' can handle the request. If not then bHandled will return as False.
         Dim bHandled
         bHandled = False
         Broadcast.EditAsset Asset, bHandled
         If bHandled = False Then
            ' Use editor defined by default windows file extension association
            Dim Shell   
            Set Shell = CreateObject ("Shell.Application")   
            Shell.ShellExecute strFileName, , , "edit"
         End If
      End If
   End If
    
   VersionControl.GetPersistStatusInformation (Asset)
   GlobalScript.UpdateVersionControlMenu
End Sub

' ------------------------------------------------------------------------------
'
Sub OnNewAssetSpline ()
   Dim Parent, NewAsset
   Set Parent = GetFocusObject (CurrentContext)
   NewAsset = RWSSpline.New (Parent)

   If NewAsset Then
      SplineToolbar.CheckMenuItem 1, False ' Uncheck New Nodes ready for Edit
   End If
End Sub

' ------------------------------------------------------------------------------
'
Sub OnNewAssetSplineAsChild ()
   Dim Parent, NewAsset
   Set Parent = GetFocusObject (CurrentContext)
   NewAsset = RWSSpline.New (Parent)

   If Parent.IDType = otAssetFolder Then
      If NewAsset Then
         SplineToolbar.CheckMenuItem 1, False ' Uncheck New Nodes ready for Edit
         CurrentContext.ExpandAndFocus NewAsset
      End If
   End If
End Sub

' ------------------------------------------------------------------------------
' Asks the user for a file name for a new sequence asset. Creates the new asset
' resource and creates an asset object that points to the new file.
Sub CreateNewSequence (ParentObject)
   ' Get the file name of the new sequence
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
         ' Create a new asset
         Dim strRelativeFilename
         strRelativeFilename = GlobalScript.RWSUtils.GetRelativePath (.Filename, GlobalScript.g_strResourceRoot)

         If Sequencer.CreateFile (.Filename) Then
            Dim NewAsset, FileSystemObject
            Set FileSystemObject = CreateObject ("Scripting.FileSystemObject")
            Set NewAsset = MenuScript.CreateAsset ()

            ' Ask the sequencer to create the sequence
            NewAsset.Name = FileSystemObject.GetBaseName (strRelativeFilename)
            NewAsset.File = strRelativeFilename
            NewAsset.Type = "RF3"
            NewAsset.BuildCommand = "SequenceAsset"

            ' Add the new asset to the parent
            ParentObject.AddChildAsset NewAsset
         Else
            MsgBox "Failed to create asset " & .Filename, vbExclamation, "Sequencer"
         End If
      End If
   End With
End Sub

' ------------------------------------------------------------------------------
'
Sub OnNewAssetSequence ()
   CreateNewSequence MenuScript.AssetHierarchy
End Sub

' ------------------------------------------------------------------------------
'
Sub OnNewAssetSequenceAsChild ()
   CreateNewSequence GetFocusObject (CurrentContext)
End Sub

' ------------------------------------------------------------------------------
' Lightmap Stuff
Sub OnFolderEditLightmap
   Dim Container, Selection  
   Set Container = CreateObject ("CSL.RWSScript.RWSContainer")  
  
   CSLRenderwareStudioWorkspace.ShowObject LightmapSceneEditor  
      
   For Each Selection In CurrentContext   
        LightmapSceneEditor.SetCurrentObject (Selection (1)) 
   Next    
End Sub

Sub OnAssetEditLightmap
   Dim Container, Selection   
   Set Container = CreateObject ("CSL.RWSScript.RWSContainer")   
   
   CSLRenderwareStudioWorkspace.ShowObject LightmapAssetEditor 
     
   For Each Selection In CurrentContext  
        LightmapAssetEditor.SetCurrentObject (Selection (1))
   Next   
End Sub

' ------------------------------------------------------------------------------
'
Sub OnGetLatestVersion ()
   Dim oAPIObject
   Set oAPIObject = GetFocusObject (CurrentContext)
   If oAPIObject.IDType = otAttribute Then 
      Set oAPIObject = Database.GetAttributesParent (oAPIObject)
   End If
   If Not oAPIObject Is Nothing Then
      VersionControl.GetLatestVersionOfAPIObject oAPIObject
   End If
End Sub

' ------------------------------------------------------------------------------
'
Sub OnUndoPendingChanges ()
   Dim oAPIObject
   Set oAPIObject = GetFocusObject (CurrentContext)
   If oAPIObject.IDType = otAttribute Then 
      Set oAPIObject = Database.GetAttributesParent (oAPIObject)
   End If
   If Not oAPIObject Is Nothing Then
      VersionControl.UndoPendingChangesForAPIObject oAPIObject
   End If
   GlobalScript.UpdateVersionControlMenu
End Sub

' ------------------------------------------------------------------------------
'
Sub OnSubmitPendingChanges ()
   Dim oAPIObject
   Set oAPIObject = GetFocusObject (CurrentContext)
   If oAPIObject.IDType = otAttribute Then 
      Set oAPIObject = Database.GetAttributesParent (oAPIObject)
   End If
   If Not oAPIObject Is Nothing Then
      VersionControl.SubmitPendingChangesForAPIObject oAPIObject
   End If
   GlobalScript.UpdateVersionControlMenu
End Sub

' ------------------------------------------------------------------------------
'
Sub OnCheckOut ()
   Dim oAPIObject
   Set oAPIObject = GetFocusObject (CurrentContext)
   If oAPIObject.IDType = otAttribute Then 
      Set oAPIObject = Database.GetAttributesParent (oAPIObject)
   End If
   If Not oAPIObject Is Nothing Then
      VersionControl.CheckOutAPIObject oAPIObject, false, true
   End If
   GlobalScript.UpdateVersionControlMenu
End Sub

' ------------------------------------------------------------------------------
'
Sub OnResourceGetLatestVersion ()
   Dim oAPIObject
   Set oAPIObject = GetFocusObject (CurrentContext)
   Dim FileName
   FileName = Persist.BuildPath (GlobalScript.g_strResourceRoot, oAPIObject.File)
   VersionControl.GetLatestVersionOfFile FileName
   VersionControl.GetResourceStatusInformation (oAPIObject)
End Sub

' ------------------------------------------------------------------------------
'
Sub OnResourceUndoPendingChanges ()
   Dim oAPIObject
   Set oAPIObject = GetFocusObject (CurrentContext)
   Dim FileName
   FileName = Persist.BuildPath (GlobalScript.g_strResourceRoot, oAPIObject.File)
   VersionControl.UndoPendingChangesForFile FileName
   VersionControl.GetResourceStatusInformation (oAPIObject)
   GlobalScript.UpdateVersionControlMenu
End Sub

' ------------------------------------------------------------------------------
'
Sub OnResourceSubmitPendingChanges ()
   Dim oAPIObject
   Set oAPIObject = GetFocusObject (CurrentContext)
   Dim FileName
   FileName = Persist.BuildPath (GlobalScript.g_strResourceRoot, oAPIObject.File)
   VersionControl.SubmitPendingChangesForFile FileName
   VersionControl.GetResourceStatusInformation (oAPIObject)
   GlobalScript.UpdateVersionControlMenu
End Sub

' ------------------------------------------------------------------------------
'
Sub OnResourceCheckOut ()
   Dim oAPIObject
   Set oAPIObject = GetFocusObject (CurrentContext)
   Dim FileName
   FileName = Persist.BuildPath (GlobalScript.g_strResourceRoot, oAPIObject.File)
   VersionControl.CheckOutFile FileName, false, true
   VersionControl.GetResourceStatusInformation (oAPIObject)
   GlobalScript.UpdateVersionControlMenu
End Sub
