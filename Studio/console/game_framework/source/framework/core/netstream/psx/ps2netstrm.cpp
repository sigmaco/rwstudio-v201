/*****************************************************************************
*
* File :     ps2netstrm.cpp
*
* Abstract : Network RwStream Implementation PS2 Version
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

#include "precomp.h"

#include "../netstrm.h"

#ifdef RWS_DESIGN


//////////////////////////////////////////////////////////////////
//
// System Includes
//
#include <sifrpc.h>
#include <sifdev.h>
#include <eekernel.h>

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "../netstrm.h"
#include "../../commsserver/psx/rpcmessages.h" // rpc constants
#include "../macros/debugmacros.h"
#include "framework/startup/sky/skyiop.h"

namespace RWS
{
   namespace NetStream
   {
      /**
      *
      * A pointer to the function used to obtain the current game time in milliseconds.
      *
      */

      RwUInt32 (*gpTimeFunc)(void) = 0;

   }  // namespace NetStream
}     // namespace RWS

namespace
{
   // Return value from internal helper functions
   const RwInt32 NETSTREAMOK       = 0;
   // Header wrapping an incoming network packet
   const RwUInt32 PACKETHEADERSIZE = 8;
   // Used to pull data off the network during a RwStreamSkip
   const RwUInt32 SKIPBUFFERSIZE   = 0x1000;
   // Used to read ahead network data to reduce the number of small network
   //       reads when dealing with large packets such as BSPs.
   const RwUInt32 READCACHESIZE    = 32 * 1024;   // 32K
   // Timeout value provided to sceInetRecv and sceInetSend
   const RwInt32 NETTIMEOUTMS      = 10;
   
   // Minimum allowable time between polling - stops iop thread starvation
   const RwUInt32 MINPOLLTIME      = 0;
   const RwUInt32 MAXPOLLTIME      = 11;

   RwUInt32 POLLTIME = MAXPOLLTIME;

   // Used to write network data into a cache to reduce the number of small
   // network writes when dealing with large packets such as BSPs.
   const RwUInt32 WRITECACHESIZE   = 32 * 1024;   // 32K

   // RPC globals
   sceSifClientData g_ClientData;
   RwUInt32 sdata[SSIZE / sizeof(RwUInt32)] __attribute__((aligned(64)));

   // Connection flag - used to keep track of whether a RWS workspace has connected.
   //  Need to know, otherwise trying to send data without a connection will cause
   //  an infinite loop.
   bool bClientConnected = false;

   /* ----------------------------------------------------------------------- */
   typedef struct
   {
      RwUInt32  Port;
      // Current stream position, size and padding
      RwUInt32  PacketPos;
      RwUInt32  PacketSize;
      RwUInt32  PacketPad;
      // Read-ahead cache data
      RwUInt32  CachePos;
      RwUInt32  CachedBytes;
      // Write-cache data
      RwUInt32  WCachePos;
      RwUInt32  WPacketSize;
      // Stream pointers (we can only open one of each type)
      RwStream *ReadStream;
      RwStream *WriteStream;
      // IP address
      RwChar    Address[16];
      // Has the IOP comms server been started successfully?
      RwBool    IOPCommsServerStarted;
   } NetStreamData;

   // Read/Write caches
   RwUInt8 g_ReadCache[READCACHESIZE]   __attribute__((aligned(32)));
   RwUInt8 g_WriteCache[WRITECACHESIZE] __attribute__((aligned(32)));

   /* ----------------------------------------------------------------------- */
   NetStreamData g_NetStreamData = { 0 };

   /*-------------------------------------------------------------------------*/
   /* Load a module.                                                          */
   /*                                                                         */
   /* Returns 0 if successful, otherwise never returns.                       */
   /*-------------------------------------------------------------------------*/
   int LoadModule (char Name[], int nArgLen, const char *Args)
   {
      RWS_FUNCTION("LoadModule");

      RWS_TRACE(Name<<" "<<RWS_VALIDSTRING(Args));

      /* add the fileserver root to the filename */
      char szModule[251];
      sprintf( szModule, "%s%s", RWS::GetRootFileServerPath(), Name );

      /* load module */
      if (sceSifLoadModule (szModule, nArgLen, Args) < 0)
      {
         RWS_ASSERTFAIL("Cannot load module " << szModule << "\nPlease check the definitions of IOP_MODULEPATH, IOP_RWSCOMMSPATH, RWS_AUDIO_MODULEPATH in core\\startup\\skyiop.h");
      };

      RWS_RETURN( 0 );
   }

   /*-------------------------------------------------------------------------*/
   /* Load the necessary modules for the server to run.                       */
   /*                                                                         */
   /* Returns 0 if successful, non-zero otherwise returns the Id of the       */
   /* module that failed to load.                                             */
   /*-------------------------------------------------------------------------*/
   int LoadCommsModules ()
   {
      RWS_FUNCTION("LoadCommsModules");

#if defined (CDROM) || defined (DVDROM)

      #define RWS_NETCNF_ICON    IOP_MODULEPATH_CDROM "SYS_NET.ICO;1"
      #define RWS_NETCNF_ICONSYS IOP_MODULEPATH_CDROM "ICON.SYS;1"
      #define RWS_NETCNF_ARG     "icon=" "cdrom0:" RWS_NETCNF_ICON "\0" "iconsys=" "cdrom0:" RWS_NETCNF_ICONSYS
      #define RWS_INETCTL_ARGS   "-no_decode" "\0" "cdrom0:" IOP_RWSCOMMSPATH_CDROM "NET000.CNF;1"

      if (LoadModule (IOP_MODULEPATH_CDROM "INET.IRX;1", 0, 0) < 0) {RWS_RETURN(1);}
      if (LoadModule (IOP_MODULEPATH_CDROM "NETCNF.IRX;1", sizeof(RWS_NETCNF_ARG), RWS_NETCNF_ARG) < 0) {RWS_RETURN(2);}
      if (LoadModule (IOP_MODULEPATH_CDROM "INETCTL.IRX;1", sizeof(RWS_INETCTL_ARGS), RWS_INETCTL_ARGS) < 0) {RWS_RETURN(3);}
      if (LoadModule (IOP_MODULEPATH_CDROM "USBD.IRX;1", 0, 0) < 0) {RWS_RETURN(4);}

      // Dev9.irx and smap.irx are only required if using the broadband adapter.
      // they can be removed.
      //
      if (LoadModule (IOP_MODULEPATH_CDROM "DEV9.IRX;1", 0, 0) < 0) {RWS_RETURN(5);}
      if (LoadModule (IOP_MODULEPATH_CDROM "SMAP.IRX;1", 0, 0) < 0) {RWS_RETURN(6);}

      if (LoadModule (IOP_RWSCOMMSPATH_CDROM "AN986.IRX;1", 0, 0) < 0) {RWS_RETURN(7);}
      if (LoadModule (IOP_RWSCOMMSPATH_CDROM "RWSCOMMS.IRX;1", 0, 0) < 0){RWS_RETURN(8);}

#else // #ifdef CDROM || DVDROM

#if !defined(ATWINMON)  // The required networking modules will already have been loaded by atwinmon (the inet version)

      #define RWS_NETCNF_ICON    IOP_MODULEPATH "sys_net.ico"
      #define RWS_NETCNF_ICONSYS IOP_MODULEPATH "icon.sys"
      #define RWS_NETCNF_ARG     "icon=" "host0:" RWS_NETCNF_ICON "\0" "iconsys=" "host0:" RWS_NETCNF_ICONSYS
      #define RWS_INETCTL_ARGS   "-no_decode" "\0" "host0:" IOP_RWSCOMMSPATH "net000.cnf"

      if (LoadModule (IOP_MODULEPATH "inet.irx", 0, 0) < 0) {RWS_RETURN(1);}
      if (LoadModule (IOP_MODULEPATH "netcnf.irx", sizeof(RWS_NETCNF_ARG), RWS_NETCNF_ARG) < 0) {RWS_RETURN(2);}
      if (LoadModule (IOP_MODULEPATH "inetctl.irx", sizeof(RWS_INETCTL_ARGS), RWS_INETCTL_ARGS) < 0) {RWS_RETURN(3);}
      if (LoadModule (IOP_MODULEPATH "usbd.irx", 0, 0) < 0){RWS_RETURN(4);}
      if (LoadModule (IOP_RWSCOMMSPATH "an986.irx", 0, 0) < 0){RWS_RETURN(5);}

      // Dev9.irx and smap.irx are only required if using the broadband adapter, if not
      // they can be removed.
      //
      if (LoadModule (IOP_MODULEPATH "dev9.irx", 0, 0) < 0){RWS_RETURN(6);}
      if (LoadModule (IOP_MODULEPATH "smap.irx", 0, 0) < 0){RWS_RETURN(7);}

#endif // # !defined ATWINMON
      
#ifdef NDEBUG
      if (LoadModule (IOP_RWSCOMMSPATH "rwscomms.irx", 0, 0) < 0){RWS_RETURN(8);}
#else // NDEBUG
      if (LoadModule (IOP_RWSCOMMSPATH "rwscommsd.irx", 0, 0) < 0){RWS_RETURN(8);}
#endif // NDEBUG

#endif // #if defined (CDROM) || defined (DVDROM)

      RWS_RETURN( 0 ); // success
   }

   /* ----------------------------------------------------------------------- */
   // Forward declaration
   RwBool NetStreamSkip (void *pData, RwUInt32 Offset);

   /* ----------------------------------------------------------------------- */
   /**
   *
   *  \ingroup NetStream
   *
   *  initializes the network stream library.
   *  Loads the necessary irx modules, starts up the RPC connection with the
   *  Comms irx module, and fills in the IP address string.
   */
   /* ----------------------------------------------------------------------- */
   RwInt32 NetStreamInit (NetStreamData *pData)
   {
      RWS_FUNCTION("NetStreamInit");
    
      static RwBool bIOPModuleLoaded = FALSE;
      RwInt32 nAttempts = 50;

      if ( !bIOPModuleLoaded )
      {
         // initialize RPC
         sceSifInitRpc (0);

         // load modules
         if (LoadCommsModules ())
         {
            RWS_ASSERTFAIL ("LoadCommsModules Failed");

            RWS_RETURN( -1 );
         }

         // bind to server functions
         RwInt32 nStatus = -1;
         while (nAttempts--)
         {
            if (sceSifBindRpc (&g_ClientData, RPC_COMMS_FUNC, 0) < 0)
            {
               RWS_ASSERTFAIL ("Couldn't bind to comms module function\n");
               nStatus = -2;
               break;
            }

            if (sceSifCheckStatRpc ((sceSifRpcData *)&g_ClientData) < 0)
            {
               RWS_ASSERTFAIL ("Comms Module RPC failed\n");
               nStatus = -3;
               break;
            }
   
            if (g_ClientData.serve != 0)
            {
               RWS_TRACE ("Initialize: initialized Server!\n");
               nStatus = 0; // success
               break;
            }

            RWS_TRACE("Retry");

            // Very crude delay
            //
            volatile int i;
         
            i = 0x10000;

            while (i) {i--;}
         }

         RWS_ASSERT (0 == nStatus, "Failed to initialize\n");

         if (nStatus)
         {
            RWS_RETURN(nStatus);
         }
         else
         {
            bIOPModuleLoaded = TRUE;
         }
      }

      // Call function to initialize
      *sdata = g_NetStreamData.Port;
      if ( sceSifCallRpc (&g_ClientData, gfcServerStart, 0,
                          sdata, sizeof (RwInt32),
                          sdata, sizeof (int) + 128, 0, 0) != 0)
      {
         RWS_ASSERTFAIL("Comms Module server failed to start\n");
         RWS_RETURN(-4);
      }
      else
      {
         g_NetStreamData.IOPCommsServerStarted = TRUE;
      }

      // Pull out the IP address string after first int in RPC buffer
      // Fill in ip address
      *pData->Address = 0;
      if (0 == *sdata)
      {
         rwstrcpy (pData->Address, reinterpret_cast<RwChar *>(sdata + 1));
         RWS_RETURN(NETSTREAMOK);
      }

      RWS_RETURN(*sdata);
   }

   /* ----------------------------------------------------------------------- */
   /**
   *
   *  \ingroup NetStream
   *
   *  Resets the state of the NetStream module and then reinitializes it. Used
   *  internally by the NetStream module when a connection is lost.
   */
   /* ----------------------------------------------------------------------- */
   void NetStreamReset ( void )
   {
      // need to remember what port was being used before shutting down the module
      RwUInt32 Port = g_NetStreamData.Port;

      RWS::NetStream::Shutdown();
      RWS::NetStream::Init(Port, RWS::NetStream::gpTimeFunc);
   }

   /* ----------------------------------------------------------------------- */
   /**
   *
   *  \ingroup NetStream
   *
   *  Reads incoming network data into a buffer, to reduce the number of small
   *  network reads.
   *
   *  Reads one cache-full, or up to end of the stream, whichever is smaller.
   */
   /* ----------------------------------------------------------------------- */
   RwBool RCacheData (NetStreamData *pData)
   {
      RWS_FUNCTION ("RCacheData");
      
      RWS_ASSERT (pData->PacketSize,   "No data to cache");
      RWS_ASSERT (!pData->CachePos,    "Not at start of cache");
      RWS_ASSERT (!pData->CachedBytes, "Bytes already cached");

      // Work out how much we can read
      RwUInt32 nBytesToRead = pData->PacketSize - pData->PacketPos;

      // Ensure we can't read more than will fit in the cache
      if (nBytesToRead > READCACHESIZE)
         nBytesToRead = READCACHESIZE;

      // Now read the data off the network
      RwUInt8 *pRecvBuffer = g_ReadCache;
      RwUInt8 *pBufferEnd  = pRecvBuffer + nBytesToRead;

      RwInt32 nBytesRead;
      while (pRecvBuffer < pBufferEnd)
      {
         RwUInt32 nLeft = pBufferEnd - pRecvBuffer;
         RwUInt32 nBytesToRead = (nLeft + sizeof (RwInt32) < SSIZE) ?
                                          nLeft + sizeof (RwInt32) : SSIZE;
         *sdata = NETTIMEOUTMS; // Timeout for sceInetRecv
         sceSifCallRpc (&g_ClientData, gfcServerListen, 0,
                        sdata, nBytesToRead, sdata, nBytesToRead, 0, 0);

         // if we failed to read, abort
         nBytesRead = *(reinterpret_cast<RwInt32 *>(sdata));
         if (nBytesRead < 0)
         {
            RWS_TRACE ("Client connection lost! Inet Error Code: error "
                         << nBytesRead);

            NetStreamReset();
            RWS_RETURN(0);
         }

         // copy data we read into header and update pointer
         memcpy (pRecvBuffer, sdata + 1, nBytesRead);
         pRecvBuffer += nBytesRead;
      }

      // Set amount of data we cached
      pData->CachedBytes = nBytesToRead;

      RWS_RETURN (TRUE);
   }

   /* ----------------------------------------------------------------------- */
   /**
   *
   *  \ingroup NetStream
   *
   *  Reads an 8-byte packet header off the network.  If there's no network data
   *  pending, returns 0.
   *
   *  Once a header is obtained, reads as much incoming data as it can into the
   *  cache.
   */
   /* ----------------------------------------------------------------------- */
   RwUInt32 ReadPacketHeader (NetStreamData *pData)
   {
      RWS_FUNCTION ("ReadPacketHeader");

      RWS_ASSERT (!pData->PacketSize,  "Packet size zero. Previous stream not closed?");
      RWS_ASSERT (!pData->PacketPos,   "Packet position zero. Previous stream not closed?");
      RWS_ASSERT (!pData->CachePos,    "Cache position zero. Previous stream not closed?");
      RWS_ASSERT (!pData->CachedBytes, "Cached bytes zero. Previous stream not closed?");
      RWS_ASSERT (RWS::NetStream::gpTimeFunc, "No 'get time' function provided. See RWS::NetStream::Init.");

      // If it's less than "POLLTIME" ms since last call, return, so we don't starve iop threads
      static RwUInt32 PreviousTime = RWS::NetStream::gpTimeFunc();

      RwUInt32 CurrentTime = RWS::NetStream::gpTimeFunc();

      // Sanity check
      //
      if (CurrentTime < PreviousTime)
      {
         PreviousTime = CurrentTime;
      }

      if (CurrentTime - PreviousTime < POLLTIME)
         RWS_RETURN (0);


      PreviousTime = CurrentTime;

      RwInt32 nBytesRead = 0;
      RwUInt32 Header[PACKETHEADERSIZE / sizeof (RwUInt32)];
      RwUInt8 *pRecvBuffer = reinterpret_cast<RwUInt8 *>(Header);
      RwUInt8 *pBufferEnd = pRecvBuffer + sizeof (Header);

      while (pRecvBuffer < pBufferEnd)
      {
         // if no data read yet, use timeout of 0 (first int in sdata is timeout)
         *sdata = (pRecvBuffer == reinterpret_cast<RwUInt8 *>(Header)) ? 0 : NETTIMEOUTMS;

         sceSifCallRpc (&g_ClientData, gfcServerListen, 0,
                        sdata, pBufferEnd - pRecvBuffer + sizeof (RwInt32),
                        sdata, pBufferEnd - pRecvBuffer + sizeof (RwInt32),
                        0, 0);

         // if we failed to read header, abort
         nBytesRead = *(reinterpret_cast<RwInt32 *>(sdata));
         if (nBytesRead < 0)
         {
            RWS_TRACE ("Client connection lost! Inet Error Code: error "
                                                                << nBytesRead);


            NetStreamReset();
            RWS_RETURN (0);
         }
         else if (0 == nBytesRead)
         {
            if (pRecvBuffer == reinterpret_cast<RwUInt8 *>(Header))
            {
               // Failed to receive data poll slower
               //
               if (POLLTIME < MAXPOLLTIME)
               {
                  POLLTIME += 1;
               }

               // Not read a chunk header yet, so return 0 if no data pending
               RWS_RETURN(0); // no data pending
            }
         }

         // Receiving data poll quicker
         //
         if (POLLTIME > MINPOLLTIME)
         {
            POLLTIME -=1;
         }

         // copy data we read into header and update pointer
         if (nBytesRead)
         {
            memcpy (pRecvBuffer, sdata + 1, nBytesRead);
            pRecvBuffer += nBytesRead;
         }
      }

      // We've read the header now, so return number of bytes in stream packet
      pData->PacketSize = *Header;
      pData->PacketPad = *(Header + 1);

      // Attempt to read a cache block
      RCacheData (pData);
      
      RWS_RETURN (pData->PacketSize);
   }

   /* ----------------------------------------------------------------------- */
   /**
   *
   *  \ingroup NetStream
   *
   *  Called by RwStreamRead etc. to read data off the network.
   *  Data is pulled out of the network cache if possible.  Once the cache is
   *  empty, another cache block is read off the network.
   */
   /* ----------------------------------------------------------------------- */
   RwUInt32 ReadPacketData (NetStreamData *pData, void *pBuffer, RwUInt32 Len)
   {
      RWS_FUNCTION("ReadPacketData");

      if (pData->PacketPos + Len > pData->PacketSize)
         Len = pData->PacketSize - pData->PacketPos;

      RwUInt8 *pOutBuffer = reinterpret_cast<RwUInt8 *>(pBuffer);
      RwUInt8 *pBufferEnd = pOutBuffer + Len;
      while (pOutBuffer < pBufferEnd)
      {
         // Work out how much we can read from the cache
         RwUInt32 nToCopy = pData->CachedBytes - pData->CachePos;
         if (nToCopy > static_cast<RwUInt32>(pBufferEnd - pOutBuffer))
            nToCopy = static_cast<RwUInt32>(pBufferEnd - pOutBuffer);

         // Copy data from cache and update data
         if (nToCopy)
         {
            memcpy (pOutBuffer, g_ReadCache + pData->CachePos, nToCopy);

            pData->CachePos  += nToCopy;
            pData->PacketPos += nToCopy;
            pOutBuffer       += nToCopy;
         }

         // Check if we need to cache more data
         if (pData->CachedBytes == pData->CachePos)
         {
            // Read another cache block
            pData->CachedBytes = pData->CachePos = 0;

            if (!RCacheData (pData))
            {
               NetStreamReset();
               RWS_RETURN (pBufferEnd - pOutBuffer); // Network error occurred
            }
         }
      }

      RWS_RETURN (Len);
   }

   /* ----------------------------------------------------------------------- */
   /**
   *
   *  \ingroup NetStream
   *
   *  Called by WCacheData or NetWStreamClose, to write data onto the network.
   *
   *  Wait until all data is sent (or a network error occurred) before returning.
   */
   /* ----------------------------------------------------------------------- */
   RwUInt32 WritePacketData (NetStreamData *pData, const void *pBuffer,
                              RwUInt32 Len)
   {
      RWS_FUNCTION("WritePacketData");

      const RwInt8 *pSendBuffer = reinterpret_cast<const RwInt8 *>(pBuffer);
      const RwInt8 *pBufferEnd = pSendBuffer + Len;

      while (pSendBuffer < pBufferEnd )
      {
         RwUInt32 nLeft = pBufferEnd - pSendBuffer;
         RwUInt32 nBytesToSend = (nLeft < SSIZE) ? nLeft : SSIZE;

         // Copy data to send into RPC buffer
         memcpy (sdata, pSendBuffer, nBytesToSend);

         // This call returns how many bytes of data it sent
         sceSifCallRpc (&g_ClientData, gfcServerMessage, 0,
                        sdata, nBytesToSend, sdata, nBytesToSend, 0, 0);

         RwInt32 nBytesSent = *reinterpret_cast<int *>(sdata);

         if (nBytesSent >= 0)
         {
            pSendBuffer += nBytesSent;
         }
         else
         {
            // if < 0, comms error
            NetStreamReset();

            RWS_RETURN(nBytesSent);
         }
      }

      RWS_RETURN(Len);
   }

   /* ----------------------------------------------------------------------- */
   /**
   *
   *  \ingroup NetStream
   *
   *  Writes data into a buffer, to reduce the number of small network writes.
   *
   *  Writes one cache-full, or up to end of the stream, whichever is smaller.
   */
   /* ----------------------------------------------------------------------- */
   RwBool WCacheData (NetStreamData *pData, const void *pBuffer, RwUInt32 Len)
   {
      RWS_FUNCTION ("WCacheData");

      RWS_ASSERT (pData->WPacketSize, "No data to cache");
      RWS_ASSERT (pData->WCachePos < WRITECACHESIZE, "End of cache reached");

      // Write the data
      const RwUInt8 *pSendBuffer = reinterpret_cast<const RwUInt8 *>(pBuffer);
      const RwUInt8 *pBufferEnd = pSendBuffer + Len;

      while (pSendBuffer < pBufferEnd)
      {
         // Work out how much we can write
         RwInt32 nBytesToWrite = WRITECACHESIZE - pData->WCachePos;

         // Only write data remaining...
         if (nBytesToWrite > pBufferEnd - pSendBuffer)
            nBytesToWrite = pBufferEnd - pSendBuffer;

         // Copy data from buffer into cache
         memcpy (g_WriteCache + pData->WCachePos, pSendBuffer, nBytesToWrite);

         // Update cache variables
         pData->WCachePos += nBytesToWrite;

         // If cache is full, write it to the network
         if (WRITECACHESIZE == pData->WCachePos)
         {
            pData->WCachePos = 0;
            if (WritePacketData (pData, g_WriteCache, WRITECACHESIZE)
                                                               < WRITECACHESIZE)
            {
               NetStreamReset();
               RWS_RETURN (FALSE);
            }
         }

         // Update buffer pointer
         pSendBuffer += nBytesToWrite;
      }

      RWS_RETURN (TRUE);
   }

   /* ----------------------------------------------------------------------- */
   /* -------------------- Custom Stream Callbacks -------------------------- */
   /* ----------------------------------------------------------------------- */
   /**
   *
   *  \ingroup NetStream
   *
   *  Called by RwStreamClose on a read stream, to read remaining network
   *  data.
   */
   /* ----------------------------------------------------------------------- */
   RwBool NetRStreamClose (void *pData)
   {
      RWS_FUNCTION("NetStreamClose");

      NetStreamData *pRecvData = reinterpret_cast<NetStreamData *>(pData);

      // Increase size so we're allowed to read the padding region
      pRecvData->PacketSize += pRecvData->PacketPad;
      pRecvData->PacketPad = 0;

      // Skip to end of stream packet, ready to read next one
      RwUInt32 nToSkip = pRecvData->PacketSize - pRecvData->PacketPos;
      if (nToSkip)
         if (!NetStreamSkip (pData, nToSkip))
            NetStreamReset (); // network failed

      // Reset globals
      pRecvData->PacketPos  = pRecvData->PacketSize = 0;
      pRecvData->CachePos   = pRecvData->CachedBytes = 0;
      pRecvData->ReadStream = 0;

      RWS_RETURN(TRUE);
   }

   /* ----------------------------------------------------------------------- */
   /**
   *
   *  \ingroup NetStream
   *
   *  Called by RwStreamClose, on a write stream.
   */
   /* ----------------------------------------------------------------------- */
   RwBool NetWStreamClose (void *pData)
   {
      RWS_FUNCTION ("NetWStreamClose");

      NetStreamData *pSendData = reinterpret_cast<NetStreamData *>(pData);

      // Flush the write cache if there's any data still in it
      if (pSendData->WCachePos)
      {
         if (WritePacketData (pSendData, g_WriteCache, pSendData->WCachePos)
                                                         < pSendData->WCachePos)
         {
            pSendData->WCachePos = pSendData->WPacketSize = 0;
            pSendData->WriteStream = 0;
            NetStreamReset();
            RWS_RETURN (FALSE);
         }
      }

      // Reset globals
      pSendData->WCachePos = pSendData->WPacketSize = 0;
      pSendData->WriteStream = 0;

      RWS_RETURN (TRUE);
   }

   /* ----------------------------------------------------------------------- */
   /**
   *
   *  Calculate the amount of data remaining in the stream.
   *
   *  \return Returns the number of bytes remaining in the stream.
   *
   */
   /* ----------------------------------------------------------------------- */
   RwUInt32 GetUnReadData(void *pData)
   {
      RWS_FUNCTION("GetUnReadData");

      NetStreamData *pRecvData = reinterpret_cast<NetStreamData *>(pData);

      if (pRecvData->PacketPos < pRecvData->PacketSize)
      {
         RWS_RETURN (pRecvData->PacketSize - pRecvData->PacketPos);
      }

      RWS_RETURN (0);
   }

   /* ----------------------------------------------------------------------- */
   /**
   *
   *  \ingroup NetStream
   *
   *  Called by RwStreamRead etc. on a read stream, to read network data into
   *  the supplied buffer.
   *
   *  \note If pBuffer or Length is zero, returns the number of bytes remaining
   *  in the stream.
   */
   /* ----------------------------------------------------------------------- */
   RwUInt32 NetStreamRead (void *pData, void *pBuffer, RwUInt32 Length)
   {
      RWS_FUNCTION("NetStreamRead");

      // Return how much data is left to read
      //
      if (pBuffer == 0 || Length == 0)
      {
         RWS_RETURN(GetUnReadData (pData));
      }

      // Now read the data the client requested
      RWS_RETURN(ReadPacketData (reinterpret_cast<NetStreamData *>(pData),
                                 pBuffer, Length));
   }

   /* ----------------------------------------------------------------------- */
   /**
   *
   *  \ingroup NetStream
   *
   *  Called by RwStreamWrite etc. on a write stream, to write stream data onto
   *  the network.
   */
   /* ----------------------------------------------------------------------- */
   RwBool NetStreamWrite (void *pData, const void *pBuffer, RwUInt32 Length)
   {
      RWS_FUNCTION("NetStreamWrite");

      // Cache the data
      if (!WCacheData (reinterpret_cast<NetStreamData *>(pData),
                       pBuffer, Length))
      {
         RWS_RETURN(FALSE);
      }

      RWS_RETURN(TRUE);
   }

   /* ----------------------------------------------------------------------- */
   /**
   *
   *  \ingroup NetStream
   *
   *  Called by RwStreamSkip on a read stream, to skip past data in the stream.
   *  Actually reads the data off the network into a temporary buffer, to ensure
   *  than the network state in sync.
   */
   /* ----------------------------------------------------------------------- */
   RwBool NetStreamSkip (void *pData, RwUInt32 Offset)
   {
      RWS_FUNCTION("NetStreamSkip");

      char Buffer[SKIPBUFFERSIZE];

      RwUInt32 nLeft = Offset;
      do
      {
         RwUInt32 nToRead = (nLeft < SKIPBUFFERSIZE) ? nLeft : SKIPBUFFERSIZE;

         if (NetStreamRead (pData, Buffer, nToRead) < nToRead)
         {
            NetStreamReset();
            RWS_RETURN(FALSE);
         }

         nLeft -= nToRead;
      } while (nLeft > 0);

      RWS_RETURN(TRUE);
   }

} // namespace

namespace RWS
{
   namespace NetStream
   {
      /* -------------------------------------------------------------------------- */
      /* ------------------------------- Interface -------------------------------- */
      /* -------------------------------------------------------------------------- */

      /**
      *
      *  \ingroup NetStream
      *
      *  Initialization of NetStream library.
      *
      *  \param Port The port number to listen for connections on.
      *
      *  \param pTimeFunc A pointer to a function which returns the current time in milliseconds. The default
      *  would be to use RWS::Time::GetTime() function.
      *
      *  \return TRUE if successful, FALSE otherwise.
      *
      *  \see RWS::Time::GetTime
      *
      */

      RwBool Init (RwUInt32 Port, RwUInt32 (*pTimeFunc)(void))
      {
         RWS_FUNCTION("RWS::NetStream::Init");

         g_NetStreamData.Port = Port;
         gpTimeFunc = pTimeFunc;

         RwBool bOK = FALSE;
         if (NETSTREAMOK == NetStreamInit (&g_NetStreamData))
            bOK = TRUE;

         if (!bOK)
            Shutdown ();

         RWS_RETURN(bOK);
      }

      /**
      *
      *  \ingroup NetStream
      *
      *  Shutdown of NetStream library.
      *
      */

      void Shutdown ()
      {
         RWS_FUNCTION("RWS::NetStream::Shutdown");

         // reset globals ready for another NetStreamInit
         memset (&g_NetStreamData, 0, sizeof (NetStreamData));

         // call function to stop server
         sceSifCallRpc (&g_ClientData, gfcServerStop, 0, 0, 0, 0, 0, 0, 0);

         // clear the read and write buffers
         memset(g_WriteCache, 0, sizeof(g_WriteCache) );
         memset(g_ReadCache, 0, sizeof(g_ReadCache) );

         // Set connected flag to stop any attempts to send data
         bClientConnected = false;

         RWS_RETURNVOID();
      }


      /*
      *  Dummy function, internally stream code doesn't check for null pointers, this allows
      *  us to assert and not crash.
      */
      RwUInt32 DummyrwCustomStreamFnRead (void *data, void *buffer,RwUInt32 length) 
      {
         RWS_ASSERTFAIL("DummyrwCustomStreamFnRead should never be called");
         return 0;
      }

      /*
      *  Dummy function, internally stream code doesn't check for null pointers, this allows
      *  us to assert and not crash.
      */
      RwBool DummyrwCustomStreamFnWrite (void *data, const void *buffer,
         RwUInt32 length)
      {
         RWS_ASSERTFAIL("DummyrwCustomStreamFnWrite should never be called");
         return FALSE;
      }

      /*
      *  Dummy function, internally stream code doesn't check for null pointers, this allows
      *  us to assert and not crash.
      */
      RwBool DummyrwCustomStreamFnSkip (void *data, RwUInt32 offset)
      {
         RWS_ASSERTFAIL("DummyrwCustomStreamFnSkip should never be called");
         return FALSE;
      }

      /**
      *
      *  \ingroup NetStream
      *
      *  Used to open a stream used to read network RwStreams.
      *
      *  \return A pointer to an RwStream if a stream is pending on the network,
      *  0 otherwise.
      *
      */

      RwStream *OpenForRead ()
      {
         RWS_FUNCTION("RWS::NetStream::OpenForRead");

         RWS_ASSERT (!g_NetStreamData.PacketSize, "Last read stream not closed");

         if (g_NetStreamData.ReadStream)
         {
            RWS_ASSERT (0, "Currently, you are only allowed one read network stream.");
            RWS_RETURN (0);
         }
         
         RwStreamCustom CustomStreamData = { 0 };

         CustomStreamData.sfnclose = NetRStreamClose;
         CustomStreamData.sfnread  = NetStreamRead;
         CustomStreamData.sfnwrite = DummyrwCustomStreamFnWrite;
         CustomStreamData.sfnskip  = NetStreamSkip;
         CustomStreamData.data     = reinterpret_cast<void *>(&g_NetStreamData);

         // If we haven't read the packet size yet, attempt to read 8-byte header
         if (!g_NetStreamData.PacketSize)
         {
            if (0 == ReadPacketHeader (&g_NetStreamData))
            {
               RWS_RETURN (0); // no data pending - caller should retry opening
            }
         }

         // Open custom RwStream
         RwStream *pNetStream = RwStreamOpen (rwSTREAMCUSTOM, rwSTREAMREAD,
                                       reinterpret_cast<void *>(&CustomStreamData));

         // Set stream pointer (so we know we've only got one stream open)
         g_NetStreamData.ReadStream = pNetStream;

         // Assume the first time a stream is read, it is a workspace connecting.
         if ( !bClientConnected && pNetStream )
         {
            bClientConnected = true;
         }

 
         RWS_RETURN (pNetStream);
      }


      /**
      *
      *  \ingroup NetStream
      *
      *  Used to open a stream used to write network RwStreams.
      *
      *  \param TotalDataSize The number of bytes that will be written to the stream.
      *  This value must be known in advance to allow network data to be cached
      *  effectively.  Any cached data will be flushed upon a stream close.
      *
      *  \return A pointer to an RwStream if successful, or 0 otherwise.
      */

      RwStream *OpenForWrite (RwUInt32 TotalDataSize)
      {
         RWS_FUNCTION("RWS::NetStream::OpenForWrite");

         // Only try to send any data if a client (e.g. RWS workspace) has connected.
         if (!bClientConnected)
         {
            RWS_RETURN(0);
         }

         // Can't open a stream for writing if the IOP comms server is not running.
         if (!g_NetStreamData.IOPCommsServerStarted)
         {
            RWS_RETURN(0);
         }


         RWS_ASSERT (!g_NetStreamData.WPacketSize, "Last write stream not closed");
         if (g_NetStreamData.WriteStream)
         {
            RWS_ASSERT (0, "Currently, you are only allowed one write network stream.");
            RWS_RETURN (0); // Only allowed one write stream open
         }

         RwStreamCustom CustomStreamData = { 0 };

         CustomStreamData.sfnclose = NetWStreamClose;
         CustomStreamData.sfnread  = DummyrwCustomStreamFnRead;
         CustomStreamData.sfnwrite = NetStreamWrite;
         CustomStreamData.sfnskip  = DummyrwCustomStreamFnSkip;
         CustomStreamData.data     = reinterpret_cast<void *>(&g_NetStreamData);

         // Initialize write-stream data
         g_NetStreamData.WPacketSize = sizeof (RwUInt32); // Header
         g_NetStreamData.WPacketSize += TotalDataSize; // User data

         // Open custom RwStream
         RwStream *pNetStream = RwStreamOpen (rwSTREAMCUSTOM, rwSTREAMWRITE,
                                       reinterpret_cast<void *>(&CustomStreamData));

         // Set stream pointer (so we know we've only got one stream open)
         g_NetStreamData.WriteStream = pNetStream;

         // Write header to cache
         if (!WCacheData (&g_NetStreamData, &g_NetStreamData.WPacketSize,
                          sizeof (RwUInt32)))
         {
            RWS_RETURN (0);
         }

         RWS_RETURN (pNetStream);
      }


      /**
      *
      *  Used to obtain the IP address of the host machine once the NetStream library
      *  has been initialized.
      *
      *  \param szAddress A pointer to a character buffer to receive the IP address.
      *  \param AddressLength The length in bytes of the buffer.
      *
      *  \return The number of bytes copied into the buffer (excluding the null
      *  terminator).
      *
      */

      RwUInt32 GetAddress (RwChar * const szAddress, RwUInt32 AddressLength)
      {
         RWS_FUNCTION("RWS::NetStream::GetAddress");

         RWS_ASSERT (szAddress, "Invalid IP address buffer supplied");

         if (!AddressLength)
            RWS_RETURN(0);

         if (AddressLength < rwstrlen (g_NetStreamData.Address))
         {
            memcpy (szAddress, g_NetStreamData.Address, AddressLength-1);
            szAddress[AddressLength - 1] = 0;
            RWS_RETURN(AddressLength - 1);
         }

         rwstrcpy (szAddress, g_NetStreamData.Address);
         RWS_RETURN (rwstrlen (g_NetStreamData.Address));
      }
   }  // namespace NetStream
}     // namespace RWS

#endif // RWS_DESIGN
