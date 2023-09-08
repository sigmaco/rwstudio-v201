'
' Enterprise Layout Management Script
'
' Layout object 'Search'
'

Option Explicit ' All variables must be explicitly declared (With Dim)

' ------------------------------------------------------------------------------
Sub SearchButton_Click (  )
    Searches.SearchForText SearchText.Text, WholeWordOnly.Value, MatchCase.Value
    Search.StopModal True
End Sub

' ------------------------------------------------------------------------------
Sub CancelButton_Click (  )
    Search.StopModal False
End Sub
