' 
' Enterprise Layout Management Script
'
' Layout object 'Database'
'
' This script holds all global variables and helper functions for handling the
' database objects.

Option Explicit ' All variables must be explicitly declared (with Dim)

'------------------------------------------------------------------------------
' Object type event mask IDs, defined in RWSScript.h  
Const otNull                 = 0 
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
Const otAll                  = 16383 ' (8192 * 2) - 1


'------------------------------------------------------------------------------ 
' Returns the object type of an API object as a string 
Function GetTypeString (APIObject) 
   Select Case APIObject.IDType 
   Case otGame                  GetTypeString = "Game" 
   Case otEntity                GetTypeString = "Entity" 
   Case otFolder                GetTypeString = "Folder" 
   Case otAsset                 GetTypeString = "Asset" 
   Case otAssetFolder           GetTypeString = "AssetFolder" 
   Case otTemplate              GetTypeString = "Template" 
   Case otTemplateFolder        GetTypeString = "TemplateFolder" 
   Case otBehavior              GetTypeString = "Behavior" 
   Case otBehaviorFolder        GetTypeString = "BehaviorFolder" 
   Case otCommand               GetTypeString = "Command" 
   Case otAttribute             GetTypeString = "Attribute" 
   Case otAttributeShare        GetTypeString = "AttributeShare" 
   Case otAttributeShareFolder  GetTypeString = "AttributeShareFolder" 
   Case otEvent                 GetTypeString = "Event" 
   Case Else                    GetTypeString = "Unknown" 
   End Select 
End Function

'------------------------------------------------------------------------------
' Create an API object using its ID.
Function GetAPIObject (ID)
   If ID = 0 Then
      Set GetAPIObject = Nothing
      Exit Function
   End If

   Select Case RWSScript.IDType (ID)
   Case otGame                 Set GetAPIObject = RWSScript.Game ()
   Case otEntity               Set GetAPIObject = RWSScript.Entity (ID)
   Case otFolder               Set GetAPIObject = RWSScript.Folder (ID)
   Case otAsset                Set GetAPIObject = RWSScript.Asset (ID)
   Case otAssetFolder          Set GetAPIObject = RWSScript.AssetFolder (ID)
   Case otTemplate             Set GetAPIObject = RWSScript.Template (ID)
   Case otTemplateFolder       Set GetAPIObject = RWSScript.TemplateFolder (ID)
   Case otBehavior             Set GetAPIObject = RWSScript.Behavior (ID)
   Case otBehaviorFolder       Set GetAPIObject = RWSScript.BehaviorFolder (ID)
   Case otCommand              Set GetAPIObject = RWSScript.Command (ID)
   Case otAttribute            Set GetAPIObject = RWSScript.Attribute (ID)
   Case otAttributeShare       Set GetAPIObject = RWSScript.AttributeShare (ID)
   Case otAttributeShareFolder Set GetAPIObject = RWSScript.AttributeShareFolder (ID)
   Case otEvent                Set GetAPIObject = RWSScript.Event (ID)
   Case Else                   Set GetAPIObject = Nothing
   End Select
End Function

'------------------------------------------------------------------------------
' Create an API object using its UID.
Function GetAPIObjectFromUID (UID)
   If UID = "" Then
      Set GetAPIObjectFromUID = Nothing
      Exit Function
   End If

   Select Case RWSScript.IDType (UID)
   Case otGame                 Set GetAPIObjectFromUID = RWSScript.Game ()
   Case otEntity               Set GetAPIObjectFromUID = RWSScript.Entity (UID)
   Case otFolder               Set GetAPIObjectFromUID = RWSScript.Folder (UID)
   Case otAsset                Set GetAPIObjectFromUID = RWSScript.Asset (UID)
   Case otAssetFolder          Set GetAPIObjectFromUID = RWSScript.AssetFolder (UID)
   Case otTemplate             Set GetAPIObjectFromUID = RWSScript.Template (UID)
   Case otTemplateFolder       Set GetAPIObjectFromUID = RWSScript.TemplateFolder (UID)
   Case otBehavior             Set GetAPIObjectFromUID = RWSScript.Behavior (UID)
   Case otBehaviorFolder       Set GetAPIObjectFromUID = RWSScript.BehaviorFolder (UID)
   Case otCommand              Set GetAPIObjectFromUID = RWSScript.Command (UID)
   Case otAttribute            Set GetAPIObjectFromUID = RWSScript.Attribute (UID)
   Case otAttributeShare       Set GetAPIObjectFromUID = RWSScript.AttributeShare (UID)
   Case otAttributeShareFolder Set GetAPIObjectFromUID = RWSScript.AttributeShareFolder (UID)
   Case otEvent                Set GetAPIObjectFromUID = RWSScript.Event (UID)
   Case Else                   Set GetAPIObjectFromUID = Nothing
   End Select
End Function

'------------------------------------------------------------------------------
' Returns a container of objects of the type specified in the TypeMask.
' Note, only a single type can be requested, if more than one is requested, then
' only the first will be returned.
Function GetObjectsOfType (nType)
   Select Case nType
   Case otGame 
      Dim Game, RequestedObjects
      Set Game = RWSScript.Game ()
      Set RequestedObjects = RWSScript.CreateContainer ()
      RequestedObjects.Add Game.ID
      Set GetObjectsOfType = RequestedObjects
   Case otEntity               Set GetObjectsOfType = RWSScript.Entities ()
   Case otFolder               Set GetObjectsOfType = RWSScript.Folders ()
   Case otAsset                Set GetObjectsOfType = RWSScript.Assets ()
   Case otAssetFolder          Set GetObjectsOfType = RWSScript.AssetFolders ()
   Case otTemplate             Set GetObjectsOfType = RWSScript.Templates ()
   Case otTemplateFolder       Set GetObjectsOfType = RWSScript.TemplateFolders ()
   Case otBehavior             Set GetObjectsOfType = RWSScript.Behaviors ()
   Case otBehaviorFolder       Set GetObjectsOfType = RWSScript.BehaviorFolders ()
   Case otCommand              Set GetObjectsOfType = RWSScript.Commands ()
   Case otAttribute            Set GetObjectsOfType = RWSScript.Attributes ()
   Case otAttributeShare       Set GetObjectsOfType = RWSScript.AttributeShares ()
   Case otAttributeShareFolder Set GetObjectsOfType = RWSScript.AttributeShareFolders ()
   Case otEvent                Set GetObjectsOfType = RWSScript.Events ()
   Case Else                   Set GetObjectsOfType = RWSScript.CreateContainer ()
   End Select
End Function


'------------------------------------------------------------------------------
' Returns the parent API object for an attribute
' Note attributes can only be parented by a single item so this function 
' returns the first parent found
Function GetAttributesParent (oAPIObject)
   If oAPIObject.ParentAttributeShares.Count > 0 Then  
      Set GetAttributesParent = oAPIObject.ParentAttributeShares(1)
      Exit Function
   ElseIf oAPIObject.ParentEntities.Count > 0 Then   
      Set GetAttributesParent = oAPIObject.ParentEntities(1)
      Exit Function
   End If  
   Set GetAttributesParent = Nothing
End Function

' ------------------------------------------------------------------------------
' Returns True if the object is nested in the hierarchy of the folder.
' Function searches up the hierarchy for performance reasons.
' Currently only setup for entities, should be expanded to check all object types.
Function IsInFolder (oObject, oFolder)
   IsInFolder = False

   Dim oParentFolder
   Select Case oObject.IDType 
   Case otFolder:
      If oObject = oFolder Or oObject.ParentFolders.Exists (oFolder) Then
         IsInFolder = True
      Else
         ' Check parent folders
         For Each oParentFolder In oObject.ParentFolders
            If IsInFolder (oParentFolder, oFolder) Then
               IsInFolder = True
               Exit Function
            End If
         Next
      End If
   Case otEntity:
      If oObject.ParentFolders.Exists (oFolder) Then
         IsInFolder = True  
      Else  
         ' Check parent folders
         For Each oParentFolder In oObject.ParentFolders
            If IsInFolder (oParentFolder, oFolder) Then
               IsInFolder = True  
               Exit Function  
            End If  
         Next  
      End If
   End Select
End Function
