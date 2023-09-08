#include <stdio.h>
#include <kernel.h>
#include <sifrpc.h>
#include "server.h"
#include "rpcmessages.h"
#include <introld.h>

ModuleInfo Module = {"Comms Stationary Module", 0x0102};

/* globals */
sceSifQueueData QueueData;
sceSifServeData ServerData;
static u_char Buffer [SSIZE];

/* prototypes */
int main (int argc, char *argv[]);

/*----------------------------------------------------------------------------*/
/* Comms RPC message handling function.                                       */
/*                                                                            */
/* Calls correct comms module function according to RPC function ID.          */
/*                                                                            */
/* Returns Pointer to RPC return data if applicable.                          */
/*----------------------------------------------------------------------------*/
void *RPCCommsHandler (unsigned int nFunction, void *pData, int nSize)
{
   switch (nFunction)
   {
   case gfcServerStart: /* initialise comms server */
      {
         /* first int in buffer is [out] return value ([in] Port), rest is IP address return */
         char *szIPAddress = ((char *)(pData)) + sizeof (int);

         /* call initialise */
         if ((*(int *)pData = ServerInitialise (*(int *)pData,
                                                szIPAddress)) != 0)
         {
            printf ("Failed to initialise server!\n");
         }

         return pData;
      }

   case gfcServerListen: /* listen */
      {
         /* First int in pData is the timeout in ms */
         int nBytes = ServerListen (pData, nSize, *((int *)pData));
         *((int *)pData) = nBytes;
         return pData;
      }

   case gfcServerMessage: /* send message to client */
      {
         /* returns number of bytes sent */
         const int nTimeoutMS = 100;
         int nBytes = ServerMessage (pData, nSize, nTimeoutMS);
         *((int *)pData) = nBytes;
         return pData;
      }

   case gfcServerStop: /* stop the server */
      {
         ServerStop ();
         return 0;
      }
   }
   return 0;
}

/*----------------------------------------------------------------------------*/
/* Thread processing function to handle RPC calls.                            */
/*                                                                            */
/* Returns 0.                                                                 */
/*----------------------------------------------------------------------------*/
static int RPCThreadLoop ()
{
   /* init */
   sceSifInitRpc (0);
   /* set queue */
   sceSifSetRpcQueue (&QueueData, GetThreadId ());
   /* register function */
   sceSifRegisterRpc (&ServerData, RPC_COMMS_FUNC, RPCCommsHandler,
                      (void *)Buffer, 0, 0, &QueueData);
   /* the RPC loop */
   sceSifRpcLoop (&QueueData);
   return 0;
}

/*----------------------------------------------------------------------------*/
/* Main entry point for comms IOP module.                                     */
/*                                                                            */
/* Kicks of a thread to listen for RPC calls from the comms server library.   */
/*                                                                            */
/* Returns -1 for failure.                                                    */
/*----------------------------------------------------------------------------*/

int start(int argc, char *argv[])   /* Params REQUIRED for ProDG build, old warns in Metrowerks. */
{
   struct ThreadParam ThreadParams;
   int Thread;
   
   argc = argc;       /* Fix to stop compiler warnings. */
   argv = argv;       /* Fix to stop compiler warnings. */

   FlushDcache ();
   CpuEnableIntr ();

   ThreadParams.attr         = TH_C;
   ThreadParams.entry        = RPCThreadLoop;
   ThreadParams.initPriority = 32;/*BASE_priority;*/
   ThreadParams.stackSize    = 0x800;
   ThreadParams.option       = 0;
   Thread = CreateThread (&ThreadParams);
   if (Thread > 0)
   {
      StartThread (Thread, 0);
      return 0;
   }
   return NO_RESIDENT_END;
}

int main(int argc, char *argv[])
{
   return(start(argc, argv));      /* Needed for ProDG version. */
}

