'
' Enterprise Layout Management Script
'
' Layout object 'MemoryCtrl'
'

'------------------------------------------------------------------------------
' Initialization dependant on other objects being loaded.
Sub OnLoad ()
   MemoryCtrl.SetMaxNumSessions 3
   MemoryCtrl.MaxMessages = 100000
End Sub

Sub MemoryCtrl_OnGotoSource ( FileName, LineBegin, LineEnd ) 
    GlobalScript.ViewFile FileName, LineBegin, LineEnd
End Sub