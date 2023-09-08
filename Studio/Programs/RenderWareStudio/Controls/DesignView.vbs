' Criterion Software ©2001
' Enterprise Layout Management Script
'
' DesignView script

Option EXPLICIT         ' FORCE all variables to be 'Dim'ed, catches miss spelt variables.

' VK_... key code values for use in keyboard event handler.

Const VK_L_MOUSE = 1    ' Left mouse button
Const VK_M_MOUSE = 4    ' Middle mouse button
Const VK_R_MOUSE = 2    ' Right mouse button
Const VK_CTRL_KEY = 17
Const VK_ALT_KEY = 18
Const VK_SPACE = 32
Const VK_4_KEY = 52
Const VK_C_KEY = 67
Const VK_E_KEY = 69
Const VK_Q_KEY = 81
Const VK_R_KEY = 82
Const VK_V_KEY = 86
Const VK_W_KEY = 87
Const VK_X_KEY = 88
Const VK_DELETE = 46

' View 3D mode values, used to store mode system was in when the fast mode change is used.

Const V3DM_FLIGHT = 0
Const V3DM_PAN = 1
Const V3DM_FAST_PAN = 2
Const V3DM_ORBIT = 3
Const V3DM_ZOOM = 4
Const V3DM_PICK_ONLY = 5
Const V3DM_PICK_TRANS = 6
Const V3DM_PICK_ROT = 7
Const V3DM_PICK_SCALE = 8
Const V3DM_PICK_DRAG = 9
Const V3DM_PICK_MATERIALS = 10

' DesignView object 3D mode values.

Const OBJ_3D_PICK_TRANS = 0
Const OBJ_3D_PICK_ROT = 1
Const OBJ_3D_PICK_SCALE = 2
Const OBJ_3D_PICK_ONLY = 3
Const OBJ_3D_PICK_DRAG = 4

' Drag camera mode values - used by DesignViews.DragCamera

Const DRAG_CAM_NULL = -1 
Const DRAG_CAM_PAN = 0
Const DRAG_CAM_ORBIT = 2
Const DRAG_CAM_ZOOM = 1

' 'Maya' style drag camera modes - defines which operation is associated
'      with each of the three mouse buttons (one of the V3DM_... values)
Dim lButtonOp
Dim MButtonOp
Dim RButtonOp 

lButtonOp = V3DM_ORBIT    ' Left mouse button operation
mButtonOp = V3DM_PAN      ' Middle mouse button operation
rButtonOp = V3DM_ZOOM     ' Right mouse button operation

' Move variables - global.

Dim gbolCtrlDown
Dim gbolAltDown
Dim gbolLeftMouseDown
Dim gbolMidMouseDown
Dim gbolRightMouseDown
Dim gintPrev3DMode             ' One of the V3DM_... values.

Dim gbolHasFocus
Dim gbCreatingVolume ' Flag to say we're in the process of creating a volume

' Default the global values.

gbolCtrlDown       = False
gbolAltDown        = False
gbolLeftMouseDown  = False
gbolMidMouseDown   = False
gbolRightMouseDown = False
gintPrev3DMode     = V3DM_FLIGHT
gbolHasFocus       = False
gbCreatingVolume   = False

Const FastSwitchModeOff = 0   ' 
Const FastSwitchModeMMB = 1   ' Max-like (middle mouse button)
Const FastSwitchModeALT = 2   ' Maya-like (ALT key)

Dim FastSwitchMode
FastSwitchMode = FastSwitchModeOff

DesignView.ModuleMouseFocus = "Camera Controller"

Dim GlobalSelection

Dim dtChar ' From RWSScript's DataType enumeration
dtChar = 6

' =============================================================================
Set GlobalSelection = CreateObject("CSL.RWSSelection.Selection")
RENHost.ConnectObject GlobalSelection, "OVSelectionLock"

' =============================================================================
' Initialization dependant on other objects being loaded.
Sub OnLoad ()
    DesignView.RWSUtility.SelectionIdentifier = GlobalScript.g_strGlobalSelection
   GlobalSelection.SelectionIdentifier = GlobalScript.g_strGlobalSelection
End Sub

' =============================================================================

' Temporary Fix, if the selection is locked but a selection event is generated
' then disable the selection lock.
'
'
Sub OVSelectionLock_OnAddSelection (id)
   If (DesignView.Object3D.SelectionLockEnabled = True) Then
      DesignView.Object3D.SelectionLockEnabled = False
      MainMenu.SetSelectionMenuChecks
   End If
End Sub

Sub OVSelectionLock_OnRemoveSelection (id)
   If (DesignView.Object3D.SelectionLockEnabled = True) Then
      DesignView.Object3D.SelectionLockEnabled = False
      MainMenu.SetSelectionMenuChecks
   End If
End Sub

' =============================================================================

Sub DesignView_RWSUtilityOnOutputText (nType, strText)
    MessageLog.Message nType, strText
End Sub

' =============================================================================

Sub DesignView_RWSUtilityOnPreImport (ObjectID)
End Sub

' =============================================================================

Function FindStringProperty (Entity, strPropertyUID)

   Dim strPropertyText
   strPropertyText = ""

   Dim Properties, Property
   Set Properties = GlobalScript.RWSScript.Properties (Entity)
   For Each Property In Properties

      If Property.UID = strPropertyUID Then
         Dim PropertyData, DataSection
         Set PropertyData = Property.Get (Entity)

         ' Copy all characters from property name
         If Not PropertyData Is Nothing Then
            If PropertyData.Count > 0 Then
               Set DataSection = PropertyData.Item (1)

               If DataSection.Type = dtChar Then
                  Dim nChar
                  For Each nChar In DataSection
                    If Not Chr (0) = Chr (nChar) Then
                        strPropertyText = strPropertyText + Chr (nChar)
                    End If
                  Next
               End If
            End If
         End If
         Exit For
      End If
   Next

   FindStringProperty = strPropertyText
End Function

' =============================================================================

Sub AddStringProperty (Entity, strPropertyUID, strData)

   ' Add a string property for the given UID
   Dim Property, PropertyData, DataSection, bCopyFlag
   Set Property = GlobalScript.RWSScript.Property (strPropertyUID)
   Set PropertyData = CreateObject ("CSL.RWSScript.RWSData")

   Set DataSection = PropertyData.Add (dtChar)
   DataSection.Add strData

   Property.Set Entity, PropertyData

   bCopyFlag = Property.GetSystemFlags (Entity) And Properties.sysflagCopyProperty

   If Not bCopyFlag Then
      Property.SetSystemFlags Entity, Property.GetSystemFlags (Entity) + Properties.sysflagCopyProperty
   End If

End Sub

' =============================================================================

Sub DesignView_RWSUtilityOnPostImport (ObjectID)

    If GlobalScript.RWSScript.IDType (ObjectID) = Database.otEntity Then

        ' If we're not creating volume in DesignView_VolumeEditOnNewBoxVolume
        If gbCreatingVolume = False Then
            ' It's an entity
            Dim Entity
            Set Entity = GlobalScript.RWSScript.Entity (ObjectID)

            ' Is this a volume entity?
            Dim strShape
            strShape = FindStringProperty (Entity, Properties.conHullShape)

            If strShape = "smBox" Then ' Box shape mode

                'Determine the texture used
                Dim strTexture
                strTexture = FindStringProperty (Entity, Properties.conHullTexture)

                ' Initialize the volume editor with the pick dispatch interface
                DesignView.VolumeEdit.SetPick DesignView.Pick3D

                ' We have a volume entity, so add the shape to it
                DesignView.VolumeEdit.AddVolume 0, DesignView.RWSUtility.GetEntityGraphic (ObjectID), strTexture
            End If
        End If
    End If

End Sub

' =============================================================================

Sub DesignView_VolumeEditOnNewBoxVolume (InstanceID)

    gbCreatingVolume = True ' Stop OnPostImport creating another volume

    ' We have a new volume object in the scene - create an entity
    Dim Undo
    Set Undo = GlobalScript.RWSScript.CreateUndo ("Create new volume entity")

    Dim Entity
    Set Entity = GlobalScript.RWSScript.CreateEntity ()
    DesignView.RWSUtility.SetEntityGraphic Entity.ID, InstanceID    
    Entity.Name = "Volume " & Entity.ID
    Entity.Behavior = "CEntity"

    ' Add a property for the shape
    AddStringProperty Entity, Properties.conHullShape, "smBox"

    ' Add a property for the texture
    AddStringProperty Entity, Properties.conHullTexture, VolumeToolbar.g_strCurrentTexture

    ' Add the entity to the working or active folder
    Dim Folder
    Set Folder = GlobalScript.RWSScript.Game.WorkingFolder
    If Folder Is Nothing Then
        Set Folder = GlobalScript.RWSScript.Game.ActiveFolder
    End If

    Dim bOK
    bOK = True
    If Not Folder Is Nothing Then
        On Error Resume Next
        Folder.AddChildEntity Entity
        If Not Err.Number = 0 Then
            bOK = False
        End If
        On Error Goto 0
    End If

    ' Set up the graphic for this object
    If Not bOK Then
        ' Inform user that volume entity couldn't be added to the working/current folder
        Dim strOutputText
        strOutputText = "[Volume Editor] Unable to add the volume entity to the working (or active) folder.  Please verify the checkout status of the folder."
        CSLRenderwareStudioWorkspace.ShowObject MessageLog
        MessageLog.Error strOutputText

        ' Destroy the entity
        Undo.Rollback
        GlobalScript.RWSScript.Delete Entity
        DesignView.VolumeEdit.DestroyVolume InstanceID
    End If

    gbCreatingVolume = False
End Sub

' =============================================================================

Sub DesignView_Object3DObjectsTransformed ( count )
    ObjectInformation.UpdateValues
End Sub

' =============================================================================

Sub DesignView_Object3DElementHighlighted ( group, axis )
    ObjectInformation.SetCurrentMode group
End Sub

' =============================================================================

Sub DesignView_Object3DTransformFinished (  )
    ObjectInformation.UpdateValues
End Sub

' =============================================================================

Sub DesignView_Object3DObjectSelected ( ID )

    ObjectInformation.SetCurrentMode DesignView.Object3D.AxisMode

    ' Check if object is 'locked', and if so, disable
    ' the ObjectInformation controls.

    if DesignView.Object3D.IsLocked then
        ObjectInformation.EnableControls false
    else
        ObjectInformation.EnableControls true
    end if

End Sub

' =============================================================================

Sub DesignView_Object3DObjectDeselected ( ID )
    ObjectInformation.SetCurrentMode 0   ' defaults to position when nothing is selected
    ObjectInformation.EnableControls true
End Sub

' =============================================================================
Sub DesignView_Object3DAxisAlignChanged ( new_alignmode )

    ObjectInformation.UpdateAxisAlign

End Sub

' =============================================================================
Sub DesignView_OrthoOnContextMenu (x, y, ChannelID, ViewID)
    ' Only show context menu when not flying
    If Not DesignView.ModuleMouseFocus is DesignView.Camera Then

        ' Set up some globals prior to any paste operations
        DesignViewsContextMenu.SetupPaste x, y, ChannelID, ViewID

        ShowContextMenu
    End If
End Sub

' =============================================================================
' Handle keyboard & mouse events via VK_... key codes. Code sets various
' boolean key state values and modes (via sub-functions).
'
' key   = VK_... key code of the key being pressed or released.
' value = If zero key is being released, if non-zero it is being pressed.
'
Sub DesignView_KeyboardKeyState ( key, value )

    Dim bolKeyDown

    If gbolHasFocus = False Then
        Exit Sub
    End If

    ' Handle 'Max 5' style keys ONLY for the moment...

    Select Case key

        Case VK_E_KEY

            ' Pick only mode.

            Set3DMode V3DM_PICK_ROT, False

        Case VK_Q_KEY

            ' Pick and translate mode.

            Set3DMode V3DM_PICK_ONLY, False

        Case VK_R_KEY

            ' Pick and rotate mode.

            Set3DMode V3DM_PICK_SCALE, False

        Case VK_W_KEY

            ' Pick and scale mode.

            Set3DMode V3DM_PICK_TRANS, False

        Case VK_4_KEY

            ' Pick and drag mode.

            Set3DMode V3DM_PICK_DRAG, False

        Case VK_X_KEY

            ' Cut if ctrl-x pressed
            If gbolCtrlDown And Not value = 0 Then
                If DesignView.RWSUtility.CanCut Then
                    DesignViewsContextMenu.OnCut
                End If
            End If

        Case VK_C_KEY

            ' Copy if ctrl-c pressed
            If gbolCtrlDown And Not value = 0 Then
                If DesignView.RWSUtility.CanCopy Then
                    DesignViewsContextMenu.OnCopy
                End If
            End If
        
        Case VK_V_KEY

            ' Paste, if ctrl-v pressed
            If gbolCtrlDown And Not value = 0 Then
                ' Can paste if there's something useful on the clipboard...
                If DesignView.RWSUtility.CanPaste Then
                    ' Paste if cursor is in a view
                    Dim x, y, ChannelID, ViewID
                    If DesignView.Ortho.GetCursor (x, y, ChannelID, ViewID) Then
                        DesignViewsContextMenu.SetupPaste x, y, ChannelID, ViewID
                        DesignViewsContextMenu.OnPaste
                    End If
                End If
            End If

        Case VK_SPACE

            ' Toggle selection lock - if no mouse buttons down (ie dragging)

            If (gbolLeftMouseDown = False And gbolMidMouseDown = False And _
                    gbolRightMouseDown = False And value <> 0) Then
                MainMenu.OnMenuLockSelection
            End If

        Case VK_CTRL_KEY

            ' Is key down or up? Do NOT directly change global.

            If value = 0 Then
                bolKeyDown = False
            Else
                bolKeyDown = True
            End If

            ' Update the camera movement mode.

            if (FastSwitchMode = FastSwitchModeMMB) Then
                UpdateMax5CameraMoveMode bolKeyDown, gbolAltDown, gbolMidMouseDown
            End If
            
        Case VK_ALT_KEY

            ' Is key down or up? Do NOT directly change global.
            If value = 0 Then
                bolKeyDown = False
            Else
                bolKeyDown = True
            End If
            
            ' Try to be a little clever, if the middle mouse button isn't already down
            '  the switch into the FastSwitchModeALT           
            if bolKeyDown then
               if Not gbolMidMouseDown then
                   FastSwitchMode = FastSwitchModeALT
               End If 
            End If               
            
            ' Update the camera movement mode.

            ' Maya style switching ...
            if (FastSwitchMode = FastSwitchModeALT) then
            
                If value = 0 Then
                    Set3DMode gintPrev3DMode, FALSE                
                Else
                    StoreCurrent3DMode
                    DesignView.ModuleMouseFocus = DesignView.DragCamera
                    DesignView.DragCamera.NavigationMode = DRAG_CAM_NULL   
                End If
                gbolAltDown = bolKeyDown
            
            elseif (FastSwitchMode = FastSwitchModeMMB) then

                ' Max5 style switching
                UpdateMax5CameraMoveMode gbolCtrlDown, bolKeyDown, gbolMidMouseDown
            
            End If 

        Case VK_L_MOUSE

            ' Can directly set variable here.

            If value = 0 Then
                gbolLeftMouseDown = False
            Else
                gbolLeftMouseDown = True
            End If
            
            ' Maya style switching
            if (FastSwitchMode = FastSwitchModeALT) then
            
                if (gbolAltDown) Then
                    if gbolLeftMouseDown Then
                        Set3DMode lButtonOp, TRUE
                    else
                        DesignView.DragCamera.NavigationMode = DRAG_CAM_NULL
                    End If 
                End If

            End If
            
        Case VK_M_MOUSE

            ' Is key down or up? Do NOT directly change global.

            If value = 0 Then
                bolKeyDown = False
            Else
                bolKeyDown = True
            End If
            
            ' Try to be a little clever, if the alt mouse button isn't already down
            '  then switch into the FastSwitchModeMMB           
            if bolKeyDown then
               if Not gbolAltDown then
                   FastSwitchMode = FastSwitchModeMMB
               End If 
            End If                 

            ' Update the camera movement mode.
            if (FastSwitchMode = FastSwitchModeALT) then
            
                ' Maya style switching
                if (gbolAltDown) Then
                    if bolKeyDown Then
                        Set3DMode mButtonOp, TRUE
                    else
                        DesignView.DragCamera.NavigationMode = DRAG_CAM_NULL
                    End If 
                End If
                gbolMidMouseDown = bolKeyDown

            Elseif (FastSwitchMode = FastSwitchModeMMB) then
            
                ' Max5 style switching
                UpdateMax5CameraMoveMode gbolCtrlDown, gbolAltDown, bolKeyDown

            End If
 
        Case VK_R_MOUSE

            ' Can directly set variable here.

            If value = 0 Then
                gbolRightMouseDown = False
            Else
                gbolRightMouseDown = True
            End If

            ' Update the camera movement mode.

            if (FastSwitchMode = FastSwitchModeALT) then
            
                ' Maya style switching            
                if (gbolAltDown) Then
                    if gbolRightMouseDown Then
                        Set3DMode rButtonOp, TRUE
                    else
                        DesignView.DragCamera.NavigationMode = DRAG_CAM_NULL
                    End If 
                End If   
                         
            End If
            
        Case Else

            ' Unrecognised key / mouse button, just ignore it.

    End Select
End Sub

' =============================================================================
' Updates the 'Max 5' style control system using the passed in key state
' values. Modifies the gintCurMode value. Uses (and modifies) the gbolCtrlDown,
' gbolAltDown, gbolMidMouseDown, etc. Uses the gbol... values to test the last
' state of the keys.
'
' bolIsCtrlDown     = boolean indicating if the Ctrl (control) key is currently pressed. True = yes.
' bolIsAltDown      = boolean indicating if the Alt (alternative) key is currently pressed. True = yes.
' bolIsMidMouseDown = boolean indicating if the middle mouse button is currently pressed. True = yes.
'
Sub UpdateMax5CameraMoveMode(bolIsCtrlDown, bolIsAltDown, bolIsMidMouseDown)

    Dim intoCamMode

    ' View adjust mode (depends on states of alt & ctrl keys). With no modifier keys
    ' gives camera translation, with Alt give camera orbit, with Ctrl (only) gives
    ' fast camera translation and with Alt and Ctrl gives camera zoom. Determin which
    ' mode the system is in.

    If bolIsMidMouseDown Then

        ' Entering the mode for the first time ?

        intoCamMode = False

        If Not gbolMidMouseDown Then

            ' Yep, so change the mode to camera movement mode. Store current mode FIRST.

            StoreCurrent3DMode
            intoCamMode = True
        End If

        ' Changing the camera movement mode type or set for the first time
        ' due to entering the camera mode.

        If (gbolCtrlDown <> bolIsCtrlDown) Or (gbolAltDown <> bolIsAltDown) Or intoCamMode Then

            ' Has changed from last time, so need to reset mode.

            If (bolIsCtrlDown) Then
                If (bolIsAltDown) Then

                    ' Alt and Ctrl pressed, in camera zoom mode.

                    Set3DMode V3DM_ZOOM, TRUE
                Else

                    ' Ctrl only pressed so in high-speed translation
                    ' mode.

                    Set3DMode V3DM_FAST_PAN, TRUE
                End If
            Else
                If (bolIsAltDown) Then

                    ' Alt only pressed, in camera orbit mode.

                    Set3DMode V3DM_ORBIT, TRUE
                Else

                    ' Mouse button on it's own (no modifiers) so
                    ' in standard translation mode.

                    Set3DMode V3DM_PAN, TRUE
                End If
            End If
        End If
    Else

        ' Exiting the mode ?

        If gbolMidMouseDown Then

            ' Yep, so change the mode back to previous pick, translate, etc mode.

            Set3DMode gintPrev3DMode, FALSE
        End If
    End If

    ' Store new values of mouse button, ctrl and alt, etc.

    gbolCtrlDown = bolIsCtrlDown
    gbolAltDown = bolIsAltDown
    gbolMidMouseDown = bolIsMidMouseDown
End Sub

'===============================================================================
'
' Stores off the current 3D mode, this includes flight, orbit, pick & translate,
' etc modes, into the gintPrev3DMode variable.
'
Sub StoreCurrent3DMode

    ' Determin the currently active mode. Which module has mouse focus ?

    If DesignView.ModuleMouseFocus is DesignView.Camera Then
        gintPrev3DMode = V3DM_FLIGHT

    ElseIf DesignView.ModuleMouseFocus is DesignView.Pick3D Then

        ' Which particular type of pick mode ?
        
        if DesignView.RWSUtility.HighlightMaterials Then
        
           ' Material picking mode
           gintPrev3DMode = V3DM_PICK_MATERIALS
          
        else

            Select Case DesignView.Object3D.AxisMode

                Case OBJ_3D_PICK_TRANS
                    gintPrev3DMode = V3DM_PICK_TRANS

                Case OBJ_3D_PICK_ROT
                    gintPrev3DMode = V3DM_PICK_ROT

                Case OBJ_3D_PICK_SCALE
                    gintPrev3DMode = V3DM_PICK_SCALE

                case OBJ_3D_PICK_ONLY
                    gintPrev3DMode = V3DM_PICK_ONLY

                case OBJ_3D_PICK_DRAG
                    gintPrev3DMode = V3DM_PICK_DRAG

                Case Else

                    ' Unknown, warn user and choose translate.
                    gintPrev3DMode = V3DM_PICK_TRANS
                    MessageLog.Warning "System in unknown 3D pick mode - using translate."
            
            End Select

        End if

    ElseIf DesignView.ModuleMouseFocus is DesignView.DragCamera Then

        ' Which particular 'max' style camera mode ?

        Select Case DesignView.DragCamera.NavigationMode

            Case DRAG_CAM_PAN
                gintPrev3DMode = V3DM_PAN

            Case DRAG_CAM_ORBIT
                gintPrev3DMode = V3DM_ORBIT

            Case DRAG_CAM_ZOOM
                gintPrev3DMode = V3DM_ZOOM

            Case Else

                ' Unknown, warn user and choose pan

                gintPrev3DMode = V3DM_PAN
                MessageLog.Warning "System in unknown 3D camera mode - using pan."
        End Select
    Else

            ' Unknown mode - warn the user and set to flight mode instead.

            MessageLog.Warning "System in unknown 3D mode - using flight mode instead."
            gintPrev3DMode = V3DM_FLIGHT
    End If
End Sub

'===============================================================================
'
' Sets the 3D mode to the passed in value (one of the V3DM_...) values. The
' bolEntering flag is used to indicate if the system is ENTERING (from middle
' mouse button down) the Pan, Fast Pan, Orbit or Scale mode. In all other
' cases it's value is ignored.
'
Sub Set3DMode(int3DMode, bolEntering)

    ' Handle 'Fast Pan' mode.

    If int3DMode = V3DM_FAST_PAN Then

        ' NOTE: fast pan detection (CTRL only key pressed) is handled in the
        ' MaxNavigation module (DragCamera) and not directly in the script.

        int3DMode = V3DM_PAN
    End If

    ' Change to appropriate mode.

    Select Case int3DMode
        Case V3DM_FLIGHT
            MainMenu.OnMenuFlyCamera
            DesignView.DragCamera.ForceMouseTrack(FALSE)

        Case V3DM_PAN
            MainMenu.OnMenuPanCameraNoPreview
            If bolEntering Then
                DesignView.DragCamera.ForceMouseTrack(TRUE)
            Else
                DesignView.DragCamera.ForceMouseTrack(FALSE)
            End If

        Case V3DM_ORBIT
            MainMenu.OnMenuOrbitCameraNoPreview
            If bolEntering Then
                DesignView.DragCamera.ForceMouseTrack(TRUE)
            Else
                DesignView.DragCamera.ForceMouseTrack(FALSE)
            End If

        Case V3DM_ZOOM
            MainMenu.OnMenuZoomCameraNoPreview
            If bolEntering Then
                DesignView.DragCamera.ForceMouseTrack(TRUE)
            Else
                DesignView.DragCamera.ForceMouseTrack(FALSE)
            End If

        Case V3DM_PICK_ONLY
            MainMenu.OnMenuPickMode
            DesignView.DragCamera.ForceMouseTrack(FALSE)

        Case V3DM_PICK_TRANS
            MainMenu.OnMenuPickTranslateMode
            DesignView.DragCamera.ForceMouseTrack(FALSE)

        Case V3DM_PICK_ROT
            MainMenu.OnMenuPickRotateMode
            DesignView.DragCamera.ForceMouseTrack(FALSE)

        Case V3DM_PICK_SCALE
            MainMenu.OnMenuPickScaleMode
            DesignView.DragCamera.ForceMouseTrack(FALSE)

        Case V3DM_PICK_DRAG
            MainMenu.OnMenuPickAndDrag
            DesignView.DragCamera.ForceMouseTrack(FALSE)
            
        Case V3DM_PICK_MATERIALS
        
            MainMenu.OnMenuPickMaterials
            DesignView.DragCamera.ForceMouseTrack(FALSE)
            
        Case Else

            ' Unknown flight mode - warn user & switch to flight mode. NOTE: fast pan mode is
            ' translated into pan mode, due to handling code in MaxNavigation (DragCamera) module.

            MainMenu.OnMenuFlyCamera
            DesignView.DragCamera.ForceMouseTrack(FALSE)
            MessageLog.Warning "Failed trying to restore unknown 3D mode - using flight mode instead."
    End Select
End Sub

'===============================================================================
'
' Activated when the axis mode changes externally - ie inside module code.
'
Sub DesignView_Object3DAxisModeChanged ( new_mode )

    MainMenu.DesignViewtoMenu_Object3DAxisModeChanged new_mode

End Sub

' ------------------------------------------------------------------------------

Sub DesignView_OnKeyDown ( KeyCode )   
    if KeyCode = VK_DELETE Then
        DesignViewsContextMenu.OnDeleteSelection
    End If     
End Sub

' -----------------------------------------------------------------------------

Sub DesignView_OnGotFocus()
    gbolHasFocus = True
End Sub

' -----------------------------------------------------------------------------

Sub DesignView_OnLostFocus()
    gbolHasFocus = False
End Sub

' -----------------------------------------------------------------------------

Sub ShowContextMenu()

    ' Enable selection specific menu items
    If GlobalSelection.Count > 0 Then
        DesignViewsContextMenu.MenuItems("DesignView Context").MenuItems.Item("Delete Selection").Enabled = True
        DesignViewsContextMenu.MenuItems("DesignView Context").MenuItems.Item("Clear Selection").Enabled = True
        DesignViewsContextMenu.MenuItems("DesignView Context").MenuItems.Item("Hide Selection").Enabled = True
        DesignViewsContextMenu.MenuItems("DesignView Context").MenuItems.Item("Freeze Selection").Enabled = True
        DesignViewsContextMenu.MenuItems("DesignView Context").MenuItems.Item("Show Asset Stream Contents").Enabled = True
    Else
        DesignViewsContextMenu.MenuItems("DesignView Context").MenuItems.Item("Delete Selection").Enabled = False
        DesignViewsContextMenu.MenuItems("DesignView Context").MenuItems.Item("Clear Selection").Enabled = False
        DesignViewsContextMenu.MenuItems("DesignView Context").MenuItems.Item("Hide Selection").Enabled = False
        DesignViewsContextMenu.MenuItems("DesignView Context").MenuItems.Item("Freeze Selection").Enabled = False
        DesignViewsContextMenu.MenuItems("DesignView Context").MenuItems.Item("Show Asset Stream Contents").Enabled = False
    End If

    if GlobalSelection.Count = 1 Then
        DesignViewsContextMenu.MenuItems("DesignView Context").MenuItems.Item("Locate In Game Explorer").Enabled = True
        DesignViewsContextMenu.MenuItems("DesignView Context").MenuItems.Item("Center In View").Enabled = True
        DesignViewsContextMenu.MenuItems("DesignView Context").MenuItems.Item("Aim").Enabled = True
    Else
        DesignViewsContextMenu.MenuItems("DesignView Context").MenuItems.Item("Locate In Game Explorer").Enabled = False
        DesignViewsContextMenu.MenuItems("DesignView Context").MenuItems.Item("Center In View").Enabled = False
        DesignViewsContextMenu.MenuItems("DesignView Context").MenuItems.Item("Aim").Enabled = False
    End If

    DesignViewsContextMenu.MenuItems("DesignView Context").MenuItems.Item("Show All").Enabled = True
    DesignViewsContextMenu.MenuItems("DesignView Context").MenuItems.Item("Unfreeze All").Enabled = True

    ' Enable cut, copy and paste menus
    DesignViewsContextMenu.MenuItems("DesignView Context").MenuItems.Item("Cut").Enabled   = DesignView.RWSUtility.CanCut
    DesignViewsContextMenu.MenuItems("DesignView Context").MenuItems.Item("Copy").Enabled  = DesignView.RWSUtility.CanCopy
    DesignViewsContextMenu.MenuItems("DesignView Context").MenuItems.Item("Paste").Enabled = DesignView.RWSUtility.CanPaste

    ' Finally, show the menu
    DesignViewsContextMenu.ShowAsContextMenu 1

End Sub

' -----------------------------------------------------------------------------

Sub DesignView_RWSUtilityOnStreamEditSelect ( rws_id, obj_identifier, mat_identifier )

    ' Tell the stream edit to select the correct object(s)
    if DesignView.RWSUtility.HighlightMaterials = True then
        if (rws_id <> 0) then
            StreamViewer.SelectItem rws_id, obj_identifier, mat_identifier
        end if
    end if
    
End Sub


' -----------------------------------------------------------------------------

Sub Broadcast_LoadGameSettings (XMLRootNode)

    Dim DisableFilter
    DisableFilter = True

    ' Look for the section in the settings file
    Dim PlatformFilterElement
    Set PlatformFilterElement = XMLRootNode.SelectSingleNode ("./DesignViews/PlatformFilter")
        
    If Not PlatformFilterElement Is Nothing Then

        ' Found the right section, so extract the data - actual filter string,
        '  and whether it's enabled/disabled
        Dim FilterEnabledElement
        Set FilterEnabledElement =  PlatformFilterElement.SelectSingleNode ("./State")
        
        If  Not FilterEnabledElement Is Nothing Then
            If FilterEnabledElement.text = "On" Then
                DesignView.RWSUtility.EnablePlatformFilter = True
            Else
                DesignView.RWSUtility.EnablePlatformfilter = False
            End If
        Else
            DesignView.RWSUtility.EnablePlatformfilter = False 
        End If
                
        Dim FilterStringElement
        Set FilterStringElement = PlatformFilterElement.SelectSingleNode ("./FilterString")  
        
        If Not FilterStringElement Is Nothing Then
            DesignView.RWSUtility.PlatformFilterString = FilterStringElement.text
        Else
            Designview.RWSUtility.PlatformFilterString = ""
        End If
        
        ' We've found some valid settings, so don't disable the filtering
        DisableFilter = False
        
    End If
   
            
    If DisableFilter Then
  
        ' Nothing valid found in the XML file - turn off the platform filter
        DesignView.RWSUtility.EnablePlatformFilter = False
        DesignView.RWSUtility.PlatformFilterString = ""
      
    End If
      
End Sub


' -----------------------------------------------------------------------------

Sub Broadcast_SaveGameSettings (XMLDoc, XMLRootNode)

    Dim RootXMLElement
    Set RootXMLElement = XMLRootNode.SelectSingleNode ("./DesignViews")
    If RootXMLElement Is Nothing Then
        Set RootXMLElement = XMLDoc.CreateElement ("DesignViews")
        XMLRootNode.AppendChild RootXMLElement
    End If

    If Not RootXMLElement Is Nothing Then

        ' Now empty child nodes
        Dim ChildElement
        For Each ChildElement In RootXMLElement.ChildNodes
            RootXMLElement.RemoveChild ChildElement
        Next

        Dim PlatformFilterElement
        Set PlatformFilterElement = XMLDoc.CreateElement ("PlatformFilter")
        

        If Not PlatformFilterElement Is Nothing Then
        
            RootXMLElement.AppendChild PlatformFilterElement

            Dim FilterStringElement
            Set FilterStringElement = XMLDoc.CreateElement ("FilterString")

            If Not FilterStringElement Is Nothing Then
                FilterStringElement.text = DesignView.RWSUtility.PlatformFilterString
                PlatformFilterElement.AppendChild FilterStringElement
            End If
 
            Dim FilterEnableElement
            Set FilterEnableElement = XMLDoc.CreateElement ("State")

            If Not FilterEnableElement Is Nothing Then

                If DesignView.RWSUtility.EnablePlatformFilter Then
                    FilterEnableElement.text = "On"
                Else
                    FilterEnableElement.text = "Off"
                End If
            
                PlatformFilterElement.AppendChild FilterEnableElement
            End If

        End If

   End If

End sub

' ------------------------------------------------------------------------------
' Catch broadcast events
' ------------------------------------------------------------------------------

' ------------------------------------------------------------------------------
' Handle broadcast event to say active folder has changed
Sub Broadcast_ActiveFolderChanged (NewRootFolder)
    DesignView.RWSUtility.SetActiveFolder NewRootFolder
End Sub
