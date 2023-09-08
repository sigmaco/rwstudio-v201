' 
' Enterprise Layout Management Script 
' 
' Layout object 'About RWStudio' 
'

Option Explicit ' All variables must be explicitly declared

Sub Initialize 
    Dim WshShell, regKeyVersion, regKeyBuild, Version, Build, AppDir 
    Set WshShell = CreateObject("WScript.Shell.1") 
         
    regKeyVersion = "HKLM\SOFTWARE\CSL\RenderWare Studio\Version"  
    regKeyBuild = "HKLM\SOFTWARE\CSL\RenderWare Studio\Build" 
       
    On Error Resume Next   
    Version = WshShell.RegRead(regKeyVersion)   
    If Err > 0 Or Version = "" Then   
       Version = "Value Not Defined"   
    End If   
    On Error Goto 0 

    On Error Resume Next    
    Build = WshShell.RegRead(regKeyBuild)    
    If Err > 0 Or Build = "" Then    
       Build = "Value Not Defined"    
    End If    
    On Error Goto 0
        
    VersionText.Caption = "Version:  " & Version & "   Build:  " & Build
End Sub 
 
Sub OkButton_Click (  )
    About.StopModal True  
End Sub