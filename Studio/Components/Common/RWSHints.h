// file used to centralise all RWS Enterprise exec messages

#ifndef __RWS__HINTS__H__
#define __RWS__HINTS__H__

#include <tchar.h>

#include "API/Impl/RWSString.h"

#if (!defined(RWFORCEENUMSIZEINT))
#define RWFORCEENUMSIZEINT ((RWSInt32)((~((RWSUInt32)0))>>1))
#endif 

namespace RWS
{
   // property key representing an RWSID.  Attached to RealiMation objects
   // as an easy way to associate visual representation of game objects with
   // actual game objects in the RWS API

   // {839C25C0-FE2D-4ac4-81D7-FDE6AB024351}
   // Type        : RTAppTypeLong
   // Description : Core RWS object
   // Company     : CSL
   const GUID guidRWSCoreObject = 
         { 0x839c25c0, 0xfe2d, 0x4ac4,
         { 0x81, 0xd7, 0xfd, 0xe6, 0xab, 0x2, 0x43, 0x51 } };

   // The clipboard format used by workspace
   const _TCHAR szClipboardFormat[] = _T ("RenderWareStudioWorkspaceCF");
   
   // The data contained in the clipboard
   struct RWSClipData
   {
      DWORD          m_dwProcessID;
      RWSDatabaseID  m_DatabaseID;
      RWSID          m_ParentID;
      RWSUInt32      m_nCount;
      RWSID          m_IDList[1];
   };
  
   // The sharable status of a command, tagged to CommandID's using tagShareScope
   enum
   {
      RWSSharableCommand = 0,
      RWSPrivateCommand,
      RWSGlobalCommand
   };

   enum FolderType
   {
      FT_FOLDER = 0, FT_ASSETFOLDER, FT_BEHAVIORFOLDER, FT_TEMPLATEFOLDER,
      FT_ATTRIBUTESHAREFOLDER
   };

   // Custom property flags (system)
   // (If updated need to also update WritePropertiesScript routine in 
   //  CreateHintsScripts.wsf)
   const RWSByte sysflagDontSaveProperty  = 0x01;
   const RWSByte sysflagCopyProperty      = 0x02;

   // Display flags, for use with the propEntityDisplayOptions property 
   // (If updated need to also update WritePropertiesScript routine in 
   //  CreateHintsScripts.wsf)
   enum dispOptionsFlags
   {
      dispOptionsFlags_Freeze           = 0x01,
      dispOptionsFlags_FORCEENUMSIZEINT = RWFORCEENUMSIZEINT  // Flag to force the values to be 32bit,
                                                              //  since we're using the values as flags
                                                              //  we need to guarantee their size
   };

   // Tag strings for registering RWSTags
   const RWSChar tagDirtyID[]          = RWSSTRING ("RWSTAG_DIRTY_ID");          // dirty in the API
   const RWSChar tagDeletedID[]        = RWSSTRING ("RWSTAG_DELETED_ID");        // deleted in the UI
   const RWSChar tagIncompleteID[]     = RWSSTRING ("RWSTAG_INCOMPLETE_ID");     // under construction
   const RWSChar tagMatrixAttribID[]   = RWSSTRING ("RWSTAG_MATRIX_ATTRIB_ID");  // a matrix
   const RWSChar tagCreatableClass[]   = RWSSTRING ("RWSTAG_CREATABLE_CLASS_ID");// creatable behaviour
   const RWSChar tagFileNotFound[]     = RWSSTRING ("RWSTAG_FILE_NOTFOUND");     // asset file not found
   const RWSChar tagTemplateEntity[]   = RWSSTRING ("RWSTAG_TEMPLATE_ENTITY");   // template entity
   const RWSChar tagCameraAttribute[]  = RWSSTRING ("RWSTAG_CAMERA_ATTRIBUTE");  // The camera attribute's tag.
   const RWSChar tagDefaultAttribute[] = RWSSTRING ("RWSTAG_DEFAULTATTRIB");     // The default attribute of a command.
   const RWSChar tagVisibilityStatus[] = RWSSTRING ("RWSTAG_VISIBILITY_STATUS"); // The visibility status of an object.
   const RWSChar tagShareScope[]       = RWSSTRING ("RWSTAG_SHARE_SCOPE");       // The share scope of a command.
   const RWSChar tagTestFireEvent[]    = RWSSTRING ("RWS_FIRE_EVENT");           // Test fire event: ID is EventID or (EntityID with data = AttributeID or CommandID)
   const RWSChar tagFolderType[]       = RWSSTRING ("RWSTAG_FOLDERTYPE");
   const RWSChar tagChunkGroupIndex[]  = RWSSTRING ("RWSTAG_CHUNK_GROUP_INDEX");  // The group index into an rws file.
   const RWSChar tagActiveFolder[]     = RWSSTRING ("RWSTAG_ACTIVE_FOLDER");      // Folder to be rendered and sent to console.
   const RWSChar tagWorkingFolder[]    = RWSSTRING ("RWSTAG_WORKING_FOLDER");     // Folder that will parent new objects.
   const RWSChar tagResourceStatus[]   = RWSSTRING ("RWSTAG_RESOURCE_STATUS");    // Status of resource file.
   const RWSChar tagRF3CreatedAsset[]  = RWSSTRING ("RWSTAG_RF3_IMPORT_CREATED"); // This ID was automatically generated by the RF3 import process.
   const RWSChar tagSequencedEntity[]  = RWSSTRING ("RWSTAG_SEQUENCED_ENTITY");   // The entity is a sequenced copy. The data is the id of the original entity.

   // Property strings for registering properties
   const RWSChar propVersionControlStatus[] = RWSSTRING ("RWSPROP_VERSIONCONTROLSTATUS");  // Version control status of files.
   const RWSChar propResourceStatus[]       = RWSSTRING ("RWSPROP_RESOURCESTATUS");        // Version control status of files.
   const RWSChar propStreamEditData[]       = RWSSTRING ("RWSPROP_STREAMEDIT_DATA");       // Stream editor data
   const RWSChar propHullShape[]            = RWSSTRING ("RWSPROP_HULL_SHAPE");            // Hull shape property
   const RWSChar propHullTexture[]          = RWSSTRING ("RWSPROP_HULL_TEXTURE");          // Hull texture property
   const RWSChar propLightMapLight[]        = RWSSTRING ("RWSPROP_LIGHTMAPPER_LIGHT");     // LightMapTool light entity
   const RWSChar propEntityDisplayOptions[] = RWSSTRING ("RWSPROP_ENTITYDISPLAY_OPTIONS"); // Entity display options

   // The following tags/properties are set on the game
   const RWSChar tagAssetHierarchy[]    = RWSSTRING ("RWS_ASSET_HIERARCHY");     // The asset hierarchy.
   const RWSChar tagBehaviorHierarchy[] = RWSSTRING ("RWS_BEHAVIOR_HIERARCHY");  // The behavior hierarchy
   const RWSChar tagTemplateHierarchy[] = RWSSTRING ("RWS_TEMPLATE_HIERARCHY");  // The template hierarchy
   const RWSChar tagAttributeShareHierarchy[] = RWSSTRING ("RWS_ATTRIBUTESHARE_HIERARCHY");   // The attribute share hierarchy
   const RWSChar tagGlobalFolder[]            = RWSSTRING ("RWSTAG_GLOBAL_FOLDER");           // global level
   const RWSChar propVersionControlUsername[] = RWSSTRING ("RWSPROP_VERSIONCONTROLUSERNAME"); // The username the user is connected as.

   // Message strings for registering RWSMessages
   const RWSChar msgEventsSuspended[]  = RWSSTRING ("RWSMSG_EVENTSSUSPENDED");  // Events have been suspended if nData is RWSTrue, enabled if nData is RWSFalse
   const RWSChar msgReloadAsset[]      = RWSSTRING ("RWSMSG_RELOADASSET");      // Force a reload of the specified asset. nData contains an RWSID of the asset
   const RWSChar msgAssetRefreshed[]   = RWSSTRING ("RWSMSG_ASSETREFRESHED");   // An asset was loaded/reloaded, so update any view of it
   const RWSChar msgPreImportAsset[]   = RWSSTRING ("RWSMSG_PREIMPORTASSET");   // An asset is about to be imported.
   const RWSChar msgPostImportAsset[]  = RWSSTRING ("RWSMSG_POSTIMPORTASSET");  // An asset has just been imported.
   const RWSChar msgPostImportAssets[] = RWSSTRING ("RWSMSG_POSTIMPORTASSETS"); // All assets have just been imported.
   const RWSChar msgDisable3DRefresh[] = RWSSTRING ("RWSMSG_DISABLE3DREFRESH"); // Temporarily suspend/re-enable calls to refresh the 3D views
   const RWSChar msgInvalidateAssets[] = RWSSTRING ("RWSMSG_INVALIDATEASSETS"); // Invalidates all assets, so that the asset previews will be reloaded.
   const RWSChar msgClipboardChanged[] = RWSSTRING ("RWSMSG_CLIPBOARDCHANGED"); // Informs that a cut/copy operation has taken place

   // GUIDS for hierarchy objects
   const RWSChar szAssetHierarchyFolderGUID[]          = RWSSTRING ("RWS_AssetHierarchyFolder");
   const RWSChar szTemplateHierarchyFolderGUID[]       = RWSSTRING ("RWS_TemplateHierarchyFolder");
   const RWSChar szAttributeShareHierarchyFolderGUID[] = RWSSTRING ("RWS_AttributeShareHierarchyFolder");
   const RWSChar szGlobalFolderGUID[]                  = RWSSTRING ("RWS_GlobalFolder");
}

#endif //__RWS__HINTS__H__