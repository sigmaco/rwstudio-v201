
' ------------------------------------------------------------------------------
' SequencerBufferTools.vbs
'
' Requires the following information to be included in the rule:
' 
'      <reference object="CSL.RWSTarget.RWSComms"/>
'      <object id="oRWSScript" progid="CSL.RWSScript.RWSScript"/>
'      <object id="oRWSComms" progid="CSL.RWSTarget.RWSComms"/>
'      <script language="VBScript" src="BufferTools.vbs"/>

' ------------------------------------------------------------------------------
' All variables must be explicitly declared (with Dim)
Option Explicit

' ------------------------------------------------------------------------------
' Instructions
Const conStringTable = 1
Const conGuidTable = 2
Const conReserved2 = 3      ' Reserved for sequence statistics.
Const conWait = 4
Const conEntity = 5
Const conBehavior = 6
Const conAttribute = 7
Const conKeyFrame = 8
Const conInterpolate = 9
Const conTransmitEvent = 10
Const conReceiveEvent = 11
Const conCreateTemplate = 12
Const conDestroyTemplate = 13
Const conDynamicInterpolate = 14


' ------------------------------------------------------------------------------
' Appends a table of strings to the buffer. Other operations reference
' these strings by an offset into the string array.
'
' Data consists of:
' [conStringTable - 4 bytes]
' [Size - 4 bytes]
' [Data - Size bytes]
'
Sub AppendStringTableCommand (oBuffer, oStringTable)
   Dim oDataBuffer, nPaddedBytes, nPaddedIndex
   Set oDataBuffer = oStringTable.Data (oBuffer.BigEndian, oBuffer.Unicode)

   'BuildWarning "[OP-StringTable] - " & oStringTable.Count & " strings"

   ' The string table must be padded to a multiple of 4 bytes.
   nPaddedBytes = PaddedSize (oDataBuffer.Size) - oDataBuffer.Size
   For nPaddedIndex = 1 To nPaddedBytes
      oDataBuffer.Write dtByte, PadByte
   Next

   oBuffer.Write dtUInt32, conStringTable
   oBuffer.Write dtUInt32, oDataBuffer.Size
   oBuffer.WriteBuffer oDataBuffer, IncludeInData
End Sub


' ------------------------------------------------------------------------------
' Appends a table of GUIDs to the buffer. Other operations reference
' these GUIDs by an offset into the guid array.
'
' Data consists of:
' [conGuidTable - 4 bytes]
' [Size - 4 bytes]
' [Data - Size bytes]
'
Sub AppendGuidTableCommand (oBuffer, oGuidTable)
   Dim oDataBuffer
   Set oDataBuffer = oGuidTable.Data (oBuffer.BigEndian, oBuffer.Unicode)

   'BuildWarning "[conGuidTable] - " & oGuidTable.Count & " guids"

   oBuffer.Write dtUInt32, conGuidTable
   oBuffer.Write dtUInt32, oDataBuffer.Size
   oBuffer.WriteBuffer oDataBuffer, IncludeInData
End Sub


' ------------------------------------------------------------------------------
' Instructs the CConductor behavior to wait for the given time.
'
' Data consists of:
' [conWait - 4 bytes]
' [Timeout(ms) - 4 bytes]
'
Sub AppendWaitCommand (oBuffer, nTimeout)
   'BuildWarning "[conWait] " & nTimeout

   oBuffer.Write dtUInt32, conWait
   oBuffer.Write dtUInt32, nTimeout
End Sub


' ------------------------------------------------------------------------------
' Instructs the CConductor behavior that the context of the next
' operations are related to the given entity.
'
' Data consists of:
' [conEntity - 4 bytes]
' [Guid - Guid table offset - 4 bytes]
'
Sub AppendEntityCommand (oBuffer, oGuidTable, strEntityUID)
   'BuildWarning "[conEntity] " & strEntityUID

   oGuidTable.Insert strEntityUID

   oBuffer.Write dtUInt32, conEntity
   oBuffer.Write dtUInt32, oGuidTable.DataOffset (strEntityUID)
End Sub


' ------------------------------------------------------------------------------
' Instructs the CConductor behavior that the following commands must be
' sent to the supplied behavior.
'
' Data consists of:
' [conBehavior - 4 bytes]
' [Behavior - String table offset - 4 bytes]
'
Sub AppendBehaviorCommand (oBuffer, oStringTable, strBehaviorUID)
   'BuildWarning "[conBehavior] " & strBehaviorUID

   oStringTable.Insert strBehaviorUID

   oBuffer.Write dtUInt32, conBehavior
   oBuffer.Write dtUInt32, oStringTable.DataOffset (strBehaviorUID)
End Sub


' ------------------------------------------------------------------------------
' Appends the command index of the supplied command to the buffer.
'
' Data consists of:
' [conAttribute - 4 bytes]
' [Attribute index - 4 bytes]
'
Sub AppendAttributeCommand (oBuffer, strBehaviorUID, strCommandID)
   'BuildWarning "[conAttribute] " & strCommandID
   oBuffer.Write dtUInt32, conAttribute

   ' Find the index of the command id
   Dim oBehavior, oCommand, nCommandIndex, bCommandFound
   Set oBehavior = oRWSScript.Behavior (strBehaviorUID)
   If Not oBehavior Is Nothing Then

      bCommandFound = False
      nCommandIndex = 0
      For Each oCommand In oBehavior.ChildCommands
         If oCommand.Name = "RWS_ATTRIBUTE" Or oCommand.Name = "RWS_MESSAGE" Then
            Dim arrayParamList
            arrayParamList = oCommand.ParamList

            ' The first argument to the RWS_ATTRIBUTE/RWS_MESSAGE
            ' structure is the strCommandID (i.e. CMD_Color)
            If arrayParamList (0) = strCommandID Then
               ' We found the command we are looking for. Write
               ' the index into the buffer and exit.
               oBuffer.Write dtUInt32, nCommandIndex
               bCommandFound = True
               Exit For
            End If

            nCommandIndex = nCommandIndex + 1 ' Increment the index
         End If
      Next

      If Not bCommandFound Then
         BuildError "Could not find attribute " & strCommandID & _
                    " in behavior " & strBehaviorUID
      End If
   Else
      BuildError "Could not find behavior " & strBehaviorUID
   End If
End Sub


' ------------------------------------------------------------------------------
' Appends the key frame data to the buffer.
'
' Data consists of:
' [conKeyFrame - 4 bytes]
' [Type - String table offset - 4 bytes]
' [Size - 4 bytes][Data - Size bytes]
'
Sub AppendKeyFrameCommand (oBuffer, oStringTable, oKeyFrameData, strKeyFrameType)
   'BuildWarning "[con-KeyFrame] " & " " & strKeyFrameType

   oStringTable.Insert strKeyFrameType

   oBuffer.Write dtUInt32, conKeyFrame
   oBuffer.Write dtUInt32, oStringTable.DataOffset (strKeyFrameType)
   oBuffer.Write dtUInt32, GetAttributeDataSize (oKeyFrameData)

   ' We really should be just calling AppendAttributePacketEntityAttribute
   ' There is a difference in the data due to the command index.
   Dim oSection, varElement
   For Each oSection In oKeyFrameData
      ' Write attribute data to buffer
      For Each varElement In oSection
         oBuffer.Write oSection.Type, varElement
      Next

      ' Pad bytes or strings to 4 byte boundary
      Dim nPadCount, nPadIndex
      nPadCount = 0

      If oSection.Type = dtByte Then
         nPadCount = PaddedSize (oSection.Count) - oSection.Count
      ElseIf oSection.Type = dtChar Then
         If oBuffer.Unicode Then
            nPadCount = PaddedSize (oSection.Count*2) - (oSection.Count*2)
         Else
            nPadCount = PaddedSize (oSection.Count) - oSection.Count
         End If
      End If

      For nPadIndex = 0 To nPadCount - 1
         oBuffer.Write dtByte, PadByte
      Next
   Next
End Sub


' ------------------------------------------------------------------------------
' Appends the interpolator id into the buffer. The console will
' interpolate the previous key frame into the next key frame by
' using the given interpolator id.
'
' Data consists of:
' [conInterpolate - 4 bytes]
' [Length - 4 bytes]
' [Interpolator class - String table offset - 4 bytes]
'
Sub AppendInterpolatorCommand (oBuffer, oStringTable, oInterpolator)
   'BuildWarning "[conInterpolate] " & oInterpolator.Id & " " & oInterpolator.Length

   oStringTable.Insert oInterpolator.Id

   oBuffer.Write dtUInt32, conInterpolate
   oBuffer.Write dtUInt32, oInterpolator.Length
   oBuffer.Write dtUInt32, oStringTable.DataOffset (oInterpolator.Id)
End Sub


' ------------------------------------------------------------------------------
' Appends a dynamic interpolator and the given time
'
' Data consists of:
' [conDynamicInterpolate - 4 bytes]
' [Time (float) - 4 bytes]
' [Length - 4 bytes]
' [Interpolator class - String table offset - 4 bytes]
'
Sub AppendDynamicInterpolatorCommand (oBuffer, oStringTable, oInterpolator, fTime)
   'BuildWarning "[conDynamicInterpolate] " & oInterpolator.Id & " (" & fTime & ")"

   oStringTable.Insert oInterpolator.Id

   oBuffer.Write dtUInt32, conDynamicInterpolate
   oBuffer.Write dtReal, fTime
   oBuffer.Write dtUInt32, oStringTable.DataOffset (oInterpolator.Id)
End Sub


' ------------------------------------------------------------------------------
' Appends a transmit event to the buffer. This will cause the console to
' fire an event with the given name at the current time.
'
' Data consists of:
' [conTransmitEvent - 4 bytes]
' [Event name - String table offset - 4 bytes]
'
Sub AppendTransmitEventCommand (oBuffer, oStringTable, strEventName)
   'BuildWarning "[conTransmitEvent] " & strEventName

   oStringTable.Insert strEventName

   oBuffer.Write dtUInt32, conTransmitEvent
   oBuffer.Write dtUInt32, oStringTable.DataOffset (strEventName)
End Sub


' ------------------------------------------------------------------------------
' Appends a receive event to the buffer. The sequence will stop at the
' current time and wait until the specified event is received.
'
' Data consists of:
' [conReceiveEvent - 4 bytes]
' [Event name - String table offset - 4 bytes]
'
Sub AppendReceiveEventCommand (oBuffer, oStringTable, strEventName)
   'BuildWarning "[conReceiveEvent] " & strEventName

   oStringTable.Insert strEventName

   oBuffer.Write dtUInt32, conReceiveEvent
   oBuffer.Write dtUInt32, oStringTable.DataOffset (strEventName)
End Sub


' ------------------------------------------------------------------------------
' Appends an instruction to the console to instance an entity that is
' identical to the template specified by Instruction.TemplateUID.
'
' Data consists of:
' [conCreateTemplate - 4 bytes]
' [Size - 4 bytes]
' [Create entity attribute packet - Size bytes]
'
Sub AppendCreateTemplateCommand (oBuffer, oInstruction, oPlatformFilter)
   'BuildWarning "[conCreateTemplate] " & oInstruction.TemplateUID

   Dim oTemplate

   ' Get a template object from the Manager API database.
   On Error Resume Next ' Turn off error handling
   Set oTemplate = oRWSScript.Template (oInstruction.TemplateUID)
   If Err.Number <> 0 Or oTemplate Is Nothing Then
      BuildError "Could not find template " & oInstruction.TemplateUID
   End If
   On Error Goto 0      ' Turn error handling back on

   If oPlatformFilter.IsValidPlatform (oTemplate.Platform) Then
      ' We need to know the size of the entity creation packet. We therefore
      ' create the packet in a temporary buffer.
      Dim oCreateEntityBuffer
      Set oCreateEntityBuffer = oRWSComms.CreateMemoryBuffer ()
      oCreateEntityBuffer.BigEndian = oBuffer.BigEndian
      oCreateEntityBuffer.Unicode = oBuffer.Unicode

      ' Add create entity of type
      AppendAttributePacketCreateClassUID oCreateEntityBuffer, oTemplate.Behavior

      ' Add attributes follow
      AppendAttributePacketEntityUID oCreateEntityBuffer, oInstruction.EntityUID

      ' Add attach assets
      AppendAttachAssetsToEntity oPlatformFilter, oCreateEntityBuffer, oTemplate

      ' Add attributes
      AppendAttachAttributesToEntity oCreateEntityBuffer, oTemplate

      ' Terminate
      AppendAttributePacketTerminator oCreateEntityBuffer

      ' Append the entity creation buffer (including size) to the
      ' sequence buffer.
      oBuffer.Write dtUInt32, conCreateTemplate
      oBuffer.Write dtUInt32, oCreateEntityBuffer.Size
      oBuffer.WriteBuffer oCreateEntityBuffer, IncludeInData
   Else
      BuildWarning "Not sending template " & Template.Name & " since " _
                   & " its platform is invalid."
   End If
End Sub


' ------------------------------------------------------------------------------
' Appends an instruction to the console to destroy an entity previously
' created by AppendCreateTemplateCommand.
'
' Data consists of:
' [conDestroyTemplate - 4 bytes]
' [Guid - Guid table offset - 4 bytes]
'
Sub AppendDestroyTemplateCommand (oBuffer, oGuidTable, oInstruction)
   'BuildWarning "[conDestroyTemplate] " & oInstruction.EntityUID

   oGuidTable.Insert oInstruction.EntityUID

   oBuffer.Write dtUInt32, conDestroyTemplate
   oBuffer.Write dtUInt32, oGuidTable.DataOffset (oInstruction.EntityUID)
End Sub


' ------------------------------------------------------------------------------
' Creates an entity on the console identical to the one represented by
' the instruction
'
' Data consists of the usual entity creation packet.
'
Sub AppendDynamicTemplateCreate (oBuffer, oInstruction, oPlatformFilter)
   ' BuildWarning "[Dynamic template creation] " & oInstruction.EntityUID

   AppendCreateTemplateCommand oBuffer, oInstruction, oPlatformFilter
End Sub


' ------------------------------------------------------------------------------
' Destroys the entity on the console with the same UID as the template's
' entity uid.
'
' Data consists of the usual delete entity packet.
'
Sub AppendDynamicTemplateDestroy (oBuffer, oGuidTable, oInstruction)
   ' BuildWarning "[Dynamic template destroy] " & oInstruction.EntityUID

   AppendDestroyTemplateCommand oBuffer, oGuidTable, oInstruction
End Sub


' ------------------------------------------------------------------------------
' A dynamic key frame instruction is sent whenever a change occurred
' within workspace that requires an update of an attribute on the console
'
' Data starts with chunk header - strfunc_SequenceUpdate (31)
'
Sub AppendDynamicKeyframe (oBuffer, oStringTable, oGuidTable, oInstruction)
   ' BuildWarning "[Dynamic keyframe update] " & oInstruction.EntityUID
   Dim fTime

   ' Append the instructions into the buffer
   AppendEntityCommand oBuffer, oGuidTable, oInstruction.EntityUID
   AppendBehaviorCommand oBuffer, oStringTable, oInstruction.Behavior
   AppendAttributeCommand oBuffer, oInstruction.Behavior, oInstruction.Attribute
   AppendKeyframeCommand oBuffer, oStringTable, oInstruction.StartData, oInstruction.Type

   ' We optionally require an interpolation to a new keyframe
   If Not oInstruction.Interpolator Is Nothing Then
      If Not oInstruction.EndData Is Nothing Then
         ' Determine the time along the interpolation, with 0.0 being the start and 1.0 being the end.
         fTime = (oInstruction.CurrentTime - oInstruction.Time) / (oInstruction.Time + oInstruction.Interpolator.Length - oInstruction.Time)

         AppendDynamicInterpolatorCommand oBuffer, oStringTable, oInstruction.Interpolator, fTime
         AppendKeyframeCommand oBuffer, oStringTable, oInstruction.EndData, oInstruction.Type
      End If
   End If
End Sub

' ------------------------------------------------------------------------------
' A dynamic event instruction is sent whenever the sequencer time is dragged
' over a transmit event.
'
' Data starts with chunk header strfunc_SequenceUpdate (31) and is then followed
' by a string table and event info.
'
Sub AppendDynamicEvent (oBuffer, oStringTable, oInstruction)
   ' BuildWarning "[Dynamic event update] " & oInstruction.EventName

   ' Append the event instruction into the buffer
   AppendTransmitEventCommand oBuffer, oStringTable, oInstruction.EventName
End Sub
