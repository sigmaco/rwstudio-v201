'==============================================================================
' BufferTools.vbs
'==============================================================================

Option Explicit ' All variables must be explicitly declared (with Dim)

'------------------------------------------------------------------------------
' Chunk headers handled by the game framework - see strfunc.h on the console.
Const strfunc_VersionNumber = -1
Const strfunc_Reset = 0
'Const strfunc_Reserved1 = 1
'Const strfunc_Reserved2 = 2
Const strfunc_SetDirectorsCameraMatrix = 3
Const strfunc_CreateEntity = 4
Const strfunc_UpdateEntityAttributes = 5
Const strfunc_SetFrozenMode = 6
Const strfunc_SetRunningMode = 7
Const strfunc_SetEnableDirectorsCamera = 8
Const strfunc_SetDisableDirectorsCamera = 9
Const strfunc_TextComment = 10
Const strfunc_StartSystem = 11
Const strfunc_StopSystem = 12
Const strfunc_DeleteEntity = 13
Const strfunc_DeleteAllEntities = 14
Const strfunc_UnLoadAsset = 15
Const strfunc_Shutdown = 16
Const strfunc_CloseConnection = 17
Const strfunc_SendTestEvent = 18
'Const strfunc_Reserved3 = 19
'Const strfunc_Reserved3b = 20
Const strfunc_LoadAsset = 21
Const strfunc_LoadEmbeddedAsset = 22
'Const strfunc_Reserved4 = 23
Const strfunc_GetCameraMatrix = 24
Const strFunc_CustomData = 25
Const strFunc_FunctionProfiler   = 26
Const strfunc_ResetEntity = 27
Const strFunc_PlacementNew = 28
Const strfunc_Initialize = 29
Const strfunc_UpdateAsset = 30
Const strfunc_SequenceUpdate = 31
Const strfunc_SequencePreview = 32

' Attribute packet headers
Const RWSPH_CLASSID       = &H80000000
Const RWSPH_INSTANCEID    = &H40000000
Const RWSPH_CREATECLASSID = &H20000000

' Used to attach assets to an entity
Const strCSystemCommands = "CSystemCommands"
Const CMD_AttachResource = 0

Const StreamVersionNumber = &H0100

Dim RWSScript
Set RWSScript = CreateObject ("CSL.RWSScript.RWSScript")

' -----------------------------------------------------------------------------
' Padding byte used to pad binary data to 4-byte boundaries
Const PadByte = &HBF ' ANSI character code for upside down question mark.

' -----------------------------------------------------------------------------
' Writes placement new information into a buffer
Sub AppendPlacementNew (Buffer, EntityPacketList, EntityListCnt)
	Dim Count
	' Write out chunk header so that Framework knows what's coming.
	Buffer.WriteDataHeader strFunc_PlacementNew

	' Write out the number of entity entries in chunk
	Buffer.Write dtUInt32, EntityListCnt

	' For each entity in the list, output to the stream the behavior name and count.
	For Count = 0 To EntityListCnt
		' Write the packet out for this entity type
		WritePaddedString Buffer, EntityPacketList(0, Count)
		Buffer.Write dtInt32, EntityPacketList(1, Count)
	Next
End Sub

' -----------------------------------------------------------------------------
' Resets the console
Sub AppendReset (Buffer)
   Buffer.WriteDataHeader strfunc_Reset
End Sub

' -----------------------------------------------------------------------------
' Enables the directors camera on the console
Sub AppendEnableDirectorsCamera (Buffer)
   Buffer.WriteDataHeader strfunc_SetEnableDirectorsCamera
End Sub

' -----------------------------------------------------------------------------
' Disables the directors camera on the console
Sub AppendDisableDirectorsCamera (Buffer)
   Buffer.WriteDataHeader strfunc_SetDisableDirectorsCamera
End Sub

' -----------------------------------------------------------------------------
' Stops the console
Sub AppendStopSystem (Buffer)
   Buffer.WriteDataHeader strfunc_StopSystem
End Sub

' -----------------------------------------------------------------------------
' Starts the console
Sub AppendStartSystem (Buffer)
   Buffer.WriteDataHeader strfunc_StartSystem
End Sub

' -----------------------------------------------------------------------------
' Pauses the console
Sub AppendFreezeSystem (Buffer)
   Buffer.WriteDataHeader strfunc_SetFrozenMode
End Sub

' -----------------------------------------------------------------------------
' Runs the console
Sub AppendRunSystem (Buffer)
   Buffer.WriteDataHeader strfunc_SetRunningMode
End Sub

' -----------------------------------------------------------------------------
' Closes a connection. Must be sent before disconnecting from a console.
Sub AppendCloseConnection (Buffer)
   Buffer.WriteDataHeader strfunc_CloseConnection
End Sub

' -----------------------------------------------------------------------------
' Tells the framework to get ready for a new game. All current assets and 
' entities will be destroyed.
Sub AppendInitialize (Buffer)
   Buffer.WriteDataHeader strfunc_Initialize
End Sub

' -----------------------------------------------------------------------------
' Sends a version number to the console
Sub AppendVersionNumber (Buffer, VersionNumber)
   Buffer.WriteDataHeader strfunc_VersionNumber
   Buffer.Write dtUInt32, VersionNumber
End Sub

' -----------------------------------------------------------------------------
' Sends a text comment to the console
Sub AppendTextComment (Buffer, TextComment)
   Buffer.WriteDataHeader strfunc_TextComment
   Buffer.Write dtChar, TextComment
End Sub

' -----------------------------------------------------------------------------
' Sent to the console in preparation for sending the game
Sub AppendSystemPreSendGame (Buffer)
   AppendInitialize Buffer
   AppendDisableDirectorsCamera Buffer
   AppendStopSystem Buffer
   AppendFreezeSystem Buffer
   AppendVersionNumber Buffer, StreamVersionNumber
End Sub

'------------------------------------------------------------------------------
' Writes a command to update the data for a hull asset to the given buffer.
Sub AppendUpdateHullAsset (Buffer, strHullFilePath, AssetUID)

   ' Write the header and UID
   Buffer.WriteDataHeader strfunc_UpdateAsset
   WriteIntegerUID Buffer, AssetUID

   ' Add the actual asset into the stream
   Dim ResourceBuffer
   Set ResourceBuffer = RWSComms.CreateFileBuffer (strHullFilePath, faRead)
   If Not ResourceBuffer Is Nothing Then
      Buffer.WriteBuffer ResourceBuffer, IncludeInData
   Else
      Buffer.Write dtUInt32, 0
      BuildError ("[AppendUpdateHullAsset] Could not find file '" & strHullFilePath & _
                    "' for hull " & AssetUID)
   End If
End Sub

'------------------------------------------------------------------------------
' Writes the data for an asset to the given buffer.
Sub AppendCreateAssetData (Buffer, AssetName, AssetUID, AssetType, _
                           strFilePath, strDependencyRoot, IsGlobal)
   Dim PacketSize, ResourceBuffer

   If GetParam ("EMBEDASSETS") Then
      ' The PacketSize is the size of each of the strings (padded to a multiple of 4Bytes),
      ' plus the padded size of each string (4 * 4Bytes),
      ' plus the size of the integer GUID (16Bytes)
      PacketSize = PaddedSize (Len (AssetName) + 1) + 16 + _
                   PaddedSize (Len (AssetType) + 1) + _
                   PaddedSize (Len (strFilePath) + 1) + _
                   PaddedSize (Len (strDependencyRoot) + 1) + 16 + 4

      Buffer.WriteDataHeader strfunc_LoadEmbeddedAsset
      Buffer.Write dtUInt32, PacketSize
   Else
      Buffer.WriteDataHeader strfunc_LoadAsset
   End If

   ' Write the header for the asset
   Buffer.Write dtUInt32, PaddedSize (Len (AssetName) + 1)
   WritePaddedString Buffer, AssetName
   WriteIntegerUID Buffer, AssetUID
   Buffer.Write dtUInt32, PaddedSize (Len (AssetType) + 1)
   WritePaddedString Buffer, AssetType
   Buffer.Write dtUInt32, PaddedSize (Len (strFilePath) + 1)
   WritePaddedString Buffer, strFilePath
   Buffer.Write dtUInt32, PaddedSize (Len (strDependencyRoot) + 1)
   WritePaddedString Buffer, strDependencyRoot
   Buffer.Write dtUInt32, IsGlobal

   If GetParam ("EMBEDASSETS") Then
      ' Add the actual asset into the stream
      Set ResourceBuffer = RWSComms.CreateFileBuffer (strFilePath, faRead)
      If Not ResourceBuffer Is Nothing Then
         Buffer.Write dtUInt32, ResourceBuffer.Size
         Buffer.WriteBuffer ResourceBuffer, IncludeInData
      Else
         Buffer.Write dtUInt32, 0
         BuildError ("[AppendCreateAssetData] Could not find file '" & strFilePath & _
                       "' for asset '" & AssetName & "' with ID '" & AssetUID & "'")
      End If
   End If

End Sub

'------------------------------------------------------------------------------
' Append the given asset to a buffer.
Sub AppendCreateAsset (Buffer, Asset, PlatformFilter)

   Dim strTgtResourceRoot, strFilePath, strDependencyRoot, GlobalFolder, IsGlobal

   ' Generate the resouce and file paths
   strTgtResourceRoot = GetParam ("TGTRESOURCEROOT")

   If GetParam ("EMBEDASSETS") Then
      strFilePath     = BuildTargetResourceFilename (Asset, GetParam ("DESTDIR"))
   Else
      strFilePath     = BuildTargetResourceFilename (Asset, GetParam ("TGTRESOURCEROOT"))
   End If

   strDependencyRoot  = BuildPath (strTgtResourceRoot, Asset.Dependencies)

   ' Is this asset global?
   IsGlobal = False
   Set GlobalFolder = RWSScript.Game.GlobalFolder
   If Not GlobalFolder Is Nothing Then
      If IsInFolder (GlobalFolder, Asset, PlatformFilter) Then
         IsGlobal = True
      End If
   End If

   ' Do we need to send the name to target?
   Dim strAssetName
   strAssetName = ""
   If Asset.SendName Then
      strAssetName = Asset.Name
   End If

   ' Append the data for the asset to the given buffer
   AppendCreateAssetData Buffer, strAssetName, Asset.UID, Asset.Type, _
                         strFilePath, strDependencyRoot, IsGlobal

End Sub

'------------------------------------------------------------------------------
' Request camera matrix
Sub AppendGetCameraMatrix (Buffer)
   Buffer.WriteDataHeader strfunc_GetCameraMatrix
End Sub

' ----------------------------------------------------------------------------
' Appends the entity UID to an attribute packet
Sub AppendAttributePacketEntityUID (Buffer, EntityUID)
    Buffer.Write dtUInt32, 8 + 16   ' Size of 2 uints and the guid data
    Buffer.Write dtUInt32, RWSPH_INSTANCEID
    WriteIntegerUID Buffer, EntityUID
End Sub

'------------------------------------------------------------------------------ 
' Appends the class UID to an attribute packet. This is used to indicate that
' the attribute packets that follow are directed at this class type.
Sub AppendAttributePacketClassUID (Buffer, ClassUID)
    Buffer.Write dtUInt32, 8 + PaddedSize (Len (ClassUID) + 1) 
    Buffer.Write dtUInt32, RWSPH_CLASSID 
    WritePaddedString Buffer, ClassUID
End Sub

'------------------------------------------------------------------------------ 
' Appends the class UID to an attribute packet. The console will construct an
' instance of the given class and send any following attributes to it.
Sub AppendAttributePacketCreateClassUID (Buffer, ClassUID)
   Buffer.Write dtUInt32, 8 + PaddedSize (Len (ClassUID) + 1)
   Buffer.Write dtUInt32, RWSPH_CREATECLASSID
   WritePaddedString Buffer, ClassUID
End Sub

'------------------------------------------------------------------------------
' Appends the Entity Attribute to an attribute packet.
Sub AppendAttributePacketEntityAttribute (Buffer, Attribute, Index)
   Dim AttributeData, Section, Element
   Set AttributeData = Attribute.Data

   ' Write the header to identify the attribute size and index
   Buffer.Write dtUInt32, 8 + GetAttributeDataSize (AttributeData)
   Buffer.Write dtUInt32, Index

   For Each Section In AttributeData
      ' Write attribute data to buffer
      For Each Element In Section
         Buffer.Write Section.Type, Element
      Next

      ' Pad bytes or strings to 4 byte boundary
      Dim PadCount, PadIndex
      PadCount = 0

      If Section.Type = dtByte Then
         PadCount = PaddedSize (Section.Count) - Section.Count
      ElseIf Section.Type = dtChar Then
         If Buffer.Unicode Then
            PadCount = PaddedSize (Section.Count*2) - (Section.Count*2)
         Else
            PadCount = PaddedSize (Section.Count) - Section.Count
         End If
      End If

      For PadIndex = 0 To PadCount - 1
         Buffer.Write dtByte, PadByte
      Next
   Next
End Sub

'------------------------------------------------------------------------------
' Appends all entity's attributes to the buffer for the given behavior.
' Note: The entity UID is not appended before sending these attributes.
' Note: Default attributes are appended for those that do not exist.
Sub AppendAttributePacketAllEntityAttributes (Buffer, Entity, Behavior)
   Dim Commands, Command, ChildBehavior, ClassUID, Attribute

   Set Commands = Behavior.ChildCommands
   If Commands.Count Then
      ' Append the class name that the attributes are targeted at
      AppendAttributePacketClassUID Buffer, Behavior.UID

      ' Append attribute for each command in behavior
      Dim Index
      Index = 0

      For Each Command In Commands
         
         ' Need to check command type so that we ignore RWS_SEPARATOR and get
         ' the correct index.
         '
         If Command.Name = "RWS_ATTRIBUTE" Or Command.Name = "RWS_MESSAGE" Then

            If FindEntityAttributeFromCommand (Entity, Behavior.UID, Command, Attribute) Then
               ' Attribute found. Append it to the buffer
               AppendAttributePacketEntityAttribute Buffer, Attribute, Index
            Else
               ' Attribute not found. Attempt to add the default attribute
               On Error Resume Next
               Set Attribute = Command.DefaultAttribute
               If Err.Number = 0 Then
                  AppendAttributePacketEntityAttribute Buffer, Attribute, Index
               End If
               On Error Goto 0
            End If

            Index = Index + 1
         End If
      Next
   End If

   ' Append each child behavior attributes
   For Each ChildBehavior In Behavior.ChildBehaviors
      AppendAttributePacketAllEntityAttributes Buffer, Entity, ChildBehavior
   Next
End Sub

'------------------------------------------------------------------------------
'
Sub AppendAttributePacketAttachAssetsToEntity (Buffer, Entity, Assets)

   If Assets.Count Then
      ' Attach the assets to the entity
      AppendAttributePacketClassUID Buffer, strCSystemCommands

      Dim Asset
      For Each Asset In Assets
         ' Don't attach root rf3 assets to the entity.
         If Not AssetIsRootRF3 (Asset) Then
            Buffer.Write dtUInt32, 8 + 16   ' Size of 2 uints and the guid data
            Buffer.Write dtUInt32, CMD_AttachResource
            WriteIntegerUID Buffer, Asset.UID
         End If
      Next
   End If

   ' If this is a volume entity, we need to attach its hull asset
   If EntityIsVolume (Entity) Then

      If Assets.Count = 0 Then ' Specify CSystemCommands target class
         AppendAttributePacketClassUID Buffer, strCSystemCommands
      End If

      Buffer.Write dtUInt32, 8 + 16   ' Size of 2 uints and the guid data
      Buffer.Write dtUInt32, CMD_AttachResource
      WriteIntegerUID Buffer, Entity.UID ' The asset's UID is actually the entity's!

   End If
End Sub

'------------------------------------------------------------------------------ 
' Terminates an attribute packet
Sub AppendAttributePacketTerminator (Buffer)
    Buffer.Write dtUInt32, 0  ' Null size and id indicate terminator
    Buffer.Write dtUInt32, 0
End Sub

'------------------------------------------------------------------------------
'
Sub AppendAllEntityAttributes (Buffer, Entity)
   Dim Behavior

   Buffer.WriteDataHeader strfunc_UpdateEntityAttributes
   AppendAttributePacketEntityUID Buffer, Entity.UID

   ' Append the attributes to the entity
   AppendAttachAttributesToEntity Buffer, Entity

   ' Terminate the attribute packet
   AppendAttributePacketTerminator Buffer
End Sub

'------------------------------------------------------------------------------
'
Sub CreateChildAssetList (PlatformFilter, Parent, AssetList)
   Dim AssetFolder, Asset

   For Each Asset In Parent.ChildAssets
      If PlatformFilter.IsValidPlatform (Asset.Platform) Then
         AssetList.Add Asset
         ' Assets can parent other assets.
         CreateChildAssetList PlatformFilter, Asset, AssetList
      End If
   Next

   ' Assets don't have ChildAssetFolders.
   If Not Parent.IDType = otAsset Then
      For Each AssetFolder In Parent.ChildAssetFolders
         If PlatformFilter.IsValidPlatform (AssetFolder.Platform) Then
            CreateChildAssetList PlatformFilter, AssetFolder, AssetList
         End If
      Next
   End If
End Sub

'------------------------------------------------------------------------------
'
'  Typically called from AppendEntityAttributePacket, add to the packet 
'  whether the entity is in the global folder or not 
'
Sub AppendEntityIsGlobal (PlatformFilter, Buffer, Entity)
   
   Dim GlobalFolder

   ' Append a flag denoting whether this entity is in the global level
   Set GlobalFolder = RWSScript.Game.GlobalFolder
   If Not GlobalFolder Is Nothing Then
      If IsInFolder (GlobalFolder, Entity, PlatformFilter) Then
         Buffer.Write dtUInt32, True
      Else
         Buffer.Write dtUInt32, False
      End If
   Else
      Buffer.Write dtUInt32, False
   End If

End Sub

'------------------------------------------------------------------------------
'
'  Typically called from AppendEntityAttributePacket, add to the packet 
'  the attach resource commands for each resource
'
Sub AppendAttachAssetsToEntity (PlatformFilter, Buffer, Entity)
   Dim Assets

   Set Assets = CreateObject ("CSL.RWSScript.RWSContainer")

   ' Add any assets that belong to this platform
   CreateChildAssetList PlatformFilter, Entity, Assets

   ' Attach any assets to the entity 
   AppendAttributePacketAttachAssetsToEntity Buffer, Entity, Assets
End Sub

'------------------------------------------------------------------------------
'
'  Typically called from AppendEntityAttributePacket, add to the packet 
'  the attributes associated with this entity.
'
Sub AppendAttachAttributesToEntity (Buffer, Entity)
   Dim Behavior

   ' Append the attributes to the entity
   Set Behavior = RWSScript.Behavior (Entity.Behavior)
   If Not Behavior Is Nothing Then
      AppendAttributePacketAllEntityAttributes Buffer, Entity, Behavior
   Else
      BuildWarning ("[BufferTools] Behavior """ & Entity.Behavior & _
                    """ not found for entity """ & Entity.Name & """")
   End If

   ' If this entity has additional behaviours attached besides the primary one,
   ' send these (they're attached as properties)
   Dim Property, PropertyData
   Set Property = RWSScript.Property ("RWS_ADDITIONAL_BEHAVIORS")

   Set PropertyData = Property.Get (Entity)
   If Not PropertyData Is Nothing Then

      ' Got the property, now get the list of behaviours from it
      Dim strBehaviour

      ' Loop over all sections in data
      If Not PropertyData Is Nothing Then

         Dim DataSection, DataSections
         For Each DataSection In PropertyData

            ' Copy all characters from behaviour name
            If DataSection.Type = dtChar Then
               strBehaviour = ""

               Dim nChar
               For Each nChar In DataSection
                  strBehaviour = strBehaviour + Chr (nChar)
               Next

               ' OK, attach this behaviour
               Set Behavior = RWSScript.Behavior (strBehaviour)
               If Not Behavior Is Nothing Then
                  AppendAttributePacketAllEntityAttributes Buffer, Entity, Behavior
               Else
                  BuildWarning ("[BufferTools] Additional Behavior """ & strBehaviour & _
                                """ not found for entity """ & Entity.Name & """")
               End If

            End If
         Next
      End If
   End If
End Sub

'------------------------------------------------------------------------------
'
'  This function builds up a standard create entity packet.
'
Sub AppendEntityAttributePacket (PlatformFilter, Buffer, Entity)
   
   ' Is this a global entity
   '
   AppendEntityIsGlobal PlatformFilter, Buffer, Entity

   ' Add create entity of type
   '
   AppendAttributePacketCreateClassUID Buffer, Entity.Behavior

   ' Add attributes follow
   '
   AppendAttributePacketEntityUID Buffer, Entity.UID

   ' Add attach assets
   '
   AppendAttachAssetsToEntity PlatformFilter, Buffer, Entity

   ' Add attributes
   '
   AppendAttachAttributesToEntity Buffer, Entity

   ' Terminate
   '
   AppendAttributePacketTerminator Buffer

End Sub

'------------------------------------------------------------------------------
'
Sub AppendCreateEntity (PlatformFilter, Buffer, Entity)
   Buffer.WriteDataHeader strfunc_CreateEntity
   AppendEntityAttributePacket PlatformFilter, Buffer, Entity
End Sub

'------------------------------------------------------------------------------
'
Sub AppendResetEntity (PlatformFilter, Buffer, Entity)
   Buffer.WriteDataHeader strfunc_ResetEntity
   AppendEntityAttributePacket PlatformFilter, Buffer, Entity
End Sub

'------------------------------------------------------------------------------
' Updates the directors camera matrix on the console. Matrix is taken from
' some attribute data
Sub AppendUpdateDirectorsCamera (Buffer, Attribute)
   Dim AttributeData, Section, Element

   Set AttributeData = Attribute.Data
   If AttributeData.Count > 0 Then
      Buffer.WriteDataHeader strfunc_SetDirectorsCameraMatrix

      For Each Section In AttributeData
         ' Write attribute data to buffer    
         For Each Element In Section
            Buffer.Write Section.Type, Element
         Next
      Next
   End If
End Sub

'------------------------------------------------------------------------------
' Removes an entity from the console
Sub AppendDeleteEntity (Buffer, Entity)
    Buffer.WriteDataHeader strfunc_DeleteEntity
    AppendAttributePacketEntityUID Buffer, Entity.UID
    AppendAttributePacketTerminator Buffer
End Sub

'------------------------------------------------------------------------------ 
' Removes an asset from the console
Sub AppendUnloadAsset (Buffer, AssetUID)
    Buffer.WriteDataHeader strfunc_UnLoadAsset
    WriteIntegerUID Buffer, AssetUID
End Sub

'------------------------------------------------------------------------------ 
' Appends an entities attribute to the buffer. The AttributeIndex
' is the value returned by GetAttributeIndex.
Sub AppendUpdateEntityAttributes (Buffer, Entity, Attribute, AttributeIndex)
    ' strfunc_UpdateEntityAttributes handles changes to attributes  
    Buffer.WriteDataHeader strfunc_UpdateEntityAttributes
    AppendAttributePacketEntityUID Buffer, Entity.UID
    AppendAttributePacketClassUID Buffer, Attribute.ClassUID

    ' Append the attribute data to the buffer
    AppendAttributePacketEntityAttribute Buffer, Attribute, AttributeIndex  

    AppendAttributePacketTerminator Buffer
End Sub

'------------------------------------------------------------------------------
' Appends an event to be fired on the console
Sub AppendTestEvent (Buffer, strEventName)
   Buffer.WriteDataHeader strfunc_SendTestEvent
   WritePaddedString Buffer, strEventName
End Sub

'------------------------------------------------------------------------------
' Returns the number of bytes required to store some data of Size bytes padded
' to a 4-byte boundary.
Function PaddedSize (Size)
   PaddedSize = Size
   If Not PaddedSize Mod 4 = 0 Then
      PaddedSize = PaddedSize + 4 - (PaddedSize Mod 4)
   End If
End Function

'------------------------------------------------------------------------------
' Writes a string to a buffer, padding the string to a 4-byte boundary.
Sub WritePaddedString (Buffer, TheString)
   ' Write the string into the buffer
   Buffer.Write dtChar, TheString

   ' Calculate how many padding bytes we need
   Dim PadBytes, PadIndex, TheStringLen
   TheStringLen = Len (TheString) + 1 'Including null terminator
   PadBytes = PaddedSize (TheStringLen) - TheStringLen

   ' Append the padding bytes
   For PadIndex = 0 To PadBytes - 1
      Buffer.Write dtChar, PadByte
   Next
End Sub

'------------------------------------------------------------------------------
' Determines the amount of bytes required to store some attribute data
Function GetAttributeDataSize (AttributeData)
   Dim Section, Data
   GetAttributeDataSize = 0

   For Each Section In AttributeData
      Select Case Section.Type
      Case dtBool:    GetAttributeDataSize = GetAttributeDataSize + Section.Count * 32/8
      Case dtInt32:   GetAttributeDataSize = GetAttributeDataSize + Section.Count * 32/8
      Case dtUInt32:  GetAttributeDataSize = GetAttributeDataSize + Section.Count * 32/8
      Case dtReal:    GetAttributeDataSize = GetAttributeDataSize + Section.Count * 32/8
      Case dtDouble:  GetAttributeDataSize = GetAttributeDataSize + Section.Count * 64/8
      Case dtByte:    GetAttributeDataSize = GetAttributeDataSize + PaddedSize (Section.Count *  8/8)
      Case dtChar:    GetAttributeDataSize = GetAttributeDataSize + PaddedSize (Section.Count *  8/8)
      End Select
   Next
End Function

'-----------------------------------------------------------------------------
' Returns the index of the attribute belonging to the entity. This will be the
' same value as the command enumeration that the attribute represents
Function GetAttributeIndex (Attribute)
   Dim Behavior
   GetAttributeIndex = -1

   Set Behavior = RWSScript.Behavior (Attribute.ClassUID)
   If Not Behavior Is Nothing Then
      Dim Commands, Command, Index
      Set Commands = Behavior.ChildCommands

      Index = 0
      For Each Command In Commands
        ' Only attributes and messages contain an attribute index
        If Command.Name = "RWS_ATTRIBUTE" Or Command.Name = "RWS_MESSAGE" Then
           Dim ParamList
           ParamList = Command.ParamList
           If ParamList (0) = Attribute.Member Then
              GetAttributeIndex = Index
              Exit Function
           End If
           Index = Index + 1
        End If
      Next
   End If
End Function

'------------------------------------------------------------------------------
' Searches for an entities attribute corresponding to the given command index
Function FindEntityAttributeFromIndex (Entity, BehaviorUID, CommandIndex, ByRef AttributeRetVal)
   Dim Behavior
   FindEntityAttributeFromIndex = False

   ' Search for the command corresponding to the given index
   Set Behavior = RWSScript.Behavior (BehaviorUID)
   If Not Behavior Is Nothing Then
      Dim Index, Commands, Command

      For Each Command In Behavior.ChildCommands
         If Command.Name = "RWS_ATTRIBUTE" Or Command.Name = "RWS_MESSAGE" Then
            If Index = CommandIndex Then
               ' Now we have the command, we can just call FindEntityAttributeFromCommand
               FindEntityAttributeFromIndex = FindEntityAttributeFromCommand (Entity, BehaviorUID, _
                                                                           Command, AttributeRetVal)
               Exit Function
            End If
            Index = Index + 1
         End If
      Next
   End If
End Function

'------------------------------------------------------------------------------
' Searches for an entities attribute corresponding to the given command..
Function FindEntityAttributeFromCommand (Entity, BehaviorUID, Command, ByRef AttributeRetVal)
   Dim Attribute, ParamList, AttributeShare

   FindEntityAttributeFromCommand = False
   ParamList = Command.ParamList

   For Each Attribute In Entity.ChildAttributes
      If Attribute.ClassUID = BehaviorUID And Attribute.Member = ParamList (0) Then
         FindEntityAttributeFromCommand = True
         Set AttributeRetVal = Attribute
         Exit Function
      End If
   Next

   ' Attribute not found - recurse into attribute shares
   For Each AttributeShare In Entity.ChildAttributeShares
      For Each Attribute In AttributeShare.ChildAttributes
         If Attribute.ClassUID = BehaviorUID And Attribute.Member = ParamList (0) Then
            FindEntityAttributeFromCommand = True
            Set AttributeRetVal = Attribute
            Exit Function
         End If
      Next
   Next
End Function

' -----------------------------------------------------------------------------
' Returns a boolean value indicating whether the attribute is used by the entity
Function EntityUsesAttribute (Entity, Attribute, Index)
   Dim ActualAttribute
   EntityUsesAttribute = False

   If FindEntityAttributeFromIndex (Entity, Attribute.ClassUID, Index, ActualAttribute) Then
      If ActualAttribute = Attribute Then
         EntityUsesAttribute = True
      End If
   End If
End Function

' -----------------------------------------------------------------------------
' Writes a GUID to the given buffer as a series of 4 32bit integer values
Sub WriteIntegerUID (Buffer, GUID)
   Dim i, UIDArray

   UIDArray = RWSScript.UIDToArray (GUID)

   If UBound (UIDArray) > 0 Then
      For i = LBound (UIDArray) To UBound (UIDArray)
         Buffer.Write dtUInt32, UIDArray (i)
      Next
   End If
End Sub

' -----------------------------------------------------------------------------
Function EntityIsVolume (Entity)

   ' Is this a volume entity?
   ' We determine this by looking for the hull-shape property

   Dim Properties, Property
   Set Properties = RWSScript.Properties (Entity)
   For Each Property In Properties
      If Property.UID = "RWSPROP_HULL_SHAPE" Then
         EntityIsVolume = True
         Exit Function
      End If
   Next

   EntityIsVolume = False

End Function

' -----------------------------------------------------------------------------
