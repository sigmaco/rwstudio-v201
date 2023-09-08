' 
' Enterprise Layout Management Script
'
' Layout object 'LicenseCheck'
'

Dim License
Set License = CreateObject("LicenseUtils.LicenseCheck.1")

dim daysleft
daysleft = License.GetDaysLeft("RWSLEVDEVELOPER")
If daysleft < 6 And daysleft >= 0 Then
    MsgBox "You have " & daysleft & " day(s) left on your current" & Chr(13) & "RenderWare Studio license", vbOKOnly + vbExclamation, "License Expiry"
End If