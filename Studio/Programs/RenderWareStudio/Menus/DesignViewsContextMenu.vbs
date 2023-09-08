'
' Enterprise Layout Management Script
'
' Layout object 'DesignViewsContextMenu'
'

Option Explicit

' -----------------------------------------------------------------------------
' Script objects
Dim GlobalSelection
Set GlobalSelection = CreateObject("CSL.RWSSelection.Selection")

' -----------------------------------------------------------------------------
' Script globals
Dim g_lngContextMenuX
Dim g_lngContextMenuY
Dim g_lngContextMenuChannelID
Dim g_lngContextMenuViewID

g_lngContextMenuX         = 0
g_lngContextMenuY         = 0
g_lngContextMenuChannelID = 0
g_lngContextMenuViewID    = 0

' -----------------------------------------------------------------------------
' Initialization dependant on other objects being loaded.
Sub OnLoad ()
    GlobalSelection.SelectionIdentifier = GlobalScript.g_strGlobalSelection
End Sub

' -----------------------------------------------------------------------------
Sub OnDeleteSelection ()
    Dim Undo
    Dim SelectedItem
    
    If GlobalSelection.Count > 0 Then
        ' Create undo point
        Set Undo = GlobalScript.RWSScript.CreateUndo("Deleting selection")
        ' Iterate through collection and delete item from database 
        For Each SelectedItem In GlobalSelection
            If Not SelectedItem = 0 Then 

                Dim APIObject
                
                Set APIObject = GlobalScript.ObjectFromId (SelectedItem)

                If not APIObject is nothing Then
                   ContextMenu.DeleteObject APIObject, GlobalScript.RWSScript, Undo
                End If
                
            End If 
        Next
    End If

   ' Tell the selection object to clear whatever was selected (now deleted)
   GlobalSelection.Clear
End Sub

' -----------------------------------------------------------------------------

Sub OnClearSelection ()
    If GlobalSelection.Count > 0 Then
       GlobalSelection.Clear
    End If
End Sub

' -----------------------------------------------------------------------------

Sub OnAimInView ()

   If GlobalSelection.Count > 0 Then
      DesignView.RWSUtility.AimAtEntity GlobalSelection.Item (1)
   End If

End Sub

' -----------------------------------------------------------------------------

Sub OnCenterInView
   ' Center in DesignView
   If GlobalSelection.Count > 0 Then
      DesignView.RWSUtility.LocateEntity GlobalSelection.Item (1)
   End If
End Sub

' -----------------------------------------------------------------------------

Sub OnLocateInGameExplorer
   ' Locate in game explorer
   If GlobalSelection.Count > 0 Then
      GameDatabase.LocateEntity GlobalSelection.Item (1)
      CSLRenderwareStudioWorkspace.ShowObject GameDatabase
   End If
End Sub

' -----------------------------------------------------------------------------

Function ShowFolderAssets (Folder)

   ShowFolderAssets = False

   if Folder.ChildAssets.Count > 0 then
   
      StreamViewer.ShowAsset Folder.ChildAssets(1).ID
      ShowFolderAssets = True
     
   else
  
      Dim SubFolder 
      for each SubFolder in Folder.ChildAssetFolders 
        
         if ShowFolderAssets (SubFolder) = True then
            ShowFolderAssets = True
            exit for
         end if
         
      next  
 
   end if

End Function

' ----------------------------------------------------------------------------- 

Sub OnShowAssetStream

   If GlobalSelection.Count > 0 Then

      Dim SelectedObject
      SelectedObject = GlobalSelection.Item (1)
      
      if RWSScript.IDType (SelectedObject) = Database.otEntity then

         ' Got an entity, need to find out hole many assets
         ' it has
         Dim EntityObject
         Set EntityObject = GlobalScript.ObjectFromId (SelectedObject)
        
         if EntityObject.ChildAssets.Count > 0 then
            
            StreamViewer.ShowAsset EntityObject.ChildAssets(1).ID

         else 
                             
            Dim Folder
            for each Folder in EntityObject.ChildAssetFolders
               
               if ShowFolderAssets (Folder) = True then
                 exit for 
               end if
        
            next         
                  
         end if

         ' Make sure the stream viewer is visible         
         CSLRenderwareStudioWorkspace.ShowObject StreamViewer

      end if
      
   end if
   
End Sub


' -----------------------------------------------------------------------------
' Hide all the currently selected items

Sub OnHideSelection ()

   If GlobalSelection.Count > 0 Then

      Dim SelectedItem
   
      ' Iterate through collection and hide item 
      For Each SelectedItem In GlobalSelection
         
         If Not SelectedItem = 0 Then 
      
            Dim APIObject
            Set APIObject = GlobalScript.ObjectFromId (SelectedItem)

            If APIObject.IDType = Database.otEntity Or APIObject.IDType = Database.otFolder Then
               GlobalScript.RWSUtils.ShowVisibility APIObject, False
            End If

         End If

      Next

   End if

End Sub



' ------------------------------------------------------------------------------

Sub ShowAllEntitiesInFolder (Folder)

   ' Show he entities in this folder
   GlobalScript.RWSUtils.ShowVisibility Folder, True

   ' Recurse through child folders
   Dim ChildFolder
   For Each ChildFolder In Folder.ChildFolders
      ShowAllEntitiesInFolder ChildFolder
   Next

End Sub


' -----------------------------------------------------------------------------
' Show all items in the currently active level

Sub OnShowAll ()

   ShowAllEntitiesInFolder GlobalScript.RWSScript.Game.ActiveFolder
 
End Sub

' ------------------------------------------------------------------------------  
Sub UnfreezeEntitiesInFolder (Folder) 
 
   ' Unfreeze the entities in this folder 
   GlobalScript.RWSUtils.FreezeObject Folder, False 
 
   ' Recurse through child folders 
   Dim ChildFolder 
   For Each ChildFolder In Folder.ChildFolders 
      UnfreezeEntitiesInFolder ChildFolder 
   Next 
 
End Sub

' ----------------------------------------------------------------------------- 

Sub OnFreezeSelection () 
 
   Dim SelectedItem 
     
   If GlobalSelection.Count > 0 Then 
    
      ' Iterate through collection and hide item  
      For Each SelectedItem In GlobalSelection 
             
         If Not SelectedItem = 0 Then  
       
            Dim APIObject 
            Set APIObject = GlobalScript.ObjectFromId (SelectedItem) 
 
            If APIObject.IDType = Database.otEntity Or APIObject.IDType = Database.otFolder Then 
               GlobalScript.RWSUtils.FreezeObject APIObject, True 
            End If 
         
         End If 
 
      Next 
 
      ' Deselect everything we've just frozen 
      GlobalSelection.Clear 
 
   End if 
 
End Sub 
 
' ----------------------------------------------------------------------------- 
 
Sub OnUnfreezeAll () 
 
   UnfreezeEntitiesInFolder GlobalScript.RWSScript.Game.ActiveFolder 
 
End Sub

' -----------------------------------------------------------------------------
Sub OnCut ()
   DesignView.RWSUtility.Cut
End Sub

' -----------------------------------------------------------------------------
'
Sub OnCopy ()
   DesignView.RWSUtility.Copy
End Sub

' -----------------------------------------------------------------------------
'
Sub OnPaste ()
   DesignView.RWSUtility.Paste g_lngContextMenuX, g_lngContextMenuY, _
                               g_lngContextMenuChannelID, g_lngContextMenuViewID
End Sub

' -----------------------------------------------------------------------------
'
Sub SetupPaste (x, y, ChannelID, ViewID)
   g_lngContextMenuX         = x
   g_lngContextMenuY         = y
   g_lngContextMenuChannelID = ChannelID
   g_lngContextMenuViewID    = ViewID
End Sub
