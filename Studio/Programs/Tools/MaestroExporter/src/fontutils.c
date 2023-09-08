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
 * fontutils.c
 *
 ****************************************************************************/

// Renderware Includes
#include "rwcore.h"
#include "rt2d.h"


struct _rt2dFontDictionaryNode
{
    RwChar *name;
    const Rt2dFont *font;
    _rt2dFontDictionaryNode *next;
};


/**
*
*  _rt2dSetFontDictionary
*
* \param const Rt2dObject *pObject
* \param const RwChar* fontName
*
*/
Rt2dFont *GetFontFromObject(Rt2dObject *pObject, RwChar* fontName)
{ 
   Rt2dFont *font=0;
   _rt2dFontDictionaryNode *node;

   if (fontName)
   {
      node = pObject->data.objectString.font;

      // look for the font
      while (node) 
      { 
        if (rwstrcmp(fontName, node->name)==0) 
        { 
            font = (Rt2dFont *)(node->font); 
            break; 
        } 
        node = node->next; 
      }
   }

   return font;
} 
