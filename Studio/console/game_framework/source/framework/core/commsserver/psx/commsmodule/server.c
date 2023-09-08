#include <kernel.h>
#include <memory.h>
#include <inet.h>
#include <inetctl.h>
#include "server.h"
#include "rpcmessages.h"
#include <stdio.h>

int g_nPort = 5609;
static int ConnectionID = 0;

/* event handling data */
static int g_IFEventID = 0;
static int g_IFID = 0;


/*
* show_info() and netstat() taken from netstat.c, a Sony IOP inet sample .irx.
*
* Useful for monitoring the state of the send / receive buffers and other connection info.
*/
/*
static sceInetInfo_t info_tbl[256];
static char buf[1024];

static void show_info(sceInetInfo_t *p)
{
   int n;

   switch(p->proto){
   case sceINETI_PROTO_TCP: printf("tcp  "); break;
   case sceINETI_PROTO_UDP: printf("udp  "); break;
   case sceINETI_PROTO_IP:  printf("ip   "); break;
   default: printf("0x%x ", p->proto); break;
   }
   printf(" %6d", p->recv_queue_length);
   printf(" %6d", p->send_queue_length);
   sceInetAddress2String(buf, sizeof(buf), &p->local_adr);
   n = printf(" %s:%d", buf, p->local_port);
   printf("%*s", 21 - n, "");
   sceInetAddress2String(buf, sizeof(buf), &p->remote_adr);
   n = printf(" %s:%d", buf, p->remote_port);
   printf("%*s", 21 - n, "");
   switch(p->state){
   case sceINETI_STATE_UNKNOWN:     printf(" UNKNOWN");  break;
   case sceINETI_STATE_CLOSED:      printf(" CLOSED");   break;
   case sceINETI_STATE_CREATED:     printf(" CREATED");  break;
   case sceINETI_STATE_OPENED:      printf(" OPENED");   break;
   case sceINETI_STATE_LISTEN:      printf(" LISTEN");   break;
   case sceINETI_STATE_SYN_SENT:    printf(" SYN_SENT"); break;
   case sceINETI_STATE_SYN_RECEIVED:   printf(" SYN_RECEIVED"); break;
   case sceINETI_STATE_ESTABLISHED: printf(" ESTABLISHED"); break;
   case sceINETI_STATE_FIN_WAIT_1:     printf(" FIN_WAIT_1");  break;
   case sceINETI_STATE_FIN_WAIT_2:     printf(" FIN_WAIT_2");  break;
   case sceINETI_STATE_CLOSE_WAIT:     printf(" CLOSE_WAIT");  break;
   case sceINETI_STATE_CLOSING:     printf(" CLOSING");  break;
   case sceINETI_STATE_LAST_ACK:    printf(" LACK_ACK"); break;
   case sceINETI_STATE_TIME_WAIT:      printf(" TIME_WAIT");   break;
   default: printf(" 0x%x", p->state); break;
   }
   printf("\n");
}

static int netstat()
{
   sceInetInfo_t *p;
   int n;

   if(0 > (n = sceInetControl(0, sceINETC_CODE_GET_INFO,
         info_tbl, sizeof(info_tbl)))){
      printf("sceInetControl -> %d\n", n);
      return(n);
   }

   if(n == 0)
      printf("\nNo Info\n");
   else{
      printf("\nProto Recv-Q Send-Q Local Address"
         "        Foreign Address      State\n");
      for(p = info_tbl; 0 < n--; ++p)
         show_info(p);
   }
   return(0);
}
*/
/*
*  End of code taken from netstat.c
*/

/*----------------------------------------------------------------------------*/
/* Thread processing to listen for connections to the console.                */
/*                                                                            */
/* The process of creating a connection blocks, so in this case, it's done in */
/* a thread.  The thread will terminate after a connection is made.           */
/*                                                                            */
/* Returns 0 if successful, non-zero if failed.                               */
/*----------------------------------------------------------------------------*/
static int ConnectionThreadLoop ()
{
   int RetVal;
   int Flags;

   struct sceInetParam InetParams;
   memset (&InetParams, 0, sizeof (struct sceInetParam));

   /* generate an address structure that defines any address */
   RetVal = sceInetName2Address (0, &InetParams.remote_addr, NULL, -1, -1);
   if (RetVal != sceINETE_OK)
   {
      printf("Server: sceInetName2Address() failed.(%d)\n", RetVal);
      return -1;
   }

   /* create a connection */
   InetParams.type = sceINETT_LISTEN; /* TCP listener */
   InetParams.local_port = g_nPort;
   InetParams.remote_port = sceINETP_ANY; /* set on first receive for TCP */

   ConnectionID = sceInetCreate (&InetParams);
   if (0 == ConnectionID)
   {
      printf("Server: Connection failed.\n");
      return -2;
   }

   /* sceInetOpen will block with TCP (until a connection is made or there's an error). */
   if (sceINETE_OK != (RetVal = sceInetOpen (ConnectionID, -1)))
   {
      printf ("Server: sceInetOpen() failed.(%d)\n", RetVal);
      sceInetClose (ConnectionID, 0);
      ConnectionID = 0;
      return -3;
   }

   /* Disable the 'Nagle Algorithm' that groups up data sent by sceInetSend() to reduce the number of
         packets sent - it doesn't work well for rapid sending of small packets (e.g. trace messages) */
   Flags = sceINETC_FLAGS_NODELAY;
   RetVal = sceInetControl(ConnectionID, sceINETC_CODE_SET_FLAGS, &Flags, sizeof(Flags));
   if ( sceINETE_OK != RetVal )
   {
      printf("Server: sceInetControl() failed.(%d)\n", RetVal );
      return -4;
   }

   printf ("Client connected, ID = %d\n", ConnectionID);

   return 0;
}

/*----------------------------------------------------------------------------*/
/* Create and start a thread to listen for connections.                       */
/*                                                                            */
/* Returns 0 if successful, non-zero if failed.                               */
/*----------------------------------------------------------------------------*/
static int StartConnectionThread ()
{
   int nThread;
   struct ThreadParam ThreadParams;
   ThreadParams.attr         = TH_C;
   ThreadParams.entry        = ConnectionThreadLoop;
   ThreadParams.initPriority = 32;/*BASE_priority;*/
   ThreadParams.stackSize    = 0x800;
   ThreadParams.option       = 0;
   nThread = CreateThread (&ThreadParams);
   if (nThread > 0)
   {
      StartThread (nThread, 0);
      return 0;
   }
   return -1;
}

/*----------------------------------------------------------------------------*/
/* Event handler function.                                                    */
/*                                                                            */
/* Handles net "interface up" event                                           */
/*----------------------------------------------------------------------------*/
static void EventHandlerFunc (int id, int type)
{
   if (sceINETCTL_IEV_Start == type)
   {
      int ret;
      g_IFID = id;
      if (KE_OK != (ret = SetEventFlag (g_IFEventID, 0x04))) /* 0x04 = start */
         printf ("SetEventFlag failed. ret = %d\n", ret);
   }
}

/*----------------------------------------------------------------------------*/
/* Initialise the comms module.                                               */
/*                                                                            */
/* If a client is already connected, disconnect them.                         */
/* Then kick of a thread to listen for connections.                           */
/*                                                                            */
/* Returns 0 if successful, non-zero if failed.                               */
/*----------------------------------------------------------------------------*/
int ServerInitialise (int nPort, char * const szIPAddress)
{
   int nRetVal;
   int nState = 0;
   int nInterfaceNum;
   int nInterface = 0;
   int anIFIDs[2];
   
   /* "interface up" event structures */
   struct sceInetCtlEventHandlers EventHandlers;
   struct EventFlagParam EventFlagParams;
   struct sceInetAddress Address;

   g_nPort = nPort;
   szIPAddress[0] = 0;
   if (ConnectionID != 0)
   {
      /* disconnect */
      printf ("Disconnecting client, ID = %d\n", ConnectionID);
      sceInetClose (ConnectionID, 0);/*-1);*/
      /* check sceInetClose return value? */
   }
   ConnectionID = 0;
   
   /* kick of a thread to listen for connections */
   StartConnectionThread ();

   /* try to find an interface that is already started */
   if ( 0 == g_IFID )
   {
      nInterfaceNum = sceInetGetInterfaceList(&anIFIDs[0], sizeof(anIFIDs) );
      if ( nInterfaceNum < 0 )
      {
         printf("ServerInitialise: sceInetGetInterfaceList failed, code [%d]\n", nInterfaceNum);
      }

      while ( nInterface < nInterfaceNum )
      {
         nRetVal = sceInetCtlGetState( anIFIDs[nInterface], &nState );
         if ( nRetVal != 0 )
         {
            printf("ServerInitialise: sceInetCtlGetState failed, code [%d]\n", nRetVal);
         }
   
         if ( nState == sceINETCTL_S_STARTED )
         {
            g_IFID = anIFIDs[nInterface];
            break;
         }

         nInterface++;
      }
   }

   if (0 == g_IFID) /* interface isn't already up */
   {
      /* wait for interface to start up */
      EventFlagParams.attr = EA_MULTI;
      EventFlagParams.initPattern = 0;
      EventFlagParams.option = 0;

      g_IFEventID = CreateEventFlag (&EventFlagParams);
      if (g_IFEventID < 0)
         return -1;

      /* set up event handler function */
      EventHandlers.func = EventHandlerFunc;

      nRetVal = sceInetCtlRegisterEventHandler (&EventHandlers);
      if (nRetVal < 0)
         return -2;

      /* wait for net interface to start */
      nRetVal = WaitEventFlag (g_IFEventID, 0x04, EW_OR, NULL); /* 0x04 = start */
      if (nRetVal != KE_OK)
         return -3;
   }

   /* get receiving address */
   if ((nRetVal = sceInetInterfaceControl (g_IFID,
                                           sceInetCC_GetAddress,
                                           &Address,
                                           sizeof (struct sceInetAddress))) < 0)
   {
      return -4;
   }

   if (sceInetAddress2String (szIPAddress, 128, &Address) < 0)
      return -5;

   return 0; /* OK */
}

/*----------------------------------------------------------------------------*/
/* Listen for RWS message data coming in from the client.                     */
/*                                                                            */
/* Only poll for data if a client is connected.                               */
/* See GFMesg.h                                                               */
/*                                                                            */
/* Returns the number of bytes received.                                      */
/*----------------------------------------------------------------------------*/
int ServerListen (void *pData, int nDataSize, int nTimeout)
{
   /* if connected, try to receive some bytes */
   if (ConnectionID != 0)
   {
      int Flags;
      int nRetVal;

      nDataSize -= sizeof (int); /* 1st returned byte is number of bytes */
      nRetVal = sceInetRecv (ConnectionID, (int *)pData + 1,
                                nDataSize, &Flags, nTimeout);

      return nRetVal;
   }
   return 0;
}

/*----------------------------------------------------------------------------*/
/* Send an RWS message to the client.                                         */
/*                                                                            */
/* Only send data if a client is connected.                                   */
/* See GFMesg.h                                                               */
/*                                                                            */
/* Returns the number of bytes sent.                                          */
/*----------------------------------------------------------------------------*/
int ServerMessage (void *pData, int nDataSize, int nTimeout)
{
   /* send message to client */
   if (ConnectionID != 0)
   {
      int Flags = 0;/*sceINETF_PSH;*/ /*sceINETF_URG*/
      int nRetVal;

      nRetVal = sceInetSend (ConnectionID, pData, nDataSize, &Flags, nTimeout);
      
      return nRetVal;
   }
   return 0;
}

/*----------------------------------------------------------------------------*/
/* Stop the comms server.                                                     */
/*                                                                            */
/* If a client is connected, close the connection.                            */
/*                                                                            */
/* Returns 0.                                                                 */
/*----------------------------------------------------------------------------*/
int ServerStop ()
{
   if (ConnectionID)
   {
      /* disconnect */
      sceInetClose (ConnectionID, 0);
   }
   ConnectionID = 0;

   return 0;
}
