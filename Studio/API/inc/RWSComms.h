/*****************************************************************************
 *
 * File :     RWSComms.h
 *
 * Abstract:  RenderWare Studio Manager API Toolkit - Network Communication
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

#ifndef CSL_RWS_COMMS
#define CSL_RWS_COMMS

#if _MSC_VER > 1000

#ifdef RWSC_EXPORTS
#define RWSC_API __declspec(dllexport)
#else
#define RWSC_API __declspec(dllimport)
#endif

#else
#define RWSC_API
#endif /* _MSC_VER > 1000 */


#ifdef __cplusplus
extern "C"
{
#endif

/**
 * \ingroup GComms
 * \typedef RWSCConnectionID
 * Represents a connection ID returned by the \ref RWSCConnect function.
 */
typedef RWSInt32 RWSCConnectionID;

/**
 * \ingroup GComms
 * \typedef pfRWSMessageCallback
 * Callback function supplied for notification of messages received from a 
 * connected target via the network.
 *
 * \param ConnectionID   An \ref RWSCConnectionID representing the ID of the 
 *                       connection that sent the message.
 * \param pData          A byte stream representing the message data sent by the
 *                       target.
 * \param nDataSize      An \ref RWSUInt32 specifying the size of the binary
 *                       message data.
 * \param UserData       A void pointer to user-supplied data.  This pointer will
 *                       be the pointer passed to the RWSCAddMessageCallback call
 *                       when the callback was registered.
 *
 * \see RWSCSetMessageCallback
 * \see RWSCCheckForMessages
 */
typedef void (*pfRWSMessageCallback)(RWSCConnectionID ConnectionID,
                                     const RWSByte * const pData,
                                     RWSUInt32 nDataSize, RWSUserData UserData);

/* -------------------------------------------------------------------------- */
RWSC_API RWSCConnectionID RWSCConnect (const RWSChar * const szTargetAddress,
                                       RWSUInt32 nPort, RWSBool bUnicodeTarget,
                                       RWSBool bBigEndian,
                                       RWSUInt32 nConnectTimeoutMs);

RWSC_API void RWSCDisconnect (RWSCConnectionID ConnectionID);

/* set default network operation timeout in milliseconds */
RWSC_API void RWSCSetTimeout (RWSCConnectionID ConnectionID,
                              RWSUInt32 TimeoutMs);

/* set pointer to a flag which when set will abort the current operation */
RWSC_API void RWSCSetAbortFlag (RWSCConnectionID ConnectionID,
                                RWSBool *pbAbortFlag);

/* receive messages */
RWSC_API void RWSCAddMessageCallback (RWSCConnectionID ConnectionID,
                                      pfRWSMessageCallback pCallBack,
                                      void *pUserData);

RWSC_API void RWSCRemoveMessageCallback (RWSCConnectionID ConnectionID,
                                         pfRWSMessageCallback pCallBack);

/* check for messages sent from target */
RWSC_API RWSBool RWSCCheckForMessages (RWSCConnectionID ConnectionID);

/* send some data */
RWSC_API void RWSCSendData (RWSCConnectionID ConnectionID,
                            const RWSByte * const pData, RWSUInt32 nDataSize);

/* send a file */
RWSC_API void RWSCSendFile (RWSCConnectionID ConnectionID,
                            const RWSChar * const szFile);

/* is connection a big endian target? */
RWSC_API RWSBool RWSCIsBigEndian (RWSCConnectionID ConnectionID);

/* is connection a unicode target? */
RWSC_API RWSBool RWSCIsUnicode (RWSCConnectionID ConnectionID);

/* -------------------------------------------------------------------------- */
#ifdef __cplusplus
}
#endif


#endif /* CSL_RWS_COMMS */
