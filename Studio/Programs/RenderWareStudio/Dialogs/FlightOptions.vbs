
' 
' Enterprise Layout Management Script 
' 
' Layout object 'Flight Options' 
'

Option Explicit

' Initialize the flight sensitivity slider controls

Const SensitivityDefaultValue = 50   ' Values to set the respective slider controls
Const ZoomFactorDefaultValue  = 50   ' when the user hits 'reset'
 
MouseSensitivityBar.Min   = 1 
MouseSensitivityBar.Max   = 100 
MouseSensitivityBar.Value = SensitivityDefaultValue

MouseWheelSensitivityBar.Min   = 1  
MouseWheelSensitivityBar.Max   = 100  
MouseWheelSensitivityBar.Value = SensitivityDefaultValue

KeySensitivityBar.Min   = 1  
KeySensitivityBar.Max   = 100  
KeySensitivityBar.Value = SensitivityDefaultValue

ZoomFactorBar.Min   = 0
ZoomFactorBar.Max   = 100
ZoomFactorBar.Value = ZoomFactorDefaultValue

' Range of values covered by the ZoomFactorBar - min and max at each end
'    The defValue corresponds to the mid-point of the bar

' The smaller the zoom factor, the closer to the object the camera will be
'  positions (beware of setting the value too low - it could end up positioning
'  the camera closer to the object than the camera's front clip plane, so the
'  object effectively 'disappears')

Const ZoomFactorMinValue = 0.5
Const ZoomFactorMaxValue = 10.0
Const ZoomFactorDefValue = 1.0

Dim DirtyFlag 
DirtyFlag = false

' --------------------------------------------------------------------- 
' Initialization 

' --------------------------------------------------------------------- 
Sub Initialize 

    KeySensitivityBar.Value = DesignView.Camera.KeySensitivityValue
    MouseSensitivityBar.Value = DesignView.Camera.SensitivityValue
    MouseWheelSensitivityBar.Value = DesignView.DragCamera.WheelSensitivity 
    
    Dim ZoomFactor
    ZoomFactor = DesignView.RWSUtility.LocateZoomFactor
    
    ' Clamp the ZoomFactor to the Min/Max ranges (defined above)
    if ZoomFactor < ZoomFactorMinValue Then
        ZoomFactor = ZoomFactorMinValue
    Elseif ZoomFactor > ZoomFactorMaxValue Then
        ZoomFactor = ZoomFactorMaxValue
    End If

    ' Setup the zoom bar -
    '   Slider bar values of 0 - 50 covers the range [ZoomFactorMinValue, ZoomFactorDefValue]
    '   Slider bar values of 50 - 100 covers the range [ZoomFactorDefValue, ZoomFactorMaxValue]
    
    If DesignView.RWSUtility.LocateZoomFactor <= ZoomFactorDefValue Then
       ZoomFactorBar.Value = 50 * (DesignView.RWSUtility.LocateZoomFactor - ZoomFactorMinValue) / _
                                  (ZoomFactorDefValue - ZoomFactorMinValue)
    Else
       ZoomFactorBar.Value = 50 + 50 * (DesignView.RWSUtility.LocateZoomFactor - ZoomFactorDefValue) / _
                                          (ZoomFactorMaxValue - ZoomFactorDefValue)
    End If
                                         
    DirtyFlag = False 
    UpDateAppearances 
End Sub

' --------------------------------------------------------------------- 
Sub UpDateAppearances 
    If DirtyFlag = True Then 
        ApplyButton.Enabled = True 
    Else 
        ApplyButton.Enabled = False 
    End If 
End Sub

' --------------------------------------------------------------------- 
Sub Apply 
    DesignView.Camera.KeySensitivityValue  = KeySensitivityBar.Value
    DesignView.Camera.SensitivityValue     = MouseSensitivityBar.Value  
    DesignView.DragCamera.WheelSensitivity = MouseWheelSensitivityBar.Value  
    
    Dim ZoomFactor
    ZoomFactor = ZoomFactorBar.Value
 
    '   Slider bar values of 0 - 50 covers the range [ZoomFactorMinValue, ZoomFactorDefValue] 
    '   Slider bar values of 50 - 100 covers the range [ZoomFactorDefValue, ZoomFactorMaxValue] 
    if ZoomFactor <= 50 Then
        DesignView.RWSUtility.LocateZoomFactor = ZoomFactorMinValue +                         _
                                                  (ZoomFactorDefValue - ZoomFactorMinValue) * _
                                                  ZoomFactor / 50.0
    Else
        DesignView.RWSUtility.LocateZoomFactor = ZoomFactorDefValue +                         _
                                                  (ZoomFactorMaxValue - ZoomFactorDefValue) * _
                                                  (ZoomFactor - 50.0) / 50.0
    End If
  
    DirtyFlag = False 
    UpDateAppearances 
End Sub

' ---------------------------------------------------------------------
Sub ApplyButton_Click (  )
    Apply
End Sub

' ---------------------------------------------------------------------
Sub CancelButton_Click (  )
    FlightOptions.StopModal False
End Sub

' ---------------------------------------------------------------------
Sub OKButton_Click (  )
    Apply 
    FlightOptions.StopModal True
End Sub

' ---------------------------------------------------------------------
Sub KeySensitivityBar_Scroll (  )
    DirtyFlag = true  
    UpDateAppearances
End Sub

' ---------------------------------------------------------------------
Sub MouseSensitivityBar_Scroll (  )
    DirtyFlag = true  
    UpDateAppearances
End Sub

' ---------------------------------------------------------------------
Sub MouseWheelSensitivityBar_Scroll (  )
    DirtyFlag = true  
    UpDateAppearances
End Sub

' ---------------------------------------------------------------------
Sub ZoomFactorBar_Scroll (  )
    DirtyFlag = true   
    UpDateAppearances
End Sub

' -----------------------------------------------------------------------------
Sub Broadcast_LoadApplicationSettings (XMLRootNode)

    Dim DisableFilter
    DisableFilter = True

    ' Look for the section in the settings file
    Dim ZoomFactorElement
    Set ZoomFactorElement = XMLRootNode.SelectSingleNode ("./FlightOptions/LocateZoomFactor")
        
    If Not ZoomFactorElement Is Nothing Then
        If IsNumeric (ZoomFactorElement.text) Then
            DesignView.RWSUtility.LocateZoomFactor = CSng (ZoomFactorElement.text)
        End If
    End If
 
End Sub

' -----------------------------------------------------------------------------
Sub Broadcast_SaveApplicationSettings (XMLDoc, XMLRootNode)

    ' Only need to store the LocateZoomFactor here - the flight sensitivity
    '  values are already stored in <./Settings/Camera> by the module

    Dim RootXMLElement
    Set RootXMLElement = XMLRootNode.SelectSingleNode ("./FlightOptions")
    If RootXMLElement Is Nothing Then
        Set RootXMLElement = XMLDoc.CreateElement ("FlightOptions")
        XMLRootNode.AppendChild RootXMLElement
    End If

    If Not RootXMLElement Is Nothing Then

        ' Now empty child nodes
        Dim ChildElement
        For Each ChildElement In RootXMLElement.ChildNodes
            RootXMLElement.RemoveChild ChildElement
        Next

        Dim ZoomFactorElement
        Set ZoomFactorElement = XMLDoc.CreateElement ("LocateZoomFactor")

        If Not ZoomFactorElement Is Nothing Then
            ZoomFactorElement.text = DesignView.RWSUtility.LocateZoomFactor
            RootXMLElement.AppendChild ZoomFactorElement
        End If

   End If

End sub

' -----------------------------------------------------------------------------
Sub ResetSensitivity_Click (  )
    MouseSensitivityBar.Value      = SensitivityDefaultValue  
    MouseWheelSensitivityBar.Value = SensitivityDefaultValue 
    KeySensitivityBar.Value        = SensitivityDefaultValue 
    DirtyFlag = true    
    UpDateAppearances
End Sub

' -----------------------------------------------------------------------------
Sub ResetZoom_Click (  )
    ZoomFactorBar.Value = ZoomFactorDefaultValue
    DirtyFlag = true    
    UpDateAppearances
End Sub
