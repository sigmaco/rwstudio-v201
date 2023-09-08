'
' Enterprise Layout Management Script
'
' Layout object 'DBValidate'
'

' ---------------------------------------------------------------------
Sub Validate
    ' Display dialog
    RENManager.ShowModal "DBValidate", "CSLRenderwareStudioWorkspace"
End Sub

' ---------------------------------------------------------------------
Sub DBValid_OKButton_Click ()
    ' Determine which items are checked
    nFlags = 0
    If DBValid_BehvLessEnts.Value Then
        nFlags = nFlags + 1
    End If
    If DBValid_AttribDataTypes.Value Then
        nFlags = nFlags + 2
    End If
    If DBValid_SharedPrivateAttribs.Value Then
        nFlags = nFlags + 4
    End If
    If DBValid_AttribsWithNoSource.Value Then
        nFlags = nFlags + 8
    End If
    If DBValid_UnusedEnts.Value Then
        nFlags = nFlags + 16
    End If
    If DBValid_UnusedFolders.Value Then
        nFlags = nFlags + 32
    End If
    If DBValid_UnusedAssets.Value Then
        nFlags = nFlags + 64
    End If
    If DBValid_UnusedAttribShares.Value Then
        nFlags = nFlags + 128
    End If

    DBValidate.StopModal True

    If RWSUtils.OptimizeDatabase (nFlags) Then
       MessageLog.Warning "[Validate Database] Validation complete - There were issues raised"
    Else
       MessageLog.Log "[Validate Database] Validation complete - There were no issues raised"
    End If
End Sub

' ---------------------------------------------------------------------
Sub DBValid_CancelButton_Click ()
    DBValidate.StopModal False
End Sub

' ---------------------------------------------------------------------