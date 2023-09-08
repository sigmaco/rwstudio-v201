
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
#include "rtpitexd.h"

#include "font.h"

/**
*
*  WriteOutFontFile
*
*  \param pFontFile Pointer to the font file
*  \param pFont Pointer to the font data to write to file.
*
*  \return TRUE on success, FALSE otherwise
*/
RwBool WriteOutFontFile(RwChar *pFontFile, Rt2dFont *pFont)
{
   RwStream *pStream;
   Rt2dFont *pRet;
   RwBool ret = FALSE;

   if (pFontFile)
   {
      // Write Data to file
      if (pFont)
      {
         // We have font data

         if (pStream = RwStreamOpen(rwSTREAMFILENAME, rwSTREAMWRITE, pFontFile))
         {
            pRet = Rt2dFontStreamWrite(pFont, pStream);            

            if (pRet)
            {
               ret = TRUE;
            }

            RwStreamClose(pStream, 0);
         }
      }
   }

   return ret;
}

/**
*
*  WriteOutFontTexDictFile
*
*  \param pFontTexDictFile Pointer to the font texture dictionary file
*
*  \return TRUE on success, FALSE otherwise
*/
RwBool WriteOutFontTexDictFile(RwChar *pFontTexDictFile)
{
   RwStream *pStream;
   RwBool ret = FALSE;

   if (pFontTexDictFile)
   {
      RwTexDictionary *pTexDict = Rt2dFontTexDictionaryGet();

      if (pTexDict)
      {
         // We have font txd data

         if (pStream = RwStreamOpen(rwSTREAMFILENAME, rwSTREAMWRITE, pFontTexDictFile))
         {
#if defined (RWSPLATFORM_OPENGL) || \
    defined (RWSPLATFORM_DIRECTX)
            // Write out platform independant texture dictionaries under windows
            const RwTexDictionary * pRet = RtPITexDictionaryStreamWrite (pTexDict, pStream);
#else     
            const RwTexDictionary * pRet = RwTexDictionaryStreamWrite (pTexDict, pStream);
#endif
            if (pRet)
            {
               ret = TRUE;
            }

            RwStreamClose(pStream, 0);
         }
      }
   }

   return ret;
}