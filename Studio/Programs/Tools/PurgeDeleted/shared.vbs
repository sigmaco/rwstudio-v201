Option Explicit

Public Const conActionReport = 1, conActionDelete = 2, conActionBackup = 4, conActionShowProgress = 8, conForceDeleteIfReadOnly = True

Public Const conBackupFolderName = "Deleted Backup"

Public Const conCalledViaScript = "via script"

Public Const conEnvVarProject = "PURGEDELETEDPROJECT"

Public Const conEnvVarBackup = "PURGEDELETEDBACKUP"

Dim fso
Set fso = CreateObject("Scripting.FileSystemObject")

Dim sstrFolderNames(5)
sstrFolderNames(0) = "Games"
sstrFolderNames(1) = "Folders"
sstrFolderNames(2) = "Entities"
sstrFolderNames(3) = "Assets"
sstrFolderNames(4) = "AttributeShares"

' Create a folder tree
Sub CreateFolder(strFolderPath)
	Dim arrFolders, strNewFolderPath, x
	If Not fso.FolderExists(strFolderPath) Then
		arrFolders = split(strFolderPath, "\")
		strNewFolderPath = fso.BuildPath(arrFolders(0), "\")
		For x = 1 To UBound(arrFolders)
			strNewFolderPath = fso.BuildPath(strNewFolderPath, arrFolders(x))
			If Not fso.FolderExists(strNewFolderPath) Then
				fso.CreateFolder strNewFolderPath
			End If
		Next
	End If 
End Sub

' Copy file, creating destination folders when necessary
Sub CopyFile(strSourceFilePath, strDestFilePath)
	Dim strDestFolderPath
	strDestFolderPath = fso.GetParentFolderName(strDestFilePath)
	If Not fso.FolderExists(strDestFolderPath) Then
		CreateFolder(strDestFolderPath)
	End If
	fso.CopyFile strSourceFilePath, strDestFilePath, conForceDeleteIfReadOnly
End Sub

Function GetBackupFilePath(strFilePath, strBackupFolderPath)
	Dim strProjectFolderPath, strFileRelativePath
	strProjectFolderPath = fso.GetParentFolderName(fso.GetParentFolderName(strFilePath))
	strFileRelativePath = Right(strFilePath, Len(strFilePath) - Len(strProjectFolderPath))
	GetBackupFilePath = fso.BuildPath(strBackupFolderPath, strFileRelativePath)
End Function

Function GetDefaultBackupFolderPath(strProjectFilePath)
	Dim strProjectFolderPath
	strProjectFolderPath = fso.BuildPath(fso.GetParentFolderName(strProjectFilePath), fso.GetBaseName(strProjectFilePath))
	GetDefaultBackupFolderPath = fso.BuildPath(strProjectFolderPath, conBackupFolderName)
End Function

Function GetProjectFilePath
	With CreateObject("CSL.RENCommonDialog")
		.Filter = "RenderWare Studio Project (*.rwstudio)|*.rwstudio|" &_ 
			   "All Files (*.*)|*.*|"
		.MaxFileSize = 260
		.Flags = .OFN_HIDEREADONLY + .OFN_FILEMUSTEXIST
		.DialogTitle = "Select RenderWare Studio Project"
		.ShowOpen()
		GetProjectFilePath = .Filename
	End With
End Function

' Steps the progress meter, catching any errors.
' Returns False if the progress meter returned an error code, True otherwise.
Function StepProgressMeter(objProgressMeter)
	StepProgressMeter = True
	If Not objProgressMeter Is Nothing Then
		On Error Resume Next
		objProgressMeter.StepIt
		If Not (Err.Number = 0) Then
			StepProgressMeter = False
		End If
		On Error Goto 0
	End If
End Function

' Returns -1 if project file does not exist, or if aborted partway through
' Otherwise, returns number of deleted objects found
Function ProcessDeletedObjects(strProjectFilePath, strBackupFolderPath, intAction)

	Dim strProjectFolderPath, intDeletedObjectsFoundInDatabase, intDeletedObjectsFoundInFolder, file, folder, objXMLDoc, objProgressMeter, intFiles, blnAborted, strFolderPath, strFolderName, x
	
	ProcessDeletedObjects = -1
	
	If fso.FileExists(strProjectFilePath) Then

		LogHTML "<h1>List of <code>&lt;Deleted/&gt;</code> files in " & fso.GetBaseName(strProjectFilePath) & " game database</h1>"
		
		LogHTML "<p><strong>Project file:</strong> <code><a target=""viewer"" href=""" & strProjectFilePath & """>" & strProjectFilePath & "</a></code>"
		
		blnAborted = False
		
		' Build path of project folder from path of project file
		strProjectFolderPath = fso.BuildPath(fso.GetParentFolderName(strProjectFilePath), fso.GetBaseName(strProjectFilePath))
		
		' Start progress meter
		If (intAction And conActionShowProgress) Then
			Set objProgressMeter = CreateObject("CSL.RWSUtils.ProgressMeter")
			If Not objProgressMeter Is Nothing Then
				' Use dummy upper range limit of one
				objProgressMeter.ShowProgressMeter "Scanning database for deleted objects", 0, 1, True, 0
			End If
			' Get total number of files
			' Note: Didn't use "For Each strFolderName In sstrFolderNames" because it
			' loops one more than number of array items
			For x = 0 To UBound(sstrFolderNames) - 1
				strFolderName = sstrFolderNames(x)
				strFolderPath = fso.BuildPath(strProjectFolderPath, strFolderName)
				intFiles = intFiles + fso.GetFolder(strFolderPath).Files.Count
			Next
			' Set upper range of progress meter to total number of files + 1
			If Not objProgressMeter Is Nothing Then
				On Error Resume Next
				objProgressMeter.SetRange 0, intFiles + 1
				If Not Err.Number = 0 Then
					blnAborted = True
				End If
				On Error Goto 0
			End If
		End If
	
		' Create XML document used to test game database files
		Set objXMLDoc = CreateObject("MSXML2.DomDocument")
		
		intDeletedObjectsFoundInDatabase = 0
		
		' Loop through folders under project folder
		For x = 0 To UBound(sstrFolderNames) - 1
			strFolderName = sstrFolderNames(x)
			strFolderPath = fso.BuildPath(strProjectFolderPath, strFolderName)
			intDeletedObjectsFoundInFolder = 0
			' Loop through files
			LogHTML "<h2 class=""folder"">" & strFolderName & "\</h2>"
			For Each file in fso.GetFolder(strFolderPath).Files
				' Is file XML?
				If objXMLDoc.Load(file.Path) Then
					' Does file contain <Deleted> element?
					' (Would use selectSingleNode, but due to a bug in MSXML,
					' it does not return Null when there is no <Deleted> element)
					' We will use "deleted" ID later to actually delete these files
					If objXMLDoc.selectNodes("//Deleted").length Then
						LogHTML "<pre class=""filename""><a id=""deleted"" target=""viewer"" href=""" & file.Path & """>" & file.Name & "</a></pre>"
						If (intAction And conActionBackup) Then
							CopyFile file.Path, GetBackupFilePath(file.Path, strBackupFolderPath)
						End If
						If (intAction And conActionDelete) Then
							' Delete file, even if it is read-only
							file.Delete conForceDeleteIfReadOnly
						End If
						
						intDeletedObjectsFoundInFolder = intDeletedObjectsFoundInFolder + 1
						intDeletedObjectsFoundInDatabase = intDeletedObjectsFoundInDatabase + 1
					End If
				End If
				If (intAction And conActionShowProgress) Then
					' Has the user clicked the Abort button on the progress meter?
					blnAborted = Not(StepProgressMeter(objProgressMeter))
				End If
				If blnAborted Then
					Exit For
				End If
			Next
			If blnAborted Then
				Exit For
			End If
			LogHTML "<p>" & intDeletedObjectsFoundInFolder & " <code>&lt;Deleted/&gt;</code> files found in " & strFolderName & " folder.</p>"
		Next
		
		If Not(blnAborted) Then
		
			LogHTML "<h2>Summary</h2>"
		
			LogHTML "<p>" & intDeletedObjectsFoundInDatabase & " <code>&lt;Deleted/&gt;</code> files found in database.</p>"
			
			ProcessDeletedObjects = True
			
		End If
		
	Else
	
		LogHTML "<p class=""error"">Project file not found:</p>"
		LogHTML "<pre class=""filename"">" & strProjectFilePath & "</pre>"
		
	End If
	
	ProcessDeletedObjects = intDeletedObjectsFoundInDatabase
	
End Function
