/*****************************************************************************
*
* File :     netstrm.cpp
*
* Abstract : Network RwStream Implementation PC/XBOX Version
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
#include "../../../core/macros/debugmacros.h"

#ifdef RWS_DESIGN

#ifdef WIN32
#if !defined(_XBOX)
#include <winsock.h>
#pragma comment (lib, "wsock32.lib")
#endif
#endif

#ifdef _XBOX
#include <xtl.h>
#include <winsockx.h>
#endif

#ifdef UNDER_CE
#include <winsock.h>
#endif

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
         RwBool ReadStreamActive;
         RwBool WriteStreamActive;
         // IP address
         RwChar    Address[16];
      } NetStreamData;

      // Read/Write caches
      RwUInt8 g_ReadCache[READCACHESIZE];
      RwUInt8 g_WriteCache[WRITECACHESIZE];

      /* ----------------------------------------------------------------------- */
      NetStreamData g_NetStreamData = { 0 };

      bool bClientConnected = false;

      RwUInt32 g_TotalDataSize;        // Used to debug writing data streams
      RwUInt32 g_CurrentDataSize;      // Used to debug writing data streams

      /* ----------------------------------------------------------------------- */
      // Forward declaration
      RwBool NetStreamSkip (void *pData, RwUInt32 Offset);

      /* ----------------------------------------------------------------------- */
      /**
      *
      *  \ingroup NetStream
      *
      *  Creates a non-blocking listener socket, and sets up the IP address we're
      *  listening on.
      */
      /* ----------------------------------------------------------------------- */
      RwInt32 NetStreamInit (NetStreamData *pData)
      {
         pData->ListenSocket = pData->MessageSocket = INVALID_SOCKET;

         // Set up local address
         SOCKADDR_IN LocalAddress = { 0 };
         LocalAddress.sin_family = AF_INET;
         LocalAddress.sin_addr.s_addr = INADDR_ANY; // Any IP address can connect
         LocalAddress.sin_port = htons (pData->Port);

         // Create a TCP/IP socket
         pData->ListenSocket = socket (AF_INET, SOCK_STREAM, 0);
         if (INVALID_SOCKET == pData->ListenSocket)
         {
            return (-1);
         }

         // Associate local address and port combination with the socket
         if (SOCKET_ERROR == bind (pData->ListenSocket,
                                   reinterpret_cast<sockaddr *>(&LocalAddress),
                                   sizeof (LocalAddress)))
         {
            return (-2);
         }

         // Listen for connections
         if (SOCKET_ERROR == listen (pData->ListenSocket, 5))
         {
            return (-3);
         }

         // Make listen socket non-blocking
         u_long nVal = 1;
         if (ioctlsocket (pData->ListenSocket, FIONBIO, &nVal))
         {
            return (-4);
         }

         // Fill in IP address
         *pData->Address = 0;

   #ifdef _XBOX
         DWORD dwStatus;
         XNADDR m_xnTitleAddress;

         dwStatus = XNetGetTitleXnAddr (&m_xnTitleAddress);
         while (XNET_GET_XNADDR_PENDING == dwStatus)
            dwStatus = XNetGetTitleXnAddr (&m_xnTitleAddress);

         if (dwStatus != XNET_GET_XNADDR_NONE)
         {
            if (&m_xnTitleAddress.ina)
            {
               rwsprintf (pData->Address, "%d.%d.%d.%d",
                          m_xnTitleAddress.ina.S_un.S_un_b.s_b1,
                          m_xnTitleAddress.ina.S_un.S_un_b.s_b2,
                          m_xnTitleAddress.ina.S_un.S_un_b.s_b3,
                          m_xnTitleAddress.ina.S_un.S_un_b.s_b4);
            }
         }
   #else
         RwChar szName[128];
         if (gethostname (szName, 16) != SOCKET_ERROR)
         {
            if (HOSTENT *pHost = gethostbyname (szName))
            {
               struct sockaddr_in Addr;
               int i = 0;
               while (pHost->h_addr_list[i++])
               {
                  memcpy (&(Addr.sin_addr), pHost->h_addr_list[0],
                          pHost->h_length);
                  if (char *szAddr = inet_ntoa (Addr.sin_addr))
                  {
                     //FE: check string length and truncate if necessary
                     rwstrcpy (pData->Address, szAddr);
                     break;
                  }
               }
            }
         }
   #endif

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
         SOCKADDR_IN from;
         RwInt32 fromlen = sizeof (from);
         pData->MessageSocket = accept (pData->ListenSocket,
                                        reinterpret_cast<sockaddr *>(&from),
                                        &fromlen);

         if (INVALID_SOCKET == pData->MessageSocket)
         {
            return (-1); // No connections pending
         }

         // Make message socket non-blocking
         u_long nVal = 1;
         if (ioctlsocket (pData->MessageSocket, FIONBIO, &nVal))
         {
            return (-2);
         }

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
            if (SOCKET_ERROR == (nBytesRead = recv (
                                             pData->MessageSocket,
                                             reinterpret_cast<char *>(pRecvBuffer),
                                             pBufferEnd - pRecvBuffer, 0)))
            {
               if (WSAEWOULDBLOCK == WSAGetLastError ()) // Socket would block
               {
                  // Wait for more data
                  Sleep (0);
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
            if (SOCKET_ERROR == (nBytesRead = recv (pData->MessageSocket,
                                                    pRecvBuffer,
                                                    pBufferEnd - pRecvBuffer, 0)))
            {
               if (WSAEWOULDBLOCK == WSAGetLastError ()) // Socket would block
               {
                  if (pRecvBuffer == reinterpret_cast<char *>(Header))
                  {
                     // Not read a chunk header yet, so return 0 if no data pending
                     return (0);
                  }

                  // Wait for more data
                  Sleep (0);
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
               Sleep (0);
            }
            pRecvBuffer += nBytesRead;
         }

         // We've read the header now, so return number of bytes in stream packet
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
         while (pSendBuffer < pBufferEnd)
         {
            if (SOCKET_ERROR == (nBytesWritten = send (
                                       pData->MessageSocket,
                                       reinterpret_cast<const char *>(pSendBuffer),
                                       pBufferEnd - pSendBuffer, 0)))
            {
               if (WSAEWOULDBLOCK == WSAGetLastError ()) // Socket would block
               {
                  // Wait for receiver
                  Sleep (0);
                  continue;
               }

               // Some other socket error, so reset socket and globals
               pData->MessageSocket = INVALID_SOCKET;
               pData->PacketPos = pData->PacketSize = pData->PacketPad =
                                                            pData->WPacketSize = 0;
               return (0);
            }

            pSendBuffer += nBytesWritten;
         }

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
         RWS_ASSERT (pData->WCachePos < WRITECACHESIZE, "End of cache reached, max = "
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
               if (WritePacketData (pData, g_WriteCache, WRITECACHESIZE)
                                                                  < WRITECACHESIZE)
               {
                  return (FALSE);
               }
               pData->WCachePos = 0;
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
         pRecvData->ReadStreamActive = FALSE;
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
               pSendData->WriteStreamActive = FALSE;
               return (FALSE);
            }
         }

         // Reset globals
         pSendData->WCachePos = pSendData->WPacketSize = 0;
         pSendData->WriteStreamActive = FALSE;

         return (TRUE);
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
            return(GetUnReadData (pData));
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

         g_CurrentDataSize += Length;

         RWS_ASSERT(g_TotalDataSize >= g_CurrentDataSize, "Error cannot write more data than specified"
            "when stream was opened ["<<g_CurrentDataSize<<"]["<<g_TotalDataSize<<"]");

         return (TRUE);
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
      *  \param pTimeFunc A pointer to a function to provide the time in milliseconds.
      *  Normally the RWS::Time::GetTime() function.
      *
      *  \return TRUE if successful, FALSE otherwise.
      *
      *  \see RWS::Time::GetTime
      */
      /* -------------------------------------------------------------------------- */
      RwBool Init (RwUInt32 Port, RwUInt32 (*pTimeFunc)(void))
      {
         g_NetStreamData.Port = Port;

         RwBool bOK = FALSE;

         // NOTE: pTimeFunc is not stored on the PC as it is not required. Must be in
         //       function prototype for compatibility.

      #ifdef _XBOX
         // Initialize XBox network stack
         XNetStartupParams xnsp = {0};
         xnsp.cfgSizeOfStruct = sizeof (XNetStartupParams);
         xnsp.cfgFlags        = XNET_STARTUP_BYPASS_SECURITY;

         RwInt32 err = XNetStartup (&xnsp);
         if (err != NO_ERROR)
            return (FALSE);
      #endif


         WSADATA wsaData;

#ifdef UNDER_CE
         if (!WSAStartup (0x101, &wsaData))
            if (1 == LOBYTE (wsaData.wVersion) && 1 == HIBYTE (wsaData.wVersion))
               if (NETSTREAMOK == NetStreamInit (&g_NetStreamData))
                   bOK = TRUE;
#else
         if (!WSAStartup (0x202, &wsaData))
            if (2 == LOBYTE (wsaData.wVersion) && 2 == HIBYTE (wsaData.wVersion))
               if (NETSTREAMOK == NetStreamInit (&g_NetStreamData))
                   bOK = TRUE;
#endif

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
            u_long nVal = 0;
            ioctlsocket (g_NetStreamData.ListenSocket, FIONBIO, &nVal);
            closesocket (g_NetStreamData.ListenSocket);
         }

         if (g_NetStreamData.MessageSocket != INVALID_SOCKET)
         {
            u_long nVal = 0;
            ioctlsocket (g_NetStreamData.MessageSocket, FIONBIO, &nVal);
            closesocket (g_NetStreamData.MessageSocket);
         }

         // Reset globals ready for another NetStreamInit
         memset (&g_NetStreamData, 0, sizeof (NetStreamData));
         g_NetStreamData.ListenSocket =
         g_NetStreamData.MessageSocket = INVALID_SOCKET;

         // Shutdown winsock
         WSACleanup ();

      #ifdef _XBOX
         // Clean up XBOX network stack
         XNetCleanup ();
      #endif

         bClientConnected = false;

         return;
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
         if (g_NetStreamData.ReadStreamActive)
         {
            RWS_ASSERTFAIL ("Currently, you are only allowed one open read network stream.");
            return (0);
         }

         RwStreamCustom CustomStreamData;

         CustomStreamData.sfnclose = NetRStreamClose;
         CustomStreamData.sfnread  = NetStreamRead;
         CustomStreamData.sfnwrite = DummyrwCustomStreamFnWrite;
         CustomStreamData.sfnskip  = NetStreamSkip;
         CustomStreamData.data     = reinterpret_cast<void *>(&g_NetStreamData);

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
                                       reinterpret_cast<void *>(&CustomStreamData));

         // Set stream pointer (so we know we've only got one stream open)
         g_NetStreamData.ReadStreamActive = TRUE;
 
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

         if (g_NetStreamData.WriteStreamActive)
         {
            RWS_ASSERTFAIL ("Currently, you are only allowed one open write network stream.");
            return (0); // Only allowed one write stream open
         }
         RwStreamCustom CustomStreamData;

         CustomStreamData.sfnclose = NetWStreamClose;
         CustomStreamData.sfnread  = DummyrwCustomStreamFnRead;
         CustomStreamData.sfnwrite = NetStreamWrite;
         CustomStreamData.sfnskip  = DummyrwCustomStreamFnSkip;
         CustomStreamData.data     = reinterpret_cast<void *>(&g_NetStreamData);

         // If socket isn't yet open, try to connect
         if (INVALID_SOCKET == g_NetStreamData.MessageSocket)
            if (EstablishConnection (&g_NetStreamData) != NETSTREAMOK)
               return (0); // Failed

         // Initialize write-stream data
         g_NetStreamData.WPacketSize = sizeof (RwUInt32); // Header
         g_NetStreamData.WPacketSize += TotalDataSize; // User data

         // Open custom RwStream
         RwStream *pNetStream = RwStreamOpen (rwSTREAMCUSTOM, rwSTREAMWRITE,
                                       reinterpret_cast<void *>(&CustomStreamData));

         // Set stream pointer (so we know we've only got one stream open)
         g_NetStreamData.WriteStreamActive = TRUE;

         // Write header to cache
         if (!WCacheData (&g_NetStreamData, &g_NetStreamData.WPacketSize,
                          sizeof (RwUInt32)))
         {
            return (0);
         }

         g_TotalDataSize = TotalDataSize;
         g_CurrentDataSize = 0;

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
