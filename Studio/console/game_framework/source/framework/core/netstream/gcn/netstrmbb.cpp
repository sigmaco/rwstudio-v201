/*****************************************************************************
*
* File :     netstrm.cpp
*
* Abstract : Network RwStream Implementation GCN Version (BB)
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

#ifdef RWS_BROADBAND

#include <dolphin/socket.h>

#ifdef __MWERKS__
#include <string.h> // for memset
#else 
#include <cstring> // for memset
#endif

#include "../netstrm.h"
#include "../../../core/macros/debugmacros.h"

#ifdef RWS_DESIGN

/* -------------------------------------------------------------------------- */
namespace RWS
{
   /* -------------------------------------------------------------------------- */
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
      const RwUInt32 READCACHESIZE    = 32*1024;   // 32 K 
      // Used to write network data into a cache to reduce the number of small
      // network writes when dealing with large packets such as BSPs.
      const RwUInt32 WRITECACHESIZE   = 32*1024;   // 32 K

      const RwInt32 INVALID_SOCKET = -1;

      /* ----------------------------------------------------------------------- */
      typedef struct
      {
         RwUInt32  Port;
         RwInt32   ListenSocket;
         RwInt32   MessageSocket;
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
         RwChar    Address[SO_INET_ADDRSTRLEN];
      } NetStreamData;

      // Read/Write caches
      RwUInt8 g_ReadCache[READCACHESIZE] ATTRIBUTE_ALIGN(32);
      RwUInt8 g_WriteCache[WRITECACHESIZE] ATTRIBUTE_ALIGN(32);

      /* ----------------------------------------------------------------------- */
      RwStreamCustom g_CustomStreamData = { 0 };
      NetStreamData g_NetStreamData = { 0 };

      bool bClientConnected = false;

      OSMutex g_AllocLock;

      /* ----------------------------------------------------------------------- */
      // Forward declaration
      RwBool NetStreamSkip (void *pData, RwUInt32 Offset);

      /* ----------------------------------------------------------------------- */
      void *NetAlloc (u32 /*name*/, s32 size)
      {
         OSLockMutex (&g_AllocLock);

         void *pData = size > 0 ? ::RWS_OP_NEW (size) : 0;

         OSUnlockMutex (&g_AllocLock);
         return pData;
      }

      /* ----------------------------------------------------------------------- */
      void NetFree (u32 /*name*/, void *ptr, s32 /*size*/)
      {
         OSLockMutex (&g_AllocLock);
         delete ptr;
         OSUnlockMutex (&g_AllocLock);
      }

      /* ----------------------------------------------------------------------- */
      /**
      *
      *  \ingroup NetStream
      *
      *  Creates a non-blocking listener socket, and sets up the IP address we're
      *  listening on.
      *
      *  In order to specify an IP address in an environment with no DHCP server,
      *  this function attempts to open a file called "rwsipcfg.txt" from the root
      *  of the DVD (emulation) drive.  If this file exists, the first line is
      *  read, and the first characters are  assumed to be a network address using
      *  dotted notation (e.g. 10.0.0.45).  If the file doesn't exist, (or exists but
      *  doesn't contain a valid IP address) the network is configured through a
      *  DHCP server.  If no server exists, initialization will fail.
      */
      /* ----------------------------------------------------------------------- */
      RwInt32 NetStreamInit (NetStreamData *pData)
      {
         // NOTE: Do NOT use RWS_FUNCTION and RWS_RETURN in this function as it would cause a recursion.

         // Initialize mutex for NetAlloc() and NetFree()
         OSInitMutex (&g_AllocLock);

         *g_NetStreamData.Address = 0;

         SOConfig Config =
         {
             SO_VENDOR_NINTENDO,      // vendor
             SO_VERSION,              // version
             NetAlloc,                // alloc
             NetFree,                 // free
             SO_FLAG_DHCP,            // flag
             SOHtoNl (SO_INADDR_ANY), // addr
             SOHtoNl (SO_INADDR_ANY), // netmask
             SOHtoNl (SO_INADDR_ANY), // router
             SOHtoNl (SO_INADDR_ANY), // dns1
             SOHtoNl (SO_INADDR_ANY), // dns1
             4096,                    // timeWaitBuffer
             4096                     // reassemblyBuffer
         };

         // Attempt to read IP address from config file
         RwFileFunctions *pFileFuncs = RwOsGetFileInterface ();
         RWS_ASSERT (pFileFuncs, "RwOsGetFileInterface failed");

         const RwChar *cszFilename = RWSTRING("/rwsipcfg.txt");
         if (void *pInFile = pFileFuncs->rwfopen (cszFilename, "r"))
         {
            if (pFileFuncs->rwfgets (g_NetStreamData.Address,
                                     SO_INET_ADDRSTRLEN, pInFile))
            {
               // If we get a valid address, disable DHCP
               if (SOInetAtoN (g_NetStreamData.Address, &Config.addr))
                  Config.flag = 0;
               else
                  *g_NetStreamData.Address = 0;
            }

            pFileFuncs->rwfclose (pInFile);
         }

         OSReport ("\n*******************************************************************\n");
         OSReport ("*************************** NetStream *****************************\n");
         OSReport ("*******************************************************************\n");
         if (*g_NetStreamData.Address)
         {
            OSReport ("\nIP Address obtained from \"%s\": %s\n",
                      cszFilename, g_NetStreamData.Address);
         }
         else
         {
            OSReport ("\nNo IP Address obtained from \"%s\", using DHCP...\n\n",
                      cszFilename);
         }
         OSReport ("*******************************************************************\n");
         OSReport ("*******************************************************************\n");
         OSReport ("*******************************************************************\n\n");

         // Start network
         if (SOStartup (&Config) != 0) // Failure
            return (-1);

         pData->ListenSocket = pData->MessageSocket = INVALID_SOCKET;

         // Wait for network interface to be configured
         SOInAddr addr;
         do 
         {
            addr.addr = SOGetHostID ();
            if (IPGetConfigError (0) < 0)
               return (-2);
         }
         while (SOHtoNl (SO_INADDR_ANY) == addr.addr);

         // Get IP Address
         SOInetNtoP (SO_PF_INET, &addr, g_NetStreamData.Address, SO_INET_ADDRSTRLEN);

         // Create a TCP/IP socket
         int nSocket = SOSocket (SO_PF_INET, SO_SOCK_STREAM, 0);
         if (nSocket < 0)
            return (-3);

         pData->ListenSocket = nSocket;

         // Set up local address
         SOSockAddrIn socket = {0};
         socket.addr.addr = SOHtoNl (SO_INADDR_ANY);
         socket.len       = sizeof (SOSockAddrIn);
         socket.family    = IP_INET;
         socket.port      = pData->Port;

         // Associate local address and port combination with the socket
         if (SOBind (pData->ListenSocket, &socket) < 0)
         {
            pData->ListenSocket = INVALID_SOCKET;
            return (-4);
         }

         // Listen for connections
         if (SOListen (pData->ListenSocket, 5) < 0)
         {
            pData->ListenSocket = INVALID_SOCKET;
            return (-5);
         }

         // Make listen socket non-blocking
         int nVal = SOFcntl (pData->ListenSocket, SO_F_GETFL, 0);
         SOFcntl (pData->ListenSocket, SO_F_SETFL, nVal | SO_O_NONBLOCK);

         return (NETSTREAMOK);
      }

      /* ----------------------------------------------------------------------- */
      /**
      *
      *  \ingroup NetStream
      *
      *  Listens for any connection attempts, and accepts them.
      */
      /* ----------------------------------------------------------------------- */
      RwInt32 EstablishConnection (NetStreamData *pData)
      {
         SOSockAddrIn from = {0};
         from.len = sizeof(IPSocket);

         int nSocket = SOAccept (pData->ListenSocket, &from);
         if (nSocket < 0)
            return -1;

         pData->MessageSocket = nSocket;

         // Make message socket non-blocking
         int nVal = SOFcntl (pData->MessageSocket, SO_F_GETFL, 0);
         SOFcntl (pData->MessageSocket, SO_F_SETFL, nVal | SO_O_NONBLOCK);

         return (NETSTREAMOK); // connected
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
         RWS_ASSERT (pData->PacketSize,   "No data to cache");
         RWS_ASSERT (!pData->CachePos,    "Not at start of cache");
         RWS_ASSERT (!pData->CachedBytes, "Bytes already cached");

         // Work out how much we can read
         RwInt32 nBytesToRead = pData->PacketSize - pData->PacketPos;

         // Ensure we can't read more than will fit in the cache
         if (nBytesToRead > READCACHESIZE)
            nBytesToRead = READCACHESIZE;

         // Now read the data off the network
         RwUInt8 *pRecvBuffer = g_ReadCache;
         RwUInt8 *pBufferEnd  = pRecvBuffer + nBytesToRead;

         RwInt32 nBytesRead;
         while (pRecvBuffer < pBufferEnd)
         {
            if ((nBytesRead = SORead (pData->MessageSocket,
                                      pRecvBuffer,
                                      pBufferEnd - pRecvBuffer)) < 0)
            {
               if (SO_EWOULDBLOCK == nBytesRead) // Socket would block
               {
                  // Wait for more data
                  continue;
               }

               // Some other socket error, so reset receive socket
               pData->MessageSocket = INVALID_SOCKET;
               pData->PacketPos = pData->PacketSize = pData->PacketPad = 0;
               return (FALSE);
            }

            pRecvBuffer += nBytesRead;
         }

         // Set amount of data we cached
         pData->CachedBytes = nBytesToRead;

         return (TRUE);
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
         RWS_ASSERT (!pData->PacketSize,  "Packet size zero. Previous stream not closed?");
         RWS_ASSERT (!pData->PacketPos,   "Packet position zero. Previous stream not closed?");
         RWS_ASSERT (!pData->CachePos,    "Cache position zero. Previous stream not closed?");
         RWS_ASSERT (!pData->CachedBytes, "Cached bytes zero. Previous stream not closed?");

         RwInt32 nBytesRead;
         RwUInt32 Header[PACKETHEADERSIZE / sizeof (RwUInt32)];
         char *pRecvBuffer = reinterpret_cast<char *>(Header);
         char *pBufferEnd = pRecvBuffer + sizeof (Header);
         while (pRecvBuffer < pBufferEnd)
         {
            if ((nBytesRead = SORead (pData->MessageSocket,
                                      pRecvBuffer, pBufferEnd - pRecvBuffer)) < 0)
            {
               if (SO_EWOULDBLOCK == nBytesRead) // Socket would block
               {
                  if (pRecvBuffer == reinterpret_cast<char *>(Header))
                  {
                     // Not read a chunk header yet, so return 0 if no data pending
                     return (0);
                  }

                  // Wait for more data
                  continue;
               }

               // Some other socket error, so reset receive socket
               pData->MessageSocket = INVALID_SOCKET;

               return (0);
            }
            if (!nBytesRead)
            {
               if (pRecvBuffer == reinterpret_cast<char *>(Header))
               {
                  // Not read a chunk header yet, so return 0 if no data pending
                  return (0);
               }
            }
            pRecvBuffer += nBytesRead;
         }

         // We've read the header now, so return number of bytes in stream packet
         // Header is always sent in little-endian format
         RwMemNative32 (Header, 2 * sizeof (RwUInt32));
         pData->PacketSize = *Header;
         pData->PacketPad = *(Header + 1);

         // Attempt to read a cache block
         RCacheData (pData);

         return (pData->PacketSize);
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
         if (pData->PacketPos + Len > pData->PacketSize)
            Len = pData->PacketSize - pData->PacketPos;

         RwUInt8 *pOutBuffer = reinterpret_cast<RwUInt8 *>(pBuffer);
         RwUInt8 *pBufferEnd = pOutBuffer + Len;
         while (pOutBuffer < pBufferEnd)
         {
            // Work out how much we can read from the cache
            RwUInt32 nToCopy = pData->CachedBytes - pData->CachePos;
            if (nToCopy > RwUInt32 (pBufferEnd - pOutBuffer))
               nToCopy = pBufferEnd - pOutBuffer;

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
                  return (pBufferEnd - pOutBuffer); // Network error occurred
            }
         }

         return (Len);
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
         RwInt32 nBytesWritten;
         const RwInt8 *pSendBuffer = reinterpret_cast<const RwInt8 *>(pBuffer);
         const RwInt8 *pBufferEnd = pSendBuffer + Len;

         // Make the socket blocking for the duration of the send
         // If we don't do this, SOWrite always returns "bad socket descriptor" :(
         int nVal = SOFcntl (pData->MessageSocket, SO_F_GETFL, 0);
         SOFcntl (pData->MessageSocket, SO_F_SETFL, nVal & ~SO_O_NONBLOCK);

         while (pSendBuffer < pBufferEnd)
         {
            if ((nBytesWritten = SOWrite (pData->MessageSocket,
                                          pSendBuffer,
                                          pBufferEnd - pSendBuffer)) < 0)
            {
               // Socket error, so reset socket and globals
               pData->MessageSocket = INVALID_SOCKET;
               pData->PacketPos = pData->PacketSize = pData->PacketPad =
                                                            pData->WPacketSize = 0;
               return (0);
            }

            pSendBuffer += nBytesWritten;
         }

         // Restore non-blocking socket status
         nVal = SOFcntl (pData->MessageSocket, SO_F_GETFL, 0);
         SOFcntl (pData->MessageSocket, SO_F_SETFL, nVal | SO_O_NONBLOCK);

         return (Len);
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
         RWS_ASSERT (pData->WPacketSize, "No data to cache");
         RWS_ASSERT (pData->WCachePos < WRITECACHESIZE, "End of cache reached, max size = "
               << WRITECACHESIZE);

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
                  return (FALSE);
               }
            }

            // Update buffer pointer
            pSendBuffer += nBytesToWrite;
         }

         return (TRUE);
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
         NetStreamData *pRecvData = reinterpret_cast<NetStreamData *>(pData);

         // Increase size so we're allowed to read the padding region
         pRecvData->PacketSize += pRecvData->PacketPad;
         pRecvData->PacketPad = 0;

         // Skip to end of stream packet, ready to read next one
         RwUInt32 nToSkip = pRecvData->PacketSize - pRecvData->PacketPos;
         if (nToSkip)
            if (!NetStreamSkip (pData, nToSkip))
               pRecvData->MessageSocket = INVALID_SOCKET; // network failed

         // Reset globals
         pRecvData->PacketPos  = pRecvData->PacketSize = 0;
         pRecvData->CachePos   = pRecvData->CachedBytes = 0;
         pRecvData->ReadStream = 0;

         return (TRUE);
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
         NetStreamData *pSendData = reinterpret_cast<NetStreamData *>(pData);

         // Flush the write cache if there's any data still in it
         if (pSendData->WCachePos)
         {
            if (WritePacketData (pSendData, g_WriteCache, pSendData->WCachePos)
                                                            < pSendData->WCachePos)
            {
               pSendData->WCachePos = pSendData->WPacketSize = 0;
               pSendData->WriteStream = 0;
               return (FALSE);
            }
         }

         // Reset globals
         pSendData->WCachePos = pSendData->WPacketSize = 0;
         pSendData->WriteStream = 0;

         return (TRUE);
      }

      /* ----------------------------------------------------------------------- */
      /**
      *
      *  Calculate the amount of data remaining in the stream.
      *
      *  \returns Returns the number of bytes remaining in the stream.
      *
      */
      /* ----------------------------------------------------------------------- */
      RwUInt32 GetUnreadData (void *pData)
      {
         NetStreamData *pRecvData = reinterpret_cast<NetStreamData *>(pData);

         if (pRecvData->PacketPos < pRecvData->PacketSize)
         {
            return (pRecvData->PacketSize - pRecvData->PacketPos);
         }

         return (0);
      }

      /* ----------------------------------------------------------------------- */
      /**
      *
      *  \ingroup NetStream
      *
      *  Called by RwStreamRead etc. on a read stream, to read network data into
      *  the supplied buffer.
      */
      /* ----------------------------------------------------------------------- */
      RwUInt32 NetStreamRead (void *pData, void *pBuffer, RwUInt32 Length)
      {
         // Return how much data is left to read
         if (pBuffer == 0 || Length == 0)
         {
            return(GetUnreadData (pData));
         }

         NetStreamData *pRecvData = reinterpret_cast<NetStreamData *>(pData);

         if (INVALID_SOCKET == pRecvData->MessageSocket)
            return (0);

         // Now read the data the client requested
         return (ReadPacketData (pRecvData, pBuffer, Length));
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
         // Cache the data
         if (!WCacheData (reinterpret_cast<NetStreamData *>(pData),
                          pBuffer, Length))
         {
            return (FALSE);
         }

         return (TRUE);
      }

      /* ----------------------------------------------------------------------- */
      /**
      *
      *  \ingroup NetStream
      *
      *  Called by RwStreamSkip on a read stream, to skip past data in the stream.
      *  Actually reads the data off the network into a temporary buffer, to ensure
      *  than the network stay in sync.
      */
      /* ----------------------------------------------------------------------- */
      RwBool NetStreamSkip (void *pData, RwUInt32 Offset)
      {
         char Buffer[SKIPBUFFERSIZE];

         RwInt32 nLeft = Offset;
         do
         {
            RwUInt32 nToRead = (nLeft < SKIPBUFFERSIZE) ? nLeft : SKIPBUFFERSIZE;

            if (NetStreamRead (pData, Buffer, nToRead) < nToRead)
               return (FALSE);

            nLeft -= nToRead;
         } while (nLeft > 0);

         return (TRUE);
      }
      /* ----------------------------------------------------------------------- */
   } // namespace

   namespace NetStream
   {
      /* -------------------------------------------------------------------------- */
      /* ------------------------------- Interface -------------------------------- */
      /* -------------------------------------------------------------------------- */
      /**
      *
      *  \ingroup NetStream
      *
      *  Initializes the NetStream library.
      *
      *  \param Port The port number to listen for connections on.
      *
      *  \return TRUE if successful, FALSE otherwise.
      */
      /* -------------------------------------------------------------------------- */
      RwBool Init (RwUInt32 Port, RwUInt32 (*)(void))
      {
         // GCN does not use time within the network code, so no need to store the
         // pointer to the function which provides this.
   
         g_NetStreamData.Port = Port;

         RwBool bOK = FALSE;

         if (NETSTREAMOK == NetStreamInit (&g_NetStreamData))
            bOK = TRUE;

         if (!bOK)
            Shutdown ();

         return (bOK);
      }

      /* -------------------------------------------------------------------------- */
      /**
      *
      *  \ingroup NetStream
      *
      *  Shuts down the NetStream library.
      */
      /* -------------------------------------------------------------------------- */
      void Shutdown ()
      {
         // Shutdown any active sockets
         if (g_NetStreamData.ListenSocket != INVALID_SOCKET)
         {
            int nVal = SOFcntl (g_NetStreamData.ListenSocket, SO_F_GETFL, 0);
            SOFcntl (g_NetStreamData.ListenSocket, SO_F_SETFL, nVal & ~SO_O_NONBLOCK);
            SOClose (g_NetStreamData.ListenSocket);
         }

         if (g_NetStreamData.MessageSocket != INVALID_SOCKET)
         {
            int nVal = SOFcntl (g_NetStreamData.MessageSocket, SO_F_GETFL, 0);
            SOFcntl (g_NetStreamData.MessageSocket, SO_F_SETFL, nVal & ~SO_O_NONBLOCK);
            SOClose (g_NetStreamData.MessageSocket);
         }

         // Reset globals ready for another NetStreamInit
         memset (&g_NetStreamData, 0, sizeof (NetStreamData));
         g_NetStreamData.ListenSocket =
         g_NetStreamData.MessageSocket = INVALID_SOCKET;
         memset (&g_CustomStreamData, 0, sizeof (RwStreamCustom));

         // Shutdown socket lib
         SOCleanup ();

         bClientConnected = false;
      }

      /* -------------------------------------------------------------------------- */
      /**
      *  \ingroup NetStream
      *
      *  Used to open a stream used to read network RwStreams.
      *
      *  \return A pointer to an RwStream if a stream is pending on the network,
      *          0 otherwise.
      */
      /* -------------------------------------------------------------------------- */
      RwStream *OpenForRead ()
      {
         RWS_ASSERT (!g_NetStreamData.PacketSize, "Last read stream not closed");
         if (g_NetStreamData.ReadStream)
         {
            RWS_ASSERT (0, "Currently, you are only allowed one read network stream.");
            return (0);
         }
         g_CustomStreamData.sfnclose = NetRStreamClose;
         g_CustomStreamData.sfnread  = NetStreamRead;
         g_CustomStreamData.sfnwrite = 0;
         g_CustomStreamData.sfnskip  = NetStreamSkip;
         g_CustomStreamData.data     = reinterpret_cast<void *>(&g_NetStreamData);

         // If socket isn't yet open, try to connect
         if (INVALID_SOCKET == g_NetStreamData.MessageSocket)
            if (EstablishConnection (&g_NetStreamData) != NETSTREAMOK)
               return (0); // Failed

         // If we haven't read the packet size yet, attempt to read 8-byte header
         if (!g_NetStreamData.PacketSize)
         {
            if (0 == ReadPacketHeader (&g_NetStreamData))
            {
               return (0); // no data pending - caller should retry opening
            }
         }

         // Open custom RwStream
         RwStream *pNetStream = RwStreamOpen (rwSTREAMCUSTOM, rwSTREAMREAD,
                                       reinterpret_cast<void *>(&g_CustomStreamData));

         // Set stream pointer (so we know we've only got one stream open)
         g_NetStreamData.ReadStream = pNetStream;

         // Assume the first time a stream is read, it is a workspace connecting.
         if ( !bClientConnected && pNetStream )
         {
            bClientConnected = true;
         }

         return (pNetStream);
      }

      /* -------------------------------------------------------------------------- */
      /**
      *
      *  \ingroup NetStream
      *
      *  Used to open a stream used to write network RwStreams.
      *
      *  \param TotalDataSize The number of bytes that will be written to the stream.
      *         This value must be known in advance to allow network data to be cached
      *         effectively.  Any cached data will be flushed upon a stream close.
      *
      *  \return A pointer to an RwStream if successful, or 0 otherwise.
      */
      /* -------------------------------------------------------------------------- */
      RwStream *OpenForWrite (RwUInt32 TotalDataSize)
      {
         // Only try to send any data if a client (e.g. RWS workspace) has connected.
         if (!bClientConnected)
         {
            return (0);
         }

         RWS_ASSERT (!g_NetStreamData.WPacketSize, "Last write stream not closed");
         if (g_NetStreamData.WriteStream)
         {
            RWS_ASSERT (0, "Currently, you are only allowed one write network stream.");
            return (0); // Only allowed one write stream open
         }
         g_CustomStreamData.sfnclose = NetWStreamClose;
         g_CustomStreamData.sfnread  = 0;
         g_CustomStreamData.sfnwrite = NetStreamWrite;
         g_CustomStreamData.sfnskip  = 0;
         g_CustomStreamData.data     = reinterpret_cast<void *>(&g_NetStreamData);

         // If socket isn't yet open, try to connect
         if (INVALID_SOCKET == g_NetStreamData.MessageSocket)
            if (EstablishConnection (&g_NetStreamData) != NETSTREAMOK)
               return (0); // Failed

         // Initialize write-stream data
         g_NetStreamData.WPacketSize = sizeof (RwUInt32); // Header
         g_NetStreamData.WPacketSize += TotalDataSize; // User data

         // Open custom RwStream
         RwStream *pNetStream = RwStreamOpen (rwSTREAMCUSTOM, rwSTREAMWRITE,
                                       reinterpret_cast<void *>(&g_CustomStreamData));

         // Set stream pointer (so we know we've only got one stream open)
         g_NetStreamData.WriteStream = pNetStream;

         // Write header to cache
         if (!WCacheData (&g_NetStreamData, &g_NetStreamData.WPacketSize,
                          sizeof (RwUInt32)))
         {
            return (0);
         }

         return (pNetStream);
      }

      /* -------------------------------------------------------------------------- */
      /**
      *
      *  \ingroup NetStream
      *
      *  Used to obtain the IP address of the host machine once the NetStream library
      *  has been initialized.
      *
      *  \param szAddress Pointer to a character buffer to receive the IP address.
      *  \param AddressLength Length in bytes of the buffer.
      *
      *  \return The number of bytes copied into the buffer (excluding the null
      *          terminator).
      */
      /* -------------------------------------------------------------------------- */
      RwUInt32 GetAddress (RwChar * const szAddress, RwUInt32 AddressLength)
      {
         RWS_ASSERT (szAddress, "Invalid IP address buffer supplied");

         if (!AddressLength)
            return (0);

         if (AddressLength < rwstrlen (g_NetStreamData.Address))
         {
            memcpy (szAddress, g_NetStreamData.Address, AddressLength-1);
            szAddress[AddressLength - 1] = 0;
            return (AddressLength - 1);
         }
         rwstrcpy (szAddress, g_NetStreamData.Address);

         return (rwstrlen (g_NetStreamData.Address));
      }
      /* -------------------------------------------------------------------------- */
   }// namespace NetStream
} // namespace RWS

#endif // RWS_DESIGN
#endif // RWS_BROADBAND
