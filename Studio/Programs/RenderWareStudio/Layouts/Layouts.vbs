'
' Layouts.vbs
'
' Script for all functions relating to the layouts.
' Note: The menu item that lists all of the layouts MUST have an index of 10000.
'       This ensures that it will be updated when you call RENManager.UpdateMenuUI

Option Explicit ' All variables must be explicitly declared

' ------------------------------------------------------------------------------
' Initialize constants
' Calling "RENManager.UpdateMenuUI" with the following constants
' updates the appropriate item list in the menu.
Const conUpdateLayouts = 0 ' updates the layouts list
Const conUpdateWindows = 1 ' updates the windows list

' ------------------------------------------------------------------------------
' Script global variables
Dim strLastLayout
strLastLayout = "" ' The layout used in the previous session of RenderWare Studio.

' ------------------------------------------------------------------------------
' Creates a new layout based on the current one.
' If successful, switch to it ready for changes to be made.
Sub CloneCurrentLayout ()
   Dim strDefaultLayoutName, strNewLayoutName, oNewLayoutObj, strErrorMsg

   ' Setup a default name for the layout to offer to the user.
   strDefaultLayoutName = "New Layout"
   If IsObject (CurrentLayout ()) Then
      strDefaultLayoutName = "Copy of " & LayoutName (CurrentLayout ())
   End If

   ' Keep asking for a layout name until a layout is created or cancel is pressed.
   While True
      strNewLayoutName = InputBox ("Enter a name for your new layout?", GlobalScript.g_strTitle, strDefaultLayoutName)
      If Len (strNewLayoutName) = 0 Then
         ' Cancel was pressed or a blank name was entered.
         Exit Sub
      End If

      ' Try and copy the layout.      
      Set oNewLayoutObj = RENManager.CopyLayout (CurrentLayout (), strNewLayoutName)
      If oNewLayoutObj Is Nothing Then
         ' Couldn't copy the layout, state the reason and ask to try again.
         strErrorMsg = "Cannot create layout"
         If LayoutNameInUse (strNewLayoutName) Then
            strErrorMsg = strErrorMsg & ": """ & strNewLayoutName & """ name is already in use"
         End If

         If vbNo = Msgbox (strErrorMsg & " - Try again?", vbYesNo + vbQuestion, GlobalScript.g_strTitle) Then
            Exit Sub
         End If
      Else
         CSLRenderwareStudioWorkspace.ActivateDockstate oNewLayoutObj
         MessageLog.Log "[Workspace] New layout created: " & strNewLayoutName

         ' Repopulate the Layouts selection menu
         RENManager.UpdateMenuUI conUpdateLayouts
         Exit Sub
      End If
   Wend
End Sub

' ------------------------------------------------------------------------------
' Renames the current layout.
' Upon exit, this will delete the old layout file and write a new one with the
' new name to disk.
Sub RenameCurrentLayout()
   Dim strOriginalLayoutName, strNewLayoutName

   ' Setup a default name for the layout to offer to the user.
   If IsObject (CurrentLayout ()) Then
      strOriginalLayoutName = LayoutName (CurrentLayout ())

      ' Keep asking for a layout name until you have a unique one, or cancel is pressed
      While True
         strNewLayoutName = InputBox ("Enter a new name for the current layout", GlobalScript.g_strTitle, strOriginalLayoutName)
         If Len (strNewLayoutName) = 0 Then
            ' Cancel was pressed or a blank name was entered.
            Exit Sub
         End If

         ' Check that the supplied name is unique 
         If Not LayoutNameInUse (strNewLayoutName) Then
            CSLRenderwareStudioWorkspace.RenameDockstate CurrentLayout () , strNewLayoutName
    
            MessageLog.Log "[Workspace] Layout """ & strOriginalLayoutName & """ renamed to """ & strNewLayoutName & """"
            
            ' Repopulate the Layouts selection menu
            RENManager.UpdateMenuUI conUpdateLayouts
            Exit Sub
         Else
            If vbNo = Msgbox ("Cannot rename the current layout. " & _
                              "There is already a layout called """ & strNewLayoutName & """ - Try again?", _
                              vbYesNo + vbQuestion, GlobalScript.g_strTitle) Then
               Exit Sub
            End If
         End If
      Wend
   End If
End Sub

' ------------------------------------------------------------------------------
' Deletes the current layout.
' Upon exit, the file on disk will be deleted.
Sub DeleteCurrentLayout ()
   If CSLRenderwareStudioWorkspace.DockStates.Count < 2 Then
       MsgBox "Can't delete the final layout!", vbOKOnly + vbExclamation, GlobalScript.g_strTitle
   Else
       ' Get the current name 
      If IsObject(CurrentLayout ()) Then 
         If vbYes = MsgBox ("Are you sure you wish to delete the current layout," & _
                            """" & LayoutName (CurrentLayout ()) & """?", _
                            vbYesNo + vbQuestion, GlobalScript.g_strTitle) Then
            ' Remove the layout and active the first layout in the list.
            CSLRenderwareStudioWorkspace.RemoveDockstate CurrentLayout ()
            CSLRenderwareStudioWorkspace.ActivateDockstate CSLRenderwareStudioWorkspace.DockStates(0)

            ' Repopulate the Layouts selection menu
            RENManager.UpdateMenuUI conUpdateLayouts
         End If
      End If
   End If
End Sub

' ------------------------------------------------------------------------------
' Reverts the current layout to the one on disk.
' Pass in False to ResetRunningLayout to reset only the current one.
' Pass in True to ResetRunningLayout to reset all layouts (will lose new ones).
Sub RestoreCurrentLayout
   RENManager.ResetRunningLayout False
End Sub

' ------------------------------------------------------------------------------
' Removes all of the user-defined layouts leaving only the factory-supplied ones.
' Pass in False to ClearRunningLayouts () if you want to make a backup of each
' layout to a .LAYOUTX file, True otherwise.
Sub RestoreToDefaultLayouts ()
   RENManager.ClearRunningLayouts False

   ' Repopulate the Layouts selection menu
   RENManager.UpdateMenuUI conUpdateLayouts
End Sub


' ------------------------------------------------------------------------------
' Catch broadcast events
' ------------------------------------------------------------------------------

' ------------------------------------------------------------------------------
' Handle broadcast message to convert settings.
'Sub Broadcast_UpdateApplicationSettingsVersion (XMLDoc, XMLRootNode, nFileVersion)
'End Sub

' ------------------------------------------------------------------------------
' Handle broadcast message to load settings.
Sub Broadcast_LoadApplicationSettings (XMLRootNode)
   Dim LastLayoutElement
   Set LastLayoutElement = XMLRootNode.SelectSingleNode ("./Layouts/LastLayout")
   If Not LastLayoutElement Is Nothing Then
      strLastLayout = LastLayoutElement.text
   End If
End Sub

' ------------------------------------------------------------------------------
' Handle broadcast message to save settings.
Sub Broadcast_SaveApplicationSettings (XMLDoc, XMLRootNode)
   Dim LayoutsElement, LastLayoutElement, ChildElement
   Set LayoutsElement = XMLRootNode.SelectSingleNode ("./Layouts")

   ' Create the root Layouts element if it isn't already in the settings file.
   If LayoutsElement Is nothing Then
         Set LayoutsElement = XMLDoc.CreateElement ("Layouts")
         XMLRootNode.AppendChild LayoutsElement
   End If

   ' Check we have a layouts element to add our settings to.
   If Not LayoutsElement Is nothing Then
      ' Remove all child nodes.
      For Each ChildElement In LayoutsElement.ChildNodes
         LayoutsElement.RemoveChild ChildElement
      Next
      ' Add in new child nodes.
      Set LastLayoutElement = XMLDoc.CreateElement ("LastLayout")
      LastLayoutElement.Text = LayoutName (CurrentLayout ())
      LayoutsElement.AppendChild LastLayoutElement
   End If
End Sub


' ------------------------------------------------------------------------------
' Helpers
' ------------------------------------------------------------------------------

' ------------------------------------------------------------------------------
' Returns if a name is already used as a layout name.
' Returns True if a name is used, False otherwise.
Function LayoutNameInUse (strName)
   Dim oLayout

   For Each oLayout In CSLRenderwareStudioWorkspace.DockStates
      If LayoutName (oLayout) = strName Then
         LayoutNameInUse = True
         Exit Function
      End If
   Next

   LayoutNameInUse = False
End Function

' ------------------------------------------------------------------------------
' Returns the name of the layout object passed in.
Function LayoutName (oLayout)
   LayoutName = CSLRenderwareStudioWorkspace.DockStates.GetKeyName (oLayout)
End Function

' ------------------------------------------------------------------------------
' Returns the current layout object.
Function CurrentLayout ()
   Set CurrentLayout = CSLRenderwareStudioWorkspace.CurrentDockState
End Function
