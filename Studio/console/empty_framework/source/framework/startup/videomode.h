/*****************************************************************************
 *
 * File :     videomode.h
 *
 * Abstract : Utility function used to select a best fit/suitable video mode.
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

namespace RWS
{
   namespace StartUp
   {
      RwBool SelectVideoMode( RwInt32& screenWidth, RwInt32& screenHeight, RwInt32 bitDepth, 
                              RwUInt32 flags, RwUInt32 zDepth);
   }
}
