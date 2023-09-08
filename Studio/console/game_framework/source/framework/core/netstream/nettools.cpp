/*****************************************************************************
*
* File :     NetTools.cpp
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

#ifdef RWS_DESIGN

//////////////////////////////////////////////////////////////////
//
// Standard Includes
//
#include <string.h>

//////////////////////////////////////////////////////////////////
//
// RenderWare  Includes
//
#include "../netstream/netstrm.h"
#include "../netstream/nettools.h"
#include "../macros/debugmacros.h"
#include "../streamhandler/strfunc.h"
#include "../memoryhandler/memoryprofile.h"

namespace RWS
{
   namespace NetTools
   {
      /**
      *
      *  \ingroup NetTools
      *
      *  Send an RwFrame to the host PC via the RenderWare Studio network connection.
      *  If pEntityID is 0 the frame is treated as the directors camera transform
      *  by the workspace.
      *
      *  \param frame A pointer to a RwFrame object.
      *
      *  \param pEntityID An optional pointer to an Entity's \ref RWSGUID,
      *                   used to identify the entity in the workspace.
      *
      *  \note Only available in design builds of the framework.
      */
      void SendTransform(RwFrame *frame, RWSGUID *pEntityID /* = 0 */)
      {
         RWS_FUNCTION("RWS::NetTools::SendTransform");

         if (frame)
         {
            RwMatrix *pMatrix = RwFrameGetLTM(frame);

            if (pMatrix)
            {
               const RwChar szSendTransform[] = RWSTRING ("RWS::SendTransform");
               RwUInt32 nMessageSize = sizeof (RwChar) * (rwstrlen (szSendTransform) + 1);
            
               // Data is uint for object ID, followed by matrix data (not padded)
               const RwUInt32 nMatrixLength = 12 * sizeof (RwReal);
               RwInt8 buffer[nMatrixLength];

               // Set matrix data
               RwInt8 *pBuf = buffer;
               memcpy (pBuf, &pMatrix->right, sizeof (RwReal) * 3);
               pBuf += sizeof (RwReal) * 3;
               memcpy (pBuf, &pMatrix->up, sizeof (RwReal) * 3);
               pBuf += sizeof (RwReal) * 3;
               memcpy (pBuf, &pMatrix->at, sizeof (RwReal) * 3);
               pBuf += sizeof (RwReal) * 3;
               memcpy (pBuf, &pMatrix->pos, sizeof (RwReal) * 3);

               // Create a network stream for writing, write data on it and close
               RwUInt32 nDataSize = nMessageSize;
               nDataSize += sizeof (RWSGUID);
               nDataSize += nMatrixLength;

#ifdef RWS_MEMORY_PROFILING
               // Open for write can cause a memory allocation, if it does it is released when 
               // RwStreamClose is called, so it is ok to leave this out of the memory profile.
               //
               RwBool flag = MemoryProfile::Enable(FALSE);
#endif
               if (RwStream *pStream = NetStream::OpenForWrite (nDataSize + ChunkHeaderSize))
               {
                  // Write the header and message ID
                  RwStreamWriteChunkHeader (pStream, strfuncsend_NamedData, nDataSize);
                  RwStreamWrite (pStream, szSendTransform, nMessageSize);

                  // Write the entity id (or null guid for camera)
                  if (pEntityID)
                  {
                     RwStreamWrite (pStream, pEntityID->m_Data, sizeof (RWSGUID));
                  }
                  else
                  {
                     RWSGUID NullGuid;
                     NullGuid.Clear ();
                     RwStreamWrite (pStream, NullGuid.m_Data, sizeof (RWSGUID));
                  }

                  // Write the matrix and close
                  RwStreamWrite (pStream, buffer, nMatrixLength);
                  RwStreamClose (pStream, 0);
               }

#ifdef RWS_MEMORY_PROFILING
               MemoryProfile::Enable(flag);
#endif
            }
         }
         RWS_RETURNVOID();
      }

      /**
      *
      *  \ingroup NetTools
      *
      *  A function to send a null-terminated string back to the host PC via 
      *  the RenderWare Studio network connection. By default all strings sent 
      *  back using this function will appear in the Output window of the 
      *  RenderWare Studio Workspace.
      *
      *  \note The szMessageName parameter of RWS::NetStream::OpenForWrite is 
      *        set to "RWS::DebugMessage". The RenderWare Studio Workspace uses
      *        this to identify the string as a debug message.
      *
      *  \param message Pointer to the string to send. The string must be 
      *                 null-terminated.
      *
      *  \note Only available in design builds of the framework.
      */
      void SendDebugMessage( const char * message )
      {
         if (message)
         {
#ifdef RWDEBUG
            // Dips under the hood of RenderWare and disable function tracing, without this
            // recursion can occur.
            //
            RwBool state = RWSRCGLOBAL(debugTrace);
            RWSRCGLOBAL(debugTrace) = FALSE;
#endif

            static const RwChar DebugMessage[] = RWSTRING("RWS::DebugMessage");            
            RwUInt32 nMessageSize = (rwstrlen (DebugMessage) + 1) * sizeof (RwChar);

            // Data is a null-terminated character string - must transmit the null terminator as well
            RwUInt32 nDataLength = ( (rwstrlen(message) + 1) * sizeof (RwChar) );

#ifdef RWS_MEMORY_PROFILING
            // Open for write can cause a memory allocation, if it does it is released when 
            // RwStreamClose is called, so it is ok to leave this out of the memory profile.
            //
            RwBool flag = MemoryProfile::Enable(FALSE);
#endif
            // Create a network stream for writing, write data on it and close
            if (RwStream *pStream = NetStream::OpenForWrite (
                                       nMessageSize + nDataLength + ChunkHeaderSize))
            {
               RwStreamWriteChunkHeader (pStream, strfuncsend_NamedData,
                                         nMessageSize + nDataLength);
               RwStreamWrite (pStream, DebugMessage, nMessageSize);
               RwStreamWrite (pStream, message, nDataLength);
               RwStreamClose (pStream, 0);
            }

#ifdef RWS_MEMORY_PROFILING
            MemoryProfile::Enable(flag);
#endif

            // Note this means we are sending the data back twice, but the memory profiler needs this
            // in order to maintain the order of memory events.
            //
            RWS::MemoryProfile::Comment(message);

#ifdef RWDEBUG
            // Reset the state
            //
            RWSRCGLOBAL(debugTrace) = state;
#endif
         }
      }

      /**
      *
      *  \ingroup NetTools
      *
      *  This function allows the framework to send a block of data to the host 
      *  PC via the RenderWare Studio network connection.
      *
      *  \note The szMessageName parameter of RWS::NetStream::OpenForWrite is 
      *        set to "RWS::FileSave". The RenderWare Studio Workspace uses
      *        this to identify the data as a file to save.
      *
      *  \param szFilename Name of the file to save the data block as. 
      *                    (The current ActiveX control in RenderWare Studio 
      *                    uses this as the default name to save to, but the 
      *                    file has to be explicitly saved by a user once it 
      *                    has been received over the network.)
      *  \param szCategory An optional category string, can be used to help 
      *                    the host sort the files it is sent. Set to NULL if 
      *                    not needed.
      *  \param pData A pointer to the block of data to send.
      *  \param nDataSize Size of the data block (in bytes).
      *
      *  \note Only available in design builds of the framework.
      */
      void SendFile( const char * szFilename, const char * szCategory, const void * pData, RwUInt32 nDataSize )
      {
         RWS_FUNCTION("RWS::NetTools::SendFile");

         // This needs to match the struct defined in the RWSFileSaver control (one of the RWS modules).
         struct FileHeader
         {
            char szFilename[32];
            char szCategory[64];
            unsigned int nDataSize;
         };

         if (pData && szFilename)
         {
            const RwChar IDString[] = RWSTRING("RWS::FileSave");
            RwUInt32 nMessageSize = rwstrlen (IDString) + sizeof (RwChar);

            FileHeader Header;
            memset( &Header, 0, sizeof(FileHeader) );

            strncpy( Header.szFilename, szFilename, sizeof(Header.szFilename) );
            if ( szCategory )
            {
               strncpy( Header.szCategory, szCategory, sizeof(Header.szCategory) );
            }
            Header.nDataSize = nDataSize;
            
#if defined(DOLPHIN)
            // Because the Gamecube is big-endian, need to change 
            //  the nDataSize field to little endian so it can be read
            //  on a PC.
            RwMemLittleEndian32( &Header.nDataSize, sizeof(unsigned int) );
#endif
            RwUInt32 nPacketSize = sizeof(FileHeader) + nDataSize;

#ifdef RWS_MEMORY_PROFILING
            // Open for write can cause a memory allocation, if it does it is released when 
            // RwStreamClose is called, so it is ok to leave this out of the memory profile.
            //
            RwBool flag = MemoryProfile::Enable(FALSE);
#endif
            // Create a network stream for writing, write data on it and close
            if ( RwStream *pStream = NetStream::OpenForWrite (
                                        nMessageSize + nPacketSize + ChunkHeaderSize) )
            {
               RwStreamWriteChunkHeader (pStream, strfuncsend_NamedData,
                                         nMessageSize + nPacketSize);
               RwStreamWrite ( pStream, IDString, nMessageSize );
               RwStreamWrite ( pStream, &Header, sizeof(FileHeader) );
               RwStreamWrite ( pStream, pData, nDataSize );
               RwStreamClose ( pStream, 0 );
            }

#ifdef RWS_MEMORY_PROFILING
            MemoryProfile::Enable(flag);
#endif
         }
         RWS_RETURNVOID();
      }
   }// namespace NetTools
}// namespace RWS
#endif
