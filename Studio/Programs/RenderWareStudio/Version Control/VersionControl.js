/////////////////////////////////////////////////////////////////////////
// VersionControl.js
//
// Script methods to provide Version Control access.
/////////////////////////////////////////////////////////////////////////

// Object types as specified within RWSScript type library
otGame                 = 1;
otEntity               = 2;
otFolder               = 4;
otAsset                = 8;
otAssetFolder          = 16;
otTemplate             = 32;
otTemplateFolder       = 64;
otBehavior             = 128;
otBehaviorFolder       = 256;
otCommand              = 512;
otAttribute            = 1024;
otAttributeShare       = 2048;
otAttributeShareFolder = 4096;
otEvent                = 8192;

/////////////////////////////////////////////////////////////////////////
// Define persist states
conStateUnknown               = 0;
conStateNotSaved              = 1;
conStateLocal                 = 2;
conStatePendingImport         = 3;
conStateManaged               = 4;
conStateCheckedOutToUser      = 5;
conStateCheckedOutToOtherUser = 6;

/////////////////////////////////////////////////////////////////////////
// Define values for the reason for asking for permission to check out.
conActionCreate    = 0; // An object is being created
conActionDelete    = 1; // An object is being deleted
conActionAdd       = 2; // An object is having a child added
conActionRemove    = 3; // An object is having a child removed
conActionSwap      = 4; // An object is having children swapped
conActionChange    = 5; // An object is being modified
conActionChangeTag = 6; // An object's tag is being modified

/////////////////////////////////////////////////////////////////////////
// Define values to state whether we're interested in the object or its
// resource.
conBIsObject   = true;
conBIsResource = false;

/////////////////////////////////////////////////////////////////////////
// Define array object to hold UIDs of objects created this session.
// If these object still exist on save then the version control implementation
// will be requested to import them such that they become managed.
var oNewAPIObjectsArray = new Array();

/////////////////////////////////////////////////////////////////////////
// Global to flag if system is currently within the OnPermit function
var bInOnPermit = false;

/////////////////////////////////////////////////////////////////////////
// Global to flag if system is importing data to version control, no need
// to reload game after the import.
var bImportingToVersionControl = false;

/////////////////////////////////////////////////////////////////////////
// Returns if version control is installed.
// Returns true if it is installed, false otherwise.
function IsInstalled ()
{
   return VersionControlImpl.IsInstalled ();
}

/////////////////////////////////////////////////////////////////////////
// Returns if the current project is managed.
function IsProjectManaged ()
{
   var oGame = null;
   try
   {
      oGame = RWSScript.Game ();
   }
   catch (e)
   {
      return false;
   }
   
   if (oGame)
   {
      if (GetPersistStatusInformation (oGame) >= conStatePendingImport)
      {
         return true;
      }
   }
   return false;
}

/////////////////////////////////////////////////////////////////////////
// Shows the dialog to offer to get the latest version of the current
// game's database and its' resources.
// Returns True if you ask to show the latest version.
//         False if you cancel the dialog.
function GetLatestVersion ()
{
   if (GetLatest.DoModal (GetLatest.conGetLatest))
   {
      // Get the latest of the game database if required.
      if (GetLatest.ProjectSelected () && GetLatest.ProjectName.Text != "")
      {
         // First the .rwstudio file
         GetLatestPath (GetLatest.ProjectName.Text);
         // Then the rest of the database
         var sRoot     = Persist.fso.GetParentFolderName (GetLatest.ProjectName.Text);
         var sGameName = Persist.fso.GetBaseName (GetLatest.ProjectName.Text);
         GetLatestPath (Persist.fso.BuildPath (sRoot, sGameName));
      }

      // Get the latest of the resources if required.
      if (GetLatest.ResourceRootSelected () && GetLatest.ResourceRoot.Text != "")
         GetLatestPath (GetLatest.ResourceRoot.Text);

      // Reload the resources if we're not getting the latest of the game database.
      if (GetLatest.ResourceRootSelected () && !GetLatest.ProjectSelected ())
      {
         AssetLister.ReloadResources;
         DesignView.RWSUtility.ReloadResources;
      }

      GlobalScript.UpdateUndoUserInterface ();
      return true;
   }

   // We may want to make the return value based on events caught in the
   // NXNScriptExtension script. e.g. if we had the latest, no need for a reload.
   return false;
}

/////////////////////////////////////////////////////////////////////////
//
function IsPathManaged (strPath)
{
    return VersionControlImpl.IsPathManaged (strPath);
}

/////////////////////////////////////////////////////////////////////////
// Calculates if the path or any one of its parent's paths are managed.
// Returns true if the path or one of its parents is managed,
//         false otherwise.
function IsPathOrParentPathManaged (strPath)
{
   if (!strPath)
      return false;

   if (IsPathManaged (strPath))
      return true;

   // Recursively call back into here with it's parent.
   var strParentPath = Persist.fso.GetParentFolderName (strPath);
   if (strParentPath)
   {
      if (IsPathOrParentPathManaged (strParentPath))
         return true;
   }

   return false;
}

/////////////////////////////////////////////////////////////////////////
// Calculates if the path or any one of its parent's paths are managed.
// Returns the deapest path that is managed, "" otherwise.
function WhichParentPathIsManaged (strPath)
{
   if (!strPath)
      return "";

   if (IsPathManaged (strPath))
      return strPath;

   // Recursively call back into here with it's parent.
   var strParentPath = Persist.fso.GetParentFolderName (strPath);
   if (strParentPath)
   {
      var strManagedPath = WhichParentPathIsManaged (strParentPath);
      if (strManagedPath)
         return strManagedPath;
   }

   return "";
}

/////////////////////////////////////////////////////////////////////////
//
function ShowProjectInExplorer ()
{
   VersionControlImpl.ShowDatabaseExplorer ()
}

//******************************************************************************
// Status information for files and API objects/IDs
//******************************************************************************

/////////////////////////////////////////////////////////////////////////
// Refreshes the version control state for every object in a game.
function RefreshManagedState (oProgressMeter, ManagedObjects, bIsObjectOrResource)
{
   if (ManagedObjects)
   {
      var nCount = ManagedObjects.Count;
      // enumerate over the objects getting the status
      var eObjects, oObject;
      eObjects = new Enumerator (ManagedObjects);
      for (; !eObjects.atEnd(); eObjects.moveNext())
      {
         var oObject = eObjects.item();
         if (bIsObjectOrResource == conBIsObject)
         {
            GetPersistStatusInformation (oObject.ID);
         }
         else // bIsObjectOrResource == conBIsResource
         {
            GetResourceStatusInformation (oObject.ID);
         }
         
         if (oProgressMeter)
         {
            try
            {
               oProgressMeter.StepIt ();
            }
            catch (e)
            {
            }
         }
      }
   }
}

/////////////////////////////////////////////////////////////////////////
// Set current persist state as property on ID
function GetPersistStatusInformation (ID)
{
   var PersistState = conStateUnknown;
   if (IsInstalled && ID)
   {
      // Ignore any attribute IDs
      if (RWSScript.IDType (ID) != Database.otAttribute)
      {
         var oAPIObject = Database.GetAPIObject (ID);
         if (oAPIObject)
         {
            PersistState = GetStatusInformation (oAPIObject, conBIsObject);
         }
         else
         {
            MessageLog.Error ("Failed to get RWSObject for ID, " + ID);
         }
      }
   }

   return PersistState;
}

////////////////////////////////////////////////////////////////////////
// Refreshes the version control state for every object in a game.
function RefreshAllObjectsManagedState ()
{
   // Clear the state cache before we try to get the new states.
   // Clear the cache on the game's folder.
   var sRoot     = Persist.fso.GetParentFolderName (Persist.strCurrentFilename);
   var sGameName = Persist.fso.GetBaseName (Persist.strCurrentFilename);
   VersionControlImpl.FlushFolderProperties (Persist.fso.BuildPath (sRoot, sGameName));
   // Clear the properties on the resources folder.
   VersionControlImpl.FlushFolderProperties (GlobalScript.g_strResourceRoot);

   // Show the progress meter.
   var oProgressMeter = new ActiveXObject("CSL.RWSUtils.ProgressMeter")
   oProgressMeter.ShowProgressMeter ("Refreshing status", 0, 1, true, 0);
   
   // Get all of the relevant objects.
   var ManagedGame            = Database.GetObjectsOfType (Database.otGame);
   var ManagedFolders         = Database.GetObjectsOfType (Database.otFolder);
   var ManagedAssetFolders    = Database.GetObjectsOfType (Database.otAssetFolder);
   var ManagedTemplateFolders = Database.GetObjectsOfType (Database.otTemplateFolder);
   var ManagedASFolders       = Database.GetObjectsOfType (Database.otAttributeShareFolder);
   var ManagedEntities        = Database.GetObjectsOfType (Database.otEntity);
   var ManagedTemplates       = Database.GetObjectsOfType (Database.otTemplate);
   var ManagedAttShares       = Database.GetObjectsOfType (Database.otAttributeShare);
   var ManagedAssets          = Database.GetObjectsOfType (Database.otAsset);

   // Count the number of objects and set the progress meter's range.
   var nObjectsCount = ManagedGame.Count + ManagedFolders.Count + ManagedAssetFolders.Count + 
                       ManagedTemplateFolders.Count + ManagedASFolders.Count + ManagedEntities.Count +
                       ManagedTemplates.Count + ManagedAttShares.Count + (ManagedAssets.Count * 2);
   oProgressMeter.SetRange (1, nObjectsCount);

   // Get the Persist state for each object in the collections.  
   RefreshManagedState (oProgressMeter, ManagedGame,            conBIsObject); 
   RefreshManagedState (oProgressMeter, ManagedFolders,         conBIsObject);
   RefreshManagedState (oProgressMeter, ManagedAssetFolders,    conBIsObject);
   RefreshManagedState (oProgressMeter, ManagedTemplateFolders, conBIsObject);
   RefreshManagedState (oProgressMeter, ManagedASFolders,       conBIsObject);
   RefreshManagedState (oProgressMeter, ManagedEntities,        conBIsObject);
   RefreshManagedState (oProgressMeter, ManagedTemplates,       conBIsObject);
   RefreshManagedState (oProgressMeter, ManagedAttShares,       conBIsObject);
   RefreshManagedState (oProgressMeter, ManagedAssets,          conBIsObject);
   RefreshManagedState (oProgressMeter, ManagedAssets,          conBIsResource);

   oProgressMeter.HideProgressMeter ();
   oProgressMeter = null;
}

/////////////////////////////////////////////////////////////////////////
// Set current persist state as property on ID
function GetResourceStatusInformation (ID)
{
   var PersistState = conStateUnknown;
   if (IsInstalled && ID)
   {
      // Only consider getting resource info for assets.
      if (RWSScript.IDType (ID) == Database.otAsset)
      {
         var oAssetObject = Database.GetAPIObject (ID);
         if (oAssetObject)
         {
            PersistState = GetStatusInformation (oAssetObject, conBIsResource);
         }
         else
         {
            MessageLog.Error ("Failed to get RWS Asset Object for ID, " + ID);
         }
      }
   }

   return PersistState;
}

/////////////////////////////////////////////////////////////////////////
// Set current persist state as property on ID
function GetStatusInformation (oAPIObject, bObjectOrResource)
{
   var PersistState = conStateUnknown;
   if (oAPIObject)
   {
      var sLocalPath = "";
      if (bObjectOrResource == conBIsObject)
      {
         sLocalPath = oAPIObject.PersistentFilename;
      }
      else // bObjectOrResource == conBIsResource
      {
         sLocalPath = Persist.fso.BuildPath (GlobalScript.g_strResourceRoot, oAPIObject.File);
      }

      PersistState = VersionControlImpl.GetPersistStatusInformation (sLocalPath, oAPIObject.ID, bObjectOrResource);
   }

   return PersistState;
}

/////////////////////////////////////////////////////////////////////////
// Get current persist state of a file. Returns one of the conStates listed 
// at the head of this file.
function GetFileStatus (strFilename)
{
   return VersionControlImpl.GetFileStatus (strFilename);
}

/////////////////////////////////////////////////////////////////////////
// Get current string containing file version in the form "12/25".
function GetFileVersionString (strFilename)
{
   return VersionControlImpl.GetFileVersionString (strFilename);
}

/////////////////////////////////////////////////////////////////////////
// Get string containing list of users who have file checked out
// in the form "Fred|Charlie|George".
function GetFileUsersString (strFilename)
{
   return VersionControlImpl.GetFileUsersString (strFilename);
}

//******************************************************************************
// File based version control operations
//******************************************************************************

/////////////////////////////////////////////////////////////////////////
// Check out a file
function CheckOutFile (strFilename, bOnlyIfLatest, bShowDialog)
{
   var bSucceeded = VersionControlImpl.CheckOut (strFilename, bOnlyIfLatest, bShowDialog);
   if (!bSucceeded)
   {
      OutputFilenameMessage ("Check out failed", strFilename, MessageLog.conWarning);
   }
   return bSucceeded;
}

/////////////////////////////////////////////////////////////////////////
// Undo pending changes for a file
function UndoPendingChangesForFile (strFilename)
{
   var bSucceeded = VersionControlImpl.UndoPendingChanges (strFilename);
   if (!bSucceeded)
   {
      OutputFilenameMessage ("Undo of pending changes failed", strFilename, MessageLog.conWarning);
   }
   return bSucceeded;
}

/////////////////////////////////////////////////////////////////////////
// Submit pending changes for a file
function SubmitPendingChangesForFile (strFilename)
{
   var bSucceeded = VersionControlImpl.SubmitPendingChanges (strFilename);
   if (!bSucceeded)
   {
      OutputFilenameMessage ("Submit of pending changes failed", strFilename, MessageLog.conWarning);
   }
   return bSucceeded;
}

/////////////////////////////////////////////////////////////////////////
// Import a file to version control
function ImportFile (strFilename)
{
   var bSucceeded = VersionControlImpl.Import (strFilename, false);
   if (!bSucceeded)
   {
      OutputFilenameMessage ("Import of file to alienbrain failed", strFilename, MessageLog.conWarning);
   }
   return bSucceeded;
}

/////////////////////////////////////////////////////////////////////////
// Import a folder to version control
function ImportFolder (strFolderName, bRecursivelyAddChildren)
{
   var bSucceeded = VersionControlImpl.Import (strFolderName, bRecursivelyAddChildren);
   if (!bSucceeded)
   {
      OutputFilenameMessage ("Import of folder to alienbrain failed", strFolderName, MessageLog.conWarning);
   }
   return bSucceeded;
}

/////////////////////////////////////////////////////////////////////////
// Imports the current project into version control.
// Adds the current project's files into the default changeset and offers
// to submit it. Any other files in the default changeset would be
// submitted too, so warn the user before adding them.
function ImportProjectToVersionControl ()
{
   var nCheckedOutFilesBefore = NumFilesInDefaultChangeSet ();
   if (GetLatest.DoModal (GetLatest.conImportProject))
   {
      // If the import project checkbox was checked, import the database files into version control.
      if (GetLatest.ProjectSelected ())
      {
         if (nCheckedOutFilesBefore > 0)
         {
            if (VBMsgBox.conVbNo == VBMsgBox.ShowMsgBox (
               "Your default changeset already has files, these will be imported with the current project. Is this ok?",
               VBMsgBox.conVbYesNo + VBMsgBox.conVbQuestion, GlobalScript.g_strTitle))
            {
               return;
            }
         }

         ImportProjectDatabaseToVersionControl ();
      }

      // If the import resources checkbox was checked, import the resource files into version control.
      if (GetLatest.ResourceRootSelected())
      {
         ImportResourceRootToVersionControl ();
      }

      // If any files have been added to the default changeset, offer to submit them.
      var nCheckedOutFilesAfter = NumFilesInDefaultChangeSet ();
      if (nCheckedOutFilesAfter != nCheckedOutFilesBefore)
      {
         if (VBMsgBox.conVbYes == VBMsgBox.ShowMsgBox (
               "Files have been added to the default changeset, do you want to submit the pending changes?",
               VBMsgBox.conVbYesNo + VBMsgBox.conVbQuestion, GlobalScript.g_strTitle))
         {
            bImportingToVersionControl = true;
            SubmitDefaultChangeSet ();
         }
      }

      GlobalScript.UpdateVersionControlMenu ();         
   }
}

/////////////////////////////////////////////////////////////////////////
// Imports the current project database files into version control.
// Adds the current project's files into the default changeset and offers
// to submit it. Any other files in the default changeset would be
// submitted too, so warn the user before adding them.
function ImportProjectDatabaseToVersionControl ()
{
   if (Persist.strCurrentFilename.length)
   {
      // Either the default changeset hasn't got any files in or the user has said it's ok to
      // import them. Add the game's files to the default changeset.
      // Import the game folder, not recursively.
      ImportFolder (Persist.strCurrentFolder, false);
      ImportFile (Persist.strCurrentFilename);
      // Import the game's folders recursively adding their children.
      ImportFolder (Persist.fso.BuildPath (Persist.strCurrentFolder, "Assets"), true);
      ImportFolder (Persist.fso.BuildPath (Persist.strCurrentFolder, "AttributeShares"), true);
      ImportFolder (Persist.fso.BuildPath (Persist.strCurrentFolder, "Build Rules"), true);
      ImportFolder (Persist.fso.BuildPath (Persist.strCurrentFolder, "Entities"), true);
      ImportFolder (Persist.fso.BuildPath (Persist.strCurrentFolder, "Folders"), true);
      ImportFolder (Persist.fso.BuildPath (Persist.strCurrentFolder, "Games"), true);
   }
}

/////////////////////////////////////////////////////////////////////////
// Imports the files from the current project's resource root into
// version control.
// Adds the current project's files into the default changeset and offers
// to submit it. Any other files in the default changeset would be
// submitted too, so warn the user before adding them.
function ImportResourceRootToVersionControl ()
{
   ImportFolder (GlobalScript.g_strResourceRoot, true);
}

/////////////////////////////////////////////////////////////////////////
// Get the latest version of a file
function GetLatestVersionOfFile (strFilename)
{
   return GetLatestPath (strFilename);
}

/////////////////////////////////////////////////////////////////////////
// Gets the latest version of the file/folder specified in strPath.
function GetLatestPath (strPath)
{
   var bSucceeded = VersionControlImpl.GetLatestPath (strPath);
   if (!bSucceeded)
   {
      OutputFilenameMessage ("Get latest version failed", strPath, MessageLog.conWarning);
   }
   return bSucceeded;
}

//******************************************************************************
// API object based version control operations
//******************************************************************************

/////////////////////////////////////////////////////////////////////////
// Check out the XML file for an API object
function CheckOutAPIObject (oAPIObject, bOnlyIfLatest, bShowDialog)
{
   if (!Persist.fso.FileExists (oAPIObject.PersistentFilename))
   {
      // If the file doesn't yet exist then return true for check out
      return true;
   }
   
   // Take copy of UID as oAPIObject may be invalidated by call to CheckOut
   var strUID = oAPIObject.UID;
   var bSucceeded = VersionControlImpl.CheckOut (oAPIObject.PersistentFilename, bOnlyIfLatest, bShowDialog);
   
   if (bSucceeded)
   {
      // Obtain new oAPIObject based on UID (since may have been reloaded)
      oAPIObject = Database.GetAPIObjectFromUID (strUID);
      if (oAPIObject != null)
      {
         var PersistState = GetPersistStatusInformation (oAPIObject.ID);
         GlobalScript.UpdateVersionControlMenu ();
         
         // Double check that file got checked out (user may have canceled operation for example)
         if (PersistState != VersionControl.conStateCheckedOutToUser &&
             PersistState != VersionControl.conStatePendingImport)
         {
            bSucceeded = false;
         }
      }
   }
         
   if (!bSucceeded && oAPIObject != null)
   {
      OutputAPIObjectMessage ("Check out failed", oAPIObject, MessageLog.conWarning);
   }
   
   return bSucceeded;
}

/////////////////////////////////////////////////////////////////////////
// Undo the check out for an API object
// Event handling in NXNScriptExtension will perform reload if required.
function UndoPendingChangesForAPIObject (oAPIObject)
{
   // Take copy of UID as oAPIObject may be invalidated by call to UndoPendingChanges
   var strUID = oAPIObject.UID;
   var bSucceeded = VersionControlImpl.UndoPendingChanges (oAPIObject.PersistentFilename);
   
   // Obtain new oAPIObject based on UID since may have been reloaded
   oAPIObject = Database.GetAPIObjectFromUID (strUID);
   if (oAPIObject != null)
   {
      GetPersistStatusInformation (oAPIObject);
      
      if (!bSucceeded)
      {
         OutputAPIObjectMessage ("Undo of pending changes failed", oAPIObject, MessageLog.conWarning);
      }
   }
   GlobalScript.UpdateVersionControlMenu ();
}

/////////////////////////////////////////////////////////////////////////
// Submit the check out for an API object
// Event handling in NXNScriptExtension will perform reload if required.
function SubmitPendingChangesForAPIObject (oAPIObject)
{
   var bSubmit = true;
   var oDirtyTag = RWSScript.RegisterTag (Tags.conDirtyID);
   
   // If the object is currently dirty then ask whether to save before submit
   if (oDirtyTag.Get (oAPIObject.ID))
   {
      var bSaveFirst = VBMsgBox.ShowMsgBox ("Do you wish to save your current changes before submitting?", VBMsgBox.conVbYesNoCancel + VBMsgBox.conVbQuestion, GlobalScript.g_strTitle);
      if (bSaveFirst == VBMsgBox.conVbYes)
      {
         if (!RWSScript.Save (oAPIObject))
         {
            VBMsgBox.ShowMsgBox ("Save failed", VBMsgBox.conVbOk + VBMsgBox.conVbExclamation, "RenderWare Studio");
            bSubmit = false;
         }
         else
         {
            // Have saved this object so clear dirty tag
            oDirtyTag.Set (oAPIObject.ID, 0);
         }
      }
      else if (bSaveFirst == VBMsgBox.conVbCancel)
      {
         bSubmit = false;
      }
   }
   
   if (bSubmit)
   {   
      // Take copy of UID as oAPIObject may be invalidated by call to SubmitPendingChanges
      var strUID = oAPIObject.UID;
      var bSucceeded = VersionControlImpl.SubmitPendingChanges (oAPIObject.PersistentFilename);

      // Obtain new oAPIObject based on UID since may have been reloaded
      oAPIObject = Database.GetAPIObjectFromUID (strUID);
      if (oAPIObject != null)
      {
         // Clear dirty tag so OnPermit events are fired again for this object
         oDirtyTag.Set (oAPIObject.ID, 0);
         GetPersistStatusInformation (oAPIObject);
         
         if (!bSucceeded)
         {
            OutputAPIObjectMessage ("Submission of pending changes failed", oAPIObject, MessageLog.conWarning);
         }
      }
      GlobalScript.UpdateVersionControlMenu ();
   }
}

/////////////////////////////////////////////////////////////////////////
// Called from NXNExtension script when a persistent API object is created.
function OnCreateAPIObject (oAPIObject)
{
   if (VersionControlImpl.IsInstalled ())
   {
      // Ignore non persistent API objects
      if (oAPIObject.IDType != otBehavior && oAPIObject.IDType != otBehaviorFolder &&
         oAPIObject.IDType != otCommand &&  oAPIObject.IDType != otEvent &&
         oAPIObject.UID != "BehaviorHierarchy")
      {
         oNewAPIObjectsArray.push (oAPIObject.UID);
      }
   }
}

/////////////////////////////////////////////////////////////////////////
// Called after a save so that newly persisted objects can be imported to 
// version control.
function ImportNewAPIObjects ()
{
   var bSucceeded = true;
   var oDeletedTag = RWSScript.RegisterTag (Tags.conDeletedID);

   // Create array of filenames to be passed to import routine
   var strObjectUID = "";
   for (var i = 0; i < oNewAPIObjectsArray.length; i++) 
   {
      strObjectUID = oNewAPIObjectsArray[i];
      if (strObjectUID != "")
      {
         // Get the API object for this UID if it still exists
         var oAPIObject = Database.GetAPIObjectFromUID (strObjectUID);
         if (oAPIObject != null)
         {
            if (!oDeletedTag.Get(oAPIObject.ID))
            {
               // Import the objects file
               if ( VersionControlImpl.Import (oAPIObject.PersistentFilename, false))
               {
                  // Get the new state of the object
                  var PersistState = GetPersistStatusInformation (oAPIObject.ID);
                  if (PersistState == VersionControl.conStatePendingImport)
                  {
                     OutputAPIObjectMessage ("Import successful", oAPIObject, MessageLog.conInformation);
                  }
                  else
                  {
                     // Import of the this appears to RWS to have failed
                     OutputAPIObjectMessage ("Import failed", oAPIObject, MessageLog.conWarning);
                     bSuceeded = false;
                  }
               }
               else
               {
                  // Import of the this file failed
                  OutputAPIObjectMessage ("Import failed", oAPIObject, MessageLog.conWarning);
                  bSuceeded = false;
               }
            }
         }
      }
   }
   
   if (!bSucceeded)
   {
      MessageLog.Warning ("[Version Control] Import of some/all new files to alienbrain failed.");
   }
      
   // Empty the array as keeping track of any failed entries is useless with the current implementation
   // (user may exit application for example if save was part of a close)
   while (oNewAPIObjectsArray.length > 0)
      oNewAPIObjectsArray.pop ();
}

/////////////////////////////////////////////////////////////////////////
// Get the latest version of an object's file
// Event handling in NXNScriptExtensions will invoke a load if required.
function GetLatestVersionOfAPIObject (oAPIObject)
{
   var bGetLatest = true;
   var oDirtyTag = RWSScript.RegisterTag (Tags.conDirtyID);
   
   // If the object is currently dirty then ask whether to save before get latest
   if (oDirtyTag.Get (oAPIObject.ID))
   {
      var bSaveFirst = VBMsgBox.ShowMsgBox ("Do you wish to save your current changes before getting latest?", VBMsgBox.conVbYesNoCancel + VBMsgBox.conVbQuestion, GlobalScript.g_strTitle);
      if (bSaveFirst == VBMsgBox.conVbYes)
      {
         if (!RWSScript.Save (oAPIObject))
         {
            VBMsgBox.ShowMsgBox ("Save failed", VBMsgBox.conVbOk + VBMsgBox.conVbExclamation, "RenderWare Studio");
            bGetLatest = false;
         }
         else
         {
            // Have saved this object so clear dirty tag
            oDirtyTag.Set (oAPIObject.ID, 0);
         }
      }
      else if (bSaveFirst == VBMsgBox.conVbCancel)
      {
         bGetLatest = false;
      }
   }
   
   if (bGetLatest)
   {   
      var strUID = oAPIObject.UID;
      if (!GetLatestPath (oAPIObject.PersistentFilename))
      {
         // Obtain new oAPIObject based on UID since may have been reloaded
         oAPIObject = Database.GetAPIObjectFromUID (strUID);
         if (oAPIObject != null)
           OutputAPIObjectMessage ("Get latest version failed", oAPIObject, MessageLog.conWarning);
      }
   }
}

/////////////////////////////////////////////////////////////////////////
// Set the NXN properties on the server file for an object
function SetNXNProperties (oAPIObject)
{
   VersionControlImpl.SetNXNProperties (oAPIObject.PersistentFilename,
                                        oAPIObject.Name,
                                        ContextMenu.GetObjectType (oAPIObject),
                                        oAPIObject.Description);
}

//******************************************************************************
// Change set operations
//******************************************************************************

/////////////////////////////////////////////////////////////////////////
// Function to query the number of checked out files in the default
// change set (where checked out files will be put).
// Returns the number of files in the change set, 0 otherwise.
function NumFilesInDefaultChangeSet ()
{
   return VersionControlImpl.NumFilesInDefaultChangeSet ();
}

/////////////////////////////////////////////////////////////////////////
//
function SubmitDefaultChangeSet ()
{
   if (!VersionControlImpl.SubmitDefaultChangeSet ())
      MessageLog.Warning ("[Version Control] Unable to submit pending changes for default change set.");
      
   if (VersionControlImpl.NumFilesInDefaultChangeSet () > 0)
      MessageLog.Warning ("[Version Control] Not all items in the default change set could be submitted.");
      
   GlobalScript.UpdateVersionControlMenu ();
}

/////////////////////////////////////////////////////////////////////////
//
function UndoDefaultChangeSet ()
{
   if (!VersionControlImpl.UndoDefaultChangeSet ())
      MessageLog.Warning ("[Version Control] Unable to undo pending changes for default change set.");
      
   if (VersionControlImpl.NumFilesInDefaultChangeSet () > 0)
      MessageLog.Warning ("[Version Control] Not all items in the default change set could be undone.");
      
   GlobalScript.UpdateVersionControlMenu ();
}

/////////////////////////////////////////////////////////////////////////
//
function GetDefaultChangeSetName ()
{
    var DefaultChangeSetName = VersionControlImpl.GetDefaultChangeSetName ();
    if (DefaultChangeSetName == "nxnserver_unnamedchangeset")
        DefaultChangeSetName = "- Unnamed -"
    
    return DefaultChangeSetName
}

//******************************************************************************
// Permission handler for editing API objects.
// Responsible for checking out files in project for API objects prior to an edit
//******************************************************************************

/////////////////////////////////////////////////////////////////////////
//
function OnPermit (ID, RefID, Action)
{
   var ret;
   bInOnPermit = true;
   
   if (!IsProjectManaged ())
   {
      bInOnPermit = false;
      return true;
   }
   
	// You can always create an object.
	if (Action == conActionCreate)
	{
      bInOnPermit = false;
		return true;
   }
   
   // Get API objects for passed in IDs
	var oAPIObject = Database.GetAPIObject (ID);
   var oRefObject = Database.GetAPIObject (RefID);
   
	if (!oAPIObject)
	{
	   // We can't get an API object for the given ID, so refuse permission.
      bInOnPermit = false;
	   return false;
	}
	else
	{
	   // Ignore non-persistent object types
      if (oAPIObject.IDType == otBehavior || oAPIObject.IDType == otBehaviorFolder ||
          oAPIObject.IDType == otCommand ||  oAPIObject.IDType == otEvent ||
          oAPIObject.UID == "BehaviorHierarchy")
      {
         bInOnPermit = false;
   		return true;
      }

      // If objects are already checked out to user then allow edit      
      if (GetPersistStatusInformation (oAPIObject) == conStateCheckedOutToUser)
      {
         // Check if both oAPIObject and oRefObject would require check out
         if ( oRefObject &&
              oRefObject.IDType != Database.otAttribute &&
             (conActionAdd == Action || conActionRemove == Action) )
         {
            if (GetPersistStatusInformation (oRefObject) == conStateCheckedOutToUser)
            {
               // Both objects are already checked out so permit edit
               bInOnPermit = false;
   		      return true;
   		   }
         }
         else  // Only oAPIObject would require check out
         {
            // Object is already checked out so permit edit
            bInOnPermit = false;
   		   return true;
         }
      }

      if (conActionChangeTag == Action)
      {
         // Only try to check out the file if the change of tag requires us to.
         var DeletedTagID        = RWSScript.RegisterTag (Tags.conDeletedID);
         var TemplateEntityTagID = RWSScript.RegisterTag (Tags.conTemplateEntity);
         var GlobalFolderTagID   = RWSScript.RegisterTag (Tags.conGlobalFolder);
         if (RefID == DeletedTagID || RefID == TemplateEntityTagID || RefID == GlobalFolderTagID)
         {
            ret = CheckOutAPIObject (oAPIObject, true, false);
            bInOnPermit = false;
            return ret;
         }
      }
      else if (conActionDelete == Action)
      {
         // Check for object deletion by actual delete (rare event)
         ret = CheckOutAPIObject (oAPIObject, true, false);
         bInOnPermit = false;
         return ret;
      }
      else // modifying an existing ID
      {
         if (CheckOutAPIObject (oAPIObject, true, false))
         {
            // If we're affecting another non-attribute object, check it out.
            if (conActionAdd == Action || conActionRemove == Action)
            {
               if (oRefObject.IDType != Database.otAttribute)
               {
	               if (oRefObject)
	               {
	                  ret = CheckOutAPIObject (oRefObject, true);
                     bInOnPermit = false;
	                  return ret;
                  }
                  else
                  {	
                     // We can't get an API object for the given ID, so refuse permission.
                     bInOnPermit = false;
                     return false;
                  }
               }
            }
         }
         else // CheckOutAPIObject failed 
         {
            bInOnPermit = false;
            return false;
         }
      }
	}
	
   bInOnPermit = false;
	return true;
}

//******************************************************************************
// User feedback
//******************************************************************************

/////////////////////////////////////////////////////////////////////////
// This is called from VersionControlImpl and NXNScriptExtension
// It writes a filename based message to the verison control log
// and, if the file is an API object or under the resource root, a
// message to the main message log.
function OutputToMessageLogs (strMessage, strFilename, nMessageSeverity)
{
   // Write filename based message to version control message log
   VersionControlLog.AddMessage (nMessageSeverity, strMessage + " '" + strFilename + "'.");
   
   // See if file relates to a loaded API object, if so output message
   var bIsAPIObject = false;
   var strUID = Persist.GetUIDFromFilename (strFilename);
   if (strUID != "")
   {
      var oAPIObject = Database.GetAPIObjectFromUID (strUID);
      if (oAPIObject)
      {
         OutputAPIObjectMessage (strMessage, oAPIObject, nMessageSeverity);
         bIsAPIObject = true;
      }
   }
   
   if (!bIsAPIObject)
   {
      // Check if file is below resource root, 
      // if so put message out to main message log
      if (strFilename.substr(0,  GlobalScript.g_strResourceRoot.length) == GlobalScript.g_strResourceRoot)
         OutputFilenameMessage (strMessage, strFilename, nMessageSeverity);
   }
}

/////////////////////////////////////////////////////////////////////////
// Write a message to the main message log regarding an API object
function OutputAPIObjectMessage (strMessage, oAPIObject, nMessageSeverity)
{
   var strFullMessage = "[Version Control] " + strMessage + " for ";

   strFullMessage += Database.GetTypeString (oAPIObject);
   strFullMessage += " object '";
   strFullMessage += oAPIObject.Name;
   strFullMessage += "', "
   strFullMessage += oAPIObject.UID;
   
   // Output the built-up message.
   MessageLog.AddMessage (nMessageSeverity, strFullMessage);   
}

/////////////////////////////////////////////////////////////////////////
// Write a message to the main message log regarding a file
function OutputFilenameMessage (strMessage, strFilename, nMessageSeverity)
{
   var strFullMessage = "[Version Control] " + strMessage + " for '" + strFilename + "'.";
   
   // Output the built-up message.
   MessageLog.AddMessage (nMessageSeverity, strFullMessage);   
}
