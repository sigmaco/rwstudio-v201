/*****************************************************************************
 *
 * File :     Win.h
 *
 * Abstract : Win32 startup, game loop and shutdown
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

#ifndef _WIN_H_
#define _WIN_H_

#include <windows.h>
#include "framework/core/eventhandler/ceventhandler.h"

namespace RWS
{
   namespace Win
   {
      /**
      *  \ingroup SystemEvents
      *  \page StartupEvents System Events
      *  \li iMsgResizeCameraRasters
      *  Sent if main raster is resized, where the main raster represents the visible display area.
      */
      
      RWS_DEFINE_EVENT(iMsgResizeCameraRasters, 0, "If the main Camera is resized, any sub-rasters need to update");

      HWND GetMainWindow();
   }

   RwBool psInstallFileSystem(void);
}

#endif // _WIN_H_