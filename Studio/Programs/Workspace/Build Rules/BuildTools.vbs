'==============================================================================
' BuildTools.vbs
'==============================================================================

Option Explicit ' All variables must be explicitly declared (with Dim)

' The stream edit tools are used by GetAssetsFilename to determine if an
' asset has been edited by the stream editor.
Dim oBtStreamEditTools
Set oBtStreamEditTools = Nothing

'------------------------------------------------------------------------------
'
Function BuildRule (APIObject, DefaultRule)
   If Len (APIObject.BuildCommand) > 0 Then
      BuildRule = APIObject.BuildCommand
   Else
      BuildRule = DefaultRule
   End If
End Function

'------------------------------------------------------------------------------
' Returns the file name of the placement new header file
Function PNHeaderFile (DestDirectory, Folder)
   PNHeaderFile = DestDirectory & "Misc\" & Folder.UID & "PNHeader.rws"
End Function

'------------------------------------------------------------------------------
' Returns the file name of the destination stream to represent this object type
Function StreamFile (DestDirectory, APIObject)
   StreamFile = DestDirectory & GetTypeString (APIObject) & "\" & APIObject.UID & ".rws"
End Function

'------------------------------------------------------------------------------
' Returns the file name of the root folders data stream
Function RootFolderFile (DestDirectory, RootFolder)
   RootFolderFile = DestDirectory & GetTypeString (RootFolder) & "\" & RootFolder.UID & ".stream"
End Function

'------------------------------------------------------------------------------
' Returns the file name of the assetglobals text file.
' This must match the path in the ren file, where the build rule is launched.
Function AssetGlobalsFile (DestDirectory)
   AssetGlobalsFile = DestDirectory
   AssetGlobalsFile = BuildPath (AssetGlobalsFile, "Misc")
   AssetGlobalsFile = BuildPath (AssetGlobalsFile, "assetglobals.txt")
End Function

'------------------------------------------------------------------------------
' Returns the file name of the entityglobals text file.
' This must match the path in the ren file, where the build rule is launched.
Function EntityGlobalsFile (DestDirectory)
   EntityGlobalsFile = DestDirectory
   EntityGlobalsFile = BuildPath (EntityGlobalsFile, "Misc")
   EntityGlobalsFile = BuildPath (EntityGlobalsFile, "entityglobals.txt")
End Function

'------------------------------------------------------------------------------
' Returns the folder name for the destination files for texture dictionaries.
Function TDOutputLocation (DestDirectory)
   TDOutputLocation = BuildPath (DestDirectory, "Texture Dictionary")
End Function

'------------------------------------------------------------------------------
' Returns the folder name for the destination files for fonts
Function FontOutputLocation (DestDirectory)
   FontOutputLocation = BuildPath (DestDirectory, "Font")
End Function

'------------------------------------------------------------------------------
' Returns the folder name for the destination file for 'binding' an asset file
' and the stream-edit data, producing a single asset file
Function StreamEditOutputLocation (Asset)
   Dim DestDirectory
   DestDirectory     = GetParam ("DESTDIR")
   
   StreamEditOutputLocation = BuildPath (DestDirectory, "Asset\Bound\") & Asset.UID & ".rws"
End Function

'------------------------------------------------------------------------------
' Returns the file name of the sent objects list.
Function SentObjectsFile (DestDirectory, Folder)
   SentObjectsFile = DestDirectory
   SentObjectsFile = BuildPath (SentObjectsFile, "Misc")
   SentObjectsFile = BuildPath (SentObjectsFile, Folder.UID & ".SentObjects")
End Function

'------------------------------------------------------------------------------
' Returns the root parent asset for an asset.
' The root asset should be an rf3 file.
Function GetRootRF3Asset (Asset)
    If Asset.ParentAssets.Count = 0 Then
        Set GetRootRF3Asset = Asset
    Else
        Set GetRootRF3Asset = GetRootRF3Asset (Asset.ParentAssets (1))
    End If
End Function

'------------------------------------------------------------------------------
' Returns: True if the Asset references an RF3 file and has no children,
'          False otherwise.
Function AssetIsRootRF3 (Asset)
   AssetIsRootRF3 = False
   If Left (Asset.Type, 3) = "RF3" Then
      If Asset.ParentAssets.Count = 0 Then
         AssetIsRootRF3 = True
      End If
   End If
End Function

'------------------------------------------------------------------------------
' Returns: True if the Asset is a child of an Asset that references an RF3 file,
'          False otherwise
Function AssetIsRF3Child (Asset)
   Dim ParentRF3Asset

   AssetIsRF3Child = False
   If Asset.ParentAssets.Count > 0 Then
      Set ParentRF3Asset = GetRootRF3Asset (Asset)
      If Left (ParentRF3Asset.Type, 3) = "RF3" Then
         AssetIsRF3Child = True
      End If
   End If
End Function

'------------------------------------------------------------------------------
' Helper function to build a filename from the original rws file to include the
' chunk index.
Function GetConcatFilename (OriginalFilename, chunkIndex)
   Dim fileExtension

   If Len (OriginalFilename) = 0 Then
      GetConcatFilename  = ""
   Else
      fileExtension = FileSystem.GetExtensionName (OriginalFilename)
      GetConcatFilename = Mid (OriginalFilename, 1, _
                         Len (OriginalFilename) - Len (fileExtension) - 1) & _
                         chunkIndex & "." & fileExtension
   End If
End Function

'------------------------------------------------------------------------------
' Returns the filename where the exported file from an RF3 asset will be for
' this build. This is stored in the property:
'   "RWSResourceForTemplate-<Full RF3 Template Path>"
Function GetRF3ResourceFilename (oAsset)
   Dim Properties, Property
   Set Properties = RWSScript.Properties (oAsset)

   ' Find the property for this asset using the current RF3 template.
   For Each Property in Properties
      If Property.UID = "RWSResourceForTemplate-" & _
                        FileSystem.GetBaseName (GetParam ("RF3PROJECTTEMPLATENAME")) Then
         Dim PropertyData, DataSection
         Set PropertyData = Property.Get (oAsset)
         For Each DataSection in PropertyData
            If DataSection.Type = 6 Then ' 6-dtChar - defined in RWSScript.h
               Dim strResourceFilename, Data
               strResourceFilename = ""
               For Each Data in DataSection
                  strResourceFilename = strResourceFilename & Chr(CStr(Data))
               Next
               GetRF3ResourceFilename = strResourceFilename
               Exit Function
            End If
         Next
      End If
   Next

   GetRF3ResourceFilename = ""
End Function

'------------------------------------------------------------------------------
' Returns the filename of the asset that should be put into the stream.
' If the asset is a contained asset from an RF3 file, then we search its xml
' file for the filename to use for the build. We do this because each build
' will produce a different file in a different location, each specified in the xml file.
' Does not look for the presence of stream edit data

Function GetAssetsFilenameNoStreamData (Asset, strResourceRoot)
   Dim XMLDoc, TargetFileNode, ChunkIndex
   GetAssetsFilenameNoStreamData = ""

   If AssetIsRF3Child (Asset) Then
      GetAssetsFilenameNoStreamData = GetRF3ResourceFilename (Asset)
   Else
      GetAssetsFilenameNoStreamData = BuildPath (strResourceRoot, Asset.File)
   End if
           
End Function


'------------------------------------------------------------------------------
' Returns the filename of the asset that should be put into the stream.
' If the asset is a contained asset from an RF3 file, then we search its xml
' file for the filename to use for the build. We do this because each build
' will produce a different file in a different location, each specified in the xml file.
Function GetAssetsFilename (Asset, strResourceRoot)

   ' Ensure that a stream edit tools object has been instanced.
   If oBtStreamEditTools Is Nothing Then
      Set oBtStreamEditTools = CreateObject ("CSL.RWSStreamEditControl.RWSStreamEdit")
   End If

   ' Check for the presence of stream edit data ....
   If oBtStreamEditTools.AssetHasStreamData (Asset.ID) then
      GetAssetsFilename = StreamEditOutputLocation (Asset)
   Else
      GetAssetsFilename = GetAssetsFilenameNoStreamData (Asset, strResourceRoot)
   End If

   ' Ensure the filename is an absolute path.
   GetAssetsFilename = FileSystem.GetAbsolutePathName (GetAssetsFilename)
End Function


'------------------------------------------------------------------------------
' Returns the filename of the asset that is actually sent to console (in the
'  "\Target Resources" sub directory, of strRoot )

Function BuildTargetResourceFilename (Asset, strRoot)

    ' Generate the source
     Dim assetFile
     assetFile = GetAssetsFilename (Asset, GetParam ("WKSRESOURCEROOT") )
            
     Dim fileExtension
     fileExtension = FileSystem.GetExtensionName (assetFile)

     BuildTargetResourceFilename = BuildPath ( strRoot, "Target Resources\" & _
                                               Asset.UID & "." & fileExtension )

End Function


'------------------------------------------------------------------------------
' Returns the object type of an API object as a string
Function GetTypeString (APIObject)
   Select Case APIObject.IDType
   Case otGame                   GetTypeString = "Game"
   Case otEntity                 GetTypeString = "Entity"
   Case otFolder                 GetTypeString = "Folder"
   Case otAsset                  GetTypeString = "Asset"
   Case otAssetFolder            GetTypeString = "AssetFolder"
   Case otTemplate               GetTypeString = "Template"
   Case otTemplateFolder         GetTypeString = "TemplateFolder"
   Case otBehavior               GetTypeString = "Behavior"
   Case otBehaviorFolder         GetTypeString = "BehaviorFolder"
   Case otCommand                GetTypeString = "Command"
   Case otAttribute              GetTypeString = "Attribute"
   Case otAttributeShare         GetTypeString = "AttributeShare"
   Case otAttributeShareFolder   GetTypeString = "AttributeShareFolder"
   Case otEvent                  GetTypeString = "Event"
   Case Else                     GetTypeString = "Unknown"
   End Select
End Function

'------------------------------------------------------------------------------
' Returns True if the object is nested in the hierarchy of the folder.
' Function searches up the hierarchy for performance reasons.
Function IsInFolder (Folder, Object, PlatformFilter) ' As Boolean 
    Dim Entity, AssetFolder, ParentFolder, AttributeShare, Asset
    IsInFolder = False 
 
    Select Case Object.IDType 
    Case otFolder:
        If PlatformFilter.IsValidPlatform (Object.Platform) Then
            If Object = Folder Or Object.ParentFolders.Exists (Folder) Then
                IsInFolder = True
            Else
                ' Check parent folders
                For Each ParentFolder In Object.ParentFolders
                    If PlatformFilter.IsValidPlatform (ParentFolder.Platform) And IsInFolder (Folder, ParentFolder, PlatformFilter) Then
                        IsInFolder = True
                        Exit Function
                    End If
                Next
            End If
        End If

    Case otAsset:
        If PlatformFilter.IsValidPlatform (Object.Platform) Then
            ' Assets can be parented by folders, entities and asset folders
            If Object.ParentFolders.Exists (Folder) Then
                IsInFolder = True
            Else
                ' Check parent entities
                For Each Entity In Object.ParentEntities
                    If PlatformFilter.IsValidPlatform (Entity.Platform) And IsInFolder (Folder, Entity, PlatformFilter) Then
                        IsInFolder = True
                        Exit Function
                    End If
                Next

                ' Check parent asset folders
                For Each AssetFolder In Object.ParentAssetFolders
                    If PlatformFilter.IsValidPlatform (AssetFolder.Platform) And IsInFolder (Folder, AssetFolder, PlatformFilter) Then
                        IsInFolder = True
                        Exit Function
                    End If
                Next

                ' Check parent folders
                For Each ParentFolder In Object.ParentFolders
                    If PlatformFilter.IsValidPlatform (ParentFolder.Platform) And IsInFolder (Folder, ParentFolder, PlatformFilter) Then
                        IsInFolder = True
                        Exit Function
                    End If
                Next

                ' Check parent assets
                For Each Asset In Object.ParentAssets
                    If PlatformFilter.IsValidPlatform (Asset.Platform) And IsInFolder (Folder, Asset, PlatformFilter) Then
                        IsInFolder = True
                        Exit Function
                    End If
                Next
            End If 
        End If
 
    Case otEntity:
        If PlatformFilter.IsValidPlatform (Object.Platform) Then
            ' Entities can be parented by folders
            If Object.ParentFolders.Exists (Folder) Then
                IsInFolder = True  
            Else  
                ' Check parent folders
                For Each ParentFolder In Object.ParentFolders
                    If PlatformFilter.IsValidPlatform (ParentFolder.Platform) And IsInFolder (Folder, ParentFolder, PlatformFilter) Then
                        IsInFolder = True  
                        Exit Function  
                    End If  
                Next  
            End If  
        End If

    Case otAttribute:
        If PlatformFilter.IsValidPlatform (Object.Platform) Then
            ' Attributes can be parented by entities or attribute shares
            For Each Entity In Object.ParentEntities
                If PlatformFilter.IsValidPlatform (Entity.Platform) And IsInFolder (Folder, Entity, PlatformFilter) Then
                    IsInFolder = True
                    Exit Function
                End If
            Next

            For Each AttributeShare In Object.ParentAttributeShares
                If IsInFolder (Folder, AttributeShare, PlatformFilter) Then
                    IsInFolder = True
                    Exit Function
                End If
            Next
        End If

    Case otAssetFolder:
        If PlatformFilter.IsValidPlatform (Object.Platform) Then
            ' Asset folders can be parented by asset folders, folders and entities
            If Object.ParentFolders.Exists (Folder) Then
                IsInFolder = True
            Else
                For Each Entity In Object.ParentEntities
                    If PlatformFilter.IsValidPlatform (Entity.Platform) And IsInFolder (Folder, Entity, PlatformFilter) Then
                        IsInFolder = True
                        Exit Function
                    End If
                Next

                For Each AssetFolder In Object.ParentAssetFolders
                    If PlatformFilter.IsValidPlatform (AssetFolder.Platform) And IsInFolder (Folder, AssetFolder, PlatformFilter) Then
                        IsInFolder = True
                        Exit Function
                    End If
                Next

                For Each ParentFolder In Object.ParentFolders
                    If PlatformFilter.IsValidPlatform (ParentFolder.Platform) And IsInFolder (Folder, ParentFolder, PlatformFilter) Then
                       IsInFolder = True
                       Exit Function
                    End If
                Next
            End If
        End If

    Case otAttributeShare:
        ' Attribute shares can be parented by entities
        For Each Entity In Object.ParentEntities
            If PlatformFilter.IsValidPlatform (Entity.Platform) And IsInFolder (Folder, Entity, PlatformFilter) Then
                IsInFolder = True
                Exit Function
            End If
        Next
    End Select
End Function

' ------------------------------------------------------------------------------
' Returns a container of all assets below the given object in the hierarchy
Function GetChildAssets (APIObject, PlatformFilter)
   Set GetChildAssets = CreateObject ("CSL.RWSScript.RWSContainer")
   GetChildAssetsHelper APIObject, PlatformFilter, GetChildAssets
End Function

' ------------------------------------------------------------------------------
' Populates a container of all child assets of the given APIObject
Sub GetChildAssetsHelper (APIObject, PlatformFilter, Assets)
   Dim Folder, AssetFolder, Entity, Asset, ChildAsset
   Select Case APIObject.IDType
      Case otFolder:
         ' Assets can be inside child folders
         For Each Folder In APIObject.ChildFolders
            If PlatformFilter.IsValidPlatform (Folder.Platform) Then
               GetChildAssetsHelper Folder, Platformfilter, Assets
            End If
         Next

         ' Assets can be inside child asset folders
         For Each AssetFolder In APIObject.ChildAssetFolders
            If PlatformFilter.IsValidPlatform (AssetFolder.Platform) Then
               GetChildAssetsHelper AssetFolder, Platformfilter, Assets
            End If
         Next

         ' Assets can be inside child entities
         For Each Entity In APIObject.ChildEntities
            If PlatformFilter.IsValidPlatform (Entity.Platform) Then
               GetChildAssetsHelper Entity, Platformfilter, Assets
            End If
         Next

         ' Assets can be children of the folder - and have children themselves
         For Each Asset In APIObject.ChildAssets
            If PlatformFilter.IsValidPlatform (Asset.Platform) Then
               Assets.Add Asset
               GetChildAssetsHelper Asset, Platformfilter, Assets
            End If
         Next

      Case otAssetFolder:

         ' Assets can be inside child asset folders
         For Each AssetFolder In APIObject.ChildAssetFolders
            If PlatformFilter.IsValidPlatform (AssetFolder.Platform) Then
               GetChildAssetsHelper AssetFolder, Platformfilter, Assets
            End If
         Next

         ' Assets can be children of an asset folder - and have children themselves
         For Each Asset In APIObject.ChildAssets
            If PlatformFilter.IsValidPlatform (Asset.Platform) Then
               Assets.Add Asset
               GetChildAssetsHelper Asset, Platformfilter, Assets
            End If
         Next

      Case otEntity, otTemplate:
         ' Assets can be inside an entities child asset folders
         For Each AssetFolder In APIObject.ChildAssetFolders
            If PlatformFilter.IsValidPlatform (AssetFolder.Platform) Then
               GetChildAssetsHelper AssetFolder, Platformfilter, Assets
            End If
         Next

         ' Assets can be an entities child asset - and have children themselves
         For Each Asset In APIObject.ChildAssets
            If PlatformFilter.IsValidPlatform (Asset.Platform) Then
               Assets.Add Asset
               GetChildAssetsHelper Asset, Platformfilter, Assets
            End If
         Next

      Case otAsset:
         ' Assets can be children of other assets.
         For Each ChildAsset In APIObject.ChildAssets
            If PlatformFilter.IsValidPlatform (ChildAsset.Platform) Then
               Assets.Add ChildAsset
               GetChildAssetsHelper ChildAsset, Platformfilter, Assets
            End If
         Next

      Case Else
         Stop ' Fire up a debugger. Unhandled type.
   End Select
End Sub

' ------------------------------------------------------------------------------
' Returns a container of all entities that are ancestors of the given object
Function GetParentEntities (APIObject)
   Set GetParentEntities = CreateObject ("CSL.RWSScript.RWSContainer")
   GetParentEntitiesHelper APIObject, GetParentEntities
End Function

' ------------------------------------------------------------------------------
' Populates a container with entities that are ancestors of the given entity 
Sub GetParentEntitiesHelper (APIObject, Entities)
   Dim AssetFolder, Entity, AttributeShare

   Select Case APIObject.IDType
   Case otAsset:
      ' Entities can be ancestors of asset folders
      For Each AssetFolder In APIObject.ParentAssetFolders
         GetParentEntitiesHelper AssetFolder, Entities
      Next

      ' Entities can be parents of the asset
      For Each Entity In APIObject.ParentEntities
         Entities.Add Entity
      Next

   Case otAssetFolder:
      ' Entities can be ancestors of the asset folder
      For Each AssetFolder In APIObject.ParentAssetFolders
         GetParentEntitiesHelper AssetFolder, Entities
      Next

      ' Entities can be parents of the asset folder
      For Each Entity In APIObject.ParentEntities
         Entities.Add Entity
      Next

   Case otAttribute:
      ' Entities can be ancestors of the attributes parent attribute shares
      For Each AttributeShare In APIObject.ParentAttributeShares
         GetParentEntitiesHelper AttributeShare, Entities
      Next

      ' Entities can be parents of the attribute
      For Each Entity In APIObject.ParentEntities
         Entities.Add Entity
      Next

   Case otAttributeShare:
      ' Entities can be parents of the attribute share
      For Each Entity In APIObject.ParentEntities
         Entities.Add Entity
      Next

   Case Else
      Stop ' Fire up the debugger. Unhandled type.
   End Select
End Sub

' ------------------------------------------------------------------------------
' 
Function GetChildEntities (APIObject, PlatformFilter)
   Dim ParentEntities
   Set ParentEntities = CreateObject ("CSL.RWSScript.RWSContainer")
   GetChildEntitiesHelper APIObject, PlatformFilter, ParentEntities
   Set GetChildEntities = ParentEntities
End Function

' ------------------------------------------------------------------------------
' 
Sub GetChildEntitiesHelper (APIObject, PlatformFilter, Entities)
   Dim Folder, Entity

   Select Case APIObject.IDType
   Case otFolder:
      ' Entities exist as children of folders
      For Each Folder In APIObject.ChildFolders
         If PlatformFilter.IsValidPlatform (Folder.Platform) Then
            GetChildEntitiesHelper Folder, PlatformFilter, Entities
         End If
      Next

      For Each Entity In APIObject.ChildEntities
         If PlatformFilter.IsValidPlatform (Entity.Platform) Then
            Entities.Add Entity
         End If
      Next

   Case Else
      Stop ' Fire up the debugger. Unhandled type.
   End Select
End Sub

' ------------------------------------------------------------------------------
' Similar to FileSystem.BuildPath except if the FileName has an absolute path then
' the Path and Filename aren't concatenated.
Function BuildPath (Path, Name)
   Dim Drive
            
   Drive = FileSystem.GetDriveName ( Name )

   ' Don't append file to path if absolute file name
   If Len (Drive) = 0 Then
      BuildPath = FileSystem.BuildPath (Path, Name)
   Else
      BuildPath = Name
   End If

End Function

'------------------------------------------------------------------------------
'
Function GetTDGeneratorExe ()
   Dim strPlatformFlags, strCommand

   ' Work out which platform we're generating dictionary for
   strPlatformFlags = LCase (GetParam ("PLATFORMFLAGS"))
   If InStr (1, strPlatformFlags, "directx") > 0 Then
      strCommand = "tdgen_dx.exe"
   ElseIf InStr (1, strPlatformFlags, "opengl") > 0 Then
      strCommand = "tdgen_gl.exe"
   ElseIf InStr (1, strPlatformFlags, "ps2") > 0 Then
      strCommand = "tdgen_p2.exe"
   ElseIf InStr (1, strPlatformFlags, "gamecube") > 0 Then
      strCommand = "tdgen_gc.exe"
   ElseIf InStr (1, strPlatformFlags, "xbox") > 0 Then
      strCommand = "tdgen_xb.exe"
   Else
      BuildWarning "GetTDGeneratorExe: Unrecognised platform flags: " & strPlatformFlags _
                   & " (defaulting to DirectX tool.)"
      strCommand = "tdgen_dx.exe"
   End If

   GetTDGeneratorExe = GetParam ("TOOLDIR") & "\" & strCommand
End Function

'------------------------------------------------------------------------------
'
Function GetStripRWSIdsExe ()
   GetStripRWSIdsExe = GetParam ("TOOLDIR") & "\stripud.exe"
End Function

'------------------------------------------------------------------------------
'
'  Checks the Type of an asset to see if it is one of the texture formats
'  supported in RenderWare Graphics
'
Function AssetIsTextureFile ( Asset )

   Select Case Asset.Type
   Case "BMP", "DDS", "PNG", "RAS", "TGA", "TIF"
      AssetIsTextureFile = True
   Case Else
      AssetIsTextureFile = False
   End Select

End Function

'------------------------------------------------------------------------------
'
Function GetHullGeneratorExe ()
   GetHullGeneratorExe = GetParam ("TOOLDIR") & "\HullGen.exe"
End Function

'------------------------------------------------------------------------------
'
Function GetFontGeneratorExe ()
   Dim strPlatformFlags, strCommand

   ' Work out which platform we're generating font for
   strPlatformFlags = LCase (GetParam ("PLATFORMFLAGS"))
   If InStr (1, strPlatformFlags, "directx") > 0 Then
      strCommand = "fontgen_dx.exe"
   ElseIf InStr (1, strPlatformFlags, "opengl") > 0 Then
      strCommand = "fontgen_gl.exe"
   ElseIf InStr (1, strPlatformFlags, "ps2") > 0 Then
      strCommand = "fontgen_p2.exe"
   ElseIf InStr (1, strPlatformFlags, "gamecube") > 0 Then
      strCommand = "fontgen_gc.exe"
   ElseIf InStr (1, strPlatformFlags, "xbox") > 0 Then
      strCommand = "fontgen_xb.exe"
   Else
      BuildWarning "GetFontGeneratorExe: Unrecognised platform flags: " & strPlatformFlags _
                   & " (defaulting to DirectX tool.)"
      strCommand = "fontgen_dx.exe"
   End If

   GetFontGeneratorExe = GetParam ("TOOLDIR") & "\" & strCommand
End Function


'------------------------------------------------------------------------------
'
'  Checks the Type of an asset to see if it is a font metrics file
'
Function AssetIsFontMetrics ( Asset )

   AssetIsFontMetrics = False
   If Asset.Type = "MET" Then
      AssetIsFontMetrics = True
   End If

End Function
