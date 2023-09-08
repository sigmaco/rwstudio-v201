'
' Enterprise Layout Management Script
'
' Layout object 'RWSPreview - Animation previewer'
'

Const psStopped = 0
Const psPlaying = 1
Const psPaused  = 2
Const psNoAnim  = 3

Const MenuPlay   = 1
Const MenuPause  = 2
Const MenuStop   = 3
Const MenuRepeat = 4

'------------------------------------------------------------------------------
' The play status (Play/Paused/Stopped) has changed.
Sub RWSPreview_OnStateChanged (NewState)

End Sub

'------------------------------------------------------------------------------
' The preview window encountered an error
Sub RWSPreview_OnError (strMessage)
    CSLRenderwareStudioWorkspace.ShowObject MessageLog
    MessageLog.Error "[Animation Preview] " & strMessage
End Sub