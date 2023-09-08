/*****************************************************************************
 *
 * File :     loadgamedatabase.h
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
      bool LoadGameDatabaseFile(RwChar *pFileName);
   }
}