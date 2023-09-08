' ------------------------------------------------------------------------------   
' Constants defined so JScript can make use of VBScript MsgBox

Const conVbOKOnly            = 0    ' Display OK button only. 
Const conVbOKCancel          = 1    ' Display OK and Cancel buttons. 
Const conVbAbortRetryIgnore  = 2    ' Display Abort, Retry, and Ignore buttons. 
Const conVbYesNoCancel       = 3    ' Display Yes, No, and Cancel buttons. 
Const conVbYesNo             = 4    ' Display Yes and No buttons. 
Const conVbRetryCancel       = 5    ' Display Retry and Cancel buttons. 
Const conVbCritical          = 16   ' Display Critical Message icon.  
Const conVbQuestion          = 32   ' Display Warning Query icon. 
Const conVbExclamation       = 48   ' Display Warning Message icon. 
Const conVbInformation       = 64   ' Display Information Message icon. 
Const conVbDefaultButton1    = 0    ' First button is default. 
Const conVbDefaultButton2    = 256  ' Second button is default. 
Const conVbDefaultButton3    = 512  ' Third button is default. 
Const conVbDefaultButton4    = 768  ' Fourth button is default. 
Const conVbApplicationModal  = 0    ' Application modal; the user must respond to the message box before continuing work in the current application. 
Const conVbSystemModal       = 4096 ' System modal; all applications are suspended until the user responds to the message box. 

Const conVbOK     = 1 ' OK 
Const conVbCancel = 2 ' Cancel 
Const conVbAbort  = 3 ' Abort 
Const conVbRetry  = 4 ' Retry 
Const conVbIgnore = 5 ' Ignore 
Const conVbYes    = 6 ' Yes 
Const conVbNo     = 7 ' No 


' ------------------------------------------------------------------------------   
' Simple function to provide VBScript MsgBox in JScript
Function ShowMsgBox ( sMsg, iStyle, sTitle )
   ShowMsgBox = MsgBox( sMsg, iStyle, sTitle )
End function
