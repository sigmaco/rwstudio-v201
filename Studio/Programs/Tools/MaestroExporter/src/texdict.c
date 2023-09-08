
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
 * texdict.c
 *
 ****************************************************************************/

// Renderware Includes
#include "stdlib.h"
#include "rwcore.h"
#include "rt2d.h"
#include "rtpitexd.h"

#include "texdict.h"


RwBool WriteOutMaestroTexDictFile(Rt2dObject *pScene, RwChar *pFileName)
{
   RwTexDictionary *pTexDict;
   RwStream *pStream;
   RwChar szFileName[_MAX_PATH];

   // Get texture dictionary to put textures into
   pTexDict = RwTexDictionaryGetCurrent();

   if (!pTexDict)
   {
      return FALSE;
   }

   sprintf(szFileName, "%s.txd", pFileName);

   if (pStream = RwStreamOpen (rwSTREAMFILENAME, rwSTREAMWRITE, szFileName))
   {
#if defined (RWSPLATFORM_OPENGL) || \
    defined (RWSPLATFORM_DIRECTX)
      // write out platform independant texture dictionarys under windows
      const RwTexDictionary * pRet = RtPITexDictionaryStreamWrite (pTexDict, pStream);
#else     
      const RwTexDictionary * pRet = RwTexDictionaryStreamWrite (pTexDict, pStream);
#endif

      if (pRet)
      {
         printf(RWSTRING("txd File saved: \n"));
         printf(RWSTRING("  %s \n"), szFileName);
      }

      RwStreamClose (pStream, 0);
   }
   else
   {
      return FALSE;
   }
   
   return TRUE;
}


RwBool WriteOutFontTexDictFile(RwChar *pFileName)
{
   RwTexDictionary *pTexDict;
   RwStream *pStream;
   RwChar szFileName[_MAX_PATH];

   // Get texture dictionary for fonts
   pTexDict = Rt2dFontTexDictionaryGet();

   if (!pTexDict)
   {
      return FALSE;
   }

   sprintf(szFileName, "%s_fonts.txd", pFileName);

   if (pStream = RwStreamOpen (rwSTREAMFILENAME, rwSTREAMWRITE, szFileName))
   {
#if defined (RWSPLATFORM_OPENGL) || \
    defined (RWSPLATFORM_DIRECTX)
      // write out platform independant texture dictionarys under windows
      const RwTexDictionary * pRet = RtPITexDictionaryStreamWrite (pTexDict, pStream);
#else     
      const RwTexDictionary * pRet = RwTexDictionaryStreamWrite (pTexDict, pStream);
#endif

      if (pRet)
      {
         printf(RWSTRING("txd File saved: \n"));
         printf(RWSTRING("  %s \n"), szFileName);
      }

      RwStreamClose (pStream, 0);
   }
   else
   {
      return FALSE;
   }
   
   return TRUE;
}