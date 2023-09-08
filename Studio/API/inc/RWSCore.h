/*****************************************************************************
 *
 * File :     RWSCore.h
 *
 * Abstract:  RenderWare Studio Manager API
 *
 *****************************************************************************
 *
 * This file is a product of Criterion Software Ltd.
 *
 * This file is provided as is with no warranties of any kind and is
 * provided without any obligation on Criterion Software Ltd. or
 * Canon Inc. to assist in its use or modification.
 *
 * Criterion Software Ltd. will not, under any
 * circumstances, be liable for any lost revenue or other damages arising
 * from the use of this file.
 *
 * Copyright © 2002 Criterion Software Ltd. All rights reserved.
 * All Rights Reserved.
 *
 *****************************************************************************/


#ifndef CSL_RWS_CORE
#define CSL_RWS_CORE

#if _MSC_VER > 1000

#ifdef RWS_EXPORTS
#define RWS_API __declspec(dllexport)
#else
#define RWS_API __declspec(dllimport)
#endif

#else
#define RWS_API
#endif /* _MSC_VER > 1000 */

#define RWSCALLBACK __cdecl

#ifndef RWSTYPES_DEFINED
#define RWSTYPES_DEFINED

#define RWSTrue 1
#define RWSFalse 0

typedef unsigned char  RWSBool;
typedef int            RWSInt32;
typedef unsigned int   RWSUInt32;
typedef float          RWSReal;
typedef double         RWSDouble;
typedef char           RWSByte;
typedef unsigned short RWSChar;
typedef void*          RWSUserData;

/**
 * \ingroup GCore
 * \typedef enum RWSDataType
 * An enumeration for embedded types within attribute or property data.
 */
typedef enum
{
   RWSTypeBool = 0, /**< A boolean type (32-Bit) */
   RWSTypeInt32,    /**< A 32-Bit signed integer */
   RWSTypeUInt32,   /**< A 32-Bit unsigned integer */
   RWSTypeReal,     /**< A single-precision floating point number */
   RWSTypeDouble,   /**< A double-precision floating point number */
   RWSTypeByte,     /**< An 8-bit signed integer */
   RWSTypeChar      /**< A string character (16-Bit)*/
} RWSDataType;

/**
 * \ingroup GCore
 * \typedef enum RWSError
 * An enumerated type. Refers to the type of an internal API error.
 */
typedef enum
{
   RWSENoError = 0,     /**< No error occurred */
   RWSENotFound,        /**< The specified object was not found */
   RWSEIsChild,         /**< Item is already a child */
   RWSEFileNotFound,    /**< The specified file was not found */
   RWSETimedOut,        /**< The current network operation timed out */
   RWSEAborted,         /**< A long operation was aborted by user */
   RWSEInvalidParam,    /**< One of the parameters was invalid */
   RWSENotAttached,     /**< A database was not attached */
   RWSENotPermitted,    /**< Operation was not permitted */
   RWSEAlreadyAttached, /**< Already attached to a database */
   RWSENotCompatible,   /**< An ID was sent to an incompatible target */
   RWSEAlreadyExists,   /**< The object already exists */
   RWSEIncompatible,    /**< The DatabaseID is incompatible with this API version */
   RWSEFailedToAttach,  /**< The database failed to attach in RWSDatabaseAttach */
   RWSEUnknown,         /**< An unknown error occurred */
   RWSELoadFailed,      /**< An error occurred while loading an object's data */
   RWSESaveFailed       /**< An error occurred while saving an object's data */
} RWSError;

/**
 * \ingroup GCore
 * \typedef enum RWSVersionType
 * The version type to be returned by \ref RWSGetVersion or 
 * \ref RWSDatabaseGetVersion, either RWSVersionMajor or RWSVersionMinor.
 */
typedef enum
{
   RWSVersionMajor = 0, /**< The major version information */
   RWSVersionMinor      /**< The minor version information */
} RWSVersionType;

/**
 * \ingroup GCore
 * \typedef enum RWSIDType
 * An enumerated type. Refers to the type of an \ref RWSID (game object ID)
 */
typedef enum
{
   RWSNullID = 0,       /**< An invalid object */
   RWSGameID,           /**< A game object (parent of all objects) */
   RWSEntityID,         /**< An entity (an in-game object) */
   RWSAttributeID,      /**< An attribute or message belonging to an entity */
   RWSEventID,          /**< A game event (a door opening etc.) */
   RWSClassID,          /**< A representation of a game source-code class */
   RWSCommandID,        /**< A source code item found in a BEGIN_COMMANDS block */
   RWSAssetID,          /**< A game asset, such as a model, sound or menu etc. */
   RWSAttributeShareID, /**< Represents a set of attributes shared between entities */
   RWSFolderID,         /**< A folder can be used to structure the database */
   RWSLastIDType        /**< The last ID type (not a valid object type). */
} RWSIDType;

/**
 * Permit action types
 * \ingroup GCore
 * \typedef enum RWSPermitAction
 * An enumerated type. Refers to the type of an API action that requires permission
 */
typedef enum
{
   RWSPermitCreate = 0,  /**< An object is being created */
   RWSPermitDelete,      /**< An object is being deleted */
   RWSPermitAdd,         /**< An object is having a child added */
   RWSPermitRemove,      /**< An object is having a child removed */
   RWSPermitSwap,        /**< An object is having children swapped */
   RWSPermitChange,      /**< An object is being modified */
   RWSPermitChangeTag    /**< An object's tag is being modified */
} RWSPermitAction;

/**
 * \ingroup GCore
 * \typedef RWSID
 * Defines the ID of any game object stored in the API
 */
typedef RWSUInt32 RWSID;

/**
 * \ingroup GCore
 * \typedef RWSDatabaseID
 * Identifies a running instance of a game database
 */
typedef RWSUInt32 RWSDatabaseID;

/**
 * \ingroup GCore
 * \typedef RWSCallbackID
 * Identifies a set of callbacks added with \ref RWSCallbackAdd.
 */
typedef RWSUInt32 RWSCallbackID;

/**
 * \ingroup GCore
 * \typedef RWSTagID
 * Defines a tag that can be used to associate data with an \ref RWSID
 * (game object ID)
 */
typedef RWSUInt32 RWSTagID;

/**
 * \ingroup GCore
 * \typedef RWSMessageID
 * Defines a message that can be sent to any thread listening to events.
 */
typedef RWSUInt32 RWSMessageID;

/**
 * \ingroup GCore
 * \typedef RWSMessageID
 * Defines a property identifier.  Properties can be attached to any API object.
 */
typedef RWSUInt32 RWSPropertyID;

/**
 * \ingroup GCore
 * \typedef RWSIDTypeMask
 * Masking parameters for filtering different id types
 */
typedef RWSUInt32 RWSIDTypeMask;
const RWSIDTypeMask RWSNullIDMask           = 0x1 << RWSNullID;
const RWSIDTypeMask RWSGameIDMask           = 0x1 << RWSGameID;
const RWSIDTypeMask RWSEntityIDMask         = 0x1 << RWSEntityID;
const RWSIDTypeMask RWSAttributeIDMask      = 0x1 << RWSAttributeID;
const RWSIDTypeMask RWSEventIDMask          = 0x1 << RWSEventID;
const RWSIDTypeMask RWSClassIDMask          = 0x1 << RWSClassID;
const RWSIDTypeMask RWSCommandIDMask        = 0x1 << RWSCommandID;
const RWSIDTypeMask RWSAssetIDMask          = 0x1 << RWSAssetID;
const RWSIDTypeMask RWSAttributeShareIDMask = 0x1 << RWSAttributeShareID;
const RWSIDTypeMask RWSFolderIDMask         = 0x1 << RWSFolderID;
const RWSIDTypeMask RWSAllIDMask            = 0xFFFFFFFF;

/**
 * \ingroup GCore
 * \typedef RWSPersistSaveFlags
 * Flags for use with \ref RWSPersistProjectSave.
 */
typedef RWSUInt32 RWSSaveFlags;
const RWSSaveFlags RWSSaveAll = 0x1 << 0;

/* structures representing RWSIDTypes */

/**
 * \ingroup GCore
 * \struct RWSGame
 * A structure storing data associated with a game object.
 */
typedef struct
{
   RWSChar *Name;          /**< A display name for this object */
   RWSChar *Description;   /**< A description of this object */
   RWSChar *Platform;      /**< A string containing a list of platforms */
   RWSChar *BuildCommand;  /**< The command used for this object's build process */
} RWSGame;

/**
 * \ingroup GCore
 * \struct RWSEntity
 * A structure storing data associated with an entity object.
 */
typedef struct
{
   RWSChar *Name;          /**< A display name for this object */
   RWSChar *Description;   /**< A description of this object */
   RWSChar *Platform;      /**< A string containing a list of platforms */
   RWSChar *Behavior;      /**< The name of the C++ class defining this entity's
                                behavior */
   RWSChar *BuildCommand;  /**< The command used for this object's build process */
} RWSEntity;

/**
 * \ingroup GCore
 * \struct RWSAttribute
 * A structure storing data associated with an attribute object.
 */
typedef struct
{
   RWSChar  *ClassUID; /**< The UID (class name) of the C++ class for this attribute */
   RWSChar  *Member;   /**< The class member variable name */
   RWSChar  *Type;     /**< The class member data type, eg. RWSBool, RWSReal etc. */
   RWSChar  *Platform; /**< A string containing a list of platforms */
   RWSByte  *pData;    /**< A pointer to the attribute's data buffer */
   RWSUInt32 DataSize; /**< The size of attached data in bytes */
} RWSAttribute;

/**
 * \ingroup GCore
 * \struct RWSEvent
 * A structure storing data associated with an event object.
 */
typedef struct
{
   RWSChar *Name;          /**< A display name for this object */
   RWSChar *Type;          /**< The event type, eg. USER or SYSTEM */
   RWSChar *Description;   /**< A description of this object */
   RWSChar *Platform;      /**< A string containing a list of platforms */
} RWSEvent;

/**
 * \ingroup GCore
 * \struct RWSClass
 * A structure storing data associated with a class object.
 */
typedef struct
{
   RWSChar  *Category;      /**< The category parsed from the source file */
   RWSChar  *ShortDesc;     /**< A short description of the class */
   RWSChar  *LongDesc;      /**< A long description of the class */
   RWSChar  *FileName;      /**< The name of file containing this class */
   RWSUInt32 LineNo;        /**< The line number where class definition begins */
   RWSBool   Creatable;     /**< Specifies whether the class can be instantiated */
} RWSClass;

/**
 * \ingroup GCore
 * \struct RWSCommand
 * A structure storing data associated with a command object.
 */
typedef struct
{
   RWSChar  *Name;          /**< A display name for this object */
   RWSChar  *ParamList;     /**< A list of parameters passed to the command macro */
   RWSUInt32 ParamLength;   /**< The number of characters in the parameter list */
   RWSChar  *FileName;      /**< The name of the file containing this command */
   RWSUInt32 LineBegin;     /**< The line number where command begins */
   RWSUInt32 LineEnd;       /**< The line number where command ends */
} RWSCommand;

/**
 * \ingroup GCore
 * \struct RWSAsset
 * A structure storing data associated with an asset object.
 */
typedef struct
{
   RWSChar *Type;          /**< The asset type, eg. Map, rwTexDictionary etc. */
   RWSChar *Name;          /**< A display name for this object */
   RWSChar *File;          /**< The primary file containing this asset */
   RWSChar *Dependencies;  /**< The directory containing extra data files */
   RWSChar *Description;   /**< A description of this object */
   RWSChar *Model;         /**< The location of file this asset was exported from */
   RWSChar *Platform;      /**< A string containing a list of platforms */
   RWSChar *BuildCommand;  /**< The command used for this object's build process */
   RWSBool SendName;       /**< Specifies whether asset's name is sent to the target */
} RWSAsset;

/**
 * \ingroup GCore
 * \struct RWSAttributeShare
 * Stores data associated with the sharing of attributes.
 */
typedef struct
{
   RWSChar *Name;          /**< A display name for this object */
   RWSChar *Description;   /**< A description of this object */
} RWSAttributeShare;

/**
 * \ingroup GCore
 * \struct RWSFolder
 * A structure storing data associated with an set of objects.
 */
typedef struct
{
   RWSChar *Name;          /**< A display name for this object */
   RWSChar *Description;   /**< A description of this object */
   RWSChar *Platform;      /**< A string containing a list of platforms */
   RWSChar *BuildCommand;  /**< The command used for this object's build process */
} RWSFolder;

/**
 * \ingroup GCore
 * \struct RWSProperty
 * A structure storing custom data that can be attached to any ID type (RWSIDType).
 */
struct RWSProperty
{
   RWSByte   SystemFlags; /**< Flags indicating extra system information about this property */
   RWSByte   UserFlags;   /**< Flags indicating extra user information about this property */
   RWSByte*  pData;       /**< A pointer to the property's data buffer */
   RWSUInt32 DataSize;    /**< The size of attached data in bytes */
};


#endif /* RWSTYPES_DEFINED */

/**
 * \ingroup GCore
 * See \ref OnCreateCB
 * \param UserData The user data passed to \ref RWSCallbackAdd.
 * \param IDType The type of the object created.
 * \param ID The \ref RWSID of the object created.
 * 
 * \see RWSCreate
 */
typedef void (*OnCreateCBFunc) (RWSUserData UserData, RWSIDType IDType, RWSID ID);

/**
 * \ingroup GCore
 * See \ref OnChangeCB
 * \param UserData The user data passed to \ref RWSCallbackAdd.
 * \param IDType The type of the object being changed.
 * \param ID The \ref RWSID of the object being changed.
 */
typedef void (*OnChangeCBFunc) (RWSUserData UserData, RWSIDType IDType, RWSID ID);

/**
 * \ingroup GCore
 * See \ref OnDeleteCB
 * \param UserData The user data passed to \ref RWSCallbackAdd.
 * \param IDType The type of the object being deleted.
 * \param ID The \ref RWSID of the object being deleted.
 */
typedef void (*OnDeleteCBFunc) (RWSUserData UserData, RWSIDType IDType, RWSID ID);

/**
 * \ingroup GCore
 * See \ref OnAddChildCB
 * \param UserData The user data passed to \ref RWSCallbackAdd.
 * \param ParentType The \ref RWSIDType type of the parent object.
 * \param ParentID The \ref RWSID identifier of the parent object.
 * \param ChildType The \ref RWSIDType of the child object.
 * \param ChildID The \ref RWSID identifier of the child object.
 */
typedef void (*OnAddChildCBFunc) (RWSUserData UserData, RWSIDType ParentType, RWSID ParentID, RWSIDType ChildType, RWSID ChildID);

/**
 * \ingroup GCore
 * See \ref OnRemoveChildCB
 * \param UserData The user data passed to \ref RWSCallbackAdd.
 * \param ParentType The \ref RWSIDType type of the parent object.
 * \param ParentID The \ref RWSID identifier of the parent object.
 * \param ChildType The \ref RWSIDType of the child object.
 * \param ChildID The \ref RWSID identifier of the child object.
 */
typedef void (*OnRemoveChildCBFunc) (RWSUserData UserData, RWSIDType ParentType, RWSID ParentID, RWSIDType ChildType, RWSID ChildID);

/**
 * \ingroup GCore
 * See \ref OnSwapChildCB
 * \param UserData The user data passed to \ref RWSCallbackAdd.
 * \param ParentType The \ref RWSIDType type of the parent object.
 * \param ParentID The \ref RWSID identifier of the parent object.
 * \param ChildType The \ref RWSIDType of the two child objects.
 * \param ChildOne The \ref RWSID of the first of the two child objects.
 * \param ChildTwo The \ref RWSID of the second of the two child objects.
 */
typedef void (*OnSwapChildCBFunc) (RWSUserData UserData, RWSIDType ParentType, RWSID ParentID, RWSIDType ChildType, RWSID ChildOne, RWSID ChildTwo);

/** 
 * \ingroup GCore
 * See \ref OnChangeTagCB
 * \param UserData The user data passed to \ref RWSCallbackAdd.
 * \param IDType The type of the object whose tag has changed.
 * \param ID The \ref RWSID of the object whose tag has changed.
 * \param TagID The \ref RWSTagID being changed.
 */
typedef void (*OnChangeTagCBFunc) (RWSUserData UserData, RWSIDType IDType, RWSID ID, RWSTagID TagID);

/**
 * \ingroup GCore
 * See \ref OnMessageCB
 * \param UserData The user data passed to \ref RWSCallbackAdd.
 * \param Msg The \ref RWSMessageID of the message being sent.
 * \param nData The data being sent with the message.
 */
typedef void (*OnMessageCBFunc) (RWSUserData UserData, RWSMessageID Msg, RWSUInt32 nData);

/**
 * \ingroup GCore
 * See \ref OnChangePropertyCB
 * \param UserData The user data passed to \ref RWSCallbackAdd.
 * \param IDType The type of the object whose property has changed.
 * \param ID The \ref RWSID of the object whose property has changed.
 * \param PropertyID The \ref RWSPropertyID being changed.
 */
typedef void (*OnChangePropertyCBFunc) (RWSUserData UserData, RWSIDType IDType, RWSID ID, RWSPropertyID PropertyID);


/**
 * \ingroup GCore
 * \struct RWSCallbackData
 * Specifies callback functions. These can include NULL pointers. All function
 * pointers must use the __cdecl calling convention.
 * \see RWSCallbackAdd
 */
typedef struct
{
   /**
    * An optional function called when an RWS object is created (invoked by
    * \ref RWSCreate).
    */
   OnCreateCBFunc OnCreateCB;

   /**
    * An optional function called when an RWS object is changed (invoked by
    * \ref RWSGameSet, \ref RWSAssetSet etc).
    */
   OnChangeCBFunc OnChangeCB;

   /**
    * An optional function called when an RWS object is destroyed (invoked by
    * \ref RWSDelete).
    */
   OnDeleteCBFunc OnDeleteCB;

   /**
    * An optional function called when an RWS object is added as a child of
    * another (invoked by \ref RWSChildAdd).
    */
   OnAddChildCBFunc OnAddChildCB;

   /**
    * An optional function called when an RWS object is removed as a child of
    * another (invoked by \ref RWSChildRemove).
    */
   OnRemoveChildCBFunc OnRemoveChildCB;

   /**
    * An optional function called when the order of two child API objects are
    * changed (invoked by \ref RWSChildSwap).
    */
   OnSwapChildCBFunc OnSwapChildCB;

   /**
    * An optional function called when the tag data of an object is changed
    * (invoked by \ref RWSTagSetData).
    */
   OnChangeTagCBFunc OnChangeTagCB;

   /**
    * An optional function called when a message is sent (invoked by 
    * \ref RWSMessageSend).
    */
   OnMessageCBFunc OnMessageCB;

   /**
    * An optional function called when a property is changed (invoked by
    * \ref RWSPropertySet).
    */
   OnChangePropertyCBFunc OnChangePropertyCB;

} RWSCallbackData;

/**
 * \ingroup GCore
 * \struct RWSOnPermitCB
 * Called to obtain permission to modify an API object.
 * \see RWSDatabasePermitCallbackSet
 */
typedef struct
{
   union
   {
      struct /* For OnAdd, OnRemove */
      {
         RWSID     ParentID;
         RWSIDType ParentType;
         RWSID     ChildID;
         RWSIDType ChildType;
      };

      struct /* For OnCreate, OnChange, OnDelete, OnSwap */
      {
         RWSID     ID;
         RWSIDType IDType;
      };

      struct /* For OnChangeTag */
      {
         RWSID     ObjectID;
         RWSIDType ObjectIDType;
         RWSTagID  TagID;
      };
   };

   RWSPermitAction Action;

} RWSPermitCBData;

/**
 * \ingroup GCore
 * \struct RWSOnPermitCBFunc
 * Called to obtain permission to modify an API object.
 * \see RWSDatabasePermitCallbackSet
 */
typedef RWSBool (RWSCALLBACK *RWSOnPermitCBFunc) (RWSUserData,
                                                  const RWSPermitCBData * const);

/**
 * \ingroup GCore
 * \struct RWSOnPersistErrorCBFunc
 * Called to notify the client that an error occurred during a persistence operation.
 * \see RWSDatabasePersistErrorCallbackSet
 */
typedef void (RWSCALLBACK *RWSOnPersistErrorCBFunc) (
                              RWSUserData, const RWSChar * const Filename,
                              const RWSChar * const Description, RWSError Error,
                              RWSUInt32 Location);

/**
 * \ingroup GCore
 * \struct RWSOnPersistProgressCBFunc
 * Called to notify the client of progress during a persist operation.
 * \see RWSDatabasePersistProgressCallbackSet
 */
typedef void (RWSCALLBACK *RWSOnPersistProgressCBFunc) (RWSUserData,
                                                        RWSReal fPercentage);


/**
 * \ingroup GCore
 * \struct RWSOwnerCallbackData
 * Specifies callback functions. These can include NULL pointers. All function
 * pointers must use the __cdecl calling convention.
 * \see RWSDatabaseCreate
 */
typedef struct
{
   /**
    * An optional function called when permission is required to make a database
    * modification.
    */
   RWSOnPermitCBFunc OnPermitCB;

   /**
    * An optional function called when a persistence error occurs.
    */
   RWSOnPersistErrorCBFunc OnPersistErrorCB;

   /**
    * An optional function called repeatedly during a persistence operation.
    */
   RWSOnPersistProgressCBFunc OnPersistProgressCB;

} RWSOwnerCallbackData;


#ifdef __cplusplus
extern "C"
{
#endif

/*
 * Library initialization / shutdown 
 */
RWS_API RWSBool RWSInitialize    ();
RWS_API void    RWSShutdown      ();

/*
 * Version information of local API
 */
RWS_API RWSUInt32 RWSGetVersion (RWSVersionType VersionType);

/*
 * Error handling routines
 */
RWS_API RWSError RWSErrorGetLast ();
RWS_API void     RWSErrorSetLast (RWSError Error);

/*
 * Database creation / destruction / enumeration
 */
RWS_API RWSDatabaseID RWSDatabaseCreate (const RWSChar * const szName, RWSBool bIsVisible,
                                         const RWSOwnerCallbackData * const pCallbacks,
                                         RWSUserData UserData);
RWS_API void          RWSDatabaseDestroy ();
RWS_API RWSDatabaseID RWSDatabaseGetFirst ();
RWS_API RWSDatabaseID RWSDatabaseGetNext (RWSDatabaseID LastDatabaseID);
RWS_API RWSUInt32     RWSDatabaseGetVersion (RWSDatabaseID DatabaseID, RWSVersionType VersionType);
RWS_API RWSUInt32     RWSDatabaseGetName (RWSDatabaseID DatabaseID, RWSChar * szBuffer, RWSUInt32 nMaxSize);
RWS_API void          RWSDatabaseAttach (RWSDatabaseID DatabaseID);
RWS_API void          RWSDatabaseDetach ();

/*
 * Callback handling for receiving notification of changes to the internal structures
 */
RWS_API RWSCallbackID RWSCallbackAdd (const RWSCallbackData * const pCallbackData, RWSUserData UserData);
RWS_API void          RWSCallbackRemove (RWSCallbackID CallbackID);
RWS_API void          RWSCallbackSetMask (RWSCallbackID CallbackID, RWSIDTypeMask nMask);
RWS_API void          RWSCallbackListen (void);
RWS_API RWSBool       RWSCallbackEnable (RWSCallbackID CallbackID, RWSBool bEnable);
RWS_API RWSBool       RWSCallbackSuspendAll (RWSBool bSuspend);

/*
 * Load/Save/Import a project
 */
RWS_API RWSID RWSProjectLoad (const RWSChar * const szLocation);
RWS_API void  RWSProjectSave (RWSID GameID, const RWSChar * const szLocation,
                              RWSSaveFlags Flags);
RWS_API RWSID RWSProjectImport (const RWSChar * const szProjectFilename,
                                const RWSChar * const szFolderGUID);

RWS_API RWSID RWSLoad (const RWSChar * const UID, RWSIDType Type);
RWS_API void  RWSSave (RWSID ID);

/* object file control */
RWS_API RWSUInt32 RWSGetLocation (RWSID ID, RWSChar * const szLocation,
                                  RWSUInt32 nMaxSize);
/*
 * Creation / deletion of API objects
 */
RWS_API RWSID RWSCreate (RWSIDType Type, const RWSChar * const UID);
RWS_API void  RWSDelete (RWSID ID);

/*
 * Retrieve information about type / unique id (UID) of objects 
 */
RWS_API RWSID     RWSGetID (RWSIDType IDType, const RWSChar * const UID);
RWS_API RWSUInt32 RWSGetUID (RWSID ID, RWSIDType IDType, RWSChar * const UID, RWSUInt32 nMaxSize);
RWS_API RWSIDType RWSGetType (RWSID ID);

/*
 * Creation of hierarchy
 */
RWS_API void RWSChildAdd (RWSID ParentID, RWSID ID);
RWS_API void RWSChildRemove (RWSID ParentID, RWSID ID);
RWS_API void RWSChildSwap (RWSID ParentID, RWSID ChildOne, RWSID ChildTwo);

/*
 * Traversal of hierarchy
 */
RWS_API RWSID RWSGetFirst (RWSIDType Type);
RWS_API RWSID RWSGetNext (RWSID PreviousID);
RWS_API RWSID RWSChildGetFirst (RWSID ParentID, RWSIDType ChildType);
RWS_API RWSID RWSChildGetNext (RWSID ParentID, RWSID PreviousChildID);
RWS_API RWSID RWSParentGetFirst (RWSID ChildID, RWSIDType ParentType);
RWS_API RWSID RWSParentGetNext (RWSID ChildID, RWSID PreviousParentID);

/*
 * Free API allocated memory
 */
RWS_API void RWSFreeMemory (void * const pMemory);

/*
 * Access functions for different API object types
 */ 
RWS_API void RWSGameGet (RWSID GameID, RWSGame* const pGame);
RWS_API void RWSGameSet (RWSID GameID, const RWSGame* const pGame);
RWS_API void RWSGameFreeData (RWSGame* const pGame);

RWS_API void RWSEntityGet (RWSID EntityID, RWSEntity* const pEntity);
RWS_API void RWSEntitySet (RWSID EntityID, const RWSEntity* const pEntity);
RWS_API void RWSEntityFreeData (RWSEntity* const pEntity);

RWS_API void RWSAttributeGet (RWSID AttributeID, RWSAttribute* const pAttribute);
RWS_API void RWSAttributeSet (RWSID AttributeID, const RWSAttribute* const pAttribute);
RWS_API void RWSAttributeFreeData (RWSAttribute* const pAttribute);

RWS_API void RWSEventGet (RWSID EventID, RWSEvent* const pEvent);
RWS_API void RWSEventSet (RWSID EventID, const RWSEvent* const pEvent);
RWS_API void RWSEventFreeData (RWSEvent* const pEvent);

RWS_API void RWSClassGet (RWSID ClassID, RWSClass* const pClass);
RWS_API void RWSClassSet (RWSID ClassID, const RWSClass* const pClass);
RWS_API void RWSClassFreeData (RWSClass* const pClass);

RWS_API void RWSCommandGet (RWSID CommandID, RWSCommand* const pCommand);
RWS_API void RWSCommandSet (RWSID CommandID, const RWSCommand* const pCommand);
RWS_API void RWSCommandFreeData (RWSCommand* const pCommand);

RWS_API void RWSAssetGet (RWSID AssetID, RWSAsset* const pAsset);
RWS_API void RWSAssetSet (RWSID AssetID, const RWSAsset* const pAsset);
RWS_API void RWSAssetFreeData (RWSAsset* const pAsset);

RWS_API void RWSAttributeShareGet (RWSID AttributeShareID, RWSAttributeShare* const pAttributeShare);
RWS_API void RWSAttributeShareSet (RWSID AttributeShareID, const RWSAttributeShare* const pAttributeShare);
RWS_API void RWSAttributeShareFreeData (RWSAttributeShare* const pAttributeShare);

RWS_API void RWSFolderGet (RWSID FolderID, RWSFolder* const pFolder);
RWS_API void RWSFolderSet (RWSID FolderID, const RWSFolder* const pFolder);
RWS_API void RWSFolderFreeData (RWSFolder* const pFolder);


/*
 * Association of a tag with an API object
 */
RWS_API RWSTagID  RWSTagRegister (const RWSChar * const TagName);
RWS_API void      RWSTagSetData (RWSID ID, RWSTagID Tag, RWSUInt32 nData);
RWS_API RWSUInt32 RWSTagGetData (RWSID ID, RWSTagID Tag);


/*
 * Send messages to callbacks
 */ 
RWS_API RWSMessageID RWSMessageRegister (const RWSChar * const MsgName);
RWS_API void         RWSMessageSend (RWSMessageID Msg, RWSUInt32 nData);


/*
 * Property functions
 */ 
RWS_API RWSPropertyID RWSPropertyRegister (const RWSChar * const PropertyUID);
RWS_API RWSUInt32 RWSPropertyGetUID (RWSPropertyID PropertyID, RWSChar * const PropertyUID, RWSUInt32 nMaxSize);

RWS_API void RWSPropertySet (RWSID ObjectID, RWSPropertyID PropertyID, const RWSProperty * const pProperty);
RWS_API void RWSPropertyGet (RWSID ObjectID, RWSPropertyID PropertyID, RWSProperty* const pProperty);
RWS_API void RWSPropertyFreeData (RWSProperty * const pProperty);

RWS_API RWSPropertyID RWSPropertyGetFirst (RWSID ObjectID);
RWS_API RWSPropertyID RWSPropertyGetNext (RWSID ObjectID, RWSPropertyID PrevPropertyID);


#ifdef __cplusplus
}
#endif

#endif /* CSL_RWS_CORE */
