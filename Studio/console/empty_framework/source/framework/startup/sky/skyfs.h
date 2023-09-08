/*****************************************************************************
 *
 * File :     SkyFS.h
 *
 * Abstract : PS2 custom file system
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

#ifndef _SKYFS_H_
#define _SKYFS_H_

#include <rwcore.h>

namespace RWS
{
   /**
   *
   */
   namespace Sky { bool InstallFileSystem(RwChar* pSubdir); }
}

#endif
