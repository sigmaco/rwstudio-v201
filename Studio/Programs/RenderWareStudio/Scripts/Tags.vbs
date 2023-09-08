
' Tags.vbs

' This file is auto-generated from RWSHints.h and contains the
' list of RenderWare Studio reserved tags and associated
' helper functions.

Option Explicit ' All variables must be explicitly declared

Const conDirtyID = "RWSTAG_DIRTY_ID" ' dirty in the API
Const conDeletedID = "RWSTAG_DELETED_ID" ' deleted in the UI
Const conIncompleteID = "RWSTAG_INCOMPLETE_ID" ' under construction
Const conMatrixAttribID = "RWSTAG_MATRIX_ATTRIB_ID" ' a matrix
Const conCreatableClass = "RWSTAG_CREATABLE_CLASS_ID" ' creatable behaviour
Const conFileNotFound = "RWSTAG_FILE_NOTFOUND" ' asset file not found
Const conTemplateEntity = "RWSTAG_TEMPLATE_ENTITY" ' template entity
Const conCameraAttribute = "RWSTAG_CAMERA_ATTRIBUTE" ' The camera attribute's tag.
Const conDefaultAttribute = "RWSTAG_DEFAULTATTRIB" ' The default attribute of a command.
Const conVisibilityStatus = "RWSTAG_VISIBILITY_STATUS" ' The visibility status of an object.
Const conShareScope = "RWSTAG_SHARE_SCOPE" ' The share scope of a command.
Const conTestFireEvent = "RWS_FIRE_EVENT" ' Test fire event: ID is EventID or (EntityID with data = AttributeID or CommandID)
Const conFolderType = "RWSTAG_FOLDERTYPE" '
Const conChunkGroupIndex = "RWSTAG_CHUNK_GROUP_INDEX" ' The group index into an rws file.
Const conActiveFolder = "RWSTAG_ACTIVE_FOLDER" ' Folder to be rendered and sent to console.
Const conWorkingFolder = "RWSTAG_WORKING_FOLDER" ' Folder that will parent new objects.
Const conResourceStatus = "RWSTAG_RESOURCE_STATUS" ' Status of resource file.
Const conRF3CreatedAsset = "RWSTAG_RF3_IMPORT_CREATED" ' This ID was automatically generated by the RF3 import process.
Const conSequencedEntity = "RWSTAG_SEQUENCED_ENTITY" ' The entity is a sequenced copy. The data is the id of the original entity.
Const conAssetHierarchy = "RWS_ASSET_HIERARCHY" ' The asset hierarchy.
Const conBehaviorHierarchy = "RWS_BEHAVIOR_HIERARCHY" ' The behavior hierarchy
Const conTemplateHierarchy = "RWS_TEMPLATE_HIERARCHY" ' The template hierarchy
Const conAttributeShareHierarchy = "RWS_ATTRIBUTESHARE_HIERARCHY" ' The attribute share hierarchy
Const conGlobalFolder = "RWSTAG_GLOBAL_FOLDER" ' global level