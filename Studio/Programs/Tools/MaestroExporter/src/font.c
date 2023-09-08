
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
 * Copyright (c) 2003 Criterion Software Ltd.
 * All Rights Reserved.
 *
 */

/****************************************************************************
 *
 * font.c
 *
 ****************************************************************************/

#include "stdlib.h"
// Renderware Includes
#include "rwcore.h"
#include "rt2d.h"

#include "font.h"
#include "fontutils.h"

#define MAX_FONTS 256

static RwInt32 g_iNumFontsFound = 0;
static RwChar g_sFontsFound[64][MAX_FONTS];
static Rt2dFont *g_pFont;
static RwChar *g_pFontName;


/**
*
*  WriteOutMaestroFontFile
*
*/
void WriteOutMaestroFontFile(void)
{
   RwChar szFileName[_MAX_PATH];
   RwStream *pStream;
   Rt2dFont * pRet;

   if (g_pFontName)
   {
      // We have font data, use the font's name to create the file
      sprintf(szFileName, "%s.fnt", g_pFontName);

      // Write Data to file
      if (g_pFont)
      {
         if (pStream = RwStreamOpen(rwSTREAMFILENAME, rwSTREAMWRITE, szFileName))
         {
            pRet = Rt2dFontStreamWrite(g_pFont, pStream);
            RwStreamClose(pStream, 0);

            if (pRet)
            {
               printf(RWSTRING("Font File saved: \n"));
               printf(RWSTRING("  %s \n"), szFileName);
            }
         }
      }
   }
}


/**
*
*  WriteOutMaestroFontFiles
*
* \param Rt2dObject *pObject
* \param RwChar *fontFile
*
*/
RwInt32 WriteOutMaestroFontFiles(Rt2dObject *pObject)
{
   g_iNumFontsFound = 0;

   if (pObject)
   {
      // Find Font Data
      TraverseSceneAndGetFont(pObject);
   }
   
   return g_iNumFontsFound;
}


/**
*
*  FontFoundAlready
*
*/
RwBool FontFoundAlready(RwChar *pFontName)
{
   RwInt32 i;

   for (i=0; i<g_iNumFontsFound; i++)
   {
      if (rwstrcmp(pFontName, g_sFontsFound[i]) == 0)
      {
         // We have found the font so we shouldn't output it again
         return TRUE;
      }
   }

   return FALSE;
}


/**
*
*  TraverseSceneAndGetFont
*
*  Traverses the entire Maestro scene looking for any string objects and extracts the font
*
* \param Rt2dObject *pObject
*
*/
void TraverseSceneAndGetFont(Rt2dObject *pObject)
{
   Rt2dObject *pChildObject=0;
   RwInt32 i, iChildCount;

   iChildCount = Rt2dSceneGetChildCount(pObject);

   for (i=0; i<iChildCount; i++)
   {
      pChildObject = Rt2dSceneGetChildByIndex(pObject, i);

      if (Rt2dObjectIsScene(pChildObject))
      {
         // Recursively traverse the scene objects
         TraverseSceneAndGetFont(pChildObject);
      }
      else if (Rt2dObjectIsObjectString(pChildObject))
      {
         g_pFontName = Rt2dObjectStringGetFont(pChildObject);

         // We've found a font so use it if we haven't already
         if (g_iNumFontsFound==0 || !FontFoundAlready(g_pFontName))
         {
            g_pFont = GetFontFromObject(pChildObject, g_pFontName);

            rwsprintf(g_sFontsFound[g_iNumFontsFound], g_pFontName);

            g_iNumFontsFound++;

            WriteOutMaestroFontFile();
         }
      }
   }
}

