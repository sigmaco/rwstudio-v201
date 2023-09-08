/****************************************************************************
 *
 * This file is a product of Criterion Software Ltd.
 *
 * This file is provided as is with no warranties of any kind and is
 * provided without any obligation on Criterion Software Ltd.
 * or Canon Inc. to assist in its use or modification.
 *
 * Criterion Software Ltd. and Canon Inc. will not, under any
 * circumstances, be liable for any lost revenue or other damages
 * arising from the use of this file.
 *
 * Copyright (c) 2002 Criterion Software Ltd.
 * All Rights Reserved.
 *
 */

/****************************************************************************
 *
 * maestro.c
 *
 ****************************************************************************/

#include "stdlib.h"
#include "string.h"

// Renderware Includes
#include "rwcore.h"
#include "rpworld.h"
#include "rt2d.h"
#include "rt2danim.h"

#include "maestro.h"
#include "font.h"
#include "texdict.h"

// ******************************************************************************
RwBool ExportFonts(Rt2dMaestro *pMaestro)
{
   RwBool ret = TRUE;
   RwInt32 numFontsFound = 0;

   numFontsFound = WriteOutMaestroFontFiles(Rt2dMaestroGetScene(pMaestro));

   if (numFontsFound)
   {
      if (numFontsFound == 1)
      {
         printf(RWSTRING("%d Font File saved: \n"), numFontsFound);
      }
      else if (numFontsFound > 1)
      {
         printf(RWSTRING("%d Font Files saved. \n"), numFontsFound);
      }
   }

   return ret;
}


// ******************************************************************************
RwBool ExportFontTextureDictionary(RwChar *pFileName)
{
   RwBool ret = TRUE;

   WriteOutFontTexDictFile(pFileName);

   return ret;
}


// ******************************************************************************
RwBool ExportMaestroTextureDictionary(Rt2dMaestro *pMaestro, RwChar *pFileName)
{
   RwBool ret = TRUE;

   WriteOutMaestroTexDictFile(Rt2dMaestroGetScene(pMaestro), pFileName);

   return ret;
}


// ******************************************************************************
RwBool ExportMaestroData(RwChar *pFileName)
{
   RwBool ret = TRUE;

   RwStream *pStream = 0;
   Rt2dMaestro *pMaestro = 0;
   RwUInt32 size, version;
   RwChar szFileName[_MAX_PATH];
   RwInt32 i;   

   pStream = RwStreamOpen(rwSTREAMFILENAME, rwSTREAMREAD, pFileName);

   if (!pStream)
   {
      return FALSE;
   }

   if (!RwStreamFindChunk(pStream, rwID_2DMAESTRO, &size, &version))
   {
      return FALSE;
   }

   RwImageSetPath(".\\textures\\");
   Rt2dFontSetPath(".\\font\\");
   
   pMaestro = Rt2dMaestroStreamRead(0, pStream);

   RwStreamClose(pStream, 0);

   if (!pMaestro)
   {
      return FALSE;
   }

   // Use the Maestro animation filename for outputting the .txd files
   strcpy(szFileName, pFileName);

   i = strlen(szFileName) - 1;

   while (i >= 0 && szFileName[i] != '.')
   {
      i--;
   }

   szFileName[i] = '\0';

   if (!ExportMaestroTextureDictionary(pMaestro, szFileName))
   {
      return FALSE;
   }

   if (!ExportFontTextureDictionary(szFileName))
   {
      return FALSE;
   }

   if (!ExportFonts(pMaestro))
   {
      return FALSE;
   }

   Rt2dMaestroDestroy(pMaestro);

   return ret;
}

