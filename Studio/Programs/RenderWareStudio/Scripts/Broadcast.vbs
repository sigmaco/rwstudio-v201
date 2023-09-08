'
' Broadcast.vbs
'
' This script contains all of the events that are broadcast to every script.
' To catch an event, add an event handler to your script with a function name
' that matches the name that is broadcast from this script.
' e.g. to catch the event from the OnLoadApplicationSettings function
'      RENManager.Broadcast.Broadcast_LoadApplicationSettings XMLRootNode
'      Use:
'      Sub Broadcast_LoadApplicationSettings (XMLRootNode)
'         ' Add code here, accesssing XMLRootNode
'      End Sub

Option Explicit ' All variables must be explicitly declared, with Dim


' ******************************************************************************
' Workspace handling
' ******************************************************************************

' ------------------------------------------------------------------------------
' This event is fired directly after workspace loads, including loading any
' projects on startup.
Sub WorkspaceStart ()
   RENManager.Broadcast.Broadcast_WorkspaceStart
End Sub

' ------------------------------------------------------------------------------
' This event is fired just prior to workspace closing.
Sub WorkspaceEnd ()
   RENManager.Broadcast.Broadcast_WorkspaceEnd
End Sub

' ******************************************************************************
' Menus
' ******************************************************************************

' ------------------------------------------------------------------------------
' This event is fired out prior to showing a context menu for an API object.
' This gives the receiver of this event the opportunity to add/edit/remove/gray
' out context menu options.
' - oContextMenu is the context menu that is about to be displayed.
' - nObjectType is one of the object type event mask IDs as defined in Database.vbs
Sub PreShowObjectContextMenu (oContextMenu, nObjectType)
   RENManager.Broadcast.Broadcast_PreShowObjectContextMenu oContextMenu, nObjectType
End Sub

' ------------------------------------------------------------------------------
' This event is fired out prior to showing a context menu for an API object's
' lister. This gives the receiver of this event the opportunity to add/edit/remove/gray
' out context menu options.
' - oContextMenu is the context menu that is about to be displayed.
' - nListerType is one of the Lister node types as defined in ContextMenu.vbs
Sub PreShowListerContextMenu (oContextMenu, nListerType)
   RENManager.Broadcast.Broadcast_PreShowListerContextMenu oContextMenu, nListerType
End Sub

' ******************************************************************************
' Project handling
' ******************************************************************************

' ------------------------------------------------------------------------------
' This event is fired directly before creating a new RenderWare Studio project,
' allowing you to save items from the current project.
Sub PreNewProject ()
   RENManager.Broadcast.Broadcast_PreNewProject
End Sub

' ------------------------------------------------------------------------------
' This event is fired directly after creating a new RenderWare Studio project.
Sub PostNewProject ()
   RENManager.Broadcast.Broadcast_PostNewProject
End Sub

' ------------------------------------------------------------------------------
' This event is fired directly before loading a RenderWare Studio project.
Sub PreLoadProject (strFilename)
   RENManager.Broadcast.Broadcast_PreLoadProject strFilename
End Sub

' ------------------------------------------------------------------------------
' This event is fired directly after loading a RenderWare Studio project.
Sub PostLoadProject (strFilename)
   RENManager.Broadcast.Broadcast_PostLoadProject strFilename
End Sub

' ------------------------------------------------------------------------------
' This event is fired directly before saving a RenderWare Studio project.
Sub PreSaveProject (strFilename)
   RENManager.Broadcast.Broadcast_PreSaveProject strFilename
End Sub

' ------------------------------------------------------------------------------
' This event is fired directly after saving a RenderWare Studio project.
Sub PostSaveProject (strFilename)
   RENManager.Broadcast.Broadcast_PostSaveProject strFilename
End Sub


' ******************************************************************************
' Settings file handling
' ******************************************************************************

' ------------------------------------------------------------------------------
' This event is fired directly before the OnLoadApplicationSettings event.
' This gives a script chance to upgrade its data between versions.
Sub UpdateApplicationSettingsVersion (XMLDoc, XMLRootNode, nFileVersion)
   RENManager.Broadcast.Broadcast_UpdateApplicationSettingsVersion XMLDoc, XMLRootNode, _
                                                         nFileVersion
End Sub

' ------------------------------------------------------------------------------
' This event is fired directly after the application settings file is loaded.
' The root XML node (<Object>/<RWSScriptedSettings>) is provided as a parameter, allowing
' you to pull out any data on any child node.
Sub LoadApplicationSettings (XMLRootNode)
   RENManager.Broadcast.Broadcast_LoadApplicationSettings XMLRootNode
End Sub

' ------------------------------------------------------------------------------
' This event is fired directly before saving the application settings file.
' The XMLDoc is provided as a parameter, allowing you to create new nodes from it.
' The root XML node (<Object>/<RWSScriptedSettings>) is provided as a parameter,
' allowing you to add child nodes onto it.
' Note, RWS uses the script name as the next child followed by the settings, e.g.
' <Object>                  <!-- Root node for settings file -->
'   <RWSScriptedSettings>   <!-- Root node for all RWS scripted settings -->
'     <Layouts>             <!-- Root node for all settings written from the Layouts.vbs script -->
'       <LastLayout>General <!-- Settings for the Layouts.vbs script -->
'       </LastLayout>
'     </Layouts> ...
'   </RWSScriptedSettings>
' </Object>
Sub SaveApplicationSettings (XMLDoc, XMLRootNode)
   RENManager.Broadcast.Broadcast_SaveApplicationSettings XMLDoc, XMLRootNode
End Sub

' ------------------------------------------------------------------------------
' This event is fired directly after the game settings file is loaded.
' The root XML node (references Object/RWS) is provided as a parameter, allowing
' you to pull out any data on any child node.
Sub LoadGameSettings (XMLRootNode)
   RENManager.Broadcast.Broadcast_LoadGameSettings XMLRootNode
End Sub

' ------------------------------------------------------------------------------
' This event is fired directly before saving the game settings file.
' The XMLDoc is provided as a parameter, allowing you to create new nodes from it.
' The root XML node (references Object/RWS) is provided as a parameter, allowing
' you to add child nodes onto it.
Sub SaveGameSettings (XMLDoc, XMLRootNode)
   RENManager.Broadcast.Broadcast_SaveGameSettings XMLDoc, XMLRootNode
End Sub


' ******************************************************************************
' Building Games
' ******************************************************************************

' ------------------------------------------------------------------------------
' This event is fired directly cleaning building a RenderWare Studio project.
Sub PreCleanProject (strConnectionID, strConnectionName)
   RENManager.Broadcast.Broadcast_PreCleanProject strConnectionID, strConnectionName
End Sub

' ------------------------------------------------------------------------------
' This event is fired directly after cleaning a RenderWare Studio project.
Sub PostCleanProject (strConnectionID, strConnectionName, bCleanSuccessful)
   RENManager.Broadcast.Broadcast_PostCleanProject strConnectionID, strConnectionName, bCleanSuccessful
End Sub

' ------------------------------------------------------------------------------
' This event is fired directly before building a RenderWare Studio project.
Sub PreBuildProject (strConnectionID, strConnectionName)
   RENManager.Broadcast.Broadcast_PreBuildProject strConnectionID, strConnectionName
End Sub

' ------------------------------------------------------------------------------
' This event is fired directly after building a RenderWare Studio project.
Sub PostBuildProject (strConnectionID, strConnectionName, bBuildSuccessful)
   RENManager.Broadcast.Broadcast_PostBuildProject strConnectionID, strConnectionName, bBuildSuccessful
End Sub

' ------------------------------------------------------------------------------
' This event is fired after connecting to a target.
Sub ConnectToTarget (nConnectionID)
   RENManager.Broadcast.Broadcast_ConnectToTarget nConnectionID
End Sub

' ------------------------------------------------------------------------------
' This event is fired after disconnecting from a target.
Sub DisconnectFromTarget (nConnectionID)
   RENManager.Broadcast.Broadcast_DisconnectFromTarget nConnectionID
End Sub


' ******************************************************************************
' Asset Functions
' ******************************************************************************

' ------------------------------------------------------------------------------
' This event is fired when the active level has changed and all of the contained
' assets have been imported.
Sub PostImportAllAssets ()
   RENManager.Broadcast.Broadcast_PostImportAllAssets
End Sub

' ------------------------------------------------------------------------------
' This event is fired to get the type of the asset's resource file.
Sub GetAssetResourceType (AssetID, strResourceFilename, strAssetType, bHandled)
   RENManager.Broadcast.Broadcast_GetAssetResourceType AssetID, strResourceFilename, strAssetType, bHandled
End Sub

' ------------------------------------------------------------------------------
' This event is fired when the user requests to edit as asset.
' If you handle this event, set the bHandled parameter to True.
' Other script handlers should check this boolean before attempting to use.
Sub EditAsset (Asset, bHandled)
   RENManager.Broadcast.Broadcast_EditAsset Asset, bHandled
End Sub


' ******************************************************************************
' Behavior functions
' ******************************************************************************

' ------------------------------------------------------------------------------
' This event is fired directly before a source parse.
Sub PreParseSource ()
   RENManager.Broadcast.Broadcast_PreParseSource
End Sub

' ------------------------------------------------------------------------------
' This event is fired directly after a source parse, allowing your control to
' update itself based on the new source.
Sub PostParseSource ()
   RENManager.Broadcast.Broadcast_PostParseSource
End Sub


' ******************************************************************************
' Folder functions
' ******************************************************************************

' ------------------------------------------------------------------------------
' This event is fired after the active folder has changed, but prior to the
' new folder's assets being imported (Broadcast_PostImportAllAssets() is fired after).
Sub ActiveFolderChanged (NewRootFolder)
   RENManager.Broadcast.Broadcast_ActiveFolderChanged NewRootFolder
End Sub
