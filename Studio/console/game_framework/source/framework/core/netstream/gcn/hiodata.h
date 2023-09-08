/*****************************************************************************
*
* File :     hiodata.h
*
* Abstract : RWS Network Module
*
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
* Copyright (c) 2000 Criterion Software Ltd.
* All Rights Reserved.
*
* RenderWare is a trademark of Canon Inc.
*
*****************************************************************************/

#ifndef __RWS_GCN_GIO_DATA_H__
#define __RWS_GCN_GIO_DATA_H__

// ************************************************************************ //
// If you change any values in this file, please ensure you recompile both the
// game framework, and the CommsServer project which runs on the host machine.
// ************************************************************************ //

// Size of buffer used to transfer network stream data
const u32 HIO_MAX_DATA_SIZE = 0xF000; // 60k is about as much as we can send.
// Size of handshake area used in startup sequence
const u32 HIO_HS_DATA_SIZE = 0x20; // Smallest block is 32 bytes

// Addresses used by RWS network code
const u32 HIO_HOST_TO_TARG_ADDR = 0x1000;
const u32 HIO_TARG_TO_HOST_ADDR = HIO_HOST_TO_TARG_ADDR + HIO_MAX_DATA_SIZE;
const u32 HIO_HOST_HANDSHAKE_ADDR = 0x500;
const u32 HIO_TARG_HANDSHAKE_ADDR = HIO_HOST_HANDSHAKE_ADDR + HIO_HS_DATA_SIZE;

enum RWSMail
{
   rwsmIsDataPending = 100, // Target asks host if any data pending
   rwsmNoDataToRead,        // Host has data to be read
   rwsmDataToRead,          // Host has data to be read
   rwsmDataToSend,          // Target has data to send to host 
   rwsmIPAddress,           // Target asks for host's IP address
   rwsmLastCommand
};

// ************************************************************************ //

#endif // __RWS_GCN_GIO_DATA_H__