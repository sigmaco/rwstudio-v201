/*****************************************************************************
*
* File :     netstrm.h
*
* Abstract : Network RwStream Interface
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

#ifndef RWSNETSTREAM_H
#define RWSNETSTREAM_H

namespace RWS
{ 
   namespace NetStream
   {
      /**
      *
      *  \addtogroup NetStream
      *
      *  All data sent between the RenderWare Studio Framework and Workspace
      *  is in the form of a custom RenderWare stream (RwStream). This module
      *  implements that stream, encapsulating the necessary network functions
      *  within the standard RwStream API.
      *
      *  \note Data is buffered for reading and writing, to reduce the number
      *        of small packets sent over the network.
      *
      */

      RwBool Init (RwUInt32 Port, RwUInt32 (*pTimeFunc)(void));
      void Shutdown (void);
      RwStream *OpenForRead (void);
      RwStream *OpenForWrite (RwUInt32 TotalDataSize);
      RwUInt32 GetAddress (RwChar * const szAddress, RwUInt32 BufferSize);
   }
}     // namespace RWS::NetStream

#endif // RWSNETSTREAM_H
