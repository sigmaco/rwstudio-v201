/*****************************************************************************
*
* File :     netstrm.cpp
*
* Abstract : Network RwStream Implementation GCN Version (USB)
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

#ifndef RWS_BROADBAND

//#define RWS_TRACEENABLE

#include <dolphin.h>
#include <dolphin/hio.h>

#ifdef __MWERKS__
#include <string.h> // for memset
#else 
#include <cstring> // for memset
#endif

#include "../netstrm.h"

#ifdef RWS_DESIGN

//////////////////////////////////////////////////////////////////
//
// System Includes
//
#include <dolphin.h>

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "../netstrm.h"
#include "../../../core/macros/debugmacros.h"
#include "hiodata.h"

/**
*
*  \addtogroup NetStreamGameCube
*
*  The GameCube version of the RenderWare Studio Framework differs from other 
*  platforms in that it does not use direct IP networking for its connection to 
*  the RenderWare Studio Workspace. Instead, it uses the HIO API to communicate
*  with the PC the devkit is connected to. It is the GameCubeCommsServer,
*  running on the connected PC, that uses the HIO API to communicate with the
*  devkit. The GameCubeCommsServer also maintains an IP network connection - 
*  it is this that the RenderWare Studio Workspace actually connects to.
*
*  The GameCubeCommsServer redirects data from its IP connection to
*  RenderWare Studio Workspace to the GameCube HIO connection, or vice versa.
*  Most of the time the operation of the GameCubeCommsServer will be 
*  transparent. However, the HIO interface requires some handshaking between
*  the GameCube and PC to synchronize them - if this fails then the connection
*  to the GameCube will be lost. To solve this, reset the GameCubeCommsServer -
*  this will force it to re-sync with the RenderWare Studio Framework
*  (assuming it is running).
*
*  Also note that the RenderWare Studio Framework needs the GameCubeCommsServer 
*  to be running before it will display the RenderWare Studio 'splash screen' 
*  with the IP address on it.
*
*/

namespace
{
   // Return value from internal helper functions
   const RwInt32 NETSTREAMOK       = 0;
   // Header wrapping an incoming network packet
   const RwUInt32 PACKETHEADERSIZE = 8;

   u8 g_SendBuffer[HIO_MAX_DATA_SIZE] ATTRIBUTE_ALIGN(32);
   u8 g_RecvBuffer[HIO_MAX_DATA_SIZE] ATTRIBUTE_ALIGN(32);
   u8 g_HandShakeH[HIO_HS_DATA_SIZE]  ATTRIBUTE_ALIGN(32);
   u8 g_HandShakeT[HIO_HS_DATA_SIZE]  ATTRIBUTE_ALIGN(32);

   /* ----------------------------------------------------------------------- */
   struct NetStreamData
   {
      NetStreamData () :   PacketPos (0), PacketSize (0), BufferPos (0),
                           PacketsLeft (0), WriteStream (0), EXIchan (-1)
                           { *IPAddress = 0; }
      void Clear ()
      {
         memset (this, 0, sizeof (NetStreamData));
         EXIchan = -1;
      }
      volatile enum
      {
         // Status flags - modified by the mailbox callback
         StatusNormal = 0,
         StatusWaitingRead,
         StatusWaitingIP,
         StatusOKToRead,
         StatusWaitingSend,
         StatusNeedToReset
      } nStatus;

      RwUInt32  PacketPos;
      RwUInt32  PacketSize;
      RwUInt32  BufferPos;      // Current position in read cache (g_RecvBuffer)
      RwUInt32  PacketsLeft;    // number of packets left in cache
      // Write-cache data
      RwUInt32  WCachePos;
      RwUInt32  WPacketSize;

      RwStream *ReadStream;
      RwStream *WriteStream;
      RwChar    IPAddress[16];
      s32       EXIchan;
   };

   /* ----------------------------------------------------------------------- */
   RwStreamCustom g_CustomStreamData = { 0 };
   NetStreamData  g_NetStreamData;

   bool bClientConnected = false;

   /* ----------------------------------------------------------------------- */
   // Forward declaration
   void NetStreamReset ( void );

   /* ----------------------------------------------------------------------- */
   /**
   *
   *  \ingroup NetStreamGameCube
   *
   *  Callback used by HIO API's Init function.
   *
   */
   /* ----------------------------------------------------------------------- */
   BOOL HIOEnumCB (s32 chan)
   {
      g_NetStreamData.EXIchan = chan;
      return FALSE;
   }

   /* ----------------------------------------------------------------------- */
   /**
   *
   *  \ingroup NetStreamGameCube
   *
   *  Callback called from HIO API when host writes to our mailbox.
   *
   */
   /* ----------------------------------------------------------------------- */
   void NewMailCB ()
   {
      RWS_FUNCTION("NetStream::NewMailCB");

      u32 Mail;
      if (HIOReadMailbox (&Mail))
      {
         // We got mail
         switch (Mail)
         {
            // Check the command
            case rwsmDataToRead: // Host asking us to read data
               RWS_TRACE("rwsmDataToRead");
               g_NetStreamData.nStatus = NetStreamData::StatusOKToRead;
               break;
               
            case rwsmNoDataToRead: // Host saying no data to read
               RWS_TRACE("rwsmNoDataToRead");
               g_NetStreamData.nStatus = NetStreamData::StatusNormal;
               break;

            case rwsmDataToSend: // Host saying the send completed
               RWS_TRACE("rwsmDataToSend");
               g_NetStreamData.nStatus = NetStreamData::StatusNormal;
               break;

            case rwsmIPAddress: // Host asking us to read IP address
               RWS_TRACE("rwsmIPAddress");
               g_NetStreamData.nStatus = NetStreamData::StatusNormal;
               break;

            case rwsmLastCommand: // Host saying it has finished with the previous connection and wants to start another
               RWS_TRACE("rwsmLastCommand");
               g_NetStreamData.nStatus = NetStreamData::StatusNeedToReset;
               break;
               
            default:
               RWS_TRACE("Invalid rwsmXXX message");
               // Invalid command, so ignore it
               break;
         }
      }

      RWS_RETURNVOID();
   }

   /* ----------------------------------------------------------------------- */
   // Forward declaration
   RwBool NetStreamSkip (void *pData, RwUInt32 Offset);

   /* ----------------------------------------------------------------------- */
   /**
   *
   *  \ingroup NetStreamGameCube
   *
   *  Does some basic handshaking between host and target.
   *  Tries to ensure that host and target are synchronized.
   *
   */
   /* ----------------------------------------------------------------------- */
   void WaitForConnection ()
   {
      *reinterpret_cast<u32 *>(g_HandShakeH) = 0xffffffffu;
      DCStoreRange (g_HandShakeH, HIO_HS_DATA_SIZE);

      // Write to my handshake area
      HIOWrite (HIO_TARG_HANDSHAKE_ADDR, g_HandShakeH, HIO_HS_DATA_SIZE);

      // Wait for value to change to 0
      do
      {
         HIORead (HIO_TARG_HANDSHAKE_ADDR, g_HandShakeT, HIO_HS_DATA_SIZE);
         DCInvalidateRange (g_HandShakeT, HIO_HS_DATA_SIZE);

         // Write 0 to the hosts handshake area 
         //  (host waits for the 0)
         *reinterpret_cast<u32 *>(g_HandShakeH) = 0;
         DCStoreRange (g_HandShakeH, HIO_HS_DATA_SIZE);
         HIOWrite (HIO_HOST_HANDSHAKE_ADDR, g_HandShakeH, HIO_HS_DATA_SIZE);
      }
      while (*reinterpret_cast<u32 *>(g_HandShakeT) != 0);
   }

   /* ----------------------------------------------------------------------- */
   /**
   *
   *  \ingroup NetStream
   *
   *  Main initialization code.
   *  Initializes the HIO API, waits for synchronization with host, and requests
   *  host's IP address to display.
   *
   */
   /* ----------------------------------------------------------------------- */
   RwInt32 NetStreamInit (NetStreamData *pData)
   {
      RWS_FUNCTION("NetStreamInit");

      HIOEnumDevices (HIOEnumCB);
      if (!HIOInit (pData->EXIchan, NewMailCB))
         RWS_RETURN (-1);

      // Read once to get started
      u32 Mail;
      if (!HIOReadMailbox (&Mail))
         RWS_RETURN (-2);

      // Reset globals
      pData->Clear ();

      WaitForConnection ();

      // Ask for IP address...
      *(pData->IPAddress) = 0;
      pData->nStatus = NetStreamData::StatusWaitingIP;
      if (!HIOWriteMailbox (rwsmIPAddress))
         RWS_RETURN (-3);

      // ... and wait for reply
      while (NetStreamData::StatusWaitingIP == pData->nStatus)
         ;

      // Now read IP
      if (!HIORead (HIO_HOST_TO_TARG_ADDR, g_RecvBuffer, HIO_MAX_DATA_SIZE))
      {
         RWS_RETURN (-4);
      }
      DCInvalidateRange (g_RecvBuffer, HIO_MAX_DATA_SIZE);

      // Copy IP address into global data
      memcpy (pData->IPAddress, g_RecvBuffer, 16);

      RWS_RETURN(NETSTREAMOK);
   }

   /* ----------------------------------------------------------------------- */
   /**
   *
   *  \ingroup NetStream
   *
   *  Attempt to read the next available stream header.  This 8-byte header is
   *  placed infront of an RwStream on the network to tell us how big it is and
   *  If it is padded out.
   *
   *  First checks a global flag to determine whether there are any more packets
   *  left in the network cache, in which case we can just copy one out.
   *  Otherwise, requests another cache full from the host via the HIO interface.
   *
   *  Called by NetStream::Open, to determine if there's a stream pending on the
   *  network.
   *
   */
   /* ----------------------------------------------------------------------- */
   RwUInt32 ReadPacketHeader (NetStreamData *pData)
   {
      RWS_FUNCTION("ReadPacketHeader");

      RWS_ASSERT (!pData->PacketPos, "Already reading packet!");

      // May need to reset - currently only if rwsmLastCommand is received.
      if ( NetStreamData::StatusNeedToReset == g_NetStreamData.nStatus )
      {
         NetStreamReset();
      }

      bool bOK = true;

      // Check if we've got any packets in the buffer
      if (pData->PacketsLeft)
      {
         RWS_ASSERT (pData->BufferPos + PACKETHEADERSIZE < HIO_MAX_DATA_SIZE,
                     "Potential buffer overrun detected!");

         // Set up packet size
         pData->PacketSize = *(reinterpret_cast<RwUInt32 *>(
                                              g_RecvBuffer + pData->BufferPos));
         pData->BufferPos += PACKETHEADERSIZE;
      }
      else // attempt to read a new block of data
      {
         // If we haven't already sent a request for data, send one
         if (NetStreamData::StatusNormal == g_NetStreamData.nStatus)
         {
            // Ask if there's any data pending
            g_NetStreamData.nStatus = NetStreamData::StatusWaitingRead;
            if (!HIOWriteMailbox (rwsmIsDataPending))
            {
               bOK = false;
            }
         }

         // Check if there's some data ready for us yet
         if (NetStreamData::StatusOKToRead == g_NetStreamData.nStatus)
         {
            // Read the data
            if (!HIORead (HIO_HOST_TO_TARG_ADDR, g_RecvBuffer,
                          HIO_MAX_DATA_SIZE))
            {
               bOK = false;
            }
            else
            {
               DCInvalidateRange (g_RecvBuffer, HIO_MAX_DATA_SIZE);
               // Got a buffer block, so set up packet size
               pData->PacketsLeft = *reinterpret_cast<RwUInt32 *>(g_RecvBuffer);
               pData->PacketSize =
                           *(reinterpret_cast<RwUInt32 *>(g_RecvBuffer) + 1);

               pData->BufferPos = sizeof (RwUInt32) + PACKETHEADERSIZE;
            }
            g_NetStreamData.nStatus = NetStreamData::StatusNormal;
         }
         else
         {
            // No data pending - caller should retry opening stream
            RWS_RETURN (0);
         }
      }

      if (!bOK)
      {
         // Error, so restart
         RWS_TRACE ("Client connection lost!");
         RWS::NetStream::Shutdown ();
         NetStreamInit (&g_NetStreamData);
      }

      // One less packet header left in buffer
      pData->PacketsLeft--;

      RWS_RETURN (PACKETHEADERSIZE);
   }

   /* ----------------------------------------------------------------------- */
   /**
   *
   *  \ingroup NetStream
   *
   *  Called by RwStreamRead for network streams.
   *
   *  Loops until all required data has been read off the "network", or an error
   *  occurs.
   *
   *  Tries to read data off the network read-ahead cache, and when it runs out,
   *  requests another page of data from the host (via the HIO API).
   */
   /* ----------------------------------------------------------------------- */
   RwUInt32 ReadPacketData (NetStreamData *pData, void *pBuffer, RwUInt32 Len)
   {
      RWS_FUNCTION ("ReadPacketData");

      // Stop caller reading off end of RwStream (eg RwStreamReadChunkHeaderInfo)
      if (pData->PacketPos + Len > pData->PacketSize)
         Len = pData->PacketSize - pData->PacketPos;

      RwUInt8 *pOutBuffer = reinterpret_cast<RwUInt8 *>(pBuffer);
      
      RwUInt32 End = pData->PacketPos + Len;
      while (pData->PacketPos < End)
      {
         // Only read to end of current buffer
         RwInt32 nToCopy =
           (End - pData->PacketPos > HIO_MAX_DATA_SIZE - pData->BufferPos) ?
              (HIO_MAX_DATA_SIZE - pData->BufferPos) : (End - pData->PacketPos);

         // Copy amount remaining in packet buffer into output buffer
         memcpy (pOutBuffer, g_RecvBuffer + pData->BufferPos, nToCopy);

         // Update packet position, and buffer pointers
         pData->PacketPos += nToCopy;
         pData->BufferPos += nToCopy;
         pOutBuffer += nToCopy;

         // Read another block if we need
         if (pData->PacketPos < End)
         {
            // Reset buffer position
            pData->BufferPos = sizeof (RwUInt32);

            // Wait for a data block
            bool bOK = true;

            // Check the status
            if (NetStreamData::StatusNormal == pData->nStatus)
            {
               // Ask if there's any data pending
               pData->nStatus = NetStreamData::StatusWaitingRead;
               if (!HIOWriteMailbox (rwsmIsDataPending))
               {
                  bOK = false;
               }

               // Wait for the data
               for (;;)
               {
                  if (NetStreamData::StatusOKToRead == pData->nStatus)
                  {
                     if (!HIORead (HIO_HOST_TO_TARG_ADDR, g_RecvBuffer,
                                   HIO_MAX_DATA_SIZE))
                     {
                        bOK = false;
                     }
                     DCInvalidateRange (g_RecvBuffer, HIO_MAX_DATA_SIZE);
                     break;
                  }
               }
               pData->nStatus = NetStreamData::StatusNormal;
               pData->PacketsLeft = *reinterpret_cast<RwUInt32 *>(g_RecvBuffer);
            }

            // Check for network error
            if (!bOK)
            {
               RwUInt32 nAmountRead = End - pData->PacketPos;
               RWS_TRACE ("Client connection lost!");

               RWS::NetStream::Shutdown ();
               NetStreamInit (pData); // need to restart

               RWS_RETURN (nAmountRead);
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
   *  Writes a block of stream data to the host's shared memory area.
   *
   *  Waits for host to signal that it's read the block before returning.
   */
   /* ----------------------------------------------------------------------- */
   RwUInt32 WritePacketData (NetStreamData *pData, RwUInt32 Len)
   {
      RWS_FUNCTION ("WritePacketData");

      RWS_ASSERT (Len <= HIO_MAX_DATA_SIZE, "Packet to write is too long, len = "
            << Len << ", max = " << HIO_MAX_DATA_SIZE);

      if (!Len)
         return 0;

      // We must wait for any pending reads to clear first
      while (NetStreamData::StatusWaitingRead == pData->nStatus)
         ;

      // If pending read returned some data, then read it
      if (NetStreamData::StatusOKToRead == pData->nStatus)
         ReadPacketHeader (pData);

      // Send some data now we're safe to do so
      pData->nStatus = NetStreamData::StatusWaitingSend;

      // Flush memory before write
      DCFlushRange (g_SendBuffer, Len);

      if (!HIOWrite (HIO_TARG_TO_HOST_ADDR, g_SendBuffer, HIO_MAX_DATA_SIZE))
         RWS_RETURN (0);

      if (!HIOWriteMailbox (rwsmDataToSend))
         RWS_RETURN (0);

      // Wait for a data to be sent
      while (NetStreamData::StatusWaitingSend == pData->nStatus)
         ;

      RWS_RETURN (Len);
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
      RWS_ASSERT (pData->WCachePos < HIO_MAX_DATA_SIZE, "End of cache reached, max size = "
            << HIO_MAX_DATA_SIZE);

      const u8 *pSendBuffer = reinterpret_cast<const u8 *>(pBuffer);
      const u8 *pBufferEnd = pSendBuffer + Len;

      // Write the data
      while (pSendBuffer < pBufferEnd)
      {
         // Work out how much we can write
         RwInt32 nBytesToWrite = HIO_MAX_DATA_SIZE - pData->WCachePos;

         // Only write data remaining...
         if (nBytesToWrite > pBufferEnd - pSendBuffer)
            nBytesToWrite = pBufferEnd - pSendBuffer;

         // Copy data from buffer into cache
         memcpy (g_SendBuffer + pData->WCachePos, pSendBuffer, nBytesToWrite);

         // Update cache variables
         pData->WCachePos += nBytesToWrite;

         // If cache is full, write it to the network
         if (HIO_MAX_DATA_SIZE == pData->WCachePos)
         {
            pData->WCachePos = 0;
            if (WritePacketData (pData, HIO_MAX_DATA_SIZE) < HIO_MAX_DATA_SIZE)
               RWS_RETURN (FALSE);
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
      RWS_FUNCTION ("NetRStreamClose");

      NetStreamData *pRecvData = reinterpret_cast<NetStreamData *>(pData);

      // Skip to end of stream packet, ready to read next one
      if (pRecvData->PacketPos < pRecvData->PacketSize)
      {
         RwUInt32 nToSkip = (pRecvData->PacketSize - pRecvData->PacketPos);
         pRecvData->BufferPos += nToSkip;

         if (!NetStreamSkip (pData, nToSkip))
         {
            RWS::NetStream::Shutdown ();
            NetStreamInit (&g_NetStreamData); // need to restart
         }
      }

      // Reset globals
      pRecvData->PacketPos = pRecvData->PacketSize = 0;
      if (!pRecvData->PacketsLeft)
         pRecvData->BufferPos = 0;

      pRecvData->ReadStream = 0;

      RWS_RETURN (TRUE);
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
         if (WritePacketData (pSendData, pSendData->WCachePos)
                                                         < pSendData->WCachePos)
         {
            pSendData->WCachePos = pSendData->WPacketSize = 0;
            pSendData->WriteStream = 0;
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
   *  \returns Returns the number of bytes remaining in the stream.
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
   *  Custom RwStream callback for reading data from the network stream.
   */
   /* ----------------------------------------------------------------------- */
   RwUInt32 NetStreamRead (void *pData, void *pBuffer, RwUInt32 Length)
   {
      RWS_FUNCTION ("NetStreamRead");

      // Return how much data is left to read
      //
      if (pBuffer == 0 || Length == 0)
      {
         RWS_RETURN( GetUnReadData(pData) );
      }

      NetStreamData *pRecvData = reinterpret_cast<NetStreamData *>(pData);

      // Now read the data the client requested
      RWS_RETURN(ReadPacketData (pRecvData, pBuffer, Length));
   }

   /* ----------------------------------------------------------------------- */
   /**
   *
   *  \ingroup NetStream
   *
   *  Custom RwStream callback for writing data to the network stream.
   *  Waits for all data to be sent before returning.
   */
   /* ----------------------------------------------------------------------- */
   RwBool NetStreamWrite (void *pData, const void *pBuffer, RwUInt32 Length)
   {
      RWS_FUNCTION("NetStreamWrite");

      // Cache the data
      if (!WCacheData (reinterpret_cast<NetStreamData *>(pData),
                       pBuffer, Length))
      {
         RWS_RETURN (FALSE);
      }

      RWS_RETURN (TRUE);
   }

   /* ----------------------------------------------------------------------- */
   /**
   *
   *  \ingroup NetStream
   *
   *  Custom RwStream callback for skipping data in the network stream.
   *  Pulls the data off the network and sticks it in a temporary buffer.  This
   *  is done so the network remains synchronized.
   */
   /* ----------------------------------------------------------------------- */
   RwBool NetStreamSkip (void *pData, RwUInt32 Offset)
   {
      RWS_FUNCTION("NetStreamSkip");

      u8 Buffer[HIO_MAX_DATA_SIZE];

      RwInt32 nLeft = Offset;
      do
      {
         RwUInt32 nToRead = (nLeft < HIO_MAX_DATA_SIZE) ?
                                                 nLeft : HIO_MAX_DATA_SIZE;

         if (NetStreamRead (pData, Buffer, nToRead) < nToRead)
            RWS_RETURN(FALSE);

         nLeft -= nToRead;
      } while (nLeft > 0);

      RWS_RETURN(TRUE);
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
      RWS::NetStream::Shutdown();

      // The actual port number used is embedded in the GamecubeCommsServer,
      // it isn't used in this module - although it is on other platforms.
      // Just pass in 0 instead.
      //
      // Because the GCN does not use the timer within the network code the
      // 2nd parameter can be set to zero here, i.e. no timer function passed in.
      
      RWS::NetStream::Init(0, 0);   
   }

   /* ----------------------------------------------------------------------- */
} // namespace

/* -------------------------------------------------------------------------- */
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
*  \return TRUE if successful, FALSE otherwise.
*/
/* -------------------------------------------------------------------------- */
RwBool Init (RwUInt32, RwUInt32 (*)(void))
{
   RWS_FUNCTION ("Init");

   // GCN does not use time within the network code, so no need to store the
   // pointer to the function which provides this.

   RwBool bOK = FALSE;
   if (NETSTREAMOK == NetStreamInit (&g_NetStreamData))
      bOK = TRUE;

   if (!bOK)
      Shutdown ();

   RWS_RETURN (bOK);
}

/* -------------------------------------------------------------------------- */
/**
*
*  \ingroup NetStream
*
*  Shutdown of NetStream library.
*/
/* -------------------------------------------------------------------------- */
void Shutdown ()
{
   RWS_FUNCTION ("Shutdown");

   // Clear valid commands
   HIOWriteMailbox (rwsmLastCommand);

   // reset globals ready for another NetStreamInit
   g_NetStreamData.Clear ();
   memset (&g_CustomStreamData, 0, sizeof (RwStreamCustom));

   bClientConnected = false;

   RWS_RETURNVOID ();
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
   RWS_FUNCTION("OpenForRead");

   RWS_ASSERT (!g_NetStreamData.PacketSize, "Last read stream not closed");

   if (g_NetStreamData.ReadStream)
   {
      RWS_ASSERT (0, "Currently, you are only allowed one read network stream.");
      RWS_RETURN (0);
   }
   g_CustomStreamData.sfnclose = NetRStreamClose;
   g_CustomStreamData.sfnread  = NetStreamRead;
   g_CustomStreamData.sfnwrite = 0;
   g_CustomStreamData.sfnskip  = NetStreamSkip;
   g_CustomStreamData.data     = reinterpret_cast<void *>(&g_NetStreamData);

   if (!ReadPacketHeader (&g_NetStreamData))
      RWS_RETURN (0);


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

   RWS_RETURN (pNetStream);
}

/* -------------------------------------------------------------------------- */
/**
*
*  \ingroup NetStream
*
*  Used to open a stream used to write network RwStreams.
*
*  \param TotalDataSize The total number of bytes that will be written to the stream.
*         This value must be known in advance to allow network data to be cached
*         effectively.  Any cached data will be flushed upon a stream close.
*
*  \return A pointer to an RwStream if successful, or 0 otherwise.
*/
/* -------------------------------------------------------------------------- */
RwStream *OpenForWrite (RwUInt32 TotalDataSize)
{
   RWS_FUNCTION("OpenForWrite");

   // Only try to send any data if a client (e.g. RWS workspace) has connected.
   if (!bClientConnected)
   {
      RWS_RETURN (0);
   }

   RWS_ASSERT (!g_NetStreamData.WPacketSize, "Last write stream not closed");
   if (g_NetStreamData.WriteStream)
   {
      RWS_ASSERT (0, "Currently, you are only allowed one write network stream.");
      RWS_RETURN (0); // Only allowed one write stream open
   }
   g_CustomStreamData.sfnclose = NetWStreamClose;
   g_CustomStreamData.sfnread  = 0;
   g_CustomStreamData.sfnwrite = NetStreamWrite;
   g_CustomStreamData.sfnskip  = 0;
   g_CustomStreamData.data     = reinterpret_cast<void *>(&g_NetStreamData);

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
      RWS_RETURN (0);
   }

   RWS_RETURN (pNetStream);
}

/* -------------------------------------------------------------------------- */
/**
*
*  \ingroup NetStream
*
*  Used to obtain the IP address of the host machine once the NetStream library
*  has been initialized.
*
*  \param szAddress A pointer to a character buffer to receive the IP address.
*  \param AddressLength The length in bytes of the buffer pointed to by szAddress.
*
*  \return The number of bytes copied into the buffer (excluding the null
*          terminator).
*/
/* -------------------------------------------------------------------------- */
RwUInt32 GetAddress (RwChar * const szAddress, RwUInt32 AddressLength)
{
   RWS_FUNCTION ("GetAddress");
   RWS_ASSERT (szAddress, "Invalid IP address buffer supplied");

   if (!AddressLength)
      return 0;

   if (AddressLength < rwstrlen (g_NetStreamData.IPAddress))
   {
      memcpy (szAddress, g_NetStreamData.IPAddress, AddressLength-1);
      szAddress[AddressLength - 1] = 0;
      RWS_RETURN (AddressLength - 1);
   }

   rwstrcpy (szAddress, g_NetStreamData.IPAddress);

   RWS_RETURN (rwstrlen (g_NetStreamData.IPAddress));
}

/* -------------------------------------------------------------------------- */
}} // namespace RWS::NetStream

#endif // RWS_DESIGN
#endif // #ifndef RWS_BROADBAND
