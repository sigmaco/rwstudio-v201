/*****************************************************************************
*
* File :     NetTools.h
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

#ifndef RWSNETTOOLS_H
#define RWSNETTOOLS_H

#include "../misc/rwsguid.h"

/**
*
*  \addtogroup NetTools
*
*  Utility functions for sending data back to the RenderWare Studio Workspace
*  using the Network Stream module.
*
*/

namespace RWS
{
   namespace NetTools
   {
#if defined (RWS_DESIGN)
         void SendTransform(RwFrame *frame, RWSGUID *pEntityID = 0);
         void SendDebugMessage( const char * message );
         void SendFile( const char * szFilename, const char * szCategory, const void * pData, RwUInt32 nDataSize );
#else
#define SendTransform(InstanceID, frame)
#define SendDebugMessage(message)
#define SendFile(szFilename, szCategory, pData, nDataSize)
#endif
   }
}

#endif