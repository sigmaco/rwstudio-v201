'
' Enterprise Layout Management Script
'
' Layout object 'ObjectInformation'
'

' Constants defining current edit mode(s)
const const_TranslateMode = 0
const const_RotationMode = 1
const const_ScaleMode = 2
const const_PickOnlyMode = 3

' Constants defining key values for key down events
const const_vkKeyEnter = 13
const const_vkKeyTab = 9

Dim current_mode
current_mode = const_TranslateMode

' =============================================================================

Sub SetCurrentMode (mode)

    current_mode = mode

    Select Case current_mode

        case const_TranslateMode
           
            ModeLabel.Caption = "Position"
            ChangeLabel.Caption = "Offset"

        case const_PickOnlyMode
            
            ModeLabel.Caption = "Position"
            ChangeLabel.Caption = "Offset"

        case const_RotationMode

            ModeLabel.Caption = "Orientation"
            ChangeLabel.Caption = "Offset"

        case const_ScaleMode

            ModeLabel.Caption = "Scale"
            ChangeLabel.Caption = "Change (%)"

    End Select

    number_of_objects = DesignView.Object3D.NumSelectedObjects

    if (number_of_objects = 1) then
        ObjectCount.Caption = CStr(number_of_objects) + " object"
    Else
        ObjectCount.Caption = CStr(number_of_objects) + " objects"
    End if

    UpdateValues

End Sub

' =============================================================================

Sub UpdateValues

    Dim currentValueRel
    Dim currentValueAbs

    Select Case current_mode

        case const_PickOnlyMode

            Set RelCurrentValue = DesignView.Object3D.TranslationRel

            EditValueXRel.Text = CStr(RelCurrentValue.x)
            EditValueYRel.Text = CStr(RelCurrentValue.y)
            EditValueZRel.Text = CStr(RelCurrentValue.z)

            Set AbsCurrentValue = DesignView.Object3D.TranslationAbs

            EditValueXAbs.Text = CStr(AbsCurrentValue.x)
            EditValueYAbs.Text = CStr(AbsCurrentValue.y)
            EditValueZAbs.Text = CStr(AbsCurrentValue.z)


        case const_TranslateMode

            Set RelCurrentValue = DesignView.Object3D.TranslationRel

            EditValueXRel.Text = CStr(RelCurrentValue.x)
            EditValueYRel.Text = CStr(RelCurrentValue.y)
            EditValueZRel.Text = CStr(RelCurrentValue.z)

            Set AbsCurrentValue = DesignView.Object3D.TranslationAbs

            EditValueXAbs.Text = CStr(AbsCurrentValue.x)
            EditValueYAbs.Text = CStr(AbsCurrentValue.y)
            EditValueZAbs.Text = CStr(AbsCurrentValue.z)

        case const_RotationMode

            Set currentValueRel = DesignView.Object3D.OrientationRel

            EditValueXRel.Text = CStr(currentValueRel.pitch)
            EditValueYRel.Text = CStr(currentValueRel.yaw)
            EditValueZRel.Text = CStr(currentValueRel.roll)

            Set currentValueAbs = DesignView.Object3D.OrientationAbs

            EditValueXAbs.Text = CStr(currentValueAbs.pitch)
            EditValueYAbs.Text = CStr(currentValueAbs.yaw)
            EditValueZAbs.Text = CStr(currentValueAbs.roll)

        case const_ScaleMode

            Set currentValueRel = DesignView.Object3D.ScaleRel

            EditValueXRel.Text = CStr(currentValueRel.x)
            EditValueYRel.Text = CStr(currentValueRel.y)
            EditValueZRel.Text = CStr(currentValueRel.z)

            Set currentValueAbs = DesignView.Object3D.ScaleAbs

            EditValueXAbs.Text = CStr(currentValueAbs.x)
            EditValueYAbs.Text = CStr(currentValueAbs.y)
            EditValueZAbs.Text = CStr(currentValueAbs.z)

    End Select
End Sub

' =============================================================================

Sub SetAbsoluteValues

    Dim currentValue

    Select Case current_mode

        case const_PickOnlyMode

           Set  currentValue = DesignView.Object3D.TranslationAbs

            If IsNumeric(EditValueXAbs.Text) Then
                currentValue.x = CSng(EditValueXAbs.Text)
            End If

            If IsNumeric(EditValueYAbs.Text) Then
                currentValue.y = CSng(EditValueYAbs.Text)
            End If

            If IsNumeric(EditValueZAbs.Text) Then
                currentValue.z = CSng(EditValueZAbs.Text)
            End If

            DesignView.Object3D.TranslationAbs = currentValue

        case const_TranslateMode

           Set  currentValue = DesignView.Object3D.TranslationAbs

            If IsNumeric(EditValueXAbs.Text) Then
                currentValue.x = CSng(EditValueXAbs.Text)
            End If

            If IsNumeric(EditValueYAbs.Text) Then
                currentValue.y = CSng(EditValueYAbs.Text)
            End If

            If IsNumeric(EditValueZAbs.Text) Then
                currentValue.z = CSng(EditValueZAbs.Text)
            End If

            DesignView.Object3D.TranslationAbs = currentValue

        case const_RotationMode

            Set currentValue = DesignView.Object3D.OrientationAbs

            If IsNumeric(EditValueXAbs.Text) Then
                currentValue.pitch = CSng(EditValueXAbs.Text)
            End If

            If IsNumeric(EditValueYAbs.Text) Then
                currentValue.yaw = CSng(EditValueYAbs.Text)
            End If

            If IsNumeric(EditValueZAbs.Text) Then
                currentValue.roll = CSng(EditValueZAbs.Text)
            End If

            DesignView.Object3D.OrientationAbs = currentValue

        case const_ScaleMode

            Set currentValue = DesignView.Object3D.ScaleAbs

            If IsNumeric(EditValueXAbs.Text) Then
                currentValue.x = CSng(EditValueXAbs.Text)
            End If

            If IsNumeric(EditValueYAbs.Text) Then
                currentValue.y = CSng(EditValueYAbs.Text)
            End If

            If IsNumeric(EditValueZAbs.Text) Then
                currentValue.z = CSng(EditValueZAbs.Text)
            End If

            DesignView.Object3D.ScaleAbs = currentValue

    End Select

End Sub

' =============================================================================

Sub SetRelativeValues

    Dim currentValue

    Select Case current_mode

        case const_PickOnlyMode

           Set  currentValue = DesignView.Object3D.TranslationRel

            If IsNumeric(EditValueXRel.Text) Then
                currentValue.x = CSng(EditValueXRel.Text)
            End If

            If IsNumeric(EditValueYRel.Text) Then
                currentValue.y = CSng(EditValueYRel.Text)
            End If

            If IsNumeric(EditValueZRel.Text) Then
                currentValue.z = CSng(EditValueZRel.Text)
            End If

            DesignView.Object3D.TranslationRel = currentValue

        case const_TranslateMode

           Set  currentValue = DesignView.Object3D.TranslationRel

            If IsNumeric(EditValueXRel.Text) Then
                currentValue.x = CSng(EditValueXRel.Text)
            End If

            If IsNumeric(EditValueYRel.Text) Then
                currentValue.y = CSng(EditValueYRel.Text)
            End If

            If IsNumeric(EditValueZRel.Text) Then
                currentValue.z = CSng(EditValueZRel.Text)
            End If

            DesignView.Object3D.TranslationRel = currentValue

        case const_RotationMode

            Set currentValue = DesignView.Object3D.OrientationRel

            If IsNumeric(EditValueXRel.Text) Then
                currentValue.pitch = CSng(EditValueXRel.Text)
            End If

            If IsNumeric(EditValueYRel.Text) Then
                currentValue.yaw = CSng(EditValueYRel.Text)
            End If

            If IsNumeric(EditValueZRel.Text) Then
                currentValue.roll = CSng(EditValueZRel.Text)
            End If

            DesignView.Object3D.OrientationRel = currentValue

        case const_ScaleMode

            Set currentValue = DesignView.Object3D.ScaleRel

            If IsNumeric(EditValueXRel.Text) Then
                currentValue.x = CSng(EditValueXRel.Text)
            End If

            If IsNumeric(EditValueYRel.Text) Then
                currentValue.y = CSng(EditValueYRel.Text)
            End If

            If IsNumeric(EditValueZRel.Text) Then
                currentValue.z = CSng(EditValueZRel.Text)
            End If

            DesignView.Object3D.ScaleRel = currentValue

    End Select

End Sub

' =============================================================================

Sub EditValueXAbs_KeyDown ( KeyCode, Shift )

    Select Case KeyCode

        case const_vkKeyEnter
            SetAbsoluteValues
    End Select

End Sub

' =============================================================================

Sub EditValueYAbs_KeyDown ( KeyCode, Shift )

    Select Case KeyCode

        case const_vkKeyEnter

            SetAbsoluteValues

    End Select

End Sub

' =============================================================================

Sub EditValueZAbs_KeyDown ( KeyCode, Shift )

    Select Case KeyCode

        case const_vkKeyEnter

            SetAbsoluteValues

    End Select

End Sub

' =============================================================================

Sub EditValueXRel_KeyDown ( KeyCode, Shift )

    Select Case KeyCode

        case const_vkKeyEnter

            SetRelativeValues

    End Select

End Sub

' =============================================================================

Sub EditValueYRel_KeyDown ( KeyCode, Shift )

    Select Case KeyCode

        case const_vkKeyEnter

            SetRelativeValues

    End Select

End Sub

' =============================================================================

Sub EditValueZRel_KeyDown ( KeyCode, Shift )

    Select Case KeyCode

        case const_vkKeyEnter

            SetRelativeValues

    End Select

End Sub

' =============================================================================

Sub UpdateAxisAlign

    if DesignView.Object3D.LocalDragging Then
        AxisSpace.Text = "Local Axis Align"
    else
        AxisSpace.Text = "World Axis Align"
    end if

End Sub

' =============================================================================
' Initialization dependant on other objects being loaded.
Sub OnLoad

    AxisSpace.AddItem "World Axis Align"
    AxisSpace.AddItem "Local Axis Align"

    UpdateAxisAlign

End Sub

' =============================================================================

Sub AxisSpace_Change (  )

    if AxisSpace.Text = "Local Axis Align" Then
        DesignView.Object3D.LocalDragging = true
    else
        DesignView.Object3D.LocalDragging = False
    end if

End Sub


' =============================================================================

Sub EnableControls ( enable )

    AxisSpace.Enabled = enable
    ModeLabel.Enabled = enable
    ChangeLabel.Enabled = enable

    EditValueXAbs.Enabled = enable
    EditValueYAbs.Enabled = enable
    EditValueZAbs.Enabled = enable

    EditValueXRel.Enabled = enable
    EditValueYRel.Enabled = enable
    EditValueZRel.Enabled = enable

end sub
