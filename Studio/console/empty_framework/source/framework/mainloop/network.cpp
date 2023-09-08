/*****************************************************************************
*
* File :     network.cpp
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
* Copyright (c) 2001 Criterion Software Ltd.
* All Rights Reserved.
*
* RenderWare is a trademark of Canon Inc.
*
*****************************************************************************/
#include "precomp.h"

#ifdef RWS_DESIGN

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "network.h"
#include "framework/toolkits/time/gftime.h"
#include "framework/core/macros/debugmacros.h"
#include "framework/core/netstream/netstrm.h"
#include "framework/core/netstream/nettools.h"
#include "framework/core/streamhandler/streamhandler.h"
#include "framework/core/streamhandler/strfunc.h"
#include "framework/core/memoryhandler/memoryprofile.h"

namespace
{
#ifndef NDEBUG
   bool s_bNetworkIsOpen = false;
#endif
   static RwChar ipAddress[128] = "NULL";
}

namespace RWS
{
   namespace MainLoop
   {
      RwChar g_szWaitingMessage[128] = "Searching For Connection";

      namespace Network
      {
         namespace
         {
            /**
            *
            *  Close network connection, note creates a new network connection.
            *
            */
            void CloseConnection (RwChunkHeaderInfo &rChunkHeader, RwStream *pStream)
            {
               RWS_FUNCTION("RWS::CDefaultStreamHandler::CloseConnection");
      
               // Profile data is only valid if all data is received, if the workspace
               // reconnects only the profile data collected since the disconnect is
               // available. The workspace cannot reconstruct the memory layout on the console
               // without all the data therefore we disable profiling.
               //
               RWS::MemoryProfile::ConnectionLost();
      
         #ifdef RWS_DESIGN
               RWS::MainLoop::Network::Close ();
               RWS::MainLoop::Network::Open ();
         #else
               RWS_TRACE("IGNORED");
         #endif
      
               RWS_RETURNVOID();
            }
         }


        /**
         *
         *  Register the chunk handlers implemented by MainLoop::Render, \see strfunc_func \see CStreamHandler.
         *
         *  MainLoop::Render implements the following chunk handlers, \ref strfunc_SetDirectorsCameraMatrix,
         *  \ref strfunc_EnableDirectorsCamera, strfunc_DisableDirectorsCamera, strfunc_GetEntityMatrix.
         *
         */
         void RegisterStreamChunkHandlers(void)
         {
            CStreamHandler::RegisterChunkHandler(MAKECHUNKID (rwVENDORID_CRITERIONRM,
               strfunc_CloseConnection), CloseConnection);
         }


         /**
         *
         *  Unregister the chunk handlers implemented by MainLoop::Render, \see strfunc_func \see CStreamHandler.
         */
         void UnRegisterStreamChunkHandlers(void)
         {
            CStreamHandler::UnRegisterChunkHandler(MAKECHUNKID (rwVENDORID_CRITERIONRM, strfunc_CloseConnection));
         }

         /**
         *
         *  \ingroup Network
         *
         *  Check for network packets.
         *
         */
         void Poll()
         {
            RWS_FUNCTION("DoNetWorkMessage");
            
            // Ensure Open has been called before calling Poll
            //
            RWS_PRE(s_bNetworkIsOpen);
            
            // Receive command packets from RenderWare Studio Workspace
            //
            while (RwStream *pNetStream = NetStream::OpenForRead ())
            {
               CStreamHandler::ProcessStream (pNetStream);
               RwStreamClose (pNetStream, 0);
            }
            
            RWS_RETURNVOID();
         }
         
         /**
         *
         *  \ingroup Network
         *
         *  Opens the network module.
         *
         */

         void Open()
         {
            RWS_FUNCTION("Open");

            RWS_PRE(!s_bNetworkIsOpen);

            g_szWaitingMessage[0] = 0;

            // Initialize network streams
            //
#if defined (_XBOX)
            const RwUInt32 port_number = 5610;
#elif defined (SKY)
            const RwUInt32 port_number = 5609;
#elif defined (DOLPHIN)
            const RwUInt32 port_number = 5611;
#elif defined (D3D8_DRVMODEL_H) || defined (D3D9_DRVMODEL_H)
            const RwUInt32 port_number = 5607;
#else
            const RwUInt32 port_number = 5608;
#endif
            RwBool bOK = NetStream::Init (port_number, RWS::Time::GetTime);

            RWS_ASSERT (bOK, "MainLoop::NetWork::Open () Failed");

            // Get the IP address we're listening on
            if (RwUInt32 nAddrLen = NetStream::GetAddress (ipAddress, 128))
            {
               sprintf(g_szWaitingMessage, "Listening On %s : %d ", ipAddress, port_number);
            }
            else
            {
               rwstrcpy (g_szWaitingMessage,
                         RWSTRING ("Error unable to establish IP address."));
            }

#ifndef NDEBUG
            s_bNetworkIsOpen = bOK ? true : false;
#endif
            
#if !defined(NDEBUG)
            // Register the debug message callback to echo debug messages to the workspace
            RWS::cdbg().rdbuf()->AddOutputCallback(RWS::NetTools::SendDebugMessage);
#endif

            RWS_RETURNVOID();
         }
         
         /**
         *
         *  \ingroup Network
         *
         *  Closes the network module.
         *
         */
         void Close()
         {
            RWS_FUNCTION("Close");

            RWS_PRE(s_bNetworkIsOpen);

#if !defined(NDEBUG)
            // Register the debug message callback to echo debug messages to the workspace
            RWS::cdbg().rdbuf()->RemoveOutputCallback(RWS::NetTools::SendDebugMessage);
#endif

            NetStream::Shutdown ();

#ifndef NDEBUG
            s_bNetworkIsOpen = false;
#endif
            RWS_RETURNVOID();
         }
         /**
         *
         *  \ingroup Network
         *
         *  Returns the ip address of the comms server
         *
         */
         char *GetIPAddress()
         {
             RWS_FUNCTION("GetIPAddress");
             RWS_RETURN(ipAddress);
         }
      }
   }
}
#endif
