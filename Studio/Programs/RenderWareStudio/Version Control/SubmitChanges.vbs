'
' Enterprise Layout Management Script
'
' Layout object 'SubmitChanges'
'


Sub YesButton_Click (  )
    if len(Persist.strCurrentFilename) then
       Persist.SaveProject (Persist.SaveChanges)
       VersionControl.SubmitDefaultChangeSet  
    End if
    SubmitChanges.StopModal True
End Sub

Sub NoButton_Click (  )
    if len(Persist.strCurrentFilename) then 
       VersionControl.SubmitDefaultChangeSet   
    End if 
    SubmitChanges.StopModal True 
End Sub 

Sub CancelButton_Click (  )
   SubmitChanges.StopModal False
End Sub