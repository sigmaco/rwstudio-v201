'
' BuildToolbar.vbs
'

Option Explicit

 
' ---------------------------------------------------------------------
Sub OnLaunch
    If Not BuildScriptManager.strActiveTarget = "" Then
         Dim CurrentPlatform, CurrentTarget, cmdVal, cmdExe, RWSMakeLauncher   
         CurrentPlatform  = TargetLink.GetField (BuildScriptManager.strActiveTarget, "Platform")
         CurrentTarget    = TargetLink.GetField (BuildScriptManager.strActiveTarget, "Target")
         
         TargetContextMenu.SetupCustomValues BuildScriptManager.strActiveTarget
                  
         cmdVal           = TargetContextMenu.ConnectionDlg.CommandValue (CurrentPlatform, CurrentTarget, 0)  
         cmdExe           = BuildScriptManager.ExpandString (BuildScriptManager.strActiveTarget, cmdVal)    
  
         MessageLog.Log cmdVal + " " + cmdExe
         
         If Not cmdExe = "" Then
            ' Create the Make object and hook up its events
            Set RWSMakeLauncher = CreateObject ("CSL.RWSBuild.Make")
            GlobalScript.RWSMakeProxy.ConnectMake RWSMakeLauncher
  
            ' Load the rule and run the build process to launch the command
            RWSMakeLauncher.LoadRule AppSettings.BuildRulesDirectory & "\TargetLaunch.rule", True  
            RWSMakeLauncher.AddTask  "RWSLaunch", cmdExe, CurrentPlatform    
            RWSMakeLauncher.Build
  
            ' Disconnect the events
            GlobalScript.RWSMakeProxy.DisconnectMake RWSMakeLauncher
         End If
    End If 
End Sub

 
' ---------------------------------------------------------------------
Sub OnConnect
    If Not BuildScriptManager.strActiveTarget = "" Then
    BuildScriptManager.bIsActiveTarget = True
      '                               Clean, Build, Connect, AllFolders, Target
      BuildScriptManager.BuildConnect False, False, True, False, BuildScriptManager.strActiveTarget
    End If
End Sub

 
' ---------------------------------------------------------------------
Sub OnBuildAndConnect
    If Not BuildScriptManager.strActiveTarget = "" Then
    BuildScriptManager.bIsActiveTarget = True
      '                               Clean, Build, Connect, AllFolders, Target
      BuildScriptManager.BuildConnect False, True, True, False, BuildScriptManager.strActiveTarget
    End If 
End Sub


' ---------------------------------------------------------------------
Sub OnClean
   If Not BuildScriptManager.strActiveTarget = "" Then
      '                            Clean, Build, Connect, AllFolders, Target
      BuildScriptManager.BuildGame True, False, False, False, BuildScriptManager.strActiveTarget
   End If 
End Sub


' ---------------------------------------------------------------------
Sub OnBuild
   If Not BuildScriptManager.strActiveTarget = "" Then
      '                            Clean, Build, Connect, AllFolders, Target
      BuildScriptManager.BuildGame False, True, False, False, BuildScriptManager.strActiveTarget
   End If
End Sub

 
' ---------------------------------------------------------------------
Sub OnRebuild
   If Not BuildScriptManager.strActiveTarget = "" Then
      '                            Clean, Build, Connect, AllFolders, Target
      BuildScriptManager.BuildGame True, True, False, False, BuildScriptManager.strActiveTarget
   End If
End Sub


' ---------------------------------------------------------------------
Sub OnSyncCamera
    If Not BuildScriptManager.strActiveTarget = "" Then
      BuildScriptManager.SendSyncCamera (BuildScriptManager.strActiveTarget)
   End If
End Sub

' ---------------------------------------------------------------------
Sub OnDirectorsCamera
    MainMenu.OnMenuDirectorsCamera
End Sub

' ---------------------------------------------------------------------
Sub OnPauseGame
    MainMenu.OnMenuPauseMode
End Sub

' ---------------------------------------------------------------------
Sub OnResetAllEntities
    MainMenu.OnMenuResetAllEntities
End Sub
