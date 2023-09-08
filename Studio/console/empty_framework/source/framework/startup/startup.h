/*****************************************************************************
 *
 * File :     StartUp.h
 *
 * Abstract : Generic startup, game loop and shutdown services
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

#ifndef _STARTUP_H_
#define _STARTUP_H_

/**
*
*  \ingroup StartUp
*
*  \page StartUpOverview Overview
*
*  Platform independent startup and shutdown of RenderWare Graphics and Audio Game Framework.
*
*/

namespace RWS
{
   /**
   *
   *  \ingroup StartUp
   *
   *  Functionality for providing platform independent startup and shutdown of
   *  RenderWare Graphics, Audio and the Game Framework
   *
   */
   namespace StartUp
   {
      typedef bool (*FileSystemInstaller)(void);

      RwBool Open(
         const FileSystemInstaller pFSInstaller,
         const RwMemoryFunctions *memfuncs,
         RwUInt32 ArenaSize,
         RwVideoMode &rVideoMode,
         const RwUInt32 ZBufferDepth,
         void* pDisplayID
         );

      void Close();
   }
}

#endif // _STARTUP_H_
