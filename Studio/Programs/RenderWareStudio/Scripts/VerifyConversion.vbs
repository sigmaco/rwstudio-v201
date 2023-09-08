dim  fso, FileObj, LogFileName, VerifyObj
set VerifyObj = CreateObject("CSL.RWS1to2Verify")
RENHost.ConnectObject VerifyObj, "VerifyObj"

Sub DoVerify ( strFrom, strTo )

   dim logfile, logfilepath
   set fso = CreateObject("Scripting.FileSystemObject")
   dim base, folder
   base = fso.GetBaseName( strTo )
   folder = fso.GetParentFolderName (strTo )
   LogFileName = folder & "\" & base & ".log"
   set FileObj = fso.CreateTextFile ( LogFileName , True)

   VerifyObj.VerifyConversion strFrom, StrTo

   FileObj.Close
End Sub


sub Error(errMsg)
   'output to MessageLog
   MessageLog.AddMessage 2, errMsg
  
   'save in log file   
   FileObj.writeLine errMsg
End Sub

sub VerifyObj_OnFileMissing (strMissingFile)
   Error "File missing: " & strMissingFile
end sub

sub VerifyObj_OnFileNoElement (strMissingFile)
   Error "No internal XML element for file: " & strMissingFile
End Sub

sub VerifyObj_OnChildLinkMissing(strFile, strchild)
   Error "Child link missing. File = " & strFile & "  Child = "  & strchild
end sub 

sub VerifyObj_OnParentLinkMissing(strFile, strparent)
   Error "Parent link missing. File = " & strFile & "  Parent = "  & strparent
end sub 

sub VerifyObj_OnError(strErrorInfo)
   Error "Unspecified problem at: " & strErrorInfo
end sub

sub VerifyObj_OnVerifyConversionStarted (strFrom, StrTo)

   MessageLog.AddMessage 0,  "Verification of conversion started from " & strFrom & " to " & strTo

   'save in log file   
   FileObj.writeLine errMsg

end sub

sub VerifyObj_OnVerifyConversionComplete(errors, codefailures)
   Dim strMsg
   strMsg = "Verification of conversion complete.  Errors = " & errors 

   if codefailures > 0 then
      strMsg = strMsg & " Problems = " & codefailures
   End If
   MessageLog.AddMessage 0,  strMsg

   'save in log file   
   FileObj.writeLine errMsg

   if errors > 0 or codefailures > 0 then
      MessageLog.AddMessage 0,  "A log of these errors is stored at " & LogFileName
   end if

end sub