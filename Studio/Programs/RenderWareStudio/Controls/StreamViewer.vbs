' 
' Enterprise Layout Management Script 
' 
' Layout object 'StreamViewer' 
' 

' Constants

Option Explicit ' All variables must be explicitly declared

Const rwID_MATERIAL = 7         ' RenderWare Graphics identifier for material object

' Disable tear-offs
StreamPropertyList.AllowTearOffs = False 

' Control behavior of new items added to the property list -
'    true means new edit controls added to the property list are automatically expanded
'    false means they are contracted

StreamPropertyList.ExpandNewItems = True

' --------------------------------------------------------------------

Sub StreamViewer_EditStreamItem ( assetID, identifier, obj_type, data_type, bFirst, bVirtualItem )
  
   if ( bFirst = True ) Then
 
      StreamEditControl.ClearEditControls
   
   End if
   
   if ( assetID <> 0 ) Then

      CSLRenderwareStudioWorkspace.ShowObject StreamPropertyList
      StreamEditControl.ShowEditControl assetID, identifier, obj_type, data_type, bVirtualItem
   
   End if
   
End Sub


' ---------------------------------------------------------------------

Sub StreamViewer_StreamItemAddData ( assetID, identifier, obj_type )  
  
   if ( assetID <> 0 ) Then

      StreamEditControl.AddPluginData assetID, identifier, obj_type
   
   End if
   
End Sub


' ---------------------------------------------------------------------

Sub StreamViewer_StreamItemRemoveData ( assetID, identifier, obj_type, data_type)  
  
   if ( assetID <> 0 ) Then

      StreamEditControl.RemovePluginData assetID, identifier, obj_type, data_type
   
   End if
   
End Sub

' ---------------------------------------------------------------------

Sub StreamViewer_StreamItemSelected ( assetID, identifier, obj_type, data_type, bVirtualItem )   
 
    ' If we're in 'material pick' mode
    if DesignView.RWSUtility.HighlightMaterials Then

        ' Check we have a valid asset, and the object type is 'rwID_MATERIAL'       
        if (AssetID <> 0) And ( obj_type = rwID_MATERIAL ) Then
            ' Highlight this material
            DesignView.RWSUtility.HighlightMaterial assetID, identifier
        else
            ' Clear the currently highlighted material
            DesignView.RWSUtility.HighlightMaterial 0, ""    
        End if
    
    End If

End Sub

' ---------------------------------------------------------------------

Sub StreamViewer_DuplicatedObjIdentifier(assetID, identifier, obj_type, objTypeName)

    ' Make sure the user sees the warning message
    CSLRenderwareStudioWorkspace.ShowObject MessageLog
    MessageLog.Warning "[StreamViewer] Stream contains multiple objects (of type '"& objTypeName &"') with the same RWSIdentifier ('" & identifier & "') - this may lead to unexpected behavior"

End Sub


