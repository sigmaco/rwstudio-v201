'
' Enterprise Layout Management Script
'
' Layout object 'RWSEventListCtrl1'
'
Dim g_bIsVisible, g_bNeedRefresh
g_bIsVisible   = False
g_bNeedRefresh = False

'------------------------------------------------------------------------------
' Initialization dependant on other objects being loaded.
Sub OnLoad ()
    ' Initialize event viz's selection to be the global selection of entities.
    RWSEventList.CurrentSelection = GlobalScript.g_strGlobalSelection
End Sub

'------------------------------------------------------------------------------
Sub RWSEventList_OnOutputMessage ( Severity, Message )
    MessageLog.Message Severity, "[Event List] " & Message
End Sub

'------------------------------------------------------------------------------
Sub RWSEventList_OnVisibleChanged (bVisible)
    If bVisible And Not g_bIsVisible Then
        ' Only refresh if we need to (source changed, or new root-folder is active)
        If g_bNeedRefresh Then
            g_bNeedRefresh = False
            RWSEventList.Refresh
        End If
    End If
    g_bIsVisible = bVisible
End Sub

' ------------------------------------------------------------------------------
' Catch broadcast events
' ------------------------------------------------------------------------------

' ------------------------------------------------------------------------------
' Handle broadcast event to say source parse has completed
Sub Broadcast_PostParseSource ()
    ' Only refresh if window is visible.
    ' Otherwise, just set flag so it will refresh when it becomes visible
    If g_bIsVisible Then
        g_bNeedRefresh = False
        RWSEventList.Refresh
    Else
        g_bNeedRefresh = True
    End If
End Sub

' ------------------------------------------------------------------------------
' Handle broadcast event to say active folder has changed
Sub Broadcast_ActiveFolderChanged (NewRootFolder)
    RWSEventList.ActiveFolder = NewRootFolder

    ' Only refresh if window is visible.
    ' Otherwise, just set flag so it will refresh when it becomes visible
    If g_bIsVisible Then
        g_bNeedRefresh = False
        RWSEventList.Refresh
    Else
        g_bNeedRefresh = True
    End If
End Sub
