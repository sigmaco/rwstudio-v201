/*****************************************************************************
 *
 * File :     loadgamedatabase.cpp
 *
 * Abstract : Utility function used to load a game database from a file.
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

//////////////////////////////////////////////////////////////////
// 
// RenderWare Studio Includes
//
#include "framework/core/macros/debugmacros.h"
#include "framework/core/streamhandler/streamhandler.h"

namespace RWS
{
   namespace StartUp
   {
      /**
      *
      *  \ingroup StartUp
      *
      *  Loads a RenderWare Studio Stream from a file, RenderWare studio streams
      *  contain the description of a complete level.
      *
      *  \param pFileName String containing the name of the game database file to load.
      *
      */
      bool LoadGameDatabaseFile(RwChar *pFileName)
      {
         RWS_FUNCTION("Startup::LoadGameDatabaseFile");

         RwStream* pStream = RwStreamOpen(rwSTREAMFILENAME, rwSTREAMREAD, pFileName);

         if (pStream)
         {
            CStreamHandler::ProcessStream(pStream);

            RwStreamClose(pStream, pFileName);

            RWS_RETURN(true);
         }

         RWS_RETURN(false);
      }
   }
}