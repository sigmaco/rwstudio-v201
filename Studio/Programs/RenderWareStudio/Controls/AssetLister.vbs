'
' Enterprise Layout Management Script
'
' Layout object 'AssetLister'
'

Option Explicit ' All variables must be explicitly declared

Const g_PersistElement = "AssetLister"

Dim g_bIsVisible, g_bNeedToPopulate ' Flags so we know when window is visible and can be populated
g_bIsVisible = False
g_bNeedToPopulate = False

' Lister view modes
Const SmallIcon      = 0
Const SmallThumbnail = 1
Const LargeThumbnail = 2
Const Detail         = 3
Const List           = 4
Const Tree           = 5

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

AssetLister.SelectionIdentifier = "AssetSelections"

' ------------------------------------------------------------------------------
' Displays a context menu on the asset lister
Sub AssetLister_OnContextMenu (Context, nX, nY, hWnd)
   ContextMenu.DisplayListerMenu Context, nX, nY, hWnd
End Sub

' ------------------------------------------------------------------------------
' Determines the type of an asset
Sub AssetLister_OnGetFileType (AssetID, strFilename, strType)
    strType = GlobalScript.GetAssetFileType (AssetID, strfilename)
End Sub

' ------------------------------------------------------------------------------
' This event is fired after assets are dropped onto the asset lister and the
' studio assets are created.
Sub AssetLister_OnNewAssetsDroppedIn (AssetIDs)
   ' Only call the rf3 import if we're not in either of the 2 thumbnail modes.
   If Not (AssetLister.ViewMode = SmallThumbnail Or _
           AssetLister.ViewMode = LargeThumbnail) Then
       Dim n, Asset
       For n = LBound (AssetIDs) To UBound (AssetIDs)
           Set Asset = GlobalScript.RWSScript.Asset (AssetIDs (n))
           BuildScriptManager.PreImportAsset Asset
       Next
   End If
End Sub

' ------------------------------------------------------------------------------
' This event is fired when the user presses the Del key
Sub AssetLister_OnDeleteRemove (Context)
   'Check that an item is selected
   If Not Context.IsNode (GlobalScript.AssetListerNode) Then
      ContextMenu.DeleteOrRemove Context
   End If
End Sub

' ------------------------------------------------------------------------------
' Calls into utils to Request / Set status of an object
Sub AssetLister_OnRequestStatus (ObjectID)
   VersionControl.GetPersistStatusInformation ObjectID
   If GlobalScript.RWSScript.IDType(ObjectID) = otAsset Then
      VersionControl.GetResourceStatusInformation ObjectID
   End If
End Sub

'------------------------------------------------------------------------------
Sub NewDatabase ()
   ' If lister is visible, populate it immediately,
   ' otherwise it will be populated when it becomes visible
   If g_bIsVisible Then
      g_bNeedToPopulate = False
      AssetLister.Populate GlobalScript.AssetListerNode
   Else
      g_bNeedToPopulate = True
   End If
End Sub

'------------------------------------------------------------------------------
Sub AssetLister_OnVisibleChanged (bVisible)
   If bVisible And Not g_bIsVisible Then
      ' Only refresh if we need to (source changed, or new root-folder is active
      If g_bNeedToPopulate Then
         g_bNeedToPopulate = False
         AssetLister.Populate GlobalScript.AssetListerNode
      End If
   End If
   g_bIsVisible = bVisible
End Sub

' ------------------------------------------------------------------------------
' Catch broadcast events
' ------------------------------------------------------------------------------

' ------------------------------------------------------------------------------
' Handle broadcast message to convert settings.
'Sub Broadcast_UpdateApplicationSettingsVersion (XMLDoc, XMLRootNode, nFileVersion)
'End Sub

' ------------------------------------------------------------------------------
' Handle broadcast message to load settings.
Sub Broadcast_LoadApplicationSettings (XMLRootNode)
   ' Load the state of the lister on startup
   ListerTools.LoadLister AssetLister, g_PersistElement, XMLRootNode
End Sub

' ------------------------------------------------------------------------------
' Handle broadcast message to save settings.
Sub Broadcast_SaveApplicationSettings (XMLDoc, XMLRootNode)
   ' Save state of lister into XML file
   ListerTools.SaveLister AssetLister, g_PersistElement, XMLDoc, XMLRootNode
End Sub

