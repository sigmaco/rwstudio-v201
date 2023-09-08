/*****************************************************************************
 *
 * File :     RWSPersist.h
 *
 * Abstract:  RenderWare Studio Manager API Toolkit - Database Persistence
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


#ifndef CSL_RWS_PERSIST_H
#define CSL_RWS_PERSIST_H

#include "RWSCore.h"

#if _MSC_VER > 1000

#ifdef RWSPERSIST_EXPORTS
#define RWSPERSIST_API __declspec(dllexport)
#else
#ifdef RWSPERSIST_CONVERTER
#define RWSPERSIST_API extern
#else
#define RWSPERSIST_API __declspec(dllimport)
#endif
#endif

#else
#define RWSPERSIST_API
#endif /* _MSC_VER > 1000 */

#ifdef __cplusplus
extern "C"
{
#endif

#define RWSPERSISTVERSION RWSSTRING("2.0")

/* init/shutdown functions */
RWSPERSIST_API RWSBool RWSPersistInitialize ();
RWSPERSIST_API void    RWSPersistShutdown ();

RWSPERSIST_API RWSBool RWSPersistHaveVersionControl ();

RWSPERSIST_API RWSBool RWSPersistProjectConnectToVersionControl ();

RWSPERSIST_API void    RWSPersistProjectSaveToVersionControl (RWSID GameID, 
                                            const RWSChar * const szFilename,
                                            const RWSChar * const szComment);
RWSPERSIST_API void  RWSPersistProjectSetLoadProgressCb (RWSUserData UserData,
                                                         RWSUInt32 nFrequency,
                                                         void (*cb)(RWSUserData, RWSUInt32));
RWSPERSIST_API void  RWSPersistProjectGetLatest (const RWSChar * const szProjectFilename,
                                                 const RWSChar * const szResourceRoot);

/* Project Import */
RWSPERSIST_API RWSUInt32 RWSPersistProjectGetLevels      (const RWSChar * const szProjectFilename,
                                                        RWSChar ***levels);
RWSPERSIST_API void    RWSPersistProjectFreeLevelNames (RWSChar **ppLevelNames, RWSUInt32 numLevels);


/* version control */
/* note: these states are ordered to allow comparison */
/**
 * \ingroup GGameFile
 * \typedef enum RWSPersistState
 * RWSPersistState - state of object or file.
 * \see RWSPersistIDGetStatus
 * \see RWSPersistFileGetStatus
*/
typedef enum
{
   RWSPersistStateUnknown = 0,           /**< unknown state               */
   RWSPersistStateNotSaved,              /**< not yet saved               */
   RWSPersistStateLocal,                 /**< persists on local machine   */
   RWSPersistStateManaged,               /**< managed by version control  */
   RWSPersistStateManagedEditedLocally,  /**< managed by version control and edited on users local machine */
   RWSPersistStateCheckedOutToUser,      /**< checked out to this user from version control  */
   RWSPersistStateCheckedOutToOtherUser, /**< checked out to another user */ 
   RWSPersistStateCheckedOutToOtherUserEditedLocally /**< checked out to another user and edited locally */ 
} RWSPersistState; 

/**
 * \ingroup GCore
 * \struct RWSPersistStateData
 * Specifies information regarding the status of a file in the project. 
 * \see RWSPersistIDGetStatus
 * \see RWSPersistFileGetStatus
 * \see RWSPersistFreeStateData
 */
typedef struct
{
   RWSPersistState State;             /**< the current state of the file \ref RWSPersistState. */
   RWSUInt32       NumCheckOutUsers;  /**< the number of users who have this file checked out. */
   RWSChar **      CheckOutUsers;     /**< array of strings holding the names of each user who has the file checked out. */
} RWSPersistStateData;

/* change list control */
/**
 * \ingroup GGameFile
 * \typedef enum RWSPersistAction
 * RWSPersistAction - actions for version control operations. 
 *        These actions are held in a change list for later submission
 *        The changelist is stored within the project hierarchy with a 
 *        ".changelist" extension.
 * \see RWSPersistChangeListRevert
 * \see RWSPersistChangeListSubmit
*/
typedef enum
{
   RWSPersistActionAdd = 0,    /**< add object to version control      */
   RWSPersistActionRemove,     /**< remove object from version control */
   RWSPersistActionModify,     /**< modify object in version control   */
} RWSPersistAction;

RWSPERSIST_API RWSBool   RWSPersistIDCheckOut   (RWSID ID, 
                                                 RWSPersistAction Action);
RWSPERSIST_API RWSBool   RWSPersistIDEditLocal (RWSID ID, 
                                                RWSPersistAction Action);
RWSPERSIST_API RWSBool   RWSPersistFileCheckOut (const RWSChar * const szFilename, 
                                                 RWSPersistAction Action);

RWSPERSIST_API void RWSPersistClearStateCache (const RWSChar * const szPath);

RWSPERSIST_API RWSPersistState RWSPersistIDGetStatus  (RWSID ID,
                                                       RWSPersistStateData * const pStateData);

RWSPERSIST_API RWSPersistState RWSPersistFileGetStatus(const RWSChar * const szFilename, 
                                                       RWSPersistStateData * const pStateData);

RWSPERSIST_API void      RWSPersistFreeStatusData     (RWSPersistStateData * const pStateData);

RWSPERSIST_API void      RWSPersistChangeListLoad (const RWSChar * const szProjectFile);
RWSPERSIST_API void      RWSPersistChangeListSubmit   (RWSID GameID, 
                                                       const RWSChar * const szFilename, 
                                                       const RWSChar * const szComment);
RWSPERSIST_API RWSBool   RWSPersistChangeListRevert   (const RWSChar * const szFilename);
RWSPERSIST_API RWSBool   RWSPersistChangeListRevertLocalEdits (const RWSChar * const szFilename);
RWSPERSIST_API RWSBool   RWSPersistChangeListHaveCheckOuts(void);
RWSPERSIST_API RWSBool   RWSPersistChangeListHaveLocalEdits(void);
RWSPERSIST_API RWSBool   RWSPersistChangeListCanBeWritten ();
RWSPERSIST_API RWSBool   RWSPersistChangeListVerify ();

RWSPERSIST_API RWSBool   RWSPersistPathIsManaged (const RWSChar * const szPath);

RWSPERSIST_API RWSBool   RWSPersistFileImportToVersionControl (const RWSChar * const szFilename,
                                                               const RWSChar * const szComment);
RWSPERSIST_API void      RWSPersistSetUserProperty (RWSID objectID);
RWSPERSIST_API RWSPersistState      RWSPersistSetPersistProperty (RWSID ObjectID);

/* Persistence error handling     
/**
 * \ingroup GGameFile
 * \typedef enum RWSPersistError
 * RWSPersistError - persistence operation error states.
 * \see RWSPersistErrorGetLast
 * \see RWSPersistErrorSetCallback
*/
typedef enum
{
   RWSPersistENoError = 0,        /**< No error                                 */
   RWSPersistENotInitialized,     /**< Not initialized                          */
   RWSPersistEFileNotFound,       /**< The specified file was not found         */
   RWSPersistECouldNotOpenFile,   /**< The specified file could not be opened   */
   RWSPersistEInvalidParam,       /**< One of the parameters was invalid        */
   RWSPersistEUnknown,            /**< Unknown error                            */
   RWSPersistENotLatestVersion,   /**< Local file is not latest version         */
   RWSPersistECheckoutDenied,     /**< Another user has the file checked out    */
   RWSPersistELocalEditFailed,    /**< Failed to make file locally writable     */
   RWSPersistEObjectNotSaved,     /**< Object does not persist in database      */
   RWSPersistENoVersionControl,   /**< Version control could not be initialised */
   RWSPersistENoVersionControlConnection, /**< Could not connect to the manager */
   RWSPersistEObjectNotManaged,   /**< Object not version controlled            */
   RWSPersistEChildNotFound,      /**< A parent references an nonexistent child */
   RWSPersistECouldNotSubmitFile, /**< The specified file could not be submitted*/
   RWSPersistECouldNotRevertFile, /**< The specified file could not be reverted */
   RWSPersistECouldNotGetLatest,  /**< The specified file/folder could not be sync'd */
   RWSPersistEGUIDNotRecognised,  /**< The GUID is not recognised               */
   RWSPersistEGameNotCreated,     /**< A game must be created to import into    */
   RWSPersistEChangeListReadOnly, /**< The change list file is read only        */
   RWSPersistEInvalidXML,         /**< XML parser error                         */
   RWSPersistECouldNotParseFile,  /**< The specified file could not be parsed   */
   RWSPersistEMissingAttribute,   /**< An attribute required for building a successful object is missing */
   RWSPersistEInvalidChangeListEntry, /**< An invalid entry in the change list has been found */
   RWSPersistECouldNotRemoveFile, /**< The specified file could not be removed */
   RWSPersistEOldFileVersion,     /**< The specified file has been parsed but may not be edited */
   RWSPersistEUnsupportedFileVersion,/**< The specified file could not be parsed as the format is not supported */
   RWSPersistEHaveLocalEdits,     /**< Could not perform action since local checkouts have been made */
} RWSPersistError;

typedef void (*RWSPersistErrorCallback)(RWSUserData UserData, 
                                        RWSID ID,
                                        const RWSChar* const szData, 
                                        RWSPersistError Error);

RWSPERSIST_API RWSPersistError RWSPersistErrorGetLast ();

RWSPERSIST_API void RWSPersistErrorSetCallback (RWSUserData UserData, RWSPersistErrorCallback ErrorCb);

#ifdef __cplusplus
}
#endif

#endif /* CSL_RWS_PERSIST_H */




