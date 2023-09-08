' Enterprise Layout Management Script
'
' Layout object 'DisplayOptions'
'
    
strAspectRatioOff = "Off"
strAspectRatio43  = "4:3"
strAspectRatio169 = "16:9"    
    
strDispMode0 = "Textured wireframe"
strDispMode1 = "Solid"
strDispMode2 = "Wireframe"
strDispMode3 = "Untextured solid"

strLightMode0 = "Worlds & atomics"
strLightMode1 = "World objects only"
strLightMode2 = "Atomics only"
strLightMode3 = "No objects"

Dim ambLightValue
ambLightValue = 50

Dim DirtyFlag
DirtyFlag = false

Dim platformFilter
platformFilter = ""

' ---------------------------------------------------------------------
' Initialize the dialog's controls
LightEffectCombo.AddItem strLightMode0
LightEffectCombo.AddItem strLightMode1
LightEffectCombo.AddItem strLightMode2
LightEffectCombo.AddItem strLightMode3  

PerspectiveDisplayMode.AddItem strDispMode1
PerspectiveDisplayMode.AddItem strDispMode2
PerspectiveDisplayMode.AddItem strDispMode0
PerspectiveDisplayMode.AddItem strDispMode3

PlanDisplayMode.AddItem strDispMode1
PlanDisplayMode.AddItem strDispMode2
PlanDisplayMode.AddItem strDispMode0
PlanDisplayMode.AddItem strDispMode3

FrontDisplayMode.AddItem strDispMode1
FrontDisplayMode.AddItem strDispMode2
FrontDisplayMode.AddItem strDispMode0
FrontDisplayMode.AddItem strDispMode3

SideDisplayMode.AddItem strDispMode1
SideDisplayMode.AddItem strDispMode2
SideDisplayMode.AddItem strDispMode0
SideDisplayMode.AddItem strDispMode3
 
AspectRatioComboBox.AddItem strAspectRatioOff
AspectRatioComboBox.AddItem strAspectRatio43 
AspectRatioComboBox.AddItem strAspectRatio169 
AspectRatioComboBox.Text = strAspectRatioOff

AmbLightIntensityBar.Min = 0
AmbLightIntensityBar.Max = 100
AmbLightIntensityBar.Value = 50

ambLightValue = 50

' ---------------------------------------------------------------------
' Initialization dependant on other objects being loaded.
Sub OnLoad ()
    DesignView.Ortho.AspectDisplayOn = False
    Initialize
End Sub

' ---------------------------------------------------------------------
Sub Initialize
    Select Case DesignView.Ortho.GetDisplayMode (0)

        case 0
            PerspectiveDisplayMode.Text = strDispMode0
        case 1
            PerspectiveDisplayMode.Text = strDispMode1
        case 2
            PerspectiveDisplayMode.Text = strDispMode2
        case 3
            PerspectiveDisplayMode.Text = strDispMode3

     End Select

     Select Case DesignView.Ortho.GetDisplayMode (1)

        case 0
            PlanDisplayMode.Text = strDispMode0
        case 1
            PlanDisplayMode.Text = strDispMode1
        case 2
            PlanDisplayMode.Text = strDispMode2
        case 3
            PlanDisplayMode.Text = strDispMode3

     End Select

     Select Case DesignView.Ortho.GetDisplayMode (2)

        case 0
            FrontDisplayMode.Text = strDispMode0
        case 1
            FrontDisplayMode.Text = strDispMode1
        case 2
            FrontDisplayMode.Text = strDispMode2
        case 3
            FrontDisplayMode.Text = strDispMode3

     End Select

     Select Case DesignView.Ortho.GetDisplayMode (3)

        case 0
            SideDisplayMode.Text = strDispMode0
        case 1
            SideDisplayMode.Text = strDispMode1
        case 2
            SideDisplayMode.Text = strDispMode2
        case 3
            SideDisplayMode.Text = strDispMode3

     End Select

     Select Case DesignView.Ortho.RWGLightingState
        
        case 0 
          LightEffectCombo.Text = strLightMode0      
        case 1 
          LightEffectCombo.Text = strLightMode1      
        case 2 
          LightEffectCombo.Text = strLightMode2      
        case 3 
          LightEffectCombo.Text = strLightMode3
          
     End Select      

    ambLightValue = DesignView.Ortho.AmbLightIntensity
    AmbLightIntensityBar.Value = ambLightValue
    BrightnessLabel.Caption = ambLightValue
      
    if DesignView.Ortho.ViewLinkValue = 2 then
        LinkViewsCheckBox.Value = True
    else
        LinkViewsCheckBox.Value = False
    end if

    if DesignView.Ortho.FOVLinkValue = 1 then
        FOVLinkBox.Value = True
    else
        FOVLinkBox.Value = False
    end if

    if DesignView.Ortho.BufferTransparentPolygons = True then
        BufferTransparentBox.Value = True
    else
        BufferTransparentBox.Value = False
    end if

    if DesignView.Ortho.DisplayCameraInfo = True Then
        ShowInfoCheckBox.Value = True
    else
        ShowInfoCheckBox.Value = False
    end if

    if DesignView.Ortho.AspectDisplayOn = True then
        dim fract
        fract = DesignView.Ortho.VerticalAspectDisplay / DesignView.Ortho.HorizontalAspectDisplay
        if fract = 0.75 then
            AspectRatioComboBox.Text = strAspectRatio43
        End If
        if fract = 0.5625 then
            AspectRatioComboBox.Text = strAspectRatio169
        End If
    Else
        AspectRatioComboBox.Text = strAspectRatioOff
    End If

    ' Store the platform filter string, and update the text box
    platformFilter = DesignView.RWSUtility.PlatformFilterString
    
    if platformFilter = "" then
        AssetFilterString.Caption = "All platforms"
    else
        AssetFilterString.Caption = platformFilter
    end if  

    EnableFilterCheckBox.Value = DesignView.RWSUtility.EnablePlatformFilter
                                    
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

    AspectRatioComboBox_Apply
    LinkViewsCheckBox_Apply

    PerspectiveDisplayMode_Apply
    PlanDisplayMode_Apply
    FrontDisplayMode_Apply
    SideDisplayMode_Apply

    RWGLightEffect_Apply
    FOVLinkBox_Apply
    BufferTransparentBox_Apply
    ShowInfoCheckBox_Apply
         
    DesignView.Ortho.AmbLightIntensity = AmbLightIntensityBar.Value 
    AmbLightValue = AmbLightIntensityBar.Value
    
    DesignView.RWSUtility.PlatformFilterString = platformFilter
    DesignView.RWSUtility.EnablePlatformFilter = EnableFilterCheckBox.Value 
        
    DirtyFlag = False
    UpDateAppearances
End Sub

' ---------------------------------------------------------------------
Sub AspectRatioComboBox_Apply (  )

    if AspectRatioComboBox.Text = strAspectRatioOff then
        DesignView.Ortho.AspectDisplayOn = False
    end if

    if AspectRatioComboBox.Text = strAspectRatio43 then
        DesignView.Ortho.SetAspectRatioDisplay 4, 3
        DesignView.Ortho.AspectDisplayOn = True
    end if

    if AspectRatioComboBox.Text = strAspectRatio169 then
        DesignView.Ortho.SetAspectRatioDisplay 16, 9
        DesignView.Ortho.AspectDisplayOn = True
    end if

End Sub

' ---------------------------------------------------------------------
Sub LinkViewsCheckBox_Apply (  )

    if LinkViewsCheckBox.Value = False then
       DesignView.Ortho.ViewLinkValue = 1
    else
       DesignView.Ortho.ViewLinkValue = 2
    end if
End Sub

' ---------------------------------------------------------------------
Sub ShowInfoCheckBox_Apply ( )

    DesignView.Ortho.DisplayCameraInfo = ShowInfoCheckBox.Value

End Sub


' ---------------------------------------------------------------------
Sub RWGLightEffect_Apply ( )

    if LightEffectCombo.Text = strLightMode0 then 
       DesignView.Ortho.RWGLightingState = 0 
    end if
    
    if LightEffectCombo.Text = strLightMode1 then 
       DesignView.Ortho.RWGLightingState = 1 
    end if
    
    if LightEffectCombo.Text = strLightMode2 then 
       DesignView.Ortho.RWGLightingState = 2 
    end if

    if LightEffectCombo.Text = strLightMode3 then 
       DesignView.Ortho.RWGLightingState = 3 
    end if        
                           

End Sub

' ---------------------------------------------------------------------
Sub PerspectiveDisplayMode_Apply (  )

    if PerspectiveDisplayMode.Text = strDispMode0 then
        DesignView.Ortho.SetDisplayMode 0, 0
    end if

    if PerspectiveDisplayMode.Text = strDispMode1 then
        DesignView.Ortho.SetDisplayMode 0, 1
    end if

    if PerspectiveDisplayMode.Text = strDispMode2 then
        DesignView.Ortho.SetDisplayMode 0, 2
    end if

    if PerspectiveDisplayMode.Text = strDispMode3 then
        DesignView.Ortho.SetDisplayMode 0, 3
    end if

End Sub

' ---------------------------------------------------------------------
Sub PlanDisplayMode_Apply (  )

    if PlanDisplayMode.Text = strDispMode0 then
        DesignView.Ortho.SetDisplayMode 1, 0
    end if

    if PlanDisplayMode.Text = strDispMode1 then
        DesignView.Ortho.SetDisplayMode 1, 1
    end if

    if PlanDisplayMode.Text = strDispMode2 then
        DesignView.Ortho.SetDisplayMode 1, 2
    end if

    if PlanDisplayMode.Text = strDispMode3 then
        DesignView.Ortho.SetDisplayMode 1, 3
    end if

End Sub

' ---------------------------------------------------------------------
Sub FrontDisplayMode_Apply (  )

    if FrontDisplayMode.Text = strDispMode0 then
        DesignView.Ortho.SetDisplayMode 2, 0
    end if

    if FrontDisplayMode.Text = strDispMode1 then
        DesignView.Ortho.SetDisplayMode 2, 1
    end if

    if FrontDisplayMode.Text = strDispMode2 then
        DesignView.Ortho.SetDisplayMode 2, 2
    end if

    if FrontDisplayMode.Text = strDispMode3 then
        DesignView.Ortho.SetDisplayMode 2, 3
    end if

End Sub

' ---------------------------------------------------------------------
Sub SideDisplayMode_Apply(  )

    if SideDisplayMode.Text = strDispMode0 then
        DesignView.Ortho.SetDisplayMode 3, 0
    end if

    if SideDisplayMode.Text = strDispMode1 then
        DesignView.Ortho.SetDisplayMode 3, 1
    end if

    if SideDisplayMode.Text = strDispMode2 then
        DesignView.Ortho.SetDisplayMode 3, 2
    end if

    if SideDisplayMode.Text = strDispMode3 then
        DesignView.Ortho.SetDisplayMode 3, 3
    end if

End Sub

' ---------------------------------------------------------------------
Sub FOVLinkBox_Apply (  )

    if FOVLinkBox.Value = True then
        DesignView.Ortho.FOVLinkValue = 1
    else
        DesignView.Ortho.FOVLinkValue = 0
    end if

End Sub


' ---------------------------------------------------------------------
Sub BufferTransparentBox_Apply (  )

    if  BufferTransparentBox.Value = True then
        DesignView.Ortho.BufferTransparentPolygons = True
    else
        DesignView.Ortho.BufferTransparentPolygons = False
    end if

End Sub

' ---------------------------------------------------------------------
Sub PerspectiveDisplayMode_Change (  )
    DirtyFlag = true
    UpDateAppearances
End Sub

' ---------------------------------------------------------------------
Sub PlanDisplayMode_Change (  )
    DirtyFlag = true
    UpDateAppearances
End Sub

' ---------------------------------------------------------------------
Sub SideDisplayMode_Change (  )
    DirtyFlag = true
    UpDateAppearances
End Sub

' ---------------------------------------------------------------------
Sub FrontDisplayMode_Change (  )
    DirtyFlag = true
    UpDateAppearances
End Sub

 ' ---------------------------------------------------------------------
Sub PerspectiveLightMode_Change (  )
    DirtyFlag = true
    UpDateAppearances
End Sub


' ---------------------------------------------------------------------
Sub AspectRatioComboBox_Change (  )
    DirtyFlag = true
    UpDateAppearances
End Sub

' ---------------------------------------------------------------------
Sub LinkViewsCheckBox_Change (  )
    DirtyFlag = true
    UpDateAppearances
End Sub

' ---------------------------------------------------------------------
Sub ShowInfoCheckBox_Change ( )
    DirtyFlag = true
    UpDateAppearances
End Sub

' ---------------------------------------------------------------------
Sub PerspectiveSolidButton_Change (  )
    DirtyFlag = true
    UpDateAppearances
End Sub

' ---------------------------------------------------------------------
Sub PlanSolidButton_Change (  )
    DirtyFlag = true
    UpDateAppearances
End Sub

' ---------------------------------------------------------------------
Sub SideSolidButton_Change (  )
    DirtyFlag = true
    UpDateAppearances
End Sub

' ---------------------------------------------------------------------
Sub FrontSolidButton_Change (  )
    DirtyFlag = true
    UpDateAppearances
End Sub

' ---------------------------------------------------------------------
Sub FOVLinkBox_Change (  )
    DirtyFlag = true
    UpDateAppearances
End Sub

' ---------------------------------------------------------------------
Sub BufferTransparentBox_Change (  )
    DirtyFlag = true
    UpDateAppearances
End Sub

' ---------------------------------------------------------------------
Sub CancelButton_Click (  )
    
    ' Reset the ambient light intensity - required if the code
    '  is changed so that the slider affects the light intensity
    '  immediately, rather than waiting for Apply
    DesignView.Ortho.AmbLightIntensity = AmbLightValue
      
    DisplayOptions.StopModal False
End Sub

' ---------------------------------------------------------------------
Sub OKButton_Click (  )
    Apply
    DisplayOptions.StopModal True
End Sub

' ---------------------------------------------------------------------
Sub ApplyButton_Click (  )
    Apply
End Sub

' ---------------------------------------------------------------------
Sub LightEffectCombo_Change (  )
    DirtyFlag = true 
    UpDateAppearances    
End Sub


' ---------------------------------------------------------------------
Sub AmbLightIntensityBar_Change (  )
    DirtyFlag = true
    UpDateAppearances

End Sub

' ---------------------------------------------------------------------
Sub AmbLightIntensityBar_Scroll (  )

    ' Uncomment this line if you want the views to respond immediately
    '  to the slider, rather than only when apply is pressed
    DesignView.Ortho.AmbLightIntensity = AmbLightIntensityBar.Value 

    ' Update the text message    
    BrightnessLabel.Caption = AmbLightIntensityBar.Value   

End Sub

' --------------------------------------------------------------------- 

Sub btnFilterStringSet_Click (  )

    Dim newPlatformFilter

    ' Edit the current filter lister  
    newPlatformFilter = GlobalScript.RWSUtils.EditPlatformString (platformFilter)
   
    ' Check if it's changed
    if (newPlatformFilter <> platformFilter) then
    
        platformFilter = newPlatformFilter
        DirtyFlag = true
      
        if platformFilter = "" then 
            AssetFilterString.Caption = "All platforms" 
        else 
            AssetFilterString.Caption = platformFilter 
        end if 
    
        EnableFilterCheckBox.Value = true
            
        UpDateAppearances 
           
   end if

End Sub

' ---------------------------------------------------------------------  

Sub EnableFilterCheckBox_Click (  )

    DirtyFlag = true 
    UpDateAppearances

End Sub
