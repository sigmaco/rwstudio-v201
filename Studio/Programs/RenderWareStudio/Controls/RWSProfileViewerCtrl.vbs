'
' Enterprise Layout Management Script
'
' Layout object 'RWSProfileViewerCtrl'
'

Sub RWSProfileViewerCtrl_OnGotoSource ( FileName, LineBegin, LineEnd )  
    GlobalScript.ViewFile FileName, LineBegin, LineEnd 
End Sub