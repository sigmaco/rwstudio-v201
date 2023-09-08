/*****************************************************************************
 *
 * File :     debugmsghandler.cpp
 *
 * Abstract : Utility function used to handle RenderWare Gfx internal messages
 * such as asserts, warning etc
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
#include "framework/core/macros/debugmacros.h"

namespace RWS
{
   namespace StartUp
   {
     /*
      *  \ingroup StartUp
      *  
      *  Debug handler for RenderWare internal error/warning messages
      *
      *  \param type RwDebugType object, used to specify type of message.
      *  \param pMessage Pointer to string containing the message. 
      */
      void DebugMessageHandler(RwDebugType type, const RwChar *pMessage)
      {
         RWS_FUNCTION("DebugMessageHandler");

         switch (type)
         {
            case rwDEBUGASSERT:              /* Print an assert message */
               RWS_DEBUGSTREAM(pMessage<< std::endl);
               RWS_DEBUGBREAK;
#ifdef RWS_DESIGN
               throw(0);                     /* In design mode we include some exception handling
                                                which allows errors to be reported in more detail
                                                to the workspace. */
#endif
               break;
            case rwDEBUGERROR:               /* Print an error message */
            case rwDEBUGMESSAGE:             /* Print an informational message */
            case rwDEBUGTRACE:               /* Print a trace message */
               RWS_DEBUGSTREAM(pMessage<< std::endl);
               break;
            default:
               RWS_ASSERTFAIL("Unknown error message, this should never happen.");
               break;
         }

         RWS_RETURNVOID();
      }
   }
}