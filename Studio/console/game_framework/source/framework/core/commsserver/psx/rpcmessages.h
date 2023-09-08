#ifndef __RPC_MESSAGES_H__
#define __RPC_MESSAGES_H__


#define RPC_COMMS_FUNC 0x11 /* function ID */

typedef enum
{
	gfcServerStart,
	gfcServerListen,
	gfcServerMessage,
	gfcServerStop
} RPC_GFCServerCall;

#define SSIZE 0x8000 /* 32K buffer */


#endif /*__RPC_MESSAGES_H__*/