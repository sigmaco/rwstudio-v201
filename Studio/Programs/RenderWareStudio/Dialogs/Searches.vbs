'
' Enterprise Layout Management Script
'
' Layout object 'Searches'
'

Option Explicit ' All variables must be explicitly declared

Dim SearchScript
Set SearchScript = CreateObject ("CSL.RWSScript.RWSScript")

' ------------------------------------------------------------------------------
' Search the game for entities using whatever is currently selected
' (in ContextMenu.CurrentContext).
' If LevelOnly is True, only entities in the active folder are considered.
' If MatchType is True, only the types of the objects must be the same.
' DoSearch is called from ContextMenu in response to one of the Search menu items.
' strSearchSummary contains a description of the search query. This function
' should also append the type of the object in ContextMenu.CurrentContext and
' a list of the objects in it.
Sub DoSearch (LevelOnly, MatchType, strSearchSummary)
   Dim ResultsContainer
   Set ResultsContainer = CreateObject("CSL.RWSScript.RWSContainer")

   ' Create and show the search progress meter.
   Dim oProgressMeter
   Set oProgressMeter = CreateObject ("CSL.RWSUtils.ProgressMeter")
   If Not oProgressMeter Is Nothing Then
      oProgressMeter.ShowProgressMeter SearchTitle (0), 0, 1, true, 0
   End If

   ' Initialize variables used during the search.
   Dim clnEntities, nNumSearched, bCancelled
   Set clnEntities = SearchScript.Entities
   nNumSearched    = 0
   bCancelled      = False

   ' Now we can set the exact range of progress.
   If Not oProgressMeter Is Nothing Then
      On Error Resume Next
      oProgressMeter.SetRange 0, clnEntities.count + 1
      If Not Err.Number = 0 Then
         bCancelled = True
      End If
      On Error Goto 0
   End If

   ' Build up the description string to include the type and match details.
   AddSearchDetailsToDescription strSearchSummary, MatchType

   ' Search entities for the selected items
   If Not bCancelled Then
      Dim LookForThisEntity, Entity
      For Each Entity In SearchScript.Entities
         If LevelOnly Then
            'Check entity is within current level
            LookForThisEntity = FindEntityInActiveFolder (Entity)
         Else
            'Look for all entities
            LookForThisEntity = true
         End If

         If LookForThisEntity Then
            If SearchEntity (Entity, MatchType) Then
               ' An entity has been found, add it to the container and update the
               ' progress meter's title to reflect the number of found entities.
               ResultsContainer.Add Entity
               oProgressMeter.Title = SearchTitle (ResultsContainer.count)
            End If
         End If

         ' Step the progress meter, if the user cancelled the search, stop searching.
         If Not StepProgessMeter (oProgressMeter, nNumSearched) Then
            bCancelled = True
            Exit For
         End If
      Next
   End If

   ' Show the search details in the message log and show the search control if required.
   ShowSearchResults strSearchSummary, bCancelled, nNumSearched, clnEntities.count, ResultsContainer
End Sub

' ------------------------------------------------------------------------------
' Search the game's active folder for the given entity
Function FindEntityInActiveFolder (Entity)
   FindEntityInActiveFolder = FindEntityInFolder (Entity, SearchScript.Game.ActiveFolder)
End Function

' ------------------------------------------------------------------------------
' Search the given folder for the given entity
Function FindEntityInFolder (Entity, Folder)
   FindEntityInFolder = false

   'Check for the entity in this folder
   Dim ChildEntity
   For Each ChildEntity In Folder.ChildEntities
      If ChildEntity = Entity Then
         FindEntityInFolder = true
         Exit Function
      End If
   Next

   'Recurse through child folders
   Dim ChildFolder
   For Each ChildFolder In Folder.ChildFolders
      If FindEntityInFolder (Entity, ChildFolder) Then
         FindEntityInFolder = true
         Exit Function
      End If
   Next
End Function

' ------------------------------------------------------------------------------
' Search an entity for the items selected
Function SearchEntity (Entity, MatchType)
   SearchEntity = false

   Select Case ContextMenu.GetFocusObject (ContextMenu.CurrentContext).IDType
   Case ContextMenu.otAsset
      SearchEntity = SearchEntityForAssets (Entity, MatchType)
   Case ContextMenu.otBehavior
      SearchEntity = SearchEntityForBehaviors (Entity) 'Behaviors do not have types
   End Select
End Function

' ------------------------------------------------------------------------------
' Search the given entity for the selected assets (in ContextMenu.CurrentContext)
Function SearchEntityForAssets (Entity, MatchType)
   SearchEntityForAssets = false

   If SearchAssetContainer (Entity.ChildAssets, MatchType) Then
      SearchEntityForAssets = true
   Else
      SearchEntityForAssets = SearchFolderContainerForAssets (Entity.ChildAssetFolders, MatchType)
   End If
End Function

' ------------------------------------------------------------------------------
' Recurse through the given folder container in search of the selected assets
Function SearchFolderContainerForAssets (AssetFolderContainer, MatchType)
   SearchFolderContainerForAssets = false

   Dim AssetFolder
   For Each AssetFolder In AssetFolderContainer
      If SearchAssetContainer (AssetFolder.ChildAssets, MatchType) Then
         SearchFolderContainerForAssets = true
         Exit Function
      End If

      If SearchFolderContainerForAssets (AssetFolder.ChildAssetFolders, MatchType) Then
         SearchFolderContainerForAssets = true
         Exit Function
      End If
   Next
End Function

' ------------------------------------------------------------------------------
' Search the given asset container for the selected assets
Function SearchAssetContainer (AssetContainer, MatchType)
   SearchAssetContainer = false

   Dim Asset
   For Each Asset In AssetContainer
      If IsAssetSelected (Asset, MatchType) Then
         SearchAssetContainer = true
         Exit Function
      End If
   Next
End Function

' ------------------------------------------------------------------------------
' Check if the given asset is one which is selected
Function IsAssetSelected (Asset, MatchType)
   IsAssetSelected = false

   Dim Container
   For Each Container In ContextMenu.CurrentContext
      If MatchType Then
         If SearchScript.Asset(Container(1)).Type = Asset.Type Then
            IsAssetSelected = true
            Exit Function
         End If
      ElseIf Container(1) = Asset Then
         IsAssetSelected = true
         Exit Function
      End If
   Next
End Function

' ------------------------------------------------------------------------------
' Search the given entity for the selected behaviors (in ContextMenu.CurrentContext)
Function SearchEntityForBehaviors (Entity)
   SearchEntityForBehaviors = false

   'Find behavior from UID
   Dim Behavior
   Set Behavior = SearchScript.Behavior (Entity.Behavior)

   SearchEntityForBehaviors = SearchBehavior (Behavior)
End Function

' ------------------------------------------------------------------------------
' Check if the given behavior (or one of its child behaviors) is one which is selected
Function SearchBehavior (Behavior)
   'Only one behavior can be selected currently
   'Handle more anyway...
   If Behavior Is Nothing Then
      SearchBehavior = False
   Else
      Dim Container
      For Each Container In ContextMenu.CurrentContext
         If Container(1) = Behavior Then
            SearchBehavior = true
            Exit Function
         End If
      Next

      'Recurse child behaviors
      Dim ChildBehavior
      For Each ChildBehavior In Behavior.ChildBehaviors
         If SearchBehavior (ChildBehavior) Then
            SearchBehavior = true
            Exit Function
         End If
      Next
   End If
End Function

' ------------------------------------------------------------------------------
' Search the game for objects whose name contains the given text.
' If WholeWordOnly is True, the name must completely match Text.
' If CaseSensitive is True, the text cases must match.
' SearchForText is called from the Search dialog
Function SearchForText (Text, WholeWordOnly, CaseSensitive)
   Dim NumToSearch, nNumSearched, bCancelled
   NumToSearch  = 0
   nNumSearched = 0
   bCancelled   = False

   ' Create the container to hold the results.
   Dim ResultsContainer
   Set ResultsContainer = CreateObject ("CSL.RWSScript.RWSContainer")

   ' Create the search progress meter.
   Dim oProgressMeter
   Set oProgressMeter = CreateObject ("CSL.RWSUtils.ProgressMeter")
   If Not oProgressMeter Is Nothing Then
      oProgressMeter.ShowProgressMeter SearchTitle (0), 0, 1, true, 0
   End If

   ' Create the container objects that we're going to search over.
   Dim clnFolders, clnEntities, clnAssetFolders, clnAssets, clnTemplateFolders
   Dim clnTemplates, clnBehaviorFolders, clnBehaviors
   Set clnFolders         = SearchScript.Folders
   Set clnEntities        = SearchScript.Entities
   Set clnAssetFolders    = SearchScript.AssetFolders
   Set clnAssets          = SearchScript.Assets
   Set clnTemplateFolders = SearchScript.TemplateFolders
   Set clnTemplates       = SearchScript.Templates
   Set clnBehaviorFolders = SearchScript.BehaviorFolders
   Set clnBehaviors       = SearchScript.Behaviors

   ' Set the actual number of items to search on the progress meter.
   NumToSearch = clnFolders.count + clnEntities.count + clnAssetFolders.count + _
                 clnAssets.count + clnTemplateFolders.count + clnTemplates.count + _
                 clnBehaviorFolders.count + clnBehaviors.count
   If Not oProgressMeter Is Nothing Then
      On Error Resume Next
      oProgressMeter.SetRange 0, NumToSearch + 1
      If Not Err.Number = 0 Then
         bCancelled = True
      End If
      On Error Goto 0
   End If

   Dim Comparison
   If CaseSensitive Then
      Comparison = 0 'Binary comparison
   Else
      Comparison = 1 'Textual comparison
   End If

   If Not bCancelled Then
      Dim Folder
      For Each Folder In clnFolders
         If FindText (Folder.Name, Text, WholeWordOnly, Comparison) Then
            ResultsContainer.Add Folder
            oProgressMeter.Title = SearchTitle (ResultsContainer.count)
         End If
         If Not StepProgessMeter (oProgressMeter, nNumSearched) Then
            bCancelled = True
            Exit For
         End If
      Next
   End If

   If Not bCancelled Then
      Dim Entity
      For Each Entity In clnEntities
         If FindText (Entity.Name, Text, WholeWordOnly, Comparison) Then
            ResultsContainer.Add Entity
            oProgressMeter.Title = SearchTitle (ResultsContainer.count)
         End If
         If Not StepProgessMeter (oProgressMeter, nNumSearched) Then
            bCancelled = True
            Exit For
         End If
      Next
   End If

   If Not bCancelled Then
      Dim AssetFolder
      For Each AssetFolder In clnAssetFolders
         If FindText (AssetFolder.Name, Text, WholeWordOnly, Comparison) Then
            ResultsContainer.Add AssetFolder
            oProgressMeter.Title = SearchTitle (ResultsContainer.count)
         End If
         If Not StepProgessMeter (oProgressMeter, nNumSearched) Then
            bCancelled = True
            Exit For
         End If
      Next
   End If

   If Not bCancelled Then
      Dim Asset
      For Each Asset In clnAssets
         If FindText (Asset.Name, Text, WholeWordOnly, Comparison) Then
            ResultsContainer.Add Asset
            oProgressMeter.Title = SearchTitle (ResultsContainer.count)
         End If
         If Not StepProgessMeter (oProgressMeter, nNumSearched) Then
            bCancelled = True
            Exit For
         End If
      Next
   End If

   If Not bCancelled Then
      Dim TemplateFolder
      For Each TemplateFolder In clnTemplateFolders
         If FindText (TemplateFolder.Name, Text, WholeWordOnly, Comparison) Then
            ResultsContainer.Add TemplateFolder
            oProgressMeter.Title = SearchTitle (ResultsContainer.count)
         End If
         If Not StepProgessMeter (oProgressMeter, nNumSearched) Then
            bCancelled = True
            Exit For
         End If
      Next
   End If

   If Not bCancelled Then
      Dim Template
      For Each Template In clnTemplates
         If FindText (Template.Name, Text, WholeWordOnly, Comparison) Then
            ResultsContainer.Add Template
            oProgressMeter.Title = SearchTitle (ResultsContainer.count)
         End If
         If Not StepProgessMeter (oProgressMeter, nNumSearched) Then
            bCancelled = True
            Exit For
         End If
      Next
   End If

   If Not bCancelled Then
      Dim BehaviorFolder
      For Each BehaviorFolder In clnBehaviorFolders
         If FindText (BehaviorFolder.Name, Text, WholeWordOnly, Comparison) Then
            ResultsContainer.Add BehaviorFolder
            oProgressMeter.Title = SearchTitle (ResultsContainer.count)
         End If
         If Not StepProgessMeter (oProgressMeter, nNumSearched) Then
            bCancelled = True
            Exit For
         End If
      Next
   End If

   If Not bCancelled Then
      Dim Behavior
      For Each Behavior In clnBehaviors
         If FindText (Behavior.UID, Text, WholeWordOnly, Comparison) Then
            ResultsContainer.Add Behavior
            oProgressMeter.Title = SearchTitle (ResultsContainer.count)
         End If
         If Not StepProgessMeter (oProgressMeter, nNumSearched) Then
            bCancelled = True
            Exit For
         End If
      Next
   End If

   ' Now show the results.
   ShowSearchResults "Find '" & Text & "'", bCancelled, nNumSearched, NumToSearch, ResultsContainer

   SearchForText = (ResultsContainer.count > 0)
End Function

' ------------------------------------------------------------------------------
' Determine if the given text contains the given search string
Function FindText (Text, SearchFor, WholeWordOnly, Comparison)
   FindText = false

   If WholeWordOnly Then
      If Len (Text) = Len (SearchFor) Then
         If Instr (1, Text, SearchFor, Comparison) = 1 Then
            FindText = true
         End If
      End If
   Else
      If Instr (1, Text, SearchFor, Comparison) > 0 Then
         FindText = true
      End If
   End If
End Function

' ------------------------------------------------------------------------------
' Returns the title for the search progress meter.
Function SearchTitle (nNumFound)
   SearchTitle = "Searching (" & nNumFound & " found) ..."
End Function

' ------------------------------------------------------------------------------
' Adds the details of the search to the description.
Sub AddSearchDetailsToDescription (strSearchSummary, MatchType)
   Dim strDetails, arrDetails (), nCount, Container, nArrayCount, bFound
   ReDim arrDetails (-1)
   nCount = 0
   Select Case ContextMenu.GetFocusObject (ContextMenu.CurrentContext).IDType
   Case ContextMenu.otAsset
      Dim strType
      For Each Container In ContextMenu.CurrentContext
         nCount = nCount + 1
         If MatchType Then
            ' Ensure that each type is only added to the description once.
            strType      = SearchScript.Asset(Container(1)).Type
            bFound = False
            For nArrayCount = LBound (arrDetails) To UBound (arrDetails)
               If strType = arrDetails (nArrayCount) Then
                  bFound = True
                  Exit For
               End If
            Next
            ' If the type hasn't been found yet, add it to the array and to the description.
            If Not bFound Then
               ReDim Preserve arrDetails (UBound (arrDetails) + 1)
               arrDetails (UBound (arrDetails)) = strType
               strDetails = strDetails & strType & ", "
            End If
         Else
            strDetails = strDetails & Container(1).Name
            If ContextMenu.CurrentContext.SelectionCount > nCount Then
               strDetails = strDetails & ", "
            Else
               strDetails = strDetails & " "
            End If
         End If
      Next
      If MatchType Then
         ' Remove the comma space, append a space
         strDetails = Left (strDetails, Len (strDetails) - 2) & " "
         If UBound (arrDetails) - LBound (arrDetails) + 1 = 1 Then
            strSearchSummary = strSearchSummary & "asset type " & strDetails
         Else
            strSearchSummary = strSearchSummary & "asset types: " & strDetails
         End If
      Else
         If ContextMenu.CurrentContext.SelectionCount = 1 Then
            strSearchSummary = strSearchSummary & "asset " & strDetails
         Else
            strSearchSummary = strSearchSummary & "assets: " & strDetails
         End If
      End If
   Case ContextMenu.otBehavior
      ' Only 1 behavior can be selected at once, add it into the description.
      strSearchSummary = strSearchSummary & "behavior " & _
                         ContextMenu.GetFocusObject (ContextMenu.CurrentContext).UID
   End Select
End Sub

' ------------------------------------------------------------------------------
' Completes the description string with details of if the search was cancelled,
' how many objects were searched, and how many were found.
' If there were some results, the search results control is populated and shown.
Sub ShowSearchResults (strSearchSummary, bCancelled, nNumSearched,_
                       nTotalToSearch, ResultsContainer)
   ' Build up a results string to write to the output log.
   Dim strResults
   strResults = "[Search] " & strSearchSummary
   If bCancelled Then
      strResults = strResults & "- Cancelled (" & nNumSearched & "/"
   Else
      strResults = strResults & "- Completed ("
   End If
   strResults = strResults & nTotalToSearch & " items searched) - " & ResultsContainer.count
   If ResultsContainer.count = 1 Then
      strResults = strResults & " match found"
   Else
      strResults = strResults & " matches found"
   End If
   MessageLog.Log strResults

   ' If there were some items found, show the search results window.
   If ResultsContainer.count > 0 Then
      CSLRenderwareStudioWorkspace.ShowObject SearchResults
      SearchResults.PopulateContainer ResultsContainer
   End If
End Sub

' ------------------------------------------------------------------------------
' Will Step the progress meter, catching any errors.
' Returns False if the progress meter returned an error code (this should cancel
'         the search), True otherwise.
Function StepProgessMeter (oProgressMeter, nNumSearched)
   StepProgessMeter = True
   nNumSearched = nNumSearched + 1
   If Not oProgressMeter Is Nothing Then
      On Error Resume Next
      oProgressMeter.StepIt
      If Not Err.Number = 0 Then
         StepProgessMeter = False
      End If
      On Error Goto 0
   End If
End Function
