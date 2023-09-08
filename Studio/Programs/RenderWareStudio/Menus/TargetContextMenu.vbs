' 
' Enterprise Layout Management Script
'
' Layout object 'TargetContextMenu'
'

Option Explicit ' All variables must be explicitly declared 

'------------------------------------------------------------------------------- 
' Constants
Const DefaultTimeout = 10000 ' 10 seconds 

'------------------------------------------------------------------------------- 
' Script objects
Dim ConnectionDlg
Set ConnectionDlg = CreateObject ("CSL.RWSTarget.RWSConnectionDlg")

'------------------------------------------------------------------------------- 
' Script variables
Dim ActiveFolderID, NumBasicMenuItems, NumExtendedMenuItems, strSelectedTarget
Dim PropertiesID, PropertiesName, PropertiesEvent

ActiveFolderID       = 0
NumBasicMenuItems    = 0 ' To be setup each time a menu is created
NumExtendedMenuItems = 0 ' To be setup each time a menu is created
PropertiesID         = 0
PropertiesName       = ""
PropertiesEvent      = ""
strSelectedTarget    = ""' The target the user has just right-clicked on.
NumBasicMenuItems    = TargetContextMenu.MenuItems ("TargetContextMenu").MenuItems.Count ' The number of items in the original menu

'-------------------------------------------------------------------------------
Sub DisplayContextMenu (nX, nY, hWnd)
    Dim MenuIndex
    Dim Menu
    Set Menu = TargetContextMenu.CloneMenu ()
    MenuIndex = SetupContextMenu (Menu)

    If Not MenuIndex = -1 Then 
        Menu.ShowAsContextMenu MenuIndex, nX, nY, hWnd
    End If 
End Sub

'------------------------------------------------------------------------------- 
Function SetupContextMenu (Menu)
    Dim TgMenu
    Set TgMenu       = Menu.MenuItems ("TargetContextMenu")
    SetupContextMenu = TgMenu.Index

    If Len (strSelectedTarget) = 0 Then
        ' No item selected, only allowed to create a new connection 
        EnableAllMenuItems TgMenu, False
        TgMenu.MenuItems.Item ("New...").Enabled = True 
    Else    
        ' Ensure that all menu items are enabled from the start. 
        EnableAllMenuItems TgMenu, True

        AddDynamicCommands Menu, TgMenu, ConnectionDlg

        Dim strID, ConnectionID
        strID = TargetLink.GetField (strSelectedTarget, "ID")
        ConnectionID = 0 
        If Len (strID) > 0 Then 
            On Error Resume Next 
            ConnectionID = CLng (strID) 
            If Not Err.Number = 0 Then
                ConnectionID = 0
            End If
            On Error Goto 0 
        End If 

        ' Enable correct menus 
        If ConnectionID > 0 Then ' We're connected
            TgMenu.MenuItems.Item ("Connect").MenuName = "Disconnect"
            TgMenu.MenuItems.Item ("Build and Connect").Enabled = False
            
            ' We're connected, so not allowed to use the build rules.
            TgMenu.MenuItems.Item ("Clean").Enabled   = False
            TgMenu.MenuItems.Item ("Build").Enabled   = False
            TgMenu.MenuItems.Item ("Rebuild").Enabled = False
            TgMenu.MenuItems.Item ("Clean (All Folders)").Enabled   = False 
            TgMenu.MenuItems.Item ("Build (All Folders)").Enabled   = False 
            TgMenu.MenuItems.Item ("Rebuild (All Folders)").Enabled = False 
            TgMenu.MenuItems.Item ("Delete").Enabled = False
            TgMenu.MenuItems.Item ("Launch Net Test").Enabled = False
        Else
            ' We're not connected, so disable items that can only be used when connected.
            TgMenu.MenuItems.Item ("Reload Game").Enabled        = False
            TgMenu.MenuItems.Item ("Reset All Entities").Enabled = False
            TgMenu.MenuItems.Item ("Synchronise Camera").Enabled = False

            ' We've no active folder, can't use the build rules unless a game's loaded 
            If ActiveFolderID = 0 Then 
                TgMenu.MenuItems.Item ("Clean").Enabled   = False 
                TgMenu.MenuItems.Item ("Build").Enabled   = False 
                TgMenu.MenuItems.Item ("Rebuild").Enabled = False 
                TgMenu.MenuItems.Item ("Connect").Enabled = False
                TgMenu.MenuItems.Item ("Clean (All Folders)").Enabled   = False  
                TgMenu.MenuItems.Item ("Build (All Folders)").Enabled   = False  
                TgMenu.MenuItems.Item ("Rebuild (All Folders)").Enabled = False                
                TgMenu.MenuItems.Item ("Build and Connect").Enabled     = False
            End If

            Dim fso
            Set fso = CreateObject ("Scripting.FileSystemObject")
            If Not fso.FileExists (AppSettings.NetTestCommand) Then
               ' Net Test isn't installed.
               TgMenu.MenuItems.Item ("Launch Net Test").Enabled = False
            ElseIf Not TargetLink.GetField (strSelectedTarget, "Address") = "localhost" Then
               ' Only allow "Launch Net Test" if we're set up for connecting to localhost.
               TgMenu.MenuItems.Item ("Launch Net Test").Enabled = False
            End If
        End If 
    End If 
End Function

'------------------------------------------------------------------------------- 
Sub AddDynamicCommands (Menu, TgMenu, ConnectionDlg)
        Dim CurrentPlatform, CurrentTarget, cmdName, Index  
        CurrentPlatform  = TargetLink.GetField (strSelectedTarget, "Platform")  
        CurrentTarget    = TargetLink.GetField (strSelectedTarget, "Target")  
        NumExtendedMenuItems  = ConnectionDlg.CommandCount (CurrentPlatform, CurrentTarget) 
        If NumExtendedMenuItems > 0 Then 
            For Index = 1 To NumExtendedMenuItems  
                cmdName = ConnectionDlg.CommandName (CurrentPlatform, CurrentTarget, Index - 1)  
                Menu.AddSubMenuItem cmdName, 1, NumBasicMenuItems + Index - 1, 0, True, "OnExtendedCommand_" & Index - 1, 0, False, False, False  
            Next 
 
            ' Add a divider 
            Menu.AddSubMenuItem "-", 1, NumBasicMenuItems + NumExtendedMenuItems, 0, True, "", 0, False, False, False  
                         
            ' Move the properties menu item to the bottom of the list 
            Dim PropItem 
            Set PropItem    = TGMenu.MenuItems.Item ("Properties") 
            PropertiesID    = PropItem.Index 
            PropertiesName  = PropItem.MenuName 
            PropertiesEvent = PropItem.Event 
            Menu.AddSubMenuItem PropertiesName, 1, NumBasicMenuItems + NumExtendedMenuItems + 1, 0, True, PropertiesEvent, 0, False, False, False  
            Menu.DeleteItem (PropertiesID)    
 
            'Menu.AddSubMenuItem PropItem.MenuName, 1, NumBasicMenuItems + NumExtendedMenuItems + 1, 0, True, PropItem.Event, 0, False, False, False 
            'Menu.DeleteItem (PropItem.Index)   
        End If  
End Sub

'-------------------------------------------------------------------------------  
' Recursively enable/disable the child menu items.
Sub EnableAllMenuItems (Parent, bEnabled)
    Dim MenuItem
    For Each MenuItem In Parent.Menuitems
        MenuItem.Enabled = bEnabled
        EnableAllMenuItems MenuItem, bEnabled
    Next 
End Sub

'------------------------------------------------------------------------------- 
' Menu Handlers
'------------------------------------------------------------------------------- 

'------------------------------------------------------------------------------ 
Sub OnExtendedCommand_0 ()
    BuildScriptManager.RunExtendedCommand 0, strSelectedTarget
End Sub

'------------------------------------------------------------------------------ 
Sub OnExtendedCommand_1 ()
    BuildScriptManager.RunExtendedCommand 1, strSelectedTarget
End Sub

'------------------------------------------------------------------------------ 
Sub OnExtendedCommand_2 ()
    BuildScriptManager.RunExtendedCommand 2, strSelectedTarget
End Sub

'------------------------------------------------------------------------------ 
Sub OnExtendedCommand_3 ()
    BuildScriptManager.RunExtendedCommand 3, strSelectedTarget
End Sub

'------------------------------------------------------------------------------ 
Sub OnExtendedCommand_4 ()
    BuildScriptManager.RunExtendedCommand 4, strSelectedTarget
End Sub

'------------------------------------------------------------------------------ 
Sub OnExtendedCommand_5 ()
    BuildScriptManager.RunExtendedCommand 5, strSelectedTarget
End Sub

'------------------------------------------------------------------------------ 
Sub OnExtendedCommand_6 ()
    BuildScriptManager.RunExtendedCommand 6, strSelectedTarget
End Sub

'------------------------------------------------------------------------------ 
Sub OnExtendedCommand_7 ()
    BuildScriptManager.RunExtendedCommand 7, strSelectedTarget
End Sub

'------------------------------------------------------------------------------ 
Sub OnExtendedCommand_8 ()
    BuildScriptManager.RunExtendedCommand 8, strSelectedTarget
End Sub

'------------------------------------------------------------------------------ 
Sub OnExtendedCommand_9 ()
    BuildScriptManager.RunExtendedCommand 9, strSelectedTarget
End Sub

'------------------------------------------------------------------------------ 
Sub OnExtendedCommand_10 ()
    BuildScriptManager.RunExtendedCommand 10, strSelectedTarget
End Sub

'------------------------------------------------------------------------------ 
Sub OnClean ()                ' Clean, Build, Connect, AllFolders, CurrentTarget
   BuildScriptManager.BuildGame True, False, False, False, strSelectedTarget
End Sub 
 
'------------------------------------------------------------------------------ 
Sub OnBuild ()                 ' Clean, Build, Connect, AllFolders, CurrentTarget
    BuildScriptManager.BuildGame False, True, False, False, strSelectedTarget
End Sub 
 
'------------------------------------------------------------------------------ 
Sub OnRebuild ()               ' Clean, Build, Connect, AllFolders, CurrentTarget
    BuildScriptManager.BuildGame True, True, False, False, strSelectedTarget
End Sub 
 
'------------------------------------------------------------------------------
Sub OnCleanAllFolders ()      ' Clean, Build, Connect, AllFolders, CurrentTarget
   BuildScriptManager.BuildGame True, False, False, True, strSelectedTarget
End Sub

'------------------------------------------------------------------------------
Sub OnBuildAllFolders ()       ' Clean, Build, Connect, AllFolders, CurrentTarget
    BuildScriptManager.BuildGame False, True, False, True, strSelectedTarget
End Sub

'------------------------------------------------------------------------------ 
Sub OnRebuildAllFolders ()     ' Clean, Build, Connect, AllFolders, CurrentTarget
    BuildScriptManager.BuildGame True, True, False, True, strSelectedTarget
End Sub 

'------------------------------------------------------------------------------ 
Sub OnConnect ()                 ' Clean, Build, Connect, AllFolders, CurrentTarget
   If Not BuildScriptManager.strActiveTarget = strSelectedTarget Then
         BuildScriptManager.bIsActiveTarget = False
   Else
         BuildScriptManager.bIsActiveTarget = True
   End If
    
   BuildScriptManager.BuildConnect False, False, True, False, strSelectedTarget
End Sub 

'------------------------------------------------------------------------------  
Sub OnBuildAndConnect ()         ' Clean, Build, Connect, AllFolders, CurrentTarget
   If Not BuildScriptManager.strActiveTarget = strSelectedTarget Then
         BuildScriptManager.bIsActiveTarget = False
   Else
         BuildScriptManager.bIsActiveTarget = True
   End If
    
   BuildScriptManager.BuildConnect False, True, True, False, strSelectedTarget
End Sub 

'------------------------------------------------------------------------------ 
Sub OnReloadGame () 
    BuildScriptManager.ReloadGame (strSelectedTarget)
End Sub 
 
'------------------------------------------------------------------------------ 
Sub OnResetAllEntities () 
    BuildScriptManager.ResetAllEntities (strSelectedTarget)
End Sub 

' -------------------------------------------------------------------- 
Sub OnSyncCamera ()
   BuildScriptManager.SendSyncCamera (strSelectedTarget)
End Sub

' --------------------------------------------------------------------  
Sub OnCreateConnection ()
   Dim i, NewRowUID, bUIDFound, RowUID

   ' Find a unique connection ID  
   i = 1  
   Do  
      NewRowUID = "Connection" & i  
      bUIDFound = False  
  
      For Each RowUID In TargetLink.GetRowUIDs  
         If RowUID = NewRowUID Then  
            bUIDFound = True  
            Exit For  
         End If  
      Next  
  
      i = i + 1  
   Loop While bUIDFound = True  
  
   ' Now create the new row in the table  
   TargetLink.AddRow NewRowUID
   TargetLink.SetField NewRowUID, "Name",        NewRowUID
   TargetLink.SetField NewRowUID, "Status",      "Disconnected"  
   TargetLink.SetField NewRowUID, "Resources",   ""  
   TargetLink.SetField NewRowUID, "EmbedAssets", "yes"  
   TargetLink.SetField NewRowUID, "Timeout",     CStr (DefaultTimeout)  
   TargetLink.SetField NewRowUID, "ID",          "0"  
   TargetLink.SetField NewRowUID, "Address",     "Enter IP Address here"  

   If Not strSelectedTarget = "" Then
      TargetLink.SetField NewRowUID, "Platform",    TargetLink.GetField (strSelectedTarget, "Platform")
      TargetLink.SetField NewRowUID, "Target",      TargetLink.GetField (strSelectedTarget, "Target")
      TargetLink.SetField NewRowUID, "Resources",   TargetLink.GetField (strSelectedTarget, "Resources")  
      TargetLink.SetField NewRowUID, "Address",     TargetLink.GetField (strSelectedTarget, "Address")
      TargetLink.SetField NewRowUID, "TargetRF3Template",    TargetLink.GetField (strSelectedTarget, "TargetRF3Template")

      ConnectionDlg.ConnectionPlatform  = TargetLink.GetField (NewRowUID, "Platform")  
      ConnectionDlg.ConnectionTarget    = TargetLink.GetField (NewRowUID, "Target")

      SetupCustomValues (strSelectedTarget)
      StoreCustomValues (NewRowUID)
   End If
   
   ' Now show the connection properties  
   strSelectedTarget = NewRowUID  

   ' Show the new row  
   If TargetContextMenu.OnProperties () Then  
      TargetLink.ShowRow NewRowUID  
   Else  
      ' Remove unwanted row if cancel was pressed  
      TargetLink.RemoveRow NewRowUID  
   End If
End Sub

' --------------------------------------------------------------------   
Sub OnDeleteConnection ()
   Dim reply
   reply = MsgBox ("Are you sure you want to delete the " & _
                   TargetLink.GetField (strSelectedTarget, "Name") & _
                   " connection?", vbYesNo + vbQuestion, "Confirm Deletion") 
 
   If reply = vbYes Then 
      TargetLink.RemoveRow strSelectedTarget 
   End If
End Sub 

' -------------------------------------------------------------------- 
Sub OnSetActiveConnection ()
    BuildScriptManager.SetActiveConnection (strSelectedTarget)
    MessageLog.Log "[Target] Active target set to " & _
                   TargetLink.GetField (strSelectedTarget, "Name")
    CSLRenderwareStudioWorkspace.ShowObject MessageLog
End Sub

' -------------------------------------------------------------------- 
Function OnProperties ()
    SetupConnectionDlg (strSelectedTarget)

    ' Display the dialog
    If ConnectionDlg.Show () Then
        StoreConnectionDlgValues (strSelectedTarget)
        OnProperties = True
    Else
        OnProperties = False
    End If
End Function

'------------------------------------------------------------------------------- 
' Helpers
'-------------------------------------------------------------------------------

'------------------------------------------------------------------------------- 
Sub SetupConnectionDlg (strCurrentTarget)
    ' Set the values directly from the xml file.
    ConnectionDlg.DialogCaption       = "Connection Properties"  
    ConnectionDlg.ConnectionName      = TargetLink.GetField (strCurrentTarget, "Name")
    ConnectionDlg.ConnectionAddress   = TargetLink.GetField (strCurrentTarget, "Address")  
    ConnectionDlg.ConnectionResources = TargetLink.GetField (strCurrentTarget, "Resources")  
    ConnectionDlg.ConnectionPlatform  = TargetLink.GetField (strCurrentTarget, "Platform")   
    ConnectionDlg.ConnectionTarget    = TargetLink.GetField (strCurrentTarget, "Target")   
    ConnectionDlg.TargetRF3Template   = TargetLink.GetField (strCurrentTarget, "TargetRF3Template")   

    ' Set the value that require validation
    ' Set the Timeout value
    Dim Timeout
    Timeout = TargetLink.GetField (strCurrentTarget, "Timeout")  
    If Timeout = "Infinite" Then  
       ConnectionDlg.ConnectionTimeout = 0  
    Else  
       ConnectionDlg.ConnectionTimeout = Timeout  
    End If  

    ' Set the Port number
    Dim strPort
    strPort = TargetLink.GetField (strCurrentTarget, "Port")  
    If Len (strPort) = 0 Then  
        strPort = "0"  
    End If
    ConnectionDlg.ConnectionPort   = CInt (strPort)

    ' Set the embed assets flag
    If "yes" = TargetLink.GetField (strCurrentTarget, "EmbedAssets") Then  
         ConnectionDlg.EmbedAssets = True  
    Else  
         ConnectionDlg.EmbedAssets = False  
    End If

    SetupCustomValues (strCurrentTarget)
End Sub

'------------------------------------------------------------------------------- 
Sub SetupCustomValues (strCurrentTarget)
    ' Set the custom values
    Dim NumCustomVals, Index, CustomValueName, CustomColUID, CustomColName
    If Not ConnectionDlg.ConnectionPlatform = "" And Not ConnectionDlg.ConnectionTarget = "" Then  
        NumCustomVals = ConnectionDlg.NumberOfCustomVariables (ConnectionDlg.ConnectionPlatform, ConnectionDlg.ConnectionTarget)  
   
        For Index = 1 To NumCustomVals  
            CustomValueName = ConnectionDlg.CustomValueNameByIndex (Index - 1)  
            CustomColUID = RemoveWhiteSpace (CustomValueName)  
            CustomColName = TargetLink.GetField (strCurrentTarget, CustomColUID)  
            ConnectionDlg.SetCustomValueByName CustomValueName, CustomColName  
       Next  
    End If
End Sub

'------------------------------------------------------------------------------- 
Sub StoreConnectionDlgValues (strCurrentTarget)
    ' Set the data for the row/column  
    TargetLink.SetField strCurrentTarget, "Name",              ConnectionDlg.ConnectionName  
    TargetLink.SetField strCurrentTarget, "Address",           ConnectionDlg.ConnectionAddress  
    TargetLink.SetField strCurrentTarget, "Resources",         ConnectionDlg.ConnectionResources  
    TargetLink.SetField strCurrentTarget, "Platform",          ConnectionDlg.ConnectionPlatform   
    TargetLink.SetField strCurrentTarget, "Target",            ConnectionDlg.ConnectionTarget   
    TargetLink.SetField strCurrentTarget, "Port",        CStr (ConnectionDlg.ConnectionPort)   
    TargetLink.SetField strCurrentTarget, "TargetRF3Template", ConnectionDlg.TargetRF3Template   
    TargetLink.SetField strCurrentTarget, "PlatformFlags",     ConnectionDlg.ConnectionPlatformFlags   
    TargetLink.SetRowIcon strCurrentTarget, GetIconFile (ConnectionDlg.ConnectionPlatform)   

    ' Set the timeout value
    Dim Timeout
    Timeout = ConnectionDlg.ConnectionTimeout  
    If Timeout = 0 Then  
        TargetLink.SetField strCurrentTarget, "Timeout", "Infinite"  
    Else  
        TargetLink.SetField strCurrentTarget, "Timeout", Timeout  
    End If

    ' Set the Embed Assets value
    Dim EmbedVal  
    If ConnectionDlg.EmbedAssets Then  
        EmbedVal = "yes"  
    Else  
        EmbedVal = "no"  
    End If  
    TargetLink.SetField strCurrentTarget, "EmbedAssets", EmbedVal

    StoreCustomValues(strCurrentTarget)
End Sub

'-------------------------------------------------------------------------------
Sub StoreCustomValues (strCurrentTarget)
    ' Set the custom values
    Dim Index, NumCustomVals, CustomColUID, CustomValue
    NumCustomVals = ConnectionDlg.NumberOfCustomVariables (ConnectionDlg.ConnectionPlatform, ConnectionDlg.ConnectionTarget)   
    For Index = 1 To NumCustomVals
       CustomColUID = RemoveWhiteSpace (ConnectionDlg.CustomValueNameByIndex (Index - 1))  
       CustomValue  = ConnectionDlg.CustomValueByIndex (Index - 1)  
       TargetLink.SetField strCurrentTarget, CustomColUID, CustomValue  
    Next
End Sub

'-------------------------------------------------------------------------------
Function RemoveWhiteSpace (LaunchString)
    Dim Reg, Match
    Set Reg          = New RegExp 
    Reg.Pattern      = "\s" 
    Reg.Global       = True 
    RemoveWhiteSpace = LaunchString  
 
    For Each Match In Reg.Execute (LaunchString) 
        RemoveWhiteSpace = (Reg.Replace (LaunchString, "")) 
    Next 
End Function 

'------------------------------------------------------------------------------ 
Function GetIconFile (strPlatform) 
   Dim Root, strIcon 

   Root        = AppSettings.InstallDirectory 
   strIcon     = Root & "\Workspace\Resources\" '" 
   strIcon     = strIcon + strPlatform + ".ico" 
   GetIconFile = strIcon 
End Function

'------------------------------------------------------------------------------  
Sub OnLaunchNetTest ()
   Dim strPort, Connection, strPlatform

   strPort = TargetLink.GetField (strSelectedTarget, "Port")
   strPlatform = TargetLink.GetField (strSelectedTarget, "Platform")
   Set Connection = GlobalScript.RWSComms.CreateNullConnection (strPlatform, 0, 0)

   If Len (strPort) > 0 Then
      Dim strCommand, WshShell, fso
      Set fso = CreateObject ("Scripting.FileSystemObject")
      If fso.FileExists (AppSettings.NetTestCommand) Then
         ' Only try and launch the NetTest command if the app exists.
         Set WshShell = CreateObject("WScript.Shell")
         WshShell.Run """" & AppSettings.NetTestCommand & """ " & strPort & " " & Connection.BigEndian
      End If
   End If
End Sub

' ------------------------------------------------------------------------------
' Catch broadcast events
' ------------------------------------------------------------------------------

' ------------------------------------------------------------------------------
' Handle broadcast event to say active folder has changed
Sub Broadcast_ActiveFolderChanged (NewRootFolder)
    ActiveFolderID = NewRootFolder
End Sub
