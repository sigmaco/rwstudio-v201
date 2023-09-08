/*****************************************************************************
*
* File :     network.h
*
* Abstract : Network connection sub-system
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

#ifndef __GAMELOOP_NETWORK_H__
#define __GAMELOOP_NETWORK_H__

#ifdef RWS_DESIGN

/**
*
*  \ingroup Network
*
*  \page NetworkOverview Overview
*
*  This module is responsible for polling the network for remote function calls 
*  from the RenderWare Studio Workspace.
*
*/

namespace RWS
{
   namespace MainLoop
   {
      /**
      *
      *  \ingroup Network
      *
      *  Namespace used to encapsulate functionality for network usage.
      *
      */
      namespace Network
      {
         void Open();   // Open Network Device

         void Poll();   // Poll Network Device

         void Close();  // Close Network Device

         char *GetIPAddress();   // Return IP Address

         void RegisterStreamChunkHandlers(void);

         void UnRegisterStreamChunkHandlers(void);
      }
   }
}

#endif

#endif
