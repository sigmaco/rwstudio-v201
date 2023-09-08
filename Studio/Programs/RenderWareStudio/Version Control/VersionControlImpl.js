/////////////////////////////////////////////////////////////////////////
// VersionControlImpl.js
//
// Script methods implementing version control access with NxN
// alienbrain. Replace this script with appropriate functions to
// access other version control systems.
/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
// For alienbrain commands called using the RunCommand routine the 
// success of the operation is indicated by the relevant event being 
// recieved by the NXNScriptExtension script from the NXN namespace.
// Failure is indicated by the WasSuccessful flag being 'false'.
// Note that the user pressing 'Cancel' in an NXN dialog will result
// in WasSuccessful being 'true' but no events being fired.
//
// All routines in this implementation, which are called from
// other scripts, should return 'true' or 'false'.
// If returning false then it is up to the calling routine to provide
// user feedback to say that the operation failed. If returning true then 
// the calling routine should not provide user feedback as this should be 
// handled by the event mechanism in NXNScriptExtension.vbs.
/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
// Create global variables used in this script.
var bAlienbrainInstalled = true;

/////////////////////////////////////////////////////////////////////////
// Verbosity level, set using: oXMLParam.VerboseLevel = level;
var conVerboseNothing       = 0;
var conVerboseErrors        = 1;
var conVerboseWarnings      = 2;
var conVerboseNotifications = 3;
var conVerboseOkMessages    = 4;

/////////////////////////////////////////////////////////////////////////
// Constants for use with NxNNamespace.Flush command
var NXN_NAMESPACE_FLUSH_OBJECTS    = 0;
var NXN_NAMESPACE_FLUSH_PROPERTIES = 2;
var NXN_NAMESPACE_FLUSH_ICONS      = 4;

/////////////////////////////////////////////////////////////////////////
// Global objects used in this script.
// The NxNNamespace object - equivalent to "document.write(tAPP_CreateNamespaceObject())" in IE script
var oNxNNamespace;
// The XMLParam object - equivalent to "var oXMLParam = tAPP_GetNxNXMLParamObject();" in IE script
var oXMLParam;

try 
{
   oNxNNamespace = new ActiveXObject("NxNNamespace.NxNNamespaceHelper");
   oXMLParam     = new ActiveXObject("NxNXMLHelper.NxNXMLParameter");
}
catch (e)
{
   bAlienbrainInstalled = false;
}

/////////////////////////////////////////////////////////////////////////
// Initialization dependant on other objects being loaded.
function OnLoad ()
{
   if (bAlienbrainInstalled)
   {
      VersionControlLog.Log ("NXN alienbrain Version: " + oNxNNamespace.GetProperty ("//", "NamespaceVersion"));
      // Check if our script extension object was created successfully.
      if (!NXNScriptExtension.oNXNScriptExtension)
      {
         // This usually happens if the user pressed "Work offline"
         VersionControlLog.Log ("You are working offline. Projects will not be managed.");
         MessageLog.Warning ("[Version Control] You are working offline. Projects will not be managed.");
         bAlienbrainInstalled = false;
         GlobalScript.UpdateVersionControlMenu ();
      }
   }
   else
   {
      VersionControlLog.Log ("alienbrain not installed");
      MessageLog.Warning ("[Version Control] alienbrain not installed");
   }
}

/////////////////////////////////////////////////////////////////////////
//
function IsInstalled ()
{
   return bAlienbrainInstalled;
}

/////////////////////////////////////////////////////////////////////////
//
function NumFilesInDefaultChangeSet ()
{
   if (bAlienbrainInstalled)
   {
      var oChangeSet = GetDefaultChangeSetObject ();
      if (oChangeSet)
      {
         var oChildren = oChangeSet.Children;
         if (oChildren.length)
         {
            return oChildren.length;
         }
      }
   }
   return 0;
}

/////////////////////////////////////////////////////////////////////////
// Submit the change set. All files referenced in the change set will be
// submitted.
function SubmitDefaultChangeSet ()
{
   if (bAlienbrainInstalled)
   {
      var oChangeSet = GetDefaultChangeSetObject ();
      if (oChangeSet)
      {
         // Build up and run the submit command.
         oXMLParam.Reset();
         oXMLParam.Command                = "SubmitPendingChange";
         oXMLParam.VerboseLevel           = conVerboseErrors;
         oXMLParam.XML                    = oChangeSet.RunCommand (oXMLParam.Command, oXMLParam.XML);

         if (oXMLParam.WasSuccessful)
            return true;
         else
            VersionControlLog.Error ("Submit pending changes failed for default change set.");
      }
   }
   return false;
}

/////////////////////////////////////////////////////////////////////////
// Revert the changes to a file.
function UndoPendingChanges (strFilename)
{
   if (bAlienbrainInstalled)
   {
      // Map the local file name to the namespace and get object
      var strNamespacePath = MapManagedPath (strFilename);
      var oObject = GetNamespaceObject (strNamespacePath);
      if (oObject)
      {
         oXMLParam.Reset();
         oXMLParam.Command      = "UndoPendingChange";
         oXMLParam.VerboseLevel = conVerboseErrors;
         oXMLParam.XML          = oObject.RunCommand (oXMLParam.Command, oXMLParam.XML);
         
         if (oXMLParam.WasSuccessful)
            return true;
         else // Note, due to NXN bug WasSuccessful may be 0 even if the operation succeeded.
            VersionControlLog.Error ("Undo pending changes failed for '" + strFilename + "', with server path '" + strNamespacePath + "'.");
      }
   }
   return false;
}

/////////////////////////////////////////////////////////////////////////
// Submit the changes to a files.
function SubmitPendingChanges (strFilename)
{
   if (bAlienbrainInstalled)
   {
      // Map the local file name to the namespace and get object
      var strNamespacePath = MapManagedPath (strFilename);
      var oObject = GetNamespaceObject (strNamespacePath);
      if (oObject)
      {    
         oXMLParam.Reset();
         oXMLParam.Command      = "SubmitPendingChange";
         oXMLParam.VerboseLevel = conVerboseErrors;
         oXMLParam.XML          = oObject.RunCommand (oXMLParam.Command, oXMLParam.XML);
         
         if (oXMLParam.WasSuccessful)
            return true;
         else
            VersionControlLog.Error ("Failed to submit pending changes for default change set.");
      }
   }
   return false;   
}

/////////////////////////////////////////////////////////////////////////
// Revert the changes to all files in the default change set.
function UndoDefaultChangeSet ()
{
   if (bAlienbrainInstalled)
   {
      var oChangeSet = GetDefaultChangeSetObject ();
      if (oChangeSet)
      {
         // Build up and run the undo command.
         oXMLParam.Reset();
         oXMLParam.Command      = "UndoPendingChange";
         oXMLParam.VerboseLevel = conVerboseErrors;
         oXMLParam.XML          = oChangeSet.RunCommand (oXMLParam.Command, oXMLParam.XML);

         if (oXMLParam.WasSuccessful)
            return true;
         else
            VersionControlLog.Error ("Failed to undo pending changes for default change set.");
      }
   }
   return false;
}

/////////////////////////////////////////////////////////////////////////
// Checkout a file
function CheckOut (strFilename, bOnlyIfLatest, bShowDialog)
{
   if (bAlienbrainInstalled)
   {
      // Map the local file name to the namespace
      var strNamespacePath = MapManagedPath (strFilename);
      if (strNamespacePath != "")
      {
         if (bOnlyIfLatest)
         {
            // Check that the user has the latest version, if not then checkout is denied
            var sHeadRevision = oNxNNamespace.GetProperty (strNamespacePath, "NxN_VersionNumber");
            var strLocalVersion = oNxNNamespace.GetProperty (strNamespacePath, "NxNWorkspace_LocalVersionNumber");
            if (sHeadRevision != strLocalVersion)
            {
               VersionControl.OutputToMessageLogs ("Check out denied since not latest version", strFilename, MessageLog.conWarning);
               return false;
            }
         }

         // Check out the file.
         var oObject = GetNamespaceObject(strNamespacePath);
         oXMLParam.Reset();
         oXMLParam.Command = "CheckOut";
         if (bShowDialog)
         {
            oXMLParam.ParamIn ("ShowMainDialog") = "1";
         }
         else
         {
            oXMLParam.HideResponseDialog ("MultipleCheckOut" , "1");
            oXMLParam.ParamIn ("ShowMainDialog") = "0";
            oXMLParam.SetProgressDialog ("", 1);
         }
         oXMLParam.VerboseLevel = conVerboseErrors;
         oXMLParam.XML = oObject.RunCommand (oXMLParam.Command, oXMLParam.XML, true);
         
         if (oXMLParam.WasSuccessful)
            return true;
         else
            VersionControlLog.Error ("Failed to check out '" + strFilename + "', with server path '" + strNamespacePath + "'.");
      }
   }
   return false;
}

/////////////////////////////////////////////////////////////////////////
// Show the NxN database explorer
function ShowDatabaseExplorer ()
{
   if (bAlienbrainInstalled)
   {
      NXNScriptExtension.oNXNScriptExtension.ShowDatabaseExplorer ("");
      return true;
   }
   return false;
}

/////////////////////////////////////////////////////////////////////////
// Show the NxN history dialog
function ShowFileHistoryDialog (strFilename)
{
   if (bAlienbrainInstalled)
   {
      NXNScriptExtension.oNXNScriptExtension.ShowFileHistoryDialog (strFilename);
      return true;
   }
   return false;
}

/////////////////////////////////////////////////////////////////////////
// Show the NxN properties dialog
function ShowFilePropertiesDialog (strFilename)
{
   if (bAlienbrainInstalled)
   {
      NXNScriptExtension.oNXNScriptExtension.ShowFilePropertiesDialog (strFilename);
      return true;
   }
   return false;
}

/////////////////////////////////////////////////////////////////////////
// Flush the properties of a folder recursively
function FlushFolderProperties (strPath)
{
   if (bAlienbrainInstalled)
   {
      var strNamespacePath = NXNScriptExtension.oNXNScriptExtension.MapManagedPath (strPath);
      oNxNNamespace.Flush (strNamespacePath, true, NXN_NAMESPACE_FLUSH_PROPERTIES, "");
   }
}

/////////////////////////////////////////////////////////////////////////
// Returns if the path is under a managed location.
function IsPathManaged (strPath)
{
   if (bAlienbrainInstalled)
   {
      // Map the local file name to the namespace and check its type
      var strNamespacePath = MapManagedPath (strPath);
      try
      {
         var strNodeType = oNxNNamespace.GetProperty (strNamespacePath, "NamespaceType");
      }
      catch (e)
      {
         return false;
      }
      
      if (strNodeType.substr(0, 17) == "\\Workspace\\DbItem")
         return true;
   }
   return false;
}

/////////////////////////////////////////////////////////////////////////
// Helper functions
/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
// Get the alienbrain namespace path for the given local path
function MapManagedPath (strLocalPath)
{
   var strNamespacePath = "";
   
   try 
   {
      strNamespacePath = NXNScriptExtension.oNXNScriptExtension.MapManagedPath (strLocalPath);
   }
   catch (e)
   {
      strNamespacePath = "";
   }

   return strNamespacePath;
}

/////////////////////////////////////////////////////////////////////////
// Get the alienbrain namespace object for the namespace path
function GetNamespaceObject (strNamespacePath)
{
   var oObject = null;
   
   if (strNamespacePath != "")
   {
      oObject = oNxNNamespace.GetNamespaceObj (strNamespacePath);
      if (oObject == null)
      {
         VersionControlLog.Error ("Failed to get alienbrain client object for server path '" + strNamespacePath + "'.");
      }
   }
   return oObject;
}

/////////////////////////////////////////////////////////////////////////
//
function GetProjectPath ()
{
   var strNamespacePath = MapManagedPath (Persist.strCurrentFilename);
   if (strNamespacePath != "")
   {
      // Namespace path for a file is always of the form "\Workspace\<project_name>\..."
      // so simply return first two parts of the path for the project path.
      var sFoldersArray = strNamespacePath.split("\\");
      if (sFoldersArray.length > 0)
      {
         return "\\" + sFoldersArray[1] + "\\" + sFoldersArray[2] + "\\";
      }
   }
   return "";
}

/////////////////////////////////////////////////////////////////////////
//
function GetDefaultChangeSetPath ()
{
   var sCommand     = "NxNChangeSet_GetDefault";
   
   var sProjectPath = GetProjectPath ();
   var oObject      = GetNamespaceObject (sProjectPath);
   if (oObject)
   {  
      oXMLParam.Reset();
      oXMLParam.Command = sCommand;        
      oXMLParam.XML     = oObject.RunCommand( sCommand, oXMLParam.XML ); 
      var sChangeSetHandle  = oXMLParam.ParamOut("Handle");
     
      oXMLParam.Reset();
      oXMLParam.Command = "GetPathFromHandle";
      oXMLParam.ParamIn("NamespacePath")  = sProjectPath;       
      sChangeSetPath = oObject.RunCommand( oXMLParam.Command, sChangeSetHandle );
      
      if (sChangeSetPath != "")
      {
         sChangeSetPath = "\\Workspace\\" + sChangeSetPath;
      }
      else
      {
         VersionControlLog.Error ("Unable to get default change set for project '" + sProjectPath +"'.");
      }
      return sChangeSetPath;
   }
   
   return "";
}

/////////////////////////////////////////////////////////////////////////
//
function GetDefaultChangeSetObject ()
{
   var sChangeSetPath = GetDefaultChangeSetPath ()
   if (sChangeSetPath != "")
   {  
      var oChangeSet = GetNamespaceObject (sChangeSetPath);
      return oChangeSet;
   }
   return null;
}

/////////////////////////////////////////////////////////////////////////
//
function GetDefaultChangeSetName ()
{
   var sChangeSetPath = GetDefaultChangeSetPath ()
   if (sChangeSetPath != "")
   {
      var nSize = sChangeSetPath.length;
      if (nSize)
      {
         // If the string ends with \ remove it.
         var sEndChar = sChangeSetPath.substr (nSize-1, 1);
         if (sEndChar == "/")
            sChangeSetPath = sChangeSetPath.substr (0, nSize-1);
         if (sEndChar == "\\")
            sChangeSetPath = sChangeSetPath.substr (0, nSize-1);

         return Persist.GetBaseName (sChangeSetPath);
      }
   }
   return "";
}

/////////////////////////////////////////////////////////////////////////
//
function GetPersistStatusInformation (strLocalPath, ID, bObjectOrAsset)
{
   var PersistState = VersionControl.conStateUnknown;
   if (bAlienbrainInstalled)
   {
      // Remove any folder browsing, i.e. .  ./  ../
      strLocalPath = Persist.fso.GetAbsolutePathName (strLocalPath)

      if (!Persist.fso.FileExists (strLocalPath))
      {
         PersistState = VersionControl.conStateNotSaved;
      }
      else
      {
         PersistState = VersionControl.conStateLocal;

         if (IsPathManaged (strLocalPath))
         {
            // Map the local file name to the namespace and get object
            var strNamespacePath = MapManagedPath (strLocalPath);
            if (strNamespacePath != "")
            {
               PersistState = VersionControl.conStateManaged;

               // Flush the properties on the file node
               oNxNNamespace.Flush (strNamespacePath, true, NXN_NAMESPACE_FLUSH_PROPERTIES, "");
               
               // Get local and server version numbers as string in the form of "13/25"
               var sVersion = oNxNNamespace.GetProperty (strNamespacePath, "NxNWorkspace_VersionNumber_Gui");
               if (sVersion.substr (0, 2) == "?/")
               {
                  VersionControlLog.Error ("Failed to get current file version for '" + strLocalPath + "'");
                  MessageLog.Error ("[Version Control] Invalid file version reported by alienbrain. Some version control operations may not function correctly.");
               }

               // Get the current user name for the file and name of user(s) who has it checked out
               var sUserName = oNxNNamespace.GetProperty (strNamespacePath, "UserName");
               var sCheckedOutTo = oNxNNamespace.GetProperty (strNamespacePath, "Locked By_All");
               if (sCheckedOutTo)
               {
                  // See if checked out to this or other user
                  var sPattern = "(^|\|)" + sUserName + "(\||$)";
                  var oReg = new RegExp (sPattern);
                  if (sCheckedOutTo.search (oReg) != -1)  // Search for user name in list of users
                     PersistState = VersionControl.conStateCheckedOutToUser;
                  else
                     PersistState = VersionControl.conStateCheckedOutToOtherUser;
               }
               
               var ChangeType = oNxNNamespace.GetProperty (strNamespacePath, "NxNServer_ChangeSetPendingChangeType");
               if (ChangeType == 1 /*Import flag*/)
               {
                  PersistState = VersionControl.conStatePendingImport;
               }
            }
         }

         // Set new persist state property on the object, different property for
         // object or resource.
         var oProperty;
         if (bObjectOrAsset == VersionControl.conBIsObject)
         {
            oProperty = RWSScript.Property (Properties.conVersionControlStatus);
         }
         else // bObjectOrAsset == VersionControl.conBIsResource
         {
            oProperty = RWSScript.Property (Properties.conResourceStatus);
         }

         var oPropertyData = new ActiveXObject ("CSL.RWSScript.RWSData");
         // Add data section for persist state
         var oDataSection = oPropertyData.Add (Properties.conDTUInt32);
         oDataSection.Add (PersistState); // Add current file status
         // Add data section for version string
         oDataSection = oPropertyData.Add (Properties.conDTChar);
         oDataSection.Add (sVersion);     // Add version string

         if (PersistState >= VersionControl.conStateCheckedOutToUser)
         {
            // Add data sections for each user name
            sPattern = "(^|\|)[A-Za-z0-9_]+(\||$)";
            oReg = new RegExp (sPattern, "g");
            var results = sCheckedOutTo.match (oReg);
            if (results != null)
            {
               for (i=0; i<results.length; i++)
               {
                  oDataSection = oPropertyData.Add (Properties.conDTChar);
                  oDataSection.Add (results[i]);   // Add a user name
               }
            }
         }

         oProperty.Set (ID, oPropertyData, 1, 0); // SystemFlags=1 => don't save
      }
   }
   
   return PersistState;
}

/////////////////////////////////////////////////////////////////////////
//
function GetFileStatus (strLocalPath)
{
   var PersistState = VersionControl.conStateUnknown;
   if (bAlienbrainInstalled)
   {
      // Remove any folder browsing, i.e. .  ./  ../
      strLocalPath = Persist.fso.GetAbsolutePathName (strLocalPath)

      if (!Persist.fso.FileExists (strLocalPath))
      {
         PersistState = VersionControl.conStateNotSaved;
      }
      else
      {
         PersistState = VersionControl.conStateLocal;

         if (IsPathManaged (strLocalPath))
         {
            // Map the local file name to the namespace and get object
            var strNamespacePath = MapManagedPath (strLocalPath);
            if (strNamespacePath != "")
            {
               PersistState = VersionControl.conStateManaged;

               // Flush the properties on the file node
               oNxNNamespace.Flush (strNamespacePath, true, NXN_NAMESPACE_FLUSH_PROPERTIES, "");
               
               // Get the current user name for the file and name of user(s) who has it checked out
               var sUserName = oNxNNamespace.GetProperty (strNamespacePath, "UserName");
               var sCheckedOutTo = oNxNNamespace.GetProperty (strNamespacePath, "Locked By_All");
               if (sCheckedOutTo)
               {
                  // See if checked out to this or other user
                  var sPattern = "(^|\|)" + sUserName + "(\||$)";
                  var oReg = new RegExp (sPattern);
                  if (sCheckedOutTo.search (oReg) != -1)  // Search for user name in list of users
                     PersistState = VersionControl.conStateCheckedOutToUser;
                  else
                     PersistState = VersionControl.conStateCheckedOutToOtherUser;
               }
               
               var ChangeType = oNxNNamespace.GetProperty (strNamespacePath, "NxNServer_ChangeSetPendingChangeType");
               if (ChangeType == 1 /*Import flag*/)
               {
                  PersistState = VersionControl.conStatePendingImport;
               }
            }
         }
      }
   }
   
   return PersistState;
}

/////////////////////////////////////////////////////////////////////////
//
function GetFileVersionString (strLocalPath)
{
   var strVersion = "Unknown";
   
   if (bAlienbrainInstalled)
   {
      // Remove any folder browsing, i.e. .  ./  ../
      strLocalPath = Persist.fso.GetAbsolutePathName (strLocalPath)

      // Map the local file name to the namespace and get object
      var strNamespacePath = MapManagedPath (strLocalPath);
      if (strNamespacePath != "")
      {
         // Flush the properties on the file node
         oNxNNamespace.Flush (strNamespacePath, true, NXN_NAMESPACE_FLUSH_PROPERTIES, "");
         // Get local and server version numbers as string in the form of "13/25"
         strVersion = oNxNNamespace.GetProperty (strNamespacePath, "NxNWorkspace_VersionNumber_Gui");
      }
   }
   return strVersion;
}

/////////////////////////////////////////////////////////////////////////
//
function GetFileUsersString (strLocalPath)
{
   var strUsersString = "";
   
   if (bAlienbrainInstalled)
   {
      // Remove any folder browsing, i.e. .  ./  ../
      strLocalPath = Persist.fso.GetAbsolutePathName (strLocalPath)

      // Map the local file name to the namespace and get object
      var strNamespacePath = MapManagedPath (strLocalPath);
      if (strNamespacePath != "")
      {
         // Flush the properties on the file node
         oNxNNamespace.Flush (strNamespacePath, true, NXN_NAMESPACE_FLUSH_PROPERTIES, "");
         // Get list of users who have this file checked out
         strUsersString = oNxNNamespace.GetProperty (strNamespacePath, "Locked By_All");
      }
   }
   return strUsersString;
}

/////////////////////////////////////////////////////////////////////////
// Gets the latest version from alienbrain of the file/folder specified
// in strPath.
function GetLatestPath (strPath)
{
   if (bAlienbrainInstalled)
   {
      // Map the local file name to the namespace
      var strNamespacePath = MapManagedPath (strPath);
      var oObject = GetNamespaceObject(strNamespacePath);
      if (oObject)
      {
         oXMLParam.Reset();
         oXMLParam.Command = "GetLatest";
         oXMLParam.ParamIn ("OverwriteWritable")   = "1"; // Skip writable files (without asking).
         oXMLParam.ParamIn ("ShowMainDialog")      = "0"; // Hide the main dialog.
         oXMLParam.VerboseLevel                    = conVerboseErrors;
         oXMLParam.XML                             = oObject.RunCommand (oXMLParam.Command, oXMLParam.XML, true);
         
         if (oXMLParam.WasSuccessful)
            return true;
         else
            VersionControlLog.Error ("Get latest version failed for '" + strPath + "', with server path '" + strNamespacePath + "'.");
      }
   }
   return false;
}

/////////////////////////////////////////////////////////////////////////
// Import a file or folder by adding an "add" entry to the current change set
function Import (strFilename, bRecursivelyAddChildren)
{
   if (bAlienbrainInstalled)
   {
      // find out if we need to create any parent folders before we can import this object.
      // (If the parent isn't managed, but one of it's parents is)
      var strPathToMap        = strFilename;
      var strParentPath       = Persist.fso.GetParentFolderName (strFilename);
      var strCreateFolderPath = ""
      if (!VersionControl.IsPathManaged (strParentPath) &&
            VersionControl.IsPathOrParentPathManaged (strParentPath))
      {
         var strManagedPath  = VersionControl.WhichParentPathIsManaged (strFilename);
         strCreateFolderPath = strParentPath.substr (strManagedPath.length+1, strParentPath.length-1);
         strPathToMap        = strManagedPath;
      }

      // Map the local file name to the namespace
      var strNamespacePath = MapManagedPath (strPathToMap);
      if (strNamespacePath != "")
      {
         // Insert the new disk item into the namespace cache (to avoid object flush)
         var strParentPath = Persist.fso.GetParentFolderName (strFilename);
         var strParentNamespacePath = NXNScriptExtension.oNXNScriptExtension.MapManagedPath (strParentPath);
         var strName = Persist.fso.GetFileName (strFilename);
         oNxNNamespace.RunCommand (strParentNamespacePath, "InsertDiskItem", strName, false);
        
         var oObject = GetNamespaceObject (strNamespacePath);
         if (oObject)
         {
            oXMLParam.Reset ();
            oXMLParam.Command                          = "Import";
            oXMLParam.ParamIn ("LocalPath")            = strFilename;
            oXMLParam.ParamIn ("AutoCreateSubFolders") = strCreateFolderPath;
            oXMLParam.ParamIn ("GetLocal")             = "0";
            oXMLParam.ParamIn ("ShowDialog")           = "0";
            if (!bRecursivelyAddChildren)
               oXMLParam.ParamIn ("Recursive")  = "0";
            oXMLParam.VerboseLevel           = conVerboseErrors;
            oXMLParam.XML                    = oObject.RunCommand (oXMLParam.Command, oXMLParam.XML, true);
            
            if (oXMLParam.WasSuccessful)
               return true;
            else
               VersionControlLog.Error ("Import to alienbrain failed for '" + strFilename + "', with server path '" + strNamespacePath + "'.");
         }
      }
   }
   return false;
}

/////////////////////////////////////////////////////////////////////////
// Set the RWS NXN properties on a file
function SetNXNProperties (strFilename, strName, strType, strDescription)
{
   if (bAlienbrainInstalled)
   {
      var strNamespacePath = MapManagedPath (strFilename);
      if (strNamespacePath != "")
      {
         oNxNNamespace.SetProperty (strNamespacePath, "RWSName", strName);
         oNxNNamespace.SetProperty (strNamespacePath, "RWSType", strType);
         oNxNNamespace.SetProperty (strNamespacePath, "RWSDescription", strDescription);
         // Flush the properties on the file node
         oNxNNamespace.Flush (strNamespacePath, true, NXN_NAMESPACE_FLUSH_PROPERTIES, "");
        
         return true;
      }
   }
   return false;
}
