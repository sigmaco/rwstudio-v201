'
' CSLRenderwareStudioWorkspace.vbs
'
' Script for handling events from RenderWare Studio's RENFrame. This handles
' the layout of windows/controls.

Option Explicit ' All variables must be explicitly declared, with Dim

Dim bLayoutLoaded
bLayoutLoaded = False

' ------------------------------------------------------------------------------
' This event is fired immediately before the first layout is loaded. This gives
' us a chance to change the startup layout.
Sub CSLRenderwareStudioWorkspace_OnActivateDefaultLayout (strLayout)
   Dim strLastLayout
   strLastLayout = Layouts.strLastLayout
   
   If Len (strLastLayout) > 0 Then
      If Layouts.LayoutNameInUse (strLastLayout) Then
         strLayout = strLastLayout
      End If
   End If
End Sub

' ------------------------------------------------------------------------------
' This event is fired immediately after the first layout is loaded
' (after all of the OnLoad() functions have been called) and after calling
' "Restore to Default Layouts"
Sub CSLRenderwareStudioWorkspace_OnDefaultLayoutActivated()
   If Not bLayoutLoaded Then
      ' If g_strCommandLineGame has been set up, by either command arguments
      ' or "Load last project" then load it
      If Len (GlobalScript.g_strCommandLineGame) > 0 Then
         Persist.LoadProject GlobalScript.g_strCommandLineGame
      End If

      ' Set the flag that states the first layout has been loaded. It's now safe to
      ' do any layout manipulation, e.g. CSLRenderwareStudioWorkspace.ShowObject()
      bLayoutLoaded = True

      ' This is the last line of script that is called before any user interaction happens.
      Broadcast.WorkspaceStart
   End If
End Sub

' ------------------------------------------------------------------------------
' Use this event to clean & build from the command line options.
Sub Broadcast_PostImportAllAssets ()
   ' Clean and build if the command line requested it
   If Len (GlobalScript.g_strCommandLineClean) > 0 Then
      ' Parameters:                Clean, Build, Connect, AllFolders, Target UID
      BuildScriptManager.BuildGame True,  False, False, True, GlobalScript.g_strCommandLineClean
      GlobalScript.g_strCommandLineClean = ""
   End If
   If Len (GlobalScript.g_strCommandLineBuild) > 0 Then
      ' Parameters:                Clean, Build, Connect, AllFolders, Target UID
      BuildScriptManager.BuildGame False, True, False, True, GlobalScript.g_strCommandLineBuild
      GlobalScript.g_strCommandLineBuild = ""
   End If

   ' We may have specified to close after a build
   If GlobalScript.g_CommandLineClose Then
      GlobalScript.g_CloseImmediately = True
      RENManager.KillLoop ' This calls through to RENManager_OnTerminate
      Exit Sub
   End If
End Sub
